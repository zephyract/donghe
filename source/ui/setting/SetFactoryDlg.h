#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"
#include "WceUiSButton.h"
#include "wceuilistbox.h"

class CSetFactoryDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetFactoryDlg, CSetBaseDlg)
public:
	CSetFactoryDlg(void);
	~CSetFactoryDlg(void);	

	virtual void OnInitDialog();

protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnSBnClick(CWceUiSButton* pSButton, int status);

	void InitSButtonStatus();
	void OnSave();
	void OnSaveToSD();
	void OnLoadFromSD();
	
	// 如果参数为空，保存配置文件自己
	BOOL SaveConfig(LPCTSTR lpszFileName=NULL);
	BOOL XMLSaveFunctions(CWceUiXmlElem *pxmlElemRoot);

	void InitLayers();



};
WCEUI_DYNCREATE_END(CSetFactoryDlg, CSetBaseDlg);

