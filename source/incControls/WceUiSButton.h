#pragma once
#include "wceuiscrolllayer.h"
#include "WceUiLoadBitmap.h"

class CWceUiSButton :	public CWceUiScrollLayer
{
	WCEUI_DYNCREATE_BEGIN(CWceUiSButton, CWceUiScrollLayer);
public:
	CWceUiSButton(void);
	virtual ~CWceUiSButton(void);

	// wceui override
	virtual void OnInitLayer();
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnLButtonDown(UINT nFlags,  POINT point);
	virtual void OnLButtonUp(UINT nFlags,	 POINT point);
	virtual void OnMouseMove(UINT nFlags,	 POINT point);

	// CWceuiScrollLayer override
	virtual int GetScrollDistance(DWORD tick, POINT point);
	virtual void OnScrollWindowEnd();
	virtual void OnMouseScrolled();
	virtual void OnMouseClicked();


	// interface
	int GetStatus();
	void SetStatus(int nStatus, BOOL bRedraw=TRUE);


protected:
	// mask用来创建显示区域region,轮廓可能不是矩形
	CWceUiLoadBitmap m_bmpMask;
	HRGN m_rgnLayer;
	int m_rgnOffsetX;
	int m_rgnOffsetY;
	// 这里的背景指的是轮廓线, 先画可滑动的位图再画轮廓线
	CWceUiLoadBitmap m_bmpBackground;
	// 正常状态下的可滑动的位图
	CWceUiLoadBitmap m_bmpNormal;
	// 下压状态下的可滑动的位图
	CWceUiLoadBitmap m_bmpDown;

	// 用来输出SButton的位图，输出完成后，才将该位图打到库提供的窗口位图上
	CWceUiGenericBitmap* m_pbmpSButton;


	int m_nMaxScrollPos;
	int m_nStatus;	// 0 ~ off, 1 ~ on
	BOOL m_bMouseScrolled; // 用来标识是否产生的这个事件，如果产生了，则滚动完毕时需判断状态是否已经改变

};
WCEUI_DYNCREATE_END(CWceUiSButton, CWceUiScrollLayer);