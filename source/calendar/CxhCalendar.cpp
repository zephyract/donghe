#include "StdAfx.h"
#include "CxhCalendar.h"
#include "WceUiLayerCtrl.h"
#include "sysUtil.h"
#include "config.h"

#define  WHITE RGB(255,255,255)
#define  BLACK RGB(0,0,0)
#define  RED RGB(255,0,0)

extern UINT m_nMenuKeyMsg;
#define WCUI_WM_HOME (WM_USER+6002)

#define IMAGE_DISPLAY_MODE

const RECT rc_staticbig[7] =
{
	{564,158,720,328},
	{500,336-5,790,368-5},
	{500,370-5,790,402-5},
	{500,404-5,790,436-5},
	{500,436-5,790,468-5},
	{532,171,756,257},
	{532,257,756,421},

};

const WCHAR StrAnimalRes[12][50] =
{
	_T("calendar/animal/mouse.565a.conf"),
	_T("calendar/animal/cow.565a.conf"),
	_T("calendar/animal/tiger.565a.conf"),
	_T("calendar/animal/rabbit.565a.conf"),
	_T("calendar/animal/dragon.565a.conf"),
	_T("calendar/animal/snake.565a.conf"),
	_T("calendar/animal/horse.565a.conf"),
	_T("calendar/animal/sheep.565a.conf"),
	_T("calendar/animal/monkey.565a.conf"),
	_T("calendar/animal/chicken.565a.conf"),
	_T("calendar/animal/dog.565a.conf"),
	_T("calendar/animal/pig.565a.conf"),
};

const WCHAR StrAnimalRes_32[12][50] =
{
	_T("calendar/Animal/mouse.32a.conf"),
	_T("calendar/Animal/cow.32a.conf"),
	_T("calendar/Animal/tiger.32a.conf"),
	_T("calendar/Animal/rabbit.32a.conf"),
	_T("calendar/Animal/dragon.32a.conf"),
	_T("calendar/Animal/snake.32a.conf"),
	_T("calendar/Animal/horse.32a.conf"),
	_T("calendar/Animal/sheep.32a.conf"),
	_T("calendar/Animal/monkey.32a.conf"),
	_T("calendar/Animal/chicken.32a.conf"),
	_T("calendar/Animal/dog.32a.conf"),
	_T("calendar/Animal/pig.32a.conf"),
};

CxhCalendar::CxhCalendar(void)
{
	m_xStart  = -1;
	m_yStart  = -1;
	m_nWidth  = -1;
	m_nHeight = -1;

	memset(m_pEachDayArr, 0, sizeof(m_pEachDayArr));

	m_pYear  = NULL;
	m_pMonth = NULL;
	m_nLanguage = GetLanguage();
}

CxhCalendar::~CxhCalendar(void)
{
}

UINT CxhCalendar::GetLanguage(void)
{
	if (sysutil::nss_get_instance()->ui_lan_id == LAN_CHINESEGB
		|| sysutil::nss_get_instance()->ui_lan_id == LAN_CHINESEBIG5)
		
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void CxhCalendar::SetProperties(PCWceUiXmlElem pCreateElem)
{
	ASSERT(NULL != pCreateElem);
	CWceUiLayer::SetProperties(pCreateElem);

	LPCTSTR pX = pCreateElem->GetAttr(L"x");
	LPCTSTR pY = pCreateElem->GetAttr(L"y");
	LPCTSTR pWidth  = pCreateElem->GetAttr(L"width");
	LPCTSTR pHeight = pCreateElem->GetAttr(L"height");
	LPCTSTR pColorIndex = pCreateElem->GetAttr(L"colorindex1");
    LPCTSTR pLunarColorIndex = pCreateElem->GetAttr(L"colorindex2");
    LPCTSTR pEnColorIndex = pCreateElem->GetAttr(L"colorindex3");
	if (NULL != pX)
	{
		m_xStart = _ttoi(pX);
	}

	if (NULL != pY)
	{
		m_yStart = _ttoi(pY);
	}

	if (NULL != pWidth)
	{
		m_nWidth = _ttoi(pWidth);
	}

	if (NULL != pHeight)
	{
		m_nHeight = _ttoi(pHeight);
	}
	if (NULL != pColorIndex)
	{
		m_nColorIndex = _ttoi(pColorIndex);
	}
    if (NULL != pLunarColorIndex)
    {
        m_nlunarColorIndex = _ttoi(pLunarColorIndex);
    }
    if (NULL != pEnColorIndex)
    {
        m_nenColorIndex = _ttoi(pEnColorIndex);
    }

}

void CxhCalendar::OnInitLayer()
{
	RC_ANIMAL          = rc_staticbig[0];
	RC_CHINESEYEAR     = rc_staticbig[1];
	RC_CHINESEMD	   = rc_staticbig[2];   
	RC_CHINESEFESTIVAL = rc_staticbig[3];
	RC_CURDAY		   = rc_staticbig[4];

	RC_ENYEARMONTH = rc_staticbig[5]; 
	RC_ENDAY = rc_staticbig[6];

	m_pYear = (CWceUiStatic*)GetChildByName(L"year");
	ASSERT(NULL != m_pYear);

	m_pMonth = (CWceUiStatic*)GetChildByName(L"month");
	ASSERT(NULL != m_pMonth);
	if (m_nColorIndex)
	{
		 m_pYear->GetStyle()->SetTextColorFromApp(WHITE);
		 m_pMonth->GetStyle()->SetTextColorFromApp(WHITE);

	}else
	{
		m_pYear->GetStyle()->SetTextColorFromApp(BLACK);
		m_pMonth->GetStyle()->SetTextColorFromApp(BLACK);

	}
	CreateAllDays();
	CreateRDStaic();
	UpdateEachDay();
}

void CxhCalendar::OnLMouseDown(PCWceUiStatic pStatic, UINT nFlags, POINT point)
{
   OnDayClicked(pStatic);  
}

void CxhCalendar::OnClick(PCWceUiButton pButton)
{
    if (pButton->IsEqualName(L"year_leftbtn"))
    {
		OnYearChange(-1);
    }
	else if (pButton->IsEqualName(L"year_rightbtn"))
	{
		OnYearChange(1);
	}
	else if (pButton->IsEqualName(L"month_leftbtn"))
	{
		OnMonthChange(-1);
	}
	else if (pButton->IsEqualName(L"month_rightbtn"))
	{
         OnMonthChange(1);
	}
}

void CxhCalendar::OnYearChange( int changeIndex )
{
	WORD iYear, iMonth, iDay;
	m_calendardate.GetDate(iYear, iMonth, iDay);

	iYear += changeIndex;
	if ( iYear<1901 || iYear>2050 )
	{
		return;
	}else
    {
        Beep(1);
    }

	if(iYear != m_calendardate.GetYear())
	{
		if(iDay > m_calendardate.MonthDays(iYear, iMonth))
			iDay = m_calendardate.MonthDays(iYear, iMonth); 
		m_calendardate.SetDate(iYear, iMonth, iDay); 
	}
	UpdateEachDay();

}

void CxhCalendar::OnMonthChange( int changeIndex )
{
	WORD iYear, iDay, iMonth;
	m_calendardate.GetDate(iYear, iMonth, iDay); 

	iMonth += changeIndex;
	iMonth = (iMonth+13)%13;
	if( iMonth == 0 )	
	{
		if(changeIndex>0)
			iMonth = 1;
		else
			iMonth = 12;
	}	

	if(iMonth != m_calendardate.GetMonth())
	{
		if(iDay > m_calendardate.MonthDays(iYear, iMonth))
			iDay = m_calendardate.MonthDays(iYear, iMonth); 
		m_calendardate.SetDate(iYear, iMonth, iDay); 
	}
	UpdateEachDay();
}

void CxhCalendar::OnDayClicked(CWceUiStatic* p)
{
	ASSERT(NULL != p);

	for (int i = 0; i < EACH_DAY_COUNT; i++)
	{
		if (p == m_pEachDayArr[i])
		{
			
			wcscpy(m_strday,p->GetText());
			if (wcslen(m_strday))
			{
				int nDay = _ttoi(m_strday);
				int nDiff = nDay - m_calendardate.GetDay();

				if (0 != nDiff)
				{
					m_calendardate.SetDate(m_calendardate.GetYear(),m_calendardate.GetMonth(),nDay);
					UpdateEachDay();
					Beep(1);
				}
                else
                {
                  //  Beep(0);
                }
			}
		}
	}
}

void CxhCalendar::CreateRDStaic()
{
	if (m_nLanguage)
	{
        m_pAnimal = (CWceUiStatic*)GetLayerCtrl()->CreateLayer(this, WCEUI_CLASS_NAME(CWceUiStatic));
		ASSERT(NULL != m_pAnimal);
		m_pAnimal->GetStyle()->SetPosition(&RC_ANIMAL,WCEUI_RELATIVE_POSITION);
		CWceUiBgItem bgItem;
		m_pAnimal->GetStyle()->GetBgItemCtrl()->AddTail(bgItem);
		m_pAnimal->GetStyle()->SetNoInput(TRUE);//设置控件是不是需要不接收鼠标消息, bNoInput: TRUE表示不接收鼠标消息,FALSE表示要接收鼠标消息
		m_pAnimal->GetStyle()->GetBgColorCtrl()->SetTransparent(TRUE);



		m_plunarYear = (CWceUiStatic*)GetLayerCtrl()->CreateLayer(this, WCEUI_CLASS_NAME(CWceUiStatic));
		ASSERT(NULL != m_plunarYear);
		m_plunarYear->GetStyle()->SetPosition(&RC_CHINESEYEAR,WCEUI_RELATIVE_POSITION);
	//	CWceUiBgItem bgItem2;
	//	m_plunarYear->GetStyle()->GetBgItemCtrl()->AddTail(bgItem2);
		m_plunarYear->GetStyle()->SetNoInput(TRUE);
		m_plunarYear->GetStyle()->GetBgColorCtrl()->SetTransparent(TRUE);

		m_plunarMonthD = (CWceUiStatic*)GetLayerCtrl()->CreateLayer(this, WCEUI_CLASS_NAME(CWceUiStatic));
		ASSERT(NULL != m_plunarMonthD);
		m_plunarMonthD->GetStyle()->SetPosition(&RC_CHINESEMD,WCEUI_RELATIVE_POSITION);
		//CWceUiBgItem bgItem3;
//	    m_plunarMonthD->GetStyle()->GetBgItemCtrl()->AddTail(bgItem3);
		m_plunarMonthD->GetStyle()->SetNoInput(TRUE);
		m_plunarMonthD->GetStyle()->GetBgColorCtrl()->SetTransparent(TRUE);


		m_pHoliday = (CWceUiStatic*)GetLayerCtrl()->CreateLayer(this, WCEUI_CLASS_NAME(CWceUiStatic));
		ASSERT(NULL != m_pHoliday);
		m_pHoliday->GetStyle()->SetPosition(&RC_CHINESEFESTIVAL,WCEUI_RELATIVE_POSITION);
		//	CWceUiBgItem bgItem4;
		//m_pHoliday->GetStyle()->GetBgItemCtrl()->AddTail(bgItem4);
		m_pHoliday->GetStyle()->SetNoInput(TRUE);
		m_pHoliday->GetStyle()->GetBgColorCtrl()->SetTransparent(TRUE);

		m_pCurday = (CWceUiStatic*)GetLayerCtrl()->CreateLayer(this, WCEUI_CLASS_NAME(CWceUiStatic));
        ASSERT(NULL != m_pCurday);
		m_pCurday->GetStyle()->SetPosition(&RC_CURDAY,WCEUI_RELATIVE_POSITION);
//		CWceUiBgItem bgItem5;
//		m_pCurday->GetStyle()->GetBgItemCtrl()->AddTail(bgItem5);
		m_pCurday->GetStyle()->SetNoInput(TRUE);
		m_pCurday->GetStyle()->GetBgColorCtrl()->SetTransparent(TRUE);


		if (m_nlunarColorIndex)
		{
			m_plunarYear->GetStyle()->SetTextColorFromApp(WHITE);
			m_plunarMonthD->GetStyle()->SetTextColorFromApp(WHITE);
			m_pHoliday->GetStyle()->SetTextColorFromApp(WHITE);
			m_pCurday->GetStyle()->SetTextColorFromApp(WHITE);

		}else
		{
			m_plunarYear->GetStyle()->SetTextColorFromApp(BLACK);
			m_plunarMonthD->GetStyle()->SetTextColorFromApp(BLACK);
			m_pHoliday->GetStyle()->SetTextColorFromApp(BLACK);
			m_pCurday->GetStyle()->SetTextColorFromApp(BLACK);

		}
	}
	else
	{
		m_pENYearM = (CWceUiStatic*)GetLayerCtrl()->CreateLayer(this, WCEUI_CLASS_NAME(CWceUiStatic));
		m_pENYearM->GetStyle()->SetPosition(&RC_ENYEARMONTH,WCEUI_RELATIVE_POSITION);
		CWceUiBgItem bgItem1;
		m_pENYearM->GetStyle()->GetBgItemCtrl()->AddTail(bgItem1);
		m_pENYearM->GetStyle()->SetNoInput(TRUE);


		m_pENDay = (CWceUiStatic*)GetLayerCtrl()->CreateLayer(this, WCEUI_CLASS_NAME(CWceUiStatic));
		m_pENDay->GetStyle()->SetPosition(&RC_ENDAY,WCEUI_RELATIVE_POSITION);
		CWceUiBgItem bgItem2;
		m_pENDay->GetStyle()->GetBgItemCtrl()->AddTail(bgItem2);
		m_pENDay->GetStyle()->SetNoInput(TRUE);
        if(m_nenColorIndex)
        {
            m_pENYearM->GetStyle()->SetTextColorFromApp(WHITE);
            m_pENDay->GetStyle()->SetTextColorFromApp(WHITE);
        }else
        {
            m_pENYearM->GetStyle()->SetTextColorFromApp(BLACK);
            m_pENDay->GetStyle()->SetTextColorFromApp(BLACK);
        }
	
	}
}

void CxhCalendar::CreateAllDays()
{
	ASSERT(m_xStart >= 0);
	ASSERT(m_yStart >= 0);
	ASSERT(m_nWidth > 0);
	ASSERT(m_nHeight > 0);

	int index = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j ++)
		{
			m_pEachDayArr[index] = (CWceUiStatic*)GetLayerCtrl()->CreateLayer(this, WCEUI_CLASS_NAME(CWceUiStatic));
			ASSERT(NULL != m_pEachDayArr[index]);

			int x = m_xStart + j * m_nWidth;
			int y = m_yStart + i * m_nHeight;

			//set item position
			CRect rcPos(x, y, x + m_nWidth, y + m_nHeight);
			m_pEachDayArr[index]->GetStyle()->SetPosition(&rcPos, WCEUI_RELATIVE_POSITION);

			//add a bgitem for display bg image
			CWceUiBgItem bgItem;
			m_pEachDayArr[index]->GetStyle()->GetBgItemCtrl()->AddTail(bgItem);
			if (m_nLanguage)//set 中文or中文简体 
			{ 
				CWceUiBgItem bgItem1;
				m_pEachDayArr[index]->GetStyle()->GetBgItemCtrl()->AddTail(bgItem1);
			}
			//set no bgcolor
			m_pEachDayArr[index]->GetStyle()->GetBgColorCtrl()->SetTransparent(TRUE);
// 			m_pEachDayArr[index]->GetStyle()->GetBgColorCtrl()->SetColorFromApp(RGB(200,0,0)); 

			if (m_nColorIndex)
			{
				m_pEachDayArr[index]->GetStyle()->SetTextColorFromApp(WHITE);
			}else
			{
               m_pEachDayArr[index]->GetStyle()->SetTextColorFromApp(BLACK);
			}
#ifndef IMAGE_DISPLAY_MODE
			if ((j==0)||(j== 6))
			{
				m_pEachDayArr[index]->GetStyle()->SetTextColorFromApp(RED);
			}
#endif

			index++;
		}
	}
}


void CxhCalendar::OnDrawItem(int nDay, BOOL bSelected, CWceUiStatic* pItem)
{
	if (m_nLanguage)
	{	
		WORD iLunarYear, iLunarMonth, iLunarDay;
		m_calendardate.GetLunarDate(m_calendardate.GetYear(), m_calendardate.GetMonth(), nDay, iLunarYear, iLunarMonth, iLunarDay);
		int nIndexBmp = 0;
		if( iLunarDay!=1 )
		{
		   nIndexBmp =iLunarDay;
		}else
		{
           nIndexBmp = 30 +iLunarMonth;//每个月的初一被正月，二月，三月，四月....取代
		}

#ifdef IMAGE_DISPLAY_MODE
		memset(m_strlunarday,0,sizeof(m_strlunarday));

		if (m_nColorIndex)
		{
			swprintf(m_strlunarday,L"calendar/lunar/BWHITE_BMP_%d.565a.conf",nIndexBmp);
		}else
		{

			swprintf(m_strlunarday,L"calendar/lunar/BBLACK_BMP_%d.565a.conf",nIndexBmp);
		}

		CWceUiBgItem *pBgItem1 = pItem->GetStyle()->GetBgItemCtrl()->GetByIndex(1);
		ASSERT(NULL != pBgItem1);
		CRect Bmprt(m_nWidth/2,0,m_nWidth,m_nHeight);
		pBgItem1->SetDstRect(&Bmprt);
		pBgItem1->SetAlign(5);
		pBgItem1->SetBitmapFromRes(m_strlunarday);
		CRect rt(0,0,m_nWidth/2,m_nHeight);
		pItem->GetStyle()->GetDrawTextCtrl()->SetRect(&rt);
		pItem->GetStyle()->GetDrawTextCtrl()->SetFormat(38);

#else

		wchar_t szlunarDayTxt[4];//农历(每日)
		wchar_t text[18];

		WORD iHolDay = m_calendardate.GetLunarDate(m_calendardate.GetYear(), m_calendardate.GetMonth(), nDay, iLunarYear, iLunarMonth, iLunarDay);

		//获取节气信息
		wchar_t *HolText[] ={ L"小寒", L"大寒", L"立春", L"雨水",L"惊蛰", L"春分", 
			L"清明", L"谷雨",L"立夏", L"小满", L"芒种", L"夏至",
			L"小暑", L"大暑", L"立秋", L"处暑",	L"白露", L"秋分", 
			L"寒露", L"霜降",L"立冬", L"小雪", L"大雪", L"冬至"    };

		memset( text,0,sizeof(text) );
		memset(szlunarDayTxt,0,sizeof(szlunarDayTxt));
		//StrHolInfo.Empty();
		if(iHolDay)
		{
			//wcscpy(text,L"今日 ");
			wcscpy(text, HolText[iHolDay-1]);
			//StrHolInfo = text;
			swprintf(szlunarDayTxt,L"%s",text);
		}
		else
		{
			m_calendardate.FormatLunarDay(iLunarDay, szlunarDayTxt);//不是节气
		}

		CRect rt(0,0,m_nWidth,m_nHeight+10);
		pItem->GetStyle()->GetDrawTextCtrl()->SetRect(&rt);
// 		pItem->GetStyle()->GetDrawTextCtrl()->SetFormat(38);
		pItem->GetStyle()->GetDrawTextCtrl()->SetFormat(DT_CENTER);

//   	DrawText(NULL, NULL, NULL,NULL, DT_SINGLELINE|DT_CENTER|DT_BOTTOM)
		if (m_bbig)
		{
		   pItem->GetStyle()->GetFontCtrl()->SetFontFromRes(L"Tahoma-27.font");
		}else
		{
		  pItem->GetStyle()->GetFontCtrl()->SetFontFromRes(L"Tahoma-16.font");
		}

		memset(m_strday,0,sizeof(m_strday));
		swprintf(m_strday,L"%d",nDay);
		if (m_nLanguage)
		{
			_tcscat(m_strday, L"\n");
			_tcscat(m_strday, szlunarDayTxt);
			pItem->GetStyle()->GetFontCtrl()->SetFontFromRes(L"Tahoma-19.font");
		}
		pItem->SetText(m_strday);
		return;
#endif 
	}

	pItem->GetStyle()->GetFontCtrl()->SetFontFromRes(L"Tahoma-27.font");


	memset(m_strday,0,sizeof(m_strday));
	swprintf(m_strday,L"%d",nDay);
	pItem->SetText(m_strday);



	// 
	if (bSelected)
	{
		CWceUiBgItem *pBgItem = pItem->GetStyle()->GetBgItemCtrl()->GetByIndex(0);
		ASSERT(NULL != pBgItem);
		pBgItem->SetBitmapFromRes(L"calendar\\selected.565a.conf");
	}
}

void CxhCalendar::GetChinaHInfoStr( LPWSTR StrChinaInfo,LPWSTR StrLunarInfo ,LPWSTR StrHolInfo )
{
	wchar_t text[18];
	memset( text,0,sizeof(text) );
	WORD iYear , iMonth, iDay;
	WORD iHolDay = m_calendardate.GetLunarDate( m_calendardate.GetYear(), 
		m_calendardate.GetMonth(),
		m_calendardate.GetDay(), 
		iYear, iMonth, iDay); 
	//StrChinaInfo.Empty();
	memset(StrChinaInfo,0,sizeof(StrChinaInfo));
	//获取农历年信息
	//CCalendarData::FormatLunarYear(iYear, text ); 
	m_calendardate.FormatLunarYear(iYear, text); 



//	StrChinaInfo.Format(_T("%s"),text) ;
//	swprintf(StrChinaInfo,L"%s",text);
    wcscpy(StrChinaInfo,text);

	//获取农历日期
	memset( text,0,sizeof(text) );
//	StrLunarInfo.Empty();
    memset(StrLunarInfo,0,sizeof(StrLunarInfo));
	m_calendardate.FormatMonth(iMonth, text);
	wcscpy(StrLunarInfo,text);

	memset( text,0,sizeof(text) );
	m_calendardate.FormatLunarDay(iDay, text);
//	StrLunarInfo.Format(_T("%s %s"),StrLunarInfo,text) ;
	wcscat(StrLunarInfo,text);
	//swprintf(StrChinaInfo,L"%s %s",iMonth,text);



	//获取节气信息
	wchar_t *HolText[] ={ L"小寒", L"大寒", L"立春", L"雨水",L"惊蛰", L"春分", 
		L"清明", L"谷雨",L"立夏", L"小满", L"芒种", L"夏至",
		L"小暑", L"大暑", L"立秋", L"处暑",	L"白露", L"秋分", 
		L"寒露", L"霜降",L"立冬", L"小雪", L"大雪", L"冬至"    };

	memset( text,0,sizeof(text) );
	memset(StrHolInfo,0,sizeof(StrHolInfo));
	//StrHolInfo.Empty();
	if(iHolDay)
	{
		//wcscpy(text,L"今日 ");
		wcscpy(text, HolText[iHolDay-1]);
		//StrHolInfo = text;
		swprintf(StrHolInfo,L"%s",text);
	}
	else
	{
		//不是节气
	}
}
void CxhCalendar::DrawRDStatic()
{
	if (m_nLanguage)
	{

		CWceUiBgItem *pBgItem = m_pAnimal->GetStyle()->GetBgItemCtrl()->GetByIndex(0);
		ASSERT(NULL != pBgItem);
		//DWORD dwyear = m_calendardate.GetYear();
		WORD iYear , iMonth, iDay;
		WORD iHolDay = m_calendardate.GetLunarDate( m_calendardate.GetYear(), 
			m_calendardate.GetMonth(),
			m_calendardate.GetDay(), 
			iYear, iMonth, iDay);
		int index = (iYear-4)%12;

		
		if (WceUiGetBitsPixel() == 16)
		{
			pBgItem->SetBitmapFromRes(StrAnimalRes[index]);
		}
		else
		{
			pBgItem->SetBitmapFromRes(StrAnimalRes_32[index]);
		}


        //m_pAnimal->SetText(StrAnimalRes[index]);

		GetChinaHInfoStr( m_strchinainfo, m_strlunainfo ,m_strholinfo );
		//CWceUiBgItem *pBgItem1 = m_plunarYear->GetStyle()->GetBgItemCtrl()->GetByIndex(0);
		//ASSERT(NULL != pBgItem);
		//m_plunarYear->SetText(m_strchinainfo);
		memset(m_strtoday,0,sizeof(m_strtoday));
		_stprintf(m_strtoday,L"%4d年%d月%d日",m_calendardate.GetYear(),m_calendardate.GetMonth(),m_calendardate.GetDay());

		m_plunarYear->GetStyle()->GetFontCtrl()->SetFontFromRes(L"Tahoma-27.font");
		m_plunarMonthD->GetStyle()->GetFontCtrl()->SetFontFromRes(L"Tahoma-27.font");
		m_pHoliday->GetStyle()->GetFontCtrl()->SetFontFromRes(L"Tahoma-27.font");
		m_pCurday->GetStyle()->GetFontCtrl()->SetFontFromRes(L"Tahoma-27.font");



		m_plunarMonthD->SetText(m_strlunainfo);
		m_plunarYear->SetText(m_strchinainfo);
       if (wcslen(m_strholinfo))
       {
		   m_pHoliday->SetText(m_strholinfo);
       }else
	   {
          m_pHoliday->SetText(L" ");
	   }

	   m_pCurday->SetText(m_strtoday);

	}
	else
	{
		memset(m_strenyearm,0,sizeof(m_strenyearm));
		memset(m_strenday,0,sizeof(m_strenday));
		swprintf(m_strenyearm,L"%4d - %2d",m_calendardate.GetYear(),m_calendardate.GetMonth());
		swprintf(m_strenday,L"%d",m_calendardate.GetDay());

		m_pENYearM->GetStyle()->GetFontCtrl()->SetFontFromRes(L"Tahoma-27.font");
		m_pENDay->GetStyle()->GetFontCtrl()->SetFontFromRes(L"Tahoma-48.font");


		m_pENYearM->SetText(m_strenyearm);
		m_pENDay->SetText(m_strenday);
        CWceUiBgItem *pBgItem1 = m_pENYearM->GetStyle()->GetBgItemCtrl()->GetByIndex(0);
        ASSERT(NULL != pBgItem1);
		pBgItem1->SetBitmapFromRes(L"calendar\\en_year.565a.conf");


		CWceUiBgItem *pBgItem2 = m_pENDay->GetStyle()->GetBgItemCtrl()->GetByIndex(0);
		ASSERT(NULL != pBgItem2);
		pBgItem2->SetBitmapFromRes(L"calendar\\en_day.565a.conf");
	}

}
void CxhCalendar::ClearRDStatic()
{
	if (m_nLanguage)
	{
		CWceUiBgItem *pBgItem = m_pAnimal->GetStyle()->GetBgItemCtrl()->GetByIndex(0);
		ASSERT(NULL != pBgItem);
		pBgItem->SetBitmapFromApp(NULL);
        m_pAnimal->SetText(L" ");
        m_plunarYear->SetText(L" ");
		m_plunarMonthD->SetText(L" ");
		m_pHoliday->SetText(L" ");
		m_pCurday->SetText(L" ");

	}
	else
	{

		m_pENYearM->SetText(L" ");
		CWceUiBgItem *pBgItem1 = m_pENYearM->GetStyle()->GetBgItemCtrl()->GetByIndex(0);
		ASSERT(NULL != pBgItem1);
		pBgItem1->SetBitmapFromApp(NULL);
		m_pENDay->SetText(L" ");
		CWceUiBgItem *pBgItem2 = m_pENDay->GetStyle()->GetBgItemCtrl()->GetByIndex(0);
		ASSERT(NULL != pBgItem2);
		pBgItem2->SetBitmapFromApp(NULL);

	}

}
void CxhCalendar::OnClearItem(CWceUiStatic* pItem)
{
	ASSERT(NULL != pItem);

	
	CWceUiBgItem* pBgItem = pItem->GetStyle()->GetBgItemCtrl()->GetByIndex(0);
	ASSERT(NULL != pBgItem);
	pBgItem->SetBitmapFromApp(NULL);
	if (m_nLanguage)
	{
		CWceUiBgItem* pBgItem1 = pItem->GetStyle()->GetBgItemCtrl()->GetByIndex(1);
		ASSERT(NULL != pBgItem1);
		pBgItem1->SetBitmapFromApp(NULL);
	}
	pItem->SetText(L"");

}
void CxhCalendar::UpdateEachDay()
{
	for (int i = 0; i < EACH_DAY_COUNT; i++)
	{
		OnClearItem(m_pEachDayArr[i]);
	}
	ClearRDStatic();
	int nIndex = m_calendardate.WeekDay(m_calendardate.GetYear(),m_calendardate.GetMonth(),1);//获取当前月 1号对应星期几

	int nCount = 1;
	int nDayCount = m_calendardate.MonthDays(m_calendardate.GetYear(),m_calendardate.GetMonth());
	while(nCount <= nDayCount)
	{
			BOOL bSelect = (m_calendardate.GetDay() == nCount);
			OnDrawItem(nCount, bSelect, m_pEachDayArr[nIndex]);
			nCount++;
			nIndex++;
	}
    memset(m_stryear,0,sizeof(m_stryear));
	memset(m_strmonth,0,sizeof(m_strmonth));
	if (m_nLanguage)
	{
		swprintf(m_stryear,L"%d 年",m_calendardate.GetYear());
		swprintf(m_strmonth,L"%d 月",m_calendardate.GetMonth());
	}else
	{
		swprintf(m_stryear,L"%d",m_calendardate.GetYear());
		swprintf(m_strmonth,L"%d",m_calendardate.GetMonth());

	}
	m_pYear->SetText(m_stryear);
	m_pMonth->SetText(m_strmonth);
	DrawRDStatic();
}

void CxhCalendar::Beep( BOOL bIsOK )
{

}