#include "stdafx.h"
#include "NaviWarningDlg.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "sysUtil.h"
#include "WceUiLoadString.h"
#include "MainWnd.h"
#include "interface.h"

CNaviWarningDlg::CNaviWarningDlg(void)
: m_LastDlgSourceID(INVALID_SOURCE_ID)
, m_LastDlgID(INVALID_DLG_ID)
{
}

CNaviWarningDlg::~CNaviWarningDlg(void)
{

}

void CNaviWarningDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_NAVI_WARNGINDLG_EXIT);

	CWceUiLayer* pLayer = GetChildByName(L"msg");
	CWceUiLoadString loadstring;
	CString str;
	for (int i=1; i<=8; i++)
	{
		CString id;
		id.Format(L"MIS_NAVI_WARNING_MSG%d", i);
		loadstring.SetTextResID(id);
		str += loadstring.GetString();
		str += L" ";
	}

	if (pLayer)
	{
		pLayer->SetText(str);
	}

	__super::OnInitDialog();
}


void CNaviWarningDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"agree"))
	{
		if (CMainWnd::GetInstance()->LoadNavigation(FALSE))
		{
			Sleep(6000);
			CMainWnd::GetInstance()->OnGpsForeground(TRUE);
		}
		CDlgManager::GetInstance()->ShowDlg(m_LastDlgID, m_LastDlgSourceID);
		return;

		// 如果加载失败,马上关闭该窗口,如果加载成功，延时关闭
		if (!CMainWnd::GetInstance()->LoadNavigation(FALSE))
		{
			CDlgManager::GetInstance()->ShowDlg(m_LastDlgID, m_LastDlgSourceID);
		}
		else
		{
			SetTimer(TIMER_ID_GOTO_LAST_DLG, 3000);
		}
	}
}

void CNaviWarningDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_GOTO_LAST_DLG)
	{
		KillTimer(TIMER_ID_GOTO_LAST_DLG);
		CDlgManager::GetInstance()->ShowDlg(m_LastDlgID, m_LastDlgSourceID);
	}

	__super::OnTimer(wIDEvent);
}

void CNaviWarningDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		DLG_INFO *pdi = CDlgManager::GetInstance()->GetLastDlgInfo();
		if (pdi && (pdi->dlg_id!=CNaviWarningDlg_ID || pdi->source_id!=SOURCE_NAVI_WARNING))
		{
			m_LastDlgID = pdi->dlg_id;
			m_LastDlgSourceID = pdi->source_id;
		}
	}

	__super::OnDlgShow(bShow);
}

LRESULT CNaviWarningDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_NAVI_WARNGINDLG_EXIT)
	{
		CDlgManager::GetInstance()->ShowDlg(m_LastDlgID, m_LastDlgSourceID);
		return 0L;
	}
	else
	{
		return __super::OnMessage(message, wParam, lParam);
	}
}