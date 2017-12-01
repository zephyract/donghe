// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiBgItem.h"
#include "WceUiBgColorCtrl.h"
#include "WceUiFontCtrl.h"
#include "WceUiDrawTextCtrl.h"
#include "WceUiFilterCtrl.h"

enum WCEUI_POSITION_WAY
{
	WCEUI_ABSOLUTE_POSITION,
	WCEUI_RELATIVE_POSITION,
};

namespace wceui
{
	class CWceUiStyleData;
}

class CWceUiLayer;
class CWceUiStyle
{
	friend class CWceUiLayer;
public:
	CWceUiStyle();
	virtual ~CWceUiStyle();
	CWceUiStyle (const CWceUiStyle &rhs);
	CWceUiStyle& operator=(const CWceUiStyle &rhs);

	void SetPosition(LPCRECT lpRect, WCEUI_POSITION_WAY posWay);
	WCEUI_POSITION_WAY GetPosition(LPRECT lpRect) const;

	void Show(BOOL bShow);
	BOOL IsVisible() const;

	void Enable(BOOL bEnable);
	BOOL IsEnabled() const;

	void SetContainer(BOOL bContainer);
	BOOL IsContainer() const;

	void SetTextColorFromRes(LPCTSTR pColorID);
	void SetTextColorFromApp(COLORREF crText);
	COLORREF GetTextColor() const;

	void SetTextResID(LPCTSTR pStrResID);
	LPCTSTR GetTextResID() const;

	void SetNoDraw(BOOL bNoDraw);
	BOOL IsNoDraw() const;

	void SetNoInput(BOOL bNoInput);
	BOOL IsNoInput() const;

	CWceUiFontCtrl*				GetFontCtrl();
	CWceUiDrawTextCtrl*			GetDrawTextCtrl();
	CWceUiBgColorCtrl*			GetBgColorCtrl();
	CWceUiBgItemCtrl*			GetBgItemCtrl();
	CWceUiAlphaFilterCtrl*		GetAlphaFilterCtrl();
	CWceUiMixColorFilterCtrl*	GetMixColorFilterCtrl();

	void CopyFromTwoStyle(const CWceUiStyle& commonStyle, const CWceUiStyle& style);

	void SetUseAll();
	void ClearUseAll();

	void SetUsePosition(BOOL bUsePosition);
	BOOL IsUsePosition() const;

	void SetUseShow(BOOL bUseShow);
	BOOL IsUseShow() const;

	void SetUseEnable(BOOL bUseEnable);
	BOOL IsUseEnable() const;

	void SetUseContainer(BOOL bUseContainer);
	BOOL IsUseContainer() const;

	void SetUseTextColor(BOOL bUseTextColor);
	BOOL IsUseTextColor() const;

	void SetUseTextResID(BOOL bUseTextResID);
	BOOL IsUseTextResID() const;

	void SetUseNoDraw(BOOL bUseNoDraw);
	BOOL IsUseNoDraw() const;

	void SetUseNoInput(BOOL bUseNoInput);
	BOOL IsUseNoInput() const;

	void SetUseDrawBelow(BOOL bUseDrawBelow);
	BOOL IsUseDrawBelow() const;

	void SetUseFontCtrl(BOOL bUseFontCtrl);
	BOOL IsUseFontCtrl() const;

	void SetUseDrawTextCtrl(BOOL bUseDrawTextCtrl);
	BOOL IsUseDrawTextCtrl() const;

	void SetUseBgColorCtrl(BOOL bUseBgColorCtrl);
	BOOL IsUseBgColorCtrl() const; 

	void SetUseBgItemCtrl(BOOL bUseBgItemCtrl);
	BOOL IsUseBgItemCtrl() const;

	void SetUseAlphaFilterCtrl(BOOL bUseAlphaFilterCtrl);
	BOOL IsUseAlphaFilterCtrl() const;

	void SetUseMixColorFilterCtrl(BOOL bUseMixColorFilterCtrl);
	BOOL IsUseMixColorFilterCtrl() const;

private:
	void SetLayer(CWceUiLayer* pLayer);

private:
	void CopyStylePos	 (const CWceUiStyle& commonStyle, const CWceUiStyle& style);
	void CopyStyleLayout (const CWceUiStyle& commonStyle, const CWceUiStyle& style);
	void CopyStyleText	 (const CWceUiStyle& commonStyle, const CWceUiStyle& style);
	void CopyStyleBg	 (const CWceUiStyle& commonStyle, const CWceUiStyle& style);
	void CopyStyleFilter (const CWceUiStyle& commonStyle, const CWceUiStyle& style);
	void CopyStyleAdvProp(const CWceUiStyle& commonStyle, const CWceUiStyle& style);

private:
	void	DoCopyAllData(const CWceUiStyle &rhs);
	void	DoCopyStyleData(const CWceUiStyle &rhs);
	void	DoCopyUseData(const CWceUiStyle &rhs);
private:
	wceui::CWceUiStyleData* GetData() const;
	wceui::CWceUiStyleData* m_pData;
};