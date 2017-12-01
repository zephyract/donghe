#include "StdAfx.h"
#include "wceuilayer.h"
#include "ClimatronicLayer.h"
#include "CanBus.h"
#include "interface.h"
#include "wceuirealwnd.h"

CClimatronicLayer_DasAuto::CClimatronicLayer_DasAuto(void)
{
}

CClimatronicLayer_DasAuto::~CClimatronicLayer_DasAuto(void)
{
}

void CClimatronicLayer_DasAuto::SetProperties(PCWceUiXmlElem pCreateElem)
{
	ASSERT(NULL != pCreateElem);
	CWceUiLayer::SetProperties(pCreateElem);
	LPCTSTR pHeadLbmp                  = pCreateElem->GetAttr(L"bmp_head_l");
	LPCTSTR pHandsLbmp                 = pCreateElem->GetAttr(L"bmp_hands_l");
	LPCTSTR pFeetLbmp                  = pCreateElem->GetAttr(L"bmp_feet_l");
	LPCTSTR pHeadRbmp                  = pCreateElem->GetAttr(L"bmp_head_r");
	LPCTSTR pHandsRbmp                 = pCreateElem->GetAttr(L"bmp_hands_r");
	LPCTSTR pFeetRbmp                  = pCreateElem->GetAttr(L"bmp_feet_r");
	LPCTSTR pFanspeedNormalbmp         = pCreateElem->GetAttr(L"bmp_fanspeed_normal");
	LPCTSTR pFanspeedDownbmp           = pCreateElem->GetAttr(L"bmp_fanspeed_down");
	LPCTSTR pInnerbmp                  = pCreateElem->GetAttr(L"bmp_inner");
	LPCTSTR pOuterbmp                  = pCreateElem->GetAttr(L"bmp_outer");
	LPCTSTR pFdemisterbmp              = pCreateElem->GetAttr(L"bmp_fdemister");
	LPCTSTR pRdemisterbmp              = pCreateElem->GetAttr(L"bmp_rdemister");
	LPCTSTR pUintbmp                   = pCreateElem->GetAttr(L"bmp_tempunit");
	LPCTSTR pLhotseatbmp0               = pCreateElem->GetAttr(L"bmp_lhotseat0");
	LPCTSTR pLhotseatbmp1               = pCreateElem->GetAttr(L"bmp_lhotseat1");
	LPCTSTR pLhotseatbmp2               = pCreateElem->GetAttr(L"bmp_lhotseat2");
	LPCTSTR pLhotseatbmp3               = pCreateElem->GetAttr(L"bmp_lhotseat3");
	LPCTSTR pRhotseatbmp0               = pCreateElem->GetAttr(L"bmp_rhotseat0");
	LPCTSTR pRhotseatbmp1               = pCreateElem->GetAttr(L"bmp_rhotseat1");
	LPCTSTR pRhotseatbmp2               = pCreateElem->GetAttr(L"bmp_rhotseat2");
	LPCTSTR pRhotseatbmp3               = pCreateElem->GetAttr(L"bmp_rhotseat3");
	LPCTSTR pFaniconLbmp                 = pCreateElem->GetAttr(L"bmp_faniconl");
	LPCTSTR pFaniconSbmp                 = pCreateElem->GetAttr(L"bmp_fanicons");
	
	

	if (NULL != pFaniconSbmp)
	{
		m_pbmp_fanicon[0].SetBitmapFromRes(pFaniconSbmp);
	}

	if (NULL != pFaniconLbmp)
	{
		m_pbmp_fanicon[1].SetBitmapFromRes(pFaniconLbmp);
	}

	if (NULL != pHandsLbmp)
	{
		m_pbmp_hands_l.SetBitmapFromRes(pHandsLbmp);
	}

	if (NULL != pFeetLbmp)
	{
		m_pbmp_feet_l.SetBitmapFromRes(pFeetLbmp);
	}

	if (NULL != pHandsRbmp)
	{
		m_pbmp_hands_r.SetBitmapFromRes(pHandsRbmp);
	}

	if (NULL != pFeetRbmp)
	{
		m_pbmp_feet_r.SetBitmapFromRes(pFeetRbmp);
	}

	if (NULL != pFanspeedNormalbmp)
	{
		m_pbmp_fanspeed_normal.SetBitmapFromRes(pFanspeedNormalbmp);
	}
	
	if (NULL != pFanspeedDownbmp)
	{
		m_pbmp_fanspeed_down.SetBitmapFromRes(pFanspeedDownbmp);
	}

	if (NULL != pInnerbmp)
	{
		m_pbmp_cycle[0].SetBitmapFromRes(pInnerbmp);
	}

	if (NULL != pOuterbmp)
	{
		m_pbmp_cycle[1].SetBitmapFromRes(pOuterbmp);
	}

	if (NULL != pFdemisterbmp)
	{
		m_pbmp_fdemister.SetBitmapFromRes(pFdemisterbmp);
		m_pbmp_head_l.SetBitmapFromApp(m_pbmp_fdemister.GetBitmap());
		m_pbmp_head_r.SetBitmapFromApp(m_pbmp_fdemister.GetBitmap());
	}

	if (NULL != pRdemisterbmp)
	{
        m_pbmp_rdemister.SetBitmapFromRes(pRdemisterbmp);
	}

	if (NULL != pUintbmp)
	{
		m_pbmp_unit.SetBitmapFromRes(pUintbmp);
	}

	if (NULL != pLhotseatbmp0 && NULL != pLhotseatbmp1 && NULL != pLhotseatbmp2 && NULL != pLhotseatbmp3)
	{
		m_pbmp_lhotseat[0].SetBitmapFromRes(pLhotseatbmp0);
		m_pbmp_lhotseat[1].SetBitmapFromRes(pLhotseatbmp1);
		m_pbmp_lhotseat[2].SetBitmapFromRes(pLhotseatbmp2);
		m_pbmp_lhotseat[3].SetBitmapFromRes(pLhotseatbmp3);
	}

	if (NULL != pRhotseatbmp0 && NULL != pRhotseatbmp1 && NULL != pRhotseatbmp2 && NULL != pRhotseatbmp3)
	{
		m_pbmp_rhotseat[0].SetBitmapFromRes(pRhotseatbmp0);
		m_pbmp_rhotseat[1].SetBitmapFromRes(pRhotseatbmp1);
		m_pbmp_rhotseat[2].SetBitmapFromRes(pRhotseatbmp2);
		m_pbmp_rhotseat[3].SetBitmapFromRes(pRhotseatbmp3);
	}

	if (WceUiGetBitsPixel() == 16)
	{
		m_pbmp_wind[0].SetBitmapFromRes(L"climatronic\\smallwind.565a.conf");
		m_pbmp_wind[1].SetBitmapFromRes(L"climatronic\\highwind.565a.conf");
	}
	else
	{
		m_pbmp_wind[0].SetBitmapFromRes(L"climatronic\\smallwind.32a.conf");
		m_pbmp_wind[1].SetBitmapFromRes(L"climatronic\\highwind.32a.conf");

	}
}

void CClimatronicLayer_DasAuto::OnInitLayer(void)
{
	RegisterMsg(UI_MSG_MCU_CANBUS_DATA);
	__super::OnInitLayer();
}


void CClimatronicLayer_DasAuto::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	OnDrawAirConData(pWndBitmap);
	OnDrawFanSpeed(pWndBitmap);
	OnDrawFanDirect(pWndBitmap);
	OnDrawHotSeat(pWndBitmap);
}


void CClimatronicLayer_DasAuto::DrawBmp(CWceUiGenericBitmap* pWndBitmap,int x,int y,CWceUiGenericBitmap* pSrcBitmap)
{	
	if (pSrcBitmap)
	{
		BLENDFUNCTION bf;
		memset(&bf, 0, sizeof(BLENDFUNCTION));
		bf.AlphaFormat = AC_SRC_ALPHA;
		CRect rc(x, y, x + pSrcBitmap->GetWidth(), y + pSrcBitmap->GetHeight());
		pWndBitmap->AlphaBlend(rc.left,rc.top,rc.Width(),rc.Height(),pSrcBitmap,0,0,bf);
	}
}

void CClimatronicLayer_DasAuto::DrawTxt(CWceUiGenericBitmap* pWndBitmap,LPCTSTR lpchText, LPRECT lprc,COLORREF clr,HFONT hFont,UINT uFormat)
{
	LPBYTE pbb;
	HBITMAP hbmp = pWndBitmap->GetHBitmap(pbb);
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	dc.SelectObject(hbmp);
	dc.SelectObject(hFont);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(clr);
	CRgn rgn;
	rgn.CreateRectRgn(lprc->left, lprc->top, lprc->right, lprc->bottom);
	dc.SelectObject(rgn);
	::DrawText(dc.GetSafeHdc(),lpchText,-1,lprc,uFormat);
	//::ExtTextOut(dc.GetSafeHdc(), lprc->left, lprc->top, ETO_OPAQUE, NULL, lpchText, ::_tcslen(lpchText), NULL);
	dc.DeleteDC();
}

void CClimatronicLayer_DasAuto::OnDrawFanSpeed(CWceUiGenericBitmap* pWndBitmap)
{
	CANBUS_RAISE_DASAUTO* pcan = CANBUS_RAISE_DASAUTO::get_instance();
	CWceUiGenericBitmap* pSrcBmp;
	BITMAP bmpinfo;
	int j = 5;
	for (int i = 0;i <= 6;i++)
	{
		pSrcBmp = m_pbmp_fanspeed_normal.GetBitmap();
		bmpinfo = pSrcBmp->GetBitMapInfo();
		pWndBitmap->BitBlt(280 + ( 28 + j) * i,374,bmpinfo.bmWidth,bmpinfo.bmHeight,pSrcBmp,0 ,0,SRCCOPY);
	}

	if (pcan->get_climatronic_FanSpeed() > 0)
	{
		for (int i = 1;i <= pcan->get_climatronic_FanSpeed() ;i++)
		{
			pSrcBmp = m_pbmp_fanspeed_down.GetBitmap();
			bmpinfo = pSrcBmp->GetBitMapInfo();
			pWndBitmap->BitBlt(280 + ( 28 + j) * (i- 1),374,bmpinfo.bmWidth,bmpinfo.bmHeight,pSrcBmp,0 ,0,SRCCOPY);
		}
	}
}

void CClimatronicLayer_DasAuto::OnDrawFanDirect(CWceUiGenericBitmap* pWndBitmap)
{
    CANBUS_RAISE_DASAUTO* pcan = CANBUS_RAISE_DASAUTO::get_instance();
	if (pcan->is_climatronic_Airdir_head())
	{
		DrawBmp(pWndBitmap,155,165,m_pbmp_head_l.GetBitmap());
		DrawBmp(pWndBitmap,580,165,m_pbmp_head_r.GetBitmap());
	}

	if (pcan->is_climatronic_Airdir_hands())
	{
		DrawBmp(pWndBitmap,140,217,m_pbmp_hands_l.GetBitmap());
		DrawBmp(pWndBitmap,557,217,m_pbmp_hands_r.GetBitmap());
	}

	if (pcan->is_climatronic_Airdir_feet())
	{
		DrawBmp(pWndBitmap,128,247,m_pbmp_feet_l.GetBitmap());
		DrawBmp(pWndBitmap,596,247,m_pbmp_feet_r.GetBitmap());
	}
}

void CClimatronicLayer_DasAuto::OnDrawAirConData(CWceUiGenericBitmap* pWndBitmap)
{
	CANBUS_RAISE_DASAUTO* pcan = CANBUS_RAISE_DASAUTO::get_instance();
	WCHAR szBuf[64] = {0};
	CRect rcDst(0,0,0,0);
	if (pcan->is_climatronic_AC())
	{
		rcDst.SetRect(285, 85, 335, 125);
		wcscpy(szBuf,L"AC");
		DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(), GetStyle()->GetFontCtrl()->GetFont());

	}

	if (pcan->is_climatronic_SmallWind() && !pcan->is_climatronic_HighWind())
	{
		rcDst.SetRect(195, 85, 275, 125);
		wcscpy(szBuf,L"Auto");
		DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(), GetStyle()->GetFontCtrl()->GetFont());
		DrawBmp(pWndBitmap,170, 85,m_pbmp_wind[0].GetBitmap());
	}

	if ((pcan->is_climatronic_HighWind() && !pcan->is_climatronic_HighWind())
		|| (pcan->is_climatronic_HighWind() && pcan->is_climatronic_HighWind())
		)
	{
		rcDst.SetRect(170, 85, 250, 125);
		wcscpy(szBuf,L"Auto");
		DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(), GetStyle()->GetFontCtrl()->GetFont());
		DrawBmp(pWndBitmap,245, 90,m_pbmp_wind[1].GetBitmap());
	}

	if (pcan->is_climatronic_Dual())
	{
		rcDst.SetRect(75, 85, 155, 125);
		wcscpy(szBuf,L"Dual");
		DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(), GetStyle()->GetFontCtrl()->GetFont());
	}

	if (pcan->is_climatronic_Switch())
	{
		if (pcan->is_climatronic_Cycle())
		{	
			DrawBmp(pWndBitmap,353, 80,m_pbmp_cycle[0].GetBitmap());
		}
		else
		{
			DrawBmp(pWndBitmap,353, 80,m_pbmp_cycle[1].GetBitmap());
		}
	}

	if (pcan->is_climatronic_FDemister())
	{
		DrawBmp(pWndBitmap,530, 80,m_pbmp_fdemister.GetBitmap());
	}

	if (pcan->is_climatronic_RDemister())
	{
		DrawBmp(pWndBitmap,460, 80,m_pbmp_rdemister.GetBitmap());
	}
	
	if (pcan->get_climatronic_LTemp() == 0x00)
	{
		wcscpy(szBuf,L"LO");
	}
	else if(pcan->get_climatronic_LTemp() == 0x1F)
	{
		wcscpy(szBuf,L"HI");
	}
	else
	{
		swprintf_s(szBuf, 64, L"%.1f", pcan->get_climatronic_LTemp());
		DrawBmp(pWndBitmap,150, 353,m_pbmp_unit.GetBitmap());
	}
	HFONT hfont = GetStyle()->GetFontCtrl()->GetFont();
	ASSERT(NULL != hfont);
	rcDst.SetRect(65, 355, 145, 400);
	DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(),hfont);


	if (pcan->get_climatronic_RTemp() == 0x00)
	{
		wcscpy(szBuf,L"LO");
	}
	else if(pcan->get_climatronic_RTemp() == 0x1F)
	{
		wcscpy(szBuf,L"HI");
	}
	else
	{
		swprintf_s(szBuf, 64, L"%.1f", pcan->get_climatronic_RTemp() );

		DrawBmp(pWndBitmap,700, 353,m_pbmp_unit.GetBitmap());
	}
	rcDst.SetRect(615, 355, 695, 400);
	DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(),hfont);

}

void CClimatronicLayer_DasAuto::OnDrawHotSeat(CWceUiGenericBitmap* pWndBitmap)
{
	CANBUS_RAISE_DASAUTO* pcan = CANBUS_RAISE_DASAUTO::get_instance();

	BYTE nValue;
	nValue = pcan->get_climatronic_LHotSeat();
    DrawBmp(pWndBitmap,315, 270,m_pbmp_lhotseat[nValue].GetBitmap());
	nValue = pcan->get_climatronic_RHotSeat();
	DrawBmp(pWndBitmap,425, 270,m_pbmp_rhotseat[nValue].GetBitmap());		

}


LRESULT CClimatronicLayer_DasAuto::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_MCU_CANBUS_DATA)
	{
		switch (lParam)
		{
		case CBDT_CLIMATRONIC:
			break;
		case CBDT_CAR_INFO:
			break;
		}
	}

	return __super::OnMessage(message, wParam, lParam);
}