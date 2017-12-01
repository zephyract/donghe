#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

#define TIMER_ID_INIT_LISTBOX	200
#define TIMER_ID_RETURN_SET_GPS	201

class CSetGpsPathDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetGpsPathDlg, CSetBaseDlg)
public:
	CSetGpsPathDlg(void);
	~CSetGpsPathDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	virtual void OnBnClick(CWceUiButton* pButton);

	void InitListbox();
	void AddFiles(LPCTSTR lpszFilePath);
	BOOL IsFindFile(LPCTSTR lpszFileName);

	void ResetData();
	void OnMassStoragePlug(UI_MASS_STORAGE_TYPE msType, UI_MASS_STORAGE_PLUG msAction);
};
WCEUI_DYNCREATE_END(CSetGpsPathDlg, CSetBaseDlg);
