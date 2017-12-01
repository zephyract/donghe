/****************************
* FileName: RussiaGame
* Function: Draw Game Area
* Author  : henry
* Date    : 2012.05.23
***************************/
#include "StdAfx.h"
#include "RussiaGame.h"
#include <string>
#include "WceUiRealWnd.h"
#include "uibase.h"

using namespace std;
#define  TIMER_ID_INVALIDATE 2
struct Calcul_Score_Level Score_Level;
int	array_status[XMAXGRID][YMAXGRID];

//地图状态
#define MAP_STATE_EMPTY             0    //空(未被占据)
#define MAP_STATE_NOT_EMPTY         1    //被占据
#define		_USE_MAKE_SOUND
int CSquareBase::iVerGridCount = 0;
int CSquareBase::iHorGridCount = 0;
CSquareBase::CSquareBase()
{

    //	set two dimension array's original status.
    for(int i=0;i<XMAXGRID;i++)
        for(int j=0;j<YMAXGRID;j++)
            array_status[i][j] = MAP_STATE_EMPTY;

    //set array's subscript to zero.
    x1=0;
    y1=0;
    x2=0;
    y2=0;
    x3=0;
    y3=0;
    x4=0;
    y4=0;
}

CSquareBase::~CSquareBase()
{
}

void CSquareBase::Left()
{
    //If collide with other squareness or exceed extent:
    

    int tmpx1 =0;
    int tmpx2 =0;
    int tmpx3 =0;
    int tmpx4 =0;

    int tmpy1 =0;
    int tmpy2 =0;
    int tmpy3 =0;
    int tmpy4 =0;

    tmpx1 = x1 -1;
    tmpx2 = x2 -1;
    tmpx3 = x3 -1;
    tmpx4 = x4 -1;
    tmpy1 = y1;
    tmpy2 = y2;
    tmpy3 = y3;
    tmpy4 = y4;
    
    array_status[x1][y1]= MAP_STATE_EMPTY;
    array_status[x2][y2]= MAP_STATE_EMPTY;
    array_status[x3][y3]= MAP_STATE_EMPTY;
    array_status[x4][y4]= MAP_STATE_EMPTY;
    //变换后一切正常
    if (IsHValid(tmpx1,tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4))
    {


        x1 = tmpx1;
        x2 = tmpx2;
        x3 = tmpx3;
        x4 = tmpx4;

        y1 = tmpy1;
        y2 = tmpy2;
        y3 = tmpy3;
        y4 = tmpy4;



    }
    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}

void CSquareBase::Right()
{
    int tmpx1 =0;
    int tmpx2 =0;
    int tmpx3 =0;
    int tmpx4 =0;

    int tmpy1 =0;
    int tmpy2 =0;
    int tmpy3 =0;
    int tmpy4 =0;

    tmpx1 = x1 +1;
    tmpx2 = x2 +1;
    tmpx3 = x3 +1;
    tmpx4 = x4 +1;
    tmpy1 = y1;
    tmpy2 = y2;
    tmpy3 = y3;
    tmpy4 = y4;

    array_status[x1][y1]= MAP_STATE_EMPTY;
    array_status[x2][y2]= MAP_STATE_EMPTY;
    array_status[x3][y3]= MAP_STATE_EMPTY;
    array_status[x4][y4]= MAP_STATE_EMPTY;
    //变换后一切正常
    if (IsHValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4))
    {


        x1 = tmpx1;
        x2 = tmpx2;
        x3 = tmpx3;
        x4 = tmpx4;

        y1 = tmpy1;
        y2 = tmpy2;
        y3 = tmpy3;
        y4 = tmpy4;



    }
    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}

//not integrity:
void CSquareBase::Rotate()
{
    return;
}

BOOL    CSquareBase::IsHValid(int x1,int y1,int x2,int y2,int x3, int y3, int x4, int y4,BOOL bleft)
{
    BOOL bIsValid  = TRUE;

    //CPoint point[4];
    //point[0].x = x1;
    //point[0].y = y1;
    //point[1].x = x2;
    //point[1].y = y2;
    //point[2].x = x3;
    //point[2].y = y3;
    //point[3].x = x4;
    //point[3].y = y4;
    ////纵坐标坐标相同时
    //for (int i = 0 ;i<4; i++)
    //{
    //    for (int j =i+1;j<4;j++)
    //    {
    //        if (point[i].y == point[j].y)
    //        {
    //            int tmpx =0;
    //            tmpx =bleft?min(point[i].x,point[j].x):max(point[i].x,point[j].x);
    //            
    //            point[i].x  = point[j].x = tmpx;
    //        }
    //    }
    //}
    if (
        (array_status[x1][y1]==1)||
        (array_status[x2][y2]==1)||
        (array_status[x3][y3]==1)||
        (array_status[x4][y4]==1)||

        x1>iHorGridCount-1||
        x2>iHorGridCount-1||
        x3>iHorGridCount-1||
        x4>iHorGridCount-1||

        x1<0||
        x2<0||
        x3<0||
        x4<0
        )
    {
        bIsValid  = FALSE;
    }
    return  bIsValid;
}
BOOL    CSquareBase::IsVValid(int x1,int y1,int x2,int y2,int x3, int y3, int x4, int y4)
{
    BOOL bIsValid  = TRUE;
    //CPoint point[4];
    //point[0].x = x1;
    //point[0].y = y1;
    //point[1].x = x2;
    //point[1].y = y2;
    //point[2].x = x3;
    //point[2].y = y3;
    //point[3].x = x4;
    //point[3].y = y4;
    ////横坐标相同时 只需要判断纵坐标较大的那个就行了
    //for (int i = 0 ;i<4; i++)
    //{
    //    for (int j =i+1;j<4;j++)
    //    {
    //        if (point[i].x == point[j].x)
    //        {
    //            point[i].y  = point[j].y = max(point[i].y,point[j].y);
    //        }
    //    }
    //}

    if (
        (array_status[x1][y1]==1)||
        (array_status[x2][y2]==1)||
        (array_status[x3][y3]==1)||
        (array_status[x4][y4]==1)||
        
         y1>iVerGridCount-1||
         y2>iVerGridCount-1||
         y3>iVerGridCount-1||
         y4>iVerGridCount-1||
        
         y1<0||
         y2<0||
         y3<0||
         y4<0
        )
    {
       bIsValid  = FALSE;
    }
    return  bIsValid;

}
void CSquareBase::Init()
{

    x1=0;
    y1=0;
    x2=0;
    y2=0;
    x3=0;
    y3=0;
    x4=0;
    y4=0;
}

//返回值:0:不能继续下落，无消行(遇到障碍物或到底,但方格未顶到头)
//       3: 不能继续下落，有消行
//		 1:可以继续下落(未遇到障碍物也未到底)
//		 2:方格已顶到头,游戏结束

int CSquareBase::Down()
{
     Score_Level.UnitScore=0;

     int tmpx1 =0;
     int tmpx2 =0;
     int tmpx3 =0;
     int tmpx4 =0;

     int tmpy1 =0;
     int tmpy2 =0;
     int tmpy3 =0;
     int tmpy4 =0;

     tmpx1 = x1;
     tmpx2 = x2;
     tmpx3 = x3;
     tmpx4 = x4;
     tmpy1 = y1 + 1;
     tmpy2 = y2 + 1;
     tmpy3 = y3 + 1;
     tmpy4 = y4 + 1;

     array_status[x1][y1]= MAP_STATE_EMPTY;
     array_status[x2][y2]= MAP_STATE_EMPTY;
     array_status[x3][y3]= MAP_STATE_EMPTY;
     array_status[x4][y4]= MAP_STATE_EMPTY;

     //变换后一切正常
     if (IsVValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4))
     {


         x1 = tmpx1;
         x2 = tmpx2;
         x3 = tmpx3;
         x4 = tmpx4;

         y1 = tmpy1;
         y2 = tmpy2;
         y3 = tmpy3;
         y4 = tmpy4;

         array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
         array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
         array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
         array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
         return 1;

     }
     //变换后遇到障碍，或者超出边框
     else
     {
         array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
         array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
         array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
         array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
         TCHAR  strout[MAX_PATH];
         //判断消行
         for(int i=0;i<=iVerGridCount -1;i++)
         {
             bool IsFilled=TRUE;
             for(int j=0;j<=iHorGridCount-1;j++)
             {
                 int k = _stprintf(strout,TEXT("array_status[%d] "),j);
                 k  +=  _stprintf(strout+k,TEXT("[%d] "),i);
                 k  +=  _stprintf(strout+k,TEXT("= %d "),array_status[j][i]);
                 OutputDebugString(strout);
                 if(array_status[j][i]==0)
                 {
                     IsFilled=FALSE;
                     break;
                 }
             }
             wstring  str =L"\r\n";
             OutputDebugString(str.c_str());

             if(IsFilled)
             {		
                 Score_Level.nRemoveRow++;//消行计数器:每消一行加1              
                 for(int k=i;k>=1;k--)
                 {
                     for(int l=0;l<=iHorGridCount-1;l++)
                     {
                         array_status[l][k]=array_status[l][k-1];
                     }
                 }
             }
              Score_Level.UnitScore=Score_Level.nRemoveRow*50;//每消掉一行加50分,每落一次但不消行加10分
         }
         for(int i=0;i<iHorGridCount;i++)//扫描第一行,如有一个为1,则方格已到头,游戏结束
         {
             if(array_status[i][0]==1)
                 return 2;
         }
         
         int nreturn  = 0;
         //消行两行以上每行多加50分
         if ( Score_Level.nRemoveRow>1 )
         {
             Score_Level.UnitScore += (Score_Level.nRemoveRow-1)*50;
         }
         if (Score_Level.nRemoveRow)
         {
            //消行成功需要播放消行声音
             nreturn = 3;
             
         }
          //_stprintf(strout,TEXT("Score_Level.UnitScore =%d\r\n "),Score_Level.UnitScore);
          //OutputDebugString(strout);
         //计分完毕 nRemoveRow清零
         Score_Level.nTotalRemoveRow+=Score_Level.nRemoveRow;//统计当前消去的总行数
         Score_Level.TotalScore+=Score_Level.UnitScore;//加分
         Score_Level.nRemoveRow = 0;
         //_stprintf(strout,TEXT("Score_Level.nTotalRemoveRow =%d\r\n "),Score_Level.nTotalRemoveRow);
         //OutputDebugString(strout);
         //_stprintf(strout,TEXT("Score_Level.TotalScore =%d\r\n "),Score_Level.TotalScore);
         //OutputDebugString(strout);
         return nreturn;
     }
}

void CSquareBase::SetVCount( int VerGridCount )
{
    iVerGridCount = VerGridCount;
}

void CSquareBase::SetHCount( int HorGridCount )
{
    iHorGridCount = HorGridCount;
}
////////////////////////////////////////////////////////////////////
//derived class overload member function:
//CSquare1:
CSquare1::CSquare1()
{
    //1	2   **
    //3	4   **
    status=1;
    x1=iHorGridCount/2;
    y1=0;
    x2=x1+1;
    y2=y1;
    x3=x1;
    y3=y2+1;
    x4=x2;
    y4=y3;

    //array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    //array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    //array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    //array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}

CSquare1::~CSquare1()
{
    ;
}

void CSquare1::Init()
{
    status=1;
    x1=iHorGridCount/2;
    y1=0;
    x2=x1+1;
    y2=y1;
    x3=x1;
    y3=y2+1;
    x4=x2;
    y4=y3;

    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}

void CSquare1::Rotate()
{
    return;

}

/////////////////////////////////////////////////////////////////////
//CSquare2:
CSquare2::CSquare2()
{
    //1	 2	3  4 ****
    status=1;
    x1=iHorGridCount/2;
    y1=0;
    x2=x1+1;
    y2=y1;
    x3=x1+2;
    y3=y1;
    x4=x1+3;
    y4=y1;
    //array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    //array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    //array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    //array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}

CSquare2::~CSquare2()
{
    ;
}
void CSquare2::Init()
{
    status=1;
    x1=iHorGridCount/2;
    y1=0;
    x2=x1+1;
    y2=y1;
    x3=x1+2;
    y3=y1;
    x4=x1+3;
    y4=y1;
    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}


void CSquare2::Rotate()
{

    int tmpx1 =0;
    int tmpx2 =0;
    int tmpx3 =0;
    int tmpx4 =0;

    int tmpy1 =0;
    int tmpy2 =0;
    int tmpy3 =0;
    int tmpy4 =0;
    int  nprestatus = status;
    switch(status)
    {
    case 1:
        status=2;
        tmpx1=x1+1;
        tmpy1=y1-1;
        tmpx2=x2;
        tmpy2=y2;
        tmpx3=x3-1;
        tmpy3=y3+1;
        tmpx4=x4-2;
        tmpy4=y4+2;
        break;
    case 2:
        status=1;
        tmpx1=x1-1;
        tmpy1=y1+1;
        tmpx2=x2;
        tmpy2=y2;
        tmpx3=x3+1;
        tmpy3=y3-1;
        tmpx4=x4+2;
        tmpy4=y4-2;
        break;
    }
    //清空当前状态
    array_status[x1][y1]= MAP_STATE_EMPTY;
    array_status[x2][y2]= MAP_STATE_EMPTY;
    array_status[x3][y3]= MAP_STATE_EMPTY;
    array_status[x4][y4]= MAP_STATE_EMPTY;
    //变换后一切正常
   if (IsVValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4)&&IsHValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4))
    {


        x1 = tmpx1;
        x2 = tmpx2;
        x3 = tmpx3;
        x4 = tmpx4;

        y1 = tmpy1;
        y2 = tmpy2;
        y3 = tmpy3;
        y4 = tmpy4;

    }
   else //旋转失败恢复status
   {
       status = nprestatus;
   }
   //设置当前状态  
   array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
   array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
   array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
   array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}

/////////////////////////////////////////////////////////////////////
//CSquare3:
CSquare3::CSquare3()
{
    //	 1       *
    //3	 2	4   ***
    status=1;
    x1=iHorGridCount/2+1; 
    y1=0;
    x2=x1;
    y2=y1+1;
    x3=x1-1;
    y3=y2;
    x4=x1+1;
    y4=y2;

    //array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    //array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    //array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    //array_status[x4][y4]= MAP_STATE_NOT_EMPTY;

}

CSquare3::~CSquare3()
{
    ;
}
void CSquare3::Init()
{
    //	 1       *
    //3	 2	4   ***
    status=1;
    x1=iHorGridCount/2+1; 
    y1=0;
    x2=x1;
    y2=y1+1;
    x3=x1-1;
    y3=y2;
    x4=x1+1;
    y4=y2;

    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}

void CSquare3::Rotate()
{

    int tmpx1 =0;
    int tmpx2 =0;
    int tmpx3 =0;
    int tmpx4 =0;

    int tmpy1 =0;
    int tmpy2 =0;
    int tmpy3 =0;
    int tmpy4 =0;
    int  nprestatus = status;
    switch(status)
    {
    case 1:
        status=2;
        tmpx1=x4;
        tmpy1=y4;
        tmpx2=x2;
        tmpy2=y2;
        tmpx3=x3+1;
        tmpy3=y3-1;
        tmpx4=x4-1;
        tmpy4=y4+1;

        break;
    case 2:
        status=3;
        tmpx1=x4;
        tmpy1=y4;
        tmpx2=x2;
        tmpy2=y2;
        tmpx3=x3+1;
        tmpy3=y3+1;
        tmpx4=x4-1;
        tmpy4=y4-1;
        break;
    case 3:
        status=4;
        tmpx1=x4;
        tmpy1=y4;
        tmpx2=x2;
        tmpy2=y2;
        tmpx3=x3-1;
        tmpy3=y3+1;
        tmpx4=x4+1;
        tmpy4=y4-1;
        break;
    case 4:
        status=1;
        tmpx1=x4;
        tmpy1=y4;
        tmpx2=x2;
        tmpy2=y2;
        tmpx3=x3-1;
        tmpy3=y3-1;
        tmpx4=x4+1;
        tmpy4=y4+1;
        break;
    }
    array_status[x1][y1]= MAP_STATE_EMPTY;
    array_status[x2][y2]= MAP_STATE_EMPTY;
    array_status[x3][y3]= MAP_STATE_EMPTY;
    array_status[x4][y4]= MAP_STATE_EMPTY;
    //变换后一切正常
    if (IsVValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4)&&IsHValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4))
    {


        x1 = tmpx1;
        x2 = tmpx2;
        x3 = tmpx3;
        x4 = tmpx4;

        y1 = tmpy1;
        y2 = tmpy2;
        y3 = tmpy3;
        y4 = tmpy4;

    }
    else //旋转失败恢复status
    {
        status = nprestatus;
    }
    //设置当前状态  
    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}
/////////////////////////////////////////////////////////////////////
//CSquare4:
CSquare4::CSquare4()
{
    //1	 2  3   ***
    //	    4     *
    status=1;
    x1=iHorGridCount/2;  
    y1=0;
    x2=x1+1;
    y2=y1;
    x3=x1+2;
    y3=y1;
    x4=x3;
    y4=y1+1;

    //array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    //array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    //array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    //array_status[x4][y4]= MAP_STATE_NOT_EMPTY;

}

CSquare4::~CSquare4()
{
    ;
}

void CSquare4::Init()
{
    //1	 2  3   ***
    //	    4     *
    status=1;
    x1=iHorGridCount/2;  
    y1=0;
    x2=x1+1;
    y2=y1;
    x3=x1+2;
    y3=y1;
    x4=x3;
    y4=y1+1;

    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}


void CSquare4::Rotate()
{

    int tmpx1 =0;
    int tmpx2 =0;
    int tmpx3 =0;
    int tmpx4 =0;

    int tmpy1 =0;
    int tmpy2 =0;
    int tmpy3 =0;
    int tmpy4 =0;
    int  nprestatus = status;
    switch(status)
    {
    case 1:
        status=2;
        tmpx1=x1+2;
        tmpy1=y1-2;
        tmpx2=x2+1;
        tmpy2=y2-1;
        tmpx3=x3;
        tmpy3=y3;
        tmpx4=x4-1;
        tmpy4=y4-1;
        break;
    case 2:
        status=3;
        tmpx1=x1+2;
        tmpy1=y1+2;
        tmpx2=x2+1;
        tmpy2=y2+1;
        tmpx3=x3;
        tmpy3=y3;
        tmpx4=x4+1;
        tmpy4=y4-1;
        break;
    case 3:
        status=4;
        tmpx1=x1-2;
        tmpy1=y1+2;
        tmpx2=x2-1;
        tmpy2=y2+1;
        tmpx3=x3;
        tmpy3=y3;
        tmpx4=x4+1;
        tmpy4=y4+1;
        break;
    case 4:
        status=1;
        tmpx1=x1-2;
        tmpy1=y1-2;
        tmpx2=x2-1;
        tmpy2=y2-1;
        tmpx3=x3;
        tmpy3=y3;
        tmpx4=x4-1;
        tmpy4=y4+1;
        break;
    }
    array_status[x1][y1]= MAP_STATE_EMPTY;
    array_status[x2][y2]= MAP_STATE_EMPTY;
    array_status[x3][y3]= MAP_STATE_EMPTY;
    array_status[x4][y4]= MAP_STATE_EMPTY;
    //变换后一切正常
    if (IsVValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4)&&IsHValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4))
    {


        x1 = tmpx1;
        x2 = tmpx2;
        x3 = tmpx3;
        x4 = tmpx4;

        y1 = tmpy1;
        y2 = tmpy2;
        y3 = tmpy3;
        y4 = tmpy4;

    }
    else //旋转失败恢复status
    {
        status = nprestatus;
    }
    //设置当前状态  
    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}
/////////////////////////////////////////////////////////////////////
//CSquare5:
CSquare5::CSquare5()
{
    //1	 2	3   ***
    //4         *
    status=1;
    x1=iHorGridCount/2; 
    y1=0;
    x2=x1+1;
    y2=y1;
    x3=x1+2;
    y3=y1;
    x4=x1;
    y4=y1+1;

    //array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    //array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    //array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    //array_status[x4][y4]= MAP_STATE_NOT_EMPTY;

}

CSquare5::~CSquare5()
{
    ;
}
void CSquare5::Init()
{
    //1	 2	3   ***
    //4         *
    status=1;
    x1=iHorGridCount/2; 
    y1=0;
    x2=x1+1;
    y2=y1;
    x3=x1+2;
    y3=y1;
    x4=x1;
    y4=y1+1;

    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;


}

//BOOL CSquare5::CanRotate()
//{
//    //CalcStatus();
//    BOOL bRtn=1;
//    int x1=cx1;
//    int x2=cx2;
//    int x3=cx3;
//    int x4=cx4;
//    int y1=cy1;
//    int y2=cy2;
//    int y3=cy3;
//    int y4=cy4;
//    switch(status)
//    {
//    case 1:
//        cx1=cx1;
//        cy1=cy1;
//        cx2=cx2-m_russaingame.iBlockWD;
//        cy2=cy2+m_russaingame.iBlockHT;
//        cx3=cx3-m_russaingame.iBlockWD*2;
//        cy3=cy3+m_russaingame.iBlockHT*2;
//        cx4=cx4-m_russaingame.iBlockWD;
//        cy4=cy4-m_russaingame.iBlockHT;
//        break;
//    case 2:
//        cx1=cx1;
//        cy1=cy1;
//        cx2=cx2-m_russaingame.iBlockWD;
//        cy2=cy2-m_russaingame.iBlockHT;
//        cx3=cx3-m_russaingame.iBlockWD*2;
//        cy3=cy3-m_russaingame.iBlockHT*2;
//        cx4=cx4+m_russaingame.iBlockWD;
//        cy4=cy4-m_russaingame.iBlockHT;
//        break;
//    case 3:
//        cx1=cx1;
//        cy1=cy1;
//        cx2=cx2+m_russaingame.iBlockWD;
//        cy2=cy2-m_russaingame.iBlockHT;
//        cx3=cx3+m_russaingame.iBlockWD*2;
//        cy3=cy3-m_russaingame.iBlockHT*2;
//        cx4=cx4+m_russaingame.iBlockWD;
//        cy4=cy4+m_russaingame.iBlockHT;
//        break;
//    case 4:
//        cx1=cx1;
//        cy1=cy1;
//        cx2=cx2+m_russaingame.iBlockWD;
//        cy2=cy2+m_russaingame.iBlockHT;
//        cx3=cx3+m_russaingame.iBlockWD*2;
//        cy3=cy3+m_russaingame.iBlockHT*2;
//        cx4=cx4-m_russaingame.iBlockWD;
//        cy4=cy4+m_russaingame.iBlockHT;
//        break;
//    }
//    //CalcStatus();
//
//    if (
//        (array_status[x1][y1]==1)||
//        (array_status[x2][y2]==1)||
//        (array_status[x3][y3]==1)||
//        (array_status[x4][y4]==1)||
//
//        cy1>(m_russaingame.iGameTop+frmwidth+(iVerGridCount-1)*m_russaingame.iBlockHT)||
//        cy2>(m_russaingame.iGameTop+frmwidth+(iVerGridCount-1)*m_russaingame.iBlockHT)||
//        cy3>(m_russaingame.iGameTop+frmwidth+(iVerGridCount-1)*m_russaingame.iBlockHT)||
//        cy4>(m_russaingame.iGameTop+frmwidth+(iVerGridCount-1)*m_russaingame.iBlockHT)||
//
//        cy1<(m_russaingame.iGameTop+frmwidth)||
//        cy2<(m_russaingame.iGameTop+frmwidth)||
//        cy3<(m_russaingame.iGameTop+frmwidth)||
//        cy4<(m_russaingame.iGameTop+frmwidth)||
//
//        cx1>(m_russaingame.iGameLeft+frmwidth+(iHorGridCount-1)*m_russaingame.iBlockWD)||
//        cx2>(m_russaingame.iGameLeft+frmwidth+(iHorGridCount-1)*m_russaingame.iBlockWD)||
//        cx3>(m_russaingame.iGameLeft+frmwidth+(iHorGridCount-1)*m_russaingame.iBlockWD)||
//        cx4>(m_russaingame.iGameLeft+frmwidth+(iHorGridCount-1)*m_russaingame.iBlockWD)||
//
//        cx1<(m_russaingame.iGameLeft+frmwidth)||
//        cx2<(m_russaingame.iGameLeft+frmwidth)||
//        cx3<(m_russaingame.iGameLeft+frmwidth)||
//        cx4<(m_russaingame.iGameLeft+frmwidth)
//        )
//    {
//        bRtn=0;
//    }
//    cx1=x1;
//    cx2=x2;
//    cx3=x3;
//    cx4=x4;
//    cy1=y1;
//    cy2=y2;
//    cy3=y3;
//    cy4=y4;
//
//    //CalcStatus();
//
//    return bRtn;
//}

void CSquare5::Rotate()
{
   
    int tmpx1 =0;
    int tmpx2 =0;
    int tmpx3 =0;
    int tmpx4 =0;

    int tmpy1 =0;
    int tmpy2 =0;
    int tmpy3 =0;
    int tmpy4 =0;
    int  nprestatus = status;
    switch(status)
    {
    case 1:
        status=2;
        tmpx1=x1;
        tmpy1=y1;
        tmpx2=x2-1;
        tmpy2=y2+1;
        tmpx3=x3-2;
        tmpy3=y3+2;
        tmpx4=x4-1;
        tmpy4=y4-1;
        break;
    case 2:
        status=3;
        tmpx1=x1;
        tmpy1=y1;
        tmpx2=x2-1;
        tmpy2=y2-1;
        tmpx3=x3-2;
        tmpy3=y3-2;
        tmpx4=x4+1;
        tmpy4=y4-1;
        break;
    case 3:
        status=4;
        tmpx1=x1;
        tmpy1=y1;
        tmpx2=x2+1;
        tmpy2=y2-1;
        tmpx3=x3+2;
        tmpy3=y3-2;
        tmpx4=x4+1;
        tmpy4=y4+1;
        break;
    case 4:
        status=1;
        tmpx1=x1;
        tmpy1=y1;
        tmpx2=x2+1;
        tmpy2=y2+1;
        tmpx3=x3+2;
        tmpy3=y3+2;
        tmpx4=x4-1;
        tmpy4=y4+1;
        break;
    }
    array_status[x1][y1]= MAP_STATE_EMPTY;
    array_status[x2][y2]= MAP_STATE_EMPTY;
    array_status[x3][y3]= MAP_STATE_EMPTY;
    array_status[x4][y4]= MAP_STATE_EMPTY;
    //变换后一切正常
    if (IsVValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4)&&IsHValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4))
    {


        x1 = tmpx1;
        x2 = tmpx2;
        x3 = tmpx3;
        x4 = tmpx4;

        y1 = tmpy1;
        y2 = tmpy2;
        y3 = tmpy3;
        y4 = tmpy4;

    }
    else //旋转失败恢复status
    {
        status = nprestatus;
    }
    //设置当前状态  
    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;

}
/////////////////////////////////////////////////////////////////////
//CSquare6:
CSquare6::CSquare6()
{
    //		1	2   **
    //	3	4      **
    status=1;
    x1=iHorGridCount/2; 
    y1=0;
    x2=x1+1;
    y2=y1;
    x3=x1-1;
    y3=y1+1;
    x4=x1;
    y4=y3;
   //array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
   //array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
   //array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
   //array_status[x4][y4]= MAP_STATE_NOT_EMPTY;



}

CSquare6::~CSquare6()
{
    ;
}

void CSquare6::Init()
{
    //		1	2   **
    //	3	4      **
    status=1;
    x1=iHorGridCount/2; 
    y1=0;
    x2=x1+1;
    y2=y1;
    x3=x1-1;
    y3=y1+1;
    x4=x1;
    y4=y3;
    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}

//BOOL CSquare6::CanRotate()
//{
//    //CalcStatus();
//    BOOL bRtn=1;
//    int x1=cx1;
//    int x2=cx2;
//    int x3=cx3;
//    int x4=cx4;
//    int y1=cy1;
//    int y2=cy2;
//    int y3=cy3;
//    int y4=cy4;
//    switch(status)
//    {
//    case 1:
//        cx1=cx1;
//        cy1=cy1;
//        cx2=cx2-m_russaingame.iBlockWD;
//        cy2=cy2-m_russaingame.iBlockHT;
//        cx3=cx3+m_russaingame.iBlockWD*2;
//        cy3=cy3;
//        cx4=cx4+m_russaingame.iBlockWD;
//        cy4=cy4-m_russaingame.iBlockHT;
//        break;
//    case 2:
//        cx1=cx1;
//        cy1=cy1;
//        cx2=cx2+m_russaingame.iBlockWD;
//        cy2=cy2+m_russaingame.iBlockHT;
//        cx3=cx3-m_russaingame.iBlockWD*2;
//        cy3=cy3;
//        cx4=cx4-m_russaingame.iBlockWD;
//        cy4=cy4+m_russaingame.iBlockHT;
//        break;
//    }
//    //CalcStatus();
//
//    if (
//        (array_status[x1][y1]==1)||
//        (array_status[x2][y2]==1)||
//        (array_status[x3][y3]==1)||
//        (array_status[x4][y4]==1)||
//
//        cy1>(m_russaingame.iGameTop+frmwidth+(iVerGridCount-1)*m_russaingame.iBlockHT)||
//        cy2>(m_russaingame.iGameTop+frmwidth+(iVerGridCount-1)*m_russaingame.iBlockHT)||
//        cy3>(m_russaingame.iGameTop+frmwidth+(iVerGridCount-1)*m_russaingame.iBlockHT)||
//        cy4>(m_russaingame.iGameTop+frmwidth+(iVerGridCount-1)*m_russaingame.iBlockHT)||
//
//        cy1<(m_russaingame.iGameTop+frmwidth)||
//        cy2<(m_russaingame.iGameTop+frmwidth)||
//        cy3<(m_russaingame.iGameTop+frmwidth)||
//        cy4<(m_russaingame.iGameTop+frmwidth)||
//
//        cx1>(m_russaingame.iGameLeft+frmwidth+(iHorGridCount-1)*m_russaingame.iBlockWD)||
//        cx2>(m_russaingame.iGameLeft+frmwidth+(iHorGridCount-1)*m_russaingame.iBlockWD)||
//        cx3>(m_russaingame.iGameLeft+frmwidth+(iHorGridCount-1)*m_russaingame.iBlockWD)||
//        cx4>(m_russaingame.iGameLeft+frmwidth+(iHorGridCount-1)*m_russaingame.iBlockWD)||
//
//        cx1<(m_russaingame.iGameLeft+frmwidth)||
//        cx2<(m_russaingame.iGameLeft+frmwidth)||
//        cx3<(m_russaingame.iGameLeft+frmwidth)||
//        cx4<(m_russaingame.iGameLeft+frmwidth)
//        )
//    {
//        bRtn=0;
//    }
//    cx1=x1;
//    cx2=x2;
//    cx3=x3;
//    cx4=x4;
//    cy1=y1;
//    cy2=y2;
//    cy3=y3;
//    cy4=y4;
//
//    //CalcStatus();
//
//    return bRtn;
//}

void CSquare6::Rotate()
{
    int tmpx1 =0;
    int tmpx2 =0;
    int tmpx3 =0;
    int tmpx4 =0;

    int tmpy1 =0;
    int tmpy2 =0;
    int tmpy3 =0;
    int tmpy4 =0;
    int  nprestatus = status;
    switch(status)
    {
    case 1:
        status=2;
        tmpx1=x1;
        tmpy1=y1;
        tmpx2=x2-1;
        tmpy2=y2-1;
        tmpx3=x3+2;
        tmpy3=y3;
        tmpx4=x4+1;
        tmpy4=y4-1;
        break;
    case 2:
        status=1;
        tmpx1=x1;
        tmpy1=y1;
        tmpx2=x2+1;
        tmpy2=y2+1;
        tmpx3=x3-2;
        tmpy3=y3;
        tmpx4=x4-1;
        tmpy4=y4+1;
        break;
    }
    array_status[x1][y1]= MAP_STATE_EMPTY;
    array_status[x2][y2]= MAP_STATE_EMPTY;
    array_status[x3][y3]= MAP_STATE_EMPTY;
    array_status[x4][y4]= MAP_STATE_EMPTY;
    //变换后一切正常
    if (IsVValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4)&&IsHValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4))
    {


        x1 = tmpx1;
        x2 = tmpx2;
        x3 = tmpx3;
        x4 = tmpx4;

        y1 = tmpy1;
        y2 = tmpy2;
        y3 = tmpy3;
        y4 = tmpy4;

    }
    else //旋转失败恢复status
    {
        status = nprestatus;
    }
    //设置当前状态  
    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;

}
/////////////////////////////////////////////////////////////////////
//CSquare7:
CSquare7::CSquare7()
{

    //1	  2         **
    //	  3  4       **
    status=1;
    x1=iHorGridCount/2; 
    y1=0;
    x2=x1+1;
    y2=y1;
    x3=x2;
    y3=y1+1;
    x4=x3+1;
    y4=y3;

    //array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    //array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    //array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    //array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}

CSquare7::~CSquare7()
{
    ;
}

void CSquare7::Init()
{
    //1	  2         **
    //	  3  4       **
    status=1;
    x1=iHorGridCount/2; 
    y1=0;
    x2=x1+1;
    y2=y1;
    x3=x2;
    y3=y1+1;
    x4=x3+1;
    y4=y3;

    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}

//BOOL CSquare7::CanRotate()
//{
//    //CalcStatus();
//    BOOL bRtn=1;
//    int x1=cx1;
//    int x2=cx2;
//    int x3=cx3;
//    int x4=cx4;
//    int y1=cy1;
//    int y2=cy2;
//    int y3=cy3;
//    int y4=cy4;
//    switch(status)
//    {
//    case 1:
//        cx1=cx1+m_russaingame.iBlockWD;
//        cy1=cy1-m_russaingame.iBlockHT;
//        cx2=cx2;
//        cy2=cy2;
//        cx3=cx3-m_russaingame.iBlockWD;
//        cy3=cy3-m_russaingame.iBlockHT;
//        cx4=cx4-m_russaingame.iBlockWD*2;
//        cy4=cy4;
//        break;
//    case 2:
//        cx1=cx1-m_russaingame.iBlockWD;
//        cy1=cy1+m_russaingame.iBlockHT;
//        cx2=cx2;
//        cy2=cy2;
//        cx3=cx3+m_russaingame.iBlockWD;
//        cy3=cy3+m_russaingame.iBlockHT;
//        cx4=cx4+m_russaingame.iBlockWD*2;
//        cy4=cy4;
//        break;
//    }
//    //CalcStatus();
//
//    if (
//        (array_status[x1][y1]==1)||
//        (array_status[x2][y2]==1)||
//        (array_status[x3][y3]==1)||
//        (array_status[x4][y4]==1)||
//
//        cy1>(m_russaingame.iGameTop+frmwidth+(iVerGridCount-1)*m_russaingame.iBlockHT)||
//        cy2>(m_russaingame.iGameTop+frmwidth+(iVerGridCount-1)*m_russaingame.iBlockHT)||
//        cy3>(m_russaingame.iGameTop+frmwidth+(iVerGridCount-1)*m_russaingame.iBlockHT)||
//        cy4>(m_russaingame.iGameTop+frmwidth+(iVerGridCount-1)*m_russaingame.iBlockHT)||
//
//        cy1<(m_russaingame.iGameTop+frmwidth)||
//        cy2<(m_russaingame.iGameTop+frmwidth)||
//        cy3<(m_russaingame.iGameTop+frmwidth)||
//        cy4<(m_russaingame.iGameTop+frmwidth)||
//
//        cx1>(m_russaingame.iGameLeft+frmwidth+(iHorGridCount-1)*m_russaingame.iBlockWD)||
//        cx2>(m_russaingame.iGameLeft+frmwidth+(iHorGridCount-1)*m_russaingame.iBlockWD)||
//        cx3>(m_russaingame.iGameLeft+frmwidth+(iHorGridCount-1)*m_russaingame.iBlockWD)||
//        cx4>(m_russaingame.iGameLeft+frmwidth+(iHorGridCount-1)*m_russaingame.iBlockWD)||
//
//        cx1<(m_russaingame.iGameLeft+frmwidth)||
//        cx2<(m_russaingame.iGameLeft+frmwidth)||
//        cx3<(m_russaingame.iGameLeft+frmwidth)||
//        cx4<(m_russaingame.iGameLeft+frmwidth)
//        )
//    {
//        bRtn=0;
//    }
//    cx1=x1;
//    cx2=x2;
//    cx3=x3;
//    cx4=x4;
//    cy1=y1;
//    cy2=y2;
//    cy3=y3;
//    cy4=y4;
//
//    //CalcStatus();
//
//    return bRtn;
//}

void CSquare7::Rotate()
{
    int tmpx1 =0;
    int tmpx2 =0;
    int tmpx3 =0;
    int tmpx4 =0;

    int tmpy1 =0;
    int tmpy2 =0;
    int tmpy3 =0;
    int tmpy4 =0;
    int  nprestatus = status;
    switch(status)
    {
    case 1:
        status=2;
        tmpx1=x1+1;
        tmpy1=y1-1;
        tmpx2=x2;
        tmpy2=y2;
        tmpx3=x3-1;
        tmpy3=y3-1;
        tmpx4=x4-2;
        tmpy4=y4;
        break;
    case 2:
        status=1;
        tmpx1=x1-1;
        tmpy1=y1+1;
        tmpx2=x2;
        tmpy2=y2;
        tmpx3=x3+1;
        tmpy3=y3+1;
        tmpx4=x4+2;
        tmpy4=y4;
        break;
    }

    array_status[x1][y1]= MAP_STATE_EMPTY;
    array_status[x2][y2]= MAP_STATE_EMPTY;
    array_status[x3][y3]= MAP_STATE_EMPTY;
    array_status[x4][y4]= MAP_STATE_EMPTY;
    //变换后一切正常
    if (IsVValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4)&&IsHValid(tmpx1, tmpy1,tmpx2,tmpy2,tmpx3,tmpy3,tmpx4,tmpy4))
    {


        x1 = tmpx1;
        x2 = tmpx2;
        x3 = tmpx3;
        x4 = tmpx4;

        y1 = tmpy1;
        y2 = tmpy2;
        y3 = tmpy3;
        y4 = tmpy4;

    }
    else //旋转失败恢复status
    {
        status = nprestatus;
    }
    //设置当前状态  
    array_status[x1][y1]= MAP_STATE_NOT_EMPTY;
    array_status[x2][y2]= MAP_STATE_NOT_EMPTY;
    array_status[x3][y3]= MAP_STATE_NOT_EMPTY;
    array_status[x4][y4]= MAP_STATE_NOT_EMPTY;
}
const POINT PtPromt[7][4]={
                                {{0,0},{1,0},{0,1},{1,1}},
                                {{0,0},{1,0},{2,0},{3,0}},
                                {{1,0},{0,1},{1,1},{2,1}},
                                {{0,0},{1,0},{2,0},{2,1}},
                                {{0,0},{1,0},{2,0},{0,1}},
                                {{1,0},{2,0},{0,1},{1,1}},
                                {{0,0},{1,0},{1,1},{2,1}}
                            };
CRussiaGame::CRussiaGame(void)
{
        iHorGridCount = 10;
        iVerGridCount = 16;
        iGameLeft = 5;
        iGameTop = 1;
        iBlockWD = 20;	//每个小方块的宽度
        iBlockHT = 20;	//每个小方块的高度

        Square_Kind = 0;
        Next_Square_Kind =0;

        m_nStartSpeed=DWC_SATRT_SPEED;
        SpeedDifference=100;
        nTimeID=1;
     //   bHaveTimer=false;
     //   m_bHasPaused = FALSE;
        m_1square=  NULL;				  
        m_2square=  NULL;
        m_3square=  NULL;				  
        m_4square=  NULL;				  
        m_5square=  NULL;				  
        m_6square=  NULL;				  
        m_7square=  NULL;				  
        m_square =  NULL;

        m_nGameState = GAME_END;
        m_pScore = NULL;
      //  m_pblockbmp = new CWceUi565Bitmap();


}

CRussiaGame::~CRussiaGame(void)
{
    delete  m_1square;	
    m_1square = NULL;
    delete 	m_2square;
    m_2square = NULL;
	delete 	m_3square;
    m_3square = NULL;
	delete 	m_4square;
    m_4square = NULL;
	delete 	m_5square;	
    m_5square = NULL;
	delete 	m_6square;
    m_6square = NULL;
    delete 	m_7square;
    m_7square = NULL;
    //delete m_pblockbmp;
    //m_pblockbmp = NULL;
}
void CRussiaGame::GenerateSqaure()
{
    srand( GetTickCount() );
    if(Square_Kind == 0 || Next_Square_Kind == 0)
    {
        Square_Kind=rand()%7+1;//generates pseudorandom between 1-7.
        Next_Square_Kind = rand()%7+1;//generates pseudorandom between 1-7.
    }
    else
    {
        Square_Kind = Next_Square_Kind;
        Next_Square_Kind =  rand()%7+1;//generates pseudorandom between 1-7.
    }
    NextSquare(Next_Square_Kind);

    //for test goal:
    //	Square_Kind=2;
    switch(Square_Kind)
    {
    case 1:									  
       m_square=m_1square;				  
        break;
    case 2:									  
       m_square=m_2square;
        break;
    case 3:									 
       m_square=m_3square;		  
        break;
    case 4:									  
       m_square=m_4square;				  
        break;
    case 5:									 
       m_square=m_5square;				  
        break;
    case 6:								
       m_square=m_6square;				
        break;
    case 7:								
       m_square=m_7square;				
        break;
    }
   m_square->Init();
   InvalidateRect(NULL);
}

void CRussiaGame::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap,rcNeedDraw);

	BLENDFUNCTION bf;
	memset(&bf, 0, sizeof(BLENDFUNCTION));
	bf.AlphaFormat = AC_SRC_ALPHA;
	BITMAP bmpinfo = m_pblockbmp.GetBitmap()->GetBitMapInfo();
	for(int i=0;i<iVerGridCount;i++)
	{
		for(int j=0;j<=iHorGridCount-1;j++)
		{
			if(array_status[j][i]==1)
			{
 				int x = j*iBlockWD+rcNeedDraw.left+iGameLeft+frmwidth;
 				int y = rcNeedDraw.top+i*iBlockHT+iGameTop+frmwidth;
 				CRect rc(x, y, x+iBlockWD, y+iBlockHT);
 				pWndBitmap->AlphaBlend( rc.left, rc.top, rc.Width(), rc.Height(), m_pblockbmp.GetBitmap(), bmpinfo.bmWidth - iBlockWD, 0, bf );
			}
		}
	}  
}

void CRussiaGame::SetProperties(PCWceUiXmlElem pCreateElem)
{

    ASSERT(NULL != pCreateElem);
    CWceUiLayer::SetProperties(pCreateElem);
    LPCTSTR pX = pCreateElem->GetAttr(L"x");
    LPCTSTR pY = pCreateElem->GetAttr(L"y");
    LPCTSTR pWidth  = pCreateElem->GetAttr(L"blockwidth");
    LPCTSTR pHeight = pCreateElem->GetAttr(L"blockheight");
    LPCTSTR pXgrid  = pCreateElem->GetAttr(L"xgrid");
    LPCTSTR pYgrid = pCreateElem->GetAttr(L"ygrid");
    LPCTSTR pblockbmp = pCreateElem->GetAttr(L"blockbmp");

    if (NULL != pX)
    {
        iGameLeft = _ttoi(pX);
    }
    if (NULL != pY)
    {
        iGameTop = _ttoi(pY);
    }
    if (NULL != pWidth)
    {
        iBlockWD = _ttoi(pWidth);
    }
    if (NULL != pHeight)
    {
        iBlockHT = _ttoi(pHeight);
    }
    if (NULL != pXgrid)
    {
        iHorGridCount = _ttoi(pXgrid);
    }
    if (NULL != pYgrid)
    {
        iVerGridCount = _ttoi(pYgrid);
    }
    if (NULL != pblockbmp)
    {
        m_pblockbmp.SetBitmapFromRes(pblockbmp);
    }

}
void CRussiaGame::OnInitLayer()
{
    
    CSquareBase::SetHCount(iHorGridCount);
    CSquareBase::SetVCount(iVerGridCount);
    m_1square=new CSquare1;				  
    m_2square=new CSquare2;
    m_3square=new CSquare3;				  
    m_4square=new CSquare4;				  
    m_5square=new CSquare5;				  
    m_6square=new CSquare6;				  
    m_7square=new CSquare7;	
 //   m_pScore = (CScore *)GetChildByName(L"score");
 //   
   // InitGlobal();
    RegisterMsg(MSG_GAME_UP);
    RegisterMsg(MSG_GAME_DOWN);
    RegisterMsg(MSG_GAME_LEFT);
    RegisterMsg(MSG_GAME_RIGHT);

    CWceUiLayer::OnInitLayer();
}
void CRussiaGame::OnTimer(WORD wIDEvent)
{
    if (wIDEvent == TIMER_ID_INVALIDATE)
    {
        KillTimer(TIMER_ID_INVALIDATE);
        if (m_nGameState == GAME_RUN)
        {
            InvalidateRect();
        }
        return;
    }
    //自由下落控制
    int status=0;
    if (m_nGameState != GAME_RUN)
    {
        KillTimer(nTimeID);
        return;
    }
//    static int  i = 0;
    if(m_square!=NULL)
    {
//         if (i == 0)
//         {
//         
//         status=m_square->Down();
//         status=m_square->Down();
//         status=m_square->Down();
//         status=m_square->Down();
//         i =4;
//         }
//         Rotate();
//         InvalidateRect();
//         return;
        status=m_square->Down();

        if(status == 0 || status == 3)//0:不能继续下落(遇到障碍物或到底,但方格未顶到头,则产生新方块)
        {
            if (status == 3)
            {
#ifdef _USE_MAKE_SOUND
				ui_play_game_sound(L"linked.wav");
                SetScore(Score_Level.Level,Score_Level.TotalScore);
#endif
            }

			//升级
            if(Score_Level.nTotalRemoveRow>=Score_Level.nLevelRow)//升等级
            {	
                m_nStartSpeed-=SpeedDifference;
                Score_Level.nTotalRemoveRow=0;
                Score_Level.Level+=1;
                if ( m_nStartSpeed<=0)
                    m_nStartSpeed = 100;	

                KillTimer(nTimeID);
                SetTimer(nTimeID,m_nStartSpeed);
                SetScore(Score_Level.Level,Score_Level.TotalScore);
            }
  

           GenerateSqaure();
        }
        else if(status==2)  //游戏已经结束
        {
            KillTimer(nTimeID);
      //      bHaveTimer=false;

#ifdef _USE_MAKE_SOUND
           ui_play_game_sound(L"gameover.wav");
#endif

            m_nGameState = GAME_END;
            KillTimer(TIMER_ID_INVALIDATE);
            GetWindow()->PostMessage(MSG_GAME_END,0,0);
            //m_nStartSpeed=DWC_SATRT_SPEED;//重置起始速度
            //InitGlobal();
        }
        else
        {
            InvalidateRect(NULL);
            // m_pScore->SetNoDraw(TRUE);
        }
    }
    else
    {
        GenerateSqaure();
    }
}

void CRussiaGame::InitGlobal()
{
    SetScore(0,0);
    for(int i=0;i<iHorGridCount;i++)
        for(int j=0;j<iVerGridCount;j++)
            array_status[i][j]=0;

    Score_Level.TotalScore=0;
    Score_Level.UnitScore=0;

    Score_Level.nRemoveRow=0;
    Score_Level.nTotalRemoveRow=0;
    Score_Level.Level=0;
    Score_Level.nLevelRow=6;
}

void CRussiaGame::Start()
{
    KillTimer(nTimeID);
    m_nStartSpeed=DWC_SATRT_SPEED;//复位起始速度
    if (m_nGameState == GAME_END)
    {
         InitGlobal();
    }
    SetTimer(nTimeID,m_nStartSpeed);
    m_nGameState = GAME_RUN;
#ifdef _USE_MAKE_SOUND
    ui_play_game_sound(L"gamestart.wav");
#endif
    InvalidateRect(NULL);
    GetWindow()->UpdateWindow();
}

void CRussiaGame::Replay()
{
#ifdef _USE_MAKE_SOUND
     ui_play_game_sound(L"gamestart.wav");
#endif

    m_nGameState = GAME_RUN;
    int OldLevel = Score_Level.Level;
    KillTimer(nTimeID);
    m_nStartSpeed=DWC_SATRT_SPEED;//复位起始速度
    Square_Kind = 0;
    Next_Square_Kind =0;
    InitGlobal();
    m_square = NULL;
    Score_Level.Level = 0;
    m_nStartSpeed = DWC_SATRT_SPEED - Score_Level.Level*SpeedDifference;
    SetTimer(nTimeID,m_nStartSpeed);
    InvalidateRect(NULL);
    GetWindow()->UpdateWindow();

}

void CRussiaGame::Pause()
{
#ifdef _USE_MAKE_SOUND
     ui_play_game_sound(L"pause.wav");
#endif
    //如果正在游戏中则点击暂停有效，否则不是游戏的暂停
    if ( m_nGameState != GAME_RUN)
    { 
        return;
    }
    m_nGameState = GAME_PAUSE;
    KillTimer(nTimeID);

}

void CRussiaGame::Right()
{
#ifdef _USE_MAKE_SOUND
    ui_play_game_sound(L"step.wav");
#endif

    if(m_nGameState != GAME_RUN)
        return;

    if(m_square==NULL)return;
    KillTimer(TIMER_ID_INVALIDATE);
    m_square->Right();
    SetTimer(TIMER_ID_INVALIDATE,50);
    //InvalidateRect();

    //m_pScore->SetNoDraw(TRUE);
}

void CRussiaGame::Down()
{
    // TODO: Add your control notification handler code here
#ifdef _USE_MAKE_SOUND
    ui_play_game_sound(L"step.wav");
#endif	

    if(m_nGameState != GAME_RUN)
        return;
    int status=0;//用于记录游戏状态

    if(m_square==NULL)return;
        for(;;)
        {
            status = m_square->Down();
            if(status == 0 || status == 3)//0:不能继续下落(遇到障碍物或到底,但方格未顶到头)
            {
                
                if (status == 3)
                {
#ifdef _USE_MAKE_SOUND
                    ui_play_game_sound(L"linked.wav");
                    SetScore(Score_Level.Level,Score_Level.TotalScore);
#endif
                }

                TCHAR strout[MAX_PATH];
                _stprintf(strout,TEXT("Score_Level.TotalScore =%d\r\n "),Score_Level.TotalScore);
                OutputDebugString(strout);
                //升等级
                if(Score_Level.nTotalRemoveRow>=Score_Level.nLevelRow)
                {	
                    m_nStartSpeed-=SpeedDifference;
                    Score_Level.nTotalRemoveRow=0;
                    Score_Level.Level+=1;
                    if ( m_nStartSpeed<=0)
                        m_nStartSpeed = 100;	

                    KillTimer(nTimeID);
                    SetTimer(nTimeID,m_nStartSpeed);
                    SetScore(Score_Level.Level,Score_Level.TotalScore);
                }
                GenerateSqaure();
                InvalidateRect();
                GetWindow()->UpdateWindow();
                return;
            }
            if(status==2)//方格顶到头
            {


#ifdef _USE_MAKE_SOUND
               ui_play_game_sound(L"gameover.wav");
#endif
                KillTimer(TIMER_ID_INVALIDATE);
                //m_nStartSpeed=DWC_SATRT_SPEED;//复位起始速度
                //InitGlobal();
                GetWindow()->PostMessage(MSG_GAME_END,0,0);
                m_nGameState = GAME_END;
                return;
            }
        }
}

void CRussiaGame::Left()
{
    // TODO: Add your control notification handler code here
#ifdef _USE_MAKE_SOUND
    ui_play_game_sound(L"step.wav");
#endif

    if(m_nGameState != GAME_RUN)
        return;
    if(m_square==NULL)return;
    KillTimer(TIMER_ID_INVALIDATE);
    m_square->Left();
    SetTimer(TIMER_ID_INVALIDATE,50);
    //InvalidateRect();
    // m_pScore->SetNoDraw(TRUE);
}

void CRussiaGame::Rotate()
{
    // TODO: Add your control notification handler code here
#ifdef _USE_MAKE_SOUND
    ui_play_game_sound(L"step.wav");
#endif

    if(m_nGameState != GAME_RUN)
        return;
    if(m_square==NULL)return;
    KillTimer(TIMER_ID_INVALIDATE);
    m_square->Rotate();
    SetTimer(TIMER_ID_INVALIDATE,50);
    //直接在这里这里Invaliadate ,由于主界面也在重绘，，导致主界面全部重绘了...
    //InvalidateRect();
}

void CRussiaGame::Exit( BOOL bSound/*= TRUE*/ )
{
#ifdef _USE_MAKE_SOUND
    if (bSound)
    {
        ui_play_game_sound(L"gameover.wav");
    }
    
#endif
     
}

GAME_STATE CRussiaGame::GetGameState()
{
    return m_nGameState;
}

void CRussiaGame::SetScore(int nLevel, int nScore)
{
    WCHAR strtest[MAX_PATH];
    memset(m_strlevel,0,_tcslen(m_strlevel));
    _stprintf(strtest,TEXT(": %d"),nLevel);
    m_strload.SetTextResID(L"GOBANG_LEVEL");
    wcscpy(m_strlevel,m_strload.GetString());
    wcscat(m_strlevel,strtest);
    memset(m_strscore,0,_tcslen(m_strscore));
    _stprintf(strtest,TEXT(": %d"),nScore);
    
    m_strload.SetTextResID(L"GOBANG_SCORE");
    wcscpy(m_strscore,m_strload.GetString());
    wcscat(m_strscore,strtest);
    m_pScore->SetScore(m_strlevel,m_strscore);
}

void CRussiaGame::NextSquare( int nextsquare )
{
    WCHAR strtest[MAX_PATH];
    POINT pt[4];
    for (int i = 0 ;i<4 ;i++)
    {
        pt[i].x = PtPromt[nextsquare -1][i].x*10;
        if (Next_Square_Kind != 2)
        {
            if (Next_Square_Kind == 1)
            {
                pt[i].x  += 10;
            }
            else
            {
                pt[i].x  += 5;
            }

        }
        pt[i].y = PtPromt[nextsquare -1][i].y;
        int k = _stprintf(strtest,TEXT("PtPromt[%d]"),nextsquare -1);
        k += _stprintf(strtest+k,TEXT("[%d]"),i);
        k += _stprintf(strtest+k,TEXT("x =%d"),PtPromt[nextsquare -1][i].x);
        k += _stprintf(strtest+k,TEXT("y =%d\r\n"),PtPromt[nextsquare -1][i].y);
        OutputDebugString(strtest);

    }
    m_pScore->SetPtPromt(&pt[0]);
}

LRESULT CRussiaGame::OnMessage( UINT message, WPARAM wParam, LPARAM lParam )
{
    if (message == MSG_GAME_UP)
    {
        Rotate();
    }
    if (message == MSG_GAME_LEFT)
    {
        Left();
    }
    if (message == MSG_GAME_RIGHT)
    {
        Right();
    }
    if (message == MSG_GAME_DOWN)
    {
        Down();
    }
    return CWceUiLayer::OnMessage(message,wParam,lParam);
}