#include "StdAfx.h"
#include "WceUiButton.h"
#include "WceUiRealWnd.h"
#include "LinkGame.h"
#include <assert.h>
#include <vector>
#include "iostream"
#include <string>
#include "uibase.h"
using namespace std;


/////////////////////////////////////////////////////////////////////////////
//��Ϸ��ز���
GAMEINFO	g_GameInfoSet;


#define X_OFFSET				g_GameInfoSet.bmpxOffset
#define Y_OFFSET				g_GameInfoSet.bmpyOffset

#define BKCOLOR					g_GameInfoSet.clGameBk					//������ɫ
#define FRONTWIDTH				g_GameInfoSet.iOneFrontWidth			//ǰ�淽��Ŀ��
#define FRONTHEIGHT				g_GameInfoSet.iOneFrontHeight			//ǰ�淽��ĸ߶�
#define BKWIDTH					g_GameInfoSet.iOneBkWidth				//��������Ŀ��
#define BKHEIGHT				g_GameInfoSet.iOntBkHeight				//��������ĸ߶�

#define X_START 				g_GameInfoSet.iGameAreaLeft
#define Y_START 				g_GameInfoSet.iGameAreaTop
#define ROWCOUNT				g_GameInfoSet.iRowCount					//����
#define COLCOUNT				g_GameInfoSet.iColumnCount				//����
#define GAME_AREA_WIDTH			g_GameInfoSet.szGameArea.cx				//��Ϸ������
#define GAME_AREA_HEIGHT		g_GameInfoSet.szGameArea.cy				//��Ϸ����߶�


#define BLANK_STATE     -1                  //�շ���(û���κζ���)

CLinkGame::CLinkGame(void)
{
	m_pbackbmp   = NULL;
	m_pfrontbmp  = NULL;

	//��¼������Ϊ��Ч״̬
	m_nY1= BLANK_STATE;
	m_nX1= BLANK_STATE;

	//��ʼ��������
	m_nRow=ROWCOUNT;
	m_nCol=COLCOUNT;

	//������������̬�����ں���������ռ�
	//m_map=new int[m_nRow*m_nCol];
	memset( m_map,BLANK_STATE,sizeof(m_map) );


	m_iNowTick = MAX_TIME_EACH;
	m_iNowLevel = MAX_TIME_EACH/60;
	m_bGameEnd = FALSE;
	m_TimerNewGame = 0;
	// m_bFirstPaint = TRUE;
	m_iTickCount = 0;
	m_iSortCount = MAX_SORT_COUNT;		//û����Ϸ�������

	m_pSortBtn = NULL;
	m_ptimeStatic = NULL;
	SetDefaultGameInfo();
}

CLinkGame::~CLinkGame(void)
{

}

BOOL CLinkGame::IsLink(int x1, int y1, int x2, int y2)
{
	//Xֱ����ʽ
	if(x1==x2)
	{
		if(X1_Link_X2(x1,y1,y2))
			return TRUE;
	}
	//Yֱ����ʽ
	else if(y1==y2)
	{
		if(Y1_Link_Y2(x1,x2,y1))
			return TRUE;
	}

	//һ��ת��ֱ�ǵ���ͨ��ʽ
	if(OneCornerLink(x1,y1,x2,y2))
	{
		return TRUE;
	}
	//����ת��ֱ�ǵ���ͨ��ʽ
	else if(TwoCornerLink(x1,y1,x2,y2))
	{
		return TRUE;
	}
	return FALSE;
}

//   ����Ƿ��Ѿ�Ӯ������Ϸ
//
BOOL CLinkGame::IsWin(void)
{
	//��������Ƿ����з�δ�������ķ���
	// (��BLANK_STATE״̬)
	for(int i=0;i<m_nRow;i++)
	{
		for( int j=0;j<m_nCol;j++)
		{
			if(m_map[i][j] != BLANK_STATE)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

//
//Xֱ����ͨ
//
BOOL CLinkGame::X1_Link_X2(int x, int y1,int y2)
{
	//��֤y1��ֵС��y2
	if(y1>y2)
	{
		//���ݽ���
		int n=y1;
		y1=y2;
		y2=n;
	}

	//ֱͨ 	
	for(int i=y1+1;i<=y2;i++)
	{
		if(i==y2)
			return TRUE;
		//if(m_map[i*m_nCol+x]!=BLANK_STATE)
		if( m_map[i][x]!=BLANK_STATE )
			break;
	}
	//��ͨ
	if(XThrough(x-1,y1,FALSE)&&XThrough(x-1,y2,FALSE))
		return TRUE;
	//��ͨ
	if(XThrough(x+1,y1,TRUE)&&XThrough(x+1,y2,TRUE))
		return TRUE;
	return FALSE;
}

//
//Yֱ����ͨ
//
BOOL CLinkGame::Y1_Link_Y2(int x1,int x2,int y)
{
	if(x1>x2)
	{
		int x=x1;
		x1=x2;
		x2=x;
	}
	//ֱͨ
	for(int i=x1+1;i<=x2;i++)
	{
		if(i==x2)
			return TRUE;
		//if(m_map[y*m_nCol+i]!=BLANK_STATE)
		if( m_map[y][i]!=BLANK_STATE )
			break;
	}
	//��ͨ
	if(YThrough(x1,y-1,FALSE)&&YThrough(x2,y-1,FALSE))
		return TRUE;
	//��ͨ
	if(YThrough(x1,y+1,TRUE)&&YThrough(x2,y+1,TRUE))
		return TRUE;
	return FALSE;
}

//
// �Ƿ�ͬһֱ��ͨ
//
BOOL CLinkGame::LineX(int x,int y1,int y2)
{
	if(y1>y2)
	{
		int y=y1;
		y1=y2;
		y2=y;
	}
	for(int y=y1;y<=y2;y++)
	{
		//if(m_map[y*m_nCol+x]!=BLANK_STATE)
		if( m_map[y][x]!=BLANK_STATE )
			return FALSE;
		if(y==y2)
			return TRUE;
	}
	return FALSE;
}

//
// �Ƿ�ͬһֱ��ͨ
//
BOOL CLinkGame::LineY(int x1,int x2,int y)
{
	if(x1>x2)
	{
		int x=x1;
		x1=x2;
		x2=x;
	}
	for(int x=x1;x<=x2;x++)
	{
		//if(m_map[y*m_nCol+x]!=BLANK_STATE)
		if( m_map[y][x]!=BLANK_STATE )
			return FALSE;
		if(x==x2)
			return TRUE;
	}
	return FALSE;
}

//
//  1ֱ�ǽӿ���ͨ
//
BOOL CLinkGame::OneCornerLink(int x1, int y1,int x2, int y2)
{
	if(x1>x2)
	{
		int n=x1;
		x1=x2;
		x2=n;
		n=y1;
		y1=y2;
		y2=n;
	}
	if(y2<y1)
	{
		if(LineY(x1+1,x2,y1)&&LineX(x2,y1,y2+1))
			return TRUE;
		if(LineY(x2-1,x1,y2)&&LineX(x1,y2,y1-1))
			return TRUE;
		return FALSE;
	}	
	else
	{
		if(LineY(x1+1,x2,y1)&&LineX(x2,y1,y2-1))
			return TRUE;
		if(LineY(x2-1,x1,y2)&&LineX(x1,y2,y1+1))
			return TRUE;		
		return FALSE;
	}
	return FALSE;
}

//
//  2ֱ�ǽӿ���ͨ
//
BOOL CLinkGame::TwoCornerLink(int x1, int y1, int x2, int y2)
{
	int x;
	int y;
	if(x1>x2)
	{
		int n=x1;
		x1=x2;
		x2=n;
		n=y1;
		y1=y2;
		y2=n;
	}
	//��ͨ
	if(XThrough(x1+1,y1,TRUE)&&XThrough(x2+1,y2,TRUE))
		return TRUE;
	//��ͨ
	if(XThrough(x1-1,y1,FALSE)&&XThrough(x2-1,y2,FALSE))
		return TRUE;
	//��ͨ
	if(YThrough(x1,y1-1,FALSE)&&YThrough(x2,y2-1,FALSE))
		return TRUE;
	//��ͨ
	if(YThrough(x1,y1+1,TRUE)&&YThrough(x2,y2+1,TRUE))
		return TRUE;
	//��
	for(x=x1+1;x<m_nCol;x++)
	{
		//if(m_map[y1*m_nCol+x]>-1)
		if( m_map[y1][x]!=BLANK_STATE )
			break;
		if(OneCornerLink(x,y1,x2,y2))
			return TRUE;
	}
	//��
	for(x=x1-1;x>-1;x--)
	{
		//if(m_map[y1*m_nCol+x]!=BLANK_STATE)
		if( m_map[y1][x]!=BLANK_STATE )
			break;
		if(OneCornerLink(x,y1,x2,y2))
			return TRUE;
	}
	//��
	for(y=y1-1;y>-1;y--)
	{
		//if(m_map[y*m_nCol+x1]!=BLANK_STATE)
		if( m_map[y][x1]!=BLANK_STATE )
			break;
		if(OneCornerLink(x1,y,x2,y2))
			return TRUE;
	}
	//��
	for(y=y1+1;y<m_nRow;y++)
	{
		//if( m_map[y*m_nCol+x1]!=BLANK_STATE )
		if( m_map[y][x1]!=BLANK_STATE )
			break;
		if(OneCornerLink(x1,y,x2,y2))
			return TRUE;
	}

	return FALSE;
}

BOOL CLinkGame::XThrough(int x, int y, BOOL bAdd)
{
	if(bAdd)
	{
		for(int i=x;i<m_nCol;i++)
		{
			//if(m_map[y*m_nCol+i]!=BLANK_STATE)
			if( m_map[y][i]!=BLANK_STATE )
				return FALSE;
		}
	}
	else
	{
		for(int i=0;i<=x;i++)
		{
			//if(m_map[y*m_nCol+i]!=BLANK_STATE)
			if( m_map[y][i]!=BLANK_STATE )
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CLinkGame::YThrough(int x, int y,BOOL bAdd)
{
	if(bAdd)
	{
		for(int i=y;i<m_nRow;i++)
		{
			//if(m_map[i*m_nCol+x]!=BLANK_STATE)
			if( m_map[i][x]!=BLANK_STATE )
				return FALSE;
		}
	}
	else
	{
		for(int i=0;i<=y;i++)
		{
			//if(m_map[i*m_nCol+x]!=BLANK_STATE)
			if( m_map[i][x]!=BLANK_STATE )
				return FALSE;
		}
	}
	return TRUE;
}

//����Ϸ����SORT
/*******************************************************************
* Function Name �� 
* Remark        ��//ˮƽ���� 
* Update Record �� 
*               �� 
* Params        �� 
* Return Value  �� 
* Author        �� Zhxj
* Emaile        �� shenzhenit@163.com
* Date Created  �� 2009��7��30�� ������
*******************************************************************/
BOOL CLinkGame::HorSortAnimalPlace( int lineIndex,BOOL bMoveLeft )
{
	if ( lineIndex<0 || lineIndex>=m_nRow )
		return FALSE;

	srand( GetTickCount() );

	//������ƥ��ɶԵĶ������ַŽ�һ����ʱ�ĵ�ͼ��
	vector <int> tmpMap;	
	for(int i=0;i<m_nCol;i++)
	{
		if ( m_map[lineIndex][i]!=BLANK_STATE )
			tmpMap.push_back( m_map[lineIndex][i] );
	}

	//ÿ�δ��������ʱ��ͼ��ȡ��(��ȡ������ʱ��ͼɾ��)
	//һ������ŵ���ͼ�Ŀշ�����

	if ( bMoveLeft )		//ȫ���ƶ������
	{
		for(int i=0;i<m_nCol;i++)
			m_map[lineIndex][i]= BLANK_STATE;
		int nCurCount = tmpMap.size();
		for( int i=0;i<nCurCount;i++ )
		{
			//�����ѡһ��λ��
			int nIndex=(int(rand()*0.1+rand()*0.01+rand()))%tmpMap.size();
			//��ȡ��ѡ������ŵ���ͼ�Ŀշ���
			m_map[lineIndex][i] = tmpMap.at(nIndex);
			//����ʱ��ͼ��ȥ�ö���
			tmpMap.erase(tmpMap.begin()+nIndex);
		}
	}
	else
	{
		for(int i=0;i<m_nCol;i++)
		{
			//�����ѡһ��λ��
			if ( m_map[lineIndex][i]!=BLANK_STATE )
			{
				int nIndex=(int(rand()*0.1+rand()*0.01+rand()))%tmpMap.size();
				//��ȡ��ѡ������ŵ���ͼ�Ŀշ���
				m_map[lineIndex][i] = tmpMap.at(nIndex);
				//����ʱ��ͼ��ȥ�ö���
				tmpMap.erase(tmpMap.begin()+nIndex);
			}
		}
	}

	tmpMap.clear();

	//������ʾ
	//DrawGameArea();
	return TRUE;	
}

/*******************************************************************
* Function Name �� 
* Remark        �� //��ֱ����
* Update Record �� 
*               �� 
* Params        �� 
* Return Value  �� 
* Author        �� Zhxj
* Emaile        �� shenzhenit@163.com
* Date Created  �� 2009��7��30�� ������
*******************************************************************/
BOOL	CLinkGame::VerSortAnimalPlace( int ColumnIndex,BOOL bMoveBottom )
{
	if ( ColumnIndex<0 || ColumnIndex>=m_nCol )
		return FALSE;

	srand( GetTickCount() );

	//������ƥ��ɶԵĶ������ַŽ�һ����ʱ�ĵ�ͼ��
	vector <int> tmpMap;	
	for(int i=0;i<m_nRow;i++)
	{
		if ( m_map[i][ColumnIndex]!=BLANK_STATE )
			tmpMap.push_back( m_map[i][ColumnIndex] );
	}

	//ÿ�δ��������ʱ��ͼ��ȡ��(��ȡ������ʱ��ͼɾ��)
	//һ������ŵ���ͼ�Ŀշ�����

	if ( bMoveBottom )		//ȫ���ƶ������
	{
		for(int i=0;i<m_nRow;i++)
			m_map[i][ColumnIndex]= BLANK_STATE;
		int nCurCount = tmpMap.size();
		for( int i=0;i<nCurCount;i++ )
		{
			//�����ѡһ��λ��
			int nIndex=(int(rand()*0.1+rand()*0.01+rand()))%tmpMap.size();
			//��ȡ��ѡ������ŵ���ͼ�Ŀշ���
			if ( m_nRow-i-1 <0 )
			{
				//������  ������������ܵ�������
				MessageBox(GetWindow()->m_hWnd,TEXT("Row index mustn't <0"),NULL,MB_OK );
			}
			m_map[m_nRow-i-1][ColumnIndex] = tmpMap.at(nIndex);
			//����ʱ��ͼ��ȥ�ö���
			tmpMap.erase(tmpMap.begin()+nIndex);
		}
	}
	else
	{
		for(int i=0;i<m_nRow;i++)
		{
			//�����ѡһ��λ��
			if ( m_map[i][ColumnIndex]!=BLANK_STATE )
			{
				int nIndex=(int(rand()*0.1+rand()*0.01+rand()))%tmpMap.size();
				//��ȡ��ѡ������ŵ���ͼ�Ŀշ���
				m_map[i][ColumnIndex] = tmpMap.at(nIndex);
				//����ʱ��ͼ��ȥ�ö���
				tmpMap.erase(tmpMap.begin()+nIndex);
			}
		}
	}

	//     tmpMap.RemoveAll();
	//     tmpMap.FreeExtra();
	tmpMap.clear();

	//������ʾ
	//DrawGameArea();
	return TRUE;	
}

/*******************************************************************
* Function Name �� 
* Remark        �� //ȫ��SORT
* Update Record �� 
*               �� 
* Params        �� 
* Return Value  �� 
* Author        �� Zhxj
* Emaile        �� shenzhenit@163.com
* Date Created  �� 2009��7��30�� ������
*******************************************************************/
BOOL    CLinkGame::AllSortAnimalPlace()
{
	srand( GetTickCount() );

	//������ƥ��ɶԵĶ������ַŽ�һ����ʱ�ĵ�ͼ��
	vector <int> tmpMap;	
	int i;
	for(i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol;j++)
		{
			if ( m_map[i][j]!=BLANK_STATE )
				tmpMap.push_back( m_map[i][j] );
		}
	}

	//ÿ�δ��������ʱ��ͼ��ȡ��(��ȡ������ʱ��ͼɾ��)
	//һ������ŵ���ͼ�Ŀշ�����
	for(i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol;j++)
		{
			//�����ѡһ��λ��
			if (  m_map[i][j]!=BLANK_STATE )
			{
				int nIndex=(int(rand()*0.1+rand()*0.01+rand()))%tmpMap.size();
				//��ȡ��ѡ������ŵ���ͼ�Ŀշ���
				m_map[i][j] = tmpMap.at(nIndex);
				//����ʱ��ͼ��ȥ�ö���
				tmpMap.erase(tmpMap.begin()+nIndex);
			}
		}
	}

	//tmpMap.RemoveAll();
	//tmpMap.FreeExtra();
	tmpMap.clear();
	return TRUE;
}

void CLinkGame::OnDraw( CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw )
{
	RECT rc = {0,0,0,0};
	// draw gameover
	if( m_bGameEnd && m_TimerNewGame!=0 )
	{
		rc.left = (WceUiGetScreenWidth() - m_pgameoverbmp->GetWidth())/2;
		rc.top = (WceUiGetScreenHeight() - m_pgameoverbmp->GetHeight())/2;
		rc.right = rc.left + m_pgameoverbmp->GetWidth();
		rc.bottom = rc.top + m_pgameoverbmp->GetHeight();
//by lzy//		wceui::wceglDrawTexture(m_pgameoverbmp, &rc);

		BLENDFUNCTION bf;
		memset(&bf, 0, sizeof(BLENDFUNCTION));
		bf.AlphaFormat = AC_SRC_ALPHA;
		BITMAP bmpinfo = m_pgameoverbmp->GetBitMapInfo();
		pWndBitmap->AlphaBlend(rc.left,rc.top,bmpinfo.bmWidth,bmpinfo.bmHeight,
			m_pgameoverbmp,0 ,0,bf);
		return;
	}


	int xoff = (BKWIDTH-FRONTWIDTH)/2;
	int yoff = (BKHEIGHT-FRONTHEIGHT)/2;
	for(int i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol;j++)
		{			
			if(m_map[i][j]==BLANK_STATE)
				continue;

			//��������
// 			rc.left = j*BKWIDTH+X_START;
// 			rc.top = i*BKHEIGHT+Y_START,BKWIDTH;
// 			rc.right = rc.left + BKWIDTH;
// 			rc.bottom = rc.top + BKHEIGHT;
// 
 			RECT src;
// 			src.left = 0;
// 			src.top = BKHEIGHT*2;
// 			src.right = src.left + BKWIDTH;
// 			src.bottom = src.top + BKHEIGHT;
// 
// 			wceui::wceglDrawTexture(m_pbackbmp, &rc, &src);

			//����
			rc.left = j*BKWIDTH+X_START+xoff;
			rc.top = i*BKHEIGHT+Y_START+yoff;
			rc.right = rc.left + FRONTWIDTH;
			rc.bottom = rc.top + FRONTHEIGHT;

			src.left = FRONTWIDTH;
			src.top = m_map[i][j]*(FRONTHEIGHT);
			src.right = src.left + FRONTWIDTH;
			src.bottom = src.top + FRONTHEIGHT;

// 			wceui::wceglDrawTexture(m_pfrontbmp, &rc, &src);
			src.left = 0;
			src.right = src.left + FRONTWIDTH;
//			wceui::wceglFillSolidRect(&rc, RGB(0,0,0));
//by lzy//	wceui::wceglDrawTexture(m_pfrontbmp, &rc, &src);
			pWndBitmap->BitBlt(rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,m_pfrontbmp,src.left ,src.top,SRCCOPY);
		}
	}

	if (m_nX1 != BLANK_STATE || m_nY1 != BLANK_STATE)
	{
		rc.left  =   m_nX1*BKWIDTH+X_START; 
		rc.top   =   m_nY1*BKHEIGHT+Y_START;
		rc.right =   rc.left + BKWIDTH;
		rc.bottom=   rc.top + BKHEIGHT;

//by lzy//wceui::wceglDrawTexture(m_bmpSelect.GetBitmap(), &rc);

		BLENDFUNCTION bf;
		memset(&bf, 0, sizeof(BLENDFUNCTION));
		bf.AlphaFormat = AC_SRC_ALPHA;
		BITMAP bmpinfo = m_bmpSelect.GetBitmap()->GetBitMapInfo();
		pWndBitmap->AlphaBlend(rc.left,rc.top,bmpinfo.bmWidth,bmpinfo.bmHeight,
			m_bmpSelect.GetBitmap(),0 ,0,bf);
	}

}

void CLinkGame::SetProperties( PCWceUiXmlElem pCreateElem )
{
	CWceUiLayer::SetProperties(pCreateElem);

	LPCTSTR pGamerc         = pCreateElem->GetAttr(L"gamerc");
	LPCTSTR pRownum         = pCreateElem->GetAttr(L"rownum");
	LPCTSTR pColnum         = pCreateElem->GetAttr(L"colnum");
	LPCTSTR pAnitotal       = pCreateElem->GetAttr(L"anitotal");
	LPCTSTR pSelect			= pCreateElem->GetAttr(L"selectbmp");
	LPCTSTR pWdselframe     = pCreateElem->GetAttr(L"wdselframe");
	LPCTSTR pBackanisz      = pCreateElem->GetAttr(L"backanisz");
	LPCTSTR pFrontanisz     = pCreateElem->GetAttr(L"frontanisz");
	LPCTSTR pFrontbmp       = pCreateElem->GetAttr(L"frontbmp");
	LPCTSTR pBackbmp        = pCreateElem->GetAttr(L"backbmp");
	LPCTSTR pGameoverbmp    = pCreateElem->GetAttr(L"gameoverbmp");

	if (NULL != pGamerc)
	{
		RECT rcGameArea = {0};
		rcGameArea = GetRectByStr(pGamerc); 
		g_GameInfoSet.iGameAreaLeft = rcGameArea.left;			//��Ϸ��ʼλ��
		g_GameInfoSet.iGameAreaTop = rcGameArea.top;				//��Ϸ��ʼλ��-������
	}

	if (NULL != pRownum)
	{
		g_GameInfoSet.iRowCount = _ttoi(pRownum);				//��Ϸ������
	}
	if (NULL != pColnum)
	{
		g_GameInfoSet.iColumnCount = _ttoi(pColnum);			//����	
	}  
	m_nRow=ROWCOUNT;
	m_nCol=COLCOUNT;
	if (NULL != pAnitotal)
	{
		g_GameInfoSet.iTotalAnimal = _ttoi(pAnitotal);
		if ( g_GameInfoSet.iTotalAnimal==0 )
			g_GameInfoSet.iTotalAnimal = 21;
	}

	m_nLevelAni = g_GameInfoSet.iTotalAnimal;


	if (NULL != pSelect)
	{
		m_bmpSelect.SetBitmapFromRes(pSelect);
	}

	if (NULL != pWdselframe)
	{
		g_GameInfoSet.usSelFrameWD = _ttoi(pWdselframe);
	}

	SIZE szTemp ={0};
	if (NULL != pBackanisz)
	{
		szTemp = GetSizeByStr( pBackanisz);
		g_GameInfoSet.iOneBkWidth =  szTemp.cx;			//��������Ŀ��	ʵ��һ������Ŀ��
		g_GameInfoSet.iOntBkHeight = szTemp.cy;			//��������ĸ߶�
	}

	if (NULL != pFrontanisz)
	{
		szTemp = GetSizeByStr( pFrontanisz);
		g_GameInfoSet.iOneFrontWidth = szTemp.cx;			//ǰ�淽��Ŀ��  
		g_GameInfoSet.iOneFrontHeight = szTemp.cy;			//ǰ�淽��ĸ߶�
	}

	if (NULL  != pBackbmp)
	{
		m_bmpbblock.SetBitmapFromRes(pBackbmp);
		m_pbackbmp = m_bmpbblock.GetBitmap();
	}
	if (NULL != pFrontbmp)
	{
		m_bmpblock.SetBitmapFromRes(pFrontbmp);
		m_pfrontbmp = m_bmpblock.GetBitmap();

	}
	if (NULL !=  pGameoverbmp)
	{
		m_bmpgameoverblock.SetBitmapFromRes(pGameoverbmp);
		m_pgameoverbmp = m_bmpgameoverblock.GetBitmap();
	}

}

void CLinkGame::OnInitLayer()
{
	CWceUiLayer::OnInitLayer();
}

void CLinkGame::OnLButtonDown( UINT nFlags, POINT point )
{
	BOOL bLinked = FALSE;
	if ( m_bGameEnd )
		return;

	if ( point.x < X_START ||  point.y < Y_START )
		return ;

	//1.�������������ĵ�λ��
	int xPt = point.x - X_START;
	int yPt = point.y - Y_START;	
	int x=xPt/BKWIDTH+(xPt%BKWIDTH?1:0)-1;
	int y=yPt/BKHEIGHT+(yPt%BKHEIGHT?1:0)-1;

	//2.����Ϸ�����ڲ��Ҹ������и������ǿյ�����
	//if(x<m_nCol&&y<m_nRow&&m_map[y*m_nCol+x]!= BLANK_STATE)
	if( x>=0&&y>=0&& x<m_nCol && y<m_nRow && m_map[y][x]!= BLANK_STATE )
	{
		//3.������δ��¼��һ������
		if(m_nX1==BLANK_STATE)
		{


			//4.��¼��һ�������λ��
			m_nX1=x;
			m_nY1=y;
			RECT rc = {m_nX1*BKWIDTH+X_START,m_nY1*BKHEIGHT+Y_START,m_nX1*BKWIDTH+X_START+BKWIDTH,m_nY1*BKHEIGHT+Y_START+BKHEIGHT};
			InvalidateRect(&rc);
			UpdateWindow(GetWindow()->m_hWnd);
			ui_play_game_sound(L"step.wav");
		}
		else
		{ 
			//5.�ж��Ƿ����ķ���Ǳ���, �Ƿ���ͬһ�ֶ���
			if( (m_nX1!=x||m_nY1!=y) && m_map[m_nY1][m_nX1]==m_map[y][x]  )
			{
				//6.����Ƿ��������
				if(IsLink(m_nX1,m_nY1,x,y))
				{
					//7.��������
					m_map[m_nY1][m_nX1]=BLANK_STATE;
					m_map[y][x]=BLANK_STATE;

					bLinked = TRUE;
					ui_play_game_sound(L"linked.wav");
				}
				else
				{
					ui_play_game_sound(L"danger.wav");
				}
			}
			else
			{
				ui_play_game_sound(L"danger.wav");
		}

			//����һ�Ժ��������򣬲�����2����ʱ��
			if ( bLinked )
			{
				DealDifficultyGame( CPoint(m_nX1,m_nY1),CPoint(x,y) );

				if (  m_iNowTick>0 )
				{
					//���һ����  ���� �Ѷ�̫���ʱ��������
					if ( m_iNowTick<60 || ( m_iNowLevel<4 ) )
					{
						m_iNowTick += 2;		
						CString StrTime;
						StrTime.Format(_T("%02d:%02d"), m_iNowTick/60, m_iNowTick%60 );
						if (NULL != m_ptimeStatic)
						{
							m_ptimeStatic->SetText(StrTime.GetBuffer(StrTime.GetLength()));
						}

					}
				}
			}


			//8.��ռ�¼�����ֵ
			m_nX1=BLANK_STATE;  
			m_nY1=BLANK_STATE;	
			InvalidateRect();
			UpdateWindow(GetWindow()->m_hWnd);


		}
	}
	else if( x<m_nCol&&y<m_nRow&&m_map[y][x]== BLANK_STATE )
	{
		; //m_GameSound.PlayGameSound(SCREEN_SOUND);
	}

	//�쿴�Ƿ��Ѿ�ʤ��
	if(IsWin())
	{
		ui_play_game_sound(L"manwin.wav");


		//MessageBox(_T("��ϲ��ʤ������,������ʼ�¾�"));
		--m_iNowLevel;
		if ( m_iNowLevel<=1 ) 
			m_iNowLevel = 1;

		//������Ϸ�Ѷ�
		m_nLevelAni += 2;	
		//��֤����������̫��
		int nMaxAni = (m_nCol*m_nRow)/2 - 4;
		m_nLevelAni = min(nMaxAni,m_nLevelAni);	
		m_nLevelAni = min(42,m_nLevelAni);	


		m_iNowTick = m_iNowLevel*60;
		StartTiming();
		StartNewGame(FALSE);
		ui_play_game_sound(L"gamestart.wav");

	}
	//InvalidateRect();
	//UpdateWindow(GetWindow()->m_hWnd);


}
/*******************************************************************
* Function Name �� DealDifficultyGame
* Remark        �� 
* Update Record �� 
*               �� 
* Params        �� 
* Return Value  �� 
* Author        �� Zhxj
* Emaile        �� shenzhenit@163.com
* Date Created  �� 2009��7��30�� ������
*******************************************************************/
BOOL CLinkGame::DealDifficultyGame( CPoint ptOldSel,CPoint ptNowSet )
{
	switch( m_iNowLevel )
	{
	case 5:
		VerSortAnimalPlace( ptOldSel.x ,FALSE);
		VerSortAnimalPlace( ptNowSet.x ,FALSE);
		break;
	case 4:
		HorSortAnimalPlace( ptOldSel.y,FALSE );
		HorSortAnimalPlace( ptNowSet.y ,FALSE );
		break;
	case 3:
		VerSortAnimalPlace( ptOldSel.x ,TRUE);
		VerSortAnimalPlace( ptNowSet.x ,TRUE);
		break;
	case 2:
		HorSortAnimalPlace( ptOldSel.y,TRUE );
		HorSortAnimalPlace( ptNowSet.y ,TRUE );
		break;
	case 1:
		VerSortAnimalPlace( ptOldSel.x ,TRUE);
		VerSortAnimalPlace( ptNowSet.x ,TRUE);
		HorSortAnimalPlace( ptOldSel.y,TRUE );
		HorSortAnimalPlace( ptNowSet.y ,TRUE );
		break;

	}
	return TRUE;	
}
CSize CLinkGame::CalculateGameAeraSize( GAMEINFO *pGameInfo )
{
	pGameInfo->szGameArea = CSize( pGameInfo->iColumnCount*pGameInfo->iOneBkWidth +2,pGameInfo->iRowCount*pGameInfo->iOntBkHeight+2 );
	return pGameInfo->szGameArea;
}

void CLinkGame::SetDefaultGameInfo()
{
	g_GameInfoSet.bmpxOffset = 1;				//����ͼƬ����߾���
	g_GameInfoSet.bmpyOffset = 0;

	//������ز���	
	g_GameInfoSet.clGameBk = RGB(0,0,0) ;		//������ɫ

	//������ز���	
	g_GameInfoSet.iOneFrontWidth = 30;			//ǰ�淽��Ŀ��  
	g_GameInfoSet.iOneFrontHeight = 30;			//ǰ�淽��ĸ߶�

	g_GameInfoSet.iOneBkWidth =  34;			//��������Ŀ��	ʵ��һ������Ŀ��
	g_GameInfoSet.iOntBkHeight = 38;			//��������ĸ߶�


	//��Ϸ��ز���
	g_GameInfoSet.iGameAreaLeft = 14;			//��Ϸ��ʼλ��
	g_GameInfoSet.iGameAreaTop =  11;			//��Ϸ��ʼλ��-������
	g_GameInfoSet.iRowCount =	  8;				//��Ϸ������
	g_GameInfoSet.iColumnCount =  15;				//����	


	g_GameInfoSet.clSelFrame =	RGB(255,0,0);
	g_GameInfoSet.usSelFrameWD = 2;


	m_nLevelAni = g_GameInfoSet.iTotalAnimal =  21;

	CalculateGameAeraSize( &g_GameInfoSet );

	m_nRow=ROWCOUNT;
	m_nCol=COLCOUNT;
}

RECT CLinkGame::GetRectByStr( LPCTSTR strRect )
{
	LPTSTR pstr = NULL;
	RECT rc ={0};
	OutputDebugString(strRect);
	rc.left = _tcstol(strRect, &pstr, 10);  ASSERT(pstr); 
	rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
	rc.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
	rc.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr); 
	return  rc;
}

SIZE CLinkGame::GetSizeByStr( LPCTSTR strSZ )
{
	LPTSTR pstr = NULL;
	SIZE sz = {0};
	sz.cx = _tcstol(strSZ, &pstr, 10);  ASSERT(pstr);    
	sz.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
	return sz;
}

void CLinkGame::StartNewGame( BOOL bFirstLevel /*= TRUE*/ )
{
	if (bFirstLevel)
	{
		m_iNowTick = MAX_TIME_EACH;
		m_iNowLevel = MAX_TIME_EACH/60;
	}
	m_nX1 = BLANK_STATE;
	m_nY1 = BLANK_STATE;

	memset( m_map,BLANK_STATE,sizeof(m_map) );
	m_iSortCount = MAX_SORT_COUNT;
	ASSERT(m_pSortBtn);
	m_pSortBtn->EnableButton(TRUE);

	memset( m_map,BLANK_STATE,sizeof(m_map) );

	//�����������
	//srand( (unsigned)time( NULL ) );
	//srand( WCE_FCTN(time) (NULL) );

	srand( GetTickCount() );

	//����Ϊ����
	if ( m_nCol*m_nRow%2 != 0 )
		// MessageBox(GetWindow()->m_hWnd,TEXT("The amount of block mustn't be odd number !!!!!"),NULL,MB_OK );
		;

	if ( m_nLevelAni == 0 )
	{
		m_nLevelAni = 22;

	}

	//���������ͼ
	//������ƥ��ɶԵĶ������ַŽ�һ����ʱ�ĵ�ͼ��
	vector <int> tmpMap;	
	int i;

#if 1
	int iAniindex = 0;
	int iLeaveStart = (( (m_nCol*m_nRow)/m_nLevelAni)/2)*2;
	iLeaveStart = iLeaveStart*m_nLevelAni ;
	for( i=0;i<iLeaveStart;i++ )
	{
		tmpMap.push_back( iAniindex );
		++iAniindex;
		if (  iAniindex>=m_nLevelAni )
			iAniindex = 0;
	}

	//��󼸸��ɶԼ���
	iAniindex = 0;
	for( i=iLeaveStart;i<m_nCol*m_nRow;i+=2 )
	{
		tmpMap.push_back( iAniindex );
		tmpMap.push_back( iAniindex );
		++iAniindex;
	}
#else
	for( i=0;i<m_nCol;i++)
		for(int j=0;j< (m_nRow/2)*2;j++)
			tmpMap.push_back(i);
#endif	

	//ÿ�δ��������ʱ��ͼ��ȡ��(��ȡ������ʱ��ͼɾ��)
	//һ������ŵ���ͼ�Ŀշ�����
	for(i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol;j++)
		{
			//�����ѡһ��λ��
			int nIndex=(int(rand()*0.1+rand()*0.01+rand()))%tmpMap.size();
			//��ȡ��ѡ������ŵ���ͼ�Ŀշ���
			m_map[i][j] = tmpMap.at(nIndex);

			//����ʱ��ͼ��ȥ�ö���
			tmpMap.erase(tmpMap.begin()+nIndex);
		}
	}

	tmpMap.clear();
	InvalidateRect();
	UpdateWindow(GetWindow()->m_hWnd);
}

void CLinkGame::StartTiming()
{
	KillTimer( TIMERID_TIMING );
	SetTimer(TIMERID_TIMING,1000);
	m_bGameEnd = FALSE;

	CString StrTime;
	StrTime.Format(_T("%02d:%02d"), m_iNowTick/60, m_iNowTick%60 );
	if (NULL != m_ptimeStatic)
	{
		m_ptimeStatic->SetText(StrTime.GetBuffer(StrTime.GetLength()));
	}


}

void CLinkGame::OnTimer( WORD wIDEvent )
{
	CString StrTime;
	switch(wIDEvent) 
	{
	case TIMERID_TIMING:	
		KillTimer( TIMERID_TIMING );
		--m_iNowTick;
		if ( m_iNowTick<=0 )
		{
			ui_play_game_sound(L"gameover.wav");
			m_bGameEnd = TRUE;
		}
		StrTime.Format(_T("%02d:%02d"), m_iNowTick/60, m_iNowTick%60 );

		if (NULL != m_ptimeStatic)
		{
			m_ptimeStatic->SetText(StrTime.GetBuffer(StrTime.GetLength()));
		}    
		if ( !m_bGameEnd )
			SetTimer(TIMERID_TIMING,1000);
		else
		{
			SetTimer( TIMERID_NEWGAME,1000);
			//Game over
			m_TimerNewGame  = 1;   //���ڻ�gameover ��һ���ж�
			m_iTickCount = 0;
			memset( m_map,BLANK_STATE,sizeof(m_map) );
			m_nX1 = BLANK_STATE;
			m_nY1 = BLANK_STATE;
			InvalidateRect();
			UpdateWindow(GetWindow()->m_hWnd);
		}
		break;
	case TIMERID_NEWGAME:
		++m_iTickCount;
		if ( m_iTickCount >=5 )
		{
			//Invalidate
			m_iTickCount = 0;
			KillTimer( TIMERID_NEWGAME );
			m_TimerNewGame = 0;
			InvalidateRect();
			UpdateWindow(GetWindow()->m_hWnd);
		}
		break;
	}
}


void CLinkGame::Exit( BOOL bSound /*= TRUE*/ )
{
	KillTimer(TIMERID_TIMING);
	KillTimer(TIMERID_NEWGAME);
	if (bSound)
	{
		ui_play_game_sound(L"gameover.wav");
	}
}

void CLinkGame::Replay( BOOL bsound /*= TRUE*/ )
{
	m_iTickCount = 0;
	KillTimer( TIMERID_NEWGAME );
	m_TimerNewGame = 0;

	StartTiming();
	StartNewGame();
	if (bsound)
	{
		ui_play_game_sound(L"gamestart.wav");
	}

}

void CLinkGame::Resort()
{
	if ( m_iSortCount <=0 )
	{
		ASSERT(m_pSortBtn);
		m_pSortBtn->EnableButton( FALSE);
		return;
	}

	if ( !m_bGameEnd )
		ui_play_game_sound(L"linked.wav");


	--m_iSortCount;
	if ( m_iSortCount <=0 )
	{
		ASSERT(m_pSortBtn);
		m_pSortBtn->EnableButton( FALSE);
	}
	m_nX1 = BLANK_STATE;
	m_nY1 = BLANK_STATE;
	AllSortAnimalPlace();
	InvalidateRect();
	UpdateWindow(GetWindow()->m_hWnd);
}

void CLinkGame::InitTimeStatic( PCWceUiLayer pStatic )
{
	m_ptimeStatic =  pStatic;
}

void CLinkGame::InitSortBtn( PCWceUiButton pButton )
{
	m_pSortBtn  = pButton;
}