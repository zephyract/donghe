#include "stdafx.h"
#include "WceUiListBox.h"
#include "WceUiBase.h"
#include "FacDefaultLanDlg.h"
#include "config.h"
#include "interface.h"
#include "sysUtil.h"

CFacDefaultLanDlg::CFacDefaultLanDlg(void)
{
}

CFacDefaultLanDlg::~CFacDefaultLanDlg(void)
{

}

void CFacDefaultLanDlg::OnInitDialog()
{
	__super::OnInitDialog();
}

void CFacDefaultLanDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		InitListbox();
	}

	__super::OnDlgShow(bShow);
}

void CFacDefaultLanDlg::OnReset()
{
//	InitListbox();
	__super::OnReset();
}

void CFacDefaultLanDlg::InitListbox()
{
	CWceUiListBox *plist = (CWceUiListBox*)GetLayerByName(L"listbox");
	if (plist)
	{
		plist->ResetContent();
		plist->SetExtraScroll(30);

		// 从语言表中获取支持哪些语言
		LANGUAGE_TABLE_INFO* table = config::get_language_table();
		for (int i=0; i<config::get_language_table_count(); i++)
		{
			// 支持的语言并且不是当前语言
			if (table[i].support == LAN_SUPPORT)
			{
				int index = plist->AddString(table[i].name);
				plist->SetItemData(index, table[i].id);		// 语言ID保存在item DATA中

				if (table[i].id == config::get_language_default())
				{
					plist->SetCurSel(index);
				}
			}
		}

	}
}


void CFacDefaultLanDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	int index = plist->GetCurSel();
	int language_id = (int)plist->GetItemData(index);
	config::set_language_default(language_id);
}

void CFacDefaultLanDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CFacLanguageDlg_ID, SOURCE_SETTING);
		return;
	}

	__super::OnBnClick(pButton);
}