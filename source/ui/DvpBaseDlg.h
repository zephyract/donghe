#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "AnimDlg.h"

#define TIMER_ID_SPECTRUM 101
#define TIMER_ID_HIDE_INVALID_TIP	102
#define TIMER_ID_HIDE_MESSAGE_TIP	103

#define TIMER_ID_RETURN_PLAY_UI		104
#define TIMER_ID_DELAY_PLAY_NEXT	1006	//遇到不能播放的文件,提示不能播放后，延时一段时间再播放下一曲
#define TIMER_ID_DELAY_PLAY_ITEM	1007	// 播放时马上弹出播放窗口,导航动画不流畅，让动画完成后再延时播放

const UINT TIME_RETURN_PLAY_UI = 15000;


class CDvpBaseDlg : public CAnimDlg
{
	WCEUI_DYNCREATE_BEGIN(CDvpBaseDlg, CAnimDlg)
public:
	CDvpBaseDlg(void);
	~CDvpBaseDlg(void);	

	virtual void OnInitDialog();
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

	// 用来处理播放按键, 参数为协议中定义的键值
	virtual void OnKeyPlayControl(UINT key, UINT param);

	void ShowMediaTimeInfo(UINT curDuration, UINT outDuration);
	void UpdataProgress(UINT curDuration, UINT outDuration);

protected:
	virtual void OnBnClick(CWceUiButton* pButton);

	void OnConnectEvent();
	
	// 显示当前EQ类型
	void ShowEQ();

	// 时间信息
	CWceUiLayer  *m_pTimeElapse;
	CWceUiLayer  *m_pTimeLeft;
	// 进度条
	CWceUiSlider* m_pProgressSlider;
	// 播放暂停按钮
	CWceUiButton *m_pLayerPlay;
	CWceUiButton *m_pLayerPause;


};
WCEUI_DYNCREATE_END(CDvpBaseDlg, CAnimDlg);
