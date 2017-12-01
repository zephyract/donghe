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

// 确定BT模块是打开的,初始化完成后，过段时间检测模块是否打开,没打开的话，重新打开模块
#define TIMER_ID_OPEN_BLUETOOTH		8001
#define TIMER_ID_BT_AUTO_CONNECT		8002
const UINT BT_AUTO_CONNECT_TIME = 25000;

// 对应结构体PBMGRRECORD_T, 只定义需要使用的数据
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
	TCHAR ciRemoteDeviceAddr[MAX_LEN_DEVICE_ADDR+1];		// 手机的地址,用来标识手机
	TCHAR ciName[32];		// 姓名
	TCHAR ciNumber[32];		// 号码
	SYSTEMTIME  ciTime;		// 通话时间
	WORD  ciCallTime;		// 通话时长,单位秒
	BYTE ciPath;			// 只取值打进，打出，未接, 类型可强制转换成 E_BT_PBAP_SYNC_PATH_T
	BYTE ciIsUnread;		// 如果是未读的未接来电,将此标志置为1, 否则为0
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
	void OnTimer(UINT_PTR nIDEvent);	// 计时器需主窗口转发过来
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
	const MediaAttribute_T*	GetMediaAttribute();	// ID3等信息
	const A2DPPlayState_T* GetPlayStateInfo();	// 进度信息
	void GetPairedRecs(CStringArray &recsArray);		// 得到历史配对设备的设备名
	void ConnectByPairedRec(UINT nIndex);				// 根据配对记录索引主动连接该设备
	BOOL DeletePairedRec(UINT nIndex);		// 返回失败,可能是设备处于连接中
	// 声音通过电话输出返回TRUE,否则返回FALSE
	BOOL GetAudioOutput();
	// 设置声音输出是通过电话还是设备,TRUE ~ 通过电话 FALSE ~ 通过设备
	void SetAudioOutput(BOOL bByPhone);
	// 返回当前Mic的静音状态
	BOOL IsMicMuteEnable();
	// 设置Mic是否要静音
	void EnableMicMute(BOOL bEnable);

	LPCTSTR GetRemoteDeviceName();
	LPCTSTR GetRemoteDeviceAddr();

	// 开始同步数据,可以下载电话本，通话记录等
	BOOL PBMGR_Sync(E_BT_PBAP_SYNC_PATH_T ePath);
	// 获取数据记录总数, 失败了返回0
	UINT PBMGR_GetCount(E_BT_PBAP_SYNC_PATH_T ePath);
	BOOL PBMGR_GetRecords(E_BT_PBAP_SYNC_PATH_T ePath, int nIndex, OUT UI_PBMGRRECORD* pRecord);

	// 返回当前通话的状态
	E_BZMWCALLMANAGER_CALLSTATE_T GetCallState();
	// 获取通话名字与号码,如果名字与号码一样，只返回号码, 缓冲区必须足够大(>64)
	void GetCallInfo(LPTSTR name, LPTSTR number);

	BOOL SendDtmf(TCHAR ch);

	BOOL IsSupportInBandRing();

	// 打开或关闭语音识别(Siri)
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
	HWND m_hwnd;	// 关联的窗口

	MediaAttribute_T	m_MediaAttribute;	// ID3等信息
	A2DPPlayState_T		m_PlayStateInfo;	// 进度信息

	// 设备连接上后,获取的设备名及地址
	CString			m_strRemoteDeviceName;
	CString			m_strRemoteDeviceAddr;

	// 本地通话记录
public:
	BOOL LoadLocalCallInfo();
	// 如果发现记录超过100条，后面的记录丢弃掉
	BOOL SaveLocalCallInfo();
	
	// 以下几个接口只针对当前已连接的设备,如果没有连接设置是不起作用的,
	// 返回的结果也只是与当前连接设备相关的
	BOOL AddLocalCallInfo(LOCAL_RECENTS_CALL_INFO* pCallInfo);
	// 因为通话时间不可能相同,根据该值可以唯一确定记录
	// 如果pTime参数为空, 清除所有记录
	BOOL RemoveLocalCallInfo(SYSTEMTIME* pTime, E_BT_PBAP_SYNC_PATH_T ePath);
	// 根据索引删除指定的记录
	BOOL RemoveLocalCallInfo(int nIndex, E_BT_PBAP_SYNC_PATH_T ePath);
	// 如果ePath为BT_PBAP_SYNC_PATH_ALL_CALLHISTORY,获取所有记录
	BOOL GetLocalCallInfo(CArray<LOCAL_RECENTS_CALL_INFO> &aCallInfo, E_BT_PBAP_SYNC_PATH_T ePath);
	// 根据索引获取指定的记录
	BOOL GetLocalCallInfo(int nIndex, LOCAL_RECENTS_CALL_INFO &CallInfo, E_BT_PBAP_SYNC_PATH_T ePath);

protected:
	// 根据索引及类型,找到该记录在数组的索引,返回-1表示失败
	int GetLocalCallInfoIndex(int nIndex, E_BT_PBAP_SYNC_PATH_T ePath);
	CArray<LOCAL_RECENTS_CALL_INFO> m_localCallInfo;

	BOOL m_bInitialized;

	// 开机自动连接蓝牙时,经常连不上,这里做下处理,如果是开机连接，尝试连接3次，3次失败了才放弃连接进入配对状态
	int m_nAutoConnectTry;
};
