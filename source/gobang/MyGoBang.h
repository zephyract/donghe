#pragma once
#include "wceuilayer.h"
#include "GoBangGame.h"
#include "WceUiLoadString.h"
#include "WceUiLoadBitmap.h"
#include "WceUiButton.h"

//WPARAM  x   LPARAM  Y
#define	 WM_MSG_DRAWSTONE			(WM_USER+3000)

//LPARAM = 1 表示电脑下棋结束
#define	 WM_MSG_JUDGERESULT			(WM_USER+3001)

//-------------------------------------------------------------------------------------------

typedef struct _GAMEINFO
{
    int			iGridWD ;			//每个小格的宽度
    int			iGridHT ;			//每个小格的高度
    CPoint		m_ptGameStart;		//棋盘起始位置 

    USHORT		m_xGridCount;		//棋盘横格数量
    USHORT		m_yGridCount;		//棋盘纵格数量

}GAMEINFO,*PGAMEINFO;




//-------------------------------------------------------------------------------------------
enum  STONE_CL { BLACK_STONE=1, WHITE_STONE=2, DOWN_BLACK_STONE=3, DOWN_WHITE_STONE=4 };
class CMyGoBang :
    public CWceUiLayer
{
WCEUI_DYNCREATE_BEGIN(CMyGoBang, CWceUiLayer)

public:
    CMyGoBang(void);
    ~CMyGoBang(void);
    virtual void SetProperties(PCWceUiXmlElem pCreateElem);
    virtual void OnInitLayer();
    BOOL IsThink();
    void InitGame();
    virtual void OnTimer(WORD wIDEvent);
    void UpdateModeBtn();
    virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
    virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
    virtual void OnLButtonDown(UINT nFlags, POINT point);
    virtual void OnLButtonUp(UINT nFlags, POINT point);
    virtual void OnClick(PCWceUiButton pButton);
    void SetDefaultGameInfo();
    LRESULT	OnMSGJudgeResule( WPARAM wp,LPARAM lp );
    BOOL  JudgeGameResult();
    DWORD computerprocess(LPVOID pParam = NULL);
    //坐标转换 
    //将屏幕的坐标转换成棋盘上的格子，和 将棋盘上的格子转换成对应的窗口的坐标
    void CoordInvert(int xSrc,int ySrc,int *pxDst,int *pyDst,BOOL bWndToGrid  );
    void OnStart();
    void OnBnClickedBtnClose(BOOL bsound =1);
    void OnBnClickedBtnMantoman();
    void MyShowMessage( LPCTSTR StrLangKey );
    void OnBnClickedBtnUndo();
    void OnBtnNextLevel();

    void ShowLevel();
    void OnBtnPreLevel();
    void InitPCBtn(PCWceUiButton pBtn)
    {
        m_pcbtn = pBtn;
    }
    void InitPPBtn(PCWceUiButton pBtn)
    {
        m_ppbtn = pBtn;
    }
    void InitTipStatic(PCWceUiLayer pStatic)
    {
        m_pTxt_Msg = pStatic;
    }

    void InitLevelStatic(PCWceUiLayer pStatic)
    {
        m_pTxt_Level = pStatic;
    }

	void DrawSpecStone( int x,int y,BOOL bWndCoord,CWceUiGenericBitmap* pWndBitmap);
public:
    //CWceUiLoadBitmap m_bmpload;
    CWceUiLoadBitmap m_pwhitebmp;
    CWceUiLoadBitmap m_pwhitedownbmp;
    CWceUiLoadBitmap m_pdarkbmp;
    CWceUiLoadBitmap m_pdarkdownbmp;
    
    POINT prept;
    CWceUiLoadString m_strload;
    CWceUiButton  *m_pcbtn;                 //人机
    CWceUiButton  *m_ppbtn;                 //人人
    CWceUiLayer *m_pTxt_Msg;
    CWceUiLayer *m_pTxt_Level;
    BOOL			m_bNotSound;
    CString			m_StrLevelPreFix;		//等级信息前缀	
    UINT			m_nMenuKeyMsg;
    BOOL		debug;					//标志是否处于调试程序状态
    BOOL		m_brenren;				//是否是人人模式
    BOOL		g_bStop;				//游戏是否结束
    CGoBangGame fivestone;
    BOOL			 bComputerFirst;		//表示计算机是否先走 
    int	 PonitsChess[X_MAX_GRID][Y_MAX_GRID];
    volatile BOOL	  g_bPCThinking;
    POINT	steps[225];						//保存一盘中走的棋
    int		stepcount;						//走的总棋数
    int		g_nGrade;						//1-5，表示计算机的智力等
    GAMEINFO g_GameInfo;
};
WCEUI_DYNCREATE_END(CMyGoBang, CWceUiLayer)
