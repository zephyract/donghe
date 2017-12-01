#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "AnimDlg.h"
#include "CppSQLite3U.h"
#include "ecarobject_dll.h"

#define TIMER_ID_DELAY_DIAL		1020
#define TIMER_ID_SEND_TNC_CODE	1021
#define TIMER_ID_DELAY_DIAL_LEFT_COUNT	1022	// ���ŵ���ʱ
enum ECAR_STAUS
{
	ES_IDLE,			// û��ִ���ر�ָ��
	ES_TO_DIAL,			// ƥ�䵽Ψһ����,���ŵ���ʱ״̬
};

class CEcarDlg : public CAnimDlg
{
	WCEUI_DYNCREATE_BEGIN(CEcarDlg, CAnimDlg)
public:
	CEcarDlg(void);
	~CEcarDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	
	virtual void OnDlgShow(BOOL bShow);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

	static BOOL IsEcarNumber(const TCHAR *pTelNumber);

protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);

	// button click
	void OnEcar();
	void OnCall();
	void OnCancel();
	void OnBack();
	void OnHome();

	// timer
	void OnTimerLeftCount();

	void OnUserCallNo(char* pTelNumber);	// �����绰
	void OnCallByName(LPCTSTR pTelName);	// �������ִ����绰
	void OnUpdateMapInfo(const char* pTncCode);	// ���õ���Ŀ�ĵ�
	void OnCallEvent(DWORD dwEventID);

	BOOL SendTncCodeToNavi();
	// ͨ�����ֻ�绰�����ѯ���ݿ�, 
	// bByName :  TRUE ~ by name,  FALSE ~ by number
	void GetContactsByName(LPCTSTR pTelName, LPCTSTR pTelNumber, BOOL bByName, CWceUiListBox* pListbox, CStringArray& PBQueryResult);
	void ShowMessageTip(LPCTSTR tip, BOOL bAutoHide=TRUE);
	void ShowMessage(LPCTSTR message);

	// ��������,������ӿ�,��BTͨ��������,����ECar����
	void SetLastDlgInfoToThis();

	// ȡ������ʱ����
	void CancelDelayDial();
	// Ecar�Ƿ��ڼ���״̬
	// ���򣺵�ECAR�����·�����Ч����ʱ��ΪEcar���ڼ���״̬, ��ECar���洦�ڵ�ǰ����ʱ,Ҳ��Ϊ��Ecar������״̬
	//       ֻ��Ecar Dlg�����˳��ˣ� �����Զ��κų�ȥ��,����ΪEcar���ڷǼ���״̬
	void SetEcarActive(BOOL bActive);	

	CECarObject* m_pECarObject;
	// ����ͨ���ĵ绰�����ж��Ƿ��ͨ����ecar�ĺ�������
	BOOL m_bIsEcarCallCenter;
	// �·��Ĵ����绰
	CString m_strTelNumberToDial;
	// ������Ϣ
	TCHAR m_szTncCode[256];
	int   m_nSendTncCodeCount;	// �����򵼺�����TNC�Ĵ���,��Ϊ�����п��ܲ��ɹ����ﵽ�������ƾͲ��ٷ���

	// �·�����ʱ,ƥ�䵽�ĵ绰�����¼,������ʾ��listbox
	CStringArray m_PBQueryResult;

	// ���ŵ���ʱ
	int m_LeftCount;
	ECAR_STAUS m_eStatus;

	// ��¼������������ʱ�ĵ�ǰ����
	DLG_INFO* m_pdiLastDlg;
};
WCEUI_DYNCREATE_END(CEcarDlg, CAnimDlg);
