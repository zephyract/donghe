#include "StdAfx.h"
#include "VCDPlayerDlg.h"
#include "WceUiButton.h"
#include "WceUiRealWnd.h"
#include <atlbase.h>
#include "dvdrender.h"
#include "uiddraw.h"
#include "WceUiPageLayer.h"
#include "MainWnd.h"
#include "WceUiLoadString.h"
#include "interface.h"
#include "OverlayDlg.h"

void CVCDPlayerDlg::DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData)
{
	CVCDPlayerDlg* pthis = (CVCDPlayerDlg*)u4CustomData;
	pthis->OnMediaEvent(u4Evt, u4Param1, u4Param2);
}

CVCDPlayerDlg::CVCDPlayerDlg(void)
{
	CDvdRender::GetInstance()->RegisterEventHandler(CVCDPlayerDlg::DvpAgentEventListener, (DWORD)this);

	m_bAudioBtnClicked = FALSE;
	m_bPBCBtnClicked = FALSE;
	m_bRandomClicked = FALSE;
	m_bRepeatClicked = FALSE;

	m_nZoom = 0;
}

CVCDPlayerDlg::~CVCDPlayerDlg(void)
{
	CDvdRender::GetInstance()->UnRegisterEventHandler(CVCDPlayerDlg::DvpAgentEventListener);
}

void CVCDPlayerDlg::InitLayer()
{
	CWceUiPageLayer* pLayer = (CWceUiPageLayer*)GetChildByName(L"bottom_frame");
	ASSERT(pLayer);
	if (pLayer)
	{
		pLayer->SetExtraScroll(30);
		pLayer->SetScrollRange(0, WceUiGetScreenWidth());
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

void CVCDPlayerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(NAVI_MSG_LAN_CHANGE);
	RegisterMsg(UI_MSG_MCU_KEY_COMMAND);
	InitLayer();

	// DVD 左边剪裁掉8个像素
	DVD_SCALE_INFO_T scale_info;
	scale_info.eSinkType = DVD_SINK_FRONT;
	::SetRect(&scale_info.rRect, 8, 6, 712, 480);
	scale_info.fgNotScale = FALSE;
	DVP_SetScaleInfo(&scale_info);
}

void CVCDPlayerDlg::OnTimer(WORD wIDEvent)
{
	switch (wIDEvent)
	{
	case TIMER_ID_DELAY_SHOW_VIDEO:
		KillTimer(TIMER_ID_DELAY_SHOW_VIDEO);
		// 计时器是个异步操作,期间状态有可能改变,保证窗口可见时才显示视频
		ShowVideo(TRUE);
		break;
	case TIMER_ID_HIDE_INVALID_TIP:
		{
			KillTimer(TIMER_ID_HIDE_INVALID_TIP);
			CWceUiLayer *player = GetChildByName(L"invalid_tip");
			if (player)
			{
				player->ShowLayer(FALSE);
			}
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

void CVCDPlayerDlg::OnAutoFullscreen()
{
	// 只有在播放状态，并且鼠标未被按下的情况下才自动全屏播放
	if (!m_bFullScreen && GetAsyncKeyState(VK_LBUTTON) >= 0
		&& SV_PLAY == CDvdRender::GetInstance()->GetState())
	{
		UCHAR state;
		DVP_GetPBState(&state);
		if (state == SV_PLAY)
		{
			SetFullScreen(TRUE);
		}
	}
}

void CVCDPlayerDlg::OnBnClick(PCWceUiButton pButton)
{
	if (pButton->IsEqualName(L"pbc"))
	{
		DVP_SetPBCState(0);
		m_bPBCBtnClicked = TRUE;
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
		CDvdRender::GetInstance()->Play();
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
		m_bAudioBtnClicked = TRUE;
	}
	else
	{
		__super::OnBnClick(pButton);

	}

}


LRESULT CVCDPlayerDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == NAVI_MSG_LAN_CHANGE)
	{
		ShowTitleChapter();
	}
	else if (message == UI_MSG_MCU_KEY_COMMAND)
	{
		int status = (wParam>>16) & 0xFF;
		int id = wParam & 0xFF;

		switch (id)
		{
		case T_PBC:
			DVP_SetPBCState(0);
			m_bPBCBtnClicked = TRUE;
			break;
		case T_ZOOM:
			if (m_nZoom == 0 || m_nZoom == 4 || m_nZoom == 6)
			{
				CDvdRender::GetInstance()->ZoomIn();
			}
			else
			{
				CDvdRender::GetInstance()->ZoomOut();
			}
			break;
		default:
			break;
		}
	}
	return __super::OnMessage(message, wParam, lParam);
}


void CVCDPlayerDlg::OnLButtonDown(UINT nFlags, POINT point)
{
	SetFullScreen(!m_bFullScreen);
}


void CVCDPlayerDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
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
	case EVT_DVP_PBC_CHANNEL:
		ShowAudio();
		break;
	case EVT_DVP_PBC_REPEAT_MODE:	
		ShowPlayMode();
		break;
	case EVT_DVP_PBC_USER_MODE:
		ShowPlayMode();
		break;
	case EVT_DVP_PBC_PBC:
		ShowPBCState();
		break;
	case EVT_DVP_PBC_ZOOM:
		if (RET_OK == DVP_GetCurrentZoomMultiple(&m_nZoom))
		{
			ShowZoomInfo(m_nZoom);
		}
		break;

	default:
		break;
	}
}

void CVCDPlayerDlg::OnEvtPBCTime()
{
	GUINT64 outDuration = 0, curDuration = 0;
	DVP_GetDuration(&outDuration);
	DVP_GetCurrentPosition(&curDuration);
	UpdataProgress((UINT)(curDuration/1000), (UINT)(outDuration/1000));			 
	ShowMediaTimeInfo((UINT)(curDuration/1000), (UINT)(outDuration/1000));

	// refresh LED
	GUINT16 u2CurIndex = 0, u2TotCnt = 0;
	DVP_GetPlayingIdx(&u2CurIndex);
	DVP_GetTrackCount(&u2TotCnt);
	if (u2CurIndex > u2TotCnt)
	{
		u2CurIndex = 0;
	}
	ui_led_set_showdata(ui_led_media_data(u2CurIndex, curDuration/1000));
	CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
}

void CVCDPlayerDlg::OnEvtCmdResponse(UINT param1, UINT param2)
{
	if (param2 == ACK_FAIL && param1 != AP_CMD_TP_COORDINATE)	// operation fail, show error tip.
	{
		ShowInvalidIcon();

		m_bAudioBtnClicked = FALSE;
		m_bPBCBtnClicked = FALSE;
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

void CVCDPlayerDlg::OnEvtPlayState(UINT state)
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

		if (state == SV_PAUSE)
		{
			ui_led_set_showdata(state == SV_PAUSE ? L" PAUS E  " : L"  STO P  ");
			CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
		}

		break;
	case SV_STOP_RESUME:
	case SV_STILL:
		CDvdRender::GetInstance()->Stop();
		break;
	case SV_FF:
		break;
	case SV_FR:
		break;
	default:
		break;
	}
}

void CVCDPlayerDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"progress") && status==SLIDER_MOUSE_UP)
	{
		UINT outDuration = CDvdRender::GetInstance()->GetDuration();
		UINT curDuration = pos * outDuration / 1000;
		CDvdRender::GetInstance()->Seek(curDuration);

		GetWindow()->IgnoreMsgBeforeTime();
	}


}

void CVCDPlayerDlg::ShowInvalidIcon()
{
	CWceUiLayer *player = GetChildByName(L"invalid_tip");
	if (player)
	{
		player->ShowLayer(TRUE);
		SetTimer(TIMER_ID_HIDE_INVALID_TIP, 500);
	}
}


void CVCDPlayerDlg::OnDlgShow(BOOL bShow)
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

void CVCDPlayerDlg::ShowVideo(BOOL bShow, RECT* prect)
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


void CVCDPlayerDlg::OnAnimateBegin()
{
	// 动画开始了,有可能之前的视频显示动作还没执行，取消掉
	KillTimer(TIMER_ID_DELAY_SHOW_VIDEO);
	KillTimer(TIMER_ID_DELAY_SHOW_OVERLAY);

	CUiDDraw::GetInstance()->UpdateOverlay(FALSE, 180);
	ShowVideo(FALSE, NULL);

	__super::OnAnimateBegin();
}


void CVCDPlayerDlg::OnAnimateEnd()
{
	if (m_bAnimShow)	// hide窗口时
	{
		SetTimer(TIMER_ID_DELAY_SHOW_OVERLAY, 10);
	}

	__super::OnAnimateEnd();
}

void CVCDPlayerDlg::OnAnimating()
{

}

void CVCDPlayerDlg::AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData)
{
	// CDVDStartDlg - > CVCDPlayerDlg  不作动画
	if (nAnimMode == ANIM_MODE_UPLEVEL)
	{
		ShowLayer(bShow);
	}
	else
	{
		__super::AnimShowWindow(bShow, nAnimMode, dwAnimData);
	}
}


void CVCDPlayerDlg::ShowPlayMode()
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


void CVCDPlayerDlg::OnKeyPlayControl(UINT key, UINT param)
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
			CDvdRender::GetInstance()->Play();
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
				CDvdRender::GetInstance()->Play();
			}
		}
		break;
	case T_STOP:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_STOP");
		SetFullScreen(FALSE);
		CDvdRender::GetInstance()->Stop();
		break;
	case T_IR_GOTO:
		if (param > 0)
		{
			GUINT16 total;
			DVP_GetTrackCount(&total);
			if (param <= total)	
			{
				CDvdRender::GetInstance()->ChooseDigitalNum(0, param);
			}
		}
		break;
	case T_TRACK:
		CDvdRender::GetInstance()->SwitchAudioChannel();
		m_bAudioBtnClicked = TRUE;
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


void CVCDPlayerDlg::ShowTitleChapter()
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


void CVCDPlayerDlg::ShowAudio(void)
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

void CVCDPlayerDlg::ShowPBCState()
{
	if (m_bPBCBtnClicked)
	{
		m_bPBCBtnClicked = FALSE;

		GUINT8 u1State = 0;
		DVP_GetPBCState(&u1State);

		CWceUiLoadString str;
		str.SetTextResID((u1State==SV_PBC_ON) ? L"DVD_PBC_ON" : L"DVD_PBC_OFF");
		ShowTipMessage(str.GetString());
	}

}


void CVCDPlayerDlg::ShowTipMessage(LPCTSTR message)
{
// 	CWceUiLayer *player = GetLayerByName(L"message_tip");
// 	if (player)
// 	{
// 		player->SetText(message);
// 		player->ShowLayer(TRUE);
// 		SetTimer(TIMER_ID_HIDE_MESSAGE_TIP, 2000);
// 	}

	// 显示在overlay上 
	COverlayDlg::GetInstance()->MessageTip_ShowUI(message, FALSE);

}

void CVCDPlayerDlg::ShowZoomInfo(UCHAR zoom)
{
	switch (zoom)
	{
	case 4:
		ShowTipMessage(L"ZOOM 2");
		break;
	case 6:
		ShowTipMessage(L"ZOOM 3");
		break;
	case 8:
		ShowTipMessage(L"ZOOM 4");
		break;
	case 9:
		ShowTipMessage(L"ZOOM 1/2");
		break;
	case 10:
		ShowTipMessage(L"ZOOM 1/3");
		break;
	case 11:
		ShowTipMessage(L"ZOOM 1/4");
		break;
	case 0:
		ShowTipMessage(L"ZOOM OFF");
		break;
	default:
		break;
	}
}