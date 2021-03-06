#include "stdafx.h"
#include "WceUiListBox.h"
#include "WceUiBase.h"
#include "VDiskListDlg.h"
#include "config.h"
#include "interface.h"
#include "sysUtil.h"
#include "vdiskFileManager.h"
#include "msdkrender.h"
#include "DVDStartDlg.h"
#include "dvdrender.h"
#include "SourceManager.h"
#include "WceUiLoadString.h"
#include "VDiskChooseDiskDlg.h"
#include "tzUtil.h"
#include "protocol.h"
#include "MainWnd.h"

#define RIP_INFO_FILE_NAME	L"ripinfo.bin"

void CVDiskListDlg::MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data)
{
	CVDiskListDlg* pthis = (CVDiskListDlg*)user_data;
	if (pthis)
	{
		pthis->OnMediaEvent(media_event, param1, param2);
	}
}

void CVDiskListDlg::DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData)
{
	CVDiskListDlg* pthis = (CVDiskListDlg*)u4CustomData;
	pthis->OnDvpMediaEvent(u4Evt, u4Param1, u4Param2);
}

CVDiskListDlg::CVDiskListDlg(void)
: m_nCurRipTrack(0)
, m_nRipDisk(-1)
, m_bIsPopup(FALSE)
{
	sysutil::nss_get_instance()->vdisk_is_ripping = FALSE;
	CMsdkRender::GetInstance()->RegisterEventHandlerF(MediaEventHandler, (DWORD)this);
	CDvdRender::GetInstance()->RegisterEventHandler(CVDiskListDlg::DvpAgentEventListener, (DWORD)this);

}

CVDiskListDlg::~CVDiskListDlg(void)
{
	sysutil::nss_get_instance()->vdisk_is_ripping = FALSE;
	CMsdkRender::GetInstance()->UnRegisterEventHandlerF(MediaEventHandler, (DWORD)this);
	CDvdRender::GetInstance()->UnRegisterEventHandler(CVDiskListDlg::DvpAgentEventListener);

}

void CVDiskListDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(UI_MSG_LBUTTON_DOWN);
	RegisterMsg(UI_MSG_LBUTTON_UP);

	InitActiveDisk();	

	SetPlayingDisk(-1);		// 开始时隐藏anim disk
	SetTimer(TIMER_ID_DELAY_ANIMATING, 3000);

}

BOOL CVDiskListDlg::IsDiskActive(int nIndex)
{
	return (CVDiskFileManager::GetInstance()->IsDiskExist(nIndex) && nIndex!=m_nRipDisk);
}

void CVDiskListDlg::InitActiveDisk()
{
	for (int i=0; i<VDISK_MAX_DISK_COUNT; i++)
	{
		TCHAR layer_name[16];
		TCHAR layer_text[8];
		_stprintf(layer_name, L"disk%d", i);
		_stprintf(layer_text, L"%02d", i+1);
		CWceUiButton* player = (CWceUiButton*)GetChildByName(layer_name);
		if (player)
		{
			player->SetText(layer_text);
			ActiveDiskButton(i, IsDiskActive(i));
		}
	}
}

void CVDiskListDlg::ActiveDiskButton(int nIndex, BOOL bActive)
{
	TCHAR layer_name[16];
	_stprintf(layer_name, L"disk%d", nIndex);
	CWceUiButton* player = (CWceUiButton*)GetChildByName(layer_name);
	if (player)
	{
		TCHAR style_normal[MAX_TAG_NAME_LEN];
		TCHAR style_down[MAX_TAG_NAME_LEN];
		if (bActive)
		{
			_stprintf(style_normal, L"disk%d_active_normal", nIndex);
			_stprintf(style_down, L"disk%d_active_down", nIndex);
		}
		else
		{
			_stprintf(style_normal, L"disk%d_normal", nIndex);
			_stprintf(style_down, L"disk%d_down", nIndex);
		}

		player->SetStatusStyleName(style_normal, CWceUiButton::STATUS_NORMAL);
		player->SetStatusStyleName(style_down, CWceUiButton::STATUS_DOWN);
	}

}

void CVDiskListDlg::SetPlayingDisk(int nDiskIndex)
{
	POINT pt = {26, 77}; // 第一个disk的位置
	int XGap = 129, YGap = 158, LineCount = 6;	// 其它坐标可根据这三个值计算

	CAnimDiskLayer* player = (CAnimDiskLayer*)GetChildByName(L"anim_disk");
	if (nDiskIndex>=0 && nDiskIndex<VDISK_MAX_DISK_COUNT)
	{
		if (player)
		{
			player->Reset();
			player->ShowLayer(TRUE);
			CRect rc;
			player->GetWindowRect(rc);
			pt.x += XGap * (nDiskIndex % LineCount);
			pt.y += YGap * (nDiskIndex / LineCount);
			player->MoveLayer(pt.x, pt.y, rc.Width(), rc.Height());
		}
	}
	else
	{
		if (player)
		{
			player->ShowLayer(FALSE);
		}

	}
}

void CVDiskListDlg::OnBnClick(CWceUiButton* pButton)
{
	// 处理disk按钮
	int i = 0;
	for (; i<VDISK_MAX_DISK_COUNT; i++)
	{
		TCHAR layer_name[16];
		_stprintf(layer_name, L"disk%d", i);
		if (pButton->IsEqualName(layer_name))
		{
			break;
		}
	}

	if (i < VDISK_MAX_DISK_COUNT)	// 点击的是disk
	{
		if (IsDiskActive(i))
		{
			if (i != CVDiskFileManager::GetInstance()->GetPlayingDiskID())
			{
				SetPlayingDisk(i);
			}
			PlayItem(i);
		}
		else if (i == CVDiskFileManager::GetInstance()->GetPlayingDiskID())		// 播放碟片与录制碟片相同时,会走这条分支(断点录制)
		{
			PlayItem(i);
		}
		else if (sysutil::nss_get_instance()->vdisk_is_ripping && i == m_nRipDisk)	// 碟片正在录制
		{
			;
		}

		return;
	}

	// 
	if (pButton->IsEqualName(L"record"))
	{
		m_bIsPopup = TRUE;
		OnBnRecord();
		m_bIsPopup = FALSE;
	}
	else if (pButton->IsEqualName(L"delete"))
	{
		if (CVDiskFileManager::GetInstance()->FirstDiskAvailable() < 0)	// 无有效碟片
		{
			CWceUiLoadString str(L"VDISK_MSG_NO_DISK_TO_DELETE");
			ShowTipMessage(str.GetString());
		}
		else
		{
			m_bIsPopup = TRUE;
			int nDiskID = CVDiskChooseDiskDlg::MessageBox(VDISK_DELETE, m_nRipDisk);
			m_bIsPopup = FALSE;
			if (nDiskID>=0 && nDiskID<VDISK_MAX_DISK_COUNT)
			{
				DeleteDisk(nDiskID);
			}
		}
	}

	__super::OnBnClick(pButton);
}

void CVDiskListDlg::DeleteDisk(int nDiskIndex)
{
	if (IsDiskActive(nDiskIndex))
	{
		int nPlayingDiskID = CVDiskFileManager::GetInstance()->GetPlayingDiskID();
		int nPlayingFileIndex = CVDiskFileManager::GetInstance()->GetPlayingFileIndex();

		if (nDiskIndex == nPlayingDiskID)	// 碟片正在播放，跳到一个可播放碟片,如果没有碟片可播放,停止播放
		{
			int next_id = CVDiskFileManager::GetInstance()->NextDiskAvailable(nDiskIndex);
			if (next_id != nDiskIndex)	// 返回的不是同一个disk,
			{
				nPlayingDiskID = next_id;
				nPlayingFileIndex = 0;
			}
			else	// 表明只有一个碟片可播放,且要删除该碟片
			{
				nPlayingDiskID = -1;
				nPlayingFileIndex = -1;
			}

			
			CMsdkRender::GetInstance()->FStopnClear();	//停止播放并释放文件句柄
			tzutil::DeleteDirectory(CVDiskFileManager::GetInstance()->GetVDiskPath(nDiskIndex));

			CVDiskFileManager::GetInstance()->InitLoadData();	// 删除目录后，重新初始化数据
			CVDiskFileManager::GetInstance()->SetPlayFileData(nPlayingDiskID, nPlayingFileIndex);
			if (nPlayingDiskID>=0 && nPlayingFileIndex>=0)
			{
				LPCTSTR file_path = CVDiskFileManager::GetInstance()->GetPlayFilePath(nPlayingFileIndex);
				CMsdkRender::GetInstance()->FSetSourceFile(file_path);
			}
		}
		else
		{
			tzutil::DeleteDirectory(CVDiskFileManager::GetInstance()->GetVDiskPath(nDiskIndex));
			CVDiskFileManager::GetInstance()->InitLoadData();	// 删除目录后，重新初始化数据
			CVDiskFileManager::GetInstance()->SetPlayFileData(nPlayingDiskID, nPlayingFileIndex);
		}

		// 有碟片删除了,重新初始化碟片状态
		InitActiveDisk();
		SetPlayingDisk(nPlayingDiskID);
	}
}

void CVDiskListDlg::OnBnRecord()
{
	CWceUiButton* player = (CWceUiButton*)GetChildByName(L"record");

	if (sysutil::nss_get_instance()->vdisk_is_ripping)
	{
		OnRipInterrupt();
	}
	else
	{
		if (sysutil::nss_get_instance()->is_source_available(SOURCE_DVD))	// 有碟片才能录制
		{
			if (CVDiskFileManager::GetInstance()->FirstDiskUnavailable() < 0)	// 无空闲碟片位置用来录制
			{
				CWceUiLoadString str(L"VDISK_MSG_NO_EMPTY_DISK");
				ShowTipMessage(str.GetString());
			}
			else
			{
				int nDiskID = CVDiskChooseDiskDlg::MessageBox(VDISK_RECORD, m_nRipDisk);
				if (nDiskID>=0 && nDiskID<VDISK_MAX_DISK_COUNT)
				{
					sysutil::nss_get_instance()->vdisk_is_ripping = TRUE;
					CSourceManager::GetInstance()->SetRearSrc(SOURCE_DVD);
					CMainWnd::GetInstance()->OnDVDLoading(TRUE);
					m_nCurRipTrack = 0;
					m_nRipDisk = nDiskID;

					if (player)
					{
						player->SetStatusStyleName(L"stop_record_normal", CWceUiButton::STATUS_NORMAL);
						player->SetStatusStyleName(L"stop_record_down", CWceUiButton::STATUS_DOWN);
					}

				}
			}
		}
		else
		{
			CWceUiLoadString str(L"MIS_DISK_NOT_EXIST");
			ShowTipMessage(str.GetString());
		}
	}
}

void CVDiskListDlg::PlayItem(int nDiskIndex)
{
	// 如果当前点击的碟片正好是播放的碟片,则不需重新播放
	if (nDiskIndex == CVDiskFileManager::GetInstance()->GetPlayingDiskID())
	{
		;
	}
	else	// 播放新碟片
	{
		if (CVDiskFileManager::GetInstance()->IsDiskExist(nDiskIndex))
		{
			CVDiskFileManager::GetInstance()->SetPlayFileData(nDiskIndex, 0);
			SetTimer(TIMER_ID_DELAY_PLAY_ITEM, 500);
		}
	}

	// 显示播放窗口
	ShowPlayingDlg();
}


void CVDiskListDlg::ShowPlayingDlg()
{
	if (IsLayerVisible())
	{
		CDlgManager::GetInstance()->ShowDlg(CVDiskPlayerDlg_ID, SOURCE_VDISK);
	}
	else	// 如果列表窗口已经在后台了,把源的当前窗口切换为播放窗口,但不显示出来
	{
		CDlgManager::GetInstance()->SetActiveDlg(CVDiskPlayerDlg_ID, SOURCE_VDISK);
		// 如果last dlg刚好是该窗口,需主动更新last dlg信息，从其它窗口返回时,应返回这里设置的活动窗口
		if (CDlgManager::GetInstance()->GetLastDlgID() == CVDiskListDlg_ID)
		{
			DLG_INFO* pdi = CDlgManager::GetInstance()->GetDlgInfo(CVDiskPlayerDlg_ID, SOURCE_VDISK);
			CDlgManager::GetInstance()->SetLastDlgInfo(pdi);
		}
	}
}


void CVDiskListDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	switch (media_event)
	{
	case EVT_MSDK_GRAPH_BUILT:
		OnGraphBuilt();
		break;
	case EVT_MSDK_FINISHED:	
// 		if (m_nErrorCode == VIDEO_ERROR_NONE)
// 		{
 			PlayNext();
// 		}
		break;
	case EVT_MSDK_ERROR_OCCURED:
//		OnErrorOccured(param1, param2);
		break;
	case EVT_MSDK_GRAPH_CHANGED:
		break;
	case EVT_MSDK_PLAYBACK_POSITION_STATUS:
		break;
	case EVT_MSDK_PLAYED:
		break;
	case EVT_MSDK_PAUSED:
		break;
	case EVT_MSDK_STOPED:
		break;

	default:
		break;
	}
}


void CVDiskListDlg::OnGraphBuilt()
{
//	KillTimer(TIMER_ID_DELAY_PLAY_NEXT);

	// 处理断点播放
	VDISK_MEMORY* pmemory = &sysutil::nss_get_instance()->vdisk_memory;

	if (pmemory && pmemory->memory_play)
	{
		// 延时一段时间再进播放界面
		pmemory->memory_play = FALSE;
		CMsdkRender::GetInstance()->FLastMemoryStart(pmemory->pos);
	}
}



void CVDiskListDlg::OnDlgShow(BOOL bShow)
{
	DLG_ID did = CDlgManager::GetInstance()->GetLastDlgID();
	if (bShow )
	{
//		SetPlayingDisk(CVDiskFileManager::GetInstance()->GetPlayingDiskID());
		SetTimer(TIMER_ID_RETURN_PLAY_UI, TIME_RETURN_PLAY_UI);
	}
	else
	{
		KillTimer(TIMER_ID_RETURN_PLAY_UI);
	}
}


void CVDiskListDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_RETURN_PLAY_UI)
	{
		if (CVDiskFileManager::GetInstance()->GetPlayingDiskID() >= 0	// 有碟片播放时才进入播放界面
			&& !m_bIsPopup)
		{
			KillTimer(TIMER_ID_RETURN_PLAY_UI);
			ShowPlayingDlg();
		}
	}
	else if (wIDEvent == TIMER_ID_DELAY_PLAY_NEXT)
	{
		KillTimer(TIMER_ID_DELAY_PLAY_NEXT);
		PlayNext();
	}
	else if (wIDEvent == TIMER_ID_DELAY_PLAY_ITEM)
	{
		KillTimer(TIMER_ID_DELAY_PLAY_ITEM);
		int index = CVDiskFileManager::GetInstance()->GetPlayingFileIndex();
		LPCTSTR file_path = CVDiskFileManager::GetInstance()->GetPlayFilePath(index);
		CMsdkRender::GetInstance()->FSetSourceFile(file_path);
	}
	else if (wIDEvent == TIMER_ID_DELAY_ANIMATING)
	{
		KillTimer(TIMER_ID_DELAY_ANIMATING);
		CAnimDiskLayer* player = (CAnimDiskLayer*)GetChildByName(L"anim_disk");
		if (player)
		{
			player->SetRotateAngle(-20.0);
			player->StartRotate();
		}
		SetPlayingDisk(CVDiskFileManager::GetInstance()->GetPlayingDiskID());
	}
	else if (wIDEvent == TIMER_ID_HIDE_MESSAGE_TIP)
	{
		KillTimer(TIMER_ID_HIDE_MESSAGE_TIP);
		CWceUiLayer *player = GetLayerByName(L"message_tip");
		if (player)
		{
			player->ShowLayer(FALSE);
		}
	}
	else if (wIDEvent == TIMER_ID_RIP_START)
	{
		if(CDvdRender::GetInstance()->GetState() != SV_PLAY)
		{
			CDvdRender::GetInstance()->Play();
		}
		else
		{
			KillTimer(TIMER_ID_RIP_START);
			OnRipStart();
		}
	}

	__super::OnTimer(wIDEvent);
}

void CVDiskListDlg::OnRipStart()
{
	// 根据碟片的ID号,查找该碟片是否需从断点重新录制
	int nDiskIndex = 0, nTracks = 0;
	if (GetRipInfo(m_DiskID, nDiskIndex, nTracks))
	{
		m_nRipDisk = nDiskIndex;
		m_nCurRipTrack = nTracks;

		// 该碟已经录制完毕了
		if (nTracks >= CDvdRender::GetInstance()->GetCurFileCount())
		{
			OnRipDiskFinish();
			CWceUiLoadString str(L"VDISK_MSG_HAVE_RECORDED");
			CString msg;
			msg.Format(L"%s %02d", str.GetString(), nDiskIndex+1);
			ShowTipMessage(msg);
			return;
		}

	}


	if (!RipTrack(m_nCurRipTrack))	// 录制失败
	{
		OnRipDiskFinish();
	}
	else
	{
		CWceUiLoadString str(L"VDISK_MSG_START_RECORD");
		ShowTipMessage(str.GetString());
	}

}

void CVDiskListDlg::OnRipInterrupt()
{
	if (sysutil::nss_get_instance()->vdisk_is_ripping)
	{
		OnRipDiskFinish();
		CWceUiLoadString str(L"VDISK_MSG_STOP_RECORD");
		ShowTipMessage(str.GetString());
	}
}

LRESULT CVDiskListDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
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



void CVDiskListDlg::PlayNext()
{
 	int index = CVDiskFileManager::GetInstance()->PlayFileNext();
 	LPCTSTR file_path = CVDiskFileManager::GetInstance()->GetPlayFilePath(index);
 	CMsdkRender::GetInstance()->FSetSourceFile(file_path);
}


void CVDiskListDlg::OnAnimateBegin()
{
	ShowStaticLayers(FALSE);
	__super::OnAnimateBegin();
}

void CVDiskListDlg::OnAnimateEnd()
{
	ShowStaticLayers(TRUE);
	__super::OnAnimateEnd();
}

void CVDiskListDlg::ShowStaticLayers(BOOL bShow)
{
	if (bShow)
	{
		SetPlayingDisk(CVDiskFileManager::GetInstance()->GetPlayingDiskID());
	}
	else
	{
		SetPlayingDisk(-1);	// 隐藏
	}
}

void CVDiskListDlg::OnDvpMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	if (!sysutil::nss_get_instance()->vdisk_is_ripping)	// 如果没有RIP,忽略DVP的消息,因为有可能后排在播放DVD
	{
		return;
	}

	RETAILMSG(TRUE, (L"[navi_car]: media event = %x \n",media_event));	

	switch (media_event)
	{
	case EVT_DVP_DISC_INSERT:
		{
			CWceUiLoadString str(L"VDISK_MSG_LOADING");
			ShowTipMessage(str.GetString());
		}
		break;
	case EVT_DVP_DISC_ID:
		{
			DVP_GetDiscID(m_DiskID, sizeof(m_DiskID));
			//RETAILMSG(TRUE, (L"[navi_car]: EVT_DVP_DISC_ID %x%x%x%x %02x%02x%02x%02x \n",uData[0],uData[1],uData[2],uData[3],uData[4],uData[5],uData[6],uData[7]));	
		}		
		break;
	case EVT_DVP_INIT_STATE:
		if (param1 == SV_NO_DISC || param1 == SV_UNKNOWN_DISC)
		{
			OnDiskError();// 碟片错误，无法rip
		}
		break;
	case EVT_DVP_DISC_TYPE:
		if (param1 != SV_CDDA)		// must be a valid disk type
		{
			OnDiskError();// 碟片错误，无法rip
		}
		else
		{
			SetTimer(TIMER_ID_RIP_START, 200);	// 识别TYPE后,Play成功后才能录制
		}
		break;
	case EVT_DVP_RIP_CUR_TRACK_OK:
		OnRipTrackFinish();
		break;
	case EVT_CD_RIP_NOT_ENOUGH_SPACE:	
		{
			RETAILMSG(TRUE, (L"[navi_car]: EVT_CD_RIP_NOT_ENOUGH_SPACE ****************** \n") );
			OnRipInterrupt();
			CWceUiLoadString str(L"VDISK_MSG_NO_ENOUGH_SPACE");
			ShowTipMessage(str.GetString());
		}
		break;	
	case EVT_DVP_RIP_ERROR:	//正常RIP时也有这个event触发，忽略该错误
		break;
	case EVT_DVP_RIP_TRK_LBA_LEN:		
		break;
	case EVT_DVP_RIP_PROGRESS:
		//RETAILMSG(TRUE, (L"[navi_car]: rip %d \n", param1) );
		ShowRipProgress(param1);
		break;
	default:
		break;
	}
}

void CVDiskListDlg::OnDiskError()
{
	CWceUiLoadString str(L"VDISK_MSG_DISK_ERROR");
	ShowTipMessage(str.GetString());

	OnRipDiskFinish();
}

void CVDiskListDlg::ShowTipMessage(LPCTSTR message)
{
	CWceUiLayer *player = GetLayerByName(L"message_tip");
	if (player)
	{
		player->SetText(message);
		player->ShowLayer(TRUE);
		SetTimer(TIMER_ID_HIDE_MESSAGE_TIP, 2000);
	}
}


BOOL CVDiskListDlg::RipTrack(int nTrack)
{
	if (nTrack >= CDvdRender::GetInstance()->GetCurFileCount())
	{
		return FALSE;
	}

	UINT len = CDvdRender::GetInstance()->GetRipTrkLen(nTrack);
	CDvdRender::GetInstance()->SetRipPath(GetRipFileName(nTrack).GetBuffer(), len);
	UINT  ret = CDvdRender::GetInstance()->StartRip(nTrack);
	switch (ret)
	{
	case ERROR_CD_RIP_NOT_ENOUGH_SPACE:
		{
			RETAILMSG(TRUE, (L"[navi_car]: No enought space to rip. \n") );
			CWceUiLoadString str(L"VDISK_MSG_NO_ENOUGH_SPACE");
			ShowTipMessage(str.GetString());
			return FALSE;
		}
	case ERROR_CD_RIP_PATH_NOT_EXIT:
	case ERROR_CD_RIP_USBSD_WRITE_PROTECT:	
	case ERROR_CD_RIP_DEVICE_ATTACHED_NOT_FUNCTION:		
	case ERROR_CD_RIP_CREATE_FILE_FAIL:
		{
			RETAILMSG(TRUE, (L"[navi_car]: Create rip file fail. \n") );
			CWceUiLoadString str(L"VDISK_MSG_CREATE_FILE_FAIL");
			ShowTipMessage(str.GetString());
			return FALSE;
		}
	default:
		break;
	}

	return TRUE;
}

CString CVDiskListDlg::GetRipFileName(int nTrack)
{
	CString str;
	str.Format(L"%s\\track%02d.mp3", CVDiskFileManager::GetInstance()->GetVDiskPath(m_nRipDisk), nTrack+1);
	return str;
}


BOOL CVDiskListDlg::GetRipInfo(const BYTE *pDiskID, OUT int& nDiskIndex, OUT int& nTracks)
{
	for (int i=0; i<VDISK_MAX_DISK_COUNT; i++)
	{
		CString str;
		str.Format(L"%s\\%s", CVDiskFileManager::GetInstance()->GetVDiskPath(i), RIP_INFO_FILE_NAME);
		FILE* fp = _tfopen(str, L"rb");
		if (fp)
		{
			BYTE buffer[9] = {0,};
			if (9 == fread(buffer, 1, 9, fp)
				&& memcmp(buffer, m_DiskID, sizeof(m_DiskID)) == 0)
			{
				nDiskIndex = i;
				nTracks = buffer[8];
				fclose(fp);
				return TRUE;
			}
			fclose(fp);
		}
	}

	return FALSE;
}


BOOL CVDiskListDlg::SaveRipInfo(int nDiskIndex, int nTracks)
{
	CString str;
	str.Format(L"%s\\%s", CVDiskFileManager::GetInstance()->GetVDiskPath(nDiskIndex), RIP_INFO_FILE_NAME);
	FILE* fp = _tfopen(str, L"wb");
	if (fp)
	{
		BYTE buffer[9];
		memcpy(buffer, m_DiskID, 8);
		buffer[8] = (BYTE)nTracks;
		fwrite(buffer, 9, 1, fp);
		fclose(fp);
		return TRUE;
	}

	return FALSE;
}

void CVDiskListDlg::OnRipTrackFinish()
{
	m_nCurRipTrack++;
	if (m_nCurRipTrack < CDvdRender::GetInstance()->GetCurFileCount())
	{
		RipTrack(m_nCurRipTrack);
	}
	else	// 全部录制完成
	{
		OnRipDiskFinish();
		CWceUiLoadString str(L"VDISK_MSG_RECORD_FINISH");
		ShowTipMessage(str.GetString());
	}

}

void CVDiskListDlg::OnRipDiskFinish()
{
	//... 将文字恢复, record按钮状态
	TCHAR layer_name[16];
	_stprintf(layer_name, L"disk%d", m_nRipDisk);
	CWceUiButton* player = (CWceUiButton*)GetChildByName(layer_name);
	if (player)
	{
		player->GetStyle()->SetTextColorFromApp(RGB(255,255,255));
		player->GetCommonStyle()->SetTextColorFromApp(RGB(255,255,255));
	}

	player = (CWceUiButton*)GetChildByName(L"record");
	if (player)
	{
		player->SetStatusStyleName(L"record_normal", CWceUiButton::STATUS_NORMAL);
		player->SetStatusStyleName(L"record_down", CWceUiButton::STATUS_DOWN);
	}

	//... 停止录制
	CDvdRender::GetInstance()->StopRip();
	if (sysutil::nss_get_instance()->vdisk_sink == DVP_SINK_FRONT_REAR)
	{
		// 如果之前是前后排播放,录制停止后，切换回前后排模式
		CSourceManager::GetInstance()->SetRearSrc(SOURCE_VDISK);
	}
	else
	{
		CSourceManager::GetInstance()->SetRearSrc(INVALID_SOURCE_ID);
	}
	CMainWnd::GetInstance()->OnDVDLoading(FALSE);

	//... 删除未录制完成文件并保存录制信息
	DeleteFile(GetRipFileName(m_nCurRipTrack));		// 如果正常录制完成了,该变量会+1,删除时，该文件应该是不存在的
	if (m_nCurRipTrack > 0)		// 至少成功录制了一首才保存
	{
		SaveRipInfo(m_nRipDisk, m_nCurRipTrack);
	}

	// 注意这二个变量的位置,有些功能依赖这二个变量,所以位置不能随便移动
	sysutil::nss_get_instance()->vdisk_is_ripping = FALSE;
	m_nRipDisk = -1;


	//... 有新碟片加入,更新文件管理,如果之前没有播放,重新开始播放
	int nPlayingDiskID = CVDiskFileManager::GetInstance()->GetPlayingDiskID();
	int nPlayingFileIndex = CVDiskFileManager::GetInstance()->GetPlayingFileIndex();

	BOOL bNeedReplay = FALSE;
	if (nPlayingDiskID < 0)		// 之前没有播放
	{
		bNeedReplay = TRUE;
	}

	CVDiskFileManager::GetInstance()->InitLoadData();	// 删除目录后，重新初始化数据
	if (nPlayingDiskID < 0)		// 之前没有播放
	{
		nPlayingDiskID = CVDiskFileManager::GetInstance()->FirstDiskAvailable();
		nPlayingFileIndex = 0;
		CVDiskFileManager::GetInstance()->SetPlayFileData(nPlayingDiskID, nPlayingFileIndex);
		OnTimer(TIMER_ID_DELAY_PLAY_ITEM);	// 播放该文件
		SetPlayingDisk(nPlayingDiskID);
	}
	else
	{
		CVDiskFileManager::GetInstance()->SetPlayFileData(nPlayingDiskID, nPlayingFileIndex);
	}

	//... 有新录制的碟片了,重新初始化碟片状态
	InitActiveDisk();
}

void CVDiskListDlg::ShowRipProgress(int nPercent)
{
	int file_count = CDvdRender::GetInstance()->GetCurFileCount();
	int progress = 0;
	if (file_count>0)
	{
		progress = ((m_nCurRipTrack * 100 + nPercent +(file_count/2))/ file_count);
		TCHAR layer_name[16];
		TCHAR layer_text[8];
		_stprintf(layer_name, L"disk%d", m_nRipDisk);
		_stprintf(layer_text, L"%d%%", progress);
		CWceUiButton* player = (CWceUiButton*)GetChildByName(layer_name);
		if (player)
		{
			player->GetStyle()->SetTextColorFromApp(RGB(255,0,0));
			player->GetCommonStyle()->SetTextColorFromApp(RGB(255,0,0));
			player->GetCommonStyle()->SetUseTextColor(TRUE);

			player->SetText(layer_text);
		}
	}
	
}