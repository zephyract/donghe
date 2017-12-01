// winmain.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "winmain.h"
#include "MainWnd.h"
#include "uibase.h"
#include "tzUtil.h"

extern "C" int VCI2CInit(void);
extern "C" int VCI2CWrite(int register_address, unsigned char *buffer, int size);
extern "C" int VCI2CRead(int register_address, unsigned char*buffer, int size);
extern "C" void VCI2CDenit(void);

void OnTaskBarButtonHide() 
{
	HWND hwndTaskBar = ::FindWindow(TEXT("HHTaskBar"), NULL);
	if (hwndTaskBar)
	{
		::ShowWindow(hwndTaskBar, SW_HIDE);
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	OnTaskBarButtonHide();

	char buffer[128];
	TCHAR buffer_t[128];
	sprintf(buffer, "[car_nav_ui] UI start. Compiled in %s %s.\n", __DATE__, __TIME__);
	MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, buffer, -1, buffer_t, sizeof(buffer_t)/sizeof(TCHAR));
	RETAILMSG(1, (buffer_t));

	// 保证只有一个实例运行
	HANDLE hmutex =  CreateMutex(NULL, FALSE, _T("__car_nav_ui_mutex__"));
	if (GetLastError() != ERROR_SUCCESS)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			RETAILMSG(1, (L"[car_nav_ui] UI started already, only one instance can be loaded.\n"));
		}
		else
		{
			RETAILMSG(1, (L"[car_nav_ui] Create mutex fail, ui could not be loaded successfully.\n"));
		}
		return 0;
	}

	
	// font resource
	CString font;
	font.Format(L"%s\\msyh.ttf", tzutil::GetAppPath());
	::AddFontResource(font);
// 	font.Format(L"%s\\map.ttf", tzutil::GetAppPath());
// 	::AddFontResource(font);
//	DWORD dwret = GetLastError();
	::SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

	// data pack
	WceUiSetPackPassword(_T("abcdefg"));
	CString wnd, res, lan;
	wnd.Format(L"%s\\wnd", ui_get_res_file());
	res.Format(L"%s\\res", ui_get_res_file());
	lan.Format(L"%s\\language\\English.ini", ui_get_res_file());
	WceUiInit(hInstance, wnd, res, lan, TRUE);


	::CoInitializeEx(NULL, COINIT_MULTITHREADED);

	CMainWnd mainwnd;
	mainwnd.DoModal(WS_VISIBLE|WS_POPUP, CRect(0,0,WceUiGetScreenWidth(),WceUiGetScreenHeight()), NULL);

	::CoUninitialize();
	CloseHandle(hmutex);
	return (int) 0;
}
