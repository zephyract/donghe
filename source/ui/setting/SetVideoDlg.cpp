#include "stdafx.h"
#include "SetVideoDlg.h"
#include "tzUtil.h"
#include "sysUtil.h"
#include "interface.h"
#include "WceUiBase.h"
#include "WceUiRealWnd.h"
#include "SetVideoParams.h"
#include "config.h"
#include "SetGeneralDlg.h"
CSetVideoDlg::CSetVideoDlg(void)
{

}

CSetVideoDlg::~CSetVideoDlg(void)
{

}

void CSetVideoDlg::OnInitDialog()
{
	__super::OnInitDialog();


	InitLayers();
}

void CSetVideoDlg::InitLayers()
{
	CWceUiScrollLayer* pLayer = (CWceUiScrollLayer*)GetChildByName(L"frame");
	if (pLayer)
	{
		pLayer->SetScrollMode(SL_VERT);
		pLayer->SetExtraScroll(30);
		pLayer->SetScrollRange(0, 348);
	}

	UpdateVideoValue();
	UpdateSlider();

	// 根据配置文件是否显示该配置项
	CWceUiLayer *parking_layer = GetLayerByName(L"parking");
	if (parking_layer)
	{
		parking_layer->ShowLayer(config::get_config_misc()->parking);
	}


	// 后视镜像
	CWceUiSButton* psbutton = (CWceUiSButton*)GetLayerByName(L"camera_switch");
	ASSERT(psbutton);
	psbutton->SetStatus(sysutil::nss_get_instance()->video_rear_camera_mirror ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);

	// 是否行车禁止视频
	psbutton = (CWceUiSButton*)GetLayerByName(L"parking_switch");
	ASSERT(psbutton);
	psbutton->SetStatus(sysutil::nss_get_instance()->video_parking ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);

}

void CSetVideoDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		UpdateVideoValue();
		UpdateSlider();
	}
}

void CSetVideoDlg::OnReset()
{
	InitLayers();
}

void CSetVideoDlg::UpdateSlider()
{
	// brightness
	CWceUiSlider* pslider = (CWceUiSlider*)GetLayerByName(L"brightness");
	if (pslider)
	{
		pslider->SetRange(0, MAX_COLOR_GAIN - MIN_COLOR_GAIN);
		pslider->SetPos(sysutil::nss_get_instance()->video_brightness - MIN_COLOR_GAIN);
	}
	// contrast
	pslider = (CWceUiSlider*)GetLayerByName(L"contrast");
	if (pslider)
	{
		pslider->SetRange(0, MAX_COLOR_GAIN - MIN_COLOR_GAIN);
		pslider->SetPos(sysutil::nss_get_instance()->video_contrast - MIN_COLOR_GAIN);
	}
	// hue
	pslider = (CWceUiSlider*)GetLayerByName(L"hue");
	if (pslider)
	{
		pslider->SetRange(0, MAX_COLOR_GAIN - MIN_COLOR_GAIN);
		pslider->SetPos(sysutil::nss_get_instance()->video_hue - MIN_COLOR_GAIN);
	}
	// saturation
	pslider = (CWceUiSlider*)GetLayerByName(L"saturation");
	if (pslider)
	{
		pslider->SetRange(0, MAX_COLOR_GAIN - MIN_COLOR_GAIN);
		pslider->SetPos(sysutil::nss_get_instance()->video_saturation - MIN_COLOR_GAIN);
	}

	// backlight normal
	pslider = (CWceUiSlider*)GetLayerByName(L"bk_day");
	if (pslider)
	{
		pslider->SetRange(0, 100-config::get_config_video()->backlight_min);
		pslider->SetPos(sysutil::nss_get_instance()->video_backlight_normal
			- config::get_config_video()->backlight_min);
	}

	// backlight night
	pslider = (CWceUiSlider*)GetLayerByName(L"bk_night");
	if (pslider)
	{
		pslider->SetRange(0, 100-config::get_config_video()->backlight_min);
		pslider->SetPos(sysutil::nss_get_instance()->video_backlight_night
			- config::get_config_video()->backlight_min);
	}
}

void CSetVideoDlg::UpdateVideoValue()
{
	// brightness
	CWceUiLayer* pvalue =  (CWceUiLayer*)GetLayerByName(L"value_brightness");
	if (pvalue)
	{
		pvalue->SetText(tzutil::Int2Ascii(sysutil::nss_get_instance()->video_brightness));
	}
	// contrast
	pvalue =  (CWceUiLayer*)GetLayerByName(L"value_contrast");
	if (pvalue)
	{
		pvalue->SetText(tzutil::Int2Ascii(sysutil::nss_get_instance()->video_contrast));
	}
	// hue
	pvalue =  (CWceUiLayer*)GetLayerByName(L"value_hue");
	if (pvalue)
	{
		pvalue->SetText(tzutil::Int2Ascii(sysutil::nss_get_instance()->video_hue));
	}
	// saturation
	pvalue =  (CWceUiLayer*)GetLayerByName(L"value_saturation");
	if (pvalue)
	{
		pvalue->SetText(tzutil::Int2Ascii(sysutil::nss_get_instance()->video_saturation));
	}

	// backlight normal
	pvalue =  (CWceUiLayer*)GetLayerByName(L"value_bk_day");
	if (pvalue)
	{
		pvalue->SetText(tzutil::Int2Ascii(sysutil::nss_get_instance()->video_backlight_normal));
	}
	// backlight night
	pvalue =  (CWceUiLayer*)GetLayerByName(L"value_bk_night");
	if (pvalue)
	{
		pvalue->SetText(tzutil::Int2Ascii(sysutil::nss_get_instance()->video_backlight_night));
	}
}

void CSetVideoDlg::OnResetVideo(int mode)
{
	sysutil::nss_get_instance()->reset_video_params(mode);

	CSetVideoParams::setBrightness((UINT32*)&sysutil::nss_get_instance()->video_brightness);
	CSetVideoParams::setContrast((UINT32*)&sysutil::nss_get_instance()->video_contrast);
	CSetVideoParams::setHue((UINT32*)&sysutil::nss_get_instance()->video_hue);
	CSetVideoParams::setSaturation((UINT32*)&sysutil::nss_get_instance()->video_saturation);

	UpdateVideoValue();
	UpdateSlider();
}

void CSetVideoDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"btn_reset"))	// found it
	{
		OnResetVideo(0);
	}
	else if (pButton->IsEqualName(L"btn_colorful"))
	{
		OnResetVideo(1);
	}
	else if (pButton->IsEqualName(L"btn_soft"))
	{
		OnResetVideo(2);
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}

LRESULT CSetVideoDlg::OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam)
{
	return __super::OnCommand(uCommand, wParam, lParam);
}


LRESULT CSetVideoDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return __super::OnMessage(message, wParam, lParam);
}

void CSetVideoDlg::OnSBnClick(CWceUiSButton* pSButton, int status)
{
	if (pSButton->IsEqualName(L"camera_switch"))
	{
		sysutil::nss_get_instance()->video_rear_camera_mirror = (status == WCEUI_SBUTTON_ON) ? 1 : 0;
	}
	else if (pSButton->IsEqualName(L"parking_switch"))
	{
		sysutil::nss_get_instance()->video_parking = (status == WCEUI_SBUTTON_ON);
	}
}

void CSetVideoDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"brightness"))
	{
		pos += MIN_COLOR_GAIN;
		sysutil::nss_get_instance()->video_brightness = pos;
		CSetVideoParams::setBrightness(&pos);
	}
	else if (pslider->IsEqualName(L"contrast"))
	{
		pos += MIN_COLOR_GAIN;
		sysutil::nss_get_instance()->video_contrast = pos;
		CSetVideoParams::setContrast(&pos);
	}
	else if (pslider->IsEqualName(L"hue"))
	{
		pos += MIN_COLOR_GAIN;
		sysutil::nss_get_instance()->video_hue = pos;
		CSetVideoParams::setHue(&pos);
	}
	else if (pslider->IsEqualName(L"saturation"))
	{
		pos += MIN_COLOR_GAIN;
		sysutil::nss_get_instance()->video_saturation = pos;
		CSetVideoParams::setSaturation(&pos);
	}
	else if (pslider->IsEqualName(L"bk_day"))
	{
		sysutil::nss_get_instance()->video_backlight_normal = 
			pos + config::get_config_video()->backlight_min;
		ui_update_backlight();
	}
	else if (pslider->IsEqualName(L"bk_night"))
	{
		sysutil::nss_get_instance()->video_backlight_night = 
			pos + config::get_config_video()->backlight_min;
		ui_update_backlight();
	}

	UpdateVideoValue();

}
