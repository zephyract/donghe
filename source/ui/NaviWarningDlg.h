#pragma once
#include "WceUiButton.h"
#include "BaseDlg.h"

#define TIMER_ID_GOTO_LAST_DLG 1002

class CNaviWarningDlg : public CBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CNaviWarningDlg, CBaseDlg)
public:
	CNaviWarningDlg(void);
	~CNaviWarningDlg(void);	

	virtual void OnInitDialog();
	virtual void OnDlgShow(BOOL bShow);
	virtual void OnTimer(WORD wIDEvent);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnBnClick(CWceUiButton* pButton);

	//
	DLG_ID m_LastDlgID;
	SOURCE_ID m_LastDlgSourceID;
};
WCEUI_DYNCREATE_END(CNaviWarningDlg, CBaseDlg);
