// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "wceuigenericbitmap.h"

class CWceUi32Bitmap :	public CWceUiGenericBitmap
{
	WCEUI_DYNCREATE_BEGIN(CWceUi32Bitmap, CWceUiGenericBitmap)
public:
	CWceUi32Bitmap(void);
	CWceUi32Bitmap (const CWceUi32Bitmap &bmpSrc);
	CWceUi32Bitmap (HBITMAP hBitmap);
	virtual ~CWceUi32Bitmap(void);

	const CWceUi32Bitmap& operator=(const CWceUi32Bitmap &bmpSrc);

	//////////////////////////////////////////////////////////////////////////
	////////////////////////     virtual functions    ////////////////////////
	//////////////////////////////////////////////////////////////////////////
	virtual BOOL CreateDIB(int nWidth, int nHeight);
	virtual BOOL MakeACopy(OUT CWceUiGenericBitmap** ppCopyBmp);
	virtual BOOL SaveFile(LPCTSTR pFilePath);
	virtual BOOL LoadFromHBitmap(HBITMAP hBitmap);
	virtual BOOL AlphaBlend(int x, int y, int nWidth, int nHeight, 
		CWceUiGenericBitmap* pSrcBmp, int xSrc, int ySrc, BLENDFUNCTION blendFunction);
	virtual BOOL SetPerPixelAlpha(BOOL bPerPixelAlpha);
	virtual BOOL MixColor(int x, int y, int nWidth, int nHeight, COLORREF crColor, int nAlpha);
	virtual BOOL Rotate(E_WCEUI_BITMAP_ROTATE_ANGLE eAngle, OUT CWceUiGenericBitmap** ppRotatedBmp);


	//////////////////////////////////////////////////////////////////////////
	///////////////////////////////      interfaces    ///////////////////////
	//////////////////////////////////////////////////////////////////////////
	BOOL	LoadFromFile(LPCTSTR pFilePath, BOOL bPerPixelAlpha = FALSE);
	BOOL	CreateDIB32(int nWidth, int nHeight, const LPVOID lpvBits, BOOL bPerPixelAlpha);
	BOOL	PreMulAlpha();
};
WCEUI_DYNCREATE_END(CWceUi32Bitmap, CWceUiGenericBitmap)
