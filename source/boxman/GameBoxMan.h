
/*******************************************************************
* Model   Name�� 
* Function    �� 
* Include File�� 
* Author      �� Zhouxingjie
* EMail       �� shenzhenit@163.com
* Date Created�� 2007��12��3�� ������
* Remark      �� 
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



//��ͼ״̬
#define MAP_BACKGROUP		48		//'0'  ��Ӧ�ַ�'0'����
#define MAP_WHITEWALL		49		//'1'  ǽ
#define MAP_BLUEWALL		50		//'2'  ͨ��
#define MAP_BALL			51		//'3'  Ŀ�ĵ�
#define MAP_YELLOWBOX		52		//'4'  ����
#define MAP_REDBOX			53		//'5'  ���źõ�����
#define MAP_MANWALL			54		//'6'  ����ͨ������
#define MAP_MANBALL			55		//'7'  ����Ŀ�ĵ�����


#define M_TAB_WIDTH			16		//���򷽿���Ŀ���
#define M_TAB_HEIGHT		14		//���򷽿���Ŀ���


extern	int		iBlockWidth;
extern	int		iBlockHeight;


//��Ϸ����С�����С 
//#define BLOCK_WIDTH		20		//  ���
//#define BLOCK_HEIGHT		20		//  ���

#define BLOCK_WIDTH    iBlockWidth   //  ���
#define BLOCK_HEIGHT    iBlockWidth  //  ���

#define _USE_GAME_BK_TRANSPARENT


enum	MAP_TYPE{ MAPNOR_INRES,MAPSMALL_INRES,MAP_EXTERN,MAP_TOTAL };
#define  ASSET_CANNOTMOV( POS )  ( (POS!=MAP_BLUEWALL) &&(POS!=MAP_BALL)&&(POS!=MAP_MANBALL) &&(POS!=MAP_MANWALL) )
#define  ASSET_CANMOV( POS )  ( (POS==MAP_BLUEWALL)||(POS==MAP_BALL)||(POS==MAP_MANBALL)||(POS==MAP_MANWALL) )


//���㳷��һ��
typedef struct _GAME_INFO
{
	TCHAR m_cMap[M_TAB_HEIGHT][M_TAB_WIDTH];  //�������
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
    //�ж��ٽ����Ƿ�ɴ�����ƶ�һ������  KeyDirect  �����ƶ�����
    BOOL	FindNearPos( int xStart,int yStart,int xEnd,int yEnd,TCHAR &KeyDirect );

    bool  search( int xStart,int yStart,int xEnd,int yEnd ); 
    void ManJump( int x,int y,int x2,int y2 );

protected:
	//��ͼ����
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
	TCHAR	m_cMap[M_TAB_HEIGHT ][M_TAB_WIDTH];  //�������
    CPoint	m_ptManPosition;
    int		m_iMissionNum;
	UINT	m_nMapType;

	int		m_soundState;                     //��ǰ����״̬
	int		m_bSound;                         //�Ƿ���������

	CDC				*m_pDCBack;						//��Ϸ����
	CPoint			m_ptOffset;						//��Ϸ��������λ��

	CDC				*m_pDCMan;						//��Ϸ�е���
	CSize			m_szMan;						//����Ĵ�С
	COLORREF		m_clManTrans;						//�����͸��ɫ

	BLENDFUNCTION	m_bfSelect;

	stack<MovStep> m_stackRedoMove;	//��¼�ѻ�����߷���ջ�����ڻ�ԭ
	USHORT		   m_direction;
	
	CString		   m_StrImagePath;
	UINT		   m_nIDRESMapSmall;
	UINT		   m_nIDRESMapBig;

public:
	BOOL			UndoMove();
	void			NewGameInit();

};

#endif // !defined(AFX_BOXMAN_H__6BDB997E_47A5_471A_9589_4BB3B6AFF679__INCLUDED_)
