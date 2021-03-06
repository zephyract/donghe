#include "stdafx.h"
#include "WceUiListBox.h"
#include "WceUiBase.h"
#include "SetColorfulLightDlg.h"
#include "config.h"
#include "interface.h"
#include "sysUtil.h"
#include "WceUiLoadString.h"
#include "protocol.h"
#include "RpcMCU.h"

CSetColorfulLightDlg::CSetColorfulLightDlg(void)
{
}

CSetColorfulLightDlg::~CSetColorfulLightDlg(void)
{

}

void CSetColorfulLightDlg::OnInitDialog()
{
	__super::OnInitDialog();
}


void CSetColorfulLightDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		InitListbox();
	}

	__super::OnDlgShow(bShow);
}

void CSetColorfulLightDlg::OnReset()
{
//	InitListbox();
	__super::OnReset();
}

void CSetColorfulLightDlg::InitListbox()
{
	CWceUiListBox *plist = (CWceUiListBox*)GetLayerByName(L"listbox");
	if (plist)
	{
		plist->ResetContent();
		plist->SetExtraScroll(30);

		LPCTSTR res_id[] = {L"SET_LIGHTS_AUTO", L"SET_LIGHTS_RED", L"SET_LIGHTS_YELLOW", L"SET_LIGHTS_GREEN", 
			L"SET_LIGHTS_LIGHTBLUE", L"SET_LIGHTS_BLUE", L"SET_LIGHTS_PURPLE", L"SET_LIGHTS_WHITE" };
		for (int i=0; i<sizeof(res_id)/sizeof(res_id[0]); i++)
		{
			CWceUiLoadString str;
			str.SetTextResID(res_id[i]);
			plist->AddString(str.GetString());
		}

		plist->SetCurSel(sysutil::nss_get_instance()->ui_colorful_light);
	}
}


void CSetColorfulLightDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	sysutil::nss_get_instance()->ui_colorful_light = plist->GetCurSel();

	protocol::get_mcu_sys_para()->set_colorful_light(sysutil::nss_get_instance()->ui_colorful_light);
	CRpcMCU::GetInstance()->RPC_SetSysParams(MCU_SYS_PARA::INDEX_GENERAL, 
		protocol::get_mcu_sys_para()->general_data, sizeof(protocol::get_mcu_sys_para()->general_data));

}


void CSetColorfulLightDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetGeneralDlg_ID, SOURCE_SETTING);
		return;
	}

	__super::OnBnClick(pButton);
}