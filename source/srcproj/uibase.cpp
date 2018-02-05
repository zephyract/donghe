#include "stdafx.h"
#include "uibase.h"
#include "config.h"
#include "tzUtil.h"
#include "sysUtil.h"
#include "WceUiLoadString.h"
#include "pkfuncs.h"
#include "VolumeManager.h"
#include "protocol.h"
#include "RpcMCU.h"
//#include "IPODManager.h"
#include "SourceManager.h"
#include "ycapi.h"
#include "bluetooth.h"
#include "winhook.h"
#include "phonelink.h"
#include "MainWnd.h"
#include "interface.h"

void ui_set_language(int lan_id)
{
	LANGUAGE_TABLE_INFO* info = config::get_language_info(lan_id);
	if (info)
	{
		TCHAR path[MAX_PATH];
		_stprintf(path, L"%s\\language\\%s.ini", ui_get_res_file(), info->file);
		WceUIChangeLanguage(path, TRUE);
	}
}

void ui_set_timezone(int timezone_id)
{
 	SYSTEMTIME	st ={0};
 	TIME_ZONE_INFORMATION tziNew	= {0};
	GetSystemTime( &st );

	if (timezone_id>=0 && timezone_id<config::get_timezone_table_count())
	{
		TIMEZONE_TABLE_INFO* ptable = config::get_timezone_table();
		if (tzutil::GetTziByRegistry(ptable[timezone_id].name, &tziNew))
		{
			::SetTimeZoneInformation(&tziNew);
		}
		Sleep(50);	// 设了时区后,好像要等底层设置成功后再设时间才有效(系统的实际时间应该是local time)
		SetSystemTime(&st);
	}
}


LPCTSTR ui_get_res_file()
{
	static TCHAR res_file[32] = L"800X480.pack";
	if (WceUiGetScreenWidth() == 1024)
	{
		return L"1024X600.pack";
	}

	return res_file;
}

void ui_get_datetime_string(OUT LPTSTR pdate, OUT LPTSTR ptime)
{
	SYSTEMTIME sttm;
	GetLocalTime( &sttm );

	if ( pdate )
	{
		if (sysutil::nss_get_instance()->ui_lan_id == LAN_CHINESEGB
			|| sysutil::nss_get_instance()->ui_lan_id == LAN_CHINESEBIG5)
		{
			wsprintf( pdate,L"%d年%d月%d日",sttm.wYear,sttm.wMonth,sttm.wDay );
		}
		else if (sysutil::nss_get_instance()->ui_lan_id == LAN_ENGLISH)
		{
			LPCTSTR month[] = {L"January", L"February", L"March", L"April", L"May", L"June", L"July", 
				L"August", L"September", L"October", L"November", L"December"};

			wsprintf( pdate,L"%02d %s %04d",sttm.wDay,month[sttm.wMonth-1],sttm.wYear);
		}
		else
		{
			wsprintf( pdate,L"%02d-%02d-%04d",sttm.wDay,sttm.wMonth,sttm.wYear);
		}
	}

	if( ptime )
	{
		if ( sysutil::nss_get_instance()->ui_24hourtime )
		{
// 			wsprintf( ptime,L"%02d:%02d:%02d",sttm.wHour,sttm.wMinute,sttm.wSecond );
			wsprintf( ptime,L"%02d:%02d",sttm.wHour,sttm.wMinute );
		}
		else
		{
			CWceUiLoadString strAM(L"MAIN_AM"); 
			CWceUiLoadString strPM(L"MAIN_PM");
// 			if ( sttm.wHour==0 )
// 				wsprintf( ptime,L"%02d:%02d:%02d %s",12,sttm.wMinute,sttm.wSecond, strAM.GetString());
// 			else if ( sttm.wHour==12 )
// 				wsprintf( ptime,L"%02d:%02d:%02d %s",sttm.wHour,sttm.wMinute,sttm.wSecond, strPM.GetString());
// 
// 			else if ( sttm.wHour>12 )
// 				wsprintf( ptime,L"%02d:%02d:%02d %s",sttm.wHour-12,sttm.wMinute,sttm.wSecond, strPM.GetString() );
// 			else
// 				wsprintf( ptime,L"%02d:%02d:%02d %s",sttm.wHour,sttm.wMinute,sttm.wSecond, strAM.GetString());

			if ( sttm.wHour==0 )
				wsprintf( ptime,L"%02d:%02d %s",12,sttm.wMinute, strAM.GetString());
			else if ( sttm.wHour==12 )
				wsprintf( ptime,L"%02d:%02d %s",sttm.wHour,sttm.wMinute, strPM.GetString());

			else if ( sttm.wHour>12 )
				wsprintf( ptime,L"%02d:%02d %s",sttm.wHour-12,sttm.wMinute, strPM.GetString() );
			else
				wsprintf( ptime,L"%02d:%02d %s",sttm.wHour,sttm.wMinute, strAM.GetString());
		}
	}
}


static int  _get_country_code_len(LPTSTR lpszNumber)
{
	LPCTSTR country_code[] = {L"60", L"62", L"63", L"65", L"66" L"673",
		L"81", L"82", L"84", L"850", L"852", L"853", L"855", L"856", L"86",L"886", L"880",
	L"90",L"91",L"92",L"93",L"94",L"95",L"960",L"961",L"962",L"963",L"964",L"965",
	L"7",L"30",L"31",L"32",L"33",L"34",L"351",L"49",L"39",L"41",L"44",
	L"20",L"212",L"213",L"216",L"218",
	L"1",L"1808",L"51",L"52",L"53",L"55",L"61",L"64",};
	if (lpszNumber && lpszNumber[0] == L'+')
	{
		for (int i=0; i<sizeof(country_code)/sizeof(country_code[0]); i++)
		{
			TCHAR *psub = _tcsstr(lpszNumber+1, country_code[i]);
			if (psub == lpszNumber+1)
			{
				return _tcslen(country_code[i]) + 1;
			}
		}
	}

	return 0;
}

void ui_format_tel_number(IN OUT LPTSTR lpszNumber)
{
	if (lpszNumber && sysutil::nss_get_instance()->ui_lan_id == LAN_CHINESEGB)	// 只有中文时才格式化
	{
		CString strOriginal = lpszNumber;
		if (lpszNumber[0] == L'0' && _tcslen(lpszNumber)>=3)	// 固话,区号0开头
		{
			int zero_code_len = 4;	// 大多数为四位
			if ((lpszNumber[1] == L'1' && lpszNumber[2] == L'0')		// 010
				|| lpszNumber[1] == L'2')								// 02X
			{
				// 区号为3位
				zero_code_len = 3;
			}
			if (strOriginal.GetLength() > zero_code_len 
				&& (strOriginal.GetLength()-zero_code_len <= 8))
			{
				_stprintf(lpszNumber, L"%s-%s", strOriginal.Mid(0, zero_code_len), 
					strOriginal.Mid(zero_code_len, strOriginal.GetLength()-zero_code_len));
			}
		}
		// 手机号码, 9到11位时才格式化
		else if (_tcslen(lpszNumber) >= 9	&& _tcslen(lpszNumber) <= 11 &&
			(_tcsncmp(lpszNumber, L"13", 2) == 0 || _tcsncmp(lpszNumber, L"15", 2) == 0 || _tcsncmp(lpszNumber, L"18", 2) == 0  ))
		{
			_stprintf(lpszNumber, L"%s-%s-%s", strOriginal.Mid(0, 3), strOriginal.Mid(3, 4), 
				strOriginal.Mid(7, strOriginal.GetLength()-7));
		}
		else if (lpszNumber[0] == L'+')		// 国家区号
		{
			int country_code_len = _get_country_code_len(lpszNumber);
			TCHAR country_code[8];
			if (country_code_len > 0 && country_code_len < 8)
			{
				_tcsncpy(country_code, lpszNumber, country_code_len);
				country_code[country_code_len] = L'\0';
				ui_format_tel_number(lpszNumber + country_code_len);
				TCHAR buffer[128];
				_stprintf(buffer, L"%s %s", country_code, lpszNumber + country_code_len);
				_tcscpy(lpszNumber, buffer);
			}

		}
		else
		{
			// 其它不格式化
		}
	}
}

static DWORD ThreadTouchCalibrate( LPVOID lpParameter )
{
	typedef BOOL (WINAPI *_TouchCalibrate)();
	_TouchCalibrate fTouchCalibrate = NULL;

	HINSTANCE  hinstance = LoadLibrary(_T("coredll.dll")); 
	if  (hinstance == NULL)  
	{  
		return 0; 
	}

	fTouchCalibrate = (_TouchCalibrate)GetProcAddress(hinstance , L"TouchCalibrate"); 
	if (fTouchCalibrate == NULL)  
	{  
		return 0; 
	}

	// 在校屏状态,有些响应需特别处理,比如来电或倒车了需退出校屏,所以这里要记录状态
	// 在校屏状态也不响应按键了,只响应关机键
	sysutil::nss_get_instance()->sys_calibrate_status = 1;
	fTouchCalibrate(); 
	sysutil::nss_get_instance()->sys_calibrate_status = 0;
	FreeLibrary(hinstance );
	return 1;
}

void ui_touch_calibrate()
{
	HANDLE hThread = ::CreateThread(0,0,ThreadTouchCalibrate,NULL,0,NULL);
	CloseHandle( hThread );
}

void ui_exit_touch_calibrate()
{
	HANDLE	hEvent =  CreateEvent(NULL, FALSE, FALSE, L"ESC_CALIBRATE_TOUCH_EVENT");
	SetEventData(hEvent,0x0c);
	SetEvent(hEvent);
	CloseHandle(hEvent);
}

static BOOL _show_navigation()
{
	_tcslwr(sysutil::nss_get_instance()->gps_path);

	if (_tcsstr(sysutil::nss_get_instance()->gps_path, L"primo.exe") != NULL)	// IGO地图
	{
		HWND hwndNav = tzutil::GetProcessMainWnd(sysutil::nss_get_instance()->gps_process_id);
		if (hwndNav)
		{
			::ShowWindow(hwndNav, SW_SHOW);
			::SetForegroundWindow(hwndNav);
			return TRUE;
		}
	} 
	else
	{
		PROCESS_INFORMATION pi;
		if (tzutil::LoadApp(sysutil::nss_get_instance()->gps_path, L"", &pi))
		{
			CloseHandle( pi.hProcess );
			CloseHandle( pi.hThread );
		}
	}

	return TRUE;
}

static BOOL _load_navigation()
{
	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	if (tzutil::LoadApp(sysutil::nss_get_instance()->gps_path, L"", &pi))
	{
		sysutil::nss_get_instance()->gps_process_handle = pi.hProcess;
		sysutil::nss_get_instance()->gps_process_id = pi.dwProcessId;
		CloseHandle(pi.hThread);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL ui_is_navigation_run()
{
	return (sysutil::nss_get_instance()->gps_process_handle != NULL
		&& WaitForSingleObject(sysutil::nss_get_instance()->gps_process_handle, 1) == WAIT_TIMEOUT);
}

BOOL ui_terminate_navigation()
{
	if (ui_is_navigation_run())
	{
		if( TerminateProcess(sysutil::nss_get_instance()->gps_process_handle, 0))
		{
			CloseHandle(sysutil::nss_get_instance()->gps_process_handle);
			sysutil::nss_get_instance()->gps_process_handle = NULL;
			sysutil::nss_get_instance()->gps_process_id = 0;
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}


BOOL ui_load_navigation()
{
	// 导航如果已经运行,弹出导航就可以了
	if (ui_is_navigation_run())
	{
		return _show_navigation();
	}
	else	// 启动导航
	{
		if (sysutil::nss_get_instance()->gps_process_handle != NULL)
		{
			CloseHandle(sysutil::nss_get_instance()->gps_process_handle);
			sysutil::nss_get_instance()->gps_process_handle = NULL;
		}

		CVolumeManager::SetGPSProcessID(0);
		 if(_load_navigation())
		 {
			 CVolumeManager::SetGPSProcessID(sysutil::nss_get_instance()->gps_process_id);
			 // 设置是否允许导航修改系统音量
			 CVolumeManager::EnableGPSSetDeviceGain(sysutil::nss_get_instance()->audio_is_gps_set_gain);
			 return TRUE;
		 }
	}

	return FALSE;
}


void ui_mcu_beep(int mode)
{
	if (sysutil::nss_get_instance()->audio_key_beep)
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_BEEP, (UINT)mode);
	}
}


//////////////////////////////////////////////////////////////////////////

BOOL NAVI_SYSTEM_STATUS::is_audio_mute()
{
	// MUTE管理前后排媒体音量,GPS音量,BT音量不受MUTE控制
	return audio_is_media_front_mute;

}

void NAVI_SYSTEM_STATUS::set_audio_mute(BOOL is_mute)
{
	audio_is_media_front_mute = is_mute;
	audio_is_media_rear_mute = is_mute;
	audio_is_gps_mute = is_mute;
}

BOOL NAVI_SYSTEM_STATUS::is_source_available_hw(SOURCE_ID sid)
{
	if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_VDISK)
	{
		DWORD  status = YC_GetDiskAttach();
		LPCTSTR disk = ui_msdk_get_disk(sid);
		if (_tcsicmp(disk, L"sdmmc") == 0)
		{
			return (status>>4) & 0x01;
		}
		else if (_tcsicmp(disk, L"storage card") == 0)
		{
			return (status>>3) & 0x01;
		}
		else if (_tcsicmp(disk, L"ExternCard") == 0)
		{
			return (status>>2) & 0x01;
		}
		else if (_tcsicmp(disk, L"USBDisk") == 0)
		{
			return (status>>1) & 0x01;
		}
		else if (_tcsicmp(disk, L"USBDisk1") == 0)
		{
			return (status>>0) & 0x01;
		}
	}

	return TRUE;	// 其它源通过这个条件无法提前检测,都认为存在
}

BOOL NAVI_SYSTEM_STATUS::is_source_available(SOURCE_ID sid)
{
	// 如果不是配置文件中配置的可用源,直接返回FALSE
	if (!config::get_config_functions()->is_support(sid))
	{
		return FALSE;
	}

	if (sid == SOURCE_DVD)
	{
		return protocol::get_mcu_status()->is_DISK_exist();
	}
	/* --++ ipod
	else if (sid == SOURCE_IPOD_USB)
	{
		return CIPODManager::GetInstance()->IsConnect();
	}
	*/
	else if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_VDISK)
	{
		return (GetFileAttributes(ui_msdk_get_disk(sid)) != (DWORD)-1);
	}
	else if (sid == SOURCE_BT)
	{
		return CBluetooth::GetInstance()->IsConnected();
	}
	else if (sid == SOURCE_PHONE_LINK)
	{
		// 只要发起连接了,就认为源可用
		#pragma message( __LOC__" PHONELINK source available judgement Only applied on Donghe's version." )

		// USB插着时,不允许进phonelink
		if ((GetFileAttributes(ui_msdk_get_disk(SOURCE_USB)) != (DWORD)-1))
		{
			return FALSE;
		}

		//CPhoneLink::GetInstance()->IsConnected() || CPhoneLink::GetInstance()->IsConnecting();
	}
	else if (sid == SOURCE_EASY_CONNECTED)
	{
		//CPhoneLink::GetInstance()->IsConnected() || CPhoneLink::GetInstance()->IsConnecting();
		return TRUE;
	}

	return TRUE;
}

void NAVI_SYSTEM_STATUS::reset_video_params(int mode)
{
	if (mode == 0)
	{
		sysutil::nss_get_instance()->video_brightness = config::get_config_video()->standard_brightness;
		sysutil::nss_get_instance()->video_contrast = config::get_config_video()->standard_contrast;
		sysutil::nss_get_instance()->video_hue = config::get_config_video()->standard_hue;
		sysutil::nss_get_instance()->video_saturation = config::get_config_video()->standard_saturation;
	}
	else if(mode == 1)
	{
		sysutil::nss_get_instance()->video_brightness = config::get_config_video()->colorful_brightness;
		sysutil::nss_get_instance()->video_contrast = config::get_config_video()->colorful_contrast;
		sysutil::nss_get_instance()->video_hue = config::get_config_video()->colorful_hue;
		sysutil::nss_get_instance()->video_saturation = config::get_config_video()->colorful_saturation;
	}
	else
	{
		sysutil::nss_get_instance()->video_brightness = config::get_config_video()->soft_brightness;
		sysutil::nss_get_instance()->video_contrast = config::get_config_video()->soft_contrast;
		sysutil::nss_get_instance()->video_hue = config::get_config_video()->soft_hue;
		sysutil::nss_get_instance()->video_saturation = config::get_config_video()->soft_saturation;
	}
}

MSDK_MEMORY* ui_msdk_get_memory(SOURCE_ID sid)
{
	MSDK_MEMORY* pmemory = NULL;
	if (sid == SOURCE_SD)
	{
		pmemory = &sysutil::nss_get_instance()->sd_memory;
	}
	else if (sid == SOURCE_USB)
	{
		pmemory = &sysutil::nss_get_instance()->usb1_memory;
	}
	else if (sid == SOURCE_USB_REAR)
	{
		pmemory = &sysutil::nss_get_instance()->usb2_memory;
	}
	return pmemory;
}

LPCTSTR ui_msdk_get_disk(SOURCE_ID sid)
{
	LPCTSTR pdisk = NULL;
	if (sid == SOURCE_SD)
	{
		pdisk = config::get_config_disk()->sd;
	}
	else if (sid == SOURCE_USB)
	{
		pdisk = config::get_config_disk()->usb1;
	}
	else if (sid == SOURCE_USB_REAR)
	{
		pdisk = config::get_config_disk()->usb2;
	}
	else if (sid == SOURCE_VDISK)
	{
		pdisk = config::get_config_disk()->vdisk;
	}
	return pdisk;
}

SOURCE_ID ui_msdk_get_cursrc()
{
	if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_SD 
		|| CSourceManager::GetInstance()->GetLastRearSrcID() == SOURCE_SD)
	{
		return SOURCE_SD;
	}
	else if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_USB 
		|| CSourceManager::GetInstance()->GetLastRearSrcID() == SOURCE_USB)
	{
		return SOURCE_USB;
	}
	else if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_USB_REAR 
		|| CSourceManager::GetInstance()->GetLastRearSrcID() == SOURCE_USB_REAR)
	{
		return SOURCE_USB_REAR;
	}

	return INVALID_SOURCE_ID;
}

void ui_update_backlight()
{
	if (protocol::get_mcu_status()->is_light_on() && sysutil::nss_get_instance()->sys_light_check)
	{
		if(sysutil::nss_get_instance()->video_backlight_mode == UIBS_MID)
		{
			YC_SetBackLightLevel(min(45, sysutil::nss_get_instance()->video_backlight_night));
		}
		else
		{
			YC_SetBackLightLevel(sysutil::nss_get_instance()->video_backlight_night);
		}
	}
	else
	{
		if(sysutil::nss_get_instance()->video_backlight_mode == UIBS_MID)
		{
			YC_SetBackLightLevel(min(55,sysutil::nss_get_instance()->video_backlight_normal));
		}
		else
		{
			YC_SetBackLightLevel(sysutil::nss_get_instance()->video_backlight_normal);
		}
	}
}



// 将路径格式化，如果太长了,将路径的中间目录用...代替,尽量保留盘符及最后的目录
void ui_format_file_path(HFONT hfont, const RECT* prc, LPCTSTR lpszPath, OUT LPTSTR pResult)
{
	if (hfont == NULL || !prc || !lpszPath || !pResult)
	{
		return;
	}

	CDC dc;
	dc.CreateCompatibleDC(NULL);
	dc.SelectObject(hfont);

	SIZE sizeSpace;
	GetTextExtentPoint32(dc.GetSafeHdc(), lpszPath, _tcslen(lpszPath), &sizeSpace);

	// 没有超出边界
	if (sizeSpace.cx <= (prc->right-prc->left))
	{
		_tcscpy(pResult, lpszPath);
		return;
	}


	CStringArray afile;
	tzutil::GetSubString(lpszPath, afile, L'\\');
	CString temp;
	while (afile.GetCount() >= 2)	// 表示有目录名
	{
		// 如果只剩盘符和文件名,则替换盘符,否则替换前面的其它目录
		int index_delete = (afile.GetCount() == 2) ? 0 : 1;
		afile.GetAt(index_delete) = L"...";
		temp.Empty();
		for (int i=0; i<afile.GetCount(); i++)
		{
			temp += afile.GetAt(i);
			if (i != afile.GetCount()-1)
			{
				temp += L'\\';
			}
		}

		GetTextExtentPoint32(dc.GetSafeHdc(), temp, _tcslen(temp), &sizeSpace);
		if (sizeSpace.cx <= (prc->right-prc->left))
		{
			_tcscpy(pResult, temp);
			return;
		}

		// 仍旧超出边界了
		afile.RemoveAt(index_delete);
	}

	// 文件名太长了，直接返回文件名
	_tcscpy(pResult, lpszPath);

}



BOOL ui_is_on_bt_call()
{
	return (sysutil::nss_get_instance()->sys_bt_call_status == CALLMANAGER_CALLSTATE_INCOMING
		|| sysutil::nss_get_instance()->sys_bt_call_status == CALLMANAGER_CALLSTATE_DIALING
		|| sysutil::nss_get_instance()->sys_bt_call_status == CALLMANAGER_CALLSTATE_SPEAKING);
}

SOURCE_ID ui_get_next_sid(SOURCE_ID sid)
{
	int next_sid = config::get_config_functions()->next_sid(sid);
	if (next_sid == INVALID_SOURCE_ID)	// 如果没有获取配置文件中定义的源顺序,则用缺省顺序
	{
		next_sid = sid;
		next_sid++;
		if (next_sid == SOURCE_MAX_ID)	// 最大源定义不超过25
		{
			next_sid = 1;
		}
	}

	return (SOURCE_ID)next_sid;
}


void ui_close_backlight()
{
	// 关屏动作
	sysutil::nss_get_instance()->sys_status = UISS_BACKLIGHT_OFF;
	winhook::DisableTouchPanel();
	YC_BackLightEnable(FALSE);
}

void ui_play_game_sound(LPCTSTR lpszFile)
{
	TCHAR szPath[MAX_PATH];
	_stprintf(szPath, L"%s\\game\\%s", tzutil::GetAppPath(), lpszFile);
	sndPlaySound(szPath, SND_ASYNC);
}

void ui_set_foreground_window(HWND hwnd)
{
	::SetForegroundWindow(hwnd);
}

//////////////////////////////////////////////////////////////////
// 触摸按键处理
//


// 如果不想音量条可屏幕操作,该函数直接返回FALSE就可以了
static BOOL Touchkey_Hook_CB(DWORD dwUserData, WPARAM wParam, LPARAM lParam)
{
	static BOOL bHookMouse = FALSE;
	static CRect rc;


	POINT pt;
	pt.x = LOWORD(lParam); 
	pt.y = HIWORD(lParam);

	if (wParam == WM_LBUTTONDOWN && pt.x>=800)
	{
		for (int i=0; i<config::get_config_touchkey()->count; i++)
		{
			if (::PtInRect(config::get_config_touchkey()->get_rect(i), pt))
			{
				CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_KEY_COMMAND, 
					config::get_config_touchkey()->get_key(i), 0);
				
				return TRUE;
			}
		}
	}


	return FALSE;
}

void ui_enable_touch_key(BOOL enable)
{
	if (enable)
	{
		winhook::HookTouchKey(Touchkey_Hook_CB, (DWORD)0);
	}
	else
	{
		winhook::UnHookTouchKey();
	}
}



static ULARGE_INTEGER _result;
ULARGE_INTEGER& _u64plus(ULARGE_INTEGER& i1, ULARGE_INTEGER& i2)
{
	_result.LowPart = i1.LowPart + i2.LowPart;
	_result.HighPart = i1.HighPart + i2.HighPart;
	if (_result.LowPart < i1.LowPart)
	{
		_result.HighPart++;
	}
	return _result;
}

ULARGE_INTEGER& _u64minus(ULARGE_INTEGER& i1, ULARGE_INTEGER& i2)
{
	if (i1.LowPart >= i2.LowPart)
	{
		_result.LowPart = i1.LowPart - i2.LowPart;
		_result.HighPart = i1.HighPart - i2.HighPart;
	}
	else
	{
		_result.LowPart = MAXDWORD - i2.LowPart + i1.LowPart + 1;
		_result.HighPart = i1.HighPart - i2.HighPart - 1;
	}

	return _result;
}

void ui_set_systemtime_DST(__in CONST SYSTEMTIME *lpSystemTime, BOOL bDST)
{
	FILETIME ft;
	SystemTimeToFileTime(lpSystemTime, &ft);
	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;

	ULARGE_INTEGER one_hour;
	one_hour.HighPart = 8;
	one_hour.LowPart = 1640261632; // 10*1000*1000*3600;
	if (bDST)
	{
		_u64plus(ui, one_hour);
	}
	else
	{
		_u64minus(ui, one_hour);
	}

	ft.dwLowDateTime = _result.LowPart;
	ft.dwHighDateTime = _result.HighPart;
	SYSTEMTIME tm;
	FileTimeToSystemTime(&ft, &tm);
	SetSystemTime(&tm);
}

void ui_time_calculate(IN OUT SYSTEMTIME *lpSystemTime, int hours, int minutes, int seconds)
{
	if (!lpSystemTime)
	{
		return;
	}

	FILETIME ft;
	SystemTimeToFileTime(lpSystemTime, &ft);
	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;

	ULARGE_INTEGER one_hour;
	one_hour.HighPart = 8;
	one_hour.LowPart = 1640261632; // 10*1000*1000*3600;

	ULARGE_INTEGER one_min;
	one_min.HighPart = 0;
	one_min.LowPart = 600000000; // 10*1000*1000*3600;

	ULARGE_INTEGER one_second;
	one_second.HighPart = 0;
	one_second.LowPart = 10000000; // 10*1000*1000*1;

	while (hours != 0)
	{
		if (hours>0)
		{
			_u64plus(ui, one_hour);
			ui.LowPart = _result.LowPart;
			ui.HighPart = _result.HighPart;
			hours--;
		}
		else
		{
			_u64minus(ui, one_hour);
			ui.LowPart = _result.LowPart;
			ui.HighPart = _result.HighPart;
			hours++;
		}
	}

	while (minutes != 0)
	{
		if (minutes>0)
		{
			_u64plus(ui, one_min);
			ui.LowPart = _result.LowPart;
			ui.HighPart = _result.HighPart;
			minutes--;
		}
		else
		{
			_u64minus(ui, one_min);
			ui.LowPart = _result.LowPart;
			ui.HighPart = _result.HighPart;
			minutes++;
		}
	}

	while (seconds != 0)
	{
		if (seconds>0)
		{
			_u64plus(ui, one_second);
			ui.LowPart = _result.LowPart;
			ui.HighPart = _result.HighPart;
			seconds--;
		}
		else
		{
			_u64minus(ui, one_second);
			ui.LowPart = _result.LowPart;
			ui.HighPart = _result.HighPart;
			seconds++;
		}
	}


	ft.dwLowDateTime = ui.LowPart;
	ft.dwHighDateTime = ui.HighPart;
	FileTimeToSystemTime(&ft, lpSystemTime);
}


////////////////////////////////////////////////////////////////////////
static BOOL s_led_is_volume_show = FALSE;
static BOOL s_led_is_btcall_show = FALSE;
static UINT s_led_btcall_status = 0;
static BYTE s_led_save_data[16];	// 保留显示音量前或通话前的显示数据,如果显示过程中有数据更新,也是更新这个值
BYTE* ui_led_get_data()
{
	static BYTE led_data[16];
	return led_data;
}

void ui_led_set_playmode(BOOL random, BOOL repeat)
{
	BYTE* data = ui_led_get_data();
	random ? (data[15] |= 0x40) : (data[15] &= ~0x40);
	repeat ? (data[15] |= 0x20) : (data[15] &= ~0x20);
}

static void _led_char2byte_s(TCHAR ch, OUT BYTE* data)
{
	const char table2[][2] = {
		'0',0xBD,
		'1',0x05,
		'2',0xDB,
		'3',0x5F,
		'4',0x67,
		'5',0x7E,
		'6',0xFE,
		'7',0x15,
		'8',0xFF,
		'9',0x7F
	};

	if (!data)
	{
		return;
	}

	if (ch>=L'0' && ch<=L'9')
	{
		data[0] = (BYTE)table2[ch-L'0'][1];
	}
	else
	{
		data[0] = 0;
	}
}

static void _led_char2byte(TCHAR ch, OUT BYTE* data)
{
	const char table1[][3] = {
		'0',0xC0,0x96,
		'1',0x05,0x00,
		'2',0x82,0x92,
		'3',0x02,0x96,
		'4',0x42,0x06,
		'5',0x42,0x94,
		'6',0xC2,0x94,
		'7',0x00,0x16,
		'8',0xC2,0x96,
		'9',0x42,0x96,
		'A',0xC2,0X16,
		'B',0x07,0x96,
		'C',0xC0,0x90,
		'D',0x05,0x96,
		'E',0xC2,0x90,
		'F',0xC2,0x10,
		'G',0xC2,0x94,
		'H',0xC2,0x06,
		'I',0x05,0x90,
		'J',0x89,0x10,
		'K',0x05,0x60,
		'L',0xC0,0x80,
		'M',0xE0,0x26,
		'N',0xE0,0x46,
		'O',0xC0,0x96,
		'P',0xC2,0x12,
		'Q',0xC0,0xD6,
		'R',0xC2,0x52,
		'S',0x42,0x94,
		'T',0x05,0x10,
		'U',0xC0,0x86,
		'V',0x20,0x46,
		'W',0xC8,0x46,
		'X',0x28,0x60,
		'Y',0x42,0x86,
		'Z',0x08,0xB0 
	};

	if (!data)
	{
		return;
	}

	if (ch>=L'0' && ch<=L'9')
	{
		data[0] = (BYTE)table1[ch-L'0'][1];
		data[1] = (BYTE)table1[ch-L'0'][2];
	}
	else if (ch>=L'A' && ch<=L'Z')
	{
		data[0] = (BYTE)table1[ch-L'A'+10][1];
		data[1] = (BYTE)table1[ch-L'A'+10][2];
	}
	else
	{
		data[0] = 0;
		data[1] = 0;
	}
}

void ui_led_set_showdata(LPCTSTR toShow, BOOL isTempData)
{
	if (!toShow || _tcslen(toShow) != 9)
	{
		return;
	}

	BYTE* data = NULL;
	if ((s_led_is_volume_show||s_led_is_btcall_show) && !isTempData)	// 如果在显示音量,把数据保存起来
	{
		data = s_led_save_data;
	}
	else
	{
		data = ui_led_get_data();
	}

	// 状态先记下来,写完数据后恢复
	BOOL dvd = data[13] & 0x08;
	BOOL sd = data[7] & 0x08;
	BOOL usb = data[11] & 0x08;
	BOOL bt = data[6] & 0x10;


	// 0~4普通字符
	for (int i=0; i<=4; i++)	
	{
		_led_char2byte(toShow[i], &data[i*2]);
	}

	// 5 是 . or :
	if (toShow[5] == L'.')
	{
		data[9] |= 0x08;
	}
	else if (toShow[5] == L':')
	{
		data[9] |= 0x01;
		data[9] |= 0x08;
	}

	// 6~7普通字符
	for (int i=6; i<=7; i++)	
	{
		_led_char2byte(toShow[i], &data[(i-1)*2]);
	}

	// 8是数字
	_led_char2byte_s(toShow[8], &data[14]);

	// 恢复状态
	ui_led_set_source_available(data, sd, usb, dvd, bt);
}

LPCTSTR ui_led_fm_data(int band, LPCTSTR freq, int preset)
{
	LPCTSTR table[] = {L"F1", L"F2", L"F3", L"A1", L"A2"};
	static TCHAR data[10];
	for (int i=0; i<9; i++)
	{
		data[i] = L' ';
	}

	if (band>=0 && band<=4)
	{
		data[0] = table[band][0];
		data[1] = table[band][1];
	}

	if (band>=0 && band<=2)	// FM
	{
		LPCTSTR pdotpos = _tcschr(freq, L'.');
		int dot_pos = -1;
		if (pdotpos)
		{
			dot_pos = pdotpos - freq;
		}
		if (dot_pos>=0 && _tcslen(freq)<=6)	// 数据必须合法
		{
			// . 的位置必须与data中.的位置对齐(pos为5)
			for (int i=0; i<_tcslen(freq); i++)
			{
				data[5-dot_pos+i] = freq[i];
			}
		}
	}
	else if (band>=3 && band<=4)	// AM
	{
		if (_tcslen(freq) <= 4)
		{
			for (int i=0; i<_tcslen(freq); i++)
			{
				if (i<2)
				{
					data[3+i] = freq[i];
				}
				else
				{
					data[4+i] = freq[i];	// 跳过:号
				}
			}
		}
	}

	if (preset >=1 && preset<=6)
	{
		const TCHAR table_d[] = {L'1', L'2', L'3', L'4', L'5', L'6' };
		data[8] = table_d[preset-1];
	}

	return data;
}

void ui_led_set_source_available(BYTE* data, BOOL sd, BOOL usb, BOOL dvd, BOOL bt)
{
	if (data == NULL) data = ui_led_get_data();
	usb ? data[11] |= 0x08 :  data[11] &= ~0x08;
	dvd ? data[13] |= 0x08 :  data[13] &= ~0x08;
	bt ? data[6] |= 0x10 :  data[6] &= ~0x10;
	sd ? data[7] |= 0x08 :  data[7] &= ~0x08;
}

LPCTSTR ui_led_media_data(int index, int duration)
{
	static TCHAR data[10];
	for (int i=0; i<9; i++)
	{
		data[i] = L' ';
	}

	int min = duration / 60;
	int sec = duration % 60;

	// 如果超出100,高位会显示不出来
	min = min % 100;
	index = index % 100;
	_stprintf(data, L"%02d %02d:%02d ", index, min, sec);
	return data;
}

void ui_led_show_volume(int vol)
{
	TCHAR data[10];
	if (!s_led_is_volume_show && !s_led_is_btcall_show)
	{
		memcpy(s_led_save_data, ui_led_get_data(), sizeof(s_led_save_data));	//  保存之前的数据
	}
	s_led_is_volume_show = TRUE;

	if (vol < 0)	// mute
	{
		ui_led_set_showdata(L"  MUT E  ", TRUE);
	}
	else if (vol>=0 && vol<=9)
	{
		_stprintf(data, L" VOL  %d  ", vol);
		ui_led_set_showdata(data, TRUE);
	}
	else
	{
		_stprintf(data, L" VOL  %d ", vol);
		ui_led_set_showdata(data, TRUE);
	}

}

static void _led_restore_data()
{
	// 状态先记下来,写完数据后恢复(因为状态是最新的)
	BYTE *data = ui_led_get_data();
	BOOL dvd = data[13] & 0x08;
	BOOL sd = data[7] & 0x08;
	BOOL usb = data[11] & 0x08;
	BOOL bt = data[6] & 0x10;
	memcpy(ui_led_get_data(), s_led_save_data, sizeof(s_led_save_data));	//  保存之前的数据

	// 恢复状态
	ui_led_set_source_available(ui_led_get_data(), sd, usb, dvd, bt);
}

void ui_led_hide_volume()
{
	s_led_is_volume_show = FALSE;

	if (s_led_is_btcall_show)
	{
		ui_led_show_btcall(s_led_btcall_status);
	}
	else
	{
		_led_restore_data();
	}
}

void ui_led_show_btcall(UINT nStatus)
{
	s_led_btcall_status = nStatus;

	if (!s_led_is_volume_show && !s_led_is_btcall_show)
	{
		memcpy(s_led_save_data, ui_led_get_data(), sizeof(s_led_save_data));	//  保存之前的数据
	}
	s_led_is_btcall_show = TRUE;

	if (!s_led_is_volume_show)	// 如果音量在显示,啥也不做
	{
		if (nStatus ==  BZ_IND_CALL_INCOMING)	// mute
		{
			ui_led_set_showdata(L"CALLI N  ", TRUE);
		}
		else if (nStatus == BZ_IND_CALL_OUTGOING)
		{
			ui_led_set_showdata(L"CALLO UT ", TRUE);
		}
	}
}

void ui_led_hide_btcall()
{
	s_led_is_btcall_show = FALSE;

	if (!s_led_is_volume_show)	// 如果音量在显示,啥也不做
	{
		_led_restore_data();
	}
}
