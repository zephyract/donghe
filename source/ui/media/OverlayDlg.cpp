#include "stdafx.h"
#include "OverlayDlg.h"
#include "WceUiBase.h"
#include "MainWnd.h"
#include "uiddraw.h"
#include "sysUtil.h"
#include "tzUtil.h"
#include "WceUiLayerCtrl.h"
#include "WceUiLoadString.h"
#include "interface.h"

COverlayDlg::COverlayDlg(void)
{
	m_bIsVolumeUIShow = FALSE;
	m_bIsCanUIShow = FALSE;
	m_bIsTipUIShow = FALSE;
	m_pVolSlider = NULL;
	m_dwVolTime = 5000;
	m_eVolumeType = VOLUME_TYPE_MEDIA;
	if (WceUiGetBitsPixel() == 16)
	{
		m_bmp = new CWceUi565Bitmap();
	}
	else
	{
		m_bmp = new CWceUi32Bitmap();
	}
	m_bmp->CreateDIB(WceUiGetScreenWidth(), WceUiGetScreenHeight());
}

COverlayDlg::~COverlayDlg(void)
{
	delete m_bmp;
	m_bmp = NULL;
}

COverlayDlg* COverlayDlg::GetInstance()
{
	static BOOL bInit = FALSE;
	static COverlayDlg* pdlg = NULL;
	if (!bInit)
	{
		bInit = TRUE;
		pdlg = (COverlayDlg*)CMainWnd::GetInstance()->GetLayerCtrl()->CreateDlg(L"media\\COverlayDlg");
		ASSERT(pdlg);
		if (pdlg)
		{
			pdlg->RunModeless();
			pdlg->ShowLayer(FALSE);
		}
	}

	return pdlg;
}

void COverlayDlg::OnInitDialog()
{
	__super::OnInitDialog();

	m_pVolSlider = (CWceUiSlider*)GetLayerByName(L"progress");
	if (m_pVolSlider)
	{
		m_pProgressSlider->EnalbeNotifyOnMove(TRUE);
		m_pVolSlider->SetRange(0, UI_MAX_VOLUME_LEVEL);
	}

	CWceUiLayer* player = GetLayerByName(L"volume_frame");
	if (player)
	{
		player->GetStyle()->GetPosition(&m_rcVolume);
		player->ShowLayer(FALSE);
	}

	player = GetLayerByName(L"message_tip");
	if (player)
	{
		player->ShowLayer(FALSE);
	}
}

void COverlayDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_HIDE_VOLUME_UI)
	{
		KillTimer(TIMER_ID_HIDE_VOLUME_UI);
		Vol_HideUI();
	}
	else if (wIDEvent == TIMER_ID_HIDE_CLIMATRONIC_UI)
	{
		KillTimer(TIMER_ID_HIDE_CLIMATRONIC_UI);
		Climatronic_HideUI();
	}
	else if (wIDEvent == TIMER_ID_HIDE_MESSAGE_TIP_UI)
	{
		KillTimer(TIMER_ID_HIDE_MESSAGE_TIP_UI);
		MessageTip_HideUI();
	}

	__super::OnTimer(wIDEvent);
}

void COverlayDlg::OnDlgShow(BOOL bShow)
{
//	CUiDDraw::GetInstance()->UpdateOverlay(bShow, 180);
}

void COverlayDlg::OnBnClick(CWceUiButton* pButton)
{
	__super::OnBnClick(pButton);
}

void COverlayDlg::Vol_ShowMuteIcon(BOOL bMute)
{
	static BOOL s_bMute = FALSE;
	if (tzutil::IsBoolEqual(bMute, s_bMute))
	{
		return;
	}

	CWceUiLayer *player = GetLayerByName(L"vol_icon");
	if (player)
	{
		if (bMute)
		{
			player->SetStyle(L"vol_mute");
		}
		else 
		{
			player->SetStyle(L"vol_normal");
		}
	}
	s_bMute = bMute;
}

void COverlayDlg::Vol_SetData(LPCTSTR lpszType, int nValue, DWORD dwTime, int nStep)
{
	CWceUiLayer* pType = GetLayerByName(L"vol_type");
	CWceUiLayer* pValue = GetLayerByName(L"vol_value");
	m_dwVolTime = dwTime;
	int volume = 0;
	if (pType && pValue)
	{
		pType->SetText(lpszType);

		TCHAR buffer[16];
		_stprintf(buffer, L"%d", nValue);
		pValue->SetText(buffer);
		if (m_pVolSlider)
		{
			m_pVolSlider->SetPos(nValue);
		}
	}

	Vol_ShowMuteIcon(sysutil::nss_get_instance()->audio_is_media_front_mute);

	if (dwTime != (DWORD)-1)
	{
		SetTimer(TIMER_ID_HIDE_VOLUME_UI, dwTime);
	}
	else
	{
		KillTimer(TIMER_ID_HIDE_VOLUME_UI);
	}

	UpdateOverlay();
}

void COverlayDlg::Vol_ShowUI(UI_VOLUME_TYPE eVolumeType)
{
	m_eVolumeType = eVolumeType;
	winhook::HookLLMouse(Vol_MouseLL_Hook_CB, (DWORD)this);
	CWceUiLayer* pLayer = GetLayerByName(L"volume_frame");
	if (pLayer)
	{
		pLayer->ShowLayer(TRUE);
		m_bIsVolumeUIShow = TRUE;
	}
	UpdateOverlay();
}

void COverlayDlg::Vol_HideUI()
{
	winhook::UnHookLLMouse();
	CWceUiLayer* pLayer = GetLayerByName(L"volume_frame");
	if (pLayer)
	{
		pLayer->ShowLayer(FALSE);
		m_bIsVolumeUIShow = FALSE;
	}
	UpdateOverlay();

	ui_led_hide_volume();
	CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
}

// 如果不想音量条可屏幕操作,该函数直接返回FALSE就可以了
BOOL COverlayDlg::Vol_MouseLL_Hook_CB(DWORD dwUserData, WPARAM wParam, LPARAM lParam)
{
	COverlayDlg* pthis = (COverlayDlg*)dwUserData;

	static BOOL bHookMouse = FALSE;
	static BOOL bInit = FALSE;
	static CRect rc;
	if (!bInit)
	{
		bInit = TRUE;
		if (pthis->m_pVolSlider)
		{
			pthis->m_pVolSlider->GetWindowRect(rc);
		}
	}

	POINT pt;
	pt.x = LOWORD(lParam); 
	pt.y = HIWORD(lParam);

	// 点击音量图标可静音
	if (!sysutil::nss_get_instance()->is_audio_mute())
	{
		CWceUiLayer *player = pthis->GetLayerByName(L"vol_icon");
		CRect rc_mute;
		if (player)
		{
			player->GetWindowRect(rc_mute);
		}

		if (wParam == WM_LBUTTONDOWN && ::PtInRect(rc_mute, pt))
		{
			CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_KEY_COMMAND, K_MUTE, 0);
			return TRUE;
		}
	}

	if (pthis->m_pVolSlider && !sysutil::nss_get_instance()->audio_is_media_front_mute)	// mute了不允许调音量
	{		
		if (wParam == WM_LBUTTONDOWN)
		{
			bHookMouse = ::PtInRect(rc, pt) ? TRUE : FALSE;
		}

		if (bHookMouse)
		{
			if (wParam == WM_LBUTTONDOWN)
			{
				pthis->m_pVolSlider->OnLButtonDown(0, pt);
				pthis->KillTimer(TIMER_ID_HIDE_VOLUME_UI);
			}
			else if (wParam == WM_MOUSEMOVE)
			{
				static DWORD tick = 0;
				if (tick++%3 == 0)
				{
					pthis->m_pVolSlider->OnMouseMove(0, pt);
				}
				else
				{
					return TRUE;
				}
			}
			else if (wParam == WM_LBUTTONUP)
			{
				pthis->m_pVolSlider->OnLButtonUp(0, pt);
				pthis->SetTimer(TIMER_ID_HIDE_VOLUME_UI, pthis->m_dwVolTime);
			}
			pthis->UpdateOverlay();
			
		}

		return bHookMouse;
	}

	return FALSE;
}

void COverlayDlg::Climatronic_ShowUI()
{
	CWceUiLayer* pLayer = NULL;
	switch (sysutil::nss_get_instance()->sys_canbus_type)
	{
	case CANBUS_TYPE_RAISE_DASAUTO:
		pLayer = GetLayerByName(L"Climatronic_DasAuto");
		break;
	case CANBUS_TYPE_RAISE_FOCUS:
		pLayer = GetLayerByName(L"Climatronic_Focus");
		break;
	case CANBUS_TYPE_RAISE_HONDA:
		pLayer = GetLayerByName(L"Climatronic_Honda");
		break;
	case CANBUS_TYPE_RAISE_PEUGEOT:
		pLayer = GetLayerByName(L"Climatronic_Peugeot");
		break;
	default:
		break;
	}

	if (pLayer)
	{
		pLayer->ShowLayer(TRUE);
		m_bIsCanUIShow = TRUE;
	}
	UpdateOverlay();

	SetTimer(TIMER_ID_HIDE_CLIMATRONIC_UI, HIDE_CLIMATRONIC_UI_TIME);
}

void COverlayDlg::Climatronic_HideUI()
{
	CWceUiLayer* pLayer = NULL;
	switch (sysutil::nss_get_instance()->sys_canbus_type)
	{
	case CANBUS_TYPE_RAISE_DASAUTO:
		pLayer = GetLayerByName(L"Climatronic_DasAuto");
		break;
	case CANBUS_TYPE_RAISE_FOCUS:
		pLayer = GetLayerByName(L"Climatronic_Focus");
		break;
	case CANBUS_TYPE_RAISE_HONDA:
		pLayer = GetLayerByName(L"Climatronic_Honda");
		break;
	case CANBUS_TYPE_RAISE_PEUGEOT:
		pLayer = GetLayerByName(L"Climatronic_Peugeot");
		break;
	default:
		break;
	}

	if (pLayer)
	{
		pLayer->ShowLayer(FALSE);
		UpdateOverlay();	// 把屏幕刷新下,因为音量有优化,有可能后面只输出部分到overlay
		m_bIsCanUIShow = FALSE;
	}
	UpdateOverlay();
}

void COverlayDlg::UpdateOverlay()
{
	CRect rc(0,0,WceUiGetScreenWidth(),WceUiGetScreenHeight());
	BYTE* pbits = NULL;
	HBITMAP hbmp = m_bmp->GetHBitmap(pbits);
	CDC dc;
	dc.CreateCompatibleDC(NULL);

	CMainWnd::GetInstance()->GetLayerCtrl()->DrawLayerToBmp(this, m_bmp, rc);

	HGDIOBJ oldobj = dc.SelectObject(hbmp);
	// 如果只有音量在显示,为了优化性能,只Blt音量区域
	if (m_bIsVolumeUIShow && !m_bIsTipUIShow && !m_bIsCanUIShow)
	{
		CUiDDraw::GetInstance()->BitBltVolumeOverlay(dc.GetSafeHdc(), m_rcVolume.left, m_rcVolume.top, 
			tzutil::_W(&m_rcVolume), tzutil::_H(&m_rcVolume), m_rcVolume.left, m_rcVolume.top);
	}
	else
	{
		CUiDDraw::GetInstance()->BitBltVolumeOverlay(dc.GetSafeHdc(), 0, 0, rc.Width(), rc.Height(), 0, 0);
	}
	dc.SelectObject(oldobj);

	if (m_bIsVolumeUIShow || m_bIsCanUIShow || m_bIsTipUIShow)
	{
		CUiDDraw::GetInstance()->UpdateVolumeOverlay(TRUE);
	}
	else	// 如果没有任何一功能项需显示,隐藏overlay
	{
		CUiDDraw::GetInstance()->UpdateVolumeOverlay(FALSE);
	}
}


void COverlayDlg::MessageTip_ShowUI(LPCTSTR lpszMessage, BOOL bIsResID, UINT nShowDuration)
{
	CWceUiLayer* pLayer = GetLayerByName(L"message_tip");
	if (pLayer && lpszMessage)
	{
		CString text(lpszMessage);
		if (bIsResID)
		{
			CWceUiLoadString str(lpszMessage);
			text = str.GetString();
		}
		pLayer->SetText(text);

		// 根据文字重新设置控件大小
		HFONT hfont = pLayer->GetStyle()->GetFontCtrl()->GetFont();
		CDC dc;
		dc.CreateCompatibleDC(NULL);
		dc.SelectObject(hfont);
		SIZE sizeSpace;
		GetTextExtentPoint32(dc.GetSafeHdc(), text, text.GetLength(), &sizeSpace);

		CRect rc;
		pLayer->GetWindowRect(rc);
		rc.right = rc.left + sizeSpace.cx + 10;
		pLayer->MoveLayer(rc);


		pLayer->ShowLayer(TRUE);
		m_bIsTipUIShow = TRUE;
	}
	UpdateOverlay();

	SetTimer(TIMER_ID_HIDE_MESSAGE_TIP_UI, nShowDuration);
}

void COverlayDlg::MessageTip_HideUI()
{
	CWceUiLayer* pLayer = GetLayerByName(L"message_tip");
	if (pLayer)
	{
		pLayer->ShowLayer(FALSE);
		UpdateOverlay();	// 把屏幕刷新下,因为音量有优化,有可能后面只输出部分到overlay
		m_bIsTipUIShow = FALSE;
	}
	UpdateOverlay();
}

void COverlayDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"progress") /*&& status==SLIDER_MOUSE_UP*/)
	{
		// RDS音量属于媒体音量
		if (m_eVolumeType == VOLUME_TYPE_BT)
		{
			sysutil::nss_get_instance()->audio_bt_volume = pos;
		}
		else if (m_eVolumeType == VOLUME_TYPE_GPS)
		{
			sysutil::nss_get_instance()->audio_gps_volume = pos;
		}
		else
		{
			// 前后排音量暂时不分开管理
			sysutil::nss_get_instance()->audio_media_front_volume = pos;
			sysutil::nss_get_instance()->audio_media_rear_volume = pos;
		}

		CVolumeManager::GetInstance()->UpdateVolume();

		CWceUiLayer* pValue = GetLayerByName(L"vol_value");
		if ( pValue)
		{
			TCHAR buffer[16];
			_stprintf(buffer, L"%d", pos);
			pValue->SetText(buffer);
		}

		int vol = sysutil::nss_get_instance()->audio_is_media_front_mute ? -1 : pos;
		ui_led_show_volume(vol);
		CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());

	}
}
