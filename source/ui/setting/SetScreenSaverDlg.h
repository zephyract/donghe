#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

#define SCREEN_SAVER_WAIT_TIME1		30
#define SCREEN_SAVER_WAIT_TIME2		60
#define SCREEN_SAVER_WAIT_TIME3		180
#define SCREEN_SAVER_WAIT_TIME4		300

class CSetScreenSaverDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetScreenSaverDlg, CSetBaseDlg)
public:
	CSetScreenSaverDlg(void);
	~CSetScreenSaverDlg(void);	

	virtual void OnInitDialog();
protected:
	virtual void OnReset();
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	void OnBnClick(CWceUiButton* pButton);

	void InitLayer();

	// 根据当前的状态，选中相应的按钮
	void CheckButton();
};
WCEUI_DYNCREATE_END(CSetScreenSaverDlg, CSetBaseDlg);
