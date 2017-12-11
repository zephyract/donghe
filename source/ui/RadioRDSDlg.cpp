#include "stdafx.h"
#include "RadioRDSDlg.h"
#include "interface.h"
#include "protocol.h"
#include "wceuibase.h"
#include "tzUtil.h"
#include "sysUtil.h"
#include "SetRadioAreaDlg.h"

static TCHAR* RDS_PROGRAM_TYPE[] = 
{
	_T("NONE"),

	_T("NEWS"),
	_T("AFFAIRS"), 
	_T("INFO"), 
	_T("SPORT"), 

	_T("EDUCATE"),		
	_T("DRAMA"),
	_T("CULTURE"), 
	_T("SCIENCE"),
	_T("VARIED"),

	_T("POP"),
	_T("ROCK"),
	_T("EASY M"), 
	_T("LIGHT M"), 
	_T("CLASSIC"), 

	_T("OTHER M"),		
	_T("WEATHER"),
	_T("FINANCE"), 
	_T("CHILDREN"),
	_T("SOCIAL"),

	_T("RELIGION"),
	_T("PHONE IN"),
	_T("TRAVEL"), 
	_T("LEISURE"), 
	_T("JAZZ"), 

	_T("COUNTRY"),		
	_T("NATION M"),
	_T("OLDIES"), 
	_T("FOLK M"),
	_T("DOCUMENT"),

	_T("TEST"),
	_T("ALARM"),

};

static TCHAR* RBDS_PROGRAM_TYPE[] = 
{
	_T("NONE"),

	_T("NEWS"),
	_T("INFROM"), 
	_T("SPORTS"), 
	_T("TALK"), 

	_T("ROCK"),		
	_T("CLS ROCK"),
	_T("ADLT HIT"), 
	_T("SOFT RCK"),
	_T("TOP 40"),

	_T("COUNTRY"),
	_T("OLDIES"),
	_T("SOFT"), 
	_T("NOSTALGIA"), 
	_T("JAZZ"), 

	_T("CLASSICL"),		
	_T("R&B"),
	_T("SOFT R&B"), 
	_T("LANGUAGE"),
	_T("REL MUSC"),

	_T("REL TALK"),
	_T("FERSNLTY"),
	_T("PUBLIC"), 
	_T("COLLEGE"), 
	_T("WEATHER"), 

	_T("UNDEF1"),		
	_T("UNDEF2"),
	_T("UNDEF3"), 
	_T("UNDEF4"),
	_T("UNDEF5"),

	_T("TEST"),
	_T("ALARM"),

};

BOOL CRadioRDSDlg::m_bIsTopMostDlg = FALSE;

CRadioRDSDlg::CRadioRDSDlg(void)
{

}

CRadioRDSDlg::~CRadioRDSDlg(void)
{

}

#include "MainWnd.h"
void CRadioRDSDlg::OnInitDialog()
{
	__super::OnInitDialog();

	OnRadioStatus();
	OnRadioPresetInfo();
}

void CRadioRDSDlg::InitLayers()
{
	__super::InitLayers();

	ShowPTYUser(sysutil::nss_get_instance()->ui_rds_pty);
	OnRadioPresetInfo();
	ShowPSPTYInfo();

	CWceUiLayer* player = GetChildByName(L"TA_alarm");
	if (player)
	{
		player->SetText(L"Traffic");
		player->ShowLayer(FALSE);
	}

	LPCTSTR name[] = {L"seek_dec", L"seek_inc", L"REG", L"PTY", L"PTY_search",L"AF_status",
		L"TA_status", L"REG_status", L"tune_dec",L"tune_inc"};
	for (int i=0; i<sizeof(name)/sizeof(name[0]); i++)
	{
		CWceUiButton* pbutton = (CWceUiButton*)GetChildByName(name[i]);
		if (pbutton)
		{
			pbutton->SetUserData(1);	// APP不响BEEP
		}
	}

}

void CRadioRDSDlg::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	CAnimDlg::OnDraw(pWndBitmap, rcNeedDraw);
}

LPCTSTR CRadioRDSDlg::GetPTYString(int type)
{
	if (type>=0 && type<sizeof(RDS_PROGRAM_TYPE)/sizeof(RDS_PROGRAM_TYPE[0]))
	{
		if (protocol::get_mcu_sys_para()->get_fm_cur_region() == FQ_SAMER1)	// 北美
		{
			return RBDS_PROGRAM_TYPE[type];
		}
		else
		{
			return RDS_PROGRAM_TYPE[type];
		}
	}

	return L"";
}

void CRadioRDSDlg::ShowPTYUser(int type)
{
	if (type>=0 && type<sizeof(RDS_PROGRAM_TYPE)/sizeof(RDS_PROGRAM_TYPE[0]))
	{
		CWceUiLayer* player = GetLayerByName(L"PTY_user");
		if (player)
		{
			player->SetText(GetPTYString(type));
		}
	}
}

void CRadioRDSDlg::ShowTAAlarm(BOOL bShow)
{
	CWceUiLayer *player = GetChildByName(L"TA_alarm");
	if (player)	{	player->ShowLayer(bShow);	}
}

void CRadioRDSDlg::OnBnLongClick(CWceUiButton* pButton)
{
	__super::OnBnLongClick(pButton);
}

void CRadioRDSDlg::OnKeyPlayControl(UINT key, UINT param)
{
	if (key == T_RADIO_PTY)
	{
		OnPTYClick();
	}
}

void CRadioRDSDlg::OnPTYClick()
{
	sysutil::nss_get_instance()->ui_rds_pty++;
	sysutil::nss_get_instance()->ui_rds_pty = 
		sysutil::nss_get_instance()->ui_rds_pty % (sizeof(RDS_PROGRAM_TYPE)/sizeof(RDS_PROGRAM_TYPE[0]));
	ShowPTYUser(sysutil::nss_get_instance()->ui_rds_pty);
	CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_PTY, sysutil::nss_get_instance()->ui_rds_pty<<24);
}

void CRadioRDSDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"REG"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_REG, 0);
	}
	else if (pButton->IsEqualName(L"REG_status"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_REG, 0);
	}
	else if (pButton->IsEqualName(L"PTY"))
	{
		OnPTYClick();
	}
	else if (pButton->IsEqualName(L"PTY_search"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_PTY_SEEK);
	}
	else if (pButton->IsEqualName(L"back"))
	{
		if (!sysutil::nss_get_instance()->sys_rds_status)
		{
			CDlgManager::GetInstance()->ShowDlg(CRadioDlg_ID, SOURCE_FM);
		}
	}
	else if (pButton->IsEqualName(L"home"))
	{
		if (!sysutil::nss_get_instance()->sys_rds_status)
		{
			__super::OnBnClick(pButton);
		}
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}


void CRadioRDSDlg::OnRadioStatus()
{
	// ta alarm
	static BOOL bTAAlarm = FALSE;
	if (!tzutil::IsBoolEqual(bTAAlarm, protocol::get_mcu_radio_status()->is_TA_Alarm()))
	{
		bTAAlarm = protocol::get_mcu_radio_status()->is_TA_Alarm();
		ShowTAAlarm(bTAAlarm);
	}

	// rds alarm
	static BOOL bIsCurRDSAlarm = FALSE;
	if (!tzutil::IsBoolEqual(sysutil::nss_get_instance()->sys_rds_status, bIsCurRDSAlarm))
	{
		bIsCurRDSAlarm = sysutil::nss_get_instance()->sys_rds_status;
		OnRDSAlarm(bIsCurRDSAlarm);
	}

	__super::OnRadioStatus();
}

void CRadioRDSDlg::OnRDSAlarm(BOOL bAlarmOn)
{
	if (bAlarmOn)
	{
		// 如果导航在前台运行
		if (sysutil::nss_get_instance()->sys_gps_foreground)
		{
			::SetForegroundWindow(GetWindow()->m_hWnd);
		}
	}
	else
	{
		if (sysutil::nss_get_instance()->sys_camera_status
			||ui_is_on_bt_call()
			|| sysutil::nss_get_instance()->sys_ecar_status)
		{
			;	// 如果还在蓝牙通话,ECAR,或倒车,结束后会进入正确的状态,这里不用任何操作
		}
		else
		{
			// 如果导航在前台运行
			if (sysutil::nss_get_instance()->sys_gps_foreground)
			{
				tzutil::SetProcessForeground(sysutil::nss_get_instance()->gps_process_id);
			}
		}
	}
}

BOOL _is_AM_band(BYTE band);
void _format_freq_value(LPTSTR buffer, WORD freq, BYTE band);
void CRadioRDSDlg::OnRadioPresetInfo()
{
	// 更新当前频点与频点单位
// 	MCU_RADIO_PRESET_INFO* pinfo = protocol::get_mcu_radio_presetinfo();
// 	TCHAR freq[16];
// 	_format_freq_value(freq, pinfo->cur_freq, pinfo->band);
// 	m_pLayerCurFreq->SetText(freq);
// 	m_pLayerFreqUnit->SetTextResID(_is_AM_band(pinfo->band) ? L"RADIO_FREQ_AM_UNIT" : L"RADIO_FREQ_FM_UNIT");

	__super::OnRadioPresetInfo();
}

void CRadioRDSDlg::ShowPSPTYInfo()
{
	CWceUiLayer *pPS = GetChildByName(L"PS_cur");
	CWceUiLayer *pPTY = GetChildByName(L"PTY_cur");
	if (pPS)
	{
		TCHAR PS[16] = L"";
		char *pps = protocol::get_mcu_radio_ps_pty()->PS;
		MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, pps,
			strlen(pps), PS, sizeof(PS)/sizeof(TCHAR));
		pPS->SetText(PS);
	}

	if (pPTY)
	{
		int index = protocol::get_mcu_radio_ps_pty()->PTY;
		if (index>0 && index<sizeof(RDS_PROGRAM_TYPE)/sizeof(RDS_PROGRAM_TYPE[0]))
		{
			pPTY->SetText(GetPTYString(index));
		}
		else
		{
			pPTY->SetText(L"");
		}
	}
}

void CRadioRDSDlg::ShowStereo(BOOL bShow)
{
	CWceUiButton *pLayerST = (CWceUiButton*)GetChildByName(L"stereo");
	if (pLayerST)
	{
		pLayerST->ShowLayer(bShow);
	}

}

BOOL CRadioRDSDlg::IsTopMostDlg()
{
	return m_bIsTopMostDlg;
}

void CRadioRDSDlg::SetTopMostDlg(BOOL bIsTopMostDlg)
{
	m_bIsTopMostDlg = bIsTopMostDlg;
}

void CRadioRDSDlg::OnDlgShow(BOOL bShow)
{
	__super::OnDlgShow(bShow);

	CWceUiLayer* player = GetChildByName(L"AF_status");
	if (player)
	{
		if (protocol::get_mcu_sys_para()->get_fm_cur_region() == FQ_SAMER1)	// 北美
		{
			player->ShowLayer(FALSE);
		}
		else
		{
			player->ShowLayer(TRUE);
		}
	}

	player = GetChildByName(L"title");
	if (player)
	{
		if (protocol::get_mcu_sys_para()->get_fm_cur_region() == FQ_SAMER1)	// 北美
		{
			player->SetTextResID(L"RADIO_RBDS");
		}
		else
		{
			player->SetTextResID(L"RADIO_RDS");
		}
	}

}