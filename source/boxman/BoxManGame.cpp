#include "StdAfx.h"
#include "BoxManGame.h"
#include "WceUiRealWnd.h"
#include "WceUiDlg.h"
#include "..\..\project\\resource.h"
#include "uibase.h"

#define	   IMAGES_PATH					TEXT("")
#define    _USE_GDI_GPE
#define	   _USE_MAKE_SOUND
#define	   _USE_GAME_BK_TRANSPARENT
#define	   USE_UI_IMAGE_CFG
static RECT g_rcDrawGameArea ={11,10,332,262};
CBoxManGame::CBoxManGame(void)
{

}

CBoxManGame::~CBoxManGame(void)
{
}

void CBoxManGame::SetProperties( PCWceUiXmlElem pCreateElem )
{
    ASSERT(NULL != pCreateElem);
    CWceUiLayer::SetProperties(pCreateElem);
    LPCTSTR pGamerc                 = pCreateElem->GetAttr(L"gamerc");
    LPCTSTR pBlocksz                = pCreateElem->GetAttr(L"blocksz");
    LPCTSTR pUseres                  = pCreateElem->GetAttr(L"useres");

    LPCTSTR pYellowboxbmp           = pCreateElem->GetAttr(L"bmpyellowbox");
    LPCTSTR pRedboxbmp              = pCreateElem->GetAttr(L"bmpredbox");
    LPCTSTR pBluewallbmp            = pCreateElem->GetAttr(L"bmpbluewall");
    LPCTSTR pWhitewallbmp           = pCreateElem->GetAttr(L"bmpwhitewall");
    LPCTSTR pManbmp                 = pCreateElem->GetAttr(L"bmpman");
    LPCTSTR pMapballbmp             = pCreateElem->GetAttr(L"bmpmapball");


    RECT rcGameArea = {0};
    SIZE szTemp ={0};
    if (NULL != pGamerc)
    {
        rcGameArea = GetRectByStr(pGamerc);
    }
    if (NULL != pBlocksz)
    {
        szTemp = GetSizeByStr(pBlocksz);
        if (szTemp.cx >0)
        {
            iBlockWidth = szTemp.cx;
        }
        if (szTemp.cy >0)
        {
            iBlockHeight =szTemp.cy;
        }
        rcGameArea.right = rcGameArea.left + iBlockWidth*M_TAB_WIDTH;
        rcGameArea.bottom =rcGameArea.top  + iBlockHeight*M_TAB_HEIGHT;
    }
    g_rcDrawGameArea  =rcGameArea;
    DWORD dwuser = 0;
    if (NULL != pUseres)
    {
        dwuser = _ttoi(pUseres);
    }


    if (NULL != pYellowboxbmp)
    {
		m_pyellowboxbmp.SetBitmapFromRes(pYellowboxbmp);
    }
    if (NULL != pRedboxbmp)
    {
		m_predboxbmp.SetBitmapFromRes(pRedboxbmp);
    }

    if (NULL != pBluewallbmp)
    {
		m_pbluebmp.SetBitmapFromRes(pBluewallbmp);
    }
    if (NULL != pWhitewallbmp)
    {
		m_pwhitewallbmp.SetBitmapFromRes(pWhitewallbmp);
    }

    if (NULL != pManbmp)
    {
		m_pmanbmp.SetBitmapFromRes(pManbmp);
    }
    if (NULL != pMapballbmp)
    {
		m_pmapballbmp.SetBitmapFromRes(pMapballbmp);
    }

}

void CBoxManGame::OnInitLayer()
{
    m_boxMan.SetResInfo( IMAGES_PATH,IDR_MAP,IDR_SMALL_MAP );
    CWceUiLayer::OnInitLayer();
}


void CBoxManGame::OnLButtonDown( UINT nFlags, POINT point )
{
    CPoint ptMan = m_boxMan.GetManPosition();
    TCHAR KeyDirect = 0; 

    if ( PtInRect(&g_rcDrawGameArea,point) )
    {
        int hor = ( point.x-g_rcDrawGameArea.left )/iBlockWidth;
        int ver = ( point.y-g_rcDrawGameArea.top )/iBlockHeight;
        if ( ptMan!= CPoint( hor,ver ) )
        {
            if ( m_boxMan.FindNearPos( ptMan.x,ptMan.y,hor,ver,KeyDirect ) )
            {
                SendKey( KeyDirect );
            }
            else if ( m_boxMan.IsPositionValid( hor,ver ) && m_boxMan.search( ptMan.x,ptMan.y,hor,ver ) )
            {
                ui_play_game_sound(L"step.wav");
                m_boxMan.ManJump( ptMan.x,ptMan.y, hor,ver );
                InvalidateRect();
                UpdateWindow(GetWindow()->m_hWnd);
            }
            else
            {
                ui_play_game_sound(L"danger.wav");
            }
        }				
    }
}

void CBoxManGame::OnLButtonUp( UINT nFlags, POINT point )
{

}

void CBoxManGame::Exit( BOOL bSound /*= TRUE*/ )
{
    if (bSound)
    {
         ui_play_game_sound(L"gameover.wav");
    }
}

void CBoxManGame::SendKey( UINT nChar )
{
    CPoint ptold,ptNow;
    m_boxMan.ReturnCurManPos( &ptold );
    DWORD dwOldMissionNum = m_boxMan.GetNowMissionNum();
    m_boxMan.KeyDown( nChar, 0, 0);

#ifdef _USE_MAKE_SOUND
    m_boxMan.ReturnCurManPos( &ptNow );

    if ( VK_BOXMAN_RESTART==nChar )
    {
		ui_play_game_sound(L"gamestart.wav");
    }
    else if ( VK_BOXMAN_FORER_BARRIER==nChar || VK_BOXMAN_NEXT_BARRIER==nChar )
    {
		ui_play_game_sound(L"pause.wav");
        m_boxMan.NewGameInit();
    }
    else if ( dwOldMissionNum != m_boxMan.GetNowMissionNum() )
    {
		ui_play_game_sound(L"wingame.wav");
    }
    else if ( ptNow == ptold )
    {
		ui_play_game_sound(L"danger.wav");
    }
    else
    {
        switch(nChar) 
        {
        case VK_LEFT:         
        case VK_UP :          
        case VK_RIGHT :       
        case VK_DOWN :
			ui_play_game_sound(L"step.wav");
            break;
        }
    }
#endif
    InvalidateRect();
    UpdateWindow(GetWindow()->m_hWnd);
    if ( dwOldMissionNum != m_boxMan.GetNowMissionNum() )
    {
      //  StartTiming();
    }
}

void CBoxManGame::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);
  
      
     TCHAR  m_Map[MAX_PATH] = {0};
      TCHAR tmpstr[MAX_PATH] ={0};
      _tcscpy((TCHAR *)m_Map,(TCHAR *)m_boxMan.GetMap());
  
  
      int i, j, x, y;
      //绘制整个地图游戏区域
      for (i = 0; i < M_TAB_HEIGHT; i++)
      {
          for (j = 0; j < M_TAB_WIDTH; j++)
          {
              x = j * BLOCK_WIDTH;
              y = i * BLOCK_HEIGHT;
              m_tmpMap[i][j] = m_Map[i*M_TAB_WIDTH+j];
              RETAILMSG(1,(TEXT(" m_tmpMap[i][j]............................= %d\r\n"),m_tmpMap[i][j]));
              switch (m_tmpMap[i][j])
              {
              case MAP_BACKGROUP://背景
                  //DrawBackGroup(x, y, pDC);
                  break;
              case MAP_WHITEWALL://墙
                  DrawWhiteWall(x, y, pWndBitmap);
                  break;
              case MAP_BLUEWALL://通道
                  DrawBlueWall(x, y, pWndBitmap);
                  break;
              case MAP_BALL://目的地
                  DrawBall(x, y, pWndBitmap);
                  break;
              case MAP_YELLOWBOX://未安放好的箱子
                  DrawYellowBox(x, y, pWndBitmap);
                  break;
              case MAP_REDBOX://安放好的箱子
                  DrawRedBox(x, y, pWndBitmap);
                  break;
              case MAP_MANWALL://人在通道区域
                  DrawManWall(x, y, pWndBitmap);
                  break;
              case MAP_MANBALL://人在目的地区域
                  DrawManBall(x, y, pWndBitmap);
                  break;
              }
          }
      }
  }



void CBoxManGame::DrawBackGroup( int x, int y, CWceUiGenericBitmap* pWndBitmap )
{

}

void CBoxManGame::DrawWhiteWall( int x, int y, CWceUiGenericBitmap* pWndBitmap )
{
	DrawBmp(pWndBitmap, x, y,m_pwhitewallbmp.GetBitmap());
}

void CBoxManGame::DrawBlueWall( int x, int y, CWceUiGenericBitmap* pWndBitmap )
{
	DrawBmp(pWndBitmap, x, y,m_pbluebmp.GetBitmap());
}

void CBoxManGame::DrawBall( int x, int y, CWceUiGenericBitmap* pWndBitmap )
{
	DrawBmp(pWndBitmap, x, y,m_pmapballbmp.GetBitmap());
}

void CBoxManGame::DrawYellowBox( int x, int y, CWceUiGenericBitmap* pWndBitmap )
{
	DrawBmp(pWndBitmap, x, y,m_pyellowboxbmp.GetBitmap());
}

void CBoxManGame::DrawRedBox( int x, int y, CWceUiGenericBitmap* pWndBitmap )
{
	DrawBmp(pWndBitmap, x, y,m_predboxbmp.GetBitmap());
}

void CBoxManGame::DrawManWall( int x, int y, CWceUiGenericBitmap* pWndBitmap )
{
	DrawBmp(pWndBitmap, x, y,m_pbluebmp.GetBitmap());
	DrawBmp(pWndBitmap, x, y,m_pmanbmp.GetBitmap());
}

void CBoxManGame::DrawManBall( int x, int y, CWceUiGenericBitmap* pWndBitmap )
{
	DrawBmp(pWndBitmap, x, y,m_pmapballbmp.GetBitmap()); 
	DrawBmp(pWndBitmap, x, y,m_pmanbmp.GetBitmap());
}

SIZE CBoxManGame::GetSizeByStr( LPCTSTR strSZ )
{
    LPTSTR pstr = NULL;
    SIZE sz = {0};
    sz.cx = _tcstol(strSZ, &pstr, 10);  ASSERT(pstr);    
    sz.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
    return sz;
}

void CBoxManGame::CopyScreen( HBITMAP hbmp, LPRECT prcSrc )
{
    HDC hdcSrc = ::CreateDC(L"DISPLAY", NULL, NULL, NULL);
    HDC hdcDest = ::CreateCompatibleDC(NULL);
    HBITMAP oldbmp = (HBITMAP)::SelectObject(hdcDest, hbmp);
    ::BitBlt(hdcDest, 0, 0,(prcSrc->right - prcSrc->left), (prcSrc->bottom - prcSrc->top), hdcSrc, prcSrc->left, prcSrc->top, SRCCOPY);
    ::SelectObject(hdcDest, oldbmp);
    DeleteDC(hdcDest);
}

RECT CBoxManGame::GetRectByStr( LPCTSTR strRect )
{
    LPTSTR pstr = NULL;
    RECT rc ={0};
    rc.left = _tcstol(strRect, &pstr, 10);  ASSERT(pstr); 
    rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
    rc.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
    rc.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr); 
    return  rc;
}

void CBoxManGame::DrawBmp( CWceUiGenericBitmap* pWndBitmap, int x, int y ,CWceUiGenericBitmap* pSrcBitmap )
{
	BLENDFUNCTION bf;
	memset(&bf, 0, sizeof(BLENDFUNCTION));
	bf.AlphaFormat = AC_SRC_ALPHA;
	pWndBitmap->AlphaBlend(x +g_rcDrawGameArea.left,y + g_rcDrawGameArea.top,iBlockWidth,iBlockHeight,pSrcBitmap,0,0,bf);
}

void CBoxManGame::OnBtnDown()
{
    SendKey(VK_DOWN);
}

void CBoxManGame::OnBtnRight()
{
    SendKey(VK_RIGHT);
}

void CBoxManGame::OnBtnUp()
{
    SendKey(VK_UP);
}

void CBoxManGame::NextLevel()
{
    SendKey(VK_BOXMAN_NEXT_BARRIER);
}

void CBoxManGame::PreLevel()
{
    SendKey(VK_BOXMAN_FORER_BARRIER);
}

void CBoxManGame::RePlay()
{
    if ( m_boxMan.UndoMove() )
    {
       ui_play_game_sound(L"undo.wav");
    }
}