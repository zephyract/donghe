#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

class CSettingDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSettingDlg, CSetBaseDlg)
public:
	CSettingDlg(void);
	~CSettingDlg(void);	

	virtual void OnInitDialog();
	
protected:
	virtual void OnBnClick(CWceUiButton* pButton);

};
WCEUI_DYNCREATE_END(CSettingDlg, CSetBaseDlg);

///////////////// CTimeShowLayer ///////////////////

class CTimeShowLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CTimeShowLayer, CWceUiLayer)
public:
	CTimeShowLayer(void);
	~CTimeShowLayer(void);	

	virtual void OnTimer(WORD wIDEvent);
	virtual void OnInitLayer();
protected:
	void UpdateTime();

};
WCEUI_DYNCREATE_END(CTimeShowLayer, CWceUiLayer);

