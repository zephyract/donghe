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
//游戏相关参数
GAMEINFO	g_GameInfoSet;


#define X_OFFSET				g_GameInfoSet.bmpxOffset
#define Y_OFFSET				g_GameInfoSet.bmpyOffset

#define BKCOLOR					g_GameInfoSet.clGameBk					//背景颜色
#define FRONTWIDTH				g_GameInfoSet.iOneFrontWidth			//前面方块的宽度
#define FRONTHEIGHT				g_GameInfoSet.iOneFrontHeight			//前面方块的高度
#define BKWIDTH					g_GameInfoSet.iOneBkWidth				//背景方块的宽度
#define BKHEIGHT				g_GameInfoSet.iOntBkHeight				//背景方块的高度

#define X_START 				g_GameInfoSet.iGameAreaLeft
#define Y_START 				g_GameInfoSet.iGameAreaTop
#define ROWCOUNT				g_GameInfoSet.iRowCount					//行数
#define COLCOUNT				g_GameInfoSet.iColumnCount				//列数
#define GAME_AREA_WIDTH			g_GameInfoSet.szGameArea.cx				//游戏区域宽度
#define GAME_AREA_HEIGHT		g_GameInfoSet.szGameArea.cy				//游戏区域高度


#define BLANK_STATE     -1                  //空方块(没有任何动物)

CLinkGame::CLinkGame(void)
{
	m_pbackbmp   = NULL;
	m_pfrontbmp  = NULL;

	//记录方块置为无效状态
	m_nY1= BLANK_STATE;
	m_nX1= BLANK_STATE;

	//初始化行列数
	m_nRow=ROWCOUNT;
	m_nCol=COLCOUNT;

	//根据行列数动态分配内核数据数组空间
	//m_map=new int[m_nRow*m_nCol];
	memset( m_map,BLANK_STATE,sizeof(m_map) );


	m_iNowTick = MAX_TIME_EACH;
	m_iNowLevel = MAX_TIME_EACH/60;
	m_bGameEnd = FALSE;
	m_TimerNewGame = 0;
	// m_bFirstPaint = TRUE;
	m_iTickCount = 0;
	m_iSortCount = MAX_SORT_COUNT;		//没论游戏帮组次数

	m_pSortBtn = NULL;
	m_ptimeStatic = NULL;
	SetDefaultGameInfo();
}

CLinkGame::~CLinkGame(void)
{

}

BOOL CLinkGame::IsLink(int x1, int y1, int x2, int y2)
{
	//X直连方式
	if(x1==x2)
	{
		if(X1_Link_X2(x1,y1,y2))
			return TRUE;
	}
	//Y直连方式
	else if(y1==y2)
	{
		if(Y1_Link_Y2(x1,x2,y1))
			return TRUE;
	}

	//一个转弯直角的联通方式
	if(OneCornerLink(x1,y1,x2,y2))
	{
		return TRUE;
	}
	//两个转弯直角的联通方式
	else if(TwoCornerLink(x1,y1,x2,y2))
	{
		return TRUE;
	}
	return FALSE;
}

//   检测是否已经赢得了游戏
//
BOOL CLinkGame::IsWin(void)
{
	//检测所有是否尚有非未被消除的方块
	// (非BLANK_STATE状态)
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
//X直接连通
//
BOOL CLinkGame::X1_Link_X2(int x, int y1,int y2)
{
	//保证y1的值小于y2
	if(y1>y2)
	{
		//数据交换
		int n=y1;
		y1=y2;
		y2=n;
	}

	//直通 	
	for(int i=y1+1;i<=y2;i++)
	{
		if(i==y2)
			return TRUE;
		//if(m_map[i*m_nCol+x]!=BLANK_STATE)
		if( m_map[i][x]!=BLANK_STATE )
			break;
	}
	//左通
	if(XThrough(x-1,y1,FALSE)&&XThrough(x-1,y2,FALSE))
		return TRUE;
	//右通
	if(XThrough(x+1,y1,TRUE)&&XThrough(x+1,y2,TRUE))
		return TRUE;
	return FALSE;
}

//
//Y直接连通
//
BOOL CLinkGame::Y1_Link_Y2(int x1,int x2,int y)
{
	if(x1>x2)
	{
		int x=x1;
		x1=x2;
		x2=x;
	}
	//直通
	for(int i=x1+1;i<=x2;i++)
	{
		if(i==x2)
			return TRUE;
		//if(m_map[y*m_nCol+i]!=BLANK_STATE)
		if( m_map[y][i]!=BLANK_STATE )
			break;
	}
	//上通
	if(YThrough(x1,y-1,FALSE)&&YThrough(x2,y-1,FALSE))
		return TRUE;
	//下通
	if(YThrough(x1,y+1,TRUE)&&YThrough(x2,y+1,TRUE))
		return TRUE;
	return FALSE;
}

//
// 是否同一直线通
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
// 是否同一直线通
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
//  1直角接口连通
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
//  2直角接口连通
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
	//右通
	if(XThrough(x1+1,y1,TRUE)&&XThrough(x2+1,y2,TRUE))
		return TRUE;
	//左通
	if(XThrough(x1-1,y1,FALSE)&&XThrough(x2-1,y2,FALSE))
		return TRUE;
	//上通
	if(YThrough(x1,y1-1,FALSE)&&YThrough(x2,y2-1,FALSE))
		return TRUE;
	//下通
	if(YThrough(x1,y1+1,TRUE)&&YThrough(x2,y2+1,TRUE))
		return TRUE;
	//右
	for(x=x1+1;x<m_nCol;x++)
	{
		//if(m_map[y1*m_nCol+x]>-1)
		if( m_map[y1][x]!=BLANK_STATE )
			break;
		if(OneCornerLink(x,y1,x2,y2))
			return TRUE;
	}
	//左
	for(x=x1-1;x>-1;x--)
	{
		//if(m_map[y1*m_nCol+x]!=BLANK_STATE)
		if( m_map[y1][x]!=BLANK_STATE )
			break;
		if(OneCornerLink(x,y1,x2,y2))
			return TRUE;
	}
	//上
	for(y=y1-1;y>-1;y--)
	{
		//if(m_map[y*m_nCol+x1]!=BLANK_STATE)
		if( m_map[y][x1]!=BLANK_STATE )
			break;
		if(OneCornerLink(x1,y,x2,y2))
			return TRUE;
	}
	//下
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

//对游戏进行SORT
/*******************************************************************
* Function Name ： 
* Remark        ：//水平方向 
* Update Record ： 
*               ： 
* Params        ： 
* Return Value  ： 
* Author        ： Zhxj
* Emaile        ： shenzhenit@163.com
* Date Created  ： 2009年7月30日 星期四
*******************************************************************/
BOOL CLinkGame::HorSortAnimalPlace( int lineIndex,BOOL bMoveLeft )
{
	if ( lineIndex<0 || lineIndex>=m_nRow )
		return FALSE;

	srand( GetTickCount() );

	//将所有匹配成对的动物物种放进一个临时的地图中
	vector <int> tmpMap;	
	for(int i=0;i<m_nCol;i++)
	{
		if ( m_map[lineIndex][i]!=BLANK_STATE )
			tmpMap.push_back( m_map[lineIndex][i] );
	}

	//每次从上面的临时地图中取走(获取后并在临时地图删除)
	//一个动物放到地图的空方块上

	if ( bMoveLeft )		//全部移动到左边
	{
		for(int i=0;i<m_nCol;i++)
			m_map[lineIndex][i]= BLANK_STATE;
		int nCurCount = tmpMap.size();
		for( int i=0;i<nCurCount;i++ )
		{
			//随机挑选一个位置
			int nIndex=(int(rand()*0.1+rand()*0.01+rand()))%tmpMap.size();
			//获取该选定物件放到地图的空方块
			m_map[lineIndex][i] = tmpMap.at(nIndex);
			//在临时地图除去该动物
			tmpMap.erase(tmpMap.begin()+nIndex);
		}
	}
	else
	{
		for(int i=0;i<m_nCol;i++)
		{
			//随机挑选一个位置
			if ( m_map[lineIndex][i]!=BLANK_STATE )
			{
				int nIndex=(int(rand()*0.1+rand()*0.01+rand()))%tmpMap.size();
				//获取该选定物件放到地图的空方块
				m_map[lineIndex][i] = tmpMap.at(nIndex);
				//在临时地图除去该动物
				tmpMap.erase(tmpMap.begin()+nIndex);
			}
		}
	}

	tmpMap.clear();

	//更新显示
	//DrawGameArea();
	return TRUE;	
}

/*******************************************************************
* Function Name ： 
* Remark        ： //垂直方向
* Update Record ： 
*               ： 
* Params        ： 
* Return Value  ： 
* Author        ： Zhxj
* Emaile        ： shenzhenit@163.com
* Date Created  ： 2009年7月30日 星期四
*******************************************************************/
BOOL	CLinkGame::VerSortAnimalPlace( int ColumnIndex,BOOL bMoveBottom )
{
	if ( ColumnIndex<0 || ColumnIndex>=m_nCol )
		return FALSE;

	srand( GetTickCount() );

	//将所有匹配成对的动物物种放进一个临时的地图中
	vector <int> tmpMap;	
	for(int i=0;i<m_nRow;i++)
	{
		if ( m_map[i][ColumnIndex]!=BLANK_STATE )
			tmpMap.push_back( m_map[i][ColumnIndex] );
	}

	//每次从上面的临时地图中取走(获取后并在临时地图删除)
	//一个动物放到地图的空方块上

	if ( bMoveBottom )		//全部移动到左边
	{
		for(int i=0;i<m_nRow;i++)
			m_map[i][ColumnIndex]= BLANK_STATE;
		int nCurCount = tmpMap.size();
		for( int i=0;i<nCurCount;i++ )
		{
			//随机挑选一个位置
			int nIndex=(int(rand()*0.1+rand()*0.01+rand()))%tmpMap.size();
			//获取该选定物件放到地图的空方块
			if ( m_nRow-i-1 <0 )
			{
				//错误处理  正常情况不会跑到这里来
				MessageBox(GetWindow()->m_hWnd,TEXT("Row index mustn't <0"),NULL,MB_OK );
			}
			m_map[m_nRow-i-1][ColumnIndex] = tmpMap.at(nIndex);
			//在临时地图除去该动物
			tmpMap.erase(tmpMap.begin()+nIndex);
		}
	}
	else
	{
		for(int i=0;i<m_nRow;i++)
		{
			//随机挑选一个位置
			if ( m_map[i][ColumnIndex]!=BLANK_STATE )
			{
				int nIndex=(int(rand()*0.1+rand()*0.01+rand()))%tmpMap.size();
				//获取该选定物件放到地图的空方块
				m_map[i][ColumnIndex] = tmpMap.at(nIndex);
				//在临时地图除去该动物
				tmpMap.erase(tmpMap.begin()+nIndex);
			}
		}
	}

	//     tmpMap.RemoveAll();
	//     tmpMap.FreeExtra();
	tmpMap.clear();

	//更新显示
	//DrawGameArea();
	return TRUE;	
}

/*******************************************************************
* Function Name ： 
* Remark        ： //全部SORT
* Update Record ： 
*               ： 
* Params        ： 
* Return Value  ： 
* Author        ： Zhxj
* Emaile        ： shenzhenit@163.com
* Date Created  ： 2009年7月30日 星期四
*******************************************************************/
BOOL    CLinkGame::AllSortAnimalPlace()
{
	srand( GetTickCount() );

	//将所有匹配成对的动物物种放进一个临时的地图中
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

	//每次从上面的临时地图中取走(获取后并在临时地图删除)
	//一个动物放到地图的空方块上
	for(i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol;j++)
		{
			//随机挑选一个位置
			if (  m_map[i][j]!=BLANK_STATE )
			{
				int nIndex=(int(rand()*0.1+rand()*0.01+rand()))%tmpMap.size();
				//获取该选定物件放到地图的空方块
				m_map[i][j] = tmpMap.at(nIndex);
				//在临时地图除去该动物
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

			//背景格子
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

			//动物
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
		g_GameInfoSet.iGameAreaLeft = rcGameArea.left;			//游戏开始位置
		g_GameInfoSet.iGameAreaTop = rcGameArea.top;				//游戏开始位置-到顶端
	}

	if (NULL != pRownum)
	{
		g_GameInfoSet.iRowCount = _ttoi(pRownum);				//游戏的行数
	}
	if (NULL != pColnum)
	{
		g_GameInfoSet.iColumnCount = _ttoi(pColnum);			//列数	
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
		g_GameInfoSet.iOneBkWidth =  szTemp.cx;			//背景方块的宽度	实际一个动物的宽度
		g_GameInfoSet.iOntBkHeight = szTemp.cy;			//背景方块的高度
	}

	if (NULL != pFrontanisz)
	{
		szTemp = GetSizeByStr( pFrontanisz);
		g_GameInfoSet.iOneFrontWidth = szTemp.cx;			//前面方块的宽度  
		g_GameInfoSet.iOneFrontHeight = szTemp.cy;			//前面方块的高度
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

	//1.计算鼠标点击方块的的位置
	int xPt = point.x - X_START;
	int yPt = point.y - Y_START;	
	int x=xPt/BKWIDTH+(xPt%BKWIDTH?1:0)-1;
	int y=yPt/BKHEIGHT+(yPt%BKHEIGHT?1:0)-1;

	//2.在游戏区域内并且该区域还有该区域不是空的区域
	//if(x<m_nCol&&y<m_nRow&&m_map[y*m_nCol+x]!= BLANK_STATE)
	if( x>=0&&y>=0&& x<m_nCol && y<m_nRow && m_map[y][x]!= BLANK_STATE )
	{
		//3.假设尚未记录第一个方块
		if(m_nX1==BLANK_STATE)
		{


			//4.记录第一个方块的位置
			m_nX1=x;
			m_nY1=y;
			RECT rc = {m_nX1*BKWIDTH+X_START,m_nY1*BKHEIGHT+Y_START,m_nX1*BKWIDTH+X_START+BKWIDTH,m_nY1*BKHEIGHT+Y_START+BKHEIGHT};
			InvalidateRect(&rc);
			UpdateWindow(GetWindow()->m_hWnd);
			ui_play_game_sound(L"step.wav");
		}
		else
		{ 
			//5.判断是否点击的方块非本身, 是否点击同一种动物
			if( (m_nX1!=x||m_nY1!=y) && m_map[m_nY1][m_nX1]==m_map[y][x]  )
			{
				//6.检测是否可以消除
				if(IsLink(m_nX1,m_nY1,x,y))
				{
					//7.数据清理
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

			//连上一对后，重新排序，并奖励2秒钟时间
			if ( bLinked )
			{
				DealDifficultyGame( CPoint(m_nX1,m_nY1),CPoint(x,y) );

				if (  m_iNowTick>0 )
				{
					//最后一分钟  或者 难度太大的时候奖励（）
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


			//8.清空记录方块的值
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

	//察看是否已经胜利
	if(IsWin())
	{
		ui_play_game_sound(L"manwin.wav");


		//MessageBox(_T("恭喜您胜利闯关,即将开始新局"));
		--m_iNowLevel;
		if ( m_iNowLevel<=1 ) 
			m_iNowLevel = 1;

		//增加游戏难度
		m_nLevelAni += 2;	
		//保证动物数不是太多
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
* Function Name ： DealDifficultyGame
* Remark        ： 
* Update Record ： 
*               ： 
* Params        ： 
* Return Value  ： 
* Author        ： Zhxj
* Emaile        ： shenzhenit@163.com
* Date Created  ： 2009年7月30日 星期四
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
	g_GameInfoSet.bmpxOffset = 1;				//动物图片的左边距离
	g_GameInfoSet.bmpyOffset = 0;

	//绘制相关参数	
	g_GameInfoSet.clGameBk = RGB(0,0,0) ;		//背景颜色

	//绘制相关参数	
	g_GameInfoSet.iOneFrontWidth = 30;			//前面方块的宽度  
	g_GameInfoSet.iOneFrontHeight = 30;			//前面方块的高度

	g_GameInfoSet.iOneBkWidth =  34;			//背景方块的宽度	实际一个动物的宽度
	g_GameInfoSet.iOntBkHeight = 38;			//背景方块的高度


	//游戏相关参数
	g_GameInfoSet.iGameAreaLeft = 14;			//游戏开始位置
	g_GameInfoSet.iGameAreaTop =  11;			//游戏开始位置-到顶端
	g_GameInfoSet.iRowCount =	  8;				//游戏的行数
	g_GameInfoSet.iColumnCount =  15;				//列数	


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

	//部下随机种子
	//srand( (unsigned)time( NULL ) );
	//srand( WCE_FCTN(time) (NULL) );

	srand( GetTickCount() );

	//不能为奇数
	if ( m_nCol*m_nRow%2 != 0 )
		// MessageBox(GetWindow()->m_hWnd,TEXT("The amount of block mustn't be odd number !!!!!"),NULL,MB_OK );
		;

	if ( m_nLevelAni == 0 )
	{
		m_nLevelAni = 22;

	}

	//生成随机地图
	//将所有匹配成对的动物物种放进一个临时的地图中
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

	//最后几个成对加入
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

	//每次从上面的临时地图中取走(获取后并在临时地图删除)
	//一个动物放到地图的空方块上
	for(i=0;i<m_nRow;i++)
	{
		for(int j=0;j<m_nCol;j++)
		{
			//随机挑选一个位置
			int nIndex=(int(rand()*0.1+rand()*0.01+rand()))%tmpMap.size();
			//获取该选定物件放到地图的空方块
			m_map[i][j] = tmpMap.at(nIndex);

			//在临时地图除去该动物
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
			m_TimerNewGame  = 1;   //用于画gameover 的一个判断
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