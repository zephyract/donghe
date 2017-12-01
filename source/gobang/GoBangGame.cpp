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


BOOL debugqian = false;//�Ƿ�쿴ǣ����ƥ�����ϸ����

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
	//color = 1��ʾ���ӣ�color = 2��ʾ����
	//�ж��Ƿ������5�������������	
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
					//����
					return true;
				}

				if ((i>=5)&&(j>=5)&&(ChessPoints[i-1][j-1]==color)&&(ChessPoints[i-2][j-2]==color)&&(ChessPoints[i-3][j-3]==color)&&(ChessPoints[i-4][j-4]==color))
				{
					//б���ϣ�б����
					return true;
				}

				if ((j>=5)&&(ChessPoints[i][j-1]==color)&&(ChessPoints[i][j-2]==color)&&(ChessPoints[i][j-3]==color)&&(ChessPoints[i][j-4]==color))
				{
					//�ϣ���
					return true;
				}

				if ((i>=5)&&(j<=11)&&(ChessPoints[i-1][j+1]==color)&&(ChessPoints[i-2][j+2]==color)&&(ChessPoints[i-3][j+3]==color)&&(ChessPoints[i-4][j+4]==color))
				{
					//б���£�б����
					return true;
				}
			}
		}
	return false;
	*/
}

BOOL CGoBangGame::is_alive(int x, int y,int color)
{
	if (ChessPoints[x][y]!=0) return false;//�Ѿ�������
	//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����
	//-1��ʾ���⣬1��ʾ��ͬ��ɫ��0��ʾ�գ�2��ʾ(x,y)����λ��
	int model[][9]={
		{-1,-1,0,1,2,1,1,0,-1},//_1*11_
		{-1,-1,-1,0,2,1,1,1,0},//_*111_	
	};

	int model_num = 2;
	int point_num = 9;
	BOOL flag;
	for (int i=0;i<4;i++)
	{
		//�ĸ��������		
		for (int j=0;j<model_num;j++)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��
		{
			flag = true;
			int k;
			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}

			if (flag) return true;//ƥ����ĳ��ģʽ
			flag = true;
			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}
			if (flag) return true;//ƥ����ĳ��ģʽ
		}
	}
	return false;
}

BOOL CGoBangGame::is_twoqianmodel(int color,int x, int y,POINT tempresult[],int n)
{
	//�˴��ж��Ƿ�������ǣ��������һ����ҪΪ�˷�ֹ�Է�����
	if (ChessPoints[x][y]!=0) return false;//�Ѿ�������
	int model[][9]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����
		//-1��ʾ���⣬1��ʾ��ͬ��ɫ��0��ʾ�գ�2��ʾ(x,y)����λ��
		//3��ʾ����ǣ���ӵ���ͬ��ɫ��
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
		//�ĸ��������		
		for (int j=0;j<model_num;j++)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��
		{
			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(color,x,y,i,k-4,tempresult,n)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(color,x,y,i,k-4,tempresult,n)!=1)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				

				else if (model[j][k]==3)
				{
					if (getcolor(color,x,y,i,k-4,tempresult,n)!=3)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}	
			}

			if (flag) return true;//ƥ����ĳ��ģʽ
			flag = true;
			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(color,x,y,i,4-k,tempresult,n)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(color,x,y,i,4-k,tempresult,n)!=1)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				

				else if (model[j][k]==3)
				{
					if (getcolor(color,x,y,i,4-k,tempresult,n)!=3)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}	
			}
			if (flag) return true;//ƥ����ĳ��ģʽ
		}
	}
	return false;
}

BOOL CGoBangGame::is_alive_old(int x, int y,int color)
{
	//����һ�����ݶ���ֱ�������Ϸ������Ƚ�������ȡ��������Ȼ����
	//�ж�һ�����ǲ���һ������
	//���Լ���һ�Ӻ󣬶Է�������ʲô�ӣ��������5������

	if (ChessPoints[x][y]!=0) return false;//�Ѿ������ˣ��������ǻ���
	int i;
	ChessPoints[x][y] = color;//�������˴���

	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
		else anticolor = 1;

	BOOL flag = true;//�����Ƿ�Է�ÿ������ʱ���Լ�����������5��

	POINT roundpoints[8];//һ������Χ8����������������Ŀ���λ��

	getroundpoints(x,y,color,roundpoints);//�˴���Ϊ�˱���ȫ�������������˽����ַ���ֻ��������Χ��8�����Ӽ���

	for(i=0;i<8;i++)
	{
		if (roundpoints[i].x!=0)//������Ч��λ��
		{
			ChessPoints[roundpoints[i].x][roundpoints[i].y] = anticolor;//�߶Է�����
			if (!can_five(color)) 
			{
				//�������һ�������������5�ӣ���ôʧ��
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
		return true;//��Ȼ�ǻ���
	}

	ChessPoints[x][y] = 0;
	return false;
}

BOOL CGoBangGame::can_five(int color,int &x,int &y)
{
	//�õ��ܹ��õ�5���ӵĵ�
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
				return true;//����һ�����Եõ�5��
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
				return true;//����һ�����Եõ�5��
			}
		}
	}
	return false;		
}

BOOL CGoBangGame::has_alive(int color,int &counter,POINT tempresult[])
{
	//�ж�һ��������û�л���
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����
	counter = 0;
	//if (can_five(color)) return false;//�Ѿ����ڿ�����5���ӣ��������ж�һ�׻���

	BOOL flag = false;	//����ֵ
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
	//�ж�һ��������û�г����
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����
	
	counter = 0;
	BOOL flag = false;	//����ֵ
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
	
	//�ж�һ��������û���赲��	
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����	
	counter = 0;	
	BOOL flag = false;	//����ֵ	
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
	//�ж�һ��������û������ǣ��������һ������
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����
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
	//�ж�һ��������û�л���
	//if (can_five(color)) return false;//�Ѿ����ڿ�����5���ӣ��������ж�һ�׻���
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
	//һ����6���㷨��ÿ���㷨16%����ÿ���㷨����thinkpos�����Գ�16
	//���return����ôһ����Ϊ100

	thinkpos = 0;
	POINT tempresult[225];
	int count=0;	
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	x=0;y=0;
	if (has_five(1)||has_five(2)) {
		thinkpos = 100;
		return;//�Ѿ�����5�ӣ�ʲô��������
	}
	thinkpos+=16;
	int cur_time = GetTickCount();
	if (can_five(color,x,y)) 
	{
		common_info(L"�������������5��!",debug,cur_time);
		thinkpos = 100;
		return;//���������5�ӣ���ô����
	}

	ShowInfo(L"can_five1:"+passtime(cur_time));
	thinkpos+=16;
	if (can_five(anticolor,x,y)) 
	{
		common_info(L"�˿�������5��!",debug,cur_time);
		thinkpos = 100;
		return;//���Է��ܷ�����5��
	}

	ShowInfo(L"can_five2:"+passtime(cur_time));
	thinkpos+=16;
	if (has_alive(color,count,tempresult))
	{
		//���ڻ��ӣ�ֱ��������ӣ�Ӧ�ӽ������randomѡȡ��
		int tempi = random(0,count-1);
		x = tempresult[tempi].x;
		y = tempresult[tempi].y;		
		common_info(L"������ҵ�һ�׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive:"+passtime(cur_time));
	thinkpos+=16;
	if (has_alive(anticolor,count,tempresult))
	{
		//���Է��Ƿ���ڻ���
		int tempi = random(0,count-1);
		x = tempresult[tempi].x;
		y = tempresult[tempi].y;
		common_info(L"�ҵ��˵�һ�׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive2:"+passtime(cur_time));
	thinkpos+=16;
	getclose_small(count,tempresult);
	int tempi = random(0,count-1);
	x = tempresult[tempi].x;
	y = tempresult[tempi].y;
	common_info(L"����㷨!",debug,cur_time);
	thinkpos = 100;
}

void CGoBangGame::getpoint2(int &x, int &y, int color,BOOL debug)
{
	//һ����10���㷨��ÿ���㷨10%����ÿ���㷨����thinkpos�����Գ�10
	//���return����ôһ����Ϊ100
	thinkpos = 0;

	//��grade3�Ļ����ϣ��ټ���һЩ���ܣ����磬��qianmodel�㷨ȫ��ȥ��

	//��������㷨

	POINT tempresult[225];

	int count=0;	

	int anticolor;//�Է���color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	x=0;y=0;

	if (has_five(1)||has_five(2)) 
	{
		thinkpos = 100;
		return;//�Ѿ�����5�ӣ�ʲô��������
	}
	thinkpos+=10;

	int cur_time = GetTickCount();

	if (can_five(color,x,y)) 

	{

		common_info(L"�������������5��!",debug,cur_time);
		thinkpos = 100;
		return;//���������5�ӣ���ô����

	}

	ShowInfo(L"can_five1:"+passtime(cur_time));
	thinkpos+=10;
	if (can_five(anticolor,x,y)) 
	{
		common_info(L"�˿�������5��!",debug,cur_time);
		thinkpos = 100;
		return;//���Է��ܷ�����5��
	}

	ShowInfo(L"can_five2:"+passtime(cur_time));
	thinkpos+=10;
	if (has_alive(color,count,tempresult))

	{

		//���ڻ��ӣ�ֱ��������ӣ�Ӧ�ӽ������randomѡȡ��

		int tempi = random(0,count-1);
		
		x = tempresult[tempi].x;
		
		y = tempresult[tempi].y;

		common_info(L"������ҵ�һ�׻���!",debug,cur_time);
		thinkpos = 100;
		return;

	}

	ShowInfo(L"has_alive:"+passtime(cur_time));
	thinkpos+=10;
	if (has_twolevel_four(color,count,tempresult))
	{
		//����һ���������ӵĶ��׻���
		int tempi = random(0,count-1);
		x = tempresult[tempi].x;		
		y = tempresult[tempi].y;
		common_info(L"������ҵ�ǿ���׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four:"+passtime(cur_time));
	thinkpos+=10;
	//1_____________________________________

	if (has_alive(anticolor,count,tempresult))
	{
		//���Է��Ƿ���ڻ���
		int tempi = random(0,count-1);		
		x = tempresult[tempi].x;		
		y = tempresult[tempi].y;
		common_info(L"�ҵ��˵�һ�׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive:"+passtime(cur_time));	
	thinkpos+=10;
	//2_____________________________________

	if (has_twolevel_four(anticolor,count,tempresult))
	{
		//����һ���������ӵĶ��׻���
		int tempi = random(0,count-1);		
		x = tempresult[tempi].x;		
		y = tempresult[tempi].y;
		common_info(L"�ҵ��˵�ǿ���׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four:"+passtime(cur_time));	
	thinkpos+=10;
	//____________________________________________

	//������һ�׻��ӣ������Ƿ���ڶ��׻���

	if (has_twolevel_pure(color,count,tempresult))
	{
		//���ڶ��׻��ӣ�ֱ��������׻��ӣ�Ӧ�ӽ������randomѡȡ��
		int tempi = random(0,count-1);		
		x = tempresult[tempi].x;		
		y = tempresult[tempi].y;
		common_info(L"������ҵ����׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel:"+passtime(cur_time));
	thinkpos+=10;
	//3___________________________

	if (has_twolevel(anticolor,count,tempresult))
	{
		//���Է��Ƿ���ڶ��׻���
		int tempi = random(0,count-1);	
		x = tempresult[tempi].x;	
		y = tempresult[tempi].y;
		common_info(L"�ҵ��˵Ķ��׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}	

	ShowInfo(L"has_twolevel:"+passtime(cur_time));	
	thinkpos+=10;
	getclose_small(count,tempresult);

	int tempi = random(0,count-1);

	x = tempresult[tempi].x;

	y = tempresult[tempi].y;

	common_info(L"����㷨!",debug,cur_time);
	thinkpos = 100;

}

void CGoBangGame::getpoint3(int &x, int &y, int color,BOOL debug)
{

	//һ����12���㷨��ÿ���㷨8%����ÿ���㷨����thinkpos�����Գ�8
	//���return����ôһ����Ϊ100
	
	thinkpos = 0;
	//��Ϊ��ʱ�򻹲��ܿ���������ǣ���ӣ��������������ǣ������صĺ�����ȥ����

	//Ҳ��ʹ��make��defent�㷨

	//ͬʱֻ�ܱ��ǣ����ƥ�䣬��������в���ǣ����ƥ�䣬��ô�Ϳ�ʤ��

	POINT tempresult[225];

	int count=0;	

	int anticolor;//�Է���color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	x=0;y=0;

	if (has_five(1)||has_five(2)) 
	{
		thinkpos = 100;
		return;//�Ѿ�����5�ӣ�ʲô��������
	}
	thinkpos += 8;
	int cur_time = GetTickCount();

	if (can_five(color,x,y)) 
	{
		common_info(L"�������������5��!",debug,cur_time);
		thinkpos = 100;
		return;//���������5�ӣ���ô����
	}

	ShowInfo(L"can_five1:"+passtime(cur_time));
	thinkpos += 8;
	if (can_five(anticolor,x,y)) 
	{
		common_info(L"�˿�������5��!",debug,cur_time);
		thinkpos = 100;
		return;//���Է��ܷ�����5��
	}

	ShowInfo(L"can_five2:"+passtime(cur_time));
	thinkpos += 8;
	if (has_alive(color,count,tempresult))
	{
		//���ڻ��ӣ�ֱ��������ӣ�Ӧ�ӽ������randomѡȡ��
		selectqianmodel3(color,x,y,tempresult,count);		
		common_info(L"������ҵ�һ�׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive:"+passtime(cur_time));
	thinkpos += 8;
	if (has_twolevel_four(color,count,tempresult))
	{
		//����һ���������ӵĶ��׻���
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"������ҵ�ǿ���׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four:"+passtime(cur_time));
	thinkpos += 8;
	//1_____________________________________

	if (has_alive(anticolor,count,tempresult))
	{
		//���Է��Ƿ���ڻ���
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"�ҵ��˵�һ�׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive:"+passtime(cur_time));	
	thinkpos += 8;
	//2_____________________________________

	if (has_twolevel_four(anticolor,count,tempresult))
	{
		//����һ���������ӵĶ��׻���
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"�ҵ��˵�ǿ���׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four:"+passtime(cur_time));	
	thinkpos += 8;
	//____________________________________________

	//������һ�׻��ӣ������Ƿ���ڶ��׻���

	if (has_twolevel_pure(color,count,tempresult))
	{
		//���ڶ��׻��ӣ�ֱ��������׻��ӣ�Ӧ�ӽ������randomѡȡ��
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"������ҵ����׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel:"+passtime(cur_time));
	thinkpos += 8;
	if (has_qianmodel(color,count,tempresult))
	{
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"���������ǣ����ƥ��!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel:"+passtime(cur_time));
	thinkpos += 8;
	//3___________________________

	if (has_twolevel(anticolor,count,tempresult))
	{
		//���Է��Ƿ���ڶ��׻���
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"�ҵ��˵Ķ��׻���!",debug,cur_time);
		thinkpos = 100;
		return;	
	}	

	ShowInfo(L"has_twolevel:"+passtime(cur_time));	
	thinkpos += 8;
	//4_________________________________

	if (has_qianmodel(anticolor,count,tempresult))
	{
		selectqianmodel3(color,x,y,tempresult,count);
		common_info(L"������ǣ����ƥ��!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel2:"+passtime(cur_time));
	thinkpos += 8;
	getmax(count,tempresult);

	int tempi = random(0,count-1);
	x = tempresult[tempi].x;
	y = tempresult[tempi].y;

	common_info(L"��ֵ�㷨!",debug,cur_time);
	thinkpos = 100;
}

void CGoBangGame::getpoint4(int &x, int &y, int color,BOOL debug)
{
	//һ����26���㷨��ÿ���㷨4%����ÿ���㷨����thinkpos�����Գ�4
	//���return����ôһ����Ϊ100
	
	thinkpos = 0;
	POINT tempresult[225];
	int count=0;	
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	x=0;y=0;
	if (has_five(1)||has_five(2)) 
	{
		thinkpos = 100;
		return;//�Ѿ�����5�ӣ�ʲô��������
	}
	thinkpos += 4;
	int cur_time = GetTickCount();

	if (can_five(color,x,y)) 
	{
		common_info(L"�������������5��!",debug,cur_time);
		thinkpos = 100;
		return;//���������5�ӣ���ô����
	}

	ShowInfo(L"can_five1:"+passtime(cur_time));
	thinkpos += 4;
	if (can_five(anticolor,x,y)) 
	{
		common_info(L"�˿�������5��!",debug,cur_time);
		thinkpos = 100;
		return;//���Է��ܷ�����5��
	}

	ShowInfo(L"can_five2:"+passtime(cur_time));
	thinkpos += 4;
	if (has_alive(color,count,tempresult))
	{
		//���ڻ��ӣ�ֱ��������ӣ�Ӧ�ӽ������randomѡȡ��
		selectqianmodel(color,x,y,tempresult,count);		
		common_info(L"������ҵ�һ�׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive:"+passtime(cur_time));
	thinkpos += 4;
	if (has_twolevel_four(color,count,tempresult))
	{
		//����һ���������ӵĶ��׻���
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"������ҵ�ǿ���׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four:"+passtime(cur_time));
	thinkpos += 4;
	//1_____________________________________
	if (has_alive_defent(anticolor,count,tempresult))
	{
		//���Է��Ƿ���ڻ���
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"�ҵ��˵�һ�׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive_defent:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//���ܷ�ס�Է���һ�׻���,����
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"���������!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_alive_defent_low(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"�ҵ��˵�һ�׻���low!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}	

	ShowInfo(L"has_alive_defent_low2:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_alive(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"�ҵ��˵�һ�׻���2!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive2:"+passtime(cur_time));
	thinkpos += 4;
	//2_____________________________________
	if (has_twolevel_four_defent(anticolor,count,tempresult))
	{
		//����һ���������ӵĶ��׻���
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"�ҵ��˵�ǿ���׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four_defent:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//���ܷ�ס�Է���ǿ���׻���,����
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"���������!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_twolevel_four_defent_low(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"�ҵ��˵�ǿ���׻���low!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four_defent_low2:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_twolevel_four(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"�ҵ��˵�ǿ���׻���2!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four2:"+passtime(cur_time));
	thinkpos += 4;
	//____________________________________________
	//������һ�׻��ӣ������Ƿ���ڶ��׻���
	if (has_twolevel_pure(color,count,tempresult))
	{
		//���ڶ��׻��ӣ�ֱ��������׻��ӣ�Ӧ�ӽ������randomѡȡ��
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"������ҵ����׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel:"+passtime(cur_time));
	thinkpos += 4;
	if (has_qianmodel_addone(color,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"���������ǣ���Ӻ�һ��������ƥ��!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel_addone:"+passtime(cur_time));
	thinkpos += 4;
	if (has_qianmodel(color,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"���������ǣ����ƥ��!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel:"+passtime(cur_time));
	thinkpos += 4;
	if (has_twolevel_qianmodel(color,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"���������ǣ����!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_qianmodel:"+passtime(cur_time));	
	thinkpos += 4;
	//3___________________________

	if (has_twolevel_defent(anticolor,count,tempresult))
	{
		//���Է��Ƿ���ڶ��׻���
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"�ҵ��˵Ķ��׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}	

	ShowInfo(L"has_twolevel_defent:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//���ܷ�ס�Է��Ķ��׻���,����
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"���������!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_twolevel_defent_low(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"�ҵ��˵Ķ��׻���low!",debug,cur_time);
		isdanger = true;
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_defent_low2:"+passtime(cur_time));
	thinkpos += 4;
	if (!defent_success&&has_twolevel(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"�ҵ��˵Ķ��׻���2!",debug,cur_time);
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
		common_info(L"������ǣ���Ӻ�һ��������ƥ��!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel_addone2:"+passtime(cur_time));	
	thinkpos += 4;
	//5_______________________________________
	if (has_qianmodel(anticolor,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"������ǣ����ƥ��!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel2:"+passtime(cur_time));
	thinkpos += 4;
	//6________________________________________
	if (has_twolevel_qianmodel(anticolor,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"�˶���ǣ����!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_qianmodel2:"+passtime(cur_time));
	thinkpos += 4;
	if (make_qianmodel(color,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"���������ǣ����ƥ��!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"make_qianmodel:"+passtime(cur_time));
	thinkpos += 4;
	//Ҳ�����ڶ��׻��ӣ����ܷ��������
	/*
	if (make_twolevel(color,count,tempresult))
	{
		selectqianmodel(color,x,y,tempresult,count);
		common_info(L"�����������׻���!",debug,cur_time);
		return;
	}
	ShowInfo(L"make_twolevel:"+passtime(cur_time));
	*/

	getmax(count,tempresult);

	int tempi = random(0,count-1);
	x = tempresult[tempi].x;
	y = tempresult[tempi].y;
	common_info(L"��ֵ�㷨!",debug,cur_time);
	thinkpos = 100;
}

void CGoBangGame::getpoint5(int &x, int &y, int color,BOOL debug)
{
	//һ����53���㷨��ÿ���㷨2%����ÿ���㷨����thinkpos�����Գ�2
	//���return����ôһ����Ϊ100
	thinkpos = 0;
	//��ߵȼ�

	/*�㷨��������

  ������ҵ�һ�׻���
  ������ҵ�ǿ���׻���
  ���������ǣ���Ӻ�һ��������ǿƥ��!
  ���������ǣ����ǿƥ��
  ���������ǣ����ǿƥ��
  ���������ǣ����ǿƥ��
  //**�����ǿ�����
  �ҵ��˵�һ�׻���d
  �ҵ��˵�ǿ���׻���d
  ������ǣ���Ӻ�һ��������ǿƥ��d
  ������ǣ����ǿƥ��d
  �˶���ǣ����ǿƥ��d
  //������ǣ����ǿƥ��d 
  ������ҵ����׻���
  ���������ǣ���Ӻ�һ��������ƥ��
  ���������ǣ����ƥ��
  ���������ǣ����
  ���������ǣ����  
  **����������
  //**��������״����>50������
  �ҵ��˵Ķ��׻���d
  ���������(color=2ʱ��<20������)
  ���������(color=2ʱ��<20������)
  ������ǣ���Ӻ�һ��������ƥ��d
  ������ǣ����ƥ��d
  �˶���ǣ����d
  //������ǣ����>50ʱ������
  //**��ǿ�����>50ʱ������
  //**����������_low
  //**��������״����_low>50������
  **�˴����>50ʱ������
  //**�˶��״����>50ʱ������  
  //�������������
  //������������ǣ����ƥ��
  //�������������ǣ����ƥ��  
  //�����������׻���
  //���������Ӻ�����ǣ����ƥ��(color=2ʱ��<10������)  
  ����������(color=2ʱ��<10������)
  ���������ǣ����ƥ��(color=2ʱ��<10������)
    
  ��ֵ�㷨
	*/

	POINT tempresult[225];
	int count=0;	
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	x=0;y=0;

	if (has_five(1)||has_five(2)) 
	{
		thinkpos = 100;
		return;//�Ѿ�����5�ӣ�ʲô��������
	}
	thinkpos += 2;
	int cur_time = GetTickCount();

	if (can_five(color,x,y)) 
	{
		common_info(L"�������������5��!",debug,cur_time);
		thinkpos = 100;
		return;//���������5�ӣ���ô����
	}

	ShowInfo(L"can_five1:"+passtime(cur_time));
	thinkpos += 2;
	if (can_five(anticolor,x,y)) 
	{
		common_info(L"�˿�������5��!",debug,cur_time);
		thinkpos = 100;
		return;//���Է��ܷ�����5��
	}

	ShowInfo(L"can_five2:"+passtime(cur_time));
	thinkpos += 2;
	if (has_alive(color,count,tempresult))
	{

		//���ڻ��ӣ�ֱ��������ӣ�Ӧ�ӽ������randomѡȡ��
		selectqianmodel5(color,x,y,tempresult,count);		
		common_info(L"������ҵ�һ�׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive:"+passtime(cur_time));
	thinkpos += 2;
	if (has_twolevel_four(color,count,tempresult))
	{
		//����һ���������ӵĶ��׻���
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"������ҵ�ǿ���׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four:"+passtime(cur_time));
	thinkpos += 2;
	if (has_qianmodel_addone(color,count,tempresult,true))	
	{	
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"���������ǣ���Ӻ�һ��������ǿƥ��!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_qianmodel_addone_qiang:"+passtime(cur_time));
	thinkpos += 2;
	if (has_qianmodel(color,count,tempresult,true))	
	{		
		selectqianmodel5(color,x,y,tempresult,count);		
		common_info(L"���������ǣ����ǿƥ��!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_qiang:"+passtime(cur_time));	
	thinkpos += 2;
	if (has_twolevelqianmodel_qiang(color,count,tempresult))	
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"���������ǣ����ǿƥ��!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_twolevelqianmodel_qiang:"+passtime(cur_time));	
	thinkpos += 2;
	if (has_threelevelqianmodel_qiang(color,count,tempresult))	
	{	
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"���������ǣ����ǿƥ��!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_threelevelqianmodel_qiang:"+passtime(cur_time));	
	thinkpos += 2;
	/*

	if (has_qiang_beat(color,count,tempresult))
	{		
		selectqianmodel5(color,x,y,tempresult,count);		
		common_info(L"�����ǿ�����!",debug,cur_time);	
		return;	
	}
	
	ShowInfo(L"has_qiang_beat:"+passtime(cur_time));
	*/

	//1_____________________________________

	if (has_alive_defent(anticolor,count,tempresult))
	{
		//���Է��Ƿ���ڻ���
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�ҵ��˵�һ�׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive_defent:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{

		//���ܷ�ס�Է���һ�׻���,����
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_alive_defent_low(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�ҵ��˵�һ�׻���low!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_alive_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_alive(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�ҵ��˵�һ�׻���2!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_alive2:"+passtime(cur_time));
	thinkpos += 2;
	//2_____________________________________
	if (has_twolevel_four_defent(anticolor,count,tempresult))	
	{
		//����һ���������ӵĶ��׻���
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�ҵ��˵�ǿ���׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_four_defent:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//���ܷ�ס�Է���ǿ���׻���,����
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevel_four_defent_low(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�ҵ��˵�ǿ���׻���low!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_twolevel_four_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevel_four(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�ҵ��˵�ǿ���׻���2!",debug,cur_time);
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
		common_info(L"������ǣ���Ӻ�һ��������ǿƥ��!",debug,cur_time);
		thinkpos = 100;
		return;		
	}
	
	ShowInfo(L"has_qianmodel_addone_defent_qiang:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))	
	{	
		//���ܷ�ס�Է�,����	
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel_addone_defent_low(anticolor,count,tempresult,true))	
	{	
		//���ǲ��ܷ�ס	
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"������ǣ���Ӻ�һ��������ǿƥ��low!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_addone_defent_qiang_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel_addone(anticolor,count,tempresult,true))	
	{	
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���	
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"������ǣ���Ӻ�һ��������ǿƥ��2!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_qianmodel_addone_qiang2:"+passtime(cur_time));
	thinkpos += 2;
	//8_______________________________________
	
	if (has_qianmodel_defent(anticolor,count,tempresult,true))	
	{	
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"������ǣ����ǿƥ��!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_defent_qiang:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{	
		//���ܷ�ס�Է�,����	
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel_defent_low(anticolor,count,tempresult,true))	
	{	
		//���ǲ��ܷ�ס
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"������ǣ����ǿƥ��low!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_qiang_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel(anticolor,count,tempresult,true))	
	{	
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���	
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"������ǣ����ǿƥ��2!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_qiang2:"+passtime(cur_time));
	thinkpos += 2;
	//9___________________________

	if (has_twolevelqianmodel_qiang_defent(anticolor,count,tempresult))	
	{		
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"�˶���ǣ����ǿƥ��!",debug,cur_time);
		thinkpos = 100;
		return;		
	}
	
	ShowInfo(L"has_twolevelqianmodel_qiang_defent:"+passtime(cur_time));	
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))		
	{
		//���ܷ�ס�Է�,����
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevelqianmodel_qiang_defent_low(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�˶���ǣ����ǿƥ��low!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_twolevelqianmodel_qiang_defent_low:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevelqianmodel_qiang(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�˶���ǣ����ǿƥ��2!",debug,cur_time);
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
		common_info(L"������ǣ����ǿƥ��!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	ShowInfo(L"has_threelevelqianmodel_qiang_defent:"+passtime(cur_time));
	thinkpos += 2;

	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//���ܷ�ס�Է�,����
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_threelevelqianmodel_qiang_defent_low(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"������ǣ����ǿƥ��low!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_threelevelqianmodel_qiang_defent_low:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_threelevelqianmodel_qiang(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"������ǣ����ǿƥ��2!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_threelevelqianmodel_qiang2:"+passtime(cur_time));
	thinkpos += 2;
*/
	//____________________________________-
	//������һ�׻��ӣ������Ƿ���ڶ��׻���

	if (has_twolevel_pure(color,count,tempresult))
	{
		//���ڶ��׻��ӣ�ֱ��������׻��ӣ�Ӧ�ӽ������randomѡȡ��
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"������ҵ����׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel:"+passtime(cur_time));		
	thinkpos += 2;
	if (has_qianmodel_addone(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"���������ǣ���Ӻ�һ��������ƥ��!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel_addone:"+passtime(cur_time));
	thinkpos += 2;
	if (has_qianmodel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"���������ǣ����ƥ��!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel:"+passtime(cur_time));	
	thinkpos += 2;
	if (has_twolevel_qianmodel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"���������ǣ����!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_qianmodel:"+passtime(cur_time));	
	thinkpos += 2;
	if (has_threelevel_qianmodel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"���������ǣ����!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_threelevel_qianmodel:"+passtime(cur_time));
	thinkpos += 2;
	if (has_beatpoints(color,count,tempresult))	
	{	
		selectqianmodel5(color,x,y,tempresult,count);	
		common_info(L"����������!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_beatpoints1:"+passtime(cur_time));
	thinkpos += 2;
	/*
	if ((countpoints()<50)&&has_twolevel_beat(color,count,tempresult))	
	{	
		selectqianmodel5(color,x,y,tempresult,count);	
		common_info(L"��������״����!",debug,cur_time);		
		return;	
	}
	ShowInfo(L"has_twolevel_beat1:"+passtime(cur_time));
	*/
	//3___________________________

	if (has_twolevel_defent(anticolor,count,tempresult))
	{
		//���Է��Ƿ���ڶ��׻���
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�ҵ��˵Ķ��׻���!",debug,cur_time);
		thinkpos = 100;
		return;
	}	

	ShowInfo(L"has_twolevel_defent:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//���ܷ�ס�Է��Ķ��׻���,����
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&make_alive(color,count,tempresult))	
	{	
		//���ܷ�ס�Է��Ķ��׻���,����		
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;
		
	}
	
	ShowInfo(L"make_alive:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevel_defent_low(anticolor,count,tempresult))
	{
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�ҵ��˵Ķ��׻���low!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_twolevel_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevel(anticolor,count,tempresult))
	{

		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�ҵ��˵Ķ��׻���2!",debug,cur_time);
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
		
		common_info(L"���������(��)!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"make_alive(good):"+passtime(cur_time));
	thinkpos += 2;
	
	if (((color==1)||(countpoints()>=20))&&has_canfour(color,count,tempresult))	
	{		
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"���������(��)!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	ShowInfo(L"has_canfour(good):"+passtime(cur_time));
	thinkpos += 2;
	
	//4_________________________________
	if (has_qianmodel_addone_defent(anticolor,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"������ǣ���Ӻ�һ��������ƥ��!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel_addone_defent:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//���ܷ�ס�Է�,����
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&make_alive(color,count,tempresult))	
	{	
		//���ܷ�ס�Է��Ķ��׻���,����	
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"make_alive:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel_addone_defent_low(anticolor,count,tempresult))	
	{	
		//���ǲ��ܷ�ס	
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"������ǣ���Ӻ�һ��������ƥ��low!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_addone_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel_addone(anticolor,count,tempresult))		
	{	
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���		
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"������ǣ���Ӻ�һ��������ƥ��2!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_addone2:"+passtime(cur_time));
	thinkpos += 2;
	/*
	if (has_qianmodel_addone(anticolor,count,tempresult))		
	{		
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���
				
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"������ǣ���Ӻ�һ��������ƥ��!",debug,cur_time);	
		return;	
	}	
	ShowInfo(L"has_qianmodel_addone2:"+passtime(cur_time));
	*/
	//5_______________________________________

	if (has_qianmodel_defent(anticolor,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"������ǣ����ƥ��!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_qianmodel_defent:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))
	{
		//���ܷ�ס�Է�,����
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&make_alive(color,count,tempresult))		
	{
		//���ܷ�ס�Է��Ķ��׻���,����
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"make_alive:"+passtime(cur_time));
	thinkpos += 2;

	if (!defent_success&&has_qianmodel_defent_low(anticolor,count,tempresult))	
	{	
		//���ǲ��ܷ�ס	
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"������ǣ����ƥ��low!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_qianmodel(anticolor,count,tempresult))	
	{	
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���		
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"������ǣ����ƥ��2!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_qianmodel2:"+passtime(cur_time));
	thinkpos += 2;
	/*
	if (has_qianmodel(anticolor,count,tempresult))	
	{	
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���		
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"������ǣ����ƥ��!",debug,cur_time);		
		return;	
	}
	
	ShowInfo(L"has_qianmodel2:"+passtime(cur_time));
	*/

	//6________________________________________	

	if (has_twolevel_qianmodel_defent(anticolor,count,tempresult))	
	{	
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"�˶���ǣ����!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	
	ShowInfo(L"has_twolevel_qianmodel_defent:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_canfour(color,count,tempresult))	
	{
		//���ܷ�ס�Է�,����
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_canfour:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&make_alive(color,count,tempresult))	
	{
		//���ܷ�ס�Է��Ķ��׻���,����
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"���������!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	ShowInfo(L"make_alive:"+passtime(cur_time));
	thinkpos += 2;
	
	if (!defent_success&&has_twolevel_qianmodel_defent_low(anticolor,count,tempresult))	
	{	
		//���ǲ��ܷ�ס
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�˶���ǣ����low!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	
	ShowInfo(L"has_twolevel_qianmodel_defent_low2:"+passtime(cur_time));
	thinkpos += 2;
	if (!defent_success&&has_twolevel_qianmodel(anticolor,count,tempresult))		
	{		
		//���ǲ��ܷ�ס����ôֻ��ĳһ�����Ӽ���				
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�˶���ǣ����2!",debug,cur_time);
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
		common_info(L"������ǣ����!",debug,cur_time);	
		return;		
	}	
	ShowInfo(L"has_threelevel_qianmodel2:"+passtime(cur_time));	
	*/
	/*
	if ((countpoints()<50)&&has_qiang_beat(anticolor,count,tempresult))		
	{		
		selectqianmodel5(color,x,y,tempresult,count,true);	
		common_info(L"��ǿ�����!",debug,cur_time);	
		return;	
	}
	ShowInfo(L"has_qiang_beat2:"+passtime(cur_time));
	*/
	/*
	if (has_beatpoints_low_low(color,count,tempresult))		
	{		
		selectqianmodel5(color,x,y,tempresult,count);	
		common_info(L"����������low!",debug,cur_time);		
		return;	
	}
	
	ShowInfo(L"has_beatpoints_low_low1:"+passtime(cur_time));
	/*
	if ((countpoints()<50)&&has_twolevel_beat_low(color,count,tempresult))		
	{		
		selectqianmodel5(color,x,y,tempresult,count);		
		common_info(L"��������״����low!",debug,cur_time);	
		return;		
	}
	
	ShowInfo(L"has_twolevel_beat_low1:"+passtime(cur_time));
	*/

	if ((countpoints()<50)&&has_beatpoints_low(anticolor,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count,true);
		common_info(L"�˴����!",debug,cur_time);
		thinkpos = 100;
		return;
	}

	ShowInfo(L"has_beatpoints_low2:"+passtime(cur_time));
	thinkpos += 2;
	/*
	if ((countpoints()<50)&&has_twolevel_beat(anticolor,count,tempresult))	
	{
		selectqianmodel5(color,x,y,tempresult,count,true);		
		common_info(L"�˶��״����!",debug,cur_time);		
		return;	
	}
	ShowInfo(L"has_twolevel_beat2:"+passtime(cur_time));	
	*/	
	
	/*
	if (make_beatChessPoints(color,count,tempresult))	
	{	
		selectqianmodel5(color,x,y,tempresult,count);		
		common_info(L"�������������!",debug,cur_time);		
		return;		
	}	
	ShowInfo(L"make_beatChessPoints:"+passtime(cur_time));
	/*
	if (make_twolevel_qianmodel(color,count,tempresult))		
	{		
		selectqianmodel5(color,x,y,tempresult,count);		
		common_info(L"������������ǣ����ƥ��!",debug,cur_time);	
		return;	
	}
	ShowInfo(L"make_twolevel_qianmodel:"+passtime(cur_time));
	/*
	if (make_threelevel_qianmodel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"�������������ǣ����ƥ��!",debug,cur_time);
		return;
	}
	ShowInfo(L"make_threelevel_qianmodel:"+passtime(cur_time));	
	*/
	//Ҳ�����ڶ��׻��ӣ����ܷ��������
	/*
	if (make_twolevel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"�����������׻���!",debug,cur_time);
		return;
	}
	ShowInfo(L"make_twolevel:"+passtime(cur_time));
	/*
	if (make_alive(color,count,tempresult))
	{
		selectmax(color,x,y,tempresult,count);
		common_info(L"���������һ�׻���!",debug);
		return;
	}	
	if (make_alive(anticolor,count,tempresult))
	{
		selectmax(color,x,y,tempresult,count,true);
		common_info(L"�����˵�һ�׻���!",debug);
		return;
	}
	//���ǰ�ú������Ѿ�ʧ�ܣ����ø�ֵ�㷨
	getclose(color,count,tempresult);
	selectqianmodel5(color,x,y,tempresult,count);
	*/
	//getmax(count,tempresult);
	//getclose(color,count,tempresult);
/*
	if (((color==1)||(countpoints()>=10))&&has_chongtwoandmakeqianmodel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);	
		common_info(L"���������Ӻ�����ǣ����ƥ��!",debug,cur_time);
		thinkpos = 100;
		return;	
	}
	ShowInfo(L"has_chongtwoandmakeqianmodel:"+passtime(cur_time));
	thinkpos += 2;
*/
	
	if (((color==1)||(countpoints()>=10))&&has_chongtwo(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		
		common_info(L"����������!",debug,cur_time);
		thinkpos = 100;
		return;
	}
	ShowInfo(L"has_chongtwo:"+passtime(cur_time));
	thinkpos += 2;
	if (((color==1)||(countpoints()>=10))&&make_qianmodel(color,count,tempresult))
	{
		selectqianmodel5(color,x,y,tempresult,count);
		common_info(L"���������ǣ����ƥ��!",debug,cur_time);
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
	common_info(L"��ֵ�㷨!",debug,cur_time);	
	thinkpos = 100;

}

BOOL CGoBangGame::twolevel_alive(int x, int y, int color)
{
	if (ChessPoints[x][y]!=0) return false;//�Ѿ�������
	int model[][9]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����
		//-1��ʾ���⣬1��ʾ��ͬ��ɫ��0��ʾ�գ�2��ʾ(x,y)����λ��
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
	};//17�ֿ�����ɻ��ӵ�����

	int model_num = 17;
	int point_num = 9;
	BOOL flag;
	int count = 0;//�ۼ��ж��ٸ����������ģʽ��ƥ��
	for (int i=0;i<4;i++)
	{
		//�ĸ��������		
		for (int j=0;j<model_num;j++)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��
		{
			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}

			if (flag) 
			{
				//ƥ����ĳ��ģʽ
				count++;
				break;//�˷���Ͳ������ۼ���
			}

			flag = true;

			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}

			if (flag) 
			{
				//ƥ����ĳ��ģʽ
				count++;
				break;//�˷���Ͳ������ۼ���
			}
		}
	}
	if (count>=2) return true;
	return false;

}

BOOL CGoBangGame::twolevel_alive_pure(int x, int y, int color)
{
	//��twolevel_alive��ͬ���ǣ�pure������ǿ���׻���
	if (ChessPoints[x][y]!=0) return false;//�Ѿ�������
	int model[][9]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����
		//-1��ʾ���⣬1��ʾ��ͬ��ɫ��0��ʾ�գ�2��ʾ(x,y)����λ��
		{0,0,1,1,2,0,-1,-1,-1},//__11*_
		{0,1,0,1,2,0,-1,-1,-1},//_1_1*_
		{0,1,1,0,2,0,-1,-1,-1},//_11_*_
		{-1,0,1,1,2,0,0,-1,-1},//_11*__

		{-1,0,0,1,2,1,0,-1,-1},//__1*1_
		{-1,0,1,0,2,1,0,-1,-1},//_1_*1_
		{-1,-1,0,1,2,0,1,0,-1},//_1*_1_
	};//17�ֿ�����ɻ��ӵ�����

	int model_num = 7;
	int point_num = 9;
	BOOL flag;
	int count = 0;//�ۼ��ж��ٸ����������ģʽ��ƥ��
	for (int i=0;i<4;i++)
	{
		//�ĸ��������		
		for (int j=0;j<model_num;j++)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��
		{
			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}
			if (flag) 
			{
				//ƥ����ĳ��ģʽ
				count++;
				break;//�˷���Ͳ������ۼ���
			}

			flag = true;
			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}
			if (flag) 
			{
				//ƥ����ĳ��ģʽ
				count++;
				break;//�˷���Ͳ������ۼ���
			}
		}
	}
	if (count>=2) return true;
	return false;
}


BOOL CGoBangGame::twolevel_alive_old(int x, int y, int color)
{
	//�����ö���ֱ�������жϣ��Ƚ�������˱�����������Ȼ���ڴ˴���Ϊʾ��
	//�������޶��׻���
	//���Լ���һ�Ӻ󣬶Է�������ʲô�ӣ�������ڻ���	
	if (ChessPoints[x][y]!=0) return false;//�Ѿ������ˣ��������Ƕ��׻���
	if (can_five(color)) return false;//����Ѿ���������5�ӣ���ôȥ��
	if (is_alive(x,y,color)) return false;//�����һ�׻��ӣ���ôȥ��
	int i;

	ChessPoints[x][y] = color;//�������˴���
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	BOOL flag = true;//�����Ƿ�Է�ÿ������ʱ���Լ������Դ��ڻ���
	POINT roundpoints[8];//һ������Χ8����������������Ŀ���λ��

	getroundpoints(x,y,color,roundpoints);//�˴���Ϊ�˱���ȫ�������������˽����ַ���ֻ��������Χ��8�����Ӽ���
	for(i=0;i<8;i++)
	{
		if (roundpoints[i].x!=0)//������Ч��λ��
		{
			ChessPoints[roundpoints[i].x][roundpoints[i].y] = anticolor;//�߶Է�����
			if (!can_five(color)&&!has_alive(color)) 
			{
				//�������һ��������ܴ��ڻ��ӣ�Ҳ������ֱ������5�ӣ���ôʧ��
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
		return true;//��Ȼ�Ƕ��׻���
	}
	ChessPoints[x][y] = 0;
	return false;
}

void CGoBangGame::getroundpoints(int x, int y,int color,POINT roundpoints[])
{
	//ע�⣬�������8���Ļ�����ô�󼸸�x,y��Ϊ0
	int counter = 0;
	int i;
	for(i=0;i<8;i++)//���
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
					//�ҵ������ұ�֤����Χ��color����
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
	//�ж�һ��������û�ж��׻���
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����
	counter = 0;
	//if (has_alive(color)) return false;//����Ѿ�����һ�׻��ӣ���ô���ٽ�������
	BOOL flag = false;	//����ֵ
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

	//�ж�һ��������û�ж��׻���
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����
	counter = 0;
	//if (has_alive(color)) return false;//����Ѿ�����һ�׻��ӣ���ô���ٽ�������

	BOOL flag = false;	//����ֵ

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

	//�ж�һ��������û�ж��׻���

	//if (has_alive(color)) return false;//����Ѿ�����һ�׻��ӣ���ô���ٽ�������

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

	//�ж�һ��������û�ж��׻���

	//if (has_alive(color)) return false;//����Ѿ�����һ�׻��ӣ���ô���ٽ�������

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

	//�ж�һ��������û�ж��׻���(����һ�����ӵ�)
	
	int anticolor;//�Է���color
	
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
				
				//�˴�ֻ�ܱ��������Ѿ��г����ӣ���������Է���һ�׻��ӻ���ǿ���׻����ܹ�ʹ�ô�ǿ���׻�����ʧ				
				//��ô�˳�����Ҳ���ܲ��ã��˴���������жϣ�����
				//�Է�Ϊ�˷���������µ��ӣ������ǶԷ���һ�׻��ӡ�canfour��
				
				ChessPoints[closepoints[i].x][closepoints[i].y]=color;//�Լ�������
				int x,y;//�Է����ڷ��ص���
				can_five(color,x,y);
				
				if (!is_alive(x,y,anticolor)&&!is_canfour(anticolor,x,y))
				{
					ChessPoints[closepoints[i].x][closepoints[i].y]=0;//�ָ���������				
					return true;				
				}
				ChessPoints[closepoints[i].x][closepoints[i].y]=0;//�ָ���������				
				
			}
			
			
		}
		
	}
	
	return false;

}



BOOL CGoBangGame::has_twolevel_four(int color,int &counter,POINT tempresult[])

{

	//�ж�һ��������û�ж��׻���(����һ�����ӵ�)

	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����

	counter = 0;
	
	int anticolor;//�Է���color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;	//����ֵ
	
	POINT closepoints[225];
	
	int count = 0;	
	
	getclose(color,count,closepoints);		
	
	for(int i=0;i<count;i++)	
	{		
		if (closepoints[i].x!=0)		
		{
			if (twolevel_four(closepoints[i].x,closepoints[i].y,color)) 		
			{
				
				//�˴�ֻ�ܱ��������Ѿ��г����ӣ���������Է���һ�׻��ӻ���ǿ���׻����ܹ�ʹ�ô�ǿ���׻�����ʧ				
				//��ô�˳�����Ҳ���ܲ��ã��˴���������жϣ�����
				//�Է�Ϊ�˷���������µ��ӣ������ǶԷ���һ�׻��ӡ�canfour��
				
				ChessPoints[closepoints[i].x][closepoints[i].y]=color;//�Լ�������
				int x,y;//�Է����ڷ��ص���
				can_five(color,x,y);
				
				if (!is_alive(x,y,anticolor)&&!is_canfour(anticolor,x,y))
				{
					flag = true;
					
					tempresult[counter].x = closepoints[i].x;
					
					tempresult[counter].y = closepoints[i].y;
					
					counter++;					
				}
				ChessPoints[closepoints[i].x][closepoints[i].y]=0;//�ָ���������					
			}			
		}
		
	}
	return flag;
}

int CGoBangGame::random(int x, int y)
{
	//���x��y֮����������x��y����ȡ��
	srand(LOWORD(GetTickCount()));
	return rand()%(y-x+1)+x;

}

void CGoBangGame::getclosest(int color,int n,POINT closepoints[])
{
	//�˺�������Ϊ���õ���ǰ���пհ׸��У���color���������ǰn����
	//��Ҫ������С������Χ�����������������п��ܽ���Ҫ�ĵ��ų�����
	//���Ժ�������getclose����������㷨������㷨����ʹ��

	int i,j;
	for (i=0;i<n;i++)
	{
		closepoints[i].x = 0;
		closepoints[i].y = 0;
	}

	int *dist = new int[n];//��¼��Ӧ�ľ���ֵ
	int maxvalue=0;//��¼���������ľ���ֵ
	int maxindex;//��¼���������ľ���ֵ������index
	int counter = 0;//������Ԫ�صĸ���

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
						{//������ͬ��ɫ�ľ���ֵ
							sum += (i-p)*(i-p)+(j-q)*(j-q);
						}
					}
				}
				if (counter<n)
				{
					//˵����û�����������Լ�����
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
					//�Ѿ���������Ҫĳ���滻�㷨
					if (sum<maxvalue)
					{
						//���Բ��룬��ֱ���滻ԭ�����ֵ
						dist[maxindex] = sum;
						closepoints[maxindex].x = i;
						closepoints[maxindex].y = j;
						//����Ҫ�һ�maxvalue��maxindex
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

	//�˺���������������һ�׻��ӵ���
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����
	counter = 0;

	//if (has_alive(color)) return false;//����Ѿ�����һ�׻��ӣ���ô���ٽ�������
	BOOL flag = false;	//����ֵ
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
					int anticolor;//�Է���color
					
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

	//�˺�����������������׻��ӵ���
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����
	counter = 0;
	//if (has_twolevel(color)) return false;//����Ѿ����ڶ��׻��ӣ���ô���ٽ�������

	BOOL flag = false;	//����ֵ
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

				//������ڶ��׻��ӣ���¼�������
				tempresult[counter].x = closepoints[i].x;
				tempresult[counter].y = closepoints[i].y;
				counter++;
				flag = true;
			}

			ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ
		}
	}
	return flag;
}

void CGoBangGame::design(int x, int y, int color)
{
	//�µĸ�ֵ�㷨������ģʽƥ��ķ���
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	int model[][9]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����
		//-1��ʾ���⣬1��ʾ��ͬ��ɫ��0��ʾ�գ�2��ʾ(x,y)����λ��
		//3��ʾ������ɫ����
		//����ģʽ���ո�ֵ�ķ����ӵ͵�������

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
		//�ĸ��������
		int tempvalue = 0;
		int j;
		BOOL find;
		for (j=model_num-1,find=false;!find&&(j>=0);j--)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��
		{//�Ӵ�Сƥ��ģʽ����֤��ȷ��

			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==3)
				{
					if (getcolor(x,i,k-4,y)!=anticolor)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}
			}

			if (flag) 
			{
				//ƥ����ĳ��ģʽ
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

			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==3)			
				{
					if (getcolor(x,i,4-k,y)!=anticolor)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}
			}

			if (flag) 
			{
				//ƥ����ĳ��ģʽ
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
	//�˴�Ϊ��ֵ�㷨��������ǰ�ľ��丳ֵ�㷨�����������Ľ���������ά�����Ķ�
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
	//���ڵõ�������ĳһ�����ɫ����ҪΪ�˼����ж�index�Ϸ��ԵĴ���
	//moveΪ�ƶ���λ�ƣ�iΪ����ȡֵ0��1��2��3

	POINT d[4];
	d[0].x = 1;d[0].y = 0;//�᷽��
	d[1].x = 0;d[1].y = 1;//�ݷ���
	d[2].x = 1;d[2].y = -1;//б����
	d[3].x = 1;d[3].y = 1;//б����

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
	//�˷��������õ���ֵ������ֵ�㣬�����ؽ����	

	counter = 0;

	int maxvalue = 0;//���ֵ����ֵ

	for(int color = 1;color<=2;color++)//��˫����Ҫ�жϣ�ȡ���ֵ
	{
		design(color);//��ֵ����

		//��ֵ���
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

						//�����ֵ������tempresult

						tempresult[0].x = closepoints[i].x;

						tempresult[0].y = closepoints[i].y;

						counter = 1;

						maxvalue = m_ptvalue[closepoints[i].x][closepoints[i].y];					

					}

					else if (m_ptvalue[closepoints[i].x][closepoints[i].y]==maxvalue)

					{

						//�����ѡ�����

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

	//�Ӵ�ѡ��Ļ����еõ���ֵ���ߵĻ���
	//tempresultΪ��ѡ���Ӽ���nΪ�˼���Ԫ�ظ���
	//defent��ʾ�Ƿ�Ϊdefent״̬����ʱ
	//colorΪ������ӵ���ɫ�����������Լ��к�ֵ����ôӦ�����˵�2��	
	//mΪ�������ӣ����ڿ����Ƿ�ԶԷ������жϣ���עһ��ʱʹ�ã�Ĭ��ֵΪ2����עһ��ʱΪ1

	if (n==1)
	{
		//ֻ��һ���㣬��ôֱ�ӷ���
		x = tempresult[0].x;
		y = tempresult[0].y;
		return;
	}

	POINT local_result[225];//�ݴ�ľֲ���ѡԪ��

	int counter = 0;//��local_result�ļ���

	int maxvalue = 0;//���ֵ����ֵ

	clearvalue();

	for (int j=1;j<=m;j++)
	{
		//�ȶ�������Ӹ�ֵ
		int i;
		for(i=0;i<n;i++)
		{
			design(tempresult[i].x,tempresult[i].y,color);//�Ե����Ӹ�ֵ
		}

		for(i=0;i<n;i++)
		{

			if (m_ptvalue[tempresult[i].x][tempresult[i].y]>maxvalue)

			{

				//�����ֵ������local_result

				local_result[0].x = tempresult[i].x;

				local_result[0].y = tempresult[i].y;

				counter = 1;

				maxvalue = m_ptvalue[tempresult[i].x][tempresult[i].y];					

			}

			else if (m_ptvalue[tempresult[i].x][tempresult[i].y]==maxvalue)

			{

				//�����ѡ�����

				local_result[counter].x = tempresult[i].x;

				local_result[counter].y = tempresult[i].y;

				counter++;

			}

		}

		if (defent)	maxvalue = maxvalue * 2 + 10;

		if (color==1) color = 2;

		else color = 1;

	}

	//��ʱ����ѡԪ���Ѿ���local_result��

	int tempi = random(0,counter-1);

	x = local_result[tempi].x;

	y = local_result[tempi].y;

}



void CGoBangGame::design_depart(int x, int y, int color)
{
	//�µĸ�ֵ�㷨������ģʽƥ��ķ���
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	int model[][9]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����

		//-1��ʾ���⣬1��ʾ��ͬ��ɫ��0��ʾ����3��2��ʾ(x,y)����λ��
		//3��ʾ������ɫ����
		//����ģʽ���ո�ֵ�ķ����ӵ͵�������
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

		//�ĸ��������
		int tempvalue = 0;
		int j;
		BOOL find;
		for (j=model_num-1,find=false;!find&&(j>=0);j--)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��
		{//�Ӵ�Сƥ��ģʽ����֤��ȷ��

			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)==anticolor)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==3)
				{
					if (getcolor(x,i,k-4,y)!=anticolor)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}
			}

			if (flag) 
			{
				//ƥ����ĳ��ģʽ
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

			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)==anticolor)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==3)
				{
					if (getcolor(x,i,4-k,y)!=anticolor)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}
			}

			if (flag) 
			{
				//ƥ����ĳ��ģʽ
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
	//��ÿһ������и�ֵ
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	int start,end,total;
	if (ChessPoints[x][y]==0)//ֻ�Կո�ֵ
	{
		for(int i=0;i<4;i++)//�ĸ�����
		{
			if (getcolor(x,i,-1,y)==anticolor) start=0;
			else
			{
				for(start=2;getcolor(x,i,-start,y)==color;start++);//�������֮ǰ��ͬ���Ӹ���
				start=start-1;
			}

			if (getcolor(x,i,+1,y)==anticolor) end=0;
			else
			{
				for(end=2;getcolor(x,i,+end,y)==color;end++);//�������֮���ͬ���Ӹ���
				end=end-1;
			}

			total=start+end;//�����ܸ���
			if (getcolor(x,i,-1,y)==0) total=total-1;
			if (getcolor(x,i,+1,y)==0) total=total-1;
			switch (total)
			{
				case 0:break;
				case 1:
					if ((start==1)&&(end==2))//�ڽ����������ǿո�
					{
						if (getcolor(x,i,+3,y)==0)
							m_ptvalue[x][y]=m_ptvalue[x][y]+2;
					}

					if ((start==2)&&(end==1))//�ڽ����������ǿո�
					{
						if (getcolor(x,i,-3,y)==0)
							m_ptvalue[x][y]=m_ptvalue[x][y]+2;
					}

					if ((start==1)&&(end==1))//�ڽ���һ���ǿո�
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
	//���value��ԭ�ȸ�ֵ
	int i,j;
	for(i=1;i<=m_xGrid;i++)
		for(j=1;j<=m_yGrid;j++)
		{
			m_ptvalue[i][j] = 0;//�����
		}
}

BOOL CGoBangGame::has_canfour(int color, int &counter, POINT tempresult[])
{
	//�ҵ���������4�ӵ��ӣ���Щ�Ӳ��ǻ��ӣ���Ϊ���Ա���ס�����ǿ�������ǣ�Ƶ���
	counter = 0;

	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	BOOL flag = false;	//����ֵ

	POINT closepoints[225];
	int count = 0;	
	getclose(color,count,closepoints);		
	for(int i=0;i<count;i++)
	{
		if (closepoints[i].x!=0)
		{
			if (is_canfour(color,closepoints[i].x,closepoints[i].y)) 
			{

				//�˴�ֻ�ܱ��������Ѿ��г����ӣ���������Է���һ�׻��ӻ���ǿ���׻����ܹ�ʹ�ô�ǿ���׻�����ʧ
				//��ô�˳�����Ҳ���ܲ��ã��˴���������жϣ�����
				//�Է�Ϊ�˷���������µ��ӣ������ǶԷ���һ�׻��ӡ����׻��ӡ�twoalive�ӡ�canfour�ӡ�qianmodel��,makealiveandtwolevel��,maketwolevelalive
				
				ChessPoints[closepoints[i].x][closepoints[i].y]=color;//�Լ�������
				int x,y;//�Է����ڷ��ص���
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
						//5�������ж��߳��Է����Ӻ��Ƿ����ǿ����ǣ����ƥ��
						ChessPoints[x][y] = anticolor;
						m_grade = 4;//���������ⷢ��ѭ������
						if (has_twolevelqianmodel_qiang(anticolor))
						{
							//����ǿ����ǣ����ƥ�䣬����¼
							tempflag = false;
						}
						m_grade = 5;//�ָ��ȼ�
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
				ChessPoints[closepoints[i].x][closepoints[i].y]=0;//�ָ���������				
			}			
		}
	}
	return flag;
}


BOOL CGoBangGame::has_canfour(int color)

{

	//�ҵ���������4�ӵ��ӣ���Щ�Ӳ��ǻ��ӣ���Ϊ���Ա���ס�����ǿ�������ǣ�Ƶ���

	int anticolor;//�Է���color
	
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
				
				//�˴�ֻ�ܱ��������Ѿ��г����ӣ���������Է���һ�׻��ӻ���ǿ���׻����ܹ�ʹ�ô�ǿ���׻�����ʧ
				
				//��ô�˳�����Ҳ���ܲ��ã��˴���������жϣ�����
				//�Է�Ϊ�˷���������µ��ӣ������ǶԷ���һ�׻��ӡ����׻��ӡ�twoalive�ӡ�canfour�ӡ�qianmodel��
				
				ChessPoints[closepoints[i].x][closepoints[i].y]=color;//�Լ�������
				int x,y;//�Է����ڷ��ص���
				can_five(color,x,y);
				
				if (!is_alive(x,y,anticolor)&&!is_canfour(anticolor,x,y)&&!twolevel_alive(x,y,anticolor)&&!is_maketwoalive(x,y,anticolor)&&!is_makealiveandtwolevel(anticolor,x,y)&&!is_maketwolevelalive(x,y,anticolor))
				{
					
					if (!is_low)
					{
						is_low = true;
						if (!is_qianmodel(anticolor,x,y))
						{
							ChessPoints[closepoints[i].x][closepoints[i].y]=0;//�ָ���������
							return true;
						}
						is_low = false;
					}
					else
					{
						ChessPoints[closepoints[i].x][closepoints[i].y]=0;//�ָ���������
						return true;
					}
					
				}
				ChessPoints[closepoints[i].x][closepoints[i].y]=0;//�ָ���������				
				
			}	
			
		}
		
	}
	
	return false;

}





BOOL CGoBangGame::has_canfive(int color, int &counter, POINT tempresult[])

{

	//�ҵ���������5�ӵ���

	counter = 0;

	BOOL flag = false;	//����ֵ

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
	//�ҵ���������4�ӵ��ӣ���Щ�Ӳ��ǻ��ӣ���Ϊ���Ա���ס�����ǿ�������ǣ�Ƶ���
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
					//�ҵ����ӣ�
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
	//ȫ��ǣ���ӣ�����һ�׻��ӡ����׻��Ӻ�������4��ǣ���ӣ��Լ�����һ�׻��ӵ���
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

	//����ǣ���ӣ�����һ�׻��ӡ����׻����Լ�����һ�׻��ӵ���

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
	//��������getclosest�����㷨Ϊ��ÿ���ӵ�roundpoints���ϲ���һ�𷵻�
	counter = 0;
	POINT local_tempresult[8];
	POINT local_round[8];//��Χ�ӵ���Χ��
	for (int i=1;i<=m_xGrid;i++)
		for (int j=1;j<=m_yGrid;j++)
		{
			if (ChessPoints[i][j]==color)
			{
				getroundpoints(i,j,color,local_tempresult);
				merge(tempresult,counter,local_tempresult,8);
				for (int k=0;k<8;k++)
				{
					//����Χ�ӵ���Χ��Ҳ�ӽ���
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
	//������Եõ�4���ӣ����ڿ��ܱȽ�Զ�����Բ�������
	int count;
	POINT canfour[225];
	if (has_canfour(color,count,canfour))
	{
		merge(tempresult,counter,canfour,count);
	}
*/

/*
	//���µĴ��뷵�����е��ӣ�Ϊ�˾�ȷ���
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
	//��������getclosest�����㷨Ϊ��ÿ���ӵ�roundpoints���ϲ���һ�𷵻�
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
	//���ڽ�append����ϲ���target�����У���֤û���ظ���
	//tn��target�ĳ��ȣ�an��append�ĳ���
	for(int i=0;i<an;i++)
	{
		if (append[i].x==0) continue;
		//�������ظ���
		BOOL flag = true;
		for (int j=0;j<tn;j++)
		{
			if ((target[j].x==append[i].x)&&(target[j].y==append[i].y))
			{
				//������ͬ��
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
	//���ڽ�append����ϲ���target�����У���֤û���ظ���
	//tn��target�ĳ��ȣ�an��append�ĳ���
	for(int i=0;i<an;i++)	
	{	
		if (append[i].x==0) continue;	
		//�������ظ���
		BOOL flag = true;	
		for (int j=0;j<tn;j++)	
		{		
			if ((target[j].x==append[i].x)&&(target[j].y==append[i].y))	
			{	
				//������ͬ��
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
	//�ж�һ����(x,y)��Χ�Ƿ���color����
	for (int i=0;i<=4;i++)
	{
		//�ĸ�����
		if (getcolor(x,i,-1,y)==color) return true;
		if (getcolor(x,i,1,y)==color) return true;
	}
	return false;
}

BOOL CGoBangGame::has_alive_defent(int color,int &counter,POINT tempresult[])
{

	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	counter = 0;
	
	int anticolor;//�Է���color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_alive(color))
		
	{
		
		//�Է�����һ�׻���
		
		int count;
		
		POINT closepoints[225];
		
		//getclose(color, count, closepoints);
		has_defent_point(color,count,closepoints);
		
		//�õ��ͶԷ������������

		BOOL has_qian = false;//�Ƿ��Ѿ��п����赲��ǣ����
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//�������ӱ�������Լ���һ��ǣ���ӣ���ô���õ�ס�Է�̫��Ķ�������
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
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
							//�����ǰû�У���ô����
							tempresult[0].x = closepoints[i].x;
							
							tempresult[0].y = closepoints[i].y;

							counter = 1;
						}						
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						//ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						//return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}
				else if (!has_qian)//û��qian��ʱ�����
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
						
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
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;	
}

BOOL CGoBangGame::has_twolevelqianmodel_qiang_defent(int color,int &counter,POINT tempresult[])

{
	
	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����
	
	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�
	
	//������ֵ��ߵĵ�
	
	counter = 0;
	
	int anticolor;//�Է���color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_twolevelqianmodel_qiang(color))
		
	{
		
		//�Է�����һ�׻���
		
		int count;
		
		POINT closepoints[225];
		
		//getclose(color, count, closepoints);
		//getclose_small(count,closepoints);
		//has_twolevelqianmodel_qiang_defent_low(color,count,closepoints);
		getclose_twolevelqianmodel_qiang(color,count,closepoints);
		
		//�õ��ͶԷ������������

		BOOL has_qian = false;//�Ƿ��Ѿ��п����赲��ǣ����
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//�������ӱ�������Լ���һ��ǣ���ӣ���ô���õ�ס�Է�̫��Ķ�������
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��						
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
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
							//�����ǰû�У���ô����
							tempresult[0].x = closepoints[i].x;
							
							tempresult[0].y = closepoints[i].y;

							counter = 1;
						}						
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						if (counter==3)
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
							
							return true;
						}
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}
				else if (!has_qian)//û��qian��ʱ�����
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��

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
									ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
									
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
								ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
								
								return true;
							}
						}
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;
	
}

BOOL CGoBangGame::has_threelevelqianmodel_qiang_defent(int color,int &counter,POINT tempresult[])

{
	
	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����
	
	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�
	
	//������ֵ��ߵĵ�
	
	counter = 0;
	
	int anticolor;//�Է���color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_threelevelqianmodel_qiang(color))
		
	{
		
		//�Է�����һ�׻���
		
		int count;
		
		POINT closepoints[225];
		
		//getclose(color, count, closepoints);
		getclose_small(count,closepoints);
		//has_threelevelqianmodel_qiang_defent_low(color,count,closepoints);
		
		//�õ��ͶԷ������������

		BOOL has_qian = false;//�Ƿ��Ѿ��п����赲��ǣ����
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//�������ӱ�������Լ���һ��ǣ���ӣ���ô���õ�ס�Է�̫��Ķ�������
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��						
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
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
							//�����ǰû�У���ô����
							tempresult[0].x = closepoints[i].x;
							
							tempresult[0].y = closepoints[i].y;

							counter = 1;
						}						
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						if (counter==3)
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
							
							return true;
						}
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}
				else if (!has_qian)//û��qian��ʱ�����
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��

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
									ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
									
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
								ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
								
								return true;
							}
						}
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;	
	
}

BOOL CGoBangGame::has_twolevelqianmodel_qiang_defent_low(int color,int &counter,POINT tempresult[])

{
	
	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����
	
	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�
	
	//������ֵ��ߵĵ�
	
	counter = 0;
	
	int anticolor;//�Է���color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_twolevelqianmodel_qiang(color))
		
	{
		
		//�Է�����һ�׻���
		
		int count;
		
		POINT closepoints[225];
		
		//getclose(color, count, closepoints);
		getclose_small(count,closepoints);
		//getclose_twolevelqianmodel_qiang(color,count,closepoints);
		
		//�õ��ͶԷ������������
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				
				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
				
				//����һ��
				
				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_twolevelqianmodel_qiang(color)) 
					
				{
					
					//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
					
					tempresult[counter].x = closepoints[i].x;
					
					tempresult[counter].y = closepoints[i].y;
					
					counter++;
					
					flag = true;
					
					defent_success = true;
					/*

					if (counter==3)
					{
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						return true;
					}*/
					
				}
				
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;
	
}

BOOL CGoBangGame::has_threelevelqianmodel_qiang_defent_low(int color,int &counter,POINT tempresult[])

{
	
	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����
	
	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�
	
	//������ֵ��ߵĵ�
	
	counter = 0;
	
	int anticolor;//�Է���color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_threelevelqianmodel_qiang(color))
		
	{
		
		//�Է�����һ�׻���
		
		int count;
		
		POINT closepoints[225];
		
		//getclose(color, count, closepoints);
		getclose_small(count,closepoints);
		
		//�õ��ͶԷ������������
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				
				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
				
				//����һ��
				
				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_threelevelqianmodel_qiang(color)) 
					
				{
					
					//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
					
					tempresult[counter].x = closepoints[i].x;
					
					tempresult[counter].y = closepoints[i].y;
					
					counter++;
					
					flag = true;
					
					defent_success = true;
					/*

					if (counter==3)
					{
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						return true;
					}*/
					
				}
				
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;
	
}






BOOL CGoBangGame::has_alive_defent(int color)

{

	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	int anticolor;//�Է���color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_alive(color))
		
	{
		
		//�Է�����һ�׻���
		
		int count;
		
		POINT closepoints[225];
		
		//getclose(color, count, closepoints);
		has_defent_point(color,count,closepoints);
		
		//�õ��ͶԷ������������

		BOOL has_qian = false;//�Ƿ��Ѿ��п����赲��ǣ����
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//�������ӱ�������Լ���һ��ǣ���ӣ���ô���õ�ס�Է�̫��Ķ�������
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��		
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}
				else if (!has_qian)//û��qian��ʱ�����
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
						flag = true;
						
						defent_success = true;
						
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						return true;
						
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;	

}



BOOL CGoBangGame::has_alive_defent_low(int color,int &counter,POINT tempresult[])

{

	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	counter = 0;

	int anticolor;//�Է���color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;

	defent_success = false;

	if (has_alive(color))

	{

		//�Է�����һ�׻���

		int count;

		POINT closepoints[225];

		//getclose(color, count, closepoints);
		has_defent_point(color,count,closepoints);

		//�õ��ͶԷ������������

		for(int i=0;i<count;i++)

		{

			if (closepoints[i].x!=0)

			{

				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;

				//����һ��

				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color))

				{

					//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��

					tempresult[counter].x = closepoints[i].x;

					tempresult[counter].y = closepoints[i].y;

					counter++;

					flag = true;

					defent_success = true;

				}

				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

			}

		}

	}

	else defent_success = true;

	return flag;

}



BOOL CGoBangGame::has_alive_defent_low(int color)

{

	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	int anticolor;//�Է���color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;

	defent_success = false;

	if (has_alive(color))

	{

		//�Է�����һ�׻���

		int count;

		POINT closepoints[225];

		//getclose(color, count, closepoints);
		has_defent_point(color,count,closepoints);

		//�õ��ͶԷ������������

		for(int i=0;i<count;i++)

		{

			if (closepoints[i].x!=0)

			{

				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;

				//����һ��

				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color))

				{

					//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��

					flag = true;

					defent_success = true;

					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

					return true;

				}

				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

			}

		}

	}

	else defent_success = true;

	return flag;

}



BOOL CGoBangGame::has_twolevel_defent(int color, int &counter, POINT tempresult[])

{

	//�˺��������赲�Է����ֶ��׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ��Ķ��׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է����׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	counter = 0;
	
	int anticolor;//�Է���color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_twolevel(color))
		
	{
		
		//�Է�����һ�׻���
		
		int count;
		
		POINT closepoints[225];
		
		getclose_twolevel(color, count, closepoints);

		//�õ��ͶԷ������������

		BOOL has_qian = false;//�Ƿ��Ѿ��п����赲��ǣ����
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//�������ӱ�������Լ���һ��ǣ���ӣ���ô���õ�ס�Է�̫��Ķ�������
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
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
							//�����ǰû�У���ô����
							tempresult[0].x = closepoints[i].x;
							
							tempresult[0].y = closepoints[i].y;

							counter = 1;
						}						
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						//ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						//return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}
				else if (!has_qian)//û��qian��ʱ�����
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��

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
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;

}



BOOL CGoBangGame::has_twolevel_defent(int color)

{

	//�˺��������赲�Է����ֶ��׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ��Ķ��׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է����׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	int anticolor;//�Է���color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_twolevel(color))
		
	{
		
		//�Է�����һ�׻���
		
		int count;
		
		POINT closepoints[225];
		
		getclose_twolevel(color, count, closepoints);
		
		//�õ��ͶԷ������������

		BOOL has_qian = false;//�Ƿ��Ѿ��п����赲��ǣ����
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//�������ӱ�������Լ���һ��ǣ���ӣ���ô���õ�ס�Է�̫��Ķ�������
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��				
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}
				else if (!has_qian)//û��qian��ʱ�����
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
						
						flag = true;
						
						defent_success = true;

						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						return true;						
						
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;

}



BOOL CGoBangGame::has_twolevel_defent_low(int color, int &counter, POINT tempresult[])

{

	//�˺��������赲�Է����ֶ��׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ��Ķ��׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է����׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	counter = 0;

	int anticolor;//�Է���color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;

	defent_success = false;

	if (has_twolevel(color))

	{

		//�Է����ڶ��׻���

		int count;

		POINT closepoints[225];

		getclose_twolevel(color, count, closepoints);

		//�õ��ͶԷ������������

		for(int i=0;i<count;i++)

		{

			if (closepoints[i].x!=0)

			{

				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;

				//����һ��

				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color))

				{

					//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��

					tempresult[counter].x = closepoints[i].x;

					tempresult[counter].y = closepoints[i].y;

					counter++;

					flag = true;

					defent_success = true;

				}

				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

			}

		}

	}

	else defent_success = true;

	return flag;

}



BOOL CGoBangGame::has_twolevel_defent_low(int color)

{

	//�˺��������赲�Է����ֶ��׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ��Ķ��׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է����׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	int anticolor;//�Է���color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;

	defent_success = false;

	if (has_twolevel(color))

	{

		//�Է����ڶ��׻���

		int count;

		POINT closepoints[225];

		getclose_twolevel(color, count, closepoints);

		//�õ��ͶԷ������������

		for(int i=0;i<count;i++)

		{

			if (closepoints[i].x!=0)

			{

				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;

				//����һ��

				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color))

				{

					//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��

					flag = true;

					defent_success = true;

					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

					return true;

				}

				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

			}

		}

	}

	else defent_success = true;

	return flag;

}



BOOL CGoBangGame::has_twolevel_four_defent(int color, int &counter, POINT tempresult[])

{

	//�˺��������赲�Է�����ǿ���׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ��Ķ��׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է����׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	counter = 0;
	
	int anticolor;//�Է���color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_twolevel_four(color))
		
	{
		
		//�Է�����һ�׻���
		
		int count;
		
		POINT closepoints[225];
		
		getclose_twolevel_four(color, count, closepoints);
		
		//�õ��ͶԷ������������

		BOOL has_qian = false;//�Ƿ��Ѿ��п����赲��ǣ����
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//�������ӱ�������Լ���һ��ǣ���ӣ���ô���õ�ס�Է�̫��Ķ�������
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
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
							//�����ǰû�У���ô����
							tempresult[0].x = closepoints[i].x;
							
							tempresult[0].y = closepoints[i].y;

							counter = 1;
						}						
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						//ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						//return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}
				else if (!has_qian)//û��qian��ʱ�����
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
						
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
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;	

}



BOOL CGoBangGame::has_twolevel_four_defent(int color)

{

	//�˺��������赲�Է�����ǿ���׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ��Ķ��׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է����׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	int anticolor;//�Է���color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	if (has_twolevel_four(color))
		
	{
		
		//�Է�����һ�׻���
		
		int count;
		
		POINT closepoints[225];
		
		getclose_twolevel_four(color, count, closepoints);
		
		//�õ��ͶԷ������������

		BOOL has_qian = false;//�Ƿ��Ѿ��п����赲��ǣ����
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//�������ӱ�������Լ���һ��ǣ���ӣ���ô���õ�ס�Է�̫��Ķ�������
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��										
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}
				else if (!has_qian)//û��qian��ʱ�����
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
						flag = true;
						
						defent_success = true;
						
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						return true;
						
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}				
				
			}
			
		}
		
	}
	
	else defent_success = true;
	
	return flag;
}



BOOL CGoBangGame::has_twolevel_four_defent_low(int color, int &counter, POINT tempresult[])

{

	//�˺��������赲�Է�����ǿ���׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ��Ķ��׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է����׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	counter = 0;

	int anticolor;//�Է���color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;

	defent_success = false;

	if (has_twolevel_four(color))

	{

		//�Է����ڶ��׻���

		int count;

		POINT closepoints[225];

		getclose_twolevel_four(color, count, closepoints);

		//�õ��ͶԷ������������

		for(int i=0;i<count;i++)

		{

			if (closepoints[i].x!=0)

			{

				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;

				//����һ��

				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color))

				{

					//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��

					tempresult[counter].x = closepoints[i].x;

					tempresult[counter].y = closepoints[i].y;

					counter++;

					flag = true;

					defent_success = true;

				}

				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

			}

		}

	}

	else defent_success = true;

	return flag;

}



BOOL CGoBangGame::has_twolevel_four_defent_low(int color)

{

	//�˺��������赲�Է�����ǿ���׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ��Ķ��׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է����׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	int anticolor;//�Է���color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	BOOL flag = false;

	defent_success = false;

	if (has_twolevel_four(color))

	{

		//�Է����ڶ��׻���

		int count;

		POINT closepoints[225];

		getclose_twolevel_four(color, count, closepoints);

		//�õ��ͶԷ������������

		for(int i=0;i<count;i++)

		{

			if (closepoints[i].x!=0)

			{

				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;

				//����һ��

				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color))

				{

					//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��

					flag = true;

					defent_success = true;

					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

					return true;

				}

				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

			}

		}

	}

	else defent_success = true;

	return flag;

}



BOOL CGoBangGame::has_qianmodel_addone_defent(int color,int &counter,POINT tempresult[],BOOL qiang)

{

	//ǣ����ƥ��!!

	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����

	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�

	//������ֵ��ߵĵ�

	counter = 0;
	
	int anticolor;//�Է���color
	
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	
	BOOL flag = false;
	
	defent_success = false;
	
	
	if (has_qianmodel_addone(color,qiang))
		
	{
		//�Է�����һ�׻���
		
		int count;
		
		POINT closepoints[225];
		
		//getclose_small(count, closepoints);
		getclose_qianmodeladdone(color,count,closepoints,qiang);
		//has_qianmodel_addone_defent_low(color,count,closepoints,qiang);
		
		//�õ��ͶԷ������������

		BOOL has_qian = false;//�Ƿ��Ѿ��п����赲��ǣ����
		
		for(int i=0;i<count;i++)
			
		{
			
			if (closepoints[i].x!=0)
				
			{
				

				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//�������ӱ�������Լ���һ��ǣ���ӣ���ô���õ�ס�Է�̫��Ķ�������
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)&&!has_qianmodel_addone(color,qiang)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
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
							//�����ǰû�У���ô����
							tempresult[0].x = closepoints[i].x;
							
							tempresult[0].y = closepoints[i].y;

							counter = 1;
						}						
						
						has_qian = true;
						
						flag = true;
						
						defent_success = true;
						if (counter==3)
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
							
							return true;
						}					
						
					}
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ

				}
				else if (!has_qian)//û��qian��ʱ�����
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 
						
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��						

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
									ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
									
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
								ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
								
								return true;
							}			
						}

					}	
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
				}				
			}
		}	
	}
	
	else defent_success = true;
	
	return flag;
}

BOOL CGoBangGame::has_qianmodel_addone_defent_low(int color,int &counter,POINT tempresult[],BOOL qiang)
{
	
	//ǣ����ƥ��!!
	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����
	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ����
	//������ֵ��ߵĵ�
	counter = 0;
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	BOOL flag = false;
	
	defent_success = false;
	if (has_qianmodel_addone(color,qiang))	
	{
		//�Է�����һ�׻���
		int count;
		POINT closepoints[225];
		
		//getclose_small(count, closepoints);
		getclose_qianmodeladdone(color,count,closepoints,qiang);
		
		//�õ��ͶԷ������������
		for(int i=0;i<count;i++)	
		{
			if (closepoints[i].x!=0)	
			{
				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
				//����һ��
				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)) 	
				{
					
					//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
					
					tempresult[counter].x = closepoints[i].x;
					tempresult[counter].y = closepoints[i].y;	
					counter++;
					flag = true;
					defent_success = true;

					/*
					if (counter==3)
					{
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						return true;
					}			
					*/
				}
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
			}
		}
	}
	else defent_success = true;
	return flag;
	
}

BOOL CGoBangGame::has_qianmodel_defent(int color,int &counter,POINT tempresult[],BOOL qiang)
{
	//����ǣ����ƥ��!!
	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����
	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�
	//������ֵ��ߵĵ�

	counter = 0;
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	
	else anticolor = 1;
	BOOL flag = false;
	defent_success = false;
	
	if (has_qianmodel(color,qiang))
	{
		//�Է�����һ�׻���
		int count;
		POINT closepoints[225];
		
		//getclose_small(count, closepoints);
		getclose_qianmodel(color,count,closepoints,qiang);
		//has_qianmodel_defent_low(color,count,closepoints,qiang);
		
		//�õ��ͶԷ������������

		BOOL has_qian = false;//�Ƿ��Ѿ��п����赲��ǣ����
		
		for(int i=0;i<count;i++)
		{
			if (closepoints[i].x!=0)
			{	
				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//�������ӱ�������Լ���һ��ǣ���ӣ���ô���õ�ס�Է�̫��Ķ�������
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)&&!has_qianmodel(color,qiang)) 
					{
						
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
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
							//�����ǰû�У���ô����
							tempresult[0].x = closepoints[i].x;
							tempresult[0].y = closepoints[i].y;
							counter = 1;
						}						
						
						has_qian = true;
						flag = true;
						defent_success = true;
						if (counter==3)
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
							
							return true;
						}				
					}
					
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
				}
				else if (!has_qian)//û��qian��ʱ�����
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					
					//����һ��
					
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 	
					{
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
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
									ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
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
								ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
								return true;
							}			
						}
					}
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
				}				
			}	
		}
	}
	
	else defent_success = true;
	
	return flag;
}

BOOL CGoBangGame::has_qianmodel_defent_low(int color,int &counter,POINT tempresult[],BOOL qiang)
{
	
	//����ǣ����ƥ��!!
	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����
	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�	
	//������ֵ��ߵĵ�
	counter = 0;
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;	
	BOOL flag = false;
	defent_success = false;
	
	if (has_qianmodel(color,qiang))	
	{
		//�Է�����һ�׻���
		int count;
		POINT closepoints[225];
		//getclose_small(count, closepoints);
		getclose_qianmodel(color,count,closepoints,qiang);
		//�õ��ͶԷ������������
		
		for(int i=0;i<count;i++)	
		{	
			if (closepoints[i].x!=0)		
			{	
				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;	
				//����һ��
				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)) 	
				{
					//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
					tempresult[counter].x = closepoints[i].x;
					tempresult[counter].y = closepoints[i].y;
					counter++;
					flag = true;
					
					defent_success = true;
					/*
					if (counter==3)
					{
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						return true;
					}*/			
					
				}
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ	
			}	
		}
	}
	else defent_success = true;
	
	return flag;
	
}

BOOL CGoBangGame::has_twolevel_qianmodel_defent_low(int color,int &counter,POINT tempresult[])
{
	//����ǣ����ƥ��!!
	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����
	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�
	//������ֵ��ߵĵ�
	
	counter = 0;
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	BOOL flag = false;
	defent_success = false;
	if (has_twolevel_qianmodel(color))	
	{	
		//�Է�����һ�׻���
		int count;
		POINT closepoints[225];
		//getclose_small(count, closepoints);
		getclose_twolevelqianmodel(color,count,closepoints);
		
		//�õ��ͶԷ������������
		for(int i=0;i<count;i++)	
		{
			if (closepoints[i].x!=0)	
			{
				ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
				//����һ��
				if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_twolevel_qianmodel(color)) 			
				{		
					//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��				
					tempresult[counter].x = closepoints[i].x;					
					tempresult[counter].y = closepoints[i].y;
					counter++;
					flag = true;
					defent_success = true;
					/*
					if (counter==3)
					{
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
						
						return true;
					}*/				
				}
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ	
			}	
		}
	}
	else defent_success = true;
	return flag;
	
}

BOOL CGoBangGame::has_twolevel_qianmodel_defent(int color,int &counter,POINT tempresult[])
{
	//����ǣ����ƥ��!!
	//�˺��������赲�Է�����һ�׻��ӣ�������ֹ�ķ�ʽ��һ�������ڶԷ���һ�׻����ϣ�����
	//���㷨�����ҳ�������ʹ�Է�һ�׻�����ʧ���ӣ�����ͨ��select_max����ѡ����Щ���ж�
	//������ֵ��ߵĵ�

	counter = 0;
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	BOOL flag = false;
	defent_success = false;
	
	if (has_twolevel_qianmodel(color))	
	{
		//�Է�����һ�׻���
		int count;
		POINT closepoints[225];
		//getclose_small(count, closepoints);
		getclose_twolevelqianmodel(color,count,closepoints);
		//has_qianmodel_addone_defent_low(color,count,closepoints,qiang);
		
		//�õ��ͶԷ������������
		BOOL has_qian = false;//�Ƿ��Ѿ��п����赲��ǣ����
		for(int i=0;i<count;i++)
		{
			if (closepoints[i].x!=0)
			{
				if (is_qian(anticolor,closepoints[i].x,closepoints[i].y))
				{
					//�������ӱ�������Լ���һ��ǣ���ӣ���ô���õ�ס�Է�̫��Ķ�������
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					//����һ��
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel_four(color)&&!has_twolevel_qianmodel(color)) 
					{
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��
						if ((m_grade==5)&&(has_twolevelqianmodel_qiang(color)||has_threelevelqianmodel_qiang(color))) 
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
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
							//�����ǰû�У���ô����
							tempresult[0].x = closepoints[i].x;			
							tempresult[0].y = closepoints[i].y;
							counter = 1;
						}						
						has_qian = true;
						flag = true;
						defent_success = true;
						if (counter==3)
						{
							ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
							return true;
						}						
					}
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
				}
				else if (!has_qian)//û��qian��ʱ�����
				{
					ChessPoints[closepoints[i].x][closepoints[i].y] = anticolor;
					//����һ��		
					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 				
					{				
						//�����ʱ�Է��Ļ�����ʧ����ô����tempresult��						
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
									ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ									
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
								ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
								
								return true;
							}			
						}
					}	
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
				}					
			}	
		}	
	}
	else defent_success = true;
	return flag;
}

BOOL CGoBangGame::is_canfour(int color, int x, int y)
{
	if (ChessPoints[x][y]!=0) return false;//�Ѿ�������
	int model[][9]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����
		//-1��ʾ���⣬1��ʾ��ͬ��ɫ��0��ʾ�գ�2��ʾ(x,y)����λ��
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
		//�ĸ��������		
		for (int j=0;j<model_num;j++)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��
		{
			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}

			if (flag) return true;//ƥ����ĳ��ģʽ
			flag = true;
			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}
			if (flag) return true;//ƥ����ĳ��ģʽ
		}
	}
	return false;	
}

void CGoBangGame::ShowInfo(CString s,BOOL debug)
{
	//������ʾĿǰ�ľ���
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
	//���˵��Ƿ��������5��
	if (ChessPoints[x][y]!=0) return false;//�Ѿ�������

	int model[][9]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����
		//-1��ʾ���⣬1��ʾ��ͬ��ɫ��0��ʾ�գ�2��ʾ(x,y)����λ��
		{-1,-1,1,1,2,1,1,-1,-1},//11*11
		{-1,-1,-1,1,2,1,1,1,-1},//1*111
		{-1,-1,-1,-1,2,1,1,1,1},//*1111		
	};

	int model_num = 3;
	int point_num = 9;
	BOOL flag;
	for (int i=0;i<4;i++)
	{
		//�ĸ��������		
		for (int j=0;j<model_num;j++)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��
		{
			flag = true;
			int k;
			for( k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
					//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}

			if (flag) return true;//ƥ����ĳ��ģʽ
			flag = true;
			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{

				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}
			if (flag) return true;//ƥ����ĳ��ģʽ
		}
	}
	return false;	
}

BOOL CGoBangGame::has_qianmodel(int color, int &counter, POINT tempresult[],BOOL qiang)
{
	//������ǣ��������ĳ��model����ǿ��������
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
	//������ǣ��������ĳ��model����ǿ��������
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
	//���ڵõ�ĳһ�������ĳһ�����ɫ����ҪΪ�˼����ж�index�Ϸ��ԵĴ���
	//moveΪ�ƶ���λ�ƣ�iΪ����ȡֵ0��1��2��3
	//n��ʾtempresult��Ԫ�صĶ���
	//���������ķ���ֵΪ1�������������ķ���ֵ���������ϵ������жϣ���������߽緵��-1

	POINT d[4];
	d[0].x = 1;d[0].y = 0;//�᷽��
	d[1].x = 0;d[1].y = 1;//�ݷ���
	d[2].x = 1;d[2].y = -1;//б����
	d[3].x = 1;d[3].y = 1;//б����
	x = x+move*d[i].x;
	y = y+move*d[i].y;

	if ((x>=1)&&(x<=m_xGrid)&&(y>=1)&&(y<=m_yGrid))
	{
		for(int i=0;i<n;i++)
		{
			if ((tempresult[i].x==x)&&(tempresult[i].y==y))
			{
				//���tempresult�д���(x,y)�㣬��ô����1
				return 1;
			}
		}

		if (ChessPoints[x][y]==0)//������(x,y)��
		{//�������ϵĿ���
			return 0;
		}
		else
		{//���������ϵĿ���
			if (ChessPoints[x][y]==color) return 3;//�˸���ɫ��ͬ
			return 4;//��ɫ��ͬ���൱���赲ɫ
		}
	}
	return -1;//�߽磬����-1
}

BOOL CGoBangGame::can_qianmodel(int color, int x, int y,POINT tempresult[],int n,BOOL qiang)

{
	if (qiang&&!is_canfour(color,x,y)) return false;

	//�����ж�һ�����Ƿ���ǣ�������γ���model

	int model[][11]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����

		//-1��ʾ���⣬1��ʾ��tempresult�еĵ㣬0��ʾ�գ�2��ʾ(x,y)����λ��

		//3��ʾ��ɫ��ͬ�Ĳ���tempresult�еĵ�

		//4��ʾ��ɫ��ͬ�Ĳ���tempresult�еĵ�

		//6��ʾ�Ȳ���1Ҳ����3

		//��ס:ÿ��ģʽ��β���߿�ͷֻ����_,3,6��������1,�����1��Ӧ����6��ס

		//��ʱ��*�൱��1

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
	
	d[0].x = 1;d[0].y = 0;//�᷽��
	
	d[1].x = 0;d[1].y = 1;//�ݷ���
	
	d[2].x = 1;d[2].y = -1;//б����
	
	d[3].x = 1;d[3].y = 1;//б����

	int model_num = 47;

	int point_num = 11;

	BOOL flag;
	int i;
	int k;
	for (i=0;i<4;i++)

	{
		//�ĸ��������		
		for (int j=0;j<model_num;j++)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��
		{
			flag = true;
			for( k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(color,x,y,i,k-5,tempresult,n)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(color,x,y,i,k-5,tempresult,n)!=1)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				

				else if (model[j][k]==3)
				{
					if (getcolor(color,x,y,i,k-5,tempresult,n)!=3)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}		

				else if (model[j][k]==4)
				{
					if (getcolor(color,x,y,i,k-5,tempresult,n)!=4)
					{

						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}		
				else if (model[j][k]==6)
				{

					if (getcolor(color,x,y,i,k-5,tempresult,n)==3)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}

					if (getcolor(color,x,y,i,k-5,tempresult,n)==1)
					{

						//��ƥ���ģʽ
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
					return true;//ƥ����ĳ��ģʽ
				}
				else
				{
					//�ж��Ƿ���ǿ����ǣ����ƥ��
					BOOL tempflag = true;
					int count = 0;
					int k;
					for ( k=0;k<point_num;k++)						
					{						
						if ((model[j][k]==1)&&!is_canfour(color,x+(k-5)*d[i].x,y+(k-5)*d[i].y))							
						{
							//�ҵ�һ������canfour���ӣ���ô�Ͳ���ǿ����ǣ����ƥ��					
							if (j<6)
							{
								tempflag = false;
								break;
							}
							else
							{
								//������һ������canfour
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
						//��ǿ����ǣ����
						return true;//ƥ����ĳ��ģʽ						
					}
				}

			}

			flag = true;

			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(color,x,y,i,5-k,tempresult,n)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(color,x,y,i,5-k,tempresult,n)!=1)
					{

						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				

				else if (model[j][k]==3)
				{
					if (getcolor(color,x,y,i,5-k,tempresult,n)!=3)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}		

				else if (model[j][k]==4)
				{
					if (getcolor(color,x,y,i,5-k,tempresult,n)!=4)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}	

				else if (model[j][k]==6)
				{
					if (getcolor(color,x,y,i,5-k,tempresult,n)==3)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}

					if (getcolor(color,x,y,i,5-k,tempresult,n)==1)
					{
						//��ƥ���ģʽ
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
					return true;//ƥ����ĳ��ģʽ
				}
				else
				{
					//�ж��Ƿ���ǿ����ǣ����ƥ��
					BOOL tempflag = true;
					int count = 0;
					for (int k=0;k<point_num;k++)						
					{						
						if ((model[j][k]==1)&&!is_canfour(color,x+(5-k)*d[i].x,y+(5-k)*d[i].y))							
						{
							//�ҵ�һ������canfour���ӣ���ô�Ͳ���ǿ����ǣ����ƥ��					
							if (j<6)
							{
								tempflag = false;
								break;
							}
							else
							{
								//������һ������canfour
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
						//��ǿ����ǣ����
						return true;//ƥ����ĳ��ģʽ						
					}
				}
			}
		}
	}
	return false;
}

BOOL CGoBangGame::can_qianmodel(int color, int x, int y,POINT tempresult[],int n,int &counter,POINT modelresult[],BOOL qiang)

{

	//qiangΪtrueʱ����ǿ����ǣ����ƥ��

	//�����ж�һ�����Ƿ���ǣ�������γ���model

	//modelresult[]��ʾ��(x,y)��ƥ����Ӽ�

	counter = 0;

	int model[][11]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����

		//-1��ʾ���⣬1��ʾ��tempresult�еĵ㣬0��ʾ�գ�2��ʾ(x,y)����λ��

		//3��ʾ��ɫ��ͬ�Ĳ���tempresult�еĵ�

		//4��ʾ��ɫ��ͬ�Ĳ���tempresult�еĵ�

		//6��ʾ�Ȳ���1Ҳ����3

		//7��ʾ����3

		//��ס:ÿ��ģʽ��β���߿�ͷֻ����_,3,6��������1,�����1��Ӧ����6��ס

		//��ʱ��*�൱��3

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

	d[0].x = 1;d[0].y = 0;//�᷽��

	d[1].x = 0;d[1].y = 1;//�ݷ���

	d[2].x = 1;d[2].y = -1;//б����

	d[3].x = 1;d[3].y = 1;//б����

	int model_num = 47;

	int point_num = 11;

	BOOL flag;
	int i,j,k;
	for ( i=0;i<4;i++)

	{

		//�ĸ��������		

		for ( j=0;j<model_num;j++)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��

		{

			flag = true;

			for( k=0;k<point_num;k++)//point_num���ӱ�����������

			{
				if (model[j][k]==0)
				{
					if (getcolor(color,x,y,i,k-5,tempresult,n)!=0)
					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}

				else if (model[j][k]==1)

				{

					if (getcolor(color,x,y,i,k-5,tempresult,n)!=1)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}				

				else if (model[j][k]==3)

				{

					if (getcolor(color,x,y,i,k-5,tempresult,n)!=3)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}	

				else if (model[j][k]==4)
				{
					if (getcolor(color,x,y,i,k-5,tempresult,n)!=4)
					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}	

				else if (model[j][k]==6)

				{

					if (getcolor(color,x,y,i,k-5,tempresult,n)==3)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

					if (getcolor(color,x,y,i,k-5,tempresult,n)==1)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}	
				else if (model[j][k]==7)
					
				{
					
					if (getcolor(color,x,y,i,k-5,tempresult,n)==3)
						
					{
						
						//��ƥ���ģʽ
						
						flag = false;
						
						break;
						
					}				
					
				}	

			}

			if (flag) 

			{

				//ƥ����ĳ��ģʽ	

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
							//һ��ƥ�����						
							modelresult[counter].x = x+(k-5)*d[i].x;						
							modelresult[counter].y = y+(k-5)*d[i].y;							
							counter++;												
						}	
					}				
				}
				else
				{
					//�ж��Ƿ���ǿ����ǣ����ƥ��
					int count = 0;
					BOOL tempflag = true;
					for (int k=0;k<point_num;k++)						
					{						
						if ((model[j][k]==1)&&!is_canfour(color,x+(k-5)*d[i].x,y+(k-5)*d[i].y))							
						{
							//�ҵ�һ������canfour���ӣ���ô�Ͳ���ǿ����ǣ����ƥ��
							if (j<6)
							{
								tempflag = false;
								break;
							}
							else
							{
								//������һ������canfour
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
						//��ǿ����ǣ����
						for (int k=0;k<point_num;k++)
							
						{
							
							if (model[j][k]==1)
								
							{
								
								//һ��ƥ�����
								
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

			for(k=0;k<point_num;k++)//point_num���ӱ�����������

			{

				if (model[j][k]==0)

				{

					if (getcolor(color,x,y,i,5-k,tempresult,n)!=0)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}

				else if (model[j][k]==1)

				{

					if (getcolor(color,x,y,i,5-k,tempresult,n)!=1)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}				

				else if (model[j][k]==3)

				{

					if (getcolor(color,x,y,i,5-k,tempresult,n)!=3)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}	

				else if (model[j][k]==4)

				{

					if (getcolor(color,x,y,i,5-k,tempresult,n)!=4)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}	

				else if (model[j][k]==6)

				{

					if (getcolor(color,x,y,i,5-k,tempresult,n)==3)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

					if (getcolor(color,x,y,i,5-k,tempresult,n)==1)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}	

				else if (model[j][k]==7)
					
				{
					
					if (getcolor(color,x,y,i,5-k,tempresult,n)==3)
						
					{
						
						//��ƥ���ģʽ
						
						flag = false;
						
						break;
						
					}					
					
				}	

			}

			if (flag) 

			{

				//ƥ����ĳ��ģʽ

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
							
							//һ��ƥ�����
							
							modelresult[counter].x = x+(5-k)*d[i].x;
							
							modelresult[counter].y = y+(5-k)*d[i].y;
							
							counter++;						
							
						}
						
					}				
				}
				else
				{
					//�ж��Ƿ���ǿ����ǣ����ƥ��
					BOOL tempflag = true;
					int count = 0;
					for (int k=0;k<point_num;k++)						
					{						
						if ((model[j][k]==1)&&!is_canfour(color,x+(5-k)*d[i].x,y+(5-k)*d[i].y))							
						{
							//�ҵ�һ������canfour���ӣ���ô�Ͳ���ǿ����ǣ����ƥ��					
							if (j<6)
							{
								tempflag = false;
								break;
							}
							else
							{
								//������һ������canfour
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
						//��ǿ����ǣ����
						for (int k=0;k<point_num;k++)
							
						{
							
							if (model[j][k]==1)
								
							{
								
								//һ��ƥ�����
								
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
	//��has_qianmodel��ͬ���ǣ���������һ���Ѿ��µ�����ƥ��model
	//��has_qianmodelֻ������ǣ������ƥ��
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
	//��has_qianmodel��ͬ���ǣ���������һ���Ѿ��µ�����ƥ��model
	//��has_qianmodelֻ������ǣ������ƥ��
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
	//�˺���������������ǣ����ƥ�����
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����
	counter = 0;
	BOOL flag = false;	//����ֵ
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
					
					//�������ƥ���ǣ���ӣ���¼�������					
					tempresult[counter].x = closepoints[i].x;					
					tempresult[counter].y = closepoints[i].y;					
					counter++;					
					flag = true;			
					if (m_grade<5)
					{
						ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ				
						return true;
					}					
				}				
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ	
			}			
		}
	}
	return flag;
}

BOOL CGoBangGame::make_threelevel_qianmodel(int color, int &counter, POINT tempresult[])
{
	//�˺���������������ǣ����ƥ�����
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����
	counter = 0;
	BOOL flag = false;	//����ֵ
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
					//�������ƥ�������ǣ���ӣ���¼�������
					tempresult[counter].x = closepoints[i].x;
					tempresult[counter].y = closepoints[i].y;
					counter++;
					flag = true;
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ
					return true;
				}
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ	
			}			
		}
	}
	return flag;
}


BOOL CGoBangGame::make_twolevel_qianmodel(int color, int &counter, POINT tempresult[])
{
	//�˺���������������ǣ����ƥ�����
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����
	counter = 0;
	BOOL flag = false;	//����ֵ
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

					//�������ƥ��Ķ���ǣ���ӣ���¼�������
					tempresult[counter].x = closepoints[i].x;
					tempresult[counter].y = closepoints[i].y;
					counter++;
					flag = true;
					ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ
					return true;
				}
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ
			}
		}
	}
	return flag;
}

BOOL CGoBangGame::make_beatpoints(int color, int &counter, POINT tempresult[])
{	
	//�˺������������������ӵ���
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����	
	counter = 0;	
	BOOL flag = false;	//����ֵ	
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
				//�������ƥ��Ĵ���ӣ���¼�������				
				tempresult[counter].x = closepoints[i].x;					
				tempresult[counter].y = closepoints[i].y;					
				counter++;					
				flag = true;
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ					
				return true;					
			}			
			ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ			
		}
	}
	return flag;
}

BOOL CGoBangGame::twolevel_four(int x, int y, int color)
{
	//�ж��Ƿ���׻����к���4���ӵ������������У���ôӦ�����ȼ����˵�һ�׻��Ӹ�
	if (ChessPoints[x][y]!=0) return false;//�Ѿ�������
	int model[][9]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����
		//-1��ʾ���⣬1��ʾ��ͬ��ɫ��0��ʾ�գ�2��ʾ(x,y)����λ��
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

	};//17�ֿ�����ɻ��ӵ�����

	int model_num = 17;
	int point_num = 9;
	BOOL flag;
	int count = 0;//�ۼ��ж��ٸ����������ģʽ��ƥ��
	BOOL existfour = false;//�ж��Ƿ���һ�������ϴ�������

	int i,j,k;
	for ( i=0;i<4;i++)
	{
		//�ĸ��������		
		for ( j=0;j<model_num;j++)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��
		{
			flag = true;
			for( k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-4,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}

			if (flag) 
			{
				//ƥ����ĳ��ģʽ
				count++;
				if (j<=9) existfour = true;
				break;//�˷���Ͳ������ۼ���
			}

			flag = true;
			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,4-k,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}

			if (flag) 
			{
				//ƥ����ĳ��ģʽ
				count++;
				if (j<=9) existfour = true;
				break;//�˷���Ͳ������ۼ���
			}
		}
	}

	if (count>=2) return existfour;
	return false;
}



BOOL CGoBangGame::has_twolevel_qianmodel(int color, int &counter, POINT tempresult[])

{

	//�˺����ж��Ƿ���ڶ���ǣ����ƥ��

	//�˶������£������ǣ���ӣ���������ô�ߣ�������ǣ����ƥ�䣬��ô��ǣ����Ϊ����ǣ����

	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����

	int anticolor;//�Է���color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	counter = 0;

	BOOL flag = false;	//����ֵ

	POINT qianChessPoints[225];

	int count = 0;

	has_qian(color, count, qianChessPoints);	

	for(int i=0;i<count;i++)

	{

		if (qianChessPoints[i].x!=0)

		{

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = color;

			BOOL tempflag = true;//�߶Է����ӣ������Ƿ񶼴���ǣ����ƥ��

			POINT roundpoints[225];
			int defentcount = 0;
			has_defent_point(color,defentcount,roundpoints);

			for(int j=0;j<defentcount;j++)

			{

				if (roundpoints[j].x!=0)//������Ч��λ��

				{
					
					if ((m_grade==5)&&is_beatpoint(anticolor,roundpoints[j].x,roundpoints[j].y))						
					{
						tempflag = false;
						
						break;//�Է���beatpoint���赲�ң���ô�Ͳ���
					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = anticolor;//�߶Է�����

					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)) 

					{

						//�������һ�����������ǣ����ƥ�䣬��ôʧ��

						ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

						tempflag = false;

						break;

					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

				}

			}

			if (tempflag)

			{

				//�Ƕ���ǣ����

				tempresult[counter].x = qianChessPoints[i].x;

				tempresult[counter].y = qianChessPoints[i].y;

				counter++;

				flag = true;



			}

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ

		}

	}

	return flag;

}



BOOL CGoBangGame::has_threelevel_qianmodel(int color, int &counter, POINT tempresult[])

{

	//�˺����ж��Ƿ��������ǣ����ƥ��

	//�˶������£������ǣ���ӣ���������ô�ߣ������ڶ���ǣ����ƥ�䣬��ô��ǣ����Ϊ����ǣ����

	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����

	int anticolor;//�Է���color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	counter = 0;

	BOOL flag = false;	//����ֵ

	POINT qianChessPoints[225];

	int count = 0;

	has_qian(color, count, qianChessPoints);	

	for(int i=0;i<count;i++)

	{

		if (qianChessPoints[i].x!=0)

		{

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = color;

			BOOL tempflag = true;//�߶Է����ӣ������Ƿ񶼴���ǣ����ƥ��

			POINT roundpoints[225];
			int defentcount = 0;
			has_defent_point(color,defentcount,roundpoints);
			
			for(int j=0;j<defentcount;j++)

			{

				if (roundpoints[j].x!=0)//������Ч��λ��

				{
					if ((m_grade==5)&&is_beatpoint(anticolor,roundpoints[j].x,roundpoints[j].y))						
					{
						tempflag = false;
						
						break;//�Է���beatpoint���赲�ң���ô�Ͳ���
					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = anticolor;//�߶Է�����

					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 

					{

						//�������һ�����������ǣ����ƥ�䣬��ôʧ��

						ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

						tempflag = false;

						break;

					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

				}

			}

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ

			if (tempflag)

			{

				//������ǣ����

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

	//�˺����ж��Ƿ���ڶ���ǣ����ƥ��

	//�˶������£������ǣ���ӣ���������ô�ߣ�������ǣ����ƥ�䣬��ô��ǣ����Ϊ����ǣ����

	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����

	int anticolor;//�Է���color

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

			BOOL tempflag = true;//�߶Է����ӣ������Ƿ񶼴���ǣ����ƥ��

			POINT roundpoints[225];
			int defentcount = 0;
			has_defent_point(color,defentcount,roundpoints);
			
			for(int j=0;j<defentcount;j++)

			{

				if (roundpoints[j].x!=0)//������Ч��λ��

				{

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = anticolor;//�߶Է�����

					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)) 

					{

						//�������һ�����������ǣ����ƥ�䣬��ôʧ��						

						ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

						tempflag = false;

						break;

					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

				}

			}

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ

			if (tempflag)

			{

				//�Ƕ���ǣ����

				return true;

				

			}			

		}

	}

	return false;

}



BOOL CGoBangGame::has_threelevel_qianmodel(int color)

{

	//�˺����ж��Ƿ��������ǣ����ƥ��

	//�˶������£������ǣ���ӣ���������ô�ߣ������ڶ���ǣ����ƥ�䣬��ô��ǣ����Ϊ����ǣ����

	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����

	int anticolor;//�Է���color

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

			BOOL tempflag = true;//�߶Է����ӣ������Ƿ񶼴���ǣ����ƥ��

			POINT roundpoints[225];
			int defentcount = 0;
			has_defent_point(color,defentcount,roundpoints);
			
			for(int j=0;j<defentcount;j++)

			{

				if (roundpoints[j].x!=0)//������Ч��λ��

				{

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = anticolor;//�߶Է�����

					if (!can_five(color)&&!has_alive(color)&&!has_twolevel(color)&&!has_qianmodel(color)&&!has_qianmodel_addone(color)&&!has_twolevel_qianmodel(color)) 

					{

						//�������һ����������ڶ���ǣ����ƥ�䣬��ôʧ��

						ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

						tempflag = false;

						break;

					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

				}

			}

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ

			if (tempflag)

			{

				//������ǣ����

				return true;

				

			}			

		}

	}

	return false;

}



BOOL CGoBangGame::has_twolevel_beat(int color, int &counter, POINT tempresult[])

{

	//�˺����ж��Ƿ���ڶ��״����

	//�˶������£������ǣ���ӣ���������ô�ߣ������ڶ���ǣ����ƥ�䣬��ô��ǣ����Ϊ����ǣ����

	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����

	int anticolor;//�Է���color

	if (color==1) anticolor = 2;

	else anticolor = 1;

	counter = 0;

	BOOL flag = false;	//����ֵ

	POINT qianChessPoints[225];

	int count = 0;

	has_qian(color, count, qianChessPoints);	

	for(int i=0;i<count;i++)

	{

		if (qianChessPoints[i].x!=0)

		{

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = color;

			BOOL tempflag = true;//�߶Է����ӣ������Ƿ񶼴���ǣ����ƥ��

			POINT roundpoints[225];
			int defentcount = 0;
			has_defent_point(color,defentcount,roundpoints);
			
			for(int j=0;j<defentcount;j++)

			{

				if (roundpoints[j].x!=0)//������Ч��λ��

				{

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = anticolor;//�߶Է�����

					if (!has_beatpoints_low(color)) 

					{

						//�������һ����������ڶ��״���ӣ���ôʧ��

						ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

						tempflag = false;

						break;

					}

					ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;

				}

			}

			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ

			if (tempflag)

			{

				//�Ƕ��״����

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
	//�˺����ж��Ƿ���ڶ��״����
	//�˶������£������ǣ���ӣ���������ô�ߣ������ڶ���ǣ����ƥ�䣬��ô��ǣ����Ϊ����ǣ����
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����
	
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	
	counter = 0;
	BOOL flag = false;	//����ֵ
	POINT qianChessPoints[225];
	int count = 0;
	has_qian(color, count, qianChessPoints);	
	for(int i=0;i<count;i++)	
	{	
		if (qianChessPoints[i].x!=0)		
		{			
			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = color;
			BOOL tempflag = true;//�߶Է����ӣ������Ƿ񶼴���ǣ����ƥ��	
			POINT roundpoints[225];
			int defentcount = 0;
			has_defent_point(color,defentcount,roundpoints);		
			for(int j=0;j<defentcount;j++)		
			{
				if (roundpoints[j].x!=0)//������Ч��λ��	
				{
					ChessPoints[roundpoints[j].x][roundpoints[j].y] = anticolor;//�߶Է�����
					if (!has_beatpoints_low(color)) 
					{
						//�������һ����������ڶ��״���ӣ���ôʧ��
						ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;
						tempflag = false;
						break;
					}
					ChessPoints[roundpoints[j].x][roundpoints[j].y] = 0;
				}
			}
			ChessPoints[qianChessPoints[i].x][qianChessPoints[i].y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ
			if (tempflag)
			{
				//�Ƕ��״����
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
	//��is_makealive��ǿ����defent_point�����ǶԷ��ĳ�����
	if (!is_makealive(x,y,color)) return false;
	int anticolor;//�Է���color	
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

	//�ж��Ƿ��ǿ�������һ�׻��ӵĵ�

	if (ChessPoints[x][y]!=0) return false;//�Ѿ�������

	int model[][9]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����

		//-1��ʾ���⣬1��ʾ��ͬ��ɫ��0��ʾ�գ�2��ʾ(x,y)����λ��

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

		//�ĸ��������		

		for ( j=0;j<model_num;j++)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��

		{

			flag = true;

			for( k=0;k<point_num;k++)//point_num���ӱ�����������

			{

				if (model[j][k]==0)

				{

					if (getcolor(x,i,k-4,y)!=0)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}

				else if (model[j][k]==1)

				{

					if (getcolor(x,i,k-4,y)!=color)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}				

			}

			if (flag) return true;//ƥ����ĳ��ģʽ

			flag = true;

			for(k=0;k<point_num;k++)//point_num���ӱ�����������

			{

				if (model[j][k]==0)

				{

					if (getcolor(x,i,4-k,y)!=0)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}

				else if (model[j][k]==1)

				{

					if (getcolor(x,i,4-k,y)!=color)

					{

						//��ƥ���ģʽ

						flag = false;

						break;

					}

				}				

			}

			if (flag) return true;//ƥ����ĳ��ģʽ

		}

	}

	return false;

}



CString CGoBangGame::passtime(int &cur_time)
{
	//ͳ���õ�ʱ�䣬cur_timeΪ�ϴμ�ʱʱ��ʱ����㣬������s��Ϊ��λ���ַ���
	//����cur_time��Ϊ��ǰʱ��
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

	//���ڽ�target�е�subChessPoints��ȥ��

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
				//���ڴ˵㣬Ӧ��ɾȥ
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

	//���ڵõ�����ǣ���ӵ�����ӣ���Ϊmake�㷨��Ҫ������

	counter = 0;

	getclose(color,counter,tempresult);	

	POINT qianChessPoints[225];

	int qiancount;

	//��Ȼǣ���Ӷ�Ӧ�ñ�ȥ��������canfour���ӻ�����һ�����ƣ��������
	//�������֣�������!!Ҳ�����������и��߽׵������ж�

	if (has_qian(color,qiancount,qianChessPoints))

		sub(tempresult,counter,qianChessPoints,qiancount);

		

}

void CGoBangGame::getclose_small_subqian(int color,int &counter, POINT tempresult[])

{
	
	//���ڵõ�����ǣ���ӵ�����ӣ���Ϊmake�㷨��Ҫ������
	
	counter = 0;
	
	getclose_small(counter,tempresult);	
	
	POINT qianChessPoints[225];
	
	int qiancount;
	
	//��Ȼǣ���Ӷ�Ӧ�ñ�ȥ��������canfour���ӻ�����һ�����ƣ��������
	//�������֣�������!!Ҳ�����������и��߽׵������ж�
	
	if (has_qian(color,qiancount,qianChessPoints))
		
		sub(tempresult,counter,qianChessPoints,qiancount);	
	
}



void CGoBangGame::selectqianmodel(int color, int &x, int &y, POINT tempresult[],int n)

{

	//�������ֻ����defent����Ҫ�������ں�ѡ�������ҳ�make_qianmodel���ӣ��ٽ�������

	if (n==1)
	{
		//ֻ��һ���㣬��ôֱ�ӷ���
		x = tempresult[0].x;
		y = tempresult[0].y;
		return;
	}

	POINT local_tempresult[225];

	int local_count = 0;

	BOOL flag = false;//�Ƿ�������makeqianmodel

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

		//û�н�������ô����ԭ���ļƻ�����

		selectless_twomodel(color,x,y,tempresult,n);

	}

	else

	{

		//�н�������ô�����������ѡȡ

		selectless_twomodel(color,x,y,local_tempresult,local_count);

	}	

}



void CGoBangGame::selectqianmodel3(int color, int &x, int &y, POINT tempresult[],int n)
{
	//ֻ����grade=3
	//�������ֻ����defent����Ҫ�������ں�ѡ�������ҳ�make_qianmodel���ӣ��ٽ�������
	if (n==1)
	{
		//ֻ��һ���㣬��ôֱ�ӷ���
		x = tempresult[0].x;
		y = tempresult[0].y;
		return;
	}

	POINT local_tempresult[225];
	int local_count = 0;
	BOOL flag = false;//�Ƿ�������makeqianmodel
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
		//û�н�������ô����ԭ���ļƻ�����
		selectless_twomodel(color,x,y,tempresult,n);
	}
	else
	{
		//�н�������ô�����������ѡȡ
		selectless_twomodel(color,x,y,local_tempresult,local_count);
	}	
}

void CGoBangGame::selectqianmodel5(int color, int &x, int &y, POINT tempresult[],int n,BOOL defent)
{
	//ֻ����grade=5ʱ
	//�������ֻ����defent����Ҫ�������ں�ѡ�������ҳ�make_qianmodel���ӣ��ٽ�������
	if (!defent)
		cut_dangerpoints(color,tempresult,n);
	if (n==1)
	{
		//ֻ��һ���㣬��ôֱ�ӷ���
		x = tempresult[0].x;
		y = tempresult[0].y;
		return;
	}

	POINT local_tempresult[225];
	int local_count = 0;
	BOOL flag = false;//�Ƿ�������makeqianmodel
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
		//û�н�������ô����ԭ���ļƻ�����
		selectchongtwo(color,x,y,tempresult,n);
	}
	else
	{
		//�н�������ô�����������ѡȡ
		selectchongtwo(color,x,y,local_tempresult,local_count);
	}	
}

void CGoBangGame::selectchongtwo(int color, int &x, int &y, POINT tempresult[],int n)

{
	
	//ֻ����grade=5ʱ
	
	//�������ֻ����defent����Ҫ�������ں�ѡ�������ҳ�make_qianmodel���ӣ��ٽ�������
	
	if (n==1)
	{
		//ֻ��һ���㣬��ôֱ�ӷ���
		x = tempresult[0].x;
		y = tempresult[0].y;
		return;
	}
	
	POINT local_tempresult[225];
	
	int local_count = 0;
	
	BOOL flag = false;//�Ƿ�������makeqianmodel
	
	
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
		
		//û�н�������ô����ԭ���ļƻ�����
		
		selectless_twomodel(color,x,y,tempresult,n);
		
	}
	
	else
		
	{
		
		//�н�������ô�����������ѡȡ
		
		selectless_twomodel(color,x,y,local_tempresult,local_count);
		
	}	
	
}



void CGoBangGame::selectless_twomodel(int color, int &x, int &y, POINT tempresult[],int n)

{

	//�����������selectqianmodel֮����Ҫ����˶Է�����ʱ��ֵ�㷨������ʵ���ֵ�����

	//��Ҫ�㷨���£�

	//1����ĳһ����ѡ�ӣ�����ʱ�Է���twomodel�ĸ���

	//2���ָ����п���

	//3��ѡ��twomodel�ĸ�����С�ĺ�ѡ�ӽ�������Ĳ���	

	if (n==1)
	{
		//ֻ��һ���㣬��ôֱ�ӷ���
		x = tempresult[0].x;
		y = tempresult[0].y;
		return;
	}

	int local_count;

	POINT local_point[225];

	int min_twomodel = 225;

	int anticolor;//�Է���color

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
			
			//�����ѡ�����
			
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
	//�ж�ƽ��
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

	//�ж�ĳ���Ƿ�����γ�qianmodel

	if (is_qian(color,x,y)) return false;
	if (has_qianmodel(color)||has_qianmodel_addone(color)) return false;

	BOOL flag = false;

	//if (can_maketwolevel_alive(color,x,y)) return false;

	ChessPoints[x][y] = color;

	if (has_qianmodel(color)||has_qianmodel_addone(color)) 

	{

		flag = true;

	}

	ChessPoints[x][y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ

	return flag;

}



BOOL CGoBangGame::can_makeqianmodel3(int color, int x, int y)

{

	//�ж�ĳ���Ƿ�����γ�qianmodel

	if (has_qianmodel(color)) return false;

	BOOL flag = false;

	ChessPoints[x][y] = color;

	if (has_qianmodel(color)) 

	{

		flag = true;

	}

	ChessPoints[x][y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ

	return flag;

}



BOOL CGoBangGame::can_maketwolevelqianmodel(int color, int x, int y)

{

	//�ж�ĳ���Ƿ�����γ�twolevelqianmodel

	if (is_qian(color,x,y)) return false;

	BOOL flag = false;

	if (can_maketwolevel_alive(color,x,y)||can_makeqianmodel(color,x,y)) return false;

	if (has_twolevel_qianmodel(color)) return false;

	ChessPoints[x][y] = color;

	if (has_twolevel_qianmodel(color))

	{

		flag = true;

	}

	ChessPoints[x][y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ

	return flag;

}



BOOL CGoBangGame::can_makethreelevelqianmodel(int color, int x, int y)

{

	//�ж�ĳ���Ƿ�����γ�threelevelqianmodel

	if (is_qian(color,x,y)) return false;

	BOOL flag = false;

	if (can_maketwolevel_alive(color,x,y)||can_makeqianmodel(color,x,y)||can_maketwolevelqianmodel(color,x,y)) return false;

	if (has_threelevel_qianmodel(color)) return false;

	ChessPoints[x][y] = color;

	if (has_threelevel_qianmodel(color))

	{

		flag = true;

	}

	ChessPoints[x][y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ

	return flag;

}



BOOL CGoBangGame::can_maketwolevel_alive(int color, int x, int y)

{

	//�ж�ĳ���Ƿ�����γ�twolevel_alive
	if (has_twolevel(color)) return false;

	BOOL flag = false;

	ChessPoints[x][y] = color;

	if (has_twolevel(color))

	{

		flag = true;

	}

	ChessPoints[x][y] = 0;//�ָ�ԭ״���������������������㷨����Ϊû��ͳһ��ģʽ

	return flag;

}



BOOL CGoBangGame::is_qian(int color, int x, int y)

{

	//�Ƿ���ǣ����

	//ȫ��ǣ���ӣ�����һ�׻��ӡ����׻��Ӻ����ĵ��ӣ��Լ�����һ�׻��ӵ���

	if (is_alive(x,y,color)||twolevel_alive(x,y,color)||is_canfour(color,x,y)||is_makealive(x,y,color))

		return true;

	return false;

}



BOOL CGoBangGame::is_qiannofour(int color, int x, int y)

{

	//�Ƿ���ǣ����

	//����ǣ���ӣ�����һ�׻��ӡ����׻����Լ�����һ�׻��ӵ���

	//���������ĵ���

	if (is_alive(x,y,color)||twolevel_alive(x,y,color)||is_makealive(x,y,color))

		return true;
	return false;
}

void CGoBangGame::setGrade(int grade)
{
	//�趨���Ե������ȼ�
	m_grade = grade;
}

BOOL CGoBangGame::has_beatpoints(int color, int &counter, POINT tempresult[])

{
	//����㷨ֻ����grade=5�У���Ҫ���öԷ��������壬��grade4������ģ�¶Է�����
	//����㷨���£��ҵ������������ӣ��ߴ��ӣ���Է������ʱ��isdanger=true����ô����Ӽ�ΪbeatChessPoints
	counter = 0;

	int anticolor;//�Է���color

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
			ChessPoints[closepoints[i].x][closepoints[i].y] = color;//����һ��
			isdanger = false;
			is_danger(anticolor);
			if (isdanger)
			{
				//�����ʱ�Է�������������ô����tempresult��
				tempresult[counter].x = closepoints[i].x;
				tempresult[counter].y = closepoints[i].y;
				counter++;
				flag = true;
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
				return true;
			}
			ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
		}
	}
	return flag;
}

BOOL CGoBangGame::has_beatpoints_low(int color, int &counter, POINT tempresult[])
{	
	//����㷨ֻ����grade=5�У���Ҫ���öԷ��������壬��grade4������ģ�¶Է�����	
	//����㷨���£��ҵ������������ӣ��ߴ��ӣ���Է������ʱ��isdanger=true����ô����Ӽ�ΪbeatChessPoints	
	counter = 0;	
	int anticolor;//�Է���color
	
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
			
			ChessPoints[closepoints[i].x][closepoints[i].y] = color;//����һ��
			
			isdanger = false;
			
			is_danger(anticolor);
			
			if (isdanger)
				
			{
				
				//�����ʱ�Է�������������ô����tempresult��
				
				tempresult[counter].x = closepoints[i].x;
				
				tempresult[counter].y = closepoints[i].y;
				
				counter++;
				
				flag = true;
				
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
				
				return true;
				
			}
			
			ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
			
		}
		
	}
	
	return flag;
	
}

BOOL CGoBangGame::has_beatpoints_low(int color)
{
	
	//����㷨ֻ����grade=5�У���Ҫ���öԷ��������壬��grade4������ģ�¶Է�����
	
	//����㷨���£��ҵ������������ӣ��ߴ��ӣ���Է������ʱ��isdanger=true����ô����Ӽ�ΪbeatChessPoints
	
	
	int anticolor;//�Է���color
	
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
			
			ChessPoints[closepoints[i].x][closepoints[i].y] = color;//����һ��
			
			isdanger = false;
			
			is_danger(anticolor);
			
			if (isdanger)
				
			{
				
				//�����ʱ�Է�������������ô����tempresult��
				
				flag = true;
				
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
				
				return true;
				
			}
			
			ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
			
		}
		
	}
	
	return flag;
	
}

BOOL CGoBangGame::has_beatpoints(int color)
{
	//����㷨ֻ����grade=5�У���Ҫ���öԷ��������壬��grade4������ģ�¶Է�����
	//����㷨���£��ҵ������������ӣ��ߴ��ӣ���Է������ʱ��isdanger=true����ô����Ӽ�ΪbeatChessPoints
	int anticolor;//�Է���color
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
			ChessPoints[closepoints[i].x][closepoints[i].y] = color;//����һ��
			isdanger = false;
			is_danger(anticolor);
			if (isdanger)
			{
				//�����ʱ�Է�������������ô����tempresult��				
				flag = true;
				ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
				return true;
			}
			ChessPoints[closepoints[i].x][closepoints[i].y] = 0;//�ָ�ԭֵ
		}
	}
	return flag;
}

void CGoBangGame::is_danger(int color)
{
	//�����ж��Ƿ���Σ����
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
	if (has_five(1)||has_five(2)) return;//�Ѿ�����5�ӣ�ʲô��������
	int x,y;	
	if (can_five(color,x,y)) 		
	{		
		return;//���������5�ӣ���ô����	
	}
	
	if (can_five(anticolor,x,y)) 	
	{		
		return;//���Է��ܷ�����5��	
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
		//���Է��Ƿ���ڻ���
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
		//����һ���������ӵĶ��׻���
		return;
	}
	if (!defent_success)
	{
		//���ܷ�ס�Է���ǿ���׻���,����
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
		//���Է��Ƿ���ڶ��׻���
		return;
	}	
	if (!defent_success)
	{
		//���ܷ�ס�Է��Ķ��׻���,����
		isdanger = true;
		return;
	}
}

BOOL CGoBangGame::make_twoalive(int color, int &counter, POINT tempresult[])
{
	//�ж�һ��������û�п���һ�������ɶ��һ�׻��ӵĵ�
	//counterΪ��ѡ����ļ�����tempresultΪ��ѡ�����
	counter = 0;
	BOOL flag = false;	//����ֵ
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
	//ǿ����ǣ����ƥ�䣬��canfour���ӣ���ס���ӣ������ʱ����ǿ����ǣ���ӻ���ǿǣ����ƥ��
	//��ô��Ϊǿ����ǣ����ƥ��
	counter = 0;
	int anticolor;//�Է���color	
	if (color==1) anticolor = 2;	
	else anticolor = 1;
	int count;
	POINT localChessPoints[225];
	BOOL flag = false;
	if (has_canfour(color,count,localChessPoints))
	{
		for (int i=0;i<count;i++)
		{
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = color;//��һ��
			int x,y;
			can_five(color,x,y);//x,yΪ�Է������ӵ���
			ChessPoints[x][y] = anticolor;//����һ��
			if (can_five(color)||has_alive(color)||has_twolevel_four(color)||has_qianmodel(color,true)||has_qianmodel_addone(color,true))
			{
				//�����ʱ����ǿһ��ǣ���ӣ���ô����Ӽ�Ϊǿ����ǣ����
				tempresult[counter].x = localChessPoints[i].x;				
				tempresult[counter].y = localChessPoints[i].y;				
				counter++;
				flag = true;
			}
			ChessPoints[x][y] = 0;//�ָ�һ��
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//�ٻָ�һ��
		}
	}
	return flag;
}

BOOL CGoBangGame::has_twolevelqianmodel_qiang(int color)
{
	//ǿ����ǣ����ƥ�䣬��canfour���ӣ���ס���ӣ������ʱ����ǿ����ǣ���ӻ���ǿǣ����ƥ��
	//��ô��Ϊǿ����ǣ����ƥ��
	int anticolor;//�Է���color	
	if (color==1) anticolor = 2;	
	else anticolor = 1;
	int count;
	POINT localChessPoints[225];
	BOOL flag = false;
	if (has_canfour(color,count,localChessPoints))
	{
		for (int i=0;i<count;i++)
		{
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = color;//��һ��
			int x,y;
			can_five(color,x,y);//x,yΪ�Է������ӵ���
			ChessPoints[x][y] = anticolor;//����һ��
			if (can_five(color)||has_alive(color)||has_twolevel_four(color)||has_qianmodel(color,true)||has_qianmodel_addone(color,true))
			{
				//�����ʱ����ǿһ��ǣ���ӣ���ô����Ӽ�Ϊǿ����ǣ����				
				flag = true;
				ChessPoints[x][y] = 0;//�ָ�һ��
				ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//�ٻָ�һ��
				return true;
			}
			ChessPoints[x][y] = 0;//�ָ�һ��
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//�ٻָ�һ��
		}
	}
	return flag;
}

BOOL CGoBangGame::has_threelevelqianmodel_qiang(int color, int &counter, POINT tempresult[])
{
	//ǿ����ƥ�䣬��һ��canfour����ס�����Ƿ����ǿ����ƥ��
	counter = 0;
	int anticolor;//�Է���color	
	if (color==1) anticolor = 2;	
	else anticolor = 1;
	int count;
	POINT localChessPoints[225];
	BOOL flag = false;
	if (has_canfour(color,count,localChessPoints))
	{
		for (int i=0;i<count;i++)
		{
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = color;//��һ��
			int x,y;
			can_five(color,x,y);//x,yΪ�Է������ӵ���
			ChessPoints[x][y] = anticolor;//����һ��
			if (has_twolevelqianmodel_qiang(color))
			{
				//�����ʱ����ǿ����ǣ���ӣ���ô����Ӽ�Ϊǿ����ǣ����
				tempresult[counter].x = localChessPoints[i].x;				
				tempresult[counter].y = localChessPoints[i].y;				
				counter++;
				flag = true;
				ChessPoints[x][y] = 0;//�ָ�һ��
				ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//�ٻָ�һ��
				return true;
			}
			ChessPoints[x][y] = 0;//�ָ�һ��
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//�ٻָ�һ��
		}
	}
	return flag;
}

BOOL CGoBangGame::has_threelevelqianmodel_qiang(int color)
{
	//ǿ����ƥ�䣬��һ��canfour����ס�����Ƿ����ǿ����ƥ��
	int anticolor;//�Է���color	
	if (color==1) anticolor = 2;	
	else anticolor = 1;
	int count;
	POINT localChessPoints[225];
	BOOL flag = false;
	if (has_canfour(color,count,localChessPoints))
	{
		for (int i=0;i<count;i++)
		{
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = color;//��һ��
			int x,y;
			can_five(color,x,y);//x,yΪ�Է������ӵ���
			ChessPoints[x][y] = anticolor;//����һ��
			if (has_twolevelqianmodel_qiang(color))
			{
				//�����ʱ����ǿ����ǣ���ӣ���ô����Ӽ�Ϊǿ����ǣ����
				flag = true;
				ChessPoints[x][y] = 0;//�ָ�һ��
				ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//�ٻָ�һ��
				return true;
			}
			ChessPoints[x][y] = 0;//�ָ�һ��
			ChessPoints[localChessPoints[i].x][localChessPoints[i].y] = 0;//�ٻָ�һ��
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
			//��������xx_x�͵�
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
	//�ж��Ƿ���һ��ǣ����
	if (!is_qian(color,x,y)) return false;//����Ҫ��ǣ����
	POINT qian[225];
	int qian_count;	
	has_qian(color,qian_count,qian);
	if (qian_count>0)
	{
		if (can_qianmodel(color,x,y,qian,qian_count)) return true;//��ǣ����
		POINT addoneqian[225];
		int addoneqian_count;
		for (int i=1;i<m_xGrid;i++)
		{
			for (int j=1;j<=m_yGrid;j++)
			{
				if ((ChessPoints[i][j]==color)&&can_qianmodel(color,i,j,qian,qian_count,addoneqian_count,addoneqian))
				{
					//����ǣ����
					for (int k=0;k<addoneqian_count;k++)
					{
						if ((addoneqian[k].x==x)&&(addoneqian[k].y==y))
						{
							//�����ڲ���ǣ������
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
	//�ҵ�ǿ����ӣ�����һ��canfour�ӣ��öԷ����ӵ�ס����ʱ�������ڲ�����beat��
	counter = 0;
	if (!has_canfour(color)||has_beatpoints_low(color)) return false;//�Ѿ���ǿ�����
	int anticolor;//�Է���color	
	if (color==1) anticolor = 2;	
	else anticolor = 1;
	int count;
	POINT localChessPoints[225];
	BOOL flag = false;
	if (has_canfour(color,count,localChessPoints))
	{
		int x1,y1;
		selectqianmodel(color,x1,y1,localChessPoints,count);//�˴���װѡ�����������ߵ����ӣ�������ȫ���ж�		
		ChessPoints[x1][y1] = color;//��һ��
		int x,y;
		can_five(color,x,y);//x,yΪ�Է������ӵ���
		ChessPoints[x][y] = anticolor;//����һ��
		if (has_beatpoints_low(color))
		{
			//�����ʱ���ڴ���ӣ���ô����Ӽ�Ϊǿ�����
			tempresult[counter].x = x1;				
			tempresult[counter].y = y1;				
			counter++;
			flag = true;
			ChessPoints[x][y] = 0;//�ָ�һ��
			ChessPoints[x1][y1] = 0;//�ٻָ�һ��
			return true;
		}
		ChessPoints[x][y] = 0;//�ָ�һ��
		ChessPoints[x1][y1] = 0;//�ٻָ�һ��
	}
	return flag;
}

int CGoBangGame::countpoints()
{
	//���������ϵ�����
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
	//�ж�һ�����Ƿ��Ǵ����
	if (ChessPoints[x][y]!=0) return false;
	if(!is_makealive(x,y,color)) return false;
	int anticolor;//�Է���color
	if (color==1) anticolor = 2;
	else anticolor = 1;
			
	ChessPoints[x][y] = color;//����һ��
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
	//�˺����ж��Ƿ��ǶԷ�color���赲��
	if (ChessPoints[x][y]!=0) return false;//�Ѿ�������
	int model[][11]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����
		//-1��ʾ���⣬1��ʾcolor��0��ʾ�գ�2��ʾ(x,y)����λ��
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
		//�ĸ��������		
		for ( j=0;j<model_num;j++)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��
		{
			flag = true;
			for( k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,k-5,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-5,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}
			if (flag) return true;//ƥ����ĳ��ģʽ
			flag = true;
			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if (getcolor(x,i,5-k,y)!=0)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,5-k,y)!=color)
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}				
			}
			if (flag) return true;//ƥ����ĳ��ģʽ
		}
	}
	return false;
}

BOOL CGoBangGame::is_chongtwo(int color, int x, int y)
{
	//�ж��Ƿ��ǳ���ӣ������ڸ�ֵ�㷨ǰ���
	//�������������ǵ���
	if (ChessPoints[x][y]!=0) return false;//�Ѿ�������
	int model[][9]={//�������ʾ����ģʽ����������ǰ�ĸ��ͺ��ĸ��ӵ�����
		//-1��ʾ���⣬1��ʾ��ͬ��ɫ��0��ʾ�ǶԷ�ɫ��ֻ����ͬɫ����0��2��ʾ(x,y)����λ��
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
		//�ĸ��������		
		for ( j=0;j<model_num;j++)//model_num��ģʽ������ÿ��ģʽ�������Σ�������һ��
		{
			flag = true;
			for( k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if ((getcolor(x,i,k-4,y)!=color)&&(getcolor(x,i,k-4,y)!=0))
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}
				else if (model[j][k]==1)
				{
					if (getcolor(x,i,k-4,y)!=color)
					{
						//��ƥ���ģʽ
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
			for(k=0;k<point_num;k++)//point_num���ӱ�����������
			{
				if (model[j][k]==0)
				{
					if ((getcolor(x,i,4-k,y)!=color)&&(getcolor(x,i,4-k,y)!=0))
					{
						//��ƥ���ģʽ
						flag = false;
						break;
					}
				}

				else if (model[j][k]==1)
				{
					if (getcolor(x,i,4-k,y)!=color)
					{
						//��ƥ���ģʽ
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
	//�õ�qianmodel����Χ��
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
	//�õ�qianmodel����Χ��
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
	//�õ�qianmodel����Χ��
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
	//�õ�qianmodel����Χ��
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
	//�õ�qianmodel����Χ��
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
	//�õ�qianmodel����Χ��
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
	//�Ƿ���һ��makealive��һ��twolevelͬʱ����
	//������Ӿ����Ǹ�makealive��
	if (!is_makealive(x,y,color)) return false;//����Ӳ���makealive�ӣ�һ������
	if (!has_twolevel(color)) return false;//������û��twolevel��һ������
	ChessPoints[x][y] = color;//��һ��
	POINT alivepoint[225];
	int alivepoint_count;
	has_alive(color,alivepoint_count,alivepoint);
	POINT twolevelpoint[225];
	int twolevelpoint_count;
	has_twolevel(color,twolevelpoint_count,twolevelpoint);
	ChessPoints[x][y] = 0;
	int temp = alivepoint_count;
	sub(alivepoint,alivepoint_count,twolevelpoint,twolevelpoint_count);
	if (temp==alivepoint_count) return true;//˵��û��һ������
	return false;
}

BOOL CGoBangGame::has_chongtwoandmakeqianmodel(int color, int &counter, POINT tempresult[])
{
	//���������chongtwo��makeqianmodel��������
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
	//�˺�������grade=5ʱ����Ҫ�����ж��Ƿ��߳�ĳһ����󣬶Է����赲���Ǵ����
	//����ǣ���ôȥ�������ѡ��
	//���tempresult���ϱ�Ф�⣬��ô�öԷ��Ĵ������Ϊ��ѡ��
	POINT localtempresult[225];
	int tempn = 0;
	POINT defentpoint[225];
	int defent_count;
	int anticolor;//�Է���color	
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


