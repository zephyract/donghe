// CGoBangGame.cpp: implementation of the CGoBangGame class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GoBangGame.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


BOOL debugqian = false;//是否察看牵制子匹配的详细内容

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//DWORD GetTickCount()
//{
//	DWORD dwCurTickCount = 0;//GetTickCount();
//    return dwCurTickCount;
//}

CGoBangGame::CGoBangGame()
{
	is_low = false;
	level = 0;
	notshowinfo = false;
	thinkpos = 0;

	m_xGrid = 15;
	m_yGrid = 15;

	memset( m_ptvalue,0,sizeof(m_ptvalue) );
	memset( ChessPoints,0,sizeof(ChessPoints) );
}

CGoBangGame::~CGoBangGame()
{

}

void CGoBangGame::SetGridPointCount( int xGrid,int yGrid )
{
	m_xGrid = xGrid;
	m_yGrid = yGrid;
}

BOOL CGoBangGame::has_five(int color)
{
	//color = 1表示黑子，color = 2表示白子
	//判断是否出现了5个子想连的情况	
	int i,j;
	for(i=1;i<=m_xGrid;i++)
		for(j=1;j<=m_yGrid;j++)
		{
			if (ChessPoints[i][j]==color)
			{
				ChessPoints[i][j] = 0;
				if (is_five(color,i,j))
				{
					ChessPoints[i][j] = color;
					return true;
				}
				ChessPoints[i][j] = color;
			}
		}
	return false;

	/*
	int i,j;
	for(i=1;i<=15;i++)
		for(j=1;j<=15;j++)
		{
			if (ChessPoints[i][j]==color)
			{
				if ((i>=5)&&(ChessPoints[i-1][j]==color)&&(ChessPoints[i-2][j]==color)&&(ChessPoints[i-3][j]==color)&&(ChessPoints[i-4][j]==color))
				{
					//左，右
					return true;
				}

				if ((i>=5)&&(j>=5)&&(ChessPoints[i-1][j-1]==color)&&(ChessPoints[i-2][j-2]==color)&&(ChessPoints[i-3][j-3]==color)&&(ChessPoints[i-4][j-4]==color))
				{
					//斜左上，斜右下
					return true;
				}

				if ((j>=5)&&(ChessPoints[i][j-1]==color)&&(ChessPoints[i][j-2]==color)&&(ChessPoints[i][j-3]==color)&&(ChessPoints[i][j-4]==color))
				{
					//上，下
					return true;
				}

				if ((i>=5)&&(j<=11)&&(ChessPoints[i-1][j+1]==color)&&(ChessPoints[i-2][j+2]==color)&&(ChessPoints[i-3][j+3]==color)&&(ChessPoints[i-4][j+4]==color))
				{
					//斜左下，斜右上
					return true;
				}
			}
		}
	return false;
	*/
}

BOOL CGoBangGame::is_alive(int x, int y,int color)
{
	if (ChessPoints[x][y]!=0) return false;//已经有子了
	//此数组表示各种模式，包括此子前四个和后四个子的排列
	//-1表示任意，1表示相同颜色，0表示空，2表示(x,y)所在位置
	int model[][9]={
		{-1,-1,0,1,2,1,1,0,-1},//_1*11_
		{-1,-1,-1,0,2,1,1,1,0},//_*111_	
	};

	int model_num = 2;
	int point_num = 9;
	BOOL flag;
	for (int i=0;i<4;i++)
	{
		//四个方向遍历		
		for (int j=0;j<model_num;j++)//model_num种模式遍历，每种模式遍历两次，正反各一次
		{
			flag = true;
			int k;
			for(k=0;k<point_num;k++)//point_num个子遍历，正遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}

			if (flag) return true;//匹配上某种模式
			flag = true;
			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}
			if (flag) return true;//匹配上某种模式
		}
	}
	return false;
}

BOOL CGoBangGame::is_twoqianmodel(int color,int x, int y,POINT tempresult[],int n)
{
	//此处判断是否有两个牵制子连在一起，主要为了防止对方造势
	if (ChessPoints[x][y]!=0) return false;//已经有子了
	int model[][9]={//此数组表示各种模式，包括此子前四个和后四个子的排列
		//-1表示任意，1表示相同颜色，0表示空，2表示(x,y)所在位置
		//3表示不是牵制子的相同颜色子
		/*
		{-1,0,0,1,2,0,0,-1,-1},//__1*__ 0
		{-1,-1,0,1,2,0,0,0,-1},//_1*___ 1
		{0,0,1,0,2,0,-1,-1,-1},//__1_*_ 2
		{-1,-1,1,0,2,0,0,0,-1},//1_*___ 3
		*/

		{-1,0,0,3,2,0,0,-1,-1},//__3*__ 4
		{-1,-1,0,3,2,0,0,0,-1},//_3*___ 5
		{0,0,0,3,2,0,-1,-1,-1},//___3*_ 6
		{0,0,3,0,2,0,-1,-1,-1},//__3_*_ 7
		{-1,0,3,0,2,0,0,-1,-1},//_3_*__ 8
		{-1,-1,3,0,2,0,0,0,-1},//3_*___ 9
	};

	int model_num = 6;
	int point_num = 9;
	BOOL flag;
	for (int i=0;i<4;i++)
	{
		//四个方向遍历		
		for (int j=0;j<model_num;j++)//model_num种模式遍历，每种模式遍历两次，正反各一次
		{
			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(color,x,y,i,k-4,tempresult,n)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(color,x,y,i,k-4,tempresult,n)!=1)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				

				else if (model[j][k]==3)
				{
					if (getcolor(color,x,y,i,k-4,tempresult,n)!=3)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}	
			}

			if (flag) return true;//匹配上某种模式
			flag = true;
			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(color,x,y,i,4-k,tempresult,n)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(color,x,y,i,4-k,tempresult,n)!=1)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				

				else if (model[j][k]==3)
				{
					if (getcolor(color,x,y,i,4-k,tempresult,n)!=3)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}	
			}
			if (flag) return true;//匹配上某种模式
		}
	}
	return false;
}

BOOL CGoBangGame::is_alive_old(int x, int y,int color)
{
	//这是一个根据定义直接做的老方法，比较慢，被取代，待仍然保留
	//判断一个子是不是一个活子
	//即自己下一子后，对方不论走什么子，都会存在5子相连

	if (ChessPoints[x][y]!=0) return false;//已经有子了，不可能是活子
	int i;
	ChessPoints[x][y] = color;//假设走了此子

	int anticolor;//对方的color
	if (color==1) anticolor = 2;
		else anticolor = 1;

	BOOL flag = true;//表明是否对方每次走子时，自己都可以连成5子

	POINT roundpoints[8];//一个子周围8个方向上离它最近的空子位置

	getroundpoints(x,y,color,roundpoints);//此处，为了避免全盘搜索，采用了近似手法，只计算它周围的8个空子即可

	for(i=0;i<8;i++)
	{
		if (roundpoints[i].x!=0)//即是有效的位置
		{
			ChessPoints[roundpoints[i].x][roundpoints[i].y] = anticolor;//走对方的子
			if (!can_five(color)) 
			{
				//如果存在一种情况不能连成5子，那么失败
				ChessPoints[roundpoints[i].x][roundpoints[i].y] = 0;
				flag = false;
				break;
			}
			ChessPoints[roundpoints[i].x][roundpoints[i].y] = 0;
		}
	}

	if (flag)
	{
		ChessPoints[x][y] = 0;
		return true;//必然是活子
	}

	ChessPoints[x][y] = 0;
	return false;
}

BOOL CGoBangGame::can_five(int color,int &x,int &y)
{
	//得到能够得到5连子的点
	POINT closepoints[225];
	int count = 0;
	getclose(color,count,closepoints);			
	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			if (is_five(color,closepoints[i].x,closepoints[i].y)) 
			{
				x = closepoints[i].x;
				y = closepoints[i].y;
				return true;//存在一个可以得到5子
			}
		}
	}
	return false;
}

BOOL CGoBangGame::can_five(int color)
{
	POINT closepoints[225];
	int count = 0;
	getclose(color,count,closepoints);			
	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			if (is_five(color,closepoints[i].x,closepoints[i].y)) 
			{
				return true;//存在一个可以得到5子
			}
		}
	}
	return false;		
}

BOOL CGoBangGame::has_alive(int color,int &counter,POINT tempresult[])
{
	//判断一个棋盘有没有活子
	//counter为候选结果的计数，tempresult为候选结果集
	counter = 0;
	//if (can_five(color)) return false;//已经存在可以连5的子，不必再判断一阶活子

	BOOL flag = false;	//返回值
	POINT closepoints[225];
	int count = 0;
	getclose(color,count,closepoints);			
	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			if (is_alive(closepoints[i].x,closepoints[i].y,color)) 
			{
				tempresult[counter].x = closepoints[i].x;
				tempresult[counter].y = closepoints[i].y;
				counter++;
				flag = true;
			}
		}
	}
	return flag;
}

BOOL CGoBangGame::has_chongtwo(int color,int &counter,POINT tempresult[])
{	
	//判断一个棋盘有没有冲二子
	//counter为候选结果的计数，tempresult为候选结果集
	
	counter = 0;
	BOOL flag = false;	//返回值
	POINT closepoints[225];	
	int count = 0;	
	getclose(color,count,closepoints);				
	for(int i=0;i<count;i++)		
	{		
		if (closepoints[i].x!=0)			
		{			
			if (is_chongtwo(color,closepoints[i].x,closepoints[i].y)) 				
			{				
				tempresult[counter].x = closepoints[i].x;				
				tempresult[counter].y = closepoints[i].y;				
				counter++;				
				flag = true;				
			}			
		}		
	}	
	return flag;
}

BOOL CGoBangGame::has_defent_point(int color,int &counter,POINT tempresult[])
{
	
	//判断一个棋盘有没有阻挡子	
	//counter为候选结果的计数，tempresult为候选结果集	
	counter = 0;	
	BOOL flag = false;	//返回值	
	POINT closepoints[225];	
	int count = 0;
	getclose(color,count,closepoints);
	for(int i=0;i<count;i++)	
	{	
		if (closepoints[i].x!=0)	
		{	
			if (is_defent_point(color,closepoints[i].x,closepoints[i].y)) 	
			{	
				tempresult[counter].x = closepoints[i].x;
				tempresult[counter].y = closepoints[i].y;	
				counter++;
				flag = true;	
			}
		}
	}
	return flag;	
}

BOOL CGoBangGame::has_twoqianmodel(int color,int &counter,POINT tempresult[])
{
	//判断一个棋盘有没有两个牵制子连在一起的情况
	//counter为候选结果的计数，tempresult为候选结果集
	counter = 0;
	int count;
	POINT local_tempresult[225];
	BOOL flag = false;

	if (has_qian(color,count,local_tempresult))
	{
		for (int i=0;i<count;i++)
		{
			if (is_twoqianmodel(color,local_tempresult[i].x,local_tempresult[i].y,local_tempresult,count))
			{
				tempresult[counter].x = local_tempresult[i].x;
				tempresult[counter].y = local_tempresult[i].y;
				counter++;
				flag = true;
			}
		}		
	}
	return flag;
}

BOOL CGoBangGame::has_alive(int color)
{
	//判断一个棋盘有没有活子
	//if (can_five(color)) return false;//已经存在可以连5的子，不必再判断一阶活子
	POINT closepoints[225];
	int count = 0;
	getclose(color,count,closepoints);	
	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			if (is_alive(closepoints[i].x,closepoints[i].y,color)) 
			{
				return true;
			}
		}
	}

	return false;
}

void CGoBangGame::GetNextStepPoint( int &x,int &y,int color ,BOOL debug )
{
	getpoint( x,y,color ,debug );
}

void CGoBangGame::getpoint( int &x,int &y,int color ,BOOL debug )
{
	switch(m_grade)
	{
	case 1:
        getpoint1(x,y,color,debug);
		break;
	case 2:
        getpoint2(x,y,color,debug);
		break;
	case 3:
        getpoint3(x,y,color,debug);
		break;
	case 4:
        getpoint4(x,y,color,debug);
		break;
	case 5:
        getpoint5(x,y,color,debug);
		break;
	default:
		break;
	}
}

void CGoBangGame::getpoint1(int &x, int &y, int color,BOOL debug)
{
	//一共有6个算法，每个算法16%，即每个算法算完thinkpos都可以长16
	//如果return，那么一下置为100

	thinkpos = 0;
	POINT tempresult[225];
	int count=0;	
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	x=0;y=0;
	if (has_five(1)||has_five(2)) {
		thinkpos = 100;
		return;//已经连成5子，什么都不做了
	}
	thinkpos+=16;
	int cur_time = GetTickCount();
	if (can_five(color,x,y)) 
	{
		common_info(L"计算机可以连成5子!",debug,cur_time);
		thinkpos = 100;
		return;//如果能连成5子，那么返回
	}

	ShowInfo(L"can_five1:"+passtime(cur_time));
	thinkpos+=16;
	if (can_five(anticolor,x,y)) 
	{
		common_info(L"人可以连成5子!",debug,cur_time);
		thinkpos = 100;
		return;//看对方能否连成5子
	}

	ShowInfo(L"can_five2:"+passtime(cur_time));
	thinkpos+=16;
	if (has_alive(color,count,tempresult))
	{
		//存在活子，直接输出活子（应从结果集中random选取）
		int tempi = random(0,count-1);
		x = tempresult[tempi].x;
		y = tempresult[tempi].y;		
		common_info(L"计算机找到一阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive:"+passtime(cur_time));
	thinkpos+=16;
	if (has_alive(anticolor,count,tempresult))
	{
		//看对方是否存在活子
		int tempi = random(0,count-1);
		x = tempresult[tempi].x;
		y = tempresult[tempi].y;
		common_info(L"找到人的一阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive2:"+passtime(cur_time));
	thinkpos+=16;
	getclose_small(count,tempresult);
	int tempi = random(0,count-1);
	x = tempresult[tempi].x;
	y = tempresult[tempi].y;
	common_info(L"随机算法!",debug,cur_time);
	thinkpos = 100;
}

void CGoBangGame::getpoint2(int &x, int &y, int color,BOOL debug)
{
	//一共有10个算法，每个算法10%，即每个算法算完thinkpos都可以长10
	//如果return，那么一下置为100
	thinkpos = 0;

	//在grade3的基础上，再减少一些功能，比如，将qianmodel算法全部去掉

	//采用随机算法

	POINT tempresult[225];

	int count=0;	

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	x=0;y=0;

	if (has_five(1)||has_five(2)) 
	{
		thinkpos = 100;
		return;//已经连成5子，什么都不做了
	}
	thinkpos+=10;

	int cur_time = GetTickCount();

	if (can_five(color,x,y)) 

	{

		common_info(L"计算机可以连成5子!",debug,cur_time);
		thinkpos = 100;
		return;//如果能连成5子，那么返回

	}

	ShowInfo(L"can_five1:"+passtime(cur_time));
	thinkpos+=10;
	if (can_five(anticolor,x,y)) 
	{
		common_info(L"人可以连成5子!",debug,cur_time);
		thinkpos = 100;
		return;//看对方能否连成5子
	}

	ShowInfo(L"can_five2:"+passtime(cur_time));
	thinkpos+=10;
	if (has_alive(color,count,tempresult))

	{

		//存在活子，直接输出活子（应从结果集中random选取）

		int tempi = random(0,count-1);
		
		x = tempresult[tempi].x;
		
		y = tempresult[tempi].y;

		common_info(L"计算机找到一阶活子!",debug,cur_time);
		thinkpos = 100;
		return;

	}

	ShowInfo(L"has_alive:"+passtime(cur_time));
	thinkpos+=10;
	if (has_twolevel_four(color,count,tempresult))
	{
		//存在一个存在四子的二阶活子
		int tempi = random(0,count-1);
		x = tempresult[tempi].x;		
		y = tempresult[tempi].y;
		common_info(L"计算机找到强二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four:"+passtime(cur_time));
	thinkpos+=10;
	//1_____________________________________

	if (has_alive(anticolor,count,tempresult))
	{
		//看对方是否存在活子
		int tempi = random(0,count-1);		
		x = tempresult[tempi].x;		
		y = tempresult[tempi].y;
		common_info(L"找到人的一阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive:"+passtime(cur_time));	
	thinkpos+=10;
	//2_____________________________________

	if (has_twolevel_four(anticolor,count,tempresult))
	{
		//存在一个存在四子的二阶活子
		int tempi = random(0,count-1);		
		x = tempresult[tempi].x;		
		y = tempresult[tempi].y;
		common_info(L"找到人的强二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four:"+passtime(cur_time));	
	thinkpos+=10;
	//____________________________________________

	//不存在一阶活子，考察是否存在二阶活子

	if (has_twolevel_pure(color,count,tempresult))
	{
		//存在二阶活子，直接输出二阶活子（应从结果集中random选取）
		int tempi = random(0,count-1);		
		x = tempresult[tempi].x;		
		y = tempresult[tempi].y;
		common_info(L"计算机找到二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel:"+passtime(cur_time));
	thinkpos+=10;
	//3___________________________

	if (has_twolevel(anticolor,count,tempresult))
	{
		//看对方是否存在二阶活子
		int tempi = random(0,count-1);	
		x = tempresult[tempi].x;	
		y = tempresult[tempi].y;
		common_info(L"找到人的二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}	

	ShowInfo(L"has_twolevel:"+passtime(cur_time));	
	thinkpos+=10;
	getclose_small(count,tempresult);

	int tempi = random(0,count-1);

	x = tempresult[tempi].x;

	y = tempresult[tempi].y;

	common_info(L"随机算法!",debug,cur_time);
	thinkpos = 100;

}

void CGoBangGame::getpoint3(int &x, int &y, int color,BOOL debug)
{

	//一共有12个算法，每个算法8%，即每个算法算完thinkpos都可以长8
	//如果return，那么一下置为100
	
	thinkpos = 0;
	//认为这时候还不能看出来二阶牵制子，所以所有与二阶牵制子相关的函数都去掉了

	//也不使用make和defent算法

	//同时只能辨别纯牵制子匹配，即如果你有掺杂牵制子匹配，那么就可胜利

	POINT tempresult[225];

	int count=0;	

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	x=0;y=0;

	if (has_five(1)||has_five(2)) 
	{
		thinkpos = 100;
		return;//已经连成5子，什么都不做了
	}
	thinkpos += 8;
	int cur_time = GetTickCount();

	if (can_five(color,x,y)) 
	{
		common_info(L"计算机可以连成5子!",debug,cur_time);
		thinkpos = 100;
		return;//如果能连成5子，那么返回
	}

	ShowInfo(L"can_five1:"+passtime(cur_time));
	thinkpos += 8;
	if (can_five(anticolor,x,y)) 
	{
		common_info(L"人可以连成5子!",debug,cur_time);
		thinkpos = 100;
		return;//看对方能否连成5子
	}

	ShowInfo(L"can_five2:"+passtime(cur_time));
	thinkpos += 8;
	if (has_alive(color,count,tempresult))
	{
		//存在活子，直接输出活子（应从结果集中random选取）
		selectqianmodel3(color,x,y,tempresult,count);		
		common_info(L"计算机找到一阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive:"+passtime(cur_time));
	thinkpos += 8;
	if (has_twolevel_four(color,count,tempresult))
	{
		//存在一个存在四子的二阶活子
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"计算机找到强二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four:"+passtime(cur_time));
	thinkpos += 8;
	//1_____________________________________

	if (has_alive(anticolor,count,tempresult))
	{
		//看对方是否存在活子
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"找到人的一阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive:"+passtime(cur_time));	
	thinkpos += 8;
	//2_____________________________________

	if (has_twolevel_four(anticolor,count,tempresult))
	{
		//存在一个存在四子的二阶活子
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"找到人的强二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four:"+passtime(cur_time));	
	thinkpos += 8;
	//____________________________________________

	//不存在一阶活子，考察是否存在二阶活子

	if (has_twolevel_pure(color,count,tempresult))
	{
		//存在二阶活子，直接输出二阶活子（应从结果集中random选取）
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"计算机找到二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel:"+passtime(cur_time));
	thinkpos += 8;
	if (has_qianmodel(color,count,tempresult))
	{
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"计算机三个牵制子匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel:"+passtime(cur_time));
	thinkpos += 8;
	//3___________________________

	if (has_twolevel(anticolor,count,tempresult))
	{
		//看对方是否存在二阶活子
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"找到人的二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;	
	}	

	ShowInfo(L"has_twolevel:"+passtime(cur_time));	
	thinkpos += 8;
	//4_________________________________

	if (has_qianmodel(anticolor,count,tempresult))
	{
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"人三个牵制子匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel2:"+passtime(cur_time));
	thinkpos += 8;
	getmax(count,tempresult);

	int tempi = random(0,count-1);
	x = tempresult[tempi].x;
	y = tempresult[tempi].y;

	common_info(L"赋值算法!",debug,cur_time);
	thinkpos = 100;
}

void CGoBangGame::getpoint4(int &x, int &y, int color,BOOL debug)
{
	//一共有26个算法，每个算法4%，即每个算法算完thinkpos都可以长4
	//如果return，那么一下置为100
	
	thinkpos = 0;
	POINT tempresult[225];
	int count=0;	
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	x=0;y=0;
	if (has_five(1)||has_five(2)) 
	{
		thinkpos = 100;
		return;//已经连成5子，什么都不做了
	}
	thinkpos += 4;
	int cur_time = GetTickCount();

	if (can_five(color,x,y)) 
	{
		common_info(L"计算机可以连成5子!",debug,cur_time);
		thinkpos = 100;
		return;//如果能连成5子，那么返回
	}

	ShowInfo(L"can_five1:"+passtime(cur_time));
	thinkpos += 4;
	if (can_five(anticolor,x,y)) 
	{
		common_info(L"人可以连成5子!",debug,cur_time);
		thinkpos = 100;
		return;//看对方能否连成5子
	}

	ShowInfo(L"can_five2:"+passtime(cur_time));
	thinkpos += 4;
	if (has_alive(color,count,tempresult))
	{
		//存在活子，直接输出活子（应从结果集中random选取）
		selectqianmodel(color,x,y,tempresult,count);		
		common_info(L"计算机找到一阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive:"+passtime(cur_time));
	thinkpos += 4;
	if (has_twolevel_four(color,count,tempresult))
	{
		//存在一个存在四子的二阶活子
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"计算机找到强二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four:"+passtime(cur_time));
	thinkpos += 4;
	//1_____________________________________
	if (has_alive_defent(anticolor,count,tempresult))
	{
		//看对方是否存在活子
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"找到人的一阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive_defent:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//不能防住对方的一阶活子,冲四
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"计算机冲四!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_alive_defent_low(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"找到人的一阶活子low!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}	

	ShowInfo(L"has_alive_defent_low2:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_alive(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"找到人的一阶活子2!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive2:"+passtime(cur_time));
	thinkpos += 4;
	//2_____________________________________
	if (has_twolevel_four_defent(anticolor,count,tempresult))
	{
		//存在一个存在四子的二阶活子
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"找到人的强二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four_defent:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//不能防住对方的强二阶活子,冲四
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"计算机冲四!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_twolevel_four_defent_low(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"找到人的强二阶活子low!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four_defent_low2:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_twolevel_four(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"找到人的强二阶活子2!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four2:"+passtime(cur_time));
	thinkpos += 4;
	//____________________________________________
	//不存在一阶活子，考察是否存在二阶活子
	if (has_twolevel_pure(color,count,tempresult))
	{
		//存在二阶活子，直接输出二阶活子（应从结果集中random选取）
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"计算机找到二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel:"+passtime(cur_time));
	thinkpos += 4;
	if (has_qianmodel_addone(color,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"计算机两个牵制子和一个已下子匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel_addone:"+passtime(cur_time));
	thinkpos += 4;
	if (has_qianmodel(color,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"计算机三个牵制子匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel:"+passtime(cur_time));
	thinkpos += 4;
	if (has_twolevel_qianmodel(color,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"计算机二阶牵制子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_qianmodel:"+passtime(cur_time));	
	thinkpos += 4;
	//3___________________________

	if (has_twolevel_defent(anticolor,count,tempresult))
	{
		//看对方是否存在二阶活子
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"找到人的二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}	

	ShowInfo(L"has_twolevel_defent:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//不能防住对方的二阶活子,冲四
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"计算机冲四!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_twolevel_defent_low(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"找到人的二阶活子low!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_defent_low2:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_twolevel(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"找到人的二阶活子2!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel2:"+passtime(cur_time));
	thinkpos += 4;
	//4_________________________________
	if (has_qianmodel_addone(anticolor,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"人两个牵制子和一个已下子匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel_addone2:"+passtime(cur_time));	
	thinkpos += 4;
	//5_______________________________________
	if (has_qianmodel(anticolor,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"人三个牵制子匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel2:"+passtime(cur_time));
	thinkpos += 4;
	//6________________________________________
	if (has_twolevel_qianmodel(anticolor,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"人二阶牵制子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_qianmodel2:"+passtime(cur_time));
	thinkpos += 4;
	if (make_qianmodel(color,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"计算机制造牵制子匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"make_qianmodel:"+passtime(cur_time));
	thinkpos += 4;
	//也不存在二阶活子，看能否制造活子
	/*
	if (make_twolevel(color,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"计算机制造二阶活子!",debug,cur_time);
		return;
	}
	ShowInfo(L"make_twolevel:"+passtime(cur_time));
	*/

	getmax(count,tempresult);

	int tempi = random(0,count-1);
	x = tempresult[tempi].x;
	y = tempresult[tempi].y;
	common_info(L"赋值算法!",debug,cur_time);
	thinkpos = 100;
}

void CGoBangGame::getpoint5(int &x, int &y, int color,BOOL debug)
{
	//一共有53个算法，每个算法2%，即每个算法算完thinkpos都可以长2
	//如果return，那么一下置为100
	thinkpos = 0;
	//最高等级

	/*算法过程如下

  计算机找到一阶活子
  计算机找到强二阶活子
  计算机两个牵制子和一个已下子强匹配!
  计算机三个牵制子强匹配
  计算机二阶牵制子强匹配
  计算机三阶牵制子强匹配
  //**计算机强打击子
  找到人的一阶活子d
  找到人的强二阶活子d
  人两个牵制子和一个已下子强匹配d
  人三个牵制子强匹配d
  人二阶牵制子强匹配d
  //人三阶牵制子强匹配d 
  计算机找到二阶活子
  计算机两个牵制子和一个已下子匹配
  计算机三个牵制子匹配
  计算机二阶牵制子
  计算机三阶牵制子  
  **计算机打击子
  //**计算机二阶打击子>50不采用
  找到人的二阶活子d
  计算机冲三(color=2时，<20不采用)
  计算机冲四(color=2时，<20不采用)
  人两个牵制子和一个已下子匹配d
  人三个牵制子匹配d
  人二阶牵制子d
  //人三阶牵制子>50时不采用
  //**人强打击子>50时不采用
  //**计算机打击子_low
  //**计算机二阶打击子_low>50不采用
  **人打击子>50时不采用
  //**人二阶打击子>50时不采用  
  //计算机制造打击子
  //计算机制造二阶牵制子匹配
  //计算机制造三阶牵制子匹配  
  //计算机制造二阶活子
  //计算机冲二子和制造牵制子匹配(color=2时，<10不采用)  
  计算机冲二子(color=2时，<10不采用)
  计算机制造牵制子匹配(color=2时，<10不采用)
    
  赋值算法
	*/

	POINT tempresult[225];
	int count=0;	
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	x=0;y=0;

	if (has_five(1)||has_five(2)) 
	{
		thinkpos = 100;
		return;//已经连成5子，什么都不做了
	}
	thinkpos += 2;
	int cur_time = GetTickCount();

	if (can_five(color,x,y)) 
	{
		common_info(L"计算机可以连成5子!",debug,cur_time);
		thinkpos = 100;
		return;//如果能连成5子，那么返回
	}

	ShowInfo(L"can_five1:"+passtime(cur_time));
	thinkpos += 2;
	if (can_five(anticolor,x,y)) 
	{
		common_info(L"人可以连成5子!",debug,cur_time);
		thinkpos = 100;
		return;//看对方能否连成5子
	}

	ShowInfo(L"can_five2:"+passtime(cur_time));
	thinkpos += 2;
	if (has_alive(color,count,tempresult))
	{

		//存在活子，直接输出活子（应从结果集中random选取）
		selectqianmodel5(color,x,y,tempresult,count);		
		common_info(L"计算机找到一阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive:"+passtime(cur_time));
	thinkpos += 2;
	if (has_twolevel_four(color,count,tempresult))
	{
		//存在一个存在四子的二阶活子
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机找到强二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four:"+passtime(cur_time));
	thinkpos += 2;
	if (has_qianmodel_addone(color,count,tempresult,true))	
	{	
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机两个牵制子和一个已下子强匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_qianmodel_addone_qiang:"+passtime(cur_time));
	thinkpos += 2;
	if (has_qianmodel(color,count,tempresult,true))	
	{		
		selectqianmodel5(color,x,y,tempresult,count);		
		common_info(L"计算机三个牵制子强匹配!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_qiang:"+passtime(cur_time));	
	thinkpos += 2;
	if (has_twolevelqianmodel_qiang(color,count,tempresult))	
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机二阶牵制子强匹配!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_twolevelqianmodel_qiang:"+passtime(cur_time));	
	thinkpos += 2;
	if (has_threelevelqianmodel_qiang(color,count,tempresult))	
	{	
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机三阶牵制子强匹配!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_threelevelqianmodel_qiang:"+passtime(cur_time));	
	thinkpos += 2;
	/*

	if (has_qiang_beat(color,count,tempresult))
	{		
		selectqianmodel5(color,x,y,tempresult,count);		
		common_info(L"计算机强打击子!",debug,cur_time);	
		return;	
	}
	
	ShowInfo(L"has_qiang_beat:"+passtime(cur_time));
	*/

	//1_____________________________________

	if (has_alive_defent(anticolor,count,tempresult))
	{
		//看对方是否存在活子
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"找到人的一阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive_defent:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{

		//不能防住对方的一阶活子,冲四
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"计算机冲四!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_alive_defent_low(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"找到人的一阶活子low!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_alive(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"找到人的一阶活子2!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_alive2:"+passtime(cur_time));
	thinkpos += 2;
	//2_____________________________________
	if (has_twolevel_four_defent(anticolor,count,tempresult))	
	{
		//存在一个存在四子的二阶活子
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"找到人的强二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four_defent:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//不能防住对方的强二阶活子,冲四
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"计算机冲四!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevel_four_defent_low(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"找到人的强二阶活子low!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_twolevel_four_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevel_four(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"找到人的强二阶活子2!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_twolevel_four2:"+passtime(cur_time));
	thinkpos += 2;
	//____________________________________________
	
	//7____________________
	if (has_qianmodel_addone_defent(anticolor,count,tempresult,true))	
	{		
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人两个牵制子和一个已下子强匹配!",debug,cur_time);
		thinkpos = 100;
		return;		
	}
	
	ShowInfo(L"has_qianmodel_addone_defent_qiang:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))	
	{	
		//不能防住对方,冲四	
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"计算机冲四!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel_addone_defent_low(anticolor,count,tempresult,true))	
	{	
		//还是不能防住	
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人两个牵制子和一个已下子强匹配low!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_addone_defent_qiang_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel_addone(anticolor,count,tempresult,true))	
	{	
		//还是不能防住，那么只挡某一个活子即可	
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人两个牵制子和一个已下子强匹配2!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_qianmodel_addone_qiang2:"+passtime(cur_time));
	thinkpos += 2;
	//8_______________________________________
	
	if (has_qianmodel_defent(anticolor,count,tempresult,true))	
	{	
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人三个牵制子强匹配!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_defent_qiang:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{	
		//不能防住对方,冲四	
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"计算机冲四!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel_defent_low(anticolor,count,tempresult,true))	
	{	
		//还是不能防住
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人三个牵制子强匹配low!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_qiang_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel(anticolor,count,tempresult,true))	
	{	
		//还是不能防住，那么只挡某一个活子即可	
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人三个牵制子强匹配2!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_qiang2:"+passtime(cur_time));
	thinkpos += 2;
	//9___________________________

	if (has_twolevelqianmodel_qiang_defent(anticolor,count,tempresult))	
	{		
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人二阶牵制子强匹配!",debug,cur_time);
		thinkpos = 100;
		return;		
	}
	
	ShowInfo(L"has_twolevelqianmodel_qiang_defent:"+passtime(cur_time));	
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))		
	{
		//不能防住对方,冲四
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"计算机冲四!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevelqianmodel_qiang_defent_low(anticolor,count,tempresult))
	{
		//还是不能防住
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人二阶牵制子强匹配low!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_twolevelqianmodel_qiang_defent_low:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevelqianmodel_qiang(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人二阶牵制子强匹配2!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_twolevelqianmodel_qiang2:"+passtime(cur_time));
	thinkpos += 2;
	//10_________________________
/*
	if (has_threelevelqianmodel_qiang_defent(anticolor,count,tempresult))
		
	{	
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人三阶牵制子强匹配!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	ShowInfo(L"has_threelevelqianmodel_qiang_defent:"+passtime(cur_time));
	thinkpos += 2;

	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//不能防住对方,冲四
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"计算机冲四!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_threelevelqianmodel_qiang_defent_low(anticolor,count,tempresult))
	{
		//还是不能防住
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人三阶牵制子强匹配low!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_threelevelqianmodel_qiang_defent_low:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_threelevelqianmodel_qiang(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人三阶牵制子强匹配2!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_threelevelqianmodel_qiang2:"+passtime(cur_time));
	thinkpos += 2;
*/
	//____________________________________-
	//不存在一阶活子，考察是否存在二阶活子

	if (has_twolevel_pure(color,count,tempresult))
	{
		//存在二阶活子，直接输出二阶活子（应从结果集中random选取）
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机找到二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel:"+passtime(cur_time));		
	thinkpos += 2;
	if (has_qianmodel_addone(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机两个牵制子和一个已下子匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel_addone:"+passtime(cur_time));
	thinkpos += 2;
	if (has_qianmodel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机三个牵制子匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel:"+passtime(cur_time));	
	thinkpos += 2;
	if (has_twolevel_qianmodel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机二阶牵制子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_qianmodel:"+passtime(cur_time));	
	thinkpos += 2;
	if (has_threelevel_qianmodel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机三阶牵制子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_threelevel_qianmodel:"+passtime(cur_time));
	thinkpos += 2;
	if (has_beatpoints(color,count,tempresult))	
	{	
		selectqianmodel5(color,x,y,tempresult,count);	
		common_info(L"计算机打击子!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_beatpoints1:"+passtime(cur_time));
	thinkpos += 2;
	/*
	if ((countpoints()<50)&&has_twolevel_beat(color,count,tempresult))	
	{	
		selectqianmodel5(color,x,y,tempresult,count);	
		common_info(L"计算机二阶打击子!",debug,cur_time);		
		return;	
	}
	ShowInfo(L"has_twolevel_beat1:"+passtime(cur_time));
	*/
	//3___________________________

	if (has_twolevel_defent(anticolor,count,tempresult))
	{
		//看对方是否存在二阶活子
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"找到人的二阶活子!",debug,cur_time);
		thinkpos = 100;
		return;
	}	

	ShowInfo(L"has_twolevel_defent:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//不能防住对方的二阶活子,冲四
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"计算机冲四!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&make_alive(color,count,tempresult))	
	{	
		//不能防住对方的二阶活子,冲三		
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"计算机冲三!",debug,cur_time);
		thinkpos = 100;
		return;
		
	}
	
	ShowInfo(L"make_alive:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevel_defent_low(anticolor,count,tempresult))
	{
		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"找到人的二阶活子low!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevel(anticolor,count,tempresult))
	{

		//还是不能防住，那么只挡某一个活子即可
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"找到人的二阶活子2!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_twolevel2:"+passtime(cur_time));
	thinkpos += 2;
	if (thinkpos>60) thinkpos = 60;
	//___________________________________

	if (((color==1)||(countpoints()>=20))&&make_alive(color,count,tempresult))	
	{	
		selectqianmodel5(color,x,y,tempresult,count);
		
		common_info(L"计算机冲三(好)!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"make_alive(good):"+passtime(cur_time));
	thinkpos += 2;
	
	if (((color==1)||(countpoints()>=20))&&has_canfour(color,count,tempresult))	
	{		
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机冲四(好)!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	ShowInfo(L"has_canfour(good):"+passtime(cur_time));
	thinkpos += 2;
	
	//4_________________________________
	if (has_qianmodel_addone_defent(anticolor,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人两个牵制子和一个已下子匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel_addone_defent:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//不能防住对方,冲四
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"计算机冲四!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&make_alive(color,count,tempresult))	
	{	
		//不能防住对方的二阶活子,冲三	
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"计算机冲三!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"make_alive:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel_addone_defent_low(anticolor,count,tempresult))	
	{	
		//还是不能防住	
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人两个牵制子和一个已下子匹配low!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_addone_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel_addone(anticolor,count,tempresult))		
	{	
		//还是不能防住，那么只挡某一个活子即可		
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人两个牵制子和一个已下子匹配2!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_addone2:"+passtime(cur_time));
	thinkpos += 2;
	/*
	if (has_qianmodel_addone(anticolor,count,tempresult))		
	{		
		//还是不能防住，那么只挡某一个活子即可
				
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人两个牵制子和一个已下子匹配!",debug,cur_time);	
		return;	
	}	
	ShowInfo(L"has_qianmodel_addone2:"+passtime(cur_time));
	*/
	//5_______________________________________

	if (has_qianmodel_defent(anticolor,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人三个牵制子匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel_defent:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//不能防住对方,冲四
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"计算机冲四!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&make_alive(color,count,tempresult))		
	{
		//不能防住对方的二阶活子,冲三
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"计算机冲三!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"make_alive:"+passtime(cur_time));
	thinkpos += 2;

	if (!defent_success&&has_qianmodel_defent_low(anticolor,count,tempresult))	
	{	
		//还是不能防住	
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人三个牵制子匹配low!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel(anticolor,count,tempresult))	
	{	
		//还是不能防住，那么只挡某一个活子即可		
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人三个牵制子匹配2!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel2:"+passtime(cur_time));
	thinkpos += 2;
	/*
	if (has_qianmodel(anticolor,count,tempresult))	
	{	
		//还是不能防住，那么只挡某一个活子即可		
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人三个牵制子匹配!",debug,cur_time);		
		return;	
	}
	
	ShowInfo(L"has_qianmodel2:"+passtime(cur_time));
	*/

	//6________________________________________	

	if (has_twolevel_qianmodel_defent(anticolor,count,tempresult))	
	{	
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人二阶牵制子!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_twolevel_qianmodel_defent:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))	
	{
		//不能防住对方,冲四
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"计算机冲四!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&make_alive(color,count,tempresult))	
	{
		//不能防住对方的二阶活子,冲三
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"计算机冲三!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	ShowInfo(L"make_alive:"+passtime(cur_time));
	thinkpos += 2;
	
	if (!defent_success&&has_twolevel_qianmodel_defent_low(anticolor,count,tempresult))	
	{	
		//还是不能防住
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人二阶牵制子low!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_twolevel_qianmodel_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevel_qianmodel(anticolor,count,tempresult))		
	{		
		//还是不能防住，那么只挡某一个活子即可				
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人二阶牵制子2!",debug,cur_time);
		thinkpos = 100;
		return;
	}	
	ShowInfo(L"has_twolevel_qianmodel2:"+passtime(cur_time));
	thinkpos += 2;

	//________________________________________________
	/*
	if ((countpoints()<50)&&has_threelevel_qianmodel(anticolor,count,tempresult))		
	{		
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人三阶牵制子!",debug,cur_time);	
		return;		
	}	
	ShowInfo(L"has_threelevel_qianmodel2:"+passtime(cur_time));	
	*/
	/*
	if ((countpoints()<50)&&has_qiang_beat(anticolor,count,tempresult))		
	{		
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"人强打击子!",debug,cur_time);	
		return;	
	}
	ShowInfo(L"has_qiang_beat2:"+passtime(cur_time));
	*/
	/*
	if (has_beatpoints_low_low(color,count,tempresult))		
	{		
		selectqianmodel5(color,x,y,tempresult,count);	
		common_info(L"计算机打击子low!",debug,cur_time);		
		return;	
	}
	
	ShowInfo(L"has_beatpoints_low_low1:"+passtime(cur_time));
	/*
	if ((countpoints()<50)&&has_twolevel_beat_low(color,count,tempresult))		
	{		
		selectqianmodel5(color,x,y,tempresult,count);		
		common_info(L"计算机二阶打击子low!",debug,cur_time);	
		return;		
	}
	
	ShowInfo(L"has_twolevel_beat_low1:"+passtime(cur_time));
	*/

	if ((countpoints()<50)&&has_beatpoints_low(anticolor,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"人打击子!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_beatpoints_low2:"+passtime(cur_time));
	thinkpos += 2;
	/*
	if ((countpoints()<50)&&has_twolevel_beat(anticolor,count,tempresult))	
	{
		selectqianmodel5(color,x,y,tempresult,count,true);		
		common_info(L"人二阶打击子!",debug,cur_time);		
		return;	
	}
	ShowInfo(L"has_twolevel_beat2:"+passtime(cur_time));	
	*/	
	
	/*
	if (make_beatChessPoints(color,count,tempresult))	
	{	
		selectqianmodel5(color,x,y,tempresult,count);		
		common_info(L"计算机制造打击子!",debug,cur_time);		
		return;		
	}	
	ShowInfo(L"make_beatChessPoints:"+passtime(cur_time));
	/*
	if (make_twolevel_qianmodel(color,count,tempresult))		
	{		
		selectqianmodel5(color,x,y,tempresult,count);		
		common_info(L"计算机制造二阶牵制子匹配!",debug,cur_time);	
		return;	
	}
	ShowInfo(L"make_twolevel_qianmodel:"+passtime(cur_time));
	/*
	if (make_threelevel_qianmodel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机制造三阶牵制子匹配!",debug,cur_time);
		return;
	}
	ShowInfo(L"make_threelevel_qianmodel:"+passtime(cur_time));	
	*/
	//也不存在二阶活子，看能否制造活子
	/*
	if (make_twolevel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机制造二阶活子!",debug,cur_time);
		return;
	}
	ShowInfo(L"make_twolevel:"+passtime(cur_time));
	/*
	if (make_alive(color,count,tempresult))
	{
		selectmax(color,x,y,tempresult,count);
		common_info(L"计算机制造一阶活子!",debug);
		return;
	}	
	if (make_alive(anticolor,count,tempresult))
	{
		selectmax(color,x,y,tempresult,count,true);
		common_info(L"制造人的一阶活子!",debug);
		return;
	}
	//最后前置函数都已经失败，采用赋值算法
	getclose(color,count,tempresult);
	selectqianmodel5(color,x,y,tempresult,count);
	*/
	//getmax(count,tempresult);
	//getclose(color,count,tempresult);
/*
	if (((color==1)||(countpoints()>=10))&&has_chongtwoandmakeqianmodel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);	
		common_info(L"计算机冲二子和制造牵制子匹配!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	ShowInfo(L"has_chongtwoandmakeqianmodel:"+passtime(cur_time));
	thinkpos += 2;
*/
	
	if (((color==1)||(countpoints()>=10))&&has_chongtwo(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		
		common_info(L"计算机冲二子!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_chongtwo:"+passtime(cur_time));
	thinkpos += 2;
	if (((color==1)||(countpoints()>=10))&&make_qianmodel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"计算机制造牵制子匹配!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"make_qianmodel:"+passtime(cur_time));
	thinkpos += 2;
	
	if (color==1)
	{
		getclose_small(count,tempresult);
		selectmax(color,x,y,tempresult,count,true);	
	}
	
	else
	{
		getmax(count,tempresult);
		int tempi = random(0,count-1);
		x = tempresult[tempi].x;
		y = tempresult[tempi].y;
	}
	common_info(L"赋值算法!",debug,cur_time);	
	thinkpos = 100;

}

BOOL CGoBangGame::twolevel_alive(int x, int y, int color)
{
	if (ChessPoints[x][y]!=0) return false;//已经有子了
	int model[][9]={//此数组表示各种模式，包括此子前四个和后四个子的排列
		//-1表示任意，1表示相同颜色，0表示空，2表示(x,y)所在位置
		{0,1,1,1,2,-1,-1,-1,-1},//_111*
		{1,0,1,1,2,-1,-1,-1,-1},//1_11*
		{1,1,0,1,2,-1,-1,-1,-1},//11_1*
		{1,1,1,0,2,-1,-1,-1,-1},//111_*
		{-1,1,1,1,2,0,-1,-1,-1},//111*_

		{-1,0,1,1,2,1,-1,-1,-1},//_11*1
		{-1,1,0,1,2,1,-1,-1,-1},//1_1*1
		{-1,1,1,0,2,1,-1,-1,-1},//11_*1
		{-1,-1,1,1,2,0,1,-1,-1},//11*_1
		{-1,-1,1,1,2,1,0,-1,-1},//11*1_

		{0,0,1,1,2,0,-1,-1,-1},//__11*_
		{0,1,0,1,2,0,-1,-1,-1},//_1_1*_
		{0,1,1,0,2,0,-1,-1,-1},//_11_*_
		{-1,0,0,1,2,1,0,-1,-1},//__1*1_

		{-1,0,1,0,2,1,0,-1,-1},//_1_*1_
		{-1,-1,0,1,2,0,1,0,-1},//_1*_1_
	};//17种可以造成活子的情形

	int model_num = 17;
	int point_num = 9;
	BOOL flag;
	int count = 0;//累计有多少个方向的上有模式被匹配
	for (int i=0;i<4;i++)
	{
		//四个方向遍历		
		for (int j=0;j<model_num;j++)//model_num种模式遍历，每种模式遍历两次，正反各一次
		{
			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}

			if (flag) 
			{
				//匹配上某种模式
				count++;
				break;//此方向就不用再累加了
			}

			flag = true;

			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}

			if (flag) 
			{
				//匹配上某种模式
				count++;
				break;//此方向就不用再累加了
			}
		}
	}
	if (count>=2) return true;
	return false;

}

BOOL CGoBangGame::twolevel_alive_pure(int x, int y, int color)
{
	//与twolevel_alive不同的是，pure不包括强二阶活子
	if (ChessPoints[x][y]!=0) return false;//已经有子了
	int model[][9]={//此数组表示各种模式，包括此子前四个和后四个子的排列
		//-1表示任意，1表示相同颜色，0表示空，2表示(x,y)所在位置
		{0,0,1,1,2,0,-1,-1,-1},//__11*_
		{0,1,0,1,2,0,-1,-1,-1},//_1_1*_
		{0,1,1,0,2,0,-1,-1,-1},//_11_*_
		{-1,0,1,1,2,0,0,-1,-1},//_11*__

		{-1,0,0,1,2,1,0,-1,-1},//__1*1_
		{-1,0,1,0,2,1,0,-1,-1},//_1_*1_
		{-1,-1,0,1,2,0,1,0,-1},//_1*_1_
	};//17种可以造成活子的情形

	int model_num = 7;
	int point_num = 9;
	BOOL flag;
	int count = 0;//累计有多少个方向的上有模式被匹配
	for (int i=0;i<4;i++)
	{
		//四个方向遍历		
		for (int j=0;j<model_num;j++)//model_num种模式遍历，每种模式遍历两次，正反各一次
		{
			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}
			if (flag) 
			{
				//匹配上某种模式
				count++;
				break;//此方向就不用再累加了
			}

			flag = true;
			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}
			if (flag) 
			{
				//匹配上某种模式
				count++;
				break;//此方向就不用再累加了
			}
		}
	}
	if (count>=2) return true;
	return false;
}


BOOL CGoBangGame::twolevel_alive_old(int x, int y, int color)
{
	//这是用定义直接做的判断，比较慢，因此被放弃，但仍然放在此处作为示范
	//计算有无二阶活子
	//即自己下一子后，对方不论走什么子，都会存在活子	
	if (ChessPoints[x][y]!=0) return false;//已经有子了，不可能是二阶活子
	if (can_five(color)) return false;//如果已经可以连成5子，那么去掉
	if (is_alive(x,y,color)) return false;//如果是一阶活子，那么去掉
	int i;

	ChessPoints[x][y] = color;//假设走了此子
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	BOOL flag = true;//表明是否对方每次走子时，自己都可以存在活子
	POINT roundpoints[8];//一个子周围8个方向上离它最近的空子位置

	getroundpoints(x,y,color,roundpoints);//此处，为了避免全盘搜索，采用了近似手法，只计算它周围的8个空子即可
	for(i=0;i<8;i++)
	{
		if (roundpoints[i].x!=0)//即是有效的位置
		{
			ChessPoints[roundpoints[i].x][roundpoints[i].y] = anticolor;//走对方的子
			if (!can_five(color)&&!has_alive(color)) 
			{
				//如果存在一种情况不能存在活子，也不存在直接生成5子，那么失败
				ChessPoints[roundpoints[i].x][roundpoints[i].y] = 0;
				flag = false;
				break;
			}
			ChessPoints[roundpoints[i].x][roundpoints[i].y] = 0;
		}
	}
	
	if (flag)
	{
		ChessPoints[x][y] = 0;
		return true;//必然是二阶活子
	}
	ChessPoints[x][y] = 0;
	return false;
}

void CGoBangGame::getroundpoints(int x, int y,int color,POINT roundpoints[])
{
	//注意，如果不足8个的话，那么后几个x,y都为0
	int counter = 0;
	int i;
	for(i=0;i<8;i++)//清空
	{
		roundpoints[i].x = 0;
		roundpoints[i].y = 0;
	}

	int tempx,tempy;
	for(i=-1;i<=1;i++)
		for(int j=-1;j<=1;j++)
		{
			if ((i==0)&&(j==0)) continue;
			for(tempx = x+i,tempy = y+j;(tempx>=1)&&(tempx<=m_xGrid)&&(tempy>=1)&&(tempy<=m_yGrid);tempx+=i,tempy+=j)
			{
				if ((ChessPoints[tempx][tempy]==0)&&has_roundcolor(tempx,tempy,color))
				{
					//找到，并且保证其周围有color的子
					roundpoints[counter].x = tempx;
					roundpoints[counter].y = tempy;
					counter++;
					break;
				}
			}
		}	
}

BOOL CGoBangGame::has_twolevel(int color,int &counter,POINT tempresult[])
{
	//判断一个棋盘有没有二阶活子
	//counter为候选结果的计数，tempresult为候选结果集
	counter = 0;
	//if (has_alive(color)) return false;//如果已经存在一阶活子，那么不再进行运算
	BOOL flag = false;	//返回值
	POINT closepoints[225];

	int count = 0;
	getclose(color,count,closepoints);		
	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			if (twolevel_alive(closepoints[i].x,closepoints[i].y,color)) 
			{
				tempresult[counter].x = closepoints[i].x;
				tempresult[counter].y = closepoints[i].y;
				counter++;
				flag = true;
			}
		}
	}
	return flag;
}

BOOL CGoBangGame::has_twolevel_pure(int color,int &counter,POINT tempresult[])
{

	//判断一个棋盘有没有二阶活子
	//counter为候选结果的计数，tempresult为候选结果集
	counter = 0;
	//if (has_alive(color)) return false;//如果已经存在一阶活子，那么不再进行运算

	BOOL flag = false;	//返回值

	POINT closepoints[225];
	int count = 0;
	getclose(color,count,closepoints);		
	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			if (twolevel_alive_pure(closepoints[i].x,closepoints[i].y,color)) 
			{
				tempresult[counter].x = closepoints[i].x;

				tempresult[counter].y = closepoints[i].y;

				counter++;

				flag = true;

			}

		}

	}

	return flag;

}



BOOL CGoBangGame::has_twolevel(int color)

{

	//判断一个棋盘有没有二阶活子

	//if (has_alive(color)) return false;//如果已经存在一阶活子，那么不再进行运算

	POINT closepoints[225];

	int count = 0;

	getclose(color,count,closepoints);		

	for(int i=0;i<count;i++)

	{

		if (closepoints[i].x!=0)

		{

			if (twolevel_alive(closepoints[i].x,closepoints[i].y,color)) 

			{

				return true;

			}

		}

	}

	return false;

}



BOOL CGoBangGame::has_twolevel_pure(int color)

{

	//判断一个棋盘有没有二阶活子

	//if (has_alive(color)) return false;//如果已经存在一阶活子，那么不再进行运算

	POINT closepoints[225];

	int count = 0;

	getclose(color,count,closepoints);		

	for(int i=0;i<count;i++)

	{

		if (closepoints[i].x!=0)

		{

			if (twolevel_alive_pure(closepoints[i].x,closepoints[i].y,color)) 

			{

				return true;

			}

		}

	}

	return false;

}



BOOL CGoBangGame::has_twolevel_four(int color)

{

	//判断一个棋盘有没有二阶活子(包含一个四子的)
	
	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	POINT closepoints[225];
	
	int count = 0;	
	
	getclose(color,count,closepoints);		
	
	for(int i=0;i<count;i++)
		
	{
		
		if (closepoints[i].x!=0)
			
		{
			
			if (twolevel_four(closepoints[i].x,closepoints[i].y,color)) 
				
			{
				
				//此处只能表明己方已经有冲四子，但是如果对方的一阶活子或者强二阶活子能够使得此强二阶活子消失				
				//那么此冲四子也不能采用！此处进行这个判断！！！
				//对方为了防这个子所下的子，不能是对方的一阶活子、canfour子
				
				ChessPoints[closepoints[i].x][closepoints[i].y]=color;//试己方的子
				int x,y;//对方用于防守的子
				can_five(color,x,y);
				
				if (!is_alive(x,y,anticolor)&&!is_canfour(anticolor,x,y))
				{
					ChessPoints[closepoints[i].x][closepoints[i].y]=0;//恢复己方的子				
					return true;				
				}
				ChessPoints[closepoints[i].x][closepoints[i].y]=0;//恢复己方的子				
				
			}
			
			
		}
		
	}
	
	return false;

}



BOOL CGoBangGame::has_twolevel_four(int color,int &counter,POINT tempresult[])

{

	//判断一个棋盘有没有二阶活子(包含一个四子的)

	//counter为候选结果的计数，tempresult为候选结果集

	counter = 0;
	
	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;	//返回值
	
	POINT closepoints[225];
	
	int count = 0;	
	
	getclose(color,count,closepoints);		
	
	for(int i=0;i<count;i++)	
	{		
		if (closepoints[i].x!=0)		
		{
			if (twolevel_four(closepoints[i].x,closepoints[i].y,color)) 		
			{
				
				//此处只能表明己方已经有冲四子，但是如果对方的一阶活子或者强二阶活子能够使得此强二阶活子消失				
				//那么此冲四子也不能采用！此处进行这个判断！！！
				//对方为了防这个子所下的子，不能是对方的一阶活子、canfour子
				
				ChessPoints[closepoints[i].x][closepoints[i].y]=color;//试己方的子
				int x,y;//对方用于防守的子
				can_five(color,x,y);
				
				if (!is_alive(x,y,anticolor)&&!is_canfour(anticolor,x,y))
				{
					flag = true;
					
					tempresult[counter].x = closepoints[i].x;
					
					tempresult[counter].y = closepoints[i].y;
					
					counter++;					
				}
				ChessPoints[closepoints[i].x][closepoints[i].y]=0;//恢复己方的子					
			}			
		}
		
	}
	return flag;
}

int CGoBangGame::random(int x, int y)
{
	//输出x和y之间的随机数，x和y均可取到
	srand(LOWORD(GetTickCount()));
	return rand()%(y-x+1)+x;

}

void CGoBangGame::getclosest(int color,int n,POINT closepoints[])
{
	//此函数功能为，得到当前所有空白格中，与color相邻最近的前n个点
	//主要用于缩小搜索范围，后来发现这样做有可能将重要的点排除在外
	//所以后来采用getclose来代替这个算法，这个算法保留使用

	int i,j;
	for (i=0;i<n;i++)
	{
		closepoints[i].x = 0;
		closepoints[i].y = 0;
	}

	int *dist = new int[n];//记录相应的距离值
	int maxvalue=0;//记录数组中最大的距离值
	int maxindex;//记录数组中最大的距离值所处的index
	int counter = 0;//数组中元素的个数

	for(i=1;i<=m_xGrid;i++)
	{
		for(j=1;j<=m_yGrid;j++)
		{
			if (ChessPoints[i][j]==0)
			{
				int sum = 0;
				for(int p=1;p<=m_xGrid;p++)
				{
					for(int q=1;q<=m_yGrid;q++)
					{
						if (ChessPoints[p][q]==color)
						{//计算相同颜色的距离值
							sum += (i-p)*(i-p)+(j-q)*(j-q);
						}
					}
				}
				if (counter<n)
				{
					//说明还没有填满，可以继续填
					dist[counter] = sum;
					closepoints[counter].x = i;
					closepoints[counter].y = j;
					if (sum>maxvalue)
					{
						maxvalue = sum;
						maxindex = counter;
					}
					counter++;
				}
				else
				{
					//已经填满，需要某种替换算法
					if (sum<maxvalue)
					{
						//可以插入，可直接替换原有最大值
						dist[maxindex] = sum;
						closepoints[maxindex].x = i;
						closepoints[maxindex].y = j;
						//但是要找回maxvalue和maxindex
						maxvalue = 0;
						for (int p=0;p<n;p++)
						{
							if (dist[p]>maxvalue)
							{
								maxvalue = dist[p];
								maxindex = p;
							}
						}
					}
				}
			}
		} //for(j=1;j<=m_yGrid;j++) 
	}	//for(i=1;i<=m_xGrid;i++)

	delete []dist;
}


void CGoBangGame::clear()
{
	//for(int i=1;i<=15;i++)
	//	for(int j=1;j<=15;j++)
	//	{
	//		ChessPoints[i][j] = 0;
	//	}
	memset( ChessPoints,0,sizeof(ChessPoints) );
}

void CGoBangGame::clearhint()
{
	for(int i=1;i<=X_MAX_GRID-1;i++)
		for(int j=1;j<=Y_MAX_GRID-1;j++)
		{
			if (ChessPoints[i][j]>2)
				ChessPoints[i][j] = 0;
		}
}

BOOL CGoBangGame::make_alive(int color, int &counter, POINT tempresult[])
{

	//此函数用来生成制造一阶活子的棋
	//counter为候选结果的计数，tempresult为候选结果集
	counter = 0;

	//if (has_alive(color)) return false;//如果已经存在一阶活子，那么不再进行运算
	BOOL flag = false;	//返回值
	POINT closepoints[225];
	int count = 0;
	getclose(color,count,closepoints);

	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			if (is_makealive(closepoints[i].x,closepoints[i].y,color)) 
			{
				BOOL tempflag = true;
				/*
				if (m_grade==5)
				{
					int anticolor;//对方的color
					
					if (color==1) anticolor = 2;
					
					else anticolor = 1;					
					POINT defentpoint[225];
					int defent_count;
					ChessPoints[closepoints[i].x][closepoints[i].y] = color;
					has_defent_point(color,defent_count,defentpoint);
					m_grade = 4;
					for (int j=0;j<defent_count;j++)
					{
						if (is_beatpoint(anticolor,defentpoint[j].x,defentpoint[j].y))
						{
							tempflag = false;
							break;
						}
					}
					m_grade = 5;
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;
				}
				*/
				if (tempflag)
				{
					tempresult[counter].x = closepoints[i].x;					
					tempresult[counter].y = closepoints[i].y;				
					counter++;			
					flag = true;
				}				
			}
		}
	}
	return flag;
}



BOOL CGoBangGame::make_twolevel(int color, int &counter, POINT tempresult[])
{

	//此函数用来生成制造二阶活子的棋
	//counter为候选结果的计数，tempresult为候选结果集
	counter = 0;
	//if (has_twolevel(color)) return false;//如果已经存在二阶活子，那么不再进行运算

	BOOL flag = false;	//返回值
	POINT closepoints[225];
	int count = 0;
	getclose_subqian(color,count,closepoints);		

	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			ChessPoints[closepoints[i].x][closepoints[i].y] = color;
			if (has_twolevel(color)) 
			{

				//如果存在二阶活子，记录到结果集
				tempresult[counter].x = closepoints[i].x;
				tempresult[counter].y = closepoints[i].y;
				counter++;
				flag = true;
			}

			ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式
		}
	}
	return flag;
}

void CGoBangGame::design(int x, int y, int color)
{
	//新的赋值算法，采用模式匹配的方法
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	int model[][9]={//此数组表示各种模式，包括此子前四个和后四个子的排列
		//-1表示任意，1表示相同颜色，0表示空，2表示(x,y)所在位置
		//3表示其他颜色的子
		//以下模式按照赋值的分数从低到高排列

		//5(0-1)

		{-1,3,1,0,2,0,-1,-1,-1},//31_*_
		{-1,0,1,0,2,3,-1,-1,-1},//_1_*3
		//10(2-4)
		{-1,0,1,0,2,0,-1,-1,-1},//_1_*_
		{-1,-1,3,1,2,0,-1,-1,-1},//31*_
		{-1,-1,0,1,2,3,-1,-1,-1},//_1*3

		//20(5-5)
		{-1,-1,0,1,2,0,-1,-1,-1},//_1*_
		//40(6-11)		
		{3,1,0,1,2,0,-1,-1,-1},//31_1*_
		{3,1,1,0,2,0,-1,-1,-1},//311_*_
		{-1,3,1,0,2,1,0,-1,-1},//31_*1_		
		{0,1,0,1,2,3,-1,-1,-1},//_1_1*3
		{0,1,1,0,2,3,-1,-1,-1},//_11_*3
		{-1,0,1,0,2,1,3,-1,-1},//_1_*13

		//50(40+50>80)(12-16)
		{-1,3,1,1,2,0,-1,-1,-1},//311*_
		{-1,-1,3,1,2,1,0,-1,-1},//31*1_
		{-1,0,1,1,2,3,-1,-1,-1},//_11*3
		{3,0,1,1,2,0,3,-1,-1},//3_11*_3
		{-1,3,0,1,2,1,0,3,-1},//3_1*1_3

		//80(17-19)		
		{0,1,0,1,2,0,-1,-1,-1},//_1_1*_
		{0,1,1,0,2,0,-1,-1,-1},//_11_*_
		{-1,0,1,0,2,1,0,-1,-1},//_1_*1_
		//90(20-22)
		{-1,0,0,1,2,1,0,-1,-1},//__1*1_		
		{0,0,1,1,2,0,-1,-1,-1},//__11*_
		{-1,0,1,1,2,0,0,-1,-1},//_11*__

		//100(80+100>160)(23-28)
		{-1,-1,1,0,2,1,1,-1,-1},//1_*11
		{-1,-1,-1,1,2,0,1,1,-1},//1*_11
		{-1,-1,-1,1,2,1,0,1,-1},//1*1_1
		{-1,-1,-1,-1,2,0,1,1,1},//*_111
		{-1,-1,-1,-1,2,1,0,1,1},//*1_11
		{-1,-1,-1,-1,2,1,1,0,1},//*11_1

		//160(29-32)
		{-1,-1,0,1,2,1,1,3,-1},//_1*113
		{-1,-1,3,1,2,1,1,0,-1},//31*11_		
		{-1,-1,-1,0,2,1,1,1,3},//_*1113
		{-1,-1,-1,3,2,1,1,1,0},//3*111_				

		//320(33-34)
		{-1,-1,0,1,2,1,1,0,-1},//_1*11_
		{-1,-1,-1,0,2,1,1,1,0},//_*111_			

		//640(35-37)
		{-1,-1,1,1,2,1,1,-1,-1},//11*11
		{-1,-1,-1,1,2,1,1,1,-1},//1*111
		{-1,-1,-1,-1,2,1,1,1,1},//*1111
	};

	int model_num = 38;
	int point_num = 9;
	BOOL flag;
	for (int i=0;i<4;i++)
	{
		//四个方向遍历
		int tempvalue = 0;
		int j;
		BOOL find;
		for (j=model_num-1,find=false;!find&&(j>=0);j--)//model_num种模式遍历，每种模式遍历两次，正反各一次
		{//从大到小匹配模式，保证正确性

			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==3)
				{
					if (getcolor(x,i,k-4,y)!=anticolor)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}
			}

			if (flag) 
			{
				//匹配上某种模式
				find = true;
				if ((j==0)||(j==1))
				{
					tempvalue = 5;
				}

				else if ((j>=2)&&(j<=4))
				{
					tempvalue = 10;
				}

				else if (j==5)
				{
					tempvalue = 20;
				}

				else if ((j>=6)&&(j<=11))
				{
					tempvalue = 40;
				}

				else if ((j>=12)&&(j<=16))
				{
					tempvalue = 50;
				}				

				else if ((j>=17)&&(j<=19))
				{
					tempvalue = 80;
				}

				else if ((j>=20)&&(j<=22))
				{
					tempvalue = 90;
				}

				else if ((j>=23)&&(j<=28))
				{
					tempvalue = 100;
				}

				else if ((j>=29)&&(j<=32))
				{
					tempvalue = 160;
				}

				else if ((j>=33)&&(j<=34))
				{
					tempvalue = 320;
				}

				else if ((j>=35)&&(j<=37))
				{
					tempvalue = 640;
				}
			}

			flag = true;

			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==3)			
				{
					if (getcolor(x,i,4-k,y)!=anticolor)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}
			}

			if (flag) 
			{
				//匹配上某种模式
				find = true;				
				if ((j==0)||(j==1))
				{
					if (tempvalue<5) tempvalue = 5;
				}

				else if ((j>=2)&&(j<=4))
				{
					if (tempvalue<10) tempvalue = 10;
				}

				else if (j==5)
				{
					if (tempvalue<20) tempvalue = 20;
				}

				else if ((j>=6)&&(j<=11))
				{
					if (tempvalue<40) tempvalue = 40;
				}

				else if ((j>=12)&&(j<=16))
				{
					if (tempvalue<50) tempvalue = 50;
				}

				else if ((j>=17)&&(j<=19))
				{
					if (tempvalue<80) tempvalue = 80;
				}

				else if ((j>=20)&&(j<=22))
				{
					if (tempvalue<90) tempvalue = 90;
				}

				else if ((j>=23)&&(j<=28))
				{
					if (tempvalue<100) tempvalue = 100;
				}

				else if ((j>=29)&&(j<=32))
				{
					if (tempvalue<160) tempvalue = 160;
				}

				else if ((j>=33)&&(j<=34))
				{
					if (tempvalue<320) tempvalue = 320;
				}

				else if ((j>=35)&&(j<=37))
				{
					if (tempvalue<640) tempvalue = 640;
				}
			}
		}
		m_ptvalue[x][y] += tempvalue;
	}
}

void CGoBangGame::design(int color)
{
	//此处为赋值算法，沿用以前的经典赋值算法，但是有所改进，跟便于维护和阅读
	clearvalue();	
	POINT closepoints[225];
	int count = 0;
	getclose(color,count,closepoints);			

	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			design(closepoints[i].x,closepoints[i].y,color);
		}
	}		
}

int CGoBangGame::getcolor(int x,int i,int move,int y)
{
	//用于得到棋盘上某一点的颜色，主要为了减少判断index合法性的代码
	//move为移动的位移，i为方向取值0，1，2，3

	POINT d[4];
	d[0].x = 1;d[0].y = 0;//横方向
	d[1].x = 0;d[1].y = 1;//纵方向
	d[2].x = 1;d[2].y = -1;//斜右上
	d[3].x = 1;d[3].y = 1;//斜左上

	x = x+move*d[i].x;
	y = y+move*d[i].y;

	if ((x>=1)&&(x<=m_xGrid)&&(y>=1)&&(y<=m_yGrid))
	{
		return ChessPoints[x][y];
	}
	return -1;
}



int CGoBangGame::getmax(int &counter, POINT tempresult[])
{
	//此方法用来得到赋值后的最大赋值点，并返回结果集	

	counter = 0;

	int maxvalue = 0;//最大赋值的数值

	for(int color = 1;color<=2;color++)//对双方都要判断，取最大值
	{
		design(color);//赋值过程

		//赋值完毕
		POINT closepoints[225];

		int count = 0;

		getclose(color,count,closepoints);			

		for(int i=0;i<count;i++)
		{
			if (closepoints[i].x!=0)
			{
				if (ChessPoints[closepoints[i].x][closepoints[i].y]==0)
				{
					if (m_ptvalue[closepoints[i].x][closepoints[i].y]>maxvalue)
					{

						//新最大值，重置tempresult

						tempresult[0].x = closepoints[i].x;

						tempresult[0].y = closepoints[i].y;

						counter = 1;

						maxvalue = m_ptvalue[closepoints[i].x][closepoints[i].y];					

					}

					else if (m_ptvalue[closepoints[i].x][closepoints[i].y]==maxvalue)

					{

						//加入候选结果集

						tempresult[counter].x = closepoints[i].x;

						tempresult[counter].y = closepoints[i].y;

						counter++;

					}

				}				

			}

		}		

	}

	return maxvalue;

}



void CGoBangGame::selectmax(int color,int &x, int &y, POINT tempresult[], int n,BOOL defent,int m)

{

	//从待选择的活子中得到赋值更高的活子
	//tempresult为候选活子集，n为此集合元素个数
	//defent表示是否为defent状态，这时
	//color为计算机子的颜色，如果计算机自己有好值，那么应该是人的2倍	
	//m为控制算子，用于控制是否对对方进行判断，孤注一掷时使用，默认值为2，孤注一掷时为1

	if (n==1)
	{
		//只有一个点，那么直接返回
		x = tempresult[0].x;
		y = tempresult[0].y;
		return;
	}

	POINT local_result[225];//暂存的局部候选元素

	int counter = 0;//对local_result的计数

	int maxvalue = 0;//最大赋值的数值

	clearvalue();

	for (int j=1;j<=m;j++)
	{
		//先对自身的子赋值
		int i;
		for(i=0;i<n;i++)
		{
			design(tempresult[i].x,tempresult[i].y,color);//对单个子赋值
		}

		for(i=0;i<n;i++)
		{

			if (m_ptvalue[tempresult[i].x][tempresult[i].y]>maxvalue)

			{

				//新最大值，重置local_result

				local_result[0].x = tempresult[i].x;

				local_result[0].y = tempresult[i].y;

				counter = 1;

				maxvalue = m_ptvalue[tempresult[i].x][tempresult[i].y];					

			}

			else if (m_ptvalue[tempresult[i].x][tempresult[i].y]==maxvalue)

			{

				//加入候选结果集

				local_result[counter].x = tempresult[i].x;

				local_result[counter].y = tempresult[i].y;

				counter++;

			}

		}

		if (defent)	maxvalue = maxvalue * 2 + 10;

		if (color==1) color = 2;

		else color = 1;

	}

	//此时，候选元素已经在local_result中

	int tempi = random(0,counter-1);

	x = local_result[tempi].x;

	y = local_result[tempi].y;

}



void CGoBangGame::design_depart(int x, int y, int color)
{
	//新的赋值算法，采用模式匹配的方法
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	int model[][9]={//此数组表示各种模式，包括此子前四个和后四个子的排列

		//-1表示任意，1表示相同颜色，0表示不是3，2表示(x,y)所在位置
		//3表示其他颜色的子
		//以下模式按照赋值的分数从低到高排列
		//5(0-1)

		{-1,3,1,0,2,0,-1,-1,-1},//31_*_
		{-1,0,1,0,2,3,-1,-1,-1},//_1_*3
		//10(2-4)
		{-1,0,1,0,2,0,-1,-1,-1},//_1_*_
		{-1,-1,3,1,2,0,-1,-1,-1},//31*_
		{-1,-1,0,1,2,3,-1,-1,-1},//_1*3

		//20(5-5)
		{-1,-1,0,1,2,0,-1,-1,-1},//_1*_
		//40(6-9)		
		{3,1,0,1,2,0,-1,-1,-1},//31_1*_
		{3,1,1,0,2,0,-1,-1,-1},//311_*_
		//{-1,3,1,0,2,1,0,-1,-1},//31_*1_		
		{0,1,0,1,2,3,-1,-1,-1},//_1_1*3
		{0,1,1,0,2,3,-1,-1,-1},//_11_*3
		//{-1,0,1,0,2,1,3,-1,-1},//_1_*13
		//50(40+50>80)(10-12)
		{-1,3,1,1,2,0,-1,-1,-1},//311*_
		//{-1,-1,3,1,2,1,0,-1,-1},//31*1_
		{-1,0,1,1,2,3,-1,-1,-1},//_11*3
		{3,0,1,1,2,0,3,-1,-1},//3_11*_3
		//{-1,3,0,1,2,1,0,3,-1},//3_1*1_3
		//80(13-14)		

		{0,1,0,1,2,0,-1,-1,-1},//_1_1*_
		{0,1,1,0,2,0,-1,-1,-1},//_11_*_

		//{-1,0,1,0,2,1,0,-1,-1},//_1_*1_
		//90(15-16)
		//{-1,0,0,1,2,1,0,-1,-1},//__1*1_		
		{0,0,1,1,2,0,-1,-1,-1},//__11*_
		{-1,0,1,1,2,0,0,-1,-1},//_11*__


		//100(80+100>160)(17-19)
		//{-1,-1,1,0,2,1,1,-1,-1},//1_*11
		//{-1,-1,-1,1,2,0,1,1,-1},//1*_11
		//{-1,-1,-1,1,2,1,0,1,-1},//1*1_1

		{-1,-1,-1,-1,2,0,1,1,1},//*_111
		{-1,-1,-1,-1,2,1,0,1,1},//*1_11
		{-1,-1,-1,-1,2,1,1,0,1},//*11_1

		//160(20-21)
		//{-1,-1,0,1,2,1,1,3,-1},//_1*113
		//{-1,-1,3,1,2,1,1,0,-1},//31*11_		
		{-1,-1,-1,0,2,1,1,1,3},//_*1113
		{-1,-1,-1,3,2,1,1,1,0},//3*111_				
		//320(22-23)
		{-1,-1,0,1,2,1,1,0,-1},//_1*11_
		{-1,-1,-1,0,2,1,1,1,0},//_*111_			

		//640(24-26)
		{-1,-1,1,1,2,1,1,-1,-1},//11*11
		{-1,-1,-1,1,2,1,1,1,-1},//1*111
		{-1,-1,-1,-1,2,1,1,1,1},//*1111
	};

	int model_num = 27;
	int point_num = 9;
	BOOL flag;
	for (int i=0;i<4;i++)
	{

		//四个方向遍历
		int tempvalue = 0;
		int j;
		BOOL find;
		for (j=model_num-1,find=false;!find&&(j>=0);j--)//model_num种模式遍历，每种模式遍历两次，正反各一次
		{//从大到小匹配模式，保证正确性

			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)==anticolor)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==3)
				{
					if (getcolor(x,i,k-4,y)!=anticolor)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}
			}

			if (flag) 
			{
				//匹配上某种模式
				find = true;
				if ((j==0)||(j==1))
				{
					tempvalue = 5;
				}

				else if ((j>=2)&&(j<=4))
				{
					tempvalue = 10;
				}

				else if (j==5)
				{
					tempvalue = 20;
				}

				else if ((j>=6)&&(j<=9))
				{
					tempvalue = 40;
				}

				else if ((j>=10)&&(j<=12))
				{
					tempvalue = 50;
				}				

				else if ((j>=13)&&(j<=14))
				{
					tempvalue = 80;
				}

				else if ((j>=15)&&(j<=16))
				{
					tempvalue = 90;
				}

				else if ((j>=17)&&(j<=19))
				{
					tempvalue = 100;
				}

				else if ((j>=20)&&(j<=21))
				{
					tempvalue = 160;
				}

				else if ((j>=22)&&(j<=23))
				{
					tempvalue = 320;
				}

				else if ((j>=24)&&(j<=26))
				{
					tempvalue = 640;
				}
			}

			flag = true;

			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)==anticolor)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==3)
				{
					if (getcolor(x,i,4-k,y)!=anticolor)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}
			}

			if (flag) 
			{
				//匹配上某种模式
				find = true;				
				if ((j==0)||(j==1))
				{
					if (tempvalue<5) tempvalue = 5;
				}

				else if ((j>=2)&&(j<=4))
				{
					if (tempvalue<10) tempvalue = 10;
				}

				else if (j==5)
				{
					if (tempvalue<20) tempvalue = 20;
				}

				else if ((j>=6)&&(j<=9))
				{
					if (tempvalue<40) tempvalue = 40;
				}

				else if ((j>=10)&&(j<=12))
				{
					if (tempvalue<50) tempvalue = 50;

				}

				else if ((j>=13)&&(j<=14))
				{
					if (tempvalue<80) tempvalue = 80;
				}

				else if ((j>=15)&&(j<=16))
				{
					if (tempvalue<90) tempvalue = 90;
				}

				else if ((j>=17)&&(j<=19))
				{
					if (tempvalue<100) tempvalue = 100;
				}

				else if ((j>=20)&&(j<=21))
				{
					if (tempvalue<160) tempvalue = 160;
				}

				else if ((j>=22)&&(j<=23))
				{
					if (tempvalue<320) tempvalue = 320;
				}

				else if ((j>=24)&&(j<=26))
				{
					if (tempvalue<640) tempvalue = 640;
				}
			}
		}
		m_ptvalue[x][y] += tempvalue;
	}
}

void CGoBangGame::design_old(int x, int y, int color)
{
	//对每一个点进行赋值
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	int start,end,total;
	if (ChessPoints[x][y]==0)//只对空格赋值
	{
		for(int i=0;i<4;i++)//四个方向
		{
			if (getcolor(x,i,-1,y)==anticolor) start=0;
			else
			{
				for(start=2;getcolor(x,i,-start,y)==color;start++);//计算此子之前的同样子个数
				start=start-1;
			}

			if (getcolor(x,i,+1,y)==anticolor) end=0;
			else
			{
				for(end=2;getcolor(x,i,+end,y)==color;end++);//计算此子之后的同样子个数
				end=end-1;
			}

			total=start+end;//两边总个数
			if (getcolor(x,i,-1,y)==0) total=total-1;
			if (getcolor(x,i,+1,y)==0) total=total-1;
			switch (total)
			{
				case 0:break;
				case 1:
					if ((start==1)&&(end==2))//邻近的两个都是空格
					{
						if (getcolor(x,i,+3,y)==0)
							m_ptvalue[x][y]=m_ptvalue[x][y]+2;
					}

					if ((start==2)&&(end==1))//邻近的两个都是空格
					{
						if (getcolor(x,i,-3,y)==0)
							m_ptvalue[x][y]=m_ptvalue[x][y]+2;
					}

					if ((start==1)&&(end==1))//邻近的一个是空格
					{
						if ((getcolor(x,i,-1,y)==color)&&(getcolor(x,i,-2,y)==0))
						{
							if (getcolor(x,i,-3,y)==color) m_ptvalue[x][y]=m_ptvalue[x][y]+10;
								else m_ptvalue[x][y]=m_ptvalue[x][y]+5;
						}

						if ((getcolor(x,i,+1,y)==color)&&(getcolor(x,i,+2,y)==0))
						{
							if (getcolor(x,i,+3,y)==color)	m_ptvalue[x][y]=m_ptvalue[x][y]+10;
								else m_ptvalue[x][y]=m_ptvalue[x][y]+5;
						}

					}

				  break;

				case 2:
					switch(start)
					{
						case 1:
							switch(end)
							{
								case 1:
									if (getcolor(x,i,-2,y)==0)
									{
										if (getcolor(x,i,+2,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+10;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+5;
									}
									else if (getcolor(x,i,+2,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+5;
									break;
								case 2:
									if (getcolor(x,i,-1,y)==color)
									{
										if (getcolor(x,i,-2,y)==0)
										{
											if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+8;
												else m_ptvalue[x][y]=m_ptvalue[x][y]+2;
										}
										else if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+2;
									}
									else if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+10;
									else m_ptvalue[x][y]=m_ptvalue[x][y]+5; 
									break;

								case 3:
									if (getcolor(x,i,+4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+8;
										else m_ptvalue[x][y]=m_ptvalue[x][y]+2;
									break;
							}
							break;

						case 2:
							switch(end)
							{
								case 1:
									if (getcolor(x,i,+1,y)==color)
									{
										if (getcolor(x,i,+2,y)==0)
										{
											if (getcolor(x,i,-3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+8;
												else m_ptvalue[x][y]=m_ptvalue[x][y]+2;
										}
										else if (getcolor(x,i,-3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+2;
									}
									else if (getcolor(x,i,-3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+10;
									else m_ptvalue[x][y]=m_ptvalue[x][y]+5; 
									break;

								case 2:
									break;
							}
							break;

					   case 3:
							if (getcolor(x,i,-4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+8;
								else m_ptvalue[x][y]=m_ptvalue[x][y]+2;
							break;
					}
					break;

				case 3:
					switch(start)
					{
						case 1:
							switch(end)
							{
								case 2:
									if (getcolor(x,i,-2,y)==0) 
									{
										if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+20;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+10;
									}

									else if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+10;
									break;

								case 3:
									if (getcolor(x,i,-1,y)==color)
									{
										if (getcolor(x,i,-2,y)==0)
										{
											if (getcolor(x,i,+4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+15;
												else m_ptvalue[x][y]=m_ptvalue[x][y]+8;
										}
										else if (getcolor(x,i,+4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+8;
									}
									else if (getcolor(x,i,+4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+20;
									else m_ptvalue[x][y]=m_ptvalue[x][y]+10; 
									break;

								case 4:
									if (getcolor(x,i,+5,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+15;
										else m_ptvalue[x][y]=m_ptvalue[x][y]+8; 
									break;
							}
							break;

						case 2:
							switch(end)
							{
								case 1:
									if (getcolor(x,i,-3,y)==0) 
									{
										if (getcolor(x,i,+2,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+20;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+10;
									}
									else if (getcolor(x,i,+2,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+10;
									break;
								case 2:
									if (getcolor(x,i,-3,y)==0)
									{
										if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+15;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+8;
									}
									else if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+8; 
									break;

								case 3:
									if (getcolor(x,i,-3,y)==0) 
									{
										if (getcolor(x,i,+4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+12;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+6;
									}
									else if (getcolor(x,i,+4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+2; 
									break;
							}
							break;

						case 3: 
							switch(end)
							{
								case 1:
									if (getcolor(x,i,+1,y)==color)
									{
										if (getcolor(x,i,+2,y)==0)
										{
											if (getcolor(x,i,-4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+15;
												else m_ptvalue[x][y]=m_ptvalue[x][y]+8;
										}
										else if (getcolor(x,i,-4,y)==0)m_ptvalue[x][y]=m_ptvalue[x][y]+8;
									}
									else if (getcolor(x,i,-4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+20;
									else m_ptvalue[x][y]=m_ptvalue[x][y]+10; 
									break;
								case 2:
									if (getcolor(x,i,-4,y)==0) 
									{
										if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+12;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+6;
									}
									else if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+2; 
									break;
							}
							break;
						case 4:
							if (getcolor(x,i,-5,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+15;
								else m_ptvalue[x][y]=m_ptvalue[x][y]+8; 
							break;
					}
					break;

				case 4:
					switch(start)
					{
						case 1:
							switch(end)
							{
								case 3:
									if (getcolor(x,i,-2,y)==0) 
									{
										if (getcolor(x,i,+4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+120;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+20;
									}
									else if (getcolor(x,i,+4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+20;
								break;
								case 4:
									if (getcolor(x,i,-1,y)==color)
									{
										if (getcolor(x,i,-2,y)==0)
										{
											if (getcolor(x,i,+5,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+200;
												else m_ptvalue[x][y]=m_ptvalue[x][y]+15;
										}
										else if (getcolor(x,i,+5,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+15;
									}
									else if (getcolor(x,i,+5,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+120;
									else m_ptvalue[x][y]=m_ptvalue[x][y]+20; 
									break;
								case 5:
									if (getcolor(x,i,+6,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+200;
										else m_ptvalue[x][y]=m_ptvalue[x][y]+15; 
									break;
							}
							break;
						case 2:
							switch(end)
							{
								case 2:
									if (getcolor(x,i,-3,y)==0)
									{
										if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+120;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+20;
									}
									else if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+20;
									break;
								case 3:
									if (getcolor(x,i,-4,y)==0) 
									{
										if (getcolor(x,i,+4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+200;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+15;
									}
									else if (getcolor(x,i,+4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+15;
									break;
								case 4:
									if (getcolor(x,i,-3,y)==0) 
									{
										if (getcolor(x,i,+5,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+100;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+12;
									}
									else if (getcolor(x,i,+5,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+12;
									break;
							}
							break;
						case 3:
							switch(end)
							{
								case 1:
									if (getcolor(x,i,-4,y)==0) 
									{
										if (getcolor(x,i,+2,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+120;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+20;
									}
									else if (getcolor(x,i,+2,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+20;
									break;

								case 2:
									if (getcolor(x,i,-4,y)==0) 
									{
										if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+200;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+15;
									}
									else if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+15;
									break;
								case 3:
									if (getcolor(x,i,-4,y)==0)
									{
										if (getcolor(x,i,+4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+100;
											else m_ptvalue[x][y]=m_ptvalue[x][y]+12;
									}
									else if (getcolor(x,i,+4,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+12;
									break;
							}
							break;
						case 4:
							switch(end)
							{
								case 1:
									if (getcolor(x,i,+1,y)==color)
									{
										if (getcolor(x,i,+2,y)==0)
										{
											if (getcolor(x,i,-5,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+200;
												else m_ptvalue[x][y]=m_ptvalue[x][y]+15;

										}
									else if (getcolor(x,i,-5,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+15;
									}
									else if (getcolor(x,i,-5,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+120;
									else m_ptvalue[x][y]=m_ptvalue[x][y]+20; 
									break;
								case 2:
									if (getcolor(x,i,-4,y)==0)
									{
										if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+100;

											else m_ptvalue[x][y]=m_ptvalue[x][y]+12;
									}
									else if (getcolor(x,i,+3,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+12;
									break;
							}
							break;
						case 5:
							if (getcolor(x,i,-6,y)==0) m_ptvalue[x][y]=m_ptvalue[x][y]+200;
								else m_ptvalue[x][y]=m_ptvalue[x][y]+15;
							break;
					}
					break;
			}
		}
	}	
}

void CGoBangGame::clearvalue()
{
	//清空value的原先赋值
	int i,j;
	for(i=1;i<=m_xGrid;i++)
		for(j=1;j<=m_yGrid;j++)
		{
			m_ptvalue[i][j] = 0;//先清空
		}
}

BOOL CGoBangGame::has_canfour(int color, int &counter, POINT tempresult[])
{
	//找到可以连成4子的子，这些子不是活子，因为可以被挡住，但是可以用来牵制敌人
	counter = 0;

	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	BOOL flag = false;	//返回值

	POINT closepoints[225];
	int count = 0;	
	getclose(color,count,closepoints);		
	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			if (is_canfour(color,closepoints[i].x,closepoints[i].y)) 
			{

				//此处只能表明己方已经有冲四子，但是如果对方的一阶活子或者强二阶活子能够使得此强二阶活子消失
				//那么此冲四子也不能采用！此处进行这个判断！！！
				//对方为了防这个子所下的子，不能是对方的一阶活子、二阶活子、twoalive子、canfour子、qianmodel子,makealiveandtwolevel子,maketwolevelalive
				
				ChessPoints[closepoints[i].x][closepoints[i].y]=color;//试己方的子
				int x,y;//对方用于防守的子
				can_five(color,x,y);

				if (!is_alive(x,y,anticolor)&&!is_canfour(anticolor,x,y)&&!twolevel_alive(x,y,anticolor)&&!is_maketwoalive(x,y,anticolor)&&!is_makealiveandtwolevel(anticolor,x,y)&&!is_maketwolevelalive(x,y,anticolor))
				{					
					
					BOOL tempflag = false;
					if (!is_low)
					{
						is_low = true;
						if (!is_qianmodel(anticolor,x,y))
						{						
							tempflag = true;							
						}
						is_low = false;
					}
					else
					{
						tempflag = true;			
					}

					/*
					if (tempflag&&(m_grade==5))
					{
						//5级电脑判断走出对方的子后，是否存在强二阶牵制子匹配
						ChessPoints[x][y] = anticolor;
						m_grade = 4;//降级，避免发生循环调用
						if (has_twolevelqianmodel_qiang(anticolor))
						{
							//存在强二阶牵制子匹配，不记录
							tempflag = false;
						}
						m_grade = 5;//恢复等级
						ChessPoints[x][y] = 0;
					}
					*/
					if (tempflag)
					{
						flag = true;
						tempresult[counter].x = closepoints[i].x;						
						tempresult[counter].y = closepoints[i].y;						
						counter++;
					}					
				}
				ChessPoints[closepoints[i].x][closepoints[i].y]=0;//恢复己方的子				
			}			
		}
	}
	return flag;
}


BOOL CGoBangGame::has_canfour(int color)

{

	//找到可以连成4子的子，这些子不是活子，因为可以被挡住，但是可以用来牵制敌人

	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	POINT closepoints[225];
	
	int count = 0;	
	
	getclose(color,count,closepoints);		
	
	for(int i=0;i<count;i++)
		
	{
		
		if (closepoints[i].x!=0)
			
		{
			
			if (is_canfour(color,closepoints[i].x,closepoints[i].y)) 
				
			{
				
				//此处只能表明己方已经有冲四子，但是如果对方的一阶活子或者强二阶活子能够使得此强二阶活子消失
				
				//那么此冲四子也不能采用！此处进行这个判断！！！
				//对方为了防这个子所下的子，不能是对方的一阶活子、二阶活子、twoalive子、canfour子、qianmodel子
				
				ChessPoints[closepoints[i].x][closepoints[i].y]=color;//试己方的子
				int x,y;//对方用于防守的子
				can_five(color,x,y);
				
				if (!is_alive(x,y,anticolor)&&!is_canfour(anticolor,x,y)&&!twolevel_alive(x,y,anticolor)&&!is_maketwoalive(x,y,anticolor)&&!is_makealiveandtwolevel(anticolor,x,y)&&!is_maketwolevelalive(x,y,anticolor))
				{
					
					if (!is_low)
					{
						is_low = true;
						if (!is_qianmodel(anticolor,x,y))
						{
							ChessPoints[closepoints[i].x][closepoints[i].y]=0;//恢复己方的子
							return true;
						}
						is_low = false;
					}
					else
					{
						ChessPoints[closepoints[i].x][closepoints[i].y]=0;//恢复己方的子
						return true;
					}
					
				}
				ChessPoints[closepoints[i].x][closepoints[i].y]=0;//恢复己方的子				
				
			}	
			
		}
		
	}
	
	return false;

}





BOOL CGoBangGame::has_canfive(int color, int &counter, POINT tempresult[])

{

	//找到可以连成5子的子

	counter = 0;

	BOOL flag = false;	//返回值

	POINT closepoints[225];

	int count = 0;

	getclose(color,count,closepoints);			

	for(int i=0;i<count;i++)

	{

		if (closepoints[i].x!=0)

		{

			if (is_five(color,closepoints[i].x,closepoints[i].y)) 

			{

				tempresult[counter].x = closepoints[i].x;

				tempresult[counter].y = closepoints[i].y;

				counter++;

				flag = true;

			}

		}

	}

	return flag;	

}

BOOL CGoBangGame::has_canfour_old(int color, int &counter, POINT tempresult[])
{
	//找到可以连成4子的子，这些子不是活子，因为可以被挡住，但是可以用来牵制敌人
	counter = 0;
	BOOL flag = false;
	for (int i=1;i<=m_xGrid;i++)
		for (int j=1;j<=m_yGrid;j++)
		{
			if (ChessPoints[i][j]==0)
			{
				ChessPoints[i][j] = color;
				if (can_five(color))
				{
					//找到此子！
					ChessPoints[i][j] = 0;
					tempresult[counter].x = i;
					tempresult[counter].y = j;
					counter++;
					flag = true;
				}
				ChessPoints[i][j] = 0;
			}
		}
	return flag;
}

BOOL CGoBangGame::has_qian(int color, int &counter, POINT tempresult[])
{
	//全部牵制子，包括一阶活子、二阶活子和能连成4的牵制子，以及制造一阶活子的子
	counter = 0;
	POINT local_tempresult[225];
	int count=0;
	if (has_alive(color,count,local_tempresult))
	{		
		merge(tempresult,counter,local_tempresult,count);
	}

	if (has_twolevel(color,count,local_tempresult))
	{
		merge(tempresult,counter,local_tempresult,count);
	}
	
	if (has_canfour(color,count,local_tempresult))
	{
		merge(tempresult,counter,local_tempresult,count);
	}

	if (make_alive(color,count,local_tempresult))
	{
		merge(tempresult,counter,local_tempresult,count);
	}

	if (counter==0) return false;		
	return true;
}



BOOL CGoBangGame::has_qian_nocanfour(int color, int &counter, POINT tempresult[])

{

	//部分牵制子，包括一阶活子、二阶活子以及制造一阶活子的子

	counter = 0;

	POINT local_tempresult[225];

	int count=0;

	if (has_alive(color,count,local_tempresult))

	{		

		merge(tempresult,counter,local_tempresult,count);

	}

	if (has_twolevel(color,count,local_tempresult))

	{

		merge(tempresult,counter,local_tempresult,count);

	}	

	if (make_alive(color,count,local_tempresult))

	{

		merge(tempresult,counter,local_tempresult,count);

	}

	if (counter==0) return false;		

	return true;

}



void CGoBangGame::getclose(int color, int &counter, POINT tempresult[])
{
	//用来代替getclosest，其算法为将每个子的roundpoints都合并到一起返回
	counter = 0;
	POINT local_tempresult[8];
	POINT local_round[8];//周围子的周围子
	for (int i=1;i<=m_xGrid;i++)
		for (int j=1;j<=m_yGrid;j++)
		{
			if (ChessPoints[i][j]==color)
			{
				getroundpoints(i,j,color,local_tempresult);
				merge(tempresult,counter,local_tempresult,8);
				for (int k=0;k<8;k++)
				{
					//将周围子的周围子也加进来
					if (local_tempresult[k].x!=0)
					{
						ChessPoints[local_tempresult[k].x][local_tempresult[k].y] = color;
						getroundpoints(local_tempresult[k].x,local_tempresult[k].y,color,local_round);
						merge(tempresult,counter,local_round,8);
						ChessPoints[local_tempresult[k].x][local_tempresult[k].y] = 0;
					}
				}
			}			
		}

/*
	//加入可以得到4的子，由于可能比较远，所以不能落了
	int count;
	POINT canfour[225];
	if (has_canfour(color,count,canfour))
	{
		merge(tempresult,counter,canfour,count);
	}
*/

/*
	//以下的代码返回所有的子，为了精确起见
	counter = 0;
	for (int i=1;i<=15;i++)
		for (int j=1;j<=15;j++)
		{
			if (ChessPoints[i][j]==0)
			{
				tempresult[counter].x = i;
				tempresult[counter].y = j;
				counter++;
			}
		}
*/

}

void CGoBangGame::getclose_small(int &counter, POINT tempresult[])
{
	//用来代替getclosest，其算法为将每个子的roundpoints都合并到一起返回
	counter = 0;
	POINT local_tempresult[8];
	for (int i=1;i<=m_xGrid;i++)
		for (int j=1;j<=m_yGrid;j++)
		{
			if ((ChessPoints[i][j]==1)||(ChessPoints[i][j]==2))
			{
				getroundpoints(i,j,ChessPoints[i][j],local_tempresult);
				merge(tempresult,counter,local_tempresult,8);				
			}			
		}	
}

void CGoBangGame::merge(POINT target[],int &tn,POINT append[],int an)
{
	//用于将append数组合并到target数组中，保证没有重复项
	//tn是target的长度，an是append的长度
	for(int i=0;i<an;i++)
	{
		if (append[i].x==0) continue;
		//不能有重复子
		BOOL flag = true;
		for (int j=0;j<tn;j++)
		{
			if ((target[j].x==append[i].x)&&(target[j].y==append[i].y))
			{
				//存在相同子
				flag = false;
				break;
			}
		}
		if (flag)
		{
			target[tn].x = append[i].x;
			target[tn].y = append[i].y;			
			tn++;
		}			
	}	
}

void CGoBangGame::merge1(POINT target[],int &tn,POINT append[],int an,int roundcolor)
{
	//用于将append数组合并到target数组中，保证没有重复项
	//tn是target的长度，an是append的长度
	for(int i=0;i<an;i++)	
	{	
		if (append[i].x==0) continue;	
		//不能有重复子
		BOOL flag = true;	
		for (int j=0;j<tn;j++)	
		{		
			if ((target[j].x==append[i].x)&&(target[j].y==append[i].y))	
			{	
				//存在相同子
				flag = false;
				break;	
			}
		}
		
		if (flag)	
		{
			if (has_roundcolor(append[i].x,append[i].y,roundcolor))
			{
				target[tn].x = append[i].x;				
				target[tn].y = append[i].y;
				tn++;
			}		
		}			
	}	
}

BOOL CGoBangGame::has_roundcolor(int x, int y, int color)
{
	//判断一个子(x,y)周围是否有color的子
	for (int i=0;i<=4;i++)
	{
		//四个方向
		if (getcolor(x,i,-1,y)==color) return true;
		if (getcolor(x,i,1,y)==color) return true;
	}
	return false;
}

BOOL CGoBangGame::has_alive_defent(int color,int &counter,POINT tempresult[])
{

	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以

	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	counter = 0;
	
	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_alive(color))
		
	{
		
		//对方存在一阶活子
		
		int count;
		
		POINT closepoints[225];
		
		//getclose(color, count, closepoints);
		has_defent_point(color,count,closepoints);
		
		//得到和对方的子最近的子

		BOOL has_qian = false;//是否已经有可以阻挡的牵制子
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//如果这个子本身就是自己的一阶牵制子，那么不用挡住对方太多的东西即可
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							continue;						
						}
						if (has_qian)
						{
							tempresult[counter].x = closepoints[i].x;
							
							tempresult[counter].y = closepoints[i].y;
							
							counter++;
						}
						else
						{
							//如果以前没有，那么清零
							tempresult[0].x = closepoints[i].x;
							
							tempresult[0].y = closepoints[i].y;

							counter = 1;
						}						
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						//ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						//return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}
				else if (!has_qian)//没有qian的时候才做
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中
						
						if (m_grade==5)
						{
							if (!has_twolevelqianmodel_qiang(color)&&!has_threelevelqianmodel_qiang(color))//&&!has_beatpoints_low(color))
							{
								tempresult[counter].x = closepoints[i].x;
								
								tempresult[counter].y = closepoints[i].y;
								
								counter++;
								
								flag = true;
								
								defent_success = true;
							}							
						}	
						else
						{
							tempresult[counter].x = closepoints[i].x;
							
							tempresult[counter].y = closepoints[i].y;
							
							counter++;
							
							flag = true;
							
							defent_success = true;
						}
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;	
}

BOOL CGoBangGame::has_twolevelqianmodel_qiang_defent(int color,int &counter,POINT tempresult[])

{
	
	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以
	
	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对
	
	//己方赋值最高的点
	
	counter = 0;
	
	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_twolevelqianmodel_qiang(color))
		
	{
		
		//对方存在一阶活子
		
		int count;
		
		POINT closepoints[225];
		
		//getclose(color, count, closepoints);
		//getclose_small(count,closepoints);
		//has_twolevelqianmodel_qiang_defent_low(color,count,closepoints);
		getclose_twolevelqianmodel_qiang(color,count,closepoints);
		
		//得到和对方的子最近的子

		BOOL has_qian = false;//是否已经有可以阻挡的牵制子
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//如果这个子本身就是自己的一阶牵制子，那么不用挡住对方太多的东西即可
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中						
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							continue;						
						}
						if (has_qian)
						{
							tempresult[counter].x = closepoints[i].x;
							
							tempresult[counter].y = closepoints[i].y;
							
							counter++;
						}
						else
						{
							//如果以前没有，那么清零
							tempresult[0].x = closepoints[i].x;
							
							tempresult[0].y = closepoints[i].y;

							counter = 1;
						}						
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						if (counter==3)
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							
							return true;
						}
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}
				else if (!has_qian)//没有qian的时候才做
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中

						if (m_grade==5)
						{
							if (!has_twolevelqianmodel_qiang(color)&&!has_threelevelqianmodel_qiang(color))//&&!has_beatpoints_low(color))
							{
								tempresult[counter].x = closepoints[i].x;
								
								tempresult[counter].y = closepoints[i].y;
								
								counter++;
								
								flag = true;
								
								defent_success = true;

								if (counter==3)
								{
									ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
									
									return true;
								}
							}							
						}	
						else
						{
							tempresult[counter].x = closepoints[i].x;
							
							tempresult[counter].y = closepoints[i].y;
							
							counter++;
							
							flag = true;
							
							defent_success = true;

							if (counter==3)
							{
								ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
								
								return true;
							}
						}
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;
	
}

BOOL CGoBangGame::has_threelevelqianmodel_qiang_defent(int color,int &counter,POINT tempresult[])

{
	
	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以
	
	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对
	
	//己方赋值最高的点
	
	counter = 0;
	
	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_threelevelqianmodel_qiang(color))
		
	{
		
		//对方存在一阶活子
		
		int count;
		
		POINT closepoints[225];
		
		//getclose(color, count, closepoints);
		getclose_small(count,closepoints);
		//has_threelevelqianmodel_qiang_defent_low(color,count,closepoints);
		
		//得到和对方的子最近的子

		BOOL has_qian = false;//是否已经有可以阻挡的牵制子
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//如果这个子本身就是自己的一阶牵制子，那么不用挡住对方太多的东西即可
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中						
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							continue;						
						}
						if (has_qian)
						{
							tempresult[counter].x = closepoints[i].x;
							
							tempresult[counter].y = closepoints[i].y;
							
							counter++;
						}
						else
						{
							//如果以前没有，那么清零
							tempresult[0].x = closepoints[i].x;
							
							tempresult[0].y = closepoints[i].y;

							counter = 1;
						}						
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						if (counter==3)
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							
							return true;
						}
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}
				else if (!has_qian)//没有qian的时候才做
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中

						if (m_grade==5)
						{
							if (!has_twolevelqianmodel_qiang(color)&&!has_threelevelqianmodel_qiang(color))//&&!has_beatpoints_low(color))
							{
								tempresult[counter].x = closepoints[i].x;
								
								tempresult[counter].y = closepoints[i].y;
								
								counter++;
								
								flag = true;
								
								defent_success = true;

								if (counter==3)
								{
									ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
									
									return true;
								}
							}							
						}	
						else
						{
							tempresult[counter].x = closepoints[i].x;
							
							tempresult[counter].y = closepoints[i].y;
							
							counter++;
							
							flag = true;
							
							defent_success = true;

							if (counter==3)
							{
								ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
								
								return true;
							}
						}
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;	
	
}

BOOL CGoBangGame::has_twolevelqianmodel_qiang_defent_low(int color,int &counter,POINT tempresult[])

{
	
	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以
	
	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对
	
	//己方赋值最高的点
	
	counter = 0;
	
	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_twolevelqianmodel_qiang(color))
		
	{
		
		//对方存在一阶活子
		
		int count;
		
		POINT closepoints[225];
		
		//getclose(color, count, closepoints);
		getclose_small(count,closepoints);
		//getclose_twolevelqianmodel_qiang(color,count,closepoints);
		
		//得到和对方的子最近的子
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				
				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
				
				//假走一步
				
				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_twolevelqianmodel_qiang(color)) 
					
				{
					
					//如果这时对方的活子消失，那么存入tempresult中
					
					tempresult[counter].x = closepoints[i].x;
					
					tempresult[counter].y = closepoints[i].y;
					
					counter++;
					
					flag = true;
					
					defent_success = true;
					/*

					if (counter==3)
					{
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						return true;
					}*/
					
				}
				
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;
	
}

BOOL CGoBangGame::has_threelevelqianmodel_qiang_defent_low(int color,int &counter,POINT tempresult[])

{
	
	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以
	
	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对
	
	//己方赋值最高的点
	
	counter = 0;
	
	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_threelevelqianmodel_qiang(color))
		
	{
		
		//对方存在一阶活子
		
		int count;
		
		POINT closepoints[225];
		
		//getclose(color, count, closepoints);
		getclose_small(count,closepoints);
		
		//得到和对方的子最近的子
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				
				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
				
				//假走一步
				
				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_threelevelqianmodel_qiang(color)) 
					
				{
					
					//如果这时对方的活子消失，那么存入tempresult中
					
					tempresult[counter].x = closepoints[i].x;
					
					tempresult[counter].y = closepoints[i].y;
					
					counter++;
					
					flag = true;
					
					defent_success = true;
					/*

					if (counter==3)
					{
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						return true;
					}*/
					
				}
				
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;
	
}






BOOL CGoBangGame::has_alive_defent(int color)

{

	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以

	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_alive(color))
		
	{
		
		//对方存在一阶活子
		
		int count;
		
		POINT closepoints[225];
		
		//getclose(color, count, closepoints);
		has_defent_point(color,count,closepoints);
		
		//得到和对方的子最近的子

		BOOL has_qian = false;//是否已经有可以阻挡的牵制子
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//如果这个子本身就是自己的一阶牵制子，那么不用挡住对方太多的东西即可
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中		
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}
				else if (!has_qian)//没有qian的时候才做
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中
						flag = true;
						
						defent_success = true;
						
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						return true;
						
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;	

}



BOOL CGoBangGame::has_alive_defent_low(int color,int &counter,POINT tempresult[])

{

	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以

	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	counter = 0;

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;

	defent_success = false;

	if (has_alive(color))

	{

		//对方存在一阶活子

		int count;

		POINT closepoints[225];

		//getclose(color, count, closepoints);
		has_defent_point(color,count,closepoints);

		//得到和对方的子最近的子

		for(int i=0;i<count;i++)

		{

			if (closepoints[i].x!=0)

			{

				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;

				//假走一步

				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color))

				{

					//如果这时对方的活子消失，那么存入tempresult中

					tempresult[counter].x = closepoints[i].x;

					tempresult[counter].y = closepoints[i].y;

					counter++;

					flag = true;

					defent_success = true;

				}

				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

			}

		}

	}

	else defent_success = true;

	return flag;

}



BOOL CGoBangGame::has_alive_defent_low(int color)

{

	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以

	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;

	defent_success = false;

	if (has_alive(color))

	{

		//对方存在一阶活子

		int count;

		POINT closepoints[225];

		//getclose(color, count, closepoints);
		has_defent_point(color,count,closepoints);

		//得到和对方的子最近的子

		for(int i=0;i<count;i++)

		{

			if (closepoints[i].x!=0)

			{

				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;

				//假走一步

				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color))

				{

					//如果这时对方的活子消失，那么存入tempresult中

					flag = true;

					defent_success = true;

					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

					return true;

				}

				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

			}

		}

	}

	else defent_success = true;

	return flag;

}



BOOL CGoBangGame::has_twolevel_defent(int color, int &counter, POINT tempresult[])

{

	//此函数用于阻挡对方出现二阶活子，由于阻止的方式不一定是下在对方的二阶活子上，所以

	//此算法就是找出所有能使对方二阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	counter = 0;
	
	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_twolevel(color))
		
	{
		
		//对方存在一阶活子
		
		int count;
		
		POINT closepoints[225];
		
		getclose_twolevel(color, count, closepoints);

		//得到和对方的子最近的子

		BOOL has_qian = false;//是否已经有可以阻挡的牵制子
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//如果这个子本身就是自己的一阶牵制子，那么不用挡住对方太多的东西即可
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							continue;						
						}
						if (has_qian)
						{
							tempresult[counter].x = closepoints[i].x;
							
							tempresult[counter].y = closepoints[i].y;
							
							counter++;
						}
						else
						{
							//如果以前没有，那么清零
							tempresult[0].x = closepoints[i].x;
							
							tempresult[0].y = closepoints[i].y;

							counter = 1;
						}						
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						//ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						//return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}
				else if (!has_qian)//没有qian的时候才做
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中

						if (m_grade==5)
						{
							if (!has_twolevelqianmodel_qiang(color)&&!has_threelevelqianmodel_qiang(color))//&&!has_beatpoints_low(color))
							{
								tempresult[counter].x = closepoints[i].x;
								
								tempresult[counter].y = closepoints[i].y;
								
								counter++;
								
								flag = true;
								
								defent_success = true;
							}							
						}	
						else
						{
							tempresult[counter].x = closepoints[i].x;
							
							tempresult[counter].y = closepoints[i].y;
							
							counter++;
							
							flag = true;
							
							defent_success = true;
						}
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;

}



BOOL CGoBangGame::has_twolevel_defent(int color)

{

	//此函数用于阻挡对方出现二阶活子，由于阻止的方式不一定是下在对方的二阶活子上，所以

	//此算法就是找出所有能使对方二阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_twolevel(color))
		
	{
		
		//对方存在一阶活子
		
		int count;
		
		POINT closepoints[225];
		
		getclose_twolevel(color, count, closepoints);
		
		//得到和对方的子最近的子

		BOOL has_qian = false;//是否已经有可以阻挡的牵制子
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//如果这个子本身就是自己的一阶牵制子，那么不用挡住对方太多的东西即可
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中				
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}
				else if (!has_qian)//没有qian的时候才做
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中
						
						flag = true;
						
						defent_success = true;

						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						return true;						
						
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;

}



BOOL CGoBangGame::has_twolevel_defent_low(int color, int &counter, POINT tempresult[])

{

	//此函数用于阻挡对方出现二阶活子，由于阻止的方式不一定是下在对方的二阶活子上，所以

	//此算法就是找出所有能使对方二阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	counter = 0;

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;

	defent_success = false;

	if (has_twolevel(color))

	{

		//对方存在二阶活子

		int count;

		POINT closepoints[225];

		getclose_twolevel(color, count, closepoints);

		//得到和对方的子最近的子

		for(int i=0;i<count;i++)

		{

			if (closepoints[i].x!=0)

			{

				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;

				//假走一步

				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color))

				{

					//如果这时对方的活子消失，那么存入tempresult中

					tempresult[counter].x = closepoints[i].x;

					tempresult[counter].y = closepoints[i].y;

					counter++;

					flag = true;

					defent_success = true;

				}

				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

			}

		}

	}

	else defent_success = true;

	return flag;

}



BOOL CGoBangGame::has_twolevel_defent_low(int color)

{

	//此函数用于阻挡对方出现二阶活子，由于阻止的方式不一定是下在对方的二阶活子上，所以

	//此算法就是找出所有能使对方二阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;

	defent_success = false;

	if (has_twolevel(color))

	{

		//对方存在二阶活子

		int count;

		POINT closepoints[225];

		getclose_twolevel(color, count, closepoints);

		//得到和对方的子最近的子

		for(int i=0;i<count;i++)

		{

			if (closepoints[i].x!=0)

			{

				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;

				//假走一步

				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color))

				{

					//如果这时对方的活子消失，那么存入tempresult中

					flag = true;

					defent_success = true;

					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

					return true;

				}

				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

			}

		}

	}

	else defent_success = true;

	return flag;

}



BOOL CGoBangGame::has_twolevel_four_defent(int color, int &counter, POINT tempresult[])

{

	//此函数用于阻挡对方出现强二阶活子，由于阻止的方式不一定是下在对方的二阶活子上，所以

	//此算法就是找出所有能使对方二阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	counter = 0;
	
	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_twolevel_four(color))
		
	{
		
		//对方存在一阶活子
		
		int count;
		
		POINT closepoints[225];
		
		getclose_twolevel_four(color, count, closepoints);
		
		//得到和对方的子最近的子

		BOOL has_qian = false;//是否已经有可以阻挡的牵制子
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//如果这个子本身就是自己的一阶牵制子，那么不用挡住对方太多的东西即可
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							continue;						
						}
						if (has_qian)
						{
							tempresult[counter].x = closepoints[i].x;
							
							tempresult[counter].y = closepoints[i].y;
							
							counter++;
						}
						else
						{
							//如果以前没有，那么清零
							tempresult[0].x = closepoints[i].x;
							
							tempresult[0].y = closepoints[i].y;

							counter = 1;
						}						
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						//ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						//return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}
				else if (!has_qian)//没有qian的时候才做
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中
						
						if (m_grade==5)
						{
							if (!has_twolevelqianmodel_qiang(color)&&!has_threelevelqianmodel_qiang(color))//&&!has_beatpoints_low(color))
							{
								tempresult[counter].x = closepoints[i].x;
								
								tempresult[counter].y = closepoints[i].y;
								
								counter++;
								
								flag = true;
								
								defent_success = true;
							}							
						}	
						else
						{
							tempresult[counter].x = closepoints[i].x;
							
							tempresult[counter].y = closepoints[i].y;
							
							counter++;
							
							flag = true;
							
							defent_success = true;
						}
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;	

}



BOOL CGoBangGame::has_twolevel_four_defent(int color)

{

	//此函数用于阻挡对方出现强二阶活子，由于阻止的方式不一定是下在对方的二阶活子上，所以

	//此算法就是找出所有能使对方二阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_twolevel_four(color))
		
	{
		
		//对方存在一阶活子
		
		int count;
		
		POINT closepoints[225];
		
		getclose_twolevel_four(color, count, closepoints);
		
		//得到和对方的子最近的子

		BOOL has_qian = false;//是否已经有可以阻挡的牵制子
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//如果这个子本身就是自己的一阶牵制子，那么不用挡住对方太多的东西即可
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中										
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}
				else if (!has_qian)//没有qian的时候才做
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中
						flag = true;
						
						defent_success = true;
						
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;
}



BOOL CGoBangGame::has_twolevel_four_defent_low(int color, int &counter, POINT tempresult[])

{

	//此函数用于阻挡对方出现强二阶活子，由于阻止的方式不一定是下在对方的二阶活子上，所以

	//此算法就是找出所有能使对方二阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	counter = 0;

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;

	defent_success = false;

	if (has_twolevel_four(color))

	{

		//对方存在二阶活子

		int count;

		POINT closepoints[225];

		getclose_twolevel_four(color, count, closepoints);

		//得到和对方的子最近的子

		for(int i=0;i<count;i++)

		{

			if (closepoints[i].x!=0)

			{

				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;

				//假走一步

				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color))

				{

					//如果这时对方的活子消失，那么存入tempresult中

					tempresult[counter].x = closepoints[i].x;

					tempresult[counter].y = closepoints[i].y;

					counter++;

					flag = true;

					defent_success = true;

				}

				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

			}

		}

	}

	else defent_success = true;

	return flag;

}



BOOL CGoBangGame::has_twolevel_four_defent_low(int color)

{

	//此函数用于阻挡对方出现强二阶活子，由于阻止的方式不一定是下在对方的二阶活子上，所以

	//此算法就是找出所有能使对方二阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;

	defent_success = false;

	if (has_twolevel_four(color))

	{

		//对方存在二阶活子

		int count;

		POINT closepoints[225];

		getclose_twolevel_four(color, count, closepoints);

		//得到和对方的子最近的子

		for(int i=0;i<count;i++)

		{

			if (closepoints[i].x!=0)

			{

				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;

				//假走一步

				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color))

				{

					//如果这时对方的活子消失，那么存入tempresult中

					flag = true;

					defent_success = true;

					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

					return true;

				}

				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

			}

		}

	}

	else defent_success = true;

	return flag;

}



BOOL CGoBangGame::has_qianmodel_addone_defent(int color,int &counter,POINT tempresult[],BOOL qiang)

{

	//牵制子匹配!!

	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以

	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对

	//己方赋值最高的点

	counter = 0;
	
	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	
	if (has_qianmodel_addone(color,qiang))
		
	{
		//对方存在一阶活子
		
		int count;
		
		POINT closepoints[225];
		
		//getclose_small(count, closepoints);
		getclose_qianmodeladdone(color,count,closepoints,qiang);
		//has_qianmodel_addone_defent_low(color,count,closepoints,qiang);
		
		//得到和对方的子最近的子

		BOOL has_qian = false;//是否已经有可以阻挡的牵制子
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//如果这个子本身就是自己的一阶牵制子，那么不用挡住对方太多的东西即可
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)&&!has_qianmodel_addone(color,qiang)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							continue;						
						}
						if (has_qian)
						{
							tempresult[counter].x = closepoints[i].x;
							
							tempresult[counter].y = closepoints[i].y;
							
							counter++;
						}
						else
						{
							//如果以前没有，那么清零
							tempresult[0].x = closepoints[i].x;
							
							tempresult[0].y = closepoints[i].y;

							counter = 1;
						}						
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						if (counter==3)
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							
							return true;
						}					
						
					}
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值

				}
				else if (!has_qian)//没有qian的时候才做
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//如果这时对方的活子消失，那么存入tempresult中						

						if (m_grade==5)
						{
							if (!has_twolevelqianmodel_qiang(color)&&!has_threelevelqianmodel_qiang(color))//&&!has_beatpoints_low(color))
							{
								tempresult[counter].x = closepoints[i].x;
								
								tempresult[counter].y = closepoints[i].y;
								
								counter++;
								
								flag = true;
								
								defent_success = true;

								if (counter==3)
								{
									ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
									
									return true;
								}			
							}							
						}	
						else
						{
							tempresult[counter].x = closepoints[i].x;
							tempresult[counter].y = closepoints[i].y;
							counter++;
							flag = true;
							defent_success = true;
							if (counter==3)
							{
								ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
								
								return true;
							}			
						}

					}	
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
				}				
			}
		}	
	}
	
	else defent_success = true;
	
	return flag;
}

BOOL CGoBangGame::has_qianmodel_addone_defent_low(int color,int &counter,POINT tempresult[],BOOL qiang)
{
	
	//牵制子匹配!!
	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以
	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中
	//己方赋值最高的点
	counter = 0;
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	BOOL flag = false;
	
	defent_success = false;
	if (has_qianmodel_addone(color,qiang))	
	{
		//对方存在一阶活子
		int count;
		POINT closepoints[225];
		
		//getclose_small(count, closepoints);
		getclose_qianmodeladdone(color,count,closepoints,qiang);
		
		//得到和对方的子最近的子
		for(int i=0;i<count;i++)	
		{
			if (closepoints[i].x!=0)	
			{
				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
				//假走一步
				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)) 	
				{
					
					//如果这时对方的活子消失，那么存入tempresult中
					
					tempresult[counter].x = closepoints[i].x;
					tempresult[counter].y = closepoints[i].y;	
					counter++;
					flag = true;
					defent_success = true;

					/*
					if (counter==3)
					{
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						return true;
					}			
					*/
				}
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
			}
		}
	}
	else defent_success = true;
	return flag;
	
}

BOOL CGoBangGame::has_qianmodel_defent(int color,int &counter,POINT tempresult[],BOOL qiang)
{
	//掺杂牵制子匹配!!
	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以
	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对
	//己方赋值最高的点

	counter = 0;
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	BOOL flag = false;
	defent_success = false;
	
	if (has_qianmodel(color,qiang))
	{
		//对方存在一阶活子
		int count;
		POINT closepoints[225];
		
		//getclose_small(count, closepoints);
		getclose_qianmodel(color,count,closepoints,qiang);
		//has_qianmodel_defent_low(color,count,closepoints,qiang);
		
		//得到和对方的子最近的子

		BOOL has_qian = false;//是否已经有可以阻挡的牵制子
		
		for(int i=0;i<count;i++)
		{
			if (closepoints[i].x!=0)
			{	
				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//如果这个子本身就是自己的一阶牵制子，那么不用挡住对方太多的东西即可
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)&&!has_qianmodel(color,qiang)) 
					{
						
						//如果这时对方的活子消失，那么存入tempresult中
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							continue;						
						}
						if (has_qian)
						{
							tempresult[counter].x = closepoints[i].x;
							tempresult[counter].y = closepoints[i].y;
							counter++;
						}
						else
						{
							//如果以前没有，那么清零
							tempresult[0].x = closepoints[i].x;
							tempresult[0].y = closepoints[i].y;
							counter = 1;
						}						
						
						has_qian = true;
						flag = true;
						defent_success = true;
						if (counter==3)
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							
							return true;
						}				
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
				}
				else if (!has_qian)//没有qian的时候才做
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//假走一步
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 	
					{
						//如果这时对方的活子消失，那么存入tempresult中
						if (m_grade==5)
						{
							if (!has_twolevelqianmodel_qiang(color)&&!has_threelevelqianmodel_qiang(color))//&&!has_beatpoints_low(color))
							{
								tempresult[counter].x = closepoints[i].x;
								tempresult[counter].y = closepoints[i].y;
								counter++;
								flag = true;
								
								defent_success = true;

								if (counter==3)
								{
									ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
									return true;
								}			
							}							
						}	
						else
						{
							tempresult[counter].x = closepoints[i].x;
							tempresult[counter].y = closepoints[i].y;
							counter++;	
							flag = true;
							defent_success = true;

							if (counter==3)
							{
								ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
								return true;
							}			
						}
					}
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
				}				
			}	
		}
	}
	
	else defent_success = true;
	
	return flag;
}

BOOL CGoBangGame::has_qianmodel_defent_low(int color,int &counter,POINT tempresult[],BOOL qiang)
{
	
	//掺杂牵制子匹配!!
	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以
	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对	
	//己方赋值最高的点
	counter = 0;
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;	
	BOOL flag = false;
	defent_success = false;
	
	if (has_qianmodel(color,qiang))	
	{
		//对方存在一阶活子
		int count;
		POINT closepoints[225];
		//getclose_small(count, closepoints);
		getclose_qianmodel(color,count,closepoints,qiang);
		//得到和对方的子最近的子
		
		for(int i=0;i<count;i++)	
		{	
			if (closepoints[i].x!=0)		
			{	
				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;	
				//假走一步
				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)) 	
				{
					//如果这时对方的活子消失，那么存入tempresult中
					tempresult[counter].x = closepoints[i].x;
					tempresult[counter].y = closepoints[i].y;
					counter++;
					flag = true;
					
					defent_success = true;
					/*
					if (counter==3)
					{
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						return true;
					}*/			
					
				}
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值	
			}	
		}
	}
	else defent_success = true;
	
	return flag;
	
}

BOOL CGoBangGame::has_twolevel_qianmodel_defent_low(int color,int &counter,POINT tempresult[])
{
	//二阶牵制子匹配!!
	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以
	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对
	//己方赋值最高的点
	
	counter = 0;
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	BOOL flag = false;
	defent_success = false;
	if (has_twolevel_qianmodel(color))	
	{	
		//对方存在一阶活子
		int count;
		POINT closepoints[225];
		//getclose_small(count, closepoints);
		getclose_twolevelqianmodel(color,count,closepoints);
		
		//得到和对方的子最近的子
		for(int i=0;i<count;i++)	
		{
			if (closepoints[i].x!=0)	
			{
				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
				//假走一步
				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_twolevel_qianmodel(color)) 			
				{		
					//如果这时对方的活子消失，那么存入tempresult中				
					tempresult[counter].x = closepoints[i].x;					
					tempresult[counter].y = closepoints[i].y;
					counter++;
					flag = true;
					defent_success = true;
					/*
					if (counter==3)
					{
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
						
						return true;
					}*/				
				}
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值	
			}	
		}
	}
	else defent_success = true;
	return flag;
	
}

BOOL CGoBangGame::has_twolevel_qianmodel_defent(int color,int &counter,POINT tempresult[])
{
	//二阶牵制子匹配!!
	//此函数用于阻挡对方出现一阶活子，由于阻止的方式不一定是下在对方的一阶活子上，所以
	//此算法就是找出所有能使对方一阶活子消失的子，最终通过select_max函数选择这些子中对
	//己方赋值最高的点

	counter = 0;
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	BOOL flag = false;
	defent_success = false;
	
	if (has_twolevel_qianmodel(color))	
	{
		//对方存在一阶活子
		int count;
		POINT closepoints[225];
		//getclose_small(count, closepoints);
		getclose_twolevelqianmodel(color,count,closepoints);
		//has_qianmodel_addone_defent_low(color,count,closepoints,qiang);
		
		//得到和对方的子最近的子
		BOOL has_qian = false;//是否已经有可以阻挡的牵制子
		for(int i=0;i<count;i++)
		{
			if (closepoints[i].x!=0)
			{
				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//如果这个子本身就是自己的一阶牵制子，那么不用挡住对方太多的东西即可
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					//假走一步
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)&&!has_twolevel_qianmodel(color)) 
					{
						//如果这时对方的活子消失，那么存入tempresult中
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							continue;						
						}
						if (has_qian)
						{
							tempresult[counter].x = closepoints[i].x;			
							tempresult[counter].y = closepoints[i].y;
							counter++;
						}
						else
						{
							//如果以前没有，那么清零
							tempresult[0].x = closepoints[i].x;			
							tempresult[0].y = closepoints[i].y;
							counter = 1;
						}						
						has_qian = true;
						flag = true;
						defent_success = true;
						if (counter==3)
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
							return true;
						}						
					}
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
				}
				else if (!has_qian)//没有qian的时候才做
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					//假走一步		
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 				
					{				
						//如果这时对方的活子消失，那么存入tempresult中						
						if (m_grade==5)
						{
							if (!has_twolevelqianmodel_qiang(color)&&!has_threelevelqianmodel_qiang(color))//&&!has_beatpoints_low(color))
							{
								tempresult[counter].x = closepoints[i].x;								
								tempresult[counter].y = closepoints[i].y;								
								counter++;
								
								flag = true;								
								defent_success = true;
								if (counter==3)
								{
									ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值									
									return true;
								}			
							}							
						}	
						else
						{
							tempresult[counter].x = closepoints[i].x;							
							tempresult[counter].y = closepoints[i].y;
							counter++;
							flag = true;							
							defent_success = true;
							if (counter==3)
							{
								ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
								
								return true;
							}			
						}
					}	
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
				}					
			}	
		}	
	}
	else defent_success = true;
	return flag;
}

BOOL CGoBangGame::is_canfour(int color, int x, int y)
{
	if (ChessPoints[x][y]!=0) return false;//已经有子了
	int model[][9]={//此数组表示各种模式，包括此子前四个和后四个子的排列
		//-1表示任意，1表示相同颜色，0表示空，2表示(x,y)所在位置
		{-1,-1,0,1,2,1,1,-1,-1},//_1*11
		{-1,-1,-1,1,2,1,1,0,-1},//1*11_
		{-1,-1,1,0,2,1,1,-1,-1},//1_*11
		{-1,-1,-1,1,2,0,1,1,-1},//1*_11
		{-1,-1,-1,1,2,1,0,1,-1},//1*1_1
		{-1,-1,-1,0,2,1,1,1,-1},//_*111
		{-1,-1,-1,-1,2,1,1,1,0},//*111_
		{-1,-1,-1,-1,2,0,1,1,1},//*_111
		{-1,-1,-1,-1,2,1,0,1,1},//*1_11
		{-1,-1,-1,-1,2,1,1,0,1},//*11_1
	};

	int model_num = 10;
	int point_num = 9;
	BOOL flag;
	for (int i=0;i<4;i++)
	{
		//四个方向遍历		
		for (int j=0;j<model_num;j++)//model_num种模式遍历，每种模式遍历两次，正反各一次
		{
			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}

			if (flag) return true;//匹配上某种模式
			flag = true;
			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}
			if (flag) return true;//匹配上某种模式
		}
	}
	return false;	
}

void CGoBangGame::ShowInfo(CString s,BOOL debug)
{
	//用于显示目前的决策
	//AfxGetApp()->OnIdle( 1 );
	Sleep(2);
	if (((debug)&&(!notshowinfo))||(timeout))
	{
		//CString temp;
		//m_outinfo->GetWindowText(temp);
		//m_outinfo->SetWindowText(temp+s+L"\r\n");
	}	
}

BOOL CGoBangGame::is_five(int color, int x, int y)
{
	//看此点是否可以连成5子
	if (ChessPoints[x][y]!=0) return false;//已经有子了

	int model[][9]={//此数组表示各种模式，包括此子前四个和后四个子的排列
		//-1表示任意，1表示相同颜色，0表示空，2表示(x,y)所在位置
		{-1,-1,1,1,2,1,1,-1,-1},//11*11
		{-1,-1,-1,1,2,1,1,1,-1},//1*111
		{-1,-1,-1,-1,2,1,1,1,1},//*1111		
	};

	int model_num = 3;
	int point_num = 9;
	BOOL flag;
	for (int i=0;i<4;i++)
	{
		//四个方向遍历		
		for (int j=0;j<model_num;j++)//model_num种模式遍历，每种模式遍历两次，正反各一次
		{
			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
					//不匹配此模式
						flag = false;
						break;
					}
				}				
			}

			if (flag) return true;//匹配上某种模式
			flag = true;
			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历
			{

				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}
			if (flag) return true;//匹配上某种模式
		}
	}
	return false;	
}

BOOL CGoBangGame::has_qianmodel(int color, int &counter, POINT tempresult[],BOOL qiang)
{
	//用来将牵制子连成某种model，增强攻击力度
	counter = 0;
	int count;
	POINT local_tempresult[225];
	BOOL flag = false;
	if (has_qian(color,count,local_tempresult))
	{
		for (int i=0;i<count;i++)
		{
			if (can_qianmodel(color,local_tempresult[i].x,local_tempresult[i].y,local_tempresult,count,qiang))
			{							
				tempresult[counter].x = local_tempresult[i].x;
				tempresult[counter].y = local_tempresult[i].y;
				counter++;
				flag = true;
			}
		}		
	}
	return flag;
}


BOOL CGoBangGame::has_qianmodel(int color,BOOL qiang)
{
	//用来将牵制子连成某种model，增强攻击力度
	int count;
	POINT local_tempresult[225];

	if (has_qian(color,count,local_tempresult))
	{
		for (int i=0;i<count;i++)
		{
			if (can_qianmodel(color,local_tempresult[i].x,local_tempresult[i].y,local_tempresult,count,qiang))
			{
				return true;
			}
		}	
	}
	return false;
}

int CGoBangGame::getcolor(int color,int x, int y, int i, int move, POINT tempresult[],int n)
{
	//用于得到某一结果集上某一点的颜色，主要为了减少判断index合法性的代码
	//move为移动的位移，i为方向取值0，1，2，3
	//n表示tempresult中元素的多少
	//符合条件的返回值为1，不符合条件的返回值根据棋盘上的子来判断，如果超过边界返回-1

	POINT d[4];
	d[0].x = 1;d[0].y = 0;//横方向
	d[1].x = 0;d[1].y = 1;//纵方向
	d[2].x = 1;d[2].y = -1;//斜右上
	d[3].x = 1;d[3].y = 1;//斜左上
	x = x+move*d[i].x;
	y = y+move*d[i].y;

	if ((x>=1)&&(x<=m_xGrid)&&(y>=1)&&(y<=m_yGrid))
	{
		for(int i=0;i<n;i++)
		{
			if ((tempresult[i].x==x)&&(tempresult[i].y==y))
			{
				//如果tempresult中存在(x,y)点，那么返回1
				return 1;
			}
		}

		if (ChessPoints[x][y]==0)//不存在(x,y)点
		{//是棋盘上的空子
			return 0;
		}
		else
		{//不是棋盘上的空子
			if (ChessPoints[x][y]==color) return 3;//此格颜色相同
			return 4;//颜色不同，相当于阻挡色
		}
	}
	return -1;//边界，返回-1
}

BOOL CGoBangGame::can_qianmodel(int color, int x, int y,POINT tempresult[],int n,BOOL qiang)

{
	if (qiang&&!is_canfour(color,x,y)) return false;

	//用于判断一个子是否在牵制子中形成了model

	int model[][11]={//此数组表示各种模式，包括此子前四个和后四个子的排列

		//-1表示任意，1表示在tempresult中的点，0表示空，2表示(x,y)所在位置

		//3表示颜色相同的不在tempresult中的点

		//4表示颜色不同的不在tempresult中的点

		//6表示既不是1也不是3

		//记住:每种模式结尾或者开头只能用_,3,6，不能是1,如果是1，应该用6挡住

		//这时的*相当于1

		{-1,0,0,1,1,2,0,-1,-1,-1,-1},//__11*_ 0

		{-1,-1,0,1,1,2,0,0,-1,-1,-1},//_11*__ 1

		{-1,0,1,0,1,2,0,-1,-1,-1,-1},//_1_1*_ 2

		{-1,0,1,1,0,2,0,-1,-1,-1,-1},//_11_*_ 3

		{-1,-1,0,1,0,2,1,0,-1,-1,-1},//_1_*1_ 4

		{-1,-1,0,0,1,2,1,0,-1,-1,-1},//__1*1_ 5

		{-1,-1,0,1,1,2,1,0,-1,-1,-1},//_11*1_ 6

		{-1,-1,-1,-1,0,2,1,1,1,0,-1},//_*111_ 7

		{-1,6,1,0,1,2,1,0,-1,-1,-1},//61_1*1_ 8

		{-1,-1,-1,-1,6,2,0,1,1,1,0},//6*_111_ 9

		{-1,-1,6,1,0,2,1,1,0,-1,-1},//61_*11_ 10

		{6,1,0,1,1,2,0,-1,-1,-1,-1},//61_11*_ 11

		{-1,-1,-1,-1,6,2,1,1,0,1,0},//6*11_1_ 12

		{-1,-1,-1,6,1,2,1,0,1,0,-1},//61*1_1_ 13

		{-1,-1,6,1,1,2,0,1,0,-1,-1},//611*_1_ 14

		{6,1,1,1,0,2,0,-1,-1,-1,-1},//6111_*_ 15

		{-1,6,1,1,0,2,1,6,-1,-1,-1},//611_*16 16

		{6,1,1,0,1,2,6,-1,-1,-1,-1},//611_1*6 17

		{-1,-1,0,1,1,2,3,6,-1,-1,-1},//_11*36 18

		{-1,-1,6,1,1,2,3,0,-1,-1,-1},//611*3_ 19

		{-1,6,1,0,1,2,3,6,-1,-1,-1},//61_1*36 20

		{-1,6,1,1,0,2,3,6,-1,-1,-1},//611_*36 21

		{-1,-1,6,1,1,2,0,3,-1,-1,-1},//611*_3 22

		{-1,-1,6,1,3,2,1,0,-1,-1,-1},//613*1_ 23

		{-1,-1,0,1,3,2,1,6,-1,-1,-1},//_13*16 24

		{-1,6,1,0,3,2,1,6,-1,-1,-1},//61_3*16 25

		{-1,6,1,3,0,2,1,6,-1,-1,-1},//613_*16 26

		{-1,-1,6,1,3,2,0,1,6,-1,-1},//613*_16 27

		{-1,-1,0,3,1,2,1,6,-1,-1,-1},//_31*16 28

		{-1,-1,3,0,1,2,1,0,-1,-1,-1},//3_1*16 29

		{-1,6,3,1,0,2,1,6,-1,-1,-1},//631_*16 30

		{-1,-1,6,3,1,2,0,1,6,-1,-1},//631*_16 31

		{-1,-1,6,3,1,2,1,0,-1,-1,-1},//631*1_ 32

		{-1,0,3,1,1,2,6,-1,-1,-1,-1},//_311*6 33

		{-1,3,0,1,1,2,6,-1,-1,-1,-1},//3_11*6 34

		{6,3,1,0,1,2,6,-1,-1,-1,-1},//631_1*6 35

		{-1,6,3,1,1,2,0,-1,-1,-1,-1},//6311*_ 36

		{-1,0,1,3,1,2,6,-1,-1,-1,-1},//_131*6 37

		{6,1,0,3,1,2,6,-1,-1,-1,-1},//61_31*6 38

		{6,1,3,0,1,2,6,-1,-1,-1,-1},//613_1*6 39

		{6,1,3,1,0,2,6,-1,-1,-1,-1},//6131_*6 40

		{-1,6,1,3,1,2,0,-1,-1,-1,-1},//6131*_ 41

		{-1,0,1,1,3,2,6,-1,-1,-1,-1},//_113*6 42

		{6,1,0,1,3,2,6,-1,-1,-1,-1},//61_13*6 43

		{0,1,1,0,3,2,6,-1,-1,-1,-1},//_11_3*6 44

		{6,1,1,3,0,2,6,-1,-1,-1,-1},//6113_*6 45

		{-1,6,1,1,3,2,0,-1,-1,-1,-1},//6113*_ 46

	};

	POINT d[4];
	
	d[0].x = 1;d[0].y = 0;//横方向
	
	d[1].x = 0;d[1].y = 1;//纵方向
	
	d[2].x = 1;d[2].y = -1;//斜右上
	
	d[3].x = 1;d[3].y = 1;//斜左上

	int model_num = 47;

	int point_num = 11;

	BOOL flag;
	int i;
	int k;
	for (i=0;i<4;i++)

	{
		//四个方向遍历		
		for (int j=0;j<model_num;j++)//model_num种模式遍历，每种模式遍历两次，正反各一次
		{
			flag = true;
			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(color,x,y,i,k-5,tempresult,n)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(color,x,y,i,k-5,tempresult,n)!=1)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				

				else if (model[j][k]==3)
				{
					if (getcolor(color,x,y,i,k-5,tempresult,n)!=3)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}		

				else if (model[j][k]==4)
				{
					if (getcolor(color,x,y,i,k-5,tempresult,n)!=4)
					{

						//不匹配此模式
						flag = false;
						break;
					}
				}		
				else if (model[j][k]==6)
				{

					if (getcolor(color,x,y,i,k-5,tempresult,n)==3)
					{
						//不匹配此模式
						flag = false;
						break;
					}

					if (getcolor(color,x,y,i,k-5,tempresult,n)==1)
					{

						//不匹配此模式
						flag = false;
						break;
					}
				}	
			}

			if (flag) 
			{
				if (debugqian)
				{
					CString s;
					s.Format(L"front:%d,(%d,%d)",j,x,y);
					AfxMessageBox(s);
				}

				if (!qiang)
				{
					return true;//匹配上某种模式
				}
				else
				{
					//判断是否是强掺杂牵制子匹配
					BOOL tempflag = true;
					int count = 0;
					int k;
					for ( k=0;k<point_num;k++)						
					{						
						if ((model[j][k]==1)&&!is_canfour(color,x+(k-5)*d[i].x,y+(k-5)*d[i].y))							
						{
							//找到一个不是canfour的子，那么就不是强掺杂牵制子匹配					
							if (j<6)
							{
								tempflag = false;
								break;
							}
							else
							{
								//可以有一个不是canfour
								if (count>=1)
								{
									tempflag = false;
									break;
								}
								else count++;
							}
						}						
					}
					if (tempflag)
					{
						//是强掺杂牵制子
						return true;//匹配上某种模式						
					}
				}

			}

			flag = true;

			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(color,x,y,i,5-k,tempresult,n)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(color,x,y,i,5-k,tempresult,n)!=1)
					{

						//不匹配此模式
						flag = false;
						break;
					}
				}				

				else if (model[j][k]==3)
				{
					if (getcolor(color,x,y,i,5-k,tempresult,n)!=3)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}		

				else if (model[j][k]==4)
				{
					if (getcolor(color,x,y,i,5-k,tempresult,n)!=4)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}	

				else if (model[j][k]==6)
				{
					if (getcolor(color,x,y,i,5-k,tempresult,n)==3)
					{
						//不匹配此模式
						flag = false;
						break;
					}

					if (getcolor(color,x,y,i,5-k,tempresult,n)==1)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}	
			}

			if (flag) 
			{
				if (debugqian)
				{
					CString s;
					s.Format(L"back:%d,(%d,%d)",j,x,y);
					AfxMessageBox(s);
				}

				if (!qiang)
				{
					return true;//匹配上某种模式
				}
				else
				{
					//判断是否是强掺杂牵制子匹配
					BOOL tempflag = true;
					int count = 0;
					for (int k=0;k<point_num;k++)						
					{						
						if ((model[j][k]==1)&&!is_canfour(color,x+(5-k)*d[i].x,y+(5-k)*d[i].y))							
						{
							//找到一个不是canfour的子，那么就不是强掺杂牵制子匹配					
							if (j<6)
							{
								tempflag = false;
								break;
							}
							else
							{
								//可以有一个不是canfour
								if (count>=1)
								{
									tempflag = false;
									break;
								}
								else count++;
							}
						}						
					}
					if (tempflag)
					{
						//是强掺杂牵制子
						return true;//匹配上某种模式						
					}
				}
			}
		}
	}
	return false;
}

BOOL CGoBangGame::can_qianmodel(int color, int x, int y,POINT tempresult[],int n,int &counter,POINT modelresult[],BOOL qiang)

{

	//qiang为true时返回强掺杂牵制子匹配

	//用于判断一个子是否在牵制子中形成了model

	//modelresult[]表示与(x,y)点匹配的子集

	counter = 0;

	int model[][11]={//此数组表示各种模式，包括此子前四个和后四个子的排列

		//-1表示任意，1表示在tempresult中的点，0表示空，2表示(x,y)所在位置

		//3表示颜色相同的不在tempresult中的点

		//4表示颜色不同的不在tempresult中的点

		//6表示既不是1也不是3

		//7表示不是3

		//记住:每种模式结尾或者开头只能用_,3,6，不能是1,如果是1，应该用6挡住

		//这时的*相当于3

		{-1,0,0,1,1,2,0,-1,-1,-1,-1},//__11*_ 0

		{-1,-1,0,1,1,2,0,0,-1,-1,-1},//_11*__ 1

		{-1,0,1,0,1,2,0,-1,-1,-1,-1},//_1_1*_ 2

		{-1,0,1,1,0,2,0,-1,-1,-1,-1},//_11_*_ 3

		{-1,-1,0,1,0,2,1,0,-1,-1,-1},//_1_*1_ 4

		{-1,-1,0,0,1,2,1,0,-1,-1,-1},//__1*1_ 5

		{-1,-1,0,1,1,2,1,0,-1,-1,-1},//_11*1_ 6

		{-1,-1,-1,-1,0,2,1,1,1,0,-1},//_*111_ 7

		{-1,6,1,0,1,2,1,0,-1,-1,-1},//61_1*1_ 8

		{-1,-1,-1,-1,6,2,0,1,1,1,0},//6*_111_ 9

		{-1,-1,6,1,0,2,1,1,0,-1,-1},//61_*11_ 10

		{6,1,0,1,1,2,0,-1,-1,-1,-1},//61_11*_ 11

		{-1,-1,-1,-1,6,2,1,1,0,1,0},//6*11_1_ 12

		{-1,-1,-1,6,1,2,1,0,1,0,-1},//61*1_1_ 13

		{-1,-1,6,1,1,2,0,1,0,-1,-1},//611*_1_ 14

		{6,1,1,1,0,2,0,-1,-1,-1,-1},//6111_*_ 15

		{-1,6,1,1,0,2,1,6,-1,-1,-1},//611_*16 16

		{6,1,1,0,1,2,6,-1,-1,-1,-1},//611_1*6 17

		{-1,-1,0,1,1,2,3,4,-1,-1,-1},//_11*34 18*

		{-1,-1,4,1,1,2,3,0,7,-1,-1},//411*3_7 19*

		{-1,6,1,0,1,2,3,6,7,-1,-1},//61_1*367 20

		{-1,6,1,1,0,2,3,6,-1,-1,-1},//611_*36 21

		{-1,-1,6,1,1,2,0,3,7,-1,-1},//611*_37 22

		{-1,-1,4,1,3,2,1,0,7,-1,-1},//413*1_7 23*

		{-1,7,0,1,3,2,1,4,-1,-1,-1},//7_13*14 24*

		{-1,6,1,0,3,2,1,6,7,-1,-1},//61_3*167 25

		{-1,6,1,3,0,2,1,6,-1,-1,-1},//613_*16 26

		{-1,7,6,1,3,2,0,1,6,-1,-1},//7613*_16 27

		{-1,7,0,3,1,2,1,4,-1,-1,-1},//7_31*14 28*

		{-1,7,3,0,1,2,1,0,-1,-1,-1},//73_1*16 29

		{-1,6,3,1,0,2,1,6,-1,-1,-1},//631_*16 30

		{-1,7,6,3,1,2,0,1,6,-1,-1},//7631*_16 31

		{-1,-1,4,3,1,2,1,0,-1,-1,-1},//431*1_ 32*

		{-1,0,3,1,1,2,4,-1,-1,-1,-1},//_311*4 33*

		{-1,3,0,1,1,2,6,-1,-1,-1,-1},//3_11*6 34

		{6,3,1,0,1,2,6,-1,-1,-1,-1},//631_1*6 35

		{-1,4,3,1,1,2,0,-1,-1,-1,-1},//4311*_ 36*

		{-1,0,1,3,1,2,4,-1,-1,-1,-1},//_131*4 37*

		{6,1,0,3,1,2,6,7,-1,-1,-1},//61_31*67 38

		{6,1,3,0,1,2,6,-1,-1,-1,-1},//613_1*6 39

		{6,1,3,1,0,2,6,-1,-1,-1,-1},//6131_*6 40

		{-1,4,1,3,1,2,0,7,-1,-1,-1},//4131*_7 41*

		{-1,0,1,1,3,2,4,-1,-1,-1,-1},//_113*4 42*

		{6,1,0,1,3,2,6,7,-1,-1,-1},//61_13*67 43

		{0,1,1,0,3,2,6,-1,-1,-1,-1},//_11_3*6 44

		{6,1,1,3,0,2,6,-1,-1,-1,-1},//6113_*6 45

		{-1,4,1,1,3,2,0,7,-1,-1,-1},//4113*_7 46*

	};

	POINT d[4];

	d[0].x = 1;d[0].y = 0;//横方向

	d[1].x = 0;d[1].y = 1;//纵方向

	d[2].x = 1;d[2].y = -1;//斜右上

	d[3].x = 1;d[3].y = 1;//斜左上

	int model_num = 47;

	int point_num = 11;

	BOOL flag;
	int i,j,k;
	for ( i=0;i<4;i++)

	{

		//四个方向遍历		

		for ( j=0;j<model_num;j++)//model_num种模式遍历，每种模式遍历两次，正反各一次

		{

			flag = true;

			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历

			{
				if (model[j][k]==0)
				{
					if (getcolor(color,x,y,i,k-5,tempresult,n)!=0)
					{

						//不匹配此模式

						flag = false;

						break;

					}

				}

				else if (model[j][k]==1)

				{

					if (getcolor(color,x,y,i,k-5,tempresult,n)!=1)

					{

						//不匹配此模式

						flag = false;

						break;

					}

				}				

				else if (model[j][k]==3)

				{

					if (getcolor(color,x,y,i,k-5,tempresult,n)!=3)

					{

						//不匹配此模式

						flag = false;

						break;

					}

				}	

				else if (model[j][k]==4)
				{
					if (getcolor(color,x,y,i,k-5,tempresult,n)!=4)
					{

						//不匹配此模式

						flag = false;

						break;

					}

				}	

				else if (model[j][k]==6)

				{

					if (getcolor(color,x,y,i,k-5,tempresult,n)==3)

					{

						//不匹配此模式

						flag = false;

						break;

					}

					if (getcolor(color,x,y,i,k-5,tempresult,n)==1)

					{

						//不匹配此模式

						flag = false;

						break;

					}

				}	
				else if (model[j][k]==7)
					
				{
					
					if (getcolor(color,x,y,i,k-5,tempresult,n)==3)
						
					{
						
						//不匹配此模式
						
						flag = false;
						
						break;
						
					}				
					
				}	

			}

			if (flag) 

			{

				//匹配上某种模式	

				if (debugqian)

				{

					CString s;

					s.Format(L"front:%d,(%d,%d)",j,x,y);

					AfxMessageBox(s);

				}				
				
				if (!qiang)
				{
					for ( k=0;k<point_num;k++)					
					{				
						if (model[j][k]==1)						
						{							
							//一起匹配的子						
							modelresult[counter].x = x+(k-5)*d[i].x;						
							modelresult[counter].y = y+(k-5)*d[i].y;							
							counter++;												
						}	
					}				
				}
				else
				{
					//判断是否是强掺杂牵制子匹配
					int count = 0;
					BOOL tempflag = true;
					for (int k=0;k<point_num;k++)						
					{						
						if ((model[j][k]==1)&&!is_canfour(color,x+(k-5)*d[i].x,y+(k-5)*d[i].y))							
						{
							//找到一个不是canfour的子，那么就不是强掺杂牵制子匹配
							if (j<6)
							{
								tempflag = false;
								break;
							}
							else
							{
								//可以有一个不是canfour
								if (count>=1)
								{
									tempflag = false;
									break;
								}
								else count++;
							}
							
						}						
					}
					if (tempflag)
					{
						//是强掺杂牵制子
						for (int k=0;k<point_num;k++)
							
						{
							
							if (model[j][k]==1)
								
							{
								
								//一起匹配的子
								
								modelresult[counter].x = x+(k-5)*d[i].x;
								
								modelresult[counter].y = y+(k-5)*d[i].y;
								
								counter++;						
								
							}
							
						}				
						
					}
				}				

				break;

			}

			flag = true;

			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历

			{

				if (model[j][k]==0)

				{

					if (getcolor(color,x,y,i,5-k,tempresult,n)!=0)

					{

						//不匹配此模式

						flag = false;

						break;

					}

				}

				else if (model[j][k]==1)

				{

					if (getcolor(color,x,y,i,5-k,tempresult,n)!=1)

					{

						//不匹配此模式

						flag = false;

						break;

					}

				}				

				else if (model[j][k]==3)

				{

					if (getcolor(color,x,y,i,5-k,tempresult,n)!=3)

					{

						//不匹配此模式

						flag = false;

						break;

					}

				}	

				else if (model[j][k]==4)

				{

					if (getcolor(color,x,y,i,5-k,tempresult,n)!=4)

					{

						//不匹配此模式

						flag = false;

						break;

					}

				}	

				else if (model[j][k]==6)

				{

					if (getcolor(color,x,y,i,5-k,tempresult,n)==3)

					{

						//不匹配此模式

						flag = false;

						break;

					}

					if (getcolor(color,x,y,i,5-k,tempresult,n)==1)

					{

						//不匹配此模式

						flag = false;

						break;

					}

				}	

				else if (model[j][k]==7)
					
				{
					
					if (getcolor(color,x,y,i,5-k,tempresult,n)==3)
						
					{
						
						//不匹配此模式
						
						flag = false;
						
						break;
						
					}					
					
				}	

			}

			if (flag) 

			{

				//匹配上某种模式

				if (debugqian)

				{

					CString s;

					s.Format(L"front:%d,(%d,%d)",j,x,y);

					AfxMessageBox(s);

				}
				if (!qiang)
				{
					for (int k=0;k<point_num;k++)
						
					{
						
						if (model[j][k]==1)
							
						{
							
							//一起匹配的子
							
							modelresult[counter].x = x+(5-k)*d[i].x;
							
							modelresult[counter].y = y+(5-k)*d[i].y;
							
							counter++;						
							
						}
						
					}				
				}
				else
				{
					//判断是否是强掺杂牵制子匹配
					BOOL tempflag = true;
					int count = 0;
					for (int k=0;k<point_num;k++)						
					{						
						if ((model[j][k]==1)&&!is_canfour(color,x+(5-k)*d[i].x,y+(5-k)*d[i].y))							
						{
							//找到一个不是canfour的子，那么就不是强掺杂牵制子匹配					
							if (j<6)
							{
								tempflag = false;
								break;
							}
							else
							{
								//可以有一个不是canfour
								if (count>=1)
								{
									tempflag = false;
									break;
								}
								else count++;
							}
						}						
					}
					if (tempflag)
					{
						//是强掺杂牵制子
						for (int k=0;k<point_num;k++)
							
						{
							
							if (model[j][k]==1)
								
							{
								
								//一起匹配的子
								
								modelresult[counter].x = x+(5-k)*d[i].x;
								
								modelresult[counter].y = y+(5-k)*d[i].y;
								
								counter++;						
								
							}
							
						}				
						
					}
				
				}

				break;

			}

		}

	}

	if (counter==0) return false;

	return true;	

}



BOOL CGoBangGame::has_qianmodel_addone(int color, int &counter, POINT tempresult[],BOOL qiang)
{
	//和has_qianmodel不同的是，它加入了一个已经下的子来匹配model
	//而has_qianmodel只对所有牵制子做匹配
	counter = 0;
	int count;
	POINT local_tempresult[225];
	int count2;
	POINT local_tempresult2[225];
	BOOL flag = false;

	if (has_qian(color,count,local_tempresult))
	{
		for (int i=1;i<=m_xGrid;i++)
			for (int j=1;j<=m_yGrid;j++)
				if ((ChessPoints[i][j]==color)&&(can_qianmodel(color,i,j,local_tempresult,count,count2,local_tempresult2,qiang)))
				{
					merge(tempresult,counter,local_tempresult2,count2);
					flag = true;
				}		
	}
	return flag;
}

BOOL CGoBangGame::has_qianmodel_addone(int color,BOOL qiang)
{
	//和has_qianmodel不同的是，它加入了一个已经下的子来匹配model
	//而has_qianmodel只对所有牵制子做匹配
	int count;
	POINT local_tempresult[225];
	int count2;
	POINT local_tempresult2[225];

	if (has_qian(color,count,local_tempresult))
	{
		for (int i=1;i<=m_xGrid;i++)
			for (int j=1;j<=m_yGrid;j++)
				if ((ChessPoints[i][j]==color)&&(can_qianmodel(color,i,j,local_tempresult,count,count2,local_tempresult2,qiang)))
				{
					return true;
				}		
	}
	return false;
}

void CGoBangGame::common_info(CString s,BOOL debug,int &cur_time)
{
	//CLogger *m_logger = CLogger::GetInstancePtr();
	//m_logger->Info(s);

//	AfxGetApp()->OnIdle( 1 );


	if (debug) ShowInfo(s+passtime(cur_time),true);
		else ShowInfo(s,true);

	if (debug) AfxMessageBox(s);
}

BOOL CGoBangGame::make_qianmodel(int color, int &counter, POINT tempresult[])
{
	//此函数用来生成制造牵制子匹配的棋
	//counter为候选结果的计数，tempresult为候选结果集
	counter = 0;
	BOOL flag = false;	//返回值
	POINT closepoints[225];
	int count = 0;
	getclose_small_subqian(color,count,closepoints);	
	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			if (!can_maketwolevel_alive(color,closepoints[i].x, closepoints[i].y))
			{
				ChessPoints[closepoints[i].x][closepoints[i].y] = color;			
				if (has_qianmodel(color)||has_qianmodel_addone(color))					
				{
					
					//如果存在匹配的牵制子，记录到结果集					
					tempresult[counter].x = closepoints[i].x;					
					tempresult[counter].y = closepoints[i].y;					
					counter++;					
					flag = true;			
					if (m_grade<5)
					{
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式				
						return true;
					}					
				}				
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式	
			}			
		}
	}
	return flag;
}

BOOL CGoBangGame::make_threelevel_qianmodel(int color, int &counter, POINT tempresult[])
{
	//此函数用来生成制造牵制子匹配的棋
	//counter为候选结果的计数，tempresult为候选结果集
	counter = 0;
	BOOL flag = false;	//返回值
	POINT closepoints[225];
	int count = 0;
	getclose_small_subqian(color,count,closepoints);
	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			if (!can_maketwolevel_alive(color,closepoints[i].x, closepoints[i].y)
				//&&!can_makeqianmodel(color,closepoints[i].x, closepoints[i].y)
				//&&!can_maketwolevelqianmodel(color,closepoints[i].x, closepoints[i].y)
				)
			{
				ChessPoints[closepoints[i].x][closepoints[i].y] = color;
				if (has_threelevel_qianmodel(color)) 
				{
					//如果存在匹配的三阶牵制子，记录到结果集
					tempresult[counter].x = closepoints[i].x;
					tempresult[counter].y = closepoints[i].y;
					counter++;
					flag = true;
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式
					return true;
				}
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式	
			}			
		}
	}
	return flag;
}


BOOL CGoBangGame::make_twolevel_qianmodel(int color, int &counter, POINT tempresult[])
{
	//此函数用来生成制造牵制子匹配的棋
	//counter为候选结果的计数，tempresult为候选结果集
	counter = 0;
	BOOL flag = false;	//返回值
	POINT closepoints[225];
	int count = 0;
	getclose_small_subqian(color,count,closepoints);
	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			if (!can_maketwolevel_alive(color,closepoints[i].x, closepoints[i].y)
				//&&!can_makeqianmodel(color,closepoints[i].x, closepoints[i].y)
				)
			{
				ChessPoints[closepoints[i].x][closepoints[i].y] = color;
				if (has_twolevel_qianmodel(color)) 
				{

					//如果存在匹配的二阶牵制子，记录到结果集
					tempresult[counter].x = closepoints[i].x;
					tempresult[counter].y = closepoints[i].y;
					counter++;
					flag = true;
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式
					return true;
				}
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式
			}
		}
	}
	return flag;
}

BOOL CGoBangGame::make_beatpoints(int color, int &counter, POINT tempresult[])
{	
	//此函数用来生成制造打击子的棋
	//counter为候选结果的计数，tempresult为候选结果集	
	counter = 0;	
	BOOL flag = false;	//返回值	
	POINT closepoints[225];
	int count = 0;
	getclose_small_subqian(color,count,closepoints);	
	for(int i=0;i<count;i++)	
	{	
		if (closepoints[i].x!=0)		
		{
			ChessPoints[closepoints[i].x][closepoints[i].y] = color;				
			if (has_beatpoints_low(color)) 					
			{			
				//如果存在匹配的打击子，记录到结果集				
				tempresult[counter].x = closepoints[i].x;					
				tempresult[counter].y = closepoints[i].y;					
				counter++;					
				flag = true;
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式					
				return true;					
			}			
			ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式			
		}
	}
	return flag;
}

BOOL CGoBangGame::twolevel_four(int x, int y, int color)
{
	//判断是否二阶活子中含有4个子的情况，如果含有，那么应该优先级比人的一阶活子高
	if (ChessPoints[x][y]!=0) return false;//已经有子了
	int model[][9]={//此数组表示各种模式，包括此子前四个和后四个子的排列
		//-1表示任意，1表示相同颜色，0表示空，2表示(x,y)所在位置
		{0,1,1,1,2,-1,-1,-1,-1},//_111* 0
		{1,0,1,1,2,-1,-1,-1,-1},//1_11* 1
		{1,1,0,1,2,-1,-1,-1,-1},//11_1* 2
		{1,1,1,0,2,-1,-1,-1,-1},//111_* 3
		{-1,1,1,1,2,0,-1,-1,-1},//111*_ 4
		{-1,0,1,1,2,1,-1,-1,-1},//_11*1 5
		{-1,1,0,1,2,1,-1,-1,-1},//1_1*1 6
		{-1,1,1,0,2,1,-1,-1,-1},//11_*1 7
		{-1,-1,1,1,2,0,1,-1,-1},//11*_1 8
		{-1,-1,1,1,2,1,0,-1,-1},//11*1_ 9
		{0,0,1,1,2,0,-1,-1,-1},//__11*_
		{0,1,0,1,2,0,-1,-1,-1},//_1_1*_
		{0,1,1,0,2,0,-1,-1,-1},//_11_*_
		{-1,0,1,1,2,0,0,-1,-1},//_11*__
		{-1,0,0,1,2,1,0,-1,-1},//__1*1_
		{-1,0,1,0,2,1,0,-1,-1},//_1_*1_
		{-1,-1,0,1,2,0,1,0,-1},//_1*_1_

	};//17种可以造成活子的情形

	int model_num = 17;
	int point_num = 9;
	BOOL flag;
	int count = 0;//累计有多少个方向的上有模式被匹配
	BOOL existfour = false;//判断是否有一个方向上存在四子

	int i,j,k;
	for ( i=0;i<4;i++)
	{
		//四个方向遍历		
		for ( j=0;j<model_num;j++)//model_num种模式遍历，每种模式遍历两次，正反各一次
		{
			flag = true;
			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}

			if (flag) 
			{
				//匹配上某种模式
				count++;
				if (j<=9) existfour = true;
				break;//此方向就不用再累加了
			}

			flag = true;
			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}

			if (flag) 
			{
				//匹配上某种模式
				count++;
				if (j<=9) existfour = true;
				break;//此方向就不用再累加了
			}
		}
	}

	if (count>=2) return existfour;
	return false;
}



BOOL CGoBangGame::has_twolevel_qianmodel(int color, int &counter, POINT tempresult[])

{

	//此函数判断是否存在二阶牵制子匹配

	//此定义如下：如果走牵制子，不论人怎么走，均存在牵制子匹配，那么此牵制子为二阶牵制子

	//counter为候选结果的计数，tempresult为候选结果集

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	counter = 0;

	BOOL flag = false;	//返回值

	POINT qianChessPoints[225];

	int count = 0;

	has_qian(color, count, qianChessPoints);	

	for(int i=0;i<count;i++)

	{

		if (qianChessPoints[i].x!=0)

		{

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = color;

			BOOL tempflag = true;//走对方的子，看看是否都存在牵制子匹配

			POINT roundpoints[225];
			int defentcount = 0;
			has_defent_point(color,defentcount,roundpoints);

			for(int j=0;j<defentcount;j++)

			{

				if (roundpoints[j].x!=0)//即是有效的位置

				{
					
					if ((m_grade==5)&&is_beatpoint(anticolor,roundpoints[j].x,roundpoints[j].y))						
					{
						tempflag = false;
						
						break;//对方用beatpoint来阻挡我，那么就不行
					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = anticolor;//走对方的子

					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)) 

					{

						//如果存在一种情况不存在牵制子匹配，那么失败

						ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

						tempflag = false;

						break;

					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

				}

			}

			if (tempflag)

			{

				//是二阶牵制子

				tempresult[counter].x = qianChessPoints[i].x;

				tempresult[counter].y = qianChessPoints[i].y;

				counter++;

				flag = true;



			}

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式

		}

	}

	return flag;

}



BOOL CGoBangGame::has_threelevel_qianmodel(int color, int &counter, POINT tempresult[])

{

	//此函数判断是否存在三阶牵制子匹配

	//此定义如下：如果走牵制子，不论人怎么走，均存在二阶牵制子匹配，那么此牵制子为三阶牵制子

	//counter为候选结果的计数，tempresult为候选结果集

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	counter = 0;

	BOOL flag = false;	//返回值

	POINT qianChessPoints[225];

	int count = 0;

	has_qian(color, count, qianChessPoints);	

	for(int i=0;i<count;i++)

	{

		if (qianChessPoints[i].x!=0)

		{

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = color;

			BOOL tempflag = true;//走对方的子，看看是否都存在牵制子匹配

			POINT roundpoints[225];
			int defentcount = 0;
			has_defent_point(color,defentcount,roundpoints);
			
			for(int j=0;j<defentcount;j++)

			{

				if (roundpoints[j].x!=0)//即是有效的位置

				{
					if ((m_grade==5)&&is_beatpoint(anticolor,roundpoints[j].x,roundpoints[j].y))						
					{
						tempflag = false;
						
						break;//对方用beatpoint来阻挡我，那么就不行
					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = anticolor;//走对方的子

					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 

					{

						//如果存在一种情况不存在牵制子匹配，那么失败

						ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

						tempflag = false;

						break;

					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

				}

			}

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式

			if (tempflag)

			{

				//是三阶牵制子

				tempresult[counter].x = qianChessPoints[i].x;

				tempresult[counter].y = qianChessPoints[i].y;

				counter++;

				flag = true;

				return true;				

			}

		}

	}

	return flag;

}



BOOL CGoBangGame::has_twolevel_qianmodel(int color)

{

	//此函数判断是否存在二阶牵制子匹配

	//此定义如下：如果走牵制子，不论人怎么走，均存在牵制子匹配，那么此牵制子为二阶牵制子

	//counter为候选结果的计数，tempresult为候选结果集

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	POINT qianChessPoints[225];

	int count = 0;

	has_qian(color, count, qianChessPoints);	

	for(int i=0;i<count;i++)

	{

		if (qianChessPoints[i].x!=0)

		{

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = color;

			BOOL tempflag = true;//走对方的子，看看是否都存在牵制子匹配

			POINT roundpoints[225];
			int defentcount = 0;
			has_defent_point(color,defentcount,roundpoints);
			
			for(int j=0;j<defentcount;j++)

			{

				if (roundpoints[j].x!=0)//即是有效的位置

				{

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = anticolor;//走对方的子

					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)) 

					{

						//如果存在一种情况不存在牵制子匹配，那么失败						

						ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

						tempflag = false;

						break;

					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

				}

			}

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式

			if (tempflag)

			{

				//是二阶牵制子

				return true;

				

			}			

		}

	}

	return false;

}



BOOL CGoBangGame::has_threelevel_qianmodel(int color)

{

	//此函数判断是否存在三阶牵制子匹配

	//此定义如下：如果走牵制子，不论人怎么走，均存在二阶牵制子匹配，那么此牵制子为三阶牵制子

	//counter为候选结果的计数，tempresult为候选结果集

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	POINT qianChessPoints[225];

	int count = 0;

	has_qian(color, count, qianChessPoints);	

	for(int i=0;i<count;i++)

	{

		if (qianChessPoints[i].x!=0)

		{

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = color;

			BOOL tempflag = true;//走对方的子，看看是否都存在牵制子匹配

			POINT roundpoints[225];
			int defentcount = 0;
			has_defent_point(color,defentcount,roundpoints);
			
			for(int j=0;j<defentcount;j++)

			{

				if (roundpoints[j].x!=0)//即是有效的位置

				{

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = anticolor;//走对方的子

					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 

					{

						//如果存在一种情况不存在二阶牵制子匹配，那么失败

						ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

						tempflag = false;

						break;

					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

				}

			}

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式

			if (tempflag)

			{

				//是三阶牵制子

				return true;

				

			}			

		}

	}

	return false;

}



BOOL CGoBangGame::has_twolevel_beat(int color, int &counter, POINT tempresult[])

{

	//此函数判断是否存在二阶打击子

	//此定义如下：如果走牵制子，不论人怎么走，均存在二阶牵制子匹配，那么此牵制子为三阶牵制子

	//counter为候选结果的计数，tempresult为候选结果集

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	counter = 0;

	BOOL flag = false;	//返回值

	POINT qianChessPoints[225];

	int count = 0;

	has_qian(color, count, qianChessPoints);	

	for(int i=0;i<count;i++)

	{

		if (qianChessPoints[i].x!=0)

		{

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = color;

			BOOL tempflag = true;//走对方的子，看看是否都存在牵制子匹配

			POINT roundpoints[225];
			int defentcount = 0;
			has_defent_point(color,defentcount,roundpoints);
			
			for(int j=0;j<defentcount;j++)

			{

				if (roundpoints[j].x!=0)//即是有效的位置

				{

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = anticolor;//走对方的子

					if (!has_beatpoints_low(color)) 

					{

						//如果存在一种情况不存在二阶打击子，那么失败

						ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

						tempflag = false;

						break;

					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

				}

			}

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式

			if (tempflag)

			{

				//是二阶打击子

				tempresult[counter].x = qianChessPoints[i].x;

				tempresult[counter].y = qianChessPoints[i].y;

				counter++;

				flag = true;

				return true;				

			}

		}

	}

	return flag;

}

BOOL CGoBangGame::has_twolevel_beat_low(int color, int &counter, POINT tempresult[])
{
	//此函数判断是否存在二阶打击子
	//此定义如下：如果走牵制子，不论人怎么走，均存在二阶牵制子匹配，那么此牵制子为三阶牵制子
	//counter为候选结果的计数，tempresult为候选结果集
	
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	
	counter = 0;
	BOOL flag = false;	//返回值
	POINT qianChessPoints[225];
	int count = 0;
	has_qian(color, count, qianChessPoints);	
	for(int i=0;i<count;i++)	
	{	
		if (qianChessPoints[i].x!=0)		
		{			
			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = color;
			BOOL tempflag = true;//走对方的子，看看是否都存在牵制子匹配	
			POINT roundpoints[225];
			int defentcount = 0;
			has_defent_point(color,defentcount,roundpoints);		
			for(int j=0;j<defentcount;j++)		
			{
				if (roundpoints[j].x!=0)//即是有效的位置	
				{
					ChessPoints[roundpoints[j].x][roundpoints[j].y] = anticolor;//走对方的子
					if (!has_beatpoints_low(color)) 
					{
						//如果存在一种情况不存在二阶打击子，那么失败
						ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;
						tempflag = false;
						break;
					}
					ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;
				}
			}
			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式
			if (tempflag)
			{
				//是二阶打击子
				tempresult[counter].x = qianChessPoints[i].x;
				tempresult[counter].y = qianChessPoints[i].y;
				counter++;
				flag = true;
				return true;
			}
		}
	}
	return flag;
}

BOOL CGoBangGame::is_makealive2(int x, int y, int color)
{
	//比is_makealive更强，其defent_point不能是对方的冲四子
	if (!is_makealive(x,y,color)) return false;
	int anticolor;//对方的color	
	if (color==1) anticolor = 2;	
	else anticolor = 1;
	ChessPoints[x][y] = color;
	int count = 0;
	POINT tempresult[225];
	has_defent_point(color,count,tempresult);
	ChessPoints[x][y] = 0;
	for (int i=0;i<count;i++)
	{
		if (is_canfour(anticolor,tempresult[i].x,tempresult[i].y))
		{
			return false;
		}
	}
	return true;
}

BOOL CGoBangGame::is_maketwolevelalive(int x, int y, int color)
{
	POINT twolevel_pre[225];
	int count_pre;
	has_twolevel(color,count_pre,twolevel_pre);
	ChessPoints[x][y] = color;
	POINT twolevel[225];
	int count;
	has_twolevel(color,count,twolevel);
	ChessPoints[x][y] = 0;
	if (count>count_pre) return true;
	return false;
}
BOOL CGoBangGame::is_makealive(int x, int y, int color)

{

	//判断是否是可以生成一阶活子的点

	if (ChessPoints[x][y]!=0) return false;//已经有子了

	int model[][9]={//此数组表示各种模式，包括此子前四个和后四个子的排列

		//-1表示任意，1表示相同颜色，0表示空，2表示(x,y)所在位置

		{0,0,1,1,2,0,-1,-1,-1},//__11*_ 0

		{-1,0,1,1,2,0,0,-1,-1},//_11*__ 1

		{0,1,0,1,2,0,-1,-1,-1},//_1_1*_ 2

		{0,1,1,0,2,0,-1,-1,-1},//_11_*_ 3

		{-1,0,1,0,2,1,0,-1,-1},//_1_*1_ 4

		{-1,0,0,1,2,1,0,-1,-1},//__1*1_ 5

	};

	int model_num = 6;

	int point_num = 9;

	BOOL flag;
	int i,j,k;
	for ( i=0;i<4;i++)

	{

		//四个方向遍历		

		for ( j=0;j<model_num;j++)//model_num种模式遍历，每种模式遍历两次，正反各一次

		{

			flag = true;

			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历

			{

				if (model[j][k]==0)

				{

					if (getcolor(x,i,k-4,y)!=0)

					{

						//不匹配此模式

						flag = false;

						break;

					}

				}

				else if (model[j][k]==1)

				{

					if (getcolor(x,i,k-4,y)!=color)

					{

						//不匹配此模式

						flag = false;

						break;

					}

				}				

			}

			if (flag) return true;//匹配上某种模式

			flag = true;

			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历

			{

				if (model[j][k]==0)

				{

					if (getcolor(x,i,4-k,y)!=0)

					{

						//不匹配此模式

						flag = false;

						break;

					}

				}

				else if (model[j][k]==1)

				{

					if (getcolor(x,i,4-k,y)!=color)

					{

						//不匹配此模式

						flag = false;

						break;

					}

				}				

			}

			if (flag) return true;//匹配上某种模式

		}

	}

	return false;

}



CString CGoBangGame::passtime(int &cur_time)
{
	//统计用的时间，cur_time为上次计时时的时间起点，返回以s作为单位的字符串
	//并将cur_time改为当前时间
#if 1	
	return L"";
#else

	CString s;
	double timeinter = (double)(GetTickCount()-cur_time)/1000.0;
	if (timeinter>10) timeout = true;
	else timeout = false;

	s.Format(L"%fs",timeinter);
	cur_time = GetTickCount();
	return s;
#endif

}

void CGoBangGame::sub(POINT target[], int &tn, POINT subChessPoints[], int sn)
{

	//用于将target中的subChessPoints点去掉

	POINT target_temp[225];

	int tn_temp = 0;
	int i;
	for (i=0;i<tn;i++)
	{
		if (target[i].x==0) continue;
		BOOL flag = true;
		for (int j=0;j<sn;j++)
		{
			if ((target[i].x==subChessPoints[j].x)&&(target[i].y==subChessPoints[j].y))
			{
				//存在此点，应该删去
				flag = false;	
				break;
			}
		}
		if (flag)
		{
			target_temp[tn_temp].x = target[i].x;
			target_temp[tn_temp].y = target[i].y;			
			tn_temp++;
		}
	}

	tn = tn_temp;

	for (i=0;i<tn;i++)

	{

		target[i].x = target_temp[i].x;

		target[i].y = target_temp[i].y;

	}

}



void CGoBangGame::getclose_subqian(int color, int &counter, POINT tempresult[])

{

	//用于得到不是牵制子的最近子，因为make算法需要这种子

	counter = 0;

	getclose(color,counter,tempresult);	

	POINT qianChessPoints[225];

	int qiancount;

	//虽然牵制子都应该被去掉，但是canfour的子还是有一定优势，因此留下
	//后来发现，不能留!!也不用留，会有更高阶的子来判断

	if (has_qian(color,qiancount,qianChessPoints))

		sub(tempresult,counter,qianChessPoints,qiancount);

		

}

void CGoBangGame::getclose_small_subqian(int color,int &counter, POINT tempresult[])

{
	
	//用于得到不是牵制子的最近子，因为make算法需要这种子
	
	counter = 0;
	
	getclose_small(counter,tempresult);	
	
	POINT qianChessPoints[225];
	
	int qiancount;
	
	//虽然牵制子都应该被去掉，但是canfour的子还是有一定优势，因此留下
	//后来发现，不能留!!也不用留，会有更高阶的子来判断
	
	if (has_qian(color,qiancount,qianChessPoints))
		
		sub(tempresult,counter,qianChessPoints,qiancount);	
	
}



void CGoBangGame::selectqianmodel(int color, int &x, int &y, POINT tempresult[],int n)

{

	//这个函数只用于defent后，主要作用是在候选的子中找出make_qianmodel的子，再进行运算

	if (n==1)
	{
		//只有一个点，那么直接返回
		x = tempresult[0].x;
		y = tempresult[0].y;
		return;
	}

	POINT local_tempresult[225];

	int local_count = 0;

	BOOL flag = false;//是否有子是makeqianmodel

	for (int i=0;i<n;i++)
		
	{
		
		if (is_qian(color,tempresult[i].x,tempresult[i].y)
			
			||can_makeqianmodel(color,tempresult[i].x,tempresult[i].y)
			
			||can_maketwolevelqianmodel(color,tempresult[i].x,tempresult[i].y)
			
			)
			
		{
			
			local_tempresult[local_count].x = tempresult[i].x;
			
			local_tempresult[local_count].y = tempresult[i].y;
			
			local_count++;
			
			flag = true;
			
		}
		
	}

	if (!flag)

	{

		//没有交集，那么按照原来的计划行事

		selectless_twomodel(color,x,y,tempresult,n);

	}

	else

	{

		//有交集，那么从这个交集中选取

		selectless_twomodel(color,x,y,local_tempresult,local_count);

	}	

}



void CGoBangGame::selectqianmodel3(int color, int &x, int &y, POINT tempresult[],int n)
{
	//只用于grade=3
	//这个函数只用于defent后，主要作用是在候选的子中找出make_qianmodel的子，再进行运算
	if (n==1)
	{
		//只有一个点，那么直接返回
		x = tempresult[0].x;
		y = tempresult[0].y;
		return;
	}

	POINT local_tempresult[225];
	int local_count = 0;
	BOOL flag = false;//是否有子是makeqianmodel
	for (int i=0;i<n;i++)
	{
		if (is_qian(color,tempresult[i].x,tempresult[i].y)	
			||can_makeqianmodel3(color,tempresult[i].x,tempresult[i].y)  )
		{		
			local_tempresult[local_count].x = tempresult[i].x;
			local_tempresult[local_count].y = tempresult[i].y;	
			local_count++;	
			flag = true;	
		}
	}

	if (!flag)
	{
		//没有交集，那么按照原来的计划行事
		selectless_twomodel(color,x,y,tempresult,n);
	}
	else
	{
		//有交集，那么从这个交集中选取
		selectless_twomodel(color,x,y,local_tempresult,local_count);
	}	
}

void CGoBangGame::selectqianmodel5(int color, int &x, int &y, POINT tempresult[],int n,BOOL defent)
{
	//只用于grade=5时
	//这个函数只用于defent后，主要作用是在候选的子中找出make_qianmodel的子，再进行运算
	if (!defent)
		cut_dangerpoints(color,tempresult,n);
	if (n==1)
	{
		//只有一个点，那么直接返回
		x = tempresult[0].x;
		y = tempresult[0].y;
		return;
	}

	POINT local_tempresult[225];
	int local_count = 0;
	BOOL flag = false;//是否有子是makeqianmodel
	for (int i=0;i<n;i++)	
	{	
		if (is_qian(color,tempresult[i].x,tempresult[i].y)
			||can_maketwolevel_alive(color,tempresult[i].x,tempresult[i].y)
			||is_maketwoalive(tempresult[i].x,tempresult[i].y,color)
			||is_makealiveandtwolevel(color,tempresult[i].x,tempresult[i].y)	
			||can_makeqianmodel(color,tempresult[i].x,tempresult[i].y)	
			||can_maketwolevelqianmodel(color,tempresult[i].x,tempresult[i].y)	
			//||can_makethreelevelqianmodel(color,tempresult[i].x,tempresult[i].y)			
			)		
		{		
			local_tempresult[local_count].x = tempresult[i].x;		
			local_tempresult[local_count].y = tempresult[i].y;	
			local_count++;
			flag = true;		
		}
	}
	
	if (!flag)
	{
		//没有交集，那么按照原来的计划行事
		selectchongtwo(color,x,y,tempresult,n);
	}
	else
	{
		//有交集，那么从这个交集中选取
		selectchongtwo(color,x,y,local_tempresult,local_count);
	}	
}

void CGoBangGame::selectchongtwo(int color, int &x, int &y, POINT tempresult[],int n)

{
	
	//只用于grade=5时
	
	//这个函数只用于defent后，主要作用是在候选的子中找出make_qianmodel的子，再进行运算
	
	if (n==1)
	{
		//只有一个点，那么直接返回
		x = tempresult[0].x;
		y = tempresult[0].y;
		return;
	}
	
	POINT local_tempresult[225];
	
	int local_count = 0;
	
	BOOL flag = false;//是否有子是makeqianmodel
	
	
	for (int i=0;i<n;i++)
		
	{
		
		if (is_chongtwo(color,tempresult[i].x,tempresult[i].y))
			
		{
			
			local_tempresult[local_count].x = tempresult[i].x;
			
			local_tempresult[local_count].y = tempresult[i].y;
			
			local_count++;
			
			flag = true;
			
		}
		
	}
	
	
	if (!flag)
		
	{
		
		//没有交集，那么按照原来的计划行事
		
		selectless_twomodel(color,x,y,tempresult,n);
		
	}
	
	else
		
	{
		
		//有交集，那么从这个交集中选取
		
		selectless_twomodel(color,x,y,local_tempresult,local_count);
		
	}	
	
}



void CGoBangGame::selectless_twomodel(int color, int &x, int &y, POINT tempresult[],int n)

{

	//这个函数用于selectqianmodel之后，主要解决了对方冲四时赋值算法不能如实体现的问题

	//主要算法如下：

	//1、走某一个候选子，看此时对方的twomodel的个数

	//2、恢复所有空子

	//3、选出twomodel的个数最小的候选子进行下面的步骤	

	if (n==1)
	{
		//只有一个点，那么直接返回
		x = tempresult[0].x;
		y = tempresult[0].y;
		return;
	}

	int local_count;

	POINT local_point[225];

	int min_twomodel = 225;

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	
	for (int i=0;i<n;i++)
		
	{
		
		ChessPoints[tempresult[i].x][tempresult[i].y] = color;
		
		int twoqian_counter;
		
		POINT twoqian_ChessPoints[225];
		
		has_twoqianmodel(anticolor,twoqian_counter,twoqian_ChessPoints);
		
		if (twoqian_counter<min_twomodel)
			
		{
			
			local_point[0].x = tempresult[i].x;
			
			local_point[0].y = tempresult[i].y;
			
			local_count = 1;
			
			min_twomodel = twoqian_counter;
			
		}
		
		else if (twoqian_counter==min_twomodel)
			
		{
			
			//加入候选结果集
			
			local_point[local_count].x = tempresult[i].x;
			
			local_point[local_count].y = tempresult[i].y;
			
			local_count++;
			
		}
		
		ChessPoints[tempresult[i].x][tempresult[i].y] = 0;

	}	

	if (color==1)
		selectmax(color,x,y,local_point,local_count,true);
	else
		selectmax(color,x,y,local_point,local_count);
}

BOOL CGoBangGame::has_tie()
{
	//判断平棋
	BOOL flag = true;
	for (int i=1;i<=m_xGrid;i++)
		for (int j=1;j<=m_yGrid;j++)
		{
			if (ChessPoints[i][j]==0)
			{
				flag = false;
				break;
			}
		}
	return flag;
}



BOOL CGoBangGame::can_makeqianmodel(int color, int x, int y)

{

	//判断某子是否可以形成qianmodel

	if (is_qian(color,x,y)) return false;
	if (has_qianmodel(color)||has_qianmodel_addone(color)) return false;

	BOOL flag = false;

	//if (can_maketwolevel_alive(color,x,y)) return false;

	ChessPoints[x][y] = color;

	if (has_qianmodel(color)||has_qianmodel_addone(color)) 

	{

		flag = true;

	}

	ChessPoints[x][y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式

	return flag;

}



BOOL CGoBangGame::can_makeqianmodel3(int color, int x, int y)

{

	//判断某子是否可以形成qianmodel

	if (has_qianmodel(color)) return false;

	BOOL flag = false;

	ChessPoints[x][y] = color;

	if (has_qianmodel(color)) 

	{

		flag = true;

	}

	ChessPoints[x][y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式

	return flag;

}



BOOL CGoBangGame::can_maketwolevelqianmodel(int color, int x, int y)

{

	//判断某子是否可以形成twolevelqianmodel

	if (is_qian(color,x,y)) return false;

	BOOL flag = false;

	if (can_maketwolevel_alive(color,x,y)||can_makeqianmodel(color,x,y)) return false;

	if (has_twolevel_qianmodel(color)) return false;

	ChessPoints[x][y] = color;

	if (has_twolevel_qianmodel(color))

	{

		flag = true;

	}

	ChessPoints[x][y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式

	return flag;

}



BOOL CGoBangGame::can_makethreelevelqianmodel(int color, int x, int y)

{

	//判断某子是否可以形成threelevelqianmodel

	if (is_qian(color,x,y)) return false;

	BOOL flag = false;

	if (can_maketwolevel_alive(color,x,y)||can_makeqianmodel(color,x,y)||can_maketwolevelqianmodel(color,x,y)) return false;

	if (has_threelevel_qianmodel(color)) return false;

	ChessPoints[x][y] = color;

	if (has_threelevel_qianmodel(color))

	{

		flag = true;

	}

	ChessPoints[x][y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式

	return flag;

}



BOOL CGoBangGame::can_maketwolevel_alive(int color, int x, int y)

{

	//判断某子是否可以形成twolevel_alive
	if (has_twolevel(color)) return false;

	BOOL flag = false;

	ChessPoints[x][y] = color;

	if (has_twolevel(color))

	{

		flag = true;

	}

	ChessPoints[x][y] = 0;//恢复原状，这个函数必须采用试子算法，因为没有统一的模式

	return flag;

}



BOOL CGoBangGame::is_qian(int color, int x, int y)

{

	//是否是牵制子

	//全部牵制子，包括一阶活子、二阶活子和连四的子，以及制造一阶活子的子

	if (is_alive(x,y,color)||twolevel_alive(x,y,color)||is_canfour(color,x,y)||is_makealive(x,y,color))

		return true;

	return false;

}



BOOL CGoBangGame::is_qiannofour(int color, int x, int y)

{

	//是否是牵制子

	//部分牵制子，包括一阶活子、二阶活子以及制造一阶活子的子

	//不包括连四的子

	if (is_alive(x,y,color)||twolevel_alive(x,y,color)||is_makealive(x,y,color))

		return true;
	return false;
}

void CGoBangGame::setGrade(int grade)
{
	//设定电脑的智力等级
	m_grade = grade;
}

BOOL CGoBangGame::has_beatpoints(int color, int &counter, POINT tempresult[])

{
	//这个算法只用于grade=5中，主要是让对方很难走棋，用grade4的棋力模仿对方走棋
	//这个算法如下：找到所有这样的子：走此子，则对方走棋的时候isdanger=true，那么这个子即为beatChessPoints
	counter = 0;

	int anticolor;//对方的color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;	

	int count;

	POINT closepoints[225];

	//getclose_small(count,closepoints);
	make_alive(color,count,closepoints);

	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			ChessPoints[closepoints[i].x][closepoints[i].y] = color;//假走一步
			isdanger = false;
			is_danger(anticolor);
			if (isdanger)
			{
				//如果这时对方陷入困境，那么存入tempresult中
				tempresult[counter].x = closepoints[i].x;
				tempresult[counter].y = closepoints[i].y;
				counter++;
				flag = true;
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
				return true;
			}
			ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
		}
	}
	return flag;
}

BOOL CGoBangGame::has_beatpoints_low(int color, int &counter, POINT tempresult[])
{	
	//这个算法只用于grade=5中，主要是让对方很难走棋，用grade4的棋力模仿对方走棋	
	//这个算法如下：找到所有这样的子：走此子，则对方走棋的时候isdanger=true，那么这个子即为beatChessPoints	
	counter = 0;	
	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;	
	
	int count;
	
	POINT closepoints[225];
	
	getclose_small_subqian(color,count,closepoints);
	
	for(int i=0;i<count;i++)
		
	{
		
		if (closepoints[i].x!=0)
			
		{
			
			ChessPoints[closepoints[i].x][closepoints[i].y] = color;//假走一步
			
			isdanger = false;
			
			is_danger(anticolor);
			
			if (isdanger)
				
			{
				
				//如果这时对方陷入困境，那么存入tempresult中
				
				tempresult[counter].x = closepoints[i].x;
				
				tempresult[counter].y = closepoints[i].y;
				
				counter++;
				
				flag = true;
				
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
				
				return true;
				
			}
			
			ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
			
		}
		
	}
	
	return flag;
	
}

BOOL CGoBangGame::has_beatpoints_low(int color)
{
	
	//这个算法只用于grade=5中，主要是让对方很难走棋，用grade4的棋力模仿对方走棋
	
	//这个算法如下：找到所有这样的子：走此子，则对方走棋的时候isdanger=true，那么这个子即为beatChessPoints
	
	
	int anticolor;//对方的color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;	
	
	int count;
	
	POINT closepoints[225];
	
	getclose_small_subqian(color,count,closepoints);
	
	for(int i=0;i<count;i++)
		
	{
		
		if (closepoints[i].x!=0)
			
		{
			
			ChessPoints[closepoints[i].x][closepoints[i].y] = color;//假走一步
			
			isdanger = false;
			
			is_danger(anticolor);
			
			if (isdanger)
				
			{
				
				//如果这时对方陷入困境，那么存入tempresult中
				
				flag = true;
				
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
				
				return true;
				
			}
			
			ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
			
		}
		
	}
	
	return flag;
	
}

BOOL CGoBangGame::has_beatpoints(int color)
{
	//这个算法只用于grade=5中，主要是让对方很难走棋，用grade4的棋力模仿对方走棋
	//这个算法如下：找到所有这样的子：走此子，则对方走棋的时候isdanger=true，那么这个子即为beatChessPoints
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	BOOL flag = false;	
	int count;

	POINT closepoints[225];
	getclose_small(count,closepoints);
	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			ChessPoints[closepoints[i].x][closepoints[i].y] = color;//假走一步
			isdanger = false;
			is_danger(anticolor);
			if (isdanger)
			{
				//如果这时对方陷入困境，那么存入tempresult中				
				flag = true;
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
				return true;
			}
			ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//恢复原值
		}
	}
	return flag;
}

void CGoBangGame::is_danger(int color)
{
	//用于判断是否在危险中
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	if (has_five(1)||has_five(2)) return;//已经连成5子，什么都不做了
	int x,y;	
	if (can_five(color,x,y)) 		
	{		
		return;//如果能连成5子，那么返回	
	}
	
	if (can_five(anticolor,x,y)) 	
	{		
		return;//看对方能否连成5子	
	}

	if (has_alive(color))	
	{			
		return;	
	}
	if (has_twolevel_four(color))	
	{	
		return;	
	}
	//1_____________________________________
	int i;
	i=1;
	if (has_alive_defent(anticolor))
	{
		//看对方是否存在活子
		return;
	}
	if (!defent_success) 
	{
		isdanger = true;
		return;
	}	
	//2_____________________________________
	if (has_twolevel_four_defent(anticolor))
	{
		//存在一个存在四子的二阶活子
		return;
	}
	if (!defent_success)
	{
		//不能防住对方的强二阶活子,冲四
		isdanger = true;
		return;
	}	
	if (has_twolevel_pure(color))
	{
		return;
	}
	if (has_qianmodel_addone(color))
	{
		return;
	}
	if (has_qianmodel(color))
	{
		return;
	}
	
/*
	if (has_twolevel_qianmodel(color))
	{
		return;
	}
*/
	//3___________________________
	if (has_twolevel_defent(anticolor))
	{
		//看对方是否存在二阶活子
		return;
	}	
	if (!defent_success)
	{
		//不能防住对方的二阶活子,冲四
		isdanger = true;
		return;
	}
}

BOOL CGoBangGame::make_twoalive(int color, int &counter, POINT tempresult[])
{
	//判断一个棋盘有没有可以一下子生成多个一阶活子的点
	//counter为候选结果的计数，tempresult为候选结果集
	counter = 0;
	BOOL flag = false;	//返回值
	POINT closepoints[225];
	int count = 0;
	getclose(color,count,closepoints);	
	for(int i=0;i<count;i++)		
	{		
		if (closepoints[i].x!=0)			
		{
			if (is_maketwoalive(closepoints[i].x,closepoints[i].y,color)) 
			{
				tempresult[counter].x = closepoints[i].x;
				tempresult[counter].y = closepoints[i].y;
				counter++;
				flag = true;
			}			
		}		
	}	
	return flag;
}

BOOL CGoBangGame::isempty()
{
	for (int i=1;i<=m_xGrid;i++)
		for (int j=1;j<=m_yGrid;j++)
		{
			if (ChessPoints[i][j]!=0)
			{
				return false;
			}
		}
	return true;
}

BOOL CGoBangGame::has_twolevelqianmodel_qiang(int color, int &counter, POINT tempresult[])
{
	//强二阶牵制子匹配，走canfour的子，挡住此子，如果这时存在强掺杂牵制子或者强牵制子匹配
	//那么即为强二阶牵制子匹配
	counter = 0;
	int anticolor;//对方的color	
	if (color==1) anticolor = 2;	
	else anticolor = 1;
	int count;
	POINT localChessPoints[225];
	BOOL flag = false;
	if (has_canfour(color,count,localChessPoints))
	{
		for (int i=0;i<count;i++)
		{
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = color;//走一步
			int x,y;
			can_five(color,x,y);//x,y为对方挡此子的子
			ChessPoints[x][y] = anticolor;//再走一步
			if (can_five(color)||has_alive(color)||has_twolevel_four(color)||has_qianmodel(color,true)||has_qianmodel_addone(color,true))
			{
				//如果此时存在强一阶牵制子，那么这个子即为强二阶牵制子
				tempresult[counter].x = localChessPoints[i].x;				
				tempresult[counter].y = localChessPoints[i].y;				
				counter++;
				flag = true;
			}
			ChessPoints[x][y] = 0;//恢复一步
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//再恢复一步
		}
	}
	return flag;
}

BOOL CGoBangGame::has_twolevelqianmodel_qiang(int color)
{
	//强二阶牵制子匹配，走canfour的子，挡住此子，如果这时存在强掺杂牵制子或者强牵制子匹配
	//那么即为强二阶牵制子匹配
	int anticolor;//对方的color	
	if (color==1) anticolor = 2;	
	else anticolor = 1;
	int count;
	POINT localChessPoints[225];
	BOOL flag = false;
	if (has_canfour(color,count,localChessPoints))
	{
		for (int i=0;i<count;i++)
		{
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = color;//走一步
			int x,y;
			can_five(color,x,y);//x,y为对方挡此子的子
			ChessPoints[x][y] = anticolor;//再走一步
			if (can_five(color)||has_alive(color)||has_twolevel_four(color)||has_qianmodel(color,true)||has_qianmodel_addone(color,true))
			{
				//如果此时存在强一阶牵制子，那么这个子即为强二阶牵制子				
				flag = true;
				ChessPoints[x][y] = 0;//恢复一步
				ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//再恢复一步
				return true;
			}
			ChessPoints[x][y] = 0;//恢复一步
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//再恢复一步
		}
	}
	return flag;
}

BOOL CGoBangGame::has_threelevelqianmodel_qiang(int color, int &counter, POINT tempresult[])
{
	//强三阶匹配，走一个canfour，挡住，看是否存在强二阶匹配
	counter = 0;
	int anticolor;//对方的color	
	if (color==1) anticolor = 2;	
	else anticolor = 1;
	int count;
	POINT localChessPoints[225];
	BOOL flag = false;
	if (has_canfour(color,count,localChessPoints))
	{
		for (int i=0;i<count;i++)
		{
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = color;//走一步
			int x,y;
			can_five(color,x,y);//x,y为对方挡此子的子
			ChessPoints[x][y] = anticolor;//再走一步
			if (has_twolevelqianmodel_qiang(color))
			{
				//如果此时存在强二阶牵制子，那么这个子即为强三阶牵制子
				tempresult[counter].x = localChessPoints[i].x;				
				tempresult[counter].y = localChessPoints[i].y;				
				counter++;
				flag = true;
				ChessPoints[x][y] = 0;//恢复一步
				ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//再恢复一步
				return true;
			}
			ChessPoints[x][y] = 0;//恢复一步
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//再恢复一步
		}
	}
	return flag;
}

BOOL CGoBangGame::has_threelevelqianmodel_qiang(int color)
{
	//强三阶匹配，走一个canfour，挡住，看是否存在强二阶匹配
	int anticolor;//对方的color	
	if (color==1) anticolor = 2;	
	else anticolor = 1;
	int count;
	POINT localChessPoints[225];
	BOOL flag = false;
	if (has_canfour(color,count,localChessPoints))
	{
		for (int i=0;i<count;i++)
		{
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = color;//走一步
			int x,y;
			can_five(color,x,y);//x,y为对方挡此子的子
			ChessPoints[x][y] = anticolor;//再走一步
			if (has_twolevelqianmodel_qiang(color))
			{
				//如果此时存在强二阶牵制子，那么这个子即为强三阶牵制子
				flag = true;
				ChessPoints[x][y] = 0;//恢复一步
				ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//再恢复一步
				return true;
			}
			ChessPoints[x][y] = 0;//恢复一步
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//再恢复一步
		}
	}
	return flag;
}

BOOL CGoBangGame::is_maketwoalive(int x, int y, int color)
{
	POINT localtemp[225];
	POINT localtemp_pre[225];
	int alive_count = 0;
	int alive_count_pre = 0;
	BOOL flag = false;
	has_alive(color,alive_count_pre,localtemp_pre);	
	ChessPoints[x][y] = color;
	if (has_alive(color,alive_count,localtemp)) 	
	{
		if (alive_count<=alive_count_pre)
		{
			ChessPoints[x][y] = 0;
			return false;
		}
		if (alive_count>2)
		{
			flag = true;
		}	
		else if (alive_count==2)
		{			
			//两个都是xx_x型的
			flag = true;		
			int x_dif,y_dif;			
			x_dif = localtemp[0].x - localtemp[1].x;			
			if (x_dif<0) x_dif = -x_dif;			
			y_dif = localtemp[0].y - localtemp[1].y;		
			if (y_dif<0) y_dif = -y_dif;			
			if (((x_dif==0)&&(y_dif==4))				
				||((y_dif==0)&&(x_dif==4))
				||((x_dif==y_dif)&&(y_dif==4))
				)			
			{			
				flag = false;
				
			}	
		}	
	}
	ChessPoints[x][y] = 0;
	return flag;
}

BOOL CGoBangGame::is_qianmodel(int color, int x, int y)
{
	//判断是否是一阶牵制子
	if (!is_qian(color,x,y)) return false;//首先要是牵制子
	POINT qian[225];
	int qian_count;	
	has_qian(color,qian_count,qian);
	if (qian_count>0)
	{
		if (can_qianmodel(color,x,y,qian,qian_count)) return true;//纯牵制子
		POINT addoneqian[225];
		int addoneqian_count;
		for (int i=1;i<m_xGrid;i++)
		{
			for (int j=1;j<=m_yGrid;j++)
			{
				if ((ChessPoints[i][j]==color)&&can_qianmodel(color,i,j,qian,qian_count,addoneqian_count,addoneqian))
				{
					//掺杂牵制子
					for (int k=0;k<addoneqian_count;k++)
					{
						if ((addoneqian[k].x==x)&&(addoneqian[k].y==y))
						{
							//存在于掺杂牵制子中
							return true;
						}
					}
				}
			}			
		}		
	}
	return false;
}

BOOL CGoBangGame::has_qiang_beat(int color, int &counter, POINT tempresult[])
{
	//找到强打击子，即走一个canfour子，用对方的子挡住，这时看看存在不存在beat子
	counter = 0;
	if (!has_canfour(color)||has_beatpoints_low(color)) return false;//已经有强打击子
	int anticolor;//对方的color	
	if (color==1) anticolor = 2;	
	else anticolor = 1;
	int count;
	POINT localChessPoints[225];
	BOOL flag = false;
	if (has_canfour(color,count,localChessPoints))
	{
		int x1,y1;
		selectqianmodel(color,x1,y1,localChessPoints,count);//此处假装选择人最容易走的四子，而不是全部判断		
		ChessPoints[x1][y1] = color;//走一步
		int x,y;
		can_five(color,x,y);//x,y为对方挡此子的子
		ChessPoints[x][y] = anticolor;//再走一步
		if (has_beatpoints_low(color))
		{
			//如果此时存在打击子，那么这个子即为强打击子
			tempresult[counter].x = x1;				
			tempresult[counter].y = y1;				
			counter++;
			flag = true;
			ChessPoints[x][y] = 0;//恢复一步
			ChessPoints[x1][y1] = 0;//再恢复一步
			return true;
		}
		ChessPoints[x][y] = 0;//恢复一步
		ChessPoints[x1][y1] = 0;//再恢复一步
	}
	return flag;
}

int CGoBangGame::countpoints()
{
	//计算棋盘上的子数
	int sum = 0;
	for (int i=1;i<=m_xGrid;i++)
		for (int j=1;j<=m_yGrid;j++)
		{
			if (ChessPoints[i][j]!=0)
			{
				sum++;
			}
		}
	return sum;
}

BOOL CGoBangGame::is_beatpoint(int color, int x, int y)
{
	//判断一个子是否是打击子
	if (ChessPoints[x][y]!=0) return false;
	if(!is_makealive(x,y,color)) return false;
	int anticolor;//对方的color
	if (color==1) anticolor = 2;
	else anticolor = 1;
			
	ChessPoints[x][y] = color;//假走一步
	isdanger = false;
	is_danger(anticolor);
	if (isdanger) 
	{
		ChessPoints[x][y] = 0;
		return true;
	}
	ChessPoints[x][y] = 0;
	return false;
}

BOOL CGoBangGame::is_defent_point(int color, int x, int y)
{
	//此函数判断是否是对方color的阻挡子
	if (ChessPoints[x][y]!=0) return false;//已经有子了
	int model[][11]={//此数组表示各种模式，包括此子前四个和后四个子的排列
		//-1表示任意，1表示color，0表示空，2表示(x,y)所在位置
		{-1,-1,-1,0,1,2,1,1,0,-1,-1},//_1*11_
		{-1,-1,-1,-1,0,2,1,1,1,0,-1},//_*111_	
		{-1,-1,-1,-1,-1,2,1,1,1,0,0},//*111__	
		{-1,-1,-1,-1,-1,2,1,1,1,1,-1},//*1111
		{-1,-1,-1,-1,1,2,1,1,1,-1,-1},//1*111
		{-1,-1,-1,1,1,2,1,1,-1,-1,-1},//11*11
	};

	int model_num = 6;
	int point_num = 11;
	BOOL flag;
	int i,j,k;
	for ( i=0;i<4;i++)
	{
		//四个方向遍历		
		for ( j=0;j<model_num;j++)//model_num种模式遍历，每种模式遍历两次，正反各一次
		{
			flag = true;
			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-5,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-5,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}
			if (flag) return true;//匹配上某种模式
			flag = true;
			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,5-k,y)!=0)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,5-k,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}
			if (flag) return true;//匹配上某种模式
		}
	}
	return false;
}

BOOL CGoBangGame::is_chongtwo(int color, int x, int y)
{
	//判断是否是冲二子，用于在赋值算法前冲二
	//即有两边以上是单子
	if (ChessPoints[x][y]!=0) return false;//已经有子了
	int model[][9]={//此数组表示各种模式，包括此子前四个和后四个子的排列
		//-1表示任意，1表示相同颜色，0表示非对方色，只能是同色或者0，2表示(x,y)所在位置
		{-1,-1,0,1,2,0,0,0,-1},//01*000 0f
		{-1,0,0,1,2,0,0,-1,-1},//001*00 1	
		{0,0,0,1,2,0,-1,-1,-1},//0001*0	2
		{0,0,1,0,2,0,-1,-1,-1},//001_*0	3
		{-1,0,1,0,2,0,0,-1,-1},//01_*00	4f
		{0,1,0,0,2,0,-1,-1,-1},//01__*0	4f
		{0,1,1,1,2,-1,-1,-1,-1},//_111*	5
		{1,0,1,1,2,-1,-1,-1,-1},//1_11*	6
		{1,1,0,1,2,-1,-1,-1,-1},//11_1*	7
		{1,1,1,0,2,-1,-1,-1,-1},//111_*	8
		{-1,1,1,1,2,0,-1,-1,-1},//111*_	9
		{-1,0,1,1,2,1,-1,-1,-1},//_11*1	10
		{-1,1,0,1,2,1,-1,-1,-1},//1_1*1	11
		{-1,1,1,0,2,1,-1,-1,-1},//11_*1	12
		{-1,-1,1,1,2,0,1,-1,-1},//11*_1	13
		{-1,-1,1,1,2,1,0,-1,-1},//11*1_	14
		{-1,0,1,1,2,0,0,-1,-1},//_11*__	15
		{0,1,0,1,2,0,-1,-1,-1},//_1_1*_	16
		{0,1,1,0,2,0,-1,-1,-1},//_11_*_	17
		{0,0,1,1,2,0,-1,-1,-1},//__11*_	18
		{-1,-1,0,1,2,1,0,0,-1},//_1*1__	19
		{-1,-1,0,1,2,0,1,0,-1},//_1*_1_	20
	};

	int model_num = 22;
	int point_num = 9;
	BOOL flag;
	int count = 0;
	int i,j,k;
	for ( i=0;i<4;i++)
	{
		//四个方向遍历		
		for ( j=0;j<model_num;j++)//model_num种模式遍历，每种模式遍历两次，正反各一次
		{
			flag = true;
			for( k=0;k<point_num;k++)//point_num个子遍历，正遍历
			{
				if (model[j][k]==0)
				{
					if ((getcolor(x,i,k-4,y)!=color)&&(getcolor(x,i,k-4,y)!=0))
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}
			if (flag) 
			{
				count++;
				break;
			}
			flag = true;
			for(k=0;k<point_num;k++)//point_num个子遍历，反遍历
			{
				if (model[j][k]==0)
				{
					if ((getcolor(x,i,4-k,y)!=color)&&(getcolor(x,i,4-k,y)!=0))
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//不匹配此模式
						flag = false;
						break;
					}
				}				
			}
			if (flag) 
			{
				count++;
				break;
			}

		}

	}

	if (count>=2) return true;

	return false;
}

void CGoBangGame::getclose_qianmodel(int color, int &counter, POINT tempresult[],BOOL qiang)
{
	//得到qianmodel的周围子
	counter = 0;
	
	POINT local_tempresult[8];
	POINT qianChessPoints[225];
	int qian_count;
	has_qianmodel(color,qian_count,qianChessPoints,qiang);

	merge1(tempresult,counter,qianChessPoints,qian_count,color);
	
	for (int i=0;i<qian_count;i++)
	{
		
		getroundpoints(qianChessPoints[i].x,qianChessPoints[i].y,color,local_tempresult);
		
		merge1(tempresult,counter,local_tempresult,8,color);								
			
	}	
	
}

void CGoBangGame::getclose_qianmodeladdone(int color, int &counter, POINT tempresult[],BOOL qiang)
{
	//得到qianmodel的周围子
	counter = 0;
	
	POINT local_tempresult[8];
	POINT qianChessPoints[225];
	int qian_count;
	has_qianmodel_addone(color,qian_count,qianChessPoints,qiang);

	merge1(tempresult,counter,qianChessPoints,qian_count,color);
	
	for (int i=0;i<qian_count;i++)
	{
		
		getroundpoints(qianChessPoints[i].x,qianChessPoints[i].y,color,local_tempresult);
		
		merge1(tempresult,counter,local_tempresult,8,color);								
		
	}	
	
}

void CGoBangGame::getclose_twolevel(int color, int &counter, POINT tempresult[])
{
	//得到qianmodel的周围子
	counter = 0;
	
	POINT local_tempresult[8];
	POINT qianChessPoints[225];
	int qian_count;
	has_twolevel(color,qian_count,qianChessPoints);

	merge1(tempresult,counter,qianChessPoints,qian_count,color);
	
	for (int i=0;i<qian_count;i++)
	{
		
		getroundpoints(qianChessPoints[i].x,qianChessPoints[i].y,color,local_tempresult);
		
		merge1(tempresult,counter,local_tempresult,8,color);								
		
	}	
	
}

void CGoBangGame::getclose_twolevel_four(int color, int &counter, POINT tempresult[])
{
	//得到qianmodel的周围子
	counter = 0;
	
	POINT local_tempresult[8];
	POINT qianChessPoints[225];
	int qian_count;
	has_twolevel_four(color,qian_count,qianChessPoints);

	merge1(tempresult,counter,qianChessPoints,qian_count,color);
	
	for (int i=0;i<qian_count;i++)
	{
		
		getroundpoints(qianChessPoints[i].x,qianChessPoints[i].y,color,local_tempresult);
		
		merge1(tempresult,counter,local_tempresult,8,color);								
		
	}	
	
}

void CGoBangGame::getclose_twolevelqianmodel_qiang(int color, int &counter, POINT tempresult[])
{
	//得到qianmodel的周围子
	counter = 0;
	
	POINT local_tempresult[8];
	POINT qianChessPoints[225];
	int qian_count;
	has_twolevelqianmodel_qiang(color,qian_count,qianChessPoints);

	merge1(tempresult,counter,qianChessPoints,qian_count,color);
	
	for (int i=0;i<qian_count;i++)
	{
		
		getroundpoints(qianChessPoints[i].x,qianChessPoints[i].y,color,local_tempresult);
		
		merge1(tempresult,counter,local_tempresult,8,color);								
		
	}	
	
}

void CGoBangGame::getclose_twolevelqianmodel(int color, int &counter, POINT tempresult[])
{
	//得到qianmodel的周围子
	counter = 0;
	
	POINT local_tempresult[8];
	POINT qianChessPoints[225];
	int qian_count;
	has_twolevel_qianmodel(color,qian_count,qianChessPoints);
	
	merge1(tempresult,counter,qianChessPoints,qian_count,color);
	
	for (int i=0;i<qian_count;i++)
	{
		
		getroundpoints(qianChessPoints[i].x,qianChessPoints[i].y,color,local_tempresult);
		
		merge1(tempresult,counter,local_tempresult,8,color);								
		
	}	
	
}

int CGoBangGame::getthinkpos()
{
	return thinkpos;
}

void CGoBangGame::setthinkpos(int pos)
{
	thinkpos = pos;
}

BOOL CGoBangGame::is_makealiveandtwolevel(int color,int x,int y)
{
	//是否是一个makealive和一个twolevel同时存在
	//而这个子就是那个makealive子
	if (!is_makealive(x,y,color)) return false;//这个子不是makealive子，一定不是
	if (!has_twolevel(color)) return false;//本来就没有twolevel，一定不是
	ChessPoints[x][y] = color;//走一步
	POINT alivepoint[225];
	int alivepoint_count;
	has_alive(color,alivepoint_count,alivepoint);
	POINT twolevelpoint[225];
	int twolevelpoint_count;
	has_twolevel(color,twolevelpoint_count,twolevelpoint);
	ChessPoints[x][y] = 0;
	int temp = alivepoint_count;
	sub(alivepoint,alivepoint_count,twolevelpoint,twolevelpoint_count);
	if (temp==alivepoint_count) return true;//说明没有一样的子
	return false;
}

BOOL CGoBangGame::has_chongtwoandmakeqianmodel(int color, int &counter, POINT tempresult[])
{
	//这个函数将chongtwo和makeqianmodel联合起来
	counter = 0;
	int count1;
	POINT tempresult1[225];
	has_chongtwo(color,counter,tempresult);
	make_qianmodel(color,count1,tempresult1);
	merge(tempresult,counter,tempresult1,count1);
	if (counter>0) return true;
	return false;
}

void CGoBangGame::cut_dangerpoints(int color, POINT tempresult[], int &n)
{
	//此函数用于grade=5时，主要用于判断是否走出某一步棋后，对方的阻挡子是打击子
	//如果是，那么去掉这个候选子
	//如果tempresult集合被肖光，那么用对方的打击子作为候选子
	POINT localtempresult[225];
	int tempn = 0;
	POINT defentpoint[225];
	int defent_count;
	int anticolor;//对方的color	
	if (color==1) anticolor = 2;	
	else anticolor = 1;
	int i;
	for ( i=0;i<n;i++)
	{
		BOOL tempflag = true;
		if (is_makealive(tempresult[i].x,tempresult[i].y,color)&&!is_canfour(color,tempresult[i].x,tempresult[i].y))
		{
			ChessPoints[tempresult[i].x][tempresult[i].y] = color;
			has_defent_point(color,defent_count,defentpoint);
			m_grade = 4;
			for (int j=0;j<defent_count;j++)
			{
				if (is_beatpoint(anticolor,defentpoint[j].x,defentpoint[j].y))
				{
					localtempresult[tempn].x = defentpoint[j].x;
					localtempresult[tempn].y = defentpoint[j].y;
					tempn++;
					tempflag = false;
					break;
				}
			}
			m_grade = 5;
			ChessPoints[tempresult[i].x][tempresult[i].y] = 0;
		}
		if (tempflag)
		{
			localtempresult[tempn].x = tempresult[i].x;
			localtempresult[tempn].y = tempresult[i].y;
			tempn++;
		}
	}	
	for (i=0;i<tempn;i++)
	{
		tempresult[i].x = localtempresult[i].x;
		tempresult[i].y = localtempresult[i].y;
	}
	n = tempn;	
}


