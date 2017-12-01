// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiLoadColor.h"

class CWceUiAlphaFilterCtrl
{
public:
	CWceUiAlphaFilterCtrl();
	~CWceUiAlphaFilterCtrl();
	CWceUiAlphaFilterCtrl (const CWceUiAlphaFilterCtrl &rhs);
	CWceUiAlphaFilterCtrl& operator=(const CWceUiAlphaFilterCtrl &rhs);

	void SetFilter(BOOL bEnable, int nAlpha);
	BOOL GetFilter(int& nAlpha) const;
	BOOL IsFilterEnabled() const;

private:
	void DoCopy(const CWceUiAlphaFilterCtrl &rhs);
	int		m_nAlpha;
	BOOL	m_bEnabled;
};

class CWceUiMixColorFilterCtrl
{
public:
	CWceUiMixColorFilterCtrl();
	~CWceUiMixColorFilterCtrl();
	CWceUiMixColorFilterCtrl (const CWceUiMixColorFilterCtrl &rhs);
	CWceUiMixColorFilterCtrl& operator=(const CWceUiMixColorFilterCtrl &rhs);

	void SetFilterFromApp(BOOL bEnable, COLORREF crMix, int nAlpha);
	void SetFilterFromRes(BOOL bEnable, LPCTSTR  pMixColor, int nAlpha);
	BOOL GetFilter(COLORREF& crMix, int& nAlpha) const;
	BOOL IsFilterEnabled() const;

private:
	void	DoCopy(const CWceUiMixColorFilterCtrl &rhs);
	CWceUiLoadColor m_LoadColor;
	int		 m_nAlpha;
	BOOL	 m_bEnabled;
};