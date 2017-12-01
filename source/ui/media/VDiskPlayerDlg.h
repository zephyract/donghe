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
	// 更新当前播放的文件的相关信息ID3, Image等
	void UpdateMediaInfo();
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	void OnPosChange(UINT param1, UINT param2);
	void OnGraphBuilt();

	// 播放下一曲
	void PlayNext();
	void PlayPrev();

	// 播放下一碟片
	void PlayNextDisk();
	void PlayPrevDisk();


	void SetSpectrumInfo();
	// 显示歌曲数如： 1/4
	void ShowFileTip();
	// 显示随机及循环模式
	void ShowPlayMode();
	void ShowTitle();

	CSpectrumLayer *m_pLayerSpectrum;

	// 当前正在播放的文件路径
	CString m_strPlayFilePath;

};
WCEUI_DYNCREATE_END(CVDiskPlayerDlg, CDvpBaseDlg);
