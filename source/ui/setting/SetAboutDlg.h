#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

#define TIMER_ID_EXIT_APP	1060
#define TIMER_ID_FACTORY_SET	1061

class CSetAboutDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetAboutDlg, CSetBaseDlg)
public:
	CSetAboutDlg(void);
	~CSetAboutDlg(void);	

	virtual void OnInitDialog();

protected:
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnLButtonDown(UINT nFlags, POINT point);
	virtual void OnLButtonUp(UINT nFlags, POINT point);

	void UpdateSysinfo();

	CString m_strBoot;
	CString m_strOS;
	CString m_strApp;
	CString m_strUUID;
	CString m_strManufacture;
	CString	m_strMcuVer;
	CString m_strDVD;

	void InitSysInfo();
	int  GetFlashSize();
public:
	static BOOL iGOGetDeviceCode(TCHAR *deviceID);
	// buffer长度大于17
	void GetDVDVersion(OUT TCHAR *pDVDVersion);

};
WCEUI_DYNCREATE_END(CSetAboutDlg, CSetBaseDlg);
