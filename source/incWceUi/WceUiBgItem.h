// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiGenericBitmap.h"

namespace wceui
{
	class CBgItemData;
	class CWceUiBgItemCtrlData;
}

class CWceUiBgItem;
typedef CWceUiBgItem* PCWceUiBgItem;
class CWceUiBgItem
{
public:
	CWceUiBgItem();
	~CWceUiBgItem();
	CWceUiBgItem (const CWceUiBgItem& rhs);
	CWceUiBgItem& operator=(const CWceUiBgItem &rhs);

	void	SetBitmapFromApp(CWceUiGenericBitmap* pBitmap);
	void	SetBitmapFromRes(LPCTSTR pBitmapID);

	//if it returns NULL, these is no bitmap
	CWceUiGenericBitmap* GetBitmap();
	void	CleanBitmap();

	void	SetName(LPCTSTR pBgItemName);
	LPCTSTR GetName() const;

	//source rect use the coordinates that are relate relative to 
	//the upper-left corner of the bitmap
	void	GetSrcRect(LPRECT lpRect) const;
	void	SetSrcRect(LPCRECT lpRect);

	//destination rect use the coordinates that are relate relative to 
	//the upper-left corner of the layer
	void	GetDstRect(LPRECT lpRect) const;
	void	SetDstRect(LPCRECT lpRect);

	void	SetAlign(UINT uAlignWay);
	UINT	GetAlign() const;

	void	SetZoom(BOOL bZoom);
	BOOL    IsZoom() const;

	//only valid for common style
	//if nIndex >= 0, from begin to end
	//if nIndex < 0,  from end to begin.
	void	SetCommonStyleIndex(int nCommonStyleIndex);
	int		GetCommonStyleIndex() const;

	void	FreeBmpRes();

public:NO_WRITE_POINTER(CWceUiBgItem, Pre);
public:NO_WRITE_POINTER(CWceUiBgItem, Next);

private:
	void	Construct();
	void	DoCopy(const CWceUiBgItem &rhs);
	wceui::CBgItemData*	 GetData() const;
	wceui::CBgItemData*  m_pBgItemData;
};

class CWceUiBgItemCtrl
{
public:
	CWceUiBgItemCtrl();
	~CWceUiBgItemCtrl();
	CWceUiBgItemCtrl (const CWceUiBgItemCtrl& rhs);
	CWceUiBgItemCtrl& operator=(const CWceUiBgItemCtrl &rhs);

	int			  GetCount() const;

	PCWceUiBgItem GetFirst() const;
	PCWceUiBgItem GetNext(PCWceUiBgItem pCurBgItem) const;
	PCWceUiBgItem GetLast() const;
	PCWceUiBgItem GetPre(PCWceUiBgItem pCurBgItem)  const;

	PCWceUiBgItem GetByName(LPCTSTR pName) const;
	PCWceUiBgItem GetByIndex(int nIndex)   const;

	PCWceUiBgItem AddHead(const CWceUiBgItem& bgItem);
	PCWceUiBgItem AddTail(const CWceUiBgItem& bgItem);
	PCWceUiBgItem InsertBefore(PCWceUiBgItem pBeforeThis, const CWceUiBgItem& bgItemInsert);
	PCWceUiBgItem InsertAfter(PCWceUiBgItem  pAfterThis,  const CWceUiBgItem& bgItemInsert);

	BOOL Remove(PCWceUiBgItem  pDelBgItem);
	void RemoveAll();

private:
	void DoCopy(const CWceUiBgItemCtrl &rhs);
	wceui::CWceUiBgItemCtrlData* GetData() const;
	wceui::CWceUiBgItemCtrlData* m_pCtrlData;
};