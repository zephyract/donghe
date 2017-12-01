#pragma once
#include "stdafx.h"
#include "AnimDlg.h"
#include "WceUiButton.h"

class CPhonelinkWarningDlg: public CAnimDlg
{
	WCEUI_DYNCREATE_BEGIN(CPhonelinkWarningDlg, CAnimDlg)
public:
	CPhonelinkWarningDlg(void);
	~CPhonelinkWarningDlg(void);

	virtual void OnInitDialog();
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);

protected:
	virtual void OnLButtonDown(UINT nFlags, POINT point);
};

WCEUI_DYNCREATE_END(CPhonelinkWarningDlg, CAnimDlg);
