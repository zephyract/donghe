#pragma once
#include "wceuiloadbitmap.h"

class CClimatronicLayer_DasAuto : public CWceUiLayer
{
WCEUI_DYNCREATE_BEGIN(CClimatronicLayer_DasAuto, CWceUiLayer)
public:
	CClimatronicLayer_DasAuto(void);
	~CClimatronicLayer_DasAuto(void);

	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnInitLayer(void);
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	void DrawBmp(CWceUiGenericBitmap* pWndBitmap,int x,int y,CWceUiGenericBitmap* pSrcBitmap);
	void DrawTxt(CWceUiGenericBitmap* pWndBitmap,LPCTSTR lpchText, LPRECT lprc,COLORREF clr,HFONT hFont,UINT uFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	void OnDrawFanSpeed(CWceUiGenericBitmap* pWndBitmap);
	void OnDrawFanDirect(CWceUiGenericBitmap* pWndBitmap);
	void OnDrawAirConData(CWceUiGenericBitmap* pWndBitmap);
	void OnDrawHotSeat(CWceUiGenericBitmap* pWndBitmap);
   
private:	

	CWceUiLoadBitmap m_pbmp_head_l;
	CWceUiLoadBitmap m_pbmp_hands_l;
	CWceUiLoadBitmap m_pbmp_feet_l;
	CWceUiLoadBitmap m_pbmp_head_r;
	CWceUiLoadBitmap m_pbmp_hands_r;
	CWceUiLoadBitmap m_pbmp_feet_r;
	CWceUiLoadBitmap m_pbmp_fanspeed_normal;
	CWceUiLoadBitmap m_pbmp_fanspeed_down;
	CWceUiLoadBitmap m_pbmp_unit;
	CWceUiLoadBitmap m_pbmp_cycle[2];
	CWceUiLoadBitmap m_pbmp_fdemister;
	CWceUiLoadBitmap m_pbmp_rdemister;
	CWceUiLoadBitmap m_pbmp_lhotseat[4];
	CWceUiLoadBitmap m_pbmp_rhotseat[4];
	CWceUiLoadBitmap m_pbmp_fanicon[2];
	CWceUiLoadBitmap m_pbmp_wind[2];
	
};
WCEUI_DYNCREATE_END(CClimatronicLayer_DasAuto, CWceUiLayer);
