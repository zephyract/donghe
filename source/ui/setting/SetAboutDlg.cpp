#include "stdafx.h"
#include "WceUiBase.h"
#include "SetAboutDlg.h"
#include "DVP_Agent.h"
#include "config.h"
#include "MainWnd.h"
#include "SourceManager.h"

#include <WinIoCtl.h>
#include <atlbase.h>

#define FILE_DEVICE_HAL					0x00000101
#define IOCTL_HAL_GET_BOOTVERSION_CODE    2077            
#define IOCTL_HAL_GET_BootVERSION	CTL_CODE(FILE_DEVICE_HAL, IOCTL_HAL_GET_BOOTVERSION_CODE, METHOD_BUFFERED, FILE_ANY_ACCESS)      //tjj++

extern "C" BOOL WINAPI KernelIoControl( DWORD dwIoControlCode, LPVOID lpInBuf, DWORD nInBufSize, LPVOID lpOutBuf, DWORD nOutBufSize, LPDWORD lpBytesReturned );
#define IOCTL_HAL_GET_DEVICE_CODE CTL_CODE( FILE_DEVICE_HAL, 2074, METHOD_BUFFERED, FILE_ANY_ACCESS )
#define IOCTL_HAL_GET_MAP_CODE CTL_CODE( FILE_DEVICE_HAL, 2075, METHOD_BUFFERED, FILE_ANY_ACCESS )

#define IOCTL_HAL_GET_DEVICEID CTL_CODE(FILE_DEVICE_HAL, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_GET_OSVERSION     CTL_CODE(FILE_DEVICE_HAL, 2076, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _DEVICE_ID {
	DWORD   dwSize;
	DWORD   dwPresetIDOffset;
	DWORD   dwPresetIDBytes;
	DWORD   dwPlatformIDOffset;
	DWORD   dwPlatformIDBytes;
} DEVICE_ID, *PDEVICE_ID;



CSetAboutDlg::CSetAboutDlg(void)
{
}

CSetAboutDlg::~CSetAboutDlg(void)
{

}

void CSetAboutDlg::OnInitDialog()
{
	CWceUiScrollLayer* pLayer = (CWceUiScrollLayer*)GetChildByName(L"frame");
	if (pLayer)
	{
		pLayer->SetScrollMode(SL_VERT);
		pLayer->SetExtraScroll(30);
	}

	UpdateSysinfo();

	__super::OnInitDialog();
}


void CSetAboutDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetGeneralDlg_ID, SOURCE_SETTING);
		return;
	}

	__super::OnBnClick(pButton);
}

void CSetAboutDlg::UpdateSysinfo()
{
	InitSysInfo();

	CWceUiLayer *player = GetLayerByName(L"os_value");
	if (player)
	{
		player->SetText(m_strOS);
	}

	player = GetLayerByName(L"app_value");
	if (player)
	{
		player->SetText(m_strApp);
	}

	player = GetLayerByName(L"mcu_value");
	if (player)
	{
		player->SetText(m_strMcuVer);
	}

	player = GetLayerByName(L"dvd_value");
	if (m_strDVD.GetLength() <= 0)	// 获取DVD版本失败或不支持DVD时,为空串,此时不显示DVD版本
	{
		player->ShowLayer(FALSE);
		CWceUiLayer *pDvd = GetLayerByName(L"dvd");
		if (pDvd) pDvd->ShowLayer(FALSE);
	}
	if (player)
	{
		player->SetText(m_strDVD);
	}

	player = GetLayerByName(L"manufacture_value");
	if (player)
	{
		player->SetText(m_strManufacture);
	}
}

void CSetAboutDlg::InitSysInfo()
{
	// boot
	KernelIoControl(IOCTL_HAL_GET_BootVERSION ,NULL,0,m_strBoot.GetBuffer(256),256,NULL);
	m_strBoot.ReleaseBuffer();

	// os
	KernelIoControl(IOCTL_HAL_GET_OSVERSION ,NULL,0,m_strOS.GetBuffer(260),260,NULL);
	m_strOS.ReleaseBuffer();

	int nTotalFlash = GetFlashSize();
	if ( nTotalFlash<=64 )
	{
		nTotalFlash = 128;	
	}

	if(nTotalFlash>=1024)
	{
		m_strOS.Format(L"%s.%dG",m_strOS,nTotalFlash/1024);
	}
	else
	{
		m_strOS.Format(L"%s.%dM",m_strOS,nTotalFlash);
	}


	// app & manufacture
	m_strApp = config::get_config_sysinfo()->app_ver;
	m_strManufacture = config::get_config_sysinfo()->manufacture;

	// uuid
	iGOGetDeviceCode(m_strUUID.GetBuffer(128));
	m_strUUID.ReleaseBuffer();

	//MCU VERSION	
	CRpcMCU::GetInstance()->RPC_GetMcuVer(m_strMcuVer.GetBuffer(64));
	m_strMcuVer.ReleaseBuffer();

	// dvd
	CWceUiLayer* player = GetLayerByName(L"dvd_value");
	if (player)		// 当不用显示DVD时,不调用这接口,3353上无DVD,调此接口会有个延时
	{
		GetDVDVersion(m_strDVD.GetBuffer(64));
		m_strDVD.ReleaseBuffer();
	}
}

int CSetAboutDlg::GetFlashSize()
{
	double		 nSpaceTotal	= 0;
	double		 nSpaceFree		= 0;

	CString strFlash = L"ResidentFlash";
	CRegKey reg;
	if(ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE,L"\\System\\StorageManager\\Profiles\\FlashDisk"))
	{
		TCHAR szTmp[MAX_PATH] = {0};
		ULONG len = MAX_PATH;
		memset(szTmp,0,sizeof(TCHAR)*MAX_PATH);
		if(reg.QueryStringValue(L"Folder", szTmp, &len))
		{
			strFlash = szTmp;
			if(strFlash.GetLength()<=0)
			{
				strFlash= L"ResidentFlash";
			}
		}
	}

	CString strFlash2 = L"ResidentFlash2";
	CRegKey reg2;
	if(ERROR_SUCCESS == reg2.Open(HKEY_LOCAL_MACHINE,L"\\System\\StorageManager\\Profiles\\HFTL2"))
	{
		TCHAR szTmp[MAX_PATH] = {0};
		ULONG len = MAX_PATH;
		memset(szTmp,0,sizeof(TCHAR)*MAX_PATH);
		if(reg2.QueryStringValue(L"Folder",szTmp, &len))
		{
			strFlash2 = szTmp;
			if(strFlash2.GetLength()<=0)
			{
				strFlash2= L"ResidentFlash2";
			}
		}
	}

	//--: 计算NandFlash分区
	ULARGE_INTEGER NandFlashFree, NandFlashTotalSpace;
	if (TRUE == GetDiskFreeSpaceEx(strFlash, &NandFlashFree, &NandFlashTotalSpace, 0))
	{
		nSpaceTotal = (double)NandFlashTotalSpace.QuadPart;
	}	
	if (TRUE == GetDiskFreeSpaceEx(strFlash2, &NandFlashFree, &NandFlashTotalSpace, 0))
	{
		nSpaceTotal += (double)NandFlashTotalSpace.QuadPart;
	}
	//--: 显用实际NandFlash 大小以及剩余大小
	int TotalNandflash = 64;
	int nTotal = (ULONG)nSpaceTotal / (1024*1024);					
	for( ;nTotal > TotalNandflash; TotalNandflash *= 2 );	// 找到最接近32的整数倍
	return TotalNandflash;
}

BOOL CSetAboutDlg::iGOGetDeviceCode(TCHAR *deviceID)
{
	PDEVICE_ID DeviceID;
	wchar_t serial[512];
	DWORD Len, OutLen;

	// Indicate the size.
	Len = 512;
	DeviceID = (PDEVICE_ID) new BYTE[512];
label_retry:

	if ( KernelIoControl (IOCTL_HAL_GET_DEVICEID, NULL, 0, (LPVOID)DeviceID, Len, &OutLen) ) 
	{
		//	mbstowcs(deviceID,(char*)((PBYTE)DeviceID + DeviceID->dwPresetIDOffset),DeviceID->dwPresetIDBytes);
		DWORD i;
		wchar_t tmpBuffer[10];
		tmpBuffer[0] = 0;
		serial[0] = 0;
		if (DeviceID->dwPresetIDOffset) 
		{
			for (i=0; i < DeviceID->dwPresetIDBytes; i++) 
			{
				if(i!=0&&i%4==0)
				{
					if(i==8)
					{
						wcscat(serial, L" - ");
					}
					else
					{
						wcscat(serial, L" - ");
					}

				}
				wsprintf(tmpBuffer, _T("%02X"), *((PBYTE)DeviceID + DeviceID->dwPresetIDOffset+ i));
				wcscat(serial, tmpBuffer);
			}
			wcscpy(deviceID, serial);
		}
		delete DeviceID;
	}
	else 
	{
		int err;
		err = GetLastError ();
		if (err == 122 && Len == 512)
		{
			Len = *(int*)DeviceID;
			goto label_retry;
		}
		wcscpy (deviceID, TEXT("00"));
		delete DeviceID;
		return true;
	}
	return TRUE;
}

void CSetAboutDlg::GetDVDVersion(OUT TCHAR *pDVDVersion)
{
	if (!config::get_config_functions()->is_support(SOURCE_DVD))
	{
		*pDVDVersion = L'\0';
		return;
	}

	DWORD dwMainVer = 0, dwSerVer = 0;
	if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_DVD
		|| CSourceManager::GetInstance()->GetRearSrcID() == SOURCE_DVD)
	{
		DVP_GetFirmwareVer( (GUINT8*)(&dwMainVer) );
		DVP_GetServoVer( (GUINT8*)(&dwSerVer) );
	}
	else
	{
		DVP_Init();
		DVP_GetFirmwareVer( (GUINT8*)(&dwMainVer) );
		DVP_GetServoVer( (GUINT8*)(&dwSerVer) );
		DVP_Deinit();
	}

	TCHAR szDvpVersion[16] = {0};
	TCHAR szSerVersion[16] = {0};
	wsprintf(szDvpVersion,L"%02x%02x%02x%02x",LOBYTE(LOWORD(dwMainVer)),HIBYTE(LOWORD(dwMainVer)),LOBYTE(HIWORD(dwMainVer)),HIBYTE(HIWORD(dwMainVer)));
	wsprintf(szSerVersion,L"%02x%02x%02x%02x",LOBYTE(LOWORD(dwSerVer)),HIBYTE(LOWORD(dwSerVer)),LOBYTE(HIWORD(dwSerVer)),HIBYTE(HIWORD(dwSerVer)));

	if (pDVDVersion)
	{
		_stprintf(pDVDVersion, L"%s-%s", szDvpVersion, szSerVersion);
	}
}

#include "interface.h"
void CSetAboutDlg::OnLButtonDown(UINT nFlags, POINT point)
{
	if (::PtInRect(CRect(WceUiGetScreenWidth()-70, WceUiGetScreenHeight()-70,WceUiGetScreenWidth(),WceUiGetScreenHeight()), point))
	{
		SetTimer(TIMER_ID_EXIT_APP, 5000);
	}
	else if (::PtInRect(CRect(0, WceUiGetScreenHeight()-70,70,WceUiGetScreenHeight()), point))
	{
		SetTimer(TIMER_ID_FACTORY_SET, 5000);
	}

	__super::OnLButtonDown(nFlags, point);
}

void CSetAboutDlg::OnLButtonUp(UINT nFlags, POINT point)
{
	KillTimer(TIMER_ID_EXIT_APP);
	KillTimer(TIMER_ID_FACTORY_SET);

	__super::OnLButtonDown(nFlags, point);
}

void CSetAboutDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_EXIT_APP)
	{
		GetWindow()->PostMessage(NAVI_MSG_APP_EXIT);
	} 
	else if (wIDEvent == TIMER_ID_FACTORY_SET)
	{
		CDlgManager::GetInstance()->ShowDlg(CSetFactoryDlg_ID, SOURCE_SETTING);
	}

	__super::OnTimer(wIDEvent);
}