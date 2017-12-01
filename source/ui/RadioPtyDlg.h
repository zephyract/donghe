#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

class CRadioPtyDlg : public CBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CRadioPtyDlg, CSetBaseDlg)
public:
	CRadioPtyDlg(void);
	~CRadioPtyDlg(void);	

	virtual void OnInitDialog();
	virtual void OnDlgShow(BOOL bShow);
protected:
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	void OnBnClick(CWceUiButton* pButton);

	void InitListbox();

	static int m_scrollPos;
};
WCEUI_DYNCREATE_END(CRadioPtyDlg, CSetBaseDlg);
