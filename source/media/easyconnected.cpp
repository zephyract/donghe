#include "StdAfx.h"
#include "easyconnected.h"
#include "MainWnd.h"
#include "sysUtil.h"
#include "interface.h"
#include "SourceManager.h"
#include "tzUtil.h"
#include "bluetooth.h"
#include "DlgManager.h"
#include "OverlayDlg.h"
#include "SetAboutDlg.h"

CEasyConnected::CEasyConnected(void)
: m_nDeviceType(-1)
{

}

CEasyConnected::~CEasyConnected(void)
{


}

CEasyConnected* CEasyConnected::GetInstance()
{
	static CEasyConnected easyconnected;
	return &easyconnected;
}

void CEasyConnected::ShowWindow(BOOL bShow)
{
	HWND hwnd = ::FindWindow(L"EasyConnected", L"EasyConnect");
	if (hwnd != NULL)
	{
		::ShowWindow(hwnd, bShow ? SW_SHOW : SW_HIDE);
		if (bShow)
		{
			::SetForegroundWindow(hwnd);
		}
	}
}

void CEasyConnected::PostMessage2EasyConnected(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = ::FindWindow(L"EasyConnected", L"EasyConnect");
	if (hwnd != NULL)
	{
		::PostMessage(hwnd, message, wParam, lParam);
	}
}

BOOL CEasyConnected::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UM_NET_EASYCONN)
	{
		RETAILMSG(1, (_T("[navi_car] -UM_NET_EASYCONN. wParm = %d \n"), wParam));
		//CEasyConnected::GetInstance()->Initialize((HWND)wParam);
		if (wParam == PROCESS_START)
		{
			;
		}
		else if (wParam == PHONE_PLUGIN || wParam == IOSPHONE_PLUGIN)	// 手机插入
		{
			m_nDeviceType = (wParam == IOSPHONE_PLUGIN)  ? 1 : 0;
			CVolumeManager::GetInstance()->UpdateVolume();
			OnConnect(TRUE);
		}
		else if (wParam == PHONE_PLUGOUT || wParam == IOSPHONE_PLUGOUT)	// 手机拔出
		{
			m_nDeviceType = -1;
			CVolumeManager::GetInstance()->UpdateVolume();
			OnConnect(FALSE);
		}
		else if (wParam == ECST_GUI_MIN)	
		{
			CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
		}
		else if (wParam == PROCESS_EXIT)	// 程序退出
		{
			CMainWnd::GetInstance()->PostMessage(UI_MSG_SOURCE_EXIT, 0, (LPARAM)SOURCE_EASY_CONNECTED);
		}
	}
	else if(message == WM_COPYDATA)
	{
		RETAILMSG(1, (_T("[navi_car] -WM_COPYDATA \r\n")));
		COPYDATASTRUCT *pcd=(COPYDATASTRUCT *)lParam;
		if (pcd && pcd->cbData == sizeof(C2U_GET_BLUE_TOOTH))
		{
			; 
		}
		return FALSE;	// 这个不是专用消息
	}
	else if (message == UI_MSG_BT_CONNECT_STATUS_CHANGE && lParam == BZ_IND_CREATE_CONNECT_SUCCESS)
	{
		if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_EASY_CONNECTED
			&& GetDeviceType() == 0)
		{
			CMainWnd::GetInstance()->SetTimer(TIMER_ID_EASYCONNECTED_AD2P, 800, NULL);
		}
		return FALSE;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


void CEasyConnected::OnConnect(BOOL bConnect)
{
	CMainWnd::GetInstance()->RestartScreenSaver();
	if (sysutil::nss_get_instance()->sys_status == UISS_BACKLIGHT_OFF)  // 如果关屏了,打开屏幕
	{
		CMainWnd::GetInstance()->PostMessage(UI_MSG_TP_CLICK);
	}


	if (bConnect && !CMainWnd::GetInstance()->IsLoadingApp())	// 开机8秒内不响应
	{
		if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_EASY_CONNECTED)
		{
			CDlgManager::GetInstance()->ShowDlg(EasyConnected_ID, SOURCE_EASY_CONNECTED);
			if (CBluetooth::GetInstance()->IsConnected()
				&& m_nDeviceType == 0)
			{
				CBluetooth::GetInstance()->A2DPActive(TRUE);
				CBluetooth::GetInstance()->MusicPlay();
			}
		}
		else
		{
			CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_EASY_CONNECTED);
		}
	}
	else
	{
		if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_EASY_CONNECTED
			|| CSourceManager::GetInstance()->GetRearSrcID() == SOURCE_EASY_CONNECTED)
		{
			CMainWnd::GetInstance()->PostMessage(UI_MSG_SOURCE_EXIT, 0, (LPARAM)SOURCE_EASY_CONNECTED);
		}

	}
}



#include "ycapi.h"
void CEasyConnected::LaunchSrc()
{
	// 亿连运行时,关闭导航进程,防止内存不足
	ui_terminate_navigation();

	LoadEasyConnected();

	if (CBluetooth::GetInstance()->IsConnected()
		&& m_nDeviceType == 0)
	{
		CBluetooth::GetInstance()->A2DPActive(TRUE);
		CBluetooth::GetInstance()->MusicPlay();
	}

}

void CEasyConnected::StopSrc()
{
	PostMessage2EasyConnected(UM_EASYCONNET_IN, INER_QUIT, 0);

	if (CBluetooth::GetInstance()->IsConnected()
		&& m_nDeviceType == 0)
	{
		CBluetooth::GetInstance()->MusicPause();
		CBluetooth::GetInstance()->A2DPActive(FALSE);
	}
}


void CEasyConnected::OpenAudio()
{

}

void CEasyConnected::CloseAudio()
{
}

UINT CEasyConnected::GetLanguageID()
{
	// 这个表与app定义的语言ID对应起来
	UINT lang_table[] = {1033, 2052, 1028, 1036, 1049, 3082, 1031, 2070, 1025, 1054, 1041/*LAN_JAPANESE*/, 1055, 1040, 1045, 1037};
	if (sysutil::nss_get_instance()->ui_lan_id>=0 
		&& sysutil::nss_get_instance()->ui_lan_id<(sizeof(lang_table)/sizeof(lang_table[0])))
	{
		return lang_table[sysutil::nss_get_instance()->ui_lan_id];
	}

	return 1033;	// 如果ID非法,返回英语
}

BOOL CEasyConnected::LoadEasyConnected()
{
	// already loaded.
	HWND hwnd = ::FindWindow(L"EasyConnected", L"EasyConnect");
	if (hwnd != NULL)
	{
		return TRUE;
	}

	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	TCHAR szPath[MAX_PATH] = L"";
	_stprintf(szPath, L"%s\\EasyConnected\\easyconnected.exe", tzutil::GetAppPath());

	TCHAR command[256];
	CString uuid;
	CSetAboutDlg::iGOGetDeviceCode(uuid.GetBuffer(128));
	uuid.ReleaseBuffer();
	_stprintf(command, L"-s[%s] -lan[%d]", uuid, GetLanguageID());
	if (tzutil::LoadApp(szPath, command, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return TRUE;
	}

	return FALSE;
}

int CEasyConnected::GetDeviceType()
{
	return m_nDeviceType;
}

void CEasyConnected::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ID_EASYCONNECTED_AD2P)	// 连上蓝牙时,如果需要打开A2DP
	{
		CMainWnd::GetInstance()->KillTimer(TIMER_ID_EASYCONNECTED_AD2P);
		if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_EASY_CONNECTED
			&& GetDeviceType() == 0
			&& CBluetooth::GetInstance()->GetConnectStatus() == BT_CONNECT_STATUS_CONNECTED)
		{
			CBluetooth::GetInstance()->A2DPActive(TRUE);
		}
	}
}