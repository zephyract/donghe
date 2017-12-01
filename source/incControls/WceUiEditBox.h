#pragma once
#include "WceUiLayer.h"
#include "WceUiScrollLayer.h"
#include "WceUiRealWnd.h"

const int LEFT_SCROLL_OFFSET = 50;

struct LINES_INFO
{
	int nPos;			// the position in the string data.
	int nCharCounts;	// how many chars included in this line.
};

// single line edit box control
class CWceUiEditBox : public CWceUiScrollLayer
{
	WCEUI_DYNCREATE_BEGIN(CWceUiEditBox, CWceUiScrollLayer);
public:
	CWceUiEditBox(void);
	virtual ~CWceUiEditBox(void);

	// wceui override
	virtual void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnLButtonUp(UINT nFlags,  POINT point);
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnInitLayer();
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnScrollWindow() { RefreshCaretPos(); }

	// interface 
	void OnKillFocus();
	void OnFocus();
	BOOL IsReadOnly();
	void SetReadOnly(BOOL bEnable);

	LPCTSTR GetData();
	void SetData(LPCTSTR strData);
	void SetRowHeight(int nRowHeight);
	int GetRowHeight();
	int GetTotalLines();
	BOOL GetLineText(int nLine, OUT LPTSTR strBuffer);

protected:

	wstring m_strData;


	// 当前光标所在的行与列(对文本而言)
	int m_nCol;
	int m_nRow;
	// 当前光标所在的位置(光标真正的位置)
	UINT m_nCaretX;
	UINT m_nCaretY;
	// 行高
	UINT m_nRowHeight;

	// 当前坐标点所处的文本位置，即处于多少行，多少列
	void CalcTextPos(POINT &pt);
	// 给定文件位置(行，列)计算出光标的真实位置
	void RefreshCaretPos();
	// 刷新布局的数据，如：共有多少行， 每行的字符数据等
	void RefreshLayoutData();
	// 得到某行最后一列的值，如果最后一个字符为'\n',则返回其前一字符的位置
	// 如没有前一字符，返回0
	int GetLineLastCol(int nLine);
	// 根据当前的数据，计算出可滚动的范围
	void RefreshScrollRange();
	int m_nTotalLines;
	CArray<LINES_INFO, LINES_INFO&> m_LinesInfo;

	// 返回可换行的位置索引
	int CheckWordWrap(wstring &strData, int nStartPos, int nEndPos);
	// 返回从开始到这行结束，共有多少个字符
	int GetLinesCharCounts(int nLine);


	BOOL m_bIsOnFocus;
	LPCTSTR m_lpszFocusStyle;
	LPCTSTR m_lpszStyle;

	// 获取可输出字符的矩形区，根据此计算换行的位置，对于单行的，可以返回一个无限宽的
	// 矩形，这样计算时就不会换行了。如果是多行，直接返回控件的大小就可以了
	void GetLayoutRect(LPRECT prc);
	BOOL IsSingleLineEditBox();

	// 如果光标超过了控件的区域，应该是不显示的，这个函数帮助光标正确显示
	BOOL m_bIsCaretInControl;
	BOOL m_bIsCaretVisible;
	void MyShowCaret();
	// auto scroll the editbox so as to make caret visible.
	void AutoScroll();

	// if the control is readonly, no need to show caret.
	BOOL m_bReadOnly;


};
WCEUI_DYNCREATE_END(CWceUiEditBox, CWceUiScrollLayer);
