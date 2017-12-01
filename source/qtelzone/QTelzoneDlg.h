#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "AnimDlg.h"

#define TIMER_ID_REFRESH_ZONE	1200

class CQTelzoneDlg : public CAnimDlg
{
	WCEUI_DYNCREATE_BEGIN(CQTelzoneDlg, CAnimDlg)
public:
	CQTelzoneDlg(void);
	~CQTelzoneDlg(void);	

	virtual void OnInitDialog();
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnTimer(WORD wIDEvent);


protected:
	void InitLayers();

	void ShowTelZone();
	// key pad
	void ShowKeypad(BOOL bShow);
	void OnKeyEnter();
	BOOL OnBnKeypadClick(CWceUiButton* pButton);
	CWceUiLayer* m_pLayerFrameKeypad;

};
WCEUI_DYNCREATE_END(CQTelzoneDlg, CAnimDlg);

