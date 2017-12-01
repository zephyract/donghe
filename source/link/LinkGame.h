#pragma once
#include "wceuilayer.h"
#include "wceuibutton.h"
#include "WceUiLoadBitmap.h"

#define		TIMERID_NEWGAME		2001
#define		TIMERID_TIMING		2002

#define		MAX_ROW		20		//�������
#define		MAX_COL		20		//�������
#define		MAX_TIME_EACH			(5*60)		//��Ϸ���ʱ��
#define		MAX_SORT_COUNT			4			//����������

typedef struct _GAMEINFO
{
    int bmpxOffset;				//����ͼƬ�뷽����߾���
    int bmpyOffset;				//����ͼƬ�뷽�򶥱߾���

    //������ز���	
    COLORREF	clGameBk;				//������ɫ
    int			iOneFrontWidth;			//ǰ�淽��Ŀ��  
    int			iOneFrontHeight;		//ǰ�淽��ĸ߶�

    int			iOneBkWidth;			//��������Ŀ��	ʵ��һ������Ŀ��
    int			iOntBkHeight;			//��������ĸ߶�

    //��Ϸ��ز���
    int			iGameAreaLeft;			//��Ϸ��ʼλ��
    int			iGameAreaTop;			//��Ϸ��ʼλ��-������
    CSize		szGameArea;				//��Ϸ�����С
    int			iRowCount;				//��Ϸ������
    int			iColumnCount;			//����	

    COLORREF	clSelFrame;				//ѡ�ֿ����ɫ
    USHORT		usSelFrameWD;			//ѡ�ֿ�ı߿���

    int			iTotalAnimal;			//��Ϸ���ܶ���ĸ���
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

    int		m_map[MAX_ROW][MAX_COL];        //��̬��ͼ����

    int		m_nRow;		  //��ͼ������(����)
    int		m_nCol;		  //��ͼ������(����)
    int		m_nLevelAni;  //��ǰ�ȼ�����ĸ���	


    int		m_nX1;        //���ѡ�еļ�¼��������
    int		m_nY1;	      //���ѡ�еļ�¼��������


    //��ǰ�ص�ʱ����
    int				m_iNowTick;
    int				m_iNowLevel;
    int				m_iSortCount;		//û����Ϸ�������

    BOOL			m_bGameEnd;			//��Ϸ�Ƿ����

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
