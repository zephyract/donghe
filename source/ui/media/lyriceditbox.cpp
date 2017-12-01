#include "stdafx.h"
#include "LyricEditBox.h"
#include "MusicPlayerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CLyricEditBox::CLyricEditBox()
{
	SetReadOnly(TRUE);

	LYRIC_EMPTY_LINES = 2;   // 为显示好看，前面空4行	
	if (WceUiGetScreenWidth() == 480)
	{
		SetRowHeight(20);
	}
	else
	{
		SetRowHeight(35);
	}
	m_crHighLight = RGB(64, 128, 255);

	m_minHighlightLines = m_maxHighlightLines = 0;
}

CLyricEditBox::~CLyricEditBox()
{

}

void CLyricEditBox::OnInitLayer()
{
	CWceUiMEditBox::OnInitLayer();
}

void CLyricEditBox::OnDrawText(CWceUiGenericBitmap* pWndBitmap, BOOL bLayerBitmap)
{
	CRect rc_client;
	GetWindowRect(&rc_client);


	// copy some data from the snap bmp, and calculate the rcPaint that need 
	// to be redrawn.
	CRect rcPaint(rc_client);
	HFONT hfont = GetStyle()->GetFontCtrl()->GetFont();
	ASSERT(NULL != hfont);

	LPBYTE pbb;
	HBITMAP hbmp = pWndBitmap->GetHBitmap(pbb);
	CDC dc;
	dc.CreateCompatibleDC(NULL);

	dc.SelectObject(hbmp);
	dc.SelectObject(hfont);
	dc.SetBkMode(TRANSPARENT);
	CRgn rgn;
	rgn.CreateRectRgn(rc_client.left, rc_client.top, rc_client.right, rc_client.bottom);
	dc.SelectObject(rgn);


	CRect rc_items(rc_client);
	rc_items.top -= GetScrollPos();
	rc_items.bottom = rc_items.top + GetRowHeight()*GetTotalLines();

	// draw items
	int first_visible_items = 0;
	int total_visible_items = 0;

	total_visible_items = GetVisibleItems(&first_visible_items, rc_client, rcPaint);

	for (int i=first_visible_items; i<first_visible_items+total_visible_items; i++)
	{
		if (i>=m_minHighlightLines && i<=m_maxHighlightLines)
		{
			dc.SetTextColor(m_crHighLight);
		}
		else
		{
			dc.SetTextColor(GetStyle()->GetTextColor());
		}
		CRect rc_out(rc_items.left, rc_items.top+i*m_nRowHeight,
			rc_items.left+rc_items.Width(), rc_items.top+(i+1)*m_nRowHeight);
		//draw text
		wstring str = m_strData.substr(m_LinesInfo[i].nPos, m_LinesInfo[i].nCharCounts);
		dc.DrawText(str.c_str(), m_LinesInfo[i].nCharCounts, rc_out, DT_CENTER);
	}
	dc.DeleteDC();

}


void CLyricEditBox::ResetLineIndex()
{
	m_aLineIndex.RemoveAll();
	CString strLine;
	for (int i=LYRIC_EMPTY_LINES; i<m_nTotalLines; i++)
	{
		m_aLineIndex.Add(i);

		while (i<m_nTotalLines)
		{
			GetLineText(i, strLine.GetBuffer(255));
			strLine.ReleaseBuffer();
			if (strLine.Find(_T('\n')) != -1)	// found it, so process next line
			{
				break;
			}
			else
			{
				i++;
			}
		}
	
	}
}

int CLyricEditBox::GetLineIndex(int nLyricLine)
{
	if (nLyricLine>=0 && nLyricLine<m_aLineIndex.GetCount())
	{
		return m_aLineIndex.GetAt(nLyricLine);
	} 
	else
	{
		return -1;
	}
}


int CLyricEditBox::GetVisibleItems(OUT int *pnIndexStart, IN CRect &rcClient, IN CRect& rcPaint)
{
	CRect rc_items(rcClient);
	rc_items.top -= GetScrollPos();
	rc_items.bottom = rc_items.top + GetTotalLines()*GetRowHeight();
	CRect rc_visble;
	rc_visble.IntersectRect(rcPaint, rc_items);

	if (rc_visble.IsRectEmpty())
	{
		return 0;
	}

	int first, last;
	first = (rc_visble.top-rc_items.top) / GetRowHeight();
	last = (rc_visble.bottom-rc_items.top-1) / GetRowHeight();
	if (pnIndexStart)
		*pnIndexStart = first;

	return (last-first+1);
}
