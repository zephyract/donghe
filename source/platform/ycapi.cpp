#include "stdafx.h"
#include "ycapi.h"

#include <atlbase.h>
#include <windows.h>
//#include <nkintr.h>
#include <ceddk.h>
//#include <ioctl_cfg.h>
//#include <YCAPIDLL.h>
//#include <YG_API.h>
//#include <YG_MetazoneDefine.h>

BOOL YC_SetDST(DWORD bEnable)
{
	HKEY 	hKey;
	if ( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Clock",0, 0, &hKey) )
	{
		return FALSE;
	}

	if ( ERROR_SUCCESS != RegSetValueEx(hKey, TEXT("AutoDST"), 0, REG_DWORD, (BYTE*)&bEnable, sizeof(DWORD)) )
	{
		RegCloseKey (hKey);
		return FALSE;
	}

	RegCloseKey (hKey);
	return TRUE;
}

BOOL YC_EnableGpsTimeAutoSet(DWORD dwEnable)
{
	HKEY 	hKey;
	if ( ERROR_SUCCESS  != RegOpenKeyEx(	HKEY_LOCAL_MACHINE, L"Drivers\\BuiltIn\\GPS",0, 0, &hKey) )
	{
		return FALSE;
	}

	if ( ERROR_SUCCESS  != RegSetValueEx(hKey, TEXT("SyncRTCEnable"), 0, REG_DWORD, (BYTE*)&dwEnable, sizeof(DWORD)) )
	{
		RegCloseKey (hKey);
		return FALSE;
	}

	RegCloseKey (hKey);
	return TRUE;
}

DWORD YC_GetBootBackCarState()
{
	HANDLE handleFSC =NULL;
	DWORD dwStatus = (DWORD)-1;
	DWORD dwSize = (DWORD)-1;
	handleFSC =   CreateFile(L"FSC1:",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(handleFSC)
	{
		DeviceIoControl(handleFSC,IOCTL_FSC_NOTIFY_APP_READY, NULL, 0, &dwStatus, sizeof(DWORD), &dwSize,NULL);
		CloseHandle(handleFSC);
	}

	// OS�Ľӿ�1��ʾ������ 2��ʾδ����������ת����
	return (dwStatus == 1) ? 1 : 0;
}

DWORD YC_GetBootBackCarState2()
{
	#define IOCTL_FSC_GET_BACKUP_STATE CTL_CODE(32768, 2054, METHOD_BUFFERED, FILE_ANY_ACCESS)

	HANDLE hFSCDriver = NULL;
	DWORD dwStatus = (DWORD)-1;
	DWORD dwSize = (DWORD)-1;
	hFSCDriver =CreateFile(L"FSC1:",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFSCDriver)
	{
		DeviceIoControl( hFSCDriver,IOCTL_FSC_GET_BACKUP_STATE,NULL,0,&dwStatus,sizeof(DWORD),&dwSize,NULL);
		CloseHandle(hFSCDriver);
	}

	// OS�Ľӿ�1��ʾ������ 2��ʾδ����������ת����
	return (dwStatus == 1) ? 1 : 0;
}


extern "C" BOOL KernelIoControl(
								DWORD dwIoControlCode,
								LPVOID lpInBuf,
								DWORD nInBufSize,
								LPVOID lpOutBuf,
								DWORD nOutBufSize,
								LPDWORD lpBytesReturned
								);

void YC_DecodeEnable(DWORD dwIoControlCode, BOOL bEnable, DWORD size)
{	
	KernelIoControl(dwIoControlCode, &bEnable, size, NULL, 0, NULL) ;
}

// --------------------------------------------------
// ��ȡUUID����
// ����:
// pData: 
// DataLen: 
// --------------------------------------------------
VOID YC_GetUUID(CHAR *pData, DWORD DataLen)
{
////-	KernelIoControl(IOCTL_HAL_GET_UUID, NULL, 0, pData, DataLen, NULL);
}

// --------------------------------------------------
// ����Logo�ļ�
// ����:
// pData: LogoԴ�ļ��洢��ַ
// DataLen: ��ȡ��Logoʵ�ʴ�С
// --------------------------------------------------
VOID YC_UpdateLogo(CHAR *pData, DWORD DataLen)
{
	HANDLE hYGAPIHandle=NULL;	
	hYGAPIHandle = CreateFile(_T("YGI1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
	if(hYGAPIHandle)
	{
////-		DeviceIoControl(hYGAPIHandle,IOCTL_YGI_UPDATE_LOGO,pData,DataLen,NULL,0,NULL,NULL);
		CloseHandle(hYGAPIHandle);
	}
}

// --------------------------------------------------
// ��ȡBoot�汾��Ϣ.
// ����:
// pWData: 
// DataLen: 
// --------------------------------------------------
VOID YC_GetBootRev(WCHAR *pWData,DWORD DataLen)
{
	HANDLE hYGAPIHandle=NULL;	
	hYGAPIHandle = CreateFile(_T("YGI1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
	if(hYGAPIHandle)
	{
////-		DeviceIoControl(hYGAPIHandle,IOCTL_YGI_GET_BOOTVERSION,NULL,0,pWData,DataLen,NULL,NULL);
		CloseHandle(hYGAPIHandle);
	}
}

// --------------------------------------------------
// ��ȡOS�汾��Ϣ.
// ����:
// pWData: 
// DataLen: 
// --------------------------------------------------
VOID YC_GetOSRev(WCHAR *pWData,DWORD DataLen)
{
////-	KernelIoControl(IOCTL_HAL_GET_OSVERSION, NULL, 0, pWData, DataLen, NULL);
}

// --------------------------------------------------
// ʹ�ܱ���
// APP���Ĳ���: bEnable: 0:disable; 1:
// ʵ�����ò���: 0-100
// --------------------------------------------------
#include "RpcMCU.h"
VOID YC_BackLightEnable(DWORD bEnable)
{
// 	HANDLE hYGBKLHandle=NULL;	
// 	hYGBKLHandle = CreateFile(_T("BKL1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
// 	if(hYGBKLHandle)
// 	{
// 		DeviceIoControl(hYGBKLHandle,IOCTL_BKL_SET_ENABLE,&bEnable,sizeof(DWORD),NULL,0,NULL,NULL);
// 		CloseHandle(hYGBKLHandle);
// 	}

	// ����汾������MCU����
	CRpcMCU::GetInstance()->RPC_KeyCommand(T_BLACKOUT, bEnable ? 0x01000000 : 0);
}

// --------------------------------------------------
// ���ñ��⺯��.
// APP���Ĳ���: u4Level: 0-100
// ʵ�����ò���: 0-100
// --------------------------------------------------
VOID YC_SetBackLightLevel(DWORD u4Level)
{
	HANDLE hYGBKLHandle=NULL;	
	hYGBKLHandle = CreateFile(_T("BKL1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
	if(hYGBKLHandle)
	{
		DeviceIoControl(hYGBKLHandle,IOCTL_BKL_SET_INTENSITY,&u4Level,sizeof(DWORD),NULL,0,NULL,NULL);
		CloseHandle(hYGBKLHandle);
	}
}

// --------------------------------------------------
// ��ȡ�����ȹ⺯��.
// ����: 0-100
// 0xff:���ش���
// --------------------------------------------------
DWORD YC_GetBackLightLevel()
{
	HANDLE hYGBKLHandle=NULL;
	DWORD u4Level = 0xff;
	hYGBKLHandle = CreateFile(_T("BKL1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
	if(hYGBKLHandle)
	{
		DeviceIoControl(hYGBKLHandle,IOCTL_BKL_GET_INTENSITY,NULL,0,&u4Level,sizeof(DWORD),NULL,NULL);
		CloseHandle(hYGBKLHandle);
	}
	return u4Level;
}


// ����GPIO����Ϊ�����������
#define IOCTL_YGI_SET_GPIOMODE					CTL_CODE(FILE_DEVICE_UNKNOWN, 0x10F, METHOD_BUFFERED, FILE_ANY_ACCESS)
VOID YC_SetGpioMode(DWORD PinNum,DWORD IOMode)
{
	HANDLE hYGAPIHandle=NULL;	
	DWORD  pBuf[2]={0};
	pBuf[0] = PinNum;
	pBuf[1] = IOMode;
	hYGAPIHandle = CreateFile(_T("YGI1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
	if(hYGAPIHandle)
	{
		DeviceIoControl(hYGAPIHandle,IOCTL_YGI_SET_GPIOMODE,pBuf,sizeof(DWORD)*2,NULL,0,NULL,NULL);
		CloseHandle(hYGAPIHandle);
	}
	else
	{
		RETAILMSG(1, ((L"  CreateFile YGI1 Error  %d \r\n"),GetLastError() ) );
	}
}

// GPIO��Ϊ����߻��
#define IOCTL_YGI_SET_GPIO_OUTPUTDATA			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x110, METHOD_BUFFERED, FILE_ANY_ACCESS)	
VOID YC_SetGpioData(DWORD PinNum, DWORD PinLevel)
{
	HANDLE hYGAPIHandle=NULL;	
	DWORD  pBuf[2]={0};
	pBuf[0] = PinNum;
	pBuf[1] = PinLevel;
	hYGAPIHandle = CreateFile(_T("YGI1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
	if(hYGAPIHandle)
	{
		DeviceIoControl(hYGAPIHandle,IOCTL_YGI_SET_GPIO_OUTPUTDATA,pBuf,sizeof(DWORD)*2,NULL,0,NULL,NULL);
		CloseHandle(hYGAPIHandle);
	}
}

// --------------------------------------------------
// ��ȡGPIO�ڵĵ�ƽ״̬
//
// --------------------------------------------------
DWORD YC_GetGpioData(DWORD PinNum)
{
	HANDLE hYGAPIHandle=NULL;	
	DWORD  bPinData = 0;

	hYGAPIHandle = CreateFile(_T("YGI1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
	if(hYGAPIHandle)
	{
//		DeviceIoControl(hYGAPIHandle,IOCTL_YGI_GET_GPIO_INPUTDATA,&PinNum,sizeof(DWORD),&bPinData,sizeof(DWORD),NULL,NULL);
		CloseHandle(hYGAPIHandle);
	}

	return bPinData;
}


// --------------------------------------------------
// YC_EnableTouch
// ���ô������Ĺ���
//
// -------------------------------------------------
VOID  YC_EnableTouch(DWORD   bType)
{
	HANDLE hYGAPIHandle=NULL;	
	hYGAPIHandle = CreateFile(_T("YGI1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
	if(hYGAPIHandle)
	{
		DeviceIoControl(hYGAPIHandle,IOCTL_YGI_TOUCH_ENABLE,&bType,sizeof(DWORD),NULL,0,NULL,NULL);
		CloseHandle(hYGAPIHandle);
	}
}

// --------------------------------------------------
// YC_EnableDebugLog
// ��ֹʹ��DEBUG��Ϣ
// ����:
// pWData: 
// DataLen: 
// --------------------------------------------------
VOID YC_EnableDebugLog(BOOL bEnable)
{
////-	KernelIoControl(IOCTL_HAL_ENABLE_UART_LOG, &bEnable, sizeof(BOOL), NULL, 0, NULL);
}

// --------------------------------------------------
// YC_GetDskAttach
// ��ȡ5��DSK�Ƿ����
// ����:
// pWData: 
// DataLen: 
// --------------------------------------------------
DWORD YC_GetDiskAttach()
{
	HANDLE hYGAPIHandle=NULL; 
	DWORD bAttach = 0xFFFFFFFF;
	hYGAPIHandle = CreateFile(_T("YGI1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
	if(hYGAPIHandle)
	{
		DeviceIoControl(hYGAPIHandle,IOCTL_YGI_GET_DSK_ATTACH,NULL,0,&bAttach,sizeof(DWORD),NULL,NULL);
		CloseHandle(hYGAPIHandle);
	}
	return bAttach;
}

DWORD YC_GetChipType()
{
	DWORD fgMt3353 = 0;
	DWORD fgSuportH264 = 0;
	DWORD fgSuport3D = 0;
	DWORD fgChipFeature = 0;
	DWORD fgInputData;

	KernelIoControl(IOCTL_HAL_GET_FEATUREDISCLESS, NULL,0, &fgChipFeature, sizeof(DWORD), NULL);
	KernelIoControl(IOCTL_HAL_GET_CHIP_VERSION_MTK, NULL,0, &fgMt3353, sizeof(DWORD), NULL);
	fgInputData = 0x80;
	KernelIoControl(IOCTL_HAL_GET_CHIP_FEATURE_MTK, &fgInputData,sizeof(DWORD), &fgSuportH264, sizeof(DWORD), NULL);
	fgInputData = 0x800;
	KernelIoControl(IOCTL_HAL_GET_CHIP_FEATURE_MTK, &fgInputData,sizeof(DWORD), &fgSuport3D, sizeof(DWORD), NULL);

	if(fgMt3353)
	{
		return CHIP_TYPE_MT3353BDG;
	}
	else if(fgChipFeature & (1<<15))
	{
		if(fgSuport3D)
		{
			return CHIP_TYPE_MT3353CBDG;
		}
		else
		{
			return CHIP_TYPE_MT3353CBCG;
		}
	}
	else
	{
		if(fgSuport3D)
		{
			return CHIP_TYPE_MT3360MXDG;
		}
		else if(fgSuportH264)
		{
			return CHIP_TYPE_MT3360KNCG;
		}
		else 
		{
			return CHIP_TYPE_MT3360BICG;
		}
	}
}
//------------------------------------------------------------------------------
// Function:GetConfigBaudrate
// ��ȡMetazeon���õĲ�����
//------------------------------------------------------------------------------
DWORD YC_GetConfigBaudrate()
{
////-
// 	HANDLE hYGAPIHandle=NULL;
// 	_MetaInfo	mMetaInfo = {0};
// 	DWORD ConfigBaudrate = 0;
// 	hYGAPIHandle = CreateFile(_T("YGI1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
// 	if(hYGAPIHandle)
// 	{
// 		DeviceIoControl(hYGAPIHandle,IOCTL_YGI_GET_METAZONEINFO,NULL,0,&mMetaInfo,sizeof(_MetaInfo),NULL,NULL);
// 		CloseHandle(hYGAPIHandle);
// 	}
// 	switch(mMetaInfo.dwYGMetaInfo.CL_McuBaudRate)
// 	{
// 	case MCU_BAUDRATE_4800:
// 		ConfigBaudrate = 4800;
// 		break;
// 	case MCU_BAUDRATE_9600:
// 		ConfigBaudrate = 9600;
// 		break;
// 	case MCU_BAUDRATE_14400:
// 		ConfigBaudrate = 14400;
// 		break;
// 	case MCU_BAUDRATE_19200:
// 		ConfigBaudrate = 19200;
// 		break;
// 	case MCU_BAUDRATE_38400:
// 		ConfigBaudrate = 38400;
// 		break;
// 	case MCU_BAUDRATE_57600:
// 		ConfigBaudrate = 57600;
// 		break;
// 	case MCU_BAUDRATE_115200:
// 		ConfigBaudrate = 115200;
// 		break;
// 	case MCU_BAUDRATE_230400:
// 		ConfigBaudrate = 230400;
// 		break;
// 	case MCU_BAUDRATE_460800:
// 		ConfigBaudrate = 460800;
// 		break;
// 	case MCU_BAUDRATE_921600:
// 		ConfigBaudrate = 921600;
// 		break;
// 	}
// 	RETAILMSG(1,(TEXT("[GetConfigBaudrate] : %d\r\n"),ConfigBaudrate));
// 	return ConfigBaudrate;
return 0;
}

BOOL YC_IsUSB11()
{
	//ֻ��USBΪ������Ӧ������Ϊ1.1�汾ʱ����Ҫ���������Ϣ��Ϊ����״̬(������Ӧ����USB2.0)��������Ϊ�ǰγ�
	CRegKey key;
	if ( ERROR_SUCCESS == key.Open(HKEY_LOCAL_MACHINE, L"Drivers\\BuiltIn\\UsbOtg\\Hcd"))
	{
		DWORD RegUSBUseDM;
		DWORD MgcPowerHsEnable;
		if (ERROR_SUCCESS == key.QueryDWORDValue(L"RegUSBUseDM",RegUSBUseDM)
			&& ERROR_SUCCESS == key.QueryDWORDValue(L"MgcPowerHsEnable",MgcPowerHsEnable)
			&& RegUSBUseDM == 0
			&& MgcPowerHsEnable == 0	)
		{
			return TRUE;
		}

	}

	return FALSE;
}

DWORD YC_GetDiskExistStatus()
{
	HANDLE hYGAPIHandle=NULL;	
	DWORD dwAttach;
	hYGAPIHandle = CreateFile(_T("YGI1:"),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,0,0);
	if(hYGAPIHandle)
	{
		DeviceIoControl(hYGAPIHandle,IOCTL_YGI_GET_DSK_ATTACH, NULL,0,&dwAttach,sizeof(DWORD),NULL,NULL);
		CloseHandle(hYGAPIHandle);
	}

	return dwAttach;
}