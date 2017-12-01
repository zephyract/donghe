#include "stdafx.h"
#include "PhotoPlayerDlg.h"
#include "WceUiPageLayer.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "msdkFileManager.h"
#include "PhotoShowLayer.h"
#include "GImaging.h"
#include "interface.h"
#include "tzUtil.h"
#include "protocol.h"
#include "WceUi32Bitmap.h"
#include "WceUiLoadString.h"
#include "OverlayDlg.h"

//#include "filefind.h"
//#include <imaging.h> 
//#include <initguid.h>
//#include <imgguids.h> 

CPhotoPlayerDlg::CPhotoPlayerDlg(void)
: m_nPlayIndex(-1)
, m_nExtraEndCount(0)
, m_nExtraFrontCount(0)
, m_pPhotoShowLayer(NULL)
{
	static BOOL bInit = FALSE;
	if (!bInit)
	{
		GInitImaging();
		bInit = TRUE;
	}
}

CPhotoPlayerDlg::~CPhotoPlayerDlg(void)
{

}

void CPhotoPlayerDlg::OnInitDialog()
{
	__super::OnInitDialog();
	RegisterMsg(UI_MSG_MCU_KEY_COMMAND);

	CWceUiPageLayer* pLayer = (CWceUiPageLayer*)GetChildByName(L"bottom_frame");
	pLayer->SetExtraScroll(30);
	pLayer->SetScrollRange(0, WceUiGetScreenWidth());

	m_pPhotoShowLayer = (CPhotoShowLayer*)GetChildByName(L"photo_show");
	ASSERT(m_pPhotoShowLayer);

	m_pPhotoShowLayer->StopPlaySlide();

	// 必须初始化，后台激活窗口时,不能执行到OnDlgShow()
	RebuildFileList();
	m_pPhotoShowLayer->InitFileList(&m_PlayFileList, m_nPlayIndex);
}

void CPhotoPlayerDlg::OnTimer(WORD wIDEvent)
{
	if(wIDEvent == TIMER_ID_HIDE_MESSAGE_TIP)
	{
		KillTimer(TIMER_ID_HIDE_MESSAGE_TIP);
		HideMessageTip();
	}
	else if(wIDEvent == TIMER_ID_HIDE_INVALID_TIP)
	{
		KillTimer(TIMER_ID_HIDE_INVALID_TIP);
		CWceUiLayer *player = GetChildByName(L"invalid_tip");
		if(player)
		{
			player->ShowLayer(FALSE);
		}
	}

	__super::OnTimer(wIDEvent);
}


LRESULT CPhotoPlayerDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_MCU_KEY_COMMAND)
	{
		int status = (wParam>>16) & 0xFF;
		int id = wParam & 0xFF;

		switch (id)
		{
		case T_ZOOM:
			{
				BOOL bZoom = m_pPhotoShowLayer->Zoom();
				ShowTipMessage(bZoom ? L"ZOOM IN" : L"ZOOM OUT");
				m_pPhotoShowLayer->StopPlaySlide();
			}
			
			break;
		default:
			break;
		}
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CPhotoPlayerDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CMMFileListDlg_ID, ui_msdk_get_cursrc());
	}
	else if (pButton->IsEqualName(L"play"))
	{
		m_pPhotoShowLayer->PlaySlide();
	}
	else if (pButton->IsEqualName(L"pause"))
	{
		m_pPhotoShowLayer->StopPlaySlide();
	}
	else if (pButton->IsEqualName(L"prev"))
	{
		m_pPhotoShowLayer->StopPlaySlide();
		m_pPhotoShowLayer->Prev();
	}
	else if (pButton->IsEqualName(L"next"))
	{
		m_pPhotoShowLayer->StopPlaySlide();
		m_pPhotoShowLayer->Next();
	}
	else if (pButton->IsEqualName(L"rotate"))
	{
		m_pPhotoShowLayer->StopPlaySlide();
		m_pPhotoShowLayer->Rotate();
	}
	else if (pButton->IsEqualName(L"zoomin"))
	{
		m_pPhotoShowLayer->StopPlaySlide();
		m_pPhotoShowLayer->ZoomIn();
	}
	else if (pButton->IsEqualName(L"zoomout"))
	{
		m_pPhotoShowLayer->StopPlaySlide();
		m_pPhotoShowLayer->ZoomOut();
	}
	else if (pButton->IsEqualName(L"repeat"))
	{
	}
	else if (pButton->IsEqualName(L"setbk"))
	{
		SaveBitmapToFile(USER_SKIN_PATH);
		::PostMessage(HWND_BROADCAST, NAVI_MSG_BK_CHANGE, 0, (LPARAM)NSS_UI_BK_INDEX_USER);

		CWceUiLoadString str(L"SET_WALLPAPER_SUCCESSFULL");
		ShowTipMessage(str.GetString());
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}

void CPhotoPlayerDlg::RebuildFileList()
{
	// 增加当前目录中的文件
	int nFolderIndex = CMsdkFileManager::GetInstance()->GetPlayFolderIndex(MSDK_FILE_PHOTO);
	const CStringArray& aFile = CMsdkFileManager::GetInstance()->GetBrowseFileData(nFolderIndex, MSDK_FILE_PHOTO);
	m_PlayFileList.Copy(aFile);
	m_nPlayIndex = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(MSDK_FILE_PHOTO);	// 当前文件索引

	m_nExtraFrontCount = 0;
	m_nExtraEndCount = 0;
	// 增加前二个文件
	int nPrevFolderIndex = nFolderIndex;
	if (nPrevFolderIndex > 0)	// 前面还有目录
	{
		nPrevFolderIndex--;		// 指向前一目录
		CMsdkFileManager::GetInstance()->GetBrowseFileData(nPrevFolderIndex, MSDK_FILE_PHOTO);
		int count = aFile.GetCount();
		if (count >= 2)	// 目录中有多于二个文件，直接追加最后的二个文件
		{
			m_PlayFileList.InsertAt(0, aFile.GetAt(count-1));
			m_PlayFileList.InsertAt(0, aFile.GetAt(count-2));

			m_nPlayIndex += 2;	// 当前文件索引跟着变化
			m_nExtraFrontCount += 2;
		}
		else
		{
			ASSERT(count == 1);		// 目录不可能为空，这种情况一定是只有一个文件
			m_PlayFileList.InsertAt(0, aFile.GetAt(count-1));
			m_nPlayIndex += 1;
			m_nExtraFrontCount++;

			// 只追加了一个，继续查找上一个目录的数据
			if (nPrevFolderIndex > 0)	// 前面还有目录
			{
				nPrevFolderIndex--;		// 指向前一目录
				CMsdkFileManager::GetInstance()->GetBrowseFileData(nPrevFolderIndex, MSDK_FILE_PHOTO);
				m_PlayFileList.InsertAt(0, aFile.GetAt(aFile.GetCount()-1));
				m_nPlayIndex += 1;
				m_nExtraFrontCount++;
			}
		}
	}

	// 增加后二个文件
	int nNextFolderIndex = nFolderIndex;
	const CStringArray& aFolder = CMsdkFileManager::GetInstance()->GetFolderData(MSDK_FILE_PHOTO);
	if (nNextFolderIndex < aFolder.GetCount()-1)	// 后面还有目录
	{
		nNextFolderIndex++;		// 指向后一目录
		CMsdkFileManager::GetInstance()->GetBrowseFileData(nNextFolderIndex, MSDK_FILE_PHOTO);
		int count = aFile.GetCount();
		if (count >= 2)	// 目录中有多于二个文件，直接追加最后的二个文件
		{
			m_PlayFileList.Add(aFile.GetAt(0));
			m_PlayFileList.Add(aFile.GetAt(1));
			m_nExtraEndCount += 2;
		}
		else
		{
			ASSERT(count == 1);		// 目录不可能为空，这种情况一定是只有一个文件
			m_PlayFileList.Add(aFile.GetAt(0));
			m_nExtraEndCount++;

			// 只追加了一个，继续查找下一个目录的数据
			if (nNextFolderIndex < aFolder.GetCount()-1)	// 后面还有目录
			{
				nNextFolderIndex++;		// 指向前一目录
				CMsdkFileManager::GetInstance()->GetBrowseFileData(nNextFolderIndex, MSDK_FILE_PHOTO);
				m_PlayFileList.Add(aFile.GetAt(0));
				m_nExtraEndCount++;
			}
		}
	}

	// 调用此接口会改变当前正在浏览的目录数据,所以追加完前后的数据后，重新加载当前目录的数据
	CMsdkFileManager::GetInstance()->GetBrowseFileData(nFolderIndex, MSDK_FILE_PHOTO);

}


LRESULT CPhotoPlayerDlg::OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam)
{
	if (uCommand == UI_CMD_PHOTOSHOW_SELCHANGE)
	{
		OnPlayFileChange((int)lParam);
		ShowFileTip();
	}
	else if (uCommand == UI_CMD_PHOTOSHOW_FULLSCREEN)
	{
		if (!IsAnimating())	// 动画过程中不响应此操作
		{
			SetFullScreen((BOOL)lParam);
		}
	}
	else if (uCommand == UI_CMD_PHOTOSHOW_PLAY_STATUS)
	{
		if(m_pLayerPlay)  m_pLayerPlay->ShowLayer(!lParam);
		if(m_pLayerPause) m_pLayerPause->ShowLayer(lParam);
	}
	else
	{
		return __super::OnCommand(uCommand, wParam, lParam);
	}

	return 0L;
}

void CPhotoPlayerDlg::OnPlayFileChange(int nIndex)
{
// 	if (nIndex<0)
// 	{
// 			// 设置正在播放的文件
// 			CMsdkFileManager::GetInstance()->SetPlayFileData(0, 0, MSDK_FILE_PHOTO);
// 
// 			m_nPlayIndex = 0;
// 			// 重建播放列表并播放
// 			RebuildFileList();	//  这里会修正m_nPlayIndex的值
// 			m_pPhotoShowLayer->InitFileList(&m_PlayFileList, m_nPlayIndex);
// 
// 		return;
// 	}
	int count = m_PlayFileList.GetCount();
	if (nIndex < 0 || nIndex > count -1)	// invalid parameters
	{
		return;
	}

	int nFileIndex = -1;
	int nFolderIndex = CMsdkFileManager::GetInstance()->GetPlayFolderIndex(MSDK_FILE_PHOTO);

	const CStringArray& aFolder = CMsdkFileManager::GetInstance()->GetFolderData(MSDK_FILE_PHOTO);

	// 当前文件及其目录名
	CString strFilePath = m_PlayFileList.GetAt(nIndex);
	CString strFileFolder;
	tzutil::GetFilePath(strFilePath, strFileFolder.GetBuffer(MAX_PATH));
	strFileFolder.ReleaseBuffer();

	int nDstFolderIndex = -1;
	int nDstFileIndex = -1;
	if (nIndex < m_nExtraFrontCount)	// 浏览到上一个目录的数据了
	{
		GetPrevFileInfo(nFolderIndex, nIndex, nDstFolderIndex, nDstFileIndex);
	}
	else if (nIndex > (m_PlayFileList.GetCount()-1) - m_nExtraEndCount)	// 浏览到下一个目录的数据了
	{
		GetNextFileInfo(nFolderIndex, nIndex, nDstFolderIndex, nDstFileIndex);
	}
	else
	{
		m_nPlayIndex = nIndex;
		CMsdkFileManager::GetInstance()->SetPlayFileData(nFolderIndex, nIndex-m_nExtraFrontCount, MSDK_FILE_PHOTO);

		return;	// 这种情况不需执行下面的重建列表代码
	}

	// 播放目录改变后，需重建列表 
	if (nDstFolderIndex>=0 && nDstFileIndex>=0)
	{
		// 设置正在播放的文件
		CMsdkFileManager::GetInstance()->SetPlayFileData(nDstFolderIndex, nDstFileIndex, MSDK_FILE_PHOTO);

		// 重建播放列表并播放
		RebuildFileList();	//  这里会修正m_nPlayIndex的值
		m_pPhotoShowLayer->InitFileList(&m_PlayFileList, m_nPlayIndex);
	}

}

BOOL CPhotoPlayerDlg::GetPrevFileInfo(int nFolderIndex, int nFileIndex, OUT int& nPrevFolderIndex, OUT int& nPrevFileIndex)
{
	const CStringArray& aFolder = CMsdkFileManager::GetInstance()->GetFolderData(MSDK_FILE_PHOTO);
	CString strFilePath = m_PlayFileList.GetAt(nFileIndex);
	CString strFileFolder;
	tzutil::GetFilePath(strFilePath, strFileFolder.GetBuffer(MAX_PATH));
	strFileFolder.ReleaseBuffer();

	BOOL bFoundFolder = FALSE;
	BOOL bFoundFile = FALSE;
	// 找到上一个目录
	nFolderIndex--;
	if (nFolderIndex >= 0 && strFileFolder.CompareNoCase(aFolder.GetAt(nFolderIndex)) == 0)
	{
		bFoundFolder = TRUE;
	}
	else 
	{
		nFolderIndex--;
		if (nFolderIndex >= 0 && strFileFolder.CompareNoCase(aFolder.GetAt(nFolderIndex)) == 0)
		{
			bFoundFolder = TRUE;
		}
	}

	ASSERT(bFoundFolder);
	if (bFoundFolder)
	{
		const CStringArray& aFile = CMsdkFileManager::GetInstance()->GetBrowseFileData(nFolderIndex, MSDK_FILE_PHOTO);
		for (int i=aFile.GetCount()-1; i>=0; i--)	// 从后往前找,找到播放的文件
		{
			if (strFilePath.CompareNoCase(aFile.GetAt(i)) == 0)
			{
				nPrevFolderIndex = nFolderIndex;
				nPrevFileIndex = i;
				bFoundFile = TRUE;
				break;
			}
		}
	}

	return (bFoundFile && bFoundFolder);
}


BOOL CPhotoPlayerDlg::GetNextFileInfo(int nFolderIndex, int nFileIndex, OUT int& nNextFolderIndex, OUT int& nNextFileIndex)
{
	const CStringArray& aFolder = CMsdkFileManager::GetInstance()->GetFolderData(MSDK_FILE_PHOTO);
	CString strFilePath = m_PlayFileList.GetAt(nFileIndex);
	CString strFileFolder;
	tzutil::GetFilePath(strFilePath, strFileFolder.GetBuffer(MAX_PATH));
	strFileFolder.ReleaseBuffer();

	BOOL bFoundFolder = FALSE;
	BOOL bFoundFile = FALSE;
	// 找到下一个目录
	nFolderIndex++;
	if (nFolderIndex < aFolder.GetCount() && strFileFolder.CompareNoCase(aFolder.GetAt(nFolderIndex)) == 0)
	{
		bFoundFolder = TRUE;
	}
	else 
	{
		nFolderIndex++;
		if (nFolderIndex < aFolder.GetCount() && strFileFolder.CompareNoCase(aFolder.GetAt(nFolderIndex)) == 0)
		{
			bFoundFolder = TRUE;
		}
	}

	ASSERT(bFoundFolder);
	if (bFoundFolder)
	{
		const CStringArray& aFile = CMsdkFileManager::GetInstance()->GetBrowseFileData(nFolderIndex, MSDK_FILE_PHOTO);
		for (int i=0; i<aFile.GetCount(); i++)	// 从前往后找,找到播放的文件
		{
			if (strFilePath.CompareNoCase(aFile.GetAt(i)) == 0)
			{
				nNextFolderIndex = nFolderIndex;
				nNextFileIndex = i;
				bFoundFile = TRUE;
				break;
			}
		}
	}

	return (bFoundFile && bFoundFolder);
}

void CPhotoPlayerDlg::OnDlgShow(BOOL bShow)
{
	__super::OnDlgShow(bShow);

	if (bShow)
	{
		RebuildFileList();
		m_pPhotoShowLayer->InitFileList(&m_PlayFileList, m_nPlayIndex);

		ShowFileTip();
	}
	else
	{
		m_pPhotoShowLayer->StopPlaySlide();
	}

	ShowVideo(bShow);
}


void CPhotoPlayerDlg::ShowFileTip()
{
	int cur = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(MSDK_FILE_PHOTO);
	int total = CMsdkFileManager::GetInstance()->GetPlayingFilesCount(MSDK_FILE_PHOTO);
	if (total<0)
	{
		total = 0;	// 如果返回数据不合法,显示0
	}

	TCHAR buffer[10] = L"";
	_stprintf(buffer, TEXT("%d/%d"), (cur+1), total); 
	CWceUiLayer *ptip = GetLayerByName(L"file_tip");
	if (ptip)
	{
		ptip->SetText(buffer);
	}
}


void CPhotoPlayerDlg::HideMessageTip()
{
	CWceUiLayer *player = GetLayerByName(L"message_tip");
	if (player)
	{
		player->ShowLayer(FALSE);
	}

	CWceUiLayer* pfiletip = GetLayerByName(L"file_tip");
	if (pfiletip)
	{
		pfiletip->ShowLayer(TRUE);	// 位置重叠了,临时隐藏下
	}
}

void CPhotoPlayerDlg::ShowTipMessage(LPCTSTR message)
{
// 	CWceUiLayer *player = GetLayerByName(L"message_tip");
// 	if (player)
// 	{
// 		player->SetText(message);
// 		player->ShowLayer(TRUE);
// 		SetTimer(TIMER_ID_HIDE_MESSAGE_TIP, 2000);
// 
// 		CWceUiLayer* pfiletip = GetLayerByName(L"file_tip");
// 		if (pfiletip)
// 		{
// 			pfiletip->ShowLayer(FALSE);	// 位置重叠了,临时隐藏下
// 		}
// 	}

	// 显示在overlay上 
	COverlayDlg::GetInstance()->MessageTip_ShowUI(message, FALSE, 1000);

}


void CPhotoPlayerDlg::OnKeyPlayControl(UINT key, UINT param)
{
	BYTE byte0 = (BYTE)((param>>24) & 0xFF);

	switch (key)
	{
	case T_PREV:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PREV");
		m_pPhotoShowLayer->Prev();
		m_pPhotoShowLayer->StopPlaySlide();
		break;
	case T_NEXT:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_NEXT");
		m_pPhotoShowLayer->Next();
		m_pPhotoShowLayer->StopPlaySlide();
		break;
	case T_PLAY:	// param byte0 : 0 ~ 播放暂停复合键, 1 ~ 播放， 2 ~ 暂停
		if (byte0 == 1)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
			m_pPhotoShowLayer->PlaySlide();
		}
		else if (byte0 == 2)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
			m_pPhotoShowLayer->StopPlaySlide();
		}
		else
		{
			if( m_pPhotoShowLayer->IsPlayingSlide())
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
				m_pPhotoShowLayer->StopPlaySlide();
			}
			else
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
				m_pPhotoShowLayer->PlaySlide();
			}
		}
		break;
	case T_STOP:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
		m_pPhotoShowLayer->StopPlaySlide();
		break;
	case T_IR_GOTO:
		if (CMsdkFileManager::GetInstance()->PlayFileGoto(param-1, MSDK_FILE_PHOTO))	//成功GOTO指定曲目
		{
			m_pPhotoShowLayer->PlayItem(param-1+m_nExtraFrontCount);
			m_pPhotoShowLayer->StopPlaySlide();
		}
		break;
	}
}


void CPhotoPlayerDlg::ShowVideo(BOOL bShow, RECT* prect)
{
	// 如果该窗口不可见，视频也不可见
	if (!IsVideoForbidden())
	{
		m_pPhotoShowLayer->ShowLayer(TRUE);
	}
	else
	{
		m_pPhotoShowLayer->ShowLayer(FALSE);
	}

}


BOOL CPhotoPlayerDlg::SaveBitmapToFile(LPCTSTR lpszFileName)
{
	GHANDLE pbmpImage = m_pPhotoShowLayer->GetCurGBitmap();
	if (pbmpImage != NULL)
	{
		CWceUi32Bitmap bmp;
		bmp.CreateDIB(WceUiGetScreenWidth(), WceUiGetScreenHeight());

		CRect rcDst(0, 0, WceUiGetScreenWidth(), WceUiGetScreenHeight());
		CDC dc;
		dc.CreateCompatibleDC(NULL);
		LPBYTE pbb = NULL;
		dc.SelectObject(bmp.GetHBitmap(pbb));
		GDrawBitmapDC(dc.GetSafeHdc(), pbmpImage, rcDst);

		BYTE* pbits = NULL;
		bmp.GetHBitmap(pbits);
		if (pbits)
		{
			for (int i=0; i<WceUiGetScreenHeight(); i++)
			{
				for (int j=0; j<WceUiGetScreenWidth(); j++)
				{
					*(pbits+(i*WceUiGetScreenWidth()+j)*4+3) = 0xFF;	// 有可能是透明图片,这里转为不透明
				}
			}
		}

		return bmp.SaveFile(lpszFileName);
	}

	return FALSE;
}

void CPhotoPlayerDlg::AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData)
{
	__super::AnimShowWindow(bShow, nAnimMode, dwAnimData);

	m_pPhotoShowLayer->SetFullScreen(m_bFullScreen);	// 同步下控件的这个变量
}

void CPhotoPlayerDlg::ShowInvalidIcon()
{
	CWceUiLayer *player = GetChildByName(L"invalid_tip");
	if (player)
	{
		player->ShowLayer(TRUE);
		SetTimer(TIMER_ID_HIDE_INVALID_TIP, 500);
	}
}
