#include "stdafx.h"
#include "BTCallDlg.h"
#include "WceUiPageLayer.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "interface.h"
#include "tzUtil.h"
#include "sysUtil.h"
#include "BluetoothDlg.h"
#include "WceUiButton.h"
#include "WceUiLoadString.h"
#include "BTDialEditBox.h"
#include "protocol.h"
#include "config.h"
#include "QTelzone.h"
//#include "EcarDlg.h"
#include "DVDStartDlg.h"

CBTCallDlg::CBTCallDlg(void)
{
	m_dwTickSpeaking = 0;
	m_status = 0;
}

CBTCallDlg::~CBTCallDlg(void)
{
	m_pLayerFrameKeypad = NULL;
}

void CBTCallDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_BT_CALL_EVENT);
	RegisterMsg(NAVI_MSG_LAN_CHANGE);
	RegisterMsg(UI_MSG_MCU_KEY_COMMAND);
	RegisterMsg(UI_MSG_MCU_RADIO_DATA);		// ����RDS
	RegisterMsg(UI_MSG_MCU_DIGITAL_KEY);

	m_pLayerFrameKeypad = GetChildByName(L"keypad_frame");
	ASSERT(m_pLayerFrameKeypad);
	m_pLayerFrameKeypad->ShowLayer(FALSE);

	//++
	CAnimDiskLayer* pAnimIncoming = (CAnimDiskLayer*)GetLayerByName(L"anim_layer");
	pAnimIncoming->SetRotateAngle(-20.0f);
	pAnimIncoming->StartRotate();
	//--

	CWceUiButton *pButton = (CWceUiButton*)GetLayerByName(L"key_0");
	ASSERT(pButton);
	pButton->EnableLongPress(TRUE);

	OnAudioSwitch(CBluetooth::GetInstance()->GetAudioOutput());

	// �ô��ڴ����󲻳���ɾ��,ֻ��֪ͨ������,���ô˱�־�������ɾ��(�������Դ������ڱ仯,�п��ܻ�����ô���)
	CDlgManager::GetInstance()->SetToDeleteFlag(CBTCallDlg_ID, SOURCE_BT_CALL, FALSE);

	__super::OnInitDialog();
}

void CBTCallDlg::OnBnLongClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"key_0"))	// ������+�ż�
	{
		CBluetooth::GetInstance()->SendDtmf(L'+');
		GetWindow()->PostMessage(WM_CHAR, (WPARAM)L'+', 0);
	}

	__super::OnBnLongClick(pButton);
}


void CBTCallDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_REFRESH_UI)
	{
		RefreshUI();
	}
	else if (wIDEvent == TIMER_ID_TERMINATE_DLG)
	{
		ui_led_hide_btcall();
		KillTimer(TIMER_ID_TERMINATE_DLG);
		CDlgManager::GetInstance()->SetToDeleteFlag(CBTCallDlg_ID, SOURCE_BT_CALL, TRUE);

		if (sysutil::nss_get_instance()->sys_camera_status == 1)
		{
			;	// ������ڵ���,����������������ȷ��״̬,���ﲻ���κβ���
		}
		else if (sysutil::nss_get_instance()->sys_rds_status)
		{
			;	// RDSͬ����
		}
		else if (sysutil::nss_get_instance()->sys_ecar_status == 1)	// ECar������
		{
			CDlgManager::GetInstance()->ShowDlg(CEcarDlg_ID, SOURCE_ECAR);
		}
		else
		{
			// ���������ǰ̨����,ͨ����������ʾ����
			if (sysutil::nss_get_instance()->sys_gps_foreground)
			{
				tzutil::SetProcessForeground(sysutil::nss_get_instance()->gps_process_id);
			}

			CDlgManager::GetInstance()->GotoLastDlg();
		}
	}
	else
	{
		__super::OnTimer(wIDEvent);
	}
}

void CBTCallDlg::OnKeyCommand(UINT key, UINT param)
{
	int status = (key>>16) & 0xFF;
	int id = key & 0xFF;
	if (ui_is_on_bt_call())
	{
		if (id == K_PHONE_ON)
		{
			OnAnswer();
		}
		else if (id == K_PHONE_OFF)
		{
			OnDecline();
		}
		else if (id == K_NUMBER || id == K_STAR)	// ͨ��ʱmainwnd�ǲ�������Щ������,����ֱ�Ӵ���
		{
			if (m_status == BZ_IND_CALL_SPEAKING)
			{
				ShowKeypad(TRUE);
				CBluetooth::GetInstance()->SendDtmf(id==K_NUMBER ? L'#' : L'*');
				GetWindow()->PostMessage(WM_CHAR, (WPARAM)(id==K_NUMBER ? L'#' : L'*'), 0);
			}

		}
	}
}


void CBTCallDlg::OnAnswer()
{
	CBluetooth::GetInstance()->AnswerCall();
}

void CBTCallDlg::OnDecline()
{
	CBluetooth::GetInstance()->TerminateCall();

	if (m_callinfo.ciPath == BT_PBAP_SYNC_PATH_MISSED_CALLHISTORY)	// �������ֱ�ӹҶ���,״̬��Ϊ�Ѷ���δ������
	{
		m_callinfo.ciIsUnread = 0;
	}
	ShowKeypad(FALSE);	//  �Ҷ�ʱ���ؼ���
	RefreshUI();
}

void CBTCallDlg::OnAudioSwitch(BOOL bByPhone)
{
	CWceUiButton* pbutton = (CWceUiButton*)GetChildByName(L"audio_switch");
	if (pbutton)
	{
		pbutton->SetStatusStyleName(bByPhone ? L"audio_switch_car" : L"audio_switch_phone", CWceUiButton::STATUS_NORMAL);
	}
}

void CBTCallDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"decline"))
	{
		OnDecline();
	}
	else if (pButton->IsEqualName(L"answer"))
	{
		OnAnswer();
	}
	else if (pButton->IsEqualName(L"audio_switch"))
	{
		BOOL bByPhone = CBluetooth::GetInstance()->GetAudioOutput();
		CBluetooth::GetInstance()->SetAudioOutput(!bByPhone);
		OnAudioSwitch(!bByPhone);
	}
	else if (pButton->IsEqualName(L"mute"))
	{
		BOOL bEnable = CBluetooth::GetInstance()->IsMicMuteEnable();
		CBluetooth::GetInstance()->EnableMicMute(!bEnable);
		OnMicMuteChange(!bEnable);
	}
	else if (pButton->IsEqualName(L"keypad"))
	{
		BOOL bVisible = m_pLayerFrameKeypad->IsLayerVisible();
		ShowKeypad(!bVisible);
	}
	else if (OnBnKeypadClick(pButton))
	{
		;
	}
	else if (pButton->IsEqualName(L"back") || pButton->IsEqualName(L"home"))	// BTͨ�����ڲ�������
	{
		;
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}

void CBTCallDlg::ShowKeypad(BOOL bShow)
{
	CBTDialEditBox* pdial_editbox = (CBTDialEditBox*)m_pLayerFrameKeypad->GetChildByName(L"dial_number");
	ASSERT(pdial_editbox);
	pdial_editbox->OnKillFocus();

	m_pLayerFrameKeypad->ShowLayer(bShow);
	if (bShow)
	{
		pdial_editbox->OnFocus();
	}
	else
	{
		pdial_editbox->OnKillFocus();
	}
}

BOOL CBTCallDlg::OnBnKeypadClick(CWceUiButton* pButton)
{
	const int KEY_COUNT = 14;
	LPCTSTR key_layers[] = {L"key_0", L"key_1", L"key_2", L"key_3", L"key_4", L"key_5", L"key_6", 
		L"key_7", L"key_8", L"key_9", L"key_star", L"key_sharp", L"key_backspace", L"key_plus"};
	TCHAR key_char[] = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'*', L'#', L'\b', L'+'};


	int i = 0;
	for (i=0; i<KEY_COUNT; i++)
	{
		if (pButton->IsEqualName(key_layers[i]))
		{
			break;
		}
	}

	if (i<KEY_COUNT)	// ƥ�䵽��
	{
		CBluetooth::GetInstance()->SendDtmf(key_char[i]);
		GetWindow()->PostMessage(WM_CHAR, (WPARAM)key_char[i], 0);
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

void CBTCallDlg::OnMicMuteChange(BOOL bEnable)
{
	CWceUiButton* pMute = (CWceUiButton*)GetChildByName(L"mute");
	if (pMute)
	{
		if (bEnable)
		{
			pMute->SetStatusStyleName(L"mute_normal", CWceUiButton::STATUS_NORMAL);
			pMute->SetStatusStyleName(L"mute_down", CWceUiButton::STATUS_DOWN);
		}
		else
		{
			pMute->SetStatusStyleName(L"unmute_normal", CWceUiButton::STATUS_NORMAL);
			pMute->SetStatusStyleName(L"unmute_down", CWceUiButton::STATUS_DOWN);
		}
	}
}


LRESULT CBTCallDlg::OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam)
{
	return __super::OnCommand(uCommand, wParam, lParam);
}

void CBTCallDlg::OnStatusChange(UINT nStatus)
{
	m_status = nStatus;

	CWceUiLayer* pStatus = GetChildByName(L"call_status");
	CWceUiLayer* pCallTime = GetChildByName(L"call_time");
	CWceUiLayer* pAnimIncoming = GetChildByName(L"anim_incoming");
	CWceUiLayer* pAnimOutgoing = GetChildByName(L"anim_outgoing");
	ASSERT(pStatus && pCallTime && pAnimOutgoing && pAnimIncoming);
	pStatus->GetStyle()->SetTextColorFromApp(RGB(255,255,255));
	pCallTime->ShowLayer(FALSE);
	pAnimIncoming->ShowLayer(FALSE);
	pAnimOutgoing->ShowLayer(FALSE);


	CWceUiButton* pdecline = (CWceUiButton*)GetChildByName(L"decline");
	CWceUiButton* panswer = (CWceUiButton*)GetChildByName(L"answer");
	CWceUiButton* pkeypad = (CWceUiButton*)GetChildByName(L"keypad");

	//CWceUiLoadString strStatus;
	if (nStatus == BZ_IND_CALL_SPEAKING)
	{
		pStatus->SetTextResID(L"BT_CALL_STATUS_SPEAKING");
		m_dwTickSpeaking = GetTickCount();
		pCallTime->ShowLayer(TRUE);

		pkeypad->EnableButton(TRUE);
		pdecline->EnableButton(TRUE);
		panswer->EnableButton(FALSE);

		RefreshUI();
		SetTimer(TIMER_ID_REFRESH_UI, 1000);
	}
	else if (nStatus == BZ_IND_CALL_OUTGOING)
	{
		m_dwTickSpeaking = 0;
		pStatus->SetTextResID(L"BT_CALL_STATUS_OUTGOING");
		pAnimOutgoing->ShowLayer(TRUE);

		((CWceUiAnimLayer*) pAnimOutgoing)->StartAnim(300);

		pkeypad->EnableButton(FALSE);
		pdecline->EnableButton(TRUE);
		panswer->EnableButton(FALSE);
		CBluetooth::GetInstance()->EnableMicMute(FALSE);
	}
	else if (nStatus == BZ_IND_CALL_INCOMING)
	{
		m_dwTickSpeaking = 0;
		pStatus->SetTextResID(L"BT_CALL_STATUS_INCOMING");
		pAnimIncoming->ShowLayer(TRUE);

		pkeypad->EnableButton(FALSE);
		pdecline->EnableButton(TRUE);
		panswer->EnableButton(TRUE);
		CBluetooth::GetInstance()->EnableMicMute(FALSE);
	}
	else if (nStatus == BZ_IND_CALL_TERMINATED)
	{
		pStatus->GetStyle()->SetTextColorFromApp(RGB(255,0,0));
		pStatus->SetTextResID(L"BT_CALL_STATUS_TEMINATED");
		pCallTime->ShowLayer(TRUE);
		SetTimer(TIMER_ID_TERMINATE_DLG, 1000);

		pkeypad->EnableButton(FALSE);
		pdecline->EnableButton(FALSE);
		panswer->EnableButton(FALSE);

		KillTimer(TIMER_ID_REFRESH_UI);

		// editbox�е��������
		CBTDialEditBox* pdial_editbox = (CBTDialEditBox*)m_pLayerFrameKeypad->GetChildByName(L"dial_number");
		if (pdial_editbox)
		{
			pdial_editbox->SetData(L"");
		}
		CBluetooth::GetInstance()->EnableMicMute(FALSE);
	}
}

LRESULT CBTCallDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_BT_CALL_EVENT)
	{
		//  ������״̬�仯�ݲ�����
		if (lParam == BZ_IND_CALL_OUTGOING || lParam == BZ_IND_CALL_INCOMING
			|| lParam == BZ_IND_CALL_SPEAKING || lParam == BZ_IND_CALL_TERMINATED)
		{
			if (lParam == BZ_IND_CALL_OUTGOING || lParam == BZ_IND_CALL_INCOMING)
			{
				ui_led_show_btcall(lParam);
			}
			OnStatusChange((UINT)lParam);
			UpdateCallInfo((UINT)lParam);
		}
		else if (lParam == BZ_IND_AUDIO_OUTPUT_INPHONE || lParam == BZ_IND_AUDIO_OUTPUT_INLOCALDEV)
		{
			OnAudioSwitch(CBluetooth::GetInstance()->GetAudioOutput());
		}
	}
	else if (message == NAVI_MSG_LAN_CHANGE)
	{
		ShowTitleInfo();	// ֧�ֶ�����
	}
	else if (message == UI_MSG_MCU_KEY_COMMAND)
	{
		OnKeyCommand(wParam, lParam);
	}
	else if (message == UI_MSG_MCU_RADIO_DATA && lParam == 0)
	{
		OnRadioStatus();
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

void CBTCallDlg::OnKeyDigital(UINT nDigitalKey)
{
	if (CDlgManager::GetInstance()->GetCurDlgID() == CBTCallDlg_ID
		&& m_status == BZ_IND_CALL_SPEAKING)
	{
		ShowKeypad(TRUE);

		TCHAR key_char[] = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9'};
		CBluetooth::GetInstance()->SendDtmf(key_char[nDigitalKey%10]);
		GetWindow()->PostMessage(WM_CHAR, (WPARAM)key_char[nDigitalKey%10], 0);
	}
}

void CBTCallDlg::UpdateCallInfo(UINT nStatus)
{
	if (nStatus == BZ_IND_CALL_SPEAKING)
	{
		// ����Ǵ���ĵ绰,��ͨ����,��Ϊ�ѽ�����
		if (m_callinfo.ciPath == BT_PBAP_SYNC_PATH_MISSED_CALLHISTORY)	// (��ʼ״̬Ϊmissed call)
		{
			m_callinfo.ciPath = BT_PBAP_SYNC_PATH_RECEIVE_CALLHISTORY;
		}
	}
	else if (nStatus == BZ_IND_CALL_OUTGOING)
	{
		InitCallInfo(nStatus);
	}
	else if (nStatus == BZ_IND_CALL_INCOMING)
	{
		InitCallInfo(nStatus);
	}
	else if (nStatus == BZ_IND_CALL_TERMINATED)
	{
		if (m_dwTickSpeaking > 0)	// ��ͨ����, ����ͨ��ʱ��, ��λ��
		{
			m_callinfo.ciCallTime = (WORD)((GetTickCount() - m_dwTickSpeaking) / 1000);
		}
	}
}

void CBTCallDlg::InitCallInfo(UINT nStatus)
{
	memset(&m_callinfo, 0, sizeof(LOCAL_RECENTS_CALL_INFO));
	_tcsncpy(m_callinfo.ciRemoteDeviceAddr, CBluetooth::GetInstance()->GetRemoteDeviceAddr(), MAX_LEN_DEVICE_ADDR);
	// ʹ���ļ�С��,ֻ������32�ַ�����,����ȷ���ڴ治�����
	TCHAR szNumber[64] = L"";
	TCHAR szName[64] = L"";
	CBluetooth::GetInstance()->GetCallInfo(szName, szNumber);
	_tcsncpy(m_callinfo.ciName, szName, sizeof(m_callinfo.ciName)/sizeof(TCHAR)-1);
	_tcsncpy(m_callinfo.ciNumber, szNumber, sizeof(m_callinfo.ciNumber)/sizeof(TCHAR)-1);

	GetLocalTime(&m_callinfo.ciTime);
	// ��ʼ��״̬Ϊ�����δ��,�����ٸ����û��Ķ������������״̬
	m_callinfo.ciPath = (nStatus == BZ_IND_CALL_OUTGOING) ? 
				BT_PBAP_SYNC_PATH_DIALED_CALLHISTORY : BT_PBAP_SYNC_PATH_MISSED_CALLHISTORY;
	// ���Ϊ����ĵ绰,��ʼ���Ϊδ��״̬
	m_callinfo.ciIsUnread = (nStatus == BZ_IND_CALL_OUTGOING) ? 0 : 1;
}

void CBTCallDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		KillTimer(TIMER_ID_TERMINATE_DLG);	// ��ֹ�ս���,���е绰����

		ShowTitleInfo();
		OnMicMuteChange(CBluetooth::GetInstance()->IsMicMuteEnable());

		TCHAR szNumber[64] = L"";
		TCHAR szName[64] = L"";
		CBluetooth::GetInstance()->GetCallInfo(szName, szNumber);

		// �����������,��ʾһ������ͼ��
		/* --++ ecar
		CWceUiLayer* pEcarIcon = GetChildByName(L"ecar_icon");
		if (pEcarIcon)
		{
			pEcarIcon->ShowLayer(CEcarDlg::IsEcarNumber(szNumber));
		}
		*/

		CWceUiLayer* pZone = GetChildByName(L"phone_zone");
		if (pZone 
			&& (sysutil::nss_get_instance()->ui_lan_id == LAN_CHINESEGB
			|| sysutil::nss_get_instance()->ui_lan_id == LAN_CHINESEBIG5))
		{
			pZone->SetText(CQTelzone::GetInstance()->GetTelZone(szNumber));
		}

		CWceUiLayer* pTel = GetChildByName(L"phone_number");
		if (pTel)
		{
			ui_format_tel_number(szNumber);
			pTel->SetText(szNumber);
		}

		CWceUiLayer* pNum = GetChildByName(L"phone_name");
		if (pNum)
		{
			pNum->SetText(szName);
		}

		// ���������ǰ̨����
		if (sysutil::nss_get_instance()->sys_gps_foreground)
		{
			ui_set_foreground_window(GetWindow()->m_hWnd);
		}
	}
	else
	{
		CBluetooth::GetInstance()->AddLocalCallInfo(&m_callinfo);
	}
}


void CBTCallDlg::ShowTitleInfo()
{
	CWceUiLayer *ptitle = GetLayerByName(L"title");
	if (ptitle)
	{
		CString strTitle;
		CBluetoothDlg::GetTitleInfo(strTitle);
		ptitle->SetText(strTitle);
	}
}

void CBTCallDlg::RefreshUI()
{
	DWORD dwTick = 0;
	if (m_status == BZ_IND_CALL_SPEAKING)
	{
		dwTick = (GetTickCount() - m_dwTickSpeaking)/1000;
	}
	
	CWceUiLayer *pCallTime = GetChildByName(L"call_time");
	TCHAR buffer[16];
	int hour = dwTick / 3600;
	int min = dwTick / 60;
	int sec = dwTick % 60;
	_stprintf(buffer, L"%02d:%02d:%02d", hour, min, sec);
	pCallTime->SetText(buffer);
}

void CBTCallDlg::OnAnimateEnd()
{

}


void CBTCallDlg::OnRadioStatus()
{
	static BOOL bByPhone = FALSE;
	static BOOL bRDSAlarm = FALSE;
	BOOL bIsCurRDSAlarm = protocol::get_mcu_radio_status()->is_TA_Alarm()
		|| protocol::get_mcu_radio_status()->is_EWS();

	// ֻ��RDS����״̬�仯ʱ,��ִ��������߼�
	if (ui_is_on_bt_call() && !tzutil::IsBoolEqual(bRDSAlarm, bIsCurRDSAlarm))
	{
		bRDSAlarm = bIsCurRDSAlarm;
		if (bIsCurRDSAlarm)
		{
			bByPhone = CBluetooth::GetInstance()->GetAudioOutput();
			if (!bByPhone)
			{
				CBluetooth::GetInstance()->SetAudioOutput(TRUE);
			}
		}
		else
		{
			if (!bByPhone)
			{
				CBluetooth::GetInstance()->SetAudioOutput(FALSE);
			}
		}
	}
}
////////////////////////////////////////////////////////////////

CWceUiAnimLayer::CWceUiAnimLayer()
{
	m_nCurrentStyle = 0;
}

CWceUiAnimLayer::~CWceUiAnimLayer()
{

}


void CWceUiAnimLayer::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_ANIM_LAYER)
	{
		m_nCurrentStyle = (m_nCurrentStyle+1) % m_style.GetCount();
		SetStyle(m_style.GetAt(m_nCurrentStyle));
	}
	else
	{
		__super::OnTimer(wIDEvent);
	}
}

void CWceUiAnimLayer::SetProperties(PCWceUiXmlElem pCreateElem)
{
	__super::SetProperties(pCreateElem);

	LPCTSTR attr = pCreateElem->GetAttr(L"anim_style");
	if (attr) SetAnimStyle(attr);
}

void CWceUiAnimLayer::SetAnimStyle(LPCTSTR strStyle)
{
	m_style.RemoveAll();
	tzutil::GetSubString(strStyle, m_style, L',');
}

// nTime ~ ÿһ֡��ʾʱ��,��λms
void CWceUiAnimLayer::StartAnim(int nTime)
{
	m_nCurrentStyle = 0;
	if (m_style.GetCount() > 1)	// ֻ�������˶������ϵ�style�ſ��ܶ���
	{
		SetStyle(m_style.GetAt(m_nCurrentStyle));
		SetTimer(TIMER_ID_ANIM_LAYER, nTime);
	}
}

void CWceUiAnimLayer::StopAnim()
{
	m_nCurrentStyle = 0;
	if (m_style.GetCount() > 1)
	{
		KillTimer(TIMER_ID_ANIM_LAYER);
		SetStyle(m_style.GetAt(m_nCurrentStyle));
	}
}


///////////////////////////////////////////////////////////////////////////////////
CAnimIncomingLayer::CAnimIncomingLayer()
{

}

CAnimIncomingLayer::~CAnimIncomingLayer()
{

}

void CAnimIncomingLayer::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_ANIM_LAYER)
	{
		if (!IsLayerVisible())
		{
			return;
		}

		CRect rc;
		GetWindowRect(rc);
		int Xc = (rc.left + rc.right) / 2;
		int Yc = (rc.top + rc.bottom) / 2;
		float offsetX = (float)(WceUiGetScreenWidth()/2 - Xc);
		float offsetY = (float)(Yc - WceUiGetScreenHeight()/2);

// 		wcegl_mat4& m = GetMatrix();
// 
// 		wcegl_mat4 m1, m2, m3, result;
// 
// 		wceui::matrixTranslate(offsetX, offsetY, -Z_AXE, m1);
// 		wceui::matrixRotateZ(30, m2);
// 		wceui::matrixTranslate(-offsetX, -offsetY, Z_AXE, m3);
// 
// 		wceui::matrixMultiply(m1, m, result);
// 		wceui::matrixCopy(m , result);
// 
// 		wceui::matrixMultiply(m2, m, result);
// 		wceui::matrixCopy(m , result);
// 
// 		wceui::matrixMultiply(m3, m, result);
// 		wceui::matrixCopy(m , result);

		InvalidateRect(NULL);

	}
	else
	{
		__super::OnTimer(wIDEvent);
	}
}

void CAnimIncomingLayer::OnInitLayer()
{
	SetTimer(TIMER_ID_ANIM_LAYER, 30);

	__super::OnInitLayer();
}

void CAnimIncomingLayer::SetProperties(PCWceUiXmlElem pCreateElem)
{
	__super::SetProperties(pCreateElem);
}

void CAnimIncomingLayer::StartAnim(int nTime)
{
	SetTimer(TIMER_ID_ANIM_LAYER, nTime);
}

void CAnimIncomingLayer::StopAnim()
{
	KillTimer(TIMER_ID_ANIM_LAYER);
}

void CAnimIncomingLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);
}
