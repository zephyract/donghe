#include "StdAfx.h"
#include "DVDAudioPlayerDlg.h"
#include "dvdrender.h"
#include "interface.h"
#include "sysUtil.h"
#include "config.h"
#include "WceUi32Bitmap.h"
#include "WceUiLoadString.h"
#include "protocol.h"
#include <imaging.h> 
#include "OverlayDlg.h"
#include "RpcMCU.h"

void CDVDAudioPlayerDlg::DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData)
{
	CDVDAudioPlayerDlg* pthis = (CDVDAudioPlayerDlg*)u4CustomData;
	pthis->OnMediaEvent(u4Evt, u4Param1, u4Param2);
}

CDVDAudioPlayerDlg::CDVDAudioPlayerDlg(void)
{
	CDvdRender::GetInstance()->RegisterEventHandler(CDVDAudioPlayerDlg::DvpAgentEventListener, (DWORD)this);

	m_bRandomClicked = FALSE;
	m_bRepeatClicked = FALSE;
}

CDVDAudioPlayerDlg::~CDVDAudioPlayerDlg(void)
{
	CDvdRender::GetInstance()->UnRegisterEventHandler(CDVDAudioPlayerDlg::DvpAgentEventListener);

}

void CDVDAudioPlayerDlg::InitLayer()
{
	m_pLayerSpectrum = (CSpectrumLayer*)GetChildByName(L"spectrum");
	m_pLayerSpectrum->SetSrcMode(SRC_MODE_DVD);
	m_pLayerSpectrum->ShowLayer(FALSE);
	SetTimer(TIMER_ID_SPECTRUM, 150);

}

void CDVDAudioPlayerDlg::OnInitDialog()
{
	InitLayer();
	ShowFileTip();

	ShowAudioID3();
	ShowMp3Image();

	__super::OnInitDialog();
}


void CDVDAudioPlayerDlg::OnTimer(WORD wIDEvent)
{
	GUINT8 state = SV_STOP;
	CWceUiLayer *player;
	switch (wIDEvent)
	{
	case TIMER_ID_HIDE_INVALID_TIP:
		KillTimer(TIMER_ID_HIDE_INVALID_TIP);
		player = GetChildByName(L"invalid_tip");
		ASSERT(player);
		player->ShowLayer(FALSE);
		break;

	case TIMER_ID_SPECTRUM:
		m_pLayerSpectrum->InvalidateRect(NULL);
		break;	
	default:
		break;
	}

	__super::OnTimer(wIDEvent);
}


void CDVDAudioPlayerDlg::OnPosChange(UINT param1, UINT param2)
{
	UINT curDuration = param1 / 1000;
	UINT outDuration = CDvdRender::GetInstance()->GetDuration();

	// 进度条
	UpdataProgress(curDuration, outDuration);
	// 播放时间
	ShowMediaTimeInfo(curDuration, outDuration);
}

void CDVDAudioPlayerDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"progress") && status==SLIDER_MOUSE_UP)
	{
		UINT outDuration = CDvdRender::GetInstance()->GetDuration();
		UINT curDuration = pos * outDuration / 1000;
		CDvdRender::GetInstance()->Seek(curDuration);

		GetWindow()->IgnoreMsgBeforeTime();
	}
}


void CDVDAudioPlayerDlg::ShowInvalidIcon()
{
	CWceUiLayer *player = GetChildByName(L"invalid_tip");
	if (player)
	{
		player->ShowLayer(TRUE);
		SetTimer(TIMER_ID_HIDE_INVALID_TIP, 500);
	}
}


void CDVDAudioPlayerDlg::OnEvtPBCTime()
{
	GUINT64 outDuration = 0, curDuration = 0;
	DVP_GetDuration(&outDuration);
	// DVD stop状态后系统返回仍是当前的位置,逻辑上stop后应该回到开始位置
	if (SV_STOP != CDvdRender::GetInstance()->GetState())
	{
		DVP_GetCurrentPosition(&curDuration);
	}
	UpdataProgress((UINT)(curDuration/1000), (UINT)(outDuration/1000));			 
	ShowMediaTimeInfo((UINT)(curDuration/1000), (UINT)(outDuration/1000));

	// refresh LED
	GUINT16 u2CurIndex = (GUINT16)-1;
	DVP_GetAudioPlayingIdx(&u2CurIndex);
	ui_led_set_showdata(ui_led_media_data(u2CurIndex == (GUINT16)-1 ? 0 : u2CurIndex+1, curDuration/1000));
	CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
}

void CDVDAudioPlayerDlg::OnEvtCmdResponse(UINT param1, UINT param2)
{
	if (param2 == ACK_FAIL && param1 != AP_CMD_TP_COORDINATE)	// operation fail, show error tip.
	{
		ShowInvalidIcon();
		m_bRandomClicked = FALSE;
		m_bRepeatClicked = FALSE;
	}

	if (param1 == AP_CMD_PBC_SEEK && param2 == ACK_FAIL)
	{
		GUINT64 outDuration = 0, curDuration = 0;
		DVP_GetDuration(&outDuration);
		DVP_GetCurrentPosition(&curDuration);
		UpdataProgress((UINT)(curDuration/1000), (UINT)(outDuration/1000));			 
	}
}

void CDVDAudioPlayerDlg::OnEvtPlayState(UINT state)
{
	switch (state)
	{
	case SV_PLAY:
		m_pLayerPlay->ShowLayer(FALSE);
		m_pLayerPause->ShowLayer(TRUE);
		break;
	case SV_PAUSE:
	case SV_STOP:
		m_pLayerPlay->ShowLayer(TRUE);
		m_pLayerPause->ShowLayer(FALSE);
		if (state == SV_STOP)
		{
			OnEvtPBCTime();		//刷新下时间和进度条
		}

		if (state == SV_PAUSE)
		{
			ui_led_set_showdata(state == SV_PAUSE ? L" PAUS E  " : L"  STO P  ");
			CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
		}

		break;
	case SV_STOP_RESUME:
	case SV_STILL:
		break;
	case SV_FF:
		break;
	case SV_FR:
		break;
	default:
		break;
	}
}

void CDVDAudioPlayerDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	switch (media_event)
	{
	case EVT_DVP_CMD_RESPONSE:
		OnEvtCmdResponse(param1, param2);
		break;
	case EVT_DVP_PBC_CUR_TIME:
		OnEvtPBCTime();
		break;
	case EVT_DVP_PBC_PLAY_STATE:
		OnEvtPlayState(param1);
		break;
	case EVT_DVP_FL_TOTAL_COUNT:
		break;
	case EVT_DVP_RIP_TRK_LBA_LEN:	
		break;
	case EVT_DVP_PBC_TRACK_NUM:		// cur track/total track
	case EVT_DVP_PBC_PLAYING_AUDIO_IDX:
		ShowFileTip();
		break;
	case EVT_DVP_PBC_REPEAT_MODE:
	case EVT_DVP_PBC_USER_MODE:
		ShowPlayMode();
		break;
	case EVT_DVP_RIP_CUR_TRACK_OK:
		break;
	case EVT_CD_RIP_NOT_ENOUGH_SPACE:		
		RETAILMSG(TRUE, (L"[DVP_StartRip] EVT_CD_RIP_NOT_ENOUGH_SPACE ****************** \n") );
		break;	
	case EVT_DVP_RIP_ERROR:
		// show rip error
		RETAILMSG(TRUE, (L"[DVP_StartRip] EVT_DVP_RIP_ERROR ****************** \n") );		
		break;
	case EVT_DVP_PBC_ADO_ID3_TXT:
		if (param1 == ID3_ARTIST)	// 这个消息会发多次，但参数不一样（代表不同的tag），这里作个判断一次刷新所有
		{
			ShowAudioID3();
		}
		break;
	case EVT_DVP_PBC_ADO_ID3_PIC:
		ShowMp3Image();
		break;

// 	case WM_MMP_MEDIA_DISATTACHED:
// 		// show rip error, SD card plug out?
// 		break;
	default:
		break;
	}
}


void CDVDAudioPlayerDlg::OnBnClick(PCWceUiButton pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CDVDFileListDlg_ID, SOURCE_DVD);
	}
	else if (pButton->IsEqualName(L"next"))
	{
		CDvdRender::GetInstance()->Next();
	}
	else if (pButton->IsEqualName(L"prev"))
	{
		CDvdRender::GetInstance()->Previous();
	}
	else if (pButton->IsEqualName(L"play"))
	{
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
		DVD_Play();
	}
	else if (pButton->IsEqualName(L"pause"))
	{
		CDvdRender::GetInstance()->Pause();
	}
	else if (pButton->IsEqualName(L"repeat"))
	{
		CDvdRender::GetInstance()->SetPBRepeat();
		m_bRepeatClicked = TRUE;
	}
	else if (pButton->IsEqualName(L"random"))
	{
		CDvdRender::GetInstance()->SetPBRandom();
		m_bRandomClicked = TRUE;
	}
	else if (pButton->IsEqualName(L"audio"))
	{
		CDvdRender::GetInstance()->SwitchAudioChannel();
	}
	else
	{
		__super::OnBnClick(pButton);

	}

}



void CDVDAudioPlayerDlg::ShowPlayMode()
{
	// 保存之前的值,只有当前值与之前的值不同时,才认为是有效的event,因为底层会重复发送event,过滤掉不需要的数据
	static UCHAR s_random = 0xFF;
	static UCHAR s_repeat = 0xFF;
	UCHAR random = CDvdRender::GetInstance()->GetPBCUserMode();
	UCHAR repeat = CDvdRender::GetInstance()->GetRepeatMode();
	CWceUiLayer* pRandom = GetChildByName(L"random_tip");
	CWceUiLayer* pRepeat = GetChildByName(L"repeat_tip");
	CWceUiLoadString strRandom;
	CWceUiLoadString strRepeat;
	if (pRandom)
	{
		pRandom->SetStyle((random == SV_RANDOM || random == SV_SHUFFLE) ? L"random_tip_on" : L"random_tip_off");
		strRandom.SetTextResID((random == SV_RANDOM || random == SV_SHUFFLE) ? L"MIS_PLAY_RANDOM_ON" : L"MIS_PLAY_RANDOM_OFF");
	}

	if (pRepeat)
	{
		switch (repeat)
		{
		case SV_REPEAT_ALL:
			pRepeat->SetStyle(L"repeat_tip_all");
			strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_ALL");
			break;
		case SV_REPEAT_FOLDER:
			pRepeat->SetStyle(L"repeat_tip_folder");
			strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_FOLDER");
			break;
		case SV_REPEAT_TRACK:
			pRepeat->SetStyle(L"repeat_tip_single");
			strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_SINGLE");
			break;
		case SV_REPEAT_NONE:
		default:
			pRepeat->SetStyle(L"repeat_tip_off");
			strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_OFF");
			break;
		}
	}

	if (m_bRandomClicked && random!=s_random)
	{
		COverlayDlg::GetInstance()->MessageTip_ShowUI(strRandom.GetString(), FALSE);
		m_bRandomClicked = FALSE;
	}

	if (m_bRepeatClicked && repeat!=s_repeat)
	{
		COverlayDlg::GetInstance()->MessageTip_ShowUI(strRepeat.GetString(), FALSE);
		m_bRepeatClicked = FALSE;
	}

	s_random = random;
	s_repeat = repeat;

	// refresh LED
	ui_led_set_playmode((random == SV_RANDOM || random == SV_SHUFFLE), (repeat != SV_REPEAT_NONE));
}


void CDVDAudioPlayerDlg::ShowFileTip()
{
	GUINT16 cur = 0;
	GUINT16 total = 0;

	TCHAR buffer[10] = L"_/_";
	CWceUiLayer *ptip = GetLayerByName(L"file_tip");
	if( RET_OK == DVP_GetAudioPlayingIdx(&cur) 
		&& RET_OK == DVP_GetAudioTrackCount(&total)
		&& cur != (GUINT16)-1 
		&& total != (GUINT16)-1)
	{
		_stprintf(buffer, TEXT("%d/%d"), (cur+1), total); 
	}
	else
	{
		;
	}

	if (ptip)
	{
		ptip->SetText(buffer);
	}
 
// 	GUINT16 cur = 0;
// 	GUINT16 total = 0;
// 
// 	DVP_GetAudioPlayingIdx(&cur);
// 	DVP_GetAudioTrackCount(&total);
// 
// 	if (total<0)
// 	{
// 		total = 0;	// 如果返回数据不合法,显示0
// 	}
// 
// 	TCHAR buffer[10] = L"";
// 	_stprintf(buffer, TEXT("%d/%d"), (cur+1), total); 
// 	CWceUiLayer *ptip = GetLayerByName(L"file_tip");
// 	if (ptip)
// 	{
// 		ptip->SetText(buffer);
// 	}
}


void CDVDAudioPlayerDlg::ShowAudioID3()
{
	CWceUiLayer *player_song = GetChildByName(L"song_text");
	CWceUiLayer *player_album = GetChildByName(L"album_text");
	CWceUiLayer *player_artist = GetChildByName(L"artist_text");


	GBOOL bID3Exist = FALSE;
	DVP_GetId3ExistFlag(&bID3Exist);
	if (!bID3Exist)
	{
		player_song->SetTextResID(L"MUSIC_UNKNOWN_TITLE");
		player_album->SetTextResID(L"MUSIC_UNKNOWN_ALBUM");
		player_artist->SetTextResID(L"MUSIC_UNKNOWN_ARTIST");
		return;
	}

	TCHAR szID3[MAX_ID3_SIZE] = L"";

	DVP_GetCurAudioId3Txt(ID3_TITLE, szID3, MAX_ID3_SIZE);
	if (!szID3[0] || !szID3[1] )	// MTK BUG
	{
		player_song->SetTextResID(L"MUSIC_UNKNOWN_TITLE");
	}
	else
	{
		player_song->SetText(szID3);
	}

	DVP_GetCurAudioId3Txt(ID3_ALBUM, szID3, MAX_ID3_SIZE);
	if (!szID3[0] || !szID3[1] )	// MTK BUG
	{
		player_album->SetTextResID(L"MUSIC_UNKNOWN_ALBUM");
	}
	else
	{
		player_album->SetText(szID3);
	}

	DVP_GetCurAudioId3Txt(ID3_ARTIST, szID3, MAX_ID3_SIZE);
	if (!szID3[0] || !szID3[1] )	// MTK BUG
	{
		player_artist->SetTextResID(L"MUSIC_UNKNOWN_ARTIST");
	}
	else
	{
		player_artist->SetText(szID3);
	}

}



void CDVDAudioPlayerDlg::ShowMp3Image()
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

	// get mp3 image data
	BOOL bRet = FALSE;
	CWceUiLayer *pLayerImage = GetChildByName(L"artist_image");
	ASSERT(pLayerImage);

	BYTE *pImageBuffer = NULL;
	GUINT32 nImageSize = 0;
	if ( RET_OK != DVP_GetId3Pic(&pImageBuffer, &nImageSize)
		|| nImageSize <= 0 )
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

	IImage *pImage = NULL;
	IImagingFactory *pImageFactory = NULL;

	// Create IPicture and render 
	LPVOID pvData = NULL;
	IStream *pStream = NULL; 
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nImageSize); 
	if (hGlobal == NULL)
	{
		goto EXIT;
	}

	if ((pvData = GlobalLock(hGlobal)) == NULL)
	{
		goto EXIT;
	}

	memcpy(pvData, pImageBuffer, nImageSize);
	GlobalUnlock(hGlobal);


	if (!SUCCEEDED(CreateStreamOnHGlobal(hGlobal, FALSE, &pStream)))
	{
		goto EXIT;
	}

	if( !SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (LPVOID*)&pImageFactory)))
	{
		goto EXIT;
	}

	if (!SUCCEEDED(pImageFactory->CreateImageFromStream(pStream, &pImage)))
	{
		goto EXIT;
	}

	if (!SUCCEEDED(pImage->Draw(dc.GetSafeHdc(), rc, NULL)))
	{
		goto EXIT;
	}
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

	// release resources
	if(pImage)
	{
		pImage->Release();
		pImage = NULL;
	}
	if (pImageFactory)
	{
		pImageFactory->Release();
		pImageFactory = NULL;
	}

	if (pStream)
	{
		pStream->Release();
		pStream = NULL;
	}

	if (hGlobal)
	{
		GlobalFree(hGlobal);
		hGlobal = NULL;
	}

	return;
}

LRESULT CDVDAudioPlayerDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == NAVI_MSG_LAN_CHANGE)
	{
		InitCodepage();
		ShowAudioID3();
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CDVDAudioPlayerDlg::InitCodepage()
{
	int lan_id = sysutil::nss_get_instance()->ui_lan_id;
	LANGUAGE_TABLE_INFO* pinfo = config::get_language_info(lan_id);
	if (pinfo)
	{
		DVP_SetCodePage(pinfo->code_page);
	}
}

void CDVDAudioPlayerDlg::DVD_Play()
{
	if (SV_STOP == CDvdRender::GetInstance()->GetState())
	{
		GUINT16 cur;
		DVP_GetAudioPlayingIdx(&cur);
		DVP_ItemClick(cur);
	}
	else
	{
		CDvdRender::GetInstance()->Play();
	}
}

void CDVDAudioPlayerDlg::OnKeyPlayControl(UINT key, UINT param)
{
	BYTE byte0 = (BYTE)((param>>24) & 0xFF);

	switch (key)
	{
	case T_PREV:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PREV");
		CDvdRender::GetInstance()->Previous();
		break;
	case T_NEXT:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_NEXT");
		CDvdRender::GetInstance()->Next();
		break;
	case T_PLAY:	// param byte0 : 0 ~ 播放暂停复合键, 1 ~ 播放， 2 ~ 暂停
		if (byte0 == 1)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
			DVD_Play();
		}
		else if (byte0 == 2)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
			CDvdRender::GetInstance()->Pause();
		}
		else
		{
			if( SV_PLAY == CDvdRender::GetInstance()->GetState())
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
				CDvdRender::GetInstance()->Pause();
			}
			else
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
				DVD_Play();
			}
		}
		break;
	case T_STOP:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_STOP");
		CDvdRender::GetInstance()->Stop();
		break;
	case T_IR_GOTO:

		{
			GUINT16 cur = (GUINT16)-1;
			if( RET_OK != DVP_GetAudioPlayingIdx(&cur) || (param-1) != cur)
			{
				DVP_ItemClick((GINT16)param-1);
			}
		}
		break;
	case T_REPEAT:
		CDvdRender::GetInstance()->SetPBRepeat();
		m_bRepeatClicked = TRUE;
		break;
	case T_SHUFFLE:
		CDvdRender::GetInstance()->SetPBRandom();
		m_bRandomClicked = TRUE;
		break;
	default:
		break;
	}
}