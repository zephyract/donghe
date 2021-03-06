#include "stdafx.h"
#include "WceUiListBox.h"
#include "WceUiBase.h"
#include "config.h"
#include "interface.h"
#include "sysUtil.h"
#include "RadioPtyDlg.h"
#include "RadioRDSDlg.h"
#include "RpcMCU.h"

int CRadioPtyDlg::m_scrollPos = -1;

CRadioPtyDlg::CRadioPtyDlg(void)
{
}

CRadioPtyDlg::~CRadioPtyDlg(void)
{

}

void CRadioPtyDlg::OnInitDialog()
{
	__super::OnInitDialog();

	CWceUiLayer *pLayer = GetLayerByName(L"title");
	if (pLayer)
	{
		pLayer->SetText(L"PTY");
	}
}


void CRadioPtyDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		InitListbox();
	}
	else
	{
		CWceUiListBox *plist = (CWceUiListBox*)GetLayerByName(L"listbox");
		if (plist)
		{
			m_scrollPos = plist->GetScrollPos();
		}
	}

	__super::OnDlgShow(bShow);
}

void CRadioPtyDlg::InitListbox()
{
	CWceUiListBox *plist = (CWceUiListBox*)GetLayerByName(L"listbox");
	if (plist)
	{
		plist->ResetContent();
		plist->SetExtraScroll(30);

		for (int i=0; i<CRadioRDSDlg::GetPTYCount(); i++)
		{
			plist->AddString(CRadioRDSDlg::GetPTYString(i));			
		}
		plist->SetCurSel(sysutil::nss_get_instance()->ui_rds_pty);
		if (m_scrollPos < 0)
		{
			plist->SetTopIndex(sysutil::nss_get_instance()->ui_rds_pty);
		}
		else
		{
			plist->ScrollWindow(m_scrollPos);
		}
	}
}


void CRadioPtyDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	sysutil::nss_get_instance()->ui_rds_pty = plist->GetCurSel();
	CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_PTY, sysutil::nss_get_instance()->ui_rds_pty<<24);
}


void CRadioPtyDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CRadioRDSDlg_ID, SOURCE_FM);
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}