#include "stdafx.h"
#include "WceUiListBox.h"
#include "WceUiBase.h"
#include "SetGpsPathDlg.h"
#include "config.h"
#include "interface.h"
#include "sysUtil.h"
#include "filefind.h"

CSetGpsPathDlg::CSetGpsPathDlg(void)
{
}

CSetGpsPathDlg::~CSetGpsPathDlg(void)
{

}

void CSetGpsPathDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_STORAGE_CARD_PLUG);

	ResetData();
	SetTimer(TIMER_ID_INIT_LISTBOX, 100);

	__super::OnInitDialog();

}

void CSetGpsPathDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_INIT_LISTBOX)
	{
		if (!IsAnimating())		// 动画结束后才初始化列表
		{
			KillTimer(TIMER_ID_INIT_LISTBOX);
			InitListbox();
			CWceUiListBox *plist = (CWceUiListBox*)GetLayerByName(L"listbox");
			CWceUiLayer *ptip = GetLayerByName(L"tip");
			if (plist == NULL || plist->GetCount() <= 0)
			{
				if (ptip)
				{
					ptip->SetTextResID(L"SET_GPS_FILE_NOT_FOUND");
				}
			}
			else
			{
				if (ptip)
				{
					ptip->ShowLayer(FALSE);
				}
			}
		}
	}
	else if (wIDEvent == TIMER_ID_RETURN_SET_GPS)
	{
		KillTimer(TIMER_ID_RETURN_SET_GPS);
		CDlgManager::GetInstance()->ShowDlg(CSetGpsDlg_ID, SOURCE_SETTING);
	}

	__super::OnTimer(wIDEvent);
}

LRESULT CSetGpsPathDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_STORAGE_CARD_PLUG)
	{
		OnMassStoragePlug((UI_MASS_STORAGE_TYPE)wParam, (UI_MASS_STORAGE_PLUG)lParam);
	}

	return __super::OnMessage(message, wParam, lParam);
}

extern LPCTSTR _get_mstype_path(UI_MASS_STORAGE_TYPE msType);
void CSetGpsPathDlg::OnMassStoragePlug(UI_MASS_STORAGE_TYPE msType, UI_MASS_STORAGE_PLUG msAction)
{
	// GPS最多支持从三个盘符上搜索
	for (int i=0; i<MAX_DISK_GPS_PATH; i++)
	{
		if (config::get_config_disk()->gps[i][0] != L'\0')
		{
			LPCTSTR pdisk_name = _get_mstype_path(msType);
			if (pdisk_name && _tcsicmp(config::get_config_disk()->gps[i], pdisk_name) == 0)
			{
				ResetData();
				SetTimer(TIMER_ID_INIT_LISTBOX, 100);
			}
		}
	}
}



void CSetGpsPathDlg::InitListbox()
{
	CWceUiListBox *plist = (CWceUiListBox*)GetLayerByName(L"listbox");
	if (plist)
	{
		plist->ResetContent();
		plist->SetExtraScroll(30);

		// GPS最多支持从三个盘符上搜索
		for (int i=0; i<MAX_DISK_GPS_PATH; i++)
		{
			if (config::get_config_disk()->gps[i][0] != L'\0')
			{
				AddFiles(config::get_config_disk()->gps[i]);
			}
		}

		int index = plist->GetCurSel();
		if (index>=0 && !plist->IsItemVisible(index))
		{
			plist->SetTopIndex(index);
		}
	}
}


void CSetGpsPathDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	CWceUiListBox *plist_file = (CWceUiListBox*)GetLayerByName(L"listbox");
	if (plist == plist_file && nItemClicked>=0)
	{
		plist->GetText(nItemClicked, sysutil::nss_get_instance()->gps_path);
		SetTimer(TIMER_ID_RETURN_SET_GPS, 500);
	}
}

void CSetGpsPathDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
// 		ResetData();
// 		SetTimer(TIMER_ID_INIT_LISTBOX, 100);
	}

	__super::OnDlgShow(bShow);
}

void CSetGpsPathDlg::AddFiles(LPCTSTR lpszFilePath)
{
	CWceUiListBox *plist = (CWceUiListBox*)GetLayerByName(L"listbox");

	CFileFind finder;
	// build a string with wildcards
	CString strWildcard(lpszFilePath);
	strWildcard += L"\\*.*";

	// start working for directory files
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots())
		{
			AddFiles(finder.GetFilePath());		// call it recursively
			continue;
		}

		CString str = finder.GetFilePath();

		if (!IsFindFile(str))
		{
			continue;
		}
		else
		{
			if (plist)
			{
				int index = plist->AddString(str);
				if (_tcsicmp(str, sysutil::nss_get_instance()->gps_path) == 0)
				{
					plist->SetCurSel(index);
				}
			}
		}
	}
	finder.Close();
}


BOOL CSetGpsPathDlg::IsFindFile(LPCTSTR lpszFileName)
{
	CString str(lpszFileName);
	int pos = str.ReverseFind(L'.');
	if (pos != -1)
	{
		str = str.Right(str.GetLength()-pos-1);
		str.MakeLower();
		if (str.CompareNoCase(L"exe") == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CSetGpsPathDlg::ResetData()
{
	CWceUiListBox *plist = (CWceUiListBox*)GetLayerByName(L"listbox");
	CWceUiLayer *ptip = GetLayerByName(L"tip");
	if (plist)
	{
		plist->ResetContent();
	}

	if (ptip)
	{
		ptip->ShowLayer(TRUE);
		//ptip->SetTextResID(L"SET_GPS_FIND_FILE");
		ptip->SetText(L"");
	}
}


void CSetGpsPathDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetGpsDlg_ID, SOURCE_SETTING);
		return;
	}

	__super::OnBnClick(pButton);
}