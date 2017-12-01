#include "stdafx.h"
#include "BalanceSlider.h"
#include "WceUiRealWnd.h"
#include "interface.h"

#define  DEFAULT_MAX_RANK_X  40
#define  DEFAULT_MAX_RANK_Y  40

CBalanceSlider::CBalanceSlider(void)
{
	m_bMouseDown = FALSE;
}

CBalanceSlider::~CBalanceSlider(void)
{
}

void CBalanceSlider::OnInitLayer()
{
	CRect rc;
	GetWindowRect(rc);
	int nx,ny;

	if (m_bmpPosSel.GetBitmap())
	{
		nx = m_bmpPosSel.GetBitmap()->GetWidth();
		ny = m_bmpPosSel.GetBitmap()->GetHeight();
	}

	POINT p1 = {rc.left+nx/2,rc.top+ny/2};
	POINT p2 = {rc.right-nx/2,rc.bottom-ny/2};
	SetRange(p1,p2);

	m_nPos.x = m_nMinPos.x + (m_nMaxPos.x-m_nMinPos.x)/2;
	m_nPos.y = m_nMinPos.y + (m_nMaxPos.y-m_nMinPos.y)/2;//置于中心

	CWceUiLayer::OnInitLayer();
}

void CBalanceSlider::SetProperties(PCWceUiXmlElem pCreateElem)
{
	// pos_sel_bmp
	LPCTSTR attr = pCreateElem->GetAttr(L"pos_sel_bmp");
	if (attr)
		m_bmpPosSel.SetBitmapFromRes(attr);

	CWceUiLayer::SetProperties(pCreateElem);
}

void CBalanceSlider::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	CRect rc_client;
	GetClientRect(&rc_client);

	//pos_sel bmp

	CWceUiGenericBitmap* ptexture = m_bmpPosSel.GetBitmap();
	if (ptexture)
	{
		int nx =m_nPos.x - ptexture->GetWidth()/2;
		int ny = m_nPos.y - ptexture->GetWidth()/2;
		if (ptexture)
		{
			CRect dst;
			CRect src;
			dst.SetRect(nx, ny, nx+ptexture->GetWidth(), ny+ptexture->GetHeight());

			BLENDFUNCTION bf;
			memset(&bf, 0, sizeof(BLENDFUNCTION));
			bf.AlphaFormat = AC_SRC_ALPHA;
			pWndBitmap->AlphaBlend(dst.left, dst.top, ptexture->GetWidth(), ptexture->GetHeight(), ptexture, 0, 0, bf);
		}
	}
}

void CBalanceSlider::OnLButtonDown(UINT nFlags, POINT point)
{
	m_bMouseDown = TRUE;
	SetCapture();

	if (CPoint(point) != CPoint(GetPosEx()))
	{
		SetPosEx(point);

		WORD dwParam = MAKEWORD(GetPos().x,GetPos().y);

		LPARAM lParam = (LPARAM)((SLIDER_MOUSE_DOWN << 16) | (MAKEWORD(GetPos().x,GetPos().y)));
		GetCmdReceiver()->OnCommand(UI_CMD_BSLIDER, (WPARAM)this, lParam);
	}
	CWceUiLayer::OnLButtonDown(nFlags, point);
}

void CBalanceSlider::OnLButtonUp(UINT nFlags, POINT point)
{
	if (m_bMouseDown)
	{
		m_bMouseDown = FALSE;
		ReleaseCapture();

		if (CPoint(point) != CPoint(GetPosEx()))
		{
			SetPosEx(point);
			WORD dwParam = MAKEWORD(GetPos().x,GetPos().y);
//			GetWindow()->PostMessage(WCEUI_WM_XH_SLIDER, (WPARAM)dwParam, (LPARAM)SLIDER_MOUSE_UP);

			LPARAM lParam = (LPARAM)((SLIDER_MOUSE_UP << 16) | (MAKEWORD(GetPos().x,GetPos().y)));
			GetCmdReceiver()->OnCommand(UI_CMD_BSLIDER, (WPARAM)this, lParam);

		}
	}

	CWceUiLayer::OnLButtonUp(nFlags, point);
}

void CBalanceSlider::OnMouseMove(UINT nFlags, POINT point)
{
	if ( m_bMouseDown)
	{
		if (CPoint(point) != CPoint(GetPosEx()))
		{
			SetPosEx(point);
			WORD dwParam = MAKEWORD(GetPos().x,GetPos().y);
			LPARAM lParam = (LPARAM)((SLIDER_MOUSE_MOVE << 16) | (MAKEWORD(GetPos().x,GetPos().y)));
			GetCmdReceiver()->OnCommand(UI_CMD_BSLIDER, (WPARAM)this, lParam);

		}
	}
	CWceUiLayer::OnMouseMove(nFlags, point);
}

void CBalanceSlider::OnLoseCapture()
{
	if (m_bMouseDown)
	{
		OnLButtonUp(0, CPoint(0,0));
	}
}

void CBalanceSlider::SetRange( POINT nMin, POINT nMax, BOOL bRedraw )
{
	if (nMin.x > nMax.x || nMin.y>nMax.y)
	{
		return;
	}
	m_nMinPos = nMin;
	m_nMaxPos = nMax;
	if (bRedraw)
	{
		InvalidateRect(NULL);
	}
}

void CBalanceSlider::GetRange( POINT& nMin, POINT& nMax ) const
{
	nMax = m_nMaxPos;
	nMin = m_nMinPos;
}

inline POINT CBalanceSlider::Pos2XY(POINT nPos)
{
	nPos.x = max( nPos.x,m_nMinPos.x);
	nPos.x = min( nPos.x,m_nMaxPos.x);

	nPos.y = max( nPos.y,m_nMinPos.y);
	nPos.y = min( nPos.y,m_nMaxPos.y);

	POINT	pt = {0,0};
	int nWidth = m_nMaxPos.x - m_nMinPos.x;
	int nHeight = m_nMaxPos.y - m_nMinPos.y;
	int nx = m_nMinPos.x + nWidth/2;
	int ny = m_nMinPos.y + nHeight/2;

	float fx = (float)((nPos.x - nx)*DEFAULT_MAX_RANK_X/(float)nWidth);
	pt.x = LONG( (fx>0)?(fx+0.5):(fx-0.5) );

	float fy = (float)((ny - nPos.y)*DEFAULT_MAX_RANK_Y/(float)nHeight);
	pt.y = LONG( (fy>0)?(fy+0.5):(fy-0.5) );

	return pt;
}

inline POINT CBalanceSlider::XY2Pos(POINT point)
{
	//(0,0)==>150,150
	int nWidth = m_nMaxPos.x - m_nMinPos.x;
	int nHeight = m_nMaxPos.y - m_nMinPos.y;
	int nx = m_nMinPos.x + nWidth/2;
	int ny = m_nMinPos.y + nHeight/2;

	POINT pt = {nx,ny};
	float fx =  (float)point.x*nWidth/DEFAULT_MAX_RANK_X;
	pt.x = LONG( (float)nx + ( (fx>=0)?(fx+0.5):(fx-0.5) ) );//让加不让减

	float fy = (float)point.y*nHeight/DEFAULT_MAX_RANK_Y;
	pt.y = LONG( (float)ny - ( (fy>0)?(fy+0.5):(fy-0.5) ) );

	return pt;
}

inline void CBalanceSlider::SetPosEx(POINT nPos)
{
	int nx = 0;
	int ny = 0;

	if (m_bmpPosSel.GetBitmap())
	{
		nx = m_bmpPosSel.GetBitmap()->GetWidth();
		ny = m_bmpPosSel.GetBitmap()->GetHeight();
	}

	if (nPos.x < (m_nMinPos.x))
		nPos.x = m_nMinPos.x;
	else if (nPos.x>(m_nMaxPos.x))
		nPos.x =m_nMaxPos.x;

	if (nPos.y<(m_nMinPos.y) )
		nPos.y = m_nMinPos.y;
	else if (nPos.y>(m_nMaxPos.y))
		nPos.y = m_nMaxPos.y;

	m_nPos = nPos;
	InvalidateRect(NULL);
}

inline POINT CBalanceSlider::GetPosEx( ) const
{
	return m_nPos;
}

void CBalanceSlider::SetPos( POINT nPos )
{
	SetPosEx(XY2Pos(nPos));
}


POINT CBalanceSlider::GetPos( )
{
	return Pos2XY(m_nPos);
}
