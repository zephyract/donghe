#pragma once
#include "stdafx.h"
#include "wceuidlg.h"
#include "DvpVideoBaseDlg.h"

#define TIMER_ID_CHECK_MENU			1100
#define TIMER_ID_AUTO_PLAY_NEXT		1101	// 不支持的音视频,自动播放下一曲

class CDVDMPSPlayerDlg: public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CDVDMPSPlayerDlg, CDvpVideoBaseDlg)
public:
	CDVDMPSPlayerDlg(void);
	~CDVDMPSPlayerDlg(void);

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
	void ShowInvalidIcon();

	// event handler
	void OnEvtPlayState(UINT state);
	void OnEvtCmdResponse(UINT param1, UINT param2);
	void OnEvtPBCTime(BOOL bDVDStopped=FALSE);

	void ShowPlayMode();
	void ShowFileTip();

	void ShowSubtitle();
	void ShowAudio();
	void ShowTipMessage(LPCTSTR message);

	BOOL m_bAudioBtnClicked;
	BOOL m_bSubtitleBtnClicked;
	BOOL m_bRandomClicked;
	BOOL m_bRepeatClicked;

	void OnErrorOccured(BYTE err);
	void ShowBadfileTipInfo(BOOL bShow, LPCTSTR text_id);
	CWceUiLayer  *m_pLayerBadfileInfo;

	void DVD_Stop();

	static void DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData);
};
WCEUI_DYNCREATE_END(CDVDMPSPlayerDlg, CDvpVideoBaseDlg);