// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiLayer.h"
#include "WceUiXml.h"

#define TIMER_ID_MULTI_CMD		1
#define TIMER_ID_LONG_PRESS		2

class CWceUiButton : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CWceUiButton, CWceUiLayer);
public:
	CWceUiButton();
	virtual ~CWceUiButton();

	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnLButtonDown(UINT nFlags,  POINT point);
	virtual void OnLButtonUp  (UINT nFlags,	 POINT point);
	virtual void OnMouseMove  (UINT nFlags,	 POINT point);
	virtual void OnLoseCapture();
	virtual void OnTimer(WORD wIDEvent);

	enum BUTTON_STATUS
	{
		STATUS_NORMAL = 1,
		STATUS_DOWN,
		STATUS_FOCUS,
		STATUS_DISABLE,
	};

public:
	// 设置四种状态的style
	void SetStatusStyleName(LPCTSTR lpszStyleName, BUTTON_STATUS eStatus);
	LPCTSTR GetStatusStyleName(BUTTON_STATUS) const;

	// 如果需要enable,disable按钮，调用下面的接口，会自动切换UI状态
	void EnableButton(BOOL bEnable);

	// 按住不放时，支持连续发送命令
	// 第一次触发时的时间间隔最好比较长一点，符合操作习惯
	void EnableMultiCmd(BOOL bEnable, UINT nTimeInterval = 200, UINT nFirstTimerInterval = 600);

	// 支持长按，当允许长按时， 短按或长按时，控件发送不同的命令
	// 注意：长按和连续发送命令不能同时存在
	void EnableLongPress(BOOL bEnable, UINT nTimeInterval = 900);

	void SetUserData(DWORD dwData);
	DWORD GetUserData();
protected:
	TCHAR m_szNormalStyleName[MAX_TAG_NAME_LEN];
	TCHAR m_szDownStyleName[MAX_TAG_NAME_LEN];
	TCHAR m_szFocusStyleName[MAX_TAG_NAME_LEN];
	TCHAR m_szDisableStyleName[MAX_TAG_NAME_LEN];

	void SetStatus(BUTTON_STATUS status);
	BUTTON_STATUS m_Status;

protected:
	BOOL m_bMultiCmdEnabled;		// 是否连续发送命令
	UINT m_nMultiCmdInterval;		// 时间间隔
	UINT m_nFirstMultiCmdInterval;	// 第一次时间间隔


	BOOL m_bLongPressEnabled;	// 是否支持长按
	UINT m_nLongPressInterval;	// 时间间隔
	BOOL m_bLongPressCmdSent;	// 支持长按时，如果命令已发出，置标志为TRUE

	// 长按
	BOOL m_bMouseDown;

	// 用来保存用户数据
	DWORD m_dwUserData;

private:
	CWceUiButton (const CWceUiButton &);
	CWceUiButton & operator=(const CWceUiButton &);
};

WCEUI_DYNCREATE_END(CWceUiButton, CWceUiLayer);

