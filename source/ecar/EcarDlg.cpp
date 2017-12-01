#include "stdafx.h"
#include "EcarDlg.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "MainWnd.h"
#include "interface.h"
#include "bluetooth.h"
#include "VolumeManager.h"
#include "BluetoothDlg.h"
#include "WceUiLoadString.h"
#include "sysUtil.h"
#include "tzUtil.h"

const UINT TIME_DELAY_DIAL = 6900;	// 延时5秒再拨出下发的号码
const UINT MAX_TNC_SEND_COUNT = 5;

CEcarDlg::CEcarDlg(void)
: m_bIsEcarCallCenter(FALSE)
, m_pdiLastDlg(NULL)
{
	CreateEcarObject((void**)&m_pECarObject);
	m_pECarObject->m_hwnParent = CMainWnd::GetInstance()->m_hWnd;

	m_LeftCount = 0;
	m_eStatus = ES_IDLE;

}

CEcarDlg::~CEcarDlg(void)
{
	if(m_pECarObject)
	{
		m_pECarObject->Stop();
		Sleep(500);
		::DestroyEcarObject(m_pECarObject);
		m_pECarObject = NULL;
	}
}

void CEcarDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_BT_CALL_EVENT);
	

	RegisterMsg(WM_CLOSE_EARMIC);
	RegisterMsg(WM_OPEN_EARMIC);
	RegisterMsg(WM_UPDT_MAPINFO);
	RegisterMsg(WM_UPDT_USERCALLNO);
	RegisterMsg(WM_UPDT_CALLCENTERNO);
	RegisterMsg(WM_DTMFDECODE_ADCRATE);
	RegisterMsg(WM_DTMFDECODE_EVENT);
	RegisterMsg(WM_ECARONEKEY_START);
	RegisterMsg(WM_ECARONEKEY_EXIT);
	RegisterMsg(WM_UPDT_CALLBYNAME);
	RegisterMsg(WM_BTHANGUP);
	RegisterMsg(WM_BTDIALEXTNUMBER);
	RegisterMsg(WM_ACTIVATIONCODE);

	__super::OnInitDialog();
}

void CEcarDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_DELAY_DIAL)
	{
		KillTimer(TIMER_ID_DELAY_DIAL);
		CBluetooth::GetInstance()->Dial(m_strTelNumberToDial);
		m_eStatus = ES_IDLE;
	}
	else if (wIDEvent == TIMER_ID_SEND_TNC_CODE)
	{
		if (SendTncCodeToNavi() || m_nSendTncCodeCount++ >= MAX_TNC_SEND_COUNT)
		{
			KillTimer(TIMER_ID_SEND_TNC_CODE);
		}
	}
	else if (wIDEvent == TIMER_ID_DELAY_DIAL_LEFT_COUNT)
	{
		OnTimerLeftCount();
	}
	else if (wIDEvent == TIMER_ID_HIDE_MESSAGE_TIP)
	{
		KillTimer(TIMER_ID_HIDE_MESSAGE_TIP);
		CWceUiLayer* player = GetLayerByName(L"message_tip");
		if (player)
		{
			player->ShowLayer(FALSE);

			CWceUiLayer* player_message = GetLayerByName(L"message");
			if (player_message)
			{
				player_message->ShowLayer(TRUE);
			}
		}
	}
// 	else if (wIDEvent == 1111)
// 	{
// 		KillTimer(1111);
// 		//static char *tel = "13609621826";
// 		static TCHAR *tel = L"张三";
// 		CMainWnd::GetInstance()->PostMessage(WM_UPDT_MAPINFO/*WM_UPDT_USERCALLNO*/, 0, (LPARAM)tel);
// 	}

	__super::OnTimer(wIDEvent);
}

void CEcarDlg::OnTimerLeftCount()
{
	if (m_LeftCount <= 0)
	{
		KillTimer(TIMER_ID_DELAY_DIAL_LEFT_COUNT);
	}
	CWceUiLoadString str(L"ECAR_TO_DIAL");
	CString tip;
	tip.Format(L"%s %ds", str.GetString(), m_LeftCount);
	ShowMessageTip(tip);

	m_LeftCount--;
}

LRESULT CEcarDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
// 	RETAILMSG(1, (L"[ecar]:  message = %d \n", message));

	if (message == UI_MSG_BT_CALL_EVENT)
	{
		OnCallEvent((DWORD)lParam);
	}
	else if (message == WM_UPDT_USERCALLNO)	//根据下发号码代用户拨号 	//wParam:长度	lParam:电话号码ascii码
	{
		SetEcarActive(TRUE);
		OnUserCallNo((char*)lParam);
	}
	else if (message == WM_UPDT_CALLBYNAME)//根据下发的姓名代用户拨号	//wParam:长度	lParam:用户姓名,Unicode
	{
		SetEcarActive(TRUE);
		OnCallByName((LPCTSTR)lParam);
	}
	else if (message == WM_CLOSE_EARMIC)
	{
		CVolumeManager::SetBTVolume(0);
	}
	else if (message == WM_OPEN_EARMIC)
	{
		CVolumeManager::UpdateVolume();
	}
	else if (message == WM_UPDT_MAPINFO)
	{
		OnUpdateMapInfo((char*)lParam);
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CEcarDlg::OnUserCallNo(char* pTelNumber)
{
	CBluetooth::GetInstance()->TerminateCall();

	if (pTelNumber)
	{
		TCHAR tel[64] = L"";
		MultiByteToWideChar(CP_ACP, 0, pTelNumber, -1, tel, sizeof(tel)/sizeof(TCHAR));

		CWceUiListBox *pListbox = (CWceUiListBox*)GetChildByName(L"phonebook_listbox");
		if (pListbox == NULL || tel[0] == L'\0')
		{
			return;
		}
		GetContactsByName(NULL, tel, FALSE, pListbox, m_PBQueryResult);
		pListbox->SetCurSel(0);

		// 如果有多条记录,只显示第一条,因为是用电话号码来匹配的
		while (pListbox->GetCount() > 1)
		{
			pListbox->DeleteString(pListbox->GetCount()-1);
		}

		if (pListbox->GetCount() <= 0)	// 电话号码不在通信录,未匹配到记录,直接显示电话号码在列表
		{
			m_PBQueryResult.RemoveAll();
			m_PBQueryResult.Add(tel);

			ui_format_tel_number(tel);
			CString str = L"      1            unkown                    ";
			str += tel;
			pListbox->AddString(str);
		}

		m_strTelNumberToDial = m_PBQueryResult.GetAt(0);

		SetTimer(TIMER_ID_DELAY_DIAL, TIME_DELAY_DIAL);
		SetTimer(TIMER_ID_DELAY_DIAL_LEFT_COUNT, 1000);
		m_eStatus = ES_TO_DIAL;
		m_LeftCount = TIME_DELAY_DIAL / 1000;

		// 通话结束回到ECar窗口
//		SetLastDlgInfoToThis();
	}
}

void CEcarDlg::GetContactsByName(LPCTSTR pTelName, LPCTSTR pTelNumber, BOOL bByName, CWceUiListBox* pListbox, CStringArray& PBQueryResult)
{
	CppSQLite3DB m_db;
	pListbox->ResetContent();
	m_PBQueryResult.RemoveAll();

	try
	{
		m_db.open(L"residentflash3\\bt\\bt.db");
		m_db.execDML(L"PRAGMA synchronous = OFF; ");
	}
	catch (CppSQLite3Exception& e)
	{
		RETAILMSG(1, (L"db create error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}

	try
	{
		TCHAR szSQL[64];
		if (bByName)	// by name
		{
			_stprintf(szSQL, L"select * from contacts where name like '%%%s%%';", pTelName);
		}
		else		// by number
		{
			_stprintf(szSQL, L"select * from contacts where number = '%s';", pTelNumber);
		}
		CppSQLite3Query q = m_db.execQuery(szSQL);
		while (!q.eof())
		{
			pListbox->AddString(q.fieldValue(FIELD_LONGSTRING));
			m_PBQueryResult.Add(q.fieldValue(FIELD_NUMBER));
			q.nextRow();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		RETAILMSG(1, (L"db select error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}
}

void CEcarDlg::OnCallByName(LPCTSTR pTelName)
{
	CBluetooth::GetInstance()->TerminateCall();

	CWceUiListBox *pListbox = (CWceUiListBox*)GetChildByName(L"phonebook_listbox");
	if (pListbox == NULL)
	{
		return;
	}
	GetContactsByName(pTelName, NULL, TRUE, pListbox, m_PBQueryResult);
	pListbox->SetCurSel(0);

	CWceUiLoadString str;
	if (pListbox->GetCount() == 1)
	{
		m_strTelNumberToDial = m_PBQueryResult.GetAt(0);

		SetTimer(TIMER_ID_DELAY_DIAL, TIME_DELAY_DIAL);
		SetTimer(TIMER_ID_DELAY_DIAL_LEFT_COUNT, 1000);
		m_eStatus = ES_TO_DIAL;
		m_LeftCount = TIME_DELAY_DIAL / 1000;

		str.SetTextResID(L"ECAR_MSG_ONE_RECORDS");
	}
	else if (pListbox->GetCount() <=0)
	{
		str.SetTextResID(L"ECAR_MSG_NO_RECORDS");
	}
	else
	{
		str.SetTextResID(L"ECAR_MSG_MULTI_RECORDS");
	}
	ShowMessage(str.GetString());

	// 通话结束回到ECar窗口
//	SetLastDlgInfoToThis();
}

void CEcarDlg::SetLastDlgInfoToThis()
{
	DLG_INFO* pdi = CDlgManager::GetInstance()->GetDlgInfo(this);
	if (pdi != CDlgManager::GetInstance()->GetLastDlgInfo())
	{
		m_pdiLastDlg = CDlgManager::GetInstance()->GetLastDlgInfo();
	}
	CDlgManager::GetInstance()->SetLastDlgInfo(pdi);
}

void CEcarDlg::OnUpdateMapInfo(const char* pTncCode)
{
	CBluetooth::GetInstance()->TerminateCall();

	if (pTncCode)
	{
		::MultiByteToWideChar(CP_ACP, 0, pTncCode, -1, m_szTncCode, sizeof(m_szTncCode)/sizeof(TCHAR));
		// 如果因为导航没启动,导致发送TNC失败,启动导航，延时2秒后再发送
		if (!SendTncCodeToNavi())
		{
			SetTimer(TIMER_ID_SEND_TNC_CODE, 2000);
			m_nSendTncCodeCount = 0;
		}

		// 启动导航或把导航带到前台
		ui_load_navigation();
	}
}
void CEcarDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	if (bClickChecked)
	{
		if (nItemClicked >= 0 && nItemClicked < m_PBQueryResult.GetCount())
		{
			CBluetooth::GetInstance()->Dial(m_PBQueryResult.GetAt(nItemClicked));
			m_eStatus = ES_IDLE;
		}
	}
}

BOOL CEcarDlg::IsEcarNumber(const TCHAR *pTelNumber)
{
	LPCTSTR ecar_callcenter[] = {L"4008005005", L"075787807155", L"075788303000", L"075536860630", L"4001050868"};

	for (int i=0; i<sizeof(ecar_callcenter)/sizeof(ecar_callcenter[0]); i++)
	{
		if (pTelNumber && _tcscmp(pTelNumber, ecar_callcenter[i]) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CEcarDlg::OnCallEvent(DWORD dwEventID)
{
	if (dwEventID == BZ_IND_CALL_OUTGOING)
	{
		TCHAR szNumber[64] = L"";
		CBluetooth::GetInstance()->GetCallInfo(NULL, szNumber);
		m_bIsEcarCallCenter = IsEcarNumber(szNumber);
		SetEcarActive(FALSE);	// ecar结束了

// 
// 		static BOOL b = FALSE;
// 		if (!b)
// 		{
// 			b = TRUE;
// 			SetTimer(1111, 3000);
// 		}
	}
	else if (dwEventID == BZ_IND_CALL_SPEAKING && m_bIsEcarCallCenter)
	{
		SetFuncCode(0);
		m_pECarObject->Start(this);
	}
	else if (dwEventID == BZ_IND_CALL_TERMINATED && m_bIsEcarCallCenter)
	{
		m_pECarObject->Stop();
		m_bIsEcarCallCenter = FALSE;
	}
}

void CEcarDlg::OnDlgShow(BOOL bShow)
{
	__super::OnDlgShow(bShow);
}

void CEcarDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"ecar"))
	{
		OnEcar();
	}
	else if (pButton->IsEqualName(L"call"))
	{
		OnCall();
	}
	else if (pButton->IsEqualName(L"cancel"))
	{
		OnCancel();
	}
	else if (pButton->IsEqualName(L"back"))
	{
		OnBack();
	}
	else if (pButton->IsEqualName(L"home"))
	{
		OnHome();
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}

void CEcarDlg::OnBack()
{
	SetEcarActive(FALSE);	// ecar结束了
	CancelDelayDial();

	// 如果导航在前台运行
	if (sysutil::nss_get_instance()->sys_gps_foreground)
	{
		tzutil::SetProcessForeground(sysutil::nss_get_instance()->gps_process_id);
	}
	CDlgManager::GetInstance()->GotoLastDlg();
}

void CEcarDlg::OnHome()
{
	SetEcarActive(FALSE);	// ecar结束了
	CancelDelayDial();

	CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
}

void CEcarDlg::OnEcar()
{
	CancelDelayDial();		// 如果有延时拨号动作，先取消
	CBluetooth::GetInstance()->Dial(L"4008005005");

	CWceUiLoadString str(L"ECAR_CALL_ECAR");
	ShowMessageTip(str.GetString());
}

void CEcarDlg::OnCall()
{
	CWceUiListBox* plistbox = (CWceUiListBox*)GetChildByName(L"phonebook_listbox");
	if (plistbox)
	{
		int index = plistbox->GetCurSel();
		if (index >= 0  && index < m_PBQueryResult.GetCount())
		{
			CancelDelayDial();		// 如果有延时拨号动作，先取消
			CBluetooth::GetInstance()->Dial(m_PBQueryResult.GetAt(index));
			m_eStatus = ES_IDLE;
		}
	}

	CWceUiLoadString str(L"ECAR_CALL");
	ShowMessageTip(str.GetString());
}

void CEcarDlg::CancelDelayDial()
{
	if (m_eStatus == ES_TO_DIAL)
	{
		KillTimer(TIMER_ID_DELAY_DIAL);		// 如果有延时拨号动作，先取消
		KillTimer(TIMER_ID_DELAY_DIAL_LEFT_COUNT);
		m_eStatus = ES_IDLE;
	}
}

void CEcarDlg::SetEcarActive(BOOL bActive)
{
	sysutil::nss_get_instance()->sys_ecar_status = bActive ? 1 : 0;
	CVolumeManager::GetInstance()->UpdateVolume();
}

void CEcarDlg::OnCancel()
{
	CancelDelayDial();

	CWceUiLoadString str(L"ECAR_CANCEL");
	ShowMessageTip(str.GetString());
}

void CEcarDlg::ShowMessageTip(LPCTSTR tip, BOOL bAutoHide)
{
	CWceUiLayer* player = GetLayerByName(L"message_tip");
	if (player)
	{
		player->ShowLayer(TRUE);
		player->SetText(tip == NULL ? L"" : tip);

		CWceUiLayer* player_message = GetLayerByName(L"message");
		if (player_message)
		{
			player_message->ShowLayer(FALSE);
		}
	}

	if (bAutoHide)
	{
		SetTimer(TIMER_ID_HIDE_MESSAGE_TIP, 2000);
	}
}

void CEcarDlg::ShowMessage(LPCTSTR message)
{
	CWceUiLayer* player = GetLayerByName(L"message");
	if (player)
	{
		player->SetText(message == NULL ? L"" : message);
	}
}

//发送TNC数据到地图
BOOL CEcarDlg::SendTncCodeToNavi()
{
	#define RT_DATA_CPY_TYPE_DEST 0x0100
	//发送的目标窗口中
	HWND hwndRtNavi = ::FindWindow(NULL, L"RtNavi");
	if (hwndRtNavi)
	{
		COPYDATASTRUCT c;
		c.dwData = 1;
		c.lpData = m_szTncCode;
		c.cbData = _tcslen(m_szTncCode)*sizeof(TCHAR);
		::SendMessage(hwndRtNavi, WM_COPYDATA, RT_DATA_CPY_TYPE_DEST, (LPARAM)&c);
		return TRUE;
	}

	return FALSE;
}
