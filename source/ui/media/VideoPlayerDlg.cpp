#include "stdafx.h"
#include "VideoPlayerDlg.h"
#include "wceuipagelayer.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "MainWnd.h"
#include "msdkrender.h"
#include "uiddraw.h"
#include "msdkFileManager.h"
#include "WceUiLoadString.h"
#include "interface.h"
#include "OverlayDlg.h"

void CVideoPlayerDlg::MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data)
{
	CVideoPlayerDlg* pthis = (CVideoPlayerDlg*)user_data;
	if (pthis)
	{
		pthis->OnMediaEvent(media_event, param1, param2);
	}
}

CVideoPlayerDlg::CVideoPlayerDlg(void)
{
	CMsdkRender::GetInstance()->RegisterEventHandlerF(MediaEventHandler, (DWORD)this);
}

CVideoPlayerDlg::~CVideoPlayerDlg(void)
{
	CMsdkRender::GetInstance()->UnRegisterEventHandlerF(MediaEventHandler, (DWORD)this);
}

void CVideoPlayerDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_MSDK_PLAY_FILE);
	__super::OnInitDialog();

	CWceUiPageLayer* pLayer = (CWceUiPageLayer*)GetChildByName(L"bottom_frame");
	if (pLayer)
	{
		pLayer->SetExtraScroll(30);
		pLayer->SetScrollRange(0, WceUiGetScreenWidth());
	}

	m_pLayerBadfileInfo = GetChildByName(L"badfile_info");

	ShowPlayMode();
}


void CVideoPlayerDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_DELAY_SHOW_VIDEO)
	{
		KillTimer(TIMER_ID_DELAY_SHOW_VIDEO);
		// ��ʱ���Ǹ��첽����,�ڼ�״̬�п��ܸı�,��֤���ڿɼ�ʱ����ʾ��Ƶ
		ShowVideo(TRUE);
	}
	else if(wIDEvent == TIMER_ID_HIDE_MESSAGE_TIP)
	{
		KillTimer(TIMER_ID_HIDE_MESSAGE_TIP);
		HideMessageTip();
	}
	else if(wIDEvent == TIMER_ID_HIDE_INVALID_TIP)
	{
		KillTimer(TIMER_ID_HIDE_INVALID_TIP);
		CWceUiLayer *player = GetChildByName(L"invalid_tip");
		ASSERT(player);
		player->ShowLayer(FALSE);
	}

	__super::OnTimer(wIDEvent);
}


void CVideoPlayerDlg::OnAutoFullscreen()
{
	// ֻ���ڲ���״̬���������δ�����µ�����²��Զ�ȫ������
	if (!m_bFullScreen && GetAsyncKeyState(VK_LBUTTON) >= 0
		&& MSDK_MGSTATE_PLAYING == CMsdkRender::GetInstance()->FGetState())
	{
		E_MSDK_MGSTATE state = CMsdkRender::GetInstance()->FGetState();
		if (state == MSDK_MGSTATE_PLAYING)
		{
			SetFullScreen(TRUE);
		}
	}
}


void CVideoPlayerDlg::OnDlgShow(BOOL bShow)
{
	ShowFileTip();

	if (!IsAnimating() && CMainWnd::GetInstance()->IsForegroundWindow())	// ���û�ж���Ч��ʱ,ֱ��show or hide
	{
		CUiDDraw::GetInstance()->UpdateOverlay(bShow, 180);
		if (bShow)
		{
			SetTimer(TIMER_ID_DELAY_SHOW_VIDEO, 500);
		}
		else
		{
			ShowVideo(bShow);
		}
	}
	// �ж���ʱ���������̻������ʾ������

	__super::OnDlgShow(bShow);
}

void CVideoPlayerDlg::ShowVideo(BOOL bShow, RECT* prect)
{
	// ����ô��ڲ��ɼ�����ƵҲ���ɼ�
	if (IsLayerVisible() 
		&& CMainWnd::GetInstance()->IsForegroundWindow()
		&& !IsVideoForbidden())
	{
		CMsdkRender::GetInstance()->FShowVideo(bShow, prect);
	}
	else
	{
		CMsdkRender::GetInstance()->FShowVideo(FALSE, prect);
	}

	// �п����ں�̨����,ǰ̨�ڲ���ͼƬ,��ʱ���ŵ�videoҲ������ʾ����
	// ִ����dlg change��ͬ�Ķ���
	CMainWnd::GetInstance()->SendMessage(UI_MSG_DLG_CHANGE);

}
#include "WceUiLayerCtrl.h"
void CVideoPlayerDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CMMFileListDlg_ID, ui_msdk_get_cursrc());
	}
	else if (pButton->IsEqualName(L"next"))
	{
		PlayNext();
	}
	else if (pButton->IsEqualName(L"prev"))
	{
		PlayPrev();
	}
	else if (pButton->IsEqualName(L"play"))
	{
		CMsdkRender::GetInstance()->FPlay();
	}
	else if (pButton->IsEqualName(L"pause"))
	{
		CMsdkRender::GetInstance()->FPause();
	}
	else if (pButton->IsEqualName(L"random"))
	{
		OnRandom();
	}
	else if (pButton->IsEqualName(L"repeat"))
	{
		OnRepeat();
	}
	else if (pButton->IsEqualName(L"audio"))
	{
		OnBnClickAudio();
	}
	else if (pButton->IsEqualName(L"subtitle"))
	{
		OnBnClickSubtitle();
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}

void CVideoPlayerDlg::OnRandom()
{
	WORD random = CMsdkFileManager::GetInstance()->GetPlayRandomMode(MSDK_FILE_VIDEO);
	random++;
	random %= 2;	// ȡֵ0, 1,
	CMsdkFileManager::GetInstance()->SetPlayRandomMode(random, MSDK_FILE_VIDEO);
	ShowPlayMode();

	// tip
	CWceUiLoadString strRandom;
	strRandom.SetTextResID((random == MSDK_RANDOM_ON) ? L"MIS_PLAY_RANDOM_ON" : L"MIS_PLAY_RANDOM_OFF");
	ShowTipMessage(strRandom.GetString());
}

void CVideoPlayerDlg::OnRepeat()
{
	WORD repeat = CMsdkFileManager::GetInstance()->GetPlayRepeateMode(MSDK_FILE_VIDEO);
	repeat++;
	repeat %= 4;	// ȡֵ0, 1, 2, 3
	CMsdkFileManager::GetInstance()->SetPlayRepeateMode(repeat, MSDK_FILE_VIDEO);
	ShowPlayMode();

	// tip
	CWceUiLoadString strRepeat;
	switch (repeat)
	{
	case MSDK_REPEATE_ALL:
		strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_ALL");
		break;
	case MSDK_REPEATE_FOLDER:
		strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_FOLDER");
		break;
	case MSDK_REPEATE_ONE:
		strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_SINGLE");
		break;
	case MSDK_REPEATE_OFF:
	default:
		strRepeat.SetTextResID(L"MIS_PLAY_REPEAT_OFF");
		break;
	}
	ShowTipMessage(strRepeat.GetString());
}

void CVideoPlayerDlg::OnBnClickSubtitle()
{
	INT nSubtitleCnt = 0;
	INT nCurSubtitle = 0;
	CMsdkRender::GetInstance()->FGetSubtitleInfo(&nSubtitleCnt, &nCurSubtitle);

	if (CMsdkRender::GetInstance()->FGetState() == MSDK_MGSTATE_STOP || nSubtitleCnt<=0)
	{
		ShowInvalidIcon();
	}
	else
	{
		nCurSubtitle = (nCurSubtitle+1) % (nSubtitleCnt+1);
		CMsdkRender::GetInstance()->FSetCurrentSubtitle(nCurSubtitle);

		TCHAR tSubtitle[32] = L"\0";
		if (nCurSubtitle == 0)	// subtitle off
		{
			CWceUiLoadString str(L"DVD_SUBTITLE_OFF");
			wsprintf(tSubtitle, TEXT("%s"), str.GetString());
		}
		else
		{
			CWceUiLoadString str(L"DVD_SUBTITLE");
			wsprintf(tSubtitle, TEXT("%s %d/%d"), str.GetString(), nCurSubtitle, nSubtitleCnt);
		}
		ShowTipMessage(tSubtitle);
	}
}

void CVideoPlayerDlg::OnBnClickAudio()
{
	//audio
	BYTE nAudioCnt = 0;
	BYTE nCurAudio = 0;
	CMsdkRender::GetInstance()->FGetAudioInfo(&nAudioCnt, &nCurAudio);


	if (CMsdkRender::GetInstance()->FGetState() == MSDK_MGSTATE_STOP || nAudioCnt<=1)
	{
		ShowInvalidIcon();
	}
	else
	{
		nCurAudio = (nCurAudio+1) % nAudioCnt;
		CMsdkRender::GetInstance()->FSetCurrentAudio(nCurAudio);

		TCHAR tAudio[32] = L"\0";
		CWceUiLoadString str(L"DVD_AUDIO");
		wsprintf(tAudio, TEXT("%s %d/%d"), str.GetString(), nCurAudio+1, nAudioCnt);
		ShowTipMessage(tAudio);
	}
}


void CVideoPlayerDlg::UpdateMediaInfo()
{
	ShowFileTip();
//	ShowBadfileTipInfo(FALSE, NULL);
	// ������Ҳ��Ϊ��ʼλ��
	m_pProgressSlider->SetPos(0);
	UINT outDuration = CMsdkRender::GetInstance()->FGetDuration();
	ShowMediaTimeInfo(0, outDuration);
}


void CVideoPlayerDlg::OnGraphBuilt()
{
	if (!IsAnimating())
	{
		ShowVideo(TRUE);
	}

	UpdateMediaInfo();
}


void CVideoPlayerDlg::OnErrorOccured(UINT param1, UINT param2)
{
	ShowFileTip();	// error����ʱ���ᴥ��OnGraphBuilt()

	if (param1 != E_MSDK_OUTOFMEMORY)
	{
		if (param1 == E_MSDK_UNSUPPORTED_VIDEO)
		{
			//ShowBadfileTipInfo(TRUE, L"MIS_UNSUPPORTED_VIDEO");
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"MIS_UNSUPPORTED_VIDEO", TRUE, 1000);
		}
		else if (param1 == E_MSDK_UNSUPPORTED_AUDIO)
		{
			//ShowBadfileTipInfo(TRUE, L"MIS_UNSUPPORTED_AUDIO");
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"MIS_UNSUPPORTED_AUDIO", TRUE, 1000);
		} 
		else
		{
			//ShowBadfileTipInfo(TRUE, L"MIS_UNSUPPORTED_FILE");
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"MIS_UNSUPPORTED_FILE", TRUE, 1000);
		}
	}
}

void CVideoPlayerDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	switch (media_event)
	{
	case EVT_MSDK_GRAPH_BUILT:
		OnGraphBuilt();
		break;
	case EVT_MSDK_FINISHED:	
		break;
	case EVT_MSDK_ERROR_OCCURED:
		OnErrorOccured(param1, param2);
		break;
	case EVT_MSDK_GRAPH_CHANGED:
		break;
	case EVT_MSDK_PLAYBACK_POSITION_STATUS:
		OnPosChange(param1, param2);
		break;
	case EVT_MSDK_PLAYED:
		m_pLayerPlay->ShowLayer(FALSE);
		m_pLayerPause->ShowLayer(TRUE);
		break;
	case EVT_MSDK_PAUSED:
	case EVT_MSDK_STOPED:
		m_pLayerPlay->ShowLayer(TRUE);
		m_pLayerPause->ShowLayer(FALSE);
		break;

	default:
		break;
	}
}


void CVideoPlayerDlg::OnPosChange(UINT param1, UINT param2)
{
	if (param1 == 0)
	{
		return;
	}

	UINT curDuration = param1 / 1000;
	UINT outDuration = CMsdkRender::GetInstance()->FGetDuration();

	// ������
	UpdataProgress(curDuration, outDuration);
	// ����ʱ��
	ShowMediaTimeInfo(curDuration, outDuration);
}


void CVideoPlayerDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"progress") && status==SLIDER_MOUSE_UP)	// seek�Ƚ���������ֻ��ӦUP�¼�
	{
		UINT outDuration = CMsdkRender::GetInstance()->FGetDuration();
		UINT curDuration = pos * outDuration / 1000;
		CMsdkRender::GetInstance()->FSeek(curDuration);

		GetWindow()->IgnoreMsgBeforeTime();
	}
}

void CVideoPlayerDlg::PlayNext()
{
	// ���������CMusicPlayerDlg::PlayNext()
	int index = CMsdkFileManager::GetInstance()->PlayFileNext(MSDK_FILE_VIDEO);
	if (index<0)	//û�гɹ�������һ��
	{
		index = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(MSDK_FILE_VIDEO);
	}
	LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, MSDK_FILE_VIDEO);
	CMsdkRender::GetInstance()->FSetSourceFile(file_path);
}

void CVideoPlayerDlg::PlayPrev()
{
	int index = CMsdkFileManager::GetInstance()->PlayFilePrev(MSDK_FILE_VIDEO);
	LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, MSDK_FILE_VIDEO);
	CMsdkRender::GetInstance()->FSetSourceFile(file_path);
}



void CVideoPlayerDlg::OnAnimateBegin()
{
	// ������ʼ��,�п���֮ǰ����Ƶ��ʾ������ûִ�У�ȡ����
	KillTimer(TIMER_ID_DELAY_SHOW_VIDEO);
	KillTimer(TIMER_ID_DELAY_SHOW_OVERLAY);

	// ��ʱ����Ƶ�����ص�,�о���Ƶ�������ǱȽ���
	if (/*m_nAnimMode == ANIM_MODE_UPLEVEL*/1)	//Ϊ�˶�������,����Ƶ�ص�,overlayҲ�ɲ���ʾ
	{
		CUiDDraw::GetInstance()->UpdateOverlay(FALSE, 180);
		CMsdkRender::GetInstance()->FShowVideo(FALSE, NULL);
	}
	else	// �������涯��
	{
		if (m_bAnimShow)
		{
			CUiDDraw::GetInstance()->UpdateOverlay(TRUE, 180);
		}
	}

	__super::OnAnimateBegin();
}

void CVideoPlayerDlg::OnAnimateEnd()
{
	if (!m_bAnimShow)	// hide����ʱ
	{
		CMsdkRender::GetInstance()->FShowVideo(FALSE, NULL);
		CUiDDraw::GetInstance()->UpdateOverlay(FALSE, 180);
	}
	else
	{
		//CUiDDraw::GetInstance()->UpdateOverlay(TRUE, 180);
		SetTimer(TIMER_ID_DELAY_SHOW_OVERLAY, 50);
		//SetTimer(TIMER_ID_DELAY_SHOW_VIDEO, 1000);
	}

	__super::OnAnimateEnd();
}

void CVideoPlayerDlg::OnAnimating()
{
	// ��ʱ����Ƶ�����ص�,�о���Ƶ�������ǱȽ���
	if (/*m_nAnimMode == ANIM_MODE_UPLEVEL*/1)
	{
		;
	}
	else
	{
		if (m_nCurrentFrame %2 == 0)	//�ٶ��е���,ÿ��֡������һ��
		{
			float param[4];
			AnimGetParameters(param, m_nCurrentFrame, FRAME_ANMI_SHOWWINDOW);

			int width = (int)(param[0] * WceUiGetScreenWidth());
			int height = (int)(param[1] * WceUiGetScreenHeight());
			int centerX = WceUiGetScreenWidth()/2 + (int)param[2];
			int centerY = WceUiGetScreenHeight()/2 + (int)(-param[3]);
			int x = centerX-width/2;
			int y = centerY-height/2;

			CRect rc;
			rc.SetRect(x, y, x+width, y+height);
			ShowVideo(TRUE, &rc);

		}
	}
}


void CVideoPlayerDlg::ShowFileTip()
{
	int cur = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(MSDK_FILE_VIDEO);
	int total = CMsdkFileManager::GetInstance()->GetPlayingFilesCount(MSDK_FILE_VIDEO);
	if (total<0)
	{
		total = 0;	// ����������ݲ��Ϸ�,��ʾ0
	}

	TCHAR buffer[10] = L"";
	_stprintf(buffer, TEXT("%d/%d"), (cur+1), total); 
	CWceUiLayer *ptip = GetLayerByName(L"file_tip");
	if (ptip)
	{
		ptip->SetText(buffer);
	}
}


void CVideoPlayerDlg::ShowPlayMode()
{
	WORD random = CMsdkFileManager::GetInstance()->GetPlayRandomMode(MSDK_FILE_VIDEO);
	WORD repeat = CMsdkFileManager::GetInstance()->GetPlayRepeateMode(MSDK_FILE_VIDEO);
	CWceUiLayer* pRandom = GetChildByName(L"random_tip");
	CWceUiLayer* pRepeat = GetChildByName(L"repeat_tip");
	if (pRandom)
	{
		pRandom->SetStyle(random == 0 ? L"random_tip_off" : L"random_tip_on");
	}

	if (pRepeat)
	{
		LPCTSTR style[] = {L"repeat_tip_off", L"repeat_tip_single", L"repeat_tip_folder", L"repeat_tip_all"};
		if (repeat>=0 && repeat<4)
		{
			pRepeat->SetStyle(style[repeat]);
		}
	}

	ui_led_set_playmode(random, repeat);
}


void CVideoPlayerDlg::OnKeyPlayControl(UINT key, UINT param)
{
	BYTE byte0 = (BYTE)((param>>24) & 0xFF);

	switch (key)
	{
	case T_PREV:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PREV");
		PlayPrev();
		break;
	case T_NEXT:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_NEXT");
		PlayNext();
		break;
	case T_PLAY:	// param byte0 : 0 ~ ������ͣ���ϼ�, 1 ~ ���ţ� 2 ~ ��ͣ
		if (byte0 == 1)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
			CMsdkRender::GetInstance()->FPlay();
		}
		else if (byte0 == 2)
		{
			COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
			CMsdkRender::GetInstance()->FPause();
		}
		else
		{
			if( MSDK_MGSTATE_PLAYING == CMsdkRender::GetInstance()->FGetState())
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PAUSE");
				CMsdkRender::GetInstance()->FPause();
			}
			else
			{
				COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_PLAY");
				CMsdkRender::GetInstance()->FPlay();
			}
		}
		break;
	case T_STOP:
		COverlayDlg::GetInstance()->MessageTip_ShowUI(L"PLAY_STOP");
		SetFullScreen(FALSE);
		CMsdkRender::GetInstance()->FStop();
		break;
	case T_IR_GOTO:
		if (CMsdkFileManager::GetInstance()->PlayFileGoto(param-1, MSDK_FILE_VIDEO))	//�ɹ�GOTOָ����Ŀ
		{
			LPCTSTR file_path = CMsdkFileManager::GetInstance()->GetPlayFilePath(param-1, MSDK_FILE_VIDEO);
			CMsdkRender::GetInstance()->FSetSourceFile(file_path);
		}
		break;
	case T_TRACK:
		OnBnClickAudio();
		break;
	case T_REPEAT:
		OnRepeat();
		break;
	case T_SHUFFLE:
		OnRandom();
		break;
	default:
		break;
	}
}

void CVideoPlayerDlg::ShowBadfileTipInfo(BOOL bShow, LPCTSTR text_id)
{
	if (m_pLayerBadfileInfo)
	{
		if (bShow)
		{
			m_pLayerBadfileInfo->GetStyle()->SetTextResID(text_id);
		}

		m_pLayerBadfileInfo->ShowLayer(bShow);
	}
}


void CVideoPlayerDlg::HideMessageTip()
{
	CWceUiLayer *player = GetLayerByName(L"message_tip");
	if (player)
	{
		player->ShowLayer(FALSE);
	}

	CWceUiLayer* pfiletip = GetLayerByName(L"file_tip");
	if (pfiletip)
	{
		pfiletip->ShowLayer(TRUE);	// λ���ص���,��ʱ������
	}
}

void CVideoPlayerDlg::ShowTipMessage(LPCTSTR message)
{
// 	CWceUiLayer *player = GetLayerByName(L"message_tip");
// 	if (player)
// 	{
// 		player->SetText(message);
// 		player->ShowLayer(TRUE);
// 		SetTimer(TIMER_ID_HIDE_MESSAGE_TIP, 2000);
// 
// 		CWceUiLayer* pfiletip = GetLayerByName(L"file_tip");
// 		if (pfiletip)
// 		{
// 			pfiletip->ShowLayer(FALSE);	// λ���ص���,��ʱ������
// 		}
// 	}

	// ��ʾ��overlay�� 
	COverlayDlg::GetInstance()->MessageTip_ShowUI(message, FALSE);
}

void CVideoPlayerDlg::ShowInvalidIcon()
{
	CWceUiLayer *player = GetChildByName(L"invalid_tip");
	if (player)
	{
		player->ShowLayer(TRUE);
		SetTimer(TIMER_ID_HIDE_INVALID_TIP, 500);
	}
}


LRESULT CVideoPlayerDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_MSDK_PLAY_FILE)
	{
		ShowBadfileTipInfo(FALSE, NULL);
	}
	else if (message == UI_MSG_MCU_KEY_COMMAND)
	{
		int status = (wParam>>16) & 0xFF;
		int id = wParam & 0xFF;

		switch (id)
		{
		case T_SUB_T:
			OnBnClickSubtitle();
			break;
		default:
			break;
		}
	}


	return __super::OnMessage(message, wParam, lParam);
}
