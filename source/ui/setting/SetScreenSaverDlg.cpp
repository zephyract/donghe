#include "stdafx.h"
#include "WceUiListBox.h"
#include "WceUiBase.h"
#include "SetScreenSaverDlg.h"
#include "WceUiLoadString.h"
#include "sysUtil.h"
#include "MainWnd.h"

CSetScreenSaverDlg::CSetScreenSaverDlg(void)
{

}

CSetScreenSaverDlg::~CSetScreenSaverDlg(void)
{

}

void CSetScreenSaverDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitLayer();
}

void CSetScreenSaverDlg::InitLayer()
{
	CWceUiScrollLayer* pLayer = (CWceUiScrollLayer*)GetChildByName(L"frame");
	if (pLayer)
	{
		pLayer->SetScrollMode(SL_VERT);
		pLayer->SetExtraScroll(30);
		pLayer->SetScrollRange(0, 180);
	}

	CheckButton();
}


void CSetScreenSaverDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	int index = plist->GetCurSel();
}


void CSetScreenSaverDlg::OnBnClick(CWceUiButton* pButton)
{

	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetGeneralDlg_ID, SOURCE_SETTING);
		return;
	}
	else if (pButton->IsEqualName(L"off"))
	{
		sysutil::nss_get_instance()->ui_screen_saver_mode = 0;
	}
	else if (pButton->IsEqualName(L"blackscreen"))
	{
		sysutil::nss_get_instance()->ui_screen_saver_mode = 1;
	}
	else if (pButton->IsEqualName(L"clock"))
	{
		sysutil::nss_get_instance()->ui_screen_saver_mode = 2;
	}
	else if (pButton->IsEqualName(L"time1"))
	{
		sysutil::nss_get_instance()->ui_screen_saver_time = SCREEN_SAVER_WAIT_TIME1;
	}
	else if (pButton->IsEqualName(L"time2"))
	{
		sysutil::nss_get_instance()->ui_screen_saver_time = SCREEN_SAVER_WAIT_TIME2;
	}	else if (pButton->IsEqualName(L"time3"))
	{
		sysutil::nss_get_instance()->ui_screen_saver_time = SCREEN_SAVER_WAIT_TIME3;
	}	else if (pButton->IsEqualName(L"time4"))
	{
		sysutil::nss_get_instance()->ui_screen_saver_time = SCREEN_SAVER_WAIT_TIME4;
	}

	CheckButton();
	CMainWnd::GetInstance()->RestartScreenSaver();

	__super::OnBnClick(pButton);
}

void CSetScreenSaverDlg::OnReset()
{
	CheckButton();
}

void CSetScreenSaverDlg::CheckButton()
{
	CWceUiButton* pButtonOff = (CWceUiButton*)GetLayerByName(L"off");
	CWceUiButton* pButtonBlackScreen = (CWceUiButton*)GetLayerByName(L"blackscreen");
	CWceUiButton* pButtonClock = (CWceUiButton*)GetLayerByName(L"clock");

	CWceUiButton* pButtonTime1 = (CWceUiButton*)GetLayerByName(L"time1");
	CWceUiButton* pButtonTime2 = (CWceUiButton*)GetLayerByName(L"time2");
	CWceUiButton* pButtonTime3 = (CWceUiButton*)GetLayerByName(L"time3");
	CWceUiButton* pButtonTime4 = (CWceUiButton*)GetLayerByName(L"time4");

	pButtonOff->SetStatusStyleName(L"off_normal", CWceUiButton::STATUS_NORMAL);
	pButtonBlackScreen->SetStatusStyleName(L"blackscreen_normal", CWceUiButton::STATUS_NORMAL);
	pButtonClock->SetStatusStyleName(L"clock_normal", CWceUiButton::STATUS_NORMAL);
	pButtonTime1->SetStatusStyleName(L"time1_normal", CWceUiButton::STATUS_NORMAL);
	pButtonTime2->SetStatusStyleName(L"time2_normal", CWceUiButton::STATUS_NORMAL);
	pButtonTime3->SetStatusStyleName(L"time3_normal", CWceUiButton::STATUS_NORMAL);
	pButtonTime4->SetStatusStyleName(L"time4_normal", CWceUiButton::STATUS_NORMAL);

	switch (sysutil::nss_get_instance()->ui_screen_saver_mode)
	{
	case 0:
		pButtonOff->SetStatusStyleName(L"off_check", CWceUiButton::STATUS_NORMAL);
		break;
	case 1:
		pButtonBlackScreen->SetStatusStyleName(L"blackscreen_check", CWceUiButton::STATUS_NORMAL);
		break;
	case 2:
	default:
		pButtonClock->SetStatusStyleName(L"clock_check", CWceUiButton::STATUS_NORMAL);
		break;
	}

	switch (sysutil::nss_get_instance()->ui_screen_saver_time)
	{
	case SCREEN_SAVER_WAIT_TIME1:
		pButtonTime1->SetStatusStyleName(L"time1_check", CWceUiButton::STATUS_NORMAL);
		break;
	case SCREEN_SAVER_WAIT_TIME2:
		pButtonTime2->SetStatusStyleName(L"time2_check", CWceUiButton::STATUS_NORMAL);
		break;
	case SCREEN_SAVER_WAIT_TIME3:
		pButtonTime3->SetStatusStyleName(L"time3_check", CWceUiButton::STATUS_NORMAL);
		break;
	case SCREEN_SAVER_WAIT_TIME4:
	default:
		pButtonTime4->SetStatusStyleName(L"time4_check", CWceUiButton::STATUS_NORMAL);
		break;
	}
}
