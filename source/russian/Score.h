
#pragma once
#include "wceuilayer.h"
#include "WceUiLoadBitmap.h"
struct Score
{
    TCHAR strlevel[MAX_PATH];
    TCHAR strscore[MAX_PATH];
};

class CScore :    public CWceUiLayer
{
WCEUI_DYNCREATE_BEGIN(CScore, CWceUiLayer)
public:
    CScore(void);
    ~CScore(void);
    virtual void OnDraw();
    virtual void SetProperties(PCWceUiXmlElem pCreateElem);
    virtual void OnInitLayer();
    void  SetScore(TCHAR* strlevel ,TCHAR* strscore);
    void  SetPtPromt(POINT *pt);
    BOOL IsSamePoints(POINT *pt);
   // void  SetNoDraw(BOOL NoDraw){m_bNoDraw = NoDraw;}
public:
    CWceUiLayer    *m_pScoreStatic;
    CWceUiLayer    *m_pLevelStatic;

    int			    iPromptLeft;
    int			    iPromptTop;
    int			    iBlockWD;	//每个小方块的宽度
    int			    iBlockHT;	//每个小方块的高度
    POINT           ptPromt[4]; //提示方块array_status的几个坐标
    Score           m_score;
    BOOL            m_bNoDraw;
    CWceUiLoadBitmap m_pblockbmp;
 
};
WCEUI_DYNCREATE_END(CScore, CWceUiLayer)
