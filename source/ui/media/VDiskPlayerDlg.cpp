#include "stdafx.h"
#include "VDiskPlayerDlg.h"
#include "wceuipagelayer.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "MainWnd.h"
#include "msdkrender.h"
#include "uiddraw.h"
#include "msdkFileManager.h"
#include "sysUtil.h"
#include "config.h"
#include "tzUtil.h"
#include "WceUi32Bitmap.h"
#include "WceUiLoadString.h"
#include "interface.h"
#include "vdiskFileManager.h"

#include <imaging.h> 

CVDiskPlayerDlg::CVDiskPlayerDlg(void)
{
	CMsdkRender::GetInstance()->RegisterEventHandlerF(MediaEventHandler, (DWORD)this);
}

CVDiskPlayerDlg::~CVDiskPlayerDlg(void)
{
	CMsdkRender::GetInstance()->UnRegisterEventHandlerF(MediaEventHandler, (DWORD)this);
}

void CVDiskPlayerDlg::OnInitDialog()
{
	RegisterMsg(NAVI_MSG_LAN_CHANGE);

	__super::OnInitDialog();

	CWceUiPageLayer* pLayer = (CWceUiPageLayer*)GetChildByName(L"bottom_frame");
	// 	pLayer->SetExtraScroll(30);
	// 	pLayer->SetScrollRange(0, 800);

	// spectrum
	m_pLayerSpectrum = (CSpectrumLayer*)GetChildByName(L"spectrum");
	m_pLayerSpectrum->ShowLayer(FALSE);
	SetTimer(TIMER_ID_SPECTRUM, 150);

	// 第一次进来时,文件可能已经开始播放了,收不到EVT_MSDK_GRAPH_BUILT,这里主动调用一次
	UpdateMediaInfo();
	SetSpectrumInfo();

	ShowPlayMode();
}

void CVDiskPlayerDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_SPECTRUM)
	{
		m_pLayerSpectrum->InvalidateRect(NULL);
	}
	else
	{
		__super::OnTimer(wIDEvent);
	}
}

void CVDiskPlayerDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		ShowFileTip();
		ShowTitle();
	}
}

#include "msdkFileManager.h"
void CVDiskPlayerDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CVDiskListDlg_ID, SOURCE_VDISK);
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
		WORD random = CVDiskFileManager::GetInstance()->GetPlayRandomMode();
		random++;
		random %= 2;	// 取值0, 1,
		CVDiskFileManager::GetInstance()->SetPlayRandomMode(random);
		ShowPlayMode();
	}
	else if (pButton->IsEqualName(L"repeat"))
	{
		WORD repeat = CVDiskFileManager::GetInstance()->GetPlayRepeateMode();
		// 对虚拟碟只能在这二者间切换
		// MSDK_REPEATE_ONE	or MSDK_REPEATE_FOLDER
		repeat = (repeat == MSDK_REPEATE_ONE) ? MSDK_REPEATE_FOLDER : MSDK_REPEATE_ONE;
		CVDiskFileManager::GetInstance()->SetPlayRepeateMode(repeat);
		ShowPlayMode();
	}
	else if (pButton->IsEqualName(L"prevdisk"))
	{
		PlayPrevDisk();
	}
	else if (pButton->IsEqualName(L"nextdisk"))
	{
		PlayNextDisk();
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}

void CVDiskPlayerDlg::PlayNextDisk()
{
	int disk_id = CVDiskFileManager::GetInstance()->GetPlayingDiskID();
	if (disk_id >= 0)
	{
		int next_id = CVDiskFileManager::GetInstance()->NextDiskAvailable(disk_id);
		if (next_id != disk_id)	// 返回的不是同一个disk,
		{
			CVDiskFileManager::GetInstance()->SetPlayFileData(next_id, 0);

			LPCTSTR file_path = CVDiskFileManager::GetInstance()->GetPlayFilePath(0);
			CMsdkRender::GetInstance()->FSetSourceFile(file_path);
		}
	}
}

void CVDiskPlayerDlg::PlayPrevDisk()
{
	int disk_id = CVDiskFileManager::GetInstance()->GetPlayingDiskID();
	if (disk_id >= 0)
	{
		int prev_id = CVDiskFileManager::GetInstance()->PrevDiskAvailable(disk_id);
		if (prev_id != disk_id)	// 返回的不是同一个disk,
		{
			CVDiskFileManager::GetInstance()->SetPlayFileData(prev_id, 0);

			LPCTSTR file_path = CVDiskFileManager::GetInstance()->GetPlayFilePath(0);
			CMsdkRender::GetInstance()->FSetSourceFile(file_path);
		}
	}
}

void CVDiskPlayerDlg::PlayNext()
{
	int index = CVDiskFileManager::GetInstance()->PlayFileNext();
	LPCTSTR file_path = CVDiskFileManager::GetInstance()->GetPlayFilePath(index);
	CMsdkRender::GetInstance()->FSetSourceFile(file_path);
}

void CVDiskPlayerDlg::PlayPrev()
{
	int index = CVDiskFileManager::GetInstance()->PlayFilePrev();
	LPCTSTR file_path = CVDiskFileManager::GetInstance()->GetPlayFilePath(index);
	CMsdkRender::GetInstance()->FSetSourceFile(file_path);
}

void CVDiskPlayerDlg::MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data)
{
	CVDiskPlayerDlg* pthis = (CVDiskPlayerDlg*)user_data;
	if (pthis)
	{
		pthis->OnMediaEvent(media_event, param1, param2);
	}
}

void CVDiskPlayerDlg::UpdateMediaInfo()
{
	int index = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(MSDK_FILE_MUSIC);
	CString lrcFilepath = CMsdkFileManager::GetInstance()->GetPlayFilePath(index, MSDK_FILE_MUSIC);

	if (m_strPlayFilePath == lrcFilepath)	// 重复调用时，不重复更新数据
	{
		return;
	}
	m_strPlayFilePath = lrcFilepath;


	ShowFileTip();
	ShowTitle();

	// 进度条也置为初始位置
	m_pProgressSlider->SetPos(0);
	UINT outDuration = CMsdkRender::GetInstance()->FGetDuration();
	ShowMediaTimeInfo(0, outDuration);
}

void CVDiskPlayerDlg::OnGraphBuilt()
{
	UpdateMediaInfo();
}


void CVDiskPlayerDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	switch (media_event)
	{
	case EVT_MSDK_GRAPH_BUILT:
		OnGraphBuilt();
		break;
	case EVT_MSDK_FINISHED:	
		break;
	case EVT_MSDK_ERROR_OCCURED:
		break;
	case EVT_MSDK_GRAPH_CHANGED:
		break;
	case EVT_MSDK_PLAYBACK_POSITION_STATUS:
		OnPosChange(param1, param2);
		break;
	case EVT_MSDK_PLAYED:
		SetSpectrumInfo();
		m_pLayerPlay->ShowLayer(FALSE);
		m_pLayerPause->ShowLayer(TRUE);
		break;
	case EVT_MSDK_PAUSED:
		m_pLayerPlay->ShowLayer(TRUE);
		m_pLayerPause->ShowLayer(FALSE);
		break;
	case EVT_MSDK_STOPED:
		break;

	default:
		break;
	}
}

void CVDiskPlayerDlg::SetSpectrumInfo()
{
	HAUDIOINFO hAudioInfo = CMsdkRender::GetInstance()->FGetAudioInfoHandle();
	if (NULL != hAudioInfo)
	{
		m_pLayerSpectrum->SetAudioInfo(hAudioInfo);
		if (CMsdkRender::GetInstance()->FIsWmaPlaying())
		{
			m_pLayerSpectrum->SetSrcMode(SRC_MODE_MSK_WMA);
		}
		else
		{
			m_pLayerSpectrum->SetSrcMode(SRC_MODE_MSK);
		}
	}
}

void CVDiskPlayerDlg::OnPosChange(UINT param1, UINT param2)
{
	if (param1 == 0)
	{
		return;
	}

	UINT curDuration = param1 / 1000;
	UINT outDuration = CMsdkRender::GetInstance()->FGetDuration();

	// 进度条
	UpdataProgress(curDuration, outDuration);
	// 播放时间
	ShowMediaTimeInfo(curDuration, outDuration);
}

void CVDiskPlayerDlg::OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos)
{
	if (pslider->IsEqualName(L"progress") && status==SLIDER_MOUSE_UP)
	{
		UINT outDuration = CMsdkRender::GetInstance()->FGetDuration();
		UINT curDuration = pos * outDuration / 1000;
		CMsdkRender::GetInstance()->FSeek(curDuration);

		GetWindow()->IgnoreMsgBeforeTime();
	}
}


void CVDiskPlayerDlg::ShowFileTip()
{
	int cur = CMsdkFileManager::GetInstance()->GetPlayingFileIndex(MSDK_FILE_MUSIC);
	int total = CMsdkFileManager::GetInstance()->GetPlayingFilesCount(MSDK_FILE_MUSIC);
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

void CVDiskPlayerDlg::ShowTitle()
{
	CWceUiLayer *ptitle = GetLayerByName(L"title");
	if (ptitle)
	{
		CWceUiLoadString strLoadTitle(L"MAIN_VDISK");
		CString strTitle;
		strTitle.Format(L"%s[Disc%02d]", strLoadTitle.GetString(), CVDiskFileManager::GetInstance()->GetPlayingDiskID()+1);
		ptitle->SetText(strTitle);
	}
}

void CVDiskPlayerDlg::ShowPlayMode()
{
	WORD random = CMsdkFileManager::GetInstance()->GetPlayRandomMode(MSDK_FILE_MUSIC);
	WORD repeat = CMsdkFileManager::GetInstance()->GetPlayRepeateMode(MSDK_FILE_MUSIC);
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
}


LRESULT CVDiskPlayerDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == NAVI_MSG_LAN_CHANGE)
	{
		ShowTitle();	// 支持多语言
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CVDiskPlayerDlg::OnKeyPlayControl(UINT key, UINT param)
{
	BYTE byte0 = (BYTE)((param>>24) & 0xFF);

	switch (key)
	{
	case T_PREV:
		PlayPrev();
		break;
	case T_NEXT:
		PlayNext();
		break;
	case T_PLAY:	// param byte0 : 0 ~ 播放暂停复合键, 1 ~ 播放， 2 ~ 暂停
		if (byte0 == 1)
		{
			CMsdkRender::GetInstance()->FPlay();
		}
		else if (byte0 == 2)
		{
			CMsdkRender::GetInstance()->FPause();
		}
		else
		{
			if( MSDK_MGSTATE_PLAYING == CMsdkRender::GetInstance()->FGetState())
				CMsdkRender::GetInstance()->FPause();
			else
				CMsdkRender::GetInstance()->FPlay();
		}
		break;
	case T_STOP:
		CMsdkRender::GetInstance()->FStop();
		break;
	}
}