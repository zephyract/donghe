// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "wceuigenericbitmap.h"

class CWceUi565Bitmap :	public CWceUiGenericBitmap
{
	WCEUI_DYNCREATE_BEGIN(CWceUi565Bitmap, CWceUiGenericBitmap)
public:
	CWceUi565Bitmap(void);
	CWceUi565Bitmap (const CWceUi565Bitmap &bmpSrc);
	CWceUi565Bitmap (HBITMAP hBitmap);
	virtual ~CWceUi565Bitmap(void);

	const CWceUi565Bitmap& operator=(const CWceUi565Bitmap &bmpSrc);

	//////////////////////////////////////////////////////////////////////////
	////////////////////////      virtual functions    ///////////////////////
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
	BOOL	LoadFromFile(LPCTSTR pFilePath, LPCTSTR pAlphaPath = NULL);
	BOOL	CreateDIB565(int nWidth, int nHeight, const void* lpvBits, const BYTE* lpAlphaData);
	//if there is no alpha info, pAlphaPath will be ignored
	BOOL	SaveFileAndAlpha(LPCTSTR pFilePath, LPCTSTR pAlphaPath);
	HBITMAP	GetBmpData(OUT LPBYTE& pBmpBits, OUT LPBYTE& pAlphaBits);

};
WCEUI_DYNCREATE_END(CWceUi565Bitmap, CWceUiGenericBitmap)