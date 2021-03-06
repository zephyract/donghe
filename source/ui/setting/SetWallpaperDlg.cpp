#include "stdafx.h"
#include "SetWallpaperDlg.h"
#include "tzUtil.h"
#include "wceUiPageLayer.h"
#include "sysUtil.h"
#include "interface.h"
#include "WceUi32Bitmap.h"

CSetWallpaperDlg::CSetWallpaperDlg(void)
{
	for (int i=0; i<WALLPAPER_COUNT; i++)
	{
		if (WceUiGetBitsPixel() == 16)
		{
			m_pBkTexture[i] = new CWceUi565Bitmap();
		}
		else
		{
			m_pBkTexture[i] = new CWceUi32Bitmap();
		}
	}
}

CSetWallpaperDlg::~CSetWallpaperDlg(void)
{
	for (int i=0; i<WALLPAPER_COUNT; i++)
	{
		if (m_pBkTexture[i] != NULL)
		{
			delete m_pBkTexture[i];
			m_pBkTexture[i] = NULL;
		}
	}
}

void CSetWallpaperDlg::OnInitDialog()
{
	__super::OnInitDialog();

	TCHAR szFilepath[MAX_PATH] = L"";
	TCHAR szLayerName[32];
	CWallPaperButton* pbutton = NULL;
	for (int i=0; i<WALLPAPER_COUNT; i++)
	{
		if (WceUiGetScreenWidth() == 1024)
		{
			_stprintf(szFilepath, L"%s\\BKSkin_1024X600.pack\\s%d.bmp", tzutil::GetAppPath(), i);
		}
		else
		{
			_stprintf(szFilepath, L"%s\\BKSkin.pack\\s%d.bmp", tzutil::GetAppPath(), i);
		}
		if (WceUiGetBitsPixel() == 16)
		{
			((CWceUi565Bitmap*)m_pBkTexture[i])->LoadFromFile(szFilepath);
		}
		else
		{
			((CWceUi32Bitmap*)m_pBkTexture[i])->LoadFromFile(szFilepath);
		}

		_stprintf(szLayerName, L"bk%d", i);
		pbutton = (CWallPaperButton*)GetLayerByName(szLayerName);
		if (pbutton)
		{
			pbutton->SetBackGround(m_pBkTexture[i]);
		}
	}
	CheckButton(sysutil::nss_get_instance()->ui_bk_index);

	CWceUiPageLayer* ppage = (CWceUiPageLayer*)GetChildByName(L"frame");
	if (ppage)
	{
		ppage->SetExtraScroll(30);
		ppage->SetScrollRange(0, WceUiGetScreenWidth() * 2);
	}
}

void CSetWallpaperDlg::OnReset()
{
	CheckButton(sysutil::nss_get_instance()->ui_bk_index);
}

void CSetWallpaperDlg::CheckButton(int nIndex)
{
	TCHAR buffer[32];
	CWallPaperButton* pbutton = NULL;
	for (int i=0; i<WALLPAPER_COUNT; i++)
	{
		_stprintf(buffer, L"bk%d", i);
		pbutton = (CWallPaperButton*)GetLayerByName(buffer);
		if (pbutton)
		{
			if (i == nIndex)
			{
				_stprintf(buffer, L"bk%d_down", i);
			}
			else
			{
				_stprintf(buffer, L"bk%d_normal", i);
			}
			pbutton->SetStatusStyleName(buffer, CWceUiButton::STATUS_NORMAL);
		}
	}
}

void CSetWallpaperDlg::OnBnClick(CWceUiButton* pButton)
{
	TCHAR layer_name[][16] = {L"bk0", L"bk1", L"bk2", L"bk3", L"bk4", L"bk5", L"bk6", L"bk7", L"bk8" };
	int count = sizeof(layer_name)/sizeof(layer_name[0]);
	int i = 0;
	for (; i<count; i++)
	{
		if (pButton->IsEqualName(layer_name[i]))
		{
			break;
		}
	}

	if (i<count)	// found it
	{
		::PostMessage(HWND_BROADCAST, NAVI_MSG_BK_CHANGE, 0, (LPARAM)i);
		CheckButton(i);
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}


//******************** CWallPaperButton  ***********************//

CWallPaperButton::CWallPaperButton(void)
{
	m_pBkTexture = NULL;
}

CWallPaperButton::~CWallPaperButton(void)
{

}

void CWallPaperButton::SetBackGround(CWceUiGenericBitmap* pTexture)
{
	m_pBkTexture = pTexture;
}

void CWallPaperButton::OnDrawBackGround(CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap)
{
	RECT rc;
	GetWindowRect(&rc);
	if (m_pBkTexture)
	{
		pDrawBitmap->BitBlt(rc.left, rc.top, m_pBkTexture->GetWidth(), m_pBkTexture->GetHeight(), 
			m_pBkTexture, 0, 0, SRCCOPY);
//		wceui::wceglDrawTexture(m_pBkTexture, &rc, NULL, GetZCoord());
	}
	else	// 如果没指定，直接用黑背景
	{
//		wceui::wceglFillSolidRect(&rc, RGB(0,0,0), 255, GetZCoord());
	}
}