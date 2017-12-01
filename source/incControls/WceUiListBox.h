#pragma once
#include "WceUiScrollLayer.h"
#include "WceUiLoadBitmap.h"
#include <string>
#include <vector>
using namespace std;

struct WCEUI_LISTBOX_ITEM_DATA 
{
	WCEUI_LISTBOX_ITEM_DATA(wstring strItem=L"", DWORD dwItemData=0, 
		DWORD dwState=0, CWceUiGenericBitmap *pItemImage=NULL)
		: m_strItem(strItem)
		, m_dwItemData(dwItemData)
		, m_dwState(dwState)
		, m_pItemImage(pItemImage)
	{

	}
	wstring m_strItem;
	DWORD m_dwItemData;
	DWORD m_dwState;
	CWceUiGenericBitmap *m_pItemImage;
};

class CWceUiListBox : public CWceUiScrollLayer
{
	WCEUI_DYNCREATE_BEGIN(CWceUiListBox, CWceUiScrollLayer);
public:
	CWceUiListBox(void);
	virtual ~CWceUiListBox(void);

	// interface
	void SetItemHeight(UINT cyItemHeight);
	int GetItemHeight() const;
	int GetCount( ) const;


	// The zero-based index to the string in the list box. 
	// The return value is LB_ERR if an error occurs
	int AddString( LPCTSTR lpszItem , BOOL bRedraw=TRUE);

	// A count of the strings remaining in the list. The return value is LB_ERR 
	// if nIndex specifies an index greater than the number of items in the list.
	int DeleteString( UINT nIndex , BOOL bRedraw=TRUE);


	// The zero-based index of the position at which the string was inserted. 
	// The return value is LB_ERR if an error occurs.
	// nIndex -- Specifies the zero-based index of the position to insert the string. 
	//           If this parameter is -1, the string is added to the end of the list.
	int InsertString( int nIndex, LPCTSTR lpszItem ,BOOL bRedraw=TRUE);

	// Removes all items from a list box.
	void ResetContent( );

	BOOL SetItemState( int nIndex, DWORD dwState, BOOL bRedraw=TRUE);
	BOOL GetItemState(int nIndex, OUT DWORD *pdwState) const;
	// set an image for an item.
	BOOL SetItemImage(int nIndex, CWceUiGenericBitmap* pItemImage);
	BOOL GetItemImage(int nIndex, CWceUiGenericBitmap** ppItemImage) const;

	// Zero if successful, or LB_ERR if an error occurs.
	// The system scrolls the list box until either the item specified by nIndex 
	// appears at the top of the list box or the maximum scroll range has been reached.
	int SetTopIndex( int nIndex, BOOL bRedraw=TRUE);

	// The zero-based index of the first visible item in a list box if successful, LB_ERR otherwise
	int GetTopIndex( ) const ;

	// The 32-bit value associated with the item, or LB_ERR if an error occurs
	DWORD GetItemData( int nIndex ) const;
	// LB_ERR if an error occurs.
	int SetItemData( int nIndex, DWORD dwItemData );

	// Return value:
	// The length (in bytes) of the string, excluding the terminating null character. 
	// If nIndex does not specify a valid index, the return value is LB_ERR.
	int GetText( int nIndex, LPTSTR lpszBuffer ) const;
	void GetText( int nIndex, wstring& rString ) const;

	int SetText(int nIndex, LPCTSTR lpszText);

	// Return Value
	// The length of the string in bytes, excluding the terminating null character.
	// If nIndex does not specify a valid index, the return value is LB_ERR.
	int GetTextLen( int nIndex ) const;

	int GetCurSel( ) const ;
	// Return LB_ERR if an error occurs.
	int SetCurSel(int nIndex, BOOL bRedraw=TRUE);

	void ShowScrollBar(BOOL bShow){ m_bShowScrollBar = bShow; }

	BOOL IsItemVisible(int nIndex);

	int GetTextLeftOffset();
	void SetTextLeftOffset(int nOffset);
	BOOL GetDrawItemImage();
	void SetDrawItemImage(BOOL bEnable);

// wceui overload
	virtual void OnInitLayer();
	virtual void OnDraw(CWceUiGenericBitmap* pDrawBitmap, RECT rcNeedDraw);

	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnLButtonDown(UINT nFlags,  POINT point);
	virtual void OnLButtonUp(UINT nFlags,	 POINT point);
	virtual void OnMouseMove(UINT nFlags,	 POINT point);
	virtual void OnTimer(WORD wIDEvent);

	// CWceUIScrollLayer overload
	virtual void OnScrollWindowEnd();

protected:
// helper functions
	virtual void DrawScrollButton(CWceUiGenericBitmap* pDrawBitmap, const LPRECT lprc);
	// 
	void DrawItemBK(CWceUiGenericBitmap* pDrawBitmap, int nIndex, const LPRECT lprcItem, const LPRECT lprcRegion);

	void DrawItemImage(CWceUiGenericBitmap* pDrawBitmap, int nIndex, const LPRECT lprcItem, const LPRECT lprcRegion);
	void DrawItemCheckIcon(CWceUiGenericBitmap* pDrawBitmap, int nIndex, const LPRECT lprcItem, const LPRECT lprcRegion);
	void DrawItemText(HDC hdc, int nIndex, const LPRECT lprcItem);
	void RefreshScrollRange(BOOL bRedraw=TRUE);
	// return the number of need_redraw items.
	// the first need_redraw item's index returned by parameter.
	int GetVisibleItems(OUT int *pnIndexStart, IN LPRECT lprcClent, IN LPRECT lprcPaint);

protected:
	UINT m_cyItemHeight;
	vector<WCEUI_LISTBOX_ITEM_DATA> m_data;

	// line background
	CWceUiLoadBitmap m_bmpLineBgNormal;
	CWceUiLoadBitmap m_bmpLineBgSelect;
	CWceUiLoadBitmap m_bmpLineBgCheck;
	CWceUiLoadBitmap m_bmpLineBgCheckSelect;
	// scroll bar
	CWceUiLoadBitmap m_bmpSBTop;
	CWceUiLoadBitmap m_bmpSBMiddle;
	CWceUiLoadBitmap m_bmpSBBottom;

	// 三组scrollbar用来实现渐变消失效果
	CWceUiLoadBitmap m_abmpSBTop[3];
	CWceUiLoadBitmap m_abmpSBMiddle[3];
	CWceUiLoadBitmap m_abmpSBBottom[3];

	// text color when clicking it
	COLORREF	m_crTextColorSelect;

	// text color when item selected(item's state is WCEUI_LBIS_SELECTED)
	COLORREF	m_crTextColorCheck;
	// 点击选中项时的文字颜色
	COLORREF	m_crTextColorCheckSelect;

	// draw an image when item selected.
	CWceUiLoadBitmap m_bmpItemCheck;

	// which item is clicked. 
	// when mouse down, set this value to indicate which item is clicked, 
	// if no one clicked or mouse up, set it to -1.
	int m_nItemClicked;

	// when click one item, we say this item is selected.
	// it's the selected item's index.
	// it will be changed until another item is clicked.
	int m_nItemSelected;

	// item default image, we can specify different image for items.
	CWceUiLoadBitmap m_bmpDefaultItemImage;
	// if need use item image, set this value to true.(configure it in dlg.xml file)
	BOOL m_bDrawItemImage;
	int  m_nItemImageWidth;	// item image的最大宽度，最大高度即为item的高度

	// left offset when draw text, it takes affects only if set "draw_item_image=false"
	// set default value to 5
	int m_nTextLeftOffset;

	// set it to FALSE if need not draw the scroll bar.
	// it will draw it in default.
	BOOL m_bShowScrollBar;

	// show scroll bar only when we are dragging listbox, auto hide it when dragging ends.
	BOOL m_bAutoHideScrollBar;
	void AutoHideScrollBar(BOOL bHide);
};

WCEUI_DYNCREATE_END(CWceUiListBox, CWceUiScrollLayer);
