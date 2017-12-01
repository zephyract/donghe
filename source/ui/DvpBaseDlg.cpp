#include "stdafx.h"
#include "DvpBaseDlg.h"
#include "interface.h"
#include "sysUtil.h"
#include "VideoParamsDlg.h"
#include "bluetooth.h"

CDvpBaseDlg::CDvpBaseDlg(void)
{

}

CDvpBaseDlg::~CDvpBaseDlg(void)
{

}

void CDvpBaseDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_EQ_CHANGE);
	RegisterMsg(UI_MSG_BT_CONNECT_STATUS_CHANGE);

	__super::OnInitDialog();

	// time info
	m_pTimeElapse = GetLayerByName(L"time_elapse");
	m_pTimeLeft = GetLayerByName(L"time_left");

	// progress
	m_pProgressSlider = (CWceUiSlider*)GetLayerByName(L"progress");
	if (m_pProgressSlider)
	{
		m_pProgressSlider->EnalbeNotifyOnMove(FALSE);
		m_pProgressSlider->SetRange(0, 1000);
	}

	// play , pause button
	m_pLayerPlay = (CWceUiButton*)GetLayerByName(L"play");
	m_pLayerPause = (CWceUiButton*)GetLayerByName(L"pause");
	if (m_pLayerPlay)
	{
		m_pLayerPlay->ShowLayer(FALSE);
	}

	ShowEQ();


	// 创建时蓝牙可能已经连上了,蓝牙状态标识也要刷新, 但前提是BT已经初始化了
	if (CBluetooth::GetInstance()->IsInitialized())
	{
		OnConnectEvent();
	}

}


void CDvpBaseDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"eq"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetAudioDlg_ID, SOURCE_EQ);
	}
	else if (pButton->IsEqualName(L"vset"))
	{
		CVideoParamsDlg::ShowDlg(TRUE);
	}
	else if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}



void CDvpBaseDlg::ShowMediaTimeInfo(UINT curDuration, UINT outDuration)
{
	// if the parameters is invalid, don't show the time info
	if ((outDuration==0 && curDuration==0) || curDuration>outDuration)
	{
		if (m_pTimeElapse)		m_pTimeElapse->SetText(L"");
		if (m_pTimeLeft)		m_pTimeLeft->SetText(L"");
		return;
	}

	int hr, min, sec;
	int hr1, min1, sec1;
//	hr = (int)curDuration/3600;
//	min = (int)(curDuration/60)%60;
	min = (int)(curDuration/60);
	sec = (int)curDuration%60;

//	hr1 = (int)(outDuration-curDuration)/3600;
// 	min1 = (int)((outDuration-curDuration)/60)%60;
	min1 = (int)((outDuration-curDuration)/60);
	sec1 = (int)(outDuration-curDuration)%60;

	CString str1;
	CString str2;
	str1.Format(_T("%02d:%02d"), min, sec);
	str2.Format(_T("%02d:%02d"), min1, sec1);
	if (m_pTimeElapse)		m_pTimeElapse->SetText(str1);
	if (m_pTimeLeft)		m_pTimeLeft->SetText(str2);

}


void CDvpBaseDlg::UpdataProgress(UINT curDuration, UINT outDuration)
{
	int min, max;
	if (m_pProgressSlider)
	{
		m_pProgressSlider->GetRange(min, max);

		// set progress position to corresponding position
		// if outDuration is ZERO, just set pos to zero.
		int pos = (outDuration==0) ? 0 : (int) (curDuration * (max - min) / outDuration);
		m_pProgressSlider->SetPos(pos);
	}
}



void CDvpBaseDlg::ShowEQ()
{
	// 顺序与宏定义顺序保持一致
	LPCTSTR res_id[] = { L"SET_AUDIO_NONE", L"SET_AUDIO_ROCK", L"SET_AUDIO_POPULAR", L"SET_AUDIO_LIVE",
		L"SET_AUDIO_DANCE", L"SET_AUDIO_CLASSIC", L"SET_AUDIO_SOFT", L"SET_AUDIO_CUSTOM"};
	UINT type = sysutil::nss_get_instance()->audio_eq_type;
	if (type>=0 && type<8)
	{
		CWceUiLayer* player = GetLayerByName(L"eq_type");
		if (player)
		{
			// 暂不显示EQ类型，不好处理多语言
			//player->SetTextResID(res_id[type]);
		}
	}	

}


LRESULT CDvpBaseDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_EQ_CHANGE)
	{
		ShowEQ();
	}
	if (message == UI_MSG_BT_CONNECT_STATUS_CHANGE)
	{
		OnConnectEvent();
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CDvpBaseDlg::OnKeyPlayControl(UINT key, UINT param)
{

}

void CDvpBaseDlg::OnConnectEvent()
{
	CWceUiLayer* player = GetLayerByName(L"bt_status");
	if (player)
	{
		player->SetStyle(CBluetooth::GetInstance()->IsConnected() ? L"bt_connect" : L"bt_disconnect");
	}

}