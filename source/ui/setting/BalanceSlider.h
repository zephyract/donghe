#pragma once
#include "WceUiLayer.h"
#include "WceUiLoadBitmap.h"
#include "WceUiBase.h"


class CBalanceSlider : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CBalanceSlider, CWceUiLayer);
public:
	CBalanceSlider(void);
	virtual ~CBalanceSlider(void);
// interface
	void SetRange( POINT nMin, POINT nMax, BOOL bRedraw = FALSE );
	void GetRange( POINT& nMin, POINT& nMax ) const;

	void SetPos( POINT nPos );
	POINT GetPos();

	void SetPosEx( POINT nPos );
	POINT GetPosEx() const;

	
// wceui overload
	virtual void OnInitLayer();
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnLButtonDown(UINT nFlags,  POINT point);
	virtual void OnLButtonUp(UINT nFlags,	 POINT point);
	virtual void OnMouseMove(UINT nFlags,	 POINT point);
	virtual void OnLoseCapture();

protected:
// helper functions

	// 根据具体位置pos, 映射到需要返回的POINT点
	POINT Pos2XY(POINT nPos);

	// 根据映射的point点，返回具体位置
	POINT XY2Pos(POINT point);


	CWceUiLoadBitmap m_bmpPosSel;

	POINT m_nMinPos;
	POINT m_nMaxPos;
	POINT m_nPos;

	BOOL m_bMouseDown;
};

WCEUI_DYNCREATE_END(CBalanceSlider, CWceUiLayer);
