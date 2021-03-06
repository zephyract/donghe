#include "stdafx.h"
#include "WceUiListBox.h"
#include "WceUiBase.h"
#include "FacFunctionsDlg.h"
#include "WceUiLoadString.h"
#include "sysUtil.h"
#include "MainWnd.h"
#include "tzUtil.h"
#include "config.h"
#include "MainDlg.h"
#include "interface.h"

CFacFunctionsDlg::CFacFunctionsDlg(void)
{

}

CFacFunctionsDlg::~CFacFunctionsDlg(void)
{

}

void CFacFunctionsDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitLayer();
}

void CFacFunctionsDlg::InitLayer()
{
	CWceUiScrollLayer* pLayer = (CWceUiScrollLayer*)GetChildByName(L"frame");
	if (pLayer)
	{
		pLayer->SetScrollMode(SL_VERT);
		pLayer->SetExtraScroll(30);
		pLayer->SetScrollRange(0, 1344);

		CWceUiButton* pInd = (CWceUiButton*)pLayer->FirstChild();
		while (pInd)
		{
			CheckButton(pInd, config::get_config_functions()->is_support(pInd->GetName()));
			pInd = (CWceUiButton*)pLayer->NextChild(pInd);
		}
	}
}

void CFacFunctionsDlg::OnDlgShow(BOOL bShow)
{
	if (!bShow)
	{
		CMainWnd::GetInstance()->PostMessage(UI_MSG_FACTORY_SET_CHANGE);
	}

	__super::OnDlgShow(bShow);
}

void CFacFunctionsDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	int index = plist->GetCurSel();
}


void CFacFunctionsDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetFactoryDlg_ID, SOURCE_SETTING);
	}
	else if (OnFunctionsClick(pButton))
	{
		return;
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}

BOOL CFacFunctionsDlg::OnFunctionsClick(CWceUiButton* pButton)
{
	if(pButton && pButton->GetParent()->IsEqualName(L"frame"))
	{
		if (config::get_config_functions()->is_support(pButton->GetName()))
		{
			CheckButton(pButton, FALSE);
			config::get_config_functions()->remove(pButton->GetName());
		}
		else
		{
			CheckButton(pButton, TRUE);
			config::get_config_functions()->add(pButton->GetName());
		}

		return TRUE;
	}

	return FALSE;
}

void CFacFunctionsDlg::CheckButton(CWceUiButton* pButton, BOOL bCheck)
{
	if (pButton)
	{
		TCHAR style[MAX_TAG_NAME_LEN];
		if(bCheck)
		{
			_stprintf(style, L"%s_check", pButton->GetName());
			pButton->SetStatusStyleName(style, CWceUiButton::STATUS_NORMAL);
		}
		else
		{
			_stprintf(style, L"%s_normal", pButton->GetName());
			pButton->SetStatusStyleName(style, CWceUiButton::STATUS_NORMAL);
		}
	}
}
