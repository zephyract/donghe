#include "StdAfx.h"
#include "DVDPlayerDlg.h"
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

void CDVDPlayerDlg::DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData)
{
	CDVDPlayerDlg* pthis = (CDVDPlayerDlg*)u4CustomData;
	pthis->OnMediaEvent(u4Evt, u4Param1, u4Param2);
}

CDVDPlayerDlg::CDVDPlayerDlg(void)
{
	CDvdRender::GetInstance()->RegisterEventHandler(CDVDPlayerDlg::DvpAgentEventListener, (DWORD)this);

	m_bAudioBtnClicked = FALSE;
	m_bSubtitleBtnClicked = FALSE;
	m_bAngelBtnClicked = FALSE;
	m_bRandomClicked = FALSE;
	m_bRepeatClicked = FALSE;

	m_nZoom = 0;
}

CDVDPlayerDlg::~CDVDPlayerDlg(void)
{
	CDvdRender::GetInstance()->UnRegisterEventHandler(CDVDPlayerDlg::DvpAgentEventListener);
}

void CDVDPlayerDlg::InitLayer()
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

	ShowMenuKey(FALSE);
}

void CDVDPlayerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(UI_MSG_MCU_KEY_COMMAND);

	InitLayer();
	SetTimer(TIMER_ID_CHECK_MENU, 1000);


	// DVD 左边剪裁掉8个像素
	DVD_SCALE_INFO_T scale_info;
	scale_info.eSinkType = DVD_SINK_FRONT;
	::SetRect(&scale_info.rRect, 8, 6, 712, 480);
	scale_info.fgNotScale = FALSE;
	DVP_SetScaleInfo(&scale_info);
}


void CDVDPlayerDlg::OnTimer(WORD wIDEvent)
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
	case TIMER_ID_CHECK_MENU:
		{
			GUINT8 uHiliStatus = DVD_HL_OFF;
			DVP_GetCurrentDVDMenuState(&uHiliStatus);
			if (uHiliStatus == DVD_HL_ON && !m_bFullScreen)
			{	
				SetFullScreen(TRUE);	
			}
		}
		break;	
	default:
		break;
	}

	__super::OnTimer(wIDEvent);
}

void CDVDPlayerDlg::OnAutoFullscreen()
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

void CDVDPlayerDlg::OnBnClick(PCWceUiButton pButton)
{
	CWceUiLoadString load_string;
	if (pButton->IsEqualName(L"menu"))
	{
		CDvdRender::GetInstance()->GotoRootMenu();
		load_string.SetTextResID(L"DVD_MENU");
		ShowTipMessage(load_string.GetString());
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
	else if (pButton->IsEqualName(L"subtitle"))
	{
		CDvdRender::GetInstance()->SwitchSubtitle();
		m_bSubtitleBtnClicked = TRUE;
	}
	else if (pButton->IsEqualName(L"angle"))
	{
		CDvdRender::GetInstance()->SwitchPBCAngle();
		m_bAngelBtnClicked = TRUE;
	}
	else if (pButton->IsEqualName(L"titlemenu"))
	{
		CDvdRender::GetInstance()->GotoTitleMenu();
		load_string.SetTextResID(L"DVD_TITLE");
		ShowTipMessage(load_string.GetString());

	}
	else if (pButton->IsEqualName(L"audio"))
	{
		CDvdRender::GetInstance()->SwitchAudioChannel();
		m_bAudioBtnClicked = TRUE;
	}
	else if (pButton->IsEqualName(L"key_up"))
	{
		OnKeyPlayControl(K_UP,0);
	}
	else if (pButton->IsEqualName(L"key_down"))
	{
		OnKeyPlayControl(K_DOWN,0);
	}
	else if (pButton->IsEqualName(L"key_left"))
	{
		OnKeyPlayControl(K_LEFT,0);
	}
	else if (pButton->IsEqualName(L"key_right"))
	{
		OnKeyPlayControl(K_RIGHT,0);
	}
	else if (pButton->IsEqualName(L"key_enter"))
	{
		OnKeyPlayControl(K_ENTER,0);
	}
	else
	{
		__super::OnBnClick(pButton);

	}

}


LRESULT CDVDPlayerDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_MCU_KEY_COMMAND)
	{
		int status = (wParam>>16) & 0xFF;
		int id = wParam & 0xFF;

		switch (id)
		{
		case T_SUB_T:
			CDvdRender::GetInstance()->SwitchSubtitle();
			m_bSubtitleBtnClicked = TRUE;
			break;
		case T_TITLE:
			{
				CDvdRender::GetInstance()->GotoTitleMenu();
				CWceUiLoadString load_string;
				load_string.SetTextResID(L"DVD_TITLE");
				ShowTipMessage(load_string.GetString());
			}
			break;
		case T_DVD_MENU:
			{
				CWceUiLoadString load_string;
				CDvdRender::GetInstance()->GotoRootMenu();
				load_string.SetTextResID(L"DVD_MENU");
				ShowTipMessage(load_string.GetString());
			}
			break;
		case T_ANGER:
			CDvdRender::GetInstance()->SwitchPBCAngle();
			m_bAngelBtnClicked = TRUE;
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


void CDVDPlayerDlg::OnKeyOSD()
{
	GUINT8 uHiliStatus = DVD_HL_OFF;
	DVP_GetCurrentDVDMenuState(&uHiliStatus);
	if (uHiliStatus == DVD_HL_ON)
	{	
		return;
	}
	else
	{
		__super::OnKeyOSD();
	}
}

void CDVDPlayerDlg::OnLButtonDown(UINT nFlags, POINT point)
{
	GUINT8 uHiliStatus = DVD_HL_OFF;
	DVP_GetCurrentDVDMenuState(&uHiliStatus);
	if (uHiliStatus == DVD_HL_ON)
	{	
		point.x = point.x * 800 / WceUiGetScreenWidth();
		point.y = point.y * 480 / WceUiGetScreenHeight();
		DVP_DVDClickMenuItem(MAKELPARAM(point.x, point.y));
	}
	else
	{
		SetFullScreen(!m_bFullScreen);
	}
}


void CDVDPlayerDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
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
	case EVT_DVP_PBC_TITLE_CHAPTER:
		ShowTitleChapter();
		break;
	case EVT_DVP_PBC_AUDIO:
		ShowAudio();
		break;
	case EVT_DVP_PBC_SUBTITLE:
		ShowSubtitle();
		break;
	case EVT_DVP_PBC_REPEAT_MODE:	
		ShowPlayMode();
		break;
	case EVT_DVP_PBC_USER_MODE:
		ShowPlayMode();
		break;

	case EVT_DVP_PBC_ZOOM:
		if (RET_OK == DVP_GetCurrentZoomMultiple(&m_nZoom))
		{
			ShowZoomInfo(m_nZoom);
		}
		break;

	case EVT_DVP_PBC_PBC:
		break;

	case EVT_DVP_PBC_ANGLE:
		ShowAngle();
		break;
	case EVT_DVP_PBC_DVDHILI_STATUS:
		ShowMenuKey(param1==0 ? TRUE : FALSE);
		break;
	default:
		break;
	}
}

void CDVDPlayerDlg::ShowZoomInfo(UCHAR zoom)
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

void CDVDPlayerDlg::OnEvtPBCTime()
{
	GUINT64 outDuration = 0, curDuration = 0;
	DVP_GetDuration(&outDuration);
	DVP_GetCurrentPosition(&curDuration);
	UpdataProgress((UINT)(curDuration/1000), (UINT)(outDuration/1000));			 
	ShowMediaTimeInfo((UINT)(curDuration/1000), (UINT)(outDuration/1000));


	// refresh LED
	GUINT8 cur;
	DVP_GetCurrentTitle(&cur);	
	if (cur == INVALID_GUINT8)		cur = 0;
	ui_led_set_showdata(ui_led_media_data(cur, curDuration/1000));
	CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());

}

void CDVDPlayerDlg::OnEvtCmdResponse(UINT param1, UINT param2)
{
	if (param2 == ACK_FAIL && param1 != AP_CMD_TP_COORDINATE)	// operation fail, show error tip.
	{
		ShowInvalidIcon();
		// 点击按钮有可能操作失败,这时清除点击状态
		m_bAngelBtnClicked = FALSE;
		m_bAudioBtnClicked = FALSE;
		m_bSubtitleBtnClicked = FALSE;
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

void CDVDPlayerDlg::OnEvtPlayState(UINT state)
{
	switch (state)
	{
	case SV_PLAY:
		m_pLayerPlay->ShowLayer(FALSE);
		m_pLayerPause->ShowLayer(TRUE);
		break;
	case SV_PAUSE:
	case SV_STOP_RESUME:
	case SV_STOP:
		m_pLayerPlay->ShowLayer(TRUE);
		m_pLayerPause->ShowLayer(FALSE);

		if (state == SV_PAUSE)
		{
			ui_led_set_showdata(state == SV_PAUSE ? L" PAUS E  " : L"  STO P  ");
			CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
		}
		break;
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

void CDVDPlayerDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"progress") && status==SLIDER_MOUSE_UP)
	{
		UINT outDuration = CDvdRender::GetInstance()->GetDuration();
		UINT curDuration = pos * outDuration / 1000;
		CDvdRender::GetInstance()->Seek(curDuration);

		GetWindow()->IgnoreMsgBeforeTime();
	}


}

void CDVDPlayerDlg::ShowInvalidIcon()
{
	CWceUiLayer *player = GetChildByName(L"invalid_tip");
	if (player)
	{
		player->ShowLayer(TRUE);
		SetTimer(TIMER_ID_HIDE_INVALID_TIP, 500);
	}
}


void CDVDPlayerDlg::OnDlgShow(BOOL bShow)
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

void CDVDPlayerDlg::ShowVideo(BOOL bShow, RECT* prect)
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

void CDVDPlayerDlg::OnAnimateBegin()
{
	// 动画开始了,有可能之前的视频显示动作还没执行，取消掉
	KillTimer(TIMER_ID_DELAY_SHOW_VIDEO);
	KillTimer(TIMER_ID_DELAY_SHOW_OVERLAY);

	CUiDDraw::GetInstance()->UpdateOverlay(FALSE, 180);
	ShowVideo(FALSE, NULL);

	__super::OnAnimateBegin();
}


void CDVDPlayerDlg::OnAnimateEnd()
{
	if (m_bAnimShow)	// hide窗口时
	{
		SetTimer(TIMER_ID_DELAY_SHOW_OVERLAY, 10);
	}

	__super::OnAnimateEnd();
}

void CDVDPlayerDlg::OnAnimating()
{

}

void CDVDPlayerDlg::AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData)
{
	// CDVDStartDlg <-> CDVDPlayerDlg  不作动画
	if (nAnimMode == ANIM_MODE_UPLEVEL)
	{
		ShowLayer(bShow);
	}
	else
	{
		__super::AnimShowWindow(bShow, nAnimMode, dwAnimData);
	}
}

void CDVDPlayerDlg::ShowPlayMode()
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
		case SV_REPEAT_TITLE:
			pRepeat->SetStyle(L"repeat_tip_tt");
			strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_TITLE");
			break;
		case SV_REPEAT_CHAPTER:
			pRepeat->SetStyle(L"repeat_tip_ch");
			strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_CHAPTER");
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


void CDVDPlayerDlg::OnKeyPlayControl(UINT key, UINT param)
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
			GUINT8 total;
			DVP_GetChapterCount(&total);
			if (total == INVALID_GUINT8 || total <=0)	// chapter不存在时选择title
			{
				CDvdRender::GetInstance()->ChooseDigitalNum(0, param);
			}
			else
			{
				CDvdRender::GetInstance()->ChooseDigitalNum(1, param);
			}
		}
		break;
	case K_UP:
		DVP_Up();
		break;
	case K_DOWN:
		DVP_Down();
		break;
	case K_LEFT:
		DVP_Left();
		break;
	case K_RIGHT:
		DVP_Right();
		break;
	case K_ENTER:
		{
			GUINT8 uHiliStatus = DVD_HL_OFF;
			DVP_GetCurrentDVDMenuState(&uHiliStatus);
			if (uHiliStatus == DVD_HL_ON)
			{           
				UCHAR state;
				DVP_GetPBState(&state);
				DVP_Play();
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


void CDVDPlayerDlg::ShowTitleChapter()
{
	GUINT8 u1CurrentTitle = 0, u1CurrentChapter = 0;
	GUINT8 u1TotalTitle = 0, u1TotalChapter = 0;
	TCHAR tTitle[32];

	DVP_GetCurrentTitle(&u1CurrentTitle);	
	if (u1CurrentTitle == INVALID_GUINT8)
	{
		u1CurrentTitle = 0;
	}	

	DVP_GetTitleCount(&u1TotalTitle);
	if (u1TotalTitle == INVALID_GUINT8)
	{
		u1TotalTitle = 0;
	}

	if (u1TotalTitle == 0)
	{
		u1CurrentTitle = 0;
	}


	DVP_GetCurrentChapter(&u1CurrentChapter);	
	if (u1CurrentChapter == INVALID_GUINT8)
	{
		u1CurrentChapter = 0;
	}	

	DVP_GetChapterCount(&u1TotalChapter);
	if ((u1TotalChapter == INVALID_GUINT8)||(u1CurrentTitle == 0))
	{
		u1TotalChapter = 0;
	}

	if (u1TotalChapter == 0)
	{
		u1CurrentChapter = 0;
	}

	wsprintf(tTitle, TEXT("T: %d/%d  C: %d/%d"), u1CurrentTitle, u1TotalTitle, u1CurrentChapter, u1TotalChapter); 

	CWceUiLayer* pTitle = GetLayerByName(L"title_tip");
	if(pTitle)
	{
		pTitle->SetText(tTitle);
	}
}



void CDVDPlayerDlg::ShowAudio(void)
{
	if (!m_bAudioBtnClicked)
	{
		return;
	}
	m_bAudioBtnClicked = FALSE;

	GUINT8 u1Current = 0;
	GUINT8 u1Total = 0;
	DVP_GetAudioChannelCount(&u1Total);
	DVP_GetCurrentAudioIndex(&u1Current);
	if (INVALID_GUINT8 == u1Total)
	{
		u1Total = 0;
	}
	if (INVALID_GUINT8 == u1Current)
	{
		u1Current = 0;
	}


	TCHAR tAudio[32] = L"\0";
	if (u1Current!=0 && u1Total!=0)
	{
		CWceUiLoadString str(L"DVD_AUDIO");
		wsprintf(tAudio, TEXT("%s %d/%d"), str.GetString(), u1Current, u1Total);
		ShowTipMessage(tAudio);
	}
}


void CDVDPlayerDlg::ShowTipMessage(LPCTSTR message)
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


void CDVDPlayerDlg::ShowSubtitle()
{
	if (!m_bSubtitleBtnClicked)
	{
		return;
	}
	m_bSubtitleBtnClicked = FALSE;

	GUINT8 u1Current = 0;
	GUINT8 u1Total = 0;
	DVP_GetCurrentSubtitle(&u1Current);
	DVP_GetSubtitleCount(&u1Total);
	if (INVALID_GUINT8 == u1Total)
	{
		u1Total = 0;
	}
	if (INVALID_GUINT8 == u1Current)
	{
		u1Current = 0;
	}

	TCHAR tSubtitle[32] = L"";
	CWceUiLoadString str;
	if (u1Current!=0 && u1Total!=0)
	{
		str.SetTextResID(L"DVD_SUBTITLE");
		wsprintf(tSubtitle, TEXT("%s %d/%d"), str.GetString(), u1Current, u1Total);
	}
	else //OFF
	{
		str.SetTextResID(L"DVD_SUBTITLE_OFF");
		wsprintf(tSubtitle, TEXT("%s"), str.GetString());
	}

	ShowTipMessage(tSubtitle);
}


void CDVDPlayerDlg::ShowAngle()
{
	if (!m_bAngelBtnClicked)
	{
		return;
	}
	m_bAngelBtnClicked = FALSE;


	GINT8 i1Current = 0;
	GUINT8 u1Total = 0;

	DVP_GetCurrentPBCAngle(&i1Current);
	DVP_GetPBCAngleCount(&u1Total);

	if (INVALID_GUINT8 == u1Total)
	{
		u1Total = 0;
	}

	if (INVALID_GINT == i1Current)
	{
		i1Current = 0;
	} 

	if (u1Total == 0 || u1Total == 1)
	{
		return;
	}  
	else
	{ 
		GTCHAR tShowText[64] = L"\0";
		CWceUiLoadString uistr;	
		uistr.SetTextResID(L"DVD_ANGLE");
		wsprintf(tShowText, TEXT("%s %d/%d"),uistr.GetString(), i1Current, u1Total); 
		ShowTipMessage(tShowText);
	}
}


void CDVDPlayerDlg::ShowMenuKey(BOOL bShow)
{
	CWceUiLayer *player = GetChildByName(L"menu_key");
	if (player)
	{
		player->ShowLayer(bShow);
	}
}

///////////////////////////////////////////
CMoveLayer::CMoveLayer(void)
{
	SetSlideMode(SLIDE_HOR_VER);
}

CMoveLayer::~CMoveLayer(void)
{

}

void CMoveLayer::OnLoseCapture()
{
	if (m_bIsMouseDown)
	{
		m_bIsMouseDown = FALSE;
	}
}

void CMoveLayer::OnLButtonDown(UINT nFlags,  POINT point)
{
	SetCapture();
	m_bIsMouseDown = TRUE;

	m_ptMouseDown = point;
	GetWindowRect(&m_rcLayer);

	CWceUiLayer::OnLButtonDown(nFlags, point);
}

void CMoveLayer::OnLButtonUp(UINT nFlags, POINT point)
{
	if (m_bIsMouseDown)
	{
		ReleaseCapture();
		m_bIsMouseDown = FALSE;
	}

	CWceUiLayer::OnLButtonUp(nFlags, point);
}

void CMoveLayer::OnMouseMove(UINT nFlags, POINT point)
{
	if (m_bIsMouseDown)
	{
		CRect rc(m_rcLayer);
		rc.OffsetRect(point.x-m_ptMouseDown.x, point.y-m_ptMouseDown.y);

		// 不能超过边界
		int h = WceUiGetScreenHeight();
		int w = WceUiGetScreenWidth();
		rc.OffsetRect(rc.left<0 ? -rc.left : 0, rc.top<0 ? -rc.top : 0);
		rc.OffsetRect(rc.right>w ? w-rc.right : 0, rc.bottom>h ? h-rc.bottom : 0);

		SetLayerPos(rc.left, rc.top, rc.Width(), rc.Height(), 0);
	}
}
