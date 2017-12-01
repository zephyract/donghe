// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiObject.h"

void __stdcall WceUiSetWindowText(HWND hWndCtrl, LPCTSTR lpszNew);

class CRealGenericWnd : public CWceUiObject
{
	WCEUI_DYNAMIC_BEGIN(CRealGenericWnd, CWceUiObject)
public:
	CRealGenericWnd(void);
	virtual ~CRealGenericWnd(void);

	DWORD GetStyle() const;
	DWORD GetExStyle() const;

	BOOL UpdateWindow();
	BOOL SetForegroundWindow();
	void SetRedraw(BOOL bRedraw);

	LRESULT SendMessage(UINT message,WPARAM wParam = 0,LPARAM lParam = 0);
	BOOL PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
	BOOL SetWindowPos( HWND WndInsertAfter, int x, int y, int cx, int cy, UINT nFlags );

	void InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE);

	UINT_PTR SetTimer( UINT_PTR nIDEvent, UINT nElapse, TIMERPROC lpfnTimer );
	BOOL KillTimer(UINT_PTR nIDEvent);

	void GetWindowRect(LPRECT lpRect) const;
	void GetClientRect(LPRECT lpRect) const;
	void ScreenToClient(LPPOINT lpPoint) const;
	void ScreenToClient( LPRECT lpRect ) const; 

	BOOL ShowWindow(int nCmdShow);
	BOOL EnableWindow(BOOL bEnable = TRUE);

	HWND GetParent();

	HDC GetDC();
	HDC GetWindowDC();
	int ReleaseDC(HDC dc);

	void SetWindowText( LPCTSTR lpszString );
	void CenterWindow(HWND pAlternateOwner = NULL);

	void SetCanInvalidate(BOOL bCanInvalidate);
	BOOL IsCanInvalidate() const;

public:
	HWND m_hWnd;

private:
	BOOL  m_bCanInvalidate;

private:
	CRealGenericWnd (const CRealGenericWnd &);
	CRealGenericWnd & operator=(const CRealGenericWnd &);
};
WCEUI_DYNAMIC_END(CRealGenericWnd, CWceUiObject)




