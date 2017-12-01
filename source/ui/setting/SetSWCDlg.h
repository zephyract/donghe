#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"
#include "protocol.h"

#define MAX_SWC_BUTTON 12

#define SWC_STUDY_END		0
#define SWC_STUDY_START		1
#define SWC_STUDY_SAVE		2
#define SWC_STUDY_RESET		3

class CSetSWCDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetSWCDlg, CSetBaseDlg)
public:
	CSetSWCDlg(void);
	~CSetSWCDlg(void);	

	virtual void OnInitDialog();
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDlgShow(BOOL bShow);
protected:
	void OnBnClick(CWceUiButton* pButton);

	void UpdateSWC(MCU_SWC_TABLE* psw_table);

	// 如果按已经学习过的键,提示warning, 否则显示操作提示
	void ShowWarning(BOOL bWarning);

	CWceUiButton* m_pButton[MAX_SWC_BUTTON];
};
WCEUI_DYNCREATE_END(CSetSWCDlg, CSetBaseDlg);
