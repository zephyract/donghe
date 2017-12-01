#pragma once

#include "bzs_pairing.h"
#include "bzi_pairing.h"
#include "BZS_Connect.h"
#include "bzi_bzmw.h"
#include "bzi_connect.h"
#include "BZS_Call.h"
#include "BZI_Call.h"
#include "bzs_music.h"
#include "bzi_music.h"
#include "BZI_PBMgr.h"
#include "BZI_SPP.h"

#define PBMGR_SYNC_TIMEOUT	(2 * 60 * 1000)
#define PBMGR_SYNC_STEP		10

// ȷ��BTģ���Ǵ򿪵�,��ʼ����ɺ󣬹���ʱ����ģ���Ƿ��,û�򿪵Ļ������´�ģ��
#define TIMER_ID_OPEN_BLUETOOTH		8001
#define TIMER_ID_BT_AUTO_CONNECT		8002
const UINT BT_AUTO_CONNECT_TIME = 25000;

// ��Ӧ�ṹ��PBMGRRECORD_T, ֻ������Ҫʹ�õ�����
struct UI_PBMGRRECORD
{
	TCHAR pswName[64]; 
	TCHAR pswNumber[64]; 
	TCHAR pswTime[32];
	E_BT_TEL_NUMBER_TYPE  eTelType;
	E_BT_PBAP_SYNC_PATH_T ePath;
};

#define MAX_LOCAL_CALL_INFO_COUNT	100
#define LOCAL_RECENTS_CALL_INFO_PATH	L"residentflash3\\bt\\callinfo.dat"
// 
struct LOCAL_RECENTS_CALL_INFO
{
	TCHAR ciRemoteDeviceAddr[MAX_LEN_DEVICE_ADDR+1];		// �ֻ��ĵ�ַ,������ʶ�ֻ�
	TCHAR ciName[32];		// ����
	TCHAR ciNumber[32];		// ����
	SYSTEMTIME  ciTime;		// ͨ��ʱ��
	WORD  ciCallTime;		// ͨ��ʱ��,��λ��
	BYTE ciPath;			// ֻȡֵ����������δ��, ���Ϳ�ǿ��ת���� E_BT_PBAP_SYNC_PATH_T
	BYTE ciIsUnread;		// �����δ����δ������,���˱�־��Ϊ1, ����Ϊ0
};

class CBluetooth
{
public:
	CBluetooth(void);
	virtual ~CBluetooth(void);

	// callback functions
	static void NotifyBTEvtCBK(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
	static void BTPairCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
	static void BTConnectCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
	static void BTCallCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam);

	static void BTPBMGRCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
	static void BTDeviceQuerryCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
	static void BTA2DPMusicCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
	static void BTOpenCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam);

	static UINT CBluetooth::InitializeProc(LPVOID pParam);
public:
	void OnTimer(UINT_PTR nIDEvent);	// ��ʱ����������ת������
	static CBluetooth* GetInstance();

	void Initialize(HWND hWnd);
	void Uninitialize();
	BOOL IsInitialized();

	/* source control */
	void LaunchSrc();
	void StopSrc();

	// interface
	void A2DPActive(BOOL bActive);
	void AnswerCall();
	void TerminateCall();
	BOOL IsConnected();
	void Connect();
	void Disconnect();
	BOOL IsAutoConnect();
	void EnableAutoConnect(BOOL bEnable);
	BOOL IsAutoAnswer();
	void EnableAutoAnswer(BOOL bEnable, UINT nSeconds=3);
	void SetPinCode(LPCTSTR lpszPinCode);


	BOOL Dial(LPCTSTR szTelNumber);
	void ReDial();
	void MusicPrev();
	void MusicPlay();
	void MusicStop();
	void MusicPause();
	void MusicNext();
	E_BT_CONNECT_STATUS_T  GetConnectStatus();
	E_BT_STATUS_T GetBluetoothStatus();
	E_BT_MUSICPLAYSTATUS_T GetMusicPlayStatus();
	const MediaAttribute_T*	GetMediaAttribute();	// ID3����Ϣ
	const A2DPPlayState_T* GetPlayStateInfo();	// ������Ϣ
	void GetPairedRecs(CStringArray &recsArray);		// �õ���ʷ����豸���豸��
	void ConnectByPairedRec(UINT nIndex);				// ������Լ�¼�����������Ӹ��豸
	BOOL DeletePairedRec(UINT nIndex);		// ����ʧ��,�������豸����������
	// ����ͨ���绰�������TRUE,���򷵻�FALSE
	BOOL GetAudioOutput();
	// �������������ͨ���绰�����豸,TRUE ~ ͨ���绰 FALSE ~ ͨ���豸
	void SetAudioOutput(BOOL bByPhone);
	// ���ص�ǰMic�ľ���״̬
	BOOL IsMicMuteEnable();
	// ����Mic�Ƿ�Ҫ����
	void EnableMicMute(BOOL bEnable);

	LPCTSTR GetRemoteDeviceName();
	LPCTSTR GetRemoteDeviceAddr();

	// ��ʼͬ������,�������ص绰����ͨ����¼��
	BOOL PBMGR_Sync(E_BT_PBAP_SYNC_PATH_T ePath);
	// ��ȡ���ݼ�¼����, ʧ���˷���0
	UINT PBMGR_GetCount(E_BT_PBAP_SYNC_PATH_T ePath);
	BOOL PBMGR_GetRecords(E_BT_PBAP_SYNC_PATH_T ePath, int nIndex, OUT UI_PBMGRRECORD* pRecord);

	// ���ص�ǰͨ����״̬
	E_BZMWCALLMANAGER_CALLSTATE_T GetCallState();
	// ��ȡͨ�����������,������������һ����ֻ���غ���, �����������㹻��(>64)
	void GetCallInfo(LPTSTR name, LPTSTR number);

	BOOL SendDtmf(TCHAR ch);

	BOOL IsSupportInBandRing();

	// �򿪻�ر�����ʶ��(Siri)
	void SetVoiceRecognition(BOOL bEnable);

	// SPP
	BOOL SPPConnect();
	BOOL SPPDisconnect();
protected:
	// event handler
	void OnBTPair(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
	void OnBTConnect(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
	void OnBTCall(DWORD dwIndID, WPARAM wParam, LPARAM lParam);

	void OnBTPBMGR(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
	void OnBTDeviceQuerry(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
	void OnBTA2DPMusic(DWORD dwIndID, WPARAM wParam, LPARAM lParam);
	void OnBTOpen(DWORD dwIndID, WPARAM wParam, LPARAM lParam);

protected:
	HWND m_hwnd;	// �����Ĵ���

	MediaAttribute_T	m_MediaAttribute;	// ID3����Ϣ
	A2DPPlayState_T		m_PlayStateInfo;	// ������Ϣ

	// �豸�����Ϻ�,��ȡ���豸������ַ
	CString			m_strRemoteDeviceName;
	CString			m_strRemoteDeviceAddr;

	// ����ͨ����¼
public:
	BOOL LoadLocalCallInfo();
	// ������ּ�¼����100��������ļ�¼������
	BOOL SaveLocalCallInfo();
	
	// ���¼����ӿ�ֻ��Ե�ǰ�����ӵ��豸,���û�����������ǲ������õ�,
	// ���صĽ��Ҳֻ���뵱ǰ�����豸��ص�
	BOOL AddLocalCallInfo(LOCAL_RECENTS_CALL_INFO* pCallInfo);
	// ��Ϊͨ��ʱ�䲻������ͬ,���ݸ�ֵ����Ψһȷ����¼
	// ���pTime����Ϊ��, ������м�¼
	BOOL RemoveLocalCallInfo(SYSTEMTIME* pTime, E_BT_PBAP_SYNC_PATH_T ePath);
	// ��������ɾ��ָ���ļ�¼
	BOOL RemoveLocalCallInfo(int nIndex, E_BT_PBAP_SYNC_PATH_T ePath);
	// ���ePathΪBT_PBAP_SYNC_PATH_ALL_CALLHISTORY,��ȡ���м�¼
	BOOL GetLocalCallInfo(CArray<LOCAL_RECENTS_CALL_INFO> &aCallInfo, E_BT_PBAP_SYNC_PATH_T ePath);
	// ����������ȡָ���ļ�¼
	BOOL GetLocalCallInfo(int nIndex, LOCAL_RECENTS_CALL_INFO &CallInfo, E_BT_PBAP_SYNC_PATH_T ePath);

protected:
	// ��������������,�ҵ��ü�¼�����������,����-1��ʾʧ��
	int GetLocalCallInfoIndex(int nIndex, E_BT_PBAP_SYNC_PATH_T ePath);
	CArray<LOCAL_RECENTS_CALL_INFO> m_localCallInfo;

	BOOL m_bInitialized;

	// �����Զ���������ʱ,����������,�������´���,����ǿ������ӣ���������3�Σ�3��ʧ���˲ŷ������ӽ������״̬
	int m_nAutoConnectTry;
};
