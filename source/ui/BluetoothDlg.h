#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpBaseDlg.h"
#include "bluetooth.h"
#include "CppSQLite3U.h"

const int FIELD_ID			= 0;
const int FIELD_NAME		= 1;
const int FIELD_NUMBER		= 2;
const int FIELD_PYNAME		= 3;
const int FIELD_LONGSTRING	= 4;
const int FIELD_SHORTSTRING = 5;


#define TIMER_ID_BT_CONNECTED	2001
#define TIMER_ID_BT_UNCONNECTED	2002
#define TIMER_ID_BT_UPDATE_DB_STRING	2003

enum BT_FUN_BUTTON_TYPE
{
	BT_FUN_CONNECT,
	BT_FUN_DIAL,
	BT_FUN_RECENTS,
	BT_FUN_PHONEBOOK,
	BT_FUN_MUSIC,
	BT_FUN_INVALID = -1
};

class CBluetoothDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CBluetoothDlg, CDvpBaseDlg)
public:
	CBluetoothDlg(void);
	~CBluetoothDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDlgShow(BOOL bShow);
	virtual void OnKeyPlayControl(UINT key, UINT param);
	static void PlayControl(UINT key, UINT param);

	// ������ż�
	void OnKeyCall();
	// �����л�
	void OnFunChange(BT_FUN_BUTTON_TYPE type);
	BT_FUN_BUTTON_TYPE GetFunType() {return m_CurFunType;}

// ��������״̬,��������ϢҲ�仯
	static void GetTitleInfo(CString &strTitle);
protected:
	void InitLayer();
	void OpenDB();
	// helper
	// ����ָ���ļ�¼�еĵ绰
	// nIndex : ��¼�е�����
	// SyncPath: ָ���������
	void Call(int nIndex, E_BT_PBAP_SYNC_PATH_T SyncPath);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	virtual void OnBnLongClick(CWceUiButton* pButton);


	// ��ť���������, ��������˷���TRUE�����򷵻�FALSE
	BOOL OnBnFunClick(CWceUiButton* pButton);			// �����л���ť
	BOOL OnBnPhonebookClick(CWceUiButton* pButton);		// �绰���еİ�ť
	BOOL OnBnRecentsClick(CWceUiButton* pButton);		// ͨ����¼�еİ�ť
	BOOL OnBnKeypadClick(CWceUiButton* pButton);		// keypad
	BOOL OnBnMusicClick(CWceUiButton* pButton);			// music


	// ѡ��ָ���Ĺ��ܰ�ť
	void CheckFunButton(BT_FUN_BUTTON_TYPE type);
	// ��ʾָ���Ĺ���ҳ��
	void ShowFuncFrame(BT_FUN_BUTTON_TYPE type);
	// ����ͨ����¼�а�ť���б����ʾ
	void ShowRecentsListbox(BOOL bShow);
	BOOL m_bShowRecentsListUI;	// ������ʶͨ����¼�б������Ƿ���ʾ��

	// ���Ƶ绰��������������ʾ���
	void ShowPBQueryUI(BOOL bShow);
	BOOL m_bShowQueryUI;	// ������ʶ���������Ƿ���ʾ��
	CStringArray m_PBQueryResult;		// ������������еĵ绰����,��Ҫ�κ�ʱ,ͨ��listbox����ֱ�ӿ��������в鵽����
	
	// ����ʷ�����豸��ӵ��б�
	void InitPairedRecsListbox();
	// ����ѡ�е���,�����ӿؼ���λ��
	void UpdatePairedLBChilds();
	void UpdateLBChilds(LPCTSTR lpszListboxName, LPCTSTR lpszChildName);
	void DeletePairedItem();

	// CALL�¼�����
	void OnCallEvent(DWORD dwEventID);
	// CALL�¼�����
	void OnMusicEvent(DWORD dwEventID);
	// PGMGR�¼�����
	void OnPBMGRSyncEvent(DWORD dwEventID, int nTotalRecords, E_BT_PBAP_SYNC_PATH_T SyncPath);

	void OnConnectEvent(DWORD dwEventID);
	void OnTimerBTConnect();
	void OnTimerBTDisconnect();

	void OnLocalCallInfoChange(int nRecentsType);

	void ShowDeviceInfo();
	void ShowTitleInfo();
	void ShowID3Tag();
	void UpgradeProgress();

	BT_FUN_BUTTON_TYPE m_CurFunType;		// ��ǰѡ�еĹ���

	// ������ż����еĺ����,���¾ɺ���,�����´��ز�
	// ��ʹ�����˳����´ν���ʱ�����ݻ�����Ч
	static CString m_strOldDialNumber;
	BOOL m_bIsDialNumber;	// �����ͨ�����Ŵ���ĺ���,�ô˱�־ΪTRUE

	// ���ͨ����¼�ӵ��б�
	void AddRecentsCall2Listbox(E_BT_PBAP_SYNC_PATH_T eSyncPath);
	// ͨ��¼���ӵ��б�
	void AddRecentsRecord2Listbox();
	// ����ѡ�е���,�����ӿؼ���λ��
	void UpdateRecentsLBChilds();
	// ɾ����ǰѡ�е�ͨ����¼
	void DeleteRecentsItem();
	// ͨ����¼��E_BT_PBAP_SYNC_PATH_T�ж��屣��һ��
	// 0xFF������ʾ¼��
	static const int RECENTS_TYPE_RECORD = 0xFF;
	int m_nRecentsType;

	// ���绰����ӵ�listbox
	// nFirst ~ �ӵڼ�����ʼ���,  nCount ~ �ܹ���Ӽ���
	BOOL AddPB2Listbox(int nFirst, int nCount);
	// �����صĵ绰����ӵ����ݿ�,���ݿ⻹�����˸�ʽ����(������ʾ),����ʽ���Ķ����첽ִ��(��Ҫ�ϳ�ʱ��),
	// ������ӵ����ݿ��,����һ����ʱ���ֲ���ʽ������
	BOOL AddPB2DB(int nFirst, int nCount);
	// ��ʽ�����ݿ���ַ�������������ʾ
	// ÿ�θ�ʽ���̶���Ŀ������,�����ʱ̫��
	// ����Ѿ���ʽ����ȫ������,����TRUE,�������FALSE,��ʾ���滹������δ��ʽ��
	BOOL UpdateDBString();
	void OnTimerUpdateDBString();
	// ��¼�Ѿ���ʽ���˵ļ�¼������
	int m_nUpdatedRecords;
	// ���µ绰���Ĳ�ѯ�����listbox
	void UpdatePBQ2Listbox();
	// ͬ���绰������
	// bForceResync ~ TRUE : ǿ�������������ݣ� FALSE : ����ʹ�ñ�������,���û�б�������������
	void SyncPBData(BOOL bForceResync);
	// ���ص绰��ʱ,��ʾ�������ض���
	void ShowWaitCursor(BOOL bShow);

	// ָ����ǰ��ͬ�����ݵ�����
//	E_BT_PBAP_SYNC_PATH_T m_eSyncPath;

	// ��ʾ��������������ͣ����ʾ��Ϣ
	void ShowTipMessage(LPCTSTR message);

	// ����ң���������ּ�
	void OnKeyDigital(UINT nDigitalKey);

	CppSQLite3DB m_db;
};
WCEUI_DYNCREATE_END(CBluetoothDlg, CDvpBaseDlg);


#include "WceUiSEditBox.h"

class CTelNumberEditBox : public CWceUiSEditBox
{
	WCEUI_DYNCREATE_BEGIN(CTelNumberEditBox, CWceUiSEditBox)

public:
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

};

WCEUI_DYNCREATE_END(CTelNumberEditBox, CWceUiSEditBox);



class CKeypadLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CKeypadLayer, CWceUiLayer)

public:
	CKeypadLayer();
	~CKeypadLayer();

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	virtual void OnLButtonDown(UINT nFlags, POINT point);
	virtual void OnLButtonUp(UINT nFlags,	 POINT point);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);

	// ���õ�һ��������, �м���� �м���� ÿ�и���
	void SetKeyRect(RECT *pRect, int nColGap, int nRowGap, int nColCount);
protected:
	static const int KEY_COUNT = 26;
	RECT m_rc[KEY_COUNT];
	CWceUiLoadBitmap m_texSelect; 
	BOOL m_bMouseDown;
	int m_nClickItem;
	int HitTest(POINT pt);
};

WCEUI_DYNCREATE_END(CKeypadLayer, CWceUiLayer);
