#include "StdAfx.h"
#include "phonelink.h"
#include "MainWnd.h"
#include "sysUtil.h"
#include "interface.h"
#include "SourceManager.h"
#include "tzUtil.h"
#include "bluetooth.h"
#include "DlgManager.h"
#include "OverlayDlg.h"

extern "C" int VCI2CInit(void);
extern "C" int VCI2CWrite(unsigned char device, unsigned char register_address, unsigned char *buffer, int size);
extern "C" int VCI2CRead(unsigned char device, unsigned char register_address, unsigned char*buffer, int size);
extern "C" void VCI2CDenit(void);

CPhoneLink::CPhoneLink(void)
: m_hwnd(NULL)
, m_bIsConnected(FALSE)
, m_bInitalized(FALSE)
, m_bIsConnecting(FALSE)
{

}

CPhoneLink::~CPhoneLink(void)
{


}

CPhoneLink* CPhoneLink::GetInstance()
{
	static CPhoneLink phonelink;
	return &phonelink;
}

void CPhoneLink::ShowWindow(BOOL bShow)
{
	if (GetInstance()->m_hwnd != NULL)
	{
		::PostMessage(GetInstance()->m_hwnd, bShow ? ID_UWM_PHONELINK_START : ID_UWM_PHONELINK_STOP, 0, 0);
	}
}

void CPhoneLink::PostMessage2Phonelink(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_hwnd)
	{
		::PostMessage(m_hwnd, message, wParam, lParam);
	}
}

BOOL CPhoneLink::HandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == ID_UWM_PHONELINK_HAND_CLIENT)
	{
		RETAILMSG(1, (_T("[navi_car] -ID_UWM_PHONELINK_HAND_CLIENT \r\n")));
		CPhoneLink::GetInstance()->Initialize((HWND)wParam);
	}
	else if(message == ID_UWM_PHONELINK_CONNECTED)
	{
		RETAILMSG(1, (_T("[navi_car] -ID_UWM_PHONELINK_CONNECTED \r\n")));
		OnConnect(TRUE);
	}
	else if(message == ID_UWM_PHONELINK_DISCONNECT)
	{
		RETAILMSG(1, (_T("[navi_car] -ID_UWM_PHONELINK_DISCONNECT \r\n")));
		ShowWindow(FALSE);
		OnConnect(FALSE);
	}
	else if (message == ID_UWM_PHONELINK_EXIT)	// 点击了phonelink的退出键
	{
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
	}
	else if(message == WM_COPYDATA)
	{
		RETAILMSG(1, (_T("[navi_car] -WM_COPYDATA \r\n")));
		COPYDATASTRUCT *pcd=(COPYDATASTRUCT *)lParam;
		if (pcd && pcd->cbData == sizeof(TRANS_DATA))
		{
			memcpy(&m_data_in, pcd->lpData, pcd->cbData); 
		}
		return FALSE;	// 这个不是专用消息
	}
	else if(message == ID_UWM_PHONELINK_CLIENT_TRANS)
	{
		RETAILMSG(1, (_T("[navi_car] -ID_UWM_PHONELINK_CLIENT_TRANS \r\n")));
		if(HandlePhonelinkData())
		{
			SendDataToPhonelink();
			::PostMessage(m_hwnd, ID_UWM_PHONELINK_MAIN_TRANS, 0, 0);
		}
	}
	else if(message == ID_UWM_PHONELINK_INSTALL)
	{
		RETAILMSG(1, (_T("[navi_car] -ID_UWM_PHONELINK_INSTALL \r\n")));
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"MIS_PHONELINK_INSTALL");

	}
	else if(message == ID_UWM_PHONELINK_INSTALL_OK)
	{
		RETAILMSG(1, (_T("[navi_car] -ID_UWM_PHONELINK_INSTALL_OK \r\n")));
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"MIS_PHONELINK_INSTALL_OK");
		//display install success UI here
	}
	else if(message == ID_UWM_PHONELINK_CONNECTING)
	{
		RETAILMSG(1, (_T("[navi_car] -ID_UWM_PHONELINK_CONNECTING \r\n")));
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"MIS_PHONELINK_CONNECTING");

		// HCF特别需求,只要发起连接了,就认为连接上了，并切源
		m_bIsConnecting = TRUE;
		CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_PHONE_LINK);
	}
	else if(message == ID_UWM_PHONELINK_RESET)
	{
		RETAILMSG(1, (_T("[navi_car] -ID_UWM_PHONELINK_RESET \r\n")));
		//let the IC power off here, power off  time 100ms
		//after reset, post reset ok message
		::PostMessage(m_hwnd, ID_UWM_PHONELINK_RESET_OK, 0, 0);
	}
	else if(message == ID_UWM_PHONELINK_NO_LICENSE)
	{
		RETAILMSG(1, (_T("[navi_car] -ID_UWM_PHONELINK_NO_LICENSE \r\n")));
	}
	else if(message == ID_UWM_PHONELINK_NO_USB_DEBUG)
	{
		RETAILMSG(1, (_T("[navi_car] -ID_UWM_PHONELINK_NO_USB_DEBUG \r\n")));
	}
	else if (message == ID_UWM_PHONELINK_CONTROL_SHOW)
	{	
		RETAILMSG(1, (L"[navi_car] -ID_UWM_PHONELINK_CONTROL_SHOW w=%d, l=%d \r\n", wParam, lParam));

	}
	else if (message == ID_UWM_PHONELINK_START_DRIVER_APP)
	{	
		RETAILMSG(1, (_T("[navi_car] -ID_UWM_PHONELINK_START_DRIVER_APP \r\n")));
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

void CPhoneLink::SendDataToPhonelink()
{  
	if(m_hwnd)
	{
		COPYDATASTRUCT cpd;  
		cpd.cbData = sizeof(m_data_out);
		cpd.lpData = &m_data_out;   
		// 向指定窗口发送WM_COPYDATA消息，不能用PostMessage方式发送  
		SendMessage(m_hwnd, WM_COPYDATA, NULL, (LPARAM)&cpd);  
	}
}

void CPhoneLink::OnConnect(BOOL bConnect)
{
	m_bIsConnected = bConnect;
	m_bIsConnecting = FALSE;

	CMainWnd::GetInstance()->RestartScreenSaver();
	if (sysutil::nss_get_instance()->sys_status == UISS_BACKLIGHT_OFF)  // 如果关屏了,打开屏幕
	{
		CMainWnd::GetInstance()->PostMessage(UI_MSG_TP_CLICK);
	}


	if (bConnect && !CMainWnd::GetInstance()->IsLoadingApp())	// 开机8秒内不响应
	{
		CDlgManager::GetInstance()->ShowDlg(PhoneLink_ID, SOURCE_PHONE_LINK);
	}
	else
	{
		if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_PHONE_LINK
			|| CSourceManager::GetInstance()->GetRearSrcID() == SOURCE_PHONE_LINK)
		{
			CMainWnd::GetInstance()->PostMessage(UI_MSG_SOURCE_EXIT, 0, (LPARAM)SOURCE_PHONE_LINK);
		}

	}

}

BOOL CPhoneLink::Initialize(HWND hWnd)
{
	if (!m_bInitalized)
	{
		m_bInitalized = TRUE;
		VCI2CInit();
		m_hwnd = hWnd;
		::PostMessage(HWND_BROADCAST, ID_UWM_PHONELINK_HAND_MAIN, (WPARAM)CMainWnd::GetInstance()->m_hWnd, 0);
	}

	return TRUE;
}



void CPhoneLink::Uninitialize()
{
	if (m_bInitalized)
	{
		m_bInitalized = FALSE;
		m_hwnd = NULL;
		VCI2CDenit();
	}
}

#include "ycapi.h"
void CPhoneLink::LaunchSrc()
{
	LoadPhoneLink();

	if (CBluetooth::GetInstance()->IsConnected())
	{
		CBluetooth::GetInstance()->A2DPActive(TRUE);
		CBluetooth::GetInstance()->MusicPlay();
	}

}

void CPhoneLink::StopSrc()
{
 	PostMessage2Phonelink(ID_UWM_PHONELINK_QUIT, 0, 0);
 	Uninitialize();

	m_bIsConnecting = FALSE;
	m_bIsConnected = FALSE;

	if (CBluetooth::GetInstance()->IsConnected())
	{
		CBluetooth::GetInstance()->MusicPause();
		CBluetooth::GetInstance()->A2DPActive(FALSE);
	}
}


void CPhoneLink::OpenAudio()
{

}

void CPhoneLink::CloseAudio()
{
}

BOOL CPhoneLink::IsConnected()
{
	return m_bIsConnected;
}

BOOL CPhoneLink::IsConnecting()
{
	return m_bIsConnecting;
}

BOOL CPhoneLink::LoadPhoneLink()
{
	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	TCHAR szPath[MAX_PATH] = L"";
	_stprintf(szPath, L"%s\\Phonelink\\%s", tzutil::GetAppPath(), PHONELINK_APP_NAME);

	if (CreatePhoneLinkProcess(szPath, PHONELINK_RUN_PW_STRING, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return TRUE;
	}

	return FALSE;
}

BOOL CPhoneLink::HandlePhonelinkData()
{
	BOOL ret = TRUE;
	if(m_data_in.type == TRANS_READ)
	{
		//VCI2CRead(m_data_in.device_addr + 1, m_data_in.sub_addr, m_data_out.buf, m_data_in.count);
		m_data_out.type = TRANS_READ_FEEDBACK;
		m_data_out.state = TRANS_STATE_SUCCESS;
		m_data_out.count = m_data_in.count;
		m_data_out.device_addr = m_data_in.device_addr;
		m_data_out.sub_addr = m_data_in.sub_addr;
	}
	else if(m_data_in.type == TRANS_WRITE)
	{
		//VCI2CWrite(m_data_in.device_addr, m_data_in.sub_addr, m_data_in.buf, m_data_in.count);
		m_data_out.type = TRANS_WRITE_FEEDBACK;
		m_data_out.state = TRANS_STATE_SUCCESS;
		m_data_out.count = m_data_in.count;
		m_data_out.device_addr = m_data_in.device_addr;
		m_data_out.sub_addr = m_data_in.sub_addr;
	}
	else
	{
		ret = FALSE;
	}
	return ret;
}

