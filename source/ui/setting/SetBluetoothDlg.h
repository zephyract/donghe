#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"
#include "WceUiSButton.h"
#include "WceUiSEditBox.h"

// 导航增益设为10级
#define MAX_GPS_GAIN	10

class CSetBluetoothDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetBluetoothDlg, CSetBaseDlg)
public:
	CSetBluetoothDlg(void);
	~CSetBluetoothDlg(void);	

	virtual void OnInitDialog();
	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnReset();
	virtual void OnBnClick(CWceUiButton* pButton);


	void OnSBnClick(CWceUiSButton* pSButton, int status);
protected:
	void InitLayers();


};
WCEUI_DYNCREATE_END(CSetBluetoothDlg, CSetBaseDlg);

