// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiGenericBitmap.h"
#include "RealDlg.h"


class CWceUiLayerCtrl;
class CWceUiLayer;			typedef CWceUiLayer*		PCWceUiLayer;
class CWceUiWndAnimate;		typedef CWceUiWndAnimate*	PCWceUiWndAnimate;

namespace wceui
{
	class CWceUiRealWndData;
}

class CWceUiRealWnd : public CRealDlg
{
	friend class CWceUiLayer;
	WCEUI_DYNCREATE_BEGIN(CWceUiRealWnd, CRealDlg)
public:
	CWceUiRealWnd();
	virtual ~CWceUiRealWnd();

	//
	//virtual functions
	//
	virtual void OnLButtonDown(UINT nFlags,	  POINT point);
	virtual void OnLButtonUp(UINT nFlags,	  POINT point);
	virtual void OnMouseMove(UINT nFlags,	  POINT point);
	virtual void OnLButtonDblClk(UINT nFlags, POINT point);

	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	virtual void OnCaptureChanged(HWND hwndNewCapture);

	virtual void OnPaint(HDC dc, RECT rcNeedPaint);
	virtual void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDestroy();

	//ignore WM_CLOSE message
	virtual void OnClose();

	virtual BOOL PreDrawWnd(CWceUiGenericBitmap* pBitmap);

public:
	CWceUiLayerCtrl* GetLayerCtrl() const;

	//window animate
public:
	CWceUiGenericBitmap* GetLastWndBmp();
	CWceUiGenericBitmap* GetCurWndBmp();

	BOOL Apply(CWceUiGenericBitmap* pFromBitmap = NULL);
	BOOL Play(CWceUiWndAnimate** ppWndAnimate, int nWndAnimate, BOOL bIgnoreInputMsg = TRUE);

	BOOL PaintWnd(CWceUiGenericBitmap* pWndBmp);
	
	void SetWndOffsetHue(int nDegree);
	int  GetWndOffsetHue() const;

public:
	void FreeBmpRes();

	//for layer
private:
	void RegisterMsg(UINT nMsg, CWceUiLayer* pLayer);
	void UnRegisterMsg(UINT nMsg, CWceUiLayer* pLayer);

	PCWceUiLayer SetCapture(PCWceUiLayer pCaptureLayer);
	void ReleaseCapture(PCWceUiLayer pCaptureLayer);
	PCWceUiLayer GetCapture() const;

private:
	CWceUiLayer* GetPtInLayer(POINT point);
	void InitLayerCtrl();
	void UnInitLayerCtrl();

	CWceUiGenericBitmap* CreateMixedBitmap();
	CWceUiGenericBitmap* GetOffsetHueBmp(CWceUiGenericBitmap *pBitmap, LPCRECT lpRect = NULL);

private:
	wceui::CWceUiRealWndData* GetData() const;
	wceui::CWceUiRealWndData* m_pData;

private:
	CWceUiRealWnd (const CWceUiRealWnd &);
	CWceUiRealWnd & operator=(const CWceUiRealWnd &);
};
WCEUI_DYNCREATE_END(CWceUiRealWnd, CRealDlg)
