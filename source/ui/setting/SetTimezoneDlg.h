#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

class CSetTimezoneDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetTimezoneDlg, CSetBaseDlg)
public:
	CSetTimezoneDlg(void);
	~CSetTimezoneDlg(void);	

	virtual void OnInitDialog();

	static LPCTSTR GetTimeZoneText(int nValue/*0 ~ 75*/);
protected:
	virtual void OnReset();
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	virtual void OnBnClick(CWceUiButton* pButton);

	void InitListbox();
};
WCEUI_DYNCREATE_END(CSetTimezoneDlg, CSetBaseDlg);
