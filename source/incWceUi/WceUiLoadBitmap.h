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
	class CWceUiLoadBitmapData;
}

class CWceUiLoadBitmap
{
public:
	CWceUiLoadBitmap(void);
	CWceUiLoadBitmap(LPCTSTR pBitmapID);
	CWceUiLoadBitmap(CWceUiGenericBitmap* pBitmap);
	CWceUiLoadBitmap (const CWceUiLoadBitmap &rhs);
	~CWceUiLoadBitmap(void);

	CWceUiLoadBitmap& operator=(const CWceUiLoadBitmap &rhs);

	void	SetBitmapFromApp(CWceUiGenericBitmap* pBitmap);
	void	SetBitmapFromRes(LPCTSTR pBitmapID);

	//if it returns NULL, these is no bitmap
	CWceUiGenericBitmap* GetBitmap();

	void	Clean();

	void	FreeBmpRes();
private:
	void	DoCopy(const CWceUiLoadBitmap& rhs);
	wceui::CWceUiLoadBitmapData* GetData() const;
	wceui::CWceUiLoadBitmapData* m_pData;
};



