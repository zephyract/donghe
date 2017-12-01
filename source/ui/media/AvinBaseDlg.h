#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpVideoBaseDlg.h"

class CAvinBaseDlg : public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CAvinBaseDlg, CDvpVideoBaseDlg)
public:
	CAvinBaseDlg(void);
	~CAvinBaseDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	
	virtual void OnDlgShow(BOOL bShow);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);


};
WCEUI_DYNCREATE_END(CAvinBaseDlg, CDvpVideoBaseDlg);
