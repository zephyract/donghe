#include "StdAfx.h"
#include "WceUiSlider.h"

#include "ChinaChessGame.h"
#include "WceUiRealWnd.h"
#include "WceUiButton.h"
#include "uibase.h"
#include "MainWnd.h"
#include "WceUiLayerCtrl.h"

GAMEINFO g_GameInfo;

#define	 GRILLEWIDTH		g_GameInfo.iGridWD			//棋盘上每个格子的宽度
#define	 GRILLEHEIGHT		g_GameInfo.iGridHT			//棋盘上每个格子的高度
#define  CHESS_X_START		g_GameInfo.iX_START			//(第一个棋子的X起始位置)
#define  CHESS_Y_START		g_GameInfo.iY_START			//(第一个棋子的Y起始位置)	



const BYTE InitChessBoard[10][9]=
{
    {B_CAR,B_HORSE,B_ELEPHANT,B_BISHOP,B_KING,B_BISHOP,B_ELEPHANT,B_HORSE,B_CAR},
    {NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
    {NOCHESS,B_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,B_CANON,NOCHESS},
    {B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN},
    {NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
    //楚河                       汉界//
    {NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
    {R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN},
    {NOCHESS,R_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,R_CANON,NOCHESS},
    {NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
    {R_CAR,R_HORSE,R_ELEPHANT,R_BISHOP,R_KING,R_BISHOP,R_ELEPHANT,R_HORSE,R_CAR}
};

enum GAME_MESSAGE { WELCOME_GAME,PC_THINKING,UNDO_OK, UNDO_EMPTY, MAN_WIN, PC_WIN, GAME_OVER, MSG_TOTAL };

LPCTSTR g_MSGArray[ MSG_TOTAL] = 
{
    TEXT("欢迎使用中国\r\n象棋"),
    TEXT("电脑思考中,请\r\n稍候..."),
    TEXT("悔棋完毕"),
    TEXT("棋已悔完,无棋\r\n可悔"),
    TEXT("恭喜你,你赢了"),					//L"老兄,你好强哦"
    TEXT("看来你不是我\r\n的对手"),
    TEXT("这盘棋已经下\r\n完了"),
};

//DWORD WINAPI ThinkProc(LPVOID pParam)
//{
//    CChinaChessGame* pChessgame=(CChinaChessGame*)pParam;
//    pChessgame->Think();
//
//    return 0;
//}
CChinaChessGame::CChinaChessGame(void)
{
    m_pStatusStatic     =   NULL;  
    m_pProgressThink    =   NULL;
    m_pmoverecordlist    =   NULL;

	m_iWhoChess=REDCHESS;
    m_Status=Chessing;
    m_bIsGameOver=false;
    m_nUserChessColor=REDCHESS;
    //m_nUserChessColor=BLACKCHESS;
    m_bIsThinking=false;

    m_bIsBegin=false;
    m_iBout=0;

    m_ptMoveChess.x= -1;
    m_ptMoveChess.y = -1;

    m_LastChessX = -1;
    m_LastChessY = -1;
    m_LastChess = -1;

    m_iEatCount = 0;

    SetDefaultGameInfo();
    m_hHandle = NULL;

}

CChinaChessGame::~CChinaChessGame(void)
{

    SAFE_DELEFE_PT( m_pSE );
 //   SAFE_DELEFE_PT( m_pMG );
 //   SAFE_DELEFE_PT( m_pEvel );
}

void CChinaChessGame::SetProperties( PCWceUiXmlElem pCreateElem )
{
    ASSERT(NULL != pCreateElem);
    CWceUiLayer::SetProperties(pCreateElem);
    LPCTSTR pstrHT = pCreateElem->GetAttr(TEXT("igridht"));
    LPCTSTR pstrWD = pCreateElem->GetAttr(TEXT("igridwd"));
    LPCTSTR pstrXstart = pCreateElem->GetAttr(TEXT("x_game_start"));
    LPCTSTR pstrYstart = pCreateElem->GetAttr(TEXT("y_game_start"));
    LPCTSTR pstrChess  = pCreateElem->GetAttr(TEXT("chessbmp"));
    LPCTSTR pstrCheck  = pCreateElem->GetAttr(TEXT("checkbmp"));


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
        g_GameInfo.iX_START = _ttoi(pstrXstart);
    }

    if (NULL != pstrYstart)
    {
        g_GameInfo.iY_START = _ttoi(pstrYstart);
    }
    if (NULL != pstrChess)
    {
		m_pchessbmp.SetBitmapFromRes(pstrChess);
        m_SizeChessBmp.cy =m_pchessbmp.GetBitmap()->GetHeight();
        m_SizeChessBmp.cx = m_pchessbmp.GetBitmap()->GetWidth()/14;
    }

    if (NULL != pstrCheck)
    {
		m_pchesscheckbmp.SetBitmapFromRes(pstrCheck);
    }
}

void CChinaChessGame::OnInitLayer()
{
    memcpy(m_byChessBoard,InitChessBoard,90);			//初始化棋盘
    memcpy(m_byShowChessBoard,InitChessBoard,90);
    memcpy(m_byBackupChessBoard,InitChessBoard,90);
    CWceUiLayer::OnInitLayer();
}


void CChinaChessGame::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	//棋子BMP
	int manwd = m_SizeChessBmp.cx;
	int manht = m_SizeChessBmp.cy; 
	int xOffset = (GRILLEWIDTH-manwd)/2;
	int yOffset = (GRILLEHEIGHT-manht)/2;

	int i,j;
	POINT pt;
	BLENDFUNCTION bf;
	memset(&bf, 0, sizeof(BLENDFUNCTION));
	bf.AlphaFormat = AC_SRC_ALPHA;
	//绘制棋盘上的棋子
	for(i=0;i<10;i++)   //行
	{
		for(j=0;j<9;j++)  //列
		{			
			if( NOCHESS==m_byShowChessBoard[i][j] )
				continue;

			pt.x=j*GRILLEWIDTH + CHESS_X_START + xOffset;
			pt.y=i*GRILLEHEIGHT + CHESS_Y_START + yOffset;

			int Chessbmpindex = m_byShowChessBoard[i][j]-1;
			CRect rc(pt.x, pt.y, pt.x+manwd, pt.y+manht);
			CRect rc_src(Chessbmpindex*manwd, 0, Chessbmpindex*manwd+manwd, manht);
			pWndBitmap->AlphaBlend(rc.left,rc.top,rc.Width(),rc.Height(),m_pchessbmp.GetBitmap(),rc_src.left,rc_src.top,bf);
		}
	}
	//check
	if( NOCHESS != m_MoveChess.nChessID )
	{
		int Box_X = m_ptMoveChess.x*GRILLEWIDTH  + CHESS_X_START;
		int Box_Y = m_ptMoveChess.y*GRILLEHEIGHT + CHESS_Y_START;

		int iWd = m_pchesscheckbmp.GetBitmap()->GetWidth();
		int iHt = m_pchesscheckbmp.GetBitmap()->GetHeight();

		int xOffset = (GRILLEWIDTH-iWd)/2;
		int yOffset = (GRILLEHEIGHT-iHt)/2;
		Box_X += xOffset;
		Box_Y += yOffset;
		CRect rc(Box_X, Box_Y, Box_X+iWd, Box_Y+iHt);
		pWndBitmap->AlphaBlend(rc.left,rc.top,rc.Width(),rc.Height(),m_pchesscheckbmp.GetBitmap(),0,0,bf);
	}

}

void CChinaChessGame::OnLButtonDown( UINT nFlags, POINT point )
{
    if( m_iChessSort==CS_CCCHESS )
    {
        ShowStatus( g_MSGArray[WELCOME_GAME] );
        return;
    }

    if(m_Status==Previewing)
    {
        return;
    }

    if(m_bIsGameOver)
    {
       ShowStatus( g_MSGArray[GAME_OVER] );
        return;
    }

    if(m_bIsThinking)//电脑正在想
        return;

    int x,y;

    //将坐标换算成棋盘上的格子

    y=(point.y-CHESS_Y_START)/GRILLEHEIGHT;
    x=(point.x-CHESS_X_START)/GRILLEWIDTH;


    //判断鼠标是否在棋盘内，并且点中了用户棋子
    if(y>=0 && y<10 && x>=0 && x<9 && (m_nUserChessColor==REDCHESS?IsRed(m_byChessBoard[y][x]):IsBlack(m_byChessBoard[y][x])))
    {
        //是之前选种的棋子
        if ( m_MoveChess.nChessID!=NOCHESS && m_ptMoveChess.x==x && m_ptMoveChess.y==y )
        {
            m_ptMoveChess.x= -1;
            m_ptMoveChess.y = -1;
            m_MoveChess.nChessID = NOCHESS;
        }	
        else	//选中当前棋子
        {
            memcpy(m_byBackupChessBoard,m_byChessBoard,90);//备份棋盘

            //将当前棋子的信息装入，记录移动棋子的结构中
            m_ptMoveChess.x=x;
            m_ptMoveChess.y=y;
            m_MoveChess.nChessID=m_byChessBoard[y][x];

            //将该棋子原位置棋子去掉
            //m_byChessBoard[y][x]=NOCHESS;
            //m_byShowChessBoard[y][x]=NOCHESS;

            point.x-= m_SizeChessBmp.cx/2;
            point.y-= m_SizeChessBmp.cy/2;

            m_MoveChess.ptMovePoint=point;
            //重绘屏幕
            //InvalidateRect(NULL,FALSE);
            //UpdateWindow();
            //SetCapture();//独占鼠标焦点
        }
        ui_play_game_sound(L"chess_sel.wav");
        InvalidateRect();
    }
    else
    {
        //if(m_Status==Chessing)
        //{
        //	if(y>=0 && y<10 && x>=0 && x<9 && (m_nUserChessColor!=REDCHESS?IsRed(m_byChessBoard[y][x]):IsBlack(m_byChessBoard[y][x])))
        //	{	
        //		m_pStatic_MSG->SetText(L"不好意思，这是我的棋子，请你不要乱动");
        //	}
        //	else
        //	{
        //		m_pStatic_MSG->SetText(L"老兄，那又没有棋子，你瞎点什么啊");
        //	}
        //}

        //if ( m_rcGameArea.PtInRect( point) ) g_GDIImageMana.PlayGameSound( DANGER_SOUND );

    }
}

void CChinaChessGame::OnLButtonUp( UINT nFlags, POINT point )
{
    if(m_Status!=Chessing)
        return;

    BOOL bEatChess = FALSE;

    if(m_bIsGameOver)
    {
       ShowStatus( g_MSGArray[GAME_OVER] );
        return;
    }

    if(m_bIsThinking || m_Status!=Chessing)
        return;

    volatile BOOL bTurnSide=FALSE;
    int x,y;
    CString str;

    //将坐标换算成棋盘上的格子
    y=( point.y-CHESS_Y_START )/GRILLEHEIGHT;
    x=( point.x-CHESS_X_START )/GRILLEWIDTH;

    //判断是否有移动棋子，并且该棋子的移动是一个合法走法
    //	if(m_MoveChess.nChessID && CMoveGenerator::IsValidMove(m_byBackupChessBoard,m_ptMoveChess.x,m_ptMoveChess.y,x,y,m_nUserChessColor))
    if( y>=0&&y<10&&x>=0&&x<9 && m_MoveChess.nChessID && m_pMG->IsValidMove(m_byBackupChessBoard,m_ptMoveChess.x,m_ptMoveChess.y,x,y,m_nUserChessColor))
    {
        //---------保存当前用户最后走的棋子和位置---------

        m_byChessBoard[m_ptMoveChess.y][m_ptMoveChess.x]=NOCHESS;
        m_byShowChessBoard[m_ptMoveChess.y][m_ptMoveChess.x]=NOCHESS;

        if ( NOCHESS!=m_byChessBoard[y][x] )
        {
            bEatChess = TRUE;
            ++m_iEatCount;
        }

        m_LastChessX = x;
        m_LastChessY = y;
        m_LastChess = m_MoveChess.nChessID;

        //---------将用户走法压栈---------
        m_cmBestMove.From.x=m_ptMoveChess.x;
        m_cmBestMove.From.y=m_ptMoveChess.y;
        m_cmBestMove.To.x=x;
        m_cmBestMove.To.y=y;
        m_cmBestMove.nChessID=m_MoveChess.nChessID;
        m_umUndoMove.cmChessMove=m_cmBestMove;
        m_umUndoMove.nChessID=m_byChessBoard[y][x];
        m_stackUndoMove.push(m_umUndoMove);
        //--------------------------------

        //鼠标释放时，棋子不在正中位置的问题
        m_MoveChess.ptMovePoint.x = x*GRILLEWIDTH + CHESS_X_START;
        m_MoveChess.ptMovePoint.y = y*GRILLEHEIGHT + CHESS_Y_START;

        //m_btnUndo.EnableWindow(1);//激活悔棋按钮
        if(m_nUserChessColor==REDCHESS)
            m_iBout++;
        this->AddChessRecord(m_ptMoveChess.x+1,m_ptMoveChess.y+1,x+1,y+1,m_nUserChessColor,m_MoveChess.nChessID,TRUE );
        m_byChessBoard[y][x]=m_MoveChess.nChessID;
        m_byShowChessBoard[y][x]=m_MoveChess.nChessID;

        TRACE(_T("People Move Chess :%d From Point(%d,%d) To Point(%d,%d) \r\n"),m_MoveChess.nChessID,m_ptMoveChess.x,m_ptMoveChess.y,x,y );

        if ( m_LastChess!=-1 && m_byChessBoard[y][x] != m_LastChess )
            TRACE(_T("!!!!ERROR IN LBTNUP People Last Chess to Point(%d,%d) is %d, but Must be %d \r\n"),x,y,m_byChessBoard[y][x], m_LastChess );

        m_MoveChess.nChessID=NOCHESS;				//将移动的棋子清空

        if ( bEatChess)
            ui_play_game_sound(L"chess_eat.wav");
        else
            ui_play_game_sound(L"chess_move.wav");

        bTurnSide=TRUE;
        //强制重绘屏幕  
		InvalidateRect();
		GetWindow()->UpdateWindow();
    }
    else//否则恢复移动前的棋盘状态
    {
        //memcpy(m_byShowChessBoard,m_byBackupChessBoard,90);
        //memcpy(m_byChessBoard,m_byBackupChessBoard,90);
    }

    if(bTurnSide==TRUE)
    {
        //游戏是否结束
        if( IsGameOver(m_byChessBoard)==-1 )
        {
            ShowStatus( g_MSGArray[MAN_WIN] );
            ui_play_game_sound(L"manwin.wav");
            return;
        }

        if ( m_LastChess!=-1 && m_byChessBoard[m_LastChessY][m_LastChessX] != m_LastChess )
            TRACE(_T("$$$$$$$$$$$ ERROR IN LBTNUP People Last Chess in Point(%d,%d) is %d, but Must be %d \r\n"),x,y,m_byChessBoard[y][x], m_LastChess );
        Think();
        //为什么wince 模式 创建线程声音就没了
        //m_hHandle = CreateThread(NULL,0,ThreadProc,this,NULL,NULL);
      //  m_hHandle=::CreateThread(0,0,ThinkProc,this,0,&m_dwThreadID);
      //  ::SetThreadPriority( m_hHandle,THREAD_PRIORITY_TIME_CRITICAL);
    }
    else
    {
        //m_pStatic_MSG->SetText( g_MSGArray[WELCOME_GAME] );
    }

}

int CChinaChessGame::IsGameOver(BYTE position[][9])
{
    int i,j;
    BOOL RedLive=FALSE,BlackLive=FALSE;

    //检查红方九宫是否有帅
    for(i=7;i<10;i++)
    {
        for(j=3;j<6;j++)
        {
            if(position[i][j]==B_KING)
                BlackLive=TRUE;
            if(position[i][j]==R_KING)
                RedLive=TRUE;
        }
    }

    //检查黑方九宫是否有将
    for(i=0;i<3;i++)
    {
        for(j=3;j<6;j++)
        {
            if(position[i][j]==B_KING)
                BlackLive=TRUE;
            if(position[i][j]==R_KING)
                RedLive=TRUE;
        }
    }

    if(m_nUserChessColor==REDCHESS)
    {
        if(!RedLive)
            return 1;

        if(!BlackLive)
            return -1;
    }
    else
    {
        if(!RedLive)
            return -1;

        if(!BlackLive)
            return 1;
    }
    return 0;
}

void CChinaChessGame::InvertChessBoard(BYTE cb[][9])
{
#ifdef _DEBUG
   OutputDebugString(TEXT("InvertChessBoard\r\n"));
#endif

    int i,j;
    BYTE btTemp;

    for(i=0;i<5;i++)
        for(j=0;j<9;j++)
        {
            btTemp=cb[i][j];
            cb[i][j]=cb[9-i][8-j];
            cb[9-i][8-j]=btTemp;
        }

        //刷新屏幕
        InvalidateRect();
        GetWindow()->UpdateWindow();
        //InvalidateRect(NULL,FALSE);
        //UpdateWindow();
}
void CChinaChessGame::AddChessRecord(int nFromX,int nFromY,int nToX,int nToY,int nUserChessColor,int nSourceID,BOOL bFirstStep )
{
    CString strChess;
    CString str;

    if(nUserChessColor==REDCHESS)//用户执红棋
        str=this->GetMoveStr(nFromX,nFromY,nToX,nToY,nSourceID);
    else
    {
        InvertChessBoard(m_byChessBoard);
        str=this->GetMoveStr(10-nFromX,11-nFromY,10-nToX,11-nToY,nSourceID);
        InvertChessBoard(m_byChessBoard);
    }
    str.Format(_T("%d-")+str,m_iBout);
    //m_StrAyMoveRecord.Add( str );
    if ( bFirstStep )
        //m_moverecordlist.AddString( str,1 );
        m_pmoverecordlist->AddString(str,1);
    else
        //m_moverecordlist.AddString( str,2 );
        m_pmoverecordlist->AddString(str,2);

}

CString CChinaChessGame::ConvertDigit2Chinese(int nNum)
{
    switch(nNum)
    {
    case 1:
        return _T("一");

    case 2:
        return L"二";

    case 3:
        return L"三";

    case 4:
        return L"四";

    case 5:
        return L"五";

    case 6:
        return L"六";

    case 7:
        return L"七";

    case 8:
        return L"八";

    case 9:
        return L"九";

    default:
        return L"";
    }
}

CString CChinaChessGame::GetMoveStr(int nFromX,int nFromY,int nToX,int nToY,int nSourceID)
{
    CString str;
    bool bIsAgain;
    int i;
    int nCount;
    int nPos[5];
    int j=0;

    switch(nSourceID)
    {
    case B_KING://黑将
        if(nFromY==nToY)
        {
            str.Format(L"黑:将%d平%d",nFromX,nToX);
            break;
        }

        if(nFromY>nToY)
            str.Format(L"黑:将%d退%d",nFromX,nFromY-nToY);
        else
            str.Format(L"黑:将%d进%d",nFromX,nToY-nFromY);

        break;

    case B_CAR://黑车
        bIsAgain=false;
        for(i=0;i<10;i++)
            if(m_byChessBoard[i][nFromX-1]==B_CAR && i!=nFromY-1 && i!=nToY-1)
            {
                bIsAgain=true;
                break;
            }

            if(nFromY>nToY)
            {
                if(bIsAgain)
                {
                    if(i>nFromY-1)
                        str.Format(L"黑:后车进%d",nFromY-nToY);
                    else
                        str.Format(L"黑:前车进%d",nFromY-nToY);
                }
                else
                    str.Format(L"黑:车%d退%d",nFromX,nFromY-nToY);
            }
            else
                if(nFromY<nToY)
                {						
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str.Format(L"黑:后车进%d",nToY-nFromY);
                        else
                            str.Format(L"黑:前车进%d",nToY-nFromY);
                    }
                    else
                        str.Format(L"黑:车%d进%d",nFromX,nToY-nFromY);
                }
                else
                {
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str.Format(L"黑:后车平%d",nToX);
                        else
                            str.Format(L"黑:前车平%d",nToX);
                    }
                    else
                        str.Format(L"黑:车%d平%d",nFromX,nToX);

                    break;
                }

                break;

    case B_HORSE://黑马
        bIsAgain=false;
        for(i=0;i<10;i++)
            if(m_byChessBoard[i][nFromX-1]==B_HORSE && i!=nFromY-1 && i!=nToY-1)
            {
                bIsAgain=true;
                break;
            }

            if(bIsAgain)
            {
                if(i>nFromY-1)
                {
                    if(nFromY>nToY)
                        str.Format(L"黑:后马退%d",nToX);
                    else
                        str.Format(L"黑:后马进%d",nToX);
                }
                else
                {
                    if(nFromY>nToY)
                        str.Format(L"黑:前马退%d",nToX);
                    else
                        str.Format(L"黑:前马进%d",nToX);
                }
            }
            else
            {
                if(nFromY>nToY)
                    str.Format(L"黑:马%d退%d",nFromX,nToX);
                else
                    str.Format(L"黑:马%d进%d",nFromX,nToX);
            }

            break;

    case B_CANON://黑炮	
        bIsAgain=false;
        for(i=0;i<10;i++)
            if(m_byChessBoard[i][nFromX-1]==B_CANON && i!=nFromY-1 && i!=nToY-1)
            {
                bIsAgain=true;
                break;
            }

            if(nFromY>nToY)
            {
                if(bIsAgain)
                {
                    if(i>nFromY-1)
                        str.Format(L"黑:后炮进%d",nFromY-nToY);
                    else
                        str.Format(L"黑:前炮进%d",nFromY-nToY);
                }
                else
                    str.Format(L"黑:炮%d退%d",nFromX,nFromY-nToY);
            }
            else
                if(nFromY<nToY)
                {
                    bIsAgain=false;
                    for(i=0;i<10;i++)
                        if(m_byChessBoard[i][nFromX-1]==B_CANON && i!=nFromY-1 && i!=nToY-1)
                        {
                            bIsAgain=true;
                            break;
                        }

                        if(bIsAgain)
                        {
                            if(i>nFromY-1)
                                str.Format(L"黑:后炮进%d",nToY-nFromY);
                            else
                                str.Format(L"黑:前炮进%d",nToY-nFromY);
                        }
                        else
                            str.Format(L"黑:炮%d进%d",nFromX,nToY-nFromY);
                }
                else
                {
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str.Format(L"黑:后炮平%d",nToX);
                        else
                            str.Format(L"黑:前炮平%d",nToX);
                    }
                    else
                        str.Format(L"黑:炮%d平%d",nFromX,nToX);
                    break;
                }

                break;

    case B_BISHOP://黑士
        if(nFromY>nToY)
            str.Format(L"黑:士%d退%d",nFromX,nToX);
        else
            str.Format(L"黑:士%d进%d",nFromX,nToX);

        break;

    case B_ELEPHANT://黑象
        bIsAgain=false;
        for(i=0;i<5;i++)
            if(m_byChessBoard[i][nFromX-1]==B_ELEPHANT && i!=nFromY-1 && i!=nToY-1)
            {
                bIsAgain=true;
                break;
            }

            if(bIsAgain)
            {
                if(i>nFromY-1)
                {
                    if(nFromY>nToY)
                        str.Format(L"黑:后象退%d",nToX);
                    else
                        str.Format(L"黑:后象进%d",nToX);
                }
                else
                {
                    if(nFromY>nToY)
                        str.Format(L"黑:前象退%d",nToX);
                    else
                        str.Format(L"黑:前象进%d",nToX);
                }
            }
            else
            {
                if(nFromY>nToY)
                    str.Format(L"黑:象%d退%d",nFromX,nToX);
                else
                    str.Format(L"黑:象%d进%d",nFromX,nToX);
            }

            break;

    case B_PAWN://黑卒
        nCount=0;
        j=0;
        for(i=0;i<5;i++)
            nPos[i]=-1;

        for(i=0;i<10;i++)
            if(m_byChessBoard[i][nFromX-1]==B_PAWN && i!=nFromY-1 && i!=nToY-1)
            {
                nPos[j]=i;
                nCount++;
            }

            if(nCount==0)
            {
                if(nFromY==nToY)
                    str.Format(L"黑:卒%d平%d",nFromX,nToX);
                else
                    str.Format(L"黑:卒%d进%d",nFromX,1);

                break;
            }
            if(nCount==1)
            {
                if(nFromY>nPos[0])
                {
                    if(nFromY==nToY)
                        str.Format(L"黑:前卒平%d",nToX);
                    else
                        str.Format(L"黑:前卒进%d",1);
                }
                else
                {
                    if(nFromY==nToY)
                        str.Format(L"黑:后卒平%d",nToX);
                    else
                        str.Format(L"黑:后卒进%d",1);
                }

                break;
            }
            j=0;
            if(nCount>1)
            {
                for(i=0;i<5;i++)
                    if(nPos[i]==-1)
                        break;
                    else
                        if(nPos[i]>nFromY)
                            break;
                        else
                            j++;

                if(nFromY==nToY)
                    str.Format(L"黑:%d卒平%d",j+1,1);
                else
                    str.Format(L"黑:%d卒进%d",j+1,1);

                break;
            }

    case R_KING://红帅
        if(nFromX==nToX)
        {
            if(nFromY>nToY)
                str=L"红:帅"+ConvertDigit2Chinese(10-nFromX)+L"进"+ConvertDigit2Chinese(nFromY-nToY);
            else
                str=L"红:帅"+ConvertDigit2Chinese(10-nFromX)+L"退"+ConvertDigit2Chinese(nToY-nFromY);
        }
        else
            str=L"红:帅"+ConvertDigit2Chinese(10-nFromX)+L"平"+ConvertDigit2Chinese(10-nToX);

        break;

    case R_CAR://红车
        bIsAgain=false;
        for(i=0;i<10;i++)
            if(m_byChessBoard[i][nFromX-1]==R_CAR && i!=nFromY-1 && i!=nToY-1)
            {
                bIsAgain=true;
                break;
            }

            if(nFromY>nToY)
            {
                if(bIsAgain)
                {
                    if(i>nFromY-1)
                        str=L"红:前车进"+ConvertDigit2Chinese(nFromY-nToY);
                    else
                        str=L"红:后车进"+ConvertDigit2Chinese(nFromY-nToY);
                }
                else
                    str=L"红:车"+ConvertDigit2Chinese(10-nFromX)+L"进"+ConvertDigit2Chinese(nFromY-nToY);
            }
            else
                if(nFromY<nToY)
                {
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str=L"红:后车退"+ConvertDigit2Chinese(nToY-nFromY);
                        else
                            str=L"红:前车退"+ConvertDigit2Chinese(nToY-nFromY);
                    }
                    else
                        str=L"红:车"+ConvertDigit2Chinese(10-nFromX)+L"退"+ConvertDigit2Chinese(nToY-nFromY);
                }
                else
                {
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str=L"红:后车平"+ConvertDigit2Chinese(nToX);
                        else
                            str=L"红:前车平"+ConvertDigit2Chinese(nToX);
                    }
                    else
                        str=L"红:车"+ConvertDigit2Chinese(10-nFromX)+L"平"+ConvertDigit2Chinese(10-nToX);

                    break;
                }

                break;

    case R_HORSE://红马
        bIsAgain=false;
        for(i=0;i<10;i++)
            if(m_byChessBoard[i][nFromX-1]==R_HORSE && i!=nFromY-1 && i!=nToY-1)
            {
                bIsAgain=true;
                break;
            }

            if(bIsAgain)
            {
                if(i>nFromY-1)
                {
                    if(nFromY>nToY)
                        str=L"红:前马进"+ConvertDigit2Chinese(10-nToX);
                    else
                        str=L"红:前马退"+ConvertDigit2Chinese(10-nToX);
                }
                else
                {
                    if(nFromY>nToY)
                        str=L"红:前马进"+ConvertDigit2Chinese(10-nToX);
                    else
                        str=L"红:前马退"+ConvertDigit2Chinese(10-nToX);
                }
            }
            else
            {
                if(nFromY>nToY)
                    str=L"红:马"+ConvertDigit2Chinese(10-nFromX)+L"进"+ConvertDigit2Chinese(10-nToX);
                else
                    str=L"红:马"+ConvertDigit2Chinese(10-nFromX)+L"退"+ConvertDigit2Chinese(10-nToX);
            }

            break;

    case R_CANON://红炮
        bIsAgain=false;
        for(i=0;i<10;i++)
            if(m_byChessBoard[i][nFromX-1]==R_CANON && i!=nFromY-1 && i!=nToY-1)
            {
                bIsAgain=true;
                break;
            }				

            if(nFromY>nToY)
            {
                if(bIsAgain)
                {
                    if(i>nFromY-1)
                        str=L"红:前炮进"+ConvertDigit2Chinese(nFromY-nToY);
                    else
                        str=L"红:后炮进"+ConvertDigit2Chinese(nFromY-nToY);
                }
                else
                {
                    //str=L"红:炮"+ConvertDigit2Chinese(10-nFromX)+L"进"+ConvertDigit2Chinese(nFromY-nToY);
                    str.Format( TEXT("%s%s%s%s"),L"红:炮",ConvertDigit2Chinese(10-nFromX),L"进",ConvertDigit2Chinese(nFromY-nToY) );
                }

            }
            else
                if(nFromY<nToY)
                {
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str=L"红:前炮退"+ConvertDigit2Chinese(nToY-nFromY);
                        else
                            str=L"红:后炮退"+ConvertDigit2Chinese(nToY-nFromY);
                    }
                    else
                        str=L"红:炮"+ConvertDigit2Chinese(nFromX)+L"退"+ConvertDigit2Chinese(nToY-nFromY);
                }
                else
                {
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str=L"红:前炮平"+ConvertDigit2Chinese(10-nToX);
                        else
                            str=L"红:后炮平"+ConvertDigit2Chinese(10-nToX);
                    }
                    else
                        str=L"红:炮"+ConvertDigit2Chinese(10-nFromX)+L"平"+ConvertDigit2Chinese(10-nToX);
                }

                break;

    case R_BISHOP://红士
        if(nFromY>nToY)
            str=L"红:士"+ConvertDigit2Chinese(10-nFromX)+L"进"+ConvertDigit2Chinese(10-nToX);
        else
            str=L"红:士"+ConvertDigit2Chinese(10-nFromX)+L"退"+ConvertDigit2Chinese(10-nToX);

        break;

    case R_ELEPHANT://红相
        bIsAgain=false;
        for(i=0;i<5;i++)
            if(m_byChessBoard[i][nFromX-1]==R_ELEPHANT && i!=nFromY-1 && i!=nToY-1)
            {
                bIsAgain=true;
                break;
            }

            if(bIsAgain)
            {
                if(i>nFromY-1)
                {
                    if(nFromY>nToY)
                        str=L"红:前相退"+ConvertDigit2Chinese(10-nToX);
                    else
                        str=L"红:前相进"+ConvertDigit2Chinese(10-nToX);
                }
                else
                {
                    if(nFromY>nToY)
                        str=L"红:后相退"+ConvertDigit2Chinese(10-nToX);
                    else
                        str=L"红:后相进"+ConvertDigit2Chinese(10-nToX);
                }
            }
            else
            {
                if(nFromY>nToY)
                    str=L"红:相"+ConvertDigit2Chinese(10-nFromX)+L"进"+ConvertDigit2Chinese(10-nToX);
                else
                    str=L"红:相"+ConvertDigit2Chinese(10-nFromX)+L"退"+ConvertDigit2Chinese(10-nToX);
            }

            break;

    case R_PAWN://红兵
        nCount=0;
        j=0;
        for(i=0;i<5;i++)
            nPos[i]=-1;

        for(i=0;i<10;i++)
            if(m_byChessBoard[i][nFromX-1]==R_PAWN && i!=nFromY-1 && i!=nToY-1)
            {
                nPos[j]=i;
                nCount++;
            }

            if(nCount==0)
            {
                if(nFromY==nToY)
                    str=L"红:兵"+ConvertDigit2Chinese(10-nFromX)+L"平"+ConvertDigit2Chinese(10-nToX);
                else
                    str=L"红:兵"+ConvertDigit2Chinese(10-nFromX)+L"进"+ConvertDigit2Chinese(1);

                break;
            }
            if(nCount==1)
            {
                if(nFromY-1>nPos[0])
                {
                    if(nFromY==nToY)
                        str=L"红:前兵平"+ConvertDigit2Chinese(10-nToX);
                    else
                        str=L"红:前兵进"+ConvertDigit2Chinese(1);
                }
                else
                {
                    if(nFromY==nToY)
                        str=L"红:后兵平"+ConvertDigit2Chinese(10-nToX);
                    else
                        str=L"红:后兵进"+ConvertDigit2Chinese(1);
                }

                break;
            }
            j=0;
            if(nCount>1)
            {
                for(i=0;i<5;i++)
                    if(nPos[i]==-1)
                        break;
                    else
                        if(nPos[i]>nFromY-1)
                            break;
                        else
                            j++;
                if(nFromY==nToY)
                    str=L"红:"+ConvertDigit2Chinese(j+1)+L"兵平"+ConvertDigit2Chinese(nToX);
                else
                    str=L"红:"+ConvertDigit2Chinese(j+1)+L"兵进"+ConvertDigit2Chinese(1);	
            }

            break;

    default:
        break;
    }

    return str;
}

void CChinaChessGame::Think()
{

    int timecount;
    CString sNodeCount;

    timecount=GetTickCount();//取初始时间

   ShowStatus( g_MSGArray[PC_THINKING] );

    if(m_nUserChessColor==BLACKCHESS)
        m_iBout++;
    m_bIsThinking=true;
    BOOL bEatChess = FALSE;

    if( m_LastChess!=-1 && m_byChessBoard[m_LastChessY][m_LastChessX] != m_LastChess )
    {
        TRACE(_T("!!!!!!!! Error before pc think:Point(%d,%d) (%d)  ,It must be:%d \r\n"),m_LastChessX,m_LastChessY,m_byChessBoard[m_LastChessY][m_LastChessX] ,m_LastChess );

        //zxj 棋子无故丢失的BUG，此处找回，并重新绘制
        m_byChessBoard[m_LastChessY][m_LastChessX] = m_LastChess;
        InvalidateRect();
        GetWindow()->UpdateWindow();
	
    }

    m_pProgressThink->SetPos(0);
    m_pSE->SearchAGoodMove(m_byChessBoard);

    memcpy(m_byShowChessBoard,m_byChessBoard,90);
    memcpy(m_byBackupChessBoard,m_byChessBoard,90);
    m_cmBestMove=m_pSE->GetBestMove();//得到最佳走法
    m_umUndoMove=m_pSE->GetUndoMove();
    m_stackUndoMove.push(m_umUndoMove);//将电脑走法压栈
    this->AddChessRecord(m_cmBestMove.From.x+1,m_cmBestMove.From.y+1,m_cmBestMove.To.x+1,m_cmBestMove.To.y+1,m_nUserChessColor,m_cmBestMove.nChessID,FALSE );



    if( m_LastChess!=-1 && m_byChessBoard[m_LastChessY][m_LastChessX] != m_LastChess  
        && m_cmBestMove.To.x!=m_LastChessX && m_cmBestMove.To.y != m_LastChessY )
    {
        TRACE(_T("!!!!!!!! Error In Thinking:Point(%d,%d) (%d)  ,It must be:%d \r\n"),
            m_LastChessX,m_LastChessY,m_byChessBoard[m_LastChessY][m_LastChessX] ,m_LastChess );
    }

  m_pProgressThink->StepToEnd();
    sNodeCount = _T("电脑下棋完毕!");
   ShowStatus(sNodeCount);

    //重绘屏幕  
    InvalidateRect();
    GetWindow()->UpdateWindow();
    m_bIsThinking=false;
    m_pEvel->ClearAccessCount();

    switch( IsGameOver(m_byChessBoard) )
    {
    case -1:
        ShowStatus( g_MSGArray[MAN_WIN] );
		ui_play_game_sound(L"manwin.wav");
        break;

    case 1:
       ShowStatus( g_MSGArray[PC_WIN] );
	   ui_play_game_sound(L"pcwin.wav");
        break;

    default:
        if ( NOCHESS!= m_umUndoMove.nChessID )
        {
			ui_play_game_sound(L"chess_eat.wav");
        }
        else
        {
			ui_play_game_sound(L"chess_move.wav");
        }

        return;
    }

    ::CloseHandle(m_hHandle);
    m_hHandle = NULL;
    m_bIsGameOver=true;
}

void CChinaChessGame::SetDefaultGameInfo()
{
    g_GameInfo.iGridWD = 25;		//每个小格的高度
    g_GameInfo.iGridHT = 25;		//每个小格的宽度

    g_GameInfo.iX_START = 11;		//(第一个棋子的X起始位置)
    g_GameInfo.iY_START = 10;		//(第一个棋子的Y起始位置)	

    m_rcGameArea = CRect( g_GameInfo.iX_START,g_GameInfo.iY_START, 
        g_GameInfo.iX_START+9*g_GameInfo.iGridWD,g_GameInfo.iY_START+10*g_GameInfo.iGridHT );

}

void CChinaChessGame::OnBnClickedBtnUndo()
{
    if(m_stackUndoMove.empty())
        return;
    m_MoveChess.nChessID = NOCHESS;


    ui_play_game_sound(L"undo.wav");

    if( m_iChessSort==CS_CCCHESS || m_Status==Previewing || m_bIsThinking ) //|| m_bIsGameOver
    {
        return;
    }

    //m_btnRedo.EnableWindow(1);//激活悔棋按钮
    m_bIsGameOver=false;
    for(int i=0;i<2;i++)
    {
        m_umUndoMove=m_stackUndoMove.top();
        m_stackRedoMove.push(m_umUndoMove);
        m_pSE->UndoChessMove(m_byChessBoard,&m_umUndoMove.cmChessMove,m_umUndoMove.nChessID);
        memcpy(m_byShowChessBoard,m_byChessBoard,90);
        memcpy(m_byBackupChessBoard,m_byChessBoard,90);
        m_stackUndoMove.pop();

        if ( m_umUndoMove.nChessID )
        {
            if ( REDCHESS== m_nUserChessColor && (  R_BEGIN <=m_umUndoMove.nChessID && m_umUndoMove.nChessID<=R_END ) )
                --m_iEatCount;
            else if ( BLACKCHESS== m_nUserChessColor && ( B_BEGIN<=m_umUndoMove.nChessID && m_umUndoMove.nChessID<=B_END ) )
                --m_iEatCount;	
        }

        //m_moverecordlist.DeleteString( m_moverecordlist.GetCount()-1);
        m_pmoverecordlist->DeleteString(m_pmoverecordlist->GetCount() -1);
        //重绘屏幕  
        InvalidateRect();
        GetWindow()->UpdateWindow();
    }
    m_iBout--;


     if(m_stackUndoMove.empty())
     {
         ShowStatus( g_MSGArray[UNDO_EMPTY] );
     }
     else
     {
         ShowStatus( g_MSGArray[UNDO_OK] );
     }
}

void CChinaChessGame::OnBnClickedBtnReplay()
{
    if(m_bIsThinking)//电脑正在想
        return;

    m_iEatCount = 0;
    m_iBout=0;
  //  m_moverecordlist.ClearList();
    m_pmoverecordlist->ClearList();
    ui_play_game_sound(L"gamestart.wav");

    memcpy(m_byChessBoard,InitChessBoard,90);    //初始化棋盘
    memcpy(m_byShowChessBoard,InitChessBoard,90);
    memcpy(m_byBackupChessBoard,InitChessBoard,90);
    m_MoveChess.nChessID=NOCHESS;                //清除移动棋子
    m_bIsGameOver=false;

    if(m_nUserChessColor==BLACKCHESS)
        this->InvertChessBoard(m_byChessBoard);

    m_pProgressThink->SetPos(0);

    //刷新屏幕
    InvalidateRect();
    GetWindow()->UpdateWindow();
//     InvalidateRect(NULL,FALSE);
//     UpdateWindow();
    m_bIsBegin=false;

    //清空栈
    while(!m_stackUndoMove.empty())
        m_stackUndoMove.pop();
    while(!m_stackRedoMove.empty())
        m_stackRedoMove.pop();

   ShowStatus( g_MSGArray[WELCOME_GAME] );
}

void CChinaChessGame::OnBnClickedBtnClose(BOOL bsound)
{
    if(m_bIsThinking)//电脑正在想
        return;

    if (bsound)
    {
        ui_play_game_sound(L"gameover.wav");
    } 
    if ( m_hHandle != NULL )
    {
        ::TerminateThread(m_hHandle,0);
        ::WaitForSingleObject(m_hHandle,INFINITE);
        ::CloseHandle(m_hHandle);
    }
}

// DWORD WINAPI CChinaChessGame::ThreadProc( LPVOID lpParameter )
// {
//     return 0;
// }

void CChinaChessGame::InitGame()
{
    m_pSE	=	new CAlphabeta_HHEngine;    //带历史启发的Alpha-Beta搜索引擎
    m_pMG	=	new CMoveGenerator;			//创建走法产生器
    m_pEvel	=	new CEveluation;			//创建估值核心
    m_pSE->SetSearchDepth(4);				//设定搜索层数为3

    m_pSE->SetMoveGenerator(m_pMG);					//给搜索引擎设定走法产生器
    m_pSE->SetEveluator(m_pEvel);					//给搜索引擎设定估值核心
    m_pSE->SetUserChessColor(m_nUserChessColor);	//设定用户为黑方或红方
    m_pSE->SetThinkProgress( m_pProgressThink );		//设定进度条
    m_MoveChess.nChessID=NOCHESS;//将移动的棋子清空
}

void CChinaChessGame::ShowStatus( LPCTSTR pText )
{
    if (NULL != m_pStatusStatic)
    {
        m_pStatusStatic->SetText(pText);
    }
}