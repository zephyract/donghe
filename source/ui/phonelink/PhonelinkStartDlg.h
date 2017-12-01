#pragma once
#include "stdafx.h"
#include "AnimDlg.h"
#include "WceUiButton.h"

#define TIMER_ID_SET_GPIO_USB	3001

class CPhonelinkStartDlg: public CAnimDlg
{
	WCEUI_DYNCREATE_BEGIN(CPhonelinkStartDlg, CAnimDlg)
public:
	CPhonelinkStartDlg(void);
	~CPhonelinkStartDlg(void);

	virtual void OnInitDialog();
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);

	
};

WCEUI_DYNCREATE_END(CPhonelinkStartDlg, CAnimDlg);
