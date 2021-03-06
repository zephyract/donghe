#include "stdafx.h"
#include "SetDateTimeDlg.h"
#include "tzUtil.h"
#include "sysUtil.h"
#include "interface.h"
#include "WceUiBase.h"
#include "WceUiRealWnd.h"

CSetDateTimeDlg::CSetDateTimeDlg(void)
{

}

CSetDateTimeDlg::~CSetDateTimeDlg(void)
{

}

void CSetDateTimeDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(UI_MSG_TIMEZONE_CHANGE);

	InitLayers();
}

void CSetDateTimeDlg::InitLayers()
{
	CWceUiScrollLayer* pLayer = (CWceUiScrollLayer*)GetChildByName(L"frame");
	if (pLayer)
	{
		pLayer->SetScrollMode(SL_VERT);
		pLayer->SetExtraScroll(30);
	}

	// year
	TCHAR buffer[16];
	m_pListYear = (CSetDateTimeListBox*)GetLayerByName(L"year");
	m_pListYear->SetItemsVisible(90);
	for (int i=0; i<270; i++)
	{
		_itot(1974+i%90, buffer, 10);
		m_pListYear->AddString(buffer);
	}

	// month
	m_pListMonth = (CSetDateTimeListBox*)GetLayerByName(L"month");
	m_pListMonth->SetItemsVisible(12);
	for (int i=0; i<36; i++)
	{
		_stprintf(buffer, L"%02d", i%12+1);
		m_pListMonth->AddString(buffer);
	}

	// day
	m_pListDay = (CSetDateTimeListBox*)GetLayerByName(L"day");
	m_pListDay->SetItemsVisible(31);
	for (int i=0; i<93; i++)
	{
		_stprintf(buffer, L"%02d", i%31+1);
		m_pListDay->AddString(buffer);
	}

	// hour
	m_pListHour = (CSetDateTimeListBox*)GetLayerByName(L"hour");
	m_pListHour->SetItemsVisible(24);
	for (int i=0; i<72; i++)
	{
		_stprintf(buffer, L"%02d", i%24);
		m_pListHour->AddString(buffer);
	}

	// minute
	m_pListMinute = (CSetDateTimeListBox*)GetLayerByName(L"minute");
	m_pListMinute->SetItemsVisible(60);
	for (int i=0; i<180; i++)
	{
		_stprintf(buffer, L"%02d", i%60);
		m_pListMinute->AddString(buffer);
	}

	// set control's time
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	SetTime(&tm);

	//
	InitLayersStatus();
}

void CSetDateTimeDlg::InitLayersStatus()
{
	// 24小时制
	CWceUiSButton* psbutton = (CWceUiSButton*)GetLayerByName(L"hourtime_switch");
	ASSERT(psbutton);
	psbutton->SetStatus(sysutil::nss_get_instance()->ui_24hourtime ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);

	// 自动设置时间
	psbutton = (CWceUiSButton*)GetLayerByName(L"autoset_switch");
	ASSERT(psbutton);
	psbutton->SetStatus(sysutil::nss_get_instance()->ui_time_autoset ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);

	// dst
	psbutton = (CWceUiSButton*)GetLayerByName(L"dst_switch");
	if (psbutton)
	{
		psbutton->SetStatus(sysutil::nss_get_instance()->ui_time_dst ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);
	}


	// 根据自动设置状态，调整控件
	OnAutosetChange();
}

void CSetDateTimeDlg::OnReset()
{
	InitLayersStatus();
}

void CSetDateTimeDlg::GetTime(LPSYSTEMTIME lpSystemTime)
{
	if (!lpSystemTime)
	{
		return;
	}

	lpSystemTime->wYear = 1974 + (m_pListYear->GetCurSel())%90;
	lpSystemTime->wMonth = 1 + (m_pListMonth->GetCurSel())%12;
	lpSystemTime->wDay = 1 + (m_pListDay->GetCurSel())%31;
	lpSystemTime->wHour = 0 + (m_pListHour->GetCurSel())%24;
	lpSystemTime->wMinute = 0 + (m_pListMinute->GetCurSel())%60;
	lpSystemTime->wSecond = 0;
}

void CSetDateTimeDlg::SetTime(LPSYSTEMTIME lpSystemTime)
{
	if (!lpSystemTime)
	{
		return;
	}

	// year
	int year = lpSystemTime->wYear;
	year = max(1974, year);
	year = min(1974+89, year);
	m_pListYear->SetTopIndex(year-1974+(90-2), FALSE);
	m_pListMonth->SetTopIndex(lpSystemTime->wMonth-1+(12-2), FALSE);
	m_pListDay->SetTopIndex(lpSystemTime->wDay-1+(31-2), FALSE);
	m_pListHour->SetTopIndex(lpSystemTime->wHour-0+(24-2), FALSE);
	m_pListMinute->SetTopIndex(lpSystemTime->wMinute-0+(60-2), FALSE);


	m_pListYear->SetCurSel(year-1974+(90), FALSE);
	m_pListMonth->SetCurSel(lpSystemTime->wMonth-1+(12), FALSE);
	m_pListDay->SetCurSel(lpSystemTime->wDay-1+(31), FALSE);
	m_pListHour->SetCurSel(lpSystemTime->wHour-0+(24), FALSE);
	m_pListMinute->SetCurSel(lpSystemTime->wMinute-0+(60), FALSE);
}

void CSetDateTimeDlg::SaveTime()
{
	// set time
	SYSTEMTIME tm;
	//	GetLocalTime(&tm);
	GetTime(&tm);

	SetLocalTime(&tm);

	// set timezone
	//	SetTimeZone(m_nIndexTimeZone);
}


static BOOL IsLeapYear(int nYear)
{
	if(nYear%400==0)
	{
		return TRUE;
	}
	if(nYear%4==0 && nYear%100!=0)
	{
		return TRUE;
	}
	return FALSE;
}

void CSetDateTimeDlg::AdjustTime()
{
	SYSTEMTIME tm;
	GetTime(&tm);
	if (tm.wMonth==4 || tm.wMonth==6 || tm.wMonth==9 || tm.wMonth==11)
	{
		if(tm.wDay==31)
			tm.wDay = 30;
	}

	if (tm.wMonth==2 && IsLeapYear(tm.wYear) && tm.wDay>29)
	{
		tm.wDay = 29;
	}

	if (tm.wMonth==2 && !IsLeapYear(tm.wYear) && tm.wDay>28)
	{
		tm.wDay = 28;
	}

	SetTime(&tm);
	m_pListDay->InvalidateRect(NULL);
}

void CSetDateTimeDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	if (plist == m_pListMonth || plist == m_pListYear || plist == m_pListDay)
	{
		AdjustTime();
	}

	SaveTime();
}


void CSetDateTimeDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"timezone"))	// found it
	{
		CDlgManager::GetInstance()->ShowDlg(CSetTimezoneDlg_ID, SOURCE_SETTING);
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}

LRESULT CSetDateTimeDlg::OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam)
{

	return __super::OnCommand(uCommand, wParam, lParam);
}


LRESULT CSetDateTimeDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_TIMEZONE_CHANGE)
	{
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		SetTime(&tm);
		return 0L;
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CSetDateTimeDlg::OnSBnClick(CWceUiSButton* pSButton, int status)
{
	if (pSButton->IsEqualName(L"hourtime_switch"))
	{
		sysutil::nss_get_instance()->ui_24hourtime = (status == WCEUI_SBUTTON_ON);
		::PostMessage(HWND_BROADCAST, NAVI_MSG_24HOURTIME_CHANGE, 0,
			(LPARAM)sysutil::nss_get_instance()->ui_24hourtime);
	}
	else if (pSButton->IsEqualName(L"autoset_switch"))
	{
		sysutil::nss_get_instance()->ui_time_autoset = (status == WCEUI_SBUTTON_ON);
		if (sysutil::nss_get_instance()->ui_time_autoset)
		{
			GetWindow()->PostMessage(UI_MSG_GPS_SETTIME, 0, 0);	//通知用GPS校正时间
		}

		OnAutosetChange();
	}
	else if (pSButton->IsEqualName(L"dst_switch"))
	{
		sysutil::nss_get_instance()->ui_time_dst = (status == WCEUI_SBUTTON_ON);

		SYSTEMTIME tm;
		GetSystemTime(&tm);
		ui_set_systemtime_DST(&tm, sysutil::nss_get_instance()->ui_time_dst);
	}
}

void CSetDateTimeDlg::OnAutosetChange()
{
	// 重新显示控件时，刷新下时间，因为时间有可能改变
	if (!sysutil::nss_get_instance()->ui_time_autoset)
	{
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		SetTime(&tm);
	}

	ShowDateTimeLayers(!sysutil::nss_get_instance()->ui_time_autoset);

	// 控件有隐藏显示动作，重新设置滚动范围
	int scroll_range = sysutil::nss_get_instance()->ui_time_autoset ? 0 : 240;
	CWceUiScrollLayer* pLayer = (CWceUiScrollLayer*)GetChildByName(L"frame");
	if (pLayer)
	{
		pLayer->SetScrollRange(0, scroll_range);

		if (!sysutil::nss_get_instance()->ui_time_autoset)	// 如果手动设置，把时间设置控件滚动到可见
		{
			pLayer->SetScrollPosAnim(scroll_range);
		}
	}
}

void CSetDateTimeDlg::ShowDateTimeLayers(BOOL bShow)
{
	m_pListYear->ShowLayer(bShow);
	m_pListMonth->ShowLayer(bShow);
	m_pListDay->ShowLayer(bShow);
	m_pListHour->ShowLayer(bShow);
	m_pListMinute->ShowLayer(bShow);
}

void CSetDateTimeDlg::OnDlgShow(BOOL bShow)
{
	__super::OnDlgShow(bShow);

	// 对话框从隐藏到显示时，刷新下时间，因为时间有可能改变
	if (!sysutil::nss_get_instance()->ui_time_autoset)	// 经时控件可见
	{
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		SetTime(&tm);
	}

	UpdateTimezone();
}

void CSetDateTimeDlg::UpdateTimezone()
{
	CWceUiLayer* player = GetLayerByName(L"timezone_value");
	if (player)
	{
		TCHAR resid[16];
		_stprintf(resid, L"tz_%02d", sysutil::nss_get_instance()->ui_timezone_id+1);
		player->SetTextResID(resid);
	}
}


/////////////////// CSetDateTimeListBox  //////////////////////////

static const int AUTO_SCROLL_FRMAE = 20;
CSetDateTimeListBox* CSetDateTimeListBox::m_pFocusListBox = NULL;

CSetDateTimeListBox::CSetDateTimeListBox()
: m_nTopIndex(-1)
{
	ShowScrollBar(FALSE);

	if (WceUiGetBitsPixel() == 16)
	{
		m_bmpSelected.SetBitmapFromRes(L"setting\\datetime\\datetime_select.565a.conf");
	}
	else
	{
		m_bmpSelected.SetBitmapFromRes(L"setting\\datetime\\datetime_select.32a.conf");
	}
}

CSetDateTimeListBox::~CSetDateTimeListBox(void)
{

}

int CSetDateTimeListBox::GetScrollDistance(DWORD speed, POINT point)
{
	int distance = CWceUiListBox::GetScrollDistance(speed, point);

	int pos = GetScrollPos();
	int topos = pos + distance;
	int item_height = GetItemHeight();
	topos = ((topos+item_height/2)/item_height)*item_height;

	return (topos - pos);
}

void CSetDateTimeListBox::OnScrollWindowEnd()
{
	int nAmount = 0;
	int offset = GetScrollPos() % GetItemHeight();
	if (offset != 0)	// 如果已经是0了，表示已经正好是一行了
	{
		if (offset > GetItemHeight()/2)
			nAmount = GetItemHeight() - offset;
		else
			nAmount = -offset;
	}

	if (nAmount != 0)
	{
		m_nScrollFrame = AUTO_SCROLL_FRMAE;
		AnimateScrollWindow(nAmount);
	}
	else	// 滚动结束，数据可能有变化，通知父窗口
	{
		int count = GetItemsPerpage()/2;	// 5 / 2 = 2
		int select = GetTopIndex() + count;
		if (select != GetCurSel())
		{
			SetCurSel(select);
			GetCmdReceiver()->OnCommand(WCEUI_CMD_LISTBOX_SELCHANGE, (WPARAM)this, (LPARAM)select);
			InvalidateRect(NULL);
		}
	}

	__super::OnScrollWindowEnd();
}


void CSetDateTimeListBox::OnScrollWindow()
{
	// not initialize, init it and return
	if (m_nTopIndex<0)
	{
		m_nTopIndex = GetTopIndex();
		return;
	}

	if (m_nTopIndex != GetTopIndex() && m_pFocusListBox==this)
	{
		//		PlaySound(L"\\residentflash2\\app\\ding.wav", NULL, SND_FILENAME|SND_ASYNC|SND_NODEFAULT);	
	}
	m_nTopIndex = GetTopIndex();
}

void CSetDateTimeListBox::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	// 中间的一行画一条幅，表示当前的选中项
	CRect rc_client;
	GetWindowRect(&rc_client);

	int count = GetItemsPerpage()/2;	// 5 / 2 = 2
	CRect dst(rc_client.left, rc_client.top+count*GetItemHeight(), rc_client.right, rc_client.top+(count+1)*GetItemHeight());
//	wceui::wceglDrawTexture(m_bmpSelected.GetBitmap(), dst);

	BLENDFUNCTION bf;
	memset(&bf, 0, sizeof(BLENDFUNCTION));
	bf.AlphaFormat = AC_SRC_ALPHA;
	pWndBitmap->AlphaBlend(dst.left, dst.top, m_bmpSelected.GetBitmap()->GetWidth(),
		m_bmpSelected.GetBitmap()->GetHeight(), m_bmpSelected.GetBitmap(), 0, 0, bf);

}

void CSetDateTimeListBox::OnLButtonDown(UINT nFlags, POINT point)
{
	m_pFocusListBox = this;

	int top = GetTopIndex();
	if (top<m_nItemsVisible-2 || top>m_nItemsVisible*2-2)
	{
		top = m_nItemsVisible + top%m_nItemsVisible;
		SetTopIndex(top);
		SetCurSel(top+GetItemsPerpage()/2);
	}

	__super::OnLButtonDown(nFlags, point);
}

void CSetDateTimeListBox::OnLButtonUp(UINT nFlags, POINT point)
{
	if (m_nItemClicked >= 0)	// valid value, notify parent to process the command
	{
		// 派生类自己来处理点击事件
		if (m_nItemClicked != GetCurSel())
		{
			m_nScrollFrame = AUTO_SCROLL_FRMAE;
			AnimateScrollWindow((m_nItemClicked-GetTopIndex()-GetItemsPerpage()/2)*GetItemHeight());
		}

		// 让基类忽略点击处理
		m_nItemClicked = -1;
	}

	__super::OnLButtonUp(nFlags, point);
}


int CSetDateTimeListBox::GetItemsPerpage()
{
	CRect rc;
	GetWindowRect(rc);

	return (rc.Height()/GetItemHeight());
}
