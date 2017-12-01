#include "StdAfx.h"
#include "MyOthello.h"
#include "WceUiRealWnd.h"
#include "uibase.h"
#include "MainWnd.h"
#include "WceUiLayerCtrl.h"

//--------------------------------------------------------------------------------
#define MSG_EMPTY			TEXT("")
#define PC_THINKING			TEXT("GOBANG_THINK")
#define GOBANG_TIE_GAME			TEXT("GOBANG_TIE")
#define GOBANG_PC_WIN				TEXT("GOBANG_PC_WIN")
#define GOBANG_MAN_WIN				TEXT("GOBANG_MAN_WIN")
#define GOBANG_BLACK_WIN			TEXT("GOBANG_BLACK_WIN")
#define GOBANG_WHITE_WIN			TEXT("GOBANG_WHITE_WIN")
#define GOBANG_BLACK_MOVE			TEXT("GOBANG_BLACK_MOVE")
#define GOBANG_WHITE_MOVE			TEXT("GOBANG_WHITE_MOVE")
#define GOBANG_CHESS_BLACK			TEXT("GOBANG_CHESS_BLACK")
#define GOBANG_CHESS_WHITE			TEXT("GOBANG_CHESS_WHITE")


CMyOthello::CMyOthello( void )
{
    m_pTxt_NumBlack = NULL;
    m_pTxt_NumWhite = NULL;
    m_pTxt_Msg      = NULL;
    m_pBtnPC        = NULL;
    m_pBtnPeople    = NULL;

    m_bComputerMov = FALSE;
    m_bMantoMan = FALSE;
    m_UGameState = GM_PLAYING;
 
    SetDefaultGameInfo();
	m_bFirstPaint = FALSE;
}

CMyOthello::~CMyOthello( void )
{

}

void CMyOthello::SetProperties( PCWceUiXmlElem pCreateElem )
{
    ASSERT(NULL != pCreateElem);
    CWceUiLayer::SetProperties(pCreateElem);
    LPCTSTR pstrGridHT = pCreateElem->GetAttr(TEXT("igridht"));
    LPCTSTR pstrGridWD = pCreateElem->GetAttr(TEXT("igridwd"));
    LPCTSTR pstrXstart = pCreateElem->GetAttr(TEXT("x_game_start"));
    LPCTSTR pstrYstart = pCreateElem->GetAttr(TEXT("y_game_start"));
    LPCTSTR pstrStoneHT = pCreateElem->GetAttr(TEXT("stoneht"));
    LPCTSTR pstrStoneWD = pCreateElem->GetAttr(TEXT("stonewd"));
    LPCTSTR pstrBmpArr  = pCreateElem->GetAttr(TEXT("bmpstr"));

    if (NULL != pstrGridHT)
    {
        m_GameInfo.iGridHT = _ttoi(pstrGridHT);
    }

    if (NULL != pstrGridWD)
    {
        m_GameInfo.iGridWD = _ttoi(pstrGridWD);
    }
    if (NULL != pstrXstart)
    {
        m_GameInfo.m_ptMapStart.x= _ttoi(pstrXstart);
    }

    if (NULL != pstrYstart)
    {
        m_GameInfo.m_ptMapStart.y = _ttoi(pstrYstart);
    }

    if (NULL != pstrStoneWD)
    {
        m_GameInfo.m_szStone.cx = _ttoi(pstrStoneWD);
    }

    if (NULL != pstrStoneHT)
    {
        m_GameInfo.m_szStone.cy = _ttoi(pstrStoneHT);
    }

	LPCTSTR pstrsrc = pstrBmpArr;
    LPCTSTR pstrdeal = NULL;
    TCHAR pstrdest[MAX_PATH] ={0};
    DWORD nbmpcount  = 0;
    while(pstrsrc != NULL)
    {    
        pstrdeal = _tcsstr(pstrsrc,TEXT(","));
        DWORD nstrCount  = 0;
        if (NULL == pstrdeal)
        {
            _tcscpy(pstrdest,pstrsrc);
            // nstrCount = _tcslen(pstrsrc);
            pstrsrc = NULL;
        }
        else
        {
            nstrCount = pstrdeal  - pstrsrc;  
            for(int i = 0 ; i< nstrCount ;i++)
            {
                pstrdest[i] = *(pstrsrc +i);
            } 
            pstrdest[nstrCount] = TEXT('\0');
        }


		if (NULL != pstrdest)
        {
			m_pbmp[nbmpcount++].SetBitmapFromRes(pstrdest);
        }
        if (NULL != pstrdeal)
        {
            pstrsrc = pstrdeal + 1;
        }
        
    }
}

void CMyOthello::OnInitLayer()
{
   RegisterMsg(WM_CHESSMOVE);
   RegisterMsg(UM_COMPTRUN);
   RegisterMsg(WM_GAME_OVER);
   RegisterMsg(WM_TRANCHESS);
   InitGame();

    CWceUiLayer::OnInitLayer();
}

void CMyOthello::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
    __super::OnDraw(pWndBitmap, rcNeedDraw);

    int xOff= (m_GameInfo.iGridWD-m_GameInfo.m_szStone.cx)/2;
    int yOff= (m_GameInfo.iGridHT-m_GameInfo.m_szStone.cy)/2;;
    BLENDFUNCTION bf;
    memset(&bf,sizeof(BLENDFUNCTION),0);
    bf.AlphaFormat =AC_SRC_ALPHA;

	DrawValidMovPos( m_nCurMovChess, pWndBitmap);

   for(int i=0; i<BOARD_ROWS; i++)
    {
        for(int j=0; j<BOARD_COLS; j++)
        {
            if(m_oChessBoard.board[i+1][j+1] == CHESS_BLACK)
            {
                int xpos = j*m_GameInfo.iGridWD+m_GameInfo.m_ptMapStart.x + xOff;
                int ypos = i*m_GameInfo.iGridHT+m_GameInfo.m_ptMapStart.y + yOff;
				CRect rc(xpos, ypos, xpos+m_GameInfo.m_szStone.cx, ypos+m_GameInfo.m_szStone.cy);
               pWndBitmap->AlphaBlend(rc.left,rc.top,rc.Width(),rc.Height(),m_pbmp[NUM_MOVEBMP -1].GetBitmap(),0,0,bf);
            }
            else if(m_oChessBoard.board[i+1][j+1] == CHESS_WHITE)
            {
                int xpos = j*m_GameInfo.iGridWD+m_GameInfo.m_ptMapStart.x + xOff;
                int ypos = i*m_GameInfo.iGridHT+m_GameInfo.m_ptMapStart.y + yOff;
				CRect rc(xpos, ypos, xpos+m_GameInfo.m_szStone.cx, ypos+m_GameInfo.m_szStone.cy);
				pWndBitmap->AlphaBlend(rc.left,rc.top,rc.Width(),rc.Height(),m_pbmp[0].GetBitmap(),0,0,bf);
            }
        }
    }
	m_bFirstPaint = TRUE;
 
}

LRESULT CMyOthello::OnMessage( UINT message, WPARAM wParam, LPARAM lParam )
{
    if (message == WM_CHESSMOVE)
    {
        OnChessMove(wParam,lParam);
    }
    if (message == WM_GAME_OVER)
    {
        OnGameOver(wParam,lParam);
    }
    if (message == WM_TRANCHESS)
    {
        OnTranChess(wParam,lParam);
        GetWindow()->IgnoreMsgBeforeTime();
    }
    if (message == UM_COMPTRUN)
    {
        OnComRun(wParam,lParam);
    }
    return CWceUiLayer::OnMessage(message,wParam,lParam);
}

void CMyOthello::OnLButtonDown( UINT nFlags, POINT point )
{
	if (!m_bFirstPaint)
	{
		return;
	}
    if ( m_bComputerMov )
    {
        return;
    }

    UINT8 nstep = cur_step-1;
    for( ;nstep>0&&cur_step>1;--nstep ) 
    {
        if ( Step_Color[nstep] == Step_Color[nstep-1] )
        {
            TRACE( TEXT("nStep Repeat:%d \r\n"),nstep );
        }
    }

    BYTE row = (point.y-m_GameInfo.m_ptMapStart.y)/m_GameInfo.iGridHT + 1;
    BYTE col = (point.x-m_GameInfo.m_ptMapStart.x)/m_GameInfo.iGridWD  +1;

    //非法的坐标
    if ( row<1||row>BOARD_ROWS || col<1 || col>BOARD_COLS)
    {
        return;
    }

    if ( m_bMantoMan )
    {
        //下棋过程中禁止响应再下棋操作
        m_bComputerMov = TRUE;

        if(do_move_chess(&m_oChessBoard, row*10+col, m_nCurMovChess, GetWindow()->m_hWnd))
        {
            m_nCurMovChess = (~m_nCurMovChess)&0x03;
            INT16 affected_list[MAX_AFFECTED_PIECES];
            //查看对手是否有棋可下	
            if(!find_move( &m_oChessBoard, 11, m_nCurMovChess, affected_list))
            {
                //如果双方都无棋可下，则游戏结束
                if(!find_move( &m_oChessBoard, 11, (~m_nCurMovChess)&0x03, affected_list))
                {
                    GetWindow()->SendMessage(WM_GAME_OVER);
                  //  SendMessage( WM_GAME_OVER );
                    m_bComputerMov = FALSE;
                    return;
                }
                else  //否则下棋子放任为当前下棋的人
                {
                    m_nCurMovChess = (~m_nCurMovChess)&0x03;
                }
            }

            GetMovPtArray(&m_oChessBoard,m_vecW,m_vecB);
            if ( m_nCurMovChess== CHESS_BLACK )
                MyShowMessage( GOBANG_BLACK_MOVE  );
            else
                MyShowMessage( GOBANG_WHITE_MOVE );

            ShowResult();
            InvalidateRect();
        }
        m_bComputerMov = FALSE;

    }
    else
    {
        m_bComputerMov = TRUE;
        if(do_move_chess(&m_oChessBoard, row*10+col, ~computer_side&3, GetWindow()->m_hWnd))
        {
            ShowResult();
            MyShowMessage(  GOBANG_WHITE_MOVE );
            m_vecW.clear();
            m_vecB.clear();	
            InvalidateRect();
            GetWindow()->PostMessage(UM_COMPTRUN);
        }
        else
        {
            m_bComputerMov = FALSE;
        }
    }
}

void CMyOthello::OnLButtonUp( UINT nFlags, POINT point )
{

}

void CMyOthello::OnBnClick( PCWceUiButton pButton )
{
//     if (pButton->IsEqualName(TEXT("start")))
//     {
//         OnBtnStart();
// 
//     }
//     if (pButton->IsEqualName(TEXT("undo")))
//     {
//         //if (!m_bComputerMov)
//             MoveBack();
//         
//     }
//     if (pButton->IsEqualName(TEXT("pcmode")))
//     {
//        // if ( !m_bComputerMov )
//             OnManToMan();  
//     }
//     if (pButton->IsEqualName(TEXT("peoplemode")))
//     {
//         //if ( !m_bComputerMov )
//             OnManToMan();     
//     }
//     if (pButton->IsEqualName(TEXT("exit")))
//     {
//         GetWindow()->PostMessage(MSG_GAME_END,0,0);
//     }
}

void CMyOthello::SetDefaultGameInfo()
{
    m_GameInfo.iGridHT = 31;
    m_GameInfo.iGridWD = 31;
    m_GameInfo.m_szStone.SetSize( m_GameInfo.iGridWD,m_GameInfo.iGridHT );

    m_GameInfo.m_ptMapStart = CPoint(25,20);
}

LRESULT CMyOthello::OnTranChess( WPARAM wParam, LPARAM lParam )
{
    //OnPaint(); 
    int row = wParam/10-1;
    int col = wParam%10-1;

    if((lParam>>16) !=0)
    {
        TrunOverChess(row, col, UINT8(lParam));
    }
    else
    {
        DrawSpecialStone( col,row );
    }

    return 0;
}

void CMyOthello::TrunOverChess( int row, int col, UINT8 obcolor )
{
	int xPos = col*m_GameInfo.iGridWD+m_GameInfo.m_ptMapStart.x;
	int ypos = row*m_GameInfo.iGridHT+m_GameInfo.m_ptMapStart.y;

	int xOff= (m_GameInfo.iGridWD-m_GameInfo.m_szStone.cx)/2;
	int yOff= (m_GameInfo.iGridHT-m_GameInfo.m_szStone.cy)/2;
	xPos += xOff;
	ypos += yOff;
	CRect rcChess( xPos, ypos, xPos+m_GameInfo.m_szStone.cx, ypos+m_GameInfo.m_szStone.cy );


	HDC pDC = GetWindowDC(NULL);
	int m_iMotiveNumber;

	HDC ChessDC;
	ChessDC = CreateCompatibleDC( pDC );
	CWceUiGenericBitmap* pWndBmp  = GetWindow()->GetCurWndBmp();
	LPBYTE pbb;
	BLENDFUNCTION bf;
	memset(&bf,sizeof(BLENDFUNCTION),0);
	bf.AlphaFormat =AC_SRC_ALPHA;
	//目标棋子为黑色
	if(obcolor == CHESS_BLACK)
	{
		for(m_iMotiveNumber =0; m_iMotiveNumber<8; m_iMotiveNumber++)
		{
			pWndBmp->AlphaBlend(rcChess.left, rcChess.top, rcChess.Width(), rcChess.Height(),m_pbmp[m_iMotiveNumber].GetBitmap(),0,0,bf);
			HBITMAP  pBmpOld = (HBITMAP)SelectObject(ChessDC,pWndBmp->GetHBitmap(pbb) );  
			BitBlt(pDC, rcChess.left, rcChess.top, rcChess.Width(), rcChess.Height(), ChessDC, rcChess.left, rcChess.top, SRCCOPY);
			SelectObject(ChessDC, pBmpOld );
			//delay( TRUN_CHESS_DELAY );
			Sleep(TRUN_CHESS_DELAY);
		}
	}
	else
	{
		for(m_iMotiveNumber =7; m_iMotiveNumber>=0; m_iMotiveNumber--)
		{
			pWndBmp->AlphaBlend(rcChess.left, rcChess.top, rcChess.Width(), rcChess.Height(),m_pbmp[m_iMotiveNumber].GetBitmap(),0,0,bf);
			HBITMAP  pBmpOld = (HBITMAP)SelectObject(ChessDC,pWndBmp->GetHBitmap(pbb) );  
			BitBlt(pDC, rcChess.left, rcChess.top, rcChess.Width(), rcChess.Height(), ChessDC, rcChess.left, rcChess.top, SRCCOPY);
			SelectObject(ChessDC, pBmpOld );
			//delay( TRUN_CHESS_DELAY );
			Sleep(TRUN_CHESS_DELAY);


		}
	}
	delete pWndBmp;
	pWndBmp = NULL;
	DeleteDC(ChessDC);
	ReleaseDC(GetWindow()->m_hWnd,pDC);
}

void CMyOthello::ShowResult()
{
    UINT16 wscore, bscore;
    get_chess_score(&m_oChessBoard, wscore, bscore);
    CString StrWhite,StrBlack;
    m_strload.SetTextResID(GOBANG_CHESS_WHITE);
    CString	StrWName = m_strload.GetString();//m_language.GetCtrlString( GOBANG_CHESS_WHITE );
    m_strload.SetTextResID(GOBANG_CHESS_BLACK);
    CString	StrBName = m_strload.GetString();;//m_language.GetCtrlString( GOBANG_CHESS_BLACK );
    if ( StrWName.GetLength()==0 ) StrWName = TEXT("WHITE: "); 
    if ( StrBName.GetLength()==0 ) StrBName = TEXT("BLACK: "); 


    StrWhite.Format( TEXT("%s %d"),StrWName, wscore );
    StrBlack.Format( TEXT("%s %d"),StrBName,bscore );

    //StrWhite.Format( TEXT("WHITE: %d"), wscore );
    //StrBlack.Format( TEXT("BLACK: %d") ,bscore );

    if (NULL != m_pTxt_NumWhite )
    {
        m_pTxt_NumWhite->SetText( StrWhite );
    }
    if (NULL != m_pTxt_NumBlack ) 
    {
        m_pTxt_NumBlack->SetText( StrBlack );
    }
}

void CMyOthello::OnNewGame()
{
    m_UGameState = GM_PLAYING;
    m_nCurMovChess = CHESS_BLACK;

    m_bComputerMov = FALSE;
    init_board(&m_oChessBoard);

    //电脑先下
    //if ( !m_bMantoMan )SendMessage( UM_COMPTRUN );

    GetMovPtArray(&m_oChessBoard,m_vecW,m_vecB);
    MyShowMessage( GOBANG_BLACK_MOVE );
}

void CMyOthello::OnManToMan()
{
    if (m_bComputerMov)
    {
        return;
    }
    g_iGameLevel = GMLEVEL_EASY;
    m_UGameState = GM_PLAYING;
    ui_play_game_sound(L"gamestart.wav");

    m_bMantoMan = !m_bMantoMan;
    m_bComputerMov = FALSE;
    //if ( m_bMantoMan )
    //    m_pBtn_RenRen->SetImageIndex(0);
    //else
    //    m_pBtn_RenRen->SetImageIndex(2);
    UpdateMode();
    OnNewGame();
    ShowResult();
    InvalidateRect(); 
    GetWindow()->UpdateWindow();
}

void CMyOthello::MoveBack()
{
    if (m_bComputerMov)
    {
        return;
    }
    if ( m_bMantoMan )
    {
        if( cur_step<1 )
            return; 
    }
    else
    {
        if( cur_step<2)
            return;
    }

    m_UGameState = GM_PLAYING;
    ui_play_game_sound(L"undo.wav");

    UINT8 comside = computer_side;
    UINT8 step = cur_step;
    INT16 movearray[128];
    UINT8 moveColor[128];
    memcpy(movearray, step_array, 128*sizeof(INT16));
    memcpy(moveColor, Step_Color, 128*sizeof(UINT8));

    init_board(&m_oChessBoard);
    computer_side = comside;
    UINT8 col= CHESS_BLACK;

    int nUndoStep = 1;
    if ( m_bMantoMan )
    {
        nUndoStep = 1;
        //无棋下的情况下可能走了2步
        if ( step>=2 && moveColor[step-1] == moveColor[step-2] )
        {
            nUndoStep = 2;
            int nTempStep = step-2;
            while( nTempStep>0 && moveColor[nTempStep] == moveColor[nTempStep-1] )
            {
                --nTempStep;
                ++nUndoStep;
            }
        }


        for(int i=0; i<step-nUndoStep; i++, col = ~col & 3)
            do_move_chess(&m_oChessBoard, movearray[i], moveColor[i], NULL);
        //do_move_chess(&m_oChessBoard, movearray[i], col, NULL);
    }
    else
    {
        nUndoStep = 2;
        //无棋下的情况下可能走了2步
        if ( step>2 && (moveColor[step-1] == moveColor[step-2]  ) )
        {
            nUndoStep = 3;
            int nTempStep = step-2;
            while( nTempStep>1 && moveColor[nTempStep] == moveColor[nTempStep-1] )
            {
                --nTempStep;
                ++nUndoStep;
            }
        }
        else if ( step>2 && moveColor[step-2] == moveColor[step-3])
        {
            nUndoStep = 3;
            int nTempStep = step-3;
            while( nTempStep>1 && moveColor[nTempStep] == moveColor[nTempStep-1] )
            {
                --nTempStep;
                ++nUndoStep;
            }
        }

        for(int i=0; i<step-nUndoStep; i++, col = ~col & 3)
            do_move_chess(&m_oChessBoard, movearray[i], moveColor[i], NULL);
        //do_move_chess(&m_oChessBoard, movearray[i], col, NULL);
    }

    m_nCurMovChess = moveColor[step-nUndoStep];
    if ( m_bMantoMan )
    {
        CString StrKey = (m_nCurMovChess==CHESS_BLACK)? GOBANG_BLACK_MOVE:GOBANG_WHITE_MOVE;
        MyShowMessage( StrKey );
    }
    else
    {
        MyShowMessage( GOBANG_BLACK_MOVE );
        m_nCurMovChess = CHESS_BLACK;
    }
    ShowResult();
    GetMovPtArray(&m_oChessBoard,m_vecW,m_vecB);
    InvalidateRect();
}

LRESULT CMyOthello::OnComRun( WPARAM wParam, LPARAM lParam )
{
    computer_play(&m_oChessBoard, GetWindow()->m_hWnd); 
    ShowResult();
    GetMovPtArray(&m_oChessBoard,m_vecW,m_vecB);
 //   DrawValidMovPos( ~computer_side&3,NULL );
    InvalidateRect();
    GetWindow()->UpdateWindow();
    if ( m_UGameState==GM_PLAYING )
        MyShowMessage(  GOBANG_BLACK_MOVE );

    m_bComputerMov = FALSE;
    return 0;
}

LRESULT CMyOthello::OnChessMove( WPARAM wParam, LPARAM lParam )
{
    ui_play_game_sound(L"chess_move.wav");
    return 0 ;
}

LRESULT CMyOthello::OnGameOver( WPARAM wParam, LPARAM lParam )
{
    LPCTSTR nIDSound;
    CString strLangKey;
    UINT16 wscore, bscore;
    get_chess_score( &m_oChessBoard, wscore, bscore );
    m_UGameState = GM_MANWIN;

    //平局提示效果一样
    if ( wscore==bscore )
    {
        strLangKey = GOBANG_TIE_GAME;
        nIDSound =  L"manwin.wav";
    }

    if ( m_bMantoMan )
    {
		nIDSound =  L"manwin.wav";
        if(wscore > bscore)
        {
            strLangKey = GOBANG_WHITE_WIN;
        }
        else if(wscore <bscore)
        {	
            strLangKey = GOBANG_BLACK_WIN;
        }
    }
    else
    {
        if(wscore > bscore)
        {
            strLangKey = GOBANG_PC_WIN;
			nIDSound =  L"pcwin.wav";
            m_UGameState = GM_COMPWIN;
        }
        else if(wscore <bscore)
        {	
            strLangKey = GOBANG_MAN_WIN;
			nIDSound =  L"manwin.wav";
            if ( g_iGameLevel<GMLEVEL_ADV )++g_iGameLevel;
        }
    }
    MyShowMessage( strLangKey );
    ui_play_game_sound(nIDSound);
    return 0 ;
}

void CMyOthello::MyShowMessage( CString strLangKey )
{
    if ( m_pTxt_Msg !=NULL )
    {
        m_strload.SetTextResID(strLangKey);
        m_pTxt_Msg->SetText(m_strload.GetString());
    }
}

void CMyOthello::DrawValidMovPos( UINT nCHessID,CWceUiGenericBitmap* pWndBitmap)
{

    PTVECAY *pAy = NULL;
    if ( nCHessID == CHESS_BLACK )
    {
        pAy = &m_vecB;        
    }
    else
    {
        pAy = &m_vecW;     
    }

    int xOff= (m_GameInfo.iGridWD-m_GameInfo.m_szStone.cx)/2;
    int yOff= (m_GameInfo.iGridHT-m_GameInfo.m_szStone.cy)/2;

    BLENDFUNCTION bf;
    memset(&bf,sizeof(BLENDFUNCTION),0);
    bf.AlphaFormat =AC_SRC_ALPHA;
    POINT ptTemp;
    for ( int i=0;i<pAy->size();i++ )
    {
        ptTemp = (*pAy)[i];
        if ( 0<ptTemp.x && ptTemp.x<=BOARD_COLS  && 0<ptTemp.y && ptTemp.y<=BOARD_ROWS )
        {
            int xpos = (ptTemp.y-1)*m_GameInfo.iGridWD+m_GameInfo.m_ptMapStart.x + xOff;
            int ypos = (ptTemp.x-1)*m_GameInfo.iGridHT+m_GameInfo.m_ptMapStart.y + yOff;
			CRect rc(xpos, ypos, xpos+m_GameInfo.m_szStone.cx, ypos+m_GameInfo.m_szStone.cy);
            if ( nCHessID == CHESS_BLACK )
            {
				pWndBitmap->AlphaBlend( rc.left, rc.top, rc.Width(), rc.Height(),m_pbmp[NUM_MOVEBMP+1].GetBitmap(),0,0,bf);
            }
            else
            {
				pWndBitmap->AlphaBlend( rc.left, rc.top, rc.Width(), rc.Height(),m_pbmp[NUM_MOVEBMP].GetBitmap(),0,0,bf);
            }
            	
        }
    }
}

void CMyOthello::UpdateMode()
{
    if (m_pBtnPC != NULL && m_pBtnPeople != NULL)
    {
        m_pBtnPC->ShowLayer(!m_bMantoMan);
        m_pBtnPeople->ShowLayer(m_bMantoMan);
    }
}

void CMyOthello::DrawSpecialStone( int x,int y, HDC hdc /*= NULL*/ )
{
	if ( y<0||y>=BOARD_ROWS || x<0||x>= BOARD_COLS )
		return;

	HDC hWhiteChessDC,hBlackChessDC;
	BOOL bGetWinDC = FALSE;
	if ( hdc ==NULL )
	{
		hdc = GetDC(NULL);
		bGetWinDC = TRUE;
	}

	hWhiteChessDC = CreateCompatibleDC( hdc );
	hBlackChessDC = CreateCompatibleDC( hdc );

	HBITMAP pBmpOldW =	NULL;
	HBITMAP pBmpOldB =	NULL;

	int xOff= (m_GameInfo.iGridWD-m_GameInfo.m_szStone.cx)/2;
	int yOff= (m_GameInfo.iGridHT-m_GameInfo.m_szStone.cy)/2;

	CWceUiGenericBitmap* pWndBmp  = GetWindow()->GetCurWndBmp();
	LPBYTE pbb;
	BLENDFUNCTION bf;
	memset(&bf,sizeof(BLENDFUNCTION),0);
	bf.AlphaFormat =AC_SRC_ALPHA;

	if( m_oChessBoard.board[y+1][x+1] == CHESS_BLACK )
	{
		int xpos = x*m_GameInfo.iGridWD+m_GameInfo.m_ptMapStart.x + xOff;
		int ypos = y*m_GameInfo.iGridHT+m_GameInfo.m_ptMapStart.y + yOff;
		pWndBmp->AlphaBlend(xpos,ypos,m_GameInfo.m_szStone.cx,m_GameInfo.m_szStone.cy,m_pbmp[NUM_MOVEBMP-1].GetBitmap(),0,0,bf);
		HBITMAP  pBmpOld = (HBITMAP)SelectObject(hBlackChessDC,pWndBmp->GetHBitmap(pbb) );  
		BitBlt(hdc, xpos,ypos,m_GameInfo.m_szStone.cx,m_GameInfo.m_szStone.cy,hBlackChessDC,xpos,ypos,SRCCOPY);
	}
	else if( m_oChessBoard.board[y+1][x+1] == CHESS_WHITE )
	{
		int xpos = x*m_GameInfo.iGridWD+m_GameInfo.m_ptMapStart.x+ xOff;
		int ypos = y*m_GameInfo.iGridHT+m_GameInfo.m_ptMapStart.y+ yOff;
		pWndBmp->AlphaBlend(xpos,ypos,m_GameInfo.m_szStone.cx,m_GameInfo.m_szStone.cy,m_pbmp[0].GetBitmap(),0,0,bf);
		HBITMAP  pBmpOld = (HBITMAP)SelectObject(hBlackChessDC,pWndBmp->GetHBitmap(pbb) );  
		BitBlt(hdc, xpos,ypos,m_GameInfo.m_szStone.cx,m_GameInfo.m_szStone.cy,hBlackChessDC,xpos,ypos,SRCCOPY);
	}


	delete pWndBmp;
	pWndBmp = NULL;
	SelectObject(hWhiteChessDC, pBmpOldW );
	SelectObject( hBlackChessDC,pBmpOldB );

	DeleteDC(hWhiteChessDC);
	DeleteDC(hBlackChessDC);

	if ( bGetWinDC )
	{
		ReleaseDC(GetWindow()->m_hWnd, hdc );
	}
}

void CMyOthello::OnBnClickedBtnClose( BOOL bSound /*= TRUE*/ )
{
    if (bSound)
    {
        ui_play_game_sound(L"gameover.wav");
    }   
}

void CMyOthello::OnBtnStart()
{
    if ( !m_bComputerMov )
    {
        ui_play_game_sound(L"gamestart.wav");
        if ( m_UGameState == GM_PLAYING )
            g_iGameLevel = GMLEVEL_EASY;

        OnNewGame();
        ShowResult();
        InvalidateRect();
        GetWindow()->UpdateWindow();
    }
}

void CMyOthello::InitGame()
{
    UpdateMode();
    init_board(&m_oChessBoard);
    //电脑先下
    //SendMessage( UM_COMPTRUN );

    m_nCurMovChess = CHESS_BLACK;
    GetMovPtArray(&m_oChessBoard,m_vecW,m_vecB);
    ShowResult();
    MyShowMessage( GOBANG_BLACK_MOVE );
}