#include "stdafx.h"
#include "SetBluetoothDlg.h"
#include "tzUtil.h"
#include "sysUtil.h"
#include "WceUiBase.h"
#include "WceUiRealWnd.h"
#include "bluetooth.h"
#include "PasswordDlg.h"

CSetBluetoothDlg::CSetBluetoothDlg(void)
{

}

CSetBluetoothDlg::~CSetBluetoothDlg(void)
{

}

void CSetBluetoothDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitLayers();
}


void CSetBluetoothDlg::InitLayers()
{
	CWceUiScrollLayer* pLayer = (CWceUiScrollLayer*)GetChildByName(L"frame");
	if (pLayer)
	{
		pLayer->SetScrollMode(SL_VERT);
		pLayer->SetExtraScroll(30);
	}

	// 自动连接
	CWceUiSButton* psbutton = (CWceUiSButton*)GetLayerByName(L"auto_connect_switch");
	ASSERT(psbutton);
	psbutton->SetStatus(CBluetooth::GetInstance()->IsAutoConnect() ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);

	// 自动应答
	psbutton = (CWceUiSButton*)GetLayerByName(L"auto_answer_switch");
	ASSERT(psbutton);
	psbutton->SetStatus(CBluetooth::GetInstance()->IsAutoAnswer() ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);
}

void CSetBluetoothDlg::OnReset()
{
	InitLayers();
}

void CSetBluetoothDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"pincode"))	
	{
		LPCTSTR password = CPasswordDlg::GetPassword();
		if (password && _tcslen(password) == 4)
		{
			CBluetooth::GetInstance()->SetPinCode(password);
		}
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}

LRESULT CSetBluetoothDlg::OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam)
{
	return __super::OnCommand(uCommand, wParam, lParam);
}


LRESULT CSetBluetoothDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{

	return __super::OnMessage(message, wParam, lParam);
}

void CSetBluetoothDlg::OnSBnClick(CWceUiSButton* pSButton, int status)
{
	if (pSButton->IsEqualName(L"auto_connect_switch"))
	{
		CBluetooth::GetInstance()->EnableAutoConnect(status == WCEUI_SBUTTON_ON);
		sysutil::nss_get_instance()->bt_auto_connect = (status == WCEUI_SBUTTON_ON);
	}
	else if (pSButton->IsEqualName(L"auto_answer_switch"))
	{
		CBluetooth::GetInstance()->EnableAutoAnswer(status == WCEUI_SBUTTON_ON);
		sysutil::nss_get_instance()->bt_auto_answer = (status == WCEUI_SBUTTON_ON);
	}
}
