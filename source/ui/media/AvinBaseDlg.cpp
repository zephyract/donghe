#include "stdafx.h"
#include "VideoPlayerDlg.h"
#include "wceuipagelayer.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "MainWnd.h"
#include "msdkrender.h"
#include "uiddraw.h"
#include "AvinBaseDlg.h"

CAvinBaseDlg::CAvinBaseDlg(void)
{
}

CAvinBaseDlg::~CAvinBaseDlg(void)
{

}

void CAvinBaseDlg::OnInitDialog()
{
	__super::OnInitDialog();

}

void CAvinBaseDlg::OnTimer(WORD wIDEvent)
{
	__super::OnTimer(wIDEvent);
}

void CAvinBaseDlg::OnDlgShow(BOOL bShow)
{
	CUiDDraw::GetInstance()->UpdateOverlay(bShow, 180);
}

void CAvinBaseDlg::OnBnClick(CWceUiButton* pButton)
{
	__super::OnBnClick(pButton);
}

