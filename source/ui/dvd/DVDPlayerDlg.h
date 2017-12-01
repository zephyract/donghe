#pragma once
#include "stdafx.h"
#include "wceuidlg.h"
#include "DvpVideoBaseDlg.h"

#define TIMER_ID_CHECK_MENU			1100

class CDVDPlayerDlg: public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CDVDPlayerDlg, CDvpVideoBaseDlg)
public:
	CDVDPlayerDlg(void);
	~CDVDPlayerDlg(void);

	virtual void OnInitDialog();
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnLButtonDown(UINT nFlags, POINT point);
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);
	virtual void ShowVideo(BOOL bShow, RECT* prect = NULL);
	void OnKeyPlayControl(UINT key, UINT param);
	void OnKeyOSD();	// 处理遥控器的OSD键

	virtual void AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData);
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

	void ShowAngle();
	void ShowSubtitle();
	void ShowAudio();
	void ShowTipMessage(LPCTSTR message);
	void ShowMenuKey(BOOL bShow);

	// audio,信息DVD会自己重复发上来,UI需做到只有点击相应的按钮才给出提示
	// 所以用变量来记录是否有点击按钮
	BOOL m_bAudioBtnClicked;
	BOOL m_bSubtitleBtnClicked;
	BOOL m_bAngelBtnClicked;
	BOOL m_bRandomClicked;
	BOOL m_bRepeatClicked;

	// 记录当前处于的zoom倍数
	// 放大时DVP返回值为4,6,8(UI设计上可对应倍数为2, 3, 4)
	// 缩小时DVP返回值为9, 10, 11(UI设计上可对应倍数为1/2, 1/3, 1/4)
	// 正常情况下倍数为0
	UCHAR m_nZoom;
	void ShowZoomInfo(UCHAR zoom);

	static void DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData);
};
WCEUI_DYNCREATE_END(CDVDPlayerDlg, CDvpVideoBaseDlg);



class CMoveLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CMoveLayer, CWceUiLayer);
public:
	CMoveLayer(void);
	virtual ~CMoveLayer(void);


	//mouse messages
	virtual void OnLButtonDown(UINT nFlags,  POINT point);
	virtual void OnLButtonUp(UINT nFlags,	 POINT point);
	virtual void OnMouseMove(UINT nFlags,	 POINT point);
	virtual void OnLoseCapture();
protected:
	BOOL m_bIsMouseDown;

	POINT m_ptMouseDown;
	RECT m_rcLayer;
};

WCEUI_DYNCREATE_END(CMoveLayer, CWceUiLayer);
