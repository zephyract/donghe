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
    short		iGridWD;				//������ÿ�����ӵĿ��
    short		iGridHT;				//������ÿ�����ӵĸ߶�

    short		iX_START;				//(��һ�����ӵ�X��ʼλ��)
    short		iY_START;				//(��һ�����ӵ�Y��ʼλ��)	

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
    CString			ConvertDigit2Chinese(int nNum);	//ת������Ϊ����

    int				IsGameOver(BYTE position[][9]);
    void			InvertChessBoard(BYTE cb[][9]);	//�Ի����˫������
    //��¼���岽��
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
    CSearchEngine*		m_pSE;					//��������ָ��

    CPoint				m_MousePoint;           //��ǰ������ڵ�
    GameStatus			m_Status;               //��ǰ״̬
    bool				m_bIsThinking;          //�����Ƿ�������
    bool				m_bIsBegin;             //�Ƿ��ѿ�ʼ����

    //int m_nWillChessColor;         //��һ�����ķ���

    CHESSMOVE	m_cmBestMove;        //����߷�
    UNDOMOVE	m_umUndoMove;        //�����߷�
    int			m_iChessSort;        //���巽ʽ
    int			m_iDepthSort;        //��ȷ�ʽ
    int			m_iBout;			 //���µĻغ���,���л������

    stack<UNDOMOVE> m_stackUndoMove;	//��¼�߷���ջ�����ڻ���
    stack<UNDOMOVE> m_stackRedoMove;	//��¼�ѻ�����߷���ջ�����ڻ�ԭ

    HANDLE		m_hHandle;
    DWORD		m_dwThreadID;
    bool		m_bIsGameOver;
    int			m_iWhoChess;					//�²���˭����
    int			m_nUserChessColor;				//�û�������ɫ

    BYTE		m_byChessBoard[10][9];			//�������飬������ʾ����
    BYTE		m_byBackupChessBoard[10][9];	//�����������飬���ڳ���ָ�
    BYTE		m_byShowChessBoard[10][9];		//������ʾ������

    MOVECHESS		m_MoveChess;				 //���ڱ��浱ǰ����ק�����ӵĽṹ
    POINT			m_ptMoveChess;				 //���ڱ��浱ǰ����ק�����ӵ�λ��


    CRect			m_rcGameArea;				 //��Ϸ����

    short			m_iEatCount;				//�Ե��������Ӹ���


    ///////////////////////////////////////////////////////////////
    //UI��ӵĴ���
protected:
    //����ߵ�����
    CSize		m_SizeChessBmp;				 //��������BMP�Ĵ�С

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
    //��ʼ����Ϸ�йص���Ϣ����,���������  �Լ���Ϸλ�õ�
    void			  SetDefaultGameInfo();
};
WCEUI_DYNCREATE_END(CChinaChessGame,CWceUiLayer)
