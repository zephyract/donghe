#include "stdafx.h"
#include "WceUiListBox.h"
#include "WceUiBase.h"
#include "SetRadioAreaDlg.h"
#include "config.h"
#include "interface.h"
#include "sysUtil.h"
#include "WceUiLoadString.h"
#include "protocol.h"
#include "RpcMCU.h"
#include "MainWnd.h"

CSetRadioAreaDlg::CSetRadioAreaDlg(void)
: m_bNeedSetMCUParam(FALSE)
{
	FREP_NATION index[RADIO_AREA_COUNT] = {FQ_CHINA, FQ_ASIA, FQ_JAPAN, FQ_RUSSIAN, FQ_EUROPE, FQ_SAMER1, FQ_SAMER2, FQ_M_EAST};
	LPCTSTR resid[RADIO_AREA_COUNT] = {L"RADIO_FREQ_CHINA", L"RADIO_FREQ_ASIA", L"RADIO_FREQ_JAPANESE", L"RADIO_FREQ_RUSSIAN", L"RADIO_FREQ_EUROPE",
		L"RADIO_FREQ_AMERICA1", L"RADIO_FREQ_AMERICA2", L"RADIO_FREQ_MIDDLE_EAST"};
	for (int i=0; i<RADIO_AREA_COUNT; i++)
	{
		m_area[i].index = index[i];
		m_area[i].resid = resid[i];
	}
}

CSetRadioAreaDlg::~CSetRadioAreaDlg(void)
{

}

void CSetRadioAreaDlg::OnInitDialog()
{
	RegisterMsg(NAVI_MSG_LAN_CHANGE);
	InitListbox();

	__super::OnInitDialog();

}

LRESULT CSetRadioAreaDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == NAVI_MSG_LAN_CHANGE)
	{
		InitListbox();
		return 0L;
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CSetRadioAreaDlg::InitListbox()
{
	CWceUiListBox *plist = (CWceUiListBox*)GetLayerByName(L"listbox");
	if (plist)
	{
		plist->ResetContent();
		plist->SetExtraScroll(30);

		CWceUiLoadString load_str;
		for (int i=0; i<RADIO_AREA_COUNT; i++)
		{
			load_str.SetTextResID(m_area[i].resid);
			plist->AddString(load_str.GetString());

			if ( protocol::get_mcu_sys_para()->get_fm_cur_region() == m_area[i].index)
			{
				plist->SetCurSel(i);
			}

		}

		

	}
}


void CSetRadioAreaDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	int index = plist->GetCurSel();
// 	int language_id = (int)plist->GetItemData(index);
// 	::PostMessage(HWND_BROADCAST, NAVI_MSG_LAN_CHANGE, 0, (LPARAM)language_id);

	m_bNeedSetMCUParam = TRUE;
	SetTimer(TIMER_ID_SET_MCU_PARAMS, 500);
}

void CSetRadioAreaDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_SET_MCU_PARAMS)
	{
		KillTimer(TIMER_ID_SET_MCU_PARAMS);
		if (m_bNeedSetMCUParam)
		{
			SetMCUParam();
			m_bNeedSetMCUParam = FALSE;
		}
	}

	__super::OnTimer(wIDEvent);
}

void CSetRadioAreaDlg::OnDlgShow(BOOL bShow)
{
	if (!bShow && m_bNeedSetMCUParam)
	{
		SetMCUParam();
		m_bNeedSetMCUParam = FALSE;
	}

	__super::OnDlgShow(bShow);
}

void CSetRadioAreaDlg::SetMCUParam()
{
	CWceUiListBox *plist = (CWceUiListBox*)GetLayerByName(L"listbox");
	if (plist)
	{
		int index = plist->GetCurSel();
		if (index >= 0 && index < RADIO_AREA_COUNT)
		{
			protocol::get_mcu_sys_para()->set_fm_cur_region((BYTE)m_area[index].index);
			RPCERR ret;
			CRpcMCU::GetInstance()->RPC_SetSysParams(MCU_SYS_PARA::INDEX_FM, 
				protocol::get_mcu_sys_para()->fm_data,	sizeof(protocol::get_mcu_sys_para()->fm_data), &ret);
			GetWindow()->PostMessage(UI_MSG_MCU_SYSPARAMS_CHANGE, 0, MCU_SYS_PARA::INDEX_FM);
		}
	}
	
}