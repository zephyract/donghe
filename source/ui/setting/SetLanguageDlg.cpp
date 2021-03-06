#include "stdafx.h"
#include "WceUiListBox.h"
#include "WceUiBase.h"
#include "SetLanguageDlg.h"
#include "config.h"
#include "interface.h"
#include "sysUtil.h"

CSetLanguageDlg::CSetLanguageDlg(void)
{
}

CSetLanguageDlg::~CSetLanguageDlg(void)
{

}

void CSetLanguageDlg::OnInitDialog()
{
	__super::OnInitDialog();
}


void CSetLanguageDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		InitListbox();
	}

	__super::OnDlgShow(bShow);
}

void CSetLanguageDlg::OnReset()
{
//	InitListbox();
	__super::OnReset();
}

void CSetLanguageDlg::InitListbox()
{
	CWceUiListBox *plist = (CWceUiListBox*)GetLayerByName(L"listbox");
	if (plist)
	{
		plist->ResetContent();
		plist->SetExtraScroll(30);

		// 当前语言置顶并选中
		int lan_id = sysutil::nss_get_instance()->ui_lan_id;
		LANGUAGE_TABLE_INFO* info = config::get_language_info(lan_id);
		if (info)
		{
			int index = plist->AddString(info->name);
			plist->SetItemData(index, info->id);		// 语言ID保存在item DATA中
			plist->SetCurSel(index);
		}
		else
		{
			RETAILMSG(1, (L"[nvai_car]: could not find language id in table.\n"));
		}

		// 从语言表中获取支持哪些语言
		LANGUAGE_TABLE_INFO* table = config::get_language_table();
		for (int i=0; i<config::get_language_table_count(); i++)
		{
			// 支持的语言并且不是当前语言
			if (table[i].support == LAN_SUPPORT && table[i].id != lan_id )
			{
				int index = plist->AddString(table[i].name);
				plist->SetItemData(index, table[i].id);		// 语言ID保存在item DATA中
			}
		}

	}
}


void CSetLanguageDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	int index = plist->GetCurSel();
	int language_id = (int)plist->GetItemData(index);
	::PostMessage(HWND_BROADCAST, NAVI_MSG_LAN_CHANGE, 0, (LPARAM)language_id);
}


void CSetLanguageDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetGeneralDlg_ID, SOURCE_SETTING);
		return;
	}

	__super::OnBnClick(pButton);
}