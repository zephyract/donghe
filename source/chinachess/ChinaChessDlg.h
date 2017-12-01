#pragma once
#include "animdlg.h"
#include "ChinaChessGame.h"
#include "CCSlider.h"

class CChinaChessDlg :   public CAnimDlg
{
WCEUI_DYNCREATE_BEGIN(CChinaChessDlg, CAnimDlg)
public:
    CChinaChessDlg(void);
    ~CChinaChessDlg(void);
    virtual void OnInitDialog();

    void InitGame();
    virtual void OnBnClick(PCWceUiButton pButton);
public:
    CChinaChessGame *m_pChinachess;
    CWceUiLayer *m_pStatusStatic;
    CCCSlider *m_pProgressThink;
    CShowMove*   m_pmoverecordlist;

};
WCEUI_DYNCREATE_END(CChinaChessDlg, CAnimDlg)
