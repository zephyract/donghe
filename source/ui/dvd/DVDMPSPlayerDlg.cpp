#include "StdAfx.h"
#include "DVDMPSPlayerDlg.h"
#include "WceUiButton.h"
#include "WceUiRealWnd.h"
#include <atlbase.h>
#include "dvdrender.h"
#include "uiddraw.h"
#include "WceUiPageLayer.h"
#include "MainWnd.h"
#include "WceUiLoadString.h"
#include "OverlayDlg.h"

void CDVDMPSPlayerDlg::DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData)
{
	CDVDMPSPlayerDlg* pthis = (CDVDMPSPlayerDlg*)u4CustomData;
	pthis->OnMediaEvent(u4Evt, u4Param1, u4Param2);
}

CDVDMPSPlayerDlg::CDVDMPSPlayerDlg(void)
{
	CDvdRender::GetInstance()->RegisterEventHandler(CDVDMPSPlayerDlg::DvpAgentEventListener, (DWORD)this);
	m_bAudioBtnClicked    = FALSE;
	m_bSubtitleBtnClicked = FALSE;
	m_bRandomClicked = FALSE;
	m_bRepeatClicked = FALSE;
}

CDVDMPSPlayerDlg::~CDVDMPSPlayerDlg(void)
{
	CDvdRender::GetInstance()->UnRegisterEventHandler(CDVDMPSPlayerDlg::DvpAgentEventListener);
}

void CDVDMPSPlayerDlg::InitLayer()
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

	m_pLayerBadfileInfo = GetChildByName(L"badfile_info");
}

void CDVDMPSPlayerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitLayer();


	SetTimer(TIMER_ID_CHECK_MENU, 1000);


	// DVD 左边剪裁掉8个像素
	DVD_SCALE_INFO_T scale_info;
	scale_info.eSinkType = DVD_SINK_FRONT;
	::SetRect(&scale_info.rRect, 8, 6, 712, 480);
	scale_info.fgNotScale = FALSE;
	DVP_SetScaleInfo(&scale_info);

	ShowFileTip();
}

void CDVDMPSPlayerDlg::OnTimer(WORD wIDEvent)
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

			CWceUiLayer *ptip = GetLayerByName(L"file_tip");
			if (ptip)
			{
				ptip->ShowLayer(TRUE);
			}
		}
		break;
	case TIMER_ID_AUTO_PLAY_NEXT:
		KillTimer(TIMER_ID_AUTO_PLAY_NEXT);
		CDvdRender::GetInstance()->Next();
		ShowBadfileTipInfo(FALSE, NULL);
		break;
	default:
		break;
	}

	__super::OnTimer(wIDEvent);
}

void CDVDMPSPlayerDlg::OnAutoFullscreen()
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

void CDVDMPSPlayerDlg::OnBnClick(PCWceUiButton pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CDVDFileListDlg_ID, SOURCE_DVD);
	}
	else if (pButton->IsEqualName(L"subtitle"))
	{
		CDvdRender::GetInstance()->SwitchSubtitle();
		m_bSubtitleBtnClicked = TRUE;
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
		m_bRepeatClicked = FALSE;
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


LRESULT CDVDMPSPlayerDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return __super::OnMessage(message, wParam, lParam);
}


void CDVDMPSPlayerDlg::OnLButtonDown(UINT nFlags, POINT point)
{
	GUINT8 uHiliStatus = DVD_HL_OFF;
	DVP_GetCurrentDVDMenuState(&uHiliStatus);
	if (uHiliStatus == DVD_HL_ON)
	{	
		DVP_DVDClickMenuItem(MAKELPARAM(point.x, point.y));
	}
	else
	{
		SetFullScreen(!m_bFullScreen);
	}
}

void CDVDMPSPlayerDlg::DVD_Stop()
{
	CDvdRender::GetInstance()->Stop();
	OnMediaEvent(EVT_DVP_PBC_PLAY_STATE, SV_STOP, 0);	// stop命令,数据碟视频时不触发该事件,这里模拟下
}

void CDVDMPSPlayerDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
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
	case EVT_DVP_PBC_PBC:
		break;
	case EVT_DVP_PBC_TRACK_NUM:
	case EVT_DVP_PBC_PLAYING_AUDIO_IDX:
		ShowFileTip();
		break;
	case EVT_DVP_MPS_ERROR:		// 播放不支持的文件时,会返回这个错误
		{
			GUINT8 err;
			DVP_GetMpsError(&err);	// 获取错误码
			RETAILMSG(1, (L"++++++++++++++++++++ DVD error  = %d . \n", err));

			OnErrorOccured(err);
		}
		break;
	default:
		break;
	}
}

void CDVDMPSPlayerDlg::OnErrorOccured(BYTE err)
{
	if (err == SV_E_V_CODEC_UNSUP)
	{
		ShowBadfileTipInfo(TRUE, L"MIS_UNSUPPORTED_VIDEO");
		SetTimer(TIMER_ID_AUTO_PLAY_NEXT, 1000);
	}
	else if (err == SV_E_A_CODEC_UNSUP)
	{
		ShowBadfileTipInfo(TRUE, L"MIS_UNSUPPORTED_AUDIO");
		SetTimer(TIMER_ID_AUTO_PLAY_NEXT, 1000);
	} 
	else if (err != SV_E_NO_ERROR)
	{
		ShowBadfileTipInfo(TRUE, L"MIS_UNSUPPORTED_FILE");
		SetTimer(TIMER_ID_AUTO_PLAY_NEXT, 1000);
	}
}

void CDVDMPSPlayerDlg::ShowBadfileTipInfo(BOOL bShow, LPCTSTR text_id)
{
	if (m_pLayerBadfileInfo)
	{
		if (bShow)
		{
			m_pLayerBadfileInfo->GetStyle()->SetTextResID(text_id);
		}

		m_pLayerBadfileInfo->ShowLayer(bShow);
	}
}


void CDVDMPSPlayerDlg::OnEvtPBCTime(BOOL bDVDStopped)
{
	GUINT64 outDuration = 0, curDuration = 0;
	DVP_GetDuration(&outDuration);
	// DVD stop状态后系统返回仍是当前的位置,逻辑上stop后应该回到开始位置
	if (!bDVDStopped)
	{
		DVP_GetCurrentPosition(&curDuration);
	}
	UpdataProgress((UINT)(curDuration/1000), (UINT)(outDuration/1000));			 
	ShowMediaTimeInfo((UINT)(curDuration/1000), (UINT)(outDuration/1000));

	// refresh LED
	GUINT16 u2CurIndex = (GUINT16)-1;
	DVP_GetPlayingIdx(&u2CurIndex);
	ui_led_set_showdata(ui_led_media_data(u2CurIndex == (GUINT16)-1 ? 0 : u2CurIndex+1, curDuration/1000));
	CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
}

void CDVDMPSPlayerDlg::OnEvtCmdResponse(UINT param1, UINT param2)
{
	if (param2 == ACK_FAIL && param1 != AP_CMD_TP_COORDINATE)	// operation fail, show error tip.
	{
		ShowInvalidIcon();

		m_bAudioBtnClicked    = FALSE;
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

void CDVDMPSPlayerDlg::OnEvtPlayState(UINT state)
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
			OnEvtPBCTime(TRUE);
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

void CDVDMPSPlayerDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"progress") && status==SLIDER_MOUSE_UP)
	{
		UINT outDuration = CDvdRender::GetInstance()->GetDuration();
		UINT curDuration = pos * outDuration / 1000;
		CDvdRender::GetInstance()->Seek(curDuration);

		GetWindow()->IgnoreMsgBeforeTime();
	}
}

void CDVDMPSPlayerDlg::ShowInvalidIcon()
{
	CWceUiLayer *player = GetChildByName(L"invalid_tip");
	if (player)
	{
		player->ShowLayer(TRUE);
		SetTimer(TIMER_ID_HIDE_INVALID_TIP, 500);
	}
}

void CDVDMPSPlayerDlg::OnDlgShow(BOOL bShow)
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


void CDVDMPSPlayerDlg::ShowVideo(BOOL bShow, RECT* prect)
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

void CDVDMPSPlayerDlg::OnAnimateBegin()
{
	// 动画开始了,有可能之前的视频显示动作还没执行，取消掉
	KillTimer(TIMER_ID_DELAY_SHOW_VIDEO);
	KillTimer(TIMER_ID_DELAY_SHOW_OVERLAY);

	CUiDDraw::GetInstance()->UpdateOverlay(FALSE, 180);
	ShowVideo(FALSE, NULL);


	__super::OnAnimateBegin();
}


void CDVDMPSPlayerDlg::OnAnimateEnd()
{
	if (m_bAnimShow)	// hide窗口时
	{
		SetTimer(TIMER_ID_DELAY_SHOW_OVERLAY, 10);
	}

	__super::OnAnimateEnd();
}

void CDVDMPSPlayerDlg::OnAnimating()
{

}

void CDVDMPSPlayerDlg::AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData)
{
	// CDVDStartDlg <-> CDVDMPSPlayerDlg  不作动画
// 	if (nAnimMode == ANIM_MODE_UPLEVEL)
// 	{
// 		ShowLayer(bShow);
// 	}
// 	else
	{
		__super::AnimShowWindow(bShow, nAnimMode, dwAnimData);
	}
}


void CDVDMPSPlayerDlg::ShowPlayMode()
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

void CDVDMPSPlayerDlg::ShowFileTip()
{
	GUINT16 cur = 0;
	GUINT16 total = 0;

	TCHAR buffer[10] = L"_/_";
	CWceUiLayer *ptip = GetLayerByName(L"file_tip");
	if( RET_OK == DVP_GetPlayingIdx(&cur) 
		&& RET_OK == DVP_GetTrackCount(&total)
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

}



void CDVDMPSPlayerDlg::OnKeyPlayControl(UINT key, UINT param)
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
		DVD_Stop();
		break;
	case T_IR_GOTO:
		{
			GUINT16 cur = (GUINT16)-1;
			if( RET_OK != DVP_GetPlayingIdx(&cur) || (param-1) != cur)
			{
				DVP_ItemClick((GINT16)param-1);
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
	default:
		break;
	}
}

void CDVDMPSPlayerDlg::ShowTipMessage(LPCTSTR message)
{
// 	CWceUiLayer *player = GetLayerByName(L"message_tip");
// 	if (player)
// 	{
// 		player->SetText(message);
// 		player->ShowLayer(TRUE);
// 		SetTimer(TIMER_ID_HIDE_MESSAGE_TIP, 2000);
// 
// 		// 位置重叠了,显示message时隐藏file tip
// 		CWceUiLayer *ptip = GetLayerByName(L"file_tip");
// 		if (ptip)
// 		{
// 			ptip->ShowLayer(FALSE);
// 		}
// 	}

	// 显示在overlay上 
	COverlayDlg::GetInstance()->MessageTip_ShowUI(message, FALSE);

}


void CDVDMPSPlayerDlg::ShowAudio(void)
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



void CDVDMPSPlayerDlg::ShowSubtitle()
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
