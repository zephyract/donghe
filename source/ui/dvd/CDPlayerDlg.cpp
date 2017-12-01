#include "StdAfx.h"
#include "CDPlayerDlg.h"
#include "dvdrender.h"
#include "protocol.h"
#include "OverlayDlg.h"
#include "WceUiLoadString.h"
#include "OverlayDlg.h"
#include "RpcMCU.h"

void CCDPlayerDlg::DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData)
{
	CCDPlayerDlg* pthis = (CCDPlayerDlg*)u4CustomData;
	pthis->OnMediaEvent(u4Evt, u4Param1, u4Param2);
}

CCDPlayerDlg::CCDPlayerDlg(void)
{
	CDvdRender::GetInstance()->RegisterEventHandler(CCDPlayerDlg::DvpAgentEventListener, (DWORD)this);

	m_bRandomClicked = FALSE;
	m_bRepeatClicked = FALSE;
}

CCDPlayerDlg::~CCDPlayerDlg(void)
{
	CDvdRender::GetInstance()->UnRegisterEventHandler(CCDPlayerDlg::DvpAgentEventListener);

}

void CCDPlayerDlg::InitLayer()
{
	m_pLayerSpectrum = (CSpectrumLayer*)GetChildByName(L"spectrum");
	m_pLayerSpectrum->SetSrcMode(SRC_MODE_DVD);
	m_pLayerSpectrum->ShowLayer(FALSE);
	SetTimer(TIMER_ID_SPECTRUM, 150);

}

void CCDPlayerDlg::OnInitDialog()
{
	InitLayer();
	ShowFileTip();
	ShowPlayMode();
	__super::OnInitDialog();
}


void CCDPlayerDlg::OnTimer(WORD wIDEvent)
{
	CWceUiLayer *player;
	switch (wIDEvent)
	{
	case TIMER_ID_HIDE_INVALID_TIP:
		KillTimer(TIMER_ID_HIDE_INVALID_TIP);
		player = GetChildByName(L"invalid_tip");
		ASSERT(player);
		player->ShowLayer(FALSE);
		break;

	case TIMER_ID_SPECTRUM:
		m_pLayerSpectrum->InvalidateRect(NULL);
		break;	

	default:
		break;
	}

	__super::OnTimer(wIDEvent);
}


void CCDPlayerDlg::OnPosChange(UINT param1, UINT param2)
{
	UINT curDuration = param1 / 1000;
	UINT outDuration = CDvdRender::GetInstance()->GetDuration();

	// 进度条
	UpdataProgress(curDuration, outDuration);
	// 播放时间
	ShowMediaTimeInfo(curDuration, outDuration);
}

void CCDPlayerDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"progress") && status==SLIDER_MOUSE_UP)
	{
		UINT outDuration = CDvdRender::GetInstance()->GetDuration();
		UINT curDuration = pos * outDuration / 1000;
		CDvdRender::GetInstance()->Seek(curDuration);

		GetWindow()->IgnoreMsgBeforeTime();
	}
}


void CCDPlayerDlg::ShowInvalidIcon()
{
	CWceUiLayer *player = GetChildByName(L"invalid_tip");
	if (player)
	{
		player->ShowLayer(TRUE);
		SetTimer(TIMER_ID_HIDE_INVALID_TIP, 500);
	}
}


void CCDPlayerDlg::OnEvtPBCTime()
{
	GUINT64 outDuration = 0, curDuration = 0;
	DVP_GetDuration(&outDuration);
	DVP_GetCurrentPosition(&curDuration);
	UpdataProgress((UINT)(curDuration/1000), (UINT)(outDuration/1000));			 
	ShowMediaTimeInfo((UINT)(curDuration/1000), (UINT)(outDuration/1000));
}

void CCDPlayerDlg::OnEvtCmdResponse(UINT param1, UINT param2)
{
	if (param2 == ACK_FAIL && param1 != AP_CMD_TP_COORDINATE)	// operation fail, show error tip.
	{
		ShowInvalidIcon();
		m_bRandomClicked = FALSE;
		m_bRepeatClicked = FALSE;
	}

	if (param1 == AP_CMD_PBC_SEEK && param2 == ACK_FAIL)
	{
		GUINT64 outDuration = 0, curDuration = 0;
		DVP_GetDuration(&outDuration);
		DVP_GetCurrentPosition(&curDuration);
		UpdataProgress((UINT)(curDuration/1000), (UINT)(outDuration/1000));			 
	}
}

void CCDPlayerDlg::OnEvtPlayState(UINT state)
{
	switch (state)
	{
	case SV_PLAY:
		m_pLayerPlay->ShowLayer(FALSE);
		m_pLayerPause->ShowLayer(TRUE);
		break;
	case SV_PAUSE:
	case SV_STOP:
		m_pLayerPlay->ShowLayer(TRUE);
		m_pLayerPause->ShowLayer(FALSE);

		if (state == SV_PAUSE)
		{
			ui_led_set_showdata(state == SV_PAUSE ? L" PAUS E  " : L"  STO P  ");
			CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
		}

		break;
	case SV_STOP_RESUME:
	case SV_STILL:
		break;
	case SV_FF:
		break;
	case SV_FR:
		break;
	default:
		break;
	}
}

void CCDPlayerDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	switch (media_event)
	{
	case EVT_DVP_CMD_RESPONSE:
		OnEvtCmdResponse(param1, param2);
		break;
	case EVT_DVP_PBC_CUR_TIME:
		OnEvtPBCTime();
		break;
	case EVT_DVP_PBC_PLAY_STATE:
		OnEvtPlayState(param1);
		break;
	case EVT_DVP_FL_TOTAL_COUNT:
		break;
	case EVT_DVP_PBC_TRACK_NUM:		// cur track/total track
		ShowFileTip();
		break;
	case EVT_DVP_PBC_REPEAT_MODE:
		ShowPlayMode();
		break;
	case EVT_DVP_PBC_USER_MODE:
		ShowPlayMode();
		break;
	case EVT_DVP_RIP_CUR_TRACK_OK:
		break;
	case EVT_CD_RIP_NOT_ENOUGH_SPACE:		
		RETAILMSG(TRUE, (L"[DVP_StartRip] EVT_CD_RIP_NOT_ENOUGH_SPACE ****************** \n") );
		break;	
	case EVT_DVP_RIP_ERROR:
		// show rip error
		RETAILMSG(TRUE, (L"[DVP_StartRip] EVT_DVP_RIP_ERROR ****************** \n") );		
		break;
// 	case WM_MMP_MEDIA_DISATTACHED:
// 		// show rip error, SD card plug out?
// 		break;
	default:
		break;
	}
}


void CCDPlayerDlg::OnBnClick(PCWceUiButton pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CCDFileListDlg_ID, SOURCE_DVD);
	}
	else if (pButton->IsEqualName(L"next"))
	{
		CDvdRender::GetInstance()->Next();
	}
	else if (pButton->IsEqualName(L"prev"))
	{
		CDvdRender::GetInstance()->Previous();
	}
	else if (pButton->IsEqualName(L"play"))
	{
		CDvdRender::GetInstance()->Play();
	}
	else if (pButton->IsEqualName(L"pause"))
	{
		CDvdRender::GetInstance()->Pause();
	}
	else if (pButton->IsEqualName(L"repeat"))
	{
		CDvdRender::GetInstance()->SetPBRepeat();
		m_bRepeatClicked = TRUE;
	}
	else if (pButton->IsEqualName(L"random"))
	{
		CDvdRender::GetInstance()->SetPBRandom();
		m_bRandomClicked = TRUE;
	}
	else if (pButton->IsEqualName(L"audio"))
	{
		CDvdRender::GetInstance()->SwitchAudioChannel();
	}
	else
	{
		__super::OnBnClick(pButton);

	}

}


void CCDPlayerDlg::ShowPlayMode()
{
	// 保存之前的值,只有当前值与之前的值不同时,才认为是有效的event,因为底层会重复发送event,过滤掉不需要的数据
	static UCHAR s_random = 0xFF;
	static UCHAR s_repeat = 0xFF;
	UCHAR random = CDvdRender::GetInstance()->GetPBCUserMode();
	UCHAR repeat = CDvdRender::GetInstance()->GetRepeatMode();
	CWceUiLayer* pRandom = GetChildByName(L"random_tip");
	CWceUiLayer* pRepeat = GetChildByName(L"repeat_tip");
	CWceUiLoadString strRandom;
	CWceUiLoadString strRepeat;
	if (pRandom)
	{
		pRandom->SetStyle((random == SV_RANDOM || random == SV_SHUFFLE) ? L"random_tip_on" : L"random_tip_off");
		strRandom.SetTextResID((random == SV_RANDOM || random == SV_SHUFFLE) ? L"MIS_PLAY_RANDOM_ON" : L"MIS_PLAY_RANDOM_OFF");
	}

	if (pRepeat)
	{
		switch (repeat)
		{
		case SV_REPEAT_ALL:
			pRepeat->SetStyle(L"repeat_tip_all");
			strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_ALL");
			break;
		case SV_REPEAT_TRACK:
			pRepeat->SetStyle(L"repeat_tip_single");
			strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_SINGLE");
			break;
		case SV_REPEAT_NONE:
		default:
			pRepeat->SetStyle(L"repeat_tip_off");
			strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_OFF");
			break;
		}
	}

	if (m_bRandomClicked && random!=s_random)
	{
		COverlayDlg::GetInstance()->MessageTip_ShowUI(strRandom.GetString(), FALSE);
		m_bRandomClicked = FALSE;
	}

	if (m_bRepeatClicked && repeat!=s_repeat)
	{
		COverlayDlg::GetInstance()->MessageTip_ShowUI(strRepeat.GetString(), FALSE);
		m_bRepeatClicked = FALSE;
	}

	s_random = random;
	s_repeat = repeat;

}

void CCDPlayerDlg::ShowFileTip()
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

	//
	CWceUiLayer* pSongText = GetLayerByName(L"song_text");
	if (pSongText)
	{
		_stprintf(buffer, TEXT("Track %02d"), (cur+1)); 
		pSongText->SetText(buffer);
	}
}



void CCDPlayerDlg::OnKeyPlayControl(UINT key, UINT param)
{
	BYTE byte0 = (BYTE)((param>>24) & 0xFF);

	switch (key)
	{
	case T_PREV:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PREV");
		CDvdRender::GetInstance()->Previous();
		break;
	case T_NEXT:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_NEXT");
		CDvdRender::GetInstance()->Next();
		break;
	case T_PLAY:	// param byte0 : 0 ~ 播放暂停复合键, 1 ~ 播放， 2 ~ 暂停
		if (byte0 == 1)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
			CDvdRender::GetInstance()->Play();
		}
		else if (byte0 == 2)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
			CDvdRender::GetInstance()->Pause();
		}
		else
		{
			if( SV_PLAY == CDvdRender::GetInstance()->GetState())
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
				CDvdRender::GetInstance()->Pause();
			}
			else
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
				CDvdRender::GetInstance()->Play();
			}
		}
		break;
	case T_STOP:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_STOP");
		CDvdRender::GetInstance()->Stop();
		break;
	case T_IR_GOTO:
		if ((param-1) != CDvdRender::GetInstance()->GetCDCurTrack())
		{
			DVP_ItemClick((GINT16)param-1);
		}
		break;
	case T_REPEAT:
		CDvdRender::GetInstance()->SetPBRepeat();
		m_bRepeatClicked = TRUE;
		break;
	case T_SHUFFLE:
		CDvdRender::GetInstance()->SetPBRandom();
		m_bRandomClicked = TRUE;
		break;
	default:
		break;
	}
}