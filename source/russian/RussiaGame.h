
#pragma once
#include "wceuilayer.h"
#include "WceUiLoadBitmap.h"
#include "WceUiLoadString.h"
#include "Score.h"


#define		EXTERNAL_BLOCK_BMP
#define		GAME_BK_TRANSPARENT			//游戏背景透明


#define frmwidth 1 //side-frame's width

#define		XMAXGRID		32
#define		YMAXGRID		32


class CSquareBase
{
public:
    CSquareBase();
    ~CSquareBase();
public:
    //only use array_status is ok 
 //   int cx1,cy1,cx2,cy2,cx3,cy3,cx4,cy4;//every block's left-top coordinate.
    int x1,y1,x2,y2,x3,y3,x4,y4;//use for save array's subscript.

public:

 //   void	CalcStatus();
    virtual void Left();
    virtual void Right();
    virtual int Down();
    virtual void Rotate();
    int status;
    virtual  void Init();
   static void   SetVCount(int VerGridCount);
   static void   SetHCount(int HorGridCount);
    BOOL    IsVValid(int x1,int y1,int x2,int y2,int x3, int y3, int x4, int y4);
    BOOL    IsHValid(int x1,int y1,int x2,int y2,int x3, int y3, int x4, int y4,BOOL bleft = TRUE);

public:
   static int     iHorGridCount;
   static int     iVerGridCount;
};

class CSquare1 : public CSquareBase
{
public:
    CSquare1();
    ~CSquare1();
    void Rotate();
    void Init();
protected:
    int status;
};

class CSquare2 : public CSquareBase
{
public:
    CSquare2();
    ~CSquare2();
//    BOOL CanRotate();
    void Rotate();
    void Init();
protected:
    int status;
};

class CSquare3 : public CSquareBase
{
public:
    CSquare3();
    ~CSquare3();
//    BOOL CanRotate();
    void Rotate();
    void Init();
protected:
    int status;
};

class CSquare4 : public CSquareBase
{
public:
    CSquare4();
    ~CSquare4();
    void Rotate();
    void Init();
//    BOOL CanRotate();
protected:
    int status;
};

class CSquare5 : public CSquareBase
{
public:
    CSquare5();
    ~CSquare5();
//    BOOL CanRotate();
    void Rotate();
    void Init();
protected:
    int status;
};

class CSquare6 : public CSquareBase
{
public:
    CSquare6();
    ~CSquare6();
//    BOOL CanRotate();
    void Rotate();
    void Init();
protected:
    int status;
};

class CSquare7 : public CSquareBase
{
public:
    CSquare7();
    ~CSquare7();
//    BOOL CanRotate();
    void Rotate();
    void Init();
protected:
    int status;
};

const DWORD DWC_SATRT_SPEED = 1000;
enum GAME_STATE{GAME_END ,GAME_RUN,GAME_PAUSE};
#define MSG_GAME_END                    WM_APP+ 2000

#define MSG_GAME_UP                     WM_APP+ 2001
#define MSG_GAME_LEFT                   WM_APP+ 2002
#define MSG_GAME_RIGHT                  WM_APP+ 2003
#define MSG_GAME_DOWN                   WM_APP+ 2004
struct Calcul_Score_Level{//计算分数和等级
    int TotalScore;//TotalScore:总分
    int UnitScore;//UnitScore:基分,每消掉一行时为50分,每落一次但不消行为10分
    int nRemoveRow;//统计一次消去的行数(当同时填满多行时,可成倍加分).消行结束时,将其复位为0
    int nTotalRemoveRow;//统计总共消去的行数(用于进行升等级计算的依据:当该值达到nLevelRow行时升
    //一级,同时将其复位为0)
    int nLevelRow;
    int Level;//记录当前的游戏等级:最高设为10级
}; 
extern struct Calcul_Score_Level Score_Level;
class CRussiaGame :   public CWceUiLayer
{
    WCEUI_DYNCREATE_BEGIN(CRussiaGame, CWceUiLayer)
public:
    CRussiaGame(void);
    ~CRussiaGame(void);

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
    virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
    void InitContorl(CScore *pScore){m_pScore = pScore;}
    void SetScore(int nLevel, int nScore);
    virtual void SetProperties(PCWceUiXmlElem pCreateElem);
    virtual void OnInitLayer();
    virtual void OnTimer(WORD wIDEvent);
    void GenerateSqaure(void);

    void NextSquare(int nextsquare);
    void InitGlobal();
    void Start();
    void Pause();
    void Replay();
    GAME_STATE GetGameState();
    void Exit(BOOL bSound= TRUE);
    void Rotate();
    void Left();
    void Down();
    void Right();
public:

    int			iHorGridCount;
    int			iVerGridCount;
    int			iGameLeft;
    int			iGameTop;

    int			iBlockWD;	//每个小方块的宽度
    int			iBlockHT;	//每个小方块的高度

    int Square_Kind;
    int Next_Square_Kind;

    UINT	nTimeID;
    UINT	m_nStartSpeed;		//起始速度
    UINT	SpeedDifference;	//速度差

    GAME_STATE  m_nGameState;

    TCHAR  m_strlevel[MAX_PATH];
    TCHAR  m_strscore[MAX_PATH];
    CWceUiLoadBitmap m_pblockbmp;
    CWceUiLoadString m_strload;            //获取字符

    CSquareBase *m_square;
    CSquareBase *m_1square;
    CSquareBase *m_2square;
    CSquareBase *m_3square;
    CSquareBase *m_4square;
    CSquareBase *m_5square;
    CSquareBase *m_6square;
    CSquareBase *m_7square;
    CScore  *m_pScore;

    
};
WCEUI_DYNCREATE_END(CRussiaGame, CWceUiLayer)