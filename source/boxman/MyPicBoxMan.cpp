#include "stdafx.h"
#include "MyPicBoxMan.h"



#define  ASSET_CANNOTMOV( POS )  ( (POS!=MAP_BLUEWALL) &&(POS!=MAP_BALL)&&(POS!=MAP_MANBALL) &&(POS!=MAP_MANWALL) )
#define  ASSET_CANMOV( POS )  ( (POS==MAP_BLUEWALL)||(POS==MAP_BALL)||(POS==MAP_MANBALL)||(POS==MAP_MANWALL) )


CMyPicBoxMan::CMyPicBoxMan(   )
:CGameManBoxMan(  )
{
	m_pDCWall = NULL;				//ǽ��
	m_pDCBoxToMove = NULL;			//���ƶ�������
	m_pDCBoxLayer = NULL;			//�Ѿ����ú��˵�����
	m_pDCWay = NULL;				//ͨ��
	m_pDCDest = NULL;				//Ŀ�ĵ�
	m_pDCPicMan = NULL;				//�����ӵ���

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
	CString		StrBmpWall = StrAppDir+ TEXT("Wall.bmp");				//ǽ��
	CString		StrBmpBoxToMove = StrAppDir+ TEXT("Box.bmp");				//���ƶ�������
	CString		StrBmpBoxLayer = StrAppDir+ TEXT("LayBox.bmp");				//�Ѿ����ú��˵�����
	CString		StrBmpBoxWay = StrAppDir+ TEXT("Way.bmp");				//ͨ��
	CString		StrBmpBoxDest = StrAppDir+ TEXT("DestPos.bmp");				//Ŀ�ĵ�
	CString		StrBmpMan = StrAppDir+ TEXT("Man.bmp");				//Ŀ�ĵ�


	m_BDCWall.InitBitmapDC( NULL, StrBmpWall );				//ǽ��
	m_BDCBox.InitBitmapDC( NULL,StrBmpBoxToMove );			//���ƶ�������
	m_BDCBoxLayer.InitBitmapDC( NULL, StrBmpBoxLayer);			//�Ѿ����ú��˵�����
	m_BDCWay.InitBitmapDC( NULL,StrBmpBoxWay );			//ͨ��
	m_BDCDest.InitBitmapDC( NULL,StrBmpBoxDest );			//Ŀ�ĵ�
	m_BDCMan.InitBitmapDC( NULL,StrBmpMan );				//�����ӵ���

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

	//����������ͼ��Ϸ����
	for (i = 0; i < M_TAB_HEIGHT; i++)
	{
		for (j = 0; j < M_TAB_WIDTH; j++)
		{
			x = j * BLOCK_WIDTH;
			y = i * BLOCK_HEIGHT;

			switch (m_cMap[i][j])
			{
				case MAP_BACKGROUP://����
					DrawBackGroup(x, y, pDC);
					break;
				case MAP_WHITEWALL://ǽ
					DrawWhiteWall(x, y, pDC);
					break;
				case MAP_BLUEWALL://ͨ��
					DrawBlueWall(x, y, pDC);
					break;
				case MAP_BALL://Ŀ�ĵ�
					DrawBall(x, y, pDC);
					break;
				case MAP_YELLOWBOX://δ���źõ�����
					DrawYellowBox(x, y, pDC);
					break;
				case MAP_REDBOX://���źõ�����
					DrawRedBox(x, y, pDC);
					break;
				case MAP_MANWALL://����ͨ������
					DrawManWall(x, y, pDC);
					break;
				case MAP_MANBALL://����Ŀ�ĵ�����
					DrawManBall(x, y, pDC);
					break;
			}
		}
	}

}

//��ͼ����
void CMyPicBoxMan::DrawBackGroup(int x, int y, CDC* pDC)
{
	CGameManBoxMan::DrawBackGroup(  x, y, pDC );
}

//ǽ
void CMyPicBoxMan::DrawWhiteWall(int x, int y, CDC* pDC)
{
	if ( NULL!=m_pDCWall )
		pDC->BitBlt( x, y, BLOCK_WIDTH, BLOCK_HEIGHT,m_pDCWall,0,0,SRCCOPY );
	else
		CGameManBoxMan::DrawWhiteWall( x,y,pDC );

}
//ͨ��
void CMyPicBoxMan::DrawBlueWall(int x, int y, CDC* pDC)
{
	if ( NULL!=m_pDCWay )
		pDC->BitBlt( x, y, BLOCK_WIDTH, BLOCK_HEIGHT,m_pDCWay,0,0,SRCCOPY );
	else
		CGameManBoxMan::DrawBlueWall( x,y,pDC );

}
//Ŀ�ĵ�
void CMyPicBoxMan::DrawBall(int x, int y, CDC* pDC)
{
	if ( NULL!=m_pDCDest )
		pDC->BitBlt( x, y, BLOCK_WIDTH, BLOCK_HEIGHT,m_pDCDest,0,0,SRCCOPY );
	else
		CGameManBoxMan::DrawBall( x,y,pDC );

}
//δ���źõ�����
void CMyPicBoxMan::DrawYellowBox(int x, int y, CDC* pDC)
{
	if ( NULL!=m_pDCBoxToMove )
		pDC->BitBlt( x, y, BLOCK_WIDTH, BLOCK_HEIGHT,m_pDCBoxToMove,0,0,SRCCOPY );
	else
		CGameManBoxMan::DrawYellowBox( x,y,pDC );

}
//���źõ�����
void CMyPicBoxMan::DrawRedBox(int x, int y, CDC* pDC)
{
	if ( NULL!=m_pDCBoxLayer )
		pDC->BitBlt( x, y, BLOCK_WIDTH, BLOCK_HEIGHT,m_pDCBoxLayer,0,0,SRCCOPY );
	else
		CGameManBoxMan::DrawRedBox( x,y,pDC );
}

//����ͨ������
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
//����Ŀ�ĵ�����
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


	//�޸ĵ�ͼ��Ϣ
	switch (m_cMap[y2][x2])
	{
	case MAP_BLUEWALL:           //ͨ������������
		m_cMap[y2][x2] = MAP_MANWALL;
		if (m_cMap[yFrom][xFrom] == MAP_MANWALL)
			m_cMap[yFrom][xFrom] = MAP_BLUEWALL;
		else if (m_cMap[yFrom][xFrom] == MAP_MANBALL)
			m_cMap[yFrom][xFrom] = MAP_BALL;
		m_ptManPosition.x = x2;
		m_ptManPosition.y = y2;
		break;

	case MAP_BALL:               //Ŀ�ĵأ���������
		m_cMap[y2][x2] = MAP_MANBALL;
		if (m_cMap[yFrom][xFrom] == MAP_MANWALL)
			m_cMap[yFrom][xFrom] = MAP_BLUEWALL;
		else if (m_cMap[yFrom][xFrom] == MAP_MANBALL)
			m_cMap[yFrom][xFrom] = MAP_BALL;        
		m_ptManPosition.x = x2;
		m_ptManPosition.y = y2;
		break;
	default:           //��ͼ��ƴ���,��Ӧ�ó���
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

//dir ��ʾ��������
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
