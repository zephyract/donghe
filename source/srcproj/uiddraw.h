#pragma once
#include <ddraw.h>

class CUiDDraw
{
public:
	CUiDDraw();
	~CUiDDraw();
	static CUiDDraw* GetInstance();
	// DDraw 
	void DeInitDDRaw();
	BOOL InitDDraw(HWND hwnd);

	// overlay update
	BOOL UpdateOverlay(BOOL bVisible, DWORD dwAlphaConst);
	BOOL UpdateRearOverlay(BOOL bVisible);

	void BitBltOverlay(HDC hdc, int x, int y, int width, int height, int xsrc=0, int ysrc=0);
	void BitBltOverlay(HBITMAP hbmp, int x, int y, int width, int height, int xsrc=0, int ysrc=0);
	void FillOverlayWithColor(LPDIRECTDRAWSURFACE lpDDSOverlay, COLORREF cr);	// 注意如果需指定alpha值,参数必须指定
	BOOL IsOverlayVisible();

	void BitBltRearOverlay(HDC hdc, int x, int y, int width, int height, int xsrc=0, int ysrc=0);
	void BitBltRearOverlay(HBITMAP hbmp, int x, int y, int width, int height, int xsrc=0, int ysrc=0);
	BOOL IsRearOverlayVisible();

	void BitBltVolumeOverlay(HDC hdc, int x, int y, int width, int height, int xsrc=0, int ysrc=0);
	void BitBltVolumeOverlay(HBITMAP hbmp, int x, int y, int width, int height, int xsrc=0, int ysrc=0);
	BOOL UpdateVolumeOverlay(BOOL bVisible);
	BOOL IsVolumeOverlayVisible();

	DWORD GetKeyColor();

public:
	LPDIRECTDRAW m_lpDD;	// DirectDraw4 interface
	LPDIRECTDRAWSURFACE m_lpDDSPrimary;	// primary surface
	LPDIRECTDRAWSURFACE m_lpDDSOverlay;	// overlay surface
	LPDIRECTDRAWSURFACE m_lpDDSOverlayRear;	// overlay surface for rear
	LPDIRECTDRAWSURFACE m_lpDDSVideo;	// video memory surface

	LPDIRECTDRAWSURFACE m_lpDDSOverlayVolume;	// overlay surface for volume
protected:

	HWND m_hWnd;
	DWORD m_dwAlphaConst;
	BOOL m_bIsOverlayVisible;

	BOOL m_bIsVolumeOverlayVisible;
	BOOL m_bIsRearOverlayVisible;

};