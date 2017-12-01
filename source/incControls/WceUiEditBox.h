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


	// ��ǰ������ڵ�������(���ı�����)
	int m_nCol;
	int m_nRow;
	// ��ǰ������ڵ�λ��(���������λ��)
	UINT m_nCaretX;
	UINT m_nCaretY;
	// �и�
	UINT m_nRowHeight;

	// ��ǰ������������ı�λ�ã������ڶ����У�������
	void CalcTextPos(POINT &pt);
	// �����ļ�λ��(�У���)�����������ʵλ��
	void RefreshCaretPos();
	// ˢ�²��ֵ����ݣ��磺���ж����У� ÿ�е��ַ����ݵ�
	void RefreshLayoutData();
	// �õ�ĳ�����һ�е�ֵ��������һ���ַ�Ϊ'\n',�򷵻���ǰһ�ַ���λ��
	// ��û��ǰһ�ַ�������0
	int GetLineLastCol(int nLine);
	// ���ݵ�ǰ�����ݣ�������ɹ����ķ�Χ
	void RefreshScrollRange();
	int m_nTotalLines;
	CArray<LINES_INFO, LINES_INFO&> m_LinesInfo;

	// ���ؿɻ��е�λ������
	int CheckWordWrap(wstring &strData, int nStartPos, int nEndPos);
	// ���شӿ�ʼ�����н��������ж��ٸ��ַ�
	int GetLinesCharCounts(int nLine);


	BOOL m_bIsOnFocus;
	LPCTSTR m_lpszFocusStyle;
	LPCTSTR m_lpszStyle;

	// ��ȡ������ַ��ľ����������ݴ˼��㻻�е�λ�ã����ڵ��еģ����Է���һ�����޿��
	// ���Σ���������ʱ�Ͳ��ỻ���ˡ�����Ƕ��У�ֱ�ӷ��ؿؼ��Ĵ�С�Ϳ�����
	void GetLayoutRect(LPRECT prc);
	BOOL IsSingleLineEditBox();

	// �����곬���˿ؼ�������Ӧ���ǲ���ʾ�ģ�����������������ȷ��ʾ
	BOOL m_bIsCaretInControl;
	BOOL m_bIsCaretVisible;
	void MyShowCaret();
	// auto scroll the editbox so as to make caret visible.
	void AutoScroll();

	// if the control is readonly, no need to show caret.
	BOOL m_bReadOnly;


};
WCEUI_DYNCREATE_END(CWceUiEditBox, CWceUiScrollLayer);
