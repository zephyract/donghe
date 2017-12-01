// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"

class CWceUiDrawTextCtrl
{
public:
	CWceUiDrawTextCtrl();
	~CWceUiDrawTextCtrl();

	CWceUiDrawTextCtrl (const CWceUiDrawTextCtrl &rhs);
	CWceUiDrawTextCtrl& operator=(const CWceUiDrawTextCtrl &rhs);

	void SetRect(LPCRECT lpRect);
	void GetRect(LPRECT  lpRect) const;

	void SetFormat(UINT uFormat);
	UINT GetFormat() const;

private:
	void	DoCopy(const CWceUiDrawTextCtrl &rhs);
	RECT	m_rcText;
	UINT	m_uFormat;
};