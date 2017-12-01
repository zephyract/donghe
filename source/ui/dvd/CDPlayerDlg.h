#pragma once
#include "wceuidlg.h"
#include "SpectrumLayer.h"
#include "DvpBaseDlg.h"

class CCDPlayerDlg: public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CCDPlayerDlg, CDvpBaseDlg)

public:
	CCDPlayerDlg(void);
	~CCDPlayerDlg(void);

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	void OnKeyPlayControl(UINT key, UINT param);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);

	void OnPosChange(UINT param1, UINT param2);
	void InitLayer();
	void ShowInvalidIcon();


	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	// event handler
	void OnEvtPlayState(UINT state);
	void OnEvtCmdResponse(UINT param1, UINT param2);
	void OnEvtPBCTime();

	void ShowPlayMode();
	void ShowFileTip();

	CSpectrumLayer *m_pLayerSpectrum;

	BOOL m_bRandomClicked;
	BOOL m_bRepeatClicked;


	static void DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData);

};
WCEUI_DYNCREATE_END(CCDPlayerDlg, CDvpBaseDlg);