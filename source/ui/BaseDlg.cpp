#include "stdafx.h"
#include "BaseDlg.h"
#include "WceUiBase.h"
#include "sysUtil.h"

#include "phonelink.h"
#include "easyconnected.h"
#include "tzUtil.h"
DLG_INFO g_dlg_info[] = 
{
	{CMainDlg_ID,		L"CMainDlg",					0,	SOURCE_MAIN,	FALSE, FALSE, FALSE, NULL, TRUE},

	{CSettingDlg_ID,	 L"CSettingDlg",				1,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	// 通用设置
	{CSetGeneralDlg_ID,	 L"setting\\CSetGeneralDlg",	2,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetLanguageDlg_ID, L"setting\\CSetLanguageDlg",	3,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetAboutDlg_ID,	L"setting\\CSetAboutDlg",		3,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetColorfulLightDlg_ID,	L"setting\\CSetColorfulLightDlg",		3,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetScreenSaverDlg_ID,	L"setting\\CSetScreenSaverDlg",		3,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},

	{CSetWallpaperDlg_ID,L"setting\\CSetWallpaperDlg",	2,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetDateTimeDlg_ID, L"setting\\CSetDateTimeDlg",	2,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetTimezoneDlg_ID, L"setting\\CSetTimezoneDlg",	3,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CGpsViewerDlg_ID,   L"setting\\CGpsViewerDlg",	    2,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetGpsDlg_ID,		 L"setting\\CSetGpsDlg",	    2,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetGpsPathDlg_ID,	 L"setting\\CSetGpsPathDlg",	3,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetRadioAreaDlg_ID,L"setting\\CSetRadioAreaDlg",	2,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetVideoDlg_ID,	L"setting\\CSetVideoDlg",		2,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetSWCDlg_ID,		L"setting\\CSetSWCDlg",			2,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetAudioDlg_ID,	L"setting\\CSetAudioDlg",		2,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetBluetoothDlg_ID,	L"setting\\CSetBluetoothDlg",2,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	// 工厂设置
	{CSetFactoryDlg_ID,	L"setting\\CSetFactoryDlg",	2,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetMcuUpdateDlg_ID,	L"setting\\CSetMcuUpdateDlg",	3,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CSetUpdateLogoDlg_ID,	L"setting\\CSetUpdateLogoDlg",	3,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CFacFunctionsDlg_ID,	L"factory\\CFacFunctionsDlg",	3,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CFacLanguageDlg_ID,	L"factory\\CFacLanguageDlg",	3,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CFacDefaultLanDlg_ID,	L"factory\\CFacDefaultLanDlg",	4,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},
	{CFacCanbusDlg_ID,		L"factory\\CFacCanbusDlg",		3,	SOURCE_SETTING, FALSE, FALSE, FALSE, NULL, TRUE},

	{CRadioDlg_ID,		L"CRadioDlg",					1,	SOURCE_FM,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CRadioRDSDlg_ID,	L"CRadioRDSDlg",				1,	SOURCE_FM,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CRadioPtyDlg_ID,	L"CRadioPtyDlg",				2,	SOURCE_FM,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CBluetoothDlg_ID,	L"bt\\CBluetoothDlg",			1,	SOURCE_BT,		FALSE, FALSE, FALSE, NULL, TRUE},
	{CBTCallDlg_ID,		L"bt\\CBTCallDlg",				1,	SOURCE_BT_CALL,		FALSE, FALSE, FALSE, NULL, TRUE},

	{CMMFileListDlg_ID,	 L"media\\CMMFileListDlg",		1,	SOURCE_SD,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CMusicPlayerDlg_ID, L"media\\CMusicPlayerDlg",		2,	SOURCE_SD,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CVideoPlayerDlg_ID, L"media\\CVideoPlayerDlg",		2,	SOURCE_SD,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CPhotoPlayerDlg_ID, L"media\\CPhotoPlayerDlg",		2,	SOURCE_SD,		FALSE, FALSE, FALSE, NULL, FALSE},

	{CMMFileListDlg_ID,	 L"media\\CMMFileListDlg",		1,	SOURCE_USB,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CMusicPlayerDlg_ID, L"media\\CMusicPlayerDlg",		2,	SOURCE_USB,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CVideoPlayerDlg_ID, L"media\\CVideoPlayerDlg",		2,	SOURCE_USB,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CPhotoPlayerDlg_ID, L"media\\CPhotoPlayerDlg",		2,	SOURCE_USB,		FALSE, FALSE, FALSE, NULL, FALSE},

	{CMMFileListDlg_ID,	 L"media\\CMMFileListDlg",		1,	SOURCE_USB_REAR,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CMusicPlayerDlg_ID, L"media\\CMusicPlayerDlg",		2,	SOURCE_USB_REAR,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CVideoPlayerDlg_ID, L"media\\CVideoPlayerDlg",		2,	SOURCE_USB_REAR,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CPhotoPlayerDlg_ID, L"media\\CPhotoPlayerDlg",		2,	SOURCE_USB_REAR,		FALSE, FALSE, FALSE, NULL, FALSE},

	// DVD
	{CDVDStartDlg_ID,	 L"dvd\\CDVDStartDlg",			1,	SOURCE_DVD,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CDVDPlayerDlg_ID,	 L"dvd\\CDVDPlayerDlg",			2,	SOURCE_DVD,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CCDFileListDlg_ID,	 L"dvd\\CCDFileListDlg",		2,	SOURCE_DVD,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CCDPlayerDlg_ID,	 L"dvd\\CCDPlayerDlg",			3,	SOURCE_DVD,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CVCDPlayerDlg_ID,	 L"dvd\\CVCDPlayerDlg",			2,	SOURCE_DVD,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CCDGPlayerDlg_ID,	 L"dvd\\CCDGPlayerDlg",			2,	SOURCE_DVD,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CDVDFileListDlg_ID, L"dvd\\CDVDFileListDlg",		2,	SOURCE_DVD,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CDVDMPSPlayerDlg_ID, L"dvd\\CDVDMPSPlayerDlg",		3,	SOURCE_DVD,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CDVDAudioPlayerDlg_ID, L"dvd\\CDVDAudioPlayerDlg",		3,	SOURCE_DVD,		FALSE, FALSE, FALSE, NULL, FALSE},
	{CDVDPhotoPlayerDlg_ID, L"dvd\\CDVDPhotoPlayerDlg",		3,	SOURCE_DVD,		FALSE, FALSE, FALSE, NULL, FALSE},
	// IPOD
	{CUIPodFileListDlg_ID,	L"usb_ipod\\CUIPodFileListDlg",	1,	SOURCE_IPOD_USB,	FALSE, FALSE, FALSE, NULL, FALSE},
	{CUIPodPlayerDlg_ID,	L"usb_ipod\\CUIPodPlayerDlg",	2,	SOURCE_IPOD_USB,	FALSE, FALSE, FALSE, NULL, FALSE},

	{CAvin1Dlg_ID,		L"media\\CAvin1Dlg",			1,	SOURCE_AVIN1,	FALSE, FALSE, FALSE, NULL, FALSE},
	{CAvin2Dlg_ID,		L"media\\CAvin2Dlg",			1,	SOURCE_AVIN2,	FALSE, FALSE, FALSE, NULL, FALSE},
	{CATVDlg_ID,		L"media\\CATVDlg",				1,	SOURCE_TV,	FALSE, FALSE, FALSE, NULL, FALSE},

	{CCameraDlg_ID,		L"media\\CCameraDlg",			1,	SOURCE_CAMERA,	FALSE, FALSE, FALSE, NULL, FALSE},
	{CSetAudioDlg_ID,	L"setting\\CSetAudioDlg",		1,	SOURCE_EQ, FALSE, FALSE, FALSE, NULL, FALSE},
	{CClockDlg_ID,	    L"CClockDlg",					1,	SOURCE_CLOCK, FALSE, FALSE, FALSE, NULL, TRUE},

	// game and tools
	{CCalendarDlg_ID,	    L"calendar\\CCalendarDlg",	  1, SOURCE_CALENDAR, FALSE, FALSE, FALSE, NULL, TRUE},
	{CCalculatorDlg_ID,	    L"calculator\\CCalculatorDlg",1, SOURCE_CALCULATOR, FALSE, FALSE, FALSE, NULL, TRUE},
	{CLinkDlg_ID,			L"link\\CLinkDlg",			  1, SOURCE_LINK, FALSE, FALSE, FALSE, NULL, TRUE},
	{CGoBangDlg_ID,			L"gobang\\CGoBangDlg",		  1, SOURCE_GOBANG, FALSE, FALSE, FALSE, NULL, TRUE},
	{COthelloDlg_ID,		L"othello\\COthelloDlg",	 1, SOURCE_OTHELLO, FALSE, FALSE, FALSE, NULL, TRUE},
	{CBoxManDlg_ID,			L"boxman\\CBoxManDlg",		  1, SOURCE_BOXMAN, FALSE, FALSE, FALSE, NULL, TRUE},
	{CChinaChessDlg_ID,		L"chinachess\\CChinaChessDlg", 1, SOURCE_CHINACHESS, FALSE, FALSE, FALSE, NULL, TRUE},
	{CRussianDlg_ID,		L"russian\\CRussianDlg", 1, SOURCE_RUSSIAN, FALSE, FALSE, FALSE, NULL, TRUE},
	{CQTelzone_ID,			L"qtelzone\\CQTelzoneDlg",	  1, SOURCE_QTELZONE, FALSE, FALSE, FALSE, NULL, TRUE},
	// ecar
	{CEcarDlg_ID,			L"ecar\\CEcarDlg",			1, SOURCE_ECAR, FALSE, FALSE, FALSE, NULL, FALSE},
	// vdisk
	{CVDiskListDlg_ID,		L"media\\CVDiskListDlg",	1, SOURCE_VDISK, FALSE, FALSE, FALSE, NULL, FALSE},
	{CVDiskPlayerDlg_ID,	L"media\\CVDiskPlayerDlg",	2, SOURCE_VDISK, FALSE, FALSE, FALSE, NULL, FALSE},
	// car info
	{CCarInfoDlg_ID,		L"canbus\\CCarInfoDlg",		1, SOURCE_CAR_INFO, FALSE, FALSE, FALSE, NULL, TRUE},
	// navi warning
	{CNaviWarningDlg_ID,	L"CNaviWarningDlg",		1, SOURCE_NAVI_WARNING, FALSE, FALSE, FALSE, NULL, TRUE},



	// 外部程序
	{CPhonelinkStartDlg_ID,		L"phonelink\\CPhonelinkStartDlg",	1, SOURCE_PHONE_LINK, FALSE, FALSE, FALSE, NULL, FALSE},
	{PhoneLink_ID,		L"",		2, SOURCE_PHONE_LINK, TRUE, FALSE, FALSE, (CBaseDlg*)CPhoneLink::ShowWindow, FALSE},
	{CPhonelinkWarningDlg_ID,		L"phonelink\\CPhonelinkWarningDlg",	2, SOURCE_PHONE_LINK, FALSE, FALSE, FALSE, NULL, FALSE},

	{EasyConnected_ID,		L"",		1, SOURCE_EASY_CONNECTED, TRUE, FALSE, FALSE, (CBaseDlg*)CEasyConnected::ShowWindow, FALSE},

};

int ui_get_dlg_info_count()
{
	return sizeof(g_dlg_info)/sizeof(g_dlg_info[0]);
}


CBaseDlg::CBaseDlg(void)
{

}

CBaseDlg::~CBaseDlg(void)
{

}

void CBaseDlg::AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData)
{

}

BOOL CBaseDlg::IsAnimating()
{
	return FALSE;
}

void CBaseDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"home") || pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
	}
}

void CBaseDlg::OnSBnClick(CWceUiSButton* pSButton, int status)
{
}

void CBaseDlg::OnBnLongClick(CWceUiButton* pButton)
{
}

void CBaseDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{

}

void CBaseDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
}

void CBaseDlg::OnDlgShow(BOOL bShow)
{

}

LRESULT CBaseDlg::OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam)
{
	if (uCommand == WCEUI_CMD_LISTBOX_SELCHANGE)
	{
		ui_mcu_beep();
		OnListSelChange((CWceUiListBox*)wParam, (int)(lParam&0xFFFF), (BOOL)(lParam>>16));
	}
	else if (uCommand == WCEUI_CMD_BUTTON_CLICK)
	{
		CWceUiButton* pthis = (CWceUiButton*)wParam;
		if ((pthis->GetUserData() & 0x01) == 0)		// user data的最低位置1时,不响BEEP音,主要是收音里有些钮MCU自己BEEP
		{
			ui_mcu_beep();
		}

		if (lParam == WCEUI_BUTTON_LONG_CLICK)
		{
			OnBnLongClick((CWceUiButton*)wParam);
		}
		else
		{
			OnBnClick((CWceUiButton*)wParam);
		}
	}
	else if (uCommand == WCEUI_CMD_SLIDER)
	{
		if (((lParam>>16)&0xFFFF) == SLIDER_MOUSE_DOWN)
		{
			ui_mcu_beep();
		}
		OnSliderChange((CWceUiSlider*)wParam, (lParam>>16)&0xFFFF, lParam & 0xFFFF);
	}
	else if (uCommand == WCEUI_CMD_SBUTTON_CLICK)
	{
		ui_mcu_beep();
		OnSBnClick((CWceUiSButton*)wParam, (int)lParam);
	}


	return __super::OnCommand(uCommand, wParam, lParam);
}

// ***************** CBKLayer ***********************//
void CBKLayer::OnDrawBackGround(CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap)
{
	RECT rc;
	GetWindowRect(&rc);
	if (sysutil::nss_get_instance()->pui_bk_texture)
	{
//		wceui::wceglDrawTexture(sysutil::nss_get_instance()->pui_bk_texture, &rc);
		pDrawBitmap->BitBlt(rc.left, rc.top, tzutil::_W(&rc), tzutil::_H(&rc), 
			sysutil::nss_get_instance()->pui_bk_texture, 0, 0, SRCCOPY);
	}
	else	// 如果加载背景失败，直接用黑背景
	{
		LPBYTE pbb;
		HBITMAP hbmp = pDrawBitmap->GetHBitmap(pbb);
		CDC dc;
		dc.CreateCompatibleDC(NULL);
		dc.FillSolidRect(&rc, RGB(0,0,0));

	}
}


//  CSetBKLayer
void CSetBKLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
// 	float tran = wceui::wceglGetTransparent();
// 	wceui::wceglSetTransparent(NSS_UI_BK_TRANSPARENT);	// 设置界面的背景淡化下

	__super::OnDraw(pWndBitmap, rcNeedDraw);

//	wceui::wceglSetTransparent(tran);
}

// CMainBKLayer
void CMainBKLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
// 	wceui::wceglPushMatrix();
// 	wceui::wceglLoadIdentity();	// 主界面的背景不作动画变换

	__super::OnDraw(pWndBitmap, rcNeedDraw);

//	wceui::wceglPopMatrix();
}


