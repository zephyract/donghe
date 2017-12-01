#pragma once
#include "stdafx.h"
#include "wceuidlg.h"
#include "DvpVideoBaseDlg.h"

#define TIMER_ID_CHECK_MENU			1100

class CCDGPlayerDlg: public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CCDGPlayerDlg, CDvpVideoBaseDlg)
public:
	CCDGPlayerDlg(void);
	~CCDGPlayerDlg(void);

	virtual void OnInitDialog();
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnLButtonDown(UINT nFlags, POINT point);
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);
	virtual void ShowVideo(BOOL bShow, RECT* prect = NULL);

	virtual void AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData);
	void OnKeyPlayControl(UINT key, UINT param);
protected:
	virtual void OnAutoFullscreen();
	virtual void OnBnClick(PCWceUiButton pButton);
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);

	virtual void OnAnimateBegin();
	virtual void OnAnimating();
	virtual void OnAnimateEnd();

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	void InitLayer();
	void ShowInvalidIcon();

	// event handler
	void OnEvtPlayState(UINT state);
	void OnEvtCmdResponse(UINT param1, UINT param2);
	void OnEvtPBCTime();

	// 显示随机及循环模式
	void ShowPlayMode();
	void ShowTitleChapter();

	// 显示声道信息
	void ShowAudio();
	void ShowTipMessage(LPCTSTR message);
	BOOL m_bAudioBtnClicked;


	static void DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData);
};
WCEUI_DYNCREATE_END(CCDGPlayerDlg, CDvpVideoBaseDlg);