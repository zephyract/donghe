// FjMessageBox.cpp: implementation of the CMsgBoxDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgBoxDlg.h"
#include "WceUiButton.h"
#include "interface.h"
#include "protocol.h"
#include "WceUiLayerCtrl.h"
#include "MainWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgBoxDlg::CMsgBoxDlg()
{
	m_nCancelResult = IDNO;

	m_strText[0] = '\0';
	m_pYes = NULL;
	m_pNo = NULL;
	m_pOK = NULL;
	m_pMsg = NULL;
	m_pInfoIcon = NULL;
	m_pQuestionIcon = NULL;
	m_pWarningIcon = NULL;
	m_pErrorIcon = NULL;
}

CMsgBoxDlg::~CMsgBoxDlg()
{

}


int CMsgBoxDlg::MessageBox(LPCTSTR lpszText, BOOL bTextResID, UINT nType /* = MB_OK */)
{
	CMsgBoxDlg *pDlg = (CMsgBoxDlg *)CMainWnd::GetInstance()->GetLayerCtrl()->CreateDlg(_T("CMsgBoxDlg"));
	int ret = -1;
	if (pDlg)
	{
		if(lpszText)
		{
			_tcsncpy(pDlg->m_strText, lpszText, 255);
		}

		pDlg->m_bTextResID = bTextResID;
		pDlg->m_nType = nType;
		ret = (int)pDlg->DoModal();
		CMainWnd::GetInstance()->GetLayerCtrl()->DeleteLayer(pDlg);
	}

	return ret;
}

void CMsgBoxDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_MCU_KEY_COMMAND);

	//step1 得到控件
	m_pYes = (CWceUiButton*)GetChildByName(L"yes");
	ASSERT(NULL != m_pYes);
	ASSERT(m_pYes->IsKindOf(WCEUI_CLASS_NAME(CWceUiButton)));

	m_pNo = (CWceUiButton*)GetChildByName(L"no");
	ASSERT(NULL != m_pNo);
	ASSERT(m_pNo->IsKindOf(WCEUI_CLASS_NAME(CWceUiButton)));

	m_pOK = (CWceUiButton*)GetChildByName(L"ok");
	ASSERT(NULL != m_pOK);
	ASSERT(m_pOK->IsKindOf(WCEUI_CLASS_NAME(CWceUiButton)));

	m_pMsg = GetChildByName(L"msg");
	ASSERT(NULL != m_pMsg);
	
	m_pInfoIcon = GetChildByName(L"info_icon");
	ASSERT(NULL != m_pInfoIcon);

	m_pQuestionIcon = GetChildByName(L"question_icon");
	ASSERT(NULL != m_pQuestionIcon);

	m_pWarningIcon = GetChildByName(L"warning_icon");
	ASSERT(NULL != m_pWarningIcon);

	m_pErrorIcon = GetChildByName(L"error_icon");
	ASSERT(NULL != m_pErrorIcon);



	//开始设置messagebox的参数

	//
	//step2: 根据用户的需求来设置MessageBox
	//设置ICON,可以不设置ICON
	//
	m_pInfoIcon->ShowLayer(FALSE);
	m_pQuestionIcon->ShowLayer(FALSE);
	m_pWarningIcon->ShowLayer(FALSE);
	m_pErrorIcon->ShowLayer(FALSE);

	// icon暂时不显示
// 	
// 	if ((m_nType & 0x000000F0L) == MB_ICONEXCLAMATION)
// 	{
// 		m_pWarningIcon->ShowLayer(TRUE);
// 	}else if ((m_nType & 0x000000F0L) == MB_ICONINFORMATION)
// 	{
// 		m_pInfoIcon->ShowLayer(TRUE);
// 	}else if ((m_nType & 0x000000F0L) == MB_ICONQUESTION)
// 	{
// 		m_pQuestionIcon->ShowLayer(TRUE);
// 	}else if ((m_nType & 0x000000F0L) == MB_ICONSTOP)
// 	{
// 		m_pErrorIcon->ShowLayer(TRUE);
// 	}

	//
	//设置text
	//
	if (m_bTextResID)
	{
		m_pMsg->SetTextResID(m_strText);
	}else
	{
		m_pMsg->SetText(m_strText);
	}

	//
	//设置按钮的显示
	//button1是ok, 2,3是yes no或者ok cancel
	//
	
	m_pOK->ShowLayer(TRUE);
	m_pYes->ShowLayer(FALSE);
	m_pNo->ShowLayer(FALSE);
	
	if(m_nType & MB_YESNO)
	{
		m_pOK->ShowLayer(FALSE);
		m_pYes->ShowLayer(TRUE);
		m_pNo->ShowLayer(TRUE);
	}

	__super::OnInitDialog();
}

void CMsgBoxDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"yes"))
	{
		EndDialog(IDYES);
	}
	else if (pButton->IsEqualName(L"no"))
	{
		EndDialog(IDNO);
	}
	else if (pButton->IsEqualName(L"ok"))
	{
		EndDialog(IDYES);
	}
}



//////////////////////////////////////////////////////////////////////////////////////
//                                 CPopupDlg
/////////////////////////////////////////////////////////////////////////////////////

CPopupDlg::CPopupDlg()
{
	m_nCancelResult = 0;
}

CPopupDlg::~CPopupDlg()
{

}

void CPopupDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_DLG_CHANGE);

	__super::OnInitDialog();
}


LRESULT CPopupDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_DLG_CHANGE)
	{
		EndDialog(m_nCancelResult);
	}

	return __super::OnMessage(message, wParam, lParam);
}