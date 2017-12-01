#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

class CSetLanguageDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetLanguageDlg, CSetBaseDlg)
public:
	CSetLanguageDlg(void);
	~CSetLanguageDlg(void);	

	virtual void OnInitDialog();
	virtual void OnDlgShow(BOOL bShow);
protected:
	virtual void OnReset();
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	void OnBnClick(CWceUiButton* pButton);

	void InitListbox();
};
WCEUI_DYNCREATE_END(CSetLanguageDlg, CSetBaseDlg);
