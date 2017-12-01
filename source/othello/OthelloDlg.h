#pragma once
#include "MyOthello.h"
#include "AnimDlg.h"

class COthelloDlg : public CAnimDlg
{
WCEUI_DYNCREATE_BEGIN(COthelloDlg, CAnimDlg)
public:
    COthelloDlg(void);
    ~COthelloDlg(void);
    virtual void OnInitDialog();

    void InitGame();
    virtual void OnBnClick(PCWceUiButton pButton);

    UINT	m_nMenuKeyMsg;
    CMyOthello *m_pOthello;

    CWceUiButton        *m_pBtnPC;
    CWceUiButton        *m_pBtnPeople;
    CWceUiLayer		*m_pTxt_NumBlack;
    CWceUiLayer		*m_pTxt_NumWhite;
    CWceUiLayer		*m_pTxt_Msg;

};
WCEUI_DYNCREATE_END(COthelloDlg, CAnimDlg)
