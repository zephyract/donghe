#include "stdafx.h"
#include "BluetoothDlg.h"
#include "WceUiPageLayer.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "interface.h"
#include "tzUtil.h"
#include "SourceManager.h"
#include "BTDialEditBox.h"
#include "WceUiLoadString.h"
#include "BTCallDlg.h"
#include "protocol.h"
#include "MainWnd.h"
#include "OverlayDlg.h"

// 一次格式化数据的最大条数
const int UPDATE_DB_STRING_MAX_COUNT = 20;

CString CBluetoothDlg::m_strOldDialNumber;

void _AddSpace2Str(CString& str, int len, HFONT hfont);

void CTelNumberEditBox::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	TCHAR buffer[256] = L"";
	CString strOrginal = m_strData.c_str();
	_tcsncpy(buffer, m_strData.c_str(), 255);
	buffer[255] = L'\0';

	ui_format_tel_number(buffer);

	// 用格式化的数据输出,输出完成后，恢复原始数据
	m_strData = buffer;
	__super::OnDraw(pWndBitmap, rcNeedDraw);
	m_strData = strOrginal;
}

CBluetoothDlg::CBluetoothDlg(void)
: m_CurFunType(BT_FUN_INVALID)
, m_bIsDialNumber(FALSE)
, m_bShowQueryUI(FALSE)
, m_nUpdatedRecords(0)
, m_bShowRecentsListUI(FALSE)
{
	m_nRecentsType = BT_PBAP_SYNC_PATH_MISSED_CALLHISTORY;
}

CBluetoothDlg::~CBluetoothDlg(void)
{

}

void CBluetoothDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(NAVI_MSG_LAN_CHANGE);
	RegisterMsg(UI_MSG_BT_CONNECT_STATUS_CHANGE);
	RegisterMsg(UI_MSG_BT_CALL_EVENT);
	RegisterMsg(UI_MSG_BT_PBMGR_SYNC_EVENT);
	RegisterMsg(UI_MSG_BT_MUSIC_EVENT);
	RegisterMsg(UI_MSG_BT_LOCAL_CALLINFO_CHANGE);
	RegisterMsg(UI_MSG_BT_PB_QUERY_CHANGE);

	RegisterMsg(UI_MSG_MCU_KEY_COMMAND);
	RegisterMsg(UI_MSG_MCU_DIGITAL_KEY);

	CBluetooth::GetInstance()->Initialize(GetWindow()->m_hWnd);
	int count = 10;
	while (count-->=0 && !CBluetooth::GetInstance()->IsInitialized())		// 初始化在另一线程执行,不一定马上能初始化完成
	{
		Sleep(500);
	}


	InitLayer();
	// 初始化连接状态,有可能BT连接好了,但该对话框并没有启动
	OnConnectEvent(BZ_IND_CONNECT_STATUS_CHANGED);



	// 第一次进入时选中拔号页面
	OnFunChange(BT_FUN_DIAL);

	// 连接过的历史设备
	InitPairedRecsListbox();

	OpenDB();

	// 刷新ID3信息
	OnMusicEvent(BZ_IND_AVRCP_ID3_SUCCESS);
}

void CBluetoothDlg::OpenDB()
{
	try
	{
		m_db.open(L"residentflash3\\bt\\bt.db");
		m_db.execDML(L"PRAGMA synchronous = OFF; ");
		//sqlite3_exec(db, "PRAGMA synchronous = OFF; ", 0,0,0);
		if (!m_db.tableExists(L"contacts"))
		{
			// CBluetooth中定义的结构体名字和号码最长为64
			m_db.execDML(L"create table contacts(id int, name nchar(64), number nchar(64), pyname nchar(8), longstring nchar(128), shortstring nchar(64));");
		}
	}
	catch (CppSQLite3Exception& e)
	{
		RETAILMSG(1, (L"db create error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}
}

void CBluetoothDlg::InitLayer()
{
	CWceUiLayer* pKeypadFrame = GetChildByName(L"keypad_frame");
	ASSERT(pKeypadFrame);

	CWceUiButton* pButton = (CWceUiButton*)pKeypadFrame->GetChildByName(L"key_backspace");
	ASSERT(pButton);
	pButton->EnableMultiCmd(TRUE);

	CWceUiEditBox* pEditbox = (CWceUiEditBox*)GetLayerByName(L"phonebook_query_string");
	ASSERT(pEditbox);
	pEditbox->SetReadOnly(TRUE);

	pButton = (CWceUiButton*)pKeypadFrame->GetLayerByName(L"phonebook_key_backspace");
	ASSERT(pButton);
	pButton->EnableMultiCmd(TRUE);

	//pButton = (CWceUiButton*)pKeypadFrame->GetLayerByName(L"key_0");
	//ASSERT(pButton);
	//pButton->EnableLongPress(TRUE);
}

void CBluetoothDlg::InitPairedRecsListbox()
{
	CWceUiListBox* plistbox = (CWceUiListBox*)GetLayerByName(L"paired_recs_listbox");
	if (plistbox)
	{
		CStringArray devArray;
		plistbox->ResetContent();
		CBluetooth::GetInstance()->GetPairedRecs(devArray);
		for (int i=0; i<devArray.GetCount(); i++)
		{
			plistbox->AddString(devArray.GetAt(i));
		}
		plistbox->SetCurSel(0);
		UpdatePairedLBChilds();
	}

}

void CBluetoothDlg::OnTimerBTConnect()
{
	// 如果当前源是BT，激活A2DP
	if ( CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_BT
		|| CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_PHONE_LINK)	// phonelink用到A2DP
	{
		CBluetooth::GetInstance()->A2DPActive(TRUE);
		CBluetooth::GetInstance()->MusicPlay();
	}
	else
	{
		CBluetooth::GetInstance()->A2DPActive(FALSE);
		//CBluetooth::GetInstance()->MusicPlay();
	}
	// 同步PB数据
	SyncPBData(FALSE);
	// 其它数据？ 通话记录？

	InitPairedRecsListbox();	// 可能是新设备
}

void CBluetoothDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_BT_CONNECTED)
	{
		KillTimer(TIMER_ID_BT_CONNECTED);
		OnTimerBTConnect();
	}
	else if (wIDEvent == TIMER_ID_BT_UNCONNECTED)
	{
		KillTimer(TIMER_ID_BT_UNCONNECTED);
		OnTimerBTDisconnect();
	}
	else if (wIDEvent == TIMER_ID_BT_UPDATE_DB_STRING)
	{
		OnTimerUpdateDBString();
	}
	else if (wIDEvent == TIMER_ID_HIDE_MESSAGE_TIP)
	{
		KillTimer(TIMER_ID_HIDE_MESSAGE_TIP);
		CWceUiLayer *player = GetLayerByName(L"message_tip");
		if (player)
		{
			player->ShowLayer(FALSE);
		}
	}
	else
	{
		__super::OnTimer(wIDEvent);
	}
}

void CBluetoothDlg::OnTimerBTDisconnect()
{
	// 如果当前源是BT，有可能激活了A2DP, 关闭A2DP,并切换到上一个源
	if ( CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_BT)
	{
//		CBluetooth::GetInstance()->A2DPActive(FALSE);
		CMainWnd::GetInstance()->PostMessage(UI_MSG_SOURCE_EXIT, 0, (LPARAM)SOURCE_BT);
	}
	// 断开后,清空蓝牙音乐的ID3信息
	ShowID3Tag();
	// 通讯录清空
	CWceUiListBox *pListbox = (CWceUiListBox*)GetLayerByName(L"phonebook_listbox");
	if (pListbox)
	{
		pListbox->ResetContent();
	}
	// 退出查询状态
	ShowPBQueryUI(FALSE);
}

void CBluetoothDlg::OnTimerUpdateDBString()
{
	int total = m_db.execScalar(L"select count(*) from contacts;");
	if (m_nUpdatedRecords >= total)		// 已经全部更新完了
	{
		KillTimer(TIMER_ID_BT_UPDATE_DB_STRING);
		if (m_bShowQueryUI)	// 在搜索界面时,全部格式化完成后重新刷新下结果就可以了
		{
			UpdatePBQ2Listbox();
		}
	}
	else
	{
		UpdateDBString();

		if (!m_bShowQueryUI)	// 没在搜索界面时才不停追加记录
		{
			CWceUiListBox *plistbox = (CWceUiListBox*)GetLayerByName(L"phonebook_listbox");
			ASSERT(plistbox);
			int list_count = plistbox->GetCount();
			//int total = m_db.execScalar(L"select count(*) from contacts;");
			AddPB2Listbox(list_count, m_nUpdatedRecords-list_count);
		}
	}
}

BOOL CBluetoothDlg::OnBnFunClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"fun_connect"))
	{
		OnFunChange(BT_FUN_CONNECT);

		#pragma message( __LOC__" temp code. save local call info. " )
	}
	else if (pButton->IsEqualName(L"fun_dial"))
	{
		OnFunChange(BT_FUN_DIAL);
	}
	else if (pButton->IsEqualName(L"fun_recents"))
	{
		OnFunChange(BT_FUN_RECENTS);
	}
	else if (pButton->IsEqualName(L"fun_phonebook"))
	{
		OnFunChange(BT_FUN_PHONEBOOK);
	}
	else if (pButton->IsEqualName(L"fun_music"))
	{
		OnFunChange(BT_FUN_MUSIC);
	}
	else 
	{
		return FALSE;
	}

	return TRUE;
}

// 从listbox中显示的信息(序号+名字+格式化的号码)中提取电话号码
// static void _extract_tel_number(IN OUT LPTSTR number)
// {
// 	if (number)
// 	{
// 		CString str(number);
// 		int pos = str.ReverseFind(L' ');	// last space
// 		if (pos >= 0)
// 		{
// 			_tcscpy(number, str.Mid(pos+1));
// 		}
// 
// 	}
// }

void CBluetoothDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	CWceUiListBox* pPairedRecsListbox = (CWceUiListBox*)GetLayerByName(L"paired_recs_listbox");
	CWceUiListBox* pPhonebookListbox = (CWceUiListBox*)GetLayerByName(L"phonebook_listbox");
	CWceUiListBox* pRecentsListbox = (CWceUiListBox*)GetLayerByName(L"recents_listbox");
	if (plist == pPairedRecsListbox )
	{
		if (bClickChecked)
		{
			CBluetooth::GetInstance()->ConnectByPairedRec((UINT)nItemClicked);
		}
		UpdatePairedLBChilds();
	}
	else if (plist == pPhonebookListbox)
	{
		if (bClickChecked)
		{
			if (m_bShowQueryUI)		// 在搜索界面
			{
				if (nItemClicked >= 0 && nItemClicked < m_PBQueryResult.GetCount())
				{
					CBluetooth::GetInstance()->Dial(m_PBQueryResult.GetAt(nItemClicked));
				}
			}
			else	// 在电话本界面
			{
				UINT total_count = CBluetooth::GetInstance()->PBMGR_GetCount(BT_PBAP_SYNC_PATH_ALL_PHONEBOOK);
				if (nItemClicked >= 0 && nItemClicked < (int)total_count)
				{
					UI_PBMGRRECORD record;
					if(CBluetooth::GetInstance()->PBMGR_GetRecords(BT_PBAP_SYNC_PATH_ALL_PHONEBOOK, nItemClicked, &record))
					{
						CBluetooth::GetInstance()->Dial((LPCTSTR)record.pswNumber);
					}	
				}
			}
		}
	}
	else if (plist == pRecentsListbox)
	{
		if (m_nRecentsType == RECENTS_TYPE_RECORD)
		{
			;
		}
		else if (bClickChecked)// 打电话
		{
			LOCAL_RECENTS_CALL_INFO callInfo;
			if(CBluetooth::GetInstance()->GetLocalCallInfo(nItemClicked, callInfo, (E_BT_PBAP_SYNC_PATH_T)m_nRecentsType))
			{
				CBluetooth::GetInstance()->Dial(callInfo.ciNumber);
			}
		}
		UpdateRecentsLBChilds();
	}
}

void CBluetoothDlg::UpdatePairedLBChilds()
{
	UpdateLBChilds(L"paired_recs_listbox", L"paired_delete");
}

void CBluetoothDlg::UpdateLBChilds(LPCTSTR lpszListboxName, LPCTSTR lpszChildName)
{
	CWceUiListBox* pRecentsListbox = (CWceUiListBox*)GetLayerByName(lpszListboxName);
	CWceUiLayer *pLayerDelete = GetLayerByName(lpszChildName);
	if (!pRecentsListbox || !pLayerDelete)
	{
		return;
	}

	int nIndex = pRecentsListbox->GetCurSel();
	if (nIndex >= 0)
	{
		pLayerDelete->ShowLayer(TRUE);
		int y = nIndex*pRecentsListbox->GetItemHeight() - pRecentsListbox->GetScrollPos();

		CRect src;
		WCEUI_POSITION_WAY way = pLayerDelete->GetStyle()->GetPosition(src);
		src.OffsetRect(0, y-src.top);

		pLayerDelete->GetStyle()->SetPosition(&src, way);
		pLayerDelete->GetCommonStyle()->SetPosition(&src, way);
		pLayerDelete->GetCommonStyle()->SetUsePosition(TRUE);
	}
	else
	{
		pLayerDelete->ShowLayer(FALSE);
	}
}


void CBluetoothDlg::UpdateRecentsLBChilds()
{
	UpdateLBChilds(L"recents_listbox", L"recents_delete");

// 	CWceUiListBox* pRecentsListbox = (CWceUiListBox*)GetLayerByName(L"recents_listbox");
// 	CWceUiLayer *pLayerDelete = GetLayerByName(L"recents_delete");
// 	ASSERT(pRecentsListbox && pLayerDelete);
// 	int nIndex = pRecentsListbox->GetCurSel();
// 	if (nIndex >= 0)
// 	{
// 		pLayerDelete->ShowLayer(TRUE);
// 		int y = nIndex*pRecentsListbox->GetItemHeight() - pRecentsListbox->GetScrollPos();
// 
// 		CRect src;
// 		WCEUI_POSITION_WAY way = pLayerDelete->GetStyle()->GetPosition(src);
// 		src.OffsetRect(0, y-src.top);
// 
// 		pLayerDelete->GetStyle()->SetPosition(&src, way);
// 		pLayerDelete->GetCommonStyle()->SetPosition(&src, way);
// 		pLayerDelete->GetCommonStyle()->SetUsePosition(TRUE);
// 	}
// 	else
// 	{
// 		pLayerDelete->ShowLayer(FALSE);
// 	}

}

void CBluetoothDlg::OnBnLongClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"key_0"))	// 长按是+号键
	{
		GetWindow()->PostMessage(WM_CHAR, (WPARAM)L'+', 0);
	}

	__super::OnBnLongClick(pButton);
}
extern "C" void BZ_ConnectByBDAddr(BT_ADDR paddr);
void CBluetoothDlg::OnBnClick(CWceUiButton* pButton)
{
	// function buttons
	if (OnBnFunClick(pButton) || OnBnPhonebookClick(pButton) || OnBnRecentsClick(pButton)
		|| OnBnKeypadClick(pButton) || OnBnMusicClick(pButton))
	{
		return;
	}
	// connect
	else if (pButton->IsEqualName(L"connect"))
	{
		// 如果点击太快,有可能计时器还没到,取消操作
 		KillTimer(TIMER_ID_BT_UNCONNECTED);
		CWceUiListBox* pPairedRecsListbox = (CWceUiListBox*)GetLayerByName(L"paired_recs_listbox");
		if (pPairedRecsListbox )
		{
			int index = pPairedRecsListbox->GetCurSel();
			if (index >= 0)
			{
				CBluetooth::GetInstance()->ConnectByPairedRec((UINT)index);
				return;
			}
		}
 		CBluetooth::GetInstance()->Connect();	// 如果没有执行上面的connect代码，直接connect
	}
	else if (pButton->IsEqualName(L"disconnect"))
	{
 		KillTimer(TIMER_ID_BT_CONNECTED);
 		CBluetooth::GetInstance()->Disconnect();
	}
	else if (pButton->IsEqualName(L"paired_delete"))
	{
		DeletePairedItem();
	}
	// call
	else if (pButton->IsEqualName(L"answer"))
	{
		CBluetooth::GetInstance()->AnswerCall();
	}
	else if (pButton->IsEqualName(L"decline"))
	{
		CBluetooth::GetInstance()->TerminateCall();
	}
	else if (pButton->IsEqualName(L"back"))
	{
		if (m_CurFunType == BT_FUN_RECENTS && m_bShowRecentsListUI)
		{
			ShowRecentsListbox(FALSE);	// 如果是在通话记录画面,需返回到功能按钮状态
		}
		else if (m_CurFunType == BT_FUN_PHONEBOOK && m_bShowQueryUI)
		{
			ShowPBQueryUI(FALSE);
		}
		else
		{
			__super::OnBnClick(pButton);
		}
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}

void CBluetoothDlg::SyncPBData(BOOL bForceResync)
{
	if (!CBluetooth::GetInstance()->IsConnected())
	{
		return;
	}

	CWceUiListBox *plistbox = (CWceUiListBox*)GetLayerByName(L"phonebook_listbox");
	ASSERT(plistbox);
	// 清空之前的数据
	plistbox->ResetContent();

	BOOL bNeedSync = BZ_PBMGRSyncIsInterruped(BT_PBAP_SYNC_PATH_ALL_PHONEBOOK);
	if(bForceResync || bNeedSync)
	{
		CBluetooth::GetInstance()->PBMGR_Sync(BT_PBAP_SYNC_PATH_ALL_PHONEBOOK);
		// 重新下载电话本,需更新数据库,先把数据数据清除
		try
		{
			m_db.execDML(L"delete from contacts;");
			m_nUpdatedRecords = 0;
		}
		catch (CppSQLite3Exception& e)
		{
			RETAILMSG(1, (L"db delete error code = %d %s \n", e.errorCode(), e.errorMessage()));
		}
		// 
		ShowWaitCursor(TRUE);
	}
	else
	{
		int total_count = m_db.execScalar(L"select count(*) from contacts;");
		AddPB2Listbox(0, total_count);
	}
}

BOOL CBluetoothDlg::OnBnPhonebookClick(CWceUiButton* pButton)
{
	CWceUiListBox *plistbox = (CWceUiListBox*)GetLayerByName(L"phonebook_listbox");
	ASSERT(plistbox);
	if (pButton->IsEqualName(L"phonebook_sync"))	// 下载电话本
	{
		SyncPBData(TRUE);
	}
	else if (pButton->IsEqualName(L"phonebook_query"))
	{
		if (CBluetooth::GetInstance()->IsConnected())
		{
			ShowPBQueryUI(TRUE);
		}
	}
	else if (pButton->IsEqualName(L"phonebook_key_backspace"))
	{
		CWceUiEditBox* peditbox = (CWceUiEditBox*)GetLayerByName(L"phonebook_query_string");
		ASSERT(peditbox);
		if (_tcslen(peditbox->GetData()) > 0)	// 如果已经是空了,不用再刷新了
		{
			GetWindow()->PostMessage(WM_CHAR, (WPARAM)L'\b', 0);
			GetWindow()->PostMessage(UI_MSG_BT_PB_QUERY_CHANGE, 0, 0);
		}
	}
	else 
	{
		return FALSE;
	}

	return TRUE;
}

void CBluetoothDlg::Call(int nIndex, E_BT_PBAP_SYNC_PATH_T SyncPath)
{
	int total_count = (int)CBluetooth::GetInstance()->PBMGR_GetCount(SyncPath);
	if (nIndex >= 0 && nIndex < total_count)
	{
		UI_PBMGRRECORD record;
		if (CBluetooth::GetInstance()->PBMGR_GetRecords(SyncPath, nIndex, &record))
		{
			CBluetooth::GetInstance()->Dial((LPCTSTR)record.pswNumber);
		}	
	}
}

void CBluetoothDlg::AddRecentsCall2Listbox(E_BT_PBAP_SYNC_PATH_T eSyncPath)
{
	CWceUiListBox *plistbox = (CWceUiListBox*)GetLayerByName(L"recents_listbox");
	ASSERT(plistbox);

	plistbox->ResetContent();
	CArray<LOCAL_RECENTS_CALL_INFO> callinfo;
	CBluetooth::GetInstance()->GetLocalCallInfo(callinfo, eSyncPath);
	for (int i=0; i<callinfo.GetCount(); i++)
	{
		CString str;
		HFONT hfont = plistbox->GetStyle()->GetFontCtrl()->GetFont();
		str.Format(L" %d", i+1);
		_AddSpace2Str(str, 10, hfont);
		str += (LPCTSTR)callinfo.GetAt(i).ciName;
		_AddSpace2Str(str, 30, hfont);
		TCHAR buffer[64];
		_tcscpy(buffer, callinfo.GetAt(i).ciNumber);
		ui_format_tel_number(buffer);
		str += buffer;

		LOCAL_RECENTS_CALL_INFO &ci = callinfo.GetAt(i);
		_stprintf(buffer, L"%04d/%02d/%02d %02d:%02d", 
			ci.ciTime.wYear, ci.ciTime.wMonth, ci.ciTime.wDay,ci.ciTime.wHour, ci.ciTime.wMinute);
		_AddSpace2Str(str, 65, hfont);
		str += buffer;


		plistbox->AddString(str);
	}

	plistbox->SetCurSel(0);
	UpdateRecentsLBChilds();
}

void CBluetoothDlg::AddRecentsRecord2Listbox()
{
	CWceUiListBox *plistbox = (CWceUiListBox*)GetLayerByName(L"recents_listbox");
	ASSERT(plistbox);

	plistbox->ResetContent();
	UpdateRecentsLBChilds();
}

BOOL CBluetoothDlg::OnBnRecentsClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"recents_missed"))	// 下载未接电话
	{
		m_nRecentsType = BT_PBAP_SYNC_PATH_MISSED_CALLHISTORY;
		AddRecentsCall2Listbox((E_BT_PBAP_SYNC_PATH_T)m_nRecentsType);
	}
	else if (pButton->IsEqualName(L"recents_received"))	// 下载已接
	{
		m_nRecentsType = BT_PBAP_SYNC_PATH_RECEIVE_CALLHISTORY;
		AddRecentsCall2Listbox((E_BT_PBAP_SYNC_PATH_T)m_nRecentsType);
	}
	else if (pButton->IsEqualName(L"recents_dialed"))	// 打出电话
	{
		m_nRecentsType = BT_PBAP_SYNC_PATH_DIALED_CALLHISTORY;
		AddRecentsCall2Listbox((E_BT_PBAP_SYNC_PATH_T)m_nRecentsType);
	}
	else if (pButton->IsEqualName(L"recents_record"))
	{
		m_nRecentsType = RECENTS_TYPE_RECORD;
		AddRecentsRecord2Listbox();
	}
	else if (pButton->IsEqualName(L"recents_delete"))
	{
		DeleteRecentsItem();
	}
	else 
	{
		return FALSE;
	}

	ShowRecentsListbox(TRUE);

	return TRUE;
}

void CBluetoothDlg::DeleteRecentsItem()
{
	CWceUiListBox *plistbox = (CWceUiListBox*)GetLayerByName(L"recents_listbox");
	if(!plistbox)
	{
		return;
	}

	if (m_nRecentsType == RECENTS_TYPE_RECORD)
	{
		; //
	}
	else	// 这里直接从列表删除记录,并删除local call info
	{
		int nIndex = plistbox->GetCurSel();
		if (nIndex >=0)
		{
			plistbox->DeleteString(nIndex);
			if (plistbox->GetCount()>0 && nIndex>=plistbox->GetCount())
			{
				nIndex--;
			}
			plistbox->SetCurSel(nIndex);
			UpdateRecentsLBChilds();
			CBluetooth::GetInstance()->RemoveLocalCallInfo(nIndex, (E_BT_PBAP_SYNC_PATH_T)m_nRecentsType);
		}
	}
}


void CBluetoothDlg::DeletePairedItem()
{
	CWceUiListBox *plistbox = (CWceUiListBox*)GetLayerByName(L"paired_recs_listbox");
	if(!plistbox)
	{
		return;
	}

	// 这里直接从列表删除记录,并删除保存的配对信息
	int nIndex = plistbox->GetCurSel();
	if (nIndex >=0)
	{
		if (CBluetooth::GetInstance()->DeletePairedRec(nIndex))
		{
			plistbox->DeleteString(nIndex);
			if (plistbox->GetCount()>0 && nIndex>=plistbox->GetCount())
			{
				nIndex--;
			}
			plistbox->SetCurSel(nIndex);
			UpdatePairedLBChilds();
		}
		else	// 删除失败
		{
			CWceUiLoadString str(L"BT_DEVICE_CONNECTED");
			ShowTipMessage(str.GetString());
		}
	}
}

void CBluetoothDlg::ShowPBQueryUI(BOOL bShow)
{
	if ((bShow && !m_bShowQueryUI) || (!bShow && m_bShowQueryUI))
	{
		m_bShowQueryUI = bShow;
		// 更新列表数据
		if (bShow)
		{
			CWceUiEditBox *peditbox = (CWceUiEditBox*)GetLayerByName(L"phonebook_query_string");
			ASSERT(peditbox);
			peditbox->SetData(L"");
			UpdatePBQ2Listbox();
		}
		else
		{
			SyncPBData(FALSE);
		}
	}

	CWceUiLayer* pframe = GetChildByName(L"phonebook_frame");
	ASSERT(pframe);
	CWceUiLayer* psync = pframe->GetChildByName(L"phonebook_sync");
	CWceUiLayer* pquery = pframe->GetChildByName(L"phonebook_query");
	CWceUiListBox* plistbox = (CWceUiListBox*)pframe->GetChildByName(L"phonebook_listbox");

	CWceUiLayer* pkeypad = pframe->GetChildByName(L"phonebook_keypad");
	CWceUiLayer* pbackspace = pframe->GetChildByName(L"phonebook_key_backspace");
	CWceUiLayer* peditbox = pframe->GetChildByName(L"phonebook_query_string");
	ASSERT(psync && pquery && plistbox && pkeypad && pbackspace && peditbox);

	psync->ShowLayer(!bShow);
	pquery->ShowLayer(!bShow);

	pkeypad->ShowLayer(bShow);
	pbackspace->ShowLayer(bShow);
	peditbox->ShowLayer(bShow);

	CRect src;
	WCEUI_POSITION_WAY way = plistbox->GetStyle()->GetPosition(src);
	if (bShow)
	{
		if (WceUiGetScreenWidth() == 1024 && WceUiGetScreenHeight() == 600)
			src.SetRect(39, 88, 39+391, 88+395);
		else
			src.SetRect(39, 88, 39+322, 88+275);
	}
	else
	{
		if (WceUiGetScreenWidth() == 1024 && WceUiGetScreenHeight() == 600)
			src.SetRect(39, 88, 39+946, 88+395);
		else
			src.SetRect(39, 88, 39+722, 88+275);
	}
	plistbox->GetStyle()->SetPosition(&src, way);
}

void CBluetoothDlg::ShowRecentsListbox(BOOL bShow)
{
	m_bShowRecentsListUI = bShow;

	CWceUiLayer* precents = GetChildByName(L"recents_frame");
	ASSERT(precents);

	CWceUiListBox* plistbox = NULL;
	CWceUiLayer *pInd = precents->FirstChild();
	while (pInd)
	{
		if (pInd->IsEqualName(L"recents_listbox"))	// 通话记录中列表先是隐藏的
		{
			plistbox = (CWceUiListBox*)pInd;
			pInd->ShowLayer(bShow);
		}
		else
		{
			pInd->ShowLayer(!bShow);
		}
		pInd = precents->NextChild(pInd);
	}

	// 如果列表为空时，显示列表为空的提示信息
	if (plistbox)
	{
		CWceUiLayer* ptip = GetLayerByName(L"empty_listbox_tip");
		if (ptip)
		{
			if (bShow && plistbox->GetCount()<=0)
			{
				ptip->ShowLayer(TRUE);
			}
			else
			{
				ptip->ShowLayer(FALSE);
			}

			// 显示不同内容时提示语略有不同
			switch (m_nRecentsType)
			{
			case BT_PBAP_SYNC_PATH_MISSED_CALLHISTORY:
				ptip->SetTextResID(L"BT_RECENTS_MISSED_EMPTY");
				break;
			case BT_PBAP_SYNC_PATH_RECEIVE_CALLHISTORY:
				ptip->SetTextResID(L"BT_RECENTS_RECEIVED_EMPTY");
				break;
			case BT_PBAP_SYNC_PATH_DIALED_CALLHISTORY:
				ptip->SetTextResID(L"BT_RECENTS_DIALED_EMPTY");
				break;
			case RECENTS_TYPE_RECORD:
				ptip->SetTextResID(L"BT_RECENTS_RECORD_EMPTY");
				break;
			default:
				break;
			}
		}
	}
}

void CBluetoothDlg::OnKeyCall()
{
	CWceUiEditBox* peditbox = (CWceUiEditBox*)GetLayerByName(L"dial_number");
	ASSERT(peditbox);
	CString strNumber = peditbox->GetData();
	if (strNumber.IsEmpty())	// 如果为空,显示上一次拨打的号码
	{
		peditbox->SetData(m_strOldDialNumber);
		peditbox->InvalidateRect();
	}
	else if (strNumber == L"*")
	{
		BT_ADDR addr;
		BZ_ConvWcharToBTAddr(addr, L"66:66:88:88:88:88");
		BZ_ConnectByBDAddr(addr);
	}
	else
	{
		m_strOldDialNumber = strNumber;	//记下拨出的号码
		CBluetooth::GetInstance()->Dial(m_strOldDialNumber);
		m_bIsDialNumber = TRUE;
	}
}

BOOL CBluetoothDlg::OnBnKeypadClick(CWceUiButton* pButton)
{
	const int KEY_COUNT = 14;
	LPCTSTR key_layers[] = {L"key_0", L"key_1", L"key_2", L"key_3", L"key_4", L"key_5", L"key_6", 
		L"key_7", L"key_8", L"key_9", L"key_star", L"key_sharp", L"key_backspace",L"key_plus"};
	TCHAR key_char[] = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'*', L'#', L'\b',L'+'};

	if (pButton->IsEqualName(L"keypad_call"))	//
	{
		OnKeyCall();
	}
	else 
	{
		int i = 0;
		for (i=0; i<KEY_COUNT; i++)
		{
			if (pButton->IsEqualName(key_layers[i]))
			{
				break;
			}
		}

		if (i<KEY_COUNT)	// 匹配到了
		{
			GetWindow()->PostMessage(WM_CHAR, (WPARAM)key_char[i], 0);
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CBluetoothDlg::OnBnMusicClick(CWceUiButton* pButton)
{
	CWceUiLoadString str;
	if (pButton->IsEqualName(L"music_prev"))	//
	{
		if (CBluetooth::GetInstance()->GetMusicPlayStatus() != BT_MUSICPLAY_STATUS_DISCONNECTED)
		{
			str.SetTextResID(L"PLAY_PREV");
			ShowTipMessage(str.GetString());

			CBluetooth::GetInstance()->MusicPrev();
		}

// 		BOOL b = CBluetooth::GetInstance()->SPPConnect();
// 		RETAILMSG(1, (L"[spp] sppconnect return %s", b ? L"TRUE" : L"FALSE"));
// 		if (BZ_SPPGetConnectStatus())
// 		{
// 			CDEPSerial dep;
// 			dep.Open(L"COM8:");
// 		}
	}
	else if (pButton->IsEqualName(L"music_play"))	//
	{
		if ( !CBluetooth::GetInstance()->IsConnected() 
			&& CSourceManager::GetInstance()->GetFrontSrcID() != SOURCE_BT)
		{
			// 未连接,不能切源
			str.SetTextResID(L"MIS_BT_NOT_CONNECTED");
			ShowTipMessage(str.GetString());
		}

		// 如果BT已连接且当前源不是BT音乐,点击play时切换到BT音乐
		if ( CBluetooth::GetInstance()->IsConnected() 
			&& CSourceManager::GetInstance()->GetFrontSrcID() != SOURCE_BT)
		{
			CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_BT);

			// 刷新ID3信息
			OnMusicEvent(BZ_IND_AVRCP_ID3_SUCCESS);
		}
		else
		{
			if (CBluetooth::GetInstance()->GetMusicPlayStatus() == BT_MUSICPLAY_STATUS_PLAYING)
			{
				str.SetTextResID(L"PLAY_PAUSE");
				ShowTipMessage(str.GetString());

				CBluetooth::GetInstance()->MusicPause();
			}
			else if(CBluetooth::GetInstance()->GetMusicPlayStatus() != BT_MUSICPLAY_STATUS_DISCONNECTED)
			{
				str.SetTextResID(L"PLAY_PLAY");
				ShowTipMessage(str.GetString());

				CBluetooth::GetInstance()->MusicPlay();
			}
		}

	}
	else if (pButton->IsEqualName(L"music_next"))	//
	{
		if (CBluetooth::GetInstance()->GetMusicPlayStatus() != BT_MUSICPLAY_STATUS_DISCONNECTED)
		{
			str.SetTextResID(L"PLAY_NEXT");
			ShowTipMessage(str.GetString());

			CBluetooth::GetInstance()->MusicNext();
		}
	}
	else 
	{
		return FALSE;
	}

	return TRUE;
}


void CBluetoothDlg::ShowTipMessage(LPCTSTR message)
{
	CWceUiLayer *player = GetLayerByName(L"message_tip");
	if (player)
	{
		player->SetText(message);
		player->ShowLayer(TRUE);
		SetTimer(TIMER_ID_HIDE_MESSAGE_TIP, 2000);
	}
}


LRESULT CBluetoothDlg::OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam)
{
	return __super::OnCommand(uCommand, wParam, lParam);
}

void CBluetoothDlg::OnLocalCallInfoChange(int nRecentsType)
{
	if (m_nRecentsType == nRecentsType)	// 当前显示的数据有更新了,刷新下数据
	{
		if (m_CurFunType == BT_FUN_RECENTS)
		{
			if (nRecentsType == RECENTS_TYPE_RECORD)
			{
				AddRecentsRecord2Listbox();
			}
			else
			{
				AddRecentsCall2Listbox((E_BT_PBAP_SYNC_PATH_T)nRecentsType);
			}
		}
	}
}

LRESULT CBluetoothDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_BT_CONNECT_STATUS_CHANGE)
	{
		OnConnectEvent((DWORD)lParam);
	}
	else if (message == UI_MSG_BT_CALL_EVENT)
	{
		OnCallEvent((DWORD)lParam);
	}
	else if (message == UI_MSG_BT_PBMGR_SYNC_EVENT)
	{
		OnPBMGRSyncEvent((DWORD)lParam, HIWORD(wParam), (E_BT_PBAP_SYNC_PATH_T)LOWORD(wParam));
	}
	else if (message == UI_MSG_BT_MUSIC_EVENT)
	{
		OnMusicEvent((DWORD)lParam);
	}
	else if (message == UI_MSG_BT_LOCAL_CALLINFO_CHANGE)
	{
		OnLocalCallInfoChange((int)lParam);
	}
	else if (message == UI_MSG_BT_PB_QUERY_CHANGE)
	{
		UpdatePBQ2Listbox();
	}
	else if (message == NAVI_MSG_LAN_CHANGE)
	{
		ShowTitleInfo();	// 支持多语言
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

void CBluetoothDlg::OnKeyDigital(UINT nDigitalKey)
{
	if (CDlgManager::GetInstance()->GetCurDlgID() == CBluetoothDlg_ID)
	{
		OnFunChange(BT_FUN_DIAL);

		TCHAR key_char[] = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9'};
		GetWindow()->PostMessage(WM_CHAR, (WPARAM)key_char[nDigitalKey%10], 0);
	}
}

void CBluetoothDlg::OnPBMGRSyncEvent(DWORD dwEventID, int nTotalRecords, E_BT_PBAP_SYNC_PATH_T SyncPath)
{
	switch (dwEventID)
	{
	case BZ_IND_PBAP_NOT_SUPPORT:
	case BZ_IND_PBAP_SYNC_COMPLETE:
	case BZ_IND_PBAP_SYNC_FAIL:
		ShowWaitCursor(FALSE);
		break;
	case BZ_IND_PBAP_PBRECORDS:
		if (SyncPath == BT_PBAP_SYNC_PATH_ALL_PHONEBOOK)
		{
			int added_count = m_db.execScalar(L"select count(*) from contacts;");
			UINT total_count = CBluetooth::GetInstance()->PBMGR_GetCount(SyncPath);
			AddPB2DB(added_count, total_count-added_count);
			SetTimer(TIMER_ID_BT_UPDATE_DB_STRING, 50);
			//
			ShowWaitCursor(FALSE);
		}
		break;
	case BZ_IND_PBAP_TIMEOUT:
		break;
	case BZ_IND_PBAP_FILLERR:
		break;
	case BZ_IND_PBAP_START_OVER:
		break;
	default:
		break;
	}
}

// 在字符串后面加空格,使字符串输出长度为len个空格的长度，如果字符串已经超过长度了,不加空格
static void _AddSpace2Str(CString& str, int len, HFONT hfont)
{
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	dc.SelectObject(hfont);

	SIZE sizeSpace;
	GetTextExtentPoint32(dc.GetSafeHdc(), L" ", 1, &sizeSpace);
	SIZE sizeStr;
	GetTextExtentPoint32(dc.GetSafeHdc(), str, str.GetLength(), &sizeStr);

	for (int i=0; i<len-sizeStr.cx/sizeSpace.cx; i++)
	{
		str += L" ";
	}
}

// bShort来标识生成长串还是短串,当搜索界面时，列表框变窄了
static void _FormatPBListboxString(CString &str, HFONT hfont, int nIndex, LPCTSTR name, LPCTSTR number, BOOL bShort)
{
	int space1 = WceUiGetScreenWidth() == 1024 ? 25 : 20;
	int space2 = WceUiGetScreenWidth() == 1024 ? 80 : 55;
	str.Format(L"       %d", nIndex);
	if (bShort)
	{
		space1 = 8;
		space2 = 20;
		str.Format(L" %d", nIndex);
	}

	_AddSpace2Str(str, space1, hfont);
	str += (LPCTSTR)name;
	_AddSpace2Str(str, space2, hfont);
	TCHAR buffer[128];
	_tcscpy(buffer, number);
	ui_format_tel_number(buffer);
	str += (LPCTSTR)buffer;
}

BOOL CBluetoothDlg::AddPB2DB(int nFirst, int nCount)
{
	int total_count = CBluetooth::GetInstance()->PBMGR_GetCount(BT_PBAP_SYNC_PATH_ALL_PHONEBOOK);
	if (nFirst<0 || nFirst>=total_count	|| (nFirst+nCount) > total_count)
	{
		return FALSE;
	}

	try
	{
		m_db.execDML(L"begin transaction;");
		CppSQLite3Statement stmt = m_db.compileStatement(L"insert into contacts values (?, ?, ?, ?, ?, ?);");

		UI_PBMGRRECORD record;
		for (int i=nFirst;i<nFirst+nCount;i++)
		{
			if(CBluetooth::GetInstance()->PBMGR_GetRecords(BT_PBAP_SYNC_PATH_ALL_PHONEBOOK, i, &record))
			{
				// 添加DB数据
				stmt.bind(1, i);
				stmt.bind(2, record.pswName);
				stmt.bind(3, record.pswNumber);
				stmt.bind(4, tzutil::GetStringPYCode(record.pswName));
				stmt.execDML();
				stmt.reset();
			}
		}
		m_db.execDML(L"commit transaction;");
	}
	catch (CppSQLite3Exception& e)
	{
		m_db.execDML(L"rollback transaction;");
		RETAILMSG(1, (L"db error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}

	return TRUE;
}

BOOL CBluetoothDlg::UpdateDBString()
{
	CWceUiListBox *plistbox = (CWceUiListBox*)GetLayerByName(L"phonebook_listbox");
	ASSERT(plistbox);
	HFONT hfont = plistbox->GetStyle()->GetFontCtrl()->GetFont();

	try
	{
		int total = m_db.execScalar(L"select count(*) from contacts;");
		int endRecords = min(total, m_nUpdatedRecords+UPDATE_DB_STRING_MAX_COUNT);

		m_db.execDML(L"begin transaction;");
		TCHAR szSQL[512];
		CString longstr;
		CString shortstr;
		int first = m_nUpdatedRecords;
		for (int i=m_nUpdatedRecords; i<endRecords; i++)
		{
			_stprintf(szSQL, L"select * from contacts where id = %d;", i);
			CppSQLite3Query q = m_db.execQuery(szSQL);
			if (!q.eof())
			{
				_FormatPBListboxString(longstr, hfont, i+1, q.fieldValue(FIELD_NAME), q.fieldValue(FIELD_NUMBER), FALSE);
				_FormatPBListboxString(shortstr, hfont, i+1, q.fieldValue(FIELD_NAME), q.fieldValue(FIELD_NUMBER), TRUE);
				longstr.Replace(L"'", L"''");	// sql语句中如果有引号，需用双引号表示
				shortstr.Replace(L"'", L"''");
				_stprintf(szSQL, L"update contacts set longstring = '%s', shortstring = '%s' where id = %d;",
					longstr, shortstr, i);
				m_db.execDML(szSQL);

				m_nUpdatedRecords++;
			}
		}
		m_db.execDML(L"commit transaction;");
	}
	catch (CppSQLite3Exception& e)
	{
		m_db.execDML(L"rollback transaction;");
		RETAILMSG(1, (L"db error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}

	return TRUE;
}

BOOL CBluetoothDlg::AddPB2Listbox(int nFirst, int nCount)
{
	CWceUiListBox *pListbox = (CWceUiListBox*)GetLayerByName(L"phonebook_listbox");
	ASSERT(pListbox);
	try
	{
		int total_count = m_db.execScalar(L"select count(*) from contacts;");
		if (nFirst<0 || nFirst>=total_count	|| pListbox == NULL || (nFirst+nCount) > total_count)
		{
			return FALSE;
		}
		TCHAR szSQL[64];
		_stprintf(szSQL, L"select * from contacts where id >= %d and id < %d;", nFirst, nFirst+nCount);
		CppSQLite3Query q = m_db.execQuery(szSQL);
		while (!q.eof())
		{
			LPCTSTR lpszItem = m_bShowQueryUI ? q.fieldValue(FIELD_SHORTSTRING) : q.fieldValue(FIELD_LONGSTRING);
			if (lpszItem)
			{
				pListbox->AddString(lpszItem);
			}
			q.nextRow();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		m_db.execDML(L"rollback transaction;");
		RETAILMSG(1, (L"db error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}

	if (pListbox->GetCurSel() < 0)
	{
		pListbox->SetCurSel(0);		// 如果没有选中的数据,选中第一项
	}

	return TRUE;
}

void CBluetoothDlg::UpdatePBQ2Listbox()
{
	CWceUiLayer* pframe = GetChildByName(L"phonebook_frame");
	ASSERT(pframe);
	CWceUiListBox *pListbox = (CWceUiListBox*)pframe->GetChildByName(L"phonebook_listbox");
	CWceUiEditBox *peditbox = (CWceUiEditBox*)pframe->GetChildByName(L"phonebook_query_string");
	ASSERT(pListbox && peditbox);

	pListbox->ResetContent();
	m_PBQueryResult.RemoveAll();
	try
	{
		TCHAR szSQL[128];
		if (_tcslen(peditbox->GetData()) < 64)	// 防止越界
		{
			_stprintf(szSQL, L"select * from contacts where pyname like '%%%s%%' or name like '%%%s%%';", peditbox->GetData(), peditbox->GetData());
			CppSQLite3Query q = m_db.execQuery(szSQL);
			while (!q.eof())
			{
				pListbox->AddString(q.fieldValue(FIELD_SHORTSTRING));
				m_PBQueryResult.Add(q.fieldValue(FIELD_NUMBER));
				q.nextRow();
			}
		}
	}
	catch (CppSQLite3Exception& e)
	{
		RETAILMSG(1, (L"db select error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}

	pListbox->SetCurSel(0);
}

void CBluetoothDlg::OnCallEvent(DWORD dwEventID)
{
	if (dwEventID == BZ_IND_CALL_TERMINATED)
	{
		if (m_bIsDialNumber)	// 如果是拨号打出的电话，通话结束后清除之前的号码
		{
			m_bIsDialNumber = FALSE;
			CWceUiEditBox* peditbox = (CWceUiEditBox*)GetLayerByName(L"dial_number");
			ASSERT(peditbox);
			peditbox->SetData(L"");
		}
	}
}

void CBluetoothDlg::OnMusicEvent(DWORD dwEventID)
{
	// 不是BT源,忽略这些消息
	if (CSourceManager::GetInstance()->GetFrontSrcID() != SOURCE_BT)
	{
		return;
	}

	switch(dwEventID)
	{
	case BZ_IND_AVRCP_ID3_SUCCESS:	
		ShowID3Tag();
		break;
	case BZ_IND_AVRCP_PlAY_STATE_CHANGE:
		UpgradeProgress();
		break;
	case BZ_IND_AVRCP_CONNECT_SUCCESS:
		CBluetooth::GetInstance()->MusicPlay();
		break;
	default:
		break;

	}
}

void CBluetoothDlg::UpgradeProgress()
{
	const A2DPPlayState_T* pstate = CBluetooth::GetInstance()->GetPlayStateInfo();
//	RETAILMSG(1, (L"[nvai_car]: total:%d, current: %d. \n", pstate->u4SongTotalTime/1000, pstate->U4CurPlayTime/1000));
}

void CBluetoothDlg::ShowID3Tag()
{
	CWceUiLayer *pframe = GetChildByName(L"music_frame");
	ASSERT(pframe);
	CWceUiLayer *partist = pframe->GetChildByName(L"music_artist");
	CWceUiLayer *ptitle = pframe->GetChildByName(L"music_title");
	CWceUiLayer *palbum = pframe->GetChildByName(L"music_album");
	const MediaAttribute_T* pattribute = CBluetooth::GetInstance()->GetMediaAttribute();
	if (partist)
	{
		partist->SetText((LPCTSTR)pattribute->uAttributeArtist);
	}

	if (ptitle)
	{
		ptitle->SetText((LPCTSTR)pattribute->uAttributeTitle);
	}

	if (palbum)
	{
		palbum->SetText((LPCTSTR)pattribute->uAttributeAlbum);
	}
}

void CBluetoothDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		ShowDeviceInfo();
	}

	__super::OnDlgShow(bShow);
}

void CBluetoothDlg::OnConnectEvent(DWORD dwEventID)
{
	E_BT_CONNECT_STATUS_T status;

	if (dwEventID == BZ_IND_CONNECT_STATUS_CHANGED)	// 其它事件暂不处理
	{
		status = CBluetooth::GetInstance()->GetConnectStatus();
	}
	else if (dwEventID == BZ_IND_CREATE_CONNECT_SUCCESS)
	{
		status = BT_CONNECT_STATUS_CONNECTED;
	}
	else if (dwEventID == BZ_IND_DISCONNECT_SUCCESS || dwEventID == BZ_IND_CREATE_CONNECT_FAIL)
	{
		status = BT_CONNECT_STATUS_UNCONNECT;
	}
	else 
	{
		return;		// 其它暂不处理
	}

	switch (status)
	{
	case BT_CONNECT_STATUS_UNCONNECT:
		KillTimer(TIMER_ID_BT_CONNECTED);
		SetTimer(TIMER_ID_BT_UNCONNECTED, 300);
// 		// 如果当前源是BT，有可能激活了A2DP
// 		if ( CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_BT)
// 		{
// 			CBluetooth::GetInstance()->A2DPActive(FALSE);
// 		}
		break;
	case BT_CONNECT_STATUS_CONNECTING:
		break;
	case BT_CONNECT_STATUS_CONNECTED:
		// 检测是否有新数据的函数在连接后马上调用返回的结果不正确
		// A2DP马上连接也会失败
		// 所以连接后起个计时器来完成相应的操作
		KillTimer(TIMER_ID_BT_UNCONNECTED);
		SetTimer(TIMER_ID_BT_CONNECTED, 800);
		break;
	case BT_CONNECT_STATUS_DISCONNECTING:
		ShowWaitCursor(FALSE);	// 下载电话本时,断开蓝牙,停止下载
		break;
	default:
		break;
	}

	// 连接状态变化，有可能设备名也会变
	ShowDeviceInfo();

	ShowTitleInfo();	// 更新标题

}


void CBluetoothDlg::ShowTitleInfo()
{
	CWceUiLayer *ptitle = GetLayerByName(L"title");
	if (ptitle)
	{
		CString strTitle;
		GetTitleInfo(strTitle);
		ptitle->SetText(strTitle);
	}
}

void CBluetoothDlg::GetTitleInfo(CString& strTitle)
{
	E_BT_CONNECT_STATUS_T status = CBluetooth::GetInstance()->GetConnectStatus();
	CWceUiLoadString strLoadTitle(L"MAIN_BLUETOOTH");
	CWceUiLoadString strLoad;
	CString strDeviceName;
	switch (status)
	{
	case BT_CONNECT_STATUS_UNCONNECT:
		strLoad.SetTextResID(L"BT_STATUS_UNCONNECT");
		break;
	case BT_CONNECT_STATUS_CONNECTING:
		strLoad.SetTextResID(L"BT_STATUS_CONNECTING");
		break;
	case BT_CONNECT_STATUS_CONNECTED:
		break;
	case BT_CONNECT_STATUS_DISCONNECTING:
		strLoad.SetTextResID(L"BT_STATUS_DISCONNECTING");
		break;
	default:
		break;
	}

	// 获取远端设备名称
	if (status == BT_CONNECT_STATUS_CONNECTED)
	{
		strTitle.Format(L"%s [%s]", strLoadTitle.GetString(), CBluetooth::GetInstance()->GetRemoteDeviceName());
	}
	else
	{
		strTitle.Format(L"%s [%s]", strLoadTitle.GetString(), strLoad.GetString());
	}

}

void CBluetoothDlg::ShowDeviceInfo()
{
	CWceUiLayer *ppincode = GetLayerByName(L"pin_code");
	ASSERT(ppincode);

	TCHAR wszPincode[MAX_LEN_PINCODE+3]={0};
	BZ_GetPinCode(wszPincode,MAX_LEN_PINCODE+3);
	ppincode->SetText(wszPincode);

// 	CWceUiLayer *pslavedevice = GetLayerByName(L"remote_device_name");
// 	ASSERT(pslavedevice);
// 	pslavedevice->SetText(CBluetooth::GetInstance()->GetRemoteDeviceName());

	TCHAR szLocalDevciceName[MAX_LEN_DEVICE_NAME];
	CWceUiLayer *plocaldevice = GetLayerByName(L"local_device_name");
	ASSERT(plocaldevice);
	BZ_GetLocalDevName(szLocalDevciceName);
	plocaldevice->SetText(szLocalDevciceName);
}

void CBluetoothDlg::OnFunChange(BT_FUN_BUTTON_TYPE type)
{
	if (m_CurFunType != type)
	{
		if (m_CurFunType == BT_FUN_PHONEBOOK)
		{
			InvalidateRect(NULL);	// 父窗口有no draw属性时，好像计算刷新区有问题,没有考虑父窗口的位置
		}

		// 如果是BT相关窗口,通知MCU, 当在A2DP时，不认为是在BT界面
		DLG_ID did = CDlgManager::GetInstance()->GetCurDlgID();
		BOOL bActive = (type != BT_FUN_MUSIC) && (did == CBluetoothDlg_ID);	// 后台启动BT时,不认为在BT
		if (!tzutil::IsBoolEqual(bActive, protocol::get_mcu_arm_status()->is_bt_active()))	// 如果跟之前状态不一致通知MCU
		{
			protocol::get_mcu_arm_status()->set_bt_active(bActive);
			CRpcMCU::GetInstance()->RPC_SetArmStatus(protocol::get_mcu_arm_status());
		}

		m_CurFunType = type;
		CheckFunButton(type);
		ShowFuncFrame(type);

		if (m_CurFunType == BT_FUN_MUSIC)
		{
			// 如果BT已连接且当前源不是BT音乐,点击时切换到BT音乐
			if ( CBluetooth::GetInstance()->IsConnected() 
				&& CSourceManager::GetInstance()->GetFrontSrcID() != SOURCE_BT)
			{
				CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_BT);

				// 刷新ID3信息
				OnMusicEvent(BZ_IND_AVRCP_ID3_SUCCESS);
			}
		}
	}
}


void CBluetoothDlg::CheckFunButton(BT_FUN_BUTTON_TYPE type)
{
	CWceUiButton* pButtonConnect = (CWceUiButton*)GetChildByName(L"fun_connect");
	CWceUiButton* pButtonDial = (CWceUiButton*)GetChildByName(L"fun_dial");
	CWceUiButton* pButtonRecents = (CWceUiButton*)GetChildByName(L"fun_recents");
	CWceUiButton* pButtonPhonebook = (CWceUiButton*)GetChildByName(L"fun_phonebook");
	CWceUiButton* pButtonMusic = (CWceUiButton*)GetChildByName(L"fun_music");
	ASSERT(pButtonMusic && pButtonConnect && pButtonDial && pButtonRecents && pButtonPhonebook);

	pButtonConnect->SetStatusStyleName(L"fun_connect_normal", CWceUiButton::STATUS_NORMAL);
	pButtonDial->SetStatusStyleName(L"fun_dial_normal", CWceUiButton::STATUS_NORMAL);
	pButtonRecents->SetStatusStyleName(L"fun_recents_normal", CWceUiButton::STATUS_NORMAL);
	pButtonPhonebook->SetStatusStyleName(L"fun_phonebook_normal", CWceUiButton::STATUS_NORMAL);
	pButtonMusic->SetStatusStyleName(L"fun_music_normal", CWceUiButton::STATUS_NORMAL);
	switch (type)
	{
	case BT_FUN_CONNECT:
		pButtonConnect->SetStatusStyleName(L"fun_connect_down", CWceUiButton::STATUS_NORMAL);
		break;
	case BT_FUN_DIAL:
		pButtonDial->SetStatusStyleName(L"fun_dial_down", CWceUiButton::STATUS_NORMAL);
		break;
	case BT_FUN_RECENTS:
		pButtonRecents->SetStatusStyleName(L"fun_recents_down", CWceUiButton::STATUS_NORMAL);
		break;
	case BT_FUN_PHONEBOOK:
		pButtonPhonebook->SetStatusStyleName(L"fun_phonebook_down", CWceUiButton::STATUS_NORMAL);
		break;
	case BT_FUN_MUSIC:
		pButtonMusic->SetStatusStyleName(L"fun_music_down", CWceUiButton::STATUS_NORMAL);
		break;
	default:
		break;
	}
}


void CBluetoothDlg::ShowFuncFrame(BT_FUN_BUTTON_TYPE type)
{
	CWceUiLayer* pconnect = GetChildByName(L"connect_frame");
	CWceUiLayer* pdial = GetChildByName(L"keypad_frame");
	CWceUiLayer* precents = GetChildByName(L"recents_frame");
	CWceUiLayer* pphonebook = GetChildByName(L"phonebook_frame");
	CWceUiLayer* pmusic = GetChildByName(L"music_frame");

	ASSERT(pconnect && pdial && precents && pphonebook && pmusic);

	pconnect->ShowLayer(FALSE);
	pdial->ShowLayer(FALSE);
	precents->ShowLayer(FALSE);
	pphonebook->ShowLayer(FALSE);
	pmusic->ShowLayer(FALSE);

	CBTDialEditBox* pdial_editbox = (CBTDialEditBox*)pdial->GetChildByName(L"dial_number");
	ASSERT(pdial_editbox);
	pdial_editbox->OnKillFocus();

	CWceUiEditBox* pquery_editbox = (CWceUiEditBox*)pphonebook->GetChildByName(L"phonebook_query_string");
	ASSERT(pquery_editbox);
	pquery_editbox->OnKillFocus();


	switch (type)
	{
	case BT_FUN_CONNECT:
		pconnect->ShowLayer(TRUE);
		break;
	case BT_FUN_DIAL:
		pdial->ShowLayer(TRUE);
		pdial_editbox->OnFocus();
		break;
	case BT_FUN_RECENTS:
		precents->ShowLayer(TRUE);
		ShowRecentsListbox(FALSE);
		break;
	case BT_FUN_PHONEBOOK:
		pphonebook->ShowLayer(TRUE);
		pquery_editbox->OnFocus();
		ShowPBQueryUI(FALSE);
		break;
	case BT_FUN_MUSIC:
		pmusic->ShowLayer(TRUE);
		break;
	default:
		break;
	}
}

void CBluetoothDlg::ShowWaitCursor(BOOL bShow)
{
	CWceUiAnimLayer* player = (CWceUiAnimLayer*)GetLayerByName(L"wait_cursor");
	if (player)
	{
		player->ShowLayer(bShow);
		if (bShow)
		{
			player->StartAnim(200);
		}
		else
		{
			player->StopAnim();
		}
	}
}

void CBluetoothDlg::PlayControl(UINT key, UINT param)
{
	BYTE byte0 = (BYTE)((param>>24) & 0xFF);

	switch (key)
	{
	case T_PREV:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PREV");
		CBluetooth::GetInstance()->MusicPrev();
		break;
	case T_NEXT:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_NEXT");
		CBluetooth::GetInstance()->MusicNext();
		break;
	case T_PLAY:	// param byte0 : 0 ~ 播放暂停复合键, 1 ~ 播放， 2 ~ 暂停
		if (byte0 == 1)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
			CBluetooth::GetInstance()->MusicPlay();
		}
		else if (byte0 == 2)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
			CBluetooth::GetInstance()->MusicPause();
		}
		else
		{
			if (CBluetooth::GetInstance()->GetMusicPlayStatus() == BT_MUSICPLAY_STATUS_PLAYING)
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
				CBluetooth::GetInstance()->MusicPause();
			}
			else if(CBluetooth::GetInstance()->GetMusicPlayStatus() != BT_MUSICPLAY_STATUS_DISCONNECTED)
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
				CBluetooth::GetInstance()->MusicPlay();
			}
		}
		break;
	case T_STOP:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_STOP");
		CBluetooth::GetInstance()->MusicStop();
		break;
	}
}

void CBluetoothDlg::OnKeyPlayControl(UINT key, UINT param)
{
	PlayControl(key, param);
}

////////////////////////  CKeypadLayer  ///////////////////////////////////////////////////

CKeypadLayer::CKeypadLayer()
{
	m_bMouseDown = FALSE;
	m_nClickItem = -1;
}

CKeypadLayer::~CKeypadLayer()
{

}

void CKeypadLayer::SetProperties(PCWceUiXmlElem pCreateElem)
{
	__super::SetProperties(pCreateElem);

	int col_gap = 0, row_gap = 0, col_count = 7;
	RECT rc;

 	LPCTSTR attr = pCreateElem->GetAttr(L"item_select");
 	if (attr) m_texSelect.SetBitmapFromRes(attr);

 	attr = pCreateElem->GetAttr(L"col_gap");
 	if (attr) col_gap = _ttoi(attr);

	attr = pCreateElem->GetAttr(L"row_gap");
	if (attr) row_gap = _ttoi(attr);

	attr = pCreateElem->GetAttr(L"col_count");
	if (attr) col_count = _ttoi(attr);

	attr = pCreateElem->GetAttr(L"key_rect");
	if (attr) WceUiConvertRectStrToRect(attr, &rc);

	RECT rcClient;
	GetWindowRect(&rcClient);
	::OffsetRect(&rc, rcClient.left, rcClient.top);

 	SetKeyRect(&rc, col_gap, row_gap, col_count);

}

void CKeypadLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	if (m_nClickItem >= 0 && m_texSelect.GetBitmap()!=NULL)
	{
		BLENDFUNCTION bf;
		memset(&bf, 0, sizeof(BLENDFUNCTION));
		bf.AlphaFormat = AC_SRC_ALPHA;

		pWndBitmap->AlphaBlend(m_rc[m_nClickItem].left, m_rc[m_nClickItem].top, 
			tzutil::_W(&m_rc[m_nClickItem]), tzutil::_H(&m_rc[m_nClickItem]),
			m_texSelect.GetBitmap(), 0, 0, bf);
	}
}

int CKeypadLayer::HitTest(POINT pt)
{
	for (int i=0; i<KEY_COUNT; i++)
	{
		if (PtInRect(&m_rc[i], pt))
		{
			return i;
		}
	}

	return -1;
}

void CKeypadLayer::OnLButtonDown(UINT nFlags, POINT point)
{
	LPCTSTR key_char = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	SetCapture();	
	m_bMouseDown = TRUE;
	m_nClickItem = HitTest(point);
	if (m_nClickItem >= 0 && m_nClickItem < (int)_tcslen(key_char))
	{
		ui_mcu_beep();
		GetWindow()->PostMessage(WM_CHAR, (WPARAM)key_char[m_nClickItem], 0);
		GetWindow()->PostMessage(UI_MSG_BT_PB_QUERY_CHANGE, 0, 0);
	}

	InvalidateRect(NULL);
	GetWindow()->UpdateWindow();

	__super::OnLButtonDown(nFlags, point);
}

void CKeypadLayer::OnLButtonUp(UINT nFlags,	 POINT point)
{
	if (m_bMouseDown)
	{
		ReleaseCapture();
		m_bMouseDown = FALSE;
		InvalidateRect(NULL);
		m_nClickItem = -1;
	}

	__super::OnLButtonUp(nFlags, point);
}

void CKeypadLayer::SetKeyRect(RECT *pRect, int nColGap, int nRowGap, int nColCount)
{
	if (!pRect || nColCount > KEY_COUNT || nColCount <= 0)
	{
		return;
	}

	for (int row=0; row<(KEY_COUNT+nColCount-1)/nColCount; row++)
	{
		for (int col=0; col<nColCount; col++)
		{
			if (row*nColCount + col >= KEY_COUNT)	// 最后一个已经计算了
			{
				break;
			}

			int dx = ((pRect->right-pRect->left) + nColGap) * col;
			int dy = ((pRect->bottom-pRect->top) + nRowGap) * row;
			m_rc[row*nColCount + col] = *pRect;
			::OffsetRect(&m_rc[row*nColCount + col], dx, dy);
		}
	}
}
