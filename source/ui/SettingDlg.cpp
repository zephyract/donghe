#include "stdafx.h"
#include "SettingDlg.h"
#include "WceUiListBox.h"
#include "WceUiSButton.h"
#include "WceUiSlider.h"
#include "WceUiSEditBox.h"
#include "WceUiMEditBox.h"
#include "WceUiBase.h"
#include "wceUiPageLayer.h"
#include "WceUiLayerCtrl.h"
#include "DlgManager.h"
#include "msgboxdlg.h"
#include "RpcMCU.h"
#include "PasswordDlg.h"

CSettingDlg::CSettingDlg(void)
{
}

CSettingDlg::~CSettingDlg(void)
{

}

void CSettingDlg::OnInitDialog()
{
	__super::OnInitDialog();

	CWceUiScrollLayer* pLayer = (CWceUiScrollLayer*)GetChildByName(L"frame");
	if (pLayer)
	{
		pLayer->SetScrollMode(SL_VERT);
		pLayer->SetExtraScroll(30);
		pLayer->SetScrollRange(0, 345);
	}
}

void CSettingDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
	}
	else if (pButton->IsEqualName(L"general"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetGeneralDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"wallpaper"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetWallpaperDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"gpsinfo"))
	{
		CDlgManager::GetInstance()->ShowDlg(CGpsViewerDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"time"))		// ÈÕÆÚ•rég
	{
		CDlgManager::GetInstance()->ShowDlg(CSetDateTimeDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"navi"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetGpsDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"calibrate"))
	{
		int ret = CMsgBoxDlg::MessageBox(L"MESSAGE_CALIBRATION", TRUE, MB_ICONQUESTION|MB_YESNO);
		if (ret == IDYES)
		{
			ui_touch_calibrate();
		}

	}
	else if (pButton->IsEqualName(L"radio_area"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetRadioAreaDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"swc"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetSWCDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"video"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetVideoDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"EQ"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetAudioDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"factory"))
	{
		LPCTSTR password = CPasswordDlg::GetPassword();
		if (password && _tcscmp(password, L"8317") == 0)
		{
			CDlgManager::GetInstance()->ShowDlg(CSetFactoryDlg_ID, SOURCE_SETTING);
		}
	}
	else if (pButton->IsEqualName(L"bluetooth"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetBluetoothDlg_ID, SOURCE_SETTING);
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}


///////////////// CTimeShowLayer ///////////////////


CTimeShowLayer::CTimeShowLayer(void)
{
}

CTimeShowLayer::~CTimeShowLayer(void)
{

}

void CTimeShowLayer::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == 1)
	{
		UpdateTime();
	}
}

void CTimeShowLayer::OnInitLayer()
{
	__super::OnInitLayer();
	UpdateTime();
	SetTimer(1, 1000);
}

void CTimeShowLayer::UpdateTime()
{
	TCHAR szDate[32];
	TCHAR szTime[32];
	TCHAR buffer[64];
	ui_get_datetime_string(szDate, szTime);
	_stprintf(buffer, L"%s  %s", szDate, szTime);
	SetText(buffer);
}



