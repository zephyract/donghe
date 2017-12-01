#include "StdAfx.h"
#include "CDGPlayerDlg.h"
#include "WceUiButton.h"
#include "WceUiRealWnd.h"
#include <atlbase.h>
#include "dvdrender.h"
#include "uiddraw.h"
#include "WceUiPageLayer.h"
#include "MainWnd.h"
#include "WceUiLoadString.h"
#include "interface.h"

void CCDGPlayerDlg::DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData)
{
	CCDGPlayerDlg* pthis = (CCDGPlayerDlg*)u4CustomData;
	pthis->OnMediaEvent(u4Evt, u4Param1, u4Param2);
}

CCDGPlayerDlg::CCDGPlayerDlg(void)
{
	CDvdRender::GetInstance()->RegisterEventHandler(CCDGPlayerDlg::DvpAgentEventListener, (DWORD)this);
	m_bAudioBtnClicked = TRUE;
}

CCDGPlayerDlg::~CCDGPlayerDlg(void)
{
	CDvdRender::GetInstance()->UnRegisterEventHandler(CCDGPlayerDlg::DvpAgentEventListener);
}

void CCDGPlayerDlg::InitLayer()
{
	CWceUiPageLayer* pLayer = (CWceUiPageLayer*)GetChildByName(L"bottom_frame");
	ASSERT(pLayer);
	if (pLayer)
	{
		pLayer->SetExtraScroll(30);
		pLayer->SetScrollRange(0, 800);
	}

	GUINT64 u8Duration = 0;
	GUINT8 u1State;
	DVP_GetDuration(&u8Duration);
	DVP_GetPBState(&u1State);
	if ((u8Duration == 0) || (u1State == SV_STOP))
	{
//		m_pProgressSlider->EnableLayer(FALSE);
	}
}

void CCDGPlayerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(NAVI_MSG_LAN_CHANGE);
	InitLayer();


	SetTimer(TIMER_ID_CHECK_MENU, 1000);


	// DVD 左边剪裁掉8个像素
	DVD_SCALE_INFO_T scale_info;
	scale_info.eSinkType = DVD_SINK_FRONT;
	::SetRect(&scale_info.rRect, 8, 6, 712, 480);
	scale_info.fgNotScale = FALSE;
	DVP_SetScaleInfo(&scale_info);
}

void CCDGPlayerDlg::OnTimer(WORD wIDEvent)
{
	switch (wIDEvent)
	{
	case TIMER_ID_HIDE_INVALID_TIP:
		{
			KillTimer(TIMER_ID_HIDE_INVALID_TIP);
			CWceUiLayer *player = GetChildByName(L"invalid_tip");
			ASSERT(player);
			player->ShowLayer(FALSE);
		}
		break;

	case TIMER_ID_HIDE_MESSAGE_TIP:
		{
			KillTimer(TIMER_ID_HIDE_MESSAGE_TIP);
			CWceUiLayer *player = GetLayerByName(L"message_tip");
			if (player)
			{
				player->ShowLayer(FALSE);
			}
		}
		break;
	default:
		break;
	}

	__super::OnTimer(wIDEvent);
}

void CCDGPlayerDlg::OnAutoFullscreen()
{
	// 只有在播放状态，并且鼠标未被按下的情况下才自动全屏播放
	if (!m_bFullScreen && GetAsyncKeyState(VK_LBUTTON) >= 0)
	{
		UCHAR state;
		DVP_GetPBState(&state);
		if (state == SV_PLAY)
		{
			SetFullScreen(TRUE);
		}
	}
}

void CCDGPlayerDlg::OnBnClick(PCWceUiButton pButton)
{
	if (pButton->IsEqualName(L"next"))
	{
		CDvdRender::GetInstance()->Next();
	}
	else if (pButton->IsEqualName(L"prev"))
	{
		CDvdRender::GetInstance()->Previous();
	}
	else if (pButton->IsEqualName(L"play"))
	{
		CDvdRender::GetInstance()->Play();
	}
	else if (pButton->IsEqualName(L"pause"))
	{
		CDvdRender::GetInstance()->Pause();
	}
	else if (pButton->IsEqualName(L"repeat"))
	{
		CDvdRender::GetInstance()->SetPBRepeat();
	}
	else if (pButton->IsEqualName(L"audio"))
	{
		CDvdRender::GetInstance()->SwitchAudioChannel();
		m_bAudioBtnClicked = TRUE;
	}
	else
	{
		__super::OnBnClick(pButton);

	}

}


LRESULT CCDGPlayerDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == NAVI_MSG_LAN_CHANGE)
	{
		ShowTitleChapter();
	}

	return __super::OnMessage(message, wParam, lParam);
}


void CCDGPlayerDlg::OnLButtonDown(UINT nFlags, POINT point)
{
	SetFullScreen(!m_bFullScreen);
}


void CCDGPlayerDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
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
	case EVT_DVP_PBC_PLAYING_IDX:
	case EVT_DVP_PBC_TRACK_NUM:
		ShowTitleChapter();
		break;
	case EVT_DVP_PBC_AUDIO:
		ShowAudio();
		break;
	case EVT_DVP_PBC_REPEAT_MODE:	
		ShowPlayMode();
		break;
	case EVT_DVP_PBC_USER_MODE:
		ShowPlayMode();
		break;
	case EVT_DVP_PBC_PBC:
		break;
	default:
		break;
	}
}

void CCDGPlayerDlg::OnEvtPBCTime()
{
	GUINT64 outDuration = 0, curDuration = 0;
	DVP_GetDuration(&outDuration);
	DVP_GetCurrentPosition(&curDuration);
	UpdataProgress((UINT)(curDuration/1000), (UINT)(outDuration/1000));			 
	ShowMediaTimeInfo((UINT)(curDuration/1000), (UINT)(outDuration/1000));
}

void CCDGPlayerDlg::OnEvtCmdResponse(UINT param1, UINT param2)
{
	if (param2 == ACK_FAIL && param1 != AP_CMD_TP_COORDINATE)	// operation fail, show error tip.
	{
		ShowInvalidIcon();
		m_bAudioBtnClicked = FALSE;
	}

	if (param1 == AP_CMD_PBC_SEEK && param2 == ACK_FAIL)
	{
		GUINT64 outDuration = 0, curDuration = 0;
		DVP_GetDuration(&outDuration);
		DVP_GetCurrentPosition(&curDuration);
		UpdataProgress((UINT)(curDuration/1000), (UINT)(outDuration/1000));			 
	}
}

void CCDGPlayerDlg::OnEvtPlayState(UINT state)
{
	switch (state)
	{
	case SV_PLAY:
		m_pLayerPlay->ShowLayer(FALSE);
		m_pLayerPause->ShowLayer(TRUE);
		break;
	case SV_PAUSE:
		m_pLayerPlay->ShowLayer(TRUE);
		m_pLayerPause->ShowLayer(FALSE);
		break;
	case SV_STOP:
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

void CCDGPlayerDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"progress"))
	{
		UINT outDuration = CDvdRender::GetInstance()->GetDuration();
		UINT curDuration = pos * outDuration / 1000;
		CDvdRender::GetInstance()->Seek(curDuration);
	}


}

void CCDGPlayerDlg::ShowInvalidIcon()
{
	CWceUiLayer *player = GetChildByName(L"invalid_tip");
	if (player)
	{
		player->ShowLayer(TRUE);
		SetTimer(TIMER_ID_HIDE_INVALID_TIP, 500);
	}
}


void CCDGPlayerDlg::OnDlgShow(BOOL bShow)
{
	if (!IsAnimating())	// 如果没有动画效果时,直接show or hide
	{
		// 如果整个窗口不可见(比如导航在前台时)
		if (bShow && !CMainWnd::GetInstance()->IsForegroundWindow())
		{
			return;
		}

		CUiDDraw::GetInstance()->UpdateOverlay(bShow, 180);
		ShowVideo(bShow);
	}
	// 有动画时，动画过程会管理显示或隐藏

	__super::OnDlgShow(bShow);
}

void CCDGPlayerDlg::ShowVideo(BOOL bShow, RECT* prect)
{
	// 如果该窗口不可见，视频也不可见
	if (IsLayerVisible() 
		&& CMainWnd::GetInstance()->IsForegroundWindow()
		&& !IsVideoForbidden())
	{
		CDvdRender::GetInstance()->ShowVideo(bShow, prect);
	}
	else
	{
		CDvdRender::GetInstance()->ShowVideo(FALSE, prect);
	}

}


void CCDGPlayerDlg::OnAnimateBegin()
{
	if (m_bAnimShow)
	{
		CUiDDraw::GetInstance()->UpdateOverlay(TRUE, 180);
	}

	__super::OnAnimateBegin();
}


void CCDGPlayerDlg::OnAnimateEnd()
{
	if (!m_bAnimShow)	// hide窗口时
	{
		ShowVideo(FALSE, NULL);
		CUiDDraw::GetInstance()->UpdateOverlay(FALSE, 180);
	}

	__super::OnAnimateEnd();
}

void CCDGPlayerDlg::OnAnimating()
{
	float param[4];
	AnimGetParameters(param, m_nCurrentFrame, FRAME_ANMI_SHOWWINDOW);

	int width = (int)(param[0] * WceUiGetScreenWidth());
	int height = (int)(param[1] * WceUiGetScreenHeight());
	int centerX = WceUiGetScreenWidth()/2 + (int)param[2];
	int centerY = WceUiGetScreenHeight()/2 + (int)(-param[3]);
	int x = centerX-width/2;
	int y = centerY-height/2;

	CRect rc;
	rc.SetRect(x, y, x+width, y+height);
	ShowVideo(TRUE, &rc);
}

void CCDGPlayerDlg::AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData)
{
	// CDVDStartDlg - > CCDGPlayerDlg  不作动画
	if (nAnimMode == ANIM_MODE_UPLEVEL)
	{
		ShowLayer(bShow);
	}
	else
	{
		__super::AnimShowWindow(bShow, nAnimMode, dwAnimData);
	}
}


void CCDGPlayerDlg::ShowPlayMode()
{
	UCHAR random = CDvdRender::GetInstance()->GetPBCUserMode();
	UCHAR repeat = CDvdRender::GetInstance()->GetRepeatMode();
	CWceUiLayer* pRandom = GetChildByName(L"random_tip");
	CWceUiLayer* pRepeat = GetChildByName(L"repeat_tip");
	if (pRandom)
	{
		pRandom->SetStyle((random == SV_RANDOM || random == SV_SHUFFLE) ? L"random_tip_on" : L"random_tip_off");
	}

	if (pRepeat)
	{
		LPCTSTR style[] = {L"repeat_tip_off", L"repeat_tip_single", L"repeat_tip_folder", L"repeat_tip_all"};
		switch (repeat)
		{
		case SV_REPEAT_ALL:
			pRepeat->SetStyle(L"repeat_tip_all");
			break;
		case SV_REPEAT_TITLE:
			pRepeat->SetStyle(L"repeat_tip_tt");
			break;
		case SV_REPEAT_CHAPTER:
			pRepeat->SetStyle(L"repeat_tip_ch");
			break;
		case SV_REPEAT_NONE:
		default:
			pRepeat->SetStyle(L"repeat_tip_off");
			break;
		}
	}
}


void CCDGPlayerDlg::OnKeyPlayControl(UINT key, UINT param)
{
	BYTE byte0 = (BYTE)((param>>24) & 0xFF);

	switch (key)
	{
	case T_PREV:
		CDvdRender::GetInstance()->Previous();
		break;
	case T_NEXT:
		CDvdRender::GetInstance()->Next();
		break;
	case T_PLAY:	// param byte0 : 0 ~ 播放暂停复合键, 1 ~ 播放， 2 ~ 暂停
		if (byte0 == 1)
		{
			CDvdRender::GetInstance()->Play();
		}
		else if (byte0 == 2)
		{
			CDvdRender::GetInstance()->Pause();
		}
		else
		{
			if( SV_PLAY == CDvdRender::GetInstance()->GetState())
				CDvdRender::GetInstance()->Pause();
			else
				CDvdRender::GetInstance()->Play();
		}
		break;
	case T_STOP:
		CDvdRender::GetInstance()->Stop();
		break;
	}
}


void CCDGPlayerDlg::ShowTitleChapter()
{
	GUINT16 u2CurIndex = 0, u2TotCnt = 0;
	DVP_GetPlayingIdx(&u2CurIndex);
	DVP_GetTrackCount(&u2TotCnt);

	if (u2CurIndex > u2TotCnt)
	{
		u2CurIndex = 0;
	}
	CWceUiLoadString strRes(L"DVD_TRACK"); 
	GTCHAR tTrkIndex[22];
	wsprintf(tTrkIndex, TEXT("%s: %d/%d"), strRes.GetString(), u2CurIndex, u2TotCnt);

	CWceUiLayer* pTitle = GetLayerByName(L"title_tip");
	if(pTitle)
	{
		pTitle->SetText(tTrkIndex);
	}
}


void CCDGPlayerDlg::ShowAudio(void)
{
	if (!m_bAudioBtnClicked)
	{
		return;
	}
	m_bAudioBtnClicked = FALSE;

	GUINT8 u1Channel = 0, u1ChannelCnt = 0;
	DVP_GetVCDCurChannel(&u1Channel);
	DVP_GetVCDAudioChannelCnt(&u1ChannelCnt);

	if (u1ChannelCnt == 1)
	{
		TCHAR tAudio[32] = L"\0";
		switch (u1Channel)
		{
		case AUDIO_STEREO: //stereo
			wcscpy(tAudio,L"DVD_AUDIO_STEREO");			
			break;

		case AUDIO_MONO_LEFT: //mono left
			wcscpy(tAudio,L"DVD_AUDIO_LEFT");	
			break;	

		case AUDIO_MONO_RIGHT: //mono right
			wcscpy(tAudio,L"DVD_AUDIO_RIGHT");	
			break;	

		case AUDIO_MONO_MIXED: //mix-mono
			wcscpy(tAudio,L"DVD_AUDIO_MIXED");	
			break;

		default:			
			return;
		}

		CWceUiLoadString str;
		str.SetTextResID(tAudio);
		ShowTipMessage(str.GetString());
	}

}

void CCDGPlayerDlg::ShowTipMessage(LPCTSTR message)
{
	CWceUiLayer *player = GetLayerByName(L"message_tip");
	if (player)
	{
		player->SetText(message);
		player->ShowLayer(TRUE);
		SetTimer(TIMER_ID_HIDE_MESSAGE_TIP, 2000);
	}
}
