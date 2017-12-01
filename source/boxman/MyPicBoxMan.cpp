#include "stdafx.h"
#include "MyPicBoxMan.h"



#define  ASSET_CANNOTMOV( POS )  ( (POS!=MAP_BLUEWALL) &&(POS!=MAP_BALL)&&(POS!=MAP_MANBALL) &&(POS!=MAP_MANWALL) )
#define  ASSET_CANMOV( POS )  ( (POS==MAP_BLUEWALL)||(POS==MAP_BALL)||(POS==MAP_MANBALL)||(POS==MAP_MANWALL) )


CMyPicBoxMan::CMyPicBoxMan(   )
:CGameManBoxMan(  )
{
	m_pDCWall = NULL;				//墙壁
	m_pDCBoxToMove = NULL;			//待移动的箱子
	m_pDCBoxLayer = NULL;			//已经放置好了的箱子
	m_pDCWay = NULL;				//通道
	m_pDCDest = NULL;				//目的地
	m_pDCPicMan = NULL;				//推箱子的人

}

CMyPicBoxMan::~CMyPicBoxMan(void)
{
}

CString CMyPicBoxMan::GetProgromRunFolder()
{
	CString strFilePath;
	CString strFolderPath;

	TCHAR lpFileName[MAX_PATH+1];
	GetModuleFileName(NULL, lpFileName, MAX_PATH);
	strFilePath = lpFileName;

	int nLastIndex = strFilePath.ReverseFind('\\');
	if (nLastIndex!=-1) 
	{
		strFolderPath = strFilePath.Left(nLastIndex+1);
	} 
	else 
	{
		strFolderPath = _T("\\");
	}
	return strFolderPath; 
}

BOOL  CMyPicBoxMan::InitBoxPicRes( LPCTSTR pStrImagePath )
{
	CString		StrAppDir = GetProgromRunFolder() + pStrImagePath;
	StrAppDir  += TEXT("BoxMan\\");
	CString		StrBmpWall = StrAppDir+ TEXT("Wall.bmp");				//墙壁
	CString		StrBmpBoxToMove = StrAppDir+ TEXT("Box.bmp");				//待移动的箱子
	CString		StrBmpBoxLayer = StrAppDir+ TEXT("LayBox.bmp");				//已经放置好了的箱子
	CString		StrBmpBoxWay = StrAppDir+ TEXT("Way.bmp");				//通道
	CString		StrBmpBoxDest = StrAppDir+ TEXT("DestPos.bmp");				//目的地
	CString		StrBmpMan = StrAppDir+ TEXT("Man.bmp");				//目的地


	m_BDCWall.InitBitmapDC( NULL, StrBmpWall );				//墙壁
	m_BDCBox.InitBitmapDC( NULL,StrBmpBoxToMove );			//待移动的箱子
	m_BDCBoxLayer.InitBitmapDC( NULL, StrBmpBoxLayer);			//已经放置好了的箱子
	m_BDCWay.InitBitmapDC( NULL,StrBmpBoxWay );			//通道
	m_BDCDest.InitBitmapDC( NULL,StrBmpBoxDest );			//目的地
	m_BDCMan.InitBitmapDC( NULL,StrBmpMan );				//推箱子的人

	if ( m_BDCWall.IsValid() )		
		m_pDCWall = m_BDCWall.GetDC();

	if ( m_BDCBox.IsValid() )		
		m_pDCBoxToMove = m_BDCBox.GetDC();

	if ( m_BDCBoxLayer.IsValid() )		
		m_pDCBoxLayer = m_BDCBoxLayer.GetDC();

	if ( m_BDCWay.IsValid() )		
		m_pDCWay = m_BDCWay.GetDC();

	if ( m_BDCDest.IsValid() )		
		m_pDCDest = m_BDCDest.GetDC();

	if ( m_BDCMan.IsValid() )		
		m_pDCPicMan = m_BDCMan.GetDC();

	return TRUE;

}

void CMyPicBoxMan::DrawGameArea(CDC* pDC)
{
	int i, j, x, y;

	//绘制整个地图游戏区域
	for (i = 0; i < M_TAB_HEIGHT; i++)
	{
		for (j = 0; j < M_TAB_WIDTH; j++)
		{
			x = j * BLOCK_WIDTH;
			y = i * BLOCK_HEIGHT;

			switch (m_cMap[i][j])
			{
				case MAP_BACKGROUP://背景
					DrawBackGroup(x, y, pDC);
					break;
				case MAP_WHITEWALL://墙
					DrawWhiteWall(x, y, pDC);
					break;
				case MAP_BLUEWALL://通道
					DrawBlueWall(x, y, pDC);
					break;
				case MAP_BALL://目的地
					DrawBall(x, y, pDC);
					break;
				case MAP_YELLOWBOX://未安放好的箱子
					DrawYellowBox(x, y, pDC);
					break;
				case MAP_REDBOX://安放好的箱子
					DrawRedBox(x, y, pDC);
					break;
				case MAP_MANWALL://人在通道区域
					DrawManWall(x, y, pDC);
					break;
				case MAP_MANBALL://人在目的地区域
					DrawManBall(x, y, pDC);
					break;
			}
		}
	}

}

//绘图操作
void CMyPicBoxMan::DrawBackGroup(int x, int y, CDC* pDC)
{
	CGameManBoxMan::DrawBackGroup(  x, y, pDC );
}

//墙
void CMyPicBoxMan::DrawWhiteWall(int x, int y, CDC* pDC)
{
	if ( NULL!=m_pDCWall )
		pDC->BitBlt( x, y, BLOCK_WIDTH, BLOCK_HEIGHT,m_pDCWall,0,0,SRCCOPY );
	else
		CGameManBoxMan::DrawWhiteWall( x,y,pDC );

}
//通道
void CMyPicBoxMan::DrawBlueWall(int x, int y, CDC* pDC)
{
	if ( NULL!=m_pDCWay )
		pDC->BitBlt( x, y, BLOCK_WIDTH, BLOCK_HEIGHT,m_pDCWay,0,0,SRCCOPY );
	else
		CGameManBoxMan::DrawBlueWall( x,y,pDC );

}
//目的地
void CMyPicBoxMan::DrawBall(int x, int y, CDC* pDC)
{
	if ( NULL!=m_pDCDest )
		pDC->BitBlt( x, y, BLOCK_WIDTH, BLOCK_HEIGHT,m_pDCDest,0,0,SRCCOPY );
	else
		CGameManBoxMan::DrawBall( x,y,pDC );

}
//未安放好的箱子
void CMyPicBoxMan::DrawYellowBox(int x, int y, CDC* pDC)
{
	if ( NULL!=m_pDCBoxToMove )
		pDC->BitBlt( x, y, BLOCK_WIDTH, BLOCK_HEIGHT,m_pDCBoxToMove,0,0,SRCCOPY );
	else
		CGameManBoxMan::DrawYellowBox( x,y,pDC );

}
//安放好的箱子
void CMyPicBoxMan::DrawRedBox(int x, int y, CDC* pDC)
{
	if ( NULL!=m_pDCBoxLayer )
		pDC->BitBlt( x, y, BLOCK_WIDTH, BLOCK_HEIGHT,m_pDCBoxLayer,0,0,SRCCOPY );
	else
		CGameManBoxMan::DrawRedBox( x,y,pDC );
}

//人在通道区域
void CMyPicBoxMan::DrawManWall(int x, int y, CDC* pDC)
{
	if (NULL!=m_pDCPicMan)
	{
		DrawBlueWall( x,  y,  pDC );
		TransparentBlt(pDC->m_hDC,x,y,BLOCK_WIDTH, BLOCK_HEIGHT,
						m_pDCPicMan->m_hDC,0,0,BLOCK_WIDTH,BLOCK_HEIGHT,m_clManTrans );
	}
	else
		CGameManBoxMan::DrawManWall( x, y,pDC );

}
//人在目的地区域
void CMyPicBoxMan::DrawManBall(int x, int y, CDC* pDC)
{
	if (NULL!=m_pDCPicMan)
	{
		DrawBall( x,  y, pDC );
		TransparentBlt(pDC->m_hDC,x,y,BLOCK_WIDTH, BLOCK_HEIGHT,
					m_pDCPicMan->m_hDC,0,0,BLOCK_WIDTH,BLOCK_HEIGHT,m_clManTrans );
	}
	else
	{
		CGameManBoxMan::DrawManBall( x,y,pDC );
	}
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//
void CMyPicBoxMan::ManJump( int xFrom,int yFrom,int x2,int y2 )
{
	MovStep mstep;
	mstep.m_manFromPt.SetPoint( xFrom,yFrom );	
	mstep.m_ManFromType = m_cMap[yFrom][xFrom];
	mstep.m_MovDirection = MY_VK_JUMP;
	mstep.m_ManToType = m_cMap[y2][x2];;


	mstep.m_BoxPtFrom.SetPoint( x2,y2 );			
	mstep.m_BoxFromType = 0;
	mstep.m_BoxToType = 0;


	//修改地图信息
	switch (m_cMap[y2][x2])
	{
	case MAP_BLUEWALL:           //通道，可以行走
		m_cMap[y2][x2] = MAP_MANWALL;
		if (m_cMap[yFrom][xFrom] == MAP_MANWALL)
			m_cMap[yFrom][xFrom] = MAP_BLUEWALL;
		else if (m_cMap[yFrom][xFrom] == MAP_MANBALL)
			m_cMap[yFrom][xFrom] = MAP_BALL;
		m_ptManPosition.x = x2;
		m_ptManPosition.y = y2;
		break;

	case MAP_BALL:               //目的地，可以行走
		m_cMap[y2][x2] = MAP_MANBALL;
		if (m_cMap[yFrom][xFrom] == MAP_MANWALL)
			m_cMap[yFrom][xFrom] = MAP_BLUEWALL;
		else if (m_cMap[yFrom][xFrom] == MAP_MANBALL)
			m_cMap[yFrom][xFrom] = MAP_BALL;        
		m_ptManPosition.x = x2;
		m_ptManPosition.y = y2;
		break;
	default:           //地图设计错误,不应该出现
		AfxMessageBox(_T("wrong !!!"));
		break;

	}
	m_stackRedoMove.push( mstep );
}

BOOL	CMyPicBoxMan::IsPositionValid( int xEnd,int yEnd )
{
	if ( xEnd<0 || xEnd<0 || xEnd>=M_TAB_WIDTH || xEnd>=M_TAB_HEIGHT )
		return FALSE;
	if ( ASSET_CANNOTMOV( m_cMap[yEnd][xEnd] ) )
		return FALSE;
	return TRUE;
}

BOOL	CMyPicBoxMan::FindNearPos( int xStart,int yStart,int xEnd,int yEnd,TCHAR &KeyDirect )
{
	BOOL br = FALSE;
	int xSub,ySub;
	xSub = abs(xStart-xEnd);
	ySub = abs(yStart-yEnd);
	if ( ySub>1 || xSub>1 )
		return FALSE;
	if ( ySub==1 && xSub==1 )
		return FALSE;

	if ( xStart+1 == xEnd )
	{
		br = TRUE;
		KeyDirect = VK_RIGHT;
	}	
	else if ( xStart-1 == xEnd )
	{
		br = TRUE;
		KeyDirect = VK_LEFT;
	}	
	if ( yStart+1 == yEnd )
	{
		br = TRUE;
		KeyDirect = VK_DOWN;
	}	
	if ( yStart-1 == yEnd )
	{
		br = TRUE;
		KeyDirect = VK_UP ;
	}	
	return br;
	
}

//dir 表示搜索方向
bool CMyPicBoxMan::search( int xStart,int yStart,int xEnd,int yEnd )
{
	SearchStep step = { xStart,yStart,TRUE,TRUE,TRUE,TRUE };
	stack<SearchStep> SearchQueue;
	SearchQueue.push( step );

	//BOOL bRight; BOOL bLeft; BOOL bUp; BOOL	bDown;

    memset( m_bSerChMap,0,sizeof(m_bSerChMap) );

	SearchStep SCurStep;
	int xCur,yCur;
	while( !SearchQueue.empty() )
	{
		SCurStep = SearchQueue.top( );
		SearchQueue.pop();
		
		RETAILMSG( 1,(L"UnStep=%d,  (%d,%d)\r\n",SearchQueue.size(),SCurStep.x,SCurStep.y ) );

		if ( SCurStep.bRight )
		{
			xCur = SCurStep.x+1;
			yCur = SCurStep.y;
			m_bSerChMap[SCurStep.y][SCurStep.x] = TRUE;

			if ( xCur==xEnd && yCur==yEnd )
			{
				while(!SearchQueue.empty()) 
					SearchQueue.pop();
				return TRUE;
			}
			if ( !( xCur<0 || yCur<0 || xCur>=M_TAB_WIDTH || yCur>=M_TAB_HEIGHT ) )
			{
				if ( ASSET_CANMOV(m_cMap[yCur][xCur])  && !m_bSerChMap[yCur][xCur] )
				{
					SearchStep step = { xCur,yCur,TRUE,FALSE,TRUE,TRUE };	
					SearchQueue.push( step );
				}
			}
		}

		if ( SCurStep.bLeft )
		{
			xCur = SCurStep.x-1;
			yCur = SCurStep.y;
			if ( xCur==xEnd  && yCur==yEnd )
			{
				while(!SearchQueue.empty()) 
					SearchQueue.pop();
				return TRUE;
			}

			if ( !( xCur<0 || yCur<0 || xCur>=M_TAB_WIDTH || yCur>=M_TAB_HEIGHT ) )
			{
				if ( ASSET_CANMOV(m_cMap[yCur][xCur])  && !m_bSerChMap[yCur][xCur] ){
					SearchStep step = { xCur,yCur,FALSE,TRUE,TRUE,TRUE };	
					SearchQueue.push( step );
				}
			}
		}

		if ( SCurStep.bUp )
		{
			xCur = SCurStep.x;
			yCur = SCurStep.y-1;
			if ( xCur==xEnd  && yCur==yEnd )
			{
				while(!SearchQueue.empty()) 
					SearchQueue.pop();
				return TRUE;
			}

			if ( !( xCur<0 || yCur<0 || xCur>=M_TAB_WIDTH || yCur>=M_TAB_HEIGHT ) )
			{
				if ( ASSET_CANMOV(m_cMap[yCur][xCur])  && !m_bSerChMap[yCur][xCur] ){
					SearchStep step = { xCur,yCur,TRUE,TRUE,TRUE,FALSE };	
					SearchQueue.push( step );
				}
			}
		}

		if ( SCurStep.bDown )
		{
			xCur = SCurStep.x;
			yCur = SCurStep.y+1;
			if ( xCur==xEnd  && yCur==yEnd )
			{
				while(!SearchQueue.empty()) 
					SearchQueue.pop();
				return TRUE;
			}


			if ( !( xCur<0 || yCur<0 || xCur>=M_TAB_WIDTH || yCur>=M_TAB_HEIGHT ) )
			{
				if ( ASSET_CANMOV(m_cMap[yCur][xCur])  && !m_bSerChMap[yCur][xCur] ){
					SearchStep step = { xCur,yCur,TRUE,TRUE,FALSE,TRUE };	
					SearchQueue.push( step );
				}
			}
		}
	}

	while(!SearchQueue.empty()) 
		SearchQueue.pop();
	
	return FALSE;
}
