#include "stdafx.h"
#include "BTDialEditBox.h"
#include "WceUiPageLayer.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "interface.h"
#include "tzUtil.h"


CBTDialEditBox::CBTDialEditBox(void)
{
	m_bReadOnly = TRUE;
}

CBTDialEditBox::~CBTDialEditBox(void)
{

}
void CBTDialEditBox::OnDrawText(CWceUiGenericBitmap* pWndBitmap, BOOL bLayerBitmap)
//void CBTDialEditBox::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
//	CWceUiLayer::OnDraw(pWndBitmap, rcNeedDraw);

	CRect rcDstShow;

	CRect rcDrawText;
	GetStyle()->GetDrawTextCtrl()->GetRect(&rcDrawText);

	CRect rcClient;
	GetClientRect(&rcClient);

	if (rcDstShow.IntersectRect(&rcDrawText, &rcClient))
	{
		ClientToWindow(&rcDstShow);

		HFONT hfont = GetStyle()->GetFontCtrl()->GetFont();
		ASSERT(NULL != hfont);
		ASSERT (IsSingleLineEditBox());
		rcDstShow.left -= GetScrollPos();
		CString strFormated;
		GetFormatedString(strFormated);

		LPBYTE pbb;
		HBITMAP hbmp = pWndBitmap->GetHBitmap(pbb);
		CDC dc;
		dc.CreateCompatibleDC(NULL);

		dc.SelectObject(hbmp);
		dc.SelectObject(hfont);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(GetStyle()->GetTextColor());
		dc.DrawText(strFormated, rcDstShow, DT_CENTER|DT_VCENTER);
	}
}

void CBTDialEditBox::GetFormatedString(CString& strFormated)
{
	strFormated = m_strData.c_str();

	ui_format_tel_number(strFormated.GetBuffer(strFormated.GetLength() + 8));
	strFormated.ReleaseBuffer();
}
