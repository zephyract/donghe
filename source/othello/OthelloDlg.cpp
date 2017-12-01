#include "StdAfx.h"
#include "OthelloDlg.h"
#include "WceUiRealWnd.h"
#include <atlbase.h>


COthelloDlg::COthelloDlg(void)
{
    m_pTxt_NumBlack = NULL;
    m_pTxt_NumWhite = NULL;
    m_pTxt_Msg      = NULL;
    m_pBtnPC        = NULL;
    m_pBtnPeople    = NULL;
}

COthelloDlg::~COthelloDlg(void)
{
}

void COthelloDlg::OnInitDialog()
{
    m_pOthello = (CMyOthello *)GetChildByName(TEXT("gamearea"));
    m_pBtnPC = (CWceUiButton *)GetChildByName(TEXT("pcmode"));
    m_pBtnPeople = (CWceUiButton *)GetChildByName(TEXT("peoplemode"));
    m_pTxt_NumWhite = (CWceUiLayer *)GetChildByName(TEXT("white_txt"));
    m_pTxt_NumBlack = (CWceUiLayer *)GetChildByName(TEXT("black_txt"));
    m_pTxt_Msg      = (CWceUiLayer *)GetChildByName(TEXT("showtip"));
    InitGame();


	LPCTSTR name[] = {L"exit", L"start", L"undo", L"pcmode", L"peoplemode"};
	for (int i=0; i<sizeof(name)/sizeof(name[0]); i++)
	{
		CWceUiButton* pbutton = (CWceUiButton*)GetChildByName(name[i]);
		if (pbutton)
		{
			pbutton->SetUserData(1);	// APP²»ÏìBEEP
		}
	}
}


void COthelloDlg::OnBnClick( PCWceUiButton pButton )
{
    if (pButton->IsEqualName(TEXT("start")))
    {
        m_pOthello->OnBtnStart();
    }
    if (pButton->IsEqualName(TEXT("undo")))
    {
        //if (!m_bComputerMov)
        m_pOthello->MoveBack();

    }
    if (pButton->IsEqualName(TEXT("pcmode")))
    {
        // if ( !m_bComputerMov )
        m_pOthello->OnManToMan();  
    }
    if (pButton->IsEqualName(TEXT("peoplemode")))
    {
        //if ( !m_bComputerMov )
        m_pOthello->OnManToMan();     
    }
    if (pButton->IsEqualName(TEXT("exit")))
    {
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
		ui_play_game_sound(L"gameover.wav");
    }
}

void COthelloDlg::InitGame()
{
    m_pOthello->InitControls(m_pBtnPC,m_pBtnPeople,m_pTxt_NumBlack,m_pTxt_NumWhite,m_pTxt_Msg);
    m_pOthello->InitGame();
}