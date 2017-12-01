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
	class CWceUiLoadStringData;
}

class CWceUiLoadString
{
public:
	CWceUiLoadString(void);
	CWceUiLoadString(LPCTSTR pTextResID);
	CWceUiLoadString(UINT    nTextResID);
	CWceUiLoadString (const CWceUiLoadString &rhs);
	~CWceUiLoadString(void);

	CWceUiLoadString& operator=(const CWceUiLoadString &rhs);

	LPCTSTR GetString();
	operator LPCTSTR();

	void SetTextResID(LPCTSTR pTextResID);
	void SetTextResID(UINT    nTextResID);

	LPCTSTR GetTextResID() const;

	void ReLoad();

public:NO_WRITE_POINTER(CWceUiLoadString, Next);

private:
	void DoCopy(const CWceUiLoadString& rhs);
	wceui::CWceUiLoadStringData* GetData() const;
	wceui::CWceUiLoadStringData* m_pData;
};
