#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "AnimDlg.h"

#define TIMER_ID_GET_CARINFO	1230

class CCarInfoDlg : public CAnimDlg
{
WCEUI_DYNCREATE_BEGIN(CCarInfoDlg, CAnimDlg)
public:
	CCarInfoDlg(void);
	~CCarInfoDlg(void);	

	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	void DrawBmp(CWceUiGenericBitmap* pWndBitmap,int x,int y,CWceUiGenericBitmap* pSrcBitmap);
    void DrawTxt(CWceUiGenericBitmap* pWndBitmap,LPCTSTR lpchText, LPRECT lprc,COLORREF clr,HFONT hFont,UINT uFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE);
private:

	CWceUiLoadBitmap m_pBmp_remailL[2];
	CWceUiLoadBitmap m_pBmp_tempoutside;
	CWceUiLoadBitmap m_pBmp_battvtg[2];
	CWceUiLoadBitmap m_pBmp_seatbelt[2];
	CWceUiLoadBitmap m_pBmp_trunk;
	CWceUiLoadBitmap m_pBmp_brake[2];
	CWceUiLoadBitmap m_pBmp_cleanwater[2];
	CWceUiLoadBitmap m_pBmp_enginespeed;
	CWceUiLoadBitmap m_pBmp_velocity;
	CWceUiLoadBitmap m_pBmp_mileage;
	CWceUiLoadBitmap m_pBmp_dr[4];
	CWceUiLoadBitmap m_pBmp_dr_alarm;
	CWceUiLoadBitmap m_pBmp_trunk_alarm;

};
WCEUI_DYNCREATE_END(CCarInfoDlg, CAnimDlg);
