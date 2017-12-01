#pragma once
#include "wceuirealwnd.h"
#include "wceuidlg.h"
#include "GpsProxy.h"
#include "RpcMCU.h"
#include "winhook.h"
#include "CanBus.h"

#define TIMER_ID_CHECK_FASTCAMERA		1001
// 检测导航是否已经退出及是否处于前台运行
#define TIMER_ID_CHECK_NAVIGATION		1002

#define TIMER_ID_DVD_EJECT_ENABLE		1003
#define DVD_EJECT_DELAY_TIME			4000	// 4秒

#define TIMER_ID_DVD_EJECT				1004

#define TIMER_ID_LOAD_SD_SOURCE			1005
#define TIMER_ID_LOAD_USB_SOURCE		1006
#define TIMER_ID_LOAD_USB_REAR_SOURCE	1007
#define TIMER_ID_LOAD_VDISK_SOURCE		1008

// 屏保
#define TIMER_ID_SCREEN_SAVER			1010

// 加载记忆的源
#define TIMER_ID_LOAD_LAST_SOURCE		1011

// 延时设定导航发音结束状态
#define TIMER_ID_NAVI_STREAM_DELAY		1012

// 导航自动加载3秒后，认为导航创建窗口已经完成,取消APP窗口的TOPMOST属性
#define TIMER_ID_NAVI_LOAD_COMPLETE		1013

// 开机后延时加载蓝牙, IPOD
#define TIMER_ID_DELAY_LOAD_BT			1014
#define TIMER_ID_DELAY_LOAD_IPOD		1015

// 后排源的UI需要定时刷新,因为前台有可能把该UI隐藏了
#define TIMER_ID_REFRESH_REAR_UI		1016

// 只有倒车和BT会在动画过程中触发显示其它窗口，这里特殊处理下. (可以把这个功能移到CDlgManager中统一处理比较好)
#define TIMER_ID_DELAY_SHOW_BTCALL		1017
#define TIMER_ID_DELAY_SHOW_CAMERA		1018

// 开机自动运行导航,因为导航卡有可能加载比较慢,用计时器确保能启动导航成功
#define TIMER_ID_NAVI_AUTO_LOAD			1019

// DAC的gpio口拉低下
#define TIMER_ID_SET_GPIO				1020

#define TIMER_ID_LOAD_BT_DLG			1021

// 触摸按键
#define TIMER_ID_TOUCHKEY_LONG_PRESS	1022
#define TIMER_ID_TOUCHKEY_MULTI_CMD		1023

// 假关机后，点亮屏幕时延时下,以免显示出主界面
#define TIMER_ID_SLEEP_BACKLIGHT_ENABLE			1024
#define SLEEP_BACKLIGHT_ENABLE_DELAY			1500

// 关屏后倒车BT，延时开屏
#define TIMER_ID_TP_CLICK						1025
#define SLEEP_TP_CLICK_DELAY					1000

#define TIMER_ID_REFRESH_LED					1026

// easyconnected自己处理
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

	// GPSProxy开放出来，供其它页面直接获取GPS信息
	CGpsProxy* GetGpsProxy();

	// 检测存储卡插拔动作
	static UINT StorageCardMonitorProc(LPVOID pParam);
	// 用OS提供的专门接口来监测导航卡拔出动作
	static UINT CMainWnd::SDCardMonitorProc(LPVOID pParam);

	// 处理触摸按键
	void OnTouchKeyDown(POINT point);
	void OnTouchKeyUp(POINT point);
	WORD m_Touchkey;	// 记录触摸按键
	WORD m_TouchkeyLong;	// 对应的长按键
	BOOL m_TouchKeyDown;	// 触摸按键被按下
	BOOL m_TouchKeyLongPressCmdSent; // 支持长按时，如果命令已发出，置标志为TRUE
	BOOL m_TouchKeyMultiCmdSent; // 支持连续发键值时，如果命令已发出，置标志为TRUE
protected:   

	// 开机时，初始化一个计时器，检测开机倒车状态变化，如果倒车结束，触发一个消息通知该事件 
	void InitFastCamera();

	// 建立与MCU的通信,必须在开机倒车结束后
	BOOL InitMCUComm();
	
	// 与MCU建立通信后，有些参数需主动获取，来初始APP系统
	// 比如时间，DISK状态，刹车状态等
	void InitMCUParams();

protected:
	static CMainWnd* m_pthis;

	// GPS
	CGpsProxy m_GPSProxy;

	// 开机时间
	DWORD m_dwAppStartTick;

	// 假关机
	void OnSleep();	// 假关机
	void OnSleepAwake();	// 假关机唤醒
	// bLoadLastSource : 标识是否加载上一次假关机时的源, DVD入碟时可唤醒机器,但不需加载上一次的源,直接进DVD
	void OnSleepPower(BOOL bLoadLastSource=TRUE);		// 假关机后开机
	SOURCE_ID m_sidSleep;	// 假关机时,源ID

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
	// 根据当前系统状态来调整音量,有可能是媒体或BT或GPS
	void OnVolumeChange(int nStep);
	// 改变系统MUTE状态
	void OnMuteChange();
	// MCU设备状态表变化时,需处理倒车,大灯检测,刹车等
	void OnMCUStatusChange(MCU_STATUS* pstatus);
	// 收音状态变化
	void OnRadioStatus();
	void ShowRDSDlg();

	void ShowCameraDlg();
	// BT来电
	void OnBTCall(DWORD dwEventID);
	void ShowBTCallDlg();
	// 黑屏时点击屏幕
	void OnTPClick(WPARAM wParam , MSLLHOOKSTRUCT* phs);

	void OnLoadSourceNotify(SOURCE_ID sid, DVP_SINK sink);
	void OnStopSourceNotify(SOURCE_ID sid);


	// 按键响应
	void OnKeyNavi();
	void OnKeyHome();		// HOME键,回到主界面
	void OnKeyHomeSrc();	// 在主界面与源界面之间切换
	void OnKeyPlayControl(UINT key, UINT param);	// 播放控制键
	void OnKeyEject(UINT param);	// 处理EJECT键	(包括进碟和出碟)
	void OnDVDEject();	// 出碟
	void OnDVDTrayIn();	// 进碟
	void OnKeyTuner();	// 切源到收音,如果已在收音,切波段
	void OnKeyDVD();	// DVD
	void OnKeyTouchMenu();	// 进触摸校准
	void OnKeySourceMode();	// 切模
	void OnKeyPhoneOn();		// 方控上的接电话按键
	void OnKeyPhoneLink();
	void OnKeySelEQ();
	void OnKeyEQ();
	void OnKeySetting();
	void OnKeyOSD();
	void OnKeyLoudness();
	void OnKeySubwoofer();

	// 根据检测事件对源进行管理
	void OniPodConnect(BOOL bConnect);

	// usb插拔时,或受干扰时的处理,
	void OnMassStoragePnP(WPARAM wParam, LPARAM lParam);

	// 
	void OnMassStoragePlug(UI_MASS_STORAGE_TYPE msType, UI_MASS_STORAGE_PLUG msAction);

	// gps发声
	// bStart : 0 ~ 停止发音，  1 ~ 开始发音
	void OnNavStreamNotify(BOOL bStart);

	// 窗口active或deactive时触发WM_ACTIVATE
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

	// 用GPS时间来设置系统时间,  如果还没定位到，设置时间设置标志
	void GPSSetTime();

	// 出碟
	void DVDEject();


	// 因为存在vdisk录制的特殊状态,刷新后排UI时,不能直接使用source manager管理的数据
	SOURCE_ID GetRearSrcID();
	void StopVdiskRip();	// 如果vdisk在录制,停止录制功能

	// 刚从DVD切到其它源时DVD还在转,不能马上出碟,要延时一段时间(4秒)才能出碟,用这个变量来控制
	// 刚切源时禁止出碟,延时一段时间后,才重新置此标志
	// 是否充许DVD出碟
	BOOL m_bDVDEjectEnable;

	//	标识是否在正在播放自己的铃声
	BOOL m_bIncomingRing;

	// 标识窗口是否设置了TOPMOST属性(应该有API可以查询该属性)
	BOOL m_bIsTopMostWindow;

	// RDS报警时禁止切源,结束报警后才切源(插拔USB,出碟等可能引起切源)
	SOURCE_ID m_sidToLoad;
	DVP_SINK  m_sinkToLoad;

public:
	// 加载指定的源,且直接指定前后排属性
	void LoadSource(SOURCE_ID sid, DVP_SINK sink);
	// UI上面触发前排源加载, 但根据当前状态(如无后排功能),也可能是前后排同时加载该源
	void LoadUiFrontSource(SOURCE_ID sid);
	// 只有支持双工时,点击后排的源功能，才需调此函数加载后排源
	void LoadUiRearSource(SOURCE_ID sid);


	void LoadMSDKSource(SOURCE_ID sid);
	void OnSourceExit(SOURCE_ID sid);	// 源要退出,切换到上一个可用源
	BOOL IsSourceAvailable(SOURCE_ID sid);

	// 屏保开始计时
	void RestartScreenSaver();
	// 进入屏保
	void OnScreenSaver();
	void ExitScreenSaver();

	void SetTopMostWindowFlag(BOOL bTop);
	// 当前窗口是否是前台窗口
	BOOL IsForegroundWindow();

	// DVD加载或退出时调用这个接口,用来管理正确出碟时机
	void OnDVDLoading(BOOL bLoading);

	// 启动导航
	BOOL LoadNavigation(BOOL bShowWarning=TRUE);
	void ShowNaviErrorMsg();	// 导航加载失败时显示对应的提示信息

	// 是否在开机过程中,开机过程中如果有插拔事件,会忽略
	BOOL IsLoadingApp();
	void RemoveTopmost();	// 导航后台启动成功后,如果需弹出第三方程序,需先移除topmost属性
	void SetTopmost();	

	// gps 前后台变化时,调用该接口
	void OnGpsForeground(BOOL bForegrounpd);

	RECT m_rcNeedPaint;
};
