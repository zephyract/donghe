// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiObject.h"
#include "WceUiStyle.h"
#include "WceUiXml.h"

class CWceUiStatic;		typedef CWceUiStatic*	 PCWceUiStatic;
class CWceUiButton;		typedef CWceUiButton*	 PCWceUiButton;
class CWceUiScroll;		typedef CWceUiScroll*	 PCWceUiScroll;
class CWceUiLayer;		typedef CWceUiLayer*	 PCWceUiLayer;
class CWceUiLayerCtrl;	typedef CWceUiLayerCtrl* PCWceUiLayerCtrl;
class CWceUiRealWnd;
class CWceUiDlg;


enum SLIDE_MODE
{
	SLIDE_NONE,	
	SLIDE_HORIZONTAL,
	SLIDE_VERTICAL,
	SLIDE_HOR_VER
};

namespace wceui
{
	class CWceUiLayerData;
}

class CWceUiLayer : public CWceUiObject
{
	friend class CWceUiLayerCtrl;
	friend class CWceUiDlg;
	friend class CWceUiStyle;
	WCEUI_DYNCREATE_BEGIN(CWceUiLayer, CWceUiObject)
public:
	CWceUiLayer();
	virtual ~CWceUiLayer();
public:

	// 设置滚动属性
	void SetSlideMode(int nMode);
	int GetSlideMode();

	// 设置控件是否正在滚动标志
	void SetScrollingFlag(BOOL bIsScrolling);
	BOOL GetScrollingFlag();

	// 是否有父窗口(包括所有的祖先窗口)有slide属性
	BOOL IsInSlideParent();
	PCWceUiLayer GetSlideParent();

public:
	//set properties
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);

	//after the dialog is created, OnLoad is called.
	virtual void OnLoad();
	
	//before the dialog's OnInitDialog, OnInitLayer is called
	virtual void OnInitLayer();

	//timer
	virtual void OnTimer(WORD wIDEvent);
	//SetCapture but not call ReleaseCapture, and the Capture is lost
	virtual	void OnLoseCapture(); 
	//messages
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	//command
	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);

	//mouse messages
	virtual void OnLButtonDown(UINT nFlags,  POINT point);
	virtual void OnLButtonUp(UINT nFlags,	 POINT point);
	virtual void OnLButtonDblClk(UINT nFlags,POINT point);
	virtual void OnMouseMove(UINT nFlags,	 POINT point);

	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	//////////////////////////////////////////////////////////////////////////
	/////////////////////////control messages/////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//static controls
	virtual void OnLMouseDown(PCWceUiStatic pStatic, UINT nFlags,  POINT point);
	virtual void OnLMouseMove(PCWceUiStatic pStatic, UINT nFlags,  POINT point);
	virtual	void OnLMouseUp(PCWceUiStatic pStatic, UINT nFlags,  POINT point);
	virtual void OnLMouseDbClick(PCWceUiStatic pStatic, UINT nFlags,  POINT point);

	//button controls
	virtual void OnClick(PCWceUiButton pButton);

	//V Scroll
	virtual void OnPageUp(CWceUiScroll		*pVScroll);
	virtual void OnPageDown(CWceUiScroll	*pVScroll);
	virtual void OnVTrack(CWceUiScroll		*pVScroll);
	virtual void OnVEndScroll(CWceUiScroll	*pVScroll);
	virtual void OnLineUp(CWceUiScroll		*pVScroll);
	virtual void OnLineDown(CWceUiScroll	*pVScroll);

	//H Scroll
	virtual void OnPageLeft(CWceUiScroll	*pHScroll);
	virtual void OnPageRight(CWceUiScroll	*pHScroll);
	virtual void OnHTrack(CWceUiScroll		*pHScroll);
	virtual void OnHEndScroll(CWceUiScroll	*pHScroll);
	virtual void OnLineLeft(CWceUiScroll	*pHScroll);
	virtual void OnLineRight(CWceUiScroll	*pHScroll);

public:
	virtual void OnSetLayerText();
public:
	BOOL		 IsCreateLayerBitmap();
	void		 GetNeedDrawRect(LPRECT lpRect);

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	virtual void OnDrawBackGround(CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap);
	virtual void OnDrawBgImgs    (CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap);
	virtual void OnDrawText      (CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap);
	virtual void OnPreDrawFilter (CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap);

public:
	void SetTimer(WORD  wIDEvent, UINT nElapse);
	void KillTimer(WORD wIDEvent);
	
	void RegisterMsg(UINT nMsg);
	void UnRegisterMsg(UINT nMsg);

	CWceUiLayer* SetCapture();
	void ReleaseCapture();

	void SetReDraw(BOOL bRedraw = TRUE);
	BOOL IsReDraw() const;
	void InvalidateRect(LPCRECT lpRect = NULL);

	void SetLayerPos(int X, int Y, int cX, int cY, UINT uFlags);
	void MoveLayer(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	void MoveLayer(LPCRECT lpRect, BOOL bRepaint = TRUE);
	
	//change the layer's brother relationship
	BOOL MoveBefore(PCWceUiLayer pBeforeThis);
	BOOL MoveAfter(PCWceUiLayer pAfterThis);

	void SetText(LPCTSTR pText);
	void SetTextResID(LPCTSTR pStrResID);
	void SetTextResID(UINT nResID);
	LPCTSTR GetTextResID();
	LPCTSTR GetText();

public:
	//search all trees, include me
	PCWceUiLayer GetLayerByName(LPCTSTR pName);
	//only search child layer
	PCWceUiLayer GetChildByName(LPCTSTR pChildName);
	//only search child layer
	PCWceUiLayer GetChildByInx(UINT nIndex);

	CWceUiLayer*	 GetCmdReceiver();

	CWceUiLayerCtrl* GetLayerCtrl() const;
	CWceUiRealWnd*  GetWindow();
	CWceUiDlg*		GetDlg();
	CWceUiLayer*	 GetParent();

public:
	void	SetName(LPCTSTR pName);
	LPCTSTR	GetName() const;
	BOOL	IsEqualName(LPCTSTR pName) const;

	WORD	GetLayerID() const;

	void	GetClientRect(LPRECT lpRect);
	void	GetWindowRect(LPRECT lpRect);

	// 经过父窗口裁剪后的区域
	void	GetClientRectVisible(LPRECT lpRect);
	void	GetWindowRectVisible(LPRECT lpRect);


	void	ClientToWindow(LPRECT lpRect);
	void	WindowToClient(LPRECT lpRect);
	void	ClientToWindow(LPPOINT lpPoint);
	void	WindowToClient(LPPOINT lpPoint);

public:
	CWceUiStyle* GetStyle();
	CWceUiStyle* GetCommonStyle();

	void ShowLayer(BOOL bShow, BOOL bUpdateCommonStyle = TRUE);
	BOOL IsLayerVisible();

	void EnableLayer(BOOL bEnable, BOOL bUpdateCommonStyle = TRUE);
	BOOL IsLayerEnabled();

	void SetLayerContainer(BOOL bContainer);
	BOOL IsLayerContainer();

	void SetLayerNoDraw(BOOL bNoDraw);
	BOOL IsLayerNoDraw() const;

	void SetLayerNoInput(BOOL bNoInput);
	BOOL IsLayerNoInput() const;

	BOOL SetStyle(LPCTSTR pStyleName);
	void SetStyle(const CWceUiStyle& styleData);
	BOOL SetCommonStyle(LPCTSTR pCommonStyleName);
	void SetCommonStyle(const CWceUiStyle& commonStyleData);
	
	BOOL IsDrawBelow();

public:
	void  SetLayerDataPtr(void* pData);
	void* GetLayerDataPtr() const;

	//////////////////////////////////////////////////////////////////////////
	////////////////////////////Access child layer////////////////////////////
	//////////////////////////////////////////////////////////////////////////
public:
	int	GetChildCount();
	PCWceUiLayer FirstChild();
	PCWceUiLayer LastChild();
	PCWceUiLayer PreChild(PCWceUiLayer	pCurLayer);
	PCWceUiLayer NextChild(PCWceUiLayer pCurLayer);
	
public:
	void FreeBmpRes();

public:
	//////////////////////////////////////////////////////////////////////////
	///////////////////////////////layer list/////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//draw list
public:	NO_WRITE_POINTER(CWceUiLayer, PreDraw);
public:	NO_WRITE_POINTER(CWceUiLayer, NextDraw);
	//need draw list, single list
public:	NO_WRITE_POINTER(CWceUiLayer, NextNeedDraw);
	//child list
public:	NO_WRITE_POINTER(CWceUiLayer, LeftBrother);
public:	NO_WRITE_POINTER(CWceUiLayer, RightBrother);
	//input list, single list
public:	NO_WRITE_POINTER(CWceUiLayer, NextInput);

	//////////////////////////////////////////////////////////////////////////
	////////////////////////   control child layer   /////////////////////////
	//////////////////////////////////////////////////////////////////////////
private:
	BOOL AppendChild(PCWceUiLayer pNewChild);
	BOOL AddHeadChild(PCWceUiLayer pNewChild);
	BOOL RemoveChild(PCWceUiLayer pChild);
	BOOL RemoveTail();
	BOOL InsertBefore(PCWceUiLayer pBeforeThis, PCWceUiLayer pNewChild);
	BOOL InsertAfter(PCWceUiLayer pAfterThis, PCWceUiLayer pNewChild);

private:
	void SetLayerCtrl(CWceUiLayerCtrl* pLayerCtrl);
	void SetNeedDrawRect(LPCRECT lpRect);
	void SetParent(CWceUiLayer* pParent);
	void SetLayerID(WORD wLayerID);

	BOOL CreateAllChilds(PCWceUiXmlElem pElem, CWceUiLayer** ppFirstChildLayer = NULL);

private:
	void SetLayerPosition(LPCRECT lpRect, WCEUI_POSITION_WAY posWay, BOOL bRepaint = TRUE);
	void ForceInvalidateRect(LPCRECT lpRect=NULL);
private:
	void DrawOneBgImg(PCWceUiBgItem pBgItem, CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap);
	void OnDrawMixColorFilter(CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap);
private:
	void AddTimer(USHORT uIDEvent);
	BOOL DelTimer(USHORT uIDEvent);
	void DelAllTimer();

	BOOL AddRegMsg(UINT nMsg);
	BOOL DelRegMsg(UINT nMsg);
	void DelAllRegedMsgs();

	// 该接口一般情况下不使用，仅给特殊接口CWceUiLayerCtrl::DrawLayerToBmp()使用
	// 只是把标志置为TRUE或FALSE，不更新其它数据（参考ShowLayer())，从而可影响子控件可见性的判断
	void SetLayerVisible(BOOL bVisible);

private:
	wceui::CWceUiLayerData* GetData() const;
	wceui::CWceUiLayerData* m_pData;

private:
	CWceUiLayer (const CWceUiLayer &);
	CWceUiLayer & operator=(const CWceUiLayer &);
};
WCEUI_DYNCREATE_END(CWceUiLayer, CWceUiObject)