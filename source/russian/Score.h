
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
    int			    iBlockWD;	//ÿ��С����Ŀ��
    int			    iBlockHT;	//ÿ��С����ĸ߶�
    POINT           ptPromt[4]; //��ʾ����array_status�ļ�������
    Score           m_score;
    BOOL            m_bNoDraw;
    CWceUiLoadBitmap m_pblockbmp;
 
};
WCEUI_DYNCREATE_END(CScore, CWceUiLayer)
