#pragma once

//++ winuser.h
typedef LRESULT (CALLBACK* HOOKPROC)(int code, WPARAM wParam, LPARAM lParam);

/*
* Structure used by WH_KEYBOARD_LL
*/
typedef struct tagKBDLLHOOKSTRUCT {
	DWORD   vkCode;
	DWORD   scanCode;
	DWORD   flags;
	DWORD   time;
	ULONG_PTR dwExtraInfo;
} KBDLLHOOKSTRUCT, FAR *LPKBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

/*
* Structure used by WH_MOUSE_LL
*/
typedef struct tagMSLLHOOKSTRUCT {
	POINT   pt;
	DWORD   mouseData;
	DWORD   flags;
	DWORD   time;
	ULONG_PTR dwExtraInfo;
} MSLLHOOKSTRUCT, FAR *LPMSLLHOOKSTRUCT, *PMSLLHOOKSTRUCT;

#define WH_KEYBOARD_LL      20
#define WH_MOUSE_LL         21


//-- winuser.h

typedef HHOOK   (WINAPI *_SetWindowsHookExW)(int, HOOKPROC, HINSTANCE, DWORD);
typedef LRESULT (WINAPI *_CallNextHookEx)(HHOOK, int, WPARAM, LPARAM);
typedef LRESULT (WINAPI *_UnhookWindowsHookEx)(HHOOK);

extern _SetWindowsHookExW SetWindowsHookExW;
extern _CallNextHookEx         CallNextHookEx;
extern _UnhookWindowsHookEx UnhookWindowsHookEx;

typedef BOOL (*HookLLMouse_CB)(DWORD dwUserData, WPARAM wParam, LPARAM lParam);
namespace winhook
{
	BOOL init();
	void uninit();


	// 可禁止触摸屏工作,在禁止期间,如果有鼠标点击,会发消息通知窗口
	void DisableTouchPanel();
	// 允许触摸屏工作,即正常工作状态
	void EnableTouchPanel();

	//
	void HookLLMouse(HookLLMouse_CB fnCB, DWORD dwUserData);
	void UnHookLLMouse();

	//
	void HookTouchKey(HookLLMouse_CB fnCB, DWORD dwUserData);
	void UnHookTouchKey();
}