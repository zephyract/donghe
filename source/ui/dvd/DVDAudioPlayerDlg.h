#pragma once
#include "wceuidlg.h"
#include "SpectrumLayer.h"
#include "DvpBaseDlg.h"

#define MAX_ID3_SIZE					28

class CDVDAudioPlayerDlg: public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CDVDAudioPlayerDlg, CDvpBaseDlg)

public:
	CDVDAudioPlayerDlg(void);
	~CDVDAudioPlayerDlg(void);

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnKeyPlayControl(UINT key, UINT param);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);

	void OnPosChange(UINT param1, UINT param2);
	void InitLayer();
	void ShowInvalidIcon();

	void DVD_Play();

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	// event handler
	void OnEvtPlayState(UINT state);
	void OnEvtCmdResponse(UINT param1, UINT param2);
	void OnEvtPBCTime();

	void ShowPlayMode();
	void ShowFileTip();
	void ShowAudioID3();
	void ShowMp3Image();
	void InitCodepage();

	CSpectrumLayer *m_pLayerSpectrum;

	BOOL m_bRandomClicked;
	BOOL m_bRepeatClicked;
	static void DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData);

};
WCEUI_DYNCREATE_END(CDVDAudioPlayerDlg, CDvpBaseDlg);