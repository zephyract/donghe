#include "stdafx.h"
#include "UIpodFileListDlg.h"
#include "WceUiListBox.h"
#include "tzUtil.h"
#include "sysUtil.h"
#include "interface.h"

CUIPodFileListDlg::CUIPodFileListDlg(void)
{
	m_curType = DB_TYPE_TOP_LEVEL;		// 当作非法值用,这里用不到这个枚举
	m_pListbox   = NULL;
}

CUIPodFileListDlg::~CUIPodFileListDlg(void)
{

}

void CUIPodFileListDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(UI_MSG_LBUTTON_DOWN);
	RegisterMsg(UI_MSG_LBUTTON_UP);

	m_pListbox   = (CWceUiListBox*)GetChildByName(L"listbox");
	ASSERT(m_pListbox);

	OnTypeChange(DB_TYPE_PLAYLIST);
	CheckButton(m_curType);

	CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PLAY);

}

void CUIPodFileListDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_RETURN_PLAY_UI)
	{
		KillTimer(TIMER_ID_RETURN_PLAY_UI);
		ShowPlayingDlg();
	}

	__super::OnTimer(wIDEvent);
}

void CUIPodFileListDlg::ShowPlayingDlg()
{
	if (IsLayerVisible())
	{
		CDlgManager::GetInstance()->ShowDlg(CUIPodPlayerDlg_ID, SOURCE_IPOD_USB);
	}
	else	// 如果列表窗口已经在后台了,把源的当前窗口切换为播放窗口,但不显示出来
	{
		CDlgManager::GetInstance()->SetActiveDlg(CUIPodPlayerDlg_ID, SOURCE_IPOD_USB);
		// 如果last dlg刚好是该窗口,需主动更新last dlg信息，从其它窗口返回时,应返回这里设置的活动窗口
		if (CDlgManager::GetInstance()->GetLastDlgID() == CUIPodFileListDlg_ID)
		{
			DLG_INFO* pdi = CDlgManager::GetInstance()->GetDlgInfo(CUIPodPlayerDlg_ID, SOURCE_IPOD_USB);
			CDlgManager::GetInstance()->SetLastDlgInfo(pdi);
		}
	}
}

LRESULT CUIPodFileListDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 没有mouse操作时,才计时返回播放界面
	if (IsLayerVisible())
	{
		if (message == UI_MSG_LBUTTON_DOWN)
		{
			KillTimer(TIMER_ID_RETURN_PLAY_UI);
		}
		else if (message == UI_MSG_LBUTTON_UP)
		{
			SetTimer(TIMER_ID_RETURN_PLAY_UI, TIME_RETURN_PLAY_UI);
		}
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CUIPodFileListDlg::UpdateListbox(MUSB_IPOD_DB_CATEGORY_TYPE_E type, UINT sub_type)
{
	m_pListbox->ResetContent();

	CiPodSDKAgent* piPodSDKAgent = CIPODManager::GetInstance()->GetIPodCtrl();
	MUSB_IPOD_DB_CATEGORY_TYPE_E curType = CIPODManager::GetInstance()->GetDBType(type, sub_type);
	UINT count = piPodSDKAgent->m_pfnGetNumberCategorizedDBRecords(curType);

	MUSB_IPOD_DB_RecItem_T rRecItem;
	TCHAR buffer[IPOD_INFO_MAX_STR_LEN];
	DWORD dwtick = GetTickCount();
	if (count > 0)
	{
		for (UINT i=0; i<count; i++)
		{
			piPodSDKAgent->m_pfnRetrieveCategorizedDBRecords(curType, i, 1, &rRecItem);
			MultiByteToWideChar(CP_UTF8, 0, rRecItem.szRecInfo, -1, buffer, IPOD_INFO_MAX_STR_LEN);
			m_pListbox->AddString(buffer);
		}
		m_pListbox->SetCurSel(0);
	}
	RETAILMSG(1, (L"get ipod record, tick = %d++++++++++++\n", GetTickCount() - dwtick));

}

void CUIPodFileListDlg::OnTypeChange(MUSB_IPOD_DB_CATEGORY_TYPE_E type)
{
	if (m_curType != type)
	{
		m_curType = type;
		m_curSubType = 0x01;	// 切换时,回到顶级目录了,后面这个逻辑应该可以优化,保留之前的数据

		CiPodSDKAgent* piPodSDKAgent = CIPODManager::GetInstance()->GetIPodCtrl();
		piPodSDKAgent->m_pfnResetDBSelection();

		UpdateListbox(m_curType, m_curSubType);
	}
}

void CUIPodFileListDlg::CheckButton(MUSB_IPOD_DB_CATEGORY_TYPE_E type)
{
	CWceUiButton* pButtonplaylist = (CWceUiButton*)GetChildByName(L"playlist");
	CWceUiButton* pButtonsong = (CWceUiButton*)GetChildByName(L"song");
	CWceUiButton* pButtonalbum = (CWceUiButton*)GetChildByName(L"album");
	CWceUiButton* pButtonartist = (CWceUiButton*)GetChildByName(L"artist");
	CWceUiButton* pButtongenre = (CWceUiButton*)GetChildByName(L"genre");
	ASSERT(pButtonsong && pButtonartist && pButtonalbum && pButtongenre);

	pButtonplaylist->SetStatusStyleName(L"playlist_normal", CWceUiButton::STATUS_NORMAL);
	pButtonsong->SetStatusStyleName(L"song_normal", CWceUiButton::STATUS_NORMAL);
	pButtonalbum->SetStatusStyleName(L"album_normal", CWceUiButton::STATUS_NORMAL);
	pButtonartist->SetStatusStyleName(L"artist_normal", CWceUiButton::STATUS_NORMAL);
	pButtongenre->SetStatusStyleName(L"genre_normal", CWceUiButton::STATUS_NORMAL);
	switch (type)
	{
	case DB_TYPE_PLAYLIST:
		pButtonplaylist->SetStatusStyleName(L"playlist_down", CWceUiButton::STATUS_NORMAL);
		break;
	case DB_TYPE_TRACK:
		pButtonsong->SetStatusStyleName(L"song_down", CWceUiButton::STATUS_NORMAL);
		break;
	case DB_TYPE_ALBUM:
		pButtonalbum->SetStatusStyleName(L"album_down", CWceUiButton::STATUS_NORMAL);
		break;
	case DB_TYPE_ARTIST:
		pButtonartist->SetStatusStyleName(L"artist_down", CWceUiButton::STATUS_NORMAL);
		break;
	case DB_TYPE_GENRE:
		pButtongenre->SetStatusStyleName(L"genre_down", CWceUiButton::STATUS_NORMAL);
		break;
	default:
		break;
	}
}

void CUIPodFileListDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		SetTimer(TIMER_ID_RETURN_PLAY_UI, TIME_RETURN_PLAY_UI);
	}
	else
	{
		KillTimer(TIMER_ID_RETURN_PLAY_UI);
		CDlgManager::GetInstance()->SetActiveDlg(CUIPodPlayerDlg_ID, SOURCE_IPOD_USB);		// 隐藏后再返回时,返回到播放界面
	}
}

void CUIPodFileListDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		if (m_curSubType > 1)	// 不是在顶级目录
		{
			m_curSubType--;

			BYTE type = (BYTE)CIPODManager::GetInstance()->GetDBType(m_curType, m_curSubType);

			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnSelectDBRecord(type, 0);

			UpdateListbox(m_curType, m_curSubType);
		}
		else
		{
			__super::OnBnClick(pButton);
		}
	}
	else if (pButton->IsEqualName(L"playlist"))
	{
		OnTypeChange(DB_TYPE_PLAYLIST);
		CheckButton(m_curType);
	}
	else if (pButton->IsEqualName(L"song"))
	{
		OnTypeChange(DB_TYPE_TRACK);
		CheckButton(m_curType);
	}
	else if (pButton->IsEqualName(L"album"))
	{
		OnTypeChange(DB_TYPE_ALBUM);
		CheckButton(m_curType);
	}
	else if (pButton->IsEqualName(L"artist"))
	{
		OnTypeChange(DB_TYPE_ARTIST);
		CheckButton(m_curType);
	}
	else if (pButton->IsEqualName(L"genre"))
	{
		OnTypeChange(DB_TYPE_GENRE);
		CheckButton(m_curType);
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}

void CUIPodFileListDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	if (plist == m_pListbox && bClickChecked)
	{
		if (CIPODManager::GetInstance()->GetDBType(m_curType, m_curSubType) == DB_TYPE_TRACK)	// 已经是最低层目录了,表示选中的歌曲
		{
			CIPODManager::GetInstance()->GetIPodCtrl()->PlayCurrentSelection(nItemClicked);

			CDlgManager::GetInstance()->ShowDlg(CUIPodPlayerDlg_ID, SOURCE_IPOD_USB);
		}
		else	// 下面还有目录
		{
			BYTE type = (BYTE)CIPODManager::GetInstance()->GetDBType(m_curType, m_curSubType);
			
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnSelectDBRecord(type, nItemClicked);

			m_curSubType++;
			UpdateListbox(m_curType, m_curSubType);
		}
			

	}
}
