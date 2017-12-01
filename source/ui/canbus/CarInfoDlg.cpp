#include "stdafx.h"
#include "CarInfoDlg.h"
#include "CanBus.h"
#include "interface.h"
#include "wceuiloadstring.h"
#include "sysUtil.h"

CCarInfoDlg::CCarInfoDlg(void)
{

}

CCarInfoDlg::~CCarInfoDlg(void)
{

}




void CCarInfoDlg::SetProperties(PCWceUiXmlElem pCreateElem)
{
	ASSERT(NULL != pCreateElem);
	CWceUiLayer::SetProperties(pCreateElem);
	LPCTSTR pBmp_remailL                     = pCreateElem->GetAttr(L"bmp_remailL");
	LPCTSTR pBmp_remailL_alarm               = pCreateElem->GetAttr(L"bmp_remailL_alarm");
	LPCTSTR pBmp_tempoutside                 = pCreateElem->GetAttr(L"bmp_tempoutside");
	LPCTSTR pBmp_battvtg					 = pCreateElem->GetAttr(L"bmp_battvtg");
	LPCTSTR pBmp_battvtg_alarm				 = pCreateElem->GetAttr(L"bmp_battvtg_alarm");
	LPCTSTR pBmp_seatbelt                    = pCreateElem->GetAttr(L"bmp_seatbelt");
	LPCTSTR pBmp_seatbelt_alarm              = pCreateElem->GetAttr(L"bmp_seatbelt_alarm");	
	LPCTSTR pBmp_trunk                       = pCreateElem->GetAttr(L"bmp_trunk");
	LPCTSTR pBmp_brake                       = pCreateElem->GetAttr(L"bmp_brake");
	LPCTSTR pBmp_brake_alarm                 = pCreateElem->GetAttr(L"bmp_brake_alarm");
	LPCTSTR pBmp_cleanwater                  = pCreateElem->GetAttr(L"bmp_cleanwater");
	LPCTSTR pBmp_cleanwater_alarm            = pCreateElem->GetAttr(L"bmp_cleanwater_alarm");
	LPCTSTR pBmp_enginespeed                 = pCreateElem->GetAttr(L"bmp_enginespeed");
	LPCTSTR pBmp_velocity                    = pCreateElem->GetAttr(L"bmp_velocity");
	LPCTSTR pBmp_mileage                     = pCreateElem->GetAttr(L"bmp_mileage");
	LPCTSTR pBmp_dr_fl                       = pCreateElem->GetAttr(L"bmp_dr_fl");
	LPCTSTR pBmp_dr_fr                       = pCreateElem->GetAttr(L"bmp_dr_fr");
	LPCTSTR pBmp_dr_rl                       = pCreateElem->GetAttr(L"bmp_dr_rl");
	LPCTSTR pBmp_dr_rr                       = pCreateElem->GetAttr(L"bmp_dr_rr");
	LPCTSTR pBmp_dr_alarm                    = pCreateElem->GetAttr(L"bmp_dr_alarm");
	LPCTSTR pBmp_trunk_alarm                 = pCreateElem->GetAttr(L"bmp_trunk_alarm");

	if (NULL != pBmp_remailL && NULL != pBmp_remailL_alarm)
	{
		m_pBmp_remailL[0].SetBitmapFromRes(pBmp_remailL);
		m_pBmp_remailL[1].SetBitmapFromRes(pBmp_remailL_alarm);
	}

	if (NULL != pBmp_tempoutside)
	{
        m_pBmp_tempoutside.SetBitmapFromRes(pBmp_tempoutside);
	}

	if (NULL != pBmp_battvtg && NULL != pBmp_battvtg_alarm)
	{
       m_pBmp_battvtg[0].SetBitmapFromRes(pBmp_battvtg);
	   m_pBmp_battvtg[1].SetBitmapFromRes(pBmp_battvtg_alarm);
	}

	if (NULL != pBmp_seatbelt && NULL != pBmp_seatbelt_alarm )
	{
		m_pBmp_seatbelt[0].SetBitmapFromRes(pBmp_seatbelt);
		m_pBmp_seatbelt[1].SetBitmapFromRes(pBmp_seatbelt_alarm);
	}

	if (NULL != pBmp_trunk)
	{
		m_pBmp_trunk.SetBitmapFromRes(pBmp_trunk);
	}

	if (NULL != pBmp_brake && NULL != pBmp_brake_alarm)
	{
		m_pBmp_brake[0].SetBitmapFromRes(pBmp_brake);
		m_pBmp_brake[1].SetBitmapFromRes(pBmp_brake_alarm);
	}

	if (NULL != pBmp_cleanwater && NULL != pBmp_cleanwater_alarm)
	{
		m_pBmp_cleanwater[0].SetBitmapFromRes(pBmp_cleanwater);
		m_pBmp_cleanwater[1].SetBitmapFromRes(pBmp_cleanwater_alarm);
	}

	if (NULL != pBmp_enginespeed)
	{
		m_pBmp_enginespeed.SetBitmapFromRes(pBmp_enginespeed);
	}

	if (NULL != pBmp_velocity)
	{
		m_pBmp_velocity.SetBitmapFromRes(pBmp_velocity);
	}

	if (NULL != pBmp_mileage)
	{
	    m_pBmp_mileage.SetBitmapFromRes(pBmp_mileage);
	}

	if (NULL != pBmp_dr_fl && NULL != pBmp_dr_fr && NULL != pBmp_dr_rl && NULL != pBmp_dr_rr && NULL != pBmp_dr_alarm)
	{
		m_pBmp_dr[0].SetBitmapFromRes(pBmp_dr_fl);
		m_pBmp_dr[1].SetBitmapFromRes(pBmp_dr_fr);
		m_pBmp_dr[2].SetBitmapFromRes(pBmp_dr_rl);
		m_pBmp_dr[3].SetBitmapFromRes(pBmp_dr_rr);
		m_pBmp_dr_alarm.SetBitmapFromRes(pBmp_dr_alarm);
	}
      

	if (NULL != pBmp_trunk_alarm)
	{
		m_pBmp_trunk_alarm.SetBitmapFromRes(pBmp_trunk_alarm);
	}	
}

static void _request_can_data(BYTE type, BYTE info_id)
{
	switch (sysutil::nss_get_instance()->sys_canbus_type)
	{
	case CANBUS_TYPE_RAISE_DASAUTO:
		CANBUS_RAISE_DASAUTO::get_instance()->request_can_data(type, info_id);
		break;
	case CANBUS_TYPE_RAISE_FOCUS:
		break;
	case CANBUS_TYPE_RAISE_HONDA:
		break;
	case CANBUS_TYPE_RAISE_PEUGEOT:
		break;
	default:
		break;
	}
}

void CCarInfoDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_MCU_CANBUS_DATA);

// 	_request_can_data(0x16, 0);
// 	_request_can_data(0x24, 0);
	_request_can_data(0x41, 1);
	_request_can_data(0x41, 2);
	_request_can_data(0x41, 3);

	SetTimer(TIMER_ID_GET_CARINFO, 500);
	__super::OnInitDialog();
}

void CCarInfoDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_GET_CARINFO)
	{
		if (IsLayerVisible())
		{
			_request_can_data(0x41, 2);
		}
	}
	__super::OnTimer(wIDEvent);
}

LRESULT CCarInfoDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_MCU_CANBUS_DATA)
	{
		switch (lParam)
		{
		case CBDT_CLIMATRONIC:
			break;
		case CBDT_CAR_INFO:
			InvalidateRect();
			break;
		}
	}
	return __super::OnMessage(message, wParam, lParam);
}

void CCarInfoDlg::OnBnClick(CWceUiButton* pButton)
{
	__super::OnBnClick(pButton);
}

CRect Set_rect(int l,int t, int w = 75,int h = 30)
{
   CRect rc(l,t, l+w, t+h);
   return rc;
}

int icon_coor[][2] = {
	{16, 67},                        
	{16, 202},                     
	{16, 337},
	{186, 81},
	{186, 182},
	{186, 283},
	{186, 387},
	{408, 67},
	{408, 202},
	{408, 337},

	{615, 178}, //DR_FL
	{745, 178}, //DR_FR
	{615, 230}, //DR_RL
	{745, 230}, //DR_RR
	{603, 374}, //alarm
	{655, 309}  //trunk alarm
};

void CCarInfoDlg::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{   
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	CANBUS_RAISE_DASAUTO* pcan = CANBUS_RAISE_DASAUTO::get_instance();
	WCHAR szBuf[64] = {0};
	CWceUiLoadString uistr;	
	CRect rcDst;
	HFONT hfont = GetStyle()->GetFontCtrl()->GetFont();
	ASSERT(NULL != hfont);

	//RemainL
	if (pcan->is_car_info_RemainL_low())
	{
		DrawBmp(pWndBitmap,icon_coor[0][0], icon_coor[0][1],m_pBmp_remailL[1].GetBitmap());
		uistr.SetTextResID(L"CARINFO_NOENOUGH");
		_tcscpy(szBuf,uistr.GetString());
	} 
	else
	{
		DrawBmp(pWndBitmap,icon_coor[0][0], icon_coor[0][1],m_pBmp_remailL[0].GetBitmap());
		swprintf_s(szBuf, 64, L"%dL", pcan->get_car_info_RemainL());
	}	
	
	
	rcDst = Set_rect(icon_coor[0][0], icon_coor[0][1] + 85);
	DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(), hfont);


    //tempoutside
	DrawBmp(pWndBitmap,icon_coor[1][0], icon_coor[1][1],m_pBmp_tempoutside.GetBitmap());
	rcDst = Set_rect(icon_coor[1][0], icon_coor[1][1] + 85);
	swprintf_s(szBuf, 64, L"%d¡æ", pcan->get_car_info_TempOutside());	
	DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(), hfont);

	//battvtg
	if (pcan->is_car_info_BattVtg_low())
	{
		DrawBmp(pWndBitmap,icon_coor[2][0], icon_coor[2][1],m_pBmp_battvtg[1].GetBitmap());
		uistr.SetTextResID(L"CARINFO_NOENOUGH");
		_tcscpy(szBuf,uistr.GetString());
	} 
	else
	{
		DrawBmp(pWndBitmap,icon_coor[2][0], icon_coor[2][1],m_pBmp_battvtg[0].GetBitmap());
		swprintf_s(szBuf, 64, L"%dV", pcan->get_car_info_BattVtg());	
	}
	rcDst = Set_rect(icon_coor[2][0], icon_coor[2][1] + 85);	
	DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(), hfont);

	//SeatBelt state
	int w_text = 150, h_text = 25;
	rcDst = Set_rect(icon_coor[3][0] + 85, icon_coor[3][1] + 35,w_text,h_text);
	if (pcan->is_car_info_SeatBelt_off())
	{
		DrawBmp(pWndBitmap,icon_coor[3][0], icon_coor[3][1],m_pBmp_seatbelt[1].GetBitmap());
		uistr.SetTextResID(L"CARINFO_SEATBLET_ON");
	}
	else
	{
		DrawBmp(pWndBitmap,icon_coor[3][0], icon_coor[3][1],m_pBmp_seatbelt[0].GetBitmap());
		uistr.SetTextResID(L"CARINFO_NORMAL");	
	}
	DrawTxt(pWndBitmap,uistr.GetString(), rcDst,GetStyle()->GetTextColor(), hfont);
 
	//trunk state
    rcDst = Set_rect(icon_coor[4][0] + 85, icon_coor[4][1] + 35,w_text,h_text);
	DrawBmp(pWndBitmap,icon_coor[4][0], icon_coor[4][1],m_pBmp_trunk.GetBitmap());
	if (pcan->is_car_info_Trunk_Open())
	{
		uistr.SetTextResID(L"CARINFO_ON");
		DrawBmp(pWndBitmap,icon_coor[15][0], icon_coor[15][1],m_pBmp_trunk_alarm.GetBitmap());
	} 
	else
	{
		uistr.SetTextResID(L"CARINFO_NORMAL");
	}
	DrawTxt(pWndBitmap,uistr.GetString(), rcDst,GetStyle()->GetTextColor(), hfont);

    //brake state
    rcDst = Set_rect(icon_coor[5][0] + 85, icon_coor[5][1] + 35,w_text,h_text);	
	if (pcan->is_car_info_Brake_hung())
	{
		DrawBmp(pWndBitmap,icon_coor[5][0], icon_coor[5][1],m_pBmp_brake[1].GetBitmap());
		uistr.SetTextResID(L"CARINFO_BRAKE_ON");		
	} 
	else
	{
		DrawBmp(pWndBitmap,icon_coor[5][0], icon_coor[5][1],m_pBmp_brake[0].GetBitmap());
		uistr.SetTextResID(L"CARINFO_NORMAL");
	}
	DrawTxt(pWndBitmap,uistr.GetString(), rcDst,GetStyle()->GetTextColor(), hfont);


    //cleanwater state
	rcDst = Set_rect(icon_coor[6][0] + 85, icon_coor[6][1] + 35,w_text,h_text);	
	if (pcan->is_car_info_CleanWater_low())
	{
		DrawBmp(pWndBitmap,icon_coor[6][0], icon_coor[6][1],m_pBmp_cleanwater[1].GetBitmap());
		uistr.SetTextResID(L"CARINFO_NOENOUGH");
	} 
	else
	{
		DrawBmp(pWndBitmap,icon_coor[6][0], icon_coor[6][1],m_pBmp_cleanwater[0].GetBitmap());
		uistr.SetTextResID(L"CARINFO_NORMAL");
	}
	DrawTxt(pWndBitmap,uistr.GetString(), rcDst,GetStyle()->GetTextColor(), hfont);
	
    //EngineSpeed
	DrawBmp(pWndBitmap,icon_coor[7][0], icon_coor[7][1],m_pBmp_enginespeed.GetBitmap());
	rcDst = Set_rect(icon_coor[7][0], icon_coor[7][1] + 85,120);
	swprintf_s(szBuf, 64, L"%dRpm", pcan->get_car_info_EngineSpeed());	
    DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(), hfont);
    
	//InstantaneousVelocity
	DrawBmp(pWndBitmap,icon_coor[8][0], icon_coor[8][1],m_pBmp_velocity.GetBitmap());
	rcDst = Set_rect(icon_coor[8][0], icon_coor[8][1] + 85,120);
	swprintf_s(szBuf, 64, L"%dKm/h", pcan->get_car_info_InstantaneousVelocity());	
    DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(), hfont);

	//Mileage
	DrawBmp(pWndBitmap,icon_coor[9][0], icon_coor[9][1],m_pBmp_mileage.GetBitmap());
	rcDst = Set_rect(icon_coor[9][0], icon_coor[9][1] + 85,120);
	swprintf_s(szBuf, 64, L"%dKm", pcan->get_car_info_Mileage());	
	DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(), hfont);

	
	memset(szBuf,NULL,sizeof(szBuf));
	uistr.SetTextResID(L"CARINFO_DR_CLOSE");
	_tcscpy(szBuf,uistr.GetString());
	DrawBmp(pWndBitmap,icon_coor[14][0], icon_coor[14][1],m_pBmp_dr_alarm.GetBitmap());
	if(pcan->is_car_info_Door_FL())
	{
       DrawBmp(pWndBitmap,icon_coor[10][0], icon_coor[10][1],m_pBmp_dr[0].GetBitmap());
	   uistr.SetTextResID(L"CARINFO_DR_FL");
	   _tcscpy(szBuf,uistr.GetString());
	}

	if(pcan->is_car_info_Door_FR())
	{
		DrawBmp(pWndBitmap,icon_coor[11][0], icon_coor[11][1],m_pBmp_dr[1].GetBitmap());
		uistr.SetTextResID(L"CARINFO_DR_FR");
		_tcscpy(szBuf,uistr.GetString());
	}

	if(pcan->is_car_info_Door_RL())
	{
		DrawBmp(pWndBitmap,icon_coor[12][0], icon_coor[12][1],m_pBmp_dr[2].GetBitmap());
		uistr.SetTextResID(L"CARINFO_DR_RL");
		_tcscpy(szBuf,uistr.GetString());
	}

	if(pcan->is_car_info_Door_RR())
	{
		DrawBmp(pWndBitmap,icon_coor[13][0], icon_coor[13][1],m_pBmp_dr[3].GetBitmap());
		uistr.SetTextResID(L"CARINFO_DR_RR");
		_tcscpy(szBuf,uistr.GetString());
	}
  
	rcDst = Set_rect(icon_coor[14][0] + 80, icon_coor[14][1] + 20,100);
	DrawTxt(pWndBitmap,szBuf, rcDst,GetStyle()->GetTextColor(), hfont);
}

void CCarInfoDlg::DrawBmp(CWceUiGenericBitmap* pWndBitmap,int x,int y,CWceUiGenericBitmap* pSrcBitmap)
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

void CCarInfoDlg::DrawTxt(CWceUiGenericBitmap* pWndBitmap,LPCTSTR lpchText, LPRECT lprc,COLORREF clr,HFONT hFont,UINT uFormat)
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

