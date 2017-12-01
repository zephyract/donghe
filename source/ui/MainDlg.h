#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "AnimDlg.h"
#include "BaseDlg.h"

// ��������๦�ܰ�ť��
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

	// ͨ��dlg id���ҸöԻ���Ķ�������
	// ������Ҫ����һ��������
	// ���õ����ĸ��ؼ��й�
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

	void InitButtonsRect();		// ���水ť��ԭʼ����,���ú�λ�ÿ��ܱ���

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

	// �����ĸ���������˳��ֱ��� zoomX, zoomY, offsetX, offsetY
	void AnimGetParameters(OUT float* params, int nStep, int nTotal);
	CWceUiLayer* m_pAnimLayer;	// ����ʱ��صĿؼ���Ҳ����Ϊ��

	CWceUiLayer *m_pDate;
	CWceUiLayer *m_pTime;

	RECT m_rcButtons[MAIN_MAX_BUTTONS];		// ����ÿ����ť�������ļ�������

};
WCEUI_DYNCREATE_END(CMainDlg, CAnimDlg);
