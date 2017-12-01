#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpVideoBaseDlg.h"

#define TIMER_ID_SHOW_SIGNAL_FLAG	1020
#define TIMER_ID_CHECK_SIGNAL		1021

class CATVDlg : public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CATVDlg, CDvpVideoBaseDlg)
public:
	static void MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data);
	CATVDlg(void);
	~CATVDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	
	virtual void OnDlgShow(BOOL bShow);
	virtual void ShowVideo(BOOL bShow, RECT* prect = NULL);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnAutoFullscreen();
	virtual void OnBnClick(CWceUiButton* pButton);

	virtual void OnLButtonDown(UINT nFlags, POINT point);

	virtual void OnAnimateBegin();
	virtual void OnAnimating();
	virtual void OnAnimateEnd();

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	void ShowNoSignalFlag();

	// 根据当前的状态设置视频参数
	void UpdateVideoParams();
	void SetVideoScale();

	BOOL m_bIsSiganlReady;

};
WCEUI_DYNCREATE_END(CATVDlg, CDvpVideoBaseDlg);
