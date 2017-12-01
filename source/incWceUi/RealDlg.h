// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once

#include "RealWnd.h"

class CRealDlg : public CRealWnd
{
	WCEUI_DYNCREATE_BEGIN(CRealDlg, CRealWnd)
public:
	CRealDlg();
	virtual ~CRealDlg(void);

	//mode less
	BOOL CreateModeless(DWORD dwStyle, const RECT& rect, HWND hWndParent);
	BOOL EndModeless();

	//modal
	virtual INT_PTR DoModal(DWORD dwStyle, const RECT& rect, HWND hWndParent);
	BOOL EndDialog(int nResult);

	//virtual functions
	virtual void OnInitDialog();

	WCEUI_DLG_RUN_WAY GetDlgRunWay() const;
private:
	INT_PTR m_nDlgRst;
	BOOL	m_bQuit;
	WCEUI_DLG_RUN_WAY m_DlgRunWay;

private:
	CRealDlg (const CRealDlg &);
	CRealDlg & operator=(const CRealDlg &);
};

WCEUI_DYNCREATE_END(CRealDlg, CRealWnd)

