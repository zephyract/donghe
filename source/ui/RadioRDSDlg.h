#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "dvpbasedlg.h"
#include "RadioDlg.h"

class CRadioRDSDlg : public CRadioBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CRadioRDSDlg, CRadioBaseDlg)
public:
	CRadioRDSDlg(void);
	~CRadioRDSDlg(void);	

	virtual void OnInitDialog();
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	virtual void OnDlgShow(BOOL bShow);
	virtual void OnKeyPlayControl(UINT key, UINT param);

	// 当因为RDS报警显示时,返回TRUE，如果是用户点击进入该界面的,返回FALSE
	static BOOL IsTopMostDlg();
	static void SetTopMostDlg(BOOL bIsTopMostDlg);

	// 返回对应的字符描述,RDS与RBDS可能不一样
	static LPCTSTR GetPTYString(int type);
	// 返回PTY的各类
	static int GetPTYCount();

protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnBnLongClick(CWceUiButton* pButton);
	void OnPTYClick();

	void OnRadioStatus();
	void OnRDSAlarm(BOOL bAlarmOn);
	void OnRadioPresetInfo(BOOL bForceRefresh = FALSE);

	// TA alarm 时显示traffic字样
	void ShowTAAlarm(BOOL bShow);

	// 根据当前台的PS,PTY信息，显示PS,PTY信息
	void ShowPSPTYInfo();

	// 显示用户选定的PTY类型
	void ShowPTYUser(int type);


	// 检测pbutton是哪个value按钮，如果是返回索引(1~6), 如果不是返回0
	int GetValueButtonIndex(CWceUiButton *pbutton);
	void InitLayers();


	static BOOL m_bIsTopMostDlg;
};
WCEUI_DYNCREATE_END(CRadioRDSDlg, CRadioBaseDlg);
