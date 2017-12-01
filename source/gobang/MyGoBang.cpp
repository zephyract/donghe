#include "StdAfx.h"
#include "MyGoBang.h"
#include "WceUiRealWnd.h"
#include "uibase.h"

#define MSG_EMPTY			TEXT("")
#define PC_THINKING			TEXT("GOBANG_THINK")
#define GOBANG_TIE			TEXT("GOBANG_TIE")
#define PC_WIN				TEXT("GOBANG_PC_WIN")
#define MAN_WIN				TEXT("GOBANG_MAN_WIN")
#define BLACK_WIN			TEXT("GOBANG_BLACK_WIN")
#define WHITE_WIN			TEXT("GOBANG_WHITE_WIN")
#define BLACK_MOVE			TEXT("GOBANG_BLACK_MOVE")
#define WHITE_MOVE			TEXT("GOBANG_WHITE_MOVE")

#define  TIMER_ID_TINK 1
CMyGoBang::CMyGoBang( void )
{
    prept.x         = -1;
    prept.y         = -1;
    m_pTxt_Msg      = NULL;
    m_pTxt_Level    = NULL;
    m_pcbtn         = NULL;
    m_ppbtn         = NULL;
    debug           = false;					//标志是否处于调试程序状态
    m_brenren        = FALSE;				//是否是人人模式
    g_bStop         = FALSE;				//游戏是否结束
    bComputerFirst  = false;		//表示计算机是否先走
    g_bPCThinking   = FALSE;
    m_bNotSound     = TRUE;
    g_nGrade        = 4;


    SetDefaultGameInfo();
}

CMyGoBang::~CMyGoBang( void )
{

}

void CMyGoBang::SetProperties( PCWceUiXmlElem pCreateElem )
{  
    ASSERT(NULL != pCreateElem);
    CWceUiLayer::SetProperties(pCreateElem);
    
    LPCTSTR pstrLevel = pCreateElem->GetAttr(TEXT("level"));
    LPCTSTR pstrHT = pCreateElem->GetAttr(TEXT("igridht"));
    LPCTSTR pstrWD = pCreateElem->GetAttr(TEXT("igridwd"));
    LPCTSTR pstrXstart = pCreateElem->GetAttr(TEXT("x_game_start"));
    LPCTSTR pstrYstart = pCreateElem->GetAttr(TEXT("y_game_start"));
    LPCTSTR pstrXcount = pCreateElem->GetAttr(TEXT("xgridcount"));
    LPCTSTR pstrYcount = pCreateElem->GetAttr(TEXT("ygridcount"));

    LPCTSTR pstrdarkbmp  = pCreateElem->GetAttr(TEXT("darkbmp"));
    LPCTSTR pstrdarkdownbmp  = pCreateElem->GetAttr(TEXT("downdarkbmp"));
    LPCTSTR pstrwhitebmp  = pCreateElem->GetAttr(TEXT("whitebmp"));
    LPCTSTR pstrwhitedownbmp  = pCreateElem->GetAttr(TEXT("downwhitebmp"));


    if (NULL != pstrHT)
    {
        g_GameInfo.iGridHT = _ttoi(pstrHT);
    }

    if (NULL != pstrWD)
    {
        g_GameInfo.iGridWD = _ttoi(pstrWD);
    }
    if (NULL != pstrXstart)
    {
       g_GameInfo.m_ptGameStart.x = _ttoi(pstrXstart);
    }

    if (NULL != pstrYstart)
    {
        g_GameInfo.m_ptGameStart.y = _ttoi(pstrYstart);
    }

    if (NULL != pstrXcount)
    {
        g_GameInfo.m_xGridCount = _ttoi(pstrXcount);
    }

    if (NULL != pstrYcount)
    {
        g_GameInfo.m_yGridCount = _ttoi(pstrYcount);
    }
   fivestone.SetGridPointCount( g_GameInfo.m_xGridCount-1, g_GameInfo.m_yGridCount-1 );

   if (NULL != pstrwhitebmp)
   {
       m_pwhitebmp.SetBitmapFromRes(pstrwhitebmp);
   }
   if (NULL != pstrwhitedownbmp)
   {
       m_pwhitedownbmp.SetBitmapFromRes(pstrwhitedownbmp);
   }
   if (NULL != pstrdarkbmp)
   {
       m_pdarkbmp.SetBitmapFromRes(pstrdarkbmp);
   }
   if (NULL != pstrdarkdownbmp)
   {
       m_pdarkdownbmp.SetBitmapFromRes(pstrdarkdownbmp);
   }

}

void CMyGoBang::OnInitLayer()
{
    CWceUiLayer::OnInitLayer();
}

void CMyGoBang::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

    //绘制棋盘上的棋子
    for (int i=1;i<=g_GameInfo.m_xGridCount-1;i++)
    {
        for(int j=1;j<=g_GameInfo.m_yGridCount-1;j++)
        {
            if ( PonitsChess[i][j] != 0 )
            {
                DrawSpecStone( i,j,TRUE,pWndBitmap);
            }
        }
    }
}

LRESULT CMyGoBang::OnMessage( UINT message, WPARAM wParam, LPARAM lParam )
{
    return CWceUiLayer::OnMessage(message,wParam,lParam);
}

void CMyGoBang::OnLButtonDown( UINT nFlags, POINT point )
{
    //电脑正在查找走法，直接退回
    if ( g_bPCThinking ) return;


    //左键下黑子
    if (g_bStop)
    {
        ui_play_game_sound(L"danger.wav");
        return;
    }

    //不在游戏区域，则返回
    if ( point.x< g_GameInfo.m_ptGameStart.x || point.y< g_GameInfo.m_ptGameStart.y )
    {
        return;
    }

    int n1,n2;
    CoordInvert(point.x,point.y,&n1,&n2, TRUE );
    ++n1;++n2;

    if ( n1>=g_GameInfo.m_xGridCount || n2>=g_GameInfo.m_yGridCount || n1<=0 || n2<=0 )
    {
        return;
    }

    //没有子
    if (fivestone.ChessPoints[n1][n2]==0)
    {
            if (PonitsChess[prept.x][prept.y] == DOWN_WHITE_STONE)
            {
                PonitsChess[prept.x][prept.y] =WHITE_STONE;
            }
            else if (PonitsChess[prept.x][prept.y] == DOWN_BLACK_STONE)
            {
                PonitsChess[prept.x][prept.y] =BLACK_STONE;
            }
        fivestone.setthinkpos(0);		
        ui_play_game_sound(L"chess_move.wav");
        if (!m_brenren)
        {
            if ( bComputerFirst )	
            {
                fivestone.ChessPoints[n1][n2] = WHITE_STONE;		//计算机先走，人用白子
                //PonitsChess[n1][n2] = 2;							//计算机先走，人用白子
                PonitsChess[n1][n2] = DOWN_WHITE_STONE;
            }
            else 
            {
                fivestone.ChessPoints[n1][n2] = 1;			//人先走，人用黑子
                //PonitsChess[n1][n2] = 1;
                PonitsChess[n1][n2] = DOWN_BLACK_STONE;
            }
            if (!debug)	
            {
                steps[stepcount].x = n1;
                steps[stepcount].y = n2;
                stepcount++;
                InvalidateRect(NULL);
                GetWindow()->UpdateWindow();
                //if (!fivestone.has_tie())
                if ( !JudgeGameResult() )
                {
                    MyShowMessage( PC_THINKING );
                //    Sleep(1000);
                   g_bPCThinking = TRUE;
                   SetTimer(TIMER_ID_TINK,100);
                  //  computerprocess();
                 //   HANDLE hThread =::CreateThread(0,0,computerprocess,this,0,&m_dwThreadID);
                    //::SetThreadPriority( hThread,THREAD_PRIORITY_TIME_CRITICAL);
                   // CloseHandle(hThread);

                }
                else
                {	
                    ui_play_game_sound(L"manwin.wav");;
                }
                //else bStop = false;
            }
            //InvalidateRect(  getpointRect(n1,n2),FALSE);
            //InvalidateRect( getpointRect(steps[stepcount-2].x,steps[stepcount-2].y),FALSE);
        }
        else
        {
            //是人人对战，那么应该。。。。
            if (stepcount%2==0)
            {
                fivestone.ChessPoints[n1][n2] = 1;
                //PonitsChess[n1][n2] = 1;
                PonitsChess[n1][n2] = DOWN_BLACK_STONE;

                MyShowMessage( WHITE_MOVE );
            }
            else
            {
                fivestone.ChessPoints[n1][n2] = 2;
                //PonitsChess[n1][n2] = 2;
                PonitsChess[n1][n2] = DOWN_WHITE_STONE;

                MyShowMessage( BLACK_MOVE );
            }
            steps[stepcount].x = n1;
            steps[stepcount].y = n2;
            stepcount++;
            InvalidateRect(NULL);
            GetWindow()->UpdateWindow();
            if ( JudgeGameResult() )
               ui_play_game_sound(L"manwin.wav");
            else			
              ;//  m_GameSound.PlayGameSound( CHESS_MOVE,TRUE );
        }
    }	
}

void CMyGoBang::OnLButtonUp( UINT nFlags, POINT point )
{

}

void CMyGoBang::OnClick( PCWceUiButton pButton )
{
   // if (g_bPCThinking)
   // {
   //     return;
   // }
   // if (pButton->IsEqualName(TEXT("btnpre")))
   // {
   //     OnBtnPreLevel();
   // }
   // if (pButton->IsEqualName(TEXT("btnnext")))
   // {
   //     OnBtnNextLevel();
   // }
   // if (pButton->IsEqualName(TEXT("btnundo")))
   // {
   //     OnBnClickedBtnUndo();
   // }
   // if (pButton->IsEqualName(TEXT("btnpeople")))
   // {
   //     OnBnClickedBtnMantoman();
   // }
   // if (pButton->IsEqualName(TEXT("btnpc")))
   // {
   //     OnBnClickedBtnMantoman();
   // }
   // if (pButton->IsEqualName(TEXT("exit")))
   // {
   //     GetWindow()->PostMessage(MSG_GAME_END,0,0);
   // }
   // if (pButton->IsEqualName(TEXT("replay")))
   // {
   //     OnStart();
   // }
}

void CMyGoBang::SetDefaultGameInfo()
{
    g_GameInfo.iGridHT = 24;		//每个小格的高度
    g_GameInfo.iGridWD = 24;		//每个小格的宽度

    g_GameInfo.m_ptGameStart = CPoint(21,17);		//棋盘起始位置 

    g_GameInfo.m_xGridCount = 16;		//棋盘横格数量
    g_GameInfo.m_yGridCount = 13;		//棋盘纵格数量

    fivestone.SetGridPointCount( g_GameInfo.m_xGridCount-1, g_GameInfo.m_yGridCount-1 );
}

LRESULT CMyGoBang::OnMSGJudgeResule( WPARAM wp,LPARAM lp )
{
    if ( lp ==1 )
    {
        if ( bComputerFirst )
        {
            if (!m_brenren) MyShowMessage( WHITE_MOVE  );	
        }
        else
        {
            if (!m_brenren) MyShowMessage( BLACK_MOVE );				

        }
    }
    if ( JudgeGameResult() )
        ui_play_game_sound(L"pcwin.wav");
    else
    {
        ui_play_game_sound(L"chess_move.wav");
    }
    return 0;
}

BOOL CMyGoBang::JudgeGameResult()
{
    //判断是否是平局
    if (!g_bStop&&fivestone.has_tie())
    {
        g_bStop = true;
        MyShowMessage( GOBANG_TIE );
    }

    if ( !g_bStop&&fivestone.has_five( DARKSTONE ) )
    {
        g_bStop = true;
        if (bComputerFirst)
        {
            if (!m_brenren) 
                MyShowMessage( PC_WIN );	
            else 
                MyShowMessage( BLACK_WIN );
        }
        else
        {
            if (!m_brenren) 
                MyShowMessage( MAN_WIN );				
            else 
                MyShowMessage( BLACK_WIN );
        }
    }

    if (!g_bStop&&fivestone.has_five(2))
    {
        g_bStop = true;
        if (!bComputerFirst)
        {
            if (!m_brenren) 
                MyShowMessage( PC_WIN );
            else 
                MyShowMessage( WHITE_WIN );
        }	
        else
        {
            if (!m_brenren) MyShowMessage( MAN_WIN );
            else MyShowMessage( WHITE_WIN );
        }
    }

    return g_bStop;
}

DWORD CMyGoBang::computerprocess( LPVOID pParam )
{
    g_bPCThinking = TRUE;
    int x,y;//计算机走的子
    if (bComputerFirst) 
        fivestone.getpoint(x,y,1,false);
    else 
        fivestone.getpoint(x,y,2,false);
    if (x!=0) 
    {
        //if (soundeffect)	sndPlaySound(L"put",SND_RESOURCE|SND_ASYNC);
        
        if (PonitsChess[prept.x][prept.y] == DOWN_WHITE_STONE)
        {
            PonitsChess[prept.x][prept.y] =WHITE_STONE;
        }
        else if (PonitsChess[prept.x][prept.y] == DOWN_BLACK_STONE)
        {
            PonitsChess[prept.x][prept.y] =BLACK_STONE;
        }
        if (bComputerFirst) 
        {
            fivestone.ChessPoints[x][y] = 1;
            PonitsChess[x][y] = 3;//用不同颜色表示的棋子
        }
        else 
        {
            fivestone.ChessPoints[x][y] = 2;
            PonitsChess[x][y] = 4;//用不同颜色表示的棋子
        }
        steps[stepcount].x = x;
        steps[stepcount].y = y;
        stepcount++;

        //CString s;s.Format(L"计算机走棋：(%d,%d)!",x,y);	
        //InvalidateRect(thiswnd,getpointRect(x,y),FALSE);
        InvalidateRect(NULL);
        GetWindow()->UpdateWindow();
       // SendMessage( thiswnd, WM_MSG_DRAWSTONE,x,y );
        g_bStop = false;
        if (stepcount>2)
        {
            //SendMessage( thiswnd, WM_MSG_DRAWSTONE,steps[stepcount-3].x,steps[stepcount-3].y );
            //InvalidateRect(thiswnd,getpointRect(steps[stepcount-3].x,steps[stepcount-3].y),FALSE);
        }
    }	
    else
    {
        //已经有一方连成五子(实际上只有人赢的时候才会到这里)
        g_bStop = false;
        InvalidateRect(NULL);
        GetWindow()->UpdateWindow();
        //InvalidateRect(thiswnd,getpointRect(x,y),FALSE);
       // SendMessage( thiswnd, WM_MSG_DRAWSTONE,x,y );
    }
   // SendMessage( thiswnd, WM_MSG_JUDGERESULT,0,1 );
    OnMSGJudgeResule(0,1);

    g_bPCThinking = FALSE;
    //return 1;
    return 0;
}

void CMyGoBang::CoordInvert( int xSrc,int ySrc,int *pxDst,int *pyDst,BOOL bWndToGrid )
{
    //将屏幕的坐标转换成棋盘上的格子
    if ( bWndToGrid )
    {
        *pxDst = (xSrc-g_GameInfo.m_ptGameStart.x)/g_GameInfo.iGridWD;
        *pyDst = (ySrc-g_GameInfo.m_ptGameStart.y)/g_GameInfo.iGridHT;
    }
    else
    {
        *pxDst = xSrc*g_GameInfo.iGridWD + g_GameInfo.m_ptGameStart.x;
        *pyDst = ySrc*g_GameInfo.iGridHT + g_GameInfo.m_ptGameStart.y;
    }

#if 0 
    double dGridWD = iGridWD;
    double dGridHT = iGridHT;
    int n1=round( (double)(point.x - m_ptGameStart.x )/dGridWD );
    int n2=round( (double)(point.y - m_ptGameStart.y)/dGridHT );
#endif
}

void CMyGoBang::OnStart()
{
    if ( !m_bNotSound )
    {
        ui_play_game_sound(L"gamestart.wav");
    }

    //if (soundeffect) sndPlaySound(L"newgame",SND_RESOURCE|SND_ASYNC);

    fivestone.setthinkpos(0);
    fivestone.clear();

    memset( PonitsChess,0,sizeof(PonitsChess) );
    prept.x         = -1;
    prept.y         = -1;
    stepcount = 0;
    g_bStop = false;
    debug = false;
    if ( bComputerFirst )
    {
        fivestone.ChessPoints[8][8] = 1;//计算机先走
        PonitsChess[8][8] = 3;//计算机先走
        if (!debug)
        {
            steps[stepcount].x = 8;
            steps[stepcount].y = 8;
            stepcount++;
        }		
    }

    MyShowMessage( BLACK_MOVE );

    //InvalidateRect( CRect(10,10,400,400),TRUE);
    InvalidateRect( NULL);
}

void CMyGoBang::OnBnClickedBtnClose( BOOL bsound /*=1*/ )
{
    if (bsound)
    {
       ui_play_game_sound(L"gameover.wav");
    }
}

void CMyGoBang::OnBnClickedBtnMantoman()
{
    m_brenren = !m_brenren ;
    UpdateModeBtn();
    OnStart();
}

void CMyGoBang::MyShowMessage( LPCTSTR StrLangKey )
{
    if (NULL != m_pTxt_Msg)
    {
        m_strload.SetTextResID(StrLangKey);
        m_pTxt_Msg->SetText(m_strload.GetString());
    }
    
}

void CMyGoBang::OnBnClickedBtnUndo()
{
    if (g_bStop)
    {
        return;
    }
    ui_play_game_sound(L"undo.wav");
    
    prept.x         = -1;
    prept.y         = -1;
    //悔棋
    if (!m_brenren)
    {
        if (stepcount>=2)
        {
            //if (soundeffect)	sndPlaySound( _T("regret"),SND_RESOURCE|SND_ASYNC);

            fivestone.ChessPoints[steps[stepcount-1].x][steps[stepcount-1].y] = 0;
            fivestone.ChessPoints[steps[stepcount-2].x][steps[stepcount-2].y] = 0;
            PonitsChess[steps[stepcount-1].x][steps[stepcount-1].y] = 0;
            PonitsChess[steps[stepcount-2].x][steps[stepcount-2].y] = 0;
            //InvalidateRect(getpointRect(steps[stepcount-1].x,steps[stepcount-1].y));
            //InvalidateRect(getpointRect(steps[stepcount-2].x,steps[stepcount-2].y));
            InvalidateRect( NULL);
            stepcount = stepcount-2;
        }	
    }
    else
    {
        //在人人对下模式下，一次只要悔一步棋
        if (stepcount>=1)
        {
            //if (soundeffect) sndPlaySound( _T("regret"),SND_RESOURCE|SND_ASYNC);

            fivestone.ChessPoints[steps[stepcount-1].x][steps[stepcount-1].y] = 0;			
            PonitsChess[steps[stepcount-1].x][steps[stepcount-1].y] = 0;			
            //InvalidateRect(getpointRect(steps[stepcount-1].x,steps[stepcount-1].y));
            InvalidateRect( NULL);
            stepcount = stepcount-1;
            if (stepcount%2==0) MyShowMessage( BLACK_MOVE );
            else MyShowMessage( WHITE_MOVE );

        }		
    }
}

void CMyGoBang::OnBtnPreLevel()
{
    if ( g_nGrade >1 )
        --g_nGrade;
    else
    {
        g_nGrade = 1;
        return;
    }

    fivestone.setGrade( g_nGrade );
    ShowLevel();
    OnStart();
}

void CMyGoBang::OnBtnNextLevel()
{
    if ( g_nGrade <5 )
        ++g_nGrade;
    else
    {
        g_nGrade = 5;
        return;
    }

    fivestone.setGrade( g_nGrade );
    ShowLevel();
    OnStart();
}

void CMyGoBang::UpdateModeBtn()
{
    if (m_ppbtn != NULL && m_pcbtn != NULL)
    {
        m_ppbtn->ShowLayer(m_brenren);
        m_pcbtn->ShowLayer(!m_brenren);
    }
}

void CMyGoBang::ShowLevel()
{
    if ( NULL!= m_pTxt_Level )
    {
        CString strname;
        m_strload.SetTextResID(TEXT("LG_LEVEL"));
        strname.Format( TEXT("%s%d"),m_strload.GetString(),g_nGrade);
        m_pTxt_Level->SetText( strname );
    }
}

void CMyGoBang::OnTimer( WORD wIDEvent )
{
    if (wIDEvent == TIMER_ID_TINK)
    {
        KillTimer(TIMER_ID_TINK);
        computerprocess();
    }
}

void CMyGoBang::DrawSpecStone( int x,int y,BOOL bWndCoord,CWceUiGenericBitmap* pWndBitmap)
{
    CWceUiLoadBitmap* pbmpStone = NULL;
    switch (PonitsChess[x][y])
    {
    case BLACK_STONE://黑色棋
        pbmpStone = &m_pdarkbmp;
        break;
    case WHITE_STONE://白色棋
        pbmpStone = &m_pwhitebmp;
        break;
    case DOWN_BLACK_STONE://gray1,表示机器刚走的黑子
        {
            pbmpStone = &m_pdarkdownbmp;
            prept.x = x;
            prept.y = y;
        }
        //        if (!debug)	PonitsChess[x][y] = 1;//变回正常色
        break;	
    case DOWN_WHITE_STONE://gray2,表示机器刚走的白子
        {
            pbmpStone = &m_pwhitedownbmp;
            prept.x = x;
            prept.y = y;
        }
        break;	
    default:break;
    }

    if ( NULL!=pbmpStone)
    {
        int tempx,tempy;

        //tempx = iGridWD*x-(bm.bmWidth+1)/2  ;
        //tempy = iGridHT*y-(bm.bmHeight+1)/2 ;												
        tempx = g_GameInfo.iGridWD*(x-1) ;
        tempy = g_GameInfo.iGridHT*(y-1) ;												

        if ( bWndCoord )
        {
            tempx += g_GameInfo.m_ptGameStart.x;
            tempy += g_GameInfo.m_ptGameStart.y;
        }

		CRect rc(tempx, tempy, tempx+pbmpStone->GetBitmap()->GetWidth(), tempy+pbmpStone->GetBitmap()->GetHeight());

		BLENDFUNCTION bf;
		memset(&bf, 0, sizeof(BLENDFUNCTION));
		bf.AlphaFormat = AC_SRC_ALPHA;
		pWndBitmap->AlphaBlend(rc.left,rc.top,rc.Width(),rc.Height(),pbmpStone->GetBitmap(),0,0,bf);
    }
}

void CMyGoBang::InitGame()
{
    UpdateModeBtn();
    fivestone.setGrade( g_nGrade );
    ShowLevel();
    memset( PonitsChess,0,sizeof(PonitsChess) );  
    OnStart();
    m_bNotSound = FALSE;
}

BOOL CMyGoBang::IsThink()
{
    return g_bPCThinking;
}