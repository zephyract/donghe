#include "StdAfx.h"
#include "BoxManDlg.h"

CBoxManDlg::CBoxManDlg(void)
{
}

CBoxManDlg::~CBoxManDlg(void)
{
}

void CBoxManDlg::OnInitDialog()
{
    m_pBoxManGame =(CBoxManGame *)GetChildByName(L"BoxManGame");


	LPCTSTR name[] = {L"exit", L"replay", L"previous", L"next", L"up", L"left", L"right", L"down"};
	for (int i=0; i<sizeof(name)/sizeof(name[0]); i++)
	{
		CWceUiButton* pbutton = (CWceUiButton*)GetChildByName(name[i]);
		if (pbutton)
		{
			pbutton->SetUserData(1);	// APP²»ÏìBEEP
		}
	}

	__super::OnInitDialog();
}

void CBoxManDlg::OnBnClick( PCWceUiButton pButton )
{
    if (pButton->IsEqualName(TEXT("exit")))
    {
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
		ui_play_game_sound(L"gameover.wav");
    }
    if (pButton->IsEqualName(TEXT("replay")))
    {
        m_pBoxManGame->RePlay();

    }
    if (pButton->IsEqualName(TEXT("previous")))
    {
        m_pBoxManGame->PreLevel();
    }
    if (pButton->IsEqualName(TEXT("next")))
    {
        m_pBoxManGame->NextLevel();

    }
    if (pButton->IsEqualName(TEXT("up")))
    {
        m_pBoxManGame->OnBtnUp();

    }
    if (pButton->IsEqualName(TEXT("left")))
    {
        m_pBoxManGame->OnBtnLeft();

    }
    if (pButton->IsEqualName(TEXT("right")))
    {
        m_pBoxManGame->OnBtnRight();

    }
    if (pButton->IsEqualName(TEXT("down")))
    {
        m_pBoxManGame->OnBtnDown();

    }
}

