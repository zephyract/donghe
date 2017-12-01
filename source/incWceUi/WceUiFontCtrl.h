// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiLoadFont.h"

class CWceUiFontCtrl
{
public:
	CWceUiFontCtrl();
	~CWceUiFontCtrl();
	CWceUiFontCtrl (const CWceUiFontCtrl &rhs);
	CWceUiFontCtrl& operator=(const CWceUiFontCtrl &rhs);

	void SetFontFromRes(LPCTSTR pFontID);
	void SetFontFromHFONT(HFONT hFont);
	void SetFontFromLOGFONT(const LOGFONT& logFont);

	//never return NULL
	HFONT GetFont();

	//clean to default font
	void	CleanFont();
private:
	void	DoCopy(const CWceUiFontCtrl &rhs);
	CWceUiLoadFont m_LoadFont;
};
