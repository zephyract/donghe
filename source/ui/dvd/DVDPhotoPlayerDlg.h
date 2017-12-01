#pragma once
#include "stdafx.h"
#include "wceuidlg.h"
#include "DvpVideoBaseDlg.h"
#include "protocol.h"

class CDVDPhotoPlayerDlg: public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CDVDPhotoPlayerDlg, CDvpVideoBaseDlg)
public:
	CDVDPhotoPlayerDlg(void);
	~CDVDPhotoPlayerDlg(void);

	virtual void OnInitDialog();
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnLButtonDown(UINT nFlags, POINT point);
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);
	virtual void ShowVideo(BOOL bShow, RECT* prect = NULL);

	virtual void AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData);
	virtual void OnKeyPlayControl(UINT key, UINT param);
protected:
	virtual void OnAutoFullscreen();
	virtual void OnBnClick(PCWceUiButton pButton);
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);

	virtual void OnAnimateBegin();
	virtual void OnAnimating();
	virtual void OnAnimateEnd();

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	void InitLayer();
	void ShowVideoForbiddenTip(BOOL bShow);
	void ShowInvalidIcon();

	// event handler
	void OnEvtPlayState(UINT state);
	void OnEvtCmdResponse(UINT param1, UINT param2);

	void ShowFileTip();
	void ShowPlayMode();

	// 记录当前处于的zoom倍数
	// 放大时DVP返回值为4,6,8(UI设计上可对应倍数为2, 3, 4)
	// 缩小时DVP返回值为9, 10, 11(UI设计上可对应倍数为1/2, 1/3, 1/4)
	// 正常情况下倍数为0
	UCHAR m_nZoom;
	void ShowZoomInfo(UCHAR zoom);
	void ShowTipMessage(LPCTSTR message);

	BOOL m_bRandomClicked;
	BOOL m_bRepeatClicked;

	static void DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData);
};
WCEUI_DYNCREATE_END(CDVDPhotoPlayerDlg, CDvpVideoBaseDlg);