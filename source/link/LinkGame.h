#pragma once
#include "wceuilayer.h"
#include "wceuibutton.h"
#include "WceUiLoadBitmap.h"

#define		TIMERID_NEWGAME		2001
#define		TIMERID_TIMING		2002

#define		MAX_ROW		20		//最大行数
#define		MAX_COL		20		//最大列数
#define		MAX_TIME_EACH			(5*60)		//游戏最大时间
#define		MAX_SORT_COUNT			4			//最多排序次数

typedef struct _GAMEINFO
{
    int bmpxOffset;				//动物图片与方框左边距离
    int bmpyOffset;				//动物图片与方框顶边距离

    //绘制相关参数	
    COLORREF	clGameBk;				//背景颜色
    int			iOneFrontWidth;			//前面方块的宽度  
    int			iOneFrontHeight;		//前面方块的高度

    int			iOneBkWidth;			//背景方块的宽度	实际一个动物的宽度
    int			iOntBkHeight;			//背景方块的高度

    //游戏相关参数
    int			iGameAreaLeft;			//游戏开始位置
    int			iGameAreaTop;			//游戏开始位置-到顶端
    CSize		szGameArea;				//游戏区域大小
    int			iRowCount;				//游戏的行数
    int			iColumnCount;			//列数	

    COLORREF	clSelFrame;				//选种框的颜色
    USHORT		usSelFrameWD;			//选种框的边框宽度

    int			iTotalAnimal;			//游戏中总动物的个数
}GAMEINFO,*PGAMEINFO;

class CLinkGame :  public CWceUiLayer
{
WCEUI_DYNCREATE_BEGIN(CLinkGame, CWceUiLayer);
public:
    CLinkGame(void);
    ~CLinkGame(void);
    virtual void OnDraw( CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw );

    virtual void SetProperties(PCWceUiXmlElem pCreateElem);
    virtual void OnInitLayer();
    virtual void OnLButtonDown(UINT nFlags, POINT point);
   // virtual void OnClick(PCWceUiButton pButton);
    void InitTimeStatic(PCWceUiLayer pStatic);
    void InitSortBtn(PCWceUiButton pButton);

    void Replay(BOOL bsound = TRUE);
    void Resort();
    virtual void OnTimer(WORD wIDEvent);
    void SetDefaultGameInfo();

    CSize CalculateGameAeraSize( GAMEINFO *pGameInfo );
    RECT GetRectByStr(LPCTSTR strRect);
    SIZE GetSizeByStr(LPCTSTR strSZ);
    void CopyScreen(HBITMAP hbmp, LPRECT prcSrc);

    void StartNewGame(BOOL bFirstLevel = TRUE);
    void  StartTiming();
    
    BOOL IsLink(int x1, int y1, int x2, int y2);
    BOOL IsWin(void);
    BOOL X1_Link_X2(int x, int y1,int y2);
    BOOL Y1_Link_Y2(int x1,int x2,int y);
    BOOL LineX(int x,int y1,int y2);
    BOOL LineY(int x1,int x2,int y);
    BOOL OneCornerLink(int x1, int y1,int x2, int y2);
    BOOL TwoCornerLink(int x1, int y1, int x2, int y2);
    BOOL XThrough(int x, int y, BOOL bAdd);
    BOOL YThrough(int x, int y,BOOL bAdd);
    BOOL HorSortAnimalPlace( int lineIndex,BOOL bMoveLeft );
    BOOL VerSortAnimalPlace( int ColumnIndex,BOOL bMoveBottom );
    BOOL AllSortAnimalPlace();
    BOOL DealDifficultyGame( CPoint ptOldSel,CPoint ptNowSet );
    void Exit(BOOL bSound = TRUE);

protected:

    int		m_map[MAX_ROW][MAX_COL];        //动态地图数据

    int		m_nRow;		  //地图的行数(虚拟)
    int		m_nCol;		  //地图的列数(虚拟)
    int		m_nLevelAni;  //当前等级动物的个数	


    int		m_nX1;        //鼠标选中的记录方块列数
    int		m_nY1;	      //鼠标选中的记录方块行数


    //当前关的时间数
    int				m_iNowTick;
    int				m_iNowLevel;
    int				m_iSortCount;		//没论游戏帮组次数

    BOOL			m_bGameEnd;			//游戏是否结束

    UINT			m_TimerNewGame;
    USHORT			m_iTickCount;

    CWceUiGenericBitmap* m_pbackbmp;
    CWceUiGenericBitmap* m_pfrontbmp;
    CWceUiGenericBitmap* m_pgameoverbmp;
    CWceUiLoadBitmap m_bmpblock;
    CWceUiLoadBitmap m_bmpbblock;
    CWceUiLoadBitmap m_bmpgameoverblock;
	CWceUiLoadBitmap m_bmpSelect;
    
    CWceUiButton*     m_pSortBtn;
    CWceUiLayer*    m_ptimeStatic;

};
WCEUI_DYNCREATE_END(CLinkGame, CWceUiLayer);
