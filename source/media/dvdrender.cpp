#include "StdAfx.h"
#include "dvdrender.h"
#include "interface.h"

#include "ycapi.h"

extern "C" BOOL KernelIoControl(
								DWORD dwIoControlCode,
								LPVOID lpInBuf,
								DWORD nInBufSize,
								LPVOID lpOutBuf,
								DWORD nOutBufSize,
								LPDWORD lpBytesReturned
								);
static DWORD DVP_LM_START_ADDRESS = 0x0071d300;		// 3353, mp5地址为 0x006da300

CDvdRender::CDvdRender(void)
{
	m_hwnd = NULL;
	m_SinkType = DVP_SINK_NONE;
	memset(&m_DVPLMFileInfo, 0, sizeof(DVP_LMFileInfo_T));
	memset(m_eventHandler, 0, sizeof(m_eventHandler));
	m_bIsRiping = FALSE;
	m_bInitialized = FALSE;

	DWORD fgChipFeature = 0;
	KernelIoControl(IOCTL_HAL_GET_FEATUREDISCLESS, NULL,0, &fgChipFeature, sizeof(DWORD), NULL);
	if (fgChipFeature == 0xCB0E)	// 3353
	{
		DVP_LM_START_ADDRESS = 0x0071d300;
	}
	else	// mp5
	{
		DVP_LM_START_ADDRESS = 0x006da300;
	}
}

CDvdRender::~CDvdRender(void)
{


}

CDvdRender* CDvdRender::GetInstance()
{
	static CDvdRender dvd_render;
	return &dvd_render;
}


static SWI_SINK_T _GetSinkType(DVP_SINK sink_type)
{
	if (sink_type == DVP_SINK_FRONT || sink_type == DVP_SINK_REAR || sink_type == DVP_SINK_FRONT_REAR)
	{
		return (SWI_SINK_T)sink_type;
	}
	else if (sink_type == DVP_SINK_FRONTV_REAR)
	{
		return SWI_SINK_REAR;
	}
	else	// 如果是未知类型，直接设为前后排输出
	{
		return SWI_SINK_FRONT_REAR;
	}
}

BOOL CDvdRender::Initialize(HWND hWnd)
{
	ReadDVPLM();
	m_hwnd = hWnd;
	return TRUE;
}

void CDvdRender::Uninitialize()
{

}

void CDvdRender::CreateFileDatabase(GUINT8 uFilterType)
{
	DVP_CreateFileDatabase(uFilterType);
	m_DVPLMFileInfo.DVPLMFileTye = uFilterType;
}

void CDvdRender::SetCodePage(GUINT32 u4CodePage)
{
	DVP_SetCodePage(u4CodePage);
}

BOOL CDvdRender::ReadDVPLM()
{
	DWORD dwBytesRead = 0;
	HANDLE hOpenFile = (HANDLE)CreateFile(DVP_LASTMEMORY_FILE_PATH ,GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE , NULL, OPEN_EXISTING, NULL, NULL); 
	if (hOpenFile == INVALID_HANDLE_VALUE) 
	{ 
		return FALSE;
	} 
	ReadFile(hOpenFile,&m_DVPLMFileInfo, sizeof(DVP_LMFileInfo_T), &dwBytesRead, NULL);	
	CloseHandle(hOpenFile);

	return TRUE;
}

void CDvdRender::SaveDVPLM(BOOL bReset)
{
	if (bReset)
	{
		memset(&m_DVPLMFileInfo, 0, sizeof(DVP_LMFileInfo_T));
	}
	else
	{
		DVP_ReadDataFromDram(DVP_LM_START_ADDRESS, m_DVPLMFileInfo.DVPLMInfo, DVP_LM_SIZE);
	}
}

BOOL CDvdRender::WriteDVPLM2File()
{
	DWORD dwBytesWritten = 0;
	HANDLE hOpenFile = (HANDLE)CreateFile(DVP_LASTMEMORY_FILE_PATH ,GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, NULL, NULL); 
	if (hOpenFile == INVALID_HANDLE_VALUE) 
	{ 
		return FALSE;
	} 

	WriteFile(hOpenFile, &m_DVPLMFileInfo, sizeof(DVP_LMFileInfo_T), &dwBytesWritten, NULL);
	CloseHandle(hOpenFile);

	return TRUE;
}

DVP_LMFileInfo_T* CDvdRender::GetDVPLMFileInfo()
{
	return &m_DVPLMFileInfo;
}

GRESULT CDvdRender::DvpAgentEventListener(GUINT32 u4Evt, GUINT32 u4Param1, GUINT32 u4Param2, GUINT32 u4CustomData)
{
	CDvdRender *pthis = (CDvdRender *)u4CustomData;
	if (pthis)
	{
		pthis->OnMediaEvent(u4Evt, u4Param1, u4Param2);
	}
	return 0;
}


void CDvdRender::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{ 
	for (int i=0; i<DVD_MAX_EVENT_HANDLER_COUNT; i++)
	{
		if (m_eventHandler[i].pfn != NULL)
		{
			(*m_eventHandler[i].pfn)(media_event, param1, param2, m_eventHandler[i].user_data);
		}
	}
}


void CDvdRender::HandleEvent()
{
	DVP_HandleEvent();
}


void CDvdRender::ShowVideo(BOOL bShow, RECT *prect, SWI_SINK_T sink)
{
	SWI_SINK_INFO_T ssi;
	ssi.u4Flag = bShow ? SWI_INFO_SHOW : SWI_INFO_HIDE;
	ssi.eSink = sink;
	if (!prect)
	{
		SetRect(&ssi.DestRect, 0, 0, GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	}
	else
	{
		ssi.DestRect = *prect;
	}
	DVP_SetSurfaceInfo(&ssi);
}

void CDvdRender::SetSink(DVP_SINK sink_type)
{
	SWI_SINK_INFO_T ssi;
	memset(&ssi, 0, sizeof(ssi));
	ssi.eSink = (SWI_SINK_T)sink_type;
	ssi.u4Flag = SWI_INFO_SHOW;

	if (sink_type == DVP_SINK_FRONT 
		|| sink_type == DVP_SINK_REAR
		|| sink_type == DVP_SINK_FRONT_REAR)
	{
		DVP_AVSwitch(&ssi);
	}
	else if (sink_type == DVP_SINK_FRONTV_REAR)
	{
		ssi.eSink = SWI_SINK_REAR;
		DVP_AVSwitch(&ssi);
		ssi.eSink = SWI_SINK_FRONT_REAR;
		DVP_AVSwitchVideo(&ssi);
	}

	m_SinkType = sink_type;
}

void CDvdRender::Next()
{
	DVP_PlayNext();	
}

void CDvdRender::Previous()
{
	DVP_PlayPrev();
}

void CDvdRender::Play()
{
	if (GetState() != SV_PLAY)
	{
		DVP_Play();
	}
}

void CDvdRender::Pause()
{
	if (GetState() != SV_PAUSE)
	{
		DVP_Pause();
	}
}

void CDvdRender::Stop()
{
	if (GetState() != SV_STOP)
	{
		DVP_Stop();
	}
}

void CDvdRender::Seek(DWORD dwPosition)
{
	DVP_SeekToTime(dwPosition);
}

BYTE CDvdRender::GetState()
{
	BYTE  state;
	if (0 == DVP_GetPBState(&state))
	{
		return state;
	}
	else
	{
		return SV_STOP;	// 如果获取状态失败认为是STOP状态
	}
}


DWORD CDvdRender::GetDuration()
{
	GUINT64 duration = 0;
	if (0 == DVP_GetDuration(&duration))
	{
		return (DWORD)(duration/1000);
	}
	else
	{
		return 0;
	}

}

DWORD CDvdRender::GetCurPostion()
{
	GUINT64 postion = 0;
	if (0 == DVP_GetCurrentPosition(&postion))
	{
		return (DWORD)(postion/1000);
	}
	else
	{
		return 0;
	}
}

void CDvdRender::LaunchSrc(DVP_SINK sink_type)
{
	m_bInitialized = TRUE;
	m_SinkType = sink_type;

	DVP_Init();

	DVP_SetNotifyWindow(m_hwnd, UI_MSG_DVP_AGENT);
	DVP_AddListener(CDvdRender::DvpAgentEventListener, (GUINT32)this);

	DVP_Connect(); //connect 8032	
	// read Last memory info
	DVP_WriteData2Dram(DVP_LM_START_ADDRESS, (void*)m_DVPLMFileInfo.DVPLMInfo, DVP_LM_SIZE);

	DVP_ChangeDevice(DEVICE_DISC); //change device to DISC

	GHANDLE hAudioDev = GOpenAudioDev();
	GChooseSpdifOutput(hAudioDev,MISC_AUD_DVD_OUT);
	GCloseAudioDev(hAudioDev);

	SWI_SINK_INFO_T ssi;
	ssi.eSink = _GetSinkType(sink_type);
	SetRect(&ssi.DestRect, 0, 0, GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	//ssi.u4Flag = (::IsWindowVisible(m_hwnd) ? SWI_INFO_SHOW : SWI_INFO_HIDE) |  SWI_INFO_SRCCOLKEY;
	// 初始化时直接HIDE, 碟片类型识别后，再show
	ssi.u4Flag = SWI_INFO_HIDE |  SWI_INFO_SRCCOLKEY;
	ssi.ovfx.dckSrcColorkey.dwColorSpaceLowValue = COLOR_SPACE_LOW_VALUE;
	ssi.ovfx.dckSrcColorkey.dwColorSpaceHighValue = COLOR_SPACE_HIGH_VALUE;

	DVP_AVInit(&ssi);

	SetSink(sink_type);

	DVP_GSetDynamicType(TRUE, SETTINGS_DVD_DOLBY_OFF);

}

void CDvdRender::StopSrc()
{
	if (m_bIsRiping)
	{
		StopRip();
		m_bIsRiping = FALSE;
	}

	SaveDVPLM();
	DVP_ChangeDevice(DEVICE_IDLE); //change device to IDLE state  
	ShowVideo(FALSE);
	DVP_AVDeinit(_GetSinkType(m_SinkType));
	DVP_Deinit();
}


void CDvdRender::OnTimer(UINT_PTR nIDEvent)
{

}


BOOL CDvdRender::RegisterEventHandler(PFN_DVDRENDER_EVENT_HANDLER pfn, DWORD user_data)
{
	for (int i=0; i<DVD_MAX_EVENT_HANDLER_COUNT; i++)
	{
		if (m_eventHandler[i].pfn == NULL)	// 该项空闲
		{
			m_eventHandler[i].pfn = pfn;
			m_eventHandler[i].user_data = user_data;
			return TRUE;
		}
	}

	RETAILMSG(1, (L"[nvai_car]: CDvdRender::RegisterEventHandler() fail.\n"));
	return FALSE;
}

BOOL CDvdRender::UnRegisterEventHandler(PFN_DVDRENDER_EVENT_HANDLER pfn)
{
	for (int i=0; i<DVD_MAX_EVENT_HANDLER_COUNT; i++)
	{
		if (m_eventHandler[i].pfn == pfn)	// 该项空闲
		{
			m_eventHandler[i].pfn = NULL;
			m_eventHandler[i].user_data = 0;
			return TRUE;
		}
	}

	RETAILMSG(1, (L"[nvai_car]: CDvdRender::UnRegisterEventHandler() fail.\n"));
	return FALSE;
}


void CDvdRender::SetPBRepeat()
{
	DVP_SetPBMode(DVP_PBMODE_REPEAT);
}

void CDvdRender::SetPBRandom()
{
	DVP_SetPBMode(DVP_PBMODE_RANDOM);
}

void CDvdRender::GotoRootMenu()
{
	DVP_GotoRootMenu();
}

BOOL CDvdRender::ChooseDigitalNum(GUINT8 uType, GUINT8 uDigitalNum)
{
	GRESULT hr = DVP_ChooseDigitalNum(uType, uDigitalNum);		// 0 ~ for title, 1 ~ for chapter
	return (hr == RET_OK);
}

void CDvdRender::MenuUp()
{
	DVP_Up();
}

void CDvdRender::MenuDown()
{
	DVP_Down();	
}

void CDvdRender::MenuLeft()
{
	DVP_Left();
}

void CDvdRender::MenuRight()
{
	DVP_Right();
}

void CDvdRender::MenuEnter()
{
	if(GetState() != SV_PLAY)
	{	
		DVP_Play();
	}
	DVP_Play();
}

void CDvdRender::SwitchSubtitle()
{
	DVP_SwitchSubtitle(0);
}

void CDvdRender::SwitchPBCAngle()
{
	DVP_SwitchPBCAngle(0);	
}

void CDvdRender::GotoTitleMenu()
{
	DVP_GotoTitleMenu();
}

void CDvdRender::SwitchAudioChannel()
{
	DVP_SwitchAudioChannel(0);
}

void CDvdRender::ZoomIn()
{
	DVP_ZoomIn();
}

void CDvdRender::ZoomOut()
{
	DVP_ZoomOut();
}

void CDvdRender::Rotate()
{
	DVP_RotateImage(0);
}

void CDvdRender::Eject()
{
	// dvp_eject
	if (m_bInitialized)		// 如果从没初始化过,不需要调用出碟接口
	{
		DVP_Init();
		DVP_Eject();
		DVP_Deinit();

		SaveDVPLM(TRUE);
	}
}

void CDvdRender::InitCDTrackInfo(int nTotal)
{
	m_cdTrackInfo.RemoveAll();
	while (nTotal > 0)
	{
		m_cdTrackInfo.Add(0);
		nTotal--;
	}
}

void CDvdRender::SetCDTrackInfo(int nIndex, int nTrackLen)
{
	if (nIndex>=0 && nIndex<m_cdTrackInfo.GetCount())
	{
		m_cdTrackInfo.SetAt(nIndex, nTrackLen);
	}
}

const CArray<int>& CDvdRender::GetCDTrackInfo()
{
	return m_cdTrackInfo;
}

void CDvdRender::SetCDCurTrack(int nIndex)
{
	if (nIndex >= 0)
	{
		m_cdCurTrack = nIndex;
	}
}

int CDvdRender::GetCDTotalTrack()
{
	return m_cdTrackInfo.GetCount();
}

int CDvdRender::GetCDCurTrack()
{
	return m_cdCurTrack;
}

int CDvdRender::GetCurFileCount()
{
	GUINT16 file_count;
	if( RET_OK == DVP_GetCurFileCount(&file_count))
	{
		return file_count;
	}

	return 0;
}

UINT CDvdRender::GetRipTrkLen(int nTrack)
{
	UINT lba_len = 0;
	UINT len = 0;
	if ( RET_OK == DVP_GetRipTrkLbaLen((nTrack+1), &lba_len))
	{
		len = lba_len*RIP_BITRATE*(1000/8)/75; //bitrate: 128kb/s
	}
	return len;
}

BOOL CDvdRender::SetRipPath(LPCTSTR lpszFileName, UINT len)
{
	return (RET_OK == DVP_SetRipPath((GTCHAR *)lpszFileName, len));
}

UINT CDvdRender::StartRip(int nTrack)
{
	m_bIsRiping = TRUE;
	return DVP_StartRip((GUINT8)nTrack);
}

BOOL CDvdRender::StopRip()
{
	if (m_bIsRiping)
	{
		m_bIsRiping = FALSE;
		return (RET_OK == DVP_StopRip());
	}
	
	return TRUE;
}


UCHAR CDvdRender::GetRepeatMode()
{
	GUINT8 u1RepeatMode = 0;
	DVP_GetRepeatMode(&u1RepeatMode);
	return u1RepeatMode;
}

UCHAR CDvdRender::GetPBCUserMode()
{
	GUINT8 u1PBCUserMode = 0;
	DVP_GetPBCUserMode(&u1PBCUserMode);
	return u1PBCUserMode;
}

void CDvdRender::OpenAudio()
{
	GHANDLE hAudioDev = GOpenAudioDev();
	GChooseSpdifOutput(hAudioDev,MISC_AUD_DVD_OUT);
	GCloseAudioDev(hAudioDev);

	SWI_SINK_INFO_T ssi;
	ssi.eSink = (SWI_SINK_T)m_SinkType;
	SetRect(&ssi.DestRect, 0, 0, GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	ssi.u4Flag = SWI_INFO_HIDE | SWI_INFO_SRCCOLKEY;;
	ssi.ovfx.dckSrcColorkey.dwColorSpaceLowValue = COLOR_SPACE_LOW_VALUE;
	ssi.ovfx.dckSrcColorkey.dwColorSpaceHighValue = COLOR_SPACE_HIGH_VALUE;
	DVP_SetSurfaceInfo(&ssi);
	DVP_AVInit(&ssi);

	// 由调用者管理该接口,如果想继续播放,主动调用play
	//DVP_Play();
}

void CDvdRender::CloseAudio()
{
	DVP_Pause();
	DVP_AVDeinit(SWI_SINK_FRONT);
}