#include "StdAfx.h"
#include "ChinaChessDlg.h"
#include "WceUiRealWnd.h"
#include "WceUiButton.h"

CChinaChessDlg::CChinaChessDlg(void)
{
    m_pStatusStatic     =   NULL;  
    m_pProgressThink    =   NULL;
    m_pmoverecordlist    =   NULL;
}

CChinaChessDlg::~CChinaChessDlg(void)
{
}

void CChinaChessDlg::OnInitDialog()
{
    m_pStatusStatic     = (CWceUiLayer *)GetChildByName(TEXT("gamestatus"));
    m_pProgressThink    = (CCCSlider *)GetChildByName(TEXT("pcthink_slider"));
    m_pmoverecordlist    = (CShowMove *)GetChildByName(TEXT("showmove"));
    m_pChinachess = (CChinaChessGame *)GetChildByName(TEXT("GameArea"));
    InitGame();   


	LPCTSTR name[] = {L"exit", L"resort", L"replay"};
	for (int i=0; i<sizeof(name)/sizeof(name[0]); i++)
	{
		CWceUiButton* pbutton = (CWceUiButton*)GetChildByName(name[i]);
		if (pbutton)
		{
			pbutton->SetUserData(1);	// APP²»ÏìBEEP
		}
	}
}

void CChinaChessDlg::OnBnClick( PCWceUiButton pButton )
{
    if (pButton->IsEqualName(TEXT("exit")))
    {
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
		ui_play_game_sound(L"gameover.wav");
    }
    if (pButton->IsEqualName(TEXT("resort")))
    {
        m_pChinachess->OnBnClickedBtnUndo();
    }
    if (pButton->IsEqualName(TEXT("replay")))
    {
        m_pChinachess->OnBnClickedBtnReplay();
    }
}


void CChinaChessDlg::InitGame()
{
    m_pChinachess->InitStatusStaic(m_pStatusStatic);
    m_pChinachess->InitProgressSlider(m_pProgressThink);
    m_pChinachess->InitShowMove(m_pmoverecordlist);
    m_pChinachess->InitGame();
}