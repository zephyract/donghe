#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpVideoBaseDlg.h"
#include "WceUiRotateBitmap.h"

class CClockDlg : public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CClockDlg, CDvpVideoBaseDlg)
public:
	CClockDlg(void);
	~CClockDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnAutoFullscreen();
	virtual void OnDlgShow(BOOL bShow);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	void SetClockMode(int mode);
	DLG_INFO* m_pLastDlg;

};
WCEUI_DYNCREATE_END(CClockDlg, CDvpVideoBaseDlg);

/////////////////////////////////////////////////////////////////////////

#define TIMER_ID_CLOCK	1030
class CClockLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CClockLayer, CWceUiLayer)
public:
	CClockLayer(void);
	~CClockLayer(void);	


	virtual void OnTimer(WORD wIDEvent);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnInitLayer();
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	void SetResID(LPCTSTR hour, LPCTSTR min, LPCTSTR sec, LPCTSTR bk);
protected:
	void OnTimerClock();
	void DrawClock(CWceUiGenericBitmap* pWndBitmap);

	CWceUiRotateBitmap m_rbmpHour;
	CWceUiRotateBitmap m_rbmpMinute;
	CWceUiRotateBitmap m_rbmpSecond;
	CWceUiLoadBitmap m_txtBK;

	float m_fHourAngle;
	float m_fMinuteAngle;
	float m_fSecondAngle;
};
WCEUI_DYNCREATE_END(CClockLayer, CWceUiLayer);

///////////////////////////////////////////////////////////////////////////////

class CDigitalClockLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CDigitalClockLayer, CWceUiLayer)
public:
	CDigitalClockLayer(void);
	~CDigitalClockLayer(void);	


	virtual void OnTimer(WORD wIDEvent);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnInitLayer();

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
protected:
	void DrawClock(CWceUiGenericBitmap* pWndBitmap);

	CWceUiLoadBitmap m_txtNumber[10];
	CWceUiLoadBitmap m_txtColon;
	CWceUiLoadBitmap m_txtAM;
	CWceUiLoadBitmap m_txtPM;
	BOOL m_bShowColcon;
};
WCEUI_DYNCREATE_END(CDigitalClockLayer, CWceUiLayer);