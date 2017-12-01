#include "stdafx.h"
#include "PasswordDlg.h"
#include "WceUiButton.h"
#include "interface.h"
#include "protocol.h"
#include "WceUiEditBox.h"
#include "MainWnd.h"
#include "WceUiLayerCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPasswordDlg::CPasswordDlg()
{

}

CPasswordDlg::~CPasswordDlg()
{

}

void CPasswordDlg::OnInitDialog()
{
	CWceUiEditBox* pEditbox = (CWceUiEditBox*)GetLayerByName(L"keypad_editbox");
	if (pEditbox)
	{
		pEditbox->SetReadOnly(TRUE);
		pEditbox->OnFocus();
	}

	__super::OnInitDialog();
}

void CPasswordDlg::OnBnClick(CWceUiButton* pButton)
{
	if (OnBnKeypadClick(pButton))
	{
		return;
	}
	else if (pButton->IsEqualName(L"key_enter"))
	{
		OnKeyEnter();
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}

void CPasswordDlg::OnKeyEnter()
{
	EndDialog(1);
}


BOOL CPasswordDlg::OnBnKeypadClick(CWceUiButton* pButton)
{
	const int KEY_COUNT = 11;
	LPCTSTR key_layers[] = {L"key_0", L"key_1", L"key_2", L"key_3", L"key_4", L"key_5", L"key_6", 
		L"key_7", L"key_8", L"key_9", L"key_backspace"};
	TCHAR key_char[] = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'\b'};


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
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

void CPasswordDlg::SetMaxPasswordLen(int nLen)
{
	CPincodeEditBox* pEditbox = (CPincodeEditBox*)GetLayerByName(L"keypad_editbox");
	if ( pEditbox && nLen>0)
	{
		return pEditbox->SetTotal(nLen);
	}
}

LPCTSTR CPasswordDlg::GetPassword(int nMaxPasswordLen)
{
	LPCTSTR result = NULL;
	CPasswordDlg *pDlg = (CPasswordDlg *)CMainWnd::GetInstance()->GetLayerCtrl()->CreateDlg(_T("CPasswordDlg"));
	if (pDlg)
	{
		pDlg->SetMaxPasswordLen(nMaxPasswordLen);
		if( pDlg->DoModal() == 1)	// 用户确认了输入
		{
			result = pDlg->GetPasswordData();
		}
		CMainWnd::GetInstance()->GetLayerCtrl()->DeleteLayer(pDlg);
	}

	return result;
}

LPCTSTR CPasswordDlg::GetPasswordData()
{
	CWceUiEditBox* pEditbox = (CWceUiEditBox*)GetLayerByName(L"keypad_editbox");
	if ( pEditbox)
	{
		return pEditbox->GetData();
	}

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////////////
//                               CPincodeEditBox                                     //
///////////////////////////////////////////////////////////////////////////////////////

CPincodeEditBox::CPincodeEditBox()
{
	m_nTotal = 4;
}

void CPincodeEditBox::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	TCHAR buffer[16] = L"";
	FormatData(buffer);
	CString strOrginal = m_strData.c_str();

	// 用格式化的数据输出,输出完成后，恢复原始数据
	m_strData = buffer;
	__super::OnDraw(pWndBitmap, rcNeedDraw);
	m_strData = strOrginal;

}

void CPincodeEditBox::FormatData(OUT LPTSTR buffer)
{
	if (m_strData.length() > m_nTotal)	// 表示输入的数值不合法,调整该值
	{
		OnChar(L'\b', 1, 0);	// 删除最后的输入
	}

	// 显示内容
	if (buffer)
	{
		for (UINT i=0; i<m_nTotal; i++)
		{
			if (i<m_strData.length())
			{
				buffer[i*2] = L'*';
			}
			else
			{
				buffer[i*2] = L'_';
			}
			buffer[i*2+1] = L' ';
		}
		buffer[m_nTotal*2] = L'\0';
	}
}

void CPincodeEditBox::SetTotal(int nTotal)
{
	if (nTotal>=0)
	{
		m_nTotal = nTotal;
	}
}

