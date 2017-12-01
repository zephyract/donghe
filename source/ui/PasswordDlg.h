#pragma once
#include "BaseDlg.h"
#include "WceUiSEditBox.h"
#include "msgboxdlg.h"

// DoModal����1ʱ���������Ч�� �������0��˵���û�ȡ���˲���(�簴��MENU����)
class CPasswordDlg : public CPopupDlg  
{
	WCEUI_DYNCREATE_BEGIN(CPasswordDlg, CPopupDlg)
public:
	CPasswordDlg();
	virtual ~CPasswordDlg();

	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnInitDialog();

	// interface
	static LPCTSTR GetPassword(int nMaxPasswordLen = 4);
protected:
	BOOL OnBnKeypadClick(CWceUiButton* pButton);
	void OnKeyEnter();

	LPCTSTR GetPasswordData();
	void SetMaxPasswordLen(int nLen);
private:
	TCHAR			 m_strText[256];
	BOOL			 m_bTextResID;
	UINT			 m_nType;
};
WCEUI_DYNCREATE_END(CPasswordDlg, CPopupDlg)

//////////////////////////////////////////////////////////

class CPincodeEditBox : public CWceUiSEditBox
{
	WCEUI_DYNCREATE_BEGIN(CPincodeEditBox, CWceUiSEditBox)
public:
	CPincodeEditBox();
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	void SetTotal(int nTotal);
protected:
	// ��ʾʱ,�����ݸ�ʽ����, (* * _ _), ͬʱ��������Ƿ�Ϸ�,������Ϸ�����Ϸ���,���ݸ������ܴ���m_nTotal
	void FormatData(OUT LPTSTR buffer);
	UINT m_nTotal;	// ���볤��
};
WCEUI_DYNCREATE_END(CPincodeEditBox, CWceUiSEditBox);