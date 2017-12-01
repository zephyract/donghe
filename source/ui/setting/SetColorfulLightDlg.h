#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

class CSetColorfulLightDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetColorfulLightDlg, CSetBaseDlg)
public:
	CSetColorfulLightDlg(void);
	~CSetColorfulLightDlg(void);	

	virtual void OnInitDialog();
	virtual void OnDlgShow(BOOL bShow);
protected:
	virtual void OnReset();
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	void OnBnClick(CWceUiButton* pButton);

	void InitListbox();
};
WCEUI_DYNCREATE_END(CSetColorfulLightDlg, CSetBaseDlg);
