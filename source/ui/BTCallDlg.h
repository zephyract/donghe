#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpBaseDlg.h"
#include "bluetooth.h"

// ˢ��UI,ͨ��ʱ��,���Ӷ�����
#define TIMER_ID_REFRESH_UI		201
#define TIMER_ID_TERMINATE_DLG	202
#define TIMER_ID_ANIM_OUTGOING		203
#define TIMER_ID_ANIM_INCOMING	204

class CBTCallDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CBTCallDlg, CDvpBaseDlg)
public:
	CBTCallDlg(void);
	~CBTCallDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDlgShow(BOOL bShow);

protected:
	virtual void OnAnimateEnd();
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnBnLongClick(CWceUiButton* pButton);
	BOOL OnBnKeypadClick(CWceUiButton* pButton);

	void OnKeyDigital(UINT nDigitalKey);

	void ShowTitleInfo();
	// ���ݲ���,�趨MUTE��ť��״̬
	void OnMicMuteChange(BOOL bEnable);

	void RefreshUI();
	void OnStatusChange(UINT nStatus);
	// rds
	void OnRadioStatus();

	// ���¶�Ӧ��ť״̬
	void OnAudioSwitch(BOOL bByPhone);

	DWORD m_dwTickSpeaking;
	UINT  m_status;

	// ��¼������Ϣ
	void UpdateCallInfo(UINT nStatus);
	void InitCallInfo(UINT nStatus);
	LOCAL_RECENTS_CALL_INFO m_callinfo;

	void ShowKeypad(BOOL bShow);

	CWceUiLayer* m_pLayerFrameKeypad;

	void OnKeyCommand(UINT key, UINT param);	// ֱ����Ӧ�����򷽿�
	void OnAnswer();	// �����绰
	void OnDecline();	// �Ҷϵ绰

};
WCEUI_DYNCREATE_END(CBTCallDlg, CDvpBaseDlg);



#define TIMER_ID_ANIM_LAYER 105
class CWceUiAnimLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CWceUiAnimLayer, CWceUiLayer)
public:
	CWceUiAnimLayer();
	~CWceUiAnimLayer();

	virtual void OnTimer(WORD wIDEvent);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);

	// nTime ~ ÿһ֡��ʾʱ��,��λms
	void StartAnim(int nTime);
	void StopAnim();

protected:
	// strStyle Ϊ���ŷָ���style����,����ʱ�����ڲ�ͬ��style���л�
	void SetAnimStyle(LPCTSTR strStyle);
	CStringArray m_style;

	// ��ǰstyle������
	int m_nCurrentStyle;
};
WCEUI_DYNCREATE_END(CWceUiAnimLayer, CWceUiLayer);


class CAnimIncomingLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CAnimIncomingLayer, CWceUiLayer)
public:
	CAnimIncomingLayer();
	~CAnimIncomingLayer();

	virtual void OnTimer(WORD wIDEvent);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnInitLayer();

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	// nTime ~ ÿһ֡��ʾʱ��,��λms
	void StartAnim(int nTime);
	void StopAnim();

protected:
};
WCEUI_DYNCREATE_END(CAnimIncomingLayer, CWceUiLayer);
