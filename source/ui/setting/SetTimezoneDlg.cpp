#include "stdafx.h"
#include "WceUiListBox.h"
#include "WceUiBase.h"
#include "SetTimezoneDlg.h"
#include "config.h"
#include "interface.h"
#include "sysUtil.h"
#include "WceUiLoadString.h"
#include "WceUiRealWnd.h"

CSetTimezoneDlg::CSetTimezoneDlg(void)
{
}

CSetTimezoneDlg::~CSetTimezoneDlg(void)
{

}

void CSetTimezoneDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitListbox();
}

void CSetTimezoneDlg::InitListbox()
{
	CWceUiListBox *plist = (CWceUiListBox*)GetLayerByName(L"listbox");
	if (plist)
	{
		plist->ResetContent();
		plist->SetExtraScroll(30);

		// 当前时区置顶并选中
		int timezone_id = sysutil::nss_get_instance()->ui_timezone_id;
		ASSERT(timezone_id>=0 && timezone_id<config::get_timezone_table_count());

		int index = plist->AddString(GetTimeZoneText(timezone_id));
		plist->SetItemData(index, timezone_id);		// 时区ID保存在item DATA中
		plist->SetCurSel(index);
	
		// 从时区表中获取支持哪些时区
		TIMEZONE_TABLE_INFO* table = config::get_timezone_table();
		for (int i=0; i<config::get_timezone_table_count(); i++)
		{
			// 支持的时区并且不是当前时区
			if (table[i].support == TIMEZONE_SUPPORT && i != timezone_id )
			{
				int index = plist->AddString(GetTimeZoneText(i));
				plist->SetItemData(index, i);		// 时区ID保存在item DATA中
			}
		}

	}
}

void CSetTimezoneDlg::OnReset()
{
	InitListbox();
}

void CSetTimezoneDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	int index = plist->GetCurSel();
	int id = (int)plist->GetItemData(index);
	sysutil::nss_get_instance()->ui_timezone_id = id;
	ui_set_timezone(id);
	GetWindow()->PostMessage(UI_MSG_TIMEZONE_CHANGE, 0, 0);
}

LPCTSTR CSetTimezoneDlg::GetTimeZoneText(int nValue/*0 ~ 75*/)
{
	static TCHAR text[64];
	TCHAR resid[16];
	CWceUiLoadString str;
	text[0] = L'\0';
	if (nValue>=0 && nValue<=75)
	{
		_stprintf(resid, L"tz_%02d", nValue+1);
		str.SetTextResID(resid);
		_tcscpy(text, str.GetString());
	}

	return text;
}


void CSetTimezoneDlg::OnBnClick(CWceUiButton* pButton)
{

	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetDateTimeDlg_ID, SOURCE_SETTING);
		return;
	}

	__super::OnBnClick(pButton);
}