#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

#define SCREEN_SAVER_WAIT_TIME1		30
#define SCREEN_SAVER_WAIT_TIME2		60
#define SCREEN_SAVER_WAIT_TIME3		180
#define SCREEN_SAVER_WAIT_TIME4		300

class CFacLanguageDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CFacLanguageDlg, CSetBaseDlg)
public:
	CFacLanguageDlg(void);
	~CFacLanguageDlg(void);	

	virtual void OnInitDialog();
	virtual void OnDlgShow(BOOL bShow);
protected:
	virtual void OnReset();
	void OnBnClick(CWceUiButton* pButton);
	BOOL OnFunctionsClick(CWceUiButton* pButton);

	void InitLayer();
	void UpdateDefaultLanguage();

	// Éè¶¨°´Å¥×´Ì¬
	void CheckButton(CWceUiButton* pButton, BOOL bCheck);

	BOOL SaveConfig();
};
WCEUI_DYNCREATE_END(CFacLanguageDlg, CSetBaseDlg);
