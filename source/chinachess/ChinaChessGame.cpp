#include "StdAfx.h"
#include "WceUiSlider.h"

#include "ChinaChessGame.h"
#include "WceUiRealWnd.h"
#include "WceUiButton.h"
#include "uibase.h"
#include "MainWnd.h"
#include "WceUiLayerCtrl.h"

GAMEINFO g_GameInfo;

#define	 GRILLEWIDTH		g_GameInfo.iGridWD			//������ÿ�����ӵĿ��
#define	 GRILLEHEIGHT		g_GameInfo.iGridHT			//������ÿ�����ӵĸ߶�
#define  CHESS_X_START		g_GameInfo.iX_START			//(��һ�����ӵ�X��ʼλ��)
#define  CHESS_Y_START		g_GameInfo.iY_START			//(��һ�����ӵ�Y��ʼλ��)	



const BYTE InitChessBoard[10][9]=
{
    {B_CAR,B_HORSE,B_ELEPHANT,B_BISHOP,B_KING,B_BISHOP,B_ELEPHANT,B_HORSE,B_CAR},
    {NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
    {NOCHESS,B_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,B_CANON,NOCHESS},
    {B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN,NOCHESS,B_PAWN},
    {NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
    //����                       ����//
    {NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
    {R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN,NOCHESS,R_PAWN},
    {NOCHESS,R_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,R_CANON,NOCHESS},
    {NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
    {R_CAR,R_HORSE,R_ELEPHANT,R_BISHOP,R_KING,R_BISHOP,R_ELEPHANT,R_HORSE,R_CAR}
};

enum GAME_MESSAGE { WELCOME_GAME,PC_THINKING,UNDO_OK, UNDO_EMPTY, MAN_WIN, PC_WIN, GAME_OVER, MSG_TOTAL };

LPCTSTR g_MSGArray[ MSG_TOTAL] = 
{
    TEXT("��ӭʹ���й�\r\n����"),
    TEXT("����˼����,��\r\n�Ժ�..."),
    TEXT("�������"),
    TEXT("���ѻ���,����\r\n�ɻ�"),
    TEXT("��ϲ��,��Ӯ��"),					//L"����,���ǿŶ"
    TEXT("�����㲻����\r\n�Ķ���"),
    TEXT("�������Ѿ���\r\n����"),
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
    memcpy(m_byChessBoard,InitChessBoard,90);			//��ʼ������
    memcpy(m_byShowChessBoard,InitChessBoard,90);
    memcpy(m_byBackupChessBoard,InitChessBoard,90);
    CWceUiLayer::OnInitLayer();
}


void CChinaChessGame::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	//����BMP
	int manwd = m_SizeChessBmp.cx;
	int manht = m_SizeChessBmp.cy; 
	int xOffset = (GRILLEWIDTH-manwd)/2;
	int yOffset = (GRILLEHEIGHT-manht)/2;

	int i,j;
	POINT pt;
	BLENDFUNCTION bf;
	memset(&bf, 0, sizeof(BLENDFUNCTION));
	bf.AlphaFormat = AC_SRC_ALPHA;
	//���������ϵ�����
	for(i=0;i<10;i++)   //��
	{
		for(j=0;j<9;j++)  //��
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

    if(m_bIsThinking)//����������
        return;

    int x,y;

    //�����껻��������ϵĸ���

    y=(point.y-CHESS_Y_START)/GRILLEHEIGHT;
    x=(point.x-CHESS_X_START)/GRILLEWIDTH;


    //�ж�����Ƿ��������ڣ����ҵ������û�����
    if(y>=0 && y<10 && x>=0 && x<9 && (m_nUserChessColor==REDCHESS?IsRed(m_byChessBoard[y][x]):IsBlack(m_byChessBoard[y][x])))
    {
        //��֮ǰѡ�ֵ�����
        if ( m_MoveChess.nChessID!=NOCHESS && m_ptMoveChess.x==x && m_ptMoveChess.y==y )
        {
            m_ptMoveChess.x= -1;
            m_ptMoveChess.y = -1;
            m_MoveChess.nChessID = NOCHESS;
        }	
        else	//ѡ�е�ǰ����
        {
            memcpy(m_byBackupChessBoard,m_byChessBoard,90);//��������

            //����ǰ���ӵ���Ϣװ�룬��¼�ƶ����ӵĽṹ��
            m_ptMoveChess.x=x;
            m_ptMoveChess.y=y;
            m_MoveChess.nChessID=m_byChessBoard[y][x];

            //��������ԭλ������ȥ��
            //m_byChessBoard[y][x]=NOCHESS;
            //m_byShowChessBoard[y][x]=NOCHESS;

            point.x-= m_SizeChessBmp.cx/2;
            point.y-= m_SizeChessBmp.cy/2;

            m_MoveChess.ptMovePoint=point;
            //�ػ���Ļ
            //InvalidateRect(NULL,FALSE);
            //UpdateWindow();
            //SetCapture();//��ռ��꽹��
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
        //		m_pStatic_MSG->SetText(L"������˼�������ҵ����ӣ����㲻Ҫ�Ҷ�");
        //	}
        //	else
        //	{
        //		m_pStatic_MSG->SetText(L"���֣�����û�����ӣ���Ϲ��ʲô��");
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

    //�����껻��������ϵĸ���
    y=( point.y-CHESS_Y_START )/GRILLEHEIGHT;
    x=( point.x-CHESS_X_START )/GRILLEWIDTH;

    //�ж��Ƿ����ƶ����ӣ����Ҹ����ӵ��ƶ���һ���Ϸ��߷�
    //	if(m_MoveChess.nChessID && CMoveGenerator::IsValidMove(m_byBackupChessBoard,m_ptMoveChess.x,m_ptMoveChess.y,x,y,m_nUserChessColor))
    if( y>=0&&y<10&&x>=0&&x<9 && m_MoveChess.nChessID && m_pMG->IsValidMove(m_byBackupChessBoard,m_ptMoveChess.x,m_ptMoveChess.y,x,y,m_nUserChessColor))
    {
        //---------���浱ǰ�û�����ߵ����Ӻ�λ��---------

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

        //---------���û��߷�ѹջ---------
        m_cmBestMove.From.x=m_ptMoveChess.x;
        m_cmBestMove.From.y=m_ptMoveChess.y;
        m_cmBestMove.To.x=x;
        m_cmBestMove.To.y=y;
        m_cmBestMove.nChessID=m_MoveChess.nChessID;
        m_umUndoMove.cmChessMove=m_cmBestMove;
        m_umUndoMove.nChessID=m_byChessBoard[y][x];
        m_stackUndoMove.push(m_umUndoMove);
        //--------------------------------

        //����ͷ�ʱ�����Ӳ�������λ�õ�����
        m_MoveChess.ptMovePoint.x = x*GRILLEWIDTH + CHESS_X_START;
        m_MoveChess.ptMovePoint.y = y*GRILLEHEIGHT + CHESS_Y_START;

        //m_btnUndo.EnableWindow(1);//������尴ť
        if(m_nUserChessColor==REDCHESS)
            m_iBout++;
        this->AddChessRecord(m_ptMoveChess.x+1,m_ptMoveChess.y+1,x+1,y+1,m_nUserChessColor,m_MoveChess.nChessID,TRUE );
        m_byChessBoard[y][x]=m_MoveChess.nChessID;
        m_byShowChessBoard[y][x]=m_MoveChess.nChessID;

        TRACE(_T("People Move Chess :%d From Point(%d,%d) To Point(%d,%d) \r\n"),m_MoveChess.nChessID,m_ptMoveChess.x,m_ptMoveChess.y,x,y );

        if ( m_LastChess!=-1 && m_byChessBoard[y][x] != m_LastChess )
            TRACE(_T("!!!!ERROR IN LBTNUP People Last Chess to Point(%d,%d) is %d, but Must be %d \r\n"),x,y,m_byChessBoard[y][x], m_LastChess );

        m_MoveChess.nChessID=NOCHESS;				//���ƶ����������

        if ( bEatChess)
            ui_play_game_sound(L"chess_eat.wav");
        else
            ui_play_game_sound(L"chess_move.wav");

        bTurnSide=TRUE;
        //ǿ���ػ���Ļ  
		InvalidateRect();
		GetWindow()->UpdateWindow();
    }
    else//����ָ��ƶ�ǰ������״̬
    {
        //memcpy(m_byShowChessBoard,m_byBackupChessBoard,90);
        //memcpy(m_byChessBoard,m_byBackupChessBoard,90);
    }

    if(bTurnSide==TRUE)
    {
        //��Ϸ�Ƿ����
        if( IsGameOver(m_byChessBoard)==-1 )
        {
            ShowStatus( g_MSGArray[MAN_WIN] );
            ui_play_game_sound(L"manwin.wav");
            return;
        }

        if ( m_LastChess!=-1 && m_byChessBoard[m_LastChessY][m_LastChessX] != m_LastChess )
            TRACE(_T("$$$$$$$$$$$ ERROR IN LBTNUP People Last Chess in Point(%d,%d) is %d, but Must be %d \r\n"),x,y,m_byChessBoard[y][x], m_LastChess );
        Think();
        //Ϊʲôwince ģʽ �����߳�������û��
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

    //���췽�Ź��Ƿ���˧
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

    //���ڷ��Ź��Ƿ��н�
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

        //ˢ����Ļ
        InvalidateRect();
        GetWindow()->UpdateWindow();
        //InvalidateRect(NULL,FALSE);
        //UpdateWindow();
}
void CChinaChessGame::AddChessRecord(int nFromX,int nFromY,int nToX,int nToY,int nUserChessColor,int nSourceID,BOOL bFirstStep )
{
    CString strChess;
    CString str;

    if(nUserChessColor==REDCHESS)//�û�ִ����
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
        return _T("һ");

    case 2:
        return L"��";

    case 3:
        return L"��";

    case 4:
        return L"��";

    case 5:
        return L"��";

    case 6:
        return L"��";

    case 7:
        return L"��";

    case 8:
        return L"��";

    case 9:
        return L"��";

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
    case B_KING://�ڽ�
        if(nFromY==nToY)
        {
            str.Format(L"��:��%dƽ%d",nFromX,nToX);
            break;
        }

        if(nFromY>nToY)
            str.Format(L"��:��%d��%d",nFromX,nFromY-nToY);
        else
            str.Format(L"��:��%d��%d",nFromX,nToY-nFromY);

        break;

    case B_CAR://�ڳ�
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
                        str.Format(L"��:�󳵽�%d",nFromY-nToY);
                    else
                        str.Format(L"��:ǰ����%d",nFromY-nToY);
                }
                else
                    str.Format(L"��:��%d��%d",nFromX,nFromY-nToY);
            }
            else
                if(nFromY<nToY)
                {						
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str.Format(L"��:�󳵽�%d",nToY-nFromY);
                        else
                            str.Format(L"��:ǰ����%d",nToY-nFromY);
                    }
                    else
                        str.Format(L"��:��%d��%d",nFromX,nToY-nFromY);
                }
                else
                {
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str.Format(L"��:��ƽ%d",nToX);
                        else
                            str.Format(L"��:ǰ��ƽ%d",nToX);
                    }
                    else
                        str.Format(L"��:��%dƽ%d",nFromX,nToX);

                    break;
                }

                break;

    case B_HORSE://����
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
                        str.Format(L"��:������%d",nToX);
                    else
                        str.Format(L"��:�����%d",nToX);
                }
                else
                {
                    if(nFromY>nToY)
                        str.Format(L"��:ǰ����%d",nToX);
                    else
                        str.Format(L"��:ǰ���%d",nToX);
                }
            }
            else
            {
                if(nFromY>nToY)
                    str.Format(L"��:��%d��%d",nFromX,nToX);
                else
                    str.Format(L"��:��%d��%d",nFromX,nToX);
            }

            break;

    case B_CANON://����	
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
                        str.Format(L"��:���ڽ�%d",nFromY-nToY);
                    else
                        str.Format(L"��:ǰ�ڽ�%d",nFromY-nToY);
                }
                else
                    str.Format(L"��:��%d��%d",nFromX,nFromY-nToY);
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
                                str.Format(L"��:���ڽ�%d",nToY-nFromY);
                            else
                                str.Format(L"��:ǰ�ڽ�%d",nToY-nFromY);
                        }
                        else
                            str.Format(L"��:��%d��%d",nFromX,nToY-nFromY);
                }
                else
                {
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str.Format(L"��:����ƽ%d",nToX);
                        else
                            str.Format(L"��:ǰ��ƽ%d",nToX);
                    }
                    else
                        str.Format(L"��:��%dƽ%d",nFromX,nToX);
                    break;
                }

                break;

    case B_BISHOP://��ʿ
        if(nFromY>nToY)
            str.Format(L"��:ʿ%d��%d",nFromX,nToX);
        else
            str.Format(L"��:ʿ%d��%d",nFromX,nToX);

        break;

    case B_ELEPHANT://����
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
                        str.Format(L"��:������%d",nToX);
                    else
                        str.Format(L"��:�����%d",nToX);
                }
                else
                {
                    if(nFromY>nToY)
                        str.Format(L"��:ǰ����%d",nToX);
                    else
                        str.Format(L"��:ǰ���%d",nToX);
                }
            }
            else
            {
                if(nFromY>nToY)
                    str.Format(L"��:��%d��%d",nFromX,nToX);
                else
                    str.Format(L"��:��%d��%d",nFromX,nToX);
            }

            break;

    case B_PAWN://����
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
                    str.Format(L"��:��%dƽ%d",nFromX,nToX);
                else
                    str.Format(L"��:��%d��%d",nFromX,1);

                break;
            }
            if(nCount==1)
            {
                if(nFromY>nPos[0])
                {
                    if(nFromY==nToY)
                        str.Format(L"��:ǰ��ƽ%d",nToX);
                    else
                        str.Format(L"��:ǰ���%d",1);
                }
                else
                {
                    if(nFromY==nToY)
                        str.Format(L"��:����ƽ%d",nToX);
                    else
                        str.Format(L"��:�����%d",1);
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
                    str.Format(L"��:%d��ƽ%d",j+1,1);
                else
                    str.Format(L"��:%d���%d",j+1,1);

                break;
            }

    case R_KING://��˧
        if(nFromX==nToX)
        {
            if(nFromY>nToY)
                str=L"��:˧"+ConvertDigit2Chinese(10-nFromX)+L"��"+ConvertDigit2Chinese(nFromY-nToY);
            else
                str=L"��:˧"+ConvertDigit2Chinese(10-nFromX)+L"��"+ConvertDigit2Chinese(nToY-nFromY);
        }
        else
            str=L"��:˧"+ConvertDigit2Chinese(10-nFromX)+L"ƽ"+ConvertDigit2Chinese(10-nToX);

        break;

    case R_CAR://�쳵
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
                        str=L"��:ǰ����"+ConvertDigit2Chinese(nFromY-nToY);
                    else
                        str=L"��:�󳵽�"+ConvertDigit2Chinese(nFromY-nToY);
                }
                else
                    str=L"��:��"+ConvertDigit2Chinese(10-nFromX)+L"��"+ConvertDigit2Chinese(nFromY-nToY);
            }
            else
                if(nFromY<nToY)
                {
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str=L"��:����"+ConvertDigit2Chinese(nToY-nFromY);
                        else
                            str=L"��:ǰ����"+ConvertDigit2Chinese(nToY-nFromY);
                    }
                    else
                        str=L"��:��"+ConvertDigit2Chinese(10-nFromX)+L"��"+ConvertDigit2Chinese(nToY-nFromY);
                }
                else
                {
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str=L"��:��ƽ"+ConvertDigit2Chinese(nToX);
                        else
                            str=L"��:ǰ��ƽ"+ConvertDigit2Chinese(nToX);
                    }
                    else
                        str=L"��:��"+ConvertDigit2Chinese(10-nFromX)+L"ƽ"+ConvertDigit2Chinese(10-nToX);

                    break;
                }

                break;

    case R_HORSE://����
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
                        str=L"��:ǰ���"+ConvertDigit2Chinese(10-nToX);
                    else
                        str=L"��:ǰ����"+ConvertDigit2Chinese(10-nToX);
                }
                else
                {
                    if(nFromY>nToY)
                        str=L"��:ǰ���"+ConvertDigit2Chinese(10-nToX);
                    else
                        str=L"��:ǰ����"+ConvertDigit2Chinese(10-nToX);
                }
            }
            else
            {
                if(nFromY>nToY)
                    str=L"��:��"+ConvertDigit2Chinese(10-nFromX)+L"��"+ConvertDigit2Chinese(10-nToX);
                else
                    str=L"��:��"+ConvertDigit2Chinese(10-nFromX)+L"��"+ConvertDigit2Chinese(10-nToX);
            }

            break;

    case R_CANON://����
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
                        str=L"��:ǰ�ڽ�"+ConvertDigit2Chinese(nFromY-nToY);
                    else
                        str=L"��:���ڽ�"+ConvertDigit2Chinese(nFromY-nToY);
                }
                else
                {
                    //str=L"��:��"+ConvertDigit2Chinese(10-nFromX)+L"��"+ConvertDigit2Chinese(nFromY-nToY);
                    str.Format( TEXT("%s%s%s%s"),L"��:��",ConvertDigit2Chinese(10-nFromX),L"��",ConvertDigit2Chinese(nFromY-nToY) );
                }

            }
            else
                if(nFromY<nToY)
                {
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str=L"��:ǰ����"+ConvertDigit2Chinese(nToY-nFromY);
                        else
                            str=L"��:������"+ConvertDigit2Chinese(nToY-nFromY);
                    }
                    else
                        str=L"��:��"+ConvertDigit2Chinese(nFromX)+L"��"+ConvertDigit2Chinese(nToY-nFromY);
                }
                else
                {
                    if(bIsAgain)
                    {
                        if(i>nFromY-1)
                            str=L"��:ǰ��ƽ"+ConvertDigit2Chinese(10-nToX);
                        else
                            str=L"��:����ƽ"+ConvertDigit2Chinese(10-nToX);
                    }
                    else
                        str=L"��:��"+ConvertDigit2Chinese(10-nFromX)+L"ƽ"+ConvertDigit2Chinese(10-nToX);
                }

                break;

    case R_BISHOP://��ʿ
        if(nFromY>nToY)
            str=L"��:ʿ"+ConvertDigit2Chinese(10-nFromX)+L"��"+ConvertDigit2Chinese(10-nToX);
        else
            str=L"��:ʿ"+ConvertDigit2Chinese(10-nFromX)+L"��"+ConvertDigit2Chinese(10-nToX);

        break;

    case R_ELEPHANT://����
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
                        str=L"��:ǰ����"+ConvertDigit2Chinese(10-nToX);
                    else
                        str=L"��:ǰ���"+ConvertDigit2Chinese(10-nToX);
                }
                else
                {
                    if(nFromY>nToY)
                        str=L"��:������"+ConvertDigit2Chinese(10-nToX);
                    else
                        str=L"��:�����"+ConvertDigit2Chinese(10-nToX);
                }
            }
            else
            {
                if(nFromY>nToY)
                    str=L"��:��"+ConvertDigit2Chinese(10-nFromX)+L"��"+ConvertDigit2Chinese(10-nToX);
                else
                    str=L"��:��"+ConvertDigit2Chinese(10-nFromX)+L"��"+ConvertDigit2Chinese(10-nToX);
            }

            break;

    case R_PAWN://���
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
                    str=L"��:��"+ConvertDigit2Chinese(10-nFromX)+L"ƽ"+ConvertDigit2Chinese(10-nToX);
                else
                    str=L"��:��"+ConvertDigit2Chinese(10-nFromX)+L"��"+ConvertDigit2Chinese(1);

                break;
            }
            if(nCount==1)
            {
                if(nFromY-1>nPos[0])
                {
                    if(nFromY==nToY)
                        str=L"��:ǰ��ƽ"+ConvertDigit2Chinese(10-nToX);
                    else
                        str=L"��:ǰ����"+ConvertDigit2Chinese(1);
                }
                else
                {
                    if(nFromY==nToY)
                        str=L"��:���ƽ"+ConvertDigit2Chinese(10-nToX);
                    else
                        str=L"��:�����"+ConvertDigit2Chinese(1);
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
                    str=L"��:"+ConvertDigit2Chinese(j+1)+L"��ƽ"+ConvertDigit2Chinese(nToX);
                else
                    str=L"��:"+ConvertDigit2Chinese(j+1)+L"����"+ConvertDigit2Chinese(1);	
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

    timecount=GetTickCount();//ȡ��ʼʱ��

   ShowStatus( g_MSGArray[PC_THINKING] );

    if(m_nUserChessColor==BLACKCHESS)
        m_iBout++;
    m_bIsThinking=true;
    BOOL bEatChess = FALSE;

    if( m_LastChess!=-1 && m_byChessBoard[m_LastChessY][m_LastChessX] != m_LastChess )
    {
        TRACE(_T("!!!!!!!! Error before pc think:Point(%d,%d) (%d)  ,It must be:%d \r\n"),m_LastChessX,m_LastChessY,m_byChessBoard[m_LastChessY][m_LastChessX] ,m_LastChess );

        //zxj �����޹ʶ�ʧ��BUG���˴��һأ������»���
        m_byChessBoard[m_LastChessY][m_LastChessX] = m_LastChess;
        InvalidateRect();
        GetWindow()->UpdateWindow();
	
    }

    m_pProgressThink->SetPos(0);
    m_pSE->SearchAGoodMove(m_byChessBoard);

    memcpy(m_byShowChessBoard,m_byChessBoard,90);
    memcpy(m_byBackupChessBoard,m_byChessBoard,90);
    m_cmBestMove=m_pSE->GetBestMove();//�õ�����߷�
    m_umUndoMove=m_pSE->GetUndoMove();
    m_stackUndoMove.push(m_umUndoMove);//�������߷�ѹջ
    this->AddChessRecord(m_cmBestMove.From.x+1,m_cmBestMove.From.y+1,m_cmBestMove.To.x+1,m_cmBestMove.To.y+1,m_nUserChessColor,m_cmBestMove.nChessID,FALSE );



    if( m_LastChess!=-1 && m_byChessBoard[m_LastChessY][m_LastChessX] != m_LastChess  
        && m_cmBestMove.To.x!=m_LastChessX && m_cmBestMove.To.y != m_LastChessY )
    {
        TRACE(_T("!!!!!!!! Error In Thinking:Point(%d,%d) (%d)  ,It must be:%d \r\n"),
            m_LastChessX,m_LastChessY,m_byChessBoard[m_LastChessY][m_LastChessX] ,m_LastChess );
    }

  m_pProgressThink->StepToEnd();
    sNodeCount = _T("�����������!");
   ShowStatus(sNodeCount);

    //�ػ���Ļ  
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
    g_GameInfo.iGridWD = 25;		//ÿ��С��ĸ߶�
    g_GameInfo.iGridHT = 25;		//ÿ��С��Ŀ��

    g_GameInfo.iX_START = 11;		//(��һ�����ӵ�X��ʼλ��)
    g_GameInfo.iY_START = 10;		//(��һ�����ӵ�Y��ʼλ��)	

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

    //m_btnRedo.EnableWindow(1);//������尴ť
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
        //�ػ���Ļ  
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
    if(m_bIsThinking)//����������
        return;

    m_iEatCount = 0;
    m_iBout=0;
  //  m_moverecordlist.ClearList();
    m_pmoverecordlist->ClearList();
    ui_play_game_sound(L"gamestart.wav");

    memcpy(m_byChessBoard,InitChessBoard,90);    //��ʼ������
    memcpy(m_byShowChessBoard,InitChessBoard,90);
    memcpy(m_byBackupChessBoard,InitChessBoard,90);
    m_MoveChess.nChessID=NOCHESS;                //����ƶ�����
    m_bIsGameOver=false;

    if(m_nUserChessColor==BLACKCHESS)
        this->InvertChessBoard(m_byChessBoard);

    m_pProgressThink->SetPos(0);

    //ˢ����Ļ
    InvalidateRect();
    GetWindow()->UpdateWindow();
//     InvalidateRect(NULL,FALSE);
//     UpdateWindow();
    m_bIsBegin=false;

    //���ջ
    while(!m_stackUndoMove.empty())
        m_stackUndoMove.pop();
    while(!m_stackRedoMove.empty())
        m_stackRedoMove.pop();

   ShowStatus( g_MSGArray[WELCOME_GAME] );
}

void CChinaChessGame::OnBnClickedBtnClose(BOOL bsound)
{
    if(m_bIsThinking)//����������
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
    m_pSE	=	new CAlphabeta_HHEngine;    //����ʷ������Alpha-Beta��������
    m_pMG	=	new CMoveGenerator;			//�����߷�������
    m_pEvel	=	new CEveluation;			//������ֵ����
    m_pSE->SetSearchDepth(4);				//�趨��������Ϊ3

    m_pSE->SetMoveGenerator(m_pMG);					//�����������趨�߷�������
    m_pSE->SetEveluator(m_pEvel);					//�����������趨��ֵ����
    m_pSE->SetUserChessColor(m_nUserChessColor);	//�趨�û�Ϊ�ڷ���췽
    m_pSE->SetThinkProgress( m_pProgressThink );		//�趨������
    m_MoveChess.nChessID=NOCHESS;//���ƶ����������
}

void CChinaChessGame::ShowStatus( LPCTSTR pText )
{
    if (NULL != m_pStatusStatic)
    {
        m_pStatusStatic->SetText(pText);
    }
}