#include "stdafx.h"
#include "MMFileListDlg.h"
#include "WceUiListBox.h"
#include "tzUtil.h"
#include "msdkrender.h"
#include "sysUtil.h"
#include "interface.h"
#include "uiddraw.h"
#include "MusicPlayerDlg.h"
#include "VideoPlayerDlg.h"
#include "interface.h"
#include "sysUtil.h"
#include "config.h"
#include "protocol.h"
#include "OverlayDlg.h"
#include "RpcMCU.h"

void CMMFileListDlg::MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data)
{
	CMMFileListDlg* pthis = (CMMFileListDlg*)user_data;
	if (pthis)
	{
		pthis->OnMediaEvent(media_event, param1, param2);
	}
}


CMMFileListDlg::CMMFileListDlg(void)
{
	m_curType = MSDK_FILE_MUSIC;
	m_curPlayingType = MSDK_FILE_INVALID;
	m_pListFolder = NULL;
	m_pListFile   = NULL;
	m_nErrorCode = VIDEO_ERROR_NONE;

	if (WceUiGetBitsPixel() == 16)
	{
		m_texMusic.SetBitmapFromRes(L"media\\filelist\\music.565a.conf");
		m_texVideo.SetBitmapFromRes(L"media\\filelist\\video.565a.conf");
		m_texPhoto.SetBitmapFromRes(L"media\\filelist\\photo.565a.conf");
		m_texFolder.SetBitmapFromRes(L"media\\filelist\\folder.565a.conf");
		m_texPlaying.SetBitmapFromRes(L"media\\filelist\\play.565a.conf");
	}
	else
	{
		m_texMusic.SetBitmapFromRes(L"media\\filelist\\music.32a.conf");
		m_texVideo.SetBitmapFromRes(L"media\\filelist\\video.32a.conf");
		m_texPhoto.SetBitmapFromRes(L"media\\filelist\\photo.32a.conf");
		m_texFolder.SetBitmapFromRes(L"media\\filelist\\folder.32a.conf");
		m_texPlaying.SetBitmapFromRes(L"media\\filelist\\play.32a.conf");
	}
	CMsdkRender::GetInstance()->RegisterEventHandlerF(MediaEventHandler, (DWORD)this);
}

CMMFileListDlg::~CMMFileListDlg(void)
{
	CMsdkRender::GetInstance()->UnRegisterEventHandlerF(MediaEventHandler, (DWORD)this);

}

void CMMFileListDlg::OnInitDialog()
{
	__super::OnInitDialog();
	OpenDB();

	RegisterMsg(UI_MSG_LBUTTON_DOWN);
	RegisterMsg(UI_MSG_LBUTTON_UP);
	RegisterMsg(UI_MSG_BT_PB_QUERY_CHANGE);
	RegisterMsg(NAVI_MSG_LAN_CHANGE);

	m_pListFolder = (CWceUiListBox*)GetChildByName(L"listbox");
	m_pListFile   = (CFileListBox*)GetChildByName(L"file_listbox");
	ASSERT(m_pListFile && m_pListFolder);
	m_pListFolder->ShowLayer(FALSE);
	m_pListFile->SetPlayImage(95, m_texPlaying.GetBitmap());

	m_pLayerFrameKeypad = GetChildByName(L"keypad_frame");
	ASSERT(m_pLayerFrameKeypad);
	m_pLayerFrameKeypad->ShowLayer(FALSE);

	CWceUiEditBox* pEditbox = (CWceUiEditBox*)GetLayerByName(L"goto_number");
	if (pEditbox)
	{
		pEditbox->SetReadOnly(TRUE);
	}

	pEditbox = (CWceUiEditBox*)GetLayerByName(L"phonebook_query_string");
	if (pEditbox)
	{
		pEditbox->SetReadOnly(TRUE);
	}

	ShowQueryUI(FALSE);

	ShowPlayingData();

}


BOOL CMMFileListDlg::OnBnKeypadClick(CWceUiButton* pButton)
{
	const int KEY_COUNT = 11;
	LPCTSTR key_layers[] = {L"key_0", L"key_1", L"key_2", L"key_3", L"key_4", L"key_5", L"key_6", 
		L"key_7", L"key_8", L"key_9", L"key_backspace"};
	TCHAR key_char[] = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'\b'};


	int i = 0;
	for (i=0; i<KEY_COUNT; i++)
	{
		if (pButton->IsEqualName(key_layers[i]))
		{
			break;
		}
	}

	if (i<KEY_COUNT)	// 匹配到了
	{
		GetWindow()->PostMessage(WM_CHAR, (WPARAM)key_char[i], 0);
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

void CMMFileListDlg::ShowPlayingData()
{
	int folder = CMsdkFileManager::GetInstance()->GetPlayFolderIndex(m_curType);
	int file = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(m_curType);
	SOURCE_ID sid = CMsdkFileManager::GetInstance()->GetCurrentSrcID();
	MSDK_MEMORY* pmemory = sysutil::nss_get_msdk_memory(sid);


	// 还在当前的目录中播放,这里不需重新初始化数据,只需选中正在播放的文件就可以了,用户体验更好
	if (pmemory && m_curType == pmemory->type && folder == m_pListFolder->GetCurSel())
	{
		m_pListFile->SetCurSel(file);
		m_pListFile->SetPlayIndex(file);
		if (!m_pListFile->IsItemVisible(file))
		{
			m_pListFile->SetTopIndex(file);
		}

		// 浏览数据也要同步
		CMsdkFileManager::GetInstance()->SetBrowseFileIndex(file, m_curType);

		return;
	}

	// 播放目录变化了，从新初始化数据
	if (pmemory)
	{
		m_curType = pmemory->type;
	}
	folder = CMsdkFileManager::GetInstance()->GetPlayFolderIndex(m_curType);
	file = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(m_curType);

	CheckButton(m_curType);
	SetFolderData(m_curType);
	SetFileData(folder, m_curType, file);
	// 浏览数据也要同步
	CMsdkFileManager::GetInstance()->SetBrowseFileIndex(file, m_curType);
}

void CMMFileListDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_RETURN_PLAY_UI)
	{
		// 如果停止播放了,返回列表,不自动进播放状态
		if (CMsdkRender::GetInstance()->FGetState() == MSDK_MGSTATE_STOP
			&& (m_curType == MSDK_FILE_MUSIC || m_curType == MSDK_FILE_VIDEO))
		{
			;
		}
		else
		{
			KillTimer(TIMER_ID_RETURN_PLAY_UI);
			ShowPlayingDlg();
		}
	}
	else if (wIDEvent == TIMER_ID_DELAY_PLAY_NEXT)
	{
		KillTimer(TIMER_ID_DELAY_PLAY_NEXT);
		//PlayNext();
		m_nErrorCode = VIDEO_ERROR_NONE;
		CMsdkRender::GetInstance()->FStop();
		OnMsdkFinished();	// 执行与播放完成相同的逻辑
	}
	else if (wIDEvent == TIMER_ID_DELAY_PLAY_ITEM)
	{
		KillTimer(TIMER_ID_DELAY_PLAY_ITEM);
		int index = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(m_curPlayingType);
		LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, m_curPlayingType);
		CMsdkRender::GetInstance()->FSetSourceFile(file_path);
	}

	__super::OnTimer(wIDEvent);
}

void CMMFileListDlg::OnTypeChange(MSDK_FILE_TYPE type)
{
	if (m_curType != type)
	{
		m_curType = type;
		SetFolderData(m_curType);
		int nFolderIndex = CMsdkFileManager::GetInstance()->GetBrowseFolderIndex(m_curType);
		int nFileIndex = CMsdkFileManager::GetInstance()->GetBrowseFileIndex(m_curType);
		SetFileData(nFolderIndex, m_curType, nFileIndex);

		// 切换类型后，直接显示到文件数据，不显示目录
		m_pListFile->ShowLayer(TRUE);
		m_pListFolder->ShowLayer(FALSE);
		ShowFileTip();
		ShowTitle();
	}
}

void CMMFileListDlg::CheckButton(MSDK_FILE_TYPE type)
{
	CWceUiButton* pButtonMusic = (CWceUiButton*)GetChildByName(L"music");
	CWceUiButton* pButtonVideo = (CWceUiButton*)GetChildByName(L"video");
	CWceUiButton* pButtonPhoto = (CWceUiButton*)GetChildByName(L"photo");
	ASSERT(pButtonMusic && pButtonPhoto && pButtonVideo);

	pButtonMusic->SetStatusStyleName(L"music_normal", CWceUiButton::STATUS_NORMAL);
	pButtonVideo->SetStatusStyleName(L"video_normal", CWceUiButton::STATUS_NORMAL);
	pButtonPhoto->SetStatusStyleName(L"photo_normal", CWceUiButton::STATUS_NORMAL);
	switch (type)
	{
	case MSDK_FILE_MUSIC:
		pButtonMusic->SetStatusStyleName(L"music_down", CWceUiButton::STATUS_NORMAL);
		break;
	case MSDK_FILE_VIDEO:
		pButtonVideo->SetStatusStyleName(L"video_down", CWceUiButton::STATUS_NORMAL);
		break;
	case MSDK_FILE_PHOTO:
		pButtonPhoto->SetStatusStyleName(L"photo_down", CWceUiButton::STATUS_NORMAL);
		break;
	default:
		break;
	}
}

void CMMFileListDlg::OnDlgShow(BOOL bShow)
{
	DLG_ID did = CDlgManager::GetInstance()->GetLastDlgID();
	if (bShow )
	{
		// 返回后再进来，定位到正在播放的文件(只有从播放窗口返回时)
		if ((did == CMusicPlayerDlg_ID || did == CVideoPlayerDlg_ID || did == CPhotoPlayerDlg_ID))
		{
			ShowPlayingData();
		}
		ShowFileTip();
		ShowTitle();
		SetTimer(TIMER_ID_RETURN_PLAY_UI, TIME_RETURN_PLAY_UI);
	}
	else
	{
		KillTimer(TIMER_ID_RETURN_PLAY_UI);
	}

	ShowKeypad(FALSE);
	ShowQueryUI(FALSE);
}

LRESULT CMMFileListDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 没有mouse操作时,才计时返回播放界面
	if (IsLayerVisible())
	{
		if (message == UI_MSG_LBUTTON_DOWN)
		{
			KillTimer(TIMER_ID_RETURN_PLAY_UI);
		}
		else if (message == UI_MSG_LBUTTON_UP)
		{
			SetTimer(TIMER_ID_RETURN_PLAY_UI, TIME_RETURN_PLAY_UI);
		}
	}

	if (message == UI_MSG_BT_PB_QUERY_CHANGE)
	{
		UpdateQuery2Listbox();
	}
	else if (message == NAVI_MSG_LAN_CHANGE)
	{
		InitCodepage();
	}
	else if (message == UI_MSG_MSDK_PLAY_FILE)	// 播放一个新文件了
	{
		m_nErrorCode = VIDEO_ERROR_NONE;	// 播放新文件后清除标志
		KillTimer(TIMER_ID_DELAY_PLAY_NEXT);
	}


	return __super::OnMessage(message, wParam, lParam);
}


void CMMFileListDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		if (m_bShowQueryUI)
		{
			ShowQueryUI(FALSE);
		}
		else if (m_pListFile->IsLayerVisible())
		{
			ShowListBox(LISTBOX_FOLDER);
		}
		else
		{
			__super::OnBnClick(pButton);
		}
	}
	else if (pButton->IsEqualName(L"music"))
	{
		if (!m_bShowQueryUI)
		{
			OnTypeChange(MSDK_FILE_MUSIC);
			CheckButton(m_curType);
		}
	}
	else if (pButton->IsEqualName(L"photo"))
	{
		if (!m_bShowQueryUI)
		{
			OnTypeChange(MSDK_FILE_PHOTO);
			CheckButton(m_curType);
		}
	}
	else if (pButton->IsEqualName(L"video"))
	{
		if (!m_bShowQueryUI)
		{
			OnTypeChange(MSDK_FILE_VIDEO);
			CheckButton(m_curType);
		}
	}
	else if (pButton->IsEqualName(L"digital_key"))
	{
		if (!m_bShowQueryUI)	// 在搜索窗口时不充许进数字选曲界面
		{
			ShowListBox(LISTBOX_FILE);
			ShowKeypad(TRUE);
		}
	}
	else if (pButton->IsEqualName(L"search"))
	{
		ShowQueryUI(!m_bShowQueryUI);
	}
	else if (OnBnKeypadClick(pButton))
	{
		;
	}
	else if (pButton->IsEqualName(L"key_enter"))
	{
		OnKeyEnter();
	}
	else if (pButton->IsEqualName(L"phonebook_key_backspace"))
	{
		CWceUiEditBox* peditbox = (CWceUiEditBox*)GetLayerByName(L"phonebook_query_string");
		ASSERT(peditbox);
		if (_tcslen(peditbox->GetData()) > 0)	// 如果已经是空了,不用再刷新了
		{
			GetWindow()->PostMessage(WM_CHAR, (WPARAM)L'\b', 0);
			GetWindow()->PostMessage(UI_MSG_BT_PB_QUERY_CHANGE, 0, 0);
		}
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}

void CMMFileListDlg::ShowListBox(int nMode)
{
	if (nMode == LISTBOX_FILE)
	{
		m_pListFile->ShowLayer(TRUE);
		m_pListFolder->ShowLayer(FALSE);
	}
	else
	{
		m_pListFile->ShowLayer(FALSE);
		m_pListFolder->ShowLayer(TRUE);
	}

	ShowTitle();
	ShowFileTip();

}

void CMMFileListDlg::ShowKeypad(BOOL bShow)
{
	m_pLayerFrameKeypad->ShowLayer(bShow);
	CGotoEditBox* pEditbox = (CGotoEditBox*)GetLayerByName(L"goto_number");
	if (pEditbox)
	{
		if (bShow)
		{
			pEditbox->SetTotal(m_pListFile->GetCount());
			pEditbox->SetData(L"");
			pEditbox->OnFocus();
		}
		else
		{
			pEditbox->OnKillFocus();
		}
	}
}

void CMMFileListDlg::OnKeyEnter()
{
	CWceUiEditBox* pEditbox = (CWceUiEditBox*)GetLayerByName(L"goto_number");
	if (pEditbox)
	{
		int index = _ttoi(pEditbox->GetData()) - 1;
		if (index>=0 && index<m_pListFile->GetCount())
		{
			PlayItem(index);
		}
	}

	ShowKeypad(FALSE);
}

DLG_ID CMMFileListDlg::GetPlayerDlg(MSDK_FILE_TYPE type)
{
	DLG_ID did = INVALID_DLG_ID;

	if (type == MSDK_FILE_MUSIC)
	{
		did = CMusicPlayerDlg_ID;
	}
	else if (type == MSDK_FILE_VIDEO)
	{
		did = CVideoPlayerDlg_ID;
	}
	else if (type == MSDK_FILE_PHOTO)
	{
		did = CPhotoPlayerDlg_ID;
	}
	else 
	{
		ASSERT(FALSE);
	}

	return did;
}

void CMMFileListDlg::ShowPlayingDlg()
{
	SOURCE_ID sid = CMsdkFileManager::GetInstance()->GetCurrentSrcID();
	MSDK_MEMORY* pmemory = sysutil::nss_get_msdk_memory(sid);
	if (pmemory)
	{
		DLG_ID did = GetPlayerDlg(pmemory->type);

		// 音视频不可能同时播放,把另外的删除掉
		// 如果是进到图片,不删除播放窗口
		if (did == CVideoPlayerDlg_ID || did == CMusicPlayerDlg_ID)
		{
			if (did != CVideoPlayerDlg_ID)
			{
				if(CDlgManager::GetInstance()->IsDlgCreated(CVideoPlayerDlg_ID, ui_msdk_get_cursrc()))
				{
					CDlgManager::GetInstance()->DeleteDlg(CVideoPlayerDlg_ID, ui_msdk_get_cursrc());
				}
			}
			else if (did != CMusicPlayerDlg_ID)
			{
				if(CDlgManager::GetInstance()->IsDlgCreated(CMusicPlayerDlg_ID, ui_msdk_get_cursrc()))
				{
					CDlgManager::GetInstance()->DeleteDlg(CMusicPlayerDlg_ID, ui_msdk_get_cursrc());
				}
			}
		}


		if (IsLayerVisible())
		{
			CDlgManager::GetInstance()->ShowDlg(did, ui_msdk_get_cursrc());
		}
		else	// 如果列表窗口已经在后台了,把源的当前窗口切换为播放窗口,但不显示出来
		{
			CDlgManager::GetInstance()->SetActiveDlg(did, ui_msdk_get_cursrc());
			// 如果last dlg刚好是该窗口,需主动更新last dlg信息，从其它窗口返回时,应返回这里设置的活动窗口
			if (CDlgManager::GetInstance()->GetLastDlgID() == CMMFileListDlg_ID)
			{
				DLG_INFO* pdi = CDlgManager::GetInstance()->GetDlgInfo(did, ui_msdk_get_cursrc());
				CDlgManager::GetInstance()->SetLastDlgInfo(pdi);
			}
		}
	}
}

void CMMFileListDlg::UpdatePlayDlgMediaInfo(MSDK_FILE_TYPE type)
{
	if (type == MSDK_FILE_MUSIC)
	{
		DLG_INFO *pinfo = CDlgManager::GetInstance()->GetDlgInfo(CMusicPlayerDlg_ID, ui_msdk_get_cursrc());
		if (pinfo && pinfo->pdlg)
		{
			((CMusicPlayerDlg*)pinfo->pdlg)->UpdateMediaInfo();	// 在动画前把ID3等信息刷新下
		}
	}
	else if (type == MSDK_FILE_VIDEO)
	{
		DLG_INFO *pinfo = CDlgManager::GetInstance()->GetDlgInfo(CVideoPlayerDlg_ID, ui_msdk_get_cursrc());
		if (pinfo && pinfo->pdlg)
		{
			((CVideoPlayerDlg*)pinfo->pdlg)->UpdateMediaInfo();	// 在动画前把进度条信息刷新下
		}
	}
}

void CMMFileListDlg::PlayItem(int index)
{
	int old_folder = -1;
	int old_file = -1;
	MSDK_FILE_TYPE old_type = (MSDK_FILE_TYPE)-1;	// invalid type

	// 当前播放文件类型变了，需更新断点记忆信息
	SOURCE_ID sid = CMsdkFileManager::GetInstance()->GetCurrentSrcID();
	MSDK_MEMORY* pmemory = sysutil::nss_get_msdk_memory(sid);
	if (m_curType == m_curPlayingType)		// 如果播放图片时，后台音乐，再回到音乐播放时，继续断点播放
	{
		// 记录下当前正在播放的文件信息
		old_folder = CMsdkFileManager::GetInstance()->GetPlayFolderIndex(m_curPlayingType);
		old_file = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(m_curPlayingType);
		old_type = m_curPlayingType;
		if (pmemory)
		{
			pmemory->type = m_curType;
		}
	}
	else
	{
		if (pmemory)
		{
			// 记录下当前正在播放的文件信息
			old_folder = CMsdkFileManager::GetInstance()->GetPlayFolderIndex(pmemory->type);
			old_file = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(pmemory->type);
			old_type = pmemory->type;

			pmemory->type = m_curType;
		}
	}

	// 更新播放文件信息
	int nFolderIndex = CMsdkFileManager::GetInstance()->GetBrowseFolderIndex(m_curType);
	CMsdkFileManager::GetInstance()->SetPlayFileData(nFolderIndex, index, m_curType);

	// 如果当前点击的文件正好是播放的文件,则不需重新播放
	if ((m_curType == old_type)
		&& nFolderIndex == old_folder && index == old_file
		// 该曲目如果已经播放完毕或没有开始播放,还是需要重新播放
		&& CMsdkRender::GetInstance()->FGetState() != MSDK_MGSTATE_STOP)
	{
		if (CMsdkRender::GetInstance()->FGetState() == MSDK_MGSTATE_PAUSE)
		{
			CMsdkRender::GetInstance()->FPlay();
		}
	}
	else	// 播放文件
	{
		LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, m_curType);
		if (m_curType == MSDK_FILE_MUSIC || m_curType == MSDK_FILE_VIDEO)
		{
			CMsdkRender::GetInstance()->FStop();	// 把之播放的停止掉,以免影响播放界面的刷新,因为播放曲目延时播放了
			// ++
			// 因为播放图片时,内存问题，关掉了MSDK播放,重新播放时,前后排要重新设定下
			CMsdkRender::GetInstance()->FSetSink(DVP_SINK_FRONT_REAR);
			// --
			m_curPlayingType = m_curType;
			UpdatePlayDlgMediaInfo(m_curType);	// 刷新播放窗口信息
			//CMsdkRender::GetInstance()->FSetSourceFile(file_path);
			SetTimer(TIMER_ID_DELAY_PLAY_ITEM, 500);
		}
		else if (m_curType == MSDK_FILE_PHOTO)
		{
			if (m_curPlayingType == MSDK_FILE_VIDEO)	// 视频播放同时播放图片,内存不够,停止视频播放
			{
				CMsdkRender::GetInstance()->FStopnClear();
			}
		}
		else 
		{
			ASSERT(FALSE);
		}
	}

	// 在列表中标识正在播放的歌曲
	m_pListFile->SetPlayIndex(index);

	// 显示播放窗口
	ShowPlayingDlg();
}

void CMMFileListDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	if (plist == m_pListFolder)
	{
		if (bClickChecked)
		{
			int index = plist->GetCurSel();
			SetFileData(index, m_curType, 0);
			CMsdkFileManager::GetInstance()->SetBrowseFileIndex(0, m_curType);	// 浏览信息也同步
			ShowListBox(LISTBOX_FILE);
		}
	}
	else if (plist == m_pListFile)
	{
		// 更新浏览文件信息
		int index = plist->GetCurSel();
		if (m_bShowQueryUI)		// 如果在查询状态,列表的索引值需转换为真正的文件索引
		{
			index = m_PBQueryResult.GetAt(index);
		}

		CMsdkFileManager::GetInstance()->SetBrowseFileIndex(index, m_curType);

		if (bClickChecked)
		{
			PlayItem(index);
		}

		ShowFileTip();
		ShowTitle();
	}

}

void CMMFileListDlg::SetFolderData(MSDK_FILE_TYPE type)
{
	m_pListFolder->ResetContent();
	const CStringArray& folder_array = CMsdkFileManager::GetInstance()->GetFolderData(type);
	AddString2Listbox(folder_array, m_pListFolder, m_texFolder.GetBitmap());
}

void CMMFileListDlg::SetFileData(int nFolderIndex, MSDK_FILE_TYPE type, int nFileIndex)
{
	// 显示哪个目录的文件就选中该目录
	m_pListFolder->SetCurSel(nFolderIndex);

	CWceUiGenericBitmap* pTexture = NULL;
	if (type == MSDK_FILE_MUSIC)
	{
		pTexture = m_texMusic.GetBitmap();
	}
	else if (type == MSDK_FILE_PHOTO)
	{
		pTexture = m_texPhoto.GetBitmap();
	}
	else if (type == MSDK_FILE_VIDEO)
	{
		pTexture = m_texVideo.GetBitmap();
	}

	m_pListFile->ResetContent();
	const CStringArray& file_array = CMsdkFileManager::GetInstance()->GetBrowseFileData(nFolderIndex, type);
	AddString2Listbox(file_array, m_pListFile, pTexture);
	AddString2DB(file_array);

	m_pListFile->SetCurSel(nFileIndex);	// 选中当前文件
	if (!m_pListFile->IsItemVisible(nFileIndex))
	{
		m_pListFile->SetTopIndex(nFileIndex);
	}

	// 只要列表数据有变化,检测是否是正在播放的目录
	// 如果是正在播放的目录,标识正在播放的歌曲
	SOURCE_ID sid = CMsdkFileManager::GetInstance()->GetCurrentSrcID();
	MSDK_MEMORY* pmemory = sysutil::nss_get_msdk_memory(sid);
	MSDK_FILE_TYPE playing_type = pmemory ? pmemory->type : MSDK_FILE_INVALID;

	int folder = CMsdkFileManager::GetInstance()->GetPlayFolderIndex(m_curType);
	int file = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(m_curType);
	if ((playing_type == type || m_curPlayingType == type)		// 当前图片时，如果后台音乐或视频时，仍记住其正在播放的曲目和位置
		&& nFolderIndex == folder)
	{
		m_pListFile->SetPlayIndex(file);
	}
	else
	{
		m_pListFile->SetPlayIndex(-1);
	}


}

void CMMFileListDlg::AddString2Listbox(const CStringArray& strArray, CWceUiListBox* plistbox, 
									   CWceUiGenericBitmap* pTexture)
{
	ASSERT(plistbox);
	TCHAR szFileName[MAX_PATH];
	TCHAR buffer[MAX_PATH];
	int index = 0;
	for (int i=0; i<strArray.GetCount(); i++)
	{
		tzutil::GetFileName(strArray[i], szFileName);
		if (plistbox->IsEqualName(L"listbox"))	// 目录列表
		{
			index = plistbox->AddString(szFileName);
		}
		else	// 文件加上序号
		{
			_stprintf(buffer, L"%02d    %s", i+1, szFileName);
			index = plistbox->AddString(buffer);
		}
		plistbox->SetItemImage(index, pTexture);
	}
}

void CMMFileListDlg::CheckMediaInfo()
{
	if (!CMsdkRender::GetInstance()->FIsMediaTypeSupport())
	{
		OnErrorOccured(E_MSDK_NOT_SUPPORT, 0);
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"MIS_UNSUPPORTED_VIDEO", TRUE, 1000);
	}
}

void CMMFileListDlg::OnErrorOccured(UINT param1, UINT param2)
{
	if (param1 != E_MSDK_OUTOFMEMORY)
	{
		if (param1 == E_MSDK_UNSUPPORTED_VIDEO)
		{
			m_nErrorCode = VIDEO_ERROR_VIDEO;
		}
		else if (param1 == E_MSDK_UNSUPPORTED_AUDIO)
		{
			m_nErrorCode = VIDEO_ERROR_AUDIO;
		} 
		else
		{
			m_nErrorCode = VIDEO_ERROR_FILE;
		}
		SetTimer(TIMER_ID_DELAY_PLAY_NEXT, 1500);
	}
}

void CMMFileListDlg::OnMsdkFinished()
{
	if (m_nErrorCode == VIDEO_ERROR_NONE)	// 文件错误时会延时播放
	{
		if (!PlayNext())	// 如果播放下一曲失败了,有可能是列表播放完毕,这时应从播放画面回到列表画面
		{
			// mp5上播放完成后，播放状态好像没有变成STOP,这里主动停止掉
			CMsdkRender::GetInstance()->FStop();

			SOURCE_ID sid = CMsdkFileManager::GetInstance()->GetCurrentSrcID();
			if (m_curType == MSDK_FILE_MUSIC || m_curType == MSDK_FILE_VIDEO)
			{
				DLG_INFO *pdi_cur = CDlgManager::GetInstance()->GetCurDlgInfo();
				if (pdi_cur && pdi_cur->source_id == sid)	// 当前窗口是当前源的
				{
					CDlgManager::GetInstance()->ShowDlg(CMMFileListDlg_ID, sid);
				}
				else
				{
					CDlgManager::GetInstance()->SetActiveDlg(CMMFileListDlg_ID, sid);
				}
			}

			//
			DLG_INFO *pdi_playdlg = CDlgManager::GetInstance()->GetDlgInfo(CMusicPlayerDlg_ID, sid);
			if (pdi_playdlg && pdi_playdlg->pdlg)
			{
				((CDvpBaseDlg*)pdi_playdlg->pdlg)->UpdataProgress(0,0);	// 播放完成后,进度条归0,UI看起来效果好点
				((CDvpBaseDlg*)pdi_playdlg->pdlg)->ShowMediaTimeInfo(0, CMsdkRender::GetInstance()->FGetDuration());
			}

			pdi_playdlg = CDlgManager::GetInstance()->GetDlgInfo(CVideoPlayerDlg_ID, sid);
			if (pdi_playdlg && pdi_playdlg->pdlg)
			{
				((CDvpBaseDlg*)pdi_playdlg->pdlg)->UpdataProgress(0,0);	// 播放完成后,进度条归0,UI看起来效果好点
				((CDvpBaseDlg*)pdi_playdlg->pdlg)->ShowMediaTimeInfo(0, CMsdkRender::GetInstance()->FGetDuration());
			}

		}
	}
}

void CMMFileListDlg::OnKeyPlayControl(UINT key, UINT param)
{
	if (m_curPlayingType != MSDK_FILE_MUSIC && m_curPlayingType != MSDK_FILE_VIDEO)
	{
		return;
	}

	BYTE byte0 = (BYTE)((param>>24) & 0xFF);

	switch (key)
	{
	case T_PREV:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PREV");
		PlayPrev();
		break;
	case T_NEXT:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_NEXT");
		PlayNext();
		break;
	case T_PLAY:	// param byte0 : 0 ~ 播放暂停复合键, 1 ~ 播放， 2 ~ 暂停
		if (byte0 == 1)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
			CMsdkRender::GetInstance()->FPlay();
		}
		else if (byte0 == 2)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
			CMsdkRender::GetInstance()->FPause();
		}
		else
		{
			if( MSDK_MGSTATE_PLAYING == CMsdkRender::GetInstance()->FGetState())
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
				CMsdkRender::GetInstance()->FPause();
			}
			else
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
				CMsdkRender::GetInstance()->FPlay();
			}
		}
		break;
	case T_STOP:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_STOP");
		CMsdkRender::GetInstance()->FStop();
		break;
	}
}

void CMMFileListDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	//RETAILMSG(1, (L"+++++++++++++++EVENT = %d ,  \n", media_event));
	switch (media_event)
	{
	case EVT_MSDK_GRAPH_BUILT:
		OnGraphBuilt();
		break;
	case EVT_MSDK_FINISHED:	
		OnMsdkFinished();
		break;
	case EVT_MSDK_ERROR_OCCURED:
		OnErrorOccured(param1, param2);
		break;
	case EVT_MSDK_GRAPH_CHANGED:
		break;
	case EVT_MSDK_PLAYBACK_POSITION_STATUS:
		{
			// refresh LED
			UINT curDuration = param1 / 1000;
			int cur = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(m_curPlayingType);
			ui_led_set_showdata(ui_led_media_data(cur+1, curDuration));
			CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
		}
		break;
	case EVT_MSDK_PLAYED:
		break;
	case EVT_MSDK_PAUSED:
		ui_led_set_showdata(media_event == EVT_MSDK_PAUSED ? L" PAUS E  " : L"  STO P  ");
		CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
		break;
	case EVT_MSDK_STOPED:	// stop不显示字符
		break;
	default:
		break;
	}
}

BOOL CMMFileListDlg::PlayNext()
{
	SOURCE_ID sid = CMsdkFileManager::GetInstance()->GetCurrentSrcID();
	int index = CMsdkFileManager::GetInstance()->PlayFileNext(m_curPlayingType);

	// 如果文件列表处于当前正在播放的目录,标识正在播放的歌曲
	if (m_curPlayingType == m_curType)
	{
		int nFolderBrowse = CMsdkFileManager::GetInstance()->GetBrowseFolderIndex(m_curType);
		int nFolderPlay = CMsdkFileManager::GetInstance()->GetPlayFolderIndex(m_curType);
		if (nFolderBrowse == nFolderPlay)
		{
			int file = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(m_curType);
			m_pListFile->SetPlayIndex(file);
		}
		else
		{
			m_pListFile->SetPlayIndex(-1);	// 可能已经跳到其它目录了
		}
	}

	LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, m_curPlayingType);
	return CMsdkRender::GetInstance()->FSetSourceFile(file_path);
}


BOOL CMMFileListDlg::PlayPrev()
{
	SOURCE_ID sid = CMsdkFileManager::GetInstance()->GetCurrentSrcID();
	int index = CMsdkFileManager::GetInstance()->PlayFilePrev(m_curPlayingType);

	// 如果文件列表处于当前正在播放的目录,标识正在播放的歌曲
	if (m_curPlayingType == m_curType)
	{
		int nFolderBrowse = CMsdkFileManager::GetInstance()->GetBrowseFolderIndex(m_curType);
		int nFolderPlay = CMsdkFileManager::GetInstance()->GetPlayFolderIndex(m_curType);
		if (nFolderBrowse == nFolderPlay)
		{
			int file = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(m_curType);
			m_pListFile->SetPlayIndex(file);
		}
		else
		{
			m_pListFile->SetPlayIndex(-1);	// 可能已经跳到其它目录了
		}
	}

	LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, m_curPlayingType);
	return CMsdkRender::GetInstance()->FSetSourceFile(file_path);
}

void CMMFileListDlg::InitCodepage()
{
	int lan_id = sysutil::nss_get_instance()->ui_lan_id;
	LANGUAGE_TABLE_INFO* pinfo = config::get_language_info(lan_id);
	if (pinfo)
	{
		CMsdkRender::GetInstance()->FSetSubtitleCodePage((E_MSDK_CODEPAGE_SET)pinfo->msdk_code_page);
	}
}

void CMMFileListDlg::OnGraphBuilt()
{
	// 处理断点播放
	SOURCE_ID sid = CMsdkFileManager::GetInstance()->GetCurrentSrcID();
	MSDK_MEMORY* pmemory = sysutil::nss_get_msdk_memory(sid);

	if (pmemory && pmemory->memory_play)
	{
		pmemory->memory_play = FALSE;
		CMsdkRender::GetInstance()->FLastMemoryStart(pmemory->pos);

		// 如果断点播放的是视频,直接进播放界面,音乐的话,是延时一段时间再进播放界面
		DLG_ID did = GetPlayerDlg(pmemory->type);
		if (did == CVideoPlayerDlg_ID)
		{
			CMsdkRender::GetInstance()->FShowVideo(FALSE);
			ShowPlayingDlg();
		}

		// 如果是断点播放,正在播放的类型应该是memory记录的类型
		m_curPlayingType = pmemory->type;
	}

	// 如果视频窗口已存在,说明视频在播放,不能用pmemory->type来判断,因为有可能视频播放后，进到图片播放了
	if (m_curPlayingType == MSDK_FILE_VIDEO)
	{
		// 如果不支持的视频格式,主动到下一曲
		CheckMediaInfo();

		InitCodepage();
	}
}


void CMMFileListDlg::ShowFileTip()
{
	int cur = CMsdkFileManager::GetInstance()->GetBrowseFileIndex(m_curType);
	int total = CMsdkFileManager::GetInstance()->GetBrowseFilesCount(m_curType);
	if (total<0)
	{
		total = 0;	// 如果返回数据不合法,显示0
	}

	TCHAR buffer[10] = L"";
	if (total>0 && cur<total)	// 数据必须合法
	{
		_stprintf(buffer, TEXT("%d/%d"), (cur+1), total); 
	}
	else	// 数据为空或非法数据
	{
		_stprintf(buffer, TEXT("_/_"), (cur+1), total); 
	}
	CWceUiLayer *ptip = GetChildByName(L"file_tip");
	if (ptip)
	{
		ptip->SetText(buffer);
	}
}


void CMMFileListDlg::ShowTitle()
{
	// title显示当前播放的目录
	CWceUiLayer* ptitle = GetLayerByName(L"title");
	if (ptitle)
	{
		const CStringArray& folder = CMsdkFileManager::GetInstance()->GetFolderData(m_curType);
		if (m_pListFolder->IsLayerVisible()			// 在目录列表状态,显示根目录就可以了
			|| folder.GetCount()<=0)			// 如果没有任何目录数据,也显示根目录
		{
			if (ui_msdk_get_cursrc() == SOURCE_SD)
				ptitle->SetText(L"SD\\");
			else if (ui_msdk_get_cursrc() == SOURCE_USB)
				ptitle->SetText(L"USB1\\");
			else if (ui_msdk_get_cursrc() == SOURCE_USB_REAR)
				ptitle->SetText(L"USB2\\");
		}
		else	// 在文件列表状态时显示当前浏览的目录
		{
			CRect rc;
			TCHAR file_path[MAX_PATH]=L"";
			int index = CMsdkFileManager::GetInstance()->GetBrowseFolderIndex(m_curType);
			ptitle->GetWindowRect(rc);
			if (index>=0 && index<folder.GetCount())	// 保证数据的合法性
			{
				ui_format_file_path(ptitle->GetStyle()->GetFontCtrl()->GetFont(), rc, folder.GetAt(index), file_path);
			}

			ptitle->SetText(file_path);
		}
	}
}

void CMMFileListDlg::ShowQueryUI(BOOL bShow)
{
	if ((bShow && !m_bShowQueryUI) || (!bShow && m_bShowQueryUI))
	{
		m_bShowQueryUI = bShow;
		CWceUiEditBox *peditbox = (CWceUiEditBox*)GetLayerByName(L"phonebook_query_string");
		// 更新列表数据
		if (bShow)
		{
			ASSERT(peditbox);
			peditbox->SetData(L"");
			UpdateQuery2Listbox();
			peditbox->OnFocus();
			ShowListBox(LISTBOX_FILE);	// 如果是在目录列表，则需切换到文件列表
		}
		else
		{
			int nFolderIndex = CMsdkFileManager::GetInstance()->GetBrowseFolderIndex(m_curType);
			int nFileIndex = CMsdkFileManager::GetInstance()->GetBrowseFileIndex(m_curType);
			SetFileData(nFolderIndex, m_curType, nFileIndex);

			peditbox->OnKillFocus();
		}
	}


	CWceUiLayer* pkeypad = GetChildByName(L"phonebook_keypad");
	CWceUiLayer* pbackspace = GetChildByName(L"phonebook_key_backspace");
	CWceUiLayer* peditbox = GetChildByName(L"phonebook_query_string");
	CWceUiListBox* plistbox = (CWceUiListBox*)GetChildByName(L"file_listbox");
	ASSERT( plistbox && pkeypad && pbackspace && peditbox);

	pkeypad->ShowLayer(bShow);
	pbackspace->ShowLayer(bShow);
	peditbox->ShowLayer(bShow);

	CRect src;
	WCEUI_POSITION_WAY way = plistbox->GetStyle()->GetPosition(src);
	if (bShow)
	{
		if (WceUiGetScreenWidth() == 1024 && WceUiGetScreenHeight() == 600)
			src.SetRect(39, 88, 39+391, 88+395);
		else
			src.SetRect(39, 88, 39+322, 88+275);

		plistbox->SetTextLeftOffset(5);
		plistbox->SetDrawItemImage(FALSE);
	}
	else
	{
		if (WceUiGetScreenWidth() == 1024 && WceUiGetScreenHeight() == 600)
			src.SetRect(39, 88, 39+946, 88+395);
		else
			src.SetRect(39, 88, 39+722, 88+275);

		plistbox->SetTextLeftOffset(20);
		plistbox->SetDrawItemImage(TRUE);
	}
	plistbox->GetStyle()->SetPosition(&src, way);
}


void CMMFileListDlg::UpdateQuery2Listbox()
{
	CFileListBox *pListbox = (CFileListBox*)GetChildByName(L"file_listbox");
	CWceUiEditBox *peditbox = (CWceUiEditBox*)GetChildByName(L"phonebook_query_string");
	ASSERT(pListbox && peditbox);

	pListbox->ResetContent();
	m_PBQueryResult.RemoveAll();
	try
	{
		TCHAR szSQL[128];
		if (_tcslen(peditbox->GetData()) < 64)	// 防止越界
		{
			_stprintf(szSQL, L"select * from media where pyname like '%%%s%%' or name like '%%%s%%';", peditbox->GetData(), peditbox->GetData());
			CppSQLite3Query q = m_db.execQuery(szSQL);
			while (!q.eof())
			{
				pListbox->AddString(q.fieldValue(1));	// 第二个字段
				m_PBQueryResult.Add(q.getIntField(0));	// 第一个字段
				q.nextRow();
			}
		}
	}
	catch (CppSQLite3Exception& e)
	{
		RETAILMSG(1, (L"db select error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}

	pListbox->SetCurSel(0);
	pListbox->SetPlayIndex(-1);
}

void CMMFileListDlg::OpenDB()
{
	try
	{
		m_db.open(L"residentflash3\\bt\\bt.db");
		m_db.execDML(L"PRAGMA synchronous = OFF; ");
		if (!m_db.tableExists(L"media"))
		{
			// CBluetooth中定义的结构体名字和号码最长为64
			m_db.execDML(L"create table media(id int, name nchar(256), pyname nchar(8));");
		}
	}
	catch (CppSQLite3Exception& e)
	{
		RETAILMSG(1, (L"db create error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}
}


BOOL CMMFileListDlg::AddString2DB(const CStringArray& strArray)
{
	// 更新数据库,先把数据数据清除
	try
	{
		m_db.execDML(L"delete from media;");
	}
	catch (CppSQLite3Exception& e)
	{
		RETAILMSG(1, (L"db delete error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}

	try
	{
		m_db.execDML(L"begin transaction;");
		CppSQLite3Statement stmt = m_db.compileStatement(L"insert into media values (?, ?, ?);");

		for (int i=0;i<strArray.GetCount();i++)
		{
			TCHAR szFileName[MAX_PATH];
			tzutil::GetFileName(strArray[i], szFileName);

			// 添加DB数据
			stmt.bind(1, i);
			stmt.bind(2, szFileName);
			stmt.bind(3, tzutil::GetStringPYCode(szFileName));
			stmt.execDML();
			stmt.reset();
		}
		m_db.execDML(L"commit transaction;");
	}
	catch (CppSQLite3Exception& e)
	{
		m_db.execDML(L"rollback transaction;");
		RETAILMSG(1, (L"db error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}

	return TRUE;
}


//////////////////////////////// CFileListBox //////////////////////////////


CFileListBox::CFileListBox()
{
	m_nIndexPlay = -1;
	m_nOffsetX = 0;
	m_pTexturePlay = NULL;
}

CFileListBox::~CFileListBox()
{

}


void CFileListBox::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	CRect rc_client;
	GetWindowRect(&rc_client);

	CRect rc_need_draw;
	GetNeedDrawRect(rc_need_draw);

	CRect rc_items(rc_client);
	rc_items.top -= GetScrollPos();
	rc_items.bottom = rc_items.top + m_cyItemHeight*GetCount();

	// draw items
	int first_visible_items = 0;
	int total_visible_items = 0;
	total_visible_items = GetVisibleItems(&first_visible_items, rc_client, rc_need_draw);

	// visible
	if (m_nIndexPlay>=first_visible_items && m_nIndexPlay<first_visible_items+total_visible_items)
	{
		if (m_pTexturePlay)
		{
			int cy = ((int)m_cyItemHeight - (int)m_pTexturePlay->GetHeight()) / 2;


			CRect rc_item(rc_items);
			rc_item.top = rc_items.top+m_nIndexPlay*m_cyItemHeight+cy;
			rc_item.bottom = rc_item.top + m_pTexturePlay->GetHeight();
			rc_item.left = m_nOffsetX;
			rc_item.right = rc_item.left + m_pTexturePlay->GetWidth();


			CRect rc_out;
			::IntersectRect(rc_out, rc_item, rc_client); // 区域不能超过
			CRect rc_src(0, 0, m_pTexturePlay->GetWidth(), m_pTexturePlay->GetHeight());
//			wceui::wceglDrawTexture(m_pTexturePlay, &rc_item, &rc_src);

			BLENDFUNCTION bf;
			memset(&bf, 0, sizeof(BLENDFUNCTION));
			bf.AlphaFormat = AC_SRC_ALPHA;

			pWndBitmap->AlphaBlend(rc_item.left, rc_item.top, m_pTexturePlay->GetWidth(), m_pTexturePlay->GetHeight(),
				m_pTexturePlay, 0, 0, bf);
		}


	}
}

void CFileListBox::SetPlayIndex(int nIndex)
{
	m_nIndexPlay = nIndex;
	InvalidateRect(NULL);
}

void CFileListBox::SetPlayImage(int nOffsetX, CWceUiGenericBitmap* pTexture)
{
	m_nOffsetX = nOffsetX;
	m_pTexturePlay = pTexture;
}

///////////////////////////////////////////////////////////////////////////////////////
//                               CGotoEditBox                                        //
///////////////////////////////////////////////////////////////////////////////////////

void CGotoEditBox::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	TCHAR buffer[16] = L"";
	FormatData(buffer);
	CString strOrginal = m_strData.c_str();

	// 用格式化的数据输出,输出完成后，恢复原始数据
	m_strData = buffer;
	__super::OnDraw(pWndBitmap, rcNeedDraw);
	m_strData = strOrginal;

}

void CGotoEditBox::FormatData(OUT LPTSTR buffer)
{
	int index = 0;
	if (m_strData.length() > 0)
	{
		index = _ttoi(m_strData.c_str());
	}

	if (index > m_nTotal)	// 表示输入的数值不合法,调整该值
	{
		index /= 10;
		
		OnChar(L'\b', 1, 0);	// 删除最后的输入

	}

	// 显示内容
	if (buffer)
	{
		if (m_nTotal == 0)	// 没有数据
		{
			_tcscpy(buffer, L"_/_");
		}
		else
		{
			if (index <= 0)		// 没有指定数据
			{
				_stprintf(buffer, L"_/%d", m_nTotal);
			}
			else
			{
				_stprintf(buffer, L"%d/%d", index, m_nTotal);
			}
		}
	}
}

void CGotoEditBox::SetTotal(int nTotal)
{
	if (nTotal>=0)
	{
		m_nTotal = nTotal;
	}
}

