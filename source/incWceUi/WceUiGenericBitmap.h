// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiObject.h"

class CWceUiGenericBitmap;
typedef CWceUiGenericBitmap* PCWceUiGenericBitmap;

CWceUiGenericBitmap* WceUiLoadBmpFromFile(LPCTSTR pFilePath, BOOL bPerPixelAlpha = FALSE, LPCTSTR pAlphaPath = NULL);
CWceUiGenericBitmap* WceUiCreateBitmapObject(int bmBitsPixel);

DWORD WceUiSetHSLOffsetHue32 (DWORD crColor32,  int nOffsetHue, LPINT lpOldHue = NULL, LPINT lpNewHue = NULL);
WORD  WceUiSetHSLOffsetHue565(WORD  crColor565, int nOffsetHue, LPINT lpOldHue = NULL, LPINT lpNewHue = NULL);


namespace wceui
{
	class CFhBmp;
	struct tFhBmpData;
}

enum E_WCEUI_BITMAP_ROTATE_ANGLE
{
	E_WCEUI_BITMAP_ROTATE_90 = 1,
	E_WCEUI_BITMAP_ROTATE_180,
	E_WCEUI_BITMAP_ROTATE_270,
};

class CWceUiGenericBitmap :	public CWceUiObject
{
	WCEUI_DYNAMIC_BEGIN(CWceUiGenericBitmap, CWceUiObject)
public:
	CWceUiGenericBitmap(void);
	virtual ~CWceUiGenericBitmap(void);

	//////////////////////////////////////////////////////////////////////////
	/////////////////////  interface:   virtual functions   //////////////////
	//////////////////////////////////////////////////////////////////////////
	virtual BOOL CreateDIB(int nWidth, int nHeight) = 0;
	virtual BOOL SaveFile(LPCTSTR pFilePath) = 0;
	virtual BOOL LoadFromHBitmap(HBITMAP hBitmap) = 0;

	virtual BOOL MakeACopy(OUT CWceUiGenericBitmap** ppCopyBmp) = 0;

	virtual BOOL SetPerPixelAlpha(BOOL bPerPixelAlpha) = 0;

	virtual BOOL AlphaBlend(int x, int y, int nWidth, int nHeight, 
		CWceUiGenericBitmap* pSrcBmp, int xSrc, int ySrc, BLENDFUNCTION blendFunction);
	virtual BOOL MixColor(int x, int y, int nWidth, int nHeight, 
		COLORREF crColor, int nAlpha) = 0;

	virtual BOOL Rotate(E_WCEUI_BITMAP_ROTATE_ANGLE eAngle, OUT CWceUiGenericBitmap** ppRotatedBmp) = 0;

	//////////////////////////////////////////////////////////////////////////
	/////////////////////              interfaces             ////////////////
	//////////////////////////////////////////////////////////////////////////
	HBITMAP	GetHBitmap(OUT LPBYTE& pBmpBits);

	BOOL IsLocked() const;
	BOOL Lock();
	void UnLock();

	void Clean();

	BOOL IsLoaded() const;
	BOOL HasPerPixelAlpha() const;

	BOOL BitBlt(int x, int y, int nWidth, int nHeight, 
		const CWceUiGenericBitmap* pSrcBmp, int xSrc, int ySrc, DWORD dwRop);
	BOOL StretchBlt( int x, int y, int nWidth, int nHeight, 
		const CWceUiGenericBitmap* pSrcBmp, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);
	BOOL TransBlt(int x, int y, int nWidth, int nHeight, 
		const CWceUiGenericBitmap* pSrcBmp, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,COLORREF crTransparent);

	BOOL BitBlt(int x, int y, int nWidth, int nHeight, 
		HBITMAP hSrcBmp, int xSrc, int ySrc, DWORD dwRop);
	BOOL StretchBlt( int x, int y, int nWidth, int nHeight, 
		HBITMAP hSrcBmp, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);
	BOOL TransBlt(int x, int y, int nWidth, int nHeight, 
		HBITMAP hSrcBmp, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,COLORREF crTransparent);



	BOOL BltToDc(HDC dcDst, int x, int y, int nWidth, int nHeight, int xSrc, int ySrc, DWORD dwRop);
	BOOL StretchToDc(HDC dcDst, int x, int y, int nWidth, int nHeight,
		int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);
	BOOL TransToDc(HDC dcDst, int x, int y, int nWidth, int nHeight, 
		int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,COLORREF crTransparent);

	BOOL FillColor(COLORREF crFill,const RECT *lprc = NULL);

	const BITMAP& GetBitMapInfo() const;
	
	int GetWidth() const;
	int GetHeight() const;
	int GetBitsPixel() const;
	int GetTotalSize() const;
	int GetRowLength() const;

	//ConstantAlpha
	BOOL SetConstantAlpha(BOOL bConstantAlpha, int nAlpha = 0);
	BOOL GetConstantAlpha(int& nAlpha);

	//Transparent Color
	BOOL SetTransColor(BOOL bHasTransColor, COLORREF crTrans = 0);
	BOOL GetTransColor(COLORREF& crTrans) const;

	//offset of hue
	BOOL SetHSLOffsetHue(int nOffsetHue);
	
	//gray
	BOOL SetGray();

protected:
	wceui::tFhBmpData* GetData() const;
	wceui::CFhBmp*	   m_pFhBmp;
	COLORREF		   m_crTrans;
	BOOL			   m_bPerPixelAlpha;
	int				   m_nConstantAlpha;
private:
	CWceUiGenericBitmap (const CWceUiGenericBitmap &);
	CWceUiGenericBitmap & operator=(const CWceUiGenericBitmap &);
};
WCEUI_DYNAMIC_END(CWceUiGenericBitmap, CWceUiObject)
