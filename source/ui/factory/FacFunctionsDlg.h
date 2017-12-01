#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

#define SCREEN_SAVER_WAIT_TIME1		30
#define SCREEN_SAVER_WAIT_TIME2		60
#define SCREEN_SAVER_WAIT_TIME3		180
#define SCREEN_SAVER_WAIT_TIME4		300

class CFacFunctionsDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CFacFunctionsDlg, CSetBaseDlg)
public:
	CFacFunctionsDlg(void);
	~CFacFunctionsDlg(void);	

	virtual void OnInitDialog();
	virtual void OnDlgShow(BOOL bShow);
protected:
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	void OnBnClick(CWceUiButton* pButton);
	BOOL OnFunctionsClick(CWceUiButton* pButton);

	void InitLayer();

	// Éè¶¨°´Å¥×´Ì¬
	void CheckButton(CWceUiButton* pButton, BOOL bCheck);

	BOOL SaveConfig();
};
WCEUI_DYNCREATE_END(CFacFunctionsDlg, CSetBaseDlg);
