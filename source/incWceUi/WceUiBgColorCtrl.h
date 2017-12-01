// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiLoadColor.h"

class CWceUiBgColorCtrl
{
public:
	CWceUiBgColorCtrl();
	~CWceUiBgColorCtrl();
	CWceUiBgColorCtrl (const CWceUiBgColorCtrl& rhs);
	CWceUiBgColorCtrl& operator=(const CWceUiBgColorCtrl& rhs);

	//check order
	//1.call IsHasBgColor for check there is background color or not
	//2.if there is background color (IsHasBgColor return TRUE), 
	//  check the background color is transparent or not
	//3.if the background color is not transparent (IsTransparent return FALSE),
	//  call GetColor to get the background color

	//bHasBgColor is TRUE for there is background color
	//bHasBgColor is FALSE for indicating there is no background color
	void SetHasBgColor(BOOL bHasBgColor);
	BOOL IsHasBgColor() const;

	void SetTransparent(BOOL bTransparent);
	//call this two API, you must make sure IsHasBgColor return TRUE
	BOOL IsTransparent() const;

	//call this API, you must make sure
	//IsHasBgColor return TRUE, and IsTransparent return FALSE
	COLORREF GetColor();

	void SetColorFromApp(COLORREF crColor);
	void SetColorFromRes(LPCTSTR  pColorID);

private:
	void	DoCopy(const CWceUiBgColorCtrl& rhs);

	BOOL	m_bHasBgColor;	//have background color or not
	BOOL	m_bTransparent; //have background color, transparent or not
	CWceUiLoadColor m_LoadColor;
};


