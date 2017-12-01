#include "StdAfx.h"
#include "msdkrender.h"
#include "interface.h"
#include "MainWnd.h"
#include "config.h"

// static functions
E_AVIN_DEST_TYPE_T _GetASinkType(DVP_SINK sink_type);
E_MSDK_SINK_TYPE _GetVSinkType(DVP_SINK sink_type);

CMsdkRender::CMsdkRender(void)
{
	m_hMediaGraphFile = NULL;
	m_hMediaGraphWma = NULL;

	m_hMediaGraphAvin1V = NULL;
	m_hMediaGraphAvin1A = NULL;
	m_hMediaGraphAvin2V = NULL;
	m_hMediaGraphAvin2A = NULL;
	m_hMediaGraphTVV = NULL;
	m_hMediaGraphTVA = NULL;
	m_hMediaGraphFM = NULL;
	m_hMediaGraphCamera = NULL;

	memset(m_eventHandlerFile, 0, sizeof(m_eventHandlerFile));
	memset(m_eventHandlerAvin, 0, sizeof(m_eventHandlerAvin));

	m_hwnd = NULL;
	m_bIsWmaPlaying = FALSE;
}

CMsdkRender::~CMsdkRender(void)
{


}

CMsdkRender* CMsdkRender::GetInstance()
{
	static CMsdkRender msdk_render;
	return &msdk_render;
}

BOOL CMsdkRender::Initialize(HWND hWnd)
{
	BOOL		ret = FALSE;
	m_hwnd = hWnd;

	if (S_MSDK_OK != MSDK_Init())
	{
		TRACE(L"MSDK_Init() fail.\n");
		goto LEXIT;
	}


	if (S_MSDK_OK != MSDK_Wma_Init())
	{
		TRACE(L"MSDK_Wma_Init() fail.\n");
		goto LEXIT;
	}

	if (S_AVIN_OK != AVIN_Init())
	{
		TRACE(L"AVIN_Init() fail.\n");
		goto LEXIT;
	}



	if (!CreateMediagraphFile())
	{
		goto LEXIT;
	}

	if (!CreateMediagraphAvin(&m_hMediaGraphAvin1V, &m_hMediaGraphAvin1A, UI_MSG_MSDK_MEDIAGRAPH_AVIN1))		// avin1
	{
		goto LEXIT;
	}

	if (!CreateMediagraphAvin(&m_hMediaGraphAvin2V, &m_hMediaGraphAvin2A, UI_MSG_MSDK_MEDIAGRAPH_AVIN2))		// avin2
	{
		goto LEXIT;
	}

	if (!CreateMediagraphAvin(NULL, &m_hMediaGraphFM, 0))	// FM
	{
		goto LEXIT;
	}

	if (!CreateMediagraphAvin(&m_hMediaGraphCamera, NULL, UI_MSG_MSDK_MEDIAGRAPH_AVIN_CAMERA))	// camera
	{
		goto LEXIT;
	}

	if (!CreateMediagraphAvin(&m_hMediaGraphTVV, &m_hMediaGraphTVA, UI_MSG_MSDK_MEDIAGRAPH_AVIN_TV))	// TV
	{
		goto LEXIT;
	}

	ret = TRUE;
LEXIT:
	return ret;
}


BOOL CMsdkRender::CreateMediagraphAvin(HMEDIAGRAPH *phMediaGraphV, HMEDIAGRAPH *phMediaGraphA, UINT msgNotify)
{
	if (phMediaGraphA)
	{
		*phMediaGraphA = AVIN_Create();
	}

	if (phMediaGraphV)
	{
		*phMediaGraphV = AVIN_Create();
		AVIN_SetNotifyWindow(*phMediaGraphV, m_hwnd, msgNotify);
	}

	return TRUE;
}


BOOL CMsdkRender::CreateMediagraphFile()
{
	BOOL		ret = FALSE;

	MRESULT hResult = MediaGraph_Create(&m_hMediaGraphFile);
	if (S_MSDK_OK != hResult)
	{
		goto LEXIT;
	}

	hResult = MediaGraph_RegisterNotifyWindow(m_hMediaGraphFile, m_hwnd, UI_MSG_MSDK_MEDIAGRAPH_FILE);
	if (S_MSDK_OK != hResult)
	{
		goto LEXIT;
	}

	MG_CONFIG_T  rConfig; 
	if (S_MSDK_OK == MediaGraph_GetConfig(m_hMediaGraphFile, &rConfig))
	{
		rConfig.u4MediaPosNotifyFreq = 1;
		hResult = MediaGraph_SetConfig(m_hMediaGraphFile, &rConfig);
		if (S_MSDK_OK != hResult)
		{
			goto LEXIT;
		}
	}

	hResult = MediaGraph_AddListener(m_hMediaGraphFile, MediaEventListenerFile, (GUINT32)this);
	if (S_MSDK_OK != hResult)
	{
		goto LEXIT;
	}

	FShowVideo(FALSE);
	//MediaGraph_SetDataSink(m_hMediaGraphFile, GetSinkType(sysutil::GetDvpSink())/*MSDK_SINK_FRONT*/);


	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//										wma															//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	hResult = MediaGraph_Wma_Create(&m_hMediaGraphWma);
	if (S_MSDK_OK != hResult)
	{
		goto LEXIT;
	}

	hResult = MediaGraph_Wma_RegisterNotifyWindow(m_hMediaGraphWma, m_hwnd, UI_MSG_MSDK_MEDIAGRAPH_WMA);
	if (S_MSDK_OK != hResult)
	{
		goto LEXIT;
	}

	if (S_MSDK_OK == MediaGraph_Wma_GetConfig(m_hMediaGraphWma, &rConfig))
	{
		rConfig.u4MediaPosNotifyFreq = 1;
		hResult = MediaGraph_Wma_SetConfig(m_hMediaGraphWma, &rConfig);
		if (S_MSDK_OK != hResult)
		{
			goto LEXIT;
		}
	}

	hResult = MediaGraph_Wma_AddListener(m_hMediaGraphWma, MediaEventListenerFile, (GUINT32)this);
	if (S_MSDK_OK != hResult)
	{
		goto LEXIT;
	}

	MediaGraph_Wma_SetDataSink(m_hMediaGraphWma, MSDK_SINK_FRONT);	


	ret = TRUE;
LEXIT:
	return ret;
}

static void _ReleaseAvinHandle(HAVINST *pHAvinst)
{
	if (pHAvinst && *pHAvinst)
	{
		AVIN_Stop(*pHAvinst);
		AVIN_Release(*pHAvinst);
		*pHAvinst = NULL;
	}
}

void CMsdkRender::Uninitialize()
{
	if (m_hMediaGraphWma)
	{
		MediaCtrl_Wma_Stop(m_hMediaGraphWma);
		MediaGraph_Wma_Clear(m_hMediaGraphWma);
		MediaGraph_Wma_Release(m_hMediaGraphWma);
		m_hMediaGraphWma = NULL;		
		MSDK_Wma_Uninit();
	}

	if (m_hMediaGraphFile)
	{
		MediaCtrl_Stop(m_hMediaGraphFile);
		MediaGraph_Clear(m_hMediaGraphFile);
		MediaGraph_Release(m_hMediaGraphFile);
		m_hMediaGraphFile = NULL;
	}

	_ReleaseAvinHandle(&m_hMediaGraphAvin1V);
	_ReleaseAvinHandle(&m_hMediaGraphAvin1A);
	_ReleaseAvinHandle(&m_hMediaGraphAvin2V);
	_ReleaseAvinHandle(&m_hMediaGraphAvin2A);
	_ReleaseAvinHandle(&m_hMediaGraphTVV);
	_ReleaseAvinHandle(&m_hMediaGraphTVA);
	_ReleaseAvinHandle(&m_hMediaGraphFM);

	MSDK_Uninit();
	AVIN_DeInit();
	
}


MRESULT CMsdkRender::MediaEventListenerFile(HMEDIAGRAPH hMediaGraph, GUINT32 u4Evt, 
								  GUINT32 u4Param1, GUINT32 u4Param2, 
								  GUINT32 u4CustomData)
{
	CMsdkRender *pthis = (CMsdkRender *)u4CustomData;
	if (pthis)
	{
		pthis->OnMediaEventFile(u4Evt, u4Param1, u4Param2);
	}
	return (S_MSDK_OK);
}


void CMsdkRender::OnMediaEventFile(UINT media_event, UINT param1, UINT param2)
{ 
	for (int i=0; i<MAX_EVENT_HANDLER_COUNT; i++)
	{
		if (m_eventHandlerFile[i].pfn != NULL)
		{
			(*m_eventHandlerFile[i].pfn)(media_event, param1, param2, m_eventHandlerFile[i].user_data);
		}
	}

	switch (media_event)
	{
	case EVT_MSDK_GRAPH_BUILT:
		OnGraphBuiltFile();		// 让回调先调用,再调用该函数.  Play要在MediaCtrl_LastMemStart才不显示视频第一帧
		break;
	case EVT_MSDK_FINISHED:	
	case EVT_MSDK_ERROR_OCCURED:
	case EVT_MSDK_GRAPH_CHANGED:
	case EVT_MSDK_PLAYBACK_POSITION_STATUS:
	case EVT_MSDK_PLAYED:
	case EVT_MSDK_PAUSED:
	case EVT_MSDK_STOPED:

	default:
		break;
	}

}

void CMsdkRender::OnGraphBuiltFile()
{
	if (m_bIsWmaPlaying)
	{
		MediaCtrl_Wma_Play(m_hMediaGraphWma);
	}
	else
	{
		MSDK_IID iid = IID_MSDK_VIDEOCTRL;
		HVIDEOCTRL  hVideoCtrl = NULL;
		MediaGraph_QueryInterface(m_hMediaGraphFile, &iid, &hVideoCtrl);
		if (hVideoCtrl)
		{
			VideoCtrl_SetOwner(hVideoCtrl, NULL);
			CMainWnd::GetInstance()->SetTimer(TIMER_ID_HIDE_MSDK_VIDEO_WIN, 1000, NULL);
		}

		if (FIsMediaTypeSupport())
		{
			MediaCtrl_Play(m_hMediaGraphFile);
		}


		// 必须调用下,要不后排没输出,这里简单处理下,因为不支持双工，所以直接设为前后排
		// 实际上这参数应该从当前状态获取。
		// 后面再优化
//		FSetSink(DVP_SINK_FRONT_REAR);
	}
}

BOOL CMsdkRender::FIsMediaTypeSupport()
{
	MSDK_MEDIA_INFO_T info;
	memset(&info,0,sizeof(MSDK_MEDIA_INFO_T));
	if (FGetMediaInfo(&info))
	{
		// 如果当前视频为h.264, 配置表中又配置为不支持h.264则不要播放
		if (info.rVdoInfo.eVCodec == MSDK_AVCODEC_ID_H264 && (config::get_config_video()->h264 == 0)
			/*|| info.rVdoInfo.eVCodec == MSDK_AVCODEC_ID_DIVX4 
			|| info.rVdoInfo.eVCodec == MSDK_AVCODEC_ID_DIVX5
			|| info.rVdoInfo.eVCodec == MSDK_AVCODEC_ID_DIVX6*/)
		{
			return FALSE;
		}
	}

	return TRUE;
}



MRESULT CMsdkRender::MediaEventListenerAvin(UINT msgNotify, GUINT32 u4Evt, 
											GUINT32 u4Param1, GUINT32 u4Param2, 
											GUINT32 u4CustomData)
{
	CMsdkRender *pthis = (CMsdkRender *)u4CustomData;
	if (pthis)
	{
		pthis->OnMediaEventAvin(msgNotify, u4Evt, u4Param1, u4Param2);
	}
	return (S_MSDK_OK);
}


void CMsdkRender::OnMediaEventAvin(UINT msgNotify, UINT media_event, UINT param1, UINT param2)
{ 
	switch(media_event)
	{
	case AVIN_SIGNAL_CHANGE:
	case AVIN_SIGNAL_READY:
		break;
	case AVIN_SIGNAL_LOST:
		break;
	default:
		break;
	}

	for (int i=0; i<MAX_EVENT_HANDLER_COUNT; i++)
	{
		if (m_eventHandlerAvin[i].pfn != NULL && m_eventHandlerAvin[i].hMediagraph == (HMEDIAGRAPH)msgNotify)
		{
			(*m_eventHandlerAvin[i].pfn)(media_event, param1, param2, m_eventHandlerAvin[i].user_data);
		}
	}
}


void CMsdkRender::SetVideoScaleAvin(HAVINST hMediaGraph, RECT *prc)
{
	if (hMediaGraph == NULL || prc == NULL)
	{
		return;
	}

	AVIN_SetSourceRect(hMediaGraph,AVIN_DEST_FRONT,prc);
}

E_AVIN_SIGNAL_MODE_T CMsdkRender::GetSignalModeAvin(HAVINST hMediaGraph)
{
	E_AVIN_SIGNAL_MODE_T eSigMode = AVIN_SIGNAL_NONE;
	AVIN_GetVideoSignalMode(hMediaGraph, &eSigMode);
	return eSigMode;
}

BOOL CMsdkRender::FSetSourceFile(LPCTSTR lpszFilePath)
{
	if (lpszFilePath)
	{
		MRESULT hResult = E_MSDK_FAILED;
		if (fgWmaFile(NULL, (GTCHAR*)lpszFilePath))
		{
			if (!m_bIsWmaPlaying)
			{
				MediaCtrl_Stop(m_hMediaGraphFile);
				MediaGraph_Clear(m_hMediaGraphFile);
			}

			hResult = MediaGraph_Wma_SetSourceFile(m_hMediaGraphWma, (GTCHAR*)lpszFilePath);
			m_bIsWmaPlaying = TRUE;
		}
		else
		{
			if (m_bIsWmaPlaying)
			{
				MediaCtrl_Wma_Stop(m_hMediaGraphWma);
				MediaGraph_Wma_Clear(m_hMediaGraphWma);
				m_bIsWmaPlaying = FALSE;
			}


			if (FGetState() != MSDK_MGSTATE_STOP)
			{
				FStop();
			}

			hResult = MediaGraph_SetSourceFile(m_hMediaGraphFile, (GTCHAR*)lpszFilePath);
		}

		CMainWnd::GetInstance()->PostMessage(UI_MSG_MSDK_PLAY_FILE);

		return (hResult == S_MSDK_OK);
	}

	return FALSE;
}

void CMsdkRender::HandleEventAvin(UINT msgNotify, UINT u4Event)
{
	// 只是为了保持与3360的接口一致,第3，4个参数是没有作用的
	CMsdkRender::MediaEventListenerAvin(msgNotify, u4Event, 0, 0, (GUINT32)this);
}

void CMsdkRender::HandleEventF()
{
	MediaGraph_HandleEvent(m_hMediaGraphFile);
}

void CMsdkRender::HandleEventWma()
{
	MediaGraph_Wma_HandleEvent(m_hMediaGraphWma);
}

void CMsdkRender::FShowVideo(BOOL bShow, LPRECT prect, E_MSDK_SINK_TYPE eSink)
{
	MSDK_SINK_INFO_T msi;
	msi.u4Flag = bShow ? MSDK_SWI_INFO_SHOW : MSDK_SWI_INFO_HIDE;
	if (!prect)
	{
		SetRect(&msi.DestRect, 0, 0, GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	}
	else
	{
		msi.DestRect = *prect;
	}
	msi.eSink = eSink;
	MediaGraph_SetSinkInfo(m_hMediaGraphFile, &msi);
}

void CMsdkRender::FSetSink(DVP_SINK sink_type)
{

	if (sink_type == DVP_SINK_FRONT 
		|| sink_type == DVP_SINK_REAR
		|| sink_type == DVP_SINK_FRONT_REAR)
	{
		MediaGraph_SetDataSink(m_hMediaGraphFile, (E_MSDK_SINK_TYPE)sink_type);

		if (m_bIsWmaPlaying)
		{
			MediaGraph_Wma_SetDataSink(m_hMediaGraphWma, (E_MSDK_SINK_TYPE)sink_type);
		}
	}
	else if (sink_type == DVP_SINK_FRONTV_REAR)
	{
		MediaGraph_SetDataSink(m_hMediaGraphFile, MSDK_SINK_REAR);
		MediaGraph_SwitchVideo(m_hMediaGraphFile, MSDK_SINK_FRONT_REAR);
		// 注： 由上层调用维护
		// SwitchVideo有可能将隐藏的视频显示出来，需确保视频处于正确的状态
		//OnShowVideo(TRUE);
	}	
}

void CMsdkRender::FPlay()
{
	if (!FIsMediaTypeSupport())
	{
		return;		// nothing to do for unsupported video.
	}

	if (FGetState() != MSDK_MGSTATE_PLAYING)
	{
		if (m_bIsWmaPlaying)
		{
			MediaCtrl_Wma_Play(m_hMediaGraphWma);
		}
		else
		{
			MediaCtrl_Play(m_hMediaGraphFile);
		}
	}
}

void CMsdkRender::FPause()
{
	if (!FIsMediaTypeSupport())
	{
		return;		// nothing to do for unsupported video.
	}

	if (FGetState() != MSDK_MGSTATE_PAUSE)
	{
		if (m_bIsWmaPlaying)
		{
			MediaCtrl_Wma_Pause(m_hMediaGraphWma);
		}
		else
		{
			MediaCtrl_Pause(m_hMediaGraphFile);
		}
	}
}

void CMsdkRender::FStop()
{
	if (FGetState() != MSDK_MGSTATE_STOP)
	{
		if (m_bIsWmaPlaying)
		{
			MediaCtrl_Wma_Stop(m_hMediaGraphWma);
		}
		else
		{
			MediaCtrl_Stop(m_hMediaGraphFile);
		}
	}
}

void CMsdkRender::FSeek(DWORD dwPosition)
{
	E_MSDK_MGSTATE  eState;

	if (m_bIsWmaPlaying)
	{
		if (MediaGraph_Wma_GetState(m_hMediaGraphWma, &eState) ==  S_MSDK_OK
			&& eState != MSDK_MGSTATE_STOP)
		{
			GUINT64 pos = (GUINT64)dwPosition*(GUINT64)1000;
			MediaCtrl_Wma_Seek(m_hMediaGraphWma, MSDK_TIMEFMT_DEFAULT,	pos);
		}
	}
	else
	{
		if (MediaGraph_GetState(m_hMediaGraphFile, &eState) ==  S_MSDK_OK
			&& eState != MSDK_MGSTATE_STOP)
		{
			GUINT64 pos = (GUINT64)dwPosition*(GUINT64)1000;
			MediaCtrl_Seek(m_hMediaGraphFile, MSDK_TIMEFMT_DEFAULT,	pos);
		}
	}
}

E_MSDK_MGSTATE CMsdkRender::FGetState()
{
	E_MSDK_MGSTATE  eState;

	if (m_bIsWmaPlaying)
	{
		if (S_MSDK_OK == MediaGraph_Wma_GetState(m_hMediaGraphWma, &eState))
		{
			return eState;
		}
		else
		{
			return MSDK_MGSTATE_STOP;	// 如果获取状态失败认为是STOP状态
		}
	}
	else
	{
		if (S_MSDK_OK == MediaGraph_GetState(m_hMediaGraphFile, &eState))
		{
			return eState;
		}
	}

	return MSDK_MGSTATE_STOP;	// 如果获取状态失败认为是STOP状态
}


DWORD CMsdkRender::FGetDuration()
{
	GUINT64 duration = 0;
	if (m_bIsWmaPlaying)
	{
		if (S_MSDK_OK == MediaCtrl_Wma_GetDuration(m_hMediaGraphWma, &duration))
		{
			return (DWORD)(duration/1000);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (S_MSDK_OK == MediaCtrl_GetDuration(m_hMediaGraphFile, &duration))
		{
			return (DWORD)(duration/1000);
		}
		else
		{
			return 0;
		}

	}

}

DWORD CMsdkRender::FGetCurPostion()
{
	GUINT64 postion = 0;
	if (m_bIsWmaPlaying)
	{
		if (S_MSDK_OK == MediaCtrl_Wma_GetCurrentPosition(m_hMediaGraphWma, &postion))
		{
			return (DWORD)(postion/1000);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		if (S_MSDK_OK == MediaCtrl_GetCurrentPosition(m_hMediaGraphFile, &postion))
		{
			return (DWORD)(postion/1000);
		}
	}

	return 0;
}

HAUDIOINFO CMsdkRender::FGetAudioInfoHandle()
{
	MSDK_IID	iid = IID_MSDK_AUDIOINFO;
	HAUDIOINFO	hAudioInfo = NULL;
	if (m_bIsWmaPlaying)
	{
		if (S_MSDK_OK == MediaGraph_Wma_QueryInterface(m_hMediaGraphWma, &iid, &hAudioInfo))
		{
			return hAudioInfo;
		}
	}
	else
	{
		if (S_MSDK_OK == MediaGraph_QueryInterface(m_hMediaGraphFile, &iid, &hAudioInfo))
		{
			return hAudioInfo;
		}
	}

	
	return NULL;
}

void CMsdkRender::FGetAudioInfo(OUT BYTE* pCount, OUT BYTE* pCurrent)
{
	BYTE count = 0, current = 0;
	HAUDIOINFO	hAudioInfo = FGetAudioInfoHandle();
	if (hAudioInfo != NULL)
	{
		AudioCtrl_GetCount(hAudioInfo, &count);
		if(count > 0)
		{
			AudioCtrl_GetCurrentAudio(hAudioInfo, &current);
		}
	}

	if (pCount)		*pCount = count;
	if (pCurrent)	*pCurrent = current;
}

BOOL CMsdkRender::FSetCurrentAudio(BYTE current)
{
	HAUDIOINFO	hAudioInfo = FGetAudioInfoHandle();
	if (hAudioInfo != NULL)
	{
		if (S_MSDK_OK == AudioCtrl_SetCurrentAudio(hAudioInfo, current))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CMsdkRender::FGetSubtitleInfo(OUT INT* pCount, OUT INT* pCurrent)
{
	UINT count = 0;
	INT current = 0;
	HSUBTITLEINFO hSubtitleInfo = NULL;
	HSUBTITLECTRL hSubtitleCtrl = NULL;

	MSDK_IID    iid = IID_MSDK_SUBTITLEINFO;
	if (S_MSDK_OK == MediaGraph_QueryInterface(m_hMediaGraphFile, &iid, &hSubtitleInfo))
	{
		SubtitleInfo_GetCount(hSubtitleInfo, &count);
	}

	if(count > 0)
	{
		iid = IID_MSDK_SUBTITLECTRL;
		if (S_MSDK_OK == MediaGraph_QueryInterface(m_hMediaGraphFile, &iid, &hSubtitleCtrl))
		{
			SubtitleCtrl_GetCurrentSubtitle(hSubtitleCtrl, &current);
		}
	}

	if (pCount)		*pCount = count;
	if (pCurrent)	*pCurrent = current;
}

BOOL CMsdkRender::FSetCurrentSubtitle(INT current)
{
	HSUBTITLECTRL hSubtitleCtrl = NULL;
	MSDK_IID    iid = IID_MSDK_SUBTITLECTRL;

	if (S_MSDK_OK == MediaGraph_QueryInterface(m_hMediaGraphFile, &iid, &hSubtitleCtrl))
	{
		return (S_MSDK_OK == SubtitleCtrl_SetCurrentSubtitle(hSubtitleCtrl, current));
	}

	return FALSE;
}

BOOL CMsdkRender::FSetSubtitleCodePage(E_MSDK_CODEPAGE_SET eCodePage)
{
	return (S_MSDK_OK == MSDK_SetCodePage(eCodePage));
}

BOOL CMsdkRender::FGetMediaInfo(MSDK_MEDIA_INFO_T* pinfo)
{
	if (pinfo && MediaGraph_GetMediaInfo(m_hMediaGraphFile, pinfo) == S_MSDK_OK)
	{
		return TRUE;
	}

	return FALSE;
}

void CMsdkRender::FLastMemoryStart(DWORD dwPosition)
{
	if (m_bIsWmaPlaying)
	{
		//MediaCtrl_Wma_LastMemStart(m_hMediaGraphWma, MSDK_TIMEFMT_MEDIA_TIME, dwPosition*1000);	// 单位毫秒
		MediaCtrl_Wma_Play(m_hMediaGraphWma);
		Sleep(200);
		FSeek(dwPosition);
	}
	else
	{
		MediaCtrl_LastMemStart(m_hMediaGraphFile, MSDK_TIMEFMT_MEDIA_TIME, dwPosition*1000);	// 单位毫秒
	}
}

void CMsdkRender::FLaunchSrc(DVP_SINK sink_type)
{
	FSetSink(sink_type);
}

void CMsdkRender::FStopSrc()
{
	if (m_bIsWmaPlaying)
	{
		MediaCtrl_Wma_Stop(m_hMediaGraphWma);
		MediaGraph_Wma_Clear(m_hMediaGraphWma);
		m_bIsWmaPlaying = FALSE;
	}
	else
	{
		MediaCtrl_Stop(m_hMediaGraphFile);
		MediaGraph_Clear(m_hMediaGraphFile);
	}
}

void CMsdkRender::FStopnClear()
{
	// 跟FStopSrc()是一样的代码,但语义不一样
	FStopSrc();
}

BOOL CMsdkRender::FIsWmaPlaying()
{
	return m_bIsWmaPlaying;
}

void CMsdkRender::FCloseAudio()
{
	if(m_bIsWmaPlaying)
	{
		MediaCtrl_Wma_Pause(m_hMediaGraphWma); 
		Sleep(150);
		MediaGraph_Wma_CloseAudio(m_hMediaGraphWma,MSDK_SINK_FRONT);
	}
	else
	{
		MediaCtrl_Pause(m_hMediaGraphFile);
		Sleep(150);
		MediaGraph_CloseAudio(m_hMediaGraphFile,MSDK_SINK_FRONT);
	}
}

void CMsdkRender::FOpenAudio()
{
	if(m_bIsWmaPlaying)
	{
		MediaGraph_Wma_OpenAudio(m_hMediaGraphWma,MSDK_SINK_FRONT);
		Sleep(150);
		MediaCtrl_Wma_Play(m_hMediaGraphWma); 
	}
	else
	{
		MediaGraph_OpenAudio(m_hMediaGraphFile,MSDK_SINK_FRONT);
		Sleep(150);
		MediaCtrl_Play(m_hMediaGraphFile);
	}
}


void CMsdkRender::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ID_HIDE_MSDK_VIDEO_WIN)
	{
		CMainWnd::GetInstance()->KillTimer(TIMER_ID_HIDE_MSDK_VIDEO_WIN);

		HWND hwnd = ::FindWindow(L"VideoRenderFilter", L"ActiveMovie Window");
		if (hwnd)
		{
			::ShowWindow(hwnd , SW_HIDE);
		}

		hwnd = ::FindWindow(L"FilterGraphWindow", L"ActiveMovie Window");
		if (hwnd)
		{
			::ShowWindow(hwnd , SW_HIDE);
		}
	}
}


//////////////////////////////   AVIN   //////////////////////////////////////////////

void CMsdkRender::OpenAvin(HMEDIAGRAPH *phMediaGraphV, E_AVIN_SOURCE_CHANNEL_T srcV, 
						   HMEDIAGRAPH *phMediaGraphA, E_AVIN_SOURCE_CHANNEL_T srcA, DVP_SINK sink_type, BOOL bInitShow)
{
	if (phMediaGraphA && srcA != AVIN_CHANNEL_NONE)
	{
		AVIN_SetSource( *phMediaGraphA, AVIN_SOURCE_A, AVIN_CHANNEL_NONE, srcA); 
		AVIN_SetDestnation(*phMediaGraphA, (E_AVIN_DEST_TYPE_T)_GetASinkType(sink_type));
	}

	if (phMediaGraphV && srcV != AVIN_CHANNEL_NONE)
	{
		AVIN_SetSource(*phMediaGraphV, AVIN_SOURCE_V, srcV, AVIN_CHANNEL_NONE); 
		//AVIN_SetDestnation(*phMediaGraphV, (E_AVIN_DEST_TYPE_T)_GetASinkType(sink_type));	
		RECT rc = {0, 0, WceUiGetScreenWidth(), WceUiGetScreenHeight()};
		AVIN_SetDestinationRect(*phMediaGraphV, (E_AVIN_DEST_TYPE_T)_GetASinkType(sink_type), &rc);	

	}

}

void CMsdkRender::CloseAvin(HMEDIAGRAPH *phMediaGraphV, HMEDIAGRAPH *phMediaGraphA)
{
	if (phMediaGraphV && *phMediaGraphV != NULL)
	{
		AVIN_Stop(*phMediaGraphV);
	}

	if (phMediaGraphA && *phMediaGraphA != NULL)
	{
		AVIN_Stop(*phMediaGraphA);
	}
}


void CMsdkRender::ShowVideoAvin(BOOL bShow, HMEDIAGRAPH *phMediaGraphV, DVP_SINK sink_type, LPRECT prect)
{
	if (phMediaGraphV == NULL )
	{
		return;
	}

	if (prect)
	{
		AVIN_SetDestinationRect(*phMediaGraphV, AVIN_DEST_FRONT, prect);
	}
	AVIN_SetVideoVisible(*phMediaGraphV, AVIN_DEST_FRONT, bShow);
}

void CMsdkRender::AOnCamera(BOOL bStart, SOURCE_ID sid, DVP_SINK sink_type)
{
	if (sid != SOURCE_AVIN1 && sid != SOURCE_AVIN2 && sid != SOURCE_TV)
	{
		return;
	}

	HMEDIAGRAPH hMediaGraphV = NULL;
	if (sid == SOURCE_AVIN1)
	{
		hMediaGraphV = m_hMediaGraphAvin1V;
	}
	else if (sid == SOURCE_AVIN2)
	{
		hMediaGraphV = m_hMediaGraphAvin2V;
	}
	else if (sid == SOURCE_TV)
	{
		hMediaGraphV = m_hMediaGraphTVV;
	}

	if (!bStart)	// 结束倒车
	{
		if (sink_type == DVP_SINK_FRONT)
		{
			MediaCtrl_PlayAvin(hMediaGraphV);
		}
		else if (sink_type == DVP_SINK_REAR )
		{
			AVIN_SetDestnation(hMediaGraphV, AVIN_DEST_REAR);
		}
		else if (sink_type == DVP_SINK_FRONTV_REAR || sink_type == DVP_SINK_FRONT_REAR)
		{
			AVIN_SetDestnation(hMediaGraphV, AVIN_DEST_FRONT_REAR);
		}
	}
	else	// 开始倒车
	{
		if (sink_type == DVP_SINK_FRONT)
		{
			MediaCtrl_StopAvin(hMediaGraphV);
		}
		else if (sink_type == DVP_SINK_REAR || sink_type == DVP_SINK_FRONTV_REAR || sink_type == DVP_SINK_FRONT_REAR)
		{
			//MediaGraph_SwitchAVInSink(hMediaGraphV, MSDK_SINK_REAR, MSDK_BYPASS);
			AVIN_SetDestnation(hMediaGraphV, AVIN_DEST_REAR);
		}
	}
}

void CMsdkRender::SetSinkAvin(HMEDIAGRAPH *phMediaGraphV, HMEDIAGRAPH *phMediaGraphA, DVP_SINK sink_type)
{
	// 前后排转换时,如果前排隐藏了,有可能已经将其stop了
	if (phMediaGraphV)
	{
// 		E_MSDK_MGSTATE state = MSDK_MGSTATE_STOP;
// 		MediaGraph_GetState(*phMediaGraphV, &state);
// 		if (state != MSDK_MGSTATE_PLAYING )
// 		{
// 			AVIN_Play(*phMediaGraphV);
// 		}
	}

	if (sink_type == DVP_SINK_FRONT 
		|| sink_type == DVP_SINK_REAR
		|| sink_type == DVP_SINK_FRONT_REAR)
	{
		if (phMediaGraphA)
		{
			AVIN_SetDestnation(*phMediaGraphA, E_AVIN_DEST_TYPE_T(sink_type-1));
		}
		if (phMediaGraphV)
		{
			AVIN_SetDestnation(*phMediaGraphV, E_AVIN_DEST_TYPE_T(sink_type-1));
		}
	}
	else if (sink_type == DVP_SINK_FRONTV_REAR)
	{
		if (phMediaGraphA)
		{
			AVIN_SetDestnation(*phMediaGraphA, AVIN_DEST_REAR);
		}
		if (phMediaGraphV)
		{
			AVIN_SetDestnation(*phMediaGraphV, AVIN_DEST_FRONT_REAR);
		}
	}
}



void CMsdkRender::ALaunchAvin1Src( DVP_SINK sink_type)
{
	OpenAvin(&m_hMediaGraphAvin1V, (E_AVIN_SOURCE_CHANNEL_T)config::get_config_avin()->avin1_v,
		&m_hMediaGraphAvin1A, (E_AVIN_SOURCE_CHANNEL_T)config::get_config_avin()->avin1_a, sink_type);
}

void CMsdkRender::AStopAvin1Src()
{
	CloseAvin(&m_hMediaGraphAvin1V, &m_hMediaGraphAvin1A);
}

void CMsdkRender::AShowVideoAvin1(BOOL bShow, DVP_SINK sink_type, LPRECT prect)
{
	ShowVideoAvin(bShow, &m_hMediaGraphAvin1V, sink_type, prect);
}

void CMsdkRender::ASetSinkAvin1(DVP_SINK sink_type)
{
	SetSinkAvin(&m_hMediaGraphAvin1V, &m_hMediaGraphAvin1A, sink_type);
}

BOOL CMsdkRender::AIsSignalReadyAvin1()
{
	BOOL isReady = FALSE;
	AVIN_IsVideoSignalReady(m_hMediaGraphAvin1V, &isReady);
	return isReady;
}

void CMsdkRender::SetVideoScaleAvin1(RECT *prc)
{
	SetVideoScaleAvin(m_hMediaGraphAvin1V, prc);
}

E_AVIN_SIGNAL_MODE_T CMsdkRender::AGetSignalModeAvin1()
{
	return GetSignalModeAvin(m_hMediaGraphAvin1V);
}

void CMsdkRender::ACloseAudioAvin1()
{
	AVIN_Stop(m_hMediaGraphAvin1A);
}

void CMsdkRender::AOpenAudioAvin1()
{
	AVIN_Play(m_hMediaGraphAvin1A);
}


void CMsdkRender::ALaunchAvin2Src( DVP_SINK sink_type)
{
 	OpenAvin(&m_hMediaGraphAvin2V, (E_AVIN_SOURCE_CHANNEL_T)config::get_config_avin()->avin2_v,
 		&m_hMediaGraphAvin2A, (E_AVIN_SOURCE_CHANNEL_T)config::get_config_avin()->avin2_a, sink_type);
}

void CMsdkRender::AStopAvin2Src()
{
	CloseAvin(&m_hMediaGraphAvin2V, &m_hMediaGraphAvin2A);
}

void CMsdkRender::AShowVideoAvin2(BOOL bShow, DVP_SINK sink_type, LPRECT prect)
{
	ShowVideoAvin(bShow, &m_hMediaGraphAvin2V, sink_type, prect);
}

void CMsdkRender::ASetSinkAvin2(DVP_SINK sink_type)
{
	SetSinkAvin(&m_hMediaGraphAvin2V, &m_hMediaGraphAvin2A, sink_type);
}

BOOL CMsdkRender::AIsSignalReadyAvin2()
{
	BOOL isReady = FALSE;
	AVIN_IsVideoSignalReady(m_hMediaGraphAvin2V, &isReady);
	return isReady;
}

void CMsdkRender::SetVideoScaleAvin2(RECT *prc)
{
	SetVideoScaleAvin(m_hMediaGraphAvin2V, prc);
}

E_AVIN_SIGNAL_MODE_T CMsdkRender::AGetSignalModeAvin2()
{
	return GetSignalModeAvin(m_hMediaGraphAvin2V);
}

void CMsdkRender::ACloseAudioAvin2()
{
	AVIN_Stop(m_hMediaGraphAvin2A);
}

void CMsdkRender::AOpenAudioAvin2()
{
	AVIN_Play(m_hMediaGraphAvin2A);
}

//
void CMsdkRender::ALaunchTVSrc( DVP_SINK sink_type)
{
	OpenAvin(&m_hMediaGraphTVV, (E_AVIN_SOURCE_CHANNEL_T)config::get_config_avin()->tv_v,
		&m_hMediaGraphTVA, (E_AVIN_SOURCE_CHANNEL_T)config::get_config_avin()->tv_a, sink_type);
}

void CMsdkRender::AStopTVSrc()
{
	CloseAvin(&m_hMediaGraphTVV, &m_hMediaGraphTVA);
}

void CMsdkRender::AShowVideoTV(BOOL bShow, DVP_SINK sink_type, LPRECT prect)
{
	ShowVideoAvin(bShow, &m_hMediaGraphTVV, sink_type, prect);
}

void CMsdkRender::ASetSinkTV(DVP_SINK sink_type)
{
	SetSinkAvin(&m_hMediaGraphTVV, &m_hMediaGraphTVA, sink_type);
}

BOOL CMsdkRender::AIsSignalReadyTV()
{
	BOOL isReady = FALSE;
	AVIN_IsVideoSignalReady(m_hMediaGraphTVV, &isReady);
	return isReady;
}

void CMsdkRender::SetVideoScaleTV(RECT *prc)
{
	SetVideoScaleAvin(m_hMediaGraphTVV, prc);
}

E_AVIN_SIGNAL_MODE_T CMsdkRender::AGetSignalModeTV()
{
	return GetSignalModeAvin(m_hMediaGraphTVV);
}

void CMsdkRender::ACloseAudioTV()
{
	AVIN_Stop(m_hMediaGraphTVA);
}

void CMsdkRender::AOpenAudioTV()
{
	AVIN_Play(m_hMediaGraphTVA);
}

//

void CMsdkRender::ALaunchFMSrc()
{
	OpenAvin(NULL, AVIN_CHANNEL_NONE, &m_hMediaGraphFM, 
		(E_AVIN_SOURCE_CHANNEL_T)config::get_config_avin()->fm_a, DVP_SINK_FRONT);
}

void CMsdkRender::AStopFMSrc()
{
	CloseAvin(NULL, &m_hMediaGraphFM);
}

BOOL CMsdkRender::RegisterEventHandlerF(PFN_MSDKRENDER_EVENT_HANDLER pfn, DWORD user_data)
{
	for (int i=0; i<MAX_EVENT_HANDLER_COUNT; i++)
	{
		if (m_eventHandlerFile[i].pfn == NULL)	// 该项空闲
		{
			m_eventHandlerFile[i].pfn = pfn;
			m_eventHandlerFile[i].user_data = user_data;
			return TRUE;
		}
	}

	RETAILMSG(1, (L"[nvai_car]: CMsdkRender::RegisterEventHandlerF() fail.\n"));
	return FALSE;
}

BOOL CMsdkRender::UnRegisterEventHandlerF(PFN_MSDKRENDER_EVENT_HANDLER pfn, DWORD user_data)
{
	for (int i=0; i<MAX_EVENT_HANDLER_COUNT; i++)
	{
		if (m_eventHandlerFile[i].pfn == pfn && m_eventHandlerFile[i].user_data == user_data)
		{
			m_eventHandlerFile[i].pfn = NULL;
			m_eventHandlerFile[i].user_data = 0;
			return TRUE;
		}
	}

	RETAILMSG(1, (L"[nvai_car]: CMsdkRender::UnRegisterEventHandlerF() fail.\n"));
	return FALSE;
}


BOOL CMsdkRender::RegisterEventHandlerAvin(PFN_MSDKRENDER_EVENT_HANDLER pfn, DWORD user_data, MSDK_AVIN_TYPE type)
{
	for (int i=0; i<MAX_EVENT_HANDLER_COUNT; i++)
	{
		if (m_eventHandlerAvin[i].pfn == NULL)	// 该项空闲
		{
			m_eventHandlerAvin[i].pfn = pfn;
			m_eventHandlerAvin[i].user_data = user_data;
			m_eventHandlerAvin[i].hMediagraph = GetAvinMediagraph(type);
			return TRUE;
		}
	}

	RETAILMSG(1, (L"[nvai_car]: CMsdkRender::RegisterEventHandlerAvin() fail.\n"));
	return FALSE;
}

HMEDIAGRAPH CMsdkRender::GetAvinMediagraph(MSDK_AVIN_TYPE type)
{
	switch (type)
	{
	case MSDK_AVIN1:
		return (HMEDIAGRAPH)UI_MSG_MSDK_MEDIAGRAPH_AVIN1;
	case MSDK_AVIN2:
		return (HMEDIAGRAPH)UI_MSG_MSDK_MEDIAGRAPH_AVIN2;
	case MSDK_AVIN_TV:
		return (HMEDIAGRAPH)UI_MSG_MSDK_MEDIAGRAPH_AVIN_TV;
	case MSDK_AVIN_CAMERA:
		return (HMEDIAGRAPH)UI_MSG_MSDK_MEDIAGRAPH_AVIN_CAMERA;
	default:
		return NULL;
	}
}

BOOL CMsdkRender::UnRegisterEventHandlerAvin(PFN_MSDKRENDER_EVENT_HANDLER pfn)
{
	for (int i=0; i<MAX_EVENT_HANDLER_COUNT; i++)
	{
		if (m_eventHandlerAvin[i].pfn == pfn)	// 该项空闲
		{
			m_eventHandlerAvin[i].pfn = NULL;
			m_eventHandlerAvin[i].user_data = 0;
			return TRUE;
		}
	}

	RETAILMSG(1, (L"[nvai_car]: CMsdkRender::UnRegisterEventHandlerAvin() fail.\n"));
	return FALSE;
}


static E_MSDK_SINK_TYPE _GetVSinkType(DVP_SINK sink_type)
{
	if (sink_type == DVP_SINK_FRONT || sink_type == DVP_SINK_REAR || sink_type == DVP_SINK_FRONT_REAR)
	{
		return (E_MSDK_SINK_TYPE)sink_type;
	}
	else if (sink_type == DVP_SINK_FRONTV_REAR)
	{
		return MSDK_SINK_FRONT_REAR;
	}
	else	// 如果是未知类型，直接设为前后排输出
	{
		ASSERT(FALSE);
		return MSDK_SINK_FRONT_REAR;
	}
}


static E_AVIN_DEST_TYPE_T _GetASinkType(DVP_SINK sink_type)
{
	if (sink_type == DVP_SINK_FRONT || sink_type == DVP_SINK_REAR || sink_type == DVP_SINK_FRONT_REAR)
	{
		return (E_AVIN_DEST_TYPE_T)(sink_type-1);
	}
	else if (sink_type == DVP_SINK_FRONTV_REAR)
	{
		return AVIN_DEST_REAR;
	}
	else	// 如果是未知类型，直接设为前后排输出
	{
		ASSERT(FALSE);
		return AVIN_DEST_FRONT_REAR;
	}
}



void CMsdkRender::AOpenCameraChannel()
{
	OpenAvin(&m_hMediaGraphCamera, (E_AVIN_SOURCE_CHANNEL_T)config::get_config_avin()->camera_v, NULL, AVIN_CHANNEL_NONE, DVP_SINK_FRONT, TRUE);
}

void CMsdkRender::ACloseCameraChannel()
{
	CloseAvin(&m_hMediaGraphCamera, NULL);
}

void CMsdkRender::ASetCameraMirror(E_AVIN_MIRROR_TYPE_T mirror)
{
	AVIN_SetMirror(m_hMediaGraphCamera, mirror);
}

BOOL CMsdkRender::AIsSignalReadyCamera()
{
	BOOL isReady = FALSE;
	AVIN_IsVideoSignalReady(m_hMediaGraphCamera, &isReady);
	return isReady;
}

void CMsdkRender::SetVideoScaleCamera(RECT *prc)
{
	SetVideoScaleAvin(m_hMediaGraphCamera, prc);
}