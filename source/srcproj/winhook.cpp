#include "stdafx.h"
#include "winhook.h"
#include "MainWnd.h"
#include "interface.h"
#include "sysUtil.h"

_SetWindowsHookExW SetWindowsHookExW;
_CallNextHookEx         CallNextHookEx;
_UnhookWindowsHookEx UnhookWindowsHookEx;

static HINSTANCE g_hInstDll = NULL;
static HHOOK	g_hHookDisableTP = NULL;

// 主要用来调节音量使用
static HHOOK	g_hHookLLMouseCB = NULL;
static HookLLMouse_CB g_fnHookLLMouseCB = NULL;
static DWORD g_dwHookLLMouseUserData = 0;

// 触摸按键
static HHOOK	g_hHookTouchKeyCB = NULL;
static HookLLMouse_CB g_fnHookTouchKeyCB = NULL;
static DWORD g_dwHookTouchKeyUserData = 0;


LRESULT CALLBACK DisableTPHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	if(nCode < 0)
	{
		return CallNextHookEx(g_hHookDisableTP, nCode, wParam, lParam);
	}

	// wParam为鼠标动作WM_LBUTTONDOWN, WM_LBUTTONUP, WM_MOUSEMOVE

// 	switch(wParam)
// 	{
// 	case WM_LBUTTONDOWN:
// 		break;
// 	case WM_MOUSEMOVE:
// 		break;
// 	case WM_LBUTTONUP:
// 		break;
// 	}

	static MSLLHOOKSTRUCT hs = *((PMSLLHOOKSTRUCT)lParam);
	if (sysutil::nss_get_instance()->sys_status == UISS_BACKLIGHT_OFF		// 如果关背光了,点屏幕重新打开背光
		&& wParam == WM_LBUTTONDOWN)										// 好像系统会自己触发WM_MOUSEMOVE消息，这里过滤下
	{
		::PostMessage(CMainWnd::GetInstance()->m_hWnd, UI_MSG_TP_CLICK, wParam, (LPARAM)&hs);
	}

	return TRUE;	// 返回TRUE,截获鼠标消息 
}

BOOL winhook::init()
{
	BOOL bRet = FALSE;
	g_hInstDll = LoadLibrary(TEXT("coredll.dll"));
	if (g_hInstDll)
	{
		SetWindowsHookExW  =  (_SetWindowsHookExW)GetProcAddress(g_hInstDll, TEXT("SetWindowsHookExW"));
		CallNextHookEx  =  (_CallNextHookEx)GetProcAddress(g_hInstDll, TEXT("CallNextHookEx"));
		UnhookWindowsHookEx  =  (_UnhookWindowsHookEx)GetProcAddress(g_hInstDll, TEXT("UnhookWindowsHookEx"));

		bRet = TRUE;
	}

	return bRet;
}

void winhook::uninit()
{
	if (g_hHookDisableTP)
	{
		UnhookWindowsHookEx (g_hHookDisableTP);
		g_hHookDisableTP = NULL;
	}

	if (g_hInstDll)
	{
		FreeLibrary(g_hInstDll);
		g_hInstDll = NULL;
	}
}

void winhook::DisableTouchPanel()
{
	if (!g_hHookDisableTP)
	{
		g_hHookDisableTP = SetWindowsHookExW(WH_MOUSE_LL, DisableTPHookProc, WceUiGetInstanceHandle(), 0);
	}
}

void winhook::EnableTouchPanel()
{
	if (g_hHookDisableTP)
	{
		UnhookWindowsHookEx (g_hHookDisableTP);
		g_hHookDisableTP = NULL;
	}
}

// 音量鼠标处理
LRESULT CALLBACK HookLLMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0)
	{
		return CallNextHookEx(g_hHookLLMouseCB, nCode, wParam, lParam);
	}

	// 临时代码,不知为啥二个hook时不能正常工作
	{
		static MSLLHOOKSTRUCT hs = *((PMSLLHOOKSTRUCT)lParam);
		if (sysutil::nss_get_instance()->sys_status == UISS_BACKLIGHT_OFF		// 如果关背光了,点屏幕重新打开背光
			&& wParam == WM_LBUTTONDOWN)										// 好像系统会自己触发WM_MOUSEMOVE消息，这里过滤下
		{
			::PostMessage(CMainWnd::GetInstance()->m_hWnd, UI_MSG_TP_CLICK, wParam, (LPARAM)&hs);
			return TRUE;
		}
	}



	MSLLHOOKSTRUCT hs = *((PMSLLHOOKSTRUCT)lParam);
	if(!g_fnHookLLMouseCB(g_dwHookLLMouseUserData, wParam, MAKELPARAM(hs.pt.x, hs.pt.y)))
	{
		return CallNextHookEx(g_hHookLLMouseCB, nCode, wParam, lParam);
	}
	else
	{
		return TRUE;
	}
}

void winhook::HookLLMouse(HookLLMouse_CB fnCB, DWORD dwUserData)
{
	g_fnHookLLMouseCB = fnCB;
	g_dwHookLLMouseUserData = dwUserData;
	if (!g_hHookLLMouseCB)
	{
		g_hHookLLMouseCB = SetWindowsHookExW(WH_MOUSE_LL, HookLLMouseProc, WceUiGetInstanceHandle(), 0);
	}
}

void winhook::UnHookLLMouse()
{
	if (g_hHookLLMouseCB)
	{
		UnhookWindowsHookEx(g_hHookLLMouseCB);
		g_hHookLLMouseCB = NULL;
	}
	g_dwHookLLMouseUserData = 0;
}

// 触摸按键
LRESULT CALLBACK HookTouchKeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0)
	{
		return CallNextHookEx(g_hHookTouchKeyCB, nCode, wParam, lParam);
	}

	MSLLHOOKSTRUCT hs = *((PMSLLHOOKSTRUCT)lParam);
	return g_fnHookTouchKeyCB(g_dwHookLLMouseUserData, wParam, MAKELPARAM(hs.pt.x, hs.pt.y));
}

void winhook::HookTouchKey(HookLLMouse_CB fnCB, DWORD dwUserData)
{
	g_fnHookTouchKeyCB = fnCB;
	g_dwHookTouchKeyUserData = dwUserData;
	if (!g_hHookTouchKeyCB)
	{
		g_hHookTouchKeyCB = SetWindowsHookExW(WH_MOUSE_LL, HookTouchKeyProc, WceUiGetInstanceHandle(), 0);
	}
}

void winhook::UnHookTouchKey()
{
	if (g_hHookTouchKeyCB)
	{
		UnhookWindowsHookEx(g_hHookTouchKeyCB);
		g_hHookTouchKeyCB = NULL;
	}
	g_dwHookTouchKeyUserData = 0;
}

