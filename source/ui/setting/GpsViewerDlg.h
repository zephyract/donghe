#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

class CGpsViewerDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CGpsViewerDlg, CAnimDlg)
public:
	CGpsViewerDlg(void);
	~CGpsViewerDlg(void);	

	virtual void OnInitDialog();
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	
protected:
	void UpdateLayer();
	void InitLayer();

	void RefreshLayer();

	CWceUiLayer*        m_platitude;
	CWceUiLayer*		m_plongitude;
	CWceUiLayer*		m_paltitude;
	CWceUiLayer*		m_pspeed;
	CWceUiLayer*		m_putc;

};
WCEUI_DYNCREATE_END(CGpsViewerDlg, CAnimDlg);


///////////////////////  CGpsLayer  ////////////////////////////

#define  GPS_MAX_SIGNAL_COUNT 12
class CGpsSignalLayer :	public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CGpsSignalLayer, CWceUiLayer)
public:
	CGpsSignalLayer(void);
	~CGpsSignalLayer(void);
	virtual void OnInitLayer();
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

protected:
	void DrawSNR(CWceUiGenericBitmap* pWndBitmap, int index, BOOL is_fix,int id, int signal);

};
WCEUI_DYNCREATE_END(CGpsSignalLayer, CWceUiLayer);

///////////////////////////  CGpsSatelliteLayer  /////////////////////////
class CGpsSatelliteLayer :	public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CGpsSatelliteLayer, CWceUiLayer)
public:
	CGpsSatelliteLayer(void);
	~CGpsSatelliteLayer(void);

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

protected:
// 	CWceglTexture* m_pTexSatellite;
// 	CWceUiGenericBitmap *m_pTexSatellite;
	CWceUiLoadBitmap  m_pSatelliteloadBmp;

};

WCEUI_DYNCREATE_END(CGpsSatelliteLayer, CWceUiLayer);