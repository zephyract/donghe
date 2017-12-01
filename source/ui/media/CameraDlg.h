#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpVideoBaseDlg.h"

#define TIMER_ID_SHOW_SIGNAL_FLAG	1020
#define TIMER_ID_CHECK_SIGNAL		1021

class CCameraDlg : public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CCameraDlg, CDvpVideoBaseDlg)
public:
	static void MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data);
	CCameraDlg(void);
	~CCameraDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnInitLayer();
	virtual void OnDlgShow(BOOL bShow);
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);

	void ShowNoSignalFlag();

	// 根据当前的状态设置视频参数
	void UpdateVideoParams();

	void DrawBmp(CWceUiGenericBitmap* pWndBitmap,int x,int y,CWceUiGenericBitmap* pSrcBitmap);
	void DrawBackCarLine(CWceUiGenericBitmap* pWndBitmap);
	void DrawRadar(CWceUiGenericBitmap* pWndBitmap);

	BOOL m_bIsSiganlReady;
	CWceUiLoadBitmap m_pBmpBk;
	CWceUiLoadBitmap m_pBmpCar;
	CWceUiLoadBitmap m_pBmpRadar[8][11];
};
WCEUI_DYNCREATE_END(CCameraDlg, CDvpVideoBaseDlg);
