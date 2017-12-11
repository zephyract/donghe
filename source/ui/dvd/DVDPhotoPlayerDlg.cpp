#include "StdAfx.h"
#include "DVDPhotoPlayerDlg.h"
#include "WceUiButton.h"
#include "WceUiRealWnd.h"
#include <atlbase.h>
#include "dvdrender.h"
#include "uiddraw.h"
#include "WceUiPageLayer.h"
#include "MainWnd.h"
#include "interface.h"
#include "OverlayDlg.h"
#include "WceUiLoadString.h"

void CDVDPhotoPlayerDlg::DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData)
{
	CDVDPhotoPlayerDlg* pthis = (CDVDPhotoPlayerDlg*)u4CustomData;
	pthis->OnMediaEvent(u4Evt, u4Param1, u4Param2);
}

CDVDPhotoPlayerDlg::CDVDPhotoPlayerDlg(void)
{
	CDvdRender::GetInstance()->RegisterEventHandler(CDVDPhotoPlayerDlg::DvpAgentEventListener, (DWORD)this);

	m_bRandomClicked = FALSE;
	m_bRepeatClicked = FALSE;
}

CDVDPhotoPlayerDlg::~CDVDPhotoPlayerDlg(void)
{
	CDvdRender::GetInstance()->UnRegisterEventHandler(CDVDPhotoPlayerDlg::DvpAgentEventListener);
}

void CDVDPhotoPlayerDlg::InitLayer()
{
	CWceUiPageLayer* pLayer = (CWceUiPageLayer*)GetChildByName(L"bottom_frame");
	ASSERT(pLayer);
	if (pLayer)
	{
		pLayer->SetExtraScroll(30);
		pLayer->SetScrollRange(0, 800);
	}
}

void CDVDPhotoPlayerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(UI_MSG_MCU_KEY_COMMAND);

	InitLayer();
	ShowFileTip();
}

void CDVDPhotoPlayerDlg::ShowVideoForbiddenTip(BOOL bShow)
{
	CWceUiLayer *pLayer = GetChildByName(L"video_forbidden");
	assert(pLayer);
	pLayer->ShowLayer(bShow);

	pLayer = GetChildByName(L"video_forbidden_tip");
	assert(pLayer);
	pLayer->ShowLayer(bShow);
}

void CDVDPhotoPlayerDlg::OnTimer(WORD wIDEvent)
{
	switch (wIDEvent)
	{
	case TIMER_ID_DELAY_SHOW_VIDEO:
		KillTimer(TIMER_ID_DELAY_SHOW_VIDEO);
		// 计时器是个异步操作,期间状态有可能改变,保证窗口可见时才显示视频
		ShowVideo(TRUE);
		break;
	case TIMER_ID_HIDE_INVALID_TIP:
		{
			KillTimer(TIMER_ID_HIDE_INVALID_TIP);
			CWceUiLayer *player = GetChildByName(L"invalid_tip");
			ASSERT(player);
			player->ShowLayer(FALSE);
		}
		break;
	default:
		break;
	}

	__super::OnTimer(wIDEvent);
}

void CDVDPhotoPlayerDlg::OnAutoFullscreen()
{
	// 只有在播放状态，并且鼠标未被按下的情况下才自动全屏播放
	if (!m_bFullScreen && GetAsyncKeyState(VK_LBUTTON) >= 0)
	{
		UCHAR state;
		DVP_GetPBState(&state);
		if (state == SV_PLAY)
		{
			SetFullScreen(TRUE);
		}
	}
}

void CDVDPhotoPlayerDlg::OnBnClick(PCWceUiButton pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CDVDFileListDlg_ID, SOURCE_DVD);
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
	else if (pButton->IsEqualName(L"zoomin"))
	{
		CDvdRender::GetInstance()->ZoomIn();
	}
	else if (pButton->IsEqualName(L"zoomout"))
	{
		CDvdRender::GetInstance()->ZoomOut();
	}
	else if (pButton->IsEqualName(L"rotate"))
	{
		CDvdRender::GetInstance()->Rotate();
	}
	else
	{
		__super::OnBnClick(pButton);

	}

}


LRESULT CDVDPhotoPlayerDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_MCU_KEY_COMMAND)
	{
		int status = (wParam>>16) & 0xFF;
		int id = wParam & 0xFF;

		switch (id)
		{
		case T_ZOOM:
			{
				static BOOL bZoomIn = TRUE;
				if (m_nZoom == 200)
				{
					bZoomIn = FALSE;
				}
				else if (m_nZoom == 50)
				{
					bZoomIn = TRUE;
				}

				if (bZoomIn)
				{
					CDvdRender::GetInstance()->ZoomIn();
				}
				else
				{
					CDvdRender::GetInstance()->ZoomOut();
				}
			}
			break;
		default:
			break;
		}
	}

	return __super::OnMessage(message, wParam, lParam);
}


void CDVDPhotoPlayerDlg::OnLButtonDown(UINT nFlags, POINT point)
{
	SetFullScreen(!m_bFullScreen);
}


void CDVDPhotoPlayerDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{	
	switch (media_event)
	{
	case EVT_DVP_CMD_RESPONSE:
		OnEvtCmdResponse(param1, param2);
		break;
	case EVT_DVP_PBC_TRACK_NUM:		// cur track/total track
	case EVT_DVP_PBC_PLAYING_AUDIO_IDX:
		ShowFileTip();
		break;

	case EVT_DVP_PBC_PLAY_STATE:
		OnEvtPlayState(param1);
		break;
	case EVT_DVP_PBC_REPEAT_MODE:
	case EVT_DVP_PBC_USER_MODE:
		ShowPlayMode();
		break;
	case EVT_DVP_PBC_ZOOM:
		if (RET_OK == DVP_GetCurrentZoomMultiple(&m_nZoom))
		{
			ShowZoomInfo(m_nZoom);
		}
		break;
	default:
		break;
	}
}

void CDVDPhotoPlayerDlg::ShowZoomInfo(UCHAR zoom)
{
	switch (zoom)
	{
	case 125:
		ShowTipMessage(L"ZOOM 2");
		break;
	case 150:
		ShowTipMessage(L"ZOOM 3");
		break;
	case 200:
		ShowTipMessage(L"ZOOM 4");
		break;
	case 75:
		ShowTipMessage(L"ZOOM 1/2");
		break;
	case 50:
		ShowTipMessage(L"ZOOM 1/4");
		break;
	case 100:
	case 0:
		ShowTipMessage(L"ZOOM OFF");
		break;
	default:
		break;
	}
}

void CDVDPhotoPlayerDlg::ShowTipMessage(LPCTSTR message)
{
	// 显示在overlay上 
	COverlayDlg::GetInstance()->MessageTip_ShowUI(message, FALSE);
}



void CDVDPhotoPlayerDlg::ShowPlayMode()
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
		case SV_REPEAT_FOLDER:
			pRepeat->SetStyle(L"repeat_tip_folder");
			strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_FOLDER");
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


void CDVDPhotoPlayerDlg::OnEvtCmdResponse(UINT param1, UINT param2)
{
	if (param2 == ACK_FAIL && param1 != AP_CMD_TP_COORDINATE)	// operation fail, show error tip.
	{
		ShowInvalidIcon();

		m_bRandomClicked = FALSE;
		m_bRepeatClicked = FALSE;
	}
}

void CDVDPhotoPlayerDlg::OnEvtPlayState(UINT state)
{
	switch (state)
	{
	case SV_PLAY:
		m_pLayerPlay->ShowLayer(FALSE);
		m_pLayerPause->ShowLayer(TRUE);
		break;
	case SV_PAUSE:
		m_pLayerPlay->ShowLayer(TRUE);
		m_pLayerPause->ShowLayer(FALSE);
		break;
	case SV_STOP:
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

void CDVDPhotoPlayerDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"progress"))
	{
		UINT outDuration = CDvdRender::GetInstance()->GetDuration();
		UINT curDuration = pos * outDuration / 1000;
		CDvdRender::GetInstance()->Seek(curDuration);
	}


}

void CDVDPhotoPlayerDlg::ShowInvalidIcon()
{
	CWceUiLayer *player = GetChildByName(L"invalid_tip");
	if (player)
	{
		player->ShowLayer(TRUE);
		SetTimer(TIMER_ID_HIDE_INVALID_TIP, 500);
	}
}


void CDVDPhotoPlayerDlg::OnDlgShow(BOOL bShow)
{
	if (!IsAnimating())	// 如果没有动画效果时,直接show or hide
	{
		CUiDDraw::GetInstance()->UpdateOverlay(bShow, 180);
		CDvdRender::GetInstance()->ShowVideo(bShow);
	}
	// 有动画时，动画过程会管理显示或隐藏
}

void CDVDPhotoPlayerDlg::OnAnimateBegin()
{
	// 动画开始了,有可能之前的视频显示动作还没执行，取消掉
	KillTimer(TIMER_ID_DELAY_SHOW_VIDEO);
	KillTimer(TIMER_ID_DELAY_SHOW_OVERLAY);

	CUiDDraw::GetInstance()->UpdateOverlay(FALSE, 180);
	ShowVideo(FALSE, NULL);

	__super::OnAnimateBegin();
}


void CDVDPhotoPlayerDlg::OnAnimateEnd()
{
	if (m_bAnimShow)	// hide窗口时
	{
		SetTimer(TIMER_ID_DELAY_SHOW_OVERLAY, 10);
	}

	__super::OnAnimateEnd();
}

void CDVDPhotoPlayerDlg::OnAnimating()
{
// 	float param[4];
// 	AnimGetParameters(param, m_nCurrentFrame, FRAME_ANMI_SHOWWINDOW);
// 
// 	int width = (int)(param[0] * WceUiGetScreenWidth());
// 	int height = (int)(param[1] * WceUiGetScreenHeight());
// 	int centerX = WceUiGetScreenWidth()/2 + (int)param[2];
// 	int centerY = WceUiGetScreenHeight()/2 + (int)(-param[3]);
// 	int x = centerX-width/2;
// 	int y = centerY-height/2;
// 
// 	CRect rc;
// 	rc.SetRect(x, y, x+width, y+height);
// 	RETAILMSG(1, (L"%d, %d, %d, %d ..... \n", rc.left, rc.top, rc.right, rc.bottom));
// 	//rc.SetRect(0, 0, 200 , 200);
// 	CDvdRender::GetInstance()->ShowVideo(TRUE, &rc);
}

void CDVDPhotoPlayerDlg::AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData)
{
	// CDVDStartDlg <-> CDVDPhotoPlayerDlg  不作动画
	if (nAnimMode == ANIM_MODE_UPLEVEL)
	{
		ShowLayer(bShow);
	}
	else
	{
		__super::AnimShowWindow(bShow, nAnimMode, dwAnimData);
	}
}


void CDVDPhotoPlayerDlg::ShowFileTip()
{
	GUINT16 cur = 0;
	GUINT16 total = 0;

	TCHAR buffer[10] = L"_/_";
	CWceUiLayer *ptip = GetLayerByName(L"file_tip");
	if( RET_OK == DVP_GetPlayingIdx(&cur) 
		&& RET_OK == DVP_GetTrackCount(&total)
		&& cur != (GUINT16)-1 
		&& total != (GUINT16)-1)
	{
		_stprintf(buffer, TEXT("%d/%d"), (cur+1), total); 
	}
	else
	{
		;
	}

	if (ptip)
	{
		ptip->SetText(buffer);
	}

}


void CDVDPhotoPlayerDlg::OnKeyPlayControl(UINT key, UINT param)
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
		CDvdRender::GetInstance()->Pause();
		break;
	case T_IR_GOTO:

		{
			GUINT16 cur = (GUINT16)-1;
			if( RET_OK != DVP_GetAudioPlayingIdx(&cur) || (param-1) != cur)
			{
				DVP_ItemClick((GINT16)param-1);
			}
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

void CDVDPhotoPlayerDlg::ShowVideo(BOOL bShow, RECT* prect)
{
	// 如果该窗口不可见，视频也不可见
	if (IsLayerVisible() 
		&& CMainWnd::GetInstance()->IsForegroundWindow()
		&& !IsVideoForbidden())
	{
		CDvdRender::GetInstance()->ShowVideo(bShow, prect);
	}
	else
	{
		CDvdRender::GetInstance()->ShowVideo(FALSE, prect);
	}

}