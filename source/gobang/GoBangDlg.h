#pragma once
#include "animdlg.h"
#include "MyGoBang.h"

class CGoBangDlg :  public CAnimDlg
{
WCEUI_DYNCREATE_BEGIN(CGoBangDlg, CAnimDlg)
public:
    CGoBangDlg(void);
    ~CGoBangDlg(void);
    virtual void OnInitDialog();
    void InitGame();
    virtual void OnBnClick(PCWceUiButton pButton);
public:
    UINT	m_nMenuKeyMsg;
    CMyGoBang *m_pGoBang;

    CWceUiButton  *m_pcbtn;                 //人机
    CWceUiButton  *m_ppbtn;                 //人人
    CWceUiLayer *m_pTxt_Msg;
    CWceUiLayer *m_pTxt_Level;

};
WCEUI_DYNCREATE_END(CGoBangDlg, CAnimDlg)
