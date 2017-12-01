#pragma once
#include "wceuilayer.h"


#include "Define.h"
#include "SearchEngine.h"
#include "NegaMaxEngine.h"
#include "AlphaBetaEngine.h"
#include "FAlphaBetaEngine.h"
#include "AspirationSearch.h"
#include "PVS_Engine.h"
#include "AlphaBeta_TTEngine.h"
#include "IDAlphaBetaEngine.h"
#include "Alphabeta_HH.h"
#include "MTD_fEngine.h"
#include "NegaScout_TT_HH.h"
#include "TranspositionTable.h"
#include "WceUiLoadBitmap.h"
#include "ShowMove.h"
#include <stack>
using namespace std;



#define     MSG_GAME_END   WM_APP+2000
//#include "GradientProgressCtrl.h"

#define  SAFE_DELEFE_PT(PT) \
    if ( NULL!= PT) \
    { \
    delete PT; \
    PT = NULL; \
    }

typedef struct
{
    BYTE nChessID;
    POINT ptMovePoint;
}MOVECHESS;

typedef enum{	Chessing,SetChessBoarding,Previewing	}GameStatus;


typedef struct _GAMEINFO
{
    short		iGridWD;				//棋盘上每个格子的宽度
    short		iGridHT;				//棋盘上每个格子的高度

    short		iX_START;				//(第一个棋子的X起始位置)
    short		iY_START;				//(第一个棋子的Y起始位置)	

}GAMEINFO,*PGAMEINFO;
class CChinaChessGame :
    public CWceUiLayer
{
WCEUI_DYNCREATE_BEGIN(CChinaChessGame,CWceUiLayer)
public:
    CChinaChessGame(void);
    ~CChinaChessGame(void);
    virtual void SetProperties(PCWceUiXmlElem pCreateElem);
    virtual void OnInitLayer();
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
    virtual void OnLButtonDown(UINT nFlags, POINT point);


    virtual void OnLButtonUp(UINT nFlags, POINT point);
    CString			GetMoveStr(int nFromX,int nFromY,int nToX,int nToY,int nSourceID);
    CString			ConvertDigit2Chinese(int nNum);	//转换数字为汉字

    int				IsGameOver(BYTE position[][9]);
    void			InvertChessBoard(BYTE cb[][9]);	//对换红黑双方棋子
    //记录下棋步骤
    void			AddChessRecord(int nFromX,int nFromY,int nToX,int nToY,int nUserChessColor,int nSourceID,BOOL bFirstStep );
    void            InitGame();
    void    ShowStatus(LPCTSTR pText);
    inline void     InitStatusStaic(PCWceUiLayer pStatic)
    {
        m_pStatusStatic = pStatic;
    }
    inline void     InitProgressSlider(CCCSlider* pSlider)
    {
        m_pProgressThink = pSlider;
    }

    void            InitShowMove(PCShowMove pShowMove)
    {
        m_pmoverecordlist = pShowMove;
    }
    void              OnBnClickedBtnUndo();
    void              OnBnClickedBtnReplay();

public:
    void		Think();
    //static DWORD WINAPI ThreadProc( LPVOID lpParameter);

    void              OnBnClickedBtnClose(BOOL bsound = TRUE);

    CMoveGenerator*		m_pMG;
    CEveluation*		m_pEvel;
    CSearchEngine*		m_pSE;					//搜索引擎指针

    CPoint				m_MousePoint;           //当前鼠标所在点
    GameStatus			m_Status;               //当前状态
    bool				m_bIsThinking;          //电脑是否正在想
    bool				m_bIsBegin;             //是否已开始下棋

    //int m_nWillChessColor;         //下一步该哪方下

    CHESSMOVE	m_cmBestMove;        //最佳走法
    UNDOMOVE	m_umUndoMove;        //悔棋走法
    int			m_iChessSort;        //下棋方式
    int			m_iDepthSort;        //深度方式
    int			m_iBout;			 //已下的回合数,如有悔棋算减

    stack<UNDOMOVE> m_stackUndoMove;	//记录走法的栈，便于悔棋
    stack<UNDOMOVE> m_stackRedoMove;	//记录已悔棋的走法的栈，便于还原

    HANDLE		m_hHandle;
    DWORD		m_dwThreadID;
    bool		m_bIsGameOver;
    int			m_iWhoChess;					//下步该谁走棋
    int			m_nUserChessColor;				//用户棋子颜色

    BYTE		m_byChessBoard[10][9];			//棋盘数组，用于显示棋盘
    BYTE		m_byBackupChessBoard[10][9];	//备份棋盘数组，用于出错恢复
    BYTE		m_byShowChessBoard[10][9];		//用于显示的棋盘

    MOVECHESS		m_MoveChess;				 //用于保存当前被拖拽的棋子的结构
    POINT			m_ptMoveChess;				 //用于保存当前被拖拽的棋子的位置


    CRect			m_rcGameArea;				 //游戏区域

    short			m_iEatCount;				//吃掉电脑棋子个数


    ///////////////////////////////////////////////////////////////
    //UI添加的代码
protected:
    //最后走的棋子
    CSize		m_SizeChessBmp;				 //单个棋子BMP的大小

    int			m_nBoardWidth;
    int			m_nBoardHeight;

    int			m_LastChessX;
    int			m_LastChessY;
    BYTE		m_LastChess;
    

    CWceUiLoadBitmap m_pchessbmp;
    CWceUiLoadBitmap m_pchesscheckbmp;

    CWceUiLayer *m_pStatusStatic;
    CCCSlider *m_pProgressThink;
    CShowMove*   m_pmoverecordlist;
    //初始化游戏有关的信息：横,纵向棋格数  以及游戏位置等
    void			  SetDefaultGameInfo();
};
WCEUI_DYNCREATE_END(CChinaChessGame,CWceUiLayer)
