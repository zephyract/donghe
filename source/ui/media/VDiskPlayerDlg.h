#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "WceUiSlider.h"
#include "DvpVideoBaseDlg.h"
#include "LyricParser.h"
#include "lyriceditbox.h"
#include "spectrumlayer.h"

class CVDiskPlayerDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CVDiskPlayerDlg, CDvpBaseDlg)
public:
	static void MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data);
	CVDiskPlayerDlg(void);
	~CVDiskPlayerDlg(void);	

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

	// ������һ��
	void PlayNext();
	void PlayPrev();

	// ������һ��Ƭ
	void PlayNextDisk();
	void PlayPrevDisk();


	void SetSpectrumInfo();
	// ��ʾ�������磺 1/4
	void ShowFileTip();
	// ��ʾ�����ѭ��ģʽ
	void ShowPlayMode();
	void ShowTitle();

	CSpectrumLayer *m_pLayerSpectrum;

	// ��ǰ���ڲ��ŵ��ļ�·��
	CString m_strPlayFilePath;

};
WCEUI_DYNCREATE_END(CVDiskPlayerDlg, CDvpBaseDlg);
