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
#include "config.h"
#include "SetRadioAreaDlg.h"
#include "sysUtil.h"

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
		pLayer->SetScrollRange(0, WceUiGetScreenWidth()==1024 ? 600 : 224);
	}

	// calibrate
	if (config::get_config_misc()->calibrate == 0)	// 如果不支持触摸校准,隐藏该选项
	{
		CWceUiButton* pcalibrate = (CWceUiButton*)GetLayerByName(L"calibrate");
		if (pcalibrate)
		{
			pcalibrate->EnableButton(FALSE);
		}
	}

}

void CSettingDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		InitRDSVolume();
	}
}

void CSettingDlg::InitRDSVolume()
{
	return; // not support RDS volume this version


	CWceUiLayer* pvolume = GetLayerByName(L"btn_rdsvolume");
	if (pvolume == NULL)
	{
		return;
	}

	// RDS 音量设置项,只有支持RDS时才显示该设置项
	if (config::get_config_misc()->rds	// 配置文件支持RDS
		&& (protocol::get_mcu_sys_para()->get_fm_cur_region() == FQ_EUROPE
		|| protocol::get_mcu_sys_para()->get_fm_cur_region() == FQ_SAMER1))	// 北美
	{
		pvolume->ShowLayer(TRUE);
	}
	else 
	{
		pvolume->ShowLayer(FALSE);
	}


	CWceUiSlider* pslider = (CWceUiSlider*)GetLayerByName(L"rdsvolume");
	CWceUiLayer* pvalue = GetLayerByName(L"value_rdsvolume");
	if (pvalue)
	{
		CString str;
		str.Format(L"%d", sysutil::nss_get_instance()->audio_ta_volume);
		pvalue->SetText(str);
	}
	if (pslider)
	{
		pslider->SetRange(0, 40);
		pslider->SetPos(sysutil::nss_get_instance()->audio_ta_volume);
	}

}

void CSettingDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"rdsvolume"))
	{
		sysutil::nss_get_instance()->audio_ta_volume = pos;

		CWceUiLayer* pvalue = GetLayerByName(L"value_rdsvolume");
		if (pvalue)
		{
			CString str;
			str.Format(L"%d", sysutil::nss_get_instance()->audio_ta_volume);
			pvalue->SetText(str);
		}
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
	else if (pButton->IsEqualName(L"time"))		// 日期時間
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



