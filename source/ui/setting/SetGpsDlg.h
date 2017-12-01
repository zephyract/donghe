#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"
#include "WceUiSButton.h"

// 导航增益设为10级
#define MAX_GPS_GAIN	10

class CSetGpsDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetGpsDlg, CSetBaseDlg)
public:
	CSetGpsDlg(void);
	~CSetGpsDlg(void);	

	virtual void OnInitDialog();
	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDlgShow(BOOL bShow);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);


	void OnSBnClick(CWceUiSButton* pSButton, int status);
protected:
	void InitLayers();
	void UpdateGainValue(int value);
};
WCEUI_DYNCREATE_END(CSetGpsDlg, CSetBaseDlg);

