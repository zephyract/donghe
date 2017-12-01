#pragma once
#include "WceUiMEditBox.h"
#include "WceUi565Bitmap.h"


class CLyricEditBox : public CWceUiMEditBox
{
	WCEUI_DYNCREATE_BEGIN(CLyricEditBox, CWceUiMEditBox);
public:
	CLyricEditBox();
	~CLyricEditBox();
	virtual void OnDrawText(CWceUiGenericBitmap* pWndBitmap, BOOL bLayerBitmap);
	virtual void OnInitLayer();

	void SetHightLightColor(COLORREF cr)
	{
		m_crHighLight = cr;
	}
	void SetHighLightLines(int min, int max)
	{
		ASSERT(min<=max);
		if (min != m_minHighlightLines || max!=m_maxHighlightLines)
		{
			m_minHighlightLines = min;
			m_maxHighlightLines = max;

			InvalidateRect();
			GetWindow()->UpdateWindow();
		}
	}
	void ResetLineIndex();
	// nLyricLine -- the index of lyric in lyric parser
	// return the corresponding lyric index in edit box, if the lyric was separated
	// into multi-line, it return the first one.
	// if the parameter is invalid, it returns -1
	int GetLineIndex(int nLyricLine);
	int LYRIC_EMPTY_LINES;
protected:
	int m_minHighlightLines;
	int m_maxHighlightLines;
	COLORREF m_crHighLight;
	// 每行歌词对应在editbox中的行，因为太宽有可能一行歌词显示时在editbox中占了几行
	// 数组的顺序即为歌词的顺序
	CArray<int, int> m_aLineIndex;


	int CLyricEditBox::GetVisibleItems(OUT int *pnIndexStart, IN CRect &rcClient, IN CRect& rcPaint);
};

WCEUI_DYNCREATE_END(CLyricEditBox, CWceUiMEditBox);
