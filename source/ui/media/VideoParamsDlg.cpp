#include "stdafx.h"
#include "WceUiLayerCtrl.h"
#include "VideoParamsDlg.h"
#include "WceUiBase.h"
#include "MainWnd.h"
#include "uiddraw.h"
#include "SetVideoParams.h"
#include "config.h"
#include "tzUtil.h"
#include "sysUtil.h"
#include "SetGeneralDlg.h"
#include "SourceManager.h"
#include "interface.h"

CVideoParamsDlg::CVideoParamsDlg(void)
{
}

CVideoParamsDlg::~CVideoParamsDlg(void)
{

}

void CVideoParamsDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(UI_MSG_LBUTTON_DOWN);
	RegisterMsg(UI_MSG_LBUTTON_UP);

	UpdateVideoValue();
	UpdateSlider();

	SetTimer(TIMER_ID_AUTO_HIDE, AUTO_HIDE_TIME);
}

void CVideoParamsDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_AUTO_HIDE)
	{
		KillTimer(TIMER_ID_AUTO_HIDE);
		ShowDlg(FALSE);
	}

	__super::OnTimer(wIDEvent);
}

void CVideoParamsDlg::OnDlgShow(BOOL bShow)
{
// 	if (!IsAnimating() && ::GetForegroundWindow() == CMainWnd::GetInstance()->m_hWnd)	// 如果没有动画效果时,直接show or hide
// 	{
// 		CUiDDraw::GetInstance()->UpdateOverlay(bShow, 180);
// 	}
	__super::OnDlgShow(bShow);
}

void CVideoParamsDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"reset"))
	{
		if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_AVIN1 
			|| CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_AVIN2
			|| CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_TV)
		{
			sysutil::nss_get_instance()->video_aux_brightness = config::get_config_video()->aux_brightness;
			sysutil::nss_get_instance()->video_aux_contrast = config::get_config_video()->aux_contrast;
			sysutil::nss_get_instance()->video_aux_hue = config::get_config_video()->aux_hue;
			sysutil::nss_get_instance()->video_aux_saturation = config::get_config_video()->aux_saturation;
		}
		else
		{
			sysutil::nss_get_instance()->reset_video_params();
		}

		UpdateVideoValue();		// 更新UI上的数值
		UpdateSlider();
		UpdateVideoParams();	// 更新实际的视频参数

	}
	else if (pButton->IsEqualName(L"return"))
	{
		ShowDlg(FALSE);
	}

	__super::OnBnClick(pButton);
}



void CVideoParamsDlg::UpdateSlider()
{
	VIDEO_PARAMETERS params;
	GetParamters(&params);

	// brightness
	CWceUiSlider* pslider = (CWceUiSlider*)GetLayerByName(L"brightness");
	if (pslider)
	{
		pslider->SetRange(0, MAX_COLOR_GAIN-MIN_COLOR_GAIN);
		pslider->SetPos(params.video_brightness-MIN_COLOR_GAIN);
	}
	// contrast
	pslider = (CWceUiSlider*)GetLayerByName(L"contrast");
	if (pslider)
	{
		pslider->SetRange(0, MAX_COLOR_GAIN-MIN_COLOR_GAIN);
		pslider->SetPos(params.video_contrast-MIN_COLOR_GAIN);
	}
	// hue
	pslider = (CWceUiSlider*)GetLayerByName(L"hue");
	if (pslider)
	{
		pslider->SetRange(0, MAX_COLOR_GAIN-MIN_COLOR_GAIN);
		pslider->SetPos(params.video_hue-MIN_COLOR_GAIN);
	}
	// saturation
	pslider = (CWceUiSlider*)GetLayerByName(L"saturation");
	if (pslider)
	{
		pslider->SetRange(0, MAX_COLOR_GAIN-MIN_COLOR_GAIN);
		pslider->SetPos(params.video_saturation-MIN_COLOR_GAIN);
	}
}

void CVideoParamsDlg::UpdateVideoValue()
{
	VIDEO_PARAMETERS params;
	GetParamters(&params);

	// brightness
	CWceUiLayer* pvalue =  (CWceUiLayer*)GetLayerByName(L"value_brightness");
	if (pvalue)
	{
		pvalue->SetText(tzutil::Int2Ascii(params.video_brightness));
	}
	// contrast
	pvalue =  (CWceUiLayer*)GetLayerByName(L"value_contrast");
	if (pvalue)
	{
		pvalue->SetText(tzutil::Int2Ascii(params.video_contrast));
	}
	// hue
	pvalue =  (CWceUiLayer*)GetLayerByName(L"value_hue");
	if (pvalue)
	{
		pvalue->SetText(tzutil::Int2Ascii(params.video_hue));
	}
	// saturation
	pvalue =  (CWceUiLayer*)GetLayerByName(L"value_saturation");
	if (pvalue)
	{
		pvalue->SetText(tzutil::Int2Ascii(params.video_saturation));
	}

}



void CVideoParamsDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	VIDEO_PARAMETERS params;
	GetParamters(&params);	//初始化一下

	if (pslider->IsEqualName(L"brightness"))
	{
		params.video_brightness = pos+MIN_COLOR_GAIN;
	}
	else if (pslider->IsEqualName(L"contrast"))
	{
		params.video_contrast = pos+MIN_COLOR_GAIN;
	}
	else if (pslider->IsEqualName(L"hue"))
	{
		params.video_hue = pos+MIN_COLOR_GAIN;
	}
	else if (pslider->IsEqualName(L"saturation"))
	{
		params.video_saturation = pos+MIN_COLOR_GAIN;
	}

	SetParamters(&params);
	UpdateVideoValue();		// 更新UI上的数值
	UpdateVideoParams();	// 更新实际的视频参数
}

void CVideoParamsDlg::GetParamters(VIDEO_PARAMETERS* pParamters)
{
	if (!pParamters)
	{
		return;
	}

	if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_AVIN1 
		|| CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_AVIN2
		|| CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_TV)
	{
		pParamters->video_brightness = sysutil::nss_get_instance()->video_aux_brightness;
		pParamters->video_contrast = sysutil::nss_get_instance()->video_aux_contrast;
		pParamters->video_hue = sysutil::nss_get_instance()->video_aux_hue;
		pParamters->video_saturation = sysutil::nss_get_instance()->video_aux_saturation;
	}
	else
	{
		pParamters->video_brightness = sysutil::nss_get_instance()->video_brightness;
		pParamters->video_contrast = sysutil::nss_get_instance()->video_contrast;
		pParamters->video_hue = sysutil::nss_get_instance()->video_hue;
		pParamters->video_saturation = sysutil::nss_get_instance()->video_saturation;
	}

}

void CVideoParamsDlg::SetParamters(VIDEO_PARAMETERS* pParamters)
{
	if (!pParamters)
	{
		return;
	}

	if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_AVIN1 
		|| CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_AVIN2
		|| CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_TV)
	{
		sysutil::nss_get_instance()->video_aux_brightness = pParamters->video_brightness;
		sysutil::nss_get_instance()->video_aux_contrast = pParamters->video_contrast;
		sysutil::nss_get_instance()->video_aux_hue = pParamters->video_hue;
		sysutil::nss_get_instance()->video_aux_saturation = pParamters->video_saturation;
	}
	else
	{
		sysutil::nss_get_instance()->video_brightness = pParamters->video_brightness;
		sysutil::nss_get_instance()->video_contrast = pParamters->video_contrast;
		sysutil::nss_get_instance()->video_hue = pParamters->video_hue;
		sysutil::nss_get_instance()->video_saturation = pParamters->video_saturation;
	}
}


void CVideoParamsDlg::UpdateVideoParams()
{
	// AUX状态时,只有有信号并且视频显示时,才更新数据,所以用消息通知AVIN窗口自己处理
	if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_AVIN1 
		|| CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_AVIN2
		|| CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_TV)

	{
		GetWindow()->PostMessage(UI_MSG_AUX_VIDEO_PARAMS_CHANGE);
	}
	else
	{
		CSetVideoParams::setBrightness((UINT*)&sysutil::nss_get_instance()->video_brightness);
		CSetVideoParams::setContrast((UINT*)&sysutil::nss_get_instance()->video_contrast);
		CSetVideoParams::setHue((UINT*)&sysutil::nss_get_instance()->video_hue);
		CSetVideoParams::setSaturation((UINT*)&sysutil::nss_get_instance()->video_saturation);
	}
}

void CVideoParamsDlg::ShowDlg(BOOL bShow)
{
	static CWceUiDlg* pdlg = NULL;
	if (bShow)
	{
		if (pdlg)
		{
			pdlg->EndModeless();
			CMainWnd::GetInstance()->GetLayerCtrl()->DeleteLayer(pdlg);
			pdlg = NULL;
		}

		pdlg = CMainWnd::GetInstance()->GetLayerCtrl()->CreateDlg(L"media\\CVideoParamsDlg");
		if (pdlg)
		{
			pdlg->RunModeless();
			pdlg->ShowLayer(TRUE);
		}
	}
	else
	{
		if (pdlg)
		{
			pdlg->ShowLayer(FALSE);
		}
	}
}


LRESULT CVideoParamsDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IsLayerVisible())
	{
		if (message == UI_MSG_LBUTTON_DOWN)		// 重新计时
		{
			KillTimer(TIMER_ID_AUTO_HIDE);
		}
		else if (message == UI_MSG_LBUTTON_UP)
		{
			SetTimer(TIMER_ID_AUTO_HIDE, AUTO_HIDE_TIME);
		}
	}

	return __super::OnMessage(message, wParam, lParam);
}
