#include "stdafx.h"
#include "Score.h"

const int   nPointCount  = 4;
const int   frmwid   =1;
CScore::CScore(void)
{
	m_bNoDraw       =FALSE;
	m_pScoreStatic  = NULL;
	m_pLevelStatic  = NULL;
	iPromptLeft     = 0;
	iPromptTop      = 0;
	iBlockWD        = 0;	
	iBlockHT        = 0;
	for (int i =0 ;i<4; i++)
	{
		ptPromt[i].x  = -1;
		ptPromt[i].y  = -1;
	}
	memset(m_score.strlevel,0,sizeof(m_score.strlevel));
	memset(m_score.strscore,0,sizeof(m_score.strscore));

}

CScore::~CScore(void)
{
	//     delete m_pblockbmp;
	//     m_pblockbmp = NULL;
}
void CScore::OnDraw()
{
//by lzy//	CWceUiLayer::OnDraw();

	CRect rc_client;
	GetWindowRect(rc_client);
	if (ptPromt[0].x != -1)
	{
		for(int i=0;i<nPointCount;i++)
		{
			int x = rc_client.left + ptPromt[i].x*iBlockWD/10+iPromptLeft+frmwid;
			int y = ptPromt[i].y*iBlockHT+iPromptTop+frmwid;
			CRect rc(x, y, x+iBlockWD, y+iBlockHT);
//by lzy//			wceui::wceglDrawTexture(m_pblockbmp.GetBitmap(), rc);
			//             pWndBitmap->AlphaBlend(,
			//                 ,iBlockWD,iBlockHT,
			//                 m_pblockbmp,bmpinfo.bmWidth - iBlockWD,0,bf);

		}
	}
	m_pLevelStatic->SetText(m_score.strlevel);
	m_pScoreStatic->SetText(m_score.strscore);
}
void CScore::SetProperties(PCWceUiXmlElem pCreateElem)
{

	CWceUiLayer::SetProperties(pCreateElem);
	LPCTSTR pX = pCreateElem->GetAttr(L"x");
	LPCTSTR pY = pCreateElem->GetAttr(L"y");
	LPCTSTR pWidth  = pCreateElem->GetAttr(L"blockwidth");
	LPCTSTR pHeight = pCreateElem->GetAttr(L"blockheight");
	LPCTSTR pblockbmp = pCreateElem->GetAttr(L"blockbmp");

	if (NULL != pX)
	{
		iPromptLeft = _ttoi(pX);
	}
	if (NULL != pY)
	{
		iPromptTop = _ttoi(pY);
	}
	if (NULL != pWidth)
	{
		iBlockWD = _ttoi(pWidth);
	}
	if (NULL != pHeight)
	{
		iBlockHT = _ttoi(pHeight);
	}
	if (NULL != pblockbmp)
	{
		m_pblockbmp.SetBitmapFromRes(pblockbmp);
	}
}

void CScore::OnInitLayer()
{
	CWceUiLayer::OnInitLayer();
	m_pScoreStatic =(CWceUiLayer *)GetChildByName(L"score1");
	m_pLevelStatic =(CWceUiLayer *)GetChildByName(L"level");

}

void CScore::SetPtPromt(POINT *pt)
{
	if (IsSamePoints(pt))
	{
		return;
	}       
	for (int i = 0 ;i<nPointCount ;i++,pt++)
	{

		ptPromt[i].x = (*pt).x;
		ptPromt[i].y = (*pt).y;

	}
	InvalidateRect(NULL);
}

void CScore::SetScore( TCHAR* strlevel ,TCHAR* strscore )
{
	if (_tcscmp(m_score.strlevel,strlevel)||_tcscmp(m_score.strscore,strscore))
	{
		_tcscpy(m_score.strlevel,strlevel);
		_tcscpy(m_score.strscore,strscore);
		InvalidateRect(NULL);
	}

}

BOOL CScore::IsSamePoints( POINT *pt )
{
	for (int i = 0 ;i<nPointCount ;i++,pt++)
	{
		if (ptPromt[i].x != (*pt).x || ptPromt[i].y != (*pt).y)
		{
			return FALSE;
		}
	}
	return TRUE;
}