#pragma once
#include "AnimDlg.h"
#include "RussiaGame.h"

class CRussianDlg : public CAnimDlg
{
    WCEUI_DYNCREATE_BEGIN(CRussianDlg, CAnimDlg)
public:
    CRussianDlg(void);
    ~CRussianDlg(void);
    virtual void OnInitDialog();

    virtual void OnBnClick(PCWceUiButton pButton);
    void  UpdateSPBtn();
    DWORD GetBKIndex();
public:
   CRussiaGame *m_pRussiaGame;
   CScore  *m_pScore;

};
WCEUI_DYNCREATE_END(CRussianDlg, CAnimDlg)
