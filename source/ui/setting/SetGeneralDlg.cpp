#include "stdafx.h"
#include "SetGeneralDlg.h"
#include "tzUtil.h"
#include "sysUtil.h"
#include "interface.h"
#include "WceUiBase.h"
#include "WceUiRealWnd.h"
#include "config.h"
#include "protocol.h"
#include "RpcMCU.h"
#include "ycapi.h"
#include "msgboxdlg.h"
#include "WceUiLayerCtrl.h"

CSetGeneralDlg::CSetGeneralDlg(void)
{

}

CSetGeneralDlg::~CSetGeneralDlg(void)
{

}

void CSetGeneralDlg::OnInitDialog()
{
	__super::OnInitDialog();
	RegisterMsg(NAVI_MSG_LAN_CHANGE);

	InitLayers();
}

void CSetGeneralDlg::InitLayers()
{
	CWceUiScrollLayer* pLayer = (CWceUiScrollLayer*)GetChildByName(L"frame");
	if (pLayer)
	{
		pLayer->SetScrollMode(SL_VERT);
		pLayer->SetExtraScroll(30);
		if (config::get_config_misc()->colorful_lights == 1)
		{
			pLayer->SetScrollRange(0, 56);
		}
	}

	InitSButtonStatus();

	// 语言
	UpdateLanguage();

	// colorful lights
	if (config::get_config_misc()->colorful_lights == 0)	// 如果不支持七彩灯,隐藏该选项
	{
		CWceUiLayer* plights = GetLayerByName(L"colorful_lights");
		CWceUiLayer* pabout =  GetLayerByName(L"about");
		if (plights)
		{
			plights->ShowLayer(FALSE);
			CRect rc;
			WCEUI_POSITION_WAY posWay = plights->GetStyle()->GetPosition(&rc);
			if (pabout)
			{
				pabout->GetStyle()->SetPosition(&rc, posWay);
				pabout->GetCommonStyle()->SetPosition(&rc, posWay);
				pabout->GetCommonStyle()->SetUsePosition(TRUE);
			}
		}
	}
}

void CSetGeneralDlg::InitSButtonStatus()
{
	// beep
	CWceUiSButton* psbutton = (CWceUiSButton*)GetLayerByName(L"beep_switch");
	if(psbutton)
	{
		psbutton->SetStatus(sysutil::nss_get_instance()->audio_key_beep ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);
	}

	// 大灯检测
	psbutton = (CWceUiSButton*)GetLayerByName(L"light_check_switch");
	if(psbutton)
	{
		psbutton->SetStatus(sysutil::nss_get_instance()->sys_light_check ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);
	}
}

void CSetGeneralDlg::OnReset()
{
	InitSButtonStatus();
}

void CSetGeneralDlg::UpdateLanguage()
{
	CWceUiLayer* pLayerValue = GetLayerByName(L"value_language");
	if (pLayerValue)
	{
		int lan_id = sysutil::nss_get_instance()->ui_lan_id;
		LANGUAGE_TABLE_INFO* info = config::get_language_info(lan_id);
		if (info)
		{
			pLayerValue->SetText(info->name);
		}
	}
}

void CSetGeneralDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"language"))	// found it
	{
		CDlgManager::GetInstance()->ShowDlg(CSetLanguageDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"about"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetAboutDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"screen_saver"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetScreenSaverDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"reset"))
	{
		int ret = CMsgBoxDlg::MessageBox(L"MESSAGE_RESET", TRUE, MB_ICONQUESTION|MB_YESNO);
		if (ret == IDYES)
		{
			sysutil::nss_reset();
		}
	}
	else if (pButton->IsEqualName(L"colorful_lights"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetColorfulLightDlg_ID, SOURCE_SETTING);
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}

LRESULT CSetGeneralDlg::OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam)
{

	return __super::OnCommand(uCommand, wParam, lParam);
}


LRESULT CSetGeneralDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == NAVI_MSG_LAN_CHANGE)
	{
		UpdateLanguage();
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CSetGeneralDlg::OnSBnClick(CWceUiSButton* pSButton, int status)
{
	if (pSButton->IsEqualName(L"beep_switch"))
	{
		BOOL bEnable = (status == WCEUI_SBUTTON_ON) ? TRUE : FALSE;
		sysutil::nss_get_instance()->audio_key_beep = bEnable;
		protocol::get_mcu_volume_table()->key_beep_on = bEnable;
		protocol::get_mcu_sys_para()->general_data[0] = sysutil::nss_get_instance()->audio_key_beep;	// MCU判断时,也判断了该标志位
		CRpcMCU::GetInstance()->RPC_SetVolume(protocol::get_mcu_volume_table());
	}
	else if (pSButton->IsEqualName(L"light_check_switch"))
	{
		sysutil::nss_get_instance()->sys_light_check = (status == WCEUI_SBUTTON_ON);
		ui_update_backlight();
	}
}
