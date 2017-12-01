#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"
#include "WceUiSButton.h"
#include "wceuilistbox.h"

#define MAX_COLOR_GAIN 100
#define MIN_COLOR_GAIN 10	// 防止全调为0,屏幕黑掉了

class CSetGeneralDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetGeneralDlg, CSetBaseDlg)
public:
	CSetGeneralDlg(void);
	~CSetGeneralDlg(void);	

	virtual void OnInitDialog();
	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnReset();
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnSBnClick(CWceUiSButton* pSButton, int status);

protected:
	void InitLayers();
	void InitSButtonStatus();
	void UpdateLanguage();



};
WCEUI_DYNCREATE_END(CSetGeneralDlg, CSetBaseDlg);

