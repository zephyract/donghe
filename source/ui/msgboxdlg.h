#pragma once
#include "BaseDlg.h"

//
// 调用domodal弹出窗口时,才从该类继承
//
class CPopupDlg : public CBaseDlg  
{
	WCEUI_DYNCREATE_BEGIN(CPopupDlg, CBaseDlg)
public:
	CPopupDlg();
	virtual ~CPopupDlg();

	virtual void OnInitDialog();
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

protected:

	// 因为界面切换或其它原因导航Popup窗口需退出时,EndDialog()返回的值
	int m_nCancelResult;
};
WCEUI_DYNCREATE_END(CPopupDlg, CBaseDlg)



class CMsgBoxDlg : public CPopupDlg  
{
	WCEUI_DYNCREATE_BEGIN(CMsgBoxDlg, CPopupDlg)
public:
	CMsgBoxDlg();
	virtual ~CMsgBoxDlg();

	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnInitDialog();
public:
	static int MessageBox(LPCTSTR lpszText, BOOL bTextResID, UINT nType = MB_OK);

private:
	CWceUiButton* m_pYes;
	CWceUiButton* m_pNo;
	CWceUiButton* m_pOK;
	CWceUiLayer*  m_pMsg;

	CWceUiLayer*  m_pInfoIcon;
	CWceUiLayer*  m_pQuestionIcon;
	CWceUiLayer*  m_pWarningIcon;
	CWceUiLayer*  m_pErrorIcon;

private:
	TCHAR			 m_strText[256];
	BOOL			 m_bTextResID;
	UINT			 m_nType;
};
WCEUI_DYNCREATE_END(CMsgBoxDlg, CPopupDlg)

