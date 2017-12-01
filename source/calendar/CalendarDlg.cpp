#include "StdAfx.h"
#include "CalendarDlg.h"
#include "CxhCalendar.h"

CCalendarDlg::CCalendarDlg(void)
{

}

CCalendarDlg::~CCalendarDlg(void)
{
}

void CCalendarDlg::OnInitDialog()
{
}


LRESULT CCalendarDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return __super::OnMessage(message,wParam,lParam);
}


void CCalendarDlg::OnBnClick(CWceUiButton* pButton)
{
	CxhCalendar* pCalendar = (CxhCalendar*)GetChildByName(L"calendar");

	if (pButton->IsEqualName(L"year_leftbtn"))
	{
		pCalendar->OnYearChange(-1);
	}
	else if (pButton->IsEqualName(L"year_rightbtn"))
	{
		pCalendar->OnYearChange(1);
	}
	else if (pButton->IsEqualName(L"month_leftbtn"))
	{
		pCalendar->OnMonthChange(-1);
	}
	else if (pButton->IsEqualName(L"month_rightbtn"))
	{
		pCalendar->OnMonthChange(1);
	}
	else if (pButton->IsKindOf(L"CWceUiStatic"))
	{
		pCalendar->OnDayClicked((CWceUiStatic*)pButton);
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}