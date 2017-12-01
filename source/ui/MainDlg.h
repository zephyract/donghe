#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "AnimDlg.h"
#include "BaseDlg.h"

// 主界面最多功能按钮数
#define MAIN_MAX_BUTTONS	32
#define MAIN_BUTTONS_PER_PAGE	8

#define TIMER_ID_REFRESH_TIME	1005

class CMainDlg : public CAnimDlg
{
	WCEUI_DYNCREATE_BEGIN(CMainDlg, CAnimDlg)
public:
	CMainDlg(void);
	~CMainDlg(void);

	virtual void OnInitDialog();

	virtual void OnTimer(WORD wIDEvent);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);

	// 通过dlg id查找该对话框的动画参数
	// 这里是要返回一个矩形区
	// 并得到与哪个控件有关
	DWORD GetAnimData(DLG_ID did);

	static CMainDlg* GetInstance();
	static CMainDlg* m_pthis;

	void RelayoutLayers();

protected:
	// command handler
	virtual void OnBnClick(CWceUiButton* pButton);
	void OnPageClick(int nPage);
	void OnPagePrevClick();
	void OnPageNextClick();
	void OnEcarClick();

	void InitButtonsRect();		// 保存按钮的原始坐标,配置后位置可能变了

	void CheckPage(int nPage);
	void UpdateDateTime();

	virtual void DoAnimate();
	virtual void OnAnimateEnd();

	void OnConnectEvent();
	void OnDvdStatusChange();
	void OnMassStoragePlug();


	void ShowBottom(BOOL bShow);
	void ShowMessageTip(LPCTSTR tip);
	static BOOL m_bBottomShow;

	// 返回四个参数，按顺序分别是 zoomX, zoomY, offsetX, offsetY
	void AnimGetParameters(OUT float* params, int nStep, int nTotal);
	CWceUiLayer* m_pAnimLayer;	// 动画时相关的控件，也可以为空

	CWceUiLayer *m_pDate;
	CWceUiLayer *m_pTime;

	RECT m_rcButtons[MAIN_MAX_BUTTONS];		// 保存每个按钮的配置文件中坐标

};
WCEUI_DYNCREATE_END(CMainDlg, CAnimDlg);
