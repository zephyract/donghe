#pragma once
#include "AnimDlg.h"
#include "BoxManGame.h"
void OnLanguageChange();
class CBoxManDlg :    public CAnimDlg
{
WCEUI_DYNCREATE_BEGIN(CBoxManDlg, CAnimDlg)
public:
    CBoxManDlg(void);
    ~CBoxManDlg(void);
    virtual void OnInitDialog();
    virtual void OnBnClick(PCWceUiButton pButton);
public:
    CBoxManGame *m_pBoxManGame;
    UINT		 m_nMenuKeyMsg;

};
WCEUI_DYNCREATE_END(CBoxManDlg, CAnimDlg)
