#pragma once
#include "WceUiLayer.h"
#include "WceUiLoadBitmap.h"
#include "WceUi565Bitmap.h"
#include "WceUiRealWnd.h"
#include "WceUiBase.h"


class CWceUiSlider : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CWceUiSlider, CWceUiLayer);
public:
	CWceUiSlider(void);
	virtual ~CWceUiSlider(void);
// interface
	void SetRange( int nMin, int nMax );
	void GetRange( int& nMin, int& nMax ) const;
	void SetPos( int nPos );
	int GetPos( ) const;

	void EnalbeNotifyOnMove(BOOL bEnable);
	void SetReadonly(BOOL bReadonly);
	// SLIDER_HORIZONTAL ~ for horizontal ,  SLIDER_VERTICAL ~ for vertical
	void SetMode(int nMode);
	
// wceui overload
	virtual void OnDraw(CWceUiGenericBitmap* pDrawBitmap, RECT rcNeedDraw);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnLButtonDown(UINT nFlags,  POINT point);
	virtual void OnLButtonUp(UINT nFlags,	 POINT point);
	virtual void OnMouseMove(UINT nFlags,	 POINT point);
	virtual void OnLoseCapture();

protected:
// helper functions
	// 根据位置pos,返回Thumb距离控件左边(Horizontal),或上边(Vertical)的距离
	int Pos2XY(int nPos);
	// 根据Thumb距离，返回位置
	int XY2Pos(int nAmount);
	int GetXY(POINT point);
	void RefreshThumbSize();

	CWceUiLoadBitmap m_bmpBg;
	CWceUiLoadBitmap m_bmpBgSel;
	CWceUiLoadBitmap m_bmpThumb;
	int m_nThumbSize;

	int m_nMinPos;
	int m_nMaxPos;
	int m_nPos;
	int m_nNotifyPos;

	BOOL m_bMouseDown;
	// TRUE in default, notify parent when mouse is moving.
	// FALSE, notify parent only when mouse is up.
	BOOL m_bNotifyOnMove;
	// FALSE in default.
	BOOL m_bIsReadonly;

	int m_nMode;
};

WCEUI_DYNCREATE_END(CWceUiSlider, CWceUiLayer);
