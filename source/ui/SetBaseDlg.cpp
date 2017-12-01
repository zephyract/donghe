#include "stdafx.h"
#include "SettingDlg.h"
#include "WceUiListBox.h"
#include "WceUiSButton.h"
#include "WceUiSlider.h"
#include "WceUiSEditBox.h"
#include "WceUiMEditBox.h"
#include "WceUiBase.h"
#include "wceUiPageLayer.h"
#include "SetLanguageDlg.h"
#include "SetBaseDlg.h"
#include "interface.h"

CSetBaseDlg::CSetBaseDlg(void)
{
// 	for (int i=0; i<TOP_LAYER_COUNT; i++)
// 	{
// 		m_pLayerTop[i] = NULL;
// 	}
// 	::memset(m_rcLayerTop, 0, sizeof(m_rcLayerTop));
}

CSetBaseDlg::~CSetBaseDlg(void)
{

}

void CSetBaseDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(UI_MSG_SYSTEM_RESET);

// 	LPCTSTR layer_name[TOP_LAYER_COUNT] = 
// 	{
// 		L"bk",
// 		L"top_bar",
// 		L"home",
// 		L"back",
// 		L"title"
// 	};
// 	for (int i=0; i<TOP_LAYER_COUNT; i++)
// 	{
// 		m_pLayerTop[i] = GetChildByName(layer_name[i]);
// 		if (m_pLayerTop[i])
// 		{
// 			m_pLayerTop[i]->GetWindowRect(&m_rcLayerTop[i]);
// 		}
// 	}
}

void CSetBaseDlg::OnReset()
{

}

LRESULT CSetBaseDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_SYSTEM_RESET)
	{
		OnReset();
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CSetBaseDlg::OnDrawEnd()
{
	__super::OnDrawEnd();

	// ResetTopLayers() 的调用位置，这里设计有些古怪，因为每个对话框的动画是独立的，靠OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)时
	// 来同步帧数，(计时器没办法保证帧同步，又没有设计其它的机制)，所以在第一次动画前调用，或每次
	// 动画结束后为下一次动画提前做计算
	// 不能在动画过程中做计算(UI库已经开始ondraw了)，因为有控件的移动，会影响UI库的计算
// 	if (m_nCurrentFrame<FRAME_ANMI_SHOWWINDOW && m_nAnimMode==ANIM_MODE_UPLEVEL )
// 	{
// 		ResetTopLayers();
// 	}
}

void CSetBaseDlg::OnAnimateBegin()
{
// 	if (m_nAnimMode == ANIM_MODE_UPLEVEL)
// 	{
// 		ResetTopLayers();
// 	}
	__super::OnAnimateBegin();
}

void CSetBaseDlg::DoAnimate()
{
// 	if (m_nAnimMode == ANIM_MODE_UPLEVEL)
// 	{
// 		float param[4];
// 		AnimGetParameters(param, m_nCurrentFrame, FRAME_ANMI_SHOWWINDOW);
// 		wceui::wceglLoadIdentity();
// 		wceui::wceglTranslate(param[2], param[3], 0);
// 		m_nCurrentFrame++;
// 	}
//	else
	{
		__super::DoAnimate();
	}
}



void CSetBaseDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSettingDlg_ID, SOURCE_SETTING);
		return;
	}

	__super::OnBnClick(pButton);
}

void CSetBaseDlg::DeleteDlg()
{
	DLG_INFO* pdlg_info = CDlgManager::GetInstance()->GetDlgInfo(this);
	if (pdlg_info)
	{
		CDlgManager::GetInstance()->SetToDeleteFlag(pdlg_info->dlg_id, pdlg_info->source_id, TRUE);
	}
}

void CSetBaseDlg::OnDlgShow(BOOL bShow)
{
	if (!bShow)
	{
//		DeleteDlg();
	}

	__super::OnDlgShow(bShow);
}