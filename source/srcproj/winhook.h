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


	// �ɽ�ֹ����������,�ڽ�ֹ�ڼ�,����������,�ᷢ��Ϣ֪ͨ����
	void DisableTouchPanel();
	// ������������,����������״̬
	void EnableTouchPanel();

	//
	void HookLLMouse(HookLLMouse_CB fnCB, DWORD dwUserData);
	void UnHookLLMouse();

	//
	void HookTouchKey(HookLLMouse_CB fnCB, DWORD dwUserData);
	void UnHookTouchKey();
}