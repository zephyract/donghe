#include "StdAfx.h"
#include "MyGoBang.h"
#include "GoBangDlg.h"
#include "WceUiRealWnd.h"
#include <atlbase.h>

CGoBangDlg::CGoBangDlg(void)
{
    m_pcbtn     =NULL;                
    m_ppbtn     =NULL;                
    m_pTxt_Msg   =NULL;
    m_pTxt_Level =NULL;
}

CGoBangDlg::~CGoBangDlg(void)
{
}

void CGoBangDlg::OnInitDialog()
{
   m_pTxt_Msg      = (CWceUiLayer *)GetChildByName(TEXT("showtip"));
   m_pTxt_Level    = (CWceUiLayer *)GetChildByName(TEXT("level"));
   m_pcbtn         = (CWceUiButton *)GetChildByName(TEXT("btnpc"));
   m_ppbtn         = (CWceUiButton *)GetChildByName(TEXT("btnpeople"));
   m_pGoBang = (CMyGoBang *)GetChildByName(TEXT("gamearea"));
   InitGame();

   LPCTSTR name[] = {L"exit", L"btnpre", L"btnnext", L"btnundo", L"btnpeople", L"btnpc", L"replay"};
   for (int i=0; i<sizeof(name)/sizeof(name[0]); i++)
   {
	   CWceUiButton* pbutton = (CWceUiButton*)GetChildByName(name[i]);
	   if (pbutton)
	   {
		   pbutton->SetUserData(1);	// APP²»ÏìBEEP
	   }
   }
}

void CGoBangDlg::OnBnClick( PCWceUiButton pButton )
{
    if (pButton->IsEqualName(TEXT("exit")))
    {
		CDlgManager::GetInstance()->ShowDlg(CMainDlg_ID, SOURCE_MAIN);
		ui_play_game_sound(L"gameover.wav");
    }
    if (m_pGoBang->IsThink())
    {
        return;
    }
    if (pButton->IsEqualName(TEXT("btnpre")))
    {
        m_pGoBang->OnBtnPreLevel();
    }
    if (pButton->IsEqualName(TEXT("btnnext")))
    {
        m_pGoBang->OnBtnNextLevel();
    }
    if (pButton->IsEqualName(TEXT("btnundo")))
    {
        m_pGoBang->OnBnClickedBtnUndo();
    }
    if (pButton->IsEqualName(TEXT("btnpeople")))
    {
        m_pGoBang->OnBnClickedBtnMantoman();
    }
    if (pButton->IsEqualName(TEXT("btnpc")))
    {
        m_pGoBang->OnBnClickedBtnMantoman();
    }
    if (pButton->IsEqualName(TEXT("replay")))
    {
        m_pGoBang->OnStart();
    }
}

void CGoBangDlg::InitGame()
{
    m_pGoBang->InitTipStatic(m_pTxt_Msg);
    m_pGoBang->InitLevelStatic(m_pTxt_Level);
    m_pGoBang->InitPCBtn(m_pcbtn);
    m_pGoBang->InitPPBtn(m_ppbtn);
    m_pGoBang->InitGame();
}