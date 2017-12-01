#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpVideoBaseDlg.h"
#include "VolumeManager.h"

#define TIMER_ID_AUTO_HIDE		1001	
const int AUTO_HIDE_TIME = 10000;

struct VIDEO_PARAMETERS
{
	int video_brightness;			// ����   ��Ƶ����ȡֵ��Χ0 ~100
	int video_contrast;				// �Աȶ�
	int video_hue;					// ɫ��
	int video_saturation;			// ���Ͷ�
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

	// ����UI�ϵ�ֵ
	void UpdateVideoValue();
	void UpdateSlider();

	// ���ݵ�ǰԴ��״̬����ȡ�����ö�Ӧ�Ĳ���
	void GetParamters(VIDEO_PARAMETERS* pParamters);
	void SetParamters(VIDEO_PARAMETERS* pParamters);

	void UpdateVideoParams();

};
WCEUI_DYNCREATE_END(CVideoParamsDlg, CDvpVideoBaseDlg);
