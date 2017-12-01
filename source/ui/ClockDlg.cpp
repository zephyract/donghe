#include "stdafx.h"
#include "ClockDlg.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "sysUtil.h"
#include "interface.h"
#include "protocol.h"
#include "tzUtil.h"
#include "config.h"

CClockDlg::CClockDlg(void)
{
	m_pLastDlg = NULL;
}

CClockDlg::~CClockDlg(void)
{

}

void CClockDlg::OnInitDialog()
{
	__super::OnInitDialog();

	if (m_pLayerPlay)
	{
		m_pLayerPlay->ShowLayer(TRUE);
	}

	SetClockMode(sysutil::nss_get_instance()->ui_clock_mode);

	// 如果不带DVD,eject按钮为disable状态
	if (!config::get_config_functions()->is_support(L"dvd"))
	{
		CWceUiButton* peject = (CWceUiButton*)GetLayerByName(L"eject");
		if (peject)
		{
			peject->EnableButton(FALSE);
		}
	}
}

void CClockDlg::OnTimer(WORD wIDEvent)
{
	__super::OnTimer(wIDEvent);
}

void CClockDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		DLG_INFO *pdi = CDlgManager::GetInstance()->GetLastDlgInfo();
		if (pdi && pdi->dlg_id != CClockDlg_ID)
		{
			m_pLastDlg = CDlgManager::GetInstance()->GetLastDlgInfo();
		}
	}

	__super::OnDlgShow(bShow);
}


void CClockDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		if (m_pLastDlg)
		{
			CDlgManager::GetInstance()->ShowDlg(m_pLastDlg->dlg_id, m_pLastDlg->source_id);
		}
		else
		{
			CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
		}
		//CDlgManager::GetInstance()->GotoLastDlg();
	}
	else if (pButton->IsEqualName(L"mode"))
	{
		sysutil::nss_get_instance()->ui_clock_mode++;
		sysutil::nss_get_instance()->ui_clock_mode %= 3;	// 取值0,1,2
		SetClockMode(sysutil::nss_get_instance()->ui_clock_mode);
	}
	else if (pButton->IsEqualName(L"prev"))
	{
		GetWindow()->PostMessage(UI_MSG_MCU_KEY_COMMAND, (WPARAM)T_PREV, 0);
	}
	else if (pButton->IsEqualName(L"next"))
	{
		GetWindow()->PostMessage(UI_MSG_MCU_KEY_COMMAND, (WPARAM)T_NEXT, 0);
	}
	else if (pButton->IsEqualName(L"play"))
	{
		GetWindow()->PostMessage(UI_MSG_MCU_KEY_COMMAND, (WPARAM)T_PLAY, 0);
	}
	else if (pButton->IsEqualName(L"eject"))
	{
		GetWindow()->PostMessage(UI_MSG_MCU_KEY_COMMAND, (WPARAM)K_EJECT, 0);	// 参数0刚好表示出碟
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}

void CClockDlg::SetClockMode(int mode)
{
	CClockLayer* pclock = (CClockLayer*)GetChildByName(L"clock");
	if (pclock)
	{
		if (mode == 0)
		{
			if (WceUiGetBitsPixel() == 16)
			{
				pclock->SetResID(L"clock\\res1\\hour.32a.bmp", L"clock\\res1\\minute.32a.bmp", 
					L"clock\\res1\\second.32a.bmp", L"clock\\res1\\bk.565a.conf");
			}
			else
			{
				pclock->SetResID(L"clock\\res1\\hour.32a.bmp", L"clock\\res1\\minute.32a.bmp", 
					L"clock\\res1\\second.32a.bmp", L"clock\\res1\\bk.32a.conf");
			}
		}
		else if (mode == 1)
		{
			if (WceUiGetBitsPixel() == 16)
			{
				pclock->SetResID(L"clock\\res2\\hour.32a.bmp", L"clock\\res2\\minute.32a.bmp", 
					L"clock\\res2\\second.32a.bmp", L"clock\\res2\\bk.565a.conf");
			}
			else
			{
				pclock->SetResID(L"clock\\res2\\hour.32a.bmp", L"clock\\res2\\minute.32a.bmp", 
					L"clock\\res2\\second.32a.bmp", L"clock\\res2\\bk.32a.conf");
			}
		}
		else
		{
			if (WceUiGetBitsPixel() == 16)
			{
				pclock->SetResID(L"clock\\res3\\hour.32a.bmp", L"clock\\res3\\minute.32a.bmp", 
					L"clock\\res3\\second.32a.bmp", L"clock\\res3\\bk.565a.conf");
			}
			else
			{
				pclock->SetResID(L"clock\\res3\\hour.32a.bmp", L"clock\\res3\\minute.32a.bmp", 
					L"clock\\res3\\second.32a.bmp", L"clock\\res3\\bk.32a.conf");
			}
		}
	}
}

void CClockDlg::OnAutoFullscreen()
{
	// 只有在播放状态，并且鼠标未被按下的情况下才自动全屏播放
	if (!m_bFullScreen && GetAsyncKeyState(VK_LBUTTON) >= 0)
	{
		SetFullScreen(TRUE);
	}
}


////////////////////////////////////////////////////////////////////
//                         CClockLayer
////////////////////////////////////////////////////////////////////
CClockLayer::CClockLayer()
{
	m_fHourAngle = 0;
	m_fMinuteAngle = 0;
	m_fSecondAngle = 0;
}

CClockLayer::~CClockLayer()
{

}

void CClockLayer::SetProperties(PCWceUiXmlElem pCreateElem)
{
	__super::SetProperties(pCreateElem);

	LPCTSTR attr_hour = pCreateElem->GetAttr(L"hour");
	LPCTSTR attr_minute = pCreateElem->GetAttr(L"minute");
	LPCTSTR attr_sec = pCreateElem->GetAttr(L"second");
	LPCTSTR attr_bk = pCreateElem->GetAttr(L"bk");

	SetResID(attr_hour, attr_minute, attr_sec, attr_bk);
}

void CClockLayer::SetResID(LPCTSTR hour, LPCTSTR min, LPCTSTR sec, LPCTSTR bk)
{
	if (hour)
	{
		CString str;
		str.Format(L"%s\\%s\\res\\bitmap\\%s", tzutil::GetAppPath(), ui_get_res_file(), hour);
		m_rbmpHour.LoadFile(str);
	}

	if (min)
	{
		CString str;
		str.Format(L"%s\\%s\\res\\bitmap\\%s", tzutil::GetAppPath(), ui_get_res_file(), min);
		m_rbmpMinute.LoadFile(str);
	}

	if (sec)
	{
		CString str;
		str.Format(L"%s\\%s\\res\\bitmap\\%s", tzutil::GetAppPath(), ui_get_res_file(), sec);
		m_rbmpSecond.LoadFile(str);
	}

	if (bk)  m_txtBK.SetBitmapFromRes(bk);
}

void CClockLayer::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_CLOCK)
	{
		OnTimerClock();
		InvalidateRect();
	}
	__super::OnTimer(wIDEvent);
}

void CClockLayer::OnInitLayer()
{
	__super::OnInitLayer();
	SetTimer(TIMER_ID_CLOCK, 1000);
	OnTimerClock();
}

void CClockLayer::OnTimerClock()
{
	static WORD millseconds = 0;
	static WORD seconds = 0;

	SYSTEMTIME tm;
	GetLocalTime( &tm );

	if (seconds != tm.wSecond)
	{
		seconds = tm.wSecond;
		millseconds = 0;
	}
	else
	{
		millseconds += 100;
		millseconds = min(millseconds, 1000);
	}

	m_fHourAngle = -((float)(tm.wHour % 12) + (float)tm.wMinute/(float)60) * (float)360 / (float)12;
	m_fMinuteAngle = -((float)tm.wMinute + (float)tm.wSecond/(float)60) * (float)360 / (float)60;
	m_fSecondAngle = -((float)tm.wSecond + (float)millseconds/(float)1000)* (float)360 / (float)60;
}

void CClockLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	DrawClock(pWndBitmap);
}

void CClockLayer::DrawClock(CWceUiGenericBitmap* pWndBitmap)
{
	// 指针与背景的偏移量
	const int offsetX = 44;
	const int offsetY = 44;

	BLENDFUNCTION bf;
	memset(&bf, 0, sizeof(BLENDFUNCTION));
	bf.AlphaFormat = AC_SRC_ALPHA;

	CRect rc;
	GetWindowRect(rc);
 	pWndBitmap->AlphaBlend(rc.left, rc.top, m_txtBK.GetBitmap()->GetWidth(), m_txtBK.GetBitmap()->GetHeight(),
 		m_txtBK.GetBitmap(), 0, 0, bf);

	CRect rc_dst(rc.left+offsetX, rc.top+offsetY, 
		rc.left+offsetX+m_rbmpSecond.GetBitmap()->GetWidth(),
		rc.top+offsetY+m_rbmpSecond.GetBitmap()->GetHeight());

	int Xc = (rc_dst.left + rc_dst.right) / 2;
	int Yc = (rc_dst.top + rc_dst.bottom) / 2;
	float x = (float)(WceUiGetScreenWidth()/2 - Xc);
	float y = (float)(Yc - WceUiGetScreenHeight()/2);

 	// hour
	m_rbmpHour.Rotate(m_fHourAngle, m_rbmpHour.GetWidth()/2, m_rbmpHour.GetHeight()/2, 1.0f, 1.0f);
	pWndBitmap->AlphaBlend(rc_dst.left, rc_dst.top, 
		m_rbmpHour.GetBitmap()->GetWidth(), m_rbmpHour.GetBitmap()->GetHeight(),
		m_rbmpHour.GetBitmap(), 0, 0, bf);
 
 	// minute
	m_rbmpMinute.Rotate(m_fMinuteAngle, m_rbmpMinute.GetWidth()/2, m_rbmpMinute.GetHeight()/2, 1.0f, 1.0f);
	pWndBitmap->AlphaBlend(rc_dst.left, rc_dst.top, 
		m_rbmpMinute.GetBitmap()->GetWidth(), m_rbmpMinute.GetBitmap()->GetHeight(),
		m_rbmpMinute.GetBitmap(), 0, 0, bf);
 
 	// second
	m_rbmpSecond.Rotate(m_fSecondAngle, m_rbmpSecond.GetWidth()/2, m_rbmpSecond.GetHeight()/2, 1.0f, 1.0f);
	pWndBitmap->AlphaBlend(rc_dst.left, rc_dst.top, 
		m_rbmpSecond.GetBitmap()->GetWidth(), m_rbmpSecond.GetBitmap()->GetHeight(),
		m_rbmpSecond.GetBitmap(), 0, 0, bf);


}




////////////////////////////////////////////////////////////////////
//                         CDigitalClockLayer
////////////////////////////////////////////////////////////////////
CDigitalClockLayer::CDigitalClockLayer()
{
	m_bShowColcon = TRUE;
}

CDigitalClockLayer::~CDigitalClockLayer()
{

}

void CDigitalClockLayer::SetProperties(PCWceUiXmlElem pCreateElem)
{
	__super::SetProperties(pCreateElem);

	LPCTSTR attr = pCreateElem->GetAttr(L"res_path");
	if (!attr) attr = L"";

	LPCTSTR pixel_format = NULL;
	if (WceUiGetBitsPixel() == 16)
	{
		pixel_format = L"565a";
	}
	else
	{
		pixel_format = L"32a";
	}

	TCHAR res[128];
	for (int i=0; i<10; i++)
	{
		_stprintf(res, L"%s\\%d.%s.conf", attr, i, pixel_format);
		m_txtNumber[i].SetBitmapFromRes(res);
	}

	_stprintf(res, L"%s\\colon.%s.conf", attr, pixel_format);
	m_txtColon.SetBitmapFromRes(res);

	_stprintf(res, L"%s\\am.%s.conf", attr, pixel_format);
	m_txtAM.SetBitmapFromRes(res);

	_stprintf(res, L"%s\\pm.%s.conf", attr, pixel_format);
	m_txtPM.SetBitmapFromRes(res);
}

void CDigitalClockLayer::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_CLOCK)
	{
		m_bShowColcon = !m_bShowColcon;
		InvalidateRect();
	}
	__super::OnTimer(wIDEvent);
}

void CDigitalClockLayer::OnInitLayer()
{
	__super::OnInitLayer();
	SetTimer(TIMER_ID_CLOCK, 500);
}


void CDigitalClockLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	DrawClock(pWndBitmap);
}

void CDigitalClockLayer::DrawClock(CWceUiGenericBitmap* pWndBitmap)
{
	SYSTEMTIME tm;
	GetLocalTime( &tm );
	BOOL isAM = (tm.wHour < 12);
	WORD hour = tm.wHour;
	if (!sysutil::nss_get_instance()->ui_24hourtime)
	{
		hour = tm.wHour % 12;
		if (hour == 0)	// 12小时制没有0点,用12点表示
		{
			hour = 12;
		}
	}

	CRect rc;
	GetWindowRect(rc);
	int offset_x[6] = {0, 27, 54, 67, 94, 126}; // hr, hr, colon, min, min, am/pm
	int offset_y = 0;
	int width[6] = {26, 26, 12, 26, 26, 26};
	int height = 43;
	CRect rcLoc[6];
	for (int i=0; i<6; i++)
	{
		rcLoc[i].SetRect(offset_x[i], offset_y, offset_x[i]+width[i], offset_y+height);
		rcLoc[i].OffsetRect(rc.left, rc.top);
	}

	BLENDFUNCTION bf;
	memset(&bf, 0, sizeof(BLENDFUNCTION));
	bf.AlphaFormat = AC_SRC_ALPHA;

	// hour
	if (hour/10 >= 1)
	{
		pWndBitmap->AlphaBlend(rcLoc[0].left, rcLoc[0].top, rcLoc[0].Width(), rcLoc[0].Height(),
			m_txtNumber[hour/10].GetBitmap(), 0, 0, bf);
	}
	pWndBitmap->AlphaBlend(rcLoc[1].left, rcLoc[1].top, rcLoc[1].Width(), rcLoc[1].Height(),
		m_txtNumber[hour%10].GetBitmap(), 0, 0, bf);

 	// colon
 	if (m_bShowColcon)
 	{
		pWndBitmap->AlphaBlend(rcLoc[2].left, rcLoc[2].top, rcLoc[2].Width(), rcLoc[2].Height(),
			m_txtColon.GetBitmap(), 0, 0, bf);
 	}
 	// minute
	pWndBitmap->AlphaBlend(rcLoc[3].left, rcLoc[3].top, rcLoc[3].Width(), rcLoc[3].Height(),
		m_txtNumber[tm.wMinute/10].GetBitmap(), 0, 0, bf);
	pWndBitmap->AlphaBlend(rcLoc[4].left, rcLoc[4].top, rcLoc[4].Width(), rcLoc[4].Height(),
		m_txtNumber[tm.wMinute%10].GetBitmap(), 0, 0, bf);

 	// am/pm
 	if (!sysutil::nss_get_instance()->ui_24hourtime)
 	{
		pWndBitmap->AlphaBlend(rcLoc[5].left, rcLoc[5].top, rcLoc[5].Width(), rcLoc[5].Height(),
			(tm.wHour < 12) ? m_txtAM.GetBitmap() : m_txtPM.GetBitmap(), 0, 0, bf);
 	}
}