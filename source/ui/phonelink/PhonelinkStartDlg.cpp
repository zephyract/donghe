#include "StdAfx.h"
#include "PhonelinkStartDlg.h"
#include "ycapi.h"
#include "SourceManager.h"
#include "MainWnd.h"

CPhonelinkStartDlg::CPhonelinkStartDlg(void)
{

}

CPhonelinkStartDlg::~CPhonelinkStartDlg(void)
{

}

void CPhonelinkStartDlg::OnInitDialog()
{
	// 把USB口断电一下,相当于重新插拔了手机
 	YC_SetGpioMode(29, 1);
 	YC_SetGpioData(29, 0);
 	SetTimer(TIMER_ID_SET_GPIO_USB, 1000);

	__super::OnInitDialog();

}

void CPhonelinkStartDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_SET_GPIO_USB)
	{
		KillTimer(TIMER_ID_SET_GPIO_USB);
 		YC_SetGpioData(29, 1);

		//CSourceManager::GetInstance()->SetFrontSrc(SOURCE_PHONE_LINK);
		CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_PHONE_LINK);
	}

	__super::OnTimer(wIDEvent);
}

void CPhonelinkStartDlg::OnBnClick(PCWceUiButton pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
	}
	else if (pButton->IsEqualName(L"home"))
	{
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
	}

	__super::OnBnClick(pButton);
}


LRESULT CPhonelinkStartDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return __super::OnMessage(message, wParam, lParam);
}



void CPhonelinkStartDlg::OnDlgShow(BOOL bShow)
{

	__super::OnDlgShow(bShow);
}

