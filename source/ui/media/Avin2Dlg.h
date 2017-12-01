#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpVideoBaseDlg.h"
#include "Avin1Dlg.h"

class CAvin2Dlg : public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CAvin2Dlg, CDvpVideoBaseDlg)
public:
	static void MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data);
	CAvin2Dlg(void);
	~CAvin2Dlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	
	virtual void OnDlgShow(BOOL bShow);
	virtual void ShowVideo(BOOL bShow, RECT* prect = NULL);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnLButtonDown(UINT nFlags, POINT point);
protected:
	virtual void OnAutoFullscreen();
	virtual void OnBnClick(CWceUiButton* pButton);

	virtual void OnAnimateBegin();
	virtual void OnAnimating();
	virtual void OnAnimateEnd();

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	void ShowNoSignalFlag();
	void ShowNoSignalLayer(BOOL bShow);

	// 根据当前的状态设置视频参数
	void UpdateVideoParams();
	void SetVideoScale();

	BOOL m_bIsSiganlReady;
};
WCEUI_DYNCREATE_END(CAvin2Dlg, CDvpVideoBaseDlg);
