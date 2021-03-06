#include "stdafx.h"
#include "WceUiBase.h"
#include "SetSWCDlg.h"
#include "config.h"
#include "interface.h"
#include "sysUtil.h"
#include "RpcMCU.h"
#include "interface.h"

CSetSWCDlg::CSetSWCDlg(void)
{
}

CSetSWCDlg::~CSetSWCDlg(void)
{

}

void CSetSWCDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(UI_MSG_MCU_SWC_TABLE);
	RegisterMsg(UI_MSG_MCU_SWC_KEY_RESULT);

	// 须按MCU的顺序
	TCHAR layer_name[16];
	for (int i=0; i<MAX_SWC_BUTTON; i++)
	{
		_stprintf(layer_name, L"btn%d", i+1);
		m_pButton[i] = (CWceUiButton*)GetChildByName(layer_name);
	}

	CRpcMCU::GetInstance()->RPC_SWCGetTable(protocol::get_mcu_swc_table());
	UpdateSWC(protocol::get_mcu_swc_table());
}

LRESULT CSetSWCDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_MCU_SWC_TABLE)
	{
		UpdateSWC((MCU_SWC_TABLE*)lParam);
	}
	else if (message == UI_MSG_MCU_SWC_KEY_RESULT)
	{
		if ((lParam&0x0000FFFF))	// LOWORD为1表示已保存
		{
			ShowWarning(TRUE);
		}
		else
		{
			ShowWarning(FALSE);
		}
	}
	
	return __super::OnMessage(message, wParam, lParam);
}

void CSetSWCDlg::ShowWarning(BOOL bWarning)
{
	CWceUiLayer* ptip = GetChildByName(L"tip");
	if (ptip)
	{
		if (bWarning)
		{
			ptip->GetStyle()->SetTextColorFromApp(RGB(255,0,0));
			ptip->SetTextResID(L"SET_SWC_WARN");
		}
		else
		{
			ptip->GetStyle()->SetTextColorFromApp(RGB(255,255,255));
			ptip->SetTextResID(L"SET_SWC_TIP");
		}
	}
}

void CSetSWCDlg::UpdateSWC(MCU_SWC_TABLE* psw_table)
{
	TCHAR style_name[16];
	for (int i=0; i<12; i++)
	{
		//RETAILMSG(1, (L"SWC table: flag=%d,  key=%d, \n", psw_table->key[i][0], psw_table->key[i][1]));
		if (psw_table->key[i][0])		// flag
		{
			if (m_pButton[i])
			{
				_stprintf(style_name, L"btn%d_down", i+1);
				m_pButton[i]->SetStatusStyleName(style_name, CWceUiButton::STATUS_NORMAL);
			}
		}
		else
		{
			if (m_pButton[i])
			{
				_stprintf(style_name, L"btn%d_normal", i+1);
				m_pButton[i]->SetStatusStyleName(style_name, CWceUiButton::STATUS_NORMAL);
			}
		}
	}
}

// 注意资源文件中每个btn的图片配置顺序与这个顺序要保持一致, 如果MCU变了,资源包中的btn的图片配置也要变
// 当前版本MCU的方控按钮设置顺序为:
// seekup, seekdown, volinc, poweroff, voldec, mute, hangin, hangup, mode, nav, radio, dvd
// 0x73, 0x72, 0x04,   0x01,  0x05,   0x02, 0x06,    0x07,   0x08, 0x43, 0x44, 0x45
void CSetSWCDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"reset"))
	{
		CRpcMCU::GetInstance()->RPC_SWCCommand(SWC_STUDY_RESET);
	}
	else
	{
		for (int i=0; i<MAX_SWC_BUTTON; i++)
		{
			if (pButton == m_pButton[i])
			{
				CRpcMCU::GetInstance()->RPC_KeyCommand(T_SEND_WHEEL_STUDY, (i+1)<<24);
			}
		}
	}

	__super::OnBnClick(pButton);
}

void CSetSWCDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		CRpcMCU::GetInstance()->RPC_SWCCommand(SWC_STUDY_START);
	}
	else
	{
		CRpcMCU::GetInstance()->RPC_SWCCommand(SWC_STUDY_END);
	}

	// tip
	CWceUiLayer *ptip = GetChildByName(L"tip");
	if (ptip)
	{
		if (sysutil::nss_get_instance()->ui_lan_id == LAN_ENGLISH)
		{
			ptip->GetStyle()->GetDrawTextCtrl()->SetFormat(DT_LEFT);
		}
		else
		{
			ptip->GetStyle()->GetDrawTextCtrl()->SetFormat(DT_LEFT | DT_VCENTER);
		}
	}

	__super::OnDlgShow(bShow);
}