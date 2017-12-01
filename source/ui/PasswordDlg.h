#pragma once
#include "BaseDlg.h"
#include "WceUiSEditBox.h"
#include "msgboxdlg.h"

// DoModal返回1时，输入才有效， 如果返回0，说明用户取消了操作(如按了MENU键等)
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
	// 显示时,将数据格式化下, (* * _ _), 同时检测数据是否合法,如果不合法将其合法化,数据个数不能大于m_nTotal
	void FormatData(OUT LPTSTR buffer);
	UINT m_nTotal;	// 密码长度
};
WCEUI_DYNCREATE_END(CPincodeEditBox, CWceUiSEditBox);