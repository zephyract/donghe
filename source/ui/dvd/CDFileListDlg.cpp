#include "stdafx.h"
#include "CDFileListDlg.h"
#include "interface.h"
#include "RpcMCU.h"

void CCDFileListDlg::DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData)
{
	CCDFileListDlg* pthis = (CCDFileListDlg*)u4CustomData;
	pthis->OnMediaEvent(u4Evt, u4Param1, u4Param2);
}

CCDFileListDlg::CCDFileListDlg(void)
{
	CDvdRender::GetInstance()->RegisterEventHandler(CCDFileListDlg::DvpAgentEventListener, (DWORD)this);

	if (WceUiGetBitsPixel() == 16)
	{
		m_texMusic.SetBitmapFromRes(L"media\\filelist\\music.565a.conf");
		m_texPlaying.SetBitmapFromRes(L"media\\filelist\\play.565a.conf");
	}
	else
	{
		m_texMusic.SetBitmapFromRes(L"media\\filelist\\music.32a.conf");
		m_texPlaying.SetBitmapFromRes(L"media\\filelist\\play.32a.conf");
	}
}

CCDFileListDlg::~CCDFileListDlg(void)
{
	CDvdRender::GetInstance()->UnRegisterEventHandler(CCDFileListDlg::DvpAgentEventListener);
}

void CCDFileListDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_LBUTTON_DOWN);
	RegisterMsg(UI_MSG_LBUTTON_UP);

	m_pListFile   = (CFileListBox*)GetChildByName(L"file_listbox");
	if(m_pListFile)
	{
		m_pListFile->SetPlayImage(95, m_texPlaying.GetBitmap());
	}

	// 立即播放，好像播放不出来
	SetTimer(TIMER_ID_START_PLAY_CD, 100);

	__super::OnInitDialog();
}

void CCDFileListDlg::InitListbox()
{
	if (m_pListFile)
	{
		m_pListFile->ResetContent();
		const CArray<int>& track = CDvdRender::GetInstance()->GetCDTrackInfo();
		CString str;
		for (int i=0; i<track.GetCount(); i++)
		{
			if (track.GetAt(i) > 0)
			{
				str.Format(L"Track%d                                                 %02d:%02d", i+1, track.GetAt(i)/60, track.GetAt(i)%60);
			}
			else
			{
				str.Format(L"Track%02d ", i+1);
			}
			m_pListFile->AddString(str);
			m_pListFile->SetItemImage(i, m_texMusic.GetBitmap());
		}

		m_pListFile->SetCurSel(CDvdRender::GetInstance()->GetCDCurTrack());
	}
}

void CCDFileListDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_RETURN_PLAY_UI)
	{
		KillTimer(TIMER_ID_RETURN_PLAY_UI);
		ShowPlayingDlg();
	}
	else if (wIDEvent == TIMER_ID_START_PLAY_CD)
	{
		if(CDvdRender::GetInstance()->GetState() != SV_PLAY)
		{
			CDvdRender::GetInstance()->Play();
		}
		else
		{
			KillTimer(TIMER_ID_START_PLAY_CD);
		}
	}
	__super::OnTimer(wIDEvent);
}


void CCDFileListDlg::ShowPlayingDlg()
{
	if (IsLayerVisible())
	{
		CDlgManager::GetInstance()->ShowDlg(CCDPlayerDlg_ID, SOURCE_DVD);
	}
	else	// 如果列表窗口已经在后台了,把源的当前窗口切换为播放窗口,但不显示出来
	{
		CDlgManager::GetInstance()->SetActiveDlg(CCDPlayerDlg_ID, SOURCE_DVD);
	}
}

void CCDFileListDlg::OnDlgShow(BOOL bShow)
{
	// 返回后再进来，定位到正在播放的文件
	if (bShow)
	{
		SetTimer(TIMER_ID_RETURN_PLAY_UI, TIME_RETURN_PLAY_UI);
	}
	else
	{
		KillTimer(TIMER_ID_RETURN_PLAY_UI);
	}

	__super::OnDlgShow(bShow);
}


void CCDFileListDlg::ShowFileTip()
{
	int cur = CDvdRender::GetInstance()->GetCDCurTrack();
	int total = CDvdRender::GetInstance()->GetCDTotalTrack();
	if (total<0)
	{
		total = 0;	// 如果返回数据不合法,显示0
	}

	TCHAR buffer[10] = L"";
	_stprintf(buffer, TEXT("%d/%d"), (cur+1), total); 
	CWceUiLayer *ptip = GetLayerByName(L"file_tip");
	if (ptip)
	{
		ptip->SetText(buffer);
	}
}


void CCDFileListDlg::OnBnClick(CWceUiButton* pButton)
{
	__super::OnBnClick(pButton);

}

void CCDFileListDlg::OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked)
{
	if (plist->IsEqualName(L"file_listbox"))
	{
		if (bClickChecked)
		{
			if (nItemClicked != CDvdRender::GetInstance()->GetCDCurTrack())
			{
				DVP_ItemClick((GINT16)nItemClicked);
			}
			ShowPlayingDlg();
		}
	}
}

LRESULT CCDFileListDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
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

void CCDFileListDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	switch (media_event)
	{
	case EVT_DVP_FL_TOTAL_COUNT:
		CDvdRender::GetInstance()->InitCDTrackInfo(param1);
		break;
	case EVT_DVP_RIP_TRK_LBA_LEN:	
		CDvdRender::GetInstance()->SetCDTrackInfo(param1-1, param2/75);	// DVP 从1开始计数，单位为1/75ms
		if (param1 == CDvdRender::GetInstance()->GetCDTotalTrack())	// 更新最后一条数据后初始化列表 
		{
			InitListbox();
		}

		break;
	case EVT_DVP_PBC_TRACK_NUM:		// cur track/total track
		CDvdRender::GetInstance()->SetCDCurTrack(param1);
		ShowFileTip();
		if (m_pListFile)
		{
			m_pListFile->SetCurSel(param1);
			m_pListFile->SetPlayIndex(param1);
		}
		break;
	case EVT_DVP_PBC_CUR_TIME:
		{
			// refresh LED
			GUINT64 curDuration = 0;
			DVP_GetCurrentPosition(&curDuration);
			GUINT16 cur = 0;
			DVP_GetPlayingIdx(&cur);
			ui_led_set_showdata(ui_led_media_data(cur+1, curDuration/1000));
			CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
		}
		break;
	case EVT_DVP_PBC_USER_MODE:
		{
			// refresh LED
			UCHAR random = CDvdRender::GetInstance()->GetPBCUserMode();
			UCHAR repeat = CDvdRender::GetInstance()->GetRepeatMode();
			ui_led_set_playmode((random == SV_RANDOM || random == SV_SHUFFLE), (repeat != SV_REPEAT_NONE));
		}
		break;
	default:
		break;
	}
}
