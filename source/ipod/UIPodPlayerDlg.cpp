#include "stdafx.h"
#include "UIPodPlayerDlg.h"
#include "wceuipagelayer.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "MainWnd.h"
#include "uiddraw.h"
#include "sysUtil.h"
#include "config.h"
#include "tzUtil.h"
#include "WceUi32Bitmap.h"
#include "WceUiLoadString.h"
#include "interface.h"
#include "IPODManager.h"
#include "ycapi.h"
#include <imaging.h> 

// typedef struct tagBITMAPINFO_RGB565 {
// 	BITMAPINFOHEADER    bmiHeader;
// 	DWORD			arrBitMasks[3];
// } BITMAPINFO_RGB565, FAR *LPBITMAPINFO_RGB565, *PBITMAPINFO_RGB565;


CUIPodPlayerDlg::CUIPodPlayerDlg(void)
{
	m_FF_REW = FALSE;
}

CUIPodPlayerDlg::~CUIPodPlayerDlg(void)
{

}

void CUIPodPlayerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(UI_MSG_USB_IPOD_PB_POS_CHANGED);
	RegisterMsg(UI_MSG_USB_IPOD_PB_TRACK_CHANGED);
	RegisterMsg(UI_MSG_USB_IPOD_PB_STATUS_CHANGED);

	CWceUiPageLayer* pLayer = (CWceUiPageLayer*)GetChildByName(L"bottom_frame");
	if (pLayer)
	{
		pLayer->SetExtraScroll(30);
	}

	CWceUiSlider* pslider = (CWceUiSlider*)GetChildByName(L"progress");
	if (pslider)
	{
		pslider->SetReadonly(TRUE);
	}


	// Long press
	CWceUiButton* pButton = (CWceUiButton*)GetLayerByName(L"prev");
	if (pButton)
	{
		pButton->EnableLongPress(TRUE);
	}
	pButton = (CWceUiButton*)GetLayerByName(L"next");
	if (pButton)
	{
		pButton->EnableLongPress(TRUE);
	}

	//
	ShowMp3Tag();

	CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnGetShuffleSetting(&m_random);
	CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnGetRepeatSetting(&m_repeat);
	ShowPlayMode(m_random, m_repeat);

	ShowFileTip();

	SetTimer(TIMER_ID_SHOW_ARMWORK, 100);
}

LRESULT CUIPodPlayerDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_USB_IPOD_PB_POS_CHANGED)
	{
		MUSB_IPOD_PLAY_ST_INFO_T status;
		if (CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnGetPlayStatus(&status) == RET_IPOD_OK)
		{
			OnPosChange(lParam, status.u4TrackLen/1000);
		}
	}
	else if (message == UI_MSG_USB_IPOD_PB_STATUS_CHANGED)
	{
		OnStatusChange((MUSB_IPOD_PLAY_ST_EX_E)lParam);
	}
	else if (message == UI_MSG_USB_IPOD_PB_TRACK_CHANGED)
	{
		ShowMp3Tag();
		ShowMp3Image();

		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnGetShuffleSetting(&m_random);
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnGetRepeatSetting(&m_repeat);
		ShowPlayMode(m_random, m_repeat);

		ShowFileTip();
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CUIPodPlayerDlg::OnStatusChange(MUSB_IPOD_PLAY_ST_EX_E status)
{
	if (status == PB_ST_EX_PLAY_0X01)
	{
		m_pLayerPlay->ShowLayer(FALSE);
		m_pLayerPause->ShowLayer(TRUE);

		m_FF_REW = FALSE;
	}
	else //if (status == PB_ST_EX_PAUSE_0X02)
	{
		m_pLayerPlay->ShowLayer(TRUE);
		m_pLayerPause->ShowLayer(FALSE);

		if (status == PB_ST_EX_PF_0X03 || PB_ST_EX_FR_0X04)
		{
			m_FF_REW = TRUE;
		}
		else
		{
			m_FF_REW = FALSE;
		}
	}

	RETAILMSG(1, (L"========================  IPOD STATUS = %d \n", status));
}

void CUIPodPlayerDlg::OnPosChange(UINT param1, UINT param2)
{

	// 进度条
	UpdataProgress(param1, param2);
	// 播放时间
	ShowMediaTimeInfo(param1, param2);

}

void CUIPodPlayerDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_SHOW_ARMWORK)
	{
		KillTimer(TIMER_ID_SHOW_ARMWORK);
		ShowMp3Image();
	}
	else if (wIDEvent == TIMER_ID_HIDE_MESSAGE_TIP)
	{
		KillTimer(TIMER_ID_HIDE_MESSAGE_TIP);
		CWceUiLayer* player = GetLayerByName(L"message_tip");
		if (player)
		{
			player->ShowLayer(FALSE);
		}

		CWceUiLayer *ptip = GetChildByName(L"file_tip");
		if (ptip)
		{
			ptip->ShowLayer(TRUE);
		}

	}

	__super::OnTimer(wIDEvent);
}

void CUIPodPlayerDlg::OnDlgShow(BOOL bShow)
{

}


void CUIPodPlayerDlg::OnBnLongClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"prev"))
	{
		if (m_FF_REW)	// 如果在快进或快退,退出快进快退模式
		{
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_END_FF_REW);
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PLAY);
		}
		else
		{
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PAUSE);
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_START_REW);
			ShowMessageTip(L"PLAY_REWIND");
		}

	}
	else if (pButton->IsEqualName(L"next"))
	{
		if (m_FF_REW)	// 如果在快进或快退,退出快进快退模式
		{
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_END_FF_REW);
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PLAY);
		}
		else
		{
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PAUSE);
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_START_FF);
			ShowMessageTip(L"PLAY_FFWD");
		}
	}

	else
	{
		__super::OnBnLongClick(pButton);
	}

}

void CUIPodPlayerDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CUIPodFileListDlg_ID, SOURCE_IPOD_USB);
	}
	else if (pButton->IsEqualName(L"next"))
	{
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_NEXT);
	}
	else if (pButton->IsEqualName(L"prev"))
	{
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PRE);
	}
	else if (pButton->IsEqualName(L"play"))
	{
		if (m_FF_REW)	// 如果在快进或快退,退出快进快退模式
		{
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_END_FF_REW);
		}
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PLAY);
	}
	else if (pButton->IsEqualName(L"pause"))
	{
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PAUSE);
	}
	else if (pButton->IsEqualName(L"random"))
	{
		m_random = (m_random == 1) ? 0 : 1;		
		ShowPlayMode(m_random, 0xFF);
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnSetShuffleMode(m_random, true);
	}
	else if (pButton->IsEqualName(L"repeat"))
	{
		m_repeat++;
		m_repeat = (m_repeat > 2) ? 0 : m_repeat;
		ShowPlayMode(0xFF, m_repeat);
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnSetRepeatMode(m_repeat, true);
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}

void CUIPodPlayerDlg::ShowPlayMode(BYTE random, BYTE repeat)
{
	CWceUiLayer* pLayerRandom = GetChildByName(L"random_tip");
	CWceUiLayer* pLayerRepeat = GetChildByName(L"repeat_tip");

	if (pLayerRandom && random != 0xFF)
	{
		pLayerRandom->SetStyle(random ? L"random_tip_on" : L"random_tip_off");
	}

	if (pLayerRepeat && repeat != 0xFF)
	{
		LPCTSTR style[] = {L"repeat_tip_off", L"repeat_tip_single", L"repeat_tip_all"};
		if (repeat>2)
		{
			repeat = 2;
		}
		pLayerRepeat->SetStyle(style[repeat]);
	}
}

void CUIPodPlayerDlg::ShowMp3Tag()
{
	CWceUiLayer *pLayerSong = GetChildByName(L"song_text");
	CWceUiLayer *pLayerAlbum = GetChildByName(L"album_text");
	CWceUiLayer *pLayerArtist = GetChildByName(L"artist_text");


	CiPodSDKAgent* pSDK = CIPODManager::GetInstance()->GetIPodCtrl();
	UINT index;
	if (RET_IPOD_OK != pSDK->m_pfnGetCurPlayingTrackIdx(&index))
	{
		return;
	}

	// display the MP3 file tag information
	TCHAR title[IPOD_MAX_STR_LEN] = L"";
	TCHAR artist[IPOD_MAX_STR_LEN] = L"";
	TCHAR album[IPOD_MAX_STR_LEN] = L"";

	pSDK->m_pfnGetIndexedPlayingTrackTitle(index, title);
	pSDK->m_pfnGetIndexedPlayingTrackArtist(index, artist);
	pSDK->m_pfnGetIndexedPlayingTrackAlbum(index, album);

	if (title[0] != L'\0')
	{
		pLayerSong->SetText(title);
	}
	else
	{
		pLayerSong->SetTextResID(L"MUSIC_UNKNOWN_TITLE");
	}

	if (album[0] != L'\0')
	{
		pLayerAlbum->SetText(album);		
	}
	else
	{
		pLayerAlbum->SetTextResID(L"MUSIC_UNKNOWN_ALBUM");
	}

	if (artist[0] != L'\0')
	{
		pLayerArtist->SetText(artist);
	}
	else
	{
		pLayerArtist->SetTextResID(L"MUSIC_UNKNOWN_ARTIST");
	}


}


void CUIPodPlayerDlg::ShowMp3Image()
{
	LPCTSTR artist_image_resid = NULL;
	if (WceUiGetBitsPixel() == 16)
	{
		artist_image_resid = L"music\\artist_image.565a.conf";
	}
	else
	{
		ASSERT(WceUiGetBitsPixel() == 32);
		artist_image_resid = L"music\\artist_image.32a.conf";
	}

	CiPodSDKAgent* pSDK = CIPODManager::GetInstance()->GetIPodCtrl();
	UINT index;
	if (RET_IPOD_OK != pSDK->m_pfnGetCurPlayingTrackIdx(&index))
	{
		return;
	}

	// get mp3 image data
	BOOL bRet = FALSE;
	CWceUiLayer *pLayerImage = GetChildByName(L"artist_image");
	ASSERT(pLayerImage);

	UIPOD_ARTWORKDATA artwork;
	memset(&artwork, 0, sizeof(UIPOD_ARTWORKDATA));
	if ( !CIPODManager::GetInstance()->GetTrackArtWork(index, &artwork) 
		|| artwork.u4ImageLen<=0)
	{
		CWceUiStyle *pstyle = pLayerImage->GetStyle();
		pstyle->GetBgItemCtrl()->GetFirst()->SetBitmapFromRes(artist_image_resid);
		pLayerImage->InvalidateRect(NULL);
		return;
	}

	// create CWceUiGenericBitmap
	CRect rc;
	pLayerImage->GetClientRect(rc);
	CWceUiGenericBitmap *pbmp = new CWceUi32Bitmap();
	pbmp->CreateDIB(rc.Width(), rc.Height());
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	LPBYTE pbb;
	HBITMAP oldbmp = (HBITMAP)::SelectObject(dc.GetSafeHdc(), pbmp->GetHBitmap(pbb));


	BITMAPINFO_RGB565 bmpInfo;
	bmpInfo.bmiHeader.biWidth = artwork.u4PicWith;  
	bmpInfo.bmiHeader.biHeight = -artwork.u4PicHeight;  
	bmpInfo.bmiHeader.biSizeImage = (artwork.u4PicWith * artwork.u4PicHeight * 2);  
	StretchDIBits(dc.GetSafeHdc(), 0,0, rc.Width(), rc.Height(), 0, 0, artwork.u4PicWith, artwork.u4PicHeight, artwork.pImageData, (const BITMAPINFO *)&bmpInfo, DIB_RGB_COLORS, SRCCOPY);
	::SelectObject(dc.GetSafeHdc(), oldbmp);
	bRet = TRUE;

EXIT:
	if (bRet)
	{
		CWceUiStyle *pstyle = pLayerImage->GetStyle();
		pstyle->GetBgItemCtrl()->GetFirst()->SetBitmapFromApp(pbmp);
		pLayerImage->InvalidateRect(NULL);
	}
	else
	{
		CWceUiStyle *pstyle = pLayerImage->GetStyle();
		pstyle->GetBgItemCtrl()->GetFirst()->SetBitmapFromRes(artist_image_resid);
		pLayerImage->InvalidateRect(NULL);
	}

	if (pbmp)
	{
		dc.SelectObject(oldbmp);
		delete pbmp;
		pbmp = NULL;
	}

	return;
}


void CUIPodPlayerDlg::ShowFileTip()
{
	UINT cur = 0;
	DWORD total = 0;
	CIPODManager::GetInstance()->GetIPodCtrl()-> m_pfnGetCurPlayingTrackIdx(&cur);
	CIPODManager::GetInstance()->GetIPodCtrl()-> m_pfnGetNumPlayingTracks(&total);

	TCHAR buffer[10] = L"";
	_stprintf(buffer, TEXT("%d/%d"), (cur+1), total); 
	CWceUiLayer *ptip = GetChildByName(L"file_tip");
	if (ptip)
	{
		ptip->SetText(buffer);
	}
}


void CUIPodPlayerDlg::OnKeyPlayControl(UINT key, UINT param)
{
	BYTE byte0 = (BYTE)((param>>24) & 0xFF);

	switch (key)
	{
	case T_PREV:
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PRE);
		break;
	case T_NEXT:
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_NEXT);
		break;
	case T_PLAY:	// param byte0 : 0 ~ 播放暂停复合键, 1 ~ 播放， 2 ~ 暂停
		if (byte0 == 1)
		{
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PLAY);
		}
		else if (byte0 == 2)
		{
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PAUSE);
		}
		else
		{
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PLAY_PAUSE);
		}
		break;
	case T_STOP:
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_STOP);
		break;
	}
}

void CUIPodPlayerDlg::ShowMessageTip(LPCTSTR tip)
{
	CWceUiLayer* player = GetLayerByName(L"message_tip");
	if (player && tip)
	{
		CWceUiLoadString str(tip);
		player->ShowLayer(TRUE);
		player->SetText(str.GetString());

		CWceUiLayer *ptip = GetChildByName(L"file_tip");
		if (ptip)
		{
			ptip->ShowLayer(FALSE);
		}

		SetTimer(TIMER_ID_HIDE_MESSAGE_TIP, 2000);
	}
}
