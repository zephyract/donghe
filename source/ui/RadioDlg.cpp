#include "stdafx.h"
#include "RadioDlg.h"
#include "interface.h"
#include "protocol.h"
#include "wceuibase.h"
#include "tzUtil.h"
#include "SourceManager.h"
#include "config.h"
// 单位0.01MHz (对应屏幕控件的范围是0~800)
const int FM_RULER_RANGE_MIN = 8750;
const int FM_RULER_RANGE_MAX = 11200;
const int FM_RULER_RANGE = (FM_RULER_RANGE_MAX-FM_RULER_RANGE_MIN);

// 
const int AM_RULER_RANGE_MIN = 500;
const int AM_RULER_RANGE_MAX = 1800;
const int AM_RULER_RANGE = (AM_RULER_RANGE_MAX-AM_RULER_RANGE_MIN);

CRadioDlg::CRadioDlg(void)
{
	m_bPSTipVisible = FALSE;
}

CRadioDlg::~CRadioDlg(void)
{

}

#include "MainWnd.h"
void CRadioDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// RPC命令一定要成功执行才能初始化,否则有可能没有初始数据，导致不能初始化
// 	if (!m_ruler_init)
// 	{
// 		if (ERROR_SUCCESS == CRpcMCU::GetInstance()->RPC_GetRadioData(0))
// 		{
// 			OnRadioStatus();
// 		}
// 		if ( ERROR_SUCCESS == CRpcMCU::GetInstance()->RPC_GetRadioData(1))
// 		{
// 			OnRadioPresetInfo();
// 		}
// 	}

}

void CRadioDlg::InitLayers()
{
	__super::InitLayers();

	CWceUiLayer *playerPStip = GetChildByName(L"ps_tip");
	if (playerPStip)	{	playerPStip->SetText(L"PS");	}
	ShowPSTip(FALSE);


	OnRadioRegionChange();

}

void CRadioDlg::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	CAnimDlg::OnDraw(pWndBitmap, rcNeedDraw);
}

void CRadioDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_PS_TIP)
	{
		ShowPSTip(!m_bPSTipVisible);
	}
	else
	{
		__super::OnTimer(wIDEvent);
	}
}

LRESULT CRadioDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_MCU_SYSPARAMS_CHANGE && lParam == MCU_SYS_PARA::INDEX_FM)
	{
		OnRadioRegionChange();
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CRadioDlg::OnBnLongClick(CWceUiButton* pButton)
{
	int index;
	if (index=GetValueButtonIndex(pButton))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_PRESET_SAVE, index<<24);
	}
	else
	{
		__super::OnBnLongClick(pButton);
	}

}


void CRadioDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"auto_scan"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_AS, 0);	// 1可预览不存台
	}
	else if (pButton->IsEqualName(L"preview_scan"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_PS, 0);
	}
	else if (pButton->IsEqualName(L"band"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_BAND, 0);
	}
	else if (pButton->IsEqualName(L"sens"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_LOC, 0);
	}
	else if (pButton->IsEqualName(L"stereo"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_STEREO_STATE_CHANGE, 0);
	}
	else if (pButton->IsEqualName(L"mute"))
	{
		//CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_AS, 0);	// 1可预览不存台
	}
	else if (pButton->IsEqualName(L"rds"))
	{
		CDlgManager::GetInstance()->ShowDlg(CRadioRDSDlg_ID, SOURCE_FM);
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}


BOOL _is_AM_band(BYTE band)
{
	return (band == 3 || band == 4);
}

// 根据波段不同,以不同的方式格式化频点值
void _format_freq_value(LPTSTR buffer, WORD freq, BYTE band)
{
	if (buffer != NULL)
	{
		if (_is_AM_band(band))
		{
			_stprintf(buffer, L"%d", freq);
		}
		else
		{
			_stprintf(buffer, L"%d.%02d", freq/100, freq%100);
		}
	}
}

void CRadioDlg::OnRadioStatus()
{
	static BOOL isPS = FALSE;
	if (protocol::get_mcu_radio_status()->is_preview_scan())	// 正在ps
	{
		if (!isPS)	// 上一状态不是PS
		{
			isPS = TRUE;
			OnPSBegin();
		}
	}
	else	// 不在PS
	{
		if (isPS)	// 上一状态是PS
		{
			isPS = FALSE;
			OnPSEnd();
		}
	}

	CWceUiButton *pLayerST = (CWceUiButton*)GetChildByName(L"stereo");
	if (pLayerST)
	{
		pLayerST->SetStatusStyleName(protocol::get_mcu_radio_status()->is_use_stereo() ? L"stereo_down" : L"stereo_normal", CWceUiButton::STATUS_NORMAL);
		pLayerST->SetStatusStyleName(protocol::get_mcu_radio_status()->is_use_stereo() ? L"stereo_down" : L"stereo_normal", CWceUiButton::STATUS_DOWN);
	}

	CWceUiLayer *pLayerSTStatus = GetChildByName(L"stereo_status");
	if (pLayerSTStatus)
	{
		pLayerSTStatus->ShowLayer(protocol::get_mcu_radio_status()->is_stereo_status());
	}

	CWceUiButton *pLayerSens = (CWceUiButton*)GetChildByName(L"sens");
	if (pLayerSens)
	{
		pLayerSens->SetStatusStyleName(protocol::get_mcu_radio_status()->is_DX_mode() ? L"sens_normal" : L"sens_down", CWceUiButton::STATUS_NORMAL);
		pLayerSens->SetStatusStyleName(protocol::get_mcu_radio_status()->is_DX_mode() ? L"sens_normal" : L"sens_down", CWceUiButton::STATUS_DOWN);
	}

	__super::OnRadioStatus();
}

void CRadioDlg::OnRadioPresetInfo(BOOL bForceRefresh)
{
	static WORD preset_list[6];
	static BYTE band = 0xFF;
	static BYTE preset_index = 0;	// 当前选中的预设台号

	// 更新当前频点与频点单位
	MCU_RADIO_PRESET_INFO* pinfo = protocol::get_mcu_radio_presetinfo();
	TCHAR freq[16];

	// preset list 有变化时才更新
	if (bForceRefresh || memcmp(preset_list, pinfo->preset_list, sizeof(preset_list)) != 0)
	{
		for (int i=0; i<6; i++)
		{
			if (m_pLayerValue[i])
			{
				_format_freq_value(freq, pinfo->preset_list[i], pinfo->band);
				m_pLayerValue[i]->SetText(freq);
			}
		}
		memcpy(preset_list, pinfo->preset_list, sizeof(preset_list));
	}

	// band 有变化时才更新
	if (bForceRefresh || band != pinfo->band)
	{
		ShowBand(pinfo->band);
		band = pinfo->band;

		ShowStereo(band>=0 && band<=2);	// FM1, FM2, FM3时才显示
		// FM才有RDS
		CWceUiLayer* pRDS = GetChildByName(L"rds");
		if (pRDS)
		{
			pRDS->EnableLayer(band>=0 && band<=2);
		}
	}

	// 预设台号有变化时才更新
	if (bForceRefresh || preset_index != pinfo->preset_index)
	{
		RETAILMSG(1, (L"prset list ----------------- old=%d,  new=%d\n", preset_index, pinfo->preset_index));
		OnCurPresetChange(preset_index, pinfo->preset_index);
		preset_index = pinfo->preset_index;
	}

	__super::OnRadioPresetInfo(bForceRefresh);
}

void CRadioDlg::OnPSBegin()
{
	ShowPSTip(TRUE);
	SetTimer(TIMER_ID_PS_TIP, 1000);
}

void CRadioDlg::OnPSEnd()
{
	KillTimer(TIMER_ID_PS_TIP);
	ShowPSTip(FALSE);
}

void CRadioDlg::ShowPSTip(BOOL bShow)
{
	m_bPSTipVisible = bShow;
	CWceUiLayer *playerPStip = GetChildByName(L"ps_tip");

	playerPStip->SetText(L"PS");

	if (playerPStip)	{	playerPStip->ShowLayer(bShow);	}

}

const UINT MAX_SCALE_COUNT = 14; 
void _get_scale_prams(BOOL isAmBand, UINT wMinFreq, UINT wMaxFreq, UINT& scale_min, UINT& scale_max, UINT& scale_count, UINT *scale)
{	
	UINT scale_granularity;
	if (isAmBand)
	{
		scale_granularity = ((wMaxFreq-wMinFreq)/(MAX_SCALE_COUNT-2)+9)/10*10;	// 刻度间距稍微大点，让后二个刻度尽量不用
		scale_min = wMinFreq/10*10;

		scale_count = 3;
		while (scale_count++)
		{
			scale_max = scale_min+(scale_count-1)*scale_granularity;
			if (scale_max >= wMaxFreq)
			{
				break;
			}
		}
		for (UINT i=0; i<scale_count; i++)
		{
			*(scale+i) = scale_min + i*scale_granularity;
		}

		scale_min-=scale_granularity/2;	// 让刻度的范围比标尺的范围大些,这样最小和最大的标尺不会太靠边，好显示
		scale_max+=scale_granularity/2;
	}
	else
	{
		scale_granularity = (UINT)((float)(wMaxFreq-wMinFreq)/(float)(MAX_SCALE_COUNT-1)+99)/100;
		scale_min = wMinFreq/100;

		scale_count = 3;
		while (scale_count++)
		{
			scale_max = scale_min+(scale_count-1)*scale_granularity;
			if (scale_max*100 >= wMaxFreq)
			{
				break;
			}
		}

		for (UINT i=0; i<scale_count; i++)
		{
			*(scale+i) = scale_min + i*scale_granularity;
		}

		scale_min-=1;	// 让刻度的范围比标尺的范围大些,这样最小和最大的标尺不会太靠边，好显示
		scale_max+=1;
	}


}

void CRadioDlg::OnKeyPlayControl(UINT key, UINT param)
{
	BYTE byte0 = (BYTE)((param>>24) & 0xFF);

	switch (key)
	{
	case T_PREV:
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_SEEK_DOWN, 0);
		break;
	case T_NEXT:
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_SEEK_UP, 0);
		break;
	}
}

#include "SetRadioAreaDlg.h"
void CRadioDlg::OnRadioRegionChange()
{
	CWceUiLayer* pRDS = GetChildByName(L"rds");
	CWceUiLayer* pClock = GetChildByName(L"clock");

	int offset = 0;
	if (pRDS && pClock)	// 只要存在RDS钮时才需调整UI
	{
		CRect rc;
		pRDS->GetStyle()->GetPosition(rc);
		offset = rc.Width();

		if (config::get_config_misc()->rds	// 配置文件支持RDS
			&& (protocol::get_mcu_sys_para()->get_fm_cur_region() == FQ_EUROPE
				|| protocol::get_mcu_sys_para()->get_fm_cur_region() == FQ_SAMER1))	// 北美
		{
			if (!pRDS->GetStyle()->IsVisible())
			{
				pRDS->ShowLayer(TRUE);

				pClock->GetStyle()->GetPosition(rc);
				rc.OffsetRect(-offset, 0);
				pClock->MoveLayer(rc);
			}

			if (protocol::get_mcu_sys_para()->get_fm_cur_region() == FQ_SAMER1)
			{
				((CWceUiButton*)pRDS)->SetStatusStyleName(L"rbds_normal", CWceUiButton::STATUS_NORMAL);
				((CWceUiButton*)pRDS)->SetStatusStyleName(L"rbds_down", CWceUiButton::STATUS_DOWN);
			}
			else
			{
				((CWceUiButton*)pRDS)->SetStatusStyleName(L"rds_normal", CWceUiButton::STATUS_NORMAL);
				((CWceUiButton*)pRDS)->SetStatusStyleName(L"rds_down", CWceUiButton::STATUS_DOWN);
			}
		}
		else
		{
			if (pRDS->GetStyle()->IsVisible())
			{
				pRDS->ShowLayer(FALSE);

				pClock->GetStyle()->GetPosition(rc);
				rc.OffsetRect(offset, 0);
				pClock->MoveLayer(rc);
			}
		}
	}

	// TA, AF
	CWceUiLayer* pTA = GetLayerByName(L"TA_status");
	CWceUiLayer* pAF = GetLayerByName(L"AF_status");
	if (pTA)
	{
		pTA->ShowLayer(/*protocol::get_mcu_sys_para()->get_fm_cur_region() == FQ_EUROPE*/FALSE);
	}

	if (pAF)
	{
		pAF->ShowLayer(/*protocol::get_mcu_sys_para()->get_fm_cur_region() == FQ_EUROPE*/FALSE);
	}

	// 如果当前FM的活动窗口是RDS,收音区域又是非欧洲,活动窗口应设为radio dlg
	if (protocol::get_mcu_sys_para()->get_fm_cur_region() != FQ_EUROPE)
	{
		DLG_INFO* pdi = CDlgManager::GetInstance()->GetActiveDlgInfo(SOURCE_FM);
		if (pdi && pdi->dlg_id == CRadioRDSDlg_ID)
		{
			CDlgManager::GetInstance()->SetActiveDlg(CRadioDlg_ID, SOURCE_FM);
		}
	}
}

//////////////////////////////// CRadioScrollLayer //////////////////////////////


CRadioScrollLayer::CRadioScrollLayer()
{

}

CRadioScrollLayer::~CRadioScrollLayer()
{

}

void CRadioScrollLayer::OnTimer(WORD wIDEvent)
{
	__super::OnTimer(wIDEvent);
}

void CRadioScrollLayer::OnInitLayer()
{

	__super::OnInitLayer();
}

void CRadioScrollLayer::SetProperties(PCWceUiXmlElem pCreateElem)
{
	__super::SetProperties(pCreateElem);
}

void CRadioScrollLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);
}

void CRadioScrollLayer::OnScrollWindow(int nAmount)
{
	__super::OnScrollWindow(nAmount);
	((CRadioBaseDlg*)GetParent())->RulerPosChange(this, FALSE); 
}

void CRadioScrollLayer::OnScrollWindowEnd()
{
	__super::OnScrollWindowEnd();

	((CRadioDlg*)GetParent())->RulerPosChange(this, TRUE); 

}

void CRadioScrollLayer::OnLButtonDown(UINT nFlags,  POINT point)
{
	ui_mcu_beep();
	__super::OnLButtonDown(nFlags, point);
}

//////////////////////////////// CRadioScaleLayer //////////////////////////////


CRadioScaleLayer::CRadioScaleLayer()
{
	m_minPos = 0;
	m_maxPos = 0;
	m_minFreq = 0;
	m_maxFreq = 0;
	m_scale_count = 0;
}

CRadioScaleLayer::~CRadioScaleLayer()
{

}


void CRadioScaleLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	CRect rc;
	GetWindowRect(rc);
	CRect rc_scale(rc);
	TCHAR szScale[8];

	BYTE *pBytes = NULL;
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	dc.SelectObject(pWndBitmap->GetHBitmap(pBytes));
	dc.SelectObject(GetStyle()->GetFontCtrl()->GetFont());
	dc.SetTextColor(GetStyle()->GetTextColor());
	dc.SetBkMode(TRANSPARENT);

	if (m_maxPos == m_minPos)
	{
		return;			// 非法数据
	}

	for (UINT i=0; i<m_scale_count; i++)
	{
		rc_scale.left = rc.left + rc.Width() * (m_scale[i] - m_minPos) / (m_maxPos - m_minPos) - 40;	// 向左偏一点
		rc_scale.right = rc_scale.left + 80;
		_stprintf(szScale, L"%d", m_scale[i]);
		if (/*m_scale[i]>=m_minFreq && m_scale[i]<=m_maxFreq*/1)	// 不区分颜色
		{
			/*dc.SetTextColor(GetStyle()->GetTextColor());*/
			dc.DrawText(szScale, -1, rc_scale, GetStyle()->GetDrawTextCtrl()->GetFormat());
		}
		else	// 如果标尺超出了有效频点的范围,用较淡的颜色显示
		{
			dc.SetTextColor(RGB(0x50,0x50,0x50));
			dc.DrawText(szScale, -1, rc_scale, GetStyle()->GetDrawTextCtrl()->GetFormat());
		}
	}
}

void CRadioScaleLayer::SetRange(UINT min, UINT max)
{
	m_minPos = min;
	m_maxPos = max;
}

void CRadioScaleLayer::SetScale(UINT* pscale, UINT count)
{
	if (pscale && count<=MAX_SCALE_COUNT)
	{
		memcpy(m_scale, pscale, count*sizeof(UINT));
		m_scale_count = count;
	}
}

void CRadioScaleLayer::SetFreqRange(UINT min, UINT max)
{
	m_minFreq = min;
	m_maxFreq = max;
}

///////////////////// CRadioBaseDlg ///////////////////////////////////
CRadioBaseDlg::CRadioBaseDlg(void)
: m_bTASeekVisible(FALSE)
, m_bAFSeekVisible(FALSE)
, m_bPTYSearchVisible(FALSE)
{
	m_is_TA_seek = FALSE;
	m_is_AF_seek = FALSE;
	m_is_PTY_search = FALSE;

	m_ruler_min_freq = 0;
	m_ruler_max_freq = 0;
	m_ruler_init = FALSE;
}

CRadioBaseDlg::~CRadioBaseDlg(void)
{

}

void CRadioBaseDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_MCU_RADIO_DATA);
	RegisterMsg(UI_MSG_MCU_SYSPARAMS_CHANGE);
	RegisterMsg(UI_MSG_MCU_DIGITAL_KEY);

	InitLayers();
	OnRDSStatusChange();	// 更新RDS状态信息

	//*--
	OnRadioPresetInfo(TRUE);	// 没有初始化过的话，不要主动调这个接口，有可能还没收到过MCU的收音数据

	__super::OnInitDialog();
}

void CRadioBaseDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_SHOW_TA_SEEK)
	{
		ShowTASeek(!m_bTASeekVisible);
	}
	else if (wIDEvent == TIMER_ID_SHOW_AF_SEEK)
	{
		ShowAFSeek(!m_bAFSeekVisible);
	}
	else if (wIDEvent == TIMER_ID_SHOW_PTY_SEARCH)
	{
		ShowPTYSearch(!m_bPTYSearchVisible);
	}
	else
	{
		__super::OnTimer(wIDEvent);
	}
}

LRESULT CRadioBaseDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_MCU_RADIO_DATA)
	{
		if (lParam == 0)	// radio status
		{
			OnRadioStatus();
		}
		else if (lParam == 1)	// preset info
		{
			OnRadioPresetInfo();
		}
		else if (lParam == 2)	// PS, PTY信息
		{
			ShowPSPTYInfo();
		}
	}
	else if (message == UI_MSG_MCU_SYSPARAMS_CHANGE && lParam == MCU_SYS_PARA::INDEX_RDS)
	{
		OnRDSStatusChange();
	}
	else if (message == UI_MSG_MCU_DIGITAL_KEY)
	{
		OnKeyDigital((UINT)lParam);
	}
	else
	{
		return __super::OnMessage(message, wParam, lParam);
	}

	return 0L;
}

void CRadioBaseDlg::OnKeyDigital(UINT nDigitalKey)
{
	if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_FM
		&& CDlgManager::GetInstance()->GetCurDlgID() != CBluetoothDlg_ID
		&& CDlgManager::GetInstance()->GetCurDlgID() != CBTCallDlg_ID)
	{
		int index = nDigitalKey%10;
		if (index>=1 && index<=6)
		{
			CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_PRESET_LOAD, index<<24);
		}
	}
}

void CRadioBaseDlg::OnRadioStatus()
{
	// ta
	if (!tzutil::IsBoolEqual(m_is_TA_seek, protocol::get_mcu_radio_status()->is_TA_seek()))
	{
		m_is_TA_seek = protocol::get_mcu_radio_status()->is_TA_seek();
		if (m_is_TA_seek)
		{
			OnTASeekBegin();
		}
		else
		{
			OnTASeekEnd();
		}
	}

	// AF
	static BOOL m_is_AF_seek = FALSE;
	if (!tzutil::IsBoolEqual(m_is_AF_seek, protocol::get_mcu_radio_status()->is_AF_seek()))
	{
		m_is_AF_seek = protocol::get_mcu_radio_status()->is_AF_seek();
		if (m_is_AF_seek)
		{
			OnAFSeekBegin();
		}
		else
		{
			OnAFSeekEnd();
		}
	}

	// pty
	static BOOL m_is_PTY_search = FALSE;
	if (!tzutil::IsBoolEqual(m_is_PTY_search, protocol::get_mcu_radio_status()->is_PTY_seek()))
	{
		m_is_PTY_search = protocol::get_mcu_radio_status()->is_PTY_seek();
		if (m_is_PTY_search)
		{
			OnPTYSearchBegin();
		}
		else
		{
			OnPTYSearchEnd();
		}
	}
}

static void _refresh_led()
{
	if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_FM)
	{
		MCU_RADIO_PRESET_INFO* pinfo = protocol::get_mcu_radio_presetinfo();

		TCHAR buffer[16];
		_format_freq_value(buffer, pinfo->cur_freq, pinfo->band);

		LPCTSTR led_show = ui_led_fm_data(pinfo->band, buffer, pinfo->preset_index);
		ui_led_set_showdata(led_show);
		CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
	}
}

void CRadioBaseDlg::OnRadioPresetInfo(BOOL bForceRefresh)
{
	MCU_RADIO_PRESET_INFO* pinfo = protocol::get_mcu_radio_presetinfo();
	_refresh_led();	//更新LED屏
	
	// 只要频点有变化了,就要初始化ruler
	if (bForceRefresh || !m_ruler_init || (m_ruler_min_freq != protocol::get_mcu_radio_status()->min_freq)
		|| (m_ruler_max_freq != protocol::get_mcu_radio_status()->max_freq))
	{
		m_ruler_init = TRUE;
		m_ruler_min_freq = protocol::get_mcu_radio_status()->min_freq;
		m_ruler_max_freq = protocol::get_mcu_radio_status()->max_freq;
		InitRuler(pinfo->band, 
			protocol::get_mcu_radio_status()->min_freq,
			protocol::get_mcu_radio_status()->max_freq,
			protocol::get_mcu_radio_status()->granularity);
	}

	// 更新标尺上的频点位置
	RefreshRuler(pinfo->cur_freq,
		protocol::get_mcu_radio_status()->min_freq,
		protocol::get_mcu_radio_status()->max_freq);

	m_pLayerFreqUnit->SetTextResID(_is_AM_band(pinfo->band) ? L"RADIO_FREQ_AM_UNIT" : L"RADIO_FREQ_FM_UNIT");

}

void CRadioBaseDlg::OnRDSStatusChange()
{
	CWceUiButton* pTA = (CWceUiButton*)GetChildByName(L"TA_status");
	CWceUiButton* pAF = (CWceUiButton*)GetChildByName(L"AF_status");
	CWceUiButton* pREG = (CWceUiButton*)GetChildByName(L"REG_status");

	if (pTA)
	{
		pTA->SetStatusStyleName(protocol::get_mcu_sys_para()->is_rds_TA_on() ? 
			L"TA_status_on" : L"TA_status_off", CWceUiButton::STATUS_NORMAL);
	}

	if (pAF)
	{
		pAF->SetStatusStyleName(protocol::get_mcu_sys_para()->is_rds_AF_on() ? 
			L"AF_status_on" : L"AF_status_off", CWceUiButton::STATUS_NORMAL);
	}

	if (pREG)
	{
		pREG->SetStatusStyleName(protocol::get_mcu_sys_para()->is_rds_REGION_on() 
			? L"REG_status_on" : L"REG_status_off", CWceUiButton::STATUS_NORMAL);
	}
}

void CRadioBaseDlg::ShowPSPTYInfo()
{

}


void CRadioBaseDlg::OnTASeekBegin()
{
	ShowTASeek(TRUE);
	SetTimer(TIMER_ID_SHOW_TA_SEEK, 1000);
}

void CRadioBaseDlg::OnTASeekEnd()
{
	KillTimer(TIMER_ID_SHOW_TA_SEEK);
	ShowTASeek(FALSE);
}

void CRadioBaseDlg::ShowTASeek(BOOL bShow)
{
	m_bTASeekVisible = bShow;
	CWceUiLayer *player = GetChildByName(L"TA_seek");
	if (player)	{	player->ShowLayer(bShow);	}
}

//
void CRadioBaseDlg::OnAFSeekBegin()
{
	ShowAFSeek(TRUE);
	SetTimer(TIMER_ID_SHOW_AF_SEEK, 1000);
}

void CRadioBaseDlg::OnAFSeekEnd()
{
	KillTimer(TIMER_ID_SHOW_AF_SEEK);
	ShowAFSeek(FALSE);
}

void CRadioBaseDlg::ShowAFSeek(BOOL bShow)
{
	m_bAFSeekVisible = bShow;
	CWceUiLayer *player = GetChildByName(L"AF_seek");
	if (player)	{	player->ShowLayer(bShow);	}
}


void CRadioBaseDlg::OnPTYSearchBegin()
{
	ShowPTYSearch(TRUE);
	SetTimer(TIMER_ID_SHOW_PTY_SEARCH, 1000);
}

void CRadioBaseDlg::OnPTYSearchEnd()
{
	KillTimer(TIMER_ID_SHOW_PTY_SEARCH);
	ShowPTYSearch(FALSE);
}

void CRadioBaseDlg::ShowPTYSearch(BOOL bShow)
{
	m_bPTYSearchVisible = bShow;
	CWceUiLayer *player = GetChildByName(L"PTY_search_tip");
	if (player)	{	player->ShowLayer(bShow);	}

	// 位置重叠的,防止出现重影,ta alarm消失的慢一些
	if (bShow)
	{
		player = GetChildByName(L"TA_alarm");
		if (player)
		{
			player->ShowLayer(FALSE);
		}
	}
}


void CRadioBaseDlg::InitLayers()
{
	m_pRuler = (CWceUiScrollLayer*)GetChildByName(L"ruler");
	m_pLayerCurFreq = GetLayerByName(L"cur_freq");
	ASSERT(m_pLayerCurFreq);
	m_pLayerFreqUnit = GetLayerByName(L"cur_freq_unit");
	ASSERT(m_pLayerFreqUnit);

	CWceUiLayer *player = GetChildByName(L"TA_seek");
	if (player)
	{
		player->SetText(L"TA Seek");
		player->ShowLayer(FALSE);
	}

	player = GetChildByName(L"AF_seek");
	if (player)
	{
		player->SetText(L"PI Seek");
		player->ShowLayer(FALSE);
	}

	player = GetChildByName(L"PTY_search_tip");
	if (player)
	{
		player->SetText(L"PTY Search");
		player->ShowLayer(FALSE);
	}
	
	// 
	CWceUiButton* pButton = (CWceUiButton*)GetChildByName(L"tune_dec");
	if(pButton)
	{
		pButton->EnableMultiCmd(TRUE);
	}

	pButton = (CWceUiButton*)GetChildByName(L"tune_inc");
	if(pButton)
	{
		pButton->EnableMultiCmd(TRUE);
	}

	for (int i=0; i<6; i++)
	{
		TCHAR layer_name[16];
		_stprintf(layer_name, L"value%d", i+1);
		m_pLayerValue[i] = (CWceUiButton*)GetLayerByName(layer_name);
		if (m_pLayerValue[i])
		{
			m_pLayerValue[i]->EnableLongPress(TRUE);
		}
	}

	LPCTSTR name[] = {L"auto_scan", L"preview_scan", L"seek_dec", L"seek_inc", L"band", 
		L"tune_dec",L"tune_inc",L"sens", L"stereo", L"value1", L"value2", L"value3",
		L"value4", L"value5", L"value6", L"AF_status",	L"TA_status"};
	for (int i=0; i<sizeof(name)/sizeof(name[0]); i++)
	{
		CWceUiButton* pbutton = (CWceUiButton*)GetChildByName(name[i]);
		if (pbutton)
		{
			pbutton->SetUserData(1);	// APP不响BEEP
		}
	}
}


void CRadioBaseDlg::OnBnClick(CWceUiButton* pButton)
{
	int index;
	if (pButton->IsEqualName(L"AF_status"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_AF, 0);	// 1可预览不存台
	}
	else if (pButton->IsEqualName(L"TA_status"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_TA, 0);
	}
	else if (pButton->IsEqualName(L"seek_dec"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_SEEK_DOWN, 0);
	}
	else if (pButton->IsEqualName(L"seek_inc"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_SEEK_UP, 0);
	}
	else if (pButton->IsEqualName(L"tune_dec"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_TUNING_DOWN, 0);
	}
	else if (pButton->IsEqualName(L"tune_inc"))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_TUNE_UP, 0);
	}
	else if (index=GetValueButtonIndex(pButton))
	{
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_PRESET_LOAD, index<<24);
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}


void CRadioBaseDlg::InitRuler(BYTE band, WORD wMinFreq, WORD wMaxFreq, WORD wGranularity)
{
	if (wGranularity <= 0)
	{
		RETAILMSG(1, (L"[navi_car] MCU send radio status data error. granularity could not be zero.\n"));

#pragma message( __LOC__" MCU Bug,  wGranularity is zero..." )
		// MCU有个BUG,断电启动，与MCU通信上后马上切收音,这个值会返回0，这里暂时赋下值
		protocol::get_mcu_radio_status()->granularity = 5;
		wGranularity = 5;
		//return;
	}

	UINT scale_count = 0;
	UINT scale_min = 0;
	UINT scale_max = 0;
	// 	UINT fm_scale[] = {88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 110, 112};
	// 	UINT am_scale[] = {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700};
	UINT scale[MAX_SCALE_COUNT];
	_get_scale_prams(_is_AM_band(band), wMinFreq, wMaxFreq, scale_min, scale_max, scale_count, scale);


	int ruler_range_min, ruler_range_max, ruler_range;
	if (_is_AM_band(band))
	{
		ruler_range_min = scale_min;
		ruler_range_max = scale_max;
		ruler_range = (scale_max-scale_min);
	}
	else	// fm
	{
		ruler_range_min = scale_min*100;
		ruler_range_max = scale_max*100;
		ruler_range = (scale_max-scale_min)*100;
	}

	int min = (wMinFreq - ruler_range_min) / wGranularity;
	int max = (wMaxFreq - ruler_range_min) / wGranularity;
	if (m_pRuler)
	{
		CRect rc;
		m_pRuler->GetClientRect(&rc);
		double rate = (double)rc.Width() / ((double)ruler_range/(double)wGranularity);	// 屏幕坐标与ruler表示的范围间需要映射下
		m_pRuler->SetScrollRange(0, (int)((double)(max-min)*rate));
		m_pRuler->SetScrollPos((int)((double)(max-min)*rate));

		CWceUiLayer* player = GetLayerByName(L"pointer");
		if (player)
		{
			int pos = (int)((double)(min)*rate);
			player->GetWindowRect(rc);
			int width = rc.Width();
			rc.left = pos;
			rc.right = rc.left +width;
			player->MoveLayer(rc);
		}
	}

	// 刻度
	CRadioScaleLayer* pscale = (CRadioScaleLayer*)GetChildByName(L"scale");
	if (pscale)
	{

		if (_is_AM_band(band))
		{
			pscale->SetRange(scale_min, scale_max);
			pscale->SetScale(scale, scale_count);
			pscale->SetFreqRange(wMinFreq, wMaxFreq);
		}
		else
		{
			pscale->SetRange(scale_min, scale_max);
			pscale->SetScale(scale, scale_count);
			pscale->SetFreqRange(wMinFreq/100, wMaxFreq/100);
		}
	}
}

void CRadioBaseDlg::RefreshRuler(WORD wFreq, WORD wMinFreq, WORD wMaxFreq)
{
	if (m_pRuler && (wMaxFreq != wMinFreq))	// 防止被0除
	{
		int minPos, maxPos;
		m_pRuler->GetScrollRange(&minPos, &maxPos);

		int pos = (wFreq - wMinFreq)* (maxPos-minPos)/ (wMaxFreq - wMinFreq);
		float percent = (float)abs((maxPos-pos)-m_pRuler->GetScrollPos())/(float)(maxPos-minPos);
		if (percent > 0.2f)
		{
			m_pRuler->SetScrollFrame(15);
		}
		else if (percent > 0.1)
		{
			m_pRuler->SetScrollFrame(10);
		}
		else if (percent > 0.05)
		{
			m_pRuler->SetScrollFrame(5);
		}
		else
		{
			m_pRuler->SetScrollFrame(1);
		}
		m_pRuler->SetScrollPosAnim(maxPos-pos);
	}
}

void CRadioBaseDlg::RulerPosChange(CWceUiScrollLayer* pLayer, BOOL bUpdateMCU)
{
	MCU_RADIO_PRESET_INFO* pinfo = protocol::get_mcu_radio_presetinfo();
	int minPos, maxPos;
	pLayer->GetScrollRange(&minPos, &maxPos);
	if (minPos == maxPos)
	{
		return;	// 非法数据,防止被0除
	}

	int pos = pLayer->GetScrollPos();
	WORD minFreq =	protocol::get_mcu_radio_status()->min_freq;
	WORD maxFreq = protocol::get_mcu_radio_status()->max_freq;

	WORD freq = (maxPos-pos) * (maxFreq - minFreq) / (maxPos-minPos) + minFreq;

	BYTE granularity = protocol::get_mcu_radio_status()->granularity;
	if (!_is_AM_band(pinfo->band) && abs(freq-pinfo->cur_freq)>granularity)	
	{
		// FM时,这个粒度指的是微调的,实际粒度是他的一倍,用户操作时,粒度为原始粒度
		// 协议的问题,有时MCU传上来的是真实粒度，目前暂时规避下,只有当值为5时,才认为是微调的
		if (granularity == 5)
		{
			granularity *= 2;
		}
	}
	WORD offset = freq - minFreq;
	offset = (offset+granularity/2) - (offset+granularity/2)%granularity;		// 对粒度进行四舍五入,返回的数据必须是gralularity的倍数
	freq = minFreq + offset;
	// 因为UI库的原因,频点更新时,需更新ruler的位置,更新操作会回调到这个函数(实际上这种情况不需回调这个)
	// 因为有计算误差,可能此时计算的freq与设置的freq不一样,所以这里增加一个阈值,变化大于阈值才更新数据
	if (abs(freq-pinfo->cur_freq) < granularity+1)
	{
		freq = pinfo->cur_freq;
	}

	// 更新UI的频点显示
	TCHAR buffer[16];
	_format_freq_value(buffer, freq, pinfo->band);
	if(m_pLayerCurFreq) m_pLayerCurFreq->SetText(buffer);


	if (bUpdateMCU && freq != pinfo->cur_freq)
	{
		// 频点与位置映射时会有误差,设了MCU频点后会更新ruler的位置,有可能与当前位置有误差产生细微的移动,这里提前移动,不会产生停滞感
		RefreshRuler(freq, minFreq, maxFreq);	
		CRpcMCU::GetInstance()->RPC_KeyCommand(T_RADIO_FM_FREQ, freq);
	}
}


void CRadioBaseDlg::ShowStereo(BOOL bShow)
{
	CWceUiButton *pLayerST = (CWceUiButton*)GetChildByName(L"stereo");
	if (pLayerST)
	{
		pLayerST->ShowLayer(bShow);
	}

}

void CRadioBaseDlg::OnCurPresetChange(int nOld, int nCur)
{
	TCHAR style[16];
	if (nOld>0 && nOld<=6)
	{
		_stprintf(style, L"value%d_normal", nOld);
		m_pLayerValue[nOld-1]->SetStatusStyleName(style, CWceUiButton::STATUS_NORMAL);
		_stprintf(style, L"value%d_down", nOld);
		m_pLayerValue[nOld-1]->SetStatusStyleName(style, CWceUiButton::STATUS_DOWN);
	}

	if (nCur>0 && nCur<=6)
	{
		_stprintf(style, L"value%d_check", nCur);
		m_pLayerValue[nCur-1]->SetStatusStyleName(style, CWceUiButton::STATUS_NORMAL);
		m_pLayerValue[nCur-1]->SetStatusStyleName(style, CWceUiButton::STATUS_DOWN);
	}
}

#include "SetRadioAreaDlg.h"
void CRadioBaseDlg::ShowBand(int nBand)
{
	// cur_band5, cur_band6 for Europe
	LPCTSTR style_str[] = {L"cur_band0", L"cur_band1", L"cur_band2", L"cur_band3", L"cur_band4", L"cur_band5", L"cur_band6" };
	CWceUiLayer* pLayerBand = GetLayerByName(L"cur_band");
	if (pLayerBand && nBand>=0 && nBand <5)
	{
		// 如果是欧洲地区,AM1,AM2显示为MW1,MW2
		if ( protocol::get_mcu_sys_para()->get_fm_cur_region() == FQ_EUROPE
			&& (nBand == 3 || nBand == 4))
		{
			nBand += 2;
		}

		pLayerBand->SetStyle(style_str[nBand]);
	}
}

int CRadioBaseDlg::GetValueButtonIndex(CWceUiButton *pbutton)
{
	if (pbutton)
	{
		for (int i=0; i<6; i++)
		{
			TCHAR layer_name[16];
			_stprintf(layer_name, L"value%d", i+1);
			if (pbutton->IsEqualName(layer_name))
			{
				return i+1;
			}
		}
	}

	return 0;
}