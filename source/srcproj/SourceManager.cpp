#include "StdAfx.h"
#include "SourceManager.h"
#include "msdkrender.h"
#include "sysUtil.h"
#include "msdkFileManager.h"
#include "vdiskFileManager.h"
#include "config.h"
#include "bluetooth.h"
#include "dvdrender.h"
//#include "IPODManager.h"
#include "MainWnd.h"
#include "interface.h"
#include "DlgManager.h"
#include "phonelink.h"
#include "easyconnected.h"
#include "VolumeManager.h"

CSourceManager::CSourceManager(void)
: m_srcLastFront(INVALID_SOURCE_ID)
{
	// set sid to INVALID_SOURCE_ID, set sink to DVP_SINK_NONE
	memset(&m_src1, 0, sizeof(SOURCE_INFO));
	memset(&m_src2, 0, sizeof(SOURCE_INFO));
}

CSourceManager::~CSourceManager(void)
{
}

CSourceManager* CSourceManager::GetInstance()
{
	static CSourceManager src_manager;
	return &src_manager;
}

BOOL CSourceManager::IsValidSrc(SOURCE_ID)
{
	return TRUE;
}

void CSourceManager::SetFrontSrc(SOURCE_ID sid)
{
	sysutil::nss_get_instance()->ui_front_source_id = sid;

	SOURCE_INFO* front_src = GetFrontSrc();
	if (front_src && front_src->sid == sid)
	{
		return;	// 已经是指定的源了
	}

	m_srcLastFront = GetFrontSrcID();


	// step1: 处理之前的前排源
	if (front_src)
	{
		if (front_src->sink == DVP_SINK_FRONT)	// 将之前的前排源停掉
		{
			// 停止源时也mute下MCU,防止POP
			CVolumeManager::GetInstance()->SetMcuMuteFlag(MUTE_MCU_MASK_SRC_LOADING, TRUE);
			CMainWnd::GetInstance()->SetTimer(TIMER_ID_SOURCE_LONDING_MUTE, 500, NULL);

			StopSrc(front_src->sid);
			front_src->sid = INVALID_SOURCE_ID;
			front_src->sink = DVP_SINK_NONE;
		}
		else if (front_src->sink == DVP_SINK_FRONT_REAR)	// 由前后排转为后排
		{
			SetSrcSink(front_src->sid, DVP_SINK_REAR);
			front_src->sink = DVP_SINK_REAR;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	// step2: 如果后排源刚好是要启动的前排源，则直接把后排源切为前后排模式
	SOURCE_INFO* rear_src = GetRearSrc();
	if (rear_src && rear_src->sid == sid)
	{
		SetSrcSink(sid, DVP_SINK_FRONT_REAR);
		rear_src->sink = DVP_SINK_FRONT_REAR;
	}
	//step3: 需要启动前排源
	else	
	{
		front_src = NULL;
		if (m_src1.sid == INVALID_SOURCE_ID)
		{
			front_src = &m_src1;
		}
		else if (m_src2.sid == INVALID_SOURCE_ID)
		{
			front_src = &m_src2;
		}
		else
		{
			// 有错误了，找不到一个空的变量
			RETAILMSG(1, (L"[nvai_car]: set front src fail. Invalid status.\n"));
			return;
		}

		LaunchSrc(sid, DVP_SINK_FRONT);
		front_src->sid = sid;
		front_src->sink = DVP_SINK_FRONT;
	}

}

void CSourceManager::SetRearSrc(SOURCE_ID sid)
{
	sysutil::nss_get_instance()->ui_rear_source_id = sid;

	SOURCE_INFO* rear_src = GetRearSrc();
	if ((rear_src && rear_src->sid == sid))		// 已经是指定的源了
	{
		return;	// 已经是指定的源了
	}

	// step1: 处理之前的后排源
	if (rear_src)
	{
		if (rear_src->sink == DVP_SINK_REAR || rear_src->sink == DVP_SINK_FRONTV_REAR )	// 将之前的前排源停掉
		{
			StopSrc(rear_src->sid);
			rear_src->sid = INVALID_SOURCE_ID;
			rear_src->sink = DVP_SINK_NONE;
		}
		else if (rear_src->sink == DVP_SINK_FRONT_REAR)	// 由前后排转为前排
		{
			SetSrcSink(rear_src->sid, DVP_SINK_FRONT);
			rear_src->sink = DVP_SINK_FRONT;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	// step2: 如果前排源刚好是要启动的后排源，则直接把前排源切为前后排模式
	SOURCE_INFO* front_src = GetFrontSrc();
	if (front_src && front_src->sid == sid)
	{
		SetSrcSink(sid, DVP_SINK_FRONT_REAR);
		front_src->sink = DVP_SINK_FRONT_REAR;
	}
	//step3: 需要启动后排源
	else	
	{
		rear_src = NULL;
		if (m_src1.sid == INVALID_SOURCE_ID)
		{
			rear_src = &m_src1;
		}
		else if (m_src2.sid == INVALID_SOURCE_ID)
		{
			rear_src = &m_src2;
		}
		else
		{
			// 有错误了，找不到一个空的变量
			RETAILMSG(1, (L"[nvai_car]: set rear src fail. Invalid status.\n"));
			return;
		}

		LaunchSrc(sid, DVP_SINK_REAR);
		rear_src->sid = sid;
		rear_src->sink = DVP_SINK_REAR;
	}
}

void CSourceManager::SetFrontRearSrc(SOURCE_ID sid)
{
	SOURCE_ID srcLastFront = INVALID_SOURCE_ID;
	BOOL bSrcLastFrontChanged = FALSE;

	// 如果前后排不是指定的源，则把源停止掉
	SOURCE_INFO* front_src = GetFrontSrc();
	if (front_src && front_src->sid != sid)
	{
		bSrcLastFrontChanged = TRUE;
		srcLastFront = GetFrontSrcID();		// 把上一个源记录下来

		// 停止源时也mute下MCU,防止POP
		CVolumeManager::GetInstance()->SetMcuMuteFlag(MUTE_MCU_MASK_SRC_LOADING, TRUE);
		CMainWnd::GetInstance()->SetTimer(TIMER_ID_SOURCE_LONDING_MUTE, 500, NULL);

		StopSrc(front_src->sid);
		front_src->sid = INVALID_SOURCE_ID;
		front_src->sink = DVP_SINK_NONE;
	}

	SOURCE_INFO* rear_src = GetRearSrc();
	if (rear_src && rear_src->sid != sid)
	{
		StopSrc(rear_src->sid);
		rear_src->sid = INVALID_SOURCE_ID;
		rear_src->sink = DVP_SINK_NONE;
	}


	SetFrontSrc(sid);
	SetRearSrc(sid);

	// 因为上面的函数调用会修改m_srcLastFront, 这里把记录的正确值,重新赋值下
	if (bSrcLastFrontChanged)
	{
		m_srcLastFront = srcLastFront;
	}
}

SOURCE_INFO* CSourceManager::GetFrontSrc()
{
	if (m_src1.sid != INVALID_SOURCE_ID
		&& (m_src1.sink == DVP_SINK_FRONT || m_src1.sink == DVP_SINK_FRONT_REAR))
	{
		return &m_src1;
	}
	else if (m_src2.sid != INVALID_SOURCE_ID
		&& (m_src2.sink == DVP_SINK_FRONT || m_src2.sink == DVP_SINK_FRONT_REAR))
	{
		return &m_src2;
	}

	return NULL;
}

SOURCE_INFO* CSourceManager::GetRearSrc()
{
	if (m_src1.sid != INVALID_SOURCE_ID
		&& (m_src1.sink == DVP_SINK_REAR || m_src1.sink == DVP_SINK_FRONT_REAR 
		    || m_src2.sink == DVP_SINK_FRONTV_REAR))
	{
		return &m_src1;
	}
	else if (m_src2.sid != INVALID_SOURCE_ID
		&& (m_src2.sink == DVP_SINK_REAR || m_src2.sink == DVP_SINK_FRONT_REAR 
		    || m_src2.sink == DVP_SINK_FRONTV_REAR))
	{
		return &m_src2;
	}

	return NULL;
}


SOURCE_ID CSourceManager::GetFrontSrcID()
{
	SOURCE_INFO* pinfo = GetFrontSrc();

	if (pinfo)
	{
		return pinfo->sid;
	}

	return INVALID_SOURCE_ID;
}

SOURCE_ID CSourceManager::GetRearSrcID()
{
	SOURCE_INFO* pinfo = GetRearSrc();

	if (pinfo)
	{
		return pinfo->sid;
	}

	return INVALID_SOURCE_ID;
}

void CSourceManager::StopSrc(SOURCE_ID sid)
{
	if (sid == INVALID_SOURCE_ID)
	{
		return;
	}
	if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR)
	{
		StopSrc_MSDK(sid);
	}
	else if (sid == SOURCE_VDISK)
	{
		StopSrc_VDisk();
	}
	else if (sid == SOURCE_BT)
	{
		CBluetooth::GetInstance()->StopSrc();
	}
	else if (sid == SOURCE_FM)
	{
		CMsdkRender::GetInstance()->AStopFMSrc();
	}
	else if (sid == SOURCE_AVIN1)
	{
		CMsdkRender::GetInstance()->AStopAvin1Src();
	}
	else if (sid == SOURCE_AVIN2)
	{
		CMsdkRender::GetInstance()->AStopAvin2Src();
	}
	else if (sid == SOURCE_TV)
	{
		CMsdkRender::GetInstance()->AStopTVSrc();
	}
	else if (sid == SOURCE_DVD)
	{
		CDvdRender::GetInstance()->StopSrc();
	}
	/* --++ ipod
	else if (sid == SOURCE_IPOD_USB)
	{
		CIPODManager::GetInstance()->StopSrc();
	}
	*/
	else if (sid == SOURCE_PHONE_LINK)
	{
		CPhoneLink::GetInstance()->StopSrc();
	}
	else if (sid == SOURCE_EASY_CONNECTED)
	{
		CEasyConnected::GetInstance()->StopSrc();
	}


	if (!IsDVDSourceByVdisk(sid))
	{
		CMainWnd::GetInstance()->PostMessage(UI_MSG_STOP_SOURCE_NOTIFY, 0, (LPARAM)sid);
	}
	else	
	{
		;	// 应该VDisk启动了DVD源,不需触发该消息,因为不是真正的源
	}

}

BOOL CSourceManager::IsDVDSourceByVdisk(SOURCE_ID sid)
{
	// VDisk录制时会在后排启动DVD,但此时DVD并不是一个真正的源,这里通过判断DVD UI是否存在来区分
	if (sid == SOURCE_DVD)
	{
		DLG_INFO* pdi =CDlgManager::GetInstance()->GetActiveDlgInfo(sid);
		if (pdi && pdi->pdlg && pdi->is_created)
		{
			return FALSE;
		}
		else
		{
			return TRUE;;// 应该VDisk启动了DVD源
		}
	}
	else
	{
		return FALSE;
	}
}

void CSourceManager::LaunchSrc(SOURCE_ID sid, DVP_SINK sink)
{
	// 只针对前排源作MUTE动作
	if (sink == DVP_SINK_FRONT || sink == DVP_SINK_FRONT_REAR)
	{
		if (sid != INVALID_SOURCE_ID)
		{
			CVolumeManager::GetInstance()->SetMcuMuteFlag(MUTE_MCU_MASK_SRC_LOADING, TRUE);
			CMainWnd::GetInstance()->SetTimer(TIMER_ID_SOURCE_LONDING_MUTE, 500, NULL);
		}
	}

	if (sid == INVALID_SOURCE_ID)
	{
		;
	}
	else if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR)
	{
		LaunchSrc_MSDK(sid, sink);
	}
	else if (sid == SOURCE_VDISK)
	{
		LaunchSrc_VDisk(sink);
	}
	else if (sid == SOURCE_FM)
	{
		CMsdkRender::GetInstance()->ALaunchFMSrc();
	}
	else if (sid == SOURCE_BT)	// 指BT的A2DP,打电话等不属于BT源
	{
		CBluetooth::GetInstance()->LaunchSrc();
	}
	else if (sid == SOURCE_AVIN1)
	{
		CMsdkRender::GetInstance()->ALaunchAvin1Src(sink);
	}
	else if (sid == SOURCE_AVIN2)
	{
		CMsdkRender::GetInstance()->ALaunchAvin2Src(sink);
	}
	else if (sid == SOURCE_TV)
	{
		CMsdkRender::GetInstance()->ALaunchTVSrc(sink);
	}
	else if (sid == SOURCE_DVD)
	{
		static BOOL isFirstLoad = TRUE;
		if (isFirstLoad)
		{
			isFirstLoad = FALSE;
			CDvdRender::GetInstance()->LaunchSrc(sink);
			CDvdRender::GetInstance()->StopSrc();
			Sleep(100);
		}
		CDvdRender::GetInstance()->LaunchSrc(sink);
	}
	/* --++ ipod
	else if (sid == SOURCE_IPOD_USB)
	{
		CIPODManager::GetInstance()->LaunchSrc(sink);
	}
	*/
	else if (sid == SOURCE_PHONE_LINK)
	{
		CPhoneLink::GetInstance()->LaunchSrc();
	}
	else if (sid == SOURCE_EASY_CONNECTED)
	{
		CEasyConnected::GetInstance()->LaunchSrc();
	}

	SOURCE_ID sid_front = GetFrontSrcID();
	SOURCE_ID sid_rear = GetLastRearSrcID();
	if (sink == DVP_SINK_FRONT_REAR)
	{
		sid_front = sid_rear = sid;
	}
	else if (sink == DVP_SINK_FRONT)
	{
		sid_front = sid;
	}
	else if (sink == DVP_SINK_REAR)
	{
		sid_rear = sid;
	}

	CRpcMCU::GetInstance()->RPC_SetSource(sid_front, sid_rear);
	CMainWnd::GetInstance()->PostMessage(UI_MSG_LOAD_SOURCE_NOTIFY, (WPARAM)sink, (LPARAM)sid);


	// refresh led
	LPCTSTR data_show = L"         ";
	if (sid == SOURCE_AVIN1)
	{
		data_show = L"   AU X  ";
	}
	else if (sid == SOURCE_AVIN2)
	{
		data_show = L"  AVI N  ";
	}
	else if (sid == SOURCE_PHONE_LINK)
	{
		data_show = L" P-LI NK ";
	}
	else if (sid == SOURCE_EASY_CONNECTED)
	{
		data_show = L" P-LI NK ";
	}
	else if (sid == SOURCE_BT)
	{
		data_show = L"  A2D P  ";
	}
	else if (sid == SOURCE_DVD)
	{
		data_show = L"LOADI NG ";
	}
	else 
	{
		return;	// 其它情况不显示
	}
	ui_led_set_showdata(data_show);
	//CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
}

void CSourceManager::SetRearSrcSink(DVP_SINK sink)
{
	// 对后排只允许传递这二个参数
	if(sink != DVP_SINK_REAR && sink != DVP_SINK_FRONTV_REAR)
	{
		RETAILMSG(1, (L"[navi car] SetRearSrcSink() error, invalid parameters.\n"));
		return;
	}

	// 后排没有合法的源 或者 前后排是同一个源，也不需做任何动作
	if (GetRearSrcID() == INVALID_SOURCE_ID || GetFrontSrcID() == GetRearSrcID())	
	{
		return;
	}

	SOURCE_INFO* prear = GetRearSrc();
	if (prear)
	{
		prear->sink = sink;
		SetSrcSink(prear->sid, sink);
	}
}

void CSourceManager::SetSrcSink(SOURCE_ID sid, DVP_SINK sink)
{
	if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR)
	{
		CMsdkRender::GetInstance()->FSetSink(sink);
	}
	else if (sid == SOURCE_VDISK)
	{
		CMsdkRender::GetInstance()->FSetSink(sink);
	}
	else if (sid == SOURCE_AVIN1)
	{
		CMsdkRender::GetInstance()->ASetSinkAvin1(sink);
	}
	else if (sid == SOURCE_AVIN2)
	{
		CMsdkRender::GetInstance()->ASetSinkAvin2(sink);
	}
	else if (sid == SOURCE_TV)
	{
		CMsdkRender::GetInstance()->ASetSinkTV(sink);
	}
	else if (sid == SOURCE_DVD)
	{
		CDvdRender::GetInstance()->SetSink(sink);
	}
	/* --++ ipod
	else if (sid == SOURCE_IPOD_USB)
	{
		CIPODManager::GetInstance()->SetSink(sink);
	}
	*/
}

void CSourceManager::LaunchSrc_VDisk(DVP_SINK sink)
{
	CMsdkRender::GetInstance()->FLaunchSrc(sink);

	// 初始化记忆的数据
	VDISK_MEMORY* pmemory = &sysutil::nss_get_instance()->vdisk_memory;
	pmemory->memory_play = TRUE;	// 刚进源，置此标志

	// 序列号判断

	int disk_index = pmemory->disk_id;
	int file_index = 0;
	// 加载数据
	CVDiskFileManager::GetInstance()->InitLoadData();

	// 如果没有找到记忆的信息
	if (!CVDiskFileManager::GetInstance()->GetFileIndexInfo(pmemory->file_path, disk_index, file_index))
	{
		disk_index = CVDiskFileManager::GetInstance()->FirstDiskAvailable();
		file_index = 0;
	}
 	
	CVDiskFileManager::GetInstance()->SetPlayFileData(disk_index, file_index);
	CVDiskFileManager::GetInstance()->SetPlayRandomMode(pmemory->random );
	CVDiskFileManager::GetInstance()->SetPlayRepeateMode(/*pmemory->repeate*/MSDK_REPEATE_FOLDER);
}

void CSourceManager::StopSrc_VDisk()
{
	SaveMemory_VDisk();
	// 停止源
	CMsdkRender::GetInstance()->FStopSrc();
}

void CSourceManager::SaveMemory_VDisk()
{
	// 保存断点数据(文件，repeate, random)
	// 类型在改变时保存
	if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_VDISK
		|| CSourceManager::GetInstance()->GetLastRearSrcID() == SOURCE_VDISK)
	{
		VDISK_MEMORY* pmemory = &sysutil::nss_get_instance()->vdisk_memory;
		CVDiskFileManager* pfile_manager = CVDiskFileManager::GetInstance();

		int index = pfile_manager->GetPlayingFileIndex();
		LPCTSTR file_path = pfile_manager->GetPlayFilePath(index);
		if (file_path)
		{
			_tcscpy(pmemory->file_path, file_path);
		}

		pmemory->disk_id = pfile_manager->GetPlayingDiskID();
		pmemory->repeate = pfile_manager->GetPlayRepeateMode();
		pmemory->random = pfile_manager->GetPlayRandomMode();
		pmemory->pos = CMsdkRender::GetInstance()->FGetCurPostion();
		pmemory->memory_play = TRUE;
	}
}

void CSourceManager::LaunchSrc_MSDK(SOURCE_ID sid, DVP_SINK sink)
{
	if(sid != SOURCE_SD && sid != SOURCE_USB && sid != SOURCE_USB_REAR)
	{
		RETAILMSG(1, (L"[navi car] LaunchSrc_MSDK() error, invalid parameters.\n"));
		return;
	}
	CMsdkRender::GetInstance()->FLaunchSrc(sink);

	// 初始化记忆的数据
	MSDK_MEMORY* pmemory = ui_msdk_get_memory(sid);
	pmemory->memory_play = TRUE;	// 刚进源，置此标志

	// 序列号判断

	int folder_index = 0;
	int file_index = 0;
	// 加载数据
	CMsdkFileManager::GetInstance()->InitLoadData(ui_msdk_get_disk(sid), pmemory->type);

	CMsdkFileManager::GetInstance()->GetFileIndexInfo(pmemory->file_path, pmemory->type, folder_index, file_index);
	
	CMsdkFileManager::GetInstance()->SetPlayFileData(folder_index, file_index, pmemory->type);
	CMsdkFileManager::GetInstance()->SetPlayRandomMode(pmemory->random, pmemory->type);
	CMsdkFileManager::GetInstance()->SetPlayRepeateMode(pmemory->repeate, pmemory->type);

	// refresh led
	LPCTSTR data_show = L"         ";
	if (pmemory->type == MSDK_FILE_MUSIC)
	{
		data_show = L"  AUD IO ";
	}
	else if (pmemory->type == MSDK_FILE_VIDEO)
	{
		data_show = L"  VID EO ";
	}
	else if (pmemory->type == MSDK_FILE_PHOTO)
	{
		data_show = L"  PHO TO ";
	}
	ui_led_set_showdata(data_show);
	//CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
}

void CSourceManager::SaveMemory_MSDK()
{
	// 保存断点数据(文件，repeate, random)
	// 类型在改变时保存
	SOURCE_ID sid = ui_msdk_get_cursrc();
	if (sid != INVALID_SOURCE_ID)
	{
		MSDK_MEMORY* pmemory = ui_msdk_get_memory(sid);
		CMsdkFileManager* pfile_manager = CMsdkFileManager::GetInstance();

		int index = pfile_manager->GetPlayingFileIndex(pmemory->type);
		LPCTSTR file_path = pfile_manager->GetPlayFilePath(index, pmemory->type);
		if (file_path)
		{
			_tcscpy(pmemory->file_path, file_path);
		}

		pmemory->repeate = pfile_manager->GetPlayRepeateMode(pmemory->type);
		pmemory->random = pfile_manager->GetPlayRandomMode(pmemory->type);
		pmemory->pos = CMsdkRender::GetInstance()->FGetCurPostion();
		pmemory->memory_play = TRUE;
	}
}

void CSourceManager::StopSrc_MSDK(SOURCE_ID sid)
{
	SaveMemory_MSDK();
	// 停止源
	CMsdkRender::GetInstance()->FStopSrc();
}

SOURCE_ID CSourceManager::GetLastFrontSrcID()
{
	if (m_srcLastFront != INVALID_SOURCE_ID)
	{
		return m_srcLastFront;
	}
	else
	{
		return SOURCE_FM;
	}
}

SOURCE_ID CSourceManager::GetLastRearSrcID()
{
	if ( GetFrontSrcID() != SOURCE_FM )		// FM不支持后排,也可能还有其它源不支持
	{
		return GetFrontSrcID();
	}
	else
	{
		return INVALID_SOURCE_ID;
	}
}

void CSourceManager::HangupPlayback(PLAYBACK_MASK maskEvent)
{
	if (m_playback_hangup_flag == 0)	// 要挂起播放了,记录下原始状态
	{
		m_playback_status = GetPlaybackStatus();
		if (m_playback_status == 1)		// 如果正在播放,暂停播放
		{
			PlaybackPause();	
		}
	}
	m_playback_hangup_flag |= maskEvent;
}

void CSourceManager::RestorePlayback(PLAYBACK_MASK maskEvent)
{
	m_playback_hangup_flag &= (~maskEvent);
	if (m_playback_hangup_flag == 0 && m_playback_status == 1)		// 不需hangup了,而且原始状态是播放,恢复播放
	{
		PlaybackPlay();
	}
}

UINT CSourceManager::GetPlaybackStatus()
{
	SOURCE_ID sid = GetFrontSrcID();
	if (sid == INVALID_SOURCE_ID)
	{
		return 0;
	}
	else if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_VDISK)
	{
		return (CMsdkRender::GetInstance()->FGetState() == MSDK_MGSTATE_PLAYING) ? 1 : 0;
	}
	else if (sid == SOURCE_BT || sid == SOURCE_PHONE_LINK || sid == SOURCE_EASY_CONNECTED)	// 指BT的A2DP,打电话等不属于BT源
	{
		return (CBluetooth::GetInstance()->GetMusicPlayStatus() == BT_MUSICPLAY_STATUS_PLAYING) ? 1 : 0;
	}
	else if (sid == SOURCE_DVD)
	{
		return (CDvdRender::GetInstance()->GetState() == SV_PLAY) ? 1 : 0;
	}
	/*
	else if (sid == SOURCE_IPOD_USB)
	{
		MUSB_IPOD_PLAY_ST_INFO_T status;
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnGetPlayStatus(&status);
		return (status.ePbStatus == PB_ST_EX_PLAY_0X01) ? 1 : 0;
	}
	*/
// 	else if (sid == SOURCE_FM || sid == SOURCE_AVIN1 || sid == SOURCE_AVIN2 || sid == SOURCE_TV)
// 	{
// 		return 0;
// 	}

	// 其它源不可控制播放状态,直接返回0
	return 0;
}

void CSourceManager::PlaybackPlay()
{
	SOURCE_ID sid = GetFrontSrcID();
	if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_VDISK)
	{
		CMsdkRender::GetInstance()->FPlay();
	}
	else if (sid == SOURCE_BT)	// 指BT的A2DP,打电话等不属于BT源
	{
		CBluetooth::GetInstance()->MusicPlay();
	}
	else if (sid == SOURCE_PHONE_LINK || sid == SOURCE_EASY_CONNECTED)
	{
		CBluetooth::GetInstance()->MusicPlay();
	}
	else if (sid == SOURCE_DVD)
	{
		CDvdRender::GetInstance()->Play();
	}
	/* --++ ipod
	else if (sid == SOURCE_IPOD_USB)
	{
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PLAY);
	}
	*/

	// 其它源不可控制播放状态,直接返回0
	return;
}

void CSourceManager::PlaybackPause()
{
	SOURCE_ID sid = GetFrontSrcID();
	if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_VDISK)
	{
		CMsdkRender::GetInstance()->FPause();
	}
	else if (sid == SOURCE_BT)	// 指BT的A2DP,打电话等不属于BT源
	{
		CBluetooth::GetInstance()->MusicPause();
	}
	else if (sid == SOURCE_PHONE_LINK || sid == SOURCE_EASY_CONNECTED)
	{
		CBluetooth::GetInstance()->MusicPause();
	}
	else if (sid == SOURCE_DVD)
	{
		CDvdRender::GetInstance()->Pause();
	}
	/* --++ ipod
	else if (sid == SOURCE_IPOD_USB)
	{
		CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnPlayControl(PLAY_CTRL_PAUSE);
	}
	*/

	// 其它源不可控制播放状态,直接返回
	return;
}


void CSourceManager::CloseAudio()
{
	SOURCE_ID sid = GetFrontSrcID();
	if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_VDISK)
	{
		CMsdkRender::GetInstance()->FCloseAudio();
	}
	else if (sid == SOURCE_BT)	// 指BT的A2DP,打电话等不属于BT源
	{
		CBluetooth::GetInstance()->StopSrc();
	}
	else if (sid == SOURCE_PHONE_LINK || sid == SOURCE_EASY_CONNECTED)
	{
		CBluetooth::GetInstance()->StopSrc();
	}
	else if (sid == SOURCE_DVD)
	{
		CDvdRender::GetInstance()->CloseAudio();
	}
	/* --++
	else if (sid == SOURCE_IPOD_USB)
	{
		CIPODManager::GetInstance()->ReleaseAudioDsp();
	}
	*/
	else if (sid == SOURCE_AVIN1)
	{
		CMsdkRender::GetInstance()->ACloseAudioAvin1();
	}
	else if (sid == SOURCE_AVIN2)
	{
		CMsdkRender::GetInstance()->ACloseAudioAvin2();
	}
	else if (sid == SOURCE_TV)
	{
		CMsdkRender::GetInstance()->ACloseAudioTV();
	}

	// 其它源不可控制播放状态,直接返回
	return;
}

void CSourceManager::OpenAudio()
{
	SOURCE_ID sid = GetFrontSrcID();
	if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_VDISK)
	{
		CMsdkRender::GetInstance()->FOpenAudio();
	}
	else if (sid == SOURCE_BT)	// 指BT的A2DP,打电话等不属于BT源
	{
		CBluetooth::GetInstance()->LaunchSrc();
	}
	else if (sid == SOURCE_PHONE_LINK || SOURCE_EASY_CONNECTED)
	{
		CBluetooth::GetInstance()->LaunchSrc();
	}
	else if (sid == SOURCE_DVD)
	{
		CDvdRender::GetInstance()->OpenAudio();
	}
	/* --++ ipod
	else if (sid == SOURCE_IPOD_USB)
	{
		CIPODManager::GetInstance()->GetAudioDsp();
	}
	*/
	else if (sid == SOURCE_AVIN1)
	{
		CMsdkRender::GetInstance()->AOpenAudioAvin1();
	}
	else if (sid == SOURCE_AVIN2)
	{
		CMsdkRender::GetInstance()->AOpenAudioAvin2();
	}
	else if (sid == SOURCE_TV)
	{
		CMsdkRender::GetInstance()->AOpenAudioTV();
	}

	// 其它源不可控制播放状态,直接返回
	return;
}

void CSourceManager::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ID_SOURCE_LONDING_MUTE)
	{
		CMainWnd::GetInstance()->KillTimer(TIMER_ID_SOURCE_LONDING_MUTE);
		CVolumeManager::GetInstance()->SetMcuMuteFlag(MUTE_MCU_MASK_SRC_LOADING, FALSE);
	}
}
