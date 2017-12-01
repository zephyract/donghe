#include "StdAfx.h"
#include "WceUiButton.h"
#include "LinkGame.h"
#include "LinkDlg.h"
#include "WceUiRealWnd.h"
#include <atlbase.h>

#define     LINKGAME_CTRL       TEXT("linkgame")

#define  TIME_NAME TEXT("time")
#define  SORTBTN_NAME TEXT("resort")

CLinkDlg::CLinkDlg(void)
{
}
CLinkDlg::~CLinkDlg(void)
{
}

void CLinkDlg::OnInitDialog()
{

    m_ptimeStatic = GetChildByName(TIME_NAME);
    m_pSortBtn    = (CWceUiButton *)GetChildByName(SORTBTN_NAME);

    m_pLinkGame = (CLinkGame *)GetChildByName(LINKGAME_CTRL);
    m_pLinkGame->InitTimeStatic(m_ptimeStatic);
    m_pLinkGame->InitSortBtn(m_pSortBtn);
    m_pLinkGame->Replay(FALSE);

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


void CLinkDlg::OnBnClick( PCWceUiButton pButton )
{
	if (pButton->IsEqualName(L"exit"))
	{
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
		m_pLinkGame->Exit();
	}
    else if (pButton->IsEqualName(TEXT("resort")))
    {
        m_pLinkGame->Resort();
    }
	else if (pButton->IsEqualName(TEXT("replay")))
    {
        m_pLinkGame->Replay();
    }
}