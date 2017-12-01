#pragma once
#include "wceuilayer.h"
#include "GoBangGame.h"
#include "WceUiLoadString.h"
#include "WceUiLoadBitmap.h"
#include "WceUiButton.h"

//WPARAM  x   LPARAM  Y
#define	 WM_MSG_DRAWSTONE			(WM_USER+3000)

//LPARAM = 1 ��ʾ�����������
#define	 WM_MSG_JUDGERESULT			(WM_USER+3001)

//-------------------------------------------------------------------------------------------

typedef struct _GAMEINFO
{
    int			iGridWD ;			//ÿ��С��Ŀ��
    int			iGridHT ;			//ÿ��С��ĸ߶�
    CPoint		m_ptGameStart;		//������ʼλ�� 

    USHORT		m_xGridCount;		//���̺������
    USHORT		m_yGridCount;		//�����ݸ�����

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
    //����ת�� 
    //����Ļ������ת���������ϵĸ��ӣ��� �������ϵĸ���ת���ɶ�Ӧ�Ĵ��ڵ�����
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
    CWceUiButton  *m_pcbtn;                 //�˻�
    CWceUiButton  *m_ppbtn;                 //����
    CWceUiLayer *m_pTxt_Msg;
    CWceUiLayer *m_pTxt_Level;
    BOOL			m_bNotSound;
    CString			m_StrLevelPreFix;		//�ȼ���Ϣǰ׺	
    UINT			m_nMenuKeyMsg;
    BOOL		debug;					//��־�Ƿ��ڵ��Գ���״̬
    BOOL		m_brenren;				//�Ƿ�������ģʽ
    BOOL		g_bStop;				//��Ϸ�Ƿ����
    CGoBangGame fivestone;
    BOOL			 bComputerFirst;		//��ʾ������Ƿ����� 
    int	 PonitsChess[X_MAX_GRID][Y_MAX_GRID];
    volatile BOOL	  g_bPCThinking;
    POINT	steps[225];						//����һ�����ߵ���
    int		stepcount;						//�ߵ�������
    int		g_nGrade;						//1-5����ʾ�������������
    GAMEINFO g_GameInfo;
};
WCEUI_DYNCREATE_END(CMyGoBang, CWceUiLayer)
