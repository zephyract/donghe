#pragma once
#include "AnimDlg.h"
class CCalendarDlg :	public CAnimDlg
{
	WCEUI_DYNCREATE_BEGIN(CCalendarDlg, CAnimDlg)

public:
	CCalendarDlg(void);
	~CCalendarDlg(void);
     virtual void OnInitDialog();
	 virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnBnClick(CWceUiButton* pButton);

};
WCEUI_DYNCREATE_END(CCalendarDlg, CAnimDlg)