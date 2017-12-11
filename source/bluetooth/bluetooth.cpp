#include "StdAfx.h"
#include "bluetooth.h"
#include "interface.h"
#include "MainWnd.h"
#include "tzUtil.h"
#include "sysUtil.h"
#include "config.h"

CBluetooth::CBluetooth(void)
{
	m_nAutoConnectTry = 0;
	m_bInitialized = FALSE;
	m_hwnd = NULL;
	#define BLUETOOTHID 1020
	BZ_RegBTCallBack(BLUETOOTHID, BZ_MODULE_PAIRING,	BTPairCallBack);
	BZ_RegBTCallBack(BLUETOOTHID, BZ_MODULE_CONNECTION, BTConnectCallBack);
	BZ_RegBTCallBack(BLUETOOTHID, BZ_MODULE_CALL,	BTCallCallBack);

	BZ_RegBTCallBack(BLUETOOTHID, BZ_MODULE_PBMGR,	BTPBMGRCallBack);
	BZ_RegBTCallBack(BLUETOOTHID, BZ_MODULE_SEARCH,	BTDeviceQuerryCallBack);
	BZ_RegBTCallBack(BLUETOOTHID, BZ_MODULE_MUSIC,	BTA2DPMusicCallBack);
	BZ_RegBTCallBack(BLUETOOTHID, BZ_MODULE_OCBT,	BTOpenCallBack);
}

CBluetooth::~CBluetooth(void)
{

	BZ_UnRegBTCallBack(BLUETOOTHID, BZ_MODULE_PAIRING);
	BZ_UnRegBTCallBack(BLUETOOTHID, BZ_MODULE_CONNECTION);
	BZ_UnRegBTCallBack(BLUETOOTHID, BZ_MODULE_CALL);

	BZ_UnRegBTCallBack(BLUETOOTHID, BZ_MODULE_PBMGR);
	BZ_UnRegBTCallBack(BLUETOOTHID, BZ_MODULE_SEARCH);
	BZ_UnRegBTCallBack(BLUETOOTHID, BZ_MODULE_MUSIC);
	BZ_UnRegBTCallBack(BLUETOOTHID, BZ_MODULE_OCBT);
}


CBluetooth* CBluetooth::GetInstance()
{
	static CBluetooth bluetooth;
	return &bluetooth;
}

void CBluetooth::Initialize(HWND hWnd)
{
	if (m_bInitialized)
	{
		return;
	}

	m_hwnd = hWnd;
	HANDLE handle = ::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)InitializeProc, LPVOID(this), NULL, NULL);
	CloseHandle(handle);
}

UINT CBluetooth::InitializeProc(LPVOID pParam)
{
	static BOOL bInit = FALSE;
	if (bInit)
	{
		return 0;	// 系统只初始化一次
	}
	bInit = TRUE;
	CBluetooth* pthis = (CBluetooth*)pParam;

	DWORD dwTick = GetTickCount();

	HRESULT hr = BZ_Init(NotifyBTEvtCBK, BZ_BLUZEON_ALL_MASK, 0);
	if (hr == BZE_OK)
	{
		CMainWnd::GetInstance()->SetTimer(TIMER_ID_OPEN_BLUETOOTH, 6000, NULL);

		BZ_PBMGRSetPreferredProfile(PBMGR_PBAP_CONNECT);
		BZ_PBMGRSetPBAPMethod(TRUE);  // PBAP下载电话本使用PullPhoneBook的方式,速度更快

		BZ_SetHFPOEMFlag(0x00000000);		// 这接口目前好像没啥用
		BZ_SetPairingTimeout(INFINITE);
		BZ_VerifySCOAudioChannel(FALSE);	// 电话声音输出到机器还是手机

		// 更新连接设备信息
		BZ_GetRemoteDevInfo(pthis->m_strRemoteDeviceName.GetBuffer(MAX_LEN_DEVICE_NAME), MAX_LEN_DEVICE_NAME, 
			pthis->m_strRemoteDeviceAddr.GetBuffer(MAX_LEN_DEVICE_ADDR), MAX_LEN_DEVICE_ADDR);
		pthis->m_strRemoteDeviceAddr.ReleaseBuffer();
		pthis->m_strRemoteDeviceName.ReleaseBuffer();

		// 加载本地记录
		pthis->LoadLocalCallInfo();

		// bluetooth, mic gain
#define WODM_MTK_SET_SPH_GAIN	(WM_USER+0x29)
		waveOutMessage((HWAVEOUT)0, WODM_MTK_SET_SPH_GAIN, /*0~63*/config::get_config_bluetooth()->mic_gain, 0);	// 设置MIC增益值
		pthis->EnableAutoConnect(sysutil::nss_get_instance()->bt_auto_connect);
		pthis->EnableAutoAnswer(sysutil::nss_get_instance()->bt_auto_answer);
		// BZ_setvolume();
		// BT_SetRingVolume();

		RETAILMSG(1, (L"[navi_car] it costs %d ms for initializing BT.\n", GetTickCount()-dwTick));

		pthis->m_bInitialized = TRUE;
	}

	// 获取手机信号电量信息
	// 	E_BT_PHONESIGHAL_LEVEL_T eSingalLvl;
	// 	E_BT_PHONEBATTERY_LEVEL_T  eBatteryLvl;
	// 	BZ_GetSignalQuality(eSingalLvl);
	// 	BZ_GetBatteryLevel(eBatteryLvl);


	return 0;
}


void CBluetooth::Uninitialize()
{
	
}

BOOL CBluetooth::IsInitialized()
{
	return m_bInitialized;
}

void CBluetooth::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ID_OPEN_BLUETOOTH)
	{
		CMainWnd::GetInstance()->KillTimer(TIMER_ID_OPEN_BLUETOOTH);

		// 确保模块是打开的
		E_BT_STATUS_T eState = MW_BT_STATUS_UNKNOWN;
		if (BZE_OK == BZ_GetBlueToothStatus(eState) && eState == MW_BT_STATUS_OFF)
		{
			BZ_OpenBlueTooth(TRUE);
			RETAILMSG(1, (L"[navi car] BZ_OpenBlueTooth.\n"));
		}
	}
	else if (nIDEvent == TIMER_ID_BT_AUTO_CONNECT)	// 如果是手机拿远了断开的蓝牙,会自动重新连接
	{
		BT_PAIREDRECORDINFO_T *prRec = NULL;
		BZ_GetPairedRec(0,&prRec);
		if (prRec)
		{
			BZ_ConnectByBDAddr(prRec->btAddr);
		}
	}
}

void CBluetooth::A2DPActive(BOOL bActive)
{
	static DWORD dwTick = 0;
	if (IsConnected())
	{
		if (GetTickCount() - dwTick < 500)
		{
			Sleep(50);
		}
		dwTick = GetTickCount();
		BZ_A2DPActive(bActive);
	}
}

void CBluetooth::LaunchSrc()
{
	if (IsConnected())
	{
		A2DPActive(TRUE);
		MusicPlay();
	}
}

void CBluetooth::StopSrc()
{
	MusicPause();
	A2DPActive(FALSE);
}

void CBluetooth::AnswerCall()
{
	E_BZMWCALLMANAGER_CALLSTATE_T eCallStatus;
	if (BZ_GetCallStatus(0, eCallStatus) == BZE_OK
		&& eCallStatus == CALLMANAGER_CALLSTATE_INCOMING)
	{
		BZ_AnswerCall();
	}  
}

void CBluetooth::TerminateCall()
{
	if (BZ_GetIfInCallState() != PHONEST_INACTIVE)
	{
		BZ_TerminateCall();
	}
}


BOOL CBluetooth::SendDtmf(TCHAR ch)
{
	E_BZMWCALLMANAGER_CALLSTATE_T eCallStatus;
	if (BZE_OK == BZ_GetCallStatus(0, eCallStatus)
		&& eCallStatus == CALLMANAGER_CALLSTATE_SPEAKING)
	{
		if (BZ_GetDtmfStatus())
		{
			TCHAR szKey[2] = {ch, L'\0'};
			BZ_SendDtmf (szKey);
// 			Sleep(200);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CBluetooth::IsSupportInBandRing()
{
	BOOL isSupportBandRing = FALSE;
	if (BZ_IsSupportInBandRing(&isSupportBandRing) == BZE_OK)
	{
		return isSupportBandRing;
	}

	return FALSE;
}

void CBluetooth::SetVoiceRecognition(BOOL bEnable)
{
	BZ_SetVoiceRecognition(bEnable);
}

E_BZMWCALLMANAGER_CALLSTATE_T CBluetooth::GetCallState()
{
	E_BZMWCALLMANAGER_CALLSTATE_T eCallStatus;
	if (BZ_GetCallStatus(0, eCallStatus) == BZE_OK)
	{
		return eCallStatus;
	}

	return CALLMANAGER_CALLSTATE_IDLE;
}

void CBluetooth::GetCallInfo(LPTSTR name, LPTSTR number)
{
	TCHAR szNumber[64] = L"";
	TCHAR szName[64] = L"";
	if (BZ_GetCallTelNum(0, szNumber) == BZE_OK
		&& 	BZ_GetCallName(0, szName) == BZE_OK)
	{
		if (_tcscmp(szName, szNumber) == 0)
		{
			szName[0] = L'\0';
		}

		if (name)
		{
			_tcscpy(name, szName);
		}

		if (number)
		{
			_tcscpy(number, szNumber);
		}
	}

}

BOOL CBluetooth::IsConnected()
{
	E_BT_CONNECT_STATUS_T status;
	if (BZ_GetConnectStatus(status) == BZE_OK
		&& status == BT_CONNECT_STATUS_CONNECTED)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CBluetooth::Connect()
{
	E_BT_CONNECT_STATUS_T status;
	if (BZ_GetConnectStatus(status) == BZE_OK
		&& status == BT_CONNECT_STATUS_UNCONNECT)
	{
		m_nAutoConnectTry = 3;
		BZ_Connect();
	}

}

void CBluetooth::Disconnect()
{
	E_BT_CONNECT_STATUS_T status;
	if (BZ_GetConnectStatus(status) == BZE_OK
		&& status == BT_CONNECT_STATUS_CONNECTED)
	{
		BZ_DisConnect();
	}
}

BOOL CBluetooth::IsAutoConnect()
{
	return BZ_GetAutoConnectionStatus();
}

void CBluetooth::EnableAutoConnect(BOOL bEnable)
{
	BZ_EnableAutoConnection(bEnable);
}

BOOL CBluetooth::IsAutoAnswer()
{
	return BZ_GetAutoAnswerCallState();
}

void CBluetooth::EnableAutoAnswer(BOOL bEnable, UINT nSeconds)
{
	BZ_EnableAutoAnswerCall(bEnable, nSeconds);
}

void CBluetooth::SetPinCode(LPCTSTR lpszPinCode)
{
	BZ_SetPinCode((wchar_t*)lpszPinCode);
}

BOOL CBluetooth::PBMGR_Sync(E_BT_PBAP_SYNC_PATH_T ePath)
{
	BZ_PBMGRSetIndStep(PBMGR_SYNC_STEP);
	BZ_PBMGRSetTimeOutVal(ePath, PBMGR_SYNC_TIMEOUT);
	
// 	if (!BZ_PBMGRIsSptSync(ePath))
// 	{
// 		return FALSE;
// 	}

	BZ_PBMGRStartSync(ePath);
	return TRUE;
}

UINT CBluetooth::PBMGR_GetCount(E_BT_PBAP_SYNC_PATH_T ePath)
{
	UINT count = 0;
	if (BZ_PBMGRGetRecCnt(ePath, count) == BZE_OK)
	{
		return count;
	}

	return 0;
}

BOOL CBluetooth::PBMGR_GetRecords(E_BT_PBAP_SYNC_PATH_T ePath, int nIndex, OUT UI_PBMGRRECORD* pRecord)
{
	P_PBMGRRECORD_T pPBMGRecord = NULL;
	HRESULT hr = BZ_PBMGRGetRecs(ePath, (UINT)nIndex, 1, &pPBMGRecord);
	if (hr == BZE_OK && pRecord)
	{
		memset(pRecord, 0, sizeof(UI_PBMGRRECORD));
		_tcsncpy(pRecord->pswName, (TCHAR*)pPBMGRecord->pswName, sizeof(pRecord->pswName)/sizeof(TCHAR)-1);
		_tcsncpy(pRecord->pswNumber, (TCHAR*)pPBMGRecord->pswNumber, sizeof(pRecord->pswNumber)/sizeof(TCHAR)-1);
		_tcsncpy(pRecord->pswTime, (TCHAR*)pPBMGRecord->pswTime, sizeof(pRecord->pswTime)/sizeof(TCHAR)-1);
		pRecord->eTelType = pPBMGRecord->eTelType;
		pRecord->ePath = pPBMGRecord->ePath;
		BZ_PBMGRRelRecs(&pPBMGRecord, 1);
	}

	return (hr == BZE_OK);
}

void CBluetooth::OnBTPBMGR(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	PBMGR_SYNC_RESULT_PARAM_T *presult = (PBMGR_SYNC_RESULT_PARAM_T*)wParam;


	switch(dwIndID)
	{
	case BZ_IND_PBAP_NOT_SUPPORT:
		RETAILMSG(1, (L"[navi_car]: Downloading phone book not supported. \n"));
		break;
	case BZ_IND_PBAP_SYNC_COMPLETE:
		RETAILMSG(1, (L"[navi_car]: Downloading phone book complete. \n"));
	
		break;
	case BZ_IND_PBAP_SYNC_FAIL:
		RETAILMSG(1, (L"[navi_car]: Downloading phone book failed. \n"));
		break;

	case BZ_IND_PBAP_PBRECORDS:
		RETAILMSG(1, (L"[navi_car]: Downloading phone book, %d/%d records OK. \n", presult->u4RecCnt, presult->u4TotleCnt));
		break;
	case BZ_IND_PBAP_TIMEOUT:
		RETAILMSG(1, (L"[navi_car]: Downloading phone book time out. \n"));
		BZ_PBMGRStopSync();
		break;
	case BZ_IND_PBAP_FILLERR:
		RETAILMSG(1, (L"[navi_car]: Downloading phone book fill error. \n"));
		break;
	case BZ_IND_PBAP_START_OVER:
		RETAILMSG(1, (L"[navi_car]: Downloading phone book start over. \n"));
		break;
	default:
		break;
	}

	DWORD param = (presult->u4TotleCnt<<16) + (presult->ePath & 0xFFFF);
	::PostMessage(m_hwnd, UI_MSG_BT_PBMGR_SYNC_EVENT, (WPARAM)param, (LPARAM)dwIndID);
}


void CBluetooth::OnBTPair(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	switch(dwIndID)
	{
	case BZ_IND_PAIR_SUCCESS:
		RETAILMSG(1, (L"[navi_car]: BT Pair success. \n"));
		break;
	case BZ_IND_PAIR_FAIL:
		RETAILMSG(1, (L"[navi_car]: BT pair fail. \n"));
		if (GetBluetoothStatus() == MW_BT_STATUS_ON)	// 蓝牙仍然是打开的
		{
			BZ_StopPairing();
			Sleep(300);
			BZ_StartPairing();
		}
		break;
	case BZ_IND_SET_LOCALDEVNAME_SUCCESS:
		RETAILMSG(1, (L"[navi_car]: BT set local device name success. \n"));
		break;
	case BZ_IND_SET_LOCALDEVNAME_FAIL:
		RETAILMSG(1, (L"[navi_car]: BT set local device name fail. \n"));
		break;
	case BZ_IND_START_PAIRING_FAIL:
		RETAILMSG(1, (L"[navi_car]: BT start pair fail. \n"));
		if (GetBluetoothStatus() == MW_BT_STATUS_ON)	// 蓝牙仍然是打开的
		{
			BZ_StopPairing();
			Sleep(300);
			BZ_StartPairing();
		}
		break;
	case BZ_IND_START_PAIRING_SUCCESS:
		RETAILMSG(1, (L"[navi_car]: BT start pair success. \n"));
		break;


	default:
		break;
	}
}

void CBluetooth::OnBTConnect(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	TCHAR szPath[MAX_PATH];
	switch(dwIndID)
	{
	case BZ_IND_CREATE_CONNECT_SUCCESS:
		m_nAutoConnectTry = 0;	// 成功了,不需尝试多次连接
		CMainWnd::GetInstance()->KillTimer(TIMER_ID_BT_AUTO_CONNECT);

		// 更新连接设备信息
		BZ_GetRemoteDevInfo(m_strRemoteDeviceName.GetBuffer(MAX_LEN_DEVICE_NAME), MAX_LEN_DEVICE_NAME, 
			m_strRemoteDeviceAddr.GetBuffer(MAX_LEN_DEVICE_ADDR), MAX_LEN_DEVICE_ADDR);
		m_strRemoteDeviceAddr.ReleaseBuffer();
		m_strRemoteDeviceName.ReleaseBuffer();

		_stprintf(szPath, L"%s\\bt\\connected.wav", tzutil::GetAppPath());
		sndPlaySound(szPath, SND_ASYNC);
		//sndPlaySound(tzutil::GetAbsPath(L"ring.wav"),SND_ASYNC|SND_LOOP|SND_NODEFAULT);
		RETAILMSG(1, (L"[navi_car]: BT connect success. \n"));
		break;

	case BZ_IND_DISCONNECT_SUCCESS:
		RETAILMSG(1, (L"[navi_car]: BT disconnect success. \n"));
		if (GetBluetoothStatus() == MW_BT_STATUS_ON)	// 蓝牙仍然是打开的
		{
			if (*(BOOL*)wParam)	// TRUE表示因为手机离远了断开的情况
			{
				CMainWnd::GetInstance()->SetTimer(TIMER_ID_BT_AUTO_CONNECT, BT_AUTO_CONNECT_TIME, NULL);
			}

			BZ_StopPairing();
			Sleep(300);
			BZ_StartPairing();
		}

		_stprintf(szPath, L"%s\\bt\\disconnected.wav", tzutil::GetAppPath());
		sndPlaySound(szPath, SND_ASYNC);

		break;

	case BZ_IND_CREATE_CONNECT_FAIL:
		RETAILMSG(1, (L"[navi_car]: BT create connect fail. \n"));
		if (m_nAutoConnectTry-- > 0)
		{
			BZ_Connect();
		}
		else if (GetBluetoothStatus() == MW_BT_STATUS_ON)	// 蓝牙仍然是打开的
		{
			BZ_StopPairing();
			Sleep(300);
			BZ_StartPairing();
		}
		break;
	case BZ_IND_CONNECT_STATUS_CHANGED:
		RETAILMSG(1, (L"[navi_car]: BT connect status change. \n"));
	default:
		break;
	}

	::PostMessage(m_hwnd, UI_MSG_BT_CONNECT_STATUS_CHANGE, 0, (LPARAM)dwIndID);
}


void CBluetooth::OnBTCall(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{

	switch(dwIndID)
	{
	case BZ_IND_AUDIO_OUTPUT_INLOCALDEV:
		RETAILMSG(1, (L"[navi_car]: OnBTCall. output inlocaldev. \n"));
		break;
	case BZ_IND_AUDIO_OUTPUT_INPHONE:
		RETAILMSG(1, (L"[navi_car]: OnBTCall. output inphone. \n"));
		break;
	case  BZ_IND_CALL_SPEAKING:
		RETAILMSG(1, (L"[navi_car]: OnBTCall. speaking. \n"));
		break;
	case  BZ_IND_CALL_OUTGOING:
		RETAILMSG(1, (L"[navi_car]: OnBTCall. outgoing. \n"));
		break;
	case  BZ_IND_CALL_INCOMING:
		RETAILMSG(1, (L"[navi_car]: OnBTCall. incoming. \n"));
		break;
	case BZ_IND_CALL_TERMINATED:
		RETAILMSG(1, (L"[navi_car]: OnBTCall. terminated. \n"));
		break;
	case BZ_IND_VOICE_RECOGNITION:
		RETAILMSG(1, (L"[navi_car]: OnBTCall. voice recognition. \n"));
		break;
	default:
		RETAILMSG(1, (L"[navi_car]: OnBTCall. BZ_IND event id = %d. \n", dwIndID));
		break;
	}

	::PostMessage(m_hwnd, UI_MSG_BT_CALL_EVENT, wParam, (LPARAM)dwIndID);
}


void CBluetooth::OnBTDeviceQuerry(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	if (dwIndID == BZ_IND_QUERYSERVICES_FINISH)
	{
		BZ_Connect();
	}
}

void CBluetooth::OnBTA2DPMusic(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	switch(dwIndID)
	{
	case BZ_IND_AVRCP_REMOTE_NOTIFY_EVENT:
		RETAILMSG(1, (L"[navi_car]: BZ_IND_AVRCP_REMOTE_NOTIFY_EVENT. \n"));
		break;
	case BZ_IND_AVRCP_ID3_SUCCESS:	
		RETAILMSG(1, (L"[navi_car]: BZ_IND_AVRCP_ID3_SUCCESS. \n"));
		break;
	case BZ_IND_AVRCP_PlAY_STATE_CHANGE:
		//RETAILMSG(1, (L"[navi_car]: BZ_IND_AVRCP_PlAY_STATE_CHANGE. \n"));
		break;
	case BZ_IND_MUSIC_QUERY_PLAY:
		RETAILMSG(1, (L"[navi_car]: BZ_IND_MUSIC_QUERY_PLAY. \n"));
		break;
	case BZ_IND_A2DP_CONNECT_SUCCESS:
		RETAILMSG(1, (L"[navi_car]: BZ_IND_A2DP_CONNECT_SUCCESS. \n"));
		break;
	case BZ_IND_A2DP_DISCONNECT_SUCCESS:
		RETAILMSG(1, (L"[navi_car]: BZ_IND_A2DP_DISCONNECT_SUCCESS. \n"));
		break;
	case BZ_IND_AVRCP_CONNECT_SUCCESS:
		RETAILMSG(1, (L"[navi_car]: BZ_IND_AVRCP_CONNECT_SUCCESS. \n"));
		break;
	case BZ_IND_MUSIC_STOP:
		RETAILMSG(1, (L"[navi_car]: BZ_IND_MUSIC_STOP. \n"));
		break;
	case BZ_IND_MUSIC_PLAYING:	
		RETAILMSG(1, (L"[navi_car]: BZ_IND_MUSIC_PLAYING. \n"));
		break;
	case BZ_IND_DISCONNECT_SUCCESS:
		RETAILMSG(1, (L"[navi_car]: BZ_IND_DISCONNECT_SUCCESS. \n"));
		break;
	case BZ_IND_ClOSE_BLUETOOTH_SUCCESS:
		RETAILMSG(1, (L"[navi_car]: BZ_IND_ClOSE_BLUETOOTH_SUCCESS. \n"));
		break;
	default:
		break;

	}

	::PostMessage(m_hwnd, UI_MSG_BT_MUSIC_EVENT, wParam, (LPARAM)dwIndID);

}

void CBluetooth::OnBTOpen(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	switch(dwIndID)
	{
	case BZ_IND_OPEN_BLUETOOTH_SUCCESS:
		RETAILMSG(1, (L"[navi_car]: BZ_IND_OPEN_BLUETOOTH_SUCCESS. \n"));
		if (GetConnectStatus() == BT_CONNECT_STATUS_UNCONNECT)
		{
			RETAILMSG(1, (L"[navi_car]: BZ_IND_OPEN_BLUETOOTH_SUCCESS. +++++ start paring.....\n"));
			BZ_StopPairing();
			Sleep(300);
			BZ_StartPairing();
		}
		break;
	case BZ_IND_OPEN_BLUETOOTH_FAIL:
		RETAILMSG(1, (L"[navi_car]: BZ_IND_OPEN_BLUETOOTH_FAIL. \n"));
		break;
	case BZ_IND_ClOSE_BLUETOOTH_SUCCESS:
		RETAILMSG(1, (L"[navi_car]: BZ_IND_ClOSE_BLUETOOTH_SUCCESS. \n"));
		break;
	case BZ_IND_ClOSE_BLUETOOTH_FAIL:
		RETAILMSG(1, (L"[navi_car]: BZ_IND_ClOSE_BLUETOOTH_FAIL. \n"));
		break;
	case BZ_IND_AUTO_CONNECT:
		m_nAutoConnectTry = 3;	// 如果是自动连接，如果失败尝试连接3次
		RETAILMSG(1, (L"[navi_car]: BZ_IND_AUTO_CONNECT. \n"));
		break;
	default:
		break;
	}
}



void CBluetooth::NotifyBTEvtCBK(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	BZ_HandleBTEventEntry();
}

void CBluetooth::BTPairCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	GetInstance()->OnBTPair(dwIndID, wParam, lParam);
}

void CBluetooth::BTConnectCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	GetInstance()->OnBTConnect(dwIndID, wParam, lParam);
}

void CBluetooth::BTCallCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	GetInstance()->OnBTCall(dwIndID, wParam, lParam);
}

void CBluetooth::BTPBMGRCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	GetInstance()->OnBTPBMGR(dwIndID, wParam, lParam);
}

void CBluetooth::BTDeviceQuerryCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	GetInstance()->OnBTDeviceQuerry(dwIndID, wParam, lParam);
}

void CBluetooth::BTA2DPMusicCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	GetInstance()->OnBTA2DPMusic(dwIndID, wParam, lParam);
}

void CBluetooth::BTOpenCallBack(DWORD dwIndID, WPARAM wParam, LPARAM lParam)
{
	GetInstance()->OnBTOpen(dwIndID, wParam, lParam);
}

BOOL CBluetooth::Dial(LPCTSTR szTelNumber)
{
	E_BZMWCALLMANAGER_CALLSTATE_T status;
	if (BZE_OK == BZ_GetCallStatus(0, status)
		&& status == CALLMANAGER_CALLSTATE_IDLE)
	{
		BZ_Dial ((TCHAR*)szTelNumber); 
		return TRUE;
	}  
	
	return FALSE;
}

void CBluetooth::ReDial()
{
	E_BZMWCALLMANAGER_CALLSTATE_T status;
	if (BZE_OK == BZ_GetCallStatus(0, status)
		&& status == CALLMANAGER_CALLSTATE_IDLE)
	{
		BZ_ReDial (); 
	}  
}

void CBluetooth::MusicPrev()
{
	E_BT_AVRCP_CONNECT_STATUS_T status;
	BZ_GetAVRCPStatus(status);
	if (status == BT_AVRCP_CONNECT_STATUS_CONNECTED)
	{
		BZ_MusicPlayPrev();
	}
}

void CBluetooth::MusicPlay()
{
	E_BT_AVRCP_CONNECT_STATUS_T status;
	BZ_GetAVRCPStatus(status);
	if (status == BT_AVRCP_CONNECT_STATUS_CONNECTED)
	{
		BZ_MusicPlay();
	}
}

void CBluetooth::MusicStop()
{
	E_BT_AVRCP_CONNECT_STATUS_T status;
	BZ_GetAVRCPStatus(status);
	if (status == BT_AVRCP_CONNECT_STATUS_CONNECTED)
	{
		BZ_MusicStop();
	}
}

void CBluetooth::MusicPause()
{
	E_BT_AVRCP_CONNECT_STATUS_T status;
	BZ_GetAVRCPStatus(status);
	if (status == BT_AVRCP_CONNECT_STATUS_CONNECTED)
	{
		BZ_MusicPause();
	}
}

void CBluetooth::MusicNext()
{
	E_BT_AVRCP_CONNECT_STATUS_T status;
	BZ_GetAVRCPStatus(status);
	if (status == BT_AVRCP_CONNECT_STATUS_CONNECTED)
	{
		BZ_MusicPlayNext();
	}
}

E_BT_MUSICPLAYSTATUS_T CBluetooth::GetMusicPlayStatus()
{
	E_BT_MUSICPLAYSTATUS_T eMusicStatus;
	if (BZ_GetMusicPlayStatus(eMusicStatus) == BZE_OK)
	{
		return eMusicStatus;
	}

	return BT_MUSICPLAY_STATUS_DISCONNECTED;
}

E_BT_STATUS_T CBluetooth::GetBluetoothStatus()
{
	E_BT_STATUS_T status;
	if (BZE_OK == BZ_GetBlueToothStatus(status))
	{
		return status;
	}

	return MW_BT_STATUS_UNKNOWN;
	
}

E_BT_CONNECT_STATUS_T CBluetooth::GetConnectStatus()
{
	E_BT_CONNECT_STATUS_T status;
	if (BZE_OK == BZ_GetConnectStatus(status))
	{
		return status;
	}

	return BT_CONNECT_STATUS_UNCONNECT;
}

const MediaAttribute_T* CBluetooth::GetMediaAttribute()
{
	if (GetConnectStatus() == BT_CONNECT_STATUS_CONNECTED)
	{
		BZ_GetID3Info(&m_MediaAttribute);
	}
	else
	{
		memset(&m_MediaAttribute, 0, sizeof(MediaAttribute_T));
	}

	return &m_MediaAttribute;
}

const A2DPPlayState_T* CBluetooth::GetPlayStateInfo()
{
	if (GetConnectStatus() == BT_CONNECT_STATUS_CONNECTED)
	{
		BZ_GetPlayStateInfo(&m_PlayStateInfo);
	}
	else
	{
		memset(&m_PlayStateInfo, 0, sizeof(A2DPPlayState_T));
	}

	return &m_PlayStateInfo;
}

BOOL CBluetooth::DeletePairedRec(UINT nIndex)
{
	// 获取远端设备名称
	TCHAR    wszDevName[MAX_LEN_DEVICE_NAME]= {0,};
	TCHAR    wszAddr[MAX_LEN_DEVICE_ADDR] = {0, };
	P_BT_PAIREDRECORDINFO_T info[1] = {NULL, };

	if (BZE_OK == BZ_GetRemoteDevInfo(wszDevName, MAX_LEN_DEVICE_NAME, wszAddr, MAX_LEN_DEVICE_ADDR))
	{
		if ( BZE_OK == BZ_GetPairedRecs(nIndex, 1, &info[0]))
		{
			// 如果当前连接设备与要删除是同一个，直接返回就可以了
			E_BT_CONNECT_STATUS_T status = GetConnectStatus();
			if (_tcscmp(wszAddr, info[0]->wszDevAddr) == 0 
				&& (status == BT_CONNECT_STATUS_CONNECTING || status == BT_CONNECT_STATUS_CONNECTED))
			{
				return FALSE;
			}
		}
	}

	BZ_DeletePairedRec(nIndex);

	return TRUE;
}

void CBluetooth::GetPairedRecs(CStringArray &recsArray)
{
	UINT count = 0;
	if ( BZE_OK == BZ_GetPairedRecCnt(count))
	{
		// 最多只获取10个
		count = min(count, 10);
		P_BT_PAIREDRECORDINFO_T info[10] = {NULL, };
		if ( BZE_OK == BZ_GetPairedRecs(0, count, &info[0]))
		{
			for (UINT i=0; i<count; i++)
			{
				recsArray.Add(info[i]->wszDevName);
			}
		}
	}
}

void CBluetooth::ConnectByPairedRec(UINT nIndex)
{
	// 获取远端设备名称
	TCHAR    wszDevName[MAX_LEN_DEVICE_NAME]= {0,};
	TCHAR    wszAddr[MAX_LEN_DEVICE_ADDR] = {0, };
	P_BT_PAIREDRECORDINFO_T info[1] = {NULL, };

	if (BZE_OK == BZ_GetRemoteDevInfo(wszDevName, MAX_LEN_DEVICE_NAME, wszAddr, MAX_LEN_DEVICE_ADDR))
	{
		if ( BZE_OK == BZ_GetPairedRecs(nIndex, 1, &info[0]))
		{
			// 如果当前连接设备与要连接是同一个，直接返回就可以了
			E_BT_CONNECT_STATUS_T status = GetConnectStatus();
			if (_tcscmp(wszAddr, info[0]->wszDevAddr) == 0 
				&& (status == BT_CONNECT_STATUS_CONNECTING || status == BT_CONNECT_STATUS_CONNECTED))
			{
				return;
			}
		}
	}

	Disconnect();
	Sleep(500);

	BZ_ConnectByPairedRec(nIndex);
	m_nAutoConnectTry = 3;
}

BOOL CBluetooth::GetAudioOutput()
{
	if (GetConnectStatus() == BT_CONNECT_STATUS_CONNECTED)
	{
		return BZ_GetAudioOutput();
	}

	return FALSE;
}

void CBluetooth::SetAudioOutput(BOOL bByPhone)
{
	if (GetConnectStatus() == BT_CONNECT_STATUS_CONNECTED)
	{
		BZ_SetAudioOutput(bByPhone);
	}
}

BOOL CBluetooth::IsMicMuteEnable()
{
	return BZ_IsMicMuteEnable();
}

void CBluetooth::EnableMicMute(BOOL bEnable)
{
	BZ_EnableMicMute(bEnable);
}


LPCTSTR CBluetooth::GetRemoteDeviceName()
{
	return m_strRemoteDeviceName;
}

LPCTSTR CBluetooth::GetRemoteDeviceAddr()
{
	return m_strRemoteDeviceAddr;
}


BOOL CBluetooth::LoadLocalCallInfo()
{
	BOOL result = FALSE;
	WORD flag = 0;
	FILE* fp = _tfopen(LOCAL_RECENTS_CALL_INFO_PATH, L"rb");
	if (fp)
	{		
		if (fread(&flag, 1, sizeof(WORD), fp) != sizeof(WORD) || flag != 0xE23C)
		{
			goto EXIT;
		}

		m_localCallInfo.RemoveAll();
		LOCAL_RECENTS_CALL_INFO callinfo;
		while (fread(&callinfo, 1, sizeof(LOCAL_RECENTS_CALL_INFO), fp) == sizeof(LOCAL_RECENTS_CALL_INFO))
		{
			m_localCallInfo.InsertAt(0, callinfo);
		}
		result = TRUE;
	}

EXIT:
	if (fp)
	{
		fclose(fp);
		fp = NULL;
	}
	return result;
}

BOOL CBluetooth::SaveLocalCallInfo()
{
	BOOL result = FALSE;
	WORD flag = 0xE23C;
	FILE* fp = _tfopen(LOCAL_RECENTS_CALL_INFO_PATH, L"wb");
	if (fp)
	{		
		if (fwrite(&flag, 1, sizeof(WORD), fp) != sizeof(WORD))
		{
			goto EXIT;
		}

		for (int i=0; i<m_localCallInfo.GetCount(); i++)
		{
			if (fwrite(&m_localCallInfo.GetAt(i), 1, sizeof(LOCAL_RECENTS_CALL_INFO), fp) != sizeof(LOCAL_RECENTS_CALL_INFO))
			{
				goto EXIT;
			}
		}

		result = TRUE;
	}

EXIT:
	if (fp)
	{
		fclose(fp);
		fp = NULL;
	}
	return result;
}

BOOL CBluetooth::AddLocalCallInfo(LOCAL_RECENTS_CALL_INFO* pCallInfo)
{
	if (pCallInfo && GetConnectStatus() == BT_CONNECT_STATUS_CONNECTED)
	{
		m_localCallInfo.InsertAt(0, *pCallInfo);
		// 如果记录太多,移除最后一个
		if(m_localCallInfo.GetCount() > MAX_LOCAL_CALL_INFO_COUNT)
		{
			m_localCallInfo.RemoveAt(m_localCallInfo.GetCount()-1);
		}
		
		PostMessage(m_hwnd, UI_MSG_BT_LOCAL_CALLINFO_CHANGE, 0, (LPARAM)pCallInfo->ciPath);

		return TRUE;
	}

	return FALSE;
}

static BOOL _IsEqualTime(SYSTEMTIME* ptm1, SYSTEMTIME *ptm2)
{
	if (ptm1 && ptm2)
	{
		if (ptm1->wYear == ptm2->wYear
			&& ptm1->wMonth == ptm2->wMonth
			&& ptm1->wDay == ptm2->wDay
			&& ptm1->wHour == ptm2->wHour
			&& ptm1->wMinute == ptm2->wMinute
			&& ptm1->wSecond == ptm2->wSecond)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CBluetooth::RemoveLocalCallInfo(SYSTEMTIME* pTime, E_BT_PBAP_SYNC_PATH_T ePath)
{
	if (GetConnectStatus() != BT_CONNECT_STATUS_CONNECTED)
	{
		RETAILMSG(1, (L"[navi_car]: RemoveLocalCallInfo() warning, BT not connected.\n"));
		return FALSE;
	}

	if (ePath != BT_PBAP_SYNC_PATH_RECEIVE_CALLHISTORY
		|| ePath != BT_PBAP_SYNC_PATH_DIALED_CALLHISTORY
		|| ePath != BT_PBAP_SYNC_PATH_MISSED_CALLHISTORY
		|| ePath != BT_PBAP_SYNC_PATH_ALL_CALLHISTORY)
	{
		RETAILMSG(1, (L"[navi_car]: RemoveLocalCallInfo() warning, ePath Parameter error.\n"));
		return FALSE;
	}

	if (pTime == NULL)	// 清除所有记录
	{
		if (ePath == BT_PBAP_SYNC_PATH_ALL_CALLHISTORY)	// 所有记录
		{
			for (int i=m_localCallInfo.GetCount()-1; i>=0; i--)
			{
				if (m_strRemoteDeviceAddr.CompareNoCase(m_localCallInfo.GetAt(i).ciRemoteDeviceAddr) == 0)
				{
					m_localCallInfo.RemoveAt(i);
				}
			}
		}
		else	// 指定类型的所有记录
		{
			for (int i=m_localCallInfo.GetCount()-1; i>=0; i--)
			{
				if (m_strRemoteDeviceAddr.CompareNoCase(m_localCallInfo.GetAt(i).ciRemoteDeviceAddr) == 0
					&& m_localCallInfo.GetAt(i).ciPath == ePath)
				{
					m_localCallInfo.RemoveAt(i);
				}
			}
		}
		return TRUE;
	}

	// 清除指定记录
	for (int i=m_localCallInfo.GetCount()-1; i>=0; i--)
	{
		if (_IsEqualTime(&m_localCallInfo.GetAt(i).ciTime, pTime)
			&& m_strRemoteDeviceAddr.CompareNoCase(m_localCallInfo.GetAt(i).ciRemoteDeviceAddr) == 0
			&& m_localCallInfo.GetAt(i).ciPath == ePath)
		{
			m_localCallInfo.RemoveAt(i);
		}
	}


	return TRUE;
}

BOOL CBluetooth::GetLocalCallInfo(CArray<LOCAL_RECENTS_CALL_INFO> &aCallInfo, E_BT_PBAP_SYNC_PATH_T ePath)
{
	if (GetConnectStatus() != BT_CONNECT_STATUS_CONNECTED)
	{
		return FALSE;
	}

	aCallInfo.RemoveAll();

	if (ePath == BT_PBAP_SYNC_PATH_ALL_CALLHISTORY)	// 所有记录
	{
		for (int i=0; i<m_localCallInfo.GetCount(); i++)
		{
			if (m_strRemoteDeviceAddr.CompareNoCase(m_localCallInfo.GetAt(i).ciRemoteDeviceAddr) == 0)
			{
				aCallInfo.Add(m_localCallInfo.GetAt(i));
			}
		}
	}
	else	// 指定类型的所有记录
	{
		for (int i=0; i<m_localCallInfo.GetCount(); i++)
		{
			if (m_strRemoteDeviceAddr.CompareNoCase(m_localCallInfo.GetAt(i).ciRemoteDeviceAddr) == 0
				&& ePath == m_localCallInfo.GetAt(i).ciPath)
			{
				aCallInfo.Add(m_localCallInfo.GetAt(i));
			}
		}
	}

	return TRUE;
}


BOOL CBluetooth::GetLocalCallInfo(int nIndex, LOCAL_RECENTS_CALL_INFO &callInfo, E_BT_PBAP_SYNC_PATH_T ePath)
{
	if (GetConnectStatus() != BT_CONNECT_STATUS_CONNECTED)
	{
		return FALSE;
	}

	int i = GetLocalCallInfoIndex(nIndex, ePath);
	if (i >= 0)
	{
		callInfo = m_localCallInfo.GetAt(i);
		return TRUE;
	}

	return FALSE;
}



BOOL CBluetooth::RemoveLocalCallInfo(int nIndex, E_BT_PBAP_SYNC_PATH_T ePath)
{
	if (GetConnectStatus() != BT_CONNECT_STATUS_CONNECTED)
	{
		return FALSE;
	}

	int i = GetLocalCallInfoIndex(nIndex, ePath);
	if (i >= 0)
	{
		m_localCallInfo.RemoveAt(i);
		return TRUE;
	}

	return FALSE;
}

int CBluetooth::GetLocalCallInfoIndex(int nIndex, E_BT_PBAP_SYNC_PATH_T ePath)
{
	int count = 0;
	if (ePath == BT_PBAP_SYNC_PATH_ALL_CALLHISTORY)	// 所有记录
	{
		for (int i=0; i<m_localCallInfo.GetCount(); i++)
		{
			if (m_strRemoteDeviceAddr.CompareNoCase(m_localCallInfo.GetAt(i).ciRemoteDeviceAddr) == 0)
			{
				if (count == nIndex)	// 找到了
				{
					return i;
				}
				count++;
			}
		}
	}
	else	// 指定类型的所有记录
	{
		for (int i=0; i<m_localCallInfo.GetCount(); i++)
		{
			if (m_strRemoteDeviceAddr.CompareNoCase(m_localCallInfo.GetAt(i).ciRemoteDeviceAddr) == 0
				&& ePath == m_localCallInfo.GetAt(i).ciPath)
			{
				if (count == nIndex)	// 找到了
				{
					return i;
				}
				count++;
			}
		}
	}

	return -1;
}