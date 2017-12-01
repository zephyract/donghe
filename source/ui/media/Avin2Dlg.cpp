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
		// ��ʱ���Ǹ��첽����,�ڼ�״̬�п��ܸı�,��֤���ڿɼ�ʱ����ʾ��Ƶ
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
		ShowNoSignalFlag();		// ɲ���߱仯ʱ,Ҳ��Ӱ�����ź���ʾ,��Ϊ��ɲ����ʾʱ���ǲ���ʾ���ź���ʾ��
	}

	return __super::OnMessage(message, wParam, lParam);
}


void CAvin2Dlg::OnDlgShow(BOOL bShow)
{
	if (!IsAnimating() && CMainWnd::GetInstance()->IsForegroundWindow())	// ���û�ж���Ч��ʱ,ֱ��show or hide
	{
		CUiDDraw::GetInstance()->UpdateOverlay(bShow, 180);
		ShowVideo(bShow);
	}
	// �ж���ʱ���������̻������ʾ������

	if (bShow)
	{
		SetTimer(TIMER_ID_SHOW_SIGNAL_FLAG, 1500);
	}
	else
	{
// 		m_bIsSiganlReady = FALSE;
// 		KillTimer(TIMER_ID_SHOW_SIGNAL_FLAG);
	}

	if (IsVideoForbidden())	// ����г���ֹ��Ƶ,����������ʾ���źű�־
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
		// ����ô��ڲ��ɼ�����ƵҲ���ɼ�
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
	if (IsVideoForbidden() || m_bIsSiganlReady)	// ����Ѿ���ֹ��Ƶ�ˣ�����ʾno signal ��ʾ
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
	// ֻ���ڲ���״̬���������δ�����µ�����²��Զ�ȫ������
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
	//  ���ź�ʱ��Ҫ��ȫ��
	if (!m_bIsSiganlReady)
	{
		return;
	}

	__super::OnLButtonDown(nFlags, point);
}

void CAvin2Dlg::OnAnimateBegin()
{
	// ������ʼ��,�п���֮ǰ����Ƶ��ʾ������ûִ�У�ȡ����
	KillTimer(TIMER_ID_DELAY_SHOW_VIDEO);
	KillTimer(TIMER_ID_DELAY_SHOW_OVERLAY);

	CUiDDraw::GetInstance()->UpdateOverlay(FALSE, 180);
	ShowVideo(FALSE, NULL);

	__super::OnAnimateBegin();
}

void CAvin2Dlg::OnAnimateEnd()
{
	if (m_bAnimShow)	// hide����ʱ
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
	else	//  ������ʽ���ü�
	{
		CMsdkRender::GetInstance()->SetVideoScaleAvin2(NULL);
	}
}
