#pragma once
#include "wceuilayer.h"
#include "OthelloGame.h"
#include "WceUiButton.h"
#include "WceUiLoadString.h"
#include "WceUiLoadBitmap.h"

#define     MSG_GAME_END   (WM_USER+2000)
#define		UM_COMPTRUN   (WM_USER +2001)  

#define		TRUN_CHESS_DELAY		6
#define     _USE_MAKE_SOUND
#define		MAIN_DLG_TITLE			TEXT("GAME_OTHELLO")
#define     TRANSPARENT_COLOR       RGB(255,0,255)


struct	GameInfo
{
    CPoint		m_ptMapStart;			//������ʼλ��
    int			iGridWD ;				//ÿ��С��Ŀ��
    int			iGridHT ;				//ÿ��С��ĸ߶�

    CSize		m_szStone;				//���ӵĴ�С

};

#define		NUM_BMP				10
#define     NUM_MOVEBMP          8
enum	GAME_STATE{ GM_PLAYING,GM_COMPWIN,GM_MANWIN };

class CMyOthello :    public CWceUiLayer
{
WCEUI_DYNCREATE_BEGIN(CMyOthello, CWceUiLayer)

public:
    CMyOthello(void);
    ~CMyOthello(void);
    virtual void SetProperties(PCWceUiXmlElem pCreateElem);
    virtual void OnInitLayer();

    void InitGame();
    void OnBnClickedBtnClose(BOOL bSound = TRUE);
    void UpdateMode();
    virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
    virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
    virtual void OnLButtonDown(UINT nFlags, POINT point);
    virtual void OnLButtonUp(UINT nFlags, POINT point);
    virtual void OnBnClick(PCWceUiButton pButton);
    void InitControls(PCWceUiButton pPCButton,PCWceUiButton pPeopleButton,PCWceUiLayer pNumBStatic,PCWceUiLayer pNumWStatic,PCWceUiLayer pTipStatic)
    {
        m_pBtnPC        = pPCButton;
        m_pBtnPeople    = pPeopleButton;
        m_pTxt_NumBlack = pNumBStatic;
        m_pTxt_NumWhite = pNumWStatic;
        m_pTxt_Msg      = pTipStatic;
    }
    void OnBtnStart();
    void SetDefaultGameInfo();
    LRESULT OnTranChess(WPARAM wParam, LPARAM lParam);
    void	TrunOverChess( int row, int col, UINT8 obcolor );
    void ShowResult();
    void  OnNewGame();
    void OnManToMan();
    void MoveBack();
    // CChessBoard message handlers
    LRESULT OnComRun(WPARAM wParam, LPARAM lParam);
    LRESULT OnChessMove(WPARAM wParam, LPARAM lParam);
    LRESULT OnGameOver(WPARAM wParam, LPARAM lParam);
    void MyShowMessage( CString strLangKey );
    void DrawValidMovPos( UINT nCHessID,CWceUiGenericBitmap* pWndBitmap);

    void DrawSpecialStone( int x,int y, HDC hdc = NULL);

public:
    BOOL		m_bFirstPaint;
    BOOL		m_bComputerMov;			//�Ƿ��ǵ�������
    BOOL		m_bMantoMan;			//�Ƿ������˶���
    USHORT		m_UGameState;
    UINT8		m_nCurMovChess;			//��ǰ���������


    PTVECAY		m_vecW;
    PTVECAY		m_vecB;

    GameInfo			m_GameInfo;
    board_type			m_oChessBoard;
    UINT				m_nMenuKeyMsg;
    
    CWceUiLoadString    m_strload;
    CWceUiLoadBitmap   m_pbmp[NUM_BMP];
    CWceUiButton        *m_pBtnPC;
    CWceUiButton        *m_pBtnPeople;
    CWceUiLayer		*m_pTxt_NumBlack;
    CWceUiLayer		*m_pTxt_NumWhite;
    CWceUiLayer		*m_pTxt_Msg;
};
WCEUI_DYNCREATE_END(CMyOthello, CWceUiLayer)
