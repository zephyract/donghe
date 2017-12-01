#include "stdafx.h"
#include "DvpVideoBaseDlg.h"
#include "interface.h"
#include "sysUtil.h"
#include "protocol.h"
#include "VideoParamsDlg.h"
#include "MainWnd.h"
#include "uiddraw.h"

CDvpVideoBaseDlg::CDvpVideoBaseDlg(void)
{
	m_bLayersShow = TRUE;
	m_nLayersCurrentFrame = 0;

	m_bFullScreen = FALSE;
}

CDvpVideoBaseDlg::~CDvpVideoBaseDlg(void)
{

}

void CDvpVideoBaseDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(UI_MSG_MCU_STATUS_CHANGE);
	RegisterMsg(UI_MSG_LBUTTON_DOWN);
	RegisterMsg(UI_MSG_LBUTTON_UP);

	SetTimer(TIMER_ID_AUTO_FULLSCREEN, AUTO_FULLSCREEN_TIME);

}

void CDvpVideoBaseDlg::OnDlgShow(BOOL bShow)
{
	ShowVideoForbiddenTip();

	// ��������ʱ,��Ƶ�������ô��ڱ�������,��Ϊ�ô���û�н���CDlgManager����,�����Զ�����
	if (!bShow)
	{
		CVideoParamsDlg::ShowDlg(FALSE);
	}

	__super::OnDlgShow(bShow);
}

LRESULT CDvpVideoBaseDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IsLayerVisible())
	{
		if (message == UI_MSG_LBUTTON_DOWN)		// ���¼�ʱ
		{
			KillTimer(TIMER_ID_AUTO_FULLSCREEN);
		}
		else if (message == UI_MSG_LBUTTON_UP)
		{
			SetTimer(TIMER_ID_AUTO_FULLSCREEN, AUTO_FULLSCREEN_TIME);
		}
	}
	
	if (message == UI_MSG_MCU_STATUS_CHANGE)
	{
		ShowVideoForbiddenTip();
		ShowVideo(TRUE);
	}

	return __super::OnMessage(message, wParam, lParam);
}


void CDvpVideoBaseDlg::OnTimer(WORD wIDEvent)
{
	switch (wIDEvent)
	{
	case TIMER_ID_AUTO_FULLSCREEN:
		if (m_bFullScreen)
		{
			KillTimer(TIMER_ID_AUTO_FULLSCREEN);
		}
		else if (!IsAnimating())
		{
			OnAutoFullscreen();
		}
		break;
	case TIMER_ID_ANIM_SHOW_LAYERS:
		OnTimerAnimShowLayers();
		break;
	case TIMER_ID_DELAY_SHOW_OVERLAY:
		KillTimer(TIMER_ID_DELAY_SHOW_OVERLAY);
		// ����򿪴��ڶ���������,������GPS�����´��ں�̨ʱ����overlay��ʾ�����ˣ����������жϡ�
		if (IsLayerVisible() && CMainWnd::GetInstance()->IsForegroundWindow())
		{
			CUiDDraw::GetInstance()->UpdateOverlay(TRUE, 180);
		}
		InvalidateRect(NULL);
		SetTimer(TIMER_ID_DELAY_SHOW_VIDEO, 35);
		break;
	default:
		break;
	}

	__super::OnTimer(wIDEvent);
}

void CDvpVideoBaseDlg::OnTimerAnimShowLayers()
{
	if (m_bLayersShow)
	{
		if (m_nLayersCurrentFrame <= 0)	// finish
		{
			KillTimer(TIMER_ID_ANIM_SHOW_LAYERS);
			ShowOtherLayers(TRUE);
		}
		else
		{
			m_nLayersCurrentFrame--;
			AnimMoveLayers(TRUE);
		}
	} 
	else
	{
		if (m_nLayersCurrentFrame >= ANIM_SHOW_LAYERS_MAX_FRAME)	// finish
		{
			KillTimer(TIMER_ID_ANIM_SHOW_LAYERS);
		}
		else
		{
			m_nLayersCurrentFrame++;
			AnimMoveLayers(FALSE);
			ShowOtherLayers(FALSE);
		}
	}
}

void CDvpVideoBaseDlg::AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData)
{
	// �������Ҫ����ʱ���պ���ȫ�����ȫ�����л���ͣ����ȫ��, ������ǿ���л��ɷ�ȫ���ٶ���
	while (m_nLayersCurrentFrame > 0)	
	{
		m_nLayersCurrentFrame--;
		AnimMoveLayers(TRUE);
	}
	m_bFullScreen = FALSE;

	// finish
	KillTimer(TIMER_ID_ANIM_SHOW_LAYERS);
	ShowOtherLayers(TRUE);

	m_bLayersShow = TRUE;

	// ��Ϊ����򵹳��Ƚ����л�����ʱ�����˷�ȫ��״̬,��ʱҪ����ȫ����ʱ��
	SetTimer(TIMER_ID_AUTO_FULLSCREEN, AUTO_FULLSCREEN_TIME);

	__super::AnimShowWindow(bShow, nAnimMode, dwAnimData);
}

void CDvpVideoBaseDlg::OnAutoFullscreen()
{

}

void CDvpVideoBaseDlg::AnimShowLayers(BOOL bShow)
{
	m_bLayersShow = bShow;
	SetTimer(TIMER_ID_ANIM_SHOW_LAYERS, 30);
}

void CDvpVideoBaseDlg::AnimMoveLayers(BOOL bShow)
{
	PCWceUiLayer player = NULL;
	CRect src;

	player = GetChildByName(L"top_bar");
	if (player)
	{
		WCEUI_POSITION_WAY way = player->GetStyle()->GetPosition(src);
		int step = (src.Height() / ANIM_SHOW_LAYERS_MAX_FRAME) + 1;
		src.OffsetRect(0, bShow ? step : -step);

		player->GetStyle()->SetPosition(&src, way);
		player->GetCommonStyle()->SetPosition(&src, way);
		player->GetCommonStyle()->SetUsePosition(TRUE);
	}


	player = GetChildByName(L"bottom_bar");
	if (player)
	{
		WCEUI_POSITION_WAY way = player->GetStyle()->GetPosition(src);
		int step = (src.Height() / ANIM_SHOW_LAYERS_MAX_FRAME) + 1;
		src.OffsetRect(0, bShow ? -step : step);

		player->GetStyle()->SetPosition(&src, way);
		player->GetCommonStyle()->SetPosition(&src, way);
		player->GetCommonStyle()->SetUsePosition(TRUE);


		CWceUiLayer *pLayerFrame = GetChildByName(L"bottom_frame");
		if (pLayerFrame)
		{
			way = pLayerFrame->GetStyle()->GetPosition(src);
			src.OffsetRect(0, bShow ? -step : step);

			pLayerFrame->GetStyle()->SetPosition(&src, way);
			pLayerFrame->GetCommonStyle()->SetPosition(&src, way);
			pLayerFrame->GetCommonStyle()->SetUsePosition(TRUE);
		}
	}

}

void CDvpVideoBaseDlg::OnLButtonDown(UINT nFlags, POINT point)
{
	if (!IsAnimating())
	{
		SetFullScreen(!m_bFullScreen);
	}

	__super::OnLButtonDown(nFlags, point);
}

void CDvpVideoBaseDlg::OnKeyOSD()
{
	SetFullScreen(!m_bFullScreen);
	SetTimer(TIMER_ID_AUTO_FULLSCREEN, AUTO_FULLSCREEN_TIME);
}

void CDvpVideoBaseDlg::SetFullScreen(BOOL bFullScreen)
{
	m_bFullScreen = bFullScreen;
	AnimShowLayers(!m_bFullScreen);
	return;
}


void CDvpVideoBaseDlg::ShowOtherLayers(BOOL bShow)
{
	CWceUiLayer* player = GetChildByName(L"repeat_tip");
	if (player)	player->ShowLayer(bShow);

	player = GetChildByName(L"random_tip");
	if (player)	player->ShowLayer(bShow);

	player = GetChildByName(L"eq");
	if (player)	player->ShowLayer(bShow);

	player = GetChildByName(L"vset");
	if (player)	player->ShowLayer(bShow);
}

BOOL CDvpVideoBaseDlg::IsVideoForbidden()
{
	if (sysutil::nss_get_instance()->video_parking 
		&& !protocol::get_mcu_status()->is_brake_on())
	{
		return TRUE;
	}

	return FALSE;
}

void CDvpVideoBaseDlg::ShowVideoForbiddenTip()
{
	BOOL bShow = FALSE;
	if (IsVideoForbidden())
	{
		bShow = TRUE;
	}

	CWceUiLayer *pLayer = GetChildByName(L"video_forbidden");
	if (pLayer)
	{
		pLayer->ShowLayer(bShow);
	}

	pLayer = GetChildByName(L"video_forbidden_tip");
	if (pLayer)
	{
		pLayer->ShowLayer(bShow);
	}
}