#include "stdafx.h"
#include "ATVDlg.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "MainWnd.h"
#include "msdkrender.h"
#include "uiddraw.h"
#include "SourceManager.h"
#include "sysUtil.h"
#include "SetVideoParams.h"
#include "interface.h"

void CATVDlg::MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data)
{
	CATVDlg* pthis = (CATVDlg*)user_data;
	if (pthis)
	{
		pthis->OnMediaEvent(media_event, param1, param2);
	}
}

CATVDlg::CATVDlg(void)
{
	m_bIsSiganlReady = FALSE;
	CMsdkRender::GetInstance()->RegisterEventHandlerAvin(MediaEventHandler, (DWORD)this, MSDK_AVIN_TV);
}

CATVDlg::~CATVDlg(void)
{
	CMsdkRender::GetInstance()->UnRegisterEventHandlerAvin(MediaEventHandler);
}

void CATVDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_AUX_VIDEO_PARAMS_CHANGE);

	__super::OnInitDialog();
}

void CATVDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_CHECK_SIGNAL)
	{
		KillTimer(TIMER_ID_CHECK_SIGNAL);
		m_bIsSiganlReady = CMsdkRender::GetInstance()->AIsSignalReadyTV();
		SetTimer(TIMER_ID_SHOW_SIGNAL_FLAG, 1000);		
	}
	else if (wIDEvent == TIMER_ID_SHOW_SIGNAL_FLAG)
	{
		KillTimer(TIMER_ID_SHOW_SIGNAL_FLAG);	
		ShowVideo(TRUE);	// ��֤video�㴦����ȷ��״̬���п�����Ҫ�ں�̨����AVIN
		ShowNoSignalFlag();
		if(m_bFullScreen) SetFullScreen(FALSE);
	}

	__super::OnTimer(wIDEvent);
}

LRESULT CATVDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_AUX_VIDEO_PARAMS_CHANGE)
	{
		UpdateVideoParams();
		return 0L;
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CATVDlg::OnDlgShow(BOOL bShow)
{
	if (!IsAnimating() && CMainWnd::GetInstance()->IsForegroundWindow())	// ���û�ж���Ч��ʱ,ֱ��show or hide
	{
		CUiDDraw::GetInstance()->UpdateOverlay(bShow, 180);
		ShowVideo(bShow);
	}
	// �ж���ʱ���������̻������ʾ������

	KillTimer(TIMER_ID_SHOW_SIGNAL_FLAG);
	if (bShow)
	{
		SetTimer(TIMER_ID_CHECK_SIGNAL, 500);
	}

	if (IsVideoForbidden())	// ����г���ֹ��Ƶ,����������ʾ���źű�־
	{
		ShowNoSignalFlag();
	}

	UpdateVideoParams();

	__super::OnDlgShow(bShow);
}

void CATVDlg::ShowVideo(BOOL bShow, RECT* prect)
{
	SOURCE_INFO *psi = CSourceManager::GetInstance()->GetFrontSrc();
	if (psi && psi->sid == SOURCE_TV)
	{
		// ����ô��ڲ��ɼ�����ƵҲ���ɼ�
		if (IsLayerVisible() 
			&& CMainWnd::GetInstance()->IsForegroundWindow()
			&& !IsVideoForbidden())
		{
			CMsdkRender::GetInstance()->AShowVideoTV(bShow, psi->sink, prect);
		}
		else
		{
			CMsdkRender::GetInstance()->AShowVideoTV(FALSE, psi->sink, prect);
		}

	}
}

void CATVDlg::UpdateVideoParams()
{
	if (IsLayerVisible() 
		&& CMainWnd::GetInstance()->IsForegroundWindow()
		&& !IsVideoForbidden()
		&& CMsdkRender::GetInstance()->AIsSignalReadyTV())

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

void CATVDlg::ShowNoSignalFlag()
{
	BOOL bShow = TRUE;
	if (IsVideoForbidden() || m_bIsSiganlReady)	// ����Ѿ���ֹ��Ƶ�ˣ�����ʾno signal ��ʾ
	{
		bShow = FALSE;
	}

	CWceUiLayer *player = GetChildByName(L"nosignal");
	if(NULL != player)
	{
		player->ShowLayer(bShow);
	}
}

void CATVDlg::OnBnClick(CWceUiButton* pButton)
{
	__super::OnBnClick(pButton);
}


void CATVDlg::OnAutoFullscreen()
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


void CATVDlg::OnAnimateBegin()
{
	// ������ʼ��,�п���֮ǰ����Ƶ��ʾ������ûִ�У�ȡ����
	KillTimer(TIMER_ID_DELAY_SHOW_OVERLAY);

	CUiDDraw::GetInstance()->UpdateOverlay(FALSE, 180);
	ShowVideo(FALSE, NULL);

	__super::OnAnimateBegin();
}

void CATVDlg::OnAnimateEnd()
{
	if (m_bAnimShow)	// hide����ʱ
	{
		SetTimer(TIMER_ID_DELAY_SHOW_OVERLAY, 10);
	}

	UpdateVideoParams();
	__super::OnAnimateEnd();
}

void CATVDlg::OnAnimating()
{

}

void CATVDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	switch(media_event)
	{
	case EVT_MSDK_AVIN_SIGNAL_CHANGE:
		break;
	case EVT_MSDK_AVIN_SIGNAL_READY:
		m_bIsSiganlReady = TRUE;	
		KillTimer(TIMER_ID_SHOW_SIGNAL_FLAG);
		ShowNoSignalFlag();	
		SetVideoScale();
		SetTimer(TIMER_ID_AUTO_FULLSCREEN, AUTO_FULLSCREEN_TIME);
		UpdateVideoParams();
		break;
	case EVT_MSDK_AVIN_SIGNAL_LOST:
		m_bIsSiganlReady = FALSE;
		SetTimer(TIMER_ID_SHOW_SIGNAL_FLAG, 1000);		
		UpdateVideoParams();
		break;
	default:
		break;
	}
}


void CATVDlg::SetVideoScale()
{
	RECT rc;
	MSDK_AVIN_SIGNAL_MODE_T mode = CMsdkRender::GetInstance()->AGetSignalModeTV();
	if (mode == MSDK_AVIN_SIGNAL_PAL)
	{
		::SetRect(&rc, 10, 4, 714, 576);		// 720X576
		CMsdkRender::GetInstance()->SetVideoScaleTV(&rc);
	}
	else if (mode == MSDK_AVIN_SIGNAL_PAL_M
		|| mode == MSDK_AVIN_SIGNAL_NTSC)
	{
		::SetRect(&rc, 10, 4, 714, 576);		// 720X480
		CMsdkRender::GetInstance()->SetVideoScaleTV(&rc);
	}
	else	//  ������ʽ���ü�
	{
		CMsdkRender::GetInstance()->SetVideoScaleTV(NULL);
	}
}


void CATVDlg::OnLButtonDown(UINT nFlags, POINT point)
{
	UINT param = ((point.x & 0xFF) << 24 ) + (((point.x >> 8) & 0xFF) << 16 ) 
		+ ((point.y & 0xFF) << 8 ) + (((point.y >> 8) & 0xFF) );
	CRpcMCU::GetInstance()->RPC_KeyCommand(T_MOUSE_POS, param);

	__super::OnLButtonDown(nFlags, point);
}
