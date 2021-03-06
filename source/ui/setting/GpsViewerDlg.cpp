#include "stdafx.h"
#include "GpsViewerDlg.h"
#include "tzUtil.h"
#include "interface.h"
#include "GpsProxy.h"
#include "MainWnd.h"
#include "WceUiLoadString.h"
#include "WceUi32Bitmap.h"
CGpsViewerDlg::CGpsViewerDlg(void)
{
}

CGpsViewerDlg::~CGpsViewerDlg(void)
{
}

void CGpsViewerDlg::OnInitDialog()
{
	__super::OnInitDialog();
	
	RegisterMsg(UI_MSG_GPS_STATUS_UPDATE);
	InitLayer();
}


void CGpsViewerDlg::InitLayer()
{
	m_platitude = GetChildByName(L"latitude_title");
	m_plongitude = GetChildByName(L"longitude_title");
	m_paltitude = GetChildByName(L"altitude_title");
	m_pspeed = GetChildByName(L"speed_title");
	m_putc = GetChildByName(L"utc_title");
}

void CGpsViewerDlg::RefreshLayer()
{
	CGpsProxy* pgps = CMainWnd::GetInstance()->GetGpsProxy();
	GPSInfo* pinfo = pgps->GetGpsInfo();

	//Position
	TCHAR tcTime[128];	
	TCHAR tcInfo[128];
	CWceUiLoadString str;
	str.SetTextResID(L"GPS_LATITUDE");
	swprintf(tcInfo,L"%s %0.6f",str.GetString(),pinfo->Lat);
	m_platitude->SetText(tcInfo);

	memset(tcInfo,0,sizeof(TCHAR)*128);
	str.SetTextResID(L"GPS_LONGITUDE");
	swprintf(tcInfo,L"%s %0.6f",str.GetString(),pinfo->Lon);
	m_plongitude->SetText(tcInfo);

	memset(tcInfo,0,sizeof(TCHAR)*128);
	str.SetTextResID(L"GPS_ALTITUDE");
	swprintf(tcInfo,L"%s %0.6f M",str.GetString(),pinfo->Alt);
	m_paltitude->SetText(tcInfo);


	memset(tcInfo,0,sizeof(TCHAR)*128);
	str.SetTextResID(L"GPS_SPEED");
	swprintf(tcInfo,L"%s %0.1f km/h",str.GetString(),pinfo->Speed);
	m_pspeed->SetText(tcInfo);


	//Display Date info
	int sec;
	sec = (int)pinfo->sec;	
	memset(tcTime,0,sizeof(TCHAR)*128);
	str.SetTextResID(L"GPS_UTC");
	swprintf(tcTime,L"%s %.4d/%.2d/%.2d %.2d:%.2d:%.2d",str.GetString(),
		pinfo->year,pinfo->mon,pinfo->day,pinfo->hour,pinfo->min,sec);
	m_putc->SetText(tcTime);
}

LRESULT CGpsViewerDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_GPS_STATUS_UPDATE)
	{
		RefreshLayer();
		InvalidateRect(NULL);
		return 0L;
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CGpsViewerDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"reset"))
	{
		CMainWnd::GetInstance()->GetGpsProxy()->ResetGps();
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}


///////////////////////  CGpsLayer  ////////////////////////////
CGpsSignalLayer::CGpsSignalLayer(void)
{

}
CGpsSignalLayer::~CGpsSignalLayer(void)
{

}
void CGpsSignalLayer::OnInitLayer()
{
}

void CGpsSignalLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	CGpsProxy* pgps = CMainWnd::GetInstance()->GetGpsProxy();
	GpsStatus* pstatus = pgps->GetGpsStatus();

	RECT rect;
	GetWindowRect(&rect);

// 	BOOL is_fix = FALSE;
// 	for(int i=0;i<pstatus->nSVCnt && i<GPS_NUM_CH;i++)
// 	{
// 		if(pstatus->svInfo[i].Fix)
// 		{
// 			is_fix = TRUE;
// 			break;
// 		}
// 	}

	int id = 0;
	int signal = 0;
	for(int i=0; i<GPS_MAX_SIGNAL_COUNT; i++)
	{
		if((pstatus->svInfo[i].SVid >0) && (pstatus->svInfo[i].SNR >0))
		{
			id = pstatus->svInfo[i].SVid;
			signal = pstatus->svInfo[i].SNR;
		}
		else
		{
			id = 0;
			signal = 0;
		}

		DrawSNR(pWndBitmap, i, /*is_fix*/pgps->IsFixed(), id, signal);
	}
}

void CGpsSignalLayer::DrawSNR(CWceUiGenericBitmap* pWndBitmap,int index, BOOL is_fix,int id, int signal)
{
	CRect rc;
	GetWindowRect(&rc);
	int nItemWidth = rc.Width() / GPS_MAX_SIGNAL_COUNT;

	HFONT hfont = GetStyle()->GetFontCtrl()->GetFont();
	ASSERT(NULL != hfont);

	LPBYTE pbb;
	HBITMAP hbmp = pWndBitmap->GetHBitmap(pbb);
	CDC dc;
	dc.CreateCompatibleDC(NULL);

// 	GetDC(NULL);
	HBITMAP hbmpOld;

	dc.SelectObject(hfont);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(GetStyle()->GetTextColor());
	
	hbmpOld = (HBITMAP)dc.SelectObject(hbmp);

	CRect rcItem(rc);
	rcItem.left = rc.left + index*nItemWidth;
	rcItem.right = rcItem.left + nItemWidth;

	const int TEXT_HEIGHT = 25;
	// 输出强度数字
	TCHAR buffer[16];
	CRect rcSignal(rcItem);		// 上面显示信号的强度
	rcSignal.bottom = rcSignal.top+TEXT_HEIGHT;
	_stprintf(buffer, L"%d", signal);
	dc.DrawText(buffer, rcSignal, DT_CENTER|DT_SINGLELINE);//改用GDI，add by wll
	// 输出ID
	CRect rcID(rcItem);			// 上面显示卫星的ID
	rcID.top = rcID.bottom - TEXT_HEIGHT;
	_stprintf(buffer, L"%d", id);
	dc.DrawText(buffer, rcID, DT_CENTER|DT_SINGLELINE);//改用GDI，add by wll

	dc.SelectObject(hbmpOld);
	dc.DeleteDC();

	// 输出柱状图
	CRect rcPillar(rcItem);		// 信号强度矩形
	rcPillar.top += TEXT_HEIGHT;
	rcPillar.bottom -= TEXT_HEIGHT;
	rcPillar.left += 0;	// 左右空一点
	rcPillar.right -= 6;

//by lzy//	wceui::wceglFillSolidRect(rcPillar, RGB(0,100,190), 100);	// 先画背景
	
	pWndBitmap->FillColor(RGB(0,100,190),rcPillar);
	// 根据强度再调整pillar的高度
	int height = signal * rcPillar.Height() / 60;	// 信号最强60
	rcPillar.top = rcPillar.bottom - height;

	COLORREF cr = is_fix ? RGB(0,255,0) : RGB(0,0,255);
//by lzy//	wceui::wceglFillSolidRect(rcPillar, cr);
	pWndBitmap->FillColor(cr,rcPillar);

	
}

///////////////////////////  CGpsSatelliteLayer  /////////////////////////
CGpsSatelliteLayer::CGpsSatelliteLayer(void)
{


//by lzy//	m_pTexSatellite = wceui::WceglGetTexture(L"setting\\gpsviewer\\satellite.bmp");
// 	if (WceUiGetBitsPixel() == 16 )
// 	{
// 		if (m_pTexSatellite == NULL)
// 		{
// 			RETAILMSG(1, (L"[navi car] Load setting\\gpsviewer\\satellite.565a.conf fail.\n"));
// 		}
// 		
// 	}
// 	else
// 	{
// 		if (m_pTexSatellite == NULL)
// 		{
// 			RETAILMSG(1, (L"[navi car] Load setting\\gpsviewer\\satellite.32a.conf fail.\n"));
// 		}
// 	}

	if (WceUiGetBitsPixel() == 16)
	{

		m_pSatelliteloadBmp.SetBitmapFromRes(L"setting\\gpsviewer\\satellite.565a.conf");

		if (m_pSatelliteloadBmp.GetBitmap() == NULL)
		{
			RETAILMSG(1, (L"[navi car] Load setting\\gpsviewer\\satellite.565a.conf fail.\n"));
		}
	}	

	else
	{
		m_pSatelliteloadBmp.SetBitmapFromRes(L"setting\\gpsviewer\\satellite.32a.conf");
		if (m_pSatelliteloadBmp.GetBitmap() == NULL)
		{
			RETAILMSG(1, (L"[navi car] Load setting\\gpsviewer\\satellite.32a.conf fail.\n"));
		}

	}
}

CGpsSatelliteLayer::~CGpsSatelliteLayer(void)
{

}

void CGpsSatelliteLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	if (NULL == m_pSatelliteloadBmp.GetBitmap())
	{
		return;		// 图像加载失败
	}

	CRect rect;
	GetWindowRect(&rect);

	//Center
	int CenterX = rect.left+rect.Width()/2;
	int CenterY = rect.top+rect.Height()/2;
	double scaleX	= (rect.Width()/2)/90.0;
	double scaleY	= (rect.Height()/2)/90.0;

	GpsStatus* pstatus = CMainWnd::GetInstance()->GetGpsProxy()->GetGpsStatus();

	// 6行7列
	const int COL_SATELLITE = 7;
	const int ROW_SATELLITE = 6;
	int width = m_pSatelliteloadBmp.GetBitmap()->GetWidth() / COL_SATELLITE;
	int height =m_pSatelliteloadBmp.GetBitmap()->GetHeight() / ROW_SATELLITE;

//  	int Satellitewidth = ;
//  	int Satelliteheight;
	
	wstring strID;
	int X, Y;

	for(int i=0; i<pstatus->nSVCnt && i<GPS_MAX_SIGNAL_COUNT; i++)
	{
		if(pstatus->svInfo[i].SVid > 0 && pstatus->svInfo[i].SNR > 0)
		{
			X = CenterX + (int)(sin(pstatus->svInfo[i].azimuth/180.0 * 3.14)*(90-pstatus->svInfo[i].elv) * 0.8*scaleX);
			Y = CenterY - (int)(cos(pstatus->svInfo[i].azimuth/180.0 * 3.14)*(90-pstatus->svInfo[i].elv) * 0.8*scaleY);

			if(rect.PtInRect(CPoint(X,Y)))
			{
				CRect dst(0,0,width,height);
				dst.OffsetRect(X-width/2, Y-height/2);

				CRect src(0,0,width,height);
				int id = pstatus->svInfo[i].SVid - 1;	// 卫星编号转为图像的编号
				if (id>=0 && id<=41)	// 保证不输出不存在的图像
				{
					src.OffsetRect(id%COL_SATELLITE*width, id/COL_SATELLITE*height);
 					//wceui::wceglDrawTexture(m_pTexSatellite, &dst, &src);


					if (m_pSatelliteloadBmp.GetBitmap()->HasPerPixelAlpha())
					{
						BLENDFUNCTION bf;
						memset(&bf, 0, sizeof(BLENDFUNCTION));
						bf.AlphaFormat = AC_SRC_ALPHA;
						pWndBitmap->AlphaBlend(dst.left, dst.top, width, height,m_pSatelliteloadBmp.GetBitmap(),src.left, src.top,bf);
					}
					else
					{
						pWndBitmap->BitBlt(dst.left, dst.top, width, height, 
							m_pSatelliteloadBmp.GetBitmap(), src.left, src.top, SRCCOPY);

					}

				}
			}
		}
	}
}
