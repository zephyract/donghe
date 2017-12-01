#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "WceUiSlider.h"
#include "DvpVideoBaseDlg.h"
#include "LyricParser.h"
#include "lyriceditbox.h"
#include "spectrumlayer.h"

class CMusicPlayerDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CMusicPlayerDlg, CDvpBaseDlg)
public:
	static void MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data);
	CMusicPlayerDlg(void);
	~CMusicPlayerDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDlgShow(BOOL bShow);
	virtual void OnKeyPlayControl(UINT key, UINT param);
	// ���µ�ǰ���ŵ��ļ��������ϢID3, Image��
	void UpdateMediaInfo();
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	void OnPosChange(UINT param1, UINT param2);
	void OnGraphBuilt();

	void OnRandom();
	void OnRepeat();

	// ������һ��
	void PlayNext();
	void PlayPrev();


	void ShowMp3Tag();
	void ShowMp3Image();
	void AddLyricInfo();
	void ShowLRC(UINT curDuration);
	void SetSpectrumInfo();
	// ��ʾ�������磺 1/4
	void ShowFileTip();
	// ��ʾ�����ѭ��ģʽ
	void ShowPlayMode();
	void ShowTitle();

	CLyricParser m_lyric;
	CLyricEditBox *m_pLayerLyric;
	CSpectrumLayer *m_pLayerSpectrum;

	// ��ǰ���ڲ��ŵ��ļ�·��
	CString m_strPlayFilePath;

};
WCEUI_DYNCREATE_END(CMusicPlayerDlg, CDvpBaseDlg);
