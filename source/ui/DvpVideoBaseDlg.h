#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpBaseDlg.h"

const int AUTO_FULLSCREEN_TIME = 5000;
#define TIMER_ID_AUTO_FULLSCREEN	    300

const int ANIM_SHOW_LAYERS_MAX_FRAME = 8;
const int ANIM_SHOW_LAYERS_TIME = 50;
#define TIMER_ID_ANIM_SHOW_LAYERS		301


#define TIMER_ID_DELAY_SHOW_VIDEO	302
#define TIMER_ID_DELAY_SHOW_OVERLAY	303

class CDvpVideoBaseDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CDvpVideoBaseDlg, CDvpBaseDlg)
public:
	CDvpVideoBaseDlg(void);
	~CDvpVideoBaseDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnLButtonDown(UINT nFlags, POINT point);
	virtual void OnDlgShow(BOOL bShow);

	virtual void OnKeyOSD();	// 处理遥控器的OSD键
protected:
	virtual void AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData);
	virtual void OnAutoFullscreen();
	BOOL m_bFullScreen;
	void SetFullScreen(BOOL bFullScreen);
	
	void OnTimerAnimShowLayers();
	void AnimMoveLayers(BOOL bShow);
	virtual void AnimShowLayers(BOOL bShow);
	int m_nLayersCurrentFrame;
	BOOL m_bLayersShow;

	// 除了上下条,如果有其它控件,也需隐藏或显示
	void ShowOtherLayers(BOOL bShow);

	void ShowVideoForbiddenTip();
	BOOL IsVideoForbidden();

};
WCEUI_DYNCREATE_END(CDvpVideoBaseDlg, CDvpBaseDlg);
