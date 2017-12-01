#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "AnimDlg.h"

#define TIMER_ID_SPECTRUM 101
#define TIMER_ID_HIDE_INVALID_TIP	102
#define TIMER_ID_HIDE_MESSAGE_TIP	103

#define TIMER_ID_RETURN_PLAY_UI		104
#define TIMER_ID_DELAY_PLAY_NEXT	1006	//�������ܲ��ŵ��ļ�,��ʾ���ܲ��ź���ʱһ��ʱ���ٲ�����һ��
#define TIMER_ID_DELAY_PLAY_ITEM	1007	// ����ʱ���ϵ������Ŵ���,�����������������ö�����ɺ�����ʱ����

const UINT TIME_RETURN_PLAY_UI = 15000;


class CDvpBaseDlg : public CAnimDlg
{
	WCEUI_DYNCREATE_BEGIN(CDvpBaseDlg, CAnimDlg)
public:
	CDvpBaseDlg(void);
	~CDvpBaseDlg(void);	

	virtual void OnInitDialog();
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

	// ���������Ű���, ����ΪЭ���ж���ļ�ֵ
	virtual void OnKeyPlayControl(UINT key, UINT param);

	void ShowMediaTimeInfo(UINT curDuration, UINT outDuration);
	void UpdataProgress(UINT curDuration, UINT outDuration);

protected:
	virtual void OnBnClick(CWceUiButton* pButton);

	void OnConnectEvent();
	
	// ��ʾ��ǰEQ����
	void ShowEQ();

	// ʱ����Ϣ
	CWceUiLayer  *m_pTimeElapse;
	CWceUiLayer  *m_pTimeLeft;
	// ������
	CWceUiSlider* m_pProgressSlider;
	// ������ͣ��ť
	CWceUiButton *m_pLayerPlay;
	CWceUiButton *m_pLayerPause;


};
WCEUI_DYNCREATE_END(CDvpBaseDlg, CAnimDlg);
