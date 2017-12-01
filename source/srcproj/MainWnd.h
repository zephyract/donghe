#pragma once
#include "wceuirealwnd.h"
#include "wceuidlg.h"
#include "GpsProxy.h"
#include "RpcMCU.h"
#include "winhook.h"
#include "CanBus.h"

#define TIMER_ID_CHECK_FASTCAMERA		1001
// ��⵼���Ƿ��Ѿ��˳����Ƿ���ǰ̨����
#define TIMER_ID_CHECK_NAVIGATION		1002

#define TIMER_ID_DVD_EJECT_ENABLE		1003
#define DVD_EJECT_DELAY_TIME			4000	// 4��

#define TIMER_ID_DVD_EJECT				1004

#define TIMER_ID_LOAD_SD_SOURCE			1005
#define TIMER_ID_LOAD_USB_SOURCE		1006
#define TIMER_ID_LOAD_USB_REAR_SOURCE	1007
#define TIMER_ID_LOAD_VDISK_SOURCE		1008

// ����
#define TIMER_ID_SCREEN_SAVER			1010

// ���ؼ����Դ
#define TIMER_ID_LOAD_LAST_SOURCE		1011

// ��ʱ�趨������������״̬
#define TIMER_ID_NAVI_STREAM_DELAY		1012

// �����Զ�����3�����Ϊ�������������Ѿ����,ȡ��APP���ڵ�TOPMOST����
#define TIMER_ID_NAVI_LOAD_COMPLETE		1013

// ��������ʱ��������, IPOD
#define TIMER_ID_DELAY_LOAD_BT			1014
#define TIMER_ID_DELAY_LOAD_IPOD		1015

// ����Դ��UI��Ҫ��ʱˢ��,��Ϊǰ̨�п��ܰѸ�UI������
#define TIMER_ID_REFRESH_REAR_UI		1016

// ֻ�е�����BT���ڶ��������д�����ʾ�������ڣ��������⴦����. (���԰���������Ƶ�CDlgManager��ͳһ����ȽϺ�)
#define TIMER_ID_DELAY_SHOW_BTCALL		1017
#define TIMER_ID_DELAY_SHOW_CAMERA		1018

// �����Զ����е���,��Ϊ�������п��ܼ��رȽ���,�ü�ʱ��ȷ�������������ɹ�
#define TIMER_ID_NAVI_AUTO_LOAD			1019

// DAC��gpio��������
#define TIMER_ID_SET_GPIO				1020

#define TIMER_ID_LOAD_BT_DLG			1021

// ��������
#define TIMER_ID_TOUCHKEY_LONG_PRESS	1022
#define TIMER_ID_TOUCHKEY_MULTI_CMD		1023

// �ٹػ��󣬵�����Ļʱ��ʱ��,������ʾ��������
#define TIMER_ID_SLEEP_BACKLIGHT_ENABLE			1024
#define SLEEP_BACKLIGHT_ENABLE_DELAY			1500

// �����󵹳�BT����ʱ����
#define TIMER_ID_TP_CLICK						1025
#define SLEEP_TP_CLICK_DELAY					1000

#define TIMER_ID_REFRESH_LED					1026

// easyconnected�Լ�����
#define TIMER_ID_EASYCONNECTED_AD2P				1027

class CMainWnd :
	public CWceUiRealWnd
{
public:
	CMainWnd(void);
	~CMainWnd(void);
	virtual void OnInitDialog();
	virtual void OnClose();
	virtual void OnLButtonDown(UINT nFlags, POINT point);
	virtual void OnLButtonUp(UINT nFlags, POINT point);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	virtual void OnPaint(HDC dc, RECT rcNeedPaint);
	virtual BOOL PreDrawWnd(CWceUiGenericBitmap* pBitmap);

	// interface
	static CMainWnd* GetInstance();

	// GPSProxy���ų�����������ҳ��ֱ�ӻ�ȡGPS��Ϣ
	CGpsProxy* GetGpsProxy();

	// ���洢����ζ���
	static UINT StorageCardMonitorProc(LPVOID pParam);
	// ��OS�ṩ��ר�Žӿ�����⵼�����γ�����
	static UINT CMainWnd::SDCardMonitorProc(LPVOID pParam);

	// ����������
	void OnTouchKeyDown(POINT point);
	void OnTouchKeyUp(POINT point);
	WORD m_Touchkey;	// ��¼��������
	WORD m_TouchkeyLong;	// ��Ӧ�ĳ�����
	BOOL m_TouchKeyDown;	// ��������������
	BOOL m_TouchKeyLongPressCmdSent; // ֧�ֳ���ʱ����������ѷ������ñ�־ΪTRUE
	BOOL m_TouchKeyMultiCmdSent; // ֧����������ֵʱ����������ѷ������ñ�־ΪTRUE
protected:   

	// ����ʱ����ʼ��һ����ʱ������⿪������״̬�仯�������������������һ����Ϣ֪ͨ���¼� 
	void InitFastCamera();

	// ������MCU��ͨ��,�����ڿ�������������
	BOOL InitMCUComm();
	
	// ��MCU����ͨ�ź���Щ������������ȡ������ʼAPPϵͳ
	// ����ʱ�䣬DISK״̬��ɲ��״̬��
	void InitMCUParams();

protected:
	static CMainWnd* m_pthis;

	// GPS
	CGpsProxy m_GPSProxy;

	// ����ʱ��
	DWORD m_dwAppStartTick;

	// �ٹػ�
	void OnSleep();	// �ٹػ�
	void OnSleepAwake();	// �ٹػ�����
	// bLoadLastSource : ��ʶ�Ƿ������һ�μٹػ�ʱ��Դ, DVD���ʱ�ɻ��ѻ���,�����������һ�ε�Դ,ֱ�ӽ�DVD
	void OnSleepPower(BOOL bLoadLastSource=TRUE);		// �ٹػ��󿪻�
	SOURCE_ID m_sidSleep;	// �ٹػ�ʱ,ԴID

	// message handler
protected:
	void OnCanBusData(CANBUS_DATA_TYPE cdt);
	void OnDlgChange();
	void OnLanguageChange(int lan_id);
	void OnAccOff();
	void OnAccON();
	void OnBKChange(int index);
	void OnGPSUpdate();
	void OnFastCameraExit();
	void OnMcuKeyCommand(UINT key, UINT param);
	// ���ݵ�ǰϵͳ״̬����������,�п�����ý���BT��GPS
	void OnVolumeChange(int nStep);
	// �ı�ϵͳMUTE״̬
	void OnMuteChange();
	// MCU�豸״̬��仯ʱ,�账����,��Ƽ��,ɲ����
	void OnMCUStatusChange(MCU_STATUS* pstatus);
	// ����״̬�仯
	void OnRadioStatus();
	void ShowRDSDlg();

	void ShowCameraDlg();
	// BT����
	void OnBTCall(DWORD dwEventID);
	void ShowBTCallDlg();
	// ����ʱ�����Ļ
	void OnTPClick(WPARAM wParam , MSLLHOOKSTRUCT* phs);

	void OnLoadSourceNotify(SOURCE_ID sid, DVP_SINK sink);
	void OnStopSourceNotify(SOURCE_ID sid);


	// ������Ӧ
	void OnKeyNavi();
	void OnKeyHome();		// HOME��,�ص�������
	void OnKeyHomeSrc();	// ����������Դ����֮���л�
	void OnKeyPlayControl(UINT key, UINT param);	// ���ſ��Ƽ�
	void OnKeyEject(UINT param);	// ����EJECT��	(���������ͳ���)
	void OnDVDEject();	// ����
	void OnDVDTrayIn();	// ����
	void OnKeyTuner();	// ��Դ������,�����������,�в���
	void OnKeyDVD();	// DVD
	void OnKeyTouchMenu();	// ������У׼
	void OnKeySourceMode();	// ��ģ
	void OnKeyPhoneOn();		// �����ϵĽӵ绰����
	void OnKeyPhoneLink();
	void OnKeySelEQ();
	void OnKeyEQ();
	void OnKeySetting();
	void OnKeyOSD();
	void OnKeyLoudness();
	void OnKeySubwoofer();

	// ���ݼ���¼���Դ���й���
	void OniPodConnect(BOOL bConnect);

	// usb���ʱ,���ܸ���ʱ�Ĵ���,
	void OnMassStoragePnP(WPARAM wParam, LPARAM lParam);

	// 
	void OnMassStoragePlug(UI_MASS_STORAGE_TYPE msType, UI_MASS_STORAGE_PLUG msAction);

	// gps����
	// bStart : 0 ~ ֹͣ������  1 ~ ��ʼ����
	void OnNavStreamNotify(BOOL bStart);

	// ����active��deactiveʱ����WM_ACTIVATE
	void OnActiveateWnd(BOOL bActive);

	// timer handler
	void OnTimerFastCamera();
	void OnCheckNavigation();
	void OnTimerLoadMSDKSource(SOURCE_ID sid);
	void OnTimerLoadVDiskSource();
	void OnTimerLoadLastSource();
	void OnTimerRefreshRearUI();
	void OnTimerAutoLoadNavi();
	void OnTimerRefreshLED();

	// ��GPSʱ��������ϵͳʱ��,  �����û��λ��������ʱ�����ñ�־
	void GPSSetTime();

	// ����
	void DVDEject();


	// ��Ϊ����vdisk¼�Ƶ�����״̬,ˢ�º���UIʱ,����ֱ��ʹ��source manager���������
	SOURCE_ID GetRearSrcID();
	void StopVdiskRip();	// ���vdisk��¼��,ֹͣ¼�ƹ���

	// �մ�DVD�е�����ԴʱDVD����ת,�������ϳ���,Ҫ��ʱһ��ʱ��(4��)���ܳ���,���������������
	// ����Դʱ��ֹ����,��ʱһ��ʱ���,�������ô˱�־
	// �Ƿ����DVD����
	BOOL m_bDVDEjectEnable;

	//	��ʶ�Ƿ������ڲ����Լ�������
	BOOL m_bIncomingRing;

	// ��ʶ�����Ƿ�������TOPMOST����(Ӧ����API���Բ�ѯ������)
	BOOL m_bIsTopMostWindow;

	// RDS����ʱ��ֹ��Դ,�������������Դ(���USB,�����ȿ���������Դ)
	SOURCE_ID m_sidToLoad;
	DVP_SINK  m_sinkToLoad;

public:
	// ����ָ����Դ,��ֱ��ָ��ǰ��������
	void LoadSource(SOURCE_ID sid, DVP_SINK sink);
	// UI���津��ǰ��Դ����, �����ݵ�ǰ״̬(���޺��Ź���),Ҳ������ǰ����ͬʱ���ظ�Դ
	void LoadUiFrontSource(SOURCE_ID sid);
	// ֻ��֧��˫��ʱ,������ŵ�Դ���ܣ�������˺������غ���Դ
	void LoadUiRearSource(SOURCE_ID sid);


	void LoadMSDKSource(SOURCE_ID sid);
	void OnSourceExit(SOURCE_ID sid);	// ԴҪ�˳�,�л�����һ������Դ
	BOOL IsSourceAvailable(SOURCE_ID sid);

	// ������ʼ��ʱ
	void RestartScreenSaver();
	// ��������
	void OnScreenSaver();
	void ExitScreenSaver();

	void SetTopMostWindowFlag(BOOL bTop);
	// ��ǰ�����Ƿ���ǰ̨����
	BOOL IsForegroundWindow();

	// DVD���ػ��˳�ʱ��������ӿ�,����������ȷ����ʱ��
	void OnDVDLoading(BOOL bLoading);

	// ��������
	BOOL LoadNavigation(BOOL bShowWarning=TRUE);
	void ShowNaviErrorMsg();	// ��������ʧ��ʱ��ʾ��Ӧ����ʾ��Ϣ

	// �Ƿ��ڿ���������,��������������в���¼�,�����
	BOOL IsLoadingApp();
	void RemoveTopmost();	// ������̨�����ɹ���,����赯������������,�����Ƴ�topmost����
	void SetTopmost();	

	// gps ǰ��̨�仯ʱ,���øýӿ�
	void OnGpsForeground(BOOL bForegrounpd);

	RECT m_rcNeedPaint;
};
