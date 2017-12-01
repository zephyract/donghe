#include "stdafx.h"
#include "Avin2Dlg.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "MainWnd.h"
#include "msdkrender.h"
#include "uiddraw.h"
#include "SourceManager.h"
#include "sysUtil.h"
#include "SetVideoParams.h"
#include "interface.h"


void CAvin2Dlg::MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data)
{
	CAvin2Dlg* pthis = (CAvin2Dlg*)user_data;
	if (pthis)
	{
		pthis->OnMediaEvent(media_event, param1, param2);
	}
}

CAvin2Dlg::CAvin2Dlg(void)
{
	m_bIsSiganlReady = FALSE;
	CMsdkRender::GetInstance()->RegisterEventHandlerAvin(MediaEventHandler, (DWORD)this, MSDK_AVIN2);

}

CAvin2Dlg::~CAvin2Dlg(void)
{
	CMsdkRender::GetInstance()->UnRegisterEventHandlerAvin(MediaEventHandler);
}

void CAvin2Dlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_AUX_VIDEO_PARAMS_CHANGE);
	RegisterMsg(UI_MSG_MCU_STATUS_CHANGE);

	__super::OnInitDialog();

}

void CAvin2Dlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_DELAY_SHOW_VIDEO)
	{
		KillTimer(TIMER_ID_DELAY_SHOW_VIDEO);
		// 计时器是个异步操作,期间状态有可能改变,保证窗口可见时才显示视频
		ShowVideo(TRUE);
	}
	else if (wIDEvent == TIMER_ID_SHOW_SIGNAL_FLAG)
	{
		KillTimer(TIMER_ID_SHOW_SIGNAL_FLAG);	
		ShowNoSignalFlag();
	}

	__super::OnTimer(wIDEvent);
}


LRESULT CAvin2Dlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_AUX_VIDEO_PARAMS_CHANGE)
	{
		UpdateVideoParams();
		return 0L;
	}
	else if (message == UI_MSG_MCU_STATUS_CHANGE)
	{
		ShowNoSignalFlag();		// 刹车线变化时,也会影响无信号提示,因为有刹车提示时，是不显示无信号提示的
	}

	return __super::OnMessage(message, wParam, lParam);
}


void CAvin2Dlg::OnDlgShow(BOOL bShow)
{
	if (!IsAnimating() && CMainWnd::GetInstance()->IsForegroundWindow())	// 如果没有动画效果时,直接show or hide
	{
		CUiDDraw::GetInstance()->UpdateOverlay(bShow, 180);
		ShowVideo(bShow);
	}
	// 有动画时，动画过程会管理显示或隐藏

	if (bShow)
	{
		SetTimer(TIMER_ID_SHOW_SIGNAL_FLAG, 1500);
	}
	else
	{
// 		m_bIsSiganlReady = FALSE;
// 		KillTimer(TIMER_ID_SHOW_SIGNAL_FLAG);
	}

	if (IsVideoForbidden())	// 如果行车禁止视频,立即隐藏显示无信号标志
	{
		ShowNoSignalFlag();
	}

	UpdateVideoParams();

	__super::OnDlgShow(bShow);
}

void CAvin2Dlg::ShowVideo(BOOL bShow, RECT* prect)
{
	SOURCE_INFO *psi = CSourceManager::GetInstance()->GetFrontSrc();
	if (psi && psi->sid == SOURCE_AVIN2)
	{
		// 如果该窗口不可见，视频也不可见
		if (IsLayerVisible() 
			&& CMainWnd::GetInstance()->IsForegroundWindow()
			&& !IsVideoForbidden())
		{
			CMsdkRender::GetInstance()->AShowVideoAvin2(bShow, psi->sink, prect);
		}
		else
		{
			CMsdkRender::GetInstance()->AShowVideoAvin2(FALSE, psi->sink, prect);
		}

	}
	UpdateVideoParams();
}


void CAvin2Dlg::UpdateVideoParams()
{
	if (IsLayerVisible() 
		&& CMainWnd::GetInstance()->IsForegroundWindow()
		&& !IsVideoForbidden()
		&& CMsdkRender::GetInstance()->AIsSignalReadyAvin2())

	{
		CSetVideoParams::setBrightness((UINT*)&sysutil::nss_get_instance()->video_aux_brightness);
		CSetVideoParams::setContrast((UINT*)&sysutil::nss_get_instance()->video_aux_contrast);
		CSetVideoParams::setHue((UINT*)&sysutil::nss_get_instance()->video_aux_hue);
		CSetVideoParams::setSaturation((UINT*)&sysutil::nss_get_instance()->video_aux_saturation);
	}
	else
	{
		CSetVideoParams::setBrightness((UINT*)&sysutil::nss_get_instance()->video_brightness);
		CSetVideoParams::setContrast((UINT*)&sysutil::nss_get_instance()->video_contrast);
		CSetVideoParams::setHue((UINT*)&sysutil::nss_get_instance()->video_hue);
		CSetVideoParams::setSaturation((UINT*)&sysutil::nss_get_instance()->video_saturation);
	}
}

void CAvin2Dlg::ShowNoSignalFlag()
{
	BOOL bShow = TRUE;
	if (IsVideoForbidden() || m_bIsSiganlReady)	// 如果已经禁止视频了，不显示no signal 提示
	{
		bShow = FALSE;
	}
	else
	{
		if(m_bFullScreen) SetFullScreen(FALSE);
	}
	ShowNoSignalLayer(bShow);
}

void CAvin2Dlg::ShowNoSignalLayer(BOOL bShow)
{
// 	CWceUiLayer *player = GetChildByName(L"nosignal");
// 	if(NULL != player)
// 	{
// 		player->ShowLayer(bShow);
// 	}

	CWceUiLayer *player = GetChildByName(L"nosignal_bk");
	if (player)
	{
		player->ShowLayer(bShow);
	}
}

void CAvin2Dlg::OnBnClick(CWceUiButton* pButton)
{
	__super::OnBnClick(pButton);
}

void CAvin2Dlg::OnAutoFullscreen()
{
	// 只有在播放状态，并且鼠标未被按下的情况下才自动全屏播放
	if (!m_bFullScreen && GetAsyncKeyState(VK_LBUTTON) >= 0)
	{
		if (m_bIsSiganlReady)
		{
			SetFullScreen(TRUE);
		}
	}
}

void CAvin2Dlg::OnLButtonDown(UINT nFlags, POINT point)
{
	//  无信号时不要进全屏
	if (!m_bIsSiganlReady)
	{
		return;
	}

	__super::OnLButtonDown(nFlags, point);
}

void CAvin2Dlg::OnAnimateBegin()
{
	// 动画开始了,有可能之前的视频显示动作还没执行，取消掉
	KillTimer(TIMER_ID_DELAY_SHOW_VIDEO);
	KillTimer(TIMER_ID_DELAY_SHOW_OVERLAY);

	CUiDDraw::GetInstance()->UpdateOverlay(FALSE, 180);
	ShowVideo(FALSE, NULL);

	__super::OnAnimateBegin();
}

void CAvin2Dlg::OnAnimateEnd()
{
	if (m_bAnimShow)	// hide窗口时
	{
		SetTimer(TIMER_ID_DELAY_SHOW_OVERLAY, 50);
	}

	UpdateVideoParams();
	
	__super::OnAnimateEnd();
}

void CAvin2Dlg::OnAnimating()
{

}


void CAvin2Dlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	switch(media_event)
	{
	case AVIN_SIGNAL_CHANGE:
		break;
	case AVIN_SIGNAL_READY:
		KillTimer(TIMER_ID_SHOW_SIGNAL_FLAG);
		m_bIsSiganlReady = TRUE;
		ShowNoSignalFlag();	
		SetVideoScale();
		SetTimer(TIMER_ID_AUTO_FULLSCREEN, AUTO_FULLSCREEN_TIME);
		UpdateVideoParams();
		break;
	case AVIN_SIGNAL_LOST:
		m_bIsSiganlReady = FALSE;
		SetTimer(TIMER_ID_SHOW_SIGNAL_FLAG, 1000);		
		UpdateVideoParams();
		SetFullScreen(FALSE);
		break;
	default:
		break;
	}
}


void CAvin2Dlg::SetVideoScale()
{
	RECT rc;
	E_AVIN_SIGNAL_MODE_T mode = CMsdkRender::GetInstance()->AGetSignalModeAvin2();
	if (mode == AVIN_SIGNAL_PAL)
	{
		::SetRect(&rc, 10, 4, 714, 576);		// 720X576
		CMsdkRender::GetInstance()->SetVideoScaleAvin2(&rc);
	}
	else if (mode == AVIN_SIGNAL_PAL_M
		|| mode == AVIN_SIGNAL_NTSC)
	{
		::SetRect(&rc, 10, 4, 714, 486);		// 720X486
		CMsdkRender::GetInstance()->SetVideoScaleAvin2(&rc);
	}
	else	//  其它制式不裁剪
	{
		CMsdkRender::GetInstance()->SetVideoScaleAvin2(NULL);
	}
}
