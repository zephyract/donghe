// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"


namespace wceui
{
	class CWceUiLoadFontData;
}

class CWceUiLoadFont
{
public:
	CWceUiLoadFont(void);
	CWceUiLoadFont(LPCTSTR pFontID);
	CWceUiLoadFont(HFONT   hFont);
	CWceUiLoadFont(const LOGFONT& logFont);
	CWceUiLoadFont (const CWceUiLoadFont &rhs);

	~CWceUiLoadFont(void);

	CWceUiLoadFont& operator=(const CWceUiLoadFont &rhs);
public:
	void SetFontFromRes(LPCTSTR pFontID);
	void SetFontFromHFONT(HFONT hFont);
	void SetFontFromLOGFONT(const LOGFONT& logFont);

	HFONT GetFont();
	void  Clean();		//clean to default font
private:
	void  SetDefaultFont();
	void  DoCopy(const CWceUiLoadFont& rhs);
	wceui::CWceUiLoadFontData* GetData() const;
	wceui::CWceUiLoadFontData* m_pData;
};
