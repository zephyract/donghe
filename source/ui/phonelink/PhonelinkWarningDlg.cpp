#include "StdAfx.h"
#include "PhonelinkWarningDlg.h"
#include "phonelink.h"
CPhonelinkWarningDlg::CPhonelinkWarningDlg(void)
{

}

CPhonelinkWarningDlg::~CPhonelinkWarningDlg(void)
{

}

void CPhonelinkWarningDlg::OnInitDialog()
{

	__super::OnInitDialog();

}

void CPhonelinkWarningDlg::OnTimer(WORD wIDEvent)
{
	__super::OnTimer(wIDEvent);
}

void CPhonelinkWarningDlg::OnBnClick(PCWceUiButton pButton)
{
	
	__super::OnBnClick(pButton);
}


LRESULT CPhonelinkWarningDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return __super::OnMessage(message, wParam, lParam);
}



void CPhonelinkWarningDlg::OnDlgShow(BOOL bShow)
{

	__super::OnDlgShow(bShow);
}

void CPhonelinkWarningDlg::OnLButtonDown(UINT nFlags, POINT point)
{
	CPhoneLink::GetInstance()->PostMessage2Phonelink(ID_UWM_PHONELINK_START_DRIVER_APP,0,0);
}