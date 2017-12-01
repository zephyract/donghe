
/*******************************************************************
* Model   Name： 
* Function    ： 
* Include File： 
* Author      ： Zhouxingjie
* EMail       ： shenzhenit@163.com
* Date Created： 2007年12月3日 星期四
* Remark      ： 
* 
*******************************************************************/


#if !defined(AFX_BOXMAN_H__6BDB997E_47A5_471A_9589_4BB3B6AFF679__INCLUDED_)
#define AFX_BOXMAN_H__6BDB997E_47A5_471A_9589_4BB3B6AFF679__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxwin.h> 
#include <stack>
using namespace std;



#define VK_BOXMAN_RESTART 82
#define VK_BOXMAN_FORER_BARRIER 112
#define VK_BOXMAN_NEXT_BARRIER 113



//地图状态
#define MAP_BACKGROUP		48		//'0'  对应字符'0'背景
#define MAP_WHITEWALL		49		//'1'  墙
#define MAP_BLUEWALL		50		//'2'  通道
#define MAP_BALL			51		//'3'  目的点
#define MAP_YELLOWBOX		52		//'4'  箱子
#define MAP_REDBOX			53		//'5'  安放好的箱子
#define MAP_MANWALL			54		//'6'  人在通道区域
#define MAP_MANBALL			55		//'7'  人在目的点区域


#define M_TAB_WIDTH			16		//横向方块数目宽度
#define M_TAB_HEIGHT		14		//纵向方块数目宽度


extern	int		iBlockWidth;
extern	int		iBlockHeight;


//游戏区域小方块大小 
//#define BLOCK_WIDTH		20		//  宽度
//#define BLOCK_HEIGHT		20		//  深度

#define BLOCK_WIDTH    iBlockWidth   //  宽度
#define BLOCK_HEIGHT    iBlockWidth  //  深度

#define _USE_GAME_BK_TRANSPARENT


enum	MAP_TYPE{ MAPNOR_INRES,MAPSMALL_INRES,MAP_EXTERN,MAP_TOTAL };
#define  ASSET_CANNOTMOV( POS )  ( (POS!=MAP_BLUEWALL) &&(POS!=MAP_BALL)&&(POS!=MAP_MANBALL) &&(POS!=MAP_MANWALL) )
#define  ASSET_CANMOV( POS )  ( (POS==MAP_BLUEWALL)||(POS==MAP_BALL)||(POS==MAP_MANBALL)||(POS==MAP_MANWALL) )


//方便撤消一步
typedef struct _GAME_INFO
{
	TCHAR m_cMap[M_TAB_HEIGHT][M_TAB_WIDTH];  //方块矩阵
    CPoint m_ptManPosition;
    int m_iMissionNum;
}GAME_INFO,*PGAME_INFO;


class MovStep
{
public:
	CPoint	 m_manFromPt;			//man start position	
	CPoint	 m_BoxPtFrom;			//box from position,Sometimes invalid(-1,-1); 
	USHORT	 m_MovDirection;		// VK_LEFT:VK_UP :VK_RIGHT :VK_DOWN : VK_JUMP:

	TCHAR	 m_ManFromType;
	TCHAR	 m_ManToType;
		
	TCHAR	 m_BoxFromType;
	TCHAR	 m_BoxToType;

	MovStep();
	~MovStep(){}
	
};

struct SearchStep 
{
    int		x;
    int		y;
    BOOL	bRight;
    BOOL	bLeft;
    BOOL	bUp;
    BOOL	bDown;
};
class CGameManBoxMan  
{
public:
	CGameManBoxMan(  );
	virtual ~CGameManBoxMan();

	void		SetResInfo( LPCTSTR pStrImagePath,UINT nIDMapSmall,UINT nIDMapBig );

public:
	BOOL	ChangeMissionNum(int iNum);
	void	KeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	int		GetNowMissionNum(void);
	void	IsSound(BOOL bUse);
	void	ReturnCurManPos( CPoint *pPos );

	virtual void DrawGameArea(CDC* pDC);

	void	SetBackDC( CDC *pDC ,CPoint ptOffset );
	void	SetManDC( CDC *pDCMan,CSize szman,COLORREF clManTrans=CLR_INVALID );
	void	SetMapType( UINT nIDType );
    CPoint	GetManPosition();

	CString		GetProgromRunFolder();
    TCHAR* GetMap();
    BOOL	IsPositionValid( int xEnd,int yEnd );
    //判断临近点是否可达，包括推动一格箱子  KeyDirect  返回移动方向
    BOOL	FindNearPos( int xStart,int yStart,int xEnd,int yEnd,TCHAR &KeyDirect );

    bool  search( int xStart,int yStart,int xEnd,int yEnd ); 
    void ManJump( int x,int y,int x2,int y2 );

protected:
	//绘图操作
    virtual void DrawBackGroup(int x, int y, CDC* pDC);
    virtual void DrawWhiteWall(int x, int y, CDC* pDC);
    virtual void DrawBlueWall(int x, int y, CDC* pDC);
    virtual void DrawBall(int x, int y, CDC* pDC);
    virtual void DrawYellowBox(int x, int y, CDC* pDC);
    virtual void DrawRedBox(int x, int y, CDC* pDC);
    virtual void DrawManWall(int x, int y, CDC* pDC);
    virtual void DrawManBall(int x, int y, CDC* pDC);

	int		LoadMaxMissionNum(void);
    void	LoadMap(int iMissionNum);
	LONG    LoadMapInRes(int iMissionNum);
	int		LoadMaxMissionNumInRes(void);



    void	DispatchMsg(UINT nChar);
    void	UpdateMap(int x1, int y1, int x2, int y2, int x3, int y3);

    bool	IsFinish(void);

	void	BoxManPlaySound(void);


    

protected:
    BOOL	m_bSerChMap[M_TAB_HEIGHT ][M_TAB_WIDTH];
	TCHAR	m_cMap[M_TAB_HEIGHT ][M_TAB_WIDTH];  //方块矩阵
    CPoint	m_ptManPosition;
    int		m_iMissionNum;
	UINT	m_nMapType;

	int		m_soundState;                     //当前播音状态
	int		m_bSound;                         //是否启用声音

	CDC				*m_pDCBack;						//游戏背景
	CPoint			m_ptOffset;						//游戏背景顶端位置

	CDC				*m_pDCMan;						//游戏中的人
	CSize			m_szMan;						//人像的大小
	COLORREF		m_clManTrans;						//人像的透明色

	BLENDFUNCTION	m_bfSelect;

	stack<MovStep> m_stackRedoMove;	//记录已悔棋的走法的栈，便于还原
	USHORT		   m_direction;
	
	CString		   m_StrImagePath;
	UINT		   m_nIDRESMapSmall;
	UINT		   m_nIDRESMapBig;

public:
	BOOL			UndoMove();
	void			NewGameInit();

};

#endif // !defined(AFX_BOXMAN_H__6BDB997E_47A5_471A_9589_4BB3B6AFF679__INCLUDED_)
