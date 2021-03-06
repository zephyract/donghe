#include "stdafx.h"
#include "WceUiListBox.h"
#include "WceUiBase.h"
#include "FacLanguageDlg.h"
#include "WceUiLoadString.h"
#include "sysUtil.h"
#include "MainWnd.h"
#include "tzUtil.h"
#include "config.h"
#include "MainDlg.h"
#include "interface.h"

CFacLanguageDlg::CFacLanguageDlg(void)
{

}

CFacLanguageDlg::~CFacLanguageDlg(void)
{

}

void CFacLanguageDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitLayer();
}

void CFacLanguageDlg::OnReset()
{
	InitLayer();
}

void CFacLanguageDlg::InitLayer()
{
	CWceUiScrollLayer* pLayer = (CWceUiScrollLayer*)GetChildByName(L"frame");
	if (pLayer)
	{
		pLayer->SetScrollMode(SL_VERT);
		pLayer->SetExtraScroll(30);
		pLayer->SetScrollRange(0, 480);

		TCHAR layer_name[16];
		for (int i=0; i<config::get_language_table_count(); i++)
		{
			_stprintf(layer_name, L"btn%d", i);
			CWceUiLayer* pchild = pLayer->GetChildByName(layer_name);
			if (pchild)
			{
				pchild->SetText(config::get_language_info(i)->name);
				CheckButton((CWceUiButton*)pchild, config::get_language_info(i)->support);
			}
		}
	}

	
}

void CFacLanguageDlg::UpdateDefaultLanguage()
{
	CWceUiLayer* pLayerValue = GetLayerByName(L"value_language");
	if (pLayerValue)
	{
		int lan_id = config::get_language_default();
		LANGUAGE_TABLE_INFO* info = config::get_language_info(lan_id);
		if (info)
		{
			pLayerValue->SetText(info->name);
		}
	}
}

void CFacLanguageDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		UpdateDefaultLanguage();
	}

	__super::OnDlgShow(bShow);
}


void CFacLanguageDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetFactoryDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"language"))
	{
		CDlgManager::GetInstance()->ShowDlg(CFacDefaultLanDlg_ID, SOURCE_SETTING);
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

BOOL CFacLanguageDlg::OnFunctionsClick(CWceUiButton* pButton)
{
	if(pButton && pButton->GetParent()->IsEqualName(L"frame"))
	{
		TCHAR layer_name[16];
		for (int i=0; i<config::get_language_table_count(); i++)
		{
			_stprintf(layer_name, L"btn%d", i);
			if (pButton->IsEqualName(layer_name))
			{
				if (config::get_language_info(i)->support == LAN_SUPPORT)
				{
					CheckButton(pButton, FALSE);
					config::get_language_info(i)->support = LAN_UNSUPPORT;
				}
				else
				{
					CheckButton(pButton, TRUE);
					config::get_language_info(i)->support = LAN_SUPPORT;
				}
				break;
			}
		}


		return TRUE;
	}

	return FALSE;
}

void CFacLanguageDlg::CheckButton(CWceUiButton* pButton, BOOL bCheck)
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
