#include "stdafx.h"
#include "DVDFileListDlg.h"
#include "WceUiListBox.h"
#include "tzUtil.h"
#include "dvdrender.h"
#include "sysUtil.h"
#include "interface.h"
#include "BTCallDlg.h"

void CDVDFileListDlg::DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData)
{
	CDVDFileListDlg* pthis = (CDVDFileListDlg*)u4CustomData;
	pthis->OnMediaEvent(u4Evt, u4Param1, u4Param2);
}



CDVDFileListDlg::CDVDFileListDlg(void)
: m_nCmdStatus(DVP_OP_CMD_NONE)
, m_nRefreshMode(-1)

{
	CDvdRender::GetInstance()->RegisterEventHandler(DvpAgentEventListener, (DWORD)this);
	m_szPlayFile[0] = L'\0';
}

CDVDFileListDlg::~CDVDFileListDlg(void)
{
	CDvdRender::GetInstance()->UnRegisterEventHandler(DvpAgentEventListener);
}

void CDVDFileListDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(UI_MSG_LBUTTON_DOWN);
	RegisterMsg(UI_MSG_LBUTTON_UP);
	RegisterMsg(YC_MESSAGE_LOAD_UI2);

	m_curType = (E_FILE_FILTER_BTN)CDvdRender::GetInstance()->GetDVPLMFileInfo()->DVPLMFileTye;
	RefreshMode(m_curType);

	m_pListFolder = (CDVDFolderListBox*)GetChildByName(L"listbox");
}


void CDVDFileListDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_RETURN_PLAY_UI)
	{
		KillTimer(TIMER_ID_RETURN_PLAY_UI);
		ShowPlayingDlg();
	}
	else if (wIDEvent == TIMER_ID_SHOW_WAIT_CUSOR)
	{
		KillTimer(TIMER_ID_SHOW_WAIT_CUSOR);
		CWceUiAnimLayer* pLayer = (CWceUiAnimLayer*)GetChildByName(L"wait_cursor");
		pLayer->ShowLayer(TRUE);
	}

	__super::OnTimer(wIDEvent);
}


void CDVDFileListDlg::OnDlgShow(BOOL bShow)
{
	// 返回后再进来，定位到正在播放的文件
	if (bShow)
	{
		SetTimer(TIMER_ID_RETURN_PLAY_UI, TIME_RETURN_PLAY_UI);
	}
	else
	{
		KillTimer(TIMER_ID_RETURN_PLAY_UI);
	}
}

LRESULT CDVDFileListDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
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

	if (message == YC_MESSAGE_LOAD_UI2)
	{
		LoadUI(lParam);

		CWceUiLayer *plist = GetChildByName(L"listbox");
		plist->EnableLayer(TRUE);
	}

	return __super::OnMessage(message, wParam, lParam);
}


void CDVDFileListDlg::LoadUI(int type)
{
	CWceUiDlg **pDlg = NULL;
	DLG_ID did = INVALID_DLG_ID;
	switch (type)
	{
	case FILE_FILTER_BTN_AUDIO:
		did = CDVDAudioPlayerDlg_ID;
		ui_led_set_showdata(L"  AUD IO ");
		break;
	case FILE_FILTER_BTN_VIDEO:
		did = CDVDMPSPlayerDlg_ID;
		ui_led_set_showdata(L"  VID EO ");
		break;
	case FILE_FILTER_BTN_PIC:
		did = CDVDPhotoPlayerDlg_ID;
		ui_led_set_showdata(L"  PHO TO ");
		break;
	default:
		break;
	}

	if (IsLayerVisible())
	{
		CDlgManager::GetInstance()->ShowDlg(did, SOURCE_DVD);
	}
	else	// 如果列表窗口已经在后台了,把源的当前窗口切换为播放窗口,但不显示出来
	{
		CDlgManager::GetInstance()->SetActiveDlg(did, SOURCE_DVD);
		// 如果last dlg刚好是该窗口,需主动更新last dlg信息，从其它窗口返回时,应返回这里设置的活动窗口
		if (CDlgManager::GetInstance()->GetLastDlgID() == CDVDFileListDlg_ID)
		{
			DLG_INFO* pdi = CDlgManager::GetInstance()->GetDlgInfo(did, SOURCE_DVD);
			CDlgManager::GetInstance()->SetLastDlgInfo(pdi);
		}
	}

	m_nCmdStatus = DVP_OP_CMD_NONE;
}



void CDVDFileListDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
	}
	if (pButton->IsEqualName(L"music"))
	{
		RefreshMode(FILE_FILTER_BTN_AUDIO);
	}
	else if (pButton->IsEqualName(L"video"))
	{
		RefreshMode(FILE_FILTER_BTN_VIDEO);
	}
	else if (pButton->IsEqualName(L"photo"))
	{
		RefreshMode(FILE_FILTER_BTN_PIC);
	}
	else if (pButton->IsEqualName(L"all"))
	{
		RefreshMode(FILE_FILTER_BTN_ALL);
	}	
	else
	{
		__super::OnBnClick(pButton);
	}

}

void CDVDFileListDlg::ShowPlayingDlg()
{
}

void CDVDFileListDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	ShowFileTip();
	if (m_nCmdStatus != DVP_OP_CMD_NONE || !bClickChecked)
	{
		return;
	}

	// get the file's information which clicked
// 	ASSERT(plist);
// 	int nIndex = plist->GetCurSel();
// 	ASSERT(nIndex>=0);
// 	plist->SetCurSel(nIndex);

	FILE_LIST_ITEM_DATA* pidSelect = (FILE_LIST_ITEM_DATA*)plist->GetItemData(nItemClicked);
	ASSERT(pidSelect);
	if (m_curType == m_nRefreshMode 
		&& _tcsicmp(m_szPlayFile, pidSelect->szFilePath) == 0)
	{
		m_bPlay_continue = TRUE;
	}
	else
	{
		m_bPlay_continue = FALSE;
	}

	int dvd_index = GetCurrentDVDIndex();
	if (pidSelect->nItemType == ID_TYPE_FOLDER)
	{		
		((CDVDFolderListBox*)plist)->ResetListContent();
		ShowStatus(TRUE);
		DVP_ChangeCurrentDir(dvd_index);
		m_nCmdStatus = DVP_OP_CMD_FL_CHG_DIR;

	}
	else if (pidSelect->nItemType == ID_TYPE_PARENT_FOLDER)
	{
		((CDVDFolderListBox*)plist)->ResetListContent();
		// show loading...
		ShowStatus(TRUE);
		DVP_GotoUpLevel();
		m_nCmdStatus = DVP_OP_CMD_FL_CHG_DIR;
	}
	else
	{
		m_curType = (E_FILE_FILTER_BTN)m_nRefreshMode;
		ASSERT(pidSelect->nItemType == ID_TYPE_FILE);

		DVP_FILEITEM_INFO_T dfi;
		DVP_GetDramFileItemInfo(dvd_index, &dfi);
		int type = CDVDFolderListBox::GetFilterByType(dfi.uType);

		if(!m_bPlay_continue || FILE_FILTER_BTN_PIC == type)
		{
			DVP_Stop();
			Sleep(400);
			ParamInit(type);
			GRESULT hr = DVP_ItemClick(dvd_index);
			m_nCmdStatus = DVP_OP_CMD_FL_CLICK_ITEM;
		}
		else
		{
			GetWindow()->PostMessage(YC_MESSAGE_LOAD_UI2, 0, type);
		}

		// 不让列表再接受鼠标消息,因为引起的重画会影响弹出的对话框输出(UI库BUG?)
		// 播放画面加载后，再恢复
		plist->EnableLayer(FALSE);

		ShowFileTip();
	}
}

int CDVDFileListDlg::GetCurrentDVDIndex()
{
	CWceUiListBox *pthis = (CWceUiListBox*)GetChildByName(L"listbox");

	// get the file's information which clicked
	ASSERT(pthis);
	int nIndex = pthis->GetCurSel();
	ASSERT(nIndex>=0);

	int dvd_index = nIndex;
	if ( !((CDVDFolderListBox*)pthis)->IsRootDir() )
	{
		dvd_index--;	// 将索引值映射到DVD的索引
		ASSERT(dvd_index >=0);
	}

	return dvd_index;
}


void CDVDFileListDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	int filter_type = 0;
	switch (media_event)
	{
	case EVT_DVP_FL_CHG_DIR:
		break;
	case EVT_DVP_FL_TOTAL_COUNT: //STOP状态会响应
		break;		
	case EVT_DVP_SAVE_ITEM_ADDRESS:
		break;
	case EVT_DVP_FL_ITEM:
		break;
	case EVT_DVP_CMD_RESPONSE:
		HandleResponse(param1, param2);
		break;
	case EVT_DVP_FL_ACK_RESULT:
		HandleFLACK(param1, param2);
		break;
	case EVT_DVP_PBC_TRACK_NUM:
	case EVT_DVP_PBC_PLAYING_AUDIO_IDX: //选中文件或者播放到下一曲响应,但是播放图片的同时后台播放音乐，只响应到图片
		if (m_curType == m_nRefreshMode) //当前播放中的模式与打开对话框显示的模式一样时，才设置当前播放文件的位置,并记录当前文件路径
		{	
			int nIdex = m_pListFolder->SetCurSelFile(param1);
			FILE_LIST_ITEM_DATA* pidCurrent = (FILE_LIST_ITEM_DATA*)m_pListFolder->GetItemData(nIdex);
			ASSERT(pidCurrent);
			_tcscpy(m_szPlayFile, pidCurrent->szFilePath); 
			ShowFileTip();
		}
		break;
	case EVT_DVP_LM_CLICK_ITEM_TYPE:        //程序第一次响应保存的断点值
 		filter_type = GetFilterByType(param1);		
 		GetWindow()->PostMessage(YC_MESSAGE_LOAD_UI2, 0, filter_type);
		break;
	case EVT_DVP_PBC_CUR_TIME:
		break;
	default:
		break;
	}

}



int CDVDFileListDlg::GetFilterByType(UINT uFileType)
{
	int filter_type = 0;

	switch (uFileType)
	{
	case FTYPE_MP1:
	case FTYPE_MP2:
	case FTYPE_MP3:
	case FTYPE_WMA:
	case FTYPE_ASF:
	case FTYPE_DST:
	case FTYPE_DSD:
	case FTYPE_AAC:
	case FTYPE_CDA:
	case FTYPE_DTS:
	case FTYPE_OGG:
		filter_type = FILE_FILTER_BTN_AUDIO;
		break;

	case FTYPE_MPG:
	case FTYPE_DAT:
	case FTYPE_VOB:
	case FTYPE_AVI:
	case FTYPE_TS1:
	case FTYPE_TS2:
	case FTYPE_TS4:
	case FTYPE_MP4:
	case FTYPE_3GP:
	case 0x20: /*FTYPE_OGM:*/
		filter_type = FILE_FILTER_BTN_VIDEO;
		break;

	case FTYPE_JPG:
		filter_type = FILE_FILTER_BTN_PIC;
		break;

	default:
		break;
	}

	return filter_type;
}

BOOL CDVDFileListDlg::HandleResponse(UINT param1, UINT param2)
{
	CDVDFolderListBox *plistbox = (CDVDFolderListBox*)GetChildByName(L"listbox");
	switch (param1)
	{
	case AP_CMD_FL_UP_LEVEL:
		if (param2 == ACK_OK)
		{
		}
		else
		{
			m_nCmdStatus = DVP_OP_CMD_NONE;
		}
		break;
	case AP_CMD_CREATE_LIST:
		if (param2 == ACK_OK)
		{
			// show loading...
			ShowStatus(TRUE);
			RefreshListBox(m_nRefreshMode);
			m_nCmdStatus = DVP_OP_CMD_FL_FILTER;
		}
		break;
	case AP_CMD_FL_CLICK_ITEM: //change dir OK after click item & up level
		if (param2 == ACK_OK)
		{
			if (m_nCmdStatus == DVP_OP_CMD_FL_CLICK_ITEM)
			{
				// 因为是消息驱动UI，执行DoModal()时，才重置此标志.注释掉该行
				// m_nCmdStatus = DVP_OP_CMD_NONE;		

				DVP_FILEITEM_INFO_T dfi;
				DVP_GetDramFileItemInfo(GetCurrentDVDIndex(), &dfi);
				int type = CDVDFolderListBox::GetFilterByType(dfi.uType);
				GetWindow()->PostMessage(YC_MESSAGE_LOAD_UI2, 0, type);
			}
		}
		else	// 失败时也要更新命令状态，否则状态就被锁住了，不能执行其它动作
		{
			if (m_nCmdStatus == DVP_OP_CMD_FL_CLICK_ITEM)
			{
				m_nCmdStatus = DVP_OP_CMD_NONE;
				CWceUiLayer *plist = GetChildByName(L"listbox");
				plist->EnableLayer(TRUE);
			}
		}
		break;
	default:
		break;
	}

	return TRUE;
}

void CDVDFileListDlg::RefreshMode(int nMode)
{
	if (nMode == m_nRefreshMode || m_nCmdStatus != DVP_OP_CMD_NONE)
	{
		return;
	}

	CDvdRender::GetInstance()->CreateFileDatabase(nMode);
	m_nRefreshMode = nMode;
}


BOOL CDVDFileListDlg::HandleFLACK(UINT param1, UINT param2)
{
	switch (param1)
	{
	case FILTER_RESULT:
		ShowStatus(FALSE);
		InitListbox();
		ShowFileTip();

		if (m_nCmdStatus == DVP_OP_CMD_FL_FILTER)
		{
			m_nCmdStatus = DVP_OP_CMD_NONE;
		}
		break;
	case CHG_DIR_RESULT:
		ShowStatus(FALSE);
		InitListbox();
		ShowFileTip();

		if (m_nCmdStatus == DVP_OP_CMD_FL_CHG_DIR)
		{
			m_nCmdStatus = DVP_OP_CMD_NONE;
		}
		break;
	case AUTO_UPDATE_DIR_REUSLT:
		ShowStatus(FALSE);
		InitListbox();
		ShowFileTip();
	break;
	default:
		break;
	}

	return TRUE;
}

void CDVDFileListDlg::ShowStatus(BOOL bShow)
{
	CWceUiAnimLayer* pLayer = (CWceUiAnimLayer*)GetChildByName(L"wait_cursor");
	if (pLayer)
	{
		if (bShow)
		{
			SetTimer(TIMER_ID_SHOW_WAIT_CUSOR, 500);	// 加载需要一些时间时,才真正显示出来
			pLayer->StartAnim(200);
		}
		else
		{
			KillTimer(TIMER_ID_SHOW_WAIT_CUSOR);
			pLayer->ShowLayer(FALSE);
			pLayer->StopAnim();
		}
	}

}

void CDVDFileListDlg::InitListbox()
{
	m_pListFolder->Init();
	if (m_pListFolder->GetCount()<=0) //无此类型文件提示
	{
// 		PCWceUiLayer pLayer = GetChildByName(L"status");
// 		ASSERT(pLayer);
// 		pLayer->SetTextResID(L"DVD_NOFILE");
// 		pLayer->ShowLayer(TRUE);
	}
}


void CDVDFileListDlg::ParamInit(int type)
{
	switch (type)
	{
	case FILE_FILTER_BTN_AUDIO:
		DVP_SetAudioPlayingIdx(INVALID_GUINT16);
		DVP_SetLastAudioPlayingIdx(INVALID_GUINT16);
		DVP_SetAudioPlayingDirIdx(INVALID_GUINT16);
		DVP_ClearId3ExistFlag();
		DVP_ClearAudioId3Txt();
		DVP_ClrCurFilePath();
		DVP_SetPBState(INVALID_GUINT8);
		DVP_SetDuration(0);
		break;

	case FILE_FILTER_BTN_VIDEO:
		DVP_SetPlayingIdx(INVALID_GUINT16);
		DVP_SetLastPlayingIdx(INVALID_GUINT16);
		DVP_SetPlayingDirIdx(INVALID_GUINT16);
		DVP_ClrCurFilePath();
		DVP_SetPBState(INVALID_GUINT8);
		DVP_SetAudioChannelCount(INVALID_GUINT8);
		DVP_SetSubtitleCount(INVALID_GUINT8);
		DVP_SetDuration(0);
		break;

	case FILE_FILTER_BTN_PIC:
		DVP_SetPlayingIdx(INVALID_GUINT16);
		DVP_SetLastPlayingIdx(INVALID_GUINT16);
		DVP_SetPlayingDirIdx(INVALID_GUINT16);
		DVP_ClrCurFilePath();
		DVP_SetPBState(INVALID_GUINT8);
		DVP_SetJPGResolution(INVALID_GUINT16, INVALID_GUINT16);
		break;

	default:
		break;
	}
}


void CDVDFileListDlg::RefreshListBox(int nMode)
{
	// check the buttons
	CWceUiButton *pbuttons[DVD_MODE_COUNT] = {NULL,};
	CString str[DVD_MODE_COUNT] = {L"all", L"music", L"video", L"photo"};
	for (int i=0; i<DVD_MODE_COUNT; i++)
	{
		pbuttons[i] = (CWceUiButton*)GetChildByName(str[i]);
		pbuttons[i]->SetStatusStyleName(str[i]+L"_normal", CWceUiButton::STATUS_NORMAL);
	}
	pbuttons[nMode]->SetStatusStyleName(str[nMode] + L"_down", CWceUiButton::STATUS_NORMAL);

	// 清空文件列表
	if (m_pListFolder)
	{
		m_pListFolder->ResetListContent();
	}
}

void CDVDFileListDlg::ShowFileTip()
{
	int cur = m_pListFolder->GetCurSelFile();
	int total = m_pListFolder->GetFileCount();
	TCHAR buffer[10] = L"";

	if (total<=0)
	{
		_tcscpy(buffer, L"_/_");
	}
	else if (cur<0)
	{
		_stprintf(buffer, TEXT("_/%d"), total); 
	}
	else
	{
		_stprintf(buffer, TEXT("%d/%d"), (cur+1), total); 
	}

	CWceUiLayer *ptip = GetChildByName(L"file_tip");
	if (ptip)
	{
		ptip->SetText(buffer);
	}
}
