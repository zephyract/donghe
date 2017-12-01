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
	class CWceUiLoadColorData;
}

class CWceUiLoadColor
{
public:
	CWceUiLoadColor(void);
	CWceUiLoadColor(LPCTSTR pColorID);
	CWceUiLoadColor(COLORREF crColor);
	CWceUiLoadColor (const CWceUiLoadColor &rhs);
	
	~CWceUiLoadColor(void);

	CWceUiLoadColor& operator=(const CWceUiLoadColor &rhs);
public:
	void SetColorFromApp(COLORREF crColor);
	void SetColorFromRes(LPCTSTR  pColorID);
	void Clean();

	COLORREF GetColor() const;
private:
	void  DoCopy(const CWceUiLoadColor& rhs);
	wceui::CWceUiLoadColorData* GetData() const;
	wceui::CWceUiLoadColorData* m_pData;
};



