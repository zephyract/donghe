#pragma once
#include "animdlg.h"
#include "LinkGame.h"
class CLinkDlg :  public CAnimDlg
{
WCEUI_DYNCREATE_BEGIN(CLinkDlg, CAnimDlg)
public:
    CLinkDlg(void);
    ~CLinkDlg(void);

    virtual void OnInitDialog();
    virtual void OnBnClick(PCWceUiButton pButton);
    
public:
    CLinkGame*		m_pLinkGame;
    UINT			m_nMenuKeyMsg;
    CWceUiButton*   m_pSortBtn;
    CWceUiLayer*   m_ptimeStatic;

};
WCEUI_DYNCREATE_END(CLinkDlg, CAnimDlg)
