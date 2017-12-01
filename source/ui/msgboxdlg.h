#pragma once
#include "BaseDlg.h"

//
// ����domodal��������ʱ,�ŴӸ���̳�
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

	// ��Ϊ�����л�������ԭ�򵼺�Popup�������˳�ʱ,EndDialog()���ص�ֵ
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

