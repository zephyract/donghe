// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifdef DEBUG
	#ifndef _DEBUG
		#define _DEBUG
	#endif
#endif

#ifdef _DEBUG
	#ifndef DEBUG
		#define DEBUG
	#endif
#endif

struct __WCEUIPOSITION {};
typedef __WCEUIPOSITION* WCEUIPOSITION;

#define WCEUI_BEFORE_START_POSITION ((WCEUIPOSITION)-1L)
#define WCEUI_EOS ('\0')

#define WCEUI_TOP              (0x00000000)
#define WCEUI_LEFT             (0x00000000)
#define WCEUI_CENTER           (0x00000001)
#define WCEUI_RIGHT            (0x00000002)
#define WCEUI_VCENTER          (0x00000004)
#define WCEUI_BOTTOM           (0x00000008)

typedef enum eWCEUI_DIRECTION
{
	WCEUI_D_UP =  1,    //bottom -> up
	WCEUI_D_DOWN,		//up     -> bottom
	WCEUI_D_LEFT,		//right  -> left
	WCEUI_D_RIGHT,		//left   -> right
}WCEUI_DIRECTION;

typedef enum eWCEUI_EIGHT_DIRECTION
{
	EIGHT_D_LEFT = 1,	//right	->	left
	EIGHT_D_RIGHT,		//left	->	right
	EIGHT_D_UP,			//down	->	up
	EIGHT_D_DOWN,		//up	->	down
	EIGHT_D_LR_MID,		//left-right	->	middle
	EIGHT_D_MID_LR,		//middle		->	left-right
	EIGHT_D_UD_MID,		//up-down		->	middle
	EIGHT_D_MID_UD,		//middle		->	up-down
}WCEUI_EIGHT_DIRECTION;


#ifndef offsetof
	#if _MSC_VER >= 1400
		#define offsetof(s,m)   (size_t)&reinterpret_cast<const volatile char&>((((s *)0)->m))
	#else
		#define offsetof(s,m)   (size_t)&(((s *)0)->m)
	#endif
#endif

#ifndef WCEUI_METHOD_PROLOGUE
#define WCEUI_METHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis; // avoid warning from compiler 
#endif


#ifndef ROUND
	#define ROUND(x,y) (((x)+(y-1))&~(y-1))
#endif

#ifndef ROUND2
	#define ROUND2(x) ROUND(x, 2)
#endif

#ifndef _countof
	#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#define WCEUI_DECLARE_OBJDATA_IN_CLASS(class_name)\
protected:\
	virtual void InnerCreateObjectData();


#define NO_WRITE_POINTER(class_name, member_pointer_name)\
	static size_t OFFSET_OF_##member_pointer_name##(){\
		return(offsetof(class_name, m_x##member_pointer_name));\
	}\
	inline class_name* GET_##member_pointer_name##(){\
		return m_x##member_pointer_name##.GET_##member_pointer_name();\
	}\
private:\
	struct tNO_WRITE_POINTER##class_name##member_pointer_name{\
		tNO_WRITE_POINTER##class_name##member_pointer_name(){\
			m_x##member_pointer_name = NULL;\
		}\
		inline class_name* GET_##member_pointer_name(){\
			return m_x##member_pointer_name;\
		}\
		private:\
			class_name* m_x##member_pointer_name;\
	}m_x##member_pointer_name;


typedef enum eWCEUI_DLG_RUN_WAY
{
	DLG_RUN_IN_MODELESS = 1,
	DLG_RUN_IN_MODAL,
	DLG_RUN_IN_INVALID,
}WCEUI_DLG_RUN_WAY;




HINSTANCE WceUiGetInstanceHandle(void);
int		  WceUiGetBitsPixel(void);
int		  WceUiGetScreenWidth(void);
int		  WceUiGetScreenHeight(void);

#define WCEUI_INVALID_FILE_SIZE INVALID_FILE_SIZE
#define WCEUI_INVALID_COLOR (0xFFFFFFFF)
#define WCEUI_INVALID_CONSTANTALPHA	(-1)
#define WCEUI_INVALID_TRANS_COLOR   (0xFFFFFFFF)

#define MAX_LAYER_LEVEL 64


LPVOID  WceUiReadFile(LPCTSTR pFilePath, LPDWORD lpFileSize, LPCTSTR pPassword = NULL);

//relative path is acceptable
BOOL	WceUiInit(HINSTANCE hInstance,
				  LPCTSTR pWndDir,
				  LPCTSTR pResDir,
				  LPCTSTR pLangFilePath,
				  BOOL bEscapeSlash = FALSE
				  );

//////////////////////////////////////////////////////////////////////////
/////////////////////////////WceUiLog/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
enum WCEUI_LOG_LEVEL
{
	WCEUI_LOG_INFO = 1,
	WCEUI_LOG_WARNING,
	WCEUI_LOG_ERROR,
};

//set log path and max log file size
BOOL   WceUiSetLog(LPCTSTR pLogFileDir, UINT nMaxLogFileSize);
//close log
void   WceUiCloseLog(void);
//write log
void __cdecl WceUiWriteLog(WCEUI_LOG_LEVEL errLevel, LPCTSTR lpszFormat, ...);



void   WceUiSetPackPassword(LPCTSTR pPackPassWord);
LPCTSTR WceUiGetPackPassword(void);


//relative path is acceptable
BOOL	WceUiChangeWndDir(LPCTSTR pWndDir);
BOOL	WceUiChangeResDir(LPCTSTR pResDir);
BOOL	WceUIChangeLanguage(LPCTSTR pLangFilePath, BOOL bEscapeSlash = FALSE);

//return full path
LPCTSTR WceUiGetWndDir(void);
LPCTSTR WceUiGetResDir(void);
LPCTSTR WceUiGetLangFilePath(void);


BOOL WceUiIsRectLarge(LPCRECT lpRect);

//pVersionStr can't be null and max has 128 character len
void WceUiGetVerString(TCHAR* pVersionStr);

void WceUiSetLicence(LPCTSTR pUserName, LPCTSTR pPassword);

BOOL WceUiConvertRectStrToRect(LPCTSTR pRectStr, RECT* pRect);



