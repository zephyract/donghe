#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpVideoBaseDlg.h"
#include "VolumeManager.h"

#define TIMER_ID_AUTO_HIDE		1001	
const int AUTO_HIDE_TIME = 10000;

struct VIDEO_PARAMETERS
{
	int video_brightness;			// 亮度   视频参数取值范围0 ~100
	int video_contrast;				// 对比度
	int video_hue;					// 色度
	int video_saturation;			// 饱和度
};

class CVideoParamsDlg : public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CVideoParamsDlg, CDvpVideoBaseDlg)
public:
	CVideoParamsDlg(void);
	~CVideoParamsDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnDlgShow(BOOL bShow);

	static void ShowDlg(BOOL bShow);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);

	// 更新UI上的值
	void UpdateVideoValue();
	void UpdateSlider();

	// 根据当前源的状态来获取或设置对应的参数
	void GetParamters(VIDEO_PARAMETERS* pParamters);
	void SetParamters(VIDEO_PARAMETERS* pParamters);

	void UpdateVideoParams();

};
WCEUI_DYNCREATE_END(CVideoParamsDlg, CDvpVideoBaseDlg);
