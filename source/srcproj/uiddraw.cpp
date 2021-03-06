#include "stdafx.h"
#include "uiddraw.h"
#include "tzUtil.h"
#include "WceUi32Bitmap.h"
#include "SourceManager.h"

CUiDDraw::CUiDDraw()
{
	m_lpDD = NULL;
	m_lpDDSPrimary = NULL;
	m_lpDDSOverlay = NULL;
	m_lpDDSOverlayRear = NULL;
	m_lpDDSVideo = NULL;

	m_bIsOverlayVisible = FALSE;
	m_bIsVolumeOverlayVisible = FALSE;
	m_bIsRearOverlayVisible = FALSE;
}

CUiDDraw::~CUiDDraw()
{

}


CUiDDraw* CUiDDraw::GetInstance()
{
	static CUiDDraw uiddaw;
	return &uiddaw;
}

BOOL CUiDDraw::InitDDraw(HWND hwnd)
{
	static DDPIXELFORMAT ddpfOverlayFormats[] = {		
		{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xF800, 0x07e0, 0x001F, 0},        // 16-bit RGB 5:6:5	
		{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 32,  0xFF0000, 0x00FF00, 0x0000FF, 0xFF000000},        // 32-bit RGB 8:8:8
	};

	m_hWnd = hwnd;

	RECT rc = {0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
	HDC hdc = NULL;

	BOOL bRet = FALSE;

	// create DirectDraw object
	HRESULT ddrval = DirectDrawCreate(NULL, &m_lpDD, NULL); 
	if(ddrval != DD_OK) 
	{ 
		TRACE(L"Craete DDraw fail.\n");
		goto LExit;
	}

	// 
	ddrval = m_lpDD->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);
	if(FAILED(ddrval)) 
	{ 
		TRACE(L"SetCooperativeLevel() fail.\n");
		goto LExit;
	}


	// create primary surface
	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd); 

	ddsd.dwFlags = DDSD_CAPS; 
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE; 
	ddrval = m_lpDD->CreateSurface(&ddsd, &m_lpDDSPrimary, NULL);
	if(FAILED(ddrval)) 
	{ 
		TRACE(L"Create primary surface fail.\n");
		goto LExit;
	}

	// create overlay surface
	DDSURFACEDESC ddsdOverlay;
	ZeroMemory(&ddsdOverlay, sizeof(ddsdOverlay));
	ddsdOverlay.dwSize = sizeof(ddsdOverlay); 

	ddsdOverlay.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	ddsdOverlay.ddsCaps.dwCaps = DDSCAPS_OVERLAY; 
	ddsdOverlay.dwWidth = GetSystemMetrics(SM_CXSCREEN);
	ddsdOverlay.dwHeight = GetSystemMetrics(SM_CYSCREEN);
	ddsdOverlay.ddpfPixelFormat.dwSize = sizeof(ddsd.ddpfPixelFormat);
	ddsdOverlay.ddpfPixelFormat =  (WceUiGetBitsPixel() == 16) ? ddpfOverlayFormats[0] : ddpfOverlayFormats[1];

	ddrval = m_lpDD->CreateSurface(&ddsdOverlay, &m_lpDDSOverlay, NULL);	
	if(FAILED(ddrval)) 
	{ 
		TRACE(L"Create overlay surface fail.\n");
		goto LExit;
	}

	// fill overlay surface with black color
	m_lpDDSOverlay->GetDC(&hdc);
	::FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	m_lpDDSOverlay->ReleaseDC(hdc);

	// create overlay surface for rear
	/* -- shut off rear UI
	ddrval = m_lpDD->CreateSurface(&ddsdOverlay, &m_lpDDSOverlayRear, NULL);	
	if(FAILED(ddrval)) 
	{ 
		TRACE(L"Create  rear overlay surface fail.\n");
		goto LExit;
	}
	*/

	// create overlay surface for volume
	ddrval = m_lpDD->CreateSurface(&ddsdOverlay, &m_lpDDSOverlayVolume, NULL);	
	if(FAILED(ddrval)) 
	{ 
		TRACE(L"Create  volume overlay surface fail.\n");
		goto LExit;
	}
	// fill overlay surface with key color
	FillOverlayWithColor(m_lpDDSOverlayVolume, GetKeyColor());

	bRet = TRUE;
LExit:

	if (!bRet)
	{
		if (m_lpDD != NULL)
		{
			m_lpDD->Release();
			m_lpDD = NULL;
		}
		if (m_lpDDSPrimary != NULL)
		{
			m_lpDDSPrimary->Release();
			m_lpDDSPrimary = NULL;
		}
		if (m_lpDDSOverlay != NULL)
		{
			m_lpDDSOverlay->Release();
			m_lpDDSOverlay = NULL;
		}
	}

	return bRet;
}


void CUiDDraw::DeInitDDRaw()
{
	if (m_lpDD != NULL)
	{
		m_lpDD->Release();
		m_lpDD = NULL;
	}
	if (m_lpDDSPrimary != NULL)
	{
		m_lpDDSPrimary->Release();
		m_lpDDSPrimary = NULL;
	}
	if (m_lpDDSOverlay != NULL)
	{
		m_lpDDSOverlay->Release();
		m_lpDDSOverlay = NULL;
	}
}


DWORD CUiDDraw::GetKeyColor()
{
	//return (WceUiGetBitsPixel() == 16) ? 0x0000 : 0xFF000000;	// 32位系统时,opengl渲染出来的数据alpha通道是有值的，所以设置关键色时把该值也带上
	return (WceUiGetBitsPixel() == 16) ? 0x0000 : 0x000000;
}

void CUiDDraw::FillOverlayWithColor(LPDIRECTDRAWSURFACE lpDDSOverlay, COLORREF cr)
{
	DDSURFACEDESC  ddsd = {0};
	ddsd.dwSize = sizeof(ddsd);
	lpDDSOverlay->Lock(0, &ddsd, DDLOCK_WAITNOTBUSY, NULL);
	for (int i = 0;i<ddsd.dwWidth;i++)
	{
		for (int j = 0;j<ddsd.dwHeight;j++)
		{
			if (WceUiGetBitsPixel() == 16)
			{
				((WORD *)ddsd.lpSurface)[ddsd.lPitch/2*j+i] = (WORD)cr;
			}
			else
			{
				((UINT32 *)ddsd.lpSurface)[ddsd.lPitch/4*j+i] = cr;
			}
		}
	}
	lpDDSOverlay->Unlock(NULL);
}

BOOL CUiDDraw::UpdateVolumeOverlay(BOOL bVisible)
{
	// 二个BOOL值相等时不用处理
	if(tzutil::IsBoolEqual(m_bIsVolumeOverlayVisible, bVisible))
	{
		return TRUE;
	}
	m_bIsVolumeOverlayVisible = bVisible;

	RECT rc = {0, 0, WceUiGetScreenWidth(), WceUiGetScreenHeight() };

	// update overlay
	DWORD dwUpdateFlags = bVisible ? DDOVER_SHOW : DDOVER_HIDE;
	dwUpdateFlags = dwUpdateFlags | DDOVER_KEYSRCOVERRIDE | DDOVER_ALPHACONSTOVERRIDE ;
	DDOVERLAYFX ovfx;
	memset(&ovfx, 0, sizeof(ovfx));
	ovfx.dwSize = sizeof(ovfx);
	ovfx.dckSrcColorkey.dwColorSpaceLowValue= GetKeyColor();
	ovfx.dckSrcColorkey.dwColorSpaceHighValue= GetKeyColor();

	ovfx.dwAlphaConstBitDepth = 8;
	ovfx.dwAlphaConst = 160;

	HRESULT ddrval = m_lpDDSOverlayVolume->UpdateOverlay(&rc, m_lpDDSPrimary, &rc, dwUpdateFlags, &ovfx);

	if(FAILED(ddrval)) 
	{ 
		TRACE(L"Update overlay fail.\n");
		return FALSE;
	}

	if (bVisible)
	{
		ddrval = m_lpDDSOverlayVolume->UpdateOverlayZOrder(DDOVERZ_SENDTOFRONT, NULL);
		if(FAILED(ddrval)) 
		{ 
			TRACE(L"UpdateOverlayZOrder fail.\n");
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CUiDDraw::UpdateOverlay(BOOL bVisible, DWORD dwAlphaConst)
{
	if (bVisible && !m_bIsOverlayVisible)	// 只要overlay从隐藏状态到显示状态，就先把屏幕上的数据同步上去
	{
		FillOverlayWithColor(m_lpDDSOverlay, GetKeyColor());
	}

	m_dwAlphaConst = dwAlphaConst;
	RECT rc = {0, 0, WceUiGetScreenWidth(), WceUiGetScreenHeight() };
	RECT rc_rear = {0, 0, 720, 480};

	// update overlay
	DWORD dwUpdateFlags = bVisible ? DDOVER_SHOW : DDOVER_HIDE;
	if (tzutil::IsBoolEqual(m_bIsOverlayVisible, bVisible) && bVisible)		// 如果只是修改透明度，不带DDOVER_SHOW参数
	{
		dwUpdateFlags = 0;
	}
	m_bIsOverlayVisible = bVisible;


	dwUpdateFlags = dwUpdateFlags | DDOVER_KEYSRCOVERRIDE | DDOVER_ALPHACONSTOVERRIDE ;
	DDOVERLAYFX ovfx;
	memset(&ovfx, 0, sizeof(ovfx));
	ovfx.dwSize = sizeof(ovfx);
	ovfx.dckSrcColorkey.dwColorSpaceLowValue= GetKeyColor();
	ovfx.dckSrcColorkey.dwColorSpaceHighValue= GetKeyColor();

	ovfx.dwAlphaConstBitDepth = 8;
	ovfx.dwAlphaConst = dwAlphaConst;

	HRESULT ddrval = m_lpDDSOverlay->UpdateOverlay(&rc, m_lpDDSPrimary, &rc, dwUpdateFlags, &ovfx);

	if(FAILED(ddrval)) 
	{ 
		TRACE(L"Update overlay fail.\n");
		return FALSE;
	}

	if (bVisible)
	{
		ddrval = m_lpDDSOverlay->UpdateOverlayZOrder(DDOVERZ_SENDTOBACK, NULL);
		if(FAILED(ddrval)) 
		{ 
			TRACE(L"UpdateOverlayZOrder fail.\n");
			return FALSE;
		}
	}

	return TRUE;
}

#define DDOVER_SHOWATREAR				        0x00200000
#define DISPLAY_SET_REAR_RECT					0x0002000c
BOOL CUiDDraw::UpdateRearOverlay(BOOL bVisible)
{
	RECT rc = {0, 0, WceUiGetScreenWidth(), WceUiGetScreenHeight() };
	RECT rc_rear = {0, 0, 720, 480};

	DWORD dwUpdateFlags = bVisible ? DDOVER_SHOW : DDOVER_HIDE;
	if (tzutil::IsBoolEqual(m_bIsRearOverlayVisible, bVisible) && bVisible)		// 如果只是修改透明度，不带DDOVER_SHOW参数
	{
		dwUpdateFlags = 0;
	}
	m_bIsRearOverlayVisible = bVisible;

	// update overlay
	dwUpdateFlags = dwUpdateFlags | DDOVER_KEYSRCOVERRIDE | DDOVER_ALPHACONSTOVERRIDE | DDOVER_SHOWATREAR;
	DDOVERLAYFX ovfx;
	memset(&ovfx, 0, sizeof(ovfx));
	ovfx.dwSize = sizeof(ovfx);
	ovfx.dckSrcColorkey.dwColorSpaceLowValue = GetKeyColor();
	ovfx.dckSrcColorkey.dwColorSpaceHighValue = GetKeyColor();
	ovfx.dwAlphaConstBitDepth = 8;
	ovfx.dwAlphaConst = 255;

	HDC hdcPrimary = NULL;
	LPDIRECTDRAWSURFACE lpDDSPrimary = m_lpDDSPrimary;
	lpDDSPrimary->GetDC(&hdcPrimary);
	UINT u4Value = 1;
	if (CSourceManager::GetInstance()->GetRearSrcID() == SOURCE_DVD)
	{
		ExtEscape(hdcPrimary, DISPLAY_SET_REAR_RECT, sizeof(u4Value), (char *)&u4Value, 0, NULL);
	}

	HRESULT ddrval = -1;
	if (m_lpDDSOverlayRear)
	{
		ddrval = m_lpDDSOverlayRear->UpdateOverlay(&rc, m_lpDDSPrimary, &rc_rear, dwUpdateFlags, &ovfx);
	}

	u4Value = 0;
	if (CSourceManager::GetInstance()->GetRearSrcID() == SOURCE_DVD)
	{
		ExtEscape(hdcPrimary, DISPLAY_SET_REAR_RECT, sizeof(u4Value), (char *)&u4Value, 0, NULL);
	}
	lpDDSPrimary->ReleaseDC(hdcPrimary);

	if(FAILED(ddrval)) 
	{ 
		TRACE(L"Update overlay fail.\n");
		return FALSE;
	}

	return TRUE;
}

BOOL CUiDDraw::IsRearOverlayVisible()
{
	return m_bIsRearOverlayVisible;
}

void CUiDDraw::BitBltOverlay(HDC hdc, int x, int y, int width, int height, int xsrc, int ysrc)
{
	if (m_lpDDSOverlay)
	{
		HDC hdcOverlay = NULL;
		m_lpDDSOverlay->GetDC(&hdcOverlay);
		::BitBlt(hdcOverlay, x, y, width, height, hdc, xsrc, ysrc, SRCCOPY);
		m_lpDDSOverlay->ReleaseDC(hdcOverlay);
	}
}

void CUiDDraw::BitBltOverlay(HBITMAP hbmp, int x, int y, int width, int height, int xsrc, int ysrc)
{

}

void CUiDDraw::BitBltVolumeOverlay(HDC hdc, int x, int y, int width, int height, int xsrc, int ysrc)
{
	if (m_lpDDSOverlayVolume)
	{
		HDC hdcOverlay = NULL;
		m_lpDDSOverlayVolume->GetDC(&hdcOverlay);
		::BitBlt(hdcOverlay, x, y, width, height, hdc, xsrc, ysrc, SRCCOPY);
		m_lpDDSOverlayVolume->ReleaseDC(hdcOverlay);
	}
}

void CUiDDraw::BitBltVolumeOverlay(HBITMAP hbmp, int x, int y, int width, int height, int xsrc, int ysrc)
{

}


void CUiDDraw::BitBltRearOverlay(HDC hdc, int x, int y, int width, int height, int xsrc, int ysrc)
{
	if (m_lpDDSOverlayRear)
	{
		HDC hdcOverlay = NULL;
		m_lpDDSOverlayRear->GetDC(&hdcOverlay);
		::BitBlt(hdcOverlay, x, y, width, height, hdc, xsrc, ysrc, SRCCOPY);
		m_lpDDSOverlayRear->ReleaseDC(hdcOverlay);
	}
}

void CUiDDraw::BitBltRearOverlay(HBITMAP hbmp, int x, int y, int width, int height, int xsrc, int ysrc)
{

}

BOOL CUiDDraw::IsOverlayVisible()
{
	return m_bIsOverlayVisible;
}


BOOL CUiDDraw::IsVolumeOverlayVisible()
{
	return m_bIsVolumeOverlayVisible;
}
