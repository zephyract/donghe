#include "StdAfx.h"
#include "MainWnd.h"
#include "WceUiLayerCtrl.h"
#include "MainDlg.h"
#include "DlgManager.h"
#include "sysUtil.h"
#include "config.h"
#include "interface.h"
#include "ycapi.h"
#include "msdkrender.h"
#include "uiddraw.h"
#include "BluetoothDlg.h"
#include "bluetooth.h"
#include "VolumeManager.h"
#include "tzUtil.h"
#include "dvdrender.h"
#include "SourceManager.h"
#include "DVDStartDlg.h"
#include "winhook.h"
#include "IPODManager.h"
#include "msdkFileManager.h"
#include "vdiskFileManager.h"
#include "VDiskListDlg.h"
#include "usbpatch.h"
#include "pkfuncs.h"
#include "OverlayDlg.h"
#include "CanBus.h"
#include "RadioRDSDlg.h"
#include "phonelink.h"
#include "easyconnected.h"
#include "SettingsAudio.h"
#include "SetAudioDlg.h"

const UINT TIME_IGNORE_SD_USB = 10000;	// 开机8秒内忽略SD，USB卡插入
const UINT TIME_NAVI_STREAM_DELAY = 2000;	// 导航发音结束后,延时一段时间后才真正认为发音结束,防止发音太短调导航音量不方便


void _MyCreateTouchScreenWnd();

CMainWnd* CMainWnd::m_pthis = NULL;
CMainWnd::CMainWnd(void)
: m_bDVDEjectEnable(TRUE)
, m_sidToLoad(INVALID_SOURCE_ID)
{
	m_pthis = this;
	m_bIncomingRing = FALSE;
	m_bIsTopMostWindow = FALSE;
}

CMainWnd::~CMainWnd(void)
{

}

void CMainWnd::OnInitDialog()
{   
	m_dwAppStartTick = GetTickCount();
	SetWindowText(UI_MAIN_WINDOW_NAME);
	YC_DecodeEnable(IOCTL_HAL_ENABLE_3D, TRUE, sizeof(DWORD));
	YC_DecodeEnable(IOCTL_HAL_ENABLE_WMA,FALSE, sizeof(DWORD));
	YC_DecodeEnable(IOCTL_HAL_ENABLE_AC3,TRUE, sizeof(DWORD));
	YC_DecodeEnable(IOCTL_HAL_ENABLE_H264,FALSE, sizeof(DWORD));
	YC_DecodeEnable(IOCTL_HAL_ENABLE_H264,FALSE, 2*sizeof(DWORD));
	YC_DecodeEnable(IOCTL_HAL_ENABLE_DIVX,TRUE, 4*sizeof(DWORD));
	YC_DecodeEnable(IOCTL_HAL_ENABLE_DIVX,TRUE, 5*sizeof(DWORD));

	// 初始化蓝牙
	SetTimer(TIMER_ID_DELAY_LOAD_BT, 3000, NULL);

	// hook
	winhook::init();

	// 初始化Direct Draw
// 	#define DISPLAY_IOCTL_HIDE_BACK_VIDEO        (0x00020002)
// 	ExtEscape(::GetDC(NULL), DISPLAY_IOCTL_HIDE_BACK_VIDEO, 0, NULL,0, NULL);	// 释放开机倒车占用的overlay
// 	CUiDDraw::GetInstance()->InitDDraw(m_hWnd);

	// ipod
	SetTimer(TIMER_ID_DELAY_LOAD_IPOD, 6000, NULL);

	//初始化配置文件
	config::config_init();

	// 是否允许系统使用夏令时
	YC_SetDST(config::get_timezone_dst_enable());

	// 初始化系统状态
	sysutil::nss_init();

	// 根据配置文件及系统状态，对整个系统进行初始化
	sysutil::system_init();

	// GPS初始化, CGpsProxy的初始化延时处理,放在TIMER_ID_LOAD_LAST_SOURCE中处理
	// 不知啥原因，开机就打开串口读写好像不正常
	YC_EnableGpsTimeAutoSet(0);	// GPS不需自动同步,由APP来处理
//	m_GPSProxy.Initialize(m_hWnd, L"COM1:");

	// 初始化开机倒车状态,收到开机倒车结束消息后才初始化与MCU的通信
	InitFastCamera();

	// 初始化MSDK和DVD
	CMsdkRender::GetInstance()->Initialize(m_hWnd);
	CDvdRender::GetInstance()->Initialize(m_hWnd);

	// 监测导航运行状态
	SetTimer(TIMER_ID_CHECK_NAVIGATION, 500, NULL);
	// 刷新后排UI
	/* --++ rear ui
	SetTimer(TIMER_ID_REFRESH_REAR_UI, 1000, NULL);
	*/

	RestartScreenSaver();

// 	CDlgManager::GetInstance()->CreateDlg(CMainDlg_ID, SOURCE_MAIN);
// 	CDlgManager::GetInstance()->CreateDlg(CEcarDlg_ID, SOURCE_ECAR);
	// MCU通信上后会加载记忆源
	// ...

	// 自动加载导航
	// 后面如果要加载第三方程序时，需先取消TOPMOST属性
	// 此UI不自动运行导航
	if (/*sysutil::nss_get_instance()->gps_autorun || sysutil::nss_get_instance()->gps_running_status != 0*/0)
	{
		SetTimer(TIMER_ID_NAVI_AUTO_LOAD, 2000, NULL);

		if (sysutil::nss_get_instance()->gps_running_status == 1)	// gps需前台运行
		{
			ShowWindow(SW_HIDE);
		}
	}

	HANDLE handle = ::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StorageCardMonitorProc, LPVOID(this), NULL, NULL);
	CloseHandle(handle);

	// 启动导航卡拔出的监控进程
	handle = ::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SDCardMonitorProc, LPVOID(this), NULL, NULL);
	SetThreadPriority(handle, THREAD_PRIORITY_TIME_CRITICAL);
	CloseHandle(handle);

	// 给DAC一个复位信号,拉低50ms(加载记忆源时拉低)  ------|__|--------
//	YC_SetGpioData(config::get_config_audio()->dac_gpio, 1);

	// BT初始化完成后,将CBluetoothDlg加载起来,以完成电话本自动下载
	SetTimer(TIMER_ID_LOAD_BT_DLG, 1000, NULL);


	if (config::get_config_touchkey()->get_enable())
	{
		// 单独创建一个窗口来接收触摸消息
		_MyCreateTouchScreenWnd();

		HANDLE h = CreateEvent(NULL, FALSE, FALSE, L"GetTouchEvent");
		SetEventData(h, 0x1);
		CloseHandle(h);
	}
}

BOOL CMainWnd::InitMCUComm()
{
	BOOL ret = FALSE;
	LONG result;
	TCHAR szport[8];
	_stprintf(szport, L"COM%d:", sysutil::nss_get_instance()->sys_mcu_port);
	result = CRpcMCU::GetInstance()->Open(szport);
	if (result != ERROR_SUCCESS)
	{
		goto LEXIT;
	}

	result = CRpcMCU::GetInstance()->GetDEP()->Setup((CDEPSerial::EBaudrate)sysutil::nss_get_instance()->sys_mcu_baudrate);
	if (result != ERROR_SUCCESS)
	{
		goto LEXIT;
	}


	RPCERR rpcret;
	result = CRpcMCU::GetInstance()->RPC_ShakeHand(&rpcret);
	if (result==ERROR_SUCCESS && rpcret==RPC_SUCCESS)
	{
		ret = TRUE;
	}

LEXIT:
	return ret;
}

void CMainWnd::InitMCUParams()
{
	// 获取一些初始信息(MCU初始化信息表)
	CRpcMCU::GetInstance()->RPC_GetInitTable(protocol::get_mcu_init_table()->data, 
			&protocol::get_mcu_init_table()->extra_data_len);
	// 设置系统时间
	// 如果不是GPS自动定位，或者GPS还没有校正时间(开机时一般GPS没那么快定到位，可以先用MCU的时间)
	if ( !sysutil::nss_get_instance()->ui_time_autoset 
		|| sysutil::nss_get_instance()->ui_time_need_autoset)
	{
		SYSTEMTIME tm;
		protocol::get_mcu_init_table()->get_system_time(&tm);
		// MCU保存的时间为中国时区时间,这里转换为UTC时间来设置系统时间
		ui_time_calculate(&tm, -8, 0, 0);
		::SetSystemTime(&tm);
	}

	// 获取一些状态的初始信息(MCU设备状态表),调用这个接口后，MCU才主动上传状态表
	CRpcMCU::GetInstance()->RPC_GetStatus(protocol::get_mcu_status());
	OnMCUStatusChange(protocol::get_mcu_status());
	// 开机时从MCU获取假关机状态
	sysutil::nss_get_instance()->sys_status = 
		protocol::get_mcu_status()->is_sleep() ? UISS_SLEEP : UISS_NORMAL;

	//---- 初始化MCU Audio相关信息(mute, beep)
	protocol::get_mcu_volume_table()->is_mute = 0/*sysutil::nss_get_instance()->is_audio_mute()*/;
	protocol::get_mcu_volume_table()->key_beep_on = sysutil::nss_get_instance()->audio_key_beep;
	protocol::get_mcu_sys_para()->general_data[0] = sysutil::nss_get_instance()->audio_key_beep;	// MCU判断时,也判断了该标志位
	CRpcMCU::GetInstance()->RPC_SetVolume(protocol::get_mcu_volume_table());

	// 设置canbus类型, 0表示不带can
	if (sysutil::nss_get_instance()->sys_canbus_type > 0)
	{
		CRpcMCU::GetInstance()->RPC_SetCanbusType(sysutil::nss_get_instance()->sys_canbus_type);
	}

	SetTimer(TIMER_ID_REFRESH_LED, 500, NULL);
}

void CMainWnd::OnClose()
{
// 	if (GetLayerCtrl()->RootLayer()->FirstChild() == NULL)	// exit directly, no dialog is running.
// 	{
// 		DeInitDDRaw();
// 		EndDialog(0);
// 	}
// 	else	// ask dialog to exit
// 	{
// 		PostMessage(MY_WM_CLOSE);
// 		m_bToExit = TRUE;
// 	}
// 
// 	SaveMMPMemoryFile();
}

void CMainWnd::InitFastCamera()
{
	DWORD status = YC_GetBootBackCarState2();	// 1 ~ 倒车, 0 ~ 未倒车
	if ( status == 1 )	// 检测到开机倒车
	{
		SetTimer(TIMER_ID_CHECK_FASTCAMERA, 200, NULL);
	}
	else
	{
		::PostMessage(HWND_BROADCAST, NAVI_MSG_FASTCAMERA_EXIT, 0, 0);
	}

	sysutil::nss_get_instance()->sys_fastcamera_status = status;

}

LRESULT CMainWnd::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{

	return CWceUiRealWnd::WindowProc(message, wParam, lParam);
}


void CMainWnd::OnPaint(HDC dc, RECT rcNeedPaint)
{
	// 如果当前没显示任何dlg或者当前的dlg为外部程序，不要渲染屏幕,以免引起开机黑屏
	DLG_INFO* pdi = CDlgManager::GetInstance()->GetCurDlgInfo();
	if (::IsRectEmpty(&rcNeedPaint) 
		|| CDlgManager::GetInstance()->GetCurDlgID() == INVALID_DLG_ID
		|| CDlgManager::GetInstance()->IsExternalApp(pdi))
	{
		return;
	}

	m_rcNeedPaint = rcNeedPaint;
	CWceUiRealWnd::OnPaint(dc, rcNeedPaint);
}

BOOL CMainWnd::PreDrawWnd(CWceUiGenericBitmap* pBitmap)
{
	// 在这里将UI输出到overlay
	CDC dc;
	LPBYTE lpbyte;
	dc.CreateCompatibleDC(NULL);
	dc.SelectObject(pBitmap->GetHBitmap(lpbyte));

	if (CUiDDraw::GetInstance()->IsOverlayVisible())
	{
		HDC hdcWnd = ::GetDC(m_hWnd);
		CUiDDraw::GetInstance()->BitBltOverlay(dc.GetSafeHdc(), m_rcNeedPaint.left, m_rcNeedPaint.top, 
			tzutil::_W(&m_rcNeedPaint), tzutil::_H(&m_rcNeedPaint),m_rcNeedPaint.left, m_rcNeedPaint.top);
		::ReleaseDC(m_hWnd, hdcWnd);
	}

	/* -- shut off rear UI

	if (CUiDDraw::GetInstance()->IsRearOverlayVisible())
	{
		DWORD dwTick = GetTickCount();

		SOURCE_ID sid = GetRearSrcID();
		if (CDlgManager::GetInstance()->GetCurDlgInfo()->source_id 	== sid
			&& !CDlgManager::GetInstance()->IsAnimating())
		{
			HDC hdcWnd = ::GetDC(m_hWnd);
			CUiDDraw::GetInstance()->BitBltRearOverlay(dc.GetSafeHdc(), m_rcNeedPaint.left, m_rcNeedPaint.top, 
				tzutil::_W(&m_rcNeedPaint), tzutil::_H(&m_rcNeedPaint),m_rcNeedPaint.left, m_rcNeedPaint.top);
			::ReleaseDC(m_hWnd, hdcWnd);
		}
	}
	*/

	return TRUE;
}

void CMainWnd::OnTouchKeyDown(POINT point)
{
	if (point.x>=WceUiGetScreenWidth() || point.y>=WceUiGetScreenHeight())
	{
		//RETAILMSG(1, (L"*****  X=%d, Y=%d\n", point.x, point.y));
		for (int i=0; i<config::get_config_touchkey()->count; i++)
		{
			if (::PtInRect(config::get_config_touchkey()->get_rect(i), point))
			{
				m_TouchKeyDown = TRUE;
				m_TouchKeyLongPressCmdSent = FALSE;
				m_TouchKeyMultiCmdSent = FALSE;
				m_Touchkey = config::get_config_touchkey()->get_key(i);
				m_TouchkeyLong = config::get_config_touchkey()->get_key_long(i);

				// 假关机状态时,基本不响应按键,除了power键,如果power键配置成长按了
				// 假关机后，应短按该键就执行power key功能
				if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP
					&& m_TouchkeyLong == K_POWER)
				{
					m_Touchkey = K_POWER;
				}

				if (m_Touchkey == K_VOL_UP || m_Touchkey == K_VOL_DN)
				{
					SetTimer(TIMER_ID_TOUCHKEY_MULTI_CMD, 500, NULL);
				}
				else if (m_TouchkeyLong != 0)	// 支持长按
				{
					SetTimer(TIMER_ID_TOUCHKEY_LONG_PRESS, 500, NULL);
				}


				if (sysutil::nss_get_instance()->sys_status != UISS_ACC_OFF
					&& sysutil::nss_get_instance()->sys_status != UISS_SLEEP)
				{
					ui_mcu_beep();
				}

				return;
			}
		}
	}

}

void CMainWnd::OnTouchKeyUp(POINT point)
{
	if (m_TouchKeyDown)
	{
		m_TouchKeyDown = FALSE;
		KillTimer(TIMER_ID_TOUCHKEY_MULTI_CMD);
		KillTimer(TIMER_ID_TOUCHKEY_LONG_PRESS);

		if (!m_TouchKeyMultiCmdSent && (m_TouchkeyLong == 0 || !m_TouchKeyLongPressCmdSent))	// 不支持长按或长按命令没发出时
		{
			if (m_Touchkey == K_EJECT)
			{
				if (protocol::get_mcu_status()->is_DISK_exist())
				{
					CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_KEY_COMMAND, m_Touchkey, 0);
				}
				else
				{
					CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_KEY_COMMAND, m_Touchkey, 1<<24);
				}
			}
			else
			{
				CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_KEY_COMMAND, m_Touchkey, 0);
			}
		}
	}
}

void CMainWnd::OnLButtonDown(UINT nFlags, POINT point)
{
	if (!CDlgManager::GetInstance()->IsAnimating())
	{
		PostMessage(UI_MSG_LBUTTON_DOWN);
		CWceUiRealWnd::OnLButtonDown(nFlags, point);
	}

	RestartScreenSaver();

	OnTouchKeyDown(point);

	// 任何操作解mute
	// 任何键都起到解mute作用
	// 这个版本在main窗口处理函数已经处理了
// 	CRect rc(0,0,WceUiGetScreenWidth(), WceUiGetScreenHeight());
// 	if (rc.PtInRect(point) && sysutil::nss_get_instance()->is_audio_mute())
// 	{
// 		PostMessage(UI_MSG_MCU_KEY_COMMAND, K_MUTE, 0);
// 	}

}

void CMainWnd::OnLButtonUp(UINT nFlags, POINT point)
{
	if (!CDlgManager::GetInstance()->IsAnimating())
	{
		// 如果关注系统的这个事件，可以接收这个消息
		PostMessage(UI_MSG_LBUTTON_UP);

		CWceUiRealWnd::OnLButtonUp(nFlags, point);
	}

	RestartScreenSaver();

	OnTouchKeyUp(point);
}


void CMainWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ID_CHECK_FASTCAMERA)
	{
		OnTimerFastCamera();
	}
	else if (nIDEvent == TIMER_ID_SET_GPIO)
	{
		KillTimer(TIMER_ID_SET_GPIO);
//		YC_SetGpioData(config::get_config_audio()->dac_gpio, 1);
	}
	else if (nIDEvent == TIMER_ID_HIDE_MSDK_VIDEO_WIN)	// 代理MSDKRender的计时器
	{
		CMsdkRender::GetInstance()->OnTimer(nIDEvent);
	}
	else if (nIDEvent == TIMER_ID_OPEN_BLUETOOTH
		|| nIDEvent == TIMER_ID_BT_AUTO_CONNECT)
	{
		CBluetooth::GetInstance()->OnTimer(nIDEvent);
	}
	else if (nIDEvent == TIMER_ID_SOURCE_LONDING_MUTE)
	{
		CSourceManager::GetInstance()->OnTimer(TIMER_ID_SOURCE_LONDING_MUTE);
	}
	else if (nIDEvent == TIMER_ID_CHECK_NAVIGATION)
	{
		OnCheckNavigation();
	}
	else if (nIDEvent == TIMER_ID_DVD_EJECT_ENABLE)
	{
		KillTimer(TIMER_ID_DVD_EJECT_ENABLE);
		m_bDVDEjectEnable = TRUE;
	}
	else if (nIDEvent == TIMER_ID_DVD_EJECT)
	{
		if (m_bDVDEjectEnable)
		{
			KillTimer(TIMER_ID_DVD_EJECT);
			DVDEject();
		}
	}
	else if (nIDEvent == TIMER_ID_LOAD_VDISK_SOURCE)
	{
		OnTimerLoadVDiskSource();
	}
	else if (nIDEvent == TIMER_ID_LOAD_SD_SOURCE)
	{
		OnTimerLoadMSDKSource(SOURCE_SD);
	}
	else if (nIDEvent == TIMER_ID_LOAD_USB_SOURCE)
	{
		OnTimerLoadMSDKSource(SOURCE_USB);
	}
	else if (nIDEvent == TIMER_ID_LOAD_USB_REAR_SOURCE)
	{
		OnTimerLoadMSDKSource(SOURCE_USB_REAR);
	}
	else if (nIDEvent == TIMER_ID_DELAY_SHOW_BTCALL)
	{
		if (!CDlgManager::GetInstance()->IsAnimating())
		{
			KillTimer(TIMER_ID_DELAY_SHOW_BTCALL);
			ShowBTCallDlg();
		}
	}
	else if (nIDEvent == TIMER_ID_DELAY_SHOW_CAMERA)
	{
		if (!CDlgManager::GetInstance()->IsAnimating())
		{
			KillTimer(TIMER_ID_DELAY_SHOW_CAMERA);
			CDlgManager::GetInstance()->ShowDlg(CCameraDlg_ID, SOURCE_CAMERA);
		}
	}
	else if (nIDEvent == TIMER_ID_SCREEN_SAVER)
	{
		OnScreenSaver();
	}
	else if (nIDEvent == TIMER_ID_LOAD_LAST_SOURCE)
	{
		static BOOL bInit = FALSE;
		if (!bInit)
		{
			bInit = TRUE;
			TCHAR szport[8];
			_stprintf(szport, L"COM%d:", config::get_config_uart()->gps_commport);
			m_GPSProxy.Initialize(m_hWnd, szport, config::get_config_uart()->gps_baudrate);

			// 确保DAC正确复位
//			YC_SetGpioData(config::get_config_audio()->dac_gpio, 0);
			SetTimer(TIMER_ID_SET_GPIO, 50, NULL);
		}

		OnTimerLoadLastSource();
	}
	else if (nIDEvent == TIMER_ID_NAVI_STREAM_DELAY)
	{
		KillTimer(TIMER_ID_NAVI_STREAM_DELAY);
		sysutil::nss_get_instance()->sys_gps_guiding = 0;
		CVolumeManager::GetInstance()->UpdateVolume();
	}
	else if (nIDEvent == TIMER_ID_NAVI_LOAD_COMPLETE)
	{
		KillTimer(TIMER_ID_NAVI_LOAD_COMPLETE);
		SetActiveWindow(m_hWnd);
	}
	else if (nIDEvent == TIMER_ID_NAVI_AUTO_LOAD)
	{
		OnTimerAutoLoadNavi();
	}
	else if (nIDEvent == TIMER_ID_DELAY_LOAD_BT)
	{
		KillTimer(TIMER_ID_DELAY_LOAD_BT);
		CBluetooth::GetInstance()->Initialize(m_hWnd);

		/* --++ ecar
		CDlgManager::GetInstance()->CreateDlg(CEcarDlg_ID, SOURCE_ECAR);
		*/
	}
	else if (nIDEvent == TIMER_ID_LOAD_BT_DLG)
	{
		if (CBluetooth::GetInstance()->IsInitialized())
		{
			KillTimer(TIMER_ID_LOAD_BT_DLG);
			CDlgManager::GetInstance()->CreateDlg(CBluetoothDlg_ID, SOURCE_BT);

			// 延时加载phonelink
			if (config::get_config_functions()->is_support(SOURCE_PHONE_LINK))
			{
				//CPhoneLink::LoadPhoneLink();
			}
		}
	}
	else if (nIDEvent == TIMER_ID_DELAY_LOAD_IPOD)
	{
		KillTimer(TIMER_ID_DELAY_LOAD_IPOD);
		/* --++ ipod
		CIPODManager::GetInstance()->InitIPodSDK(m_hWnd);
		*/
	}
	else if (nIDEvent == TIMER_ID_REFRESH_REAR_UI)
	{
		OnTimerRefreshRearUI();
	}
	else if (nIDEvent == TIMER_ID_TOUCHKEY_LONG_PRESS)
	{
		KillTimer(TIMER_ID_TOUCHKEY_LONG_PRESS);
		CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_KEY_COMMAND, m_TouchkeyLong, 0);
		m_TouchKeyLongPressCmdSent = TRUE;
	}
	else if (nIDEvent == TIMER_ID_TOUCHKEY_MULTI_CMD)
	{
		m_TouchKeyMultiCmdSent = TRUE;
		SetTimer(TIMER_ID_TOUCHKEY_MULTI_CMD, 120, NULL);	// 第一次间隔时间长， 所以这里重新设置下时长
		CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_KEY_COMMAND, m_Touchkey, 0);

	}
	else if (nIDEvent == TIMER_ID_SLEEP_BACKLIGHT_ENABLE)
	{
		if (sysutil::nss_get_instance()->sys_status != UISS_SLEEP)
		{
			KillTimer(TIMER_ID_SLEEP_BACKLIGHT_ENABLE);
			YC_BackLightEnable(TRUE);	// 打开背光
		}
	}
	else if (nIDEvent == TIMER_ID_TP_CLICK)
	{
		KillTimer(TIMER_ID_TP_CLICK);
		PostMessage(UI_MSG_TP_CLICK);
	}
	else if (nIDEvent == TIMER_ID_BT_CONNECTED)
	{
		KillTimer(TIMER_ID_BT_CONNECTED);
		if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_PHONE_LINK)
		{
			// 该版本BT窗口可能被关闭了,这里处理下(BT窗口有处理该工作)
			// 如果当前源是phonelink而且BT连接上了，把A2DP打开
			DLG_INFO *pdi = CDlgManager::GetInstance()->GetActiveDlgInfo(SOURCE_BT);
			if (pdi && pdi->pdlg == NULL)
			{
				CBluetooth::GetInstance()->A2DPActive(TRUE);
				CBluetooth::GetInstance()->MusicPlay();
			}
		}
	}
	else if (nIDEvent == TIMER_ID_REFRESH_LED)
	{
		OnTimerRefreshLED();
	}
	else if (nIDEvent == TIMER_ID_EASYCONNECTED_AD2P)
	{
		CEasyConnected::GetInstance()->OnTimer(nIDEvent);
	}
	else
	{
		CWceUiRealWnd::OnTimer(nIDEvent);
	}

}

void CMainWnd::OnTimerRefreshLED()
{
	BOOL usb = sysutil::nss_get_instance()->is_source_available(SOURCE_USB)
		|| sysutil::nss_get_instance()->is_source_available(SOURCE_USB_REAR);
	BOOL sd = sysutil::nss_get_instance()->is_source_available(SOURCE_SD);
	BOOL dvd = sysutil::nss_get_instance()->is_source_available(SOURCE_DVD);
	BOOL bt = CBluetooth::GetInstance()->IsConnected();

	// 主要是为了控制当前源的图标闪烁效果
	static BOOL visible = FALSE;
	SOURCE_ID sid = CSourceManager::GetInstance()->GetFrontSrcID();
	if (sid == SOURCE_DVD || sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR)
	{
		visible = !visible;
	}

	switch (sid)
	{
	case SOURCE_DVD:
		dvd = visible;
		break;
	case SOURCE_SD:
		sd = visible;
		break;
	case SOURCE_USB:
	case SOURCE_USB_REAR:
		usb = visible;
		break;
	default:
		break;
	}

	ui_led_set_source_available(ui_led_get_data(), sd, usb, dvd, bt);
	CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
}

void CMainWnd::OnTimerAutoLoadNavi()
{
	if (ui_load_navigation())
	{
		KillTimer(TIMER_ID_NAVI_AUTO_LOAD);
		if (sysutil::nss_get_instance()->gps_running_status != 1)	// gps后台启动
		{
			SetTopmost();
		}
	}
	else if (!IsLoadingApp())
	{
		KillTimer(TIMER_ID_NAVI_AUTO_LOAD);		// 太长时间加载不成功,不再尝试
		ShowWindow(SW_SHOW);	// 如果前台导航没加载成功,窗口需显示出来
	}
}

void CMainWnd::OnTimerLoadLastSource()
{
	// 如果是开机假关机,MCU这时是没有打开背光的
	// 如果是要求开机进入假关机状态,直接进入假关机,不加载源
	if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP)
	{
		KillTimer(TIMER_ID_LOAD_LAST_SOURCE);
		OnSleep();
		return;
	}

	SOURCE_ID sid = (SOURCE_ID)sysutil::nss_get_instance()->ui_front_source_id;
	// 如果记忆源是USB或IPOD,因为设备检测可能比较慢,可延时最长8秒加载，只有延长时间内还找不到设备才切换其它源
	if ( !sysutil::nss_get_instance()->is_source_available(sid)
		&& (sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_IPOD_USB || sid == SOURCE_SD)
		&& IsLoadingApp()
		&& sysutil::nss_get_instance()->is_source_available_hw(sid))
	{
		;
	}
	else
	{
		KillTimer(TIMER_ID_LOAD_LAST_SOURCE);
		if (!sysutil::nss_get_instance()->is_source_available(sid))
		{
			sid = SOURCE_FM;
		}
		LoadUiFrontSource(sid);
	}
}

void CMainWnd::ShowBTCallDlg()
{
	if (sysutil::nss_get_instance()->sys_camera_status == 1)	// 正在倒车
	{
		CDlgManager::GetInstance()->SetActiveDlg(CBTCallDlg_ID, SOURCE_BT_CALL);
	}
	else if (sysutil::nss_get_instance()->sys_rds_status)
	{
		CDlgManager::GetInstance()->SetActiveDlg(CBTCallDlg_ID, SOURCE_BT_CALL);
	}
	else
	{
		CDlgManager::GetInstance()->ShowDlg(CBTCallDlg_ID, SOURCE_BT_CALL);
	}

	if (!tzutil::IsBoolEqual(TRUE, protocol::get_mcu_arm_status()->is_bt_active()))	// 如果跟之前状态不一致通知MCU
	{
		protocol::get_mcu_arm_status()->set_bt_active(TRUE);
		CRpcMCU::GetInstance()->RPC_SetArmStatus(protocol::get_mcu_arm_status());
	}

}

void CMainWnd::ShowRDSDlg()
{
	if (sysutil::nss_get_instance()->sys_camera_status == 1)	// 正在倒车
	{
		CDlgManager::GetInstance()->SetActiveDlg(CRadioRDSDlg_ID, SOURCE_FM);
	}
	else
	{
		CDlgManager::GetInstance()->ShowDlg(CRadioRDSDlg_ID, SOURCE_FM);
	}
}

void CMainWnd::OnTimerLoadVDiskSource()
{
	if (/*!CMainDlg::GetInstance()->IsAnimating()*/1)
	{
		KillTimer(TIMER_ID_LOAD_VDISK_SOURCE);

		VDISK_MEMORY* pmemory = &sysutil::nss_get_instance()->vdisk_memory;
		// 播放文件
		int index = CVDiskFileManager::GetInstance()->GetPlayingFileIndex();
		LPCTSTR file_path = CVDiskFileManager::GetInstance()->GetPlayFilePath(index);
		CMsdkRender::GetInstance()->FSetSourceFile(file_path);
	}
	else 
	{
		RETAILMSG(1, (L"is animating................\n"));
	}
}

void CMainWnd::OnTimerLoadMSDKSource(SOURCE_ID sid)
{
	if (/*!CMainDlg::GetInstance()->IsAnimating()*/1)
	{
		if (sid == SOURCE_SD)				KillTimer(TIMER_ID_LOAD_SD_SOURCE);
		else if(sid == SOURCE_USB)			KillTimer(TIMER_ID_LOAD_USB_SOURCE);
		else if(sid == SOURCE_USB_REAR)		KillTimer(TIMER_ID_LOAD_USB_REAR_SOURCE);

		MSDK_MEMORY* pmemory = ui_msdk_get_memory(sid);
		// 播放文件
		if (pmemory->type == MSDK_FILE_MUSIC || pmemory->type == MSDK_FILE_VIDEO)
		{
			int index = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(pmemory->type);
			LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, pmemory->type);
			CMsdkRender::GetInstance()->FSetSourceFile(file_path);
		}
	}
	else 
	{
		RETAILMSG(1, (L"is animating................\n"));
	}
}

void CMainWnd::OnCheckNavigation()
{
	// 如果是蓝牙来电或倒车界面,不能更新导航是否前台的状态,因为这二个事件会主动控制导航是否前台显示
	if (CDlgManager::GetInstance()->IsDlgShow(CBTCallDlg_ID, SOURCE_BT_CALL)
		|| CDlgManager::GetInstance()->IsDlgShow(CCameraDlg_ID, SOURCE_CAMERA) 
		|| CDlgManager::GetInstance()->IsDlgShow(CEcarDlg_ID, SOURCE_ECAR) 
		|| sysutil::nss_get_instance()->sys_rds_status)
	{
		return;
	}

	if (sysutil::nss_get_instance()->gps_process_handle != NULL)	// 导航在运行
	{
		if (WaitForSingleObject(sysutil::nss_get_instance()->gps_process_handle, 1) == WAIT_OBJECT_0)	// 导航已退出
		{
			::CloseHandle(sysutil::nss_get_instance()->gps_process_handle);
			sysutil::nss_get_instance()->gps_process_handle = NULL;
			sysutil::nss_get_instance()->gps_process_id = 0;
			OnGpsForeground(FALSE);

			// 第三程序,导航,app相互切换时,确保正确的窗口在显示
			DLG_INFO* pdi = CDlgManager::GetInstance()->GetCurDlgInfo();
			if (pdi)
			{
				if (CDlgManager::GetInstance()->IsExternalApp(pdi))
				{
					ExternalApp_Show_Fun show_fun = (ExternalApp_Show_Fun)pdi->pdlg;
					show_fun(TRUE);
				}
				else
				{
					ui_set_foreground_window(m_hWnd);
				}
			}
		}
		else
		{
			DWORD dwForegroundProcessID = 0;
			GetWindowThreadProcessId( GetForegroundWindow(), &dwForegroundProcessID );
			
			if (m_bIsTopMostWindow)	// 如果窗口是TOPMOST,在后台加载导航后,GetForegroundWindow()返回的是导航的,需要复合条件来判断
			{
				GetWindowThreadProcessId( m_hWnd, &dwForegroundProcessID );
			}
			if (sysutil::nss_get_instance()->gps_process_id == dwForegroundProcessID 
				&& !sysutil::nss_get_instance()->sys_gps_foreground)
			{
				OnGpsForeground(TRUE);
			}
			else if (sysutil::nss_get_instance()->gps_process_id != dwForegroundProcessID 
				&& sysutil::nss_get_instance()->sys_gps_foreground)
			{
				OnGpsForeground(FALSE);
			}

			if (sysutil::nss_get_instance()->gps_process_id == dwForegroundProcessID)
			{
				if (sysutil::nss_get_instance()->gps_running_status == 1	// gps需前台启动运行
					&& !::IsWindowVisible(m_hWnd))
				{
					::ShowWindow(m_hWnd, SW_SHOWNA);
				}
			}

		}
	}
}

void CMainWnd::OnTimerFastCamera()
{
	if (YC_GetBootBackCarState2() != 1 )	// 检测到开机倒车结束了
	{
		KillTimer(TIMER_ID_CHECK_FASTCAMERA);
		::PostMessage(HWND_BROADCAST, NAVI_MSG_FASTCAMERA_EXIT, 0, 1);
	}
}

void CMainWnd::OniPodConnect(BOOL bConnect)
{
	RestartScreenSaver();
	if (sysutil::nss_get_instance()->sys_status == UISS_BACKLIGHT_OFF)  // 如果关屏了,打开屏幕
	{
		PostMessage(UI_MSG_TP_CLICK);
	}


	if (bConnect && !IsLoadingApp())	// 开机8秒内不响应
	{
		LoadUiFrontSource(SOURCE_IPOD_USB);
	}
	else
	{
		if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_IPOD_USB
			|| CSourceManager::GetInstance()->GetRearSrcID() == SOURCE_IPOD_USB)
		{
			CMainWnd::GetInstance()->PostMessage(UI_MSG_SOURCE_EXIT, 0, (LPARAM)SOURCE_IPOD_USB);
		}

	}

}

BOOL CMainWnd::IsLoadingApp()
{
	return (GetTickCount()-m_dwAppStartTick < TIME_IGNORE_SD_USB);
}

void CMainWnd::OnActiveateWnd(BOOL bActive)
{
	RestartScreenSaver();
	if (!bActive)
	{
		ExitScreenSaver();
	}
}

void _SetMaxVolumeGain(UINT max_gain)
{
	GHANDLE hAudioDev = GOpenAudioDev();
	GClientSetVolume(hAudioDev, max_gain);
	GClientSetRearVolume(hAudioDev, max_gain);
	GCloseAudioDev(hAudioDev);
}


LRESULT CMainWnd::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_ACTIVATE)
	{
		OnActiveateWnd(LOWORD(wParam));
	}
	else if (message == UI_MSG_DVD_EJECT_NOTIFY && wParam == 1)
	{
		// 任何操作起到解mute作用
		if (sysutil::nss_get_instance()->is_audio_mute())
		{
			PostMessage(UI_MSG_MCU_KEY_COMMAND, K_MUTE, 0);
		}

		// 如果假关机了,进碟可唤醒
		if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP && lParam == 1)
		{
			OnSleepPower(FALSE);
		}

		if (lParam == 1)	// 进碟
		{
			LoadUiFrontSource(SOURCE_DVD);
			PostMessage(UI_MSG_DVD_ANIM_LOADING);

			RestartScreenSaver();
			if (sysutil::nss_get_instance()->sys_status == UISS_BACKLIGHT_OFF)  // 如果关屏了,打开屏幕
			{
				PostMessage(UI_MSG_TP_CLICK);
			}
		}
	}
	else if (message == UI_MSG_USB_IPOD_CONNECT)
	{
		OniPodConnect(lParam);
	}
	else if (message == UI_MSG_SOURCE_EXIT)
	{
		OnSourceExit((SOURCE_ID)lParam);
	}
	else if (message == NAVI_MSG_LAN_CHANGE)
	{
		OnLanguageChange((int)lParam);
	}
	else if (message == NAVI_MSG_BK_CHANGE)
	{
		OnBKChange((int)lParam);
	}
	else if (message == NAVI_MSG_ACC_OFF)
	{
		OnAccOff();
	}
	else if (message == NAVI_MSG_ACC_ON)
	{
		OnAccON();
	}
	else if (message == UI_MSG_GPS_STATUS_UPDATE)
	{
		OnGPSUpdate();
	}
	else if (message == UI_MSG_GPS_SETTIME)
	{
		GPSSetTime();
	}
	else if (message == NAVI_MSG_GPS_FIXED)
	{
		if (lParam)		// 定到位
		{
			if (sysutil::nss_get_instance()->ui_time_autoset && sysutil::nss_get_instance()->ui_time_need_autoset)
			{
				GPSSetTime();
			}
		}
	}
	else if (message == NAVI_MSG_FASTCAMERA_EXIT)
	{
		OnFastCameraExit();
		InitMCUComm();		// 释放串口后再初始化与MCU的通信

		InitMCUParams();	// 获取一些初始化信息
		// 如果是掉B+,收音恢复默认区域
		if (!sysutil::nss_get_instance()->sys_acc_off)
		{
			protocol::get_mcu_sys_para()->set_fm_cur_region(config::get_config_misc()->fm_area);
			CRpcMCU::GetInstance()->RPC_SetSysParams(MCU_SYS_PARA::INDEX_FM, 
				protocol::get_mcu_sys_para()->fm_data,	sizeof(protocol::get_mcu_sys_para()->fm_data));
			Sleep(800);
		}
		// 与MCU的通信建立起来后，才自动加载源，防止FM起来了，MCU还没完全通信上
		SetTimer(TIMER_ID_LOAD_LAST_SOURCE, 10, NULL);

		// 与MCU通信上后才写这个标志,关机状态倒车,MCU会给arm上电,这时APP有可能会被运行,但倒车完毕后,MCU直接断电了
		// 这种情况与掉B+是有差别的
		sysutil::nss_save(FALSE);	// 将nss状态写为掉B+，这样只有正常ACC OFF关机时才会把该状态改为TRUE

	}
	else if (message == UI_MSG_MCU_KEY_COMMAND)
	{
		OnMcuKeyCommand(wParam, lParam);
	}
	else if (message == UI_MSG_MSDK_MEDIAGRAPH_FILE)
	{
		CMsdkRender::GetInstance()->HandleEventF();
	}
	else if (message == UI_MSG_MSDK_MEDIAGRAPH_WMA)
	{
		CMsdkRender::GetInstance()->HandleEventWma();
	}
	else if (message == UI_MSG_MSDK_MEDIAGRAPH_AVIN)
	{
		CMsdkRender::GetInstance()->HandleEventAvin((HMEDIAGRAPH)wParam);
	}
	else if (message == UI_MSG_DVP_AGENT)
	{
		CDvdRender::GetInstance()->HandleEvent();
	}
	else if (message == UI_MSG_BT_CALL_EVENT)
	{
		OnBTCall((DWORD)lParam);
	}
	else if (message == UI_MSG_MCU_STATUS_CHANGE)
	{
		OnMCUStatusChange((MCU_STATUS*)lParam);
	}
	else if (message == UI_MSG_MCU_RADIO_DATA && lParam == 0)
	{
		OnRadioStatus();
	}
	else if (message == WAVMSG_GPS_STREAMING_NOTIFY)
	{
		OnNavStreamNotify(wParam);
	}
	else if (message == UI_MSG_LOAD_SOURCE_NOTIFY)
	{
		OnLoadSourceNotify((SOURCE_ID)lParam, (DVP_SINK)wParam);
	}
	else if (message == UI_MSG_STOP_SOURCE_NOTIFY)
	{
		OnStopSourceNotify((SOURCE_ID)lParam);
	}
	else if (message == UI_MSG_TP_CLICK)
	{
		OnTPClick(wParam, (MSLLHOOKSTRUCT*)lParam);
	}
	else if (message == WM_USER_MTKPNPMS_RESERVED)
	{
		// 不用干扰消息
		//OnMassStoragePnP(wParam, lParam);
	}
	else if (message == UI_MSG_STORAGE_CARD_PLUG)
	{
		// 任何操作起到解mute作用
		if (sysutil::nss_get_instance()->is_audio_mute())
		{
			PostMessage(UI_MSG_MCU_KEY_COMMAND, K_MUTE, 0);
		}

		RestartScreenSaver();
		OnMassStoragePlug((UI_MASS_STORAGE_TYPE)wParam, (UI_MASS_STORAGE_PLUG)lParam);
	}
	else if (message == NAVI_MSG_APP_EXIT || message == NAVI_MSG_APP_UPDATE_START)
	{
		CSourceManager::GetInstance()->SetFrontSrc(INVALID_SOURCE_ID);
		CSourceManager::GetInstance()->SetRearSrc(INVALID_SOURCE_ID);
		CDlgManager::GetInstance()->DeleteAllDlg();
		CPhoneLink::GetInstance()->PostMessage2Phonelink(ID_UWM_PHONELINK_QUIT, 0, 0);	// 退出phonelink

		EndDialog(0);
		CString font;
		font.Format(L"%s\\msyh.ttf", tzutil::GetAppPath());
		::RemoveFontResource(font);
		if (message == NAVI_MSG_APP_EXIT)	// 主动退出时加explorer加载起来
		{
			tzutil::LoadApp(L"windows\\explorer.exe", L"", NULL);
		}
	}
	else if (message == UI_MSG_DLG_CHANGE)
	{
		OnDlgChange();
	}
	else if (message == UI_MSG_MCU_CANBUS_DATA)
	{
		OnCanBusData((CANBUS_DATA_TYPE)lParam);
	}
	// PhoneLink消息处理
	else if (CPhoneLink::GetInstance()->HandleMessage(message, wParam, lParam))
	{
		return 0L;
	}
	else if (CEasyConnected::GetInstance()->HandleMessage(message, wParam, lParam))
	{
		return 0L;
	}
	else if (message == UI_MSG_MCU_DIGITAL_KEY)
	{
		SOURCE_ID sid = CSourceManager::GetInstance()->GetFrontSrcID();
		if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_DVD)	//其它源不响应GOTO
		{
			// 当前窗口为蓝牙时,数字键用来拨号
			if (CDlgManager::GetInstance()->GetCurDlgID() != CBluetoothDlg_ID
				&& CDlgManager::GetInstance()->GetCurDlgID() != CBTCallDlg_ID)
			{
				CString str;
				str.Format(L"GOTO: %d", lParam);
				COverlayDlg::GetInstance()->MessageTip_ShowUI(str, FALSE);
			}
		}
	}
	else if (message == UI_MSG_BT_CONNECT_STATUS_CHANGE)
	{
		if (lParam == BZ_IND_CREATE_CONNECT_SUCCESS)
		{
			SetTimer(TIMER_ID_BT_CONNECTED, 800, NULL);
		}
	}
	else if (message == WM_USER+0x199)
	{
		_SetMaxVolumeGain(lParam);
	}
	



	return CWceUiRealWnd::OnMessage(message, wParam, lParam);
}

void CMainWnd::OnRadioStatus()
{
	static BOOL bByPhone = FALSE;	// BT通话声音输出到手机还是车机

	static DLG_INFO* lastdlg = NULL;
	static DLG_INFO* active_dlg = NULL;	// FM源当前的活动窗口
	// rds alarm
	BOOL bIsCurRDSAlarm = protocol::get_mcu_radio_status()->is_TA_Alarm()
		|| protocol::get_mcu_radio_status()->is_EWS();
	if (!tzutil::IsBoolEqual(sysutil::nss_get_instance()->sys_rds_status, bIsCurRDSAlarm))
	{
		// ******  处理BT通话  ************** //
		// 如果需要，把通话声音切换到手机
		bByPhone = FALSE;
		if (sysutil::nss_get_instance()->sys_bt_call_status == CALLMANAGER_CALLSTATE_SPEAKING)
		{
			bByPhone = CBluetooth::GetInstance()->GetAudioOutput();
			if (!bByPhone)	CBluetooth::GetInstance()->SetAudioOutput(TRUE);
		}
		if (m_bIncomingRing)	// 如果在来电响铃,停止响铃
		{
			sndPlaySound(0, 0);
			m_bIncomingRing = FALSE;
		}


		sysutil::nss_get_instance()->sys_rds_status = bIsCurRDSAlarm;
		// rds状态改变时要刷新音量
		//CVolumeManager::GetInstance()->UpdateVolume();
		if (bIsCurRDSAlarm)		// rds alarm
		{
			// 如果在校屏,退出校屏
			if (sysutil::nss_get_instance()->sys_calibrate_status == 1)	ui_exit_touch_calibrate();
			// 如果关屏了,打开屏幕
			if (sysutil::nss_get_instance()->sys_status == UISS_BACKLIGHT_OFF) PostMessage(UI_MSG_TP_CLICK);

			// 如果假关机,唤醒
			if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP)
			{
				OnSleepAwake();
			}

			// 
			RestartScreenSaver();

			//RDS报警，设置MCU unmute标志
			CVolumeManager::GetInstance()->SetMcuUnmuteFlag(UNMUTE_MCU_MASK_RDS, TRUE);


			// 正确显示UI，并记录当前的状态
			if (sysutil::nss_get_instance()->sys_camera_status == 1
				|| ui_is_on_bt_call()
				|| sysutil::nss_get_instance()->sys_ecar_status == 1)
			{
				lastdlg = CDlgManager::GetInstance()->GetLastDlgInfo();
			}
			else
			{
				lastdlg = CDlgManager::GetInstance()->GetCurDlgInfo();
			}
			active_dlg = CDlgManager::GetInstance()->GetActiveDlgInfo(SOURCE_FM);
			CRadioRDSDlg::SetTopMostDlg(TRUE);
			ShowRDSDlg();

			// 暂停播放, 
			// 关闭其它源的声音通道,
			// 如果当前源不是FM,把FM通道打开
			CSourceManager::GetInstance()->HangupPlayback(PLAYBACK_MASK_RDS);
			CSourceManager::GetInstance()->CloseAudio();
			if (CSourceManager::GetInstance()->GetFrontSrcID() != SOURCE_FM)
			{
				CMsdkRender::GetInstance()->ALaunchFMSrc();
				//CMsdkRender::GetInstance()->OpenRDS();
			}
		}
		else		// rds报警结束
		{
			// 如果不是FM源,关闭FM通道
			//  恢复声音通道
			// 恢复播放
			if (CSourceManager::GetInstance()->GetFrontSrcID() != SOURCE_FM)
			{
				CMsdkRender::GetInstance()->AStopFMSrc();
				//CMsdkRender::GetInstance()->CloseRDS();
			}
			CSourceManager::GetInstance()->OpenAudio();
			CSourceManager::GetInstance()->RestorePlayback(PLAYBACK_MASK_RDS);

			//RDS报警结束，取消MCU unmute标志
			CVolumeManager::GetInstance()->SetMcuUnmuteFlag(UNMUTE_MCU_MASK_RDS, FALSE);


			if (sysutil::nss_get_instance()->sys_camera_status == 1)
			{
				// 如果还在倒车,倒车结束后会进入正确的状态,这里不用任何操作
			}
			else if (ui_is_on_bt_call())
			{
				ShowBTCallDlg();
			}
			else if (sysutil::nss_get_instance()->sys_ecar_status == 1)	// 翼卡激活了
			{
				CDlgManager::GetInstance()->ShowDlg(CEcarDlg_ID, SOURCE_ECAR);
			}
			else
			{
				// 如果导航在前台运行,报警结束后显示导航
				if (sysutil::nss_get_instance()->sys_gps_foreground)
				{
					tzutil::SetProcessForeground(sysutil::nss_get_instance()->gps_process_id);
				}

				if (lastdlg && lastdlg->dlg_id == CRadioRDSDlg_ID)
				{
					;	// 如果之前已经在RDS界面，不用切换窗口了
				}
				else
				{
					CDlgManager::GetInstance()->GotoLastDlg();
				}
			}

			CRadioRDSDlg::SetTopMostDlg(FALSE);
			// 不要因为弹出的报警，影响了FM源的活动窗口
			if (active_dlg && active_dlg->dlg_id == CRadioDlg_ID)
			{
				CDlgManager::GetInstance()->SetActiveDlg(CRadioDlg_ID, SOURCE_FM);
			}

			// ******  处理BT通话  ************** //
			// 如果需要，把通话声音切换到车机
			if (sysutil::nss_get_instance()->sys_bt_call_status == CALLMANAGER_CALLSTATE_SPEAKING)
			{
				if (!bByPhone)	CBluetooth::GetInstance()->SetAudioOutput(FALSE);
			}
			else if (sysutil::nss_get_instance()->sys_bt_call_status == CALLMANAGER_CALLSTATE_INCOMING 
				&& !CBluetooth::GetInstance()->IsSupportInBandRing())	// 如果手机不支持来电响铃, 自己播放铃声
			{
				TCHAR szPath[MAX_PATH];
				_stprintf(szPath, L"%s\\bt\\ring.wav", tzutil::GetAppPath());
				sndPlaySound(szPath, SND_ASYNC|SND_LOOP|SND_NODEFAULT);
				m_bIncomingRing = TRUE;
			}

			// *************  处理需延时切换的源 ***************** //
			if (!sysutil::nss_get_instance()->sys_bt_call_status	// 如果BT在通话,BT结束后会处理
				&& m_sidToLoad != INVALID_SOURCE_ID)
			{
				LoadSource(m_sidToLoad, m_sinkToLoad);
			}


			// 如果是在唤醒状态,而且没有BT通话,重新进入休眠状态
			if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP_AWAKE
				&& !(sysutil::nss_get_instance()->sys_bt_call_status
				|| sysutil::nss_get_instance()->sys_ecar_status)
				&& !sysutil::nss_get_instance()->sys_camera_status)
			{
				OnSleep();
			}
		}

		// rds状态改变时要刷新音量
		CVolumeManager::GetInstance()->UpdateVolume();

	}
}

void CMainWnd::OnCanBusData(CANBUS_DATA_TYPE cbdt)
{
	if (cbdt == CBDT_CLIMATRONIC)
	{
		BOOL bShowUI = FALSE;
		switch (sysutil::nss_get_instance()->sys_canbus_type)
		{
		case CANBUS_TYPE_RAISE_DASAUTO:
			bShowUI = CANBUS_RAISE_DASAUTO::get_instance()->is_climatronic_REQ_show();
			break;
		case CANBUS_TYPE_RAISE_FOCUS:
			bShowUI = CANBUS_RAISE_FOCUS::get_instance()->is_climatronic_REQ_show();
			break;
		case CANBUS_TYPE_RAISE_HONDA:
			bShowUI = CANBUS_RAISE_HONDA::get_instance()->is_climatronic_REQ_show();
			break;
		case CANBUS_TYPE_RAISE_PEUGEOT:
			bShowUI = CANBUS_RAISE_PEUGEOT::get_instance()->is_climatronic_REQ_show();
			break;
		default:
			break;
		}
		if (bShowUI)
		{
			COverlayDlg::GetInstance()->Climatronic_ShowUI();
		}
	}
	else if (cbdt == CBDT_SWC_KEY)
	{
		switch (CANBUS_RAISE_DASAUTO::get_instance()->get_swc_key_Value())
		{
		case 0x01:		// vol+
			OnMcuKeyCommand(K_VOL_UP, 1);
			break;
		case 0x02:		// vol-
			OnMcuKeyCommand(K_VOL_DN, 1);
			break;
		case 0x03:		// UP
			OnMcuKeyCommand(T_PREV, 0);
			break;
		case 0x04:		// down
			OnMcuKeyCommand(T_NEXT, 0);
			break;
		case 0x05:		// tel
			OnMcuKeyCommand(K_PHONE_ON, 0);
			break;
		case 0x06:		// mute
			OnMcuKeyCommand(K_MUTE, 0);
			break;
		case 0x07:		// SRC
			OnMcuKeyCommand(K_SOURCE_MODE, 0);
			break;
		}
	}
}

LPCTSTR _get_mstype_path(UI_MASS_STORAGE_TYPE msType)
{
	LPCTSTR path = L"";
	if (msType == UIMS_SDMMC)
	{
		path = L"SDMMC";
	}
	else if (msType == UIMS_STORAGE_CARD)
	{
		path = L"Storage Card";
	}
	else if (msType == UIMS_USB)
	{
		path = L"USBDisk";
	}
	else if (msType == UIMS_USB1)
	{
		path = L"USBDisk1";
	}

	return path;
}

// 因为源的盘符是可配置的,SD源不一定对应SDMMC,所以这里根据配置文件重新映射下
static SOURCE_ID _get_sid_by_mstype(UI_MASS_STORAGE_TYPE msType)
{
	LPCTSTR path = _get_mstype_path(msType);

	// SOURCE_SD
	if (_tcsicmp(path, ui_msdk_get_disk(SOURCE_SD)) == 0)
	{
		return SOURCE_SD;
	}
	else if (_tcsicmp(path, ui_msdk_get_disk(SOURCE_USB)) == 0)
	{
		return SOURCE_USB;
	}
	else if (_tcsicmp(path, ui_msdk_get_disk(SOURCE_USB_REAR)) == 0)
	{
		return SOURCE_USB_REAR;
	}
	else 
	{
		return INVALID_SOURCE_ID;
	}
}

// GPS选择路径配置文件中可配置多个盘符,这里需判断当前的GPS路径是否位于指定的TYPE类型中
static BOOL _is_gps_path_by_mstype(UI_MASS_STORAGE_TYPE msType)
{
	LPCTSTR path = _get_mstype_path(msType);
	CStringArray afile;
	tzutil::GetSubString(sysutil::nss_get_instance()->gps_path, afile, L'\\');
	if (afile.GetCount() > 0 && _tcsicmp(afile.GetAt(0), path) == 0)
	{
		return TRUE;
	}

	return FALSE;	
}

// 因为vdisk的位置是可配置,如果vdisk的路径在msType指定的盘符上,返回TRUE
static BOOL _is_vdisk_sid_by_mstype(UI_MASS_STORAGE_TYPE msType)
{
	LPCTSTR path = _get_mstype_path(msType);
	CStringArray afile;
	tzutil::GetSubString(sysutil::nss_get_instance()->vdisk_path, afile, L'\\');
	if (afile.GetCount() > 0 && _tcsicmp(afile.GetAt(0), path) == 0)
	{
		return TRUE;
	}

	return FALSE;	
}

void CMainWnd::OnMassStoragePlug(UI_MASS_STORAGE_TYPE msType, UI_MASS_STORAGE_PLUG msAction)
{
	// 如果假关机了,不响应
	if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP)
	{
		return;
	}

	RestartScreenSaver();
	if (sysutil::nss_get_instance()->sys_status == UISS_BACKLIGHT_OFF)  // 如果关屏了,打开屏幕
	{
		PostMessage(UI_MSG_TP_CLICK);
	}


	SOURCE_ID sid = _get_sid_by_mstype(msType);
	if (sid != INVALID_SOURCE_ID)
	{
		if (msAction == UIMS_PLUG_IN 
			&& !IsLoadingApp())	// 开机8秒内不响应插入的卡或U盘
		{
			LoadUiFrontSource(sid);
		}
		else if (msAction == UIMS_PLUG_OUT)
		{
			if (CSourceManager::GetInstance()->GetFrontSrcID() == sid
				|| CSourceManager::GetInstance()->GetRearSrcID() == sid)
			{
				CMainWnd::GetInstance()->PostMessage(UI_MSG_SOURCE_EXIT, 0, (LPARAM)sid);
			}
			else if (_is_vdisk_sid_by_mstype(msType))	// vdisk虚拟碟所在的盘符
			{
				if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_VDISK
					|| CSourceManager::GetInstance()->GetRearSrcID() == SOURCE_VDISK)
				{
					CMainWnd::GetInstance()->PostMessage(UI_MSG_SOURCE_EXIT, 0, (LPARAM)SOURCE_VDISK);
				}
			
			}

		}
	}
}


static UI_MASS_STORAGE_TYPE _get_storage_type(WPARAM wParam)
{
	if (IS_USBPORT0_PRESENT(wParam))
	{
		return UIMS_USB;
	}
	else if (IS_USBPORT1_PRESENT(wParam))
	{
		return UIMS_USB1;
	}
	else if (IS_SDCSLOT0_PRESENT(wParam))
	{
		return UIMS_SDMMC;
	}
	else if (IS_SDCSLOT1_PRESENT(wParam))
	{
		return UIMS_STORAGE_CARD;
	}
	else if (IS_SDCSLOT2_PRESENT(wParam))
	{
		return UIMS_SD3;
	}
	else
	{
		return UIMS_INVALID;
	}
}

void CMainWnd::OnMassStoragePnP(WPARAM wParam, LPARAM lParam)
{
	BOOL	bOnUsbExit = FALSE;

	UI_MASS_STORAGE_TYPE msType = _get_storage_type(wParam);
	// 只对USB执行这种机制
	if (msType != UIMS_USB && msType != UIMS_USB1)
	{
//		return;
	}

	switch(GET_MSG_CAUSE(wParam))
	{
	case FS_DAEMON_UNKNOW:
		RETAILMSG(TRUE, (TEXT("[navi_car] FS_DAEMON_UNKNOW. \n")));
		break;
	case FS_DAEMON_MOUNT_SINCEBABBLE:		// 受干扰恢复
		RETAILMSG(TRUE, (L"[navi_car] FS_DAEMON_MOUNT_SINCEBABBLE \n"));
		break;
	case FS_DAEMON_DISMOUNT_SINCEBABBLE:	// 受干扰卸载,暂时也认为拔掉了,USB1.1时如果干扰太严重要专门处理下
		RETAILMSG(TRUE, (L"[navi_car] FS_DAEMON_DISMOUNT_SINCEBABBLE \n"));
		PostMessage(UI_MSG_STORAGE_CARD_PLUG, msType, UIMS_PLUG_OUT);
		break;
	case FS_DAEMON_DISMOUNT:
		RETAILMSG(TRUE, (L"[navi_car] FS_DAEMON_DISMOUNT \n"));
		PostMessage(UI_MSG_STORAGE_CARD_PLUG, msType, UIMS_PLUG_OUT);

		break;
	case FS_DAEMON_MOUNT:
		RETAILMSG(TRUE, (L"[navi_car]  FS_DAEMON_MOUNT \n"));
		PostMessage(UI_MSG_STORAGE_CARD_PLUG, msType, UIMS_PLUG_IN);
		break;
	case FS_DAEMON_MOUNTED:
		RETAILMSG(TRUE, (L"[navi_car]  FS_DAEMON_MOUNTED \n"));
		break;
	case FS_DAEMON_INVALID:
		RETAILMSG(TRUE, (L"[navi_car]  FS_DAEMON_INVALID \n"));
		break;
	default:
		RETAILMSG(TRUE, (L"[navi_car]  FS DAEMON status = %d \n", GET_MSG_CAUSE(wParam)));
		break;
	}

}

void CMainWnd::OnTPClick(WPARAM wParam , MSLLHOOKSTRUCT* phs)
{
	if (wParam != 0)	// 表示是点屏触发的,不是倒车或蓝牙其它动作触发的
	{
		ui_mcu_beep(MCU_BEEP_NORMAL);
	}
	sysutil::nss_get_instance()->sys_status = UISS_NORMAL;
	winhook::EnableTouchPanel();
	YC_BackLightEnable(TRUE);	// 打开背光
}

void CMainWnd::OnDVDLoading(BOOL bLoading)
{
	if (bLoading)
	{
		m_bDVDEjectEnable = FALSE;
	}
	else
	{
		SetTimer(TIMER_ID_DVD_EJECT_ENABLE, DVD_EJECT_DELAY_TIME, NULL);
	}
}

void CMainWnd::OnLoadSourceNotify(SOURCE_ID sid, DVP_SINK sink)
{
	if (sid == SOURCE_DVD)
	{
		OnDVDLoading(TRUE);
	}

	// 只要切源，清除该标志
	if (sid != INVALID_SOURCE_ID)
	{
		ui_led_set_playmode(FALSE, FALSE);
	}
}

void CMainWnd::OnStopSourceNotify(SOURCE_ID sid)
{
	if (sid == SOURCE_DVD)
	{
		OnDVDLoading(FALSE);
	}

	// FM有优化设计,不删除
	// 其它不需删除的也可在这里添加,不删除显示速度会,但会占内存,如果不在意内存,可以不删除
	if (/*sid == SOURCE_FM ||*/ sid == SOURCE_BT)
	{
		return;
	}

	CDlgManager::GetInstance()->DeleteSrcDlg(sid);
}


void CMainWnd::OnNavStreamNotify(BOOL bStart)
{
	if (bStart)	// 发音开始
	{
		KillTimer(TIMER_ID_NAVI_STREAM_DELAY);
		sysutil::nss_get_instance()->sys_gps_guiding = bStart;
		CVolumeManager::GetInstance()->UpdateVolume();
		// 导航要发音，设置MCU unmute标志
		CVolumeManager::GetInstance()->SetMcuUnmuteFlag(UNMUTE_MCU_MASK_NAVI, TRUE);
	}
	else	// 发音结束，延时一定时间才认为真正发音结束
	{
		SetTimer(TIMER_ID_NAVI_STREAM_DELAY, TIME_NAVI_STREAM_DELAY, NULL);
		// 导航发音结束，取消MCU unmute标志
		CVolumeManager::GetInstance()->SetMcuUnmuteFlag(UNMUTE_MCU_MASK_NAVI, FALSE);
	}
}

void CMainWnd::OnBTCall(DWORD dwEventID)
{
	RestartScreenSaver();
	sysutil::nss_get_instance()->sys_bt_call_status = CBluetooth::GetInstance()->GetCallState();
	CVolumeManager::GetInstance()->UpdateVolume();

	if (dwEventID == BZ_IND_CALL_INCOMING || dwEventID == BZ_IND_CALL_OUTGOING || dwEventID == BZ_IND_CALL_SPEAKING)
	{
		SetTopmost();

		// 如果在校屏,退出校屏
		if (sysutil::nss_get_instance()->sys_calibrate_status == 1)
		{
			ui_exit_touch_calibrate();
		}

		// 如果关屏了,打开屏幕
		if (sysutil::nss_get_instance()->sys_status == UISS_BACKLIGHT_OFF)
		{
			SetTimer(TIMER_ID_TP_CLICK, SLEEP_TP_CLICK_DELAY, NULL);
			//PostMessage(UI_MSG_TP_CLICK);
		}

		// 如果假关机,唤醒
		if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP)
		{
			OnSleepAwake();
		}

		// 如果BT来时刚好在动画,等动画完成后再弹出BT窗口
		if (CDlgManager::GetInstance()->IsAnimating())
		{
			SetTimer(TIMER_ID_DELAY_SHOW_BTCALL, 10, NULL);
		}
		else
		{
			ShowBTCallDlg();
		}

		BZ_VerifySCOAudioChannel(TRUE);

		// 如果手机不支持来电响铃, 自己播放铃声
		if (dwEventID == BZ_IND_CALL_INCOMING 
			&& !CBluetooth::GetInstance()->IsSupportInBandRing())
		{
			TCHAR szPath[MAX_PATH];
			_stprintf(szPath, L"%s\\bt\\ring.wav", tzutil::GetAppPath());
			sndPlaySound(szPath, SND_ASYNC|SND_LOOP|SND_NODEFAULT);
			m_bIncomingRing = TRUE;
		}

		if (dwEventID == BZ_IND_CALL_SPEAKING)
		{
			if (m_bIncomingRing)	// 停止响铃
			{
				sndPlaySound(0, 0);
				m_bIncomingRing = FALSE;
			}

			if (sysutil::nss_get_instance()->sys_rds_status)	// 接通时,有RDS报警,直接转到手机
			{
				CBluetooth::GetInstance()->SetAudioOutput(TRUE);
			}
		}

		// 来电暂停播放
		CSourceManager::GetInstance()->HangupPlayback(PLAYBACK_MASK_BT);
		CVolumeManager::GetInstance()->SetMcuUnmuteFlag(UNMUTE_MCU_MASK_BT, TRUE);
	}
// 	else if (dwEventID == BZ_IND_CALL_SPEAKING)
// 	{
// 		BZ_VerifySCOAudioChannel(TRUE);	// SIRI手机激活时,直接触发了BZ_IND_CALL_SPEAKING
// 		if (m_bIncomingRing)	// 停止响铃
// 		{
// 			sndPlaySound(0, 0);
// 			m_bIncomingRing = FALSE;
// 		}
// 	}
	else if (dwEventID == BZ_IND_CALL_TERMINATED)
	{
		RemoveTopmost();

		BZ_VerifySCOAudioChannel(FALSE);

		if (m_bIncomingRing)	// 停止响铃
		{
			sndPlaySound(0, 0);
			m_bIncomingRing = FALSE;
		}

		// 来电结束恢复播放
		CSourceManager::GetInstance()->RestorePlayback(PLAYBACK_MASK_BT);
		// 来电结束时，取消MCU Unmute标志
		CVolumeManager::GetInstance()->SetMcuUnmuteFlag(UNMUTE_MCU_MASK_BT, FALSE);


		// 如果是在唤醒状态,而且没有倒车,重新进入休眠状态
		if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP_AWAKE
			&& !sysutil::nss_get_instance()->sys_camera_status
			&& !sysutil::nss_get_instance()->sys_rds_status)
		{
			OnSleep();
		}

		// *************  处理需延时切换的源 ***************** //
		if (!sysutil::nss_get_instance()->sys_rds_status	// 如果在RDS,RDS结束后会处理
			&& m_sidToLoad != INVALID_SOURCE_ID)
		{
			LoadSource(m_sidToLoad, m_sinkToLoad);
		}


	}

}

void CMainWnd::OnMCUStatusChange(MCU_STATUS* pstatus)
{
	static MCU_STATUS status;	// 初始值均为0
	ASSERT(pstatus);
	if (pstatus == NULL)
	{
		return;
	}
	// 倒车
	if (pstatus->is_camera_on() && !status.is_camera_on())	// 启动倒车
	{
		// 倒车测试时有可能状态改变很快,传进来的参数为全局变量,MCU handler线程有可能改变这个参数的值,
		// 这里尽快赋值，逻辑上这地方仍可能有BUG
		status = *pstatus;	

		// 如果在校屏,退出校屏
		if (sysutil::nss_get_instance()->sys_calibrate_status == 1)
		{
			ui_exit_touch_calibrate();
		}

		// 如果关屏了,打开屏幕
		if (sysutil::nss_get_instance()->sys_status == UISS_BACKLIGHT_OFF)
		{
			SetTimer(TIMER_ID_TP_CLICK, SLEEP_TP_CLICK_DELAY, NULL);
			//PostMessage(UI_MSG_TP_CLICK);
		}

		// 如果假关机,唤醒
		if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP)
		{
			OnSleepAwake();
		}

		RestartScreenSaver();


		sysutil::nss_get_instance()->sys_camera_status = 1;
		CVolumeManager::GetInstance()->UpdateVolume();
		
		if (CDlgManager::GetInstance()->IsAnimating())
		{
			SetTimer(TIMER_ID_DELAY_SHOW_CAMERA, 100, NULL);
		} 
		else
		{
			CDlgManager::GetInstance()->ShowDlg(CCameraDlg_ID, SOURCE_CAMERA);
		}
		// 如果当前源是AVIN,则需对AVIN进行处理
		SOURCE_INFO *psi = CSourceManager::GetInstance()->GetFrontSrc();
		if (psi)
		{
			CMsdkRender::GetInstance()->AOnCamera(TRUE, psi->sid, psi->sink);
		}

		CMsdkRender::GetInstance()->AOpenCameraChannel();
		// 倒车镜像一般是指上下镜像
		MSDK_AVIN_MIRROR_E mirror = sysutil::nss_get_instance()->video_rear_camera_mirror ? MSDK_AVIN_FLIP_V : MSDK_AVIN_FLIP_NONE;
		CMsdkRender::GetInstance()->ASetCameraMirror(mirror);

		// 倒车暂停播放
		CSourceManager::GetInstance()->HangupPlayback(PLAYBACK_MASK_CAMERA);
	}
	else if (!pstatus->is_camera_on() && status.is_camera_on())	// 结束倒车
	{
		status = *pstatus;
		sysutil::nss_get_instance()->sys_camera_status = 0;

		CMsdkRender::GetInstance()->ACloseCameraChannel();	// 先关掉通道
		// 如果当前源是AVIN,则需对AVIN进行处理
		SOURCE_INFO *psi = CSourceManager::GetInstance()->GetFrontSrc();
		if (psi)
		{
			CMsdkRender::GetInstance()->AOnCamera(FALSE, psi->sid, psi->sink);
		}


		KillTimer(TIMER_ID_DELAY_SHOW_CAMERA);	// 如果窗口还没显示出来就不显示了
		if (CDlgManager::GetInstance()->GetCurDlgID() == CCameraDlg_ID)	// 如果窗口都没显示出来,不用切换dlg
		{
			if (sysutil::nss_get_instance()->sys_rds_status)
			{
				ShowRDSDlg();
			}
			else if (ui_is_on_bt_call())
			{
				ShowBTCallDlg();
			}
			else if (sysutil::nss_get_instance()->sys_ecar_status == 1)	// 翼卡激活了
			{
				CDlgManager::GetInstance()->ShowDlg(CEcarDlg_ID, SOURCE_ECAR);
			}
			else
			{
				// 如果导航在前台运行
				if (sysutil::nss_get_instance()->sys_gps_foreground)
				{
					tzutil::SetProcessForeground(sysutil::nss_get_instance()->gps_process_id);
					// 如果是在唤醒状态,马上会进入休眠状态,这里不延时,会导致地图画面闪一下
					if (sysutil::nss_get_instance()->sys_status != UISS_SLEEP_AWAKE)
					{
						Sleep(200);
					}
				}
				CDlgManager::GetInstance()->GotoLastDlg();
			}
		}
		CDlgManager::GetInstance()->DeleteDlg(CCameraDlg_ID, SOURCE_CAMERA);

		CVolumeManager::GetInstance()->UpdateVolume();

		// 倒车结束恢复播放
		CSourceManager::GetInstance()->RestorePlayback(PLAYBACK_MASK_CAMERA);

		// 如果是在唤醒状态,而且没有BT通话,重新进入休眠状态
		if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP_AWAKE
			&& !(sysutil::nss_get_instance()->sys_bt_call_status
			|| sysutil::nss_get_instance()->sys_ecar_status)
			&& !sysutil::nss_get_instance()->sys_rds_status)
		{
			OnSleep();
		}
	}

	// 大灯检测
	ui_update_backlight();
	RestartScreenSaver();
}


void CMainWnd::OnVolumeChange(int nStep)
{
	// 不需考虑越界问题,CVolumeManager会处理
	UI_VOLUME_TYPE eVolumeType = VOLUME_TYPE_MEDIA;
	int volume_old;
	// RDS优先级最高
	if (sysutil::nss_get_instance()->sys_rds_status)
	{
		volume_old = sysutil::nss_get_instance()->audio_ta_volume;
		sysutil::nss_get_instance()->audio_ta_volume += nStep;
		CVolumeManager::GetInstance()->ValidateVolumeLevel(&sysutil::nss_get_instance()->audio_ta_volume);
		nStep = sysutil::nss_get_instance()->audio_ta_volume - volume_old;	// 因为有可能越界,校正下step
		eVolumeType = VOLUME_TYPE_TA;
	}
	else if (sysutil::nss_get_instance()->sys_bt_call_status)
	{
		volume_old = sysutil::nss_get_instance()->audio_bt_volume;
		sysutil::nss_get_instance()->audio_bt_volume += nStep;
		CVolumeManager::GetInstance()->ValidateVolumeLevel(&sysutil::nss_get_instance()->audio_bt_volume);
		nStep = sysutil::nss_get_instance()->audio_bt_volume - volume_old;	// 因为有可能越界,校正下step
		eVolumeType = VOLUME_TYPE_BT;
	}
// 	else if (sysutil::nss_get_instance()->sys_gps_guiding
// 		&& !sysutil::nss_get_instance()->sys_rds_status)
// 	{
// 		volume_old = sysutil::nss_get_instance()->audio_gps_volume;
// 		sysutil::nss_get_instance()->audio_gps_volume += nStep;
// 		CVolumeManager::GetInstance()->ValidateVolumeLevel(&sysutil::nss_get_instance()->audio_gps_volume);
// 		nStep = sysutil::nss_get_instance()->audio_gps_volume - volume_old;	// 因为有可能越界,校正下step
// 		eVolumeType = VOLUME_TYPE_GPS;
// 	}
	else 
	{
		volume_old = sysutil::nss_get_instance()->audio_media_front_volume;
		// 前后排音量暂时不分开管理
		sysutil::nss_get_instance()->audio_media_front_volume += nStep;
		sysutil::nss_get_instance()->audio_media_rear_volume += nStep;
		CVolumeManager::GetInstance()->ValidateVolumeLevel(&sysutil::nss_get_instance()->audio_media_front_volume);
		CVolumeManager::GetInstance()->ValidateVolumeLevel(&sysutil::nss_get_instance()->audio_media_rear_volume);
		nStep = sysutil::nss_get_instance()->audio_media_front_volume - volume_old;	// 因为有可能越界,校正下step
	}
	// 只要引起音量变化,就把mute状态解掉
	if (sysutil::nss_get_instance()->is_audio_mute())
	{
		sysutil::nss_get_instance()->set_audio_mute(FALSE);
	}

	CVolumeManager::GetInstance()->UpdateVolume();
	CVolumeManager::GetInstance()->UpdateMcuMute();

	// 放在UpdateVolume后,其会校正数据
	CVolumeManager::GetInstance()->ShowUI(eVolumeType, config::get_config_audio()->ui_delay, nStep);
}

void CMainWnd::OnMuteChange()
{
	// MUTE管理前后排媒体音量,GPS音量,BT音量不受MUTE控制
	BOOL isMute = !sysutil::nss_get_instance()->is_audio_mute();

	// 设置mute状态
	sysutil::nss_get_instance()->set_audio_mute(isMute);
// 	sysutil::nss_get_instance()->audio_is_media_front_mute = isMute;
// 	sysutil::nss_get_instance()->audio_is_media_rear_mute = isMute;
// 	sysutil::nss_get_instance()->audio_is_gps_mute = isMute;

	CVolumeManager::GetInstance()->UpdateVolume();
	if (isMute)
	{
		CVolumeManager::GetInstance()->ShowUI(VOLUME_TYPE_NULL, /*(DWORD)-1*/3000, 0);
	}
	else
	{
		CVolumeManager::GetInstance()->ShowUI(VOLUME_TYPE_NULL, 3000, 0);
	}

	// 更新MCU的mute状态
	CVolumeManager::GetInstance()->UpdateMcuMute();
}

#include "VideoParamsDlg.h"
void CMainWnd::OnMcuKeyCommand(UINT key, UINT param)
{
	static DWORD dwTick = 0;
	if (GetTickCount() - dwTick < 100)
	{
		return;
	}
	dwTick = GetTickCount();

	int status = (key>>16) & 0xFF;
	int id = key & 0xFF;

	RestartScreenSaver();
	// 如果关屏了,打开屏幕
	if (sysutil::nss_get_instance()->sys_status == UISS_BACKLIGHT_OFF)
	{
		PostMessage(UI_MSG_TP_CLICK);
		return;
	}
	else if (sysutil::nss_get_instance()->sys_status == UISS_ACC_OFF		// ACC OFF状态下不响应按键
		|| sysutil::nss_get_instance()->sys_camera_status == 1)				// 倒车不响应任何键
	{
		return;
	}
	else if ( (ui_is_on_bt_call() || sysutil::nss_get_instance()->sys_ecar_status || sysutil::nss_get_instance()->sys_rds_status)
		&& id != K_VOL_UP && id != K_VOL_DN && id != K_MUTE)	// 通话时不响应任何键, 但可调节音量,
	{
		return;
	}
	else if (sysutil::nss_get_instance()->sys_calibrate_status == 1)	// 校屏不响应任何键
	{
		return;
	}
	else if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP	// 假关机只响应唤醒键
		&& id != K_POWER)
	{
		if (id == K_EJECT)	// 假关机下可以响应进出碟键
		{
			OnKeyEject(param);
		}
		return;
	}


	// 任何键都起到解mute作用
	if ((id!=K_MUTE && id!=K_VOL_DN && id!=K_VOL_UP) && sysutil::nss_get_instance()->is_audio_mute())
	{
		Sleep(100);
		PostMessage(UI_MSG_MCU_KEY_COMMAND, K_MUTE, 0);

	}


	switch (id)
	{
	case K_SOURCE_HOME:		// 回主界面
  		ui_exit_touch_calibrate();
  		OnKeyHome();
		break;
	case K_HOME_FRONTSRC:		// 主界面与源切换
 		ui_exit_touch_calibrate();
 		OnKeyHomeSrc();
		break;
	case K_VOL_UP:			// volume+, 参数表示按键的重复次数
		OnVolumeChange(max(1,param));	// 防止参数为0 的情况
		break;
	case K_VOL_DN:			// volume-
		OnVolumeChange(-(int)max(1,param));
		break;
	case K_MUTE:			// mute
		OnMuteChange();
		break;
	case K_NAVI:			// navigation
		OnKeyNavi();
		break;
	case T_IR_GOTO:
		// 当前窗口为蓝牙时,数字键用来拨号
		if (CDlgManager::GetInstance()->GetCurDlgID() == CBluetoothDlg_ID
			|| CDlgManager::GetInstance()->GetCurDlgID() == CBTCallDlg_ID)
		{
			return;
		}
	case T_PREV:
	case T_NEXT:
	case T_PLAY:
	case T_STOP:
	case K_UP:
	case K_DOWN:
	case K_LEFT:
	case K_RIGHT:
	case K_ENTER:
	case T_RADIO_PTY:
	case T_REPEAT:
	case T_SHUFFLE:
	case T_TRACK:	// 声道
		OnKeyPlayControl(id, param);
		break;
	case K_EJECT:	// 出碟
		OnKeyEject(param);
		break;
	case K_TUNER:	// 切源到收音,如果已在收音,切波段
		OnKeyTuner();
		break;
	case K_DVD:
		OnKeyDVD();
		break;
	case K_TOUCH_MENU:
		OnKeyTouchMenu();
		break;
	case K_PHONE_ON:	// handled by CBTCallDlg directly
		OnKeyPhoneOn();
		break;
	case K_PHONE_OFF:	// handled by CBTCallDlg directly
		break;
	case K_BLUETOOTH:
		CDlgManager::GetInstance()->ShowDlg(CBluetoothDlg_ID, SOURCE_BT);
		if (sysutil::nss_get_instance()->sys_gps_foreground)
		{
			ui_set_foreground_window(m_hWnd);
			OnGpsForeground(FALSE);
		}
		break;
	case T_SEL_EQ:
		OnKeySelEQ();
		break;
	case K_EQ:
		OnKeyEQ();
		break;
	case K_SETTING:
		OnKeySetting();
		break;

	case K_SOURCE_MODE:		// 切模
		OnKeySourceMode();
		break;
	case T_BLACKOUT:	// 关屏
		ui_close_backlight();
		break;
	case K_POWER:
		if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP)
		{
			OnSleepPower();
		}
		else if (sysutil::nss_get_instance()->sys_status == UISS_NORMAL)
		{
			OnSleep();
		}
		break;
	case K_PHONELINK:
		OnKeyPhoneLink();
		break;
	case T_FULL_SCREEN:
		OnKeyOSD();
		break;
	case K_CLEAR:	// 删除键
		PostMessage(WM_CHAR, (WPARAM)L'\b', 0);
		break;
	case K_STAR:	// *
		PostMessage(WM_CHAR, (WPARAM)L'*', 0);
		break;
	case K_NUMBER:	// #
		PostMessage(WM_CHAR, (WPARAM)L'#', 0);
		break;
	case K_PLUS:	// +
		PostMessage(WM_CHAR, (WPARAM)L'+', 0);
		break;
	case T_LOUDNESS:
		OnKeyLoudness();
		break;
	case T_SUBWOOFER:
		OnKeySubwoofer();
		break;
	case K_AVIN1:
		LoadUiFrontSource(SOURCE_AVIN1);
		break;
	case K_AVIN2:
		LoadUiFrontSource(SOURCE_AVIN2);
		break;
	default:
		break;
	}
}

void CMainWnd::OnKeySubwoofer()
{
	BOOL bEnable = !sysutil::nss_get_instance()->audio_eq_subwoofer_enable;
	sysutil::nss_get_instance()->audio_eq_subwoofer_enable = bEnable;
	SettingsAudio::GetInstance()->EnableSubwoofer(bEnable);
	if (bEnable)
	{
		SettingsAudio::GetInstance()->SetSubwoofer(sysutil::nss_get_instance()->audio_eq_subwoofer);
	}

	COverlayDlg::GetInstance()->MessageTip_ShowUI(bEnable ? L"MIS_SUBWOOFER_ON" : L"MIS_SUBWOOFER_OFF");
}

void CMainWnd::OnKeyLoudness()
{
	if (sysutil::nss_get_instance()->audio_eq_loudness_gain == 0 
		|| sysutil::nss_get_instance()->audio_eq_loudness == 0 )	// 如果值已经为0了,也视为关了,重新打开
	{
		sysutil::nss_get_instance()->audio_eq_loudness_gain = 1;
		if (sysutil::nss_get_instance()->audio_eq_loudness == 0)	// 如果值为0恢复为缺省值
		{
			sysutil::nss_get_instance()->audio_eq_loudness = 10;
		}
	}
	else if (sysutil::nss_get_instance()->audio_eq_loudness_gain == 1)
	{
		sysutil::nss_get_instance()->audio_eq_loudness_gain = 0;
	}

	UINT loudness = sysutil::nss_get_instance()->audio_eq_loudness
		* sysutil::nss_get_instance()->audio_eq_loudness_gain;
	SettingsAudio::GetInstance()->setLoudness(&loudness);

	COverlayDlg::GetInstance()->MessageTip_ShowUI(loudness > 0 ? L"MIS_LOUDNESS_ON" : L"MIS_LOUDNESS_OFF");
}

void CMainWnd::OnKeyOSD()
{
	DLG_INFO* pdi = CDlgManager::GetInstance()->GetCurDlgInfo();
	if (pdi && pdi->pdlg && pdi->pdlg->IsKindOf(L"CDvpVideoBaseDlg"))
	{
		((CDvpVideoBaseDlg*)pdi->pdlg)->OnKeyOSD();
	}
}

void CMainWnd::OnKeySelEQ()
{
	// 如果在导航界面,回到EQ
	if (sysutil::nss_get_instance()->sys_gps_foreground)
	{
		CDlgManager::GetInstance()->ShowDlg(CSetAudioDlg_ID, SOURCE_EQ);
		ui_set_foreground_window(m_hWnd);
		OnGpsForeground(FALSE);
	}
	else if (CDlgManager::GetInstance()->GetCurDlgID() == CSetAudioDlg_ID)	// 已经在EQ界面,回上界面
	{
		//CDlgManager::GetInstance()->GotoLastDlg();
		CSetAudioDlg* pdlg = (CSetAudioDlg*)CDlgManager::GetInstance()->GetCurDlgInfo()->pdlg;
		pdlg->OnBack();
	}
	else
	{
		CDlgManager::GetInstance()->ShowDlg(CSetAudioDlg_ID, SOURCE_EQ);
	}
}

void CMainWnd::OnKeyEQ()
{
	UINT eqType = (sysutil::nss_get_instance()->audio_eq_type+1) % EQ_TYPE_COUNT;
	// 设置EQ
	if (eqType == SETTING_AUDIO_EQ_CUSTOM)	// 个性
	{
		SettingsAudio::GetInstance()->SetCustomEQbyIndex(sysutil::nss_get_instance()->audio_eq_custom);
	}
	else
	{
		UINT value[10];
		SettingsAudio::GetInstance()->GetEQValues(eqType, value);
		SettingsAudio::GetInstance()->SetCustomEQbyIndex(value);
	}
	
	sysutil::nss_get_instance()->audio_eq_type = eqType;
	GetLayerCtrl()->GetWindow()->PostMessage(UI_MSG_EQ_CHANGE, 0, (LPARAM)eqType);

	// tip
	LPCTSTR res_id[] = { L"SET_AUDIO_NONE", L"SET_AUDIO_ROCK", L"SET_AUDIO_POPULAR", L"SET_AUDIO_LIVE",
		L"SET_AUDIO_DANCE", L"SET_AUDIO_CLASSIC", L"SET_AUDIO_SOFT", L"SET_AUDIO_CUSTOM"};
	COverlayDlg::GetInstance()->MessageTip_ShowUI(res_id[eqType]);
}

void CMainWnd::OnKeySetting()
{
	// 如果在导航界面,回到setting
	if (sysutil::nss_get_instance()->sys_gps_foreground)
	{
		CDlgManager::GetInstance()->GotoSource(SOURCE_SETTING);
		ui_set_foreground_window(m_hWnd);
		OnGpsForeground(FALSE);
	}
	else if (CDlgManager::GetInstance()->GetCurDlgInfo()->source_id == SOURCE_SETTING)	// 已经在setting界面,回源界面
	{
		CDlgManager::GetInstance()->GotoSource(CSourceManager::GetInstance()->GetFrontSrcID());
	}
	else
	{
		CDlgManager::GetInstance()->GotoSource(SOURCE_SETTING);
	}
}

void CMainWnd::OnKeyPhoneLink()
{
	if(sysutil::nss_get_instance()->is_source_available(SOURCE_PHONE_LINK))
	{
		CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_PHONE_LINK);
	}
	else
	{
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"MIS_PHONELINK_NOT_EXIST");
	}

}

void CMainWnd::OnKeyPhoneOn()
{
	// 如果当前窗口不是蓝牙窗口,切换到蓝牙窗口,如果已经在蓝牙窗口,拨打上一次的电话
	// 倒车或通话时,按键被拦截了，不会执行这段代码,所以这里不再判断
	if (CDlgManager::GetInstance()->GetCurDlgID() != CBluetoothDlg_ID
		|| sysutil::nss_get_instance()->sys_gps_foreground)
	{
		CDlgManager::GetInstance()->ShowDlg(CBluetoothDlg_ID, SOURCE_BT);
		DLG_INFO *pdi = CDlgManager::GetInstance()->GetCurDlgInfo();
		if (pdi && pdi->pdlg && pdi->pdlg->IsKindOf(L"CBluetoothDlg"))
		{
			((CBluetoothDlg*)pdi->pdlg)->OnFunChange(BT_FUN_DIAL);	// 切换到拨号页面
		}

		if (sysutil::nss_get_instance()->sys_gps_foreground)
		{
			ui_set_foreground_window(m_hWnd);
			OnGpsForeground(FALSE);
		}
	}
	else 	// 如果在蓝牙窗口,执行拨号功能
	{
		DLG_INFO *pdi = CDlgManager::GetInstance()->GetCurDlgInfo();
		if (pdi && pdi->pdlg && pdi->pdlg->IsKindOf(L"CBluetoothDlg"))
		{
			((CBluetoothDlg*)pdi->pdlg)->OnFunChange(BT_FUN_DIAL);	// 切换到拨号页面
			((CBluetoothDlg*)pdi->pdlg)->OnKeyCall();
		}
	}

}

void CMainWnd::OnKeySourceMode()
{
	SOURCE_ID sid = CSourceManager::GetInstance()->GetFrontSrcID();
	sid = ui_get_next_sid(sid);
	while (!sysutil::nss_get_instance()->is_source_available(sid))
	{
		sid = ui_get_next_sid(sid);
	}

	// phonelink切源该版本改了下逻辑
	if(sid == SOURCE_PHONE_LINK)
	{
		// 由phonelinkstardlg自己加载源,因为做了USB端口拉低动作
		CDlgManager::GetInstance()->GotoSource(SOURCE_PHONE_LINK);
		return;
	}

	LoadUiFrontSource(sid);
}

void CMainWnd::OnKeyEject(UINT param)
{
	BYTE byte0 = (BYTE)((param>>24) & 0xFF);			// 0 表示出碟， 1 表示进碟
	BYTE byte1 = (BYTE)((param>>16) & 0xFF);		// 为1表示此键无效,忽略， 为0表示byte0有效
	BYTE byte2 = (BYTE)((param>>8) & 0xFF);	// 为1表示强制出碟
	if (byte2 || (byte1 == 0 && byte0 == 0))	// 出碟
	{
		OnDVDEject();
	}
	else if (byte1 == 0 && byte0 == 1)			// 进碟
	{
		OnDVDTrayIn();
	}
}

void CMainWnd::OnDVDEject()
{
	if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_DVD
		|| CSourceManager::GetInstance()->GetRearSrcID() == SOURCE_DVD)
	{
		DLG_INFO* pdi = CDlgManager::GetInstance()->GetDlgInfo(CDVDStartDlg_ID, SOURCE_DVD);
		if (pdi && pdi->pdlg)
		{
			((CDVDStartDlg*)pdi->pdlg)->OnEject();
		}
		else	// 有可能是虚拟碟在录制碟片
		{
			pdi = CDlgManager::GetInstance()->GetDlgInfo(CVDiskListDlg_ID, SOURCE_VDISK);
			if (pdi && pdi->pdlg)
			{
				((CVDiskListDlg*)pdi->pdlg)->OnRipInterrupt();
				SetTimer(TIMER_ID_DVD_EJECT, 200, NULL);	// 出碟
			}
		}
	}
	else	// DVD不是当前源,可立即出碟
	{
		if (protocol::get_mcu_status()->is_DISK_exist())
		{
			if (m_bDVDEjectEnable)
			{
				DVDEject();
			}
			else
			{
				SetTimer(TIMER_ID_DVD_EJECT, 200, NULL);
			}
		}
	}
}

void CMainWnd::OnDVDTrayIn()
{
	CRpcMCU::GetInstance()->RPC_KeyCommand(K_EJECT, (UINT)(1<<24));
}


void CMainWnd::OnKeyTuner()
{
	// 窗口不在前台,或FM没有show出来,需将FM显示出来
	if (!IsForegroundWindow()
		|| CDlgManager::GetInstance()->GetCurDlgID() != CRadioDlg_ID)	
	{
		LoadUiFrontSource(SOURCE_FM);
	}
	else
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_BAND, 0);
	}
}

void CMainWnd::OnKeyDVD()
{
	if(sysutil::nss_get_instance()->is_source_available(SOURCE_DVD))
	{
		LoadUiFrontSource(SOURCE_DVD);
	}
	else if (!config::get_config_functions()->is_support(SOURCE_DVD))
	{
		; // 如果不带DVD功能,不作任何提示
	}
	else
	{
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"MIS_DISK_NOT_EXIST");
	}
}

void CMainWnd::OnKeyTouchMenu()
{
	ui_touch_calibrate();
}

void CMainWnd::OnKeyPlayControl(UINT key, UINT param)
{
	// 由当前源的活动窗口处理该命令
	SOURCE_ID sid = CSourceManager::GetInstance()->GetFrontSrcID();
	if (sid != INVALID_SOURCE_ID)
	{
		DLG_INFO* pdi = CDlgManager::GetInstance()->GetActiveDlgInfo(sid);
		if (pdi && pdi->pdlg
			&& !CDlgManager::GetInstance()->IsExternalApp(pdi)
			&& pdi->pdlg->IsKindOf(L"CDvpBaseDlg"))
		{
			((CDvpBaseDlg*)pdi->pdlg)->OnKeyPlayControl(key, param);
		}
		else if (sid == SOURCE_BT)	// 节省内存,BT窗口可能被删除,这种情况下这里直接处理
		{
			CBluetoothDlg::PlayControl(key, param);
		}
	}
}

void CMainWnd::OnKeyHome()
{
	CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
	if (sysutil::nss_get_instance()->sys_gps_foreground)
	{
 		ui_set_foreground_window(m_hWnd);
		OnGpsForeground(FALSE);
	}
}

void CMainWnd::OnKeyHomeSrc()
{
	// 如果在导航界面,回到主界面
	if (sysutil::nss_get_instance()->sys_gps_foreground)
	{
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
		ui_set_foreground_window(m_hWnd);
		OnGpsForeground(FALSE);
	}
	else if (CDlgManager::GetInstance()->GetCurDlgID() == CMainDlg_ID)	// 已经在主界面,回源界面
	{
		CDlgManager::GetInstance()->GotoSource(CSourceManager::GetInstance()->GetFrontSrcID());
	}
	else
	{
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
	}
}

void CMainWnd::OnKeyNavi()
{
	// 开启了截屏功能
	if (sysutil::nss_get_instance()->ui_capture_screen)
	{
		DLG_INFO *pdi = CDlgManager::GetInstance()->GetCurDlgInfo();
		if (pdi && pdi->pdlg)
		{
			CWceUiGenericBitmap* pbmp = NULL;
			if (WceUiGetBitsPixel() == 32)
			{
				pbmp = new CWceUi32Bitmap();
			}
			else
			{
				pbmp = new CWceUi565Bitmap();
			}
			//CWceUi32Bitmap bmp;
			pbmp->CreateDIB(WceUiGetScreenWidth(), WceUiGetScreenHeight());
			CRect rc(0,0,WceUiGetScreenWidth(),WceUiGetScreenHeight());
			GetLayerCtrl()->DrawLayerToBmp(pdi->pdlg, pbmp, rc);
			SYSTEMTIME st;
			GetLocalTime(&st);
			TCHAR file_name[64];
			_stprintf(file_name, L"sdmmc\\sc%d_%d_%d.bmp", st.wHour, st.wMinute, st.wSecond);
			pbmp->SaveFile(file_name);
			delete pbmp;
		}
		return;
	}

	// 因为是用计时器检测GPS状态,如果key切换太快的话，有可能计时器没有执行到,
	// 从而产生状态不一致问题，切换时这里主动检测一次
	OnCheckNavigation();

	if (sysutil::nss_get_instance()->sys_gps_foreground)
	{
 		//ui_set_foreground_window(m_hWnd);
		DLG_INFO* pdi = CDlgManager::GetInstance()->GetCurDlgInfo();
		if (pdi)
		{
			if (CDlgManager::GetInstance()->IsExternalApp(pdi))
			{
				ExternalApp_Show_Fun show_fun = (ExternalApp_Show_Fun)pdi->pdlg;
				show_fun(TRUE);
			}
			else
			{
				ui_set_foreground_window(m_hWnd);
			}
		}

		OnGpsForeground(FALSE);
	}
	else
	{
		if (CDlgManager::GetInstance()->GetCurDlgID() == CNaviWarningDlg_ID)	// 如果已经在warning界面了,隐藏
		{
			//CDlgManager::GetInstance()->GotoLastDlg();
			PostMessage(UI_MSG_NAVI_WARNGINDLG_EXIT);
			return;
		}

		LoadNavigation();
	}

}

BOOL CMainWnd::LoadNavigation(BOOL bShowWarning)
{
	if (!ui_is_navigation_run() && bShowWarning)
	{
		CDlgManager::GetInstance()->ShowDlg(CNaviWarningDlg_ID, SOURCE_NAVI_WARNING);
		return TRUE;
	}

	// EasyConnected源时,退出源再进导航
// 	if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_EASY_CONNECTED)
// 	{
// 		LoadUiFrontSource(SOURCE_FM);
// 	}

	RemoveTopmost();
	if (!ui_load_navigation())
	{
		ShowNaviErrorMsg();
		return FALSE;
	}

	return TRUE;
}

void CMainWnd::RemoveTopmost()
{
	if (m_bIsTopMostWindow)
	{
		m_bIsTopMostWindow = FALSE;
		::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOREPOSITION | SWP_NOSIZE);
	}
}

void CMainWnd::SetTopmost()
{
	if (!m_bIsTopMostWindow)
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOREPOSITION | SWP_NOSIZE);
		m_bIsTopMostWindow = TRUE;
	}
}

void CMainWnd::ShowNaviErrorMsg()
{
	CString str = sysutil::nss_get_instance()->gps_path;

	if (str.GetLength() > 0 && str[0] == L'\\')	// 路径可能配成了\Storage card\Navione\navi.exe，也是合法的
	{
		str = str.Mid(1);	// 取消第一个'\'
	}

	int pos = str.Find(L'\\');
	if (pos>=0)		// 找到了'\',获取路径的盘符
	{
		str = str.Mid(0, pos);	
	}

	if (sysutil::nss_get_instance()->gps_path[0] == L'\0')
	{
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"MIS_NAVI_PATH_NOT_SET");
	}
	else if (GetFileAttributes(str) == (DWORD)-1)
	{
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"MIS_NAVI_CARD_NOT_EXIST");
	}
	else
	{
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"MIS_NAVI_LOAD_FAIL");
	}
}

void CMainWnd::SetTopMostWindowFlag(BOOL bTop)
{
	m_bIsTopMostWindow = bTop;
}

void CMainWnd::OnFastCameraExit()
{
	// 开机倒车退出,或没有开机倒车，都需调用下面的接口，让ARM2释放MCU串口，
	DWORD ret1 = YC_GetBootBackCarState();
	HWND hwnd = ::FindWindow(NULL, L"FastcamerUI");
	if (hwnd)
	{
		// 这个不是必须的,YC_GetBootBackCarState()调用与退出fastcamera间sleep下就不会弹出开机LOG了
		// fastcamera退出时，会将最后一帧数据COPY到primary surface
		// 为什么sleep下就不闪了??????
		Sleep(2);	
		::PostMessage(hwnd, YC_MSG_EXIT_BACKCAR, 0, 0);
	}

	sysutil::nss_get_instance()->sys_fastcamera_status = 0;		// 开机倒车已经退出,设置标志


#define DISPLAY_IOCTL_HIDE_BACK_VIDEO        (0x00020002)
	ExtEscape(::GetDC(NULL), DISPLAY_IOCTL_HIDE_BACK_VIDEO, 0, NULL,0, NULL);	// 释放开机倒车占用的overlay
	CUiDDraw::GetInstance()->InitDDraw(m_hWnd);

}

void CMainWnd::GPSSetTime()
{
	// 如果已经定位了，直接设置时间，如果未定位，设置需要设置时间的标志
	if (m_GPSProxy.IsFixed())
	{
		//GPSSetTime(m_GPSProxy.GetGpsInfo());
		GPSInfo* pinfo = m_GPSProxy.GetGpsInfo();
		SYSTEMTIME tm;
		tm.wYear	= pinfo->year;
		tm.wMonth	= pinfo->mon;
		tm.wDay		= pinfo->day;
		tm.wHour	= pinfo->hour;
		tm.wMinute	= pinfo->min;
		tm.wSecond	= (WORD)pinfo->sec;

		if (sysutil::nss_get_instance()->ui_time_dst)
		{
			ui_set_systemtime_DST(&tm, TRUE);
		}
		else
		{
			::SetSystemTime(&tm);	// DST未打开时,以当前GPS时间为准
		}

		sysutil::nss_get_instance()->ui_time_need_autoset = FALSE;
	}
	else
	{
		sysutil::nss_get_instance()->ui_time_need_autoset = TRUE;
	}
}

void CMainWnd::OnLanguageChange(int lan_id)
{
	// 设置UI的语言
	sysutil::nss_get_instance()->ui_lan_id = lan_id;
	ui_set_language(lan_id);
	InvalidateRect(NULL);

	// 通知MCU
	//
}

void CMainWnd::OnBKChange(int index)
{
	sysutil::nss_get_instance()->ui_bk_index = index;
	sysutil::nss_load_bkground(index);
}

CMainWnd* CMainWnd::GetInstance()
{
	return m_pthis;
}

void CMainWnd::OnAccOff()
{
	// 如果在假关机状态下,把之前的源加载起来,不影响后面的逻辑
	if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP
		|| sysutil::nss_get_instance()->sys_status == UISS_SLEEP_AWAKE)
	{
// 		if (!sysutil::nss_get_instance()->is_source_available(m_sidSleep))
// 		{
// 			m_sidSleep = SOURCE_FM;
// 		}
// 		LoadUiFrontSource(m_sidSleep);

		// 记忆假关机前的源
		sysutil::nss_get_instance()->ui_front_source_id = m_sidSleep;
	}


	sysutil::nss_get_instance()->sys_status = UISS_ACC_OFF;
	// ACC OFF需静音
	CVolumeManager::GetInstance()->UpdateVolume();

	if (ui_msdk_get_cursrc() != INVALID_SOURCE_ID)	// 说明当前源是MSDK相关源
	{
		CSourceManager::GetInstance()->SaveMemory_MSDK();
	}
	// vdisk
	CSourceManager::GetInstance()->SaveMemory_VDisk();
	DLG_INFO *pdi = CDlgManager::GetInstance()->GetDlgInfo(CVDiskListDlg_ID, SOURCE_VDISK);
	if (pdi && pdi->pdlg)
	{
		((CVDiskListDlg*)pdi->pdlg)->OnRipInterrupt();
	}
	// 导航状态: 0~未运行,1~前台运行,2~后台运行
	if (ui_is_navigation_run())
	{
		sysutil::nss_get_instance()->gps_running_status = sysutil::nss_get_instance()->sys_gps_foreground ? 1 : 2;
	}
	else
	{
		sysutil::nss_get_instance()->gps_running_status = 0;
	}

	// 保存当前的用户状态
	sysutil::nss_save(TRUE);

	//*** MCU需保存的数据 ***//
	// 时间, 必须是中国时区的时间,系统启动时MCU会将该时间发给OS,OS只认中国时区时间
	SYSTEMTIME tm;
	RPCERR ret;
	GetSystemTime(&tm);
	ui_time_calculate(&tm, 8, 0, 0);	// UTC时间加上8个小时即为中国时区时间
	protocol::get_mcu_sys_para()->set_system_time(&tm);
	CRpcMCU::GetInstance()->RPC_SetSysParams(MCU_SYS_PARA::INDEX_TIME, protocol::get_mcu_sys_para()->time_data, 
		sizeof(protocol::get_mcu_sys_para()->time_data), &ret);
	// video参数
	NAVI_SYSTEM_STATUS* pnss = sysutil::nss_get_instance();
	protocol::get_mcu_sys_para()->set_video_data(pnss->video_backlight_normal, pnss->video_brightness, 
		pnss->video_contrast, pnss->video_hue, pnss->video_saturation, 
		pnss->video_rear_camera_mirror ? MSDK_AVIN_FLIP_V : MSDK_AVIN_FLIP_NONE);
	CRpcMCU::GetInstance()->RPC_SetSysParams(MCU_SYS_PARA::INDEX_VIDEO, protocol::get_mcu_sys_para()->video_data, 
		sizeof(protocol::get_mcu_sys_para()->video_data), &ret);

	// dvd数据
	if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_DVD)
	{
		CDvdRender::GetInstance()->SaveDVPLM();	// 如果是当前源,先保存数据到内存
	}
	CDvdRender::GetInstance()->WriteDVPLM2File();

	// BT通话记录
	CBluetooth::GetInstance()->SaveLocalCallInfo();

	// 保存完毕，通知MCU可以断电了
	CRpcMCU::GetInstance()->RPC_KeyCommand(K_SAVE_ALL);

	// 关背光
	winhook::DisableTouchPanel();
	YC_BackLightEnable(FALSE);
}

void CMainWnd::OnAccON()
{
	CRpcMCU::GetInstance()->RPC_GetStatus(protocol::get_mcu_status());
	// AccON时从MCU获取假关机状态
	sysutil::nss_get_instance()->sys_status = 
		protocol::get_mcu_status()->is_sleep() ? UISS_SLEEP : UISS_NORMAL;

	// 如果是假关机状态,什么都不做,保持假关机状态
	if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP)
	{
		return;
	}

	//sysutil::nss_get_instance()->sys_status = UISS_NORMAL;
	// ACC ON更新音量状态
	CVolumeManager::GetInstance()->UpdateVolume();

	winhook::EnableTouchPanel();
	YC_BackLightEnable(TRUE);	// 打开背光
}

void CMainWnd::OnSleep()
{
	sysutil::nss_get_instance()->sys_status = UISS_SLEEP;
	// SLEEP需静音
	CVolumeManager::GetInstance()->UpdateVolume();

	// 关背光
	KillTimer(TIMER_ID_SLEEP_BACKLIGHT_ENABLE);
	winhook::DisableTouchPanel();
	YC_BackLightEnable(FALSE);

	if ((SOURCE_ID)sysutil::nss_get_instance()->ui_front_source_id != INVALID_SOURCE_ID)	// 当前有源时才记忆
	{
		m_sidSleep = (SOURCE_ID)sysutil::nss_get_instance()->ui_front_source_id;
		CSourceManager::GetInstance()->SetFrontRearSrc(INVALID_SOURCE_ID);
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
	}

	// 通知MCU
	CRpcMCU::GetInstance()->RPC_KeyCommand(K_POWER, 0x01000000);
}

void CMainWnd::OnSleepAwake()
{
	sysutil::nss_get_instance()->sys_status = UISS_SLEEP_AWAKE;
	// 唤醒后只能BT出声音
	CVolumeManager::GetInstance()->UpdateVolume();

	winhook::EnableTouchPanel();
	//YC_BackLightEnable(TRUE);	// 打开背光
	SetTimer(TIMER_ID_SLEEP_BACKLIGHT_ENABLE, SLEEP_BACKLIGHT_ENABLE_DELAY, NULL);

	CRpcMCU::GetInstance()->RPC_KeyCommand(K_POWER, 0);
}

void CMainWnd::OnSleepPower(BOOL bLoadLastSource)
{
	sysutil::nss_get_instance()->sys_status = UISS_NORMAL;
	CVolumeManager::GetInstance()->UpdateVolume();

	winhook::EnableTouchPanel();
	//YC_BackLightEnable(TRUE);	// 打开背光
	if ((m_sidSleep == SOURCE_SD || m_sidSleep == SOURCE_USB || m_sidSleep == SOURCE_USB_REAR)
		&& !sysutil::nss_get_instance()->sys_bt_call_status
		&& !sysutil::nss_get_instance()->sys_camera_status)	// 如果唤醒后加载SD或USB源，背光打开晚一点
	{
		SetTimer(TIMER_ID_SLEEP_BACKLIGHT_ENABLE, SLEEP_BACKLIGHT_ENABLE_DELAY+1000, NULL);
	}
	else
	{
		SetTimer(TIMER_ID_SLEEP_BACKLIGHT_ENABLE, SLEEP_BACKLIGHT_ENABLE_DELAY, NULL);
	}

	// 只要是假关机开机,响一下beep音
	ui_mcu_beep();


	if (!sysutil::nss_get_instance()->is_source_available(m_sidSleep))
	{
		m_sidSleep = SOURCE_FM;
	}
	if (bLoadLastSource)
	{
		LoadUiFrontSource(m_sidSleep);
	}
	m_sidSleep = INVALID_SOURCE_ID;

	CRpcMCU::GetInstance()->RPC_KeyCommand(K_POWER, 0);
}


void CMainWnd::OnGPSUpdate()
{
	// 监控GPS定位的状态变化

	static BOOL bFixed = FALSE;		// 刚启动时认为未定到位
	int b = m_GPSProxy.IsFixed();	// 返回值为定位到的恒星个数，如果个数为0表示未定位到
	if ((bFixed && !b) || (!bFixed && b))
	{
		bFixed = b ? TRUE : FALSE;
		::PostMessage(HWND_BROADCAST, NAVI_MSG_GPS_FIXED, 0, (LPARAM)(b ? 1 : 0));
	}
	else
	{
		;// 状态没有变化
	}
}

CGpsProxy* CMainWnd::GetGpsProxy()
{
	return &m_GPSProxy;
}

void CMainWnd::DVDEject()
{
	CDvdRender::GetInstance()->Eject();
	CRpcMCU::GetInstance()->RPC_KeyCommand(K_EJECT);
}

void CMainWnd::LoadMSDKSource(SOURCE_ID sid)
{
	CDlgManager::GetInstance()->GotoSource(sid);
	// 用计时器来播放记忆文件,加载速度有点慢,会影响UI动画,所以延时播放
	MSDK_MEMORY* pmemory = ui_msdk_get_memory(sid);
	if (pmemory && pmemory->memory_play )	// 刚进源时,sourcemanager会置此标志
	{
		if (sid == SOURCE_SD)
		{
			SetTimer(TIMER_ID_LOAD_SD_SOURCE, 500, NULL);
		}
		else if (sid == SOURCE_USB)
		{
			SetTimer(TIMER_ID_LOAD_USB_SOURCE, 500, NULL);
		}
		else if (sid == SOURCE_USB_REAR)
		{
			SetTimer(TIMER_ID_LOAD_USB_REAR_SOURCE, 500, NULL);
		}
	}


	return;
}

void CMainWnd::LoadSource(SOURCE_ID sid, DVP_SINK sink)
{
	if (sysutil::nss_get_instance()->sys_rds_status || sysutil::nss_get_instance()->sys_bt_call_status)
	{
		m_sidToLoad = sid;
		m_sinkToLoad = sink;
		return;
	}
	else
	{
		m_sidToLoad = INVALID_SOURCE_ID;
	}

	// 如果源不可能,直接返回,不切换(用户提示？声音或文字？)
	if (!sysutil::nss_get_instance()->is_source_available(sid))
	{
		return;
	}

	// 目前所有的源都是从该入口加载的, 这里控制一下切源的时间, 不充许1秒内多次切源
	SOURCE_ID old_sid = CSourceManager::GetInstance()->GetFrontSrcID();
	if (sid != old_sid)
	{
		static DWORD dwTick = 0;
		if (GetTickCount() - dwTick < 1000
			&& sysutil::nss_get_instance()->is_source_available(old_sid))	// 如果源不可用了，还是要切源
		{
			return;
		}
		dwTick = GetTickCount();
	}

	// 如果需要BT, IPOD初始化
	if (sid == SOURCE_IPOD_USB)
	{
		/* --++ ipod
		if (!CIPODManager::GetInstance()->IsIPODSDKInitlized())
		{
			KillTimer(TIMER_ID_DELAY_LOAD_IPOD);
			CIPODManager::GetInstance()->InitIPodSDK(m_hWnd);
		}
		*/
	}

	// 这里对vdisk源作个假设,不能是单独的后排源，这种情况暂时不支持
	if (sysutil::nss_get_instance()->vdisk_is_ripping)	// vdisk在录制时是一种特殊状态(前排是vdisk后排是dvd),需处理下
	{
		if (sid == SOURCE_VDISK)
		{
			sysutil::nss_get_instance()->vdisk_sink = sink;		// 记录下sink状态,只对前排执行后面的逻辑
			sink = DVP_SINK_FRONT;
		}
		else	// 不管是前排还是后排需切换到其它源，都要退出RIP
		{
			StopVdiskRip();
		}
	}
	else
	{
		if (sid == SOURCE_VDISK)
		{
			sysutil::nss_get_instance()->vdisk_sink = sink;		// 记录下sink状态
		}
	}
	// MSDK有延时播放的动作,如果源切换太快,有可能计时器没到,如果切另一个源,把计时器关掉
	KillTimer(TIMER_ID_LOAD_SD_SOURCE);
	KillTimer(TIMER_ID_LOAD_USB_SOURCE);
	KillTimer(TIMER_ID_LOAD_USB_REAR_SOURCE);
	KillTimer(TIMER_ID_LOAD_VDISK_SOURCE);

	// 加载源
	if (sink == DVP_SINK_FRONT)
	{
		CSourceManager::GetInstance()->SetFrontSrc(sid);
	}
	else if (sink == DVP_SINK_REAR)
	{
		CSourceManager::GetInstance()->SetRearSrc(sid);
	}
	else if (sink = DVP_SINK_FRONT_REAR)
	{
		CSourceManager::GetInstance()->SetFrontRearSrc(sid);
	}

	// 加载源UI
	if ((sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR))
	{
		LoadMSDKSource(sid);
	}
	else if (sid == SOURCE_VDISK)
	{
		CDlgManager::GetInstance()->GotoSource(sid);
		// 用计时器来播放记忆文件,加载速度有点慢,会影响UI动画,所以延时播放
		VDISK_MEMORY* pmemory = &sysutil::nss_get_instance()->vdisk_memory;
		if (pmemory && pmemory->memory_play )	// 刚进源时,sourcemanager会置此标志
		{
			SetTimer(TIMER_ID_LOAD_VDISK_SOURCE, 500, NULL);
		}
	}
	else if (sid == SOURCE_PHONE_LINK && CPhoneLink::GetInstance()->IsConnected())	// 如果已经连接上了,直接进phonelink画面,不进起始画面
	{
		CDlgManager::GetInstance()->ShowDlg(PhoneLink_ID, SOURCE_PHONE_LINK);
	}
	else
	{
		CDlgManager::GetInstance()->GotoSource(sid);
	}


	// 只要是要主动加载源了,或切换源,就把GPS切到后台(这个逻辑是否完全成立?)
	DLG_INFO* pdi = CDlgManager::GetInstance()->GetActiveDlgInfo(sid);
	if (!CDlgManager::GetInstance()->IsExternalApp(pdi))	// 外部程序源,不显示当前窗口
	{
		ui_set_foreground_window(m_hWnd);
	}
	else	// 外部源,显示外部源窗口
	{
		if(pdi)
		{
			ExternalApp_Show_Fun show_fun = (ExternalApp_Show_Fun)pdi->pdlg;
			show_fun(TRUE);
		}
	}
	OnGpsForeground(FALSE);


	// 源切换后,音量增益有可能有变化,更新下当前音量
	CVolumeManager::GetInstance()->UpdateVolume();
}

void CMainWnd::LoadUiFrontSource(SOURCE_ID sid)
{
	if (sysutil::nss_get_instance()->ui_dual_source_on)
	{
		if (sid == SOURCE_FM || sid == SOURCE_BT || sid == SOURCE_PHONE_LINK || sid == SOURCE_EASY_CONNECTED)	// FM, BT不从后排输出
		{
			CMainWnd::GetInstance()->LoadSource(sid, DVP_SINK_FRONT);
		}
		else	// 这些源前后排可输出
		{
			// 如果后排没有源, 将源设为前后排播放,如果有源,则只在前排播放
			if (CSourceManager::GetInstance()->GetRearSrcID() == INVALID_SOURCE_ID)	
			{
				CMainWnd::GetInstance()->LoadSource(sid, DVP_SINK_FRONT_REAR);
			}
			else
			{
				CMainWnd::GetInstance()->LoadSource(sid, DVP_SINK_FRONT);
			}
		}
	}
	else	// 无双工功能
	{
		if (sid == SOURCE_FM || sid == SOURCE_BT || sid == SOURCE_PHONE_LINK || sid == SOURCE_EASY_CONNECTED)	// FM, BT不从后排输出
		{
			StopVdiskRip();	// 录制时是一种特殊源状态,先停止录制
			CSourceManager::GetInstance()->SetRearSrc(INVALID_SOURCE_ID);
			CMainWnd::GetInstance()->LoadSource(sid, DVP_SINK_FRONT);
		}
		else	// 这些源前后排始终保持一致
		{
			CMainWnd::GetInstance()->LoadSource(sid, DVP_SINK_FRONT_REAR);
		}
	}

}

void CMainWnd::StopVdiskRip()
{
	if (sysutil::nss_get_instance()->vdisk_is_ripping)
	{
		DLG_INFO *pdi = CDlgManager::GetInstance()->GetDlgInfo(CVDiskListDlg_ID, SOURCE_VDISK);
		if (pdi && pdi->pdlg)
		{
			((CVDiskListDlg*)pdi->pdlg)->OnRipInterrupt();
		}
	}
}

void CMainWnd::LoadUiRearSource(SOURCE_ID sid)
{
	if (sysutil::nss_get_instance()->ui_dual_source_on)
	{
		if (sid == SOURCE_FM || sid == SOURCE_BT)	// FM, BT不从后排输出
		{
			RETAILMSG(1, (L"[naiv car]: LoadUiRearSource() ~ FM or BT doesnot support rear zone.\n"));
		}
		else	// 这些源前后排可输出
		{
			CMainWnd::GetInstance()->LoadSource(sid, DVP_SINK_REAR);
		}
	}
	else	// 无双工功能
	{
		RETAILMSG(1, (L"[naiv car]: LoadUiRearSource() ~ dual zone is off.\n"));
	}

}



void CMainWnd::OnSourceExit(SOURCE_ID sid)
{
	// 如果快速插拔,有可能这个计时器还没执行就要退出源了,这种情况不能执行计时器代码
	if (sid == SOURCE_SD)				KillTimer(TIMER_ID_LOAD_SD_SOURCE);
	else if(sid == SOURCE_USB)			KillTimer(TIMER_ID_LOAD_USB_SOURCE);
	else if(sid == SOURCE_USB_REAR)		KillTimer(TIMER_ID_LOAD_USB_REAR_SOURCE);

	if (CSourceManager::GetInstance()->GetFrontSrcID() == sid)
	{
		SOURCE_ID sid = CSourceManager::GetInstance()->GetLastFrontSrcID();
		if (!sysutil::nss_get_instance()->is_source_available(sid))
		{
			sid = SOURCE_FM;
		}
		LoadUiFrontSource(sid);
	}

	if (CSourceManager::GetInstance()->GetRearSrcID() == sid)
	{
		LoadUiRearSource(CSourceManager::GetInstance()->GetLastRearSrcID());
	}
}



#include <Storemgr.h>
#include <msgqueue.h>
#include <pnp.h>

UINT CMainWnd::StorageCardMonitorProc(LPVOID pParam)
{
	CMainWnd* pthis = (CMainWnd*)pParam;

	typedef union {
		DEVDETAIL d;
		char pad[sizeof(DEVDETAIL)+MAX_DEVCLASS_NAMELEN];
	} MYDEVDETAIL;

	HANDLE hNotify = NULL;
	HANDLE hMsgQueue = NULL;
	MSGQUEUEOPTIONS msgopt;
	memset(&msgopt, 0, sizeof(MSGQUEUEOPTIONS));
	msgopt.dwSize = sizeof(MSGQUEUEOPTIONS);
	msgopt.cbMaxMessage = 64;
	msgopt.bReadAccess = TRUE;
	hMsgQueue = CreateMsgQueue(UI_MAIN_WINDOW_NAME, &msgopt);


	if (hMsgQueue == NULL)
	{
		RETAILMSG(1,(L"CreateMsgQueue \r\n"));
		goto EXIT;
	}


	hNotify = RequestDeviceNotifications(&FATFS_MOUNT_GUID, hMsgQueue, FALSE);
	if (hNotify == NULL)
	{
		RETAILMSG(1,(L"RequestDeviceNotifications \r\n"));
		goto EXIT;
	}

	MYDEVDETAIL detail;
	DWORD dwRead, dwFlags;
	while (1)
	{
		if (ReadMsgQueue(hMsgQueue, &detail, sizeof(detail), &dwRead, INFINITE, &dwFlags))
		{
			if (_tcsicmp(detail.d.szName, L"\\SDMMC") == 0||_tcsicmp(detail.d.szName, L"SDMMC") == 0)
			{
				pthis->PostMessage(UI_MSG_STORAGE_CARD_PLUG, UIMS_SDMMC, detail.d.fAttached ? UIMS_PLUG_IN : UIMS_PLUG_OUT);
			}
			if (_tcsicmp(detail.d.szName, L"\\STORAGE CARD") == 0||_tcsicmp(detail.d.szName, L"STORAGE CARD") == 0)
			{
				pthis->PostMessage(UI_MSG_STORAGE_CARD_PLUG, UIMS_STORAGE_CARD, detail.d.fAttached ? UIMS_PLUG_IN : UIMS_PLUG_OUT);
			}
			else if (_tcsicmp(detail.d.szName, L"\\USBDISK") == 0||_tcsicmp(detail.d.szName, L"USBDISK") == 0)
			{
				pthis->PostMessage(UI_MSG_STORAGE_CARD_PLUG, UIMS_USB, detail.d.fAttached ? UIMS_PLUG_IN : UIMS_PLUG_OUT);
			}
			else if (_tcsicmp(detail.d.szName, L"\\USBDISK1") == 0||_tcsicmp(detail.d.szName, L"USBDISK1") == 0)
			{
				pthis->PostMessage(UI_MSG_STORAGE_CARD_PLUG, UIMS_USB1, detail.d.fAttached ? UIMS_PLUG_IN : UIMS_PLUG_OUT);
			}
		}
		else
		{

			RETAILMSG(1,(L"ReadMsgQueue \r\n"));
		}
	}

EXIT:
	if (hNotify)
	{
		StopDeviceNotifications(hNotify);
		hNotify = NULL;
	}

	if (hMsgQueue)
	{
		CloseMsgQueue(hMsgQueue);
		hMsgQueue = NULL;
	}

	return 0;
}

//SD插入和拔出时，OS通知的事件
#define SD_PLUG_IN_OUT   L"MTK_SD_CARD_PLUG_IN_OUT_EVENT" 
#define SD_CARD_PLUGIN_EVENT       			((DWORD)1 << 0)
#define SD_CARD_PLUGOUT_EVENT      			((DWORD)1 << 1)
#define SD_CARD_WP_EVENT           			((DWORD)1 << 2)
#define SD_CARD_WR_EVENT           			((DWORD)1 << 3)
#define SD_CARD_MSDCINDEX_EVENT(index)		((DWORD)(index) >> 16)
#define SD_CARD_MSDCINDEX_MASK				(0xFFFF0000)
UINT CMainWnd::SDCardMonitorProc(LPVOID pParam)
{
	CMainWnd* pthis = (CMainWnd*)pParam;

	HANDLE hEventSD = ::CreateEvent(NULL, TRUE, FALSE, SD_PLUG_IN_OUT);
	while (1)
	{
		DWORD dwResult = WaitForSingleObject(hEventSD, INFINITE);
		if (dwResult == WAIT_OBJECT_0)
		{
			DWORD dwData = ::GetEventData( hEventSD );
			DWORD dwIndex = SD_CARD_MSDCINDEX_EVENT(dwData & SD_CARD_MSDCINDEX_MASK);	// 0 ~ SDMMC, 1 ~ Storage Card
			BOOL bSDOut =(dwData & SD_CARD_PLUGOUT_EVENT) ? TRUE : FALSE;

			UI_MASS_STORAGE_TYPE msType = dwIndex == 0 ? UIMS_SDMMC : UIMS_STORAGE_CARD;
			UI_MASS_STORAGE_PLUG msAction = (LPARAM)bSDOut ? UIMS_PLUG_OUT : UIMS_PLUG_IN;
			if (msAction == UIMS_PLUG_OUT && _is_gps_path_by_mstype(msType))
			{
				// 导航卡被拔出, 关闭导航进程
				ui_terminate_navigation();
			}
		}
		ResetEvent( hEventSD );
	}

	return 0;
}


void CMainWnd::RestartScreenSaver()
{
	KillTimer(TIMER_ID_SCREEN_SAVER);
	if (sysutil::nss_get_instance()->ui_screen_saver_mode != 0	// 0表示关闭 
		&& GetAsyncKeyState(VK_LBUTTON) >= 0
		&& IsForegroundWindow())
	{
		SetTimer(TIMER_ID_SCREEN_SAVER, 
			sysutil::nss_get_instance()->ui_screen_saver_time*1000, NULL);
	}
}

void CMainWnd::OnScreenSaver()
{
	DLG_INFO *pinfo = CDlgManager::GetInstance()->GetCurDlgInfo();
	BOOL bIsVideoDlg = pinfo && pinfo->pdlg->IsKindOf(L"CDvpVideoBaseDlg");
	if (bIsVideoDlg)
	{
		if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_AVIN1 
			&& config::get_config_avin()->avin1_v == 0)	// 当前源是AVIN1,但视频通道没设,则认为是AUX,不认为是视频窗口
		{
			bIsVideoDlg = FALSE;
		}
		else if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_AVIN2
			&& config::get_config_avin()->avin2_v == 0)
		{
			bIsVideoDlg = FALSE;
		}
	}

	if (bIsVideoDlg 		// 视频时不进屏保
		|| (ui_is_on_bt_call() || sysutil::nss_get_instance()->sys_ecar_status)	// 通话时不屏保
		|| sysutil::nss_get_instance()->sys_rds_status		// RDS时不进屏保
		|| sysutil::nss_get_instance()->sys_status != UISS_NORMAL	// 正常状态才进屏保
		|| protocol::get_mcu_status()->is_camera_on())	
	{
		return;
	}

	if (sysutil::nss_get_instance()->ui_screen_saver_mode == 1)		// 黑屏
	{
		// 关屏动作
		ui_close_backlight();
	}
	else if (sysutil::nss_get_instance()->ui_screen_saver_mode == 2)
	{
		CDlgManager::GetInstance()->ShowDlg(CClockDlg_ID, SOURCE_CLOCK);
	}

}

void CMainWnd::ExitScreenSaver()
{
	if (CDlgManager::GetInstance()->GetCurDlgID() == CClockDlg_ID)
	{
		CDlgManager::GetInstance()->GotoLastDlg();
	}
}

void CMainWnd::OnGpsForeground(BOOL bForeground)
{
	if (!tzutil::IsBoolEqual(bForeground, sysutil::nss_get_instance()->sys_gps_foreground))
	{
		sysutil::nss_get_instance()->sys_gps_foreground = bForeground;
		CVolumeManager::UpdateVolume();
	}
}

BOOL CMainWnd::IsForegroundWindow()
{
	return (::GetForegroundWindow() == m_hWnd || (m_bIsTopMostWindow && ::IsWindowVisible(m_hWnd)));
}

SOURCE_ID CMainWnd::GetRearSrcID()
{
	SOURCE_ID sid = CSourceManager::GetInstance()->GetRearSrcID();
	// 如果正在录制而且vdisk是前后排模式,此时认为后排源是vdisk,UI输出vdisk,但无法输出声音
	if (sysutil::nss_get_instance()->vdisk_is_ripping )
	{
		if (sysutil::nss_get_instance()->vdisk_sink == DVP_SINK_FRONT_REAR
			|| sysutil::nss_get_instance()->vdisk_sink == DVP_SINK_REAR)
		{
			sid = SOURCE_VDISK;
		}
		else
		{
			sid = INVALID_SOURCE_ID;
		}
	}

	return sid;
}

void CMainWnd::OnTimerRefreshRearUI()
{
	/* -- shut off rear UI

	static BOOL bInit = FALSE;
	static CWceUiGenericBitmap *m_bmp = NULL;
	SOURCE_ID sid = GetRearSrcID();

	if ( sid != INVALID_SOURCE_ID)	// 有后排源
	{
		if (!bInit)
		{
			bInit = TRUE;
			if (WceUiGetBitsPixel() == 16)
			{
				m_bmp = new CWceUi565Bitmap();
			}
			else
			{
				m_bmp = new CWceUi32Bitmap();
			}
			m_bmp->CreateDIB(WceUiGetScreenWidth(), WceUiGetScreenHeight());
		}

		// 当前窗口不是后排源窗口时，才在计时器中刷新后排UI
		if (CDlgManager::GetInstance()->GetCurDlgInfo()->source_id 	!= sid
			|| !IsForegroundWindow())		
		{
			CRect rc(0,0,800,480);
			BYTE* pbits = NULL;
			HBITMAP hbmp = m_bmp->GetHBitmap(pbits);
			CDC dc;
			dc.CreateCompatibleDC(NULL);

			DLG_INFO *pdi = CDlgManager::GetInstance()->GetActiveDlgInfo(sid);
			if (pdi && !CDlgManager::GetInstance()->IsExternalApp(pdi) && pdi->pdlg)
			{
				GetLayerCtrl()->DrawLayerToBmp(pdi->pdlg, m_bmp, rc);

				HGDIOBJ oldobj = dc.SelectObject(hbmp);
				CUiDDraw::GetInstance()->BitBltRearOverlay(dc.GetSafeHdc(), 0, 0, WceUiGetScreenWidth(), WceUiGetScreenHeight());
				dc.SelectObject(oldobj);
			}
		}
	}

	*/
}


void CMainWnd::OnDlgChange()
{
	// 当窗口有变化时，对后排UI及video的显示情况作处理
	// AVIN视频显示隐藏时,自己会处理,这里不特别处理，这个源也没有多层dlg
	SOURCE_ID sid = GetRearSrcID();
	DLG_INFO* pdi = CDlgManager::GetInstance()->GetActiveDlgInfo(sid);
	if ( sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR)	// 有后排源
	{
		if (pdi && pdi->pdlg && pdi->pdlg->IsKindOf(L"CVideoPlayerDlg"))
		{
			//CMsdkRender::GetInstance()->FShowVideo(TRUE, NULL, MSDK_SINK_REAR);
		}
		else
		{
			//CMsdkRender::GetInstance()->FShowVideo(FALSE, NULL, MSDK_SINK_REAR);
		}

		CMsdkRender::GetInstance()->FShowVideo(TRUE, NULL, MSDK_SINK_REAR);	// 这套APP后排只显示视频不显示UI
	}
	else if (sid == SOURCE_DVD)
	{
		if (pdi && pdi->pdlg && pdi->pdlg->IsKindOf(L"CDvpVideoBaseDlg"))
		{
			//CDvdRender::GetInstance()->ShowVideo(TRUE, NULL, SWI_SINK_REAR);
		}
		else
		{
			//CDvdRender::GetInstance()->ShowVideo(FALSE, NULL, SWI_SINK_REAR);
		}
		CDvdRender::GetInstance()->ShowVideo(TRUE, NULL, SWI_SINK_REAR);
	}

	// 对后排UI进行控制
	// 策略:视频时，不输出UI，其它情况输出UI
	/* shut off rear UI
	if (sid != INVALID_SOURCE_ID)		// 有后排源
	{
		if (pdi && pdi->pdlg 
			&& pdi->pdlg->IsKindOf(L"CDvpVideoBaseDlg")
			&& !pdi->pdlg->IsKindOf(L"CPhotoPlayerDlg") )	// 图片不是真正的视频
		{
			if (CUiDDraw::GetInstance()->IsRearOverlayVisible())
			{
				CUiDDraw::GetInstance()->UpdateRearOverlay(FALSE);
			}
		}
		else
		{
			if (!CUiDDraw::GetInstance()->IsRearOverlayVisible())
			{
				CUiDDraw::GetInstance()->UpdateRearOverlay(TRUE);
			}
		}
	}
	else
	{
		if (CUiDDraw::GetInstance()->IsRearOverlayVisible())
		{
			CUiDDraw::GetInstance()->UpdateRearOverlay(FALSE);
		}
	}
	*/


	// 如果是BT相关窗口,通知MCU
	DLG_ID did = CDlgManager::GetInstance()->GetCurDlgID();
	BOOL bIsInA2DPPage = FALSE;	// 是否在蓝牙界面的A2DP功能页面,当在A2DP时，不认为是在BT界面
	if (did == CBluetoothDlg_ID)
	{
		DLG_INFO *pdi = CDlgManager::GetInstance()->GetCurDlgInfo();
		CBluetoothDlg *pdlg = NULL;
		if (pdi && (pdlg = (CBluetoothDlg*)pdi->pdlg))
		{
			bIsInA2DPPage = (pdlg->GetFunType() == BT_FUN_MUSIC);
		}
	}
	BOOL bActive = ((did == CBluetoothDlg_ID && !bIsInA2DPPage) || /*did == CBTCallDlg_ID*/ui_is_on_bt_call()); // 只要在通话，就认为在BT状态
	if (!tzutil::IsBoolEqual(bActive, protocol::get_mcu_arm_status()->is_bt_active()))	// 如果跟之前状态不一致通知MCU
	{
		protocol::get_mcu_arm_status()->set_bt_active(bActive);
		CRpcMCU::GetInstance()->RPC_SetArmStatus(protocol::get_mcu_arm_status());
	}
}

////////////////////////////////////////////////////////////////////
// _MyCreateTouchScreenWnd();
//
static LRESULT _MyTouchScreenWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL bMouseDown = FALSE;
	if (message == WM_LBUTTONDOWN)
	{
		POINT point;
		point.x = LOWORD(lParam); 
		point.y = HIWORD(lParam);
		if (point.x>=WceUiGetScreenWidth() || point.y>=WceUiGetScreenHeight())
		{
			CMainWnd::GetInstance()->PostMessage(WM_LBUTTONDOWN, 0, lParam);
			bMouseDown = TRUE;
		}
		else
		{
			// 任何操作解mute
			// 任何键都起到解mute作用
			if (sysutil::nss_get_instance()->is_audio_mute())
			{
				CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_KEY_COMMAND, K_MUTE, 0);
			}
		}
	}
	else if (message == WM_LBUTTONUP && bMouseDown)
	{
		CMainWnd::GetInstance()->PostMessage(WM_LBUTTONUP, 0, lParam);
		bMouseDown = FALSE;
	}

	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

static void _MyCreateTouchScreenWnd()
{
	WNDCLASS     wndclass ;

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = _MyTouchScreenWndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = WceUiGetInstanceHandle();
	wndclass.hIcon         = NULL;
	wndclass.hCursor       = 0; 
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = L"main";
	RegisterClass (&wndclass);

	::CreateWindow(L"main", L"TouchScreenWnd", WS_VISIBLE|WS_POPUP, 0, 0, 0, 0, NULL, NULL, WceUiGetInstanceHandle(),0);
}

