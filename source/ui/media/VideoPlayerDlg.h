#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpVideoBaseDlg.h"


class CVideoPlayerDlg : public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CVideoPlayerDlg, CDvpVideoBaseDlg)
public:
	static void MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data);
	CVideoPlayerDlg(void);
	~CVideoPlayerDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	
	virtual void OnDlgShow(BOOL bShow);
	virtual void ShowVideo(BOOL bShow, RECT* prect = NULL);
	virtual void OnKeyPlayControl(UINT key, UINT param);

	void UpdateMediaInfo();
protected:
	virtual void OnAutoFullscreen();
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);

	void OnRandom();
	void OnRepeat();

	virtual void OnAnimateBegin();
	virtual void OnAnimating();
	virtual void OnAnimateEnd();

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	void OnPosChange(UINT param1, UINT param2);
	void OnGraphBuilt();
	void OnErrorOccured(UINT param1, UINT param2);

	// 显示歌曲数如： 1/4
	void ShowFileTip();
	// 显示随机及循环模式
	void ShowPlayMode();
	void ShowTipMessage(LPCTSTR message);
	void HideMessageTip();

	// 播放下一曲
	void PlayNext();
	void PlayPrev();

	void OnBnClickSubtitle();
	void OnBnClickAudio();
	void ShowInvalidIcon();

	void ShowBadfileTipInfo(BOOL bShow, LPCTSTR text_id);
	CWceUiLayer  *m_pLayerBadfileInfo;
};
WCEUI_DYNCREATE_END(CVideoPlayerDlg, CDvpVideoBaseDlg);
