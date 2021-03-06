#include "stdafx.h"
#include "WceUiBase.h"
#include "SetAudioDlg.h"
#include "SettingsAudio.h"
#include "sysUtil.h"
#include "BalanceSlider.h"
#include "wceuiloadstring.h"
#include "interface.h"
#include "WceUiLayerCtrl.h"
#include "protocol.h"

CSetAudioDlg::CSetAudioDlg(void)
: m_CurFunType(EQ_FUN_INVALID)
, m_LastDlgSourceID(INVALID_SOURCE_ID)
, m_LastDlgID(INVALID_DLG_ID)
{
	memset(m_pSliderEQ, 0, sizeof(m_pSliderEQ));
	memset(m_pButtonEQ, 0, sizeof(m_pButtonEQ));
}

CSetAudioDlg::~CSetAudioDlg(void)
{

}

void CSetAudioDlg::OnInitDialog()
{
	__super::OnInitDialog();
	RegisterMsg(UI_MSG_MCU_KEY_COMMAND);

	OnFunChange(EQ_FUN_AUDIO);

}

void CSetAudioDlg::OnDlgShow(BOOL bShow)
{
	InitLayer();	// 该窗口可以创建二个实例,二个实例都可以修改数据,所以这里要重新初始化下,保持数据同步

	if (bShow)
	{
		DLG_INFO *pdi = CDlgManager::GetInstance()->GetLastDlgInfo();
		if (pdi && (pdi->dlg_id!=CSetAudioDlg_ID || pdi->source_id!=SOURCE_EQ))
		{
			m_LastDlgID = pdi->dlg_id;
			m_LastDlgSourceID = pdi->source_id;
		}
	}

	__super::OnDlgShow(bShow);
}

void CSetAudioDlg::InitLayer()
{
	// hint
	LPCTSTR hint[10] = {L"20", L"64", L"150", L"250", L"500", L"1K", L"2K", L"4K", L"8K", L"16K"};
	TCHAR layer_name[16];
	CWceUiLayer* pHint = NULL;
	for (int i=0; i<10; i++)
	{
		_stprintf(layer_name, L"hint%02d", i+1);
		pHint = GetLayerByName(layer_name);
		if (pHint)
		{
			pHint->SetText(hint[i]);
		}
	}

	// slider
	for (int i=0; i<EQ_FREQ_COUNT; i++)
	{
		_stprintf(layer_name, L"freq%02d", i+1);
		m_pSliderEQ[i] = (CWceUiSlider*)GetLayerByName(layer_name);
		if (m_pSliderEQ[i])
		{
			m_pSliderEQ[i]->SetRange(0, MAX_EQ_LEVEL-1);
		}
	}

	// eq buttons
	// 数组顺序与EQ 类型描述保持一致, 见NAVI_SYSTEM_STATUS中关于audio_eq_type的说明
	m_pButtonEQ[0] = (CWceUiButton*)GetLayerByName(L"none");
	m_pButtonEQ[1] = (CWceUiButton*)GetLayerByName(L"rock");
	m_pButtonEQ[2] = (CWceUiButton*)GetLayerByName(L"popular");
	m_pButtonEQ[3] = (CWceUiButton*)GetLayerByName(L"live");
	m_pButtonEQ[4] = (CWceUiButton*)GetLayerByName(L"dance");
	m_pButtonEQ[5] = (CWceUiButton*)GetLayerByName(L"classic");
	m_pButtonEQ[6] = (CWceUiButton*)GetLayerByName(L"soft");
	m_pButtonEQ[7] = (CWceUiButton*)GetLayerByName(L"custom");

	ASSERT(m_pButtonEQ[0] && m_pButtonEQ[1] && m_pButtonEQ[2] && m_pButtonEQ[3] && m_pButtonEQ[4] 
	&& m_pButtonEQ[5] && m_pButtonEQ[6] && m_pButtonEQ[7]);

	UINT eqType = sysutil::nss_get_instance()->audio_eq_type;
	CheckEQButton(eqType);

	// eq value
	if (eqType == SETTING_AUDIO_EQ_CUSTOM)	// 个性
	{
		UpdateEQValues(sysutil::nss_get_instance()->audio_eq_custom);
	}
	else
	{
		UINT value[10];
		SettingsAudio::GetInstance()->GetEQValues(eqType, value);
		UpdateEQValues(value);
	}

	// subwoofer
	CheckSubwooferButton(sysutil::nss_get_instance()->audio_eq_subwoofer_enable);

	// loudness
	CWceUiSlider* pLoudness = (CWceUiSlider*)GetLayerByName(L"loudness");
	if (pLoudness)
	{
		pLoudness->SetRange(0, MAX_EQ_LOUDNESS_LEVEL-1);
		pLoudness->SetPos(sysutil::nss_get_instance()->audio_eq_loudness
			*sysutil::nss_get_instance()->audio_eq_loudness_gain);
	}

	// balance
	CBalanceSlider* pSlider = (CBalanceSlider*)GetLayerByName(L"bslider");
	if (pSlider)
	{
		POINT pt = pSlider->GetPos();
		UpdateBSlider(sysutil::nss_get_instance()->audio_eq_balance_lr - pt.x, 
			sysutil::nss_get_instance()->audio_eq_balance_fr - pt.y);
	}

}


LRESULT CSetAudioDlg::OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam)
{
	if (uCommand == UI_CMD_BSLIDER)
	{
		int x = (char)(lParam & 0xFF);
		int y = (char)((lParam>>8) & 0xFF);
		RETAILMSG(1, (L"+++++++++ x = %d , y = %d. \n", x, y));

		POINT pt = {x, y};
		UpdateBalance(pt);
		UpdateBValue(pt);

	}

	return __super::OnCommand(uCommand, wParam, lParam);
}

void CSetAudioDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"audio"))
	{
		OnFunChange(EQ_FUN_AUDIO);
	}
	else if (pButton->IsEqualName(L"balance"))
	{
		OnFunChange(EQ_FUN_BALANCE);
	}
	else if (OnBnEQTypeClick(pButton))
	{
		;
	}
	else if (pButton->IsEqualName(L"subwoofer"))
	{
		BOOL bEnable = !sysutil::nss_get_instance()->audio_eq_subwoofer_enable;
		sysutil::nss_get_instance()->audio_eq_subwoofer_enable = bEnable;
		SettingsAudio::GetInstance()->EnableSubwoofer(bEnable);
		if (bEnable)
		{
			SettingsAudio::GetInstance()->SetSubwoofer(sysutil::nss_get_instance()->audio_eq_subwoofer);
		}
		CheckSubwooferButton(bEnable);
	}
	else if (pButton->IsEqualName(L"left"))
	{
		UpdateBSlider(-1, 0);
	}
	else if (pButton->IsEqualName(L"right"))
	{
		UpdateBSlider(1, 0);
	}
	else if (pButton->IsEqualName(L"up"))
	{
		UpdateBSlider(0, 1);
	}
	else if (pButton->IsEqualName(L"down"))
	{
		UpdateBSlider(0, -1);
	}
	else if (pButton->IsEqualName(L"back"))
	{
		OnBack();
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}

void CSetAudioDlg::OnBack()
{
	// 这个窗口可能创建二个实例,处理不一样
	DLG_INFO* pdi = CDlgManager::GetInstance()->GetDlgInfo(this);
	if (pdi && pdi->source_id == SOURCE_EQ)
	{
		// lastdlg有可能因为弹出BT或倒车被改变,这里直接返回记忆的dlg
		//CDlgManager::GetInstance()->GotoLastDlg();
		// 记忆的dlg有可能是个中间状态，对应的源有可能切换到其它活动窗口了,
		// 这里需始终返回活动窗口
		DLG_INFO *last_pdi = CDlgManager::GetInstance()->GetActiveDlgInfo(m_LastDlgSourceID);
		if (last_pdi)
		{
			m_LastDlgID = last_pdi->dlg_id;
		}
		CDlgManager::GetInstance()->ShowDlg(m_LastDlgID, m_LastDlgSourceID);
	}
	else	// SOURCE_SETTING
	{
		CDlgManager::GetInstance()->ShowDlg(CSettingDlg_ID, SOURCE_SETTING);
	}
}

void CSetAudioDlg::UpdateBValue(POINT pt)
{
	CWceUiLoadString strFR(L"SET_AUDIO_BALANCE_FR");
	CWceUiLoadString strLR(L"SET_AUDIO_BALANCE_LR");
	CWceUiLayer* pLayerFR = GetLayerByName(L"fr_value");
	CWceUiLayer* pLayerLR = GetLayerByName(L"lr_value");
	CString str;
	if (pLayerFR)
	{
		str.Format(L"%s  %d", strFR.GetString(), pt.y);
		pLayerFR->SetText(str);
	}

	if (pLayerLR)
	{
		str.Format(L"%s  %d", strLR.GetString(), pt.x);
		pLayerLR->SetText(str);
	}
}

void CSetAudioDlg::UpdateBSlider(int offsetx, int offsety)
{
	CBalanceSlider* pslider = (CBalanceSlider*)GetLayerByName(L"bslider");
	if (pslider)
	{
		POINT pt = pslider->GetPos();
		pt.x += offsetx;
		pt.y += offsety;
		int abs_value_max = MAX_EQ_BALANCE_LEVEL/2;
		pt.x = max(-abs_value_max, pt.x);
		pt.x = min(abs_value_max, pt.x);
		pt.y = max(-abs_value_max, pt.y);
		pt.y = min(abs_value_max, pt.y);

		pslider->SetPos(pt);
		UpdateBalance(pt);
		UpdateBValue(pt);
	}
}

void CSetAudioDlg::UpdateBalance(POINT pt)
{
	MISC_DVP_BALANCES_T bv;
	SettingsAudio::GetInstance()->CalculatorBalance(pt, bv);
	SettingsAudio::GetInstance()->SetBalance(bv);

	sysutil::nss_get_instance()->audio_eq_balance_lr = (char)pt.x;
	sysutil::nss_get_instance()->audio_eq_balance_fr = (char)pt.y;
}

BOOL CSetAudioDlg::OnBnEQTypeClick(CWceUiButton* pButton)
{
	LPCTSTR name[EQ_TYPE_COUNT] = {L"none", L"rock", L"popular", L"live", L"dance", L"classic", L"soft", L"custom"};
	for (int i=0; i<EQ_TYPE_COUNT; i++)
	{
		if (pButton->IsEqualName(name[i]))
		{
			UINT eqType = i;
			// 设置EQ
			if (eqType == SETTING_AUDIO_EQ_CUSTOM)	// 个性
			{
				SettingsAudio::GetInstance()->SetCustomEQbyIndex(sysutil::nss_get_instance()->audio_eq_custom);
				UpdateEQValues(sysutil::nss_get_instance()->audio_eq_custom);
			}
			else
			{
				//SettingsAudio::GetInstance()->setEQ(&eqType);
				UINT value[10];
				SettingsAudio::GetInstance()->GetEQValues(eqType, value);
				SettingsAudio::GetInstance()->SetCustomEQbyIndex(value);
				UpdateEQValues(value);
			}

			// 选中EQ button
			CheckEQButton(eqType);

			sysutil::nss_get_instance()->audio_eq_type = eqType;
			GetLayerCtrl()->GetWindow()->PostMessage(UI_MSG_EQ_CHANGE, 0, (LPARAM)eqType);

			return TRUE;
		}
	}

	return FALSE;

// 	if (pButton->IsEqualName(L"rock"))
// 	{
// 		eqType = SETTING_AUDIO_EQ_ROCK;
// 	}
// 	else if (pButton->IsEqualName(L"popular"))
// 	{
// 		eqType = SETTING_AUDIO_EQ_POP;
// 	}
// 	else if (pButton->IsEqualName(L"live"))
// 	{
// 		eqType = SETTING_AUDIO_EQ_LIVE;
// 	}
// 	else if (pButton->IsEqualName(L"dance"))
// 	{
// 		eqType = SETTING_AUDIO_EQ_DANCE;
// 	}
// 	else if (pButton->IsEqualName(L"classic"))
// 	{
// 		eqType = SETTING_AUDIO_EQ_CLASSIC;
// 	}
// 	else if (pButton->IsEqualName(L"soft"))
// 	{
// 		eqType = SETTING_AUDIO_EQ_SOFT;
// 	}
// 	else if (pButton->IsEqualName(L"personal"))
// 	{
// 		eqType = SETTING_AUDIO_EQ_CUSTOM;
// 	}
// 	else if (pButton->IsEqualName(L"none"))
// 	{
// 		eqType = SETTING_AUDIO_EQ_NONE;
// 	}
// 	else
// 	{
// 		return FALSE;
// 	}


}

void CSetAudioDlg::UpdateEQValues(UINT* pvalue)
{
	if (pvalue)
	{
		for (int i=0; i<EQ_FREQ_COUNT; i++)
		{
			if (m_pSliderEQ[i])
			{
				m_pSliderEQ[i]->SetPos(pvalue[i]);
			}
		}
	}
}

void CSetAudioDlg::GetEQValues(UINT* pvalue)
{
	if (pvalue)
	{
		for (int i=0; i<EQ_FREQ_COUNT; i++)
		{
			if (m_pSliderEQ[i])
			{
				*(pvalue+i) = m_pSliderEQ[i]->GetPos();
			}
		}
	}
}

void CSetAudioDlg::OnFunChange(EQ_FUN_BUTTON_TYPE type)
{
	if (m_CurFunType != type)
	{
		m_CurFunType = type;
		CheckFunButton(type);
		ShowFuncFrame(type);
	}
}


void CSetAudioDlg::CheckFunButton(EQ_FUN_BUTTON_TYPE type)
{
	CWceUiButton* pButtonAudio = (CWceUiButton*)GetChildByName(L"audio");
	CWceUiButton* pButtonBalance = (CWceUiButton*)GetChildByName(L"balance");
	ASSERT(pButtonAudio && pButtonBalance);

	pButtonAudio->SetStatusStyleName(L"audio_normal", CWceUiButton::STATUS_NORMAL);
	pButtonBalance->SetStatusStyleName(L"balance_normal", CWceUiButton::STATUS_NORMAL);

	switch (type)
	{
	case EQ_FUN_AUDIO:
		pButtonAudio->SetStatusStyleName(L"audio_down", CWceUiButton::STATUS_NORMAL);
		break;
	case EQ_FUN_BALANCE:
		pButtonBalance->SetStatusStyleName(L"balance_down", CWceUiButton::STATUS_NORMAL);
		break;
	default:
		break;
	}

	// title
	CWceUiLayer *ptitle = GetLayerByName(L"title");
	if (ptitle)
	{
		ptitle->SetTextResID(type == EQ_FUN_AUDIO ? L"SET_EQ" : L"SET_BALANCE");
	}
}


void CSetAudioDlg::CheckEQButton(UINT type)
{
	if (type >= EQ_TYPE_COUNT)
	{
		return;
	}

	LPCTSTR style[EQ_TYPE_COUNT] = {L"none", L"rock", L"popular", L"live", L"dance", L"classic", L"soft", L"custom"};

	TCHAR style_name[16];
	for (int i=0; i<EQ_TYPE_COUNT; i++)
	{
		_stprintf(style_name, L"%s_normal", style[i]);
		if (m_pButtonEQ[i])
		{
			m_pButtonEQ[i]->SetStatusStyleName(style_name, CWceUiButton::STATUS_NORMAL);
		}
	}

	_stprintf(style_name, L"%s_down", style[type]);
	if (m_pButtonEQ[type])
	{
		m_pButtonEQ[type]->SetStatusStyleName(style_name, CWceUiButton::STATUS_NORMAL);
	}
}

void CSetAudioDlg::CheckSubwooferButton(BOOL bCheck)
{
	CWceUiButton* pbutton = (CWceUiButton*)GetLayerByName(L"subwoofer");
	pbutton->SetStatusStyleName(bCheck ? L"subwoofer_down" : L"subwoofer_normal", CWceUiButton::STATUS_NORMAL);
}



void CSetAudioDlg::ShowFuncFrame(EQ_FUN_BUTTON_TYPE type)
{
	CWceUiLayer* paudio = GetChildByName(L"audio_frame");
	CWceUiLayer* pbalance = GetChildByName(L"balance_frame");

	ASSERT(paudio && pbalance);

	paudio->ShowLayer(FALSE);
	pbalance->ShowLayer(FALSE);

	switch (type)
	{
	case EQ_FUN_AUDIO:
		paudio->ShowLayer(TRUE);
		break;
	case EQ_FUN_BALANCE:
		pbalance->ShowLayer(TRUE);
		break;

	default:
		break;
	}
}


void CSetAudioDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	// slider
	for (int i=0; i<EQ_FREQ_COUNT; i++)
	{
		if (pslider == m_pSliderEQ[i])
		{
			GetEQValues(sysutil::nss_get_instance()->audio_eq_custom);	// 读取控件slider数据
			SettingsAudio::GetInstance()->SetCustomEQbyIndex(sysutil::nss_get_instance()->audio_eq_custom);	// 设置EQ

			// 选中custom钮
			if (sysutil::nss_get_instance()->audio_eq_type != SETTING_AUDIO_EQ_CUSTOM)
			{
				sysutil::nss_get_instance()->audio_eq_type = SETTING_AUDIO_EQ_CUSTOM;
				GetLayerCtrl()->GetWindow()->PostMessage(UI_MSG_EQ_CHANGE, 0, (LPARAM)sysutil::nss_get_instance()->audio_eq_type);
				CheckEQButton(SETTING_AUDIO_EQ_CUSTOM);
			}
			return;
		}
	}

	if (pslider->IsEqualName(L"loudness"))
	{
		UINT loudness = pslider->GetPos();
		sysutil::nss_get_instance()->audio_eq_loudness = pslider->GetPos();
		sysutil::nss_get_instance()->audio_eq_loudness_gain = 1;	// 只要调动slider了,就认为打开loudless
		SettingsAudio::GetInstance()->setLoudness(&loudness);
	}

}

LRESULT CSetAudioDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_MCU_KEY_COMMAND)
	{
		int id = wParam & 0xFF;
		if (id == T_LOUDNESS || id == T_SUBWOOFER || id == K_EQ)
		{
			InitLayer();	// 刷新UI
		}
	}

	return __super::OnMessage(message, wParam, lParam);
}