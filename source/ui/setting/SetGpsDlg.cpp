#include "stdafx.h"
#include "SetGpsDlg.h"
#include "tzUtil.h"
#include "sysUtil.h"
#include "interface.h"
#include "WceUiBase.h"
#include "WceUiRealWnd.h"

CSetGpsDlg::CSetGpsDlg(void)
{

}

CSetGpsDlg::~CSetGpsDlg(void)
{

}

void CSetGpsDlg::OnInitDialog()
{
	__super::OnInitDialog();

}

void CSetGpsDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		InitLayers();
	}
}

void CSetGpsDlg::InitLayers()
{
	CWceUiScrollLayer* pLayer = (CWceUiScrollLayer*)GetChildByName(L"frame");
	if (pLayer)
	{
		pLayer->SetScrollMode(SL_VERT);
		pLayer->SetExtraScroll(30);
	}

	// 关闭背景音
	CWceUiSButton* psbutton = (CWceUiSButton*)GetLayerByName(L"gain_off_switch");
	ASSERT(psbutton);
	psbutton->SetStatus(sysutil::nss_get_instance()->audio_is_gps_gain_off ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);

	// 导航路径
	CWceUiLayer *ppath = GetLayerByName(L"navi_path_show");
	if (ppath)
	{
		RECT rc;
		HFONT hfont;
		ppath->GetStyle()->GetPosition(&rc);
		hfont = ppath->GetStyle()->GetFontCtrl()->GetFont();
		TCHAR buffer[MAX_PATH] = {0, };
		ui_format_file_path(hfont, &rc, sysutil::nss_get_instance()->gps_path, buffer);
		ppath->SetText(buffer);
	}

	// 导航增益
	CWceUiSlider* pslider = (CWceUiSlider*)GetLayerByName(L"progress");
	int value = sysutil::nss_get_instance()->audio_gps_gain * MAX_GPS_GAIN / 100;
	if (pslider)
	{
		pslider->SetRange(0, MAX_GPS_GAIN);
		pslider->SetPos(value);
	}
	UpdateGainValue(value);

	// 自动运行
	psbutton = (CWceUiSButton*)GetLayerByName(L"autorun_switch");
	if(psbutton)
	{
		psbutton->SetStatus(sysutil::nss_get_instance()->gps_autorun ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);
	}


}


void CSetGpsDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"navi_path"))	// found it
	{
		CDlgManager::GetInstance()->ShowDlg(CSetGpsPathDlg_ID, SOURCE_SETTING);
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}

LRESULT CSetGpsDlg::OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam)
{
	return __super::OnCommand(uCommand, wParam, lParam);
}


LRESULT CSetGpsDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{

	return __super::OnMessage(message, wParam, lParam);
}

void CSetGpsDlg::OnSBnClick(CWceUiSButton* pSButton, int status)
{
	if (pSButton->IsEqualName(L"gain_off_switch"))
	{
		sysutil::nss_get_instance()->audio_is_gps_gain_off = (status == WCEUI_SBUTTON_ON);
	}
	else if (pSButton->IsEqualName(L"autorun_switch"))
	{
		sysutil::nss_get_instance()->gps_autorun = (status == WCEUI_SBUTTON_ON);
	}
}

void CSetGpsDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"progress"))
	{
		sysutil::nss_get_instance()->audio_gps_gain = pos*100/MAX_GPS_GAIN;
		UpdateGainValue(pos);
	}
}

void CSetGpsDlg::UpdateGainValue(int value)
{
	CWceUiLayer* pvalue = GetLayerByName(L"gain_value");
	if (pvalue)
	{
		TCHAR szBuffer[16];
		_stprintf(szBuffer, L"%d", value);
		pvalue->SetText(szBuffer);
	}
}