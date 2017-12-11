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
		return;	// �Ѿ���ָ����Դ��
	}

	m_srcLastFront = GetFrontSrcID();


	// step1: ����֮ǰ��ǰ��Դ
	if (front_src)
	{
		if (front_src->sink == DVP_SINK_FRONT)	// ��֮ǰ��ǰ��Դͣ��
		{
			// ֹͣԴʱҲmute��MCU,��ֹPOP
			CVolumeManager::GetInstance()->SetMcuMuteFlag(MUTE_MCU_MASK_SRC_LOADING, TRUE);
			CMainWnd::GetInstance()->SetTimer(TIMER_ID_SOURCE_LONDING_MUTE, 500, NULL);

			StopSrc(front_src->sid);
			front_src->sid = INVALID_SOURCE_ID;
			front_src->sink = DVP_SINK_NONE;
		}
		else if (front_src->sink == DVP_SINK_FRONT_REAR)	// ��ǰ����תΪ����
		{
			SetSrcSink(front_src->sid, DVP_SINK_REAR);
			front_src->sink = DVP_SINK_REAR;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	// step2: �������Դ�պ���Ҫ������ǰ��Դ����ֱ�ӰѺ���Դ��Ϊǰ����ģʽ
	SOURCE_INFO* rear_src = GetRearSrc();
	if (rear_src && rear_src->sid == sid)
	{
		SetSrcSink(sid, DVP_SINK_FRONT_REAR);
		rear_src->sink = DVP_SINK_FRONT_REAR;
	}
	//step3: ��Ҫ����ǰ��Դ
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
			// �д����ˣ��Ҳ���һ���յı���
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
	if ((rear_src && rear_src->sid == sid))		// �Ѿ���ָ����Դ��
	{
		return;	// �Ѿ���ָ����Դ��
	}

	// step1: ����֮ǰ�ĺ���Դ
	if (rear_src)
	{
		if (rear_src->sink == DVP_SINK_REAR || rear_src->sink == DVP_SINK_FRONTV_REAR )	// ��֮ǰ��ǰ��Դͣ��
		{
			StopSrc(rear_src->sid);
			rear_src->sid = INVALID_SOURCE_ID;
			rear_src->sink = DVP_SINK_NONE;
		}
		else if (rear_src->sink == DVP_SINK_FRONT_REAR)	// ��ǰ����תΪǰ��
		{
			SetSrcSink(rear_src->sid, DVP_SINK_FRONT);
			rear_src->sink = DVP_SINK_FRONT;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	// step2: ���ǰ��Դ�պ���Ҫ�����ĺ���Դ����ֱ�Ӱ�ǰ��Դ��Ϊǰ����ģʽ
	SOURCE_INFO* front_src = GetFrontSrc();
	if (front_src && front_src->sid == sid)
	{
		SetSrcSink(sid, DVP_SINK_FRONT_REAR);
		front_src->sink = DVP_SINK_FRONT_REAR;
	}
	//step3: ��Ҫ��������Դ
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
			// �д����ˣ��Ҳ���һ���յı���
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

	// ���ǰ���Ų���ָ����Դ�����Դֹͣ��
	SOURCE_INFO* front_src = GetFrontSrc();
	if (front_src && front_src->sid != sid)
	{
		bSrcLastFrontChanged = TRUE;
		srcLastFront = GetFrontSrcID();		// ����һ��Դ��¼����

		// ֹͣԴʱҲmute��MCU,��ֹPOP
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

	// ��Ϊ����ĺ������û��޸�m_srcLastFront, ����Ѽ�¼����ȷֵ,���¸�ֵ��
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
		;	// Ӧ��VDisk������DVDԴ,���败������Ϣ,��Ϊ����������Դ
	}

}

BOOL CSourceManager::IsDVDSourceByVdisk(SOURCE_ID sid)
{
	// VDisk¼��ʱ���ں�������DVD,����ʱDVD������һ��������Դ,����ͨ���ж�DVD UI�Ƿ����������
	if (sid == SOURCE_DVD)
	{
		DLG_INFO* pdi =CDlgManager::GetInstance()->GetActiveDlgInfo(sid);
		if (pdi && pdi->pdlg && pdi->is_created)
		{
			return FALSE;
		}
		else
		{
			return TRUE;;// Ӧ��VDisk������DVDԴ
		}
	}
	else
	{
		return FALSE;
	}
}

void CSourceManager::LaunchSrc(SOURCE_ID sid, DVP_SINK sink)
{
	// ֻ���ǰ��Դ��MUTE����
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
	else if (sid == SOURCE_BT)	// ָBT��A2DP,��绰�Ȳ�����BTԴ
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
		return;	// �����������ʾ
	}
	ui_led_set_showdata(data_show);
	//CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
}

void CSourceManager::SetRearSrcSink(DVP_SINK sink)
{
	// �Ժ���ֻ���������������
	if(sink != DVP_SINK_REAR && sink != DVP_SINK_FRONTV_REAR)
	{
		RETAILMSG(1, (L"[navi car] SetRearSrcSink() error, invalid parameters.\n"));
		return;
	}

	// ����û�кϷ���Դ ���� ǰ������ͬһ��Դ��Ҳ�������κζ���
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

	// ��ʼ�����������
	VDISK_MEMORY* pmemory = &sysutil::nss_get_instance()->vdisk_memory;
	pmemory->memory_play = TRUE;	// �ս�Դ���ô˱�־

	// ���к��ж�

	int disk_index = pmemory->disk_id;
	int file_index = 0;
	// ��������
	CVDiskFileManager::GetInstance()->InitLoadData();

	// ���û���ҵ��������Ϣ
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
	// ֹͣԴ
	CMsdkRender::GetInstance()->FStopSrc();
}

void CSourceManager::SaveMemory_VDisk()
{
	// ����ϵ�����(�ļ���repeate, random)
	// �����ڸı�ʱ����
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

	// ��ʼ�����������
	MSDK_MEMORY* pmemory = ui_msdk_get_memory(sid);
	pmemory->memory_play = TRUE;	// �ս�Դ���ô˱�־

	// ���к��ж�

	int folder_index = 0;
	int file_index = 0;
	// ��������
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
	// ����ϵ�����(�ļ���repeate, random)
	// �����ڸı�ʱ����
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
	// ֹͣԴ
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
	if ( GetFrontSrcID() != SOURCE_FM )		// FM��֧�ֺ���,Ҳ���ܻ�������Դ��֧��
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
	if (m_playback_hangup_flag == 0)	// Ҫ���𲥷���,��¼��ԭʼ״̬
	{
		m_playback_status = GetPlaybackStatus();
		if (m_playback_status == 1)		// ������ڲ���,��ͣ����
		{
			PlaybackPause();	
		}
	}
	m_playback_hangup_flag |= maskEvent;
}

void CSourceManager::RestorePlayback(PLAYBACK_MASK maskEvent)
{
	m_playback_hangup_flag &= (~maskEvent);
	if (m_playback_hangup_flag == 0 && m_playback_status == 1)		// ����hangup��,����ԭʼ״̬�ǲ���,�ָ�����
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
	else if (sid == SOURCE_BT || sid == SOURCE_PHONE_LINK || sid == SOURCE_EASY_CONNECTED)	// ָBT��A2DP,��绰�Ȳ�����BTԴ
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

	// ����Դ���ɿ��Ʋ���״̬,ֱ�ӷ���0
	return 0;
}

void CSourceManager::PlaybackPlay()
{
	SOURCE_ID sid = GetFrontSrcID();
	if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_VDISK)
	{
		CMsdkRender::GetInstance()->FPlay();
	}
	else if (sid == SOURCE_BT)	// ָBT��A2DP,��绰�Ȳ�����BTԴ
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

	// ����Դ���ɿ��Ʋ���״̬,ֱ�ӷ���0
	return;
}

void CSourceManager::PlaybackPause()
{
	SOURCE_ID sid = GetFrontSrcID();
	if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_VDISK)
	{
		CMsdkRender::GetInstance()->FPause();
	}
	else if (sid == SOURCE_BT)	// ָBT��A2DP,��绰�Ȳ�����BTԴ
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

	// ����Դ���ɿ��Ʋ���״̬,ֱ�ӷ���
	return;
}


void CSourceManager::CloseAudio()
{
	SOURCE_ID sid = GetFrontSrcID();
	if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_VDISK)
	{
		CMsdkRender::GetInstance()->FCloseAudio();
	}
	else if (sid == SOURCE_BT)	// ָBT��A2DP,��绰�Ȳ�����BTԴ
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

	// ����Դ���ɿ��Ʋ���״̬,ֱ�ӷ���
	return;
}

void CSourceManager::OpenAudio()
{
	SOURCE_ID sid = GetFrontSrcID();
	if (sid == SOURCE_SD || sid == SOURCE_USB || sid == SOURCE_USB_REAR || sid == SOURCE_VDISK)
	{
		CMsdkRender::GetInstance()->FOpenAudio();
	}
	else if (sid == SOURCE_BT)	// ָBT��A2DP,��绰�Ȳ�����BTԴ
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

	// ����Դ���ɿ��Ʋ���״̬,ֱ�ӷ���
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
