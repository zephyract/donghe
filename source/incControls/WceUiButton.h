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
	// ��������״̬��style
	void SetStatusStyleName(LPCTSTR lpszStyleName, BUTTON_STATUS eStatus);
	LPCTSTR GetStatusStyleName(BUTTON_STATUS) const;

	// �����Ҫenable,disable��ť����������Ľӿڣ����Զ��л�UI״̬
	void EnableButton(BOOL bEnable);

	// ��ס����ʱ��֧��������������
	// ��һ�δ���ʱ��ʱ������ñȽϳ�һ�㣬���ϲ���ϰ��
	void EnableMultiCmd(BOOL bEnable, UINT nTimeInterval = 200, UINT nFirstTimerInterval = 600);

	// ֧�ֳ�������������ʱ�� �̰��򳤰�ʱ���ؼ����Ͳ�ͬ������
	// ע�⣺�������������������ͬʱ����
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
	BOOL m_bMultiCmdEnabled;		// �Ƿ�������������
	UINT m_nMultiCmdInterval;		// ʱ����
	UINT m_nFirstMultiCmdInterval;	// ��һ��ʱ����


	BOOL m_bLongPressEnabled;	// �Ƿ�֧�ֳ���
	UINT m_nLongPressInterval;	// ʱ����
	BOOL m_bLongPressCmdSent;	// ֧�ֳ���ʱ����������ѷ������ñ�־ΪTRUE

	// ����
	BOOL m_bMouseDown;

	// ���������û�����
	DWORD m_dwUserData;

private:
	CWceUiButton (const CWceUiButton &);
	CWceUiButton & operator=(const CWceUiButton &);
};

WCEUI_DYNCREATE_END(CWceUiButton, CWceUiLayer);

