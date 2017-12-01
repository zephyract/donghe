#include "StdAfx.h"
#include "RussiaDlg.h"

CRussianDlg::CRussianDlg(void)
{
	m_pScore = NULL;
}

CRussianDlg::~CRussianDlg(void)
{
}

void CRussianDlg::OnInitDialog()
{
	m_pRussiaGame = (CRussiaGame *)GetChildByName(L"GameArea");
	m_pScore = (CScore *)GetChildByName(L"score");
	m_pRussiaGame->InitContorl(m_pScore);
	m_pRussiaGame->InitGlobal();

	UpdateSPBtn();

	LPCTSTR name[] = {L"exit", L"start", L"pause", L"replay", L"up", L"left", L"right", L"down"};
	for (int i=0; i<sizeof(name)/sizeof(name[0]); i++)
	{
		CWceUiButton* pbutton = (CWceUiButton*)GetChildByName(name[i]);
		if (pbutton)
		{
			pbutton->SetUserData(1);	// APP²»ÏìBEEP
		}
	}
}

void CRussianDlg::OnBnClick(PCWceUiButton pButton)
{
	if (pButton->IsEqualName(L"exit"))
	{
		m_pRussiaGame->Exit();
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
	}
	if (pButton->IsEqualName(L"start"))
	{
		m_pRussiaGame->Start();
		UpdateSPBtn();
	}
	if (pButton->IsEqualName(L"pause"))
	{
		m_pRussiaGame->Pause();
		UpdateSPBtn();
	}
	if (pButton->IsEqualName(L"replay"))
	{
		m_pRussiaGame->Replay();
		UpdateSPBtn();
	}
	if (pButton->IsEqualName(L"up"))
	{
		m_pRussiaGame->Rotate();
	}
	if (pButton->IsEqualName(L"left"))
	{
		m_pRussiaGame->Left();
	}
	if (pButton->IsEqualName(L"right"))
	{
		m_pRussiaGame->Right();
	}
	if (pButton->IsEqualName(L"down"))
	{
		m_pRussiaGame->Down();        
	}
}

void CRussianDlg::UpdateSPBtn()
{
	GetChildByName(L"start")->ShowLayer(GAME_RUN != m_pRussiaGame->GetGameState());
	GetChildByName(L"pause")->ShowLayer(GAME_RUN == m_pRussiaGame->GetGameState());
}

