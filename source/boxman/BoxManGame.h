#pragma once
#include "wceuilayer.h"
#include "WceUiButton.h"
#include "GameBoxMan.h"
#include "WceUiLoadBitmap.h"

#define BLOCK_WIDTH    iBlockWidth   //  ¿í¶È
#define BLOCK_HEIGHT    iBlockWidth  //  Éî¶È

class CBoxManGame :  public CWceUiLayer
{
WCEUI_DYNCREATE_BEGIN(CBoxManGame, CWceUiLayer)
public:
    CBoxManGame(void);
    ~CBoxManGame(void);
    virtual void SetProperties(PCWceUiXmlElem pCreateElem);
    virtual void OnInitLayer();
    virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
    virtual void OnLButtonDown(UINT nFlags, POINT point);
    virtual void OnLButtonUp(UINT nFlags, POINT point);

    void OnBtnDown();
    void OnBtnRight();
    void OnBtnLeft()
    {
        SendKey(VK_LEFT);
    }
    void OnBtnUp();
    void NextLevel();
    void PreLevel();
    void RePlay();
    void Exit(BOOL bSound = TRUE);
    void SendKey(UINT nChar);
	void DrawBackGroup(int x, int y, CWceUiGenericBitmap* pWndBitmap);
	void DrawWhiteWall(int x, int y, CWceUiGenericBitmap* pWndBitmap);
	void DrawBlueWall(int x, int y, CWceUiGenericBitmap* pWndBitmap);
	void DrawBall(int x, int y, CWceUiGenericBitmap* pWndBitmap);

	void DrawBmp( CWceUiGenericBitmap* pWndBitmap, int x, int y ,CWceUiGenericBitmap* pSrcBitmap);
	void DrawYellowBox(int x, int y, CWceUiGenericBitmap* pWndBitmap);
	void DrawRedBox(int x, int y, CWceUiGenericBitmap* pWndBitmap);
	void DrawManWall(int x, int y, CWceUiGenericBitmap* pWndBitmap);
	void DrawManBall(int x, int y, CWceUiGenericBitmap* pWndBitmap);

    RECT GetRectByStr( LPCTSTR strRect );

    SIZE GetSizeByStr( LPCTSTR strSZ );

    void CopyScreen( HBITMAP hbmp, LPRECT prcSrc );


public:
    CGameManBoxMan m_boxMan;
    TCHAR	m_tmpMap[M_TAB_HEIGHT][M_TAB_WIDTH];

    CWceUiLoadBitmap m_pyellowboxbmp;
    CWceUiLoadBitmap m_predboxbmp;
    CWceUiLoadBitmap m_pwhitewallbmp;
    CWceUiLoadBitmap m_pbluebmp;
    CWceUiLoadBitmap m_pmanbmp;
    CWceUiLoadBitmap m_pmapballbmp;
	CWceUiLoadBitmap m_bmpblock;
};
WCEUI_DYNCREATE_END(CBoxManGame, CWceUiLayer)

