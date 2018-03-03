#include "stdafx.h"
#include "sysUtil.h"
#include "tzUtil.h"
#include "config.h"
#include "MainWnd.h"
#include "interface.h"
#include "DVP_Agent.h"
#include "VolumeManager.h"
#include "SettingsAudio.h"
#include "SetVideoParams.h"
#include "bluetooth.h"
#include "config.h"

#include <WinIoCtl.h>
#include <storemgr.h>
#include <diskio.h>

NAVI_SYSTEM_STATUS* sysutil::nss_get_instance()
{
	static NAVI_SYSTEM_STATUS nss;
	return &nss;
}

static void _nss_init_by_config()
{
	NAVI_SYSTEM_STATUS* pnss = sysutil::nss_get_instance();

	// ʹ�ð��еĵ�һ�ű���
	pnss->ui_bk_index = 0;

	// ʹ�������ļ���ָ����ȱʡ����
	pnss->ui_lan_id = config::get_language_default();

	// ʹ�������ļ���ָ����ȱʡʱ��
	pnss->ui_timezone_id = config::get_timezone_default();

	// ȱʡ���ʱ���Զ����ã�ʹ��12Сʱ��
	pnss->ui_time_autoset = TRUE;
	pnss->ui_24hourtime   = config::get_config_misc()->hourtime24;
	pnss->ui_time_dst   = FALSE;

	pnss->ui_clock_mode = 0;
	pnss->ui_capture_screen = 0;
	pnss->ui_rds_pty = 0;

	// mcu comm
	pnss->sys_mcu_port = config::get_config_uart()->mcu_commport;
	pnss->sys_mcu_baudrate = config::get_config_uart()->mcu_baudrate;

	// audio
	// �����ļ��в�������mute�����ѡ��,��Щ����Ҳ���ó�ʼ��,ֱ��ʹ��ȱʡֵ0
	pnss->audio_media_front_volume = config::get_config_audio()->media_front_volume;
	pnss->audio_media_rear_volume = config::get_config_audio()->meida_rear_volume;
	pnss->audio_init_media_max_volume = config::get_config_audio()->init_max_volume;
	pnss->audio_bt_volume = config::get_config_audio()->bt_volume;
	pnss->audio_ta_volume = config::get_config_audio()->ta_volume;
	pnss->audio_gps_volume = config::get_config_audio()->gps_volume;
	pnss->audio_gps_gain = config::get_config_audio()->gps_gain;
	pnss->audio_is_gps_gain_off = config::get_config_audio()->gps_gain_off;
	pnss->audio_is_gps_set_gain = config::get_config_audio()->gps_set_gain_enable;
	pnss->audio_key_beep = config::get_config_audio()->key_beep;
	pnss->audio_eq_subwoofer = config::get_config_audio()->subwoofer;
	pnss->audio_eq_loudness = 10;
	pnss->audio_eq_loudness_gain = 0;	// Ĭ�����loudΪ��

	// video
	pnss->video_mode = 0;
	pnss->video_brightness = config::get_config_video()->standard_brightness;
	pnss->video_contrast = config::get_config_video()->standard_contrast;
	pnss->video_hue = config::get_config_video()->standard_hue;
	pnss->video_saturation = config::get_config_video()->standard_saturation;

	pnss->video_aux_brightness = config::get_config_video()->aux_brightness;
	pnss->video_aux_contrast = config::get_config_video()->aux_contrast;
	pnss->video_aux_hue = config::get_config_video()->aux_hue;
	pnss->video_aux_saturation = config::get_config_video()->aux_saturation;
	
	pnss->video_camera_brightness = config::get_config_video()->camera_brightness;
	pnss->video_camera_contrast = config::get_config_video()->camera_contrast;
	pnss->video_camera_hue = config::get_config_video()->camera_hue;
	pnss->video_camera_saturation = config::get_config_video()->camera_saturation;


	pnss->video_backlight_normal = config::get_config_video()->backlight_normal;
	pnss->video_backlight_night = config::get_config_video()->backlight_night;
	pnss->video_backlight_mode = UIBS_NORMAL;

	pnss->video_rear_camera_mirror = config::get_config_video()->camera_mirror;
	pnss->video_parking = config::get_config_video()->parking;

	// backlightֵ��֤��Ҫ����,�������
	pnss->video_backlight_normal = max(10, pnss->video_backlight_normal);
	pnss->video_backlight_night = max(10, pnss->video_backlight_night);


	// ��Ƽ��, ȱʡ�ǹ�
	pnss->sys_light_check = FALSE;

	//����ȱʡΪ�ر�
	pnss->ui_screen_saver_mode = 0;
	pnss->ui_screen_saver_time = 180;

	// bt
	pnss->bt_auto_answer = config::get_config_bluetooth()->auto_answer;
	pnss->bt_auto_connect = config::get_config_bluetooth()->auto_connect;

	// VDisk and GPS path
	_tcscpy(pnss->vdisk_path, config::get_config_disk()->vdisk);
	_tcscpy(pnss->gps_path, config::get_config_disk()->gps_path);

	// canbus
	pnss->sys_canbus_type = config::get_config_canbus()->canbus_type;

	// �߲ʵ�ȱʡΪ0
	pnss->ui_colorful_light = 5;

	// EQ customȱʡֵ
	for (int i=0; i<sizeof(pnss->audio_eq_custom)/sizeof(pnss->audio_eq_custom[0]); i++)
	{
		pnss->audio_eq_custom[i] = MAX_EQ_LEVEL/2;
	}	
}

void sysutil::nss_init()
{
	// �����ȡʧ�ܻ������ϴε�B+�ػ�����Щ��������ȱʡֵ
	if (!nss_read() || !nss_get_instance()->sys_acc_off)
	{
		memset(nss_get_instance(), 0, sizeof(NAVI_SYSTEM_STATUS));	// ��B+ʱ��Ҫ��ֵ��0
		_nss_init_by_config();
	}
	else
	{
		// ��Щ������ʵ����д���ļ��ģ�Ϊ��ȫд���ˣ������������������Ч�ģ����¸�ֵ
		nss_get_instance()->pui_bk_texture = NULL;
		nss_get_instance()->ui_capture_screen = 0;

		nss_get_instance()->sys_fastcamera_status = 0;
		nss_get_instance()->sys_camera_status = 0;
		nss_get_instance()->sys_acc_status = 0;
		nss_get_instance()->sys_rds_status = 0;
		nss_get_instance()->sys_bt_call_status = 0;
		nss_get_instance()->sys_gps_guiding = 0;
		nss_get_instance()->sys_gps_foreground = 0;
		//nss_get_instance()->sys_status = UISS_NORMAL;
		nss_get_instance()->sys_calibrate_status = 0;
		nss_get_instance()->sys_ecar_status = 0;
		// ֻ����ٹػ�״̬,����״̬������
		nss_get_instance()->sys_status = 
			(nss_get_instance()->sys_status == UISS_SLEEP) ? UISS_SLEEP : UISS_NORMAL;

		nss_get_instance()->audio_is_media_front_mute = 0;
		nss_get_instance()->audio_is_media_rear_mute = 0;
		nss_get_instance()->audio_is_bt_mute = 0;
		nss_get_instance()->audio_is_gps_mute = 0;

		// backlightֵ��֤��Ҫ����,�������
		nss_get_instance()->video_backlight_normal = max(10, nss_get_instance()->video_backlight_normal);
		nss_get_instance()->video_backlight_night = max(10, nss_get_instance()->video_backlight_night);
		nss_get_instance()->video_backlight_mode = (nss_get_instance()->video_backlight_mode == UIBS_OFF) ? UIBS_NORMAL : nss_get_instance()->video_backlight_mode;

		// vdisk
		nss_get_instance()->vdisk_is_ripping = 0;
		nss_get_instance()->vdisk_sink = DVP_SINK_NONE;


	}
}

void sysutil::nss_reset()
{
	_nss_init_by_config();	// �ú������뱣֤����Ҫreset����������0,��ʹ�������ļ��е�ֵ
	system_init();

	protocol::get_mcu_volume_table()->key_beep_on = nss_get_instance()->audio_key_beep;
	protocol::get_mcu_sys_para()->general_data[0] = nss_get_instance()->audio_key_beep;	// MCU�ж�ʱ,Ҳ�ж��˸ñ�־λ
	CRpcMCU::GetInstance()->RPC_SetVolume(protocol::get_mcu_volume_table());

	// about video
	ui_update_backlight();
	CSetVideoParams::setBrightness((UINT*)&nss_get_instance()->video_brightness);
	CSetVideoParams::setContrast((UINT*)&nss_get_instance()->video_contrast);
	CSetVideoParams::setHue((UINT*)&nss_get_instance()->video_hue);
	CSetVideoParams::setSaturation((UINT*)&nss_get_instance()->video_saturation);

	::PostMessage(HWND_BROADCAST, NAVI_MSG_LAN_CHANGE, 0, (LPARAM)nss_get_instance()->ui_lan_id);
	::PostMessage(HWND_BROADCAST, NAVI_MSG_BK_CHANGE, 0, (LPARAM)nss_get_instance()->ui_bk_index);
	::PostMessage(HWND_BROADCAST, NAVI_MSG_24HOURTIME_CHANGE, 0, (LPARAM)nss_get_instance()->ui_24hourtime);
	CMainWnd::GetInstance()->PostMessage(UI_MSG_TIMEZONE_CHANGE, 0, 0);
	CMainWnd::GetInstance()->PostMessage(UI_MSG_EQ_CHANGE, 0, (LPARAM)nss_get_instance()->audio_eq_type);
	CMainWnd::GetInstance()->PostMessage(UI_MSG_AUX_VIDEO_PARAMS_CHANGE);

	// bt
	CBluetooth::GetInstance()->EnableAutoConnect(nss_get_instance()->bt_auto_connect);
	CBluetooth::GetInstance()->EnableAutoAnswer(nss_get_instance()->bt_auto_answer);

	// fm area
	// ֻ�лָ���������ʱ�Żָ�����ȱʡ����,����������ָ�����������
	protocol::get_mcu_sys_para()->set_fm_cur_region(config::get_config_misc()->fm_area);
	CRpcMCU::GetInstance()->RPC_SetSysParams(MCU_SYS_PARA::INDEX_FM, 
		protocol::get_mcu_sys_para()->fm_data,	sizeof(protocol::get_mcu_sys_para()->fm_data));
	CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_SYSPARAMS_CHANGE, 0, MCU_SYS_PARA::INDEX_FM);

	// �ָ��߲ʵ�ȱʡֵ0(auto)
	protocol::get_mcu_sys_para()->set_colorful_light(nss_get_instance()->ui_colorful_light);
	CRpcMCU::GetInstance()->RPC_SetSysParams(MCU_SYS_PARA::INDEX_GENERAL, 
		protocol::get_mcu_sys_para()->general_data, sizeof(protocol::get_mcu_sys_para()->general_data));

	CMainWnd::GetInstance()->PostMessage(UI_MSG_SYSTEM_RESET);
}

void sysutil::system_init()
{

	// ��һ�γ�ʼ����ACC״̬�϶�ΪON
	nss_get_instance()->sys_acc_status = NSS_ACC_ON;

	// ���ر���
	nss_load_bkground(nss_get_instance()->ui_bk_index);

	// ��������
	ui_set_language(nss_get_instance()->ui_lan_id);

	// ����ʱ��
	ui_set_timezone(nss_get_instance()->ui_timezone_id);

	// ��һ������ʱ����Զ�����ʱ�䣬������Ϣ֪ͨ�Զ�����ʱ��
	if (nss_get_instance()->ui_time_autoset)
	{
		CMainWnd::GetInstance()->PostMessage(UI_MSG_GPS_SETTIME, 0, 0);
	}

	// audio
	CVolumeManager::Init();
	CVolumeManager::SetDAC(config::get_config_audio()->front_dac, config::get_config_audio()->rear_dac);
	// eq
	MISC_DVP_BALANCES_T bv;
	POINT pt;
	pt.x = nss_get_instance()->audio_eq_balance_lr;
	pt.y = nss_get_instance()->audio_eq_balance_fr;
	SettingsAudio::GetInstance()->CalculatorBalance(pt, bv);
	SettingsAudio::GetInstance()->SetBalance(bv);

	// ��������
	// ��������������ܳ����趨ֵ
	if (nss_get_instance()->audio_media_front_volume > nss_get_instance()->audio_init_media_max_volume)
	{
		nss_get_instance()->audio_media_front_volume = nss_get_instance()->audio_init_media_max_volume;
	}
	CVolumeManager::GetInstance()->UpdateVolume();

	// EQ
	if (sysutil::nss_get_instance()->audio_eq_type == SETTING_AUDIO_EQ_CUSTOM)	// ����
	{
		SettingsAudio::GetInstance()->SetCustomEQbyIndex(sysutil::nss_get_instance()->audio_eq_custom);
	}
	else
	{
		UINT value[10];
		SettingsAudio::GetInstance()->GetEQValues(sysutil::nss_get_instance()->audio_eq_type, value);
		SettingsAudio::GetInstance()->SetCustomEQbyIndex(value);
	}
	SettingsAudio::GetInstance()->EnableSubwoofer(nss_get_instance()->audio_eq_subwoofer_enable);
	if (nss_get_instance()->audio_eq_subwoofer_enable)
	{
		SettingsAudio::GetInstance()->SetSubwoofer(sysutil::nss_get_instance()->audio_eq_subwoofer);
	}

	UINT loudness = sysutil::nss_get_instance()->audio_eq_loudness
		*sysutil::nss_get_instance()->audio_eq_loudness_gain;
	SettingsAudio::GetInstance()->setLoudness(&loudness);
}

BOOL sysutil::nss_save(BOOL is_acc_off)
{
	nss_get_instance()->sys_acc_off = is_acc_off;

	DWORD dwBytesWritten = 0;

	TCHAR folder_name[64];
	tzutil::GetFilePath(NSS_PATH, folder_name);
	::CreateDirectory(folder_name, NULL);	// ��֤��Ŀ¼һ������

	HANDLE hOpenFile = (HANDLE)CreateFile(NSS_PATH ,GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE ,
		NULL, OPEN_ALWAYS, NULL, NULL); 
	if (hOpenFile == INVALID_HANDLE_VALUE) 
	{ 
		return FALSE;
	} 

	WriteFile(hOpenFile, nss_get_instance(), sizeof(NAVI_SYSTEM_STATUS), &dwBytesWritten, NULL);
	CloseHandle(hOpenFile);


	return (dwBytesWritten == sizeof(NAVI_SYSTEM_STATUS));
}

BOOL sysutil::nss_read()
{
	DWORD dwBytesRead = 0;
	HANDLE hOpenFile = (HANDLE)CreateFile(NSS_PATH ,GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE , NULL, OPEN_EXISTING, NULL, NULL); 
	if (hOpenFile == INVALID_HANDLE_VALUE) 
	{ 
		return FALSE;
	} 
	ReadFile(hOpenFile,nss_get_instance(), sizeof(NAVI_SYSTEM_STATUS), &dwBytesRead, NULL);	
	CloseHandle(hOpenFile);

	if (dwBytesRead != sizeof(NAVI_SYSTEM_STATUS))	// ��ȡ����ʧ�ܻ�������������0
	{
		memset(nss_get_instance(), 0, sizeof(NAVI_SYSTEM_STATUS));	
	}

	return (dwBytesRead == sizeof(NAVI_SYSTEM_STATUS));
}

BOOL sysutil::nss_load_bkground(int index)
{
	if (index<0 || (index>NSS_UI_BK_INDEX_MAX && index!=NSS_UI_BK_INDEX_USER))
	{
		RETAILMSG(1, (L"[nvai_car]: sysutil::nss_load_bkground(), background index(%d) error.\n", index));
		return FALSE;
	}

	if (nss_get_instance()->pui_bk_texture == NULL)
	{
		if (WceUiGetBitsPixel() == 16)
		{
			nss_get_instance()->pui_bk_texture = new CWceUi565Bitmap();;
		}
		else
		{
			nss_get_instance()->pui_bk_texture = new CWceUi32Bitmap();
		}
	}

	if (index == NSS_UI_BK_INDEX_USER)	// �û��Զ���
	{
		BYTE* pbyte = NULL;
		HBITMAP hbmp = CreateDibSectionFromDibFile(USER_SKIN_PATH, &pbyte);
		if (hbmp)
		{
			nss_get_instance()->pui_bk_texture->LoadFromHBitmap(hbmp);
			DeleteObject(hbmp);
		}

		// �������ʧ���ˣ�ִ������Ĵ��룬�����ص�һ�ű���
		if (!nss_get_instance()->pui_bk_texture->IsLoaded())
		{
			index = 0;
		}
		else
		{
			return TRUE;
		}
	}


	TCHAR szFilepath[MAX_PATH] = L"";

//	_stprintf(szFilepath, L"%s\\BKSkin.pack\\Skin%d.bmp", tzutil::GetAppPath(), index);
// 	nss_get_instance()->pui_bk_texture = wceui::WceglLoadTextureFromFile(szFilepath);
// 
// 	if(nss_get_instance()->pui_bk_texture == NULL)	// ����ʧ��
// 	{
// 		RETAILMSG(1, (L"[nvai_car]: sysutil::nss_load_bkground(), background index(%d) error.check bkskin.pack pls.\n", index));
// 		return FALSE;
// 	}

	//////////////////////////////
	if (WceUiGetScreenWidth() == 1024)
	{
		_stprintf(szFilepath, L"%s\\BKSkin_1024X600.pack\\Skin%d.bmp", tzutil::GetAppPath(), index);
	}
	else
	{
		_stprintf(szFilepath, L"%s\\BKSkin.pack\\Skin%d.bmp", tzutil::GetAppPath(), index);
	}
	if (WceUiGetBitsPixel() == 16)
	{
		((CWceUi565Bitmap*)nss_get_instance()->pui_bk_texture)->LoadFromFile(szFilepath);
	}
	else
	{
		((CWceUi32Bitmap*)nss_get_instance()->pui_bk_texture)->LoadFromFile(szFilepath);;
	}

	return TRUE;
}


MSDK_MEMORY* sysutil::nss_get_msdk_memory(SOURCE_ID sid)
{
	if (sid == SOURCE_SD)
	{
		return &nss_get_instance()->sd_memory;
	}
	else if (sid == SOURCE_USB)
	{
		return &nss_get_instance()->usb1_memory;
	}
	else if (sid == SOURCE_USB_REAR)
	{
		return &nss_get_instance()->usb2_memory;
	}
	else
	{
		return NULL;
	}
}


////////////////////// Storage ID //////////////////////////////////////

typedef struct _MY_STORAGE_IDENTIFICATION_
{
	STORAGE_IDENTIFICATION	si;
	char ID[64];
} MY_STORAGE_IDENTIFICATION;


static BOOL _get_storageID_SD(const CString &strDiskName, CString &strStorageID)
{
	BOOL bRes = FALSE;

	MY_STORAGE_IDENTIFICATION StorageInfo;
	memset(&StorageInfo, 0, sizeof(MY_STORAGE_IDENTIFICATION));
	StorageInfo.si.dwSize = sizeof(MY_STORAGE_IDENTIFICATION);

	HANDLE hDisk = INVALID_HANDLE_VALUE;

	//Open the driver
	hDisk = CreateFile(strDiskName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); 

	if(hDisk == INVALID_HANDLE_VALUE)
	{
		goto EXIT;
	}

	//Get the ID from the driver
	DWORD dwBytesRet = 0;
	if (DeviceIoControl(hDisk, IOCTL_DISK_GET_STORAGEID, NULL, 0, &StorageInfo, 
		sizeof(MY_STORAGE_IDENTIFICATION), &dwBytesRet, NULL) )
	{
		CString strManufactureID;
		CString strSerialNum;

		//Get the manufacture ID
		if (StorageInfo.si.dwManufactureIDOffset)
		{
			strManufactureID = StorageInfo.ID + (StorageInfo.si.dwManufactureIDOffset-sizeof(STORAGE_IDENTIFICATION));
		}
		if (StorageInfo.si.dwSerialNumOffset)
		{
			strSerialNum = StorageInfo.ID + (StorageInfo.si.dwSerialNumOffset -sizeof(STORAGE_IDENTIFICATION));
		}
		strStorageID = strManufactureID+strSerialNum;

		bRes = TRUE;
	}


EXIT:
	return bRes;
}

static BOOL _get_storageID_USB(const CString &strDiskName, CString &strStorageID)
{
	HKEY  hKey;
	TCHAR szVidPid[64] = L"";
	TCHAR szSerialNumber[64] = L"";
	if ( ERROR_SUCCESS  == RegOpenKeyEx( HKEY_LOCAL_MACHINE,L"Drivers\\USB\\ClientDrivers\\Mass_Storage_Class" ,0, 0, &hKey) )
	{
		DWORD dwSize = 64;
		RegQueryValueEx( hKey, TEXT("USBVidPid"), NULL, NULL, (LPBYTE)szVidPid, &dwSize);
		dwSize = 64; 
		RegQueryValueEx( hKey, TEXT("Serial Number"), NULL, NULL, (LPBYTE)szSerialNumber, &dwSize);
		strStorageID.Format(L"%s%s", szVidPid, szSerialNumber);

		RegCloseKey(hKey);
		return TRUE;
	}

	return FALSE;
}


LPCTSTR _get_device_name(LPCTSTR lpszDiskName)
{
	static TCHAR szDeviceName[64] = {L'\0', };
	szDeviceName[0] = L'\0';

	TCHAR tempPath[MAX_PATH] = {L'\0', };
	if (_tcsicmp(lpszDiskName, L"SDMMC") == 0)
	{
		_tcscpy(tempPath,L"Drivers\\SDCARD\\ClientDrivers\\Class\\SDMemory_Class");
	}
	else if (_tcsicmp(lpszDiskName, L"STORAGE CARD") == 0)
	{
		_tcscpy(tempPath,L"Drivers\\SDCARD\\ClientDrivers\\Class\\SDMemory1_Class");
	}
	else if (_tcsicmp(lpszDiskName, L"USBDISK") == 0)
	{
		_tcscpy(tempPath,L"Drivers\\USB\\ClientDrivers\\Mass_Storage_Class\\EXTHCD0\\6");
	}
	else if (_tcsicmp(lpszDiskName, L"USBDISK1") == 0)
	{
		_tcscpy(tempPath,L"Drivers\\USB\\ClientDrivers\\Mass_Storage_Class\\EXTHCD1\\6");
	}
	else
	{
		return L"";
	}

	HKEY hKey = NULL;
	HKEY hKeySub = NULL;
	LPCTSTR lpszDriversActive = L"Drivers\\Active";
	DWORD dwIndex = 0;
	if ( ERROR_SUCCESS  == RegOpenKeyEx( HKEY_LOCAL_MACHINE, lpszDriversActive ,0, 0, &hKey) )
	{
		int index = 0;
		TCHAR szSubkeyName[64];
		DWORD size = sizeof(szSubkeyName)/sizeof(TCHAR);
		while (ERROR_SUCCESS == RegEnumKeyEx(hKey, index, szSubkeyName, &size, NULL, NULL, NULL, NULL))
		{
			index++;
			size = sizeof(szSubkeyName)/sizeof(TCHAR);

			TCHAR szSubkeyPath[128];
			_stprintf(szSubkeyPath, L"%s\\%s", lpszDriversActive, szSubkeyName);
			if (ERROR_SUCCESS  == RegOpenKeyEx( HKEY_LOCAL_MACHINE, szSubkeyPath ,0, 0, &hKeySub))
			{
				TCHAR szKey[128];
				DWORD dwKeySize = sizeof(szKey); 
				if( ERROR_SUCCESS == RegQueryValueEx( hKeySub, TEXT("Key"), NULL, NULL, (LPBYTE)szKey, &dwKeySize))
				{
					if (_tcsstr(szKey, tempPath) != NULL)
					{
						DWORD dwDeviceSize = sizeof(szDeviceName); 
						RegQueryValueEx( hKeySub, TEXT("Name"), NULL, NULL, (LPBYTE)szDeviceName, &dwDeviceSize);

						RegCloseKey(hKeySub);
						hKeySub = NULL;
						break;
					}
				}

				RegCloseKey(hKeySub);
				hKeySub = NULL;
			}
		}
	}

	if (hKey)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	RETAILMSG(TRUE, (L"********************************szDeviceName = %s******************************** \n",szDeviceName) );
	return szDeviceName;
}


static BOOL _is_USB_diskname(const CString &strDiskName)
{
	if (strDiskName == _get_device_name(L"USBDISK")
		|| strDiskName == _get_device_name(L"USBDISK1"))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL sysutil::get_storageID(const CString &strDiskName, CString &strStorageID)
{
	if (_is_USB_diskname(strDiskName))
	{
		strStorageID = L"USBVidPid_Serial_Number"; //USB ID set for fixed value
		return TRUE;//_get_storageID_USB(strDiskName, strStorageID);
	}
	else
	{
		return _get_storageID_SD(strDiskName, strStorageID);
	}
}


HBITMAP sysutil::CreateDibSectionFromDibFile(LPCTSTR szFileName, LPBYTE *pData )
{
	ASSERT(szFileName != NULL);
	*pData = NULL;
	HBITMAP	hBitmap = NULL;
	BITMAPFILEHEADER	bmfh;
	BITMAPINFO		*pbim;
	BYTE	*pBits = NULL;
	BOOL		bSuccess;
	DWORD	dwInfoSize, dwBytesRead;
	HANDLE	hFile;
	DWORD	dwDataLength = 0;
	DWORD   dwResidual = 0;

	hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	bSuccess = ReadFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
	if (!bSuccess || (dwBytesRead != sizeof(BITMAPFILEHEADER))
		||(bmfh.bfType != *(WORD*)"BM"))
	{
		CloseHandle(hFile);
		return NULL;
	}

	dwInfoSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
	pbim = (BITMAPINFO*)malloc(dwInfoSize);

	bSuccess = ReadFile(hFile, pbim, dwInfoSize, &dwBytesRead, NULL);
	if (!bSuccess || (dwBytesRead != dwInfoSize))
	{
		free (pbim) ;
		CloseHandle(hFile);
		return NULL;
	}

	hBitmap = CreateDIBSection(NULL,pbim, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
	if (hBitmap == NULL)
	{
		free(pbim);
		CloseHandle(hFile);
		return NULL;
	}

	//����λͼ�洢�����ֽڶ��룬���ܼ򵥵��ó�*��������ͼ�����ݴ�С,����,biSizeImage�е�ʱ��Ϊ0��Ҳ�޷������������
	dwResidual = ( pbim->bmiHeader.biWidth * pbim->bmiHeader.biBitCount ) % 32;
	if( dwResidual != 0 )
	{
		dwDataLength = ( ( pbim->bmiHeader.biWidth * pbim->bmiHeader.biBitCount ) + ( 32 - dwResidual ) ) * pbim->bmiHeader.biHeight;
		dwDataLength = dwDataLength / 8;
	}
	else
	{
		dwDataLength = pbim->bmiHeader.biWidth * pbim->bmiHeader.biHeight * pbim->bmiHeader.biBitCount / 8;
	}


	ReadFile (hFile, pBits, dwDataLength, &dwBytesRead, NULL);

	free (pbim) ;
	CloseHandle (hFile) ;

	if (pData)
	{
		*pData = pBits;
	}

	return hBitmap ;
}
