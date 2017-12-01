// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "RealGenericWnd.h"

enum WCEUI_WND_PRIVATE_MSG
{
	WCEUI_WND_RELEASE_CAPTURE = (WM_USER + 704897426),
};

//the max number of messages can handle
void InvalidateAllWceUiWnd();

//ignore messages
BOOL WceUiAddIgnoreMsg(UINT nMsg);
void WceUiDelIgnoreMsg(UINT nMsg);
BOOL WceUiIsIgnoreMsg(const MSG* lpMsg);

class CRealWnd; typedef CRealWnd *PCRealWnd;

class CRealWnd : public CRealGenericWnd
{
	WCEUI_DYNCREATE_BEGIN(CRealWnd, CRealGenericWnd)
public:
	CRealWnd(void);
	virtual ~CRealWnd(void);

	BOOL Create(DWORD dwStyle, const RECT& rect, HWND hwndParent);
	BOOL DestroyWindow();

	void IgnoreMsgBeforeTime(DWORD dwBeforeThisTime = 0);

	//
	//virtual functions
	//
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
protected:
	virtual void	OnLButtonDown(UINT nFlags,   POINT point);
	virtual void	OnLButtonUp(UINT nFlags,	 POINT point);
	virtual void	OnMouseMove(UINT nFlags,	 POINT point);
	virtual void	OnLButtonDblClk(UINT nFlags, POINT point);
	virtual void	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void	OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	virtual void	OnCaptureChanged(HWND hwndNewCapture);
	virtual BOOL	OnEraseBkgnd(HDC dc);
	virtual void	OnPaint(HDC dc, RECT rcNeedPaint);
	virtual void	OnTimer(UINT_PTR nIDEvent);
	virtual	void	OnClose();
	virtual void	OnDestroy();
	virtual int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:
	CRealWnd (const CRealWnd &);
	CRealWnd & operator=(const CRealWnd &);
};
WCEUI_DYNCREATE_END(CRealWnd, CRealGenericWnd)
