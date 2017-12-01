#include "stdafx.h"
#include "IPODManager.h"
#include "interface.h"
#include <queue>
using namespace std;

CIPODManager CIPODManager::m_Instance;
#define QUEUE_ITEM_SIZE      (sizeof(MUSB_IPOD_NOTIFY_INFO_T))

void CALLBACK ConnectDeviceCB(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
	CIPODManager::GetInstance()->ConnectDevice();
}

CIPODManager::CIPODManager() 
: m_bInit(FALSE)
{
	m_iPodSDKAgent=NULL;
	m_hEvtQueueNotify=NULL;
	//  m_bAuthPassed=false;
	m_bConnected=false;
	m_bInsertPowerOn=false;
	m_pbTrackArtworkData=NULL;
	memset(&m_rArtWorkFmtInfo, 0, sizeof(MUSB_IPOD_ARTWORK_FMTS_INFO_T));
	memset(&m_rArtWorkFmts[0], 0, sizeof(MUSB_IPOD_ARTWORK_FMTS) * MAX_ARTWORK_FTM_NUM);
	m_rArtWorkFmtInfo.prArtworkFmt = &m_rArtWorkFmts[0];
	m_pServceData=new  CInterProcessCommunication(L"YECON_IPOD",512,FALSE,TRUE);
	m_pServerArtWork=new CInterProcessCommunication (L"Art_work",1024*2048,FALSE,TRUE);
}

CIPODManager::~CIPODManager ()
{
	if (m_iPodSDKAgent)
	{
		delete m_iPodSDKAgent;
	}
	if (m_pbTrackArtworkData)
	{
		delete []m_pbTrackArtworkData;
		m_pbTrackArtworkData = NULL;
	}
	if (m_pServceData)
	{
		delete m_pServceData;
		m_pServceData=NULL;
	}
	if (m_pServerArtWork)
	{
		delete m_pServerArtWork;
		m_pServerArtWork=NULL;
	}
	m_iPodSDKAgent=NULL;

}


void    CIPODManager::SetIPodMute()
{        
	if (m_bConnected)
	{
		/*	MUSB_IPOD_PLAY_ST_INFO_T rPbStatus;
		ZeroMemory(&rPbStatus, sizeof(MUSB_IPOD_PLAY_ST_INFO_T));
		CALL_IPOD_API(m_iPodSDKAgent->m_pfnGetPlayStatus(&rPbStatus));
		if (rPbStatus.ePbStatus==PB_ST_EX_PLAY_0X01)
		{
		m_iPodSDKAgent->m_pfnPlayControl(PLAY_CTRL_STOP);
		} 
		Sleep(100);*/
		if (m_iPodSDKAgent->IsInit())
			m_iPodSDKAgent->m_pfnSetVolume(0);

	}

}

void CIPODManager::OnPlugIn()
{
	ConnectDevice();

	// 有可能连接失败，起个计时器重复操作,确保成功
	SetTimer(m_hMainWnd, TIMER_CONNECTDEVICE, 800, ConnectDeviceCB); 
}

void CIPODManager::OnPlugOut()
{
	KillTimer(m_hMainWnd, TIMER_CONNECTDEVICE);	// 拔出时有可能还没验证成功,停止连接
	if (m_bConnected)
	{
		m_iPodSDKAgent->m_pfnReleaseAudioDSP();
		m_bConnected = FALSE;
	}

	if (m_biPodInited)
	{
		m_iPodSDKAgent->m_pfnIPOD_DeInit();
		m_biPodInited = FALSE;
	}

	::PostMessage(m_hMainWnd, UI_MSG_USB_IPOD_CONNECT, NULL, 0);
	RETAILMSG(1, (L"[navi_car] IPOD plug out...\n"));
}

BOOL CIPODManager::IsIPODSDKInitlized()
{
	return m_bInit;
}

BOOL CIPODManager::InitIPodSDK(HWND hWnd)
{
	BOOL bRet = FALSE;

	if (m_bInit)
	{
		RETAILMSG(1, (L"[navi_car]: warning ~ Don't call InitIPodSDK() multi-times.\n"));
		return TRUE;
	}

	m_hMainWnd=hWnd;
	m_iPodSDKAgent = new CiPodSDKAgent;
	if (m_iPodSDKAgent==NULL || !m_iPodSDKAgent->Init())
	{
		goto LEXIT;
	}

	// Queue Notify Event
	m_hEvtQueueNotify = CreateEvent(NULL, FALSE, FALSE, QUEUE_NOTIFY_EVENT_NAME);
	if (m_hEvtQueueNotify == NULL)
	{
		RETAILMSG (1, (TEXT("[navi_car]: Create Event for IPOD_QUEUE_NOTIFY Failed.\n")));
		goto LEXIT;
	}

	// Create Message Queue
	MSGQUEUEOPTIONS mqOptions;
	memset(&mqOptions, 0, sizeof(mqOptions));
	mqOptions.dwFlags       = MSGQUEUE_ALLOW_BROKEN;
	mqOptions.dwSize        = sizeof(mqOptions);
	mqOptions.dwMaxMessages = 256;
	mqOptions.cbMaxMessage  = QUEUE_ITEM_SIZE;
	mqOptions.bReadAccess   = TRUE;
	m_hReadMsgQueue			= CreateMsgQueue(_T("MTKIPODMSGQUE"), &mqOptions);
	if (m_hReadMsgQueue == NULL)
	{
		RETAILMSG (1, (TEXT("[navi_car]: Create IPOD Notify Message Queue Failed...\n")));
		goto LEXIT;
	}

	// Create Receive Notify Thread
	m_bExitNotifyThread = FALSE;
	m_hEvtNotifyThreadExit = CreateEvent( NULL, FALSE, FALSE, NULL );
	m_hNotifyThread = CreateThread(NULL, 0, NotifyThreadProc, (LPVOID)this, 0, NULL); 
	if (m_hNotifyThread == NULL)
	{
		RETAILMSG (1, (TEXT("[navi_car]: Create IPOD Notify Thread Failed..\n")));
		goto LEXIT;
	}

	m_bInit = TRUE;
	bRet = TRUE;

LEXIT:
	if (!bRet)
	{
		if (m_iPodSDKAgent)
		{
			delete m_iPodSDKAgent;
			m_iPodSDKAgent = NULL;
		}

		if (m_hEvtQueueNotify)
		{
			CloseHandle(m_hEvtQueueNotify);
			m_hEvtQueueNotify = NULL;
		}
	}

	return bRet;

}

void CIPODManager::ConnectDevice()
{
	if (m_bConnected)
	{
		KillTimer(m_hMainWnd, TIMER_CONNECTDEVICE);
		return;
	}

	if(!m_biPodInited && !(m_biPodInited = m_iPodSDKAgent->m_pfnIPOD_Init()))	// 未初始化且初始化失败了
	{
		RETAILMSG(1, (L"[navi_car]: m_pfnIPOD_Init() fail. \n"));
		return;
	}

	MUSB_AUTH_RESULT_T rAuthResult;
	if (RET_IPOD_OK == m_iPodSDKAgent->m_pfnGetAuthStatus(&rAuthResult))
	{
		if (rAuthResult.u4AuthResult == IPOD_AUTHENTICATION_PASS)
		{
			if(RET_IPOD_OK != m_iPodSDKAgent->m_pfnEnterRemoteUIMode())	
			{
				return;
			}

			UINT32 u4RemoteEventMask = 0x0000800A;
			m_iPodSDKAgent->m_pfnSetRemoteEvent(u4RemoteEventMask);

			GetiPodArtworkFormats();
			CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnStartPlayAudio();

			::PostMessage(m_hMainWnd, UI_MSG_USB_IPOD_CONNECT, NULL, 1);
			m_bConnected = TRUE;

			KillTimer(m_hMainWnd,TIMER_CONNECTDEVICE);
			RETAILMSG(1, (_T("[navi_car]: IPOD connect successful. \n")));
		}
	}
}


void CIPODManager::HandleiPodStatusChanged(UINT32 u4IPodStatus)
{
	if (u4IPodStatus == IPOD_STATUS_PLUG_IN)
	{
		OnPlugIn();
	}
	else if (u4IPodStatus == IPOD_STATUS_PLUG_OUT)
	{
		::SendMessage(HWND_BROADCAST,WM_IPOD_OUTMSG,0,m_bInsertPowerOn);
		OnPlugOut();
	}

}
void    CIPODManager::HandlePlaybackStatusChanged(UINT32 u4Status)
{


	if (u4Status == PB_ST_EX_STOP_0X00)
	{
	}
	else if (u4Status == PB_ST_EX_PLAY_0X01)
	{
	}
	else if (u4Status == PB_ST_EX_PAUSE_0X02)
	{
	}
	else if (u4Status == PB_ST_EX_PF_0X03)
	{
	}
	else if (u4Status == PB_ST_EX_FR_0X04)
	{
	}
}
bool    CIPODManager::GetiPodArtworkFormats()
{
	if (!m_iPodSDKAgent->IsInit())
		return false;
#ifdef _WIN32_WCE
	CALL_IPOD_API_BOOL(m_iPodSDKAgent->m_pfnGetArtWorkFormats((void*)&m_rArtWorkFmtInfo));
	// Sort them (Largest --> Smallest)
	UINT32 i, j;
	MUSB_IPOD_ARTWORK_FMTS rTempArtWorkFmts;
	for(i = 0; i < m_rArtWorkFmtInfo.u4FmtNum; i++)
	{
		for(j = (i + 1); j < m_rArtWorkFmtInfo.u4FmtNum; j++)
		{
			if(m_rArtWorkFmts[i].u2ImgWidth < m_rArtWorkFmts[j].u2ImgWidth)
			{
				rTempArtWorkFmts.u1PixFormat = m_rArtWorkFmts[i].u1PixFormat;
				rTempArtWorkFmts.u2FormatID = m_rArtWorkFmts[i].u2FormatID;
				rTempArtWorkFmts.u2ImgWidth = m_rArtWorkFmts[i].u2ImgWidth;
				rTempArtWorkFmts.u2ImgHeight = m_rArtWorkFmts[i].u2ImgHeight;

				m_rArtWorkFmts[i].u1PixFormat = m_rArtWorkFmts[j].u1PixFormat;
				m_rArtWorkFmts[i].u2FormatID = m_rArtWorkFmts[j].u2FormatID;
				m_rArtWorkFmts[i].u2ImgWidth = m_rArtWorkFmts[j].u2ImgWidth;
				m_rArtWorkFmts[i].u2ImgHeight = m_rArtWorkFmts[j].u2ImgHeight;

				m_rArtWorkFmts[j].u1PixFormat = rTempArtWorkFmts.u1PixFormat;
				m_rArtWorkFmts[j].u2FormatID = rTempArtWorkFmts.u2FormatID;
				m_rArtWorkFmts[j].u2ImgWidth = rTempArtWorkFmts.u2ImgWidth;
				m_rArtWorkFmts[j].u2ImgHeight = rTempArtWorkFmts.u2ImgHeight;
			}
		}
	}

	m_u4MaxArtworkHeight = m_rArtWorkFmts[0].u2ImgHeight;
#endif
	return true;
}
bool    CIPODManager::CreateArtworkDataBuffer()
{
	if (m_pbTrackArtworkData)
	{
		delete []m_pbTrackArtworkData;
		m_pbTrackArtworkData = NULL;
	}


	m_pbTrackArtworkData = new BYTE[m_u4MaxArtworkHeight * m_u4MaxArtworkHeight * 2];
	return true;
}
bool    CIPODManager::HandleTrackArtwork(int index,UINT8 **puId3Pic, UINT32 *u4PicLen,UINT32 *u4PicWith,UINT32 *u4PicHeight)
{

	UINT32 i = 0;
	UINT32 j = 0;
	if (!m_iPodSDKAgent->IsInit())
		return false;
	// Step1. Get Current Track Artwork Information
	UINT32 u4ArtworkCount = 0;
	MUSB_IPOD_ARTWORK_CNT_T		  rArtWorkCnt[MAX_ARTWORK_INFO_NUM];
	MUSB_IPOD_ARTWORK_CNT_INFO_T  rArtWorkCntInfo;
	rArtWorkCntInfo.u2InfoNum = 0;
	rArtWorkCntInfo.prArtworkCnt = &rArtWorkCnt[0];

	CALL_IPOD_API_BOOL(m_iPodSDKAgent->m_pfnGetIndexedPlayingTrackInfo(index, 0, TRACK_ARTWORK_CNT, (void*)&rArtWorkCntInfo));
	for (UINT16 i = 0; i < rArtWorkCntInfo.u2InfoNum; i++)
	{
		RETAILMSG(1, (_T(" === Artwork(%d): FormatID=0x%04X, Count=%d ===\n"), i, rArtWorkCnt[i].u2FormatID, rArtWorkCnt[i].u2ArtWorkCnt));
		u4ArtworkCount +=  rArtWorkCnt[i].u2ArtWorkCnt;
	}

	if (u4ArtworkCount == 0)
	{

		*u4PicLen=0;
		*u4PicLen=NULL;
		return TRUE;
	}
	// Step2. Select the Largest Artwork
	UINT32 u4FmtsIndex = 0;
	BOOL bFound = FALSE;
	for (i = 0; i < m_rArtWorkFmtInfo.u4FmtNum; i++)
	{
		for (j = 0; j < rArtWorkCntInfo.u2InfoNum; j++)
		{
			if ((rArtWorkCnt[j].u2FormatID == m_rArtWorkFmts[i].u2FormatID) &&
				(rArtWorkCnt[j].u2ArtWorkCnt != 0))
			{
				u4FmtsIndex = i;
				bFound = TRUE;
				break;
			}
		}

		if (bFound)
		{
			break;
		}
	}

	// Step3. Get Track Artwork Times
	UINT32 u4MsTimePos[MAX_ARTWORK_TIME_NUM];
	MUSB_IPOD_ARTWORK_TIMES_T rArtWorkTimes;
	rArtWorkTimes.u4InfoNum = 0;
	rArtWorkTimes.pi4MsTimePos = &u4MsTimePos[0];
	UINT16 u2ArtWorkIndex = 0;
	UINT16 u2ArtWorkCount = MAX_ARTWORK_TIME_NUM;
	UINT32 u2FmtID = m_rArtWorkFmts[u4FmtsIndex].u2FormatID;

	MUSB_IPOD_ARTWORK_DATA_T rArtWorkData;

	CALL_IPOD_API_BOOL(m_iPodSDKAgent->m_pfnGetTrackArtWorkTimes(index, u2FmtID, u2ArtWorkIndex, u2ArtWorkCount, (void*)&rArtWorkTimes));
	RETAILMSG(1, (_T("====== GetArtWorkTimes %d ======\n"), rArtWorkTimes.u4InfoNum));
	if(rArtWorkTimes.u4InfoNum == 0)
	{
		*u4PicLen=0;
		*u4PicLen=NULL;

		return TRUE;
	}
	else
	{
		for (j = 0; j < rArtWorkTimes.u4InfoNum; j++)
		{
			m_u4ArtworkDataSize = m_rArtWorkFmts[u4FmtsIndex].u2ImgHeight * m_rArtWorkFmts[u4FmtsIndex].u2ImgWidth * 2;
			CreateArtworkDataBuffer();
			rArtWorkData.pvData = m_pbTrackArtworkData;
			rArtWorkData.u4DataBufSize = m_u4ArtworkDataSize;
			// Step4. Get Artwork Data
			RETAILMSG(1, (L"++++++++++++ m_pfnGetTrackArtWorkData. begin \n"));
			CALL_IPOD_API_BOOL(m_iPodSDKAgent->m_pfnGetTrackArtWorkData(index, u2FmtID, u4MsTimePos[j], (void*)&rArtWorkData));
			RETAILMSG(1, (L"++++++++++++ m_pfnGetTrackArtWorkData. END \n"));
			// Step5. Show Artwork
			//SaveArtwork();
			//	ShowArtwork(m_rArtWorkFmts[u4FmtsIndex].u2ImgWidth, m_rArtWorkFmts[u4FmtsIndex].u2ImgHeight);
			//	ShowArtwork();
			goto END_SHOW_ARTWORK;
		}
	}

END_SHOW_ARTWORK:
	*u4PicLen=m_u4ArtworkDataSize;
	*puId3Pic=m_pbTrackArtworkData;
	*u4PicWith=m_rArtWorkFmts[u4FmtsIndex].u2ImgWidth;
	*u4PicHeight= m_rArtWorkFmts[u4FmtsIndex].u2ImgHeight;

	return TRUE;
}

DWORD WINAPI CIPODManager::NotifyThreadProc(LPVOID lpParm)
{
	DWORD dwWaitVal = 0;
	MUSB_IPOD_NOTIFY_INFO_T rIPodNotify;
	DWORD dwRead = 0;
	DWORD dwFlags = 0;
	CIPODManager* pIPODManager = reinterpret_cast<CIPODManager*>(lpParm);

	// 底层出错后,会通知该事件,这时需重新插拔ipod才能恢复正常
//	HANDLE hIpodQuitEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("IPOD_EVENT_QUIT"));

	HANDLE hNotify[2] = {pIPODManager->m_hReadMsgQueue, pIPODManager->m_hEvtNotifyThreadExit};

	while(1)
	{
		dwWaitVal = WaitForMultipleObjects(2, hNotify, FALSE, INFINITE);
		if (dwWaitVal == WAIT_OBJECT_0  + 1)
		{
			RETAILMSG(TRUE, (_T("[navi_car] iPod Notify Thread exit...\n") ) );
			break;
		}

		if (!ReadMsgQueue(pIPODManager->m_hReadMsgQueue, (LPVOID)&rIPodNotify,
			sizeof(MUSB_IPOD_NOTIFY_INFO_T), &dwRead, 0, &dwFlags))
		{
			RETAILMSG(TRUE,(_T("[iPod Notify Thread] ReadMsgQueue Failed...\n")));
			continue;
		}

		switch(rIPodNotify.eNotifyType)
		{
		case IPOD_AUTH_STATE:
			RETAILMSG (1, (TEXT("------> Receive Auth State Notify (%d) <------\n"), rIPodNotify.rNotify.rAuthResult.u4AuthResult));
			break;
		case IPOD_PB_STATUS:
			// Play Track Offset
			if(rIPodNotify.rNotify.rPbInfo.fgTrackSecOffsetValid)
			{
				UINT32 pos = rIPodNotify.rNotify.rPbInfo.u4TrackSecOffset;
				PostMessage(pIPODManager->m_hMainWnd, UI_MSG_USB_IPOD_PB_POS_CHANGED, 0, (LPARAM)pos);
			}

			// Track Index Changed
			if(rIPodNotify.rNotify.rPbInfo.fgTrackIndexValid)
			{
				UINT32 u4TrackIndex = rIPodNotify.rNotify.rPbInfo.u4TrackIndex;
				PostMessage(pIPODManager->m_hMainWnd, UI_MSG_USB_IPOD_PB_TRACK_CHANGED, 0, (LPARAM)u4TrackIndex);
			}

			// Play Status 
			if(rIPodNotify.rNotify.rPbInfo.fgPbStatuValid)
			{
				MUSB_IPOD_PLAY_ST_EX_E eStatus = rIPodNotify.rNotify.rPbInfo.eStatus;
				PostMessage(pIPODManager->m_hMainWnd, UI_MSG_USB_IPOD_PB_STATUS_CHANGED, 0, (LPARAM)eStatus);
			}

			// Track UID
			if(rIPodNotify.rNotify.rPbInfo.fgTrackUIDValid)
			{
				UINT64 u8TrackUID = rIPodNotify.rNotify.rPbInfo.u8TrackUID;
				RETAILMSG (1, (TEXT("------> Receive Track UID Notify: %ld <------\n"), u8TrackUID));
			}

			break;

		case IPOD_DIGITALAUIDO_CFG:
			RETAILMSG (1, (TEXT("------> Receive Audio Config Notify <------\n")));
			break;

		case IPOD_STATUS_NOTIFY:
			{
				if (rIPodNotify.rNotify.eiPodStatus == IPOD_STATUS_PLUG_IN)
				{
					RETAILMSG (1, (TEXT("------> Receive iPod Status Notify: PLUG_IN <------\n")));
				}
				else if (rIPodNotify.rNotify.eiPodStatus == IPOD_STATUS_PLUG_OUT)
				{
					RETAILMSG (1, (TEXT("------> Receive iPod Status Notify: PLUG_OUT <------\n")));
				}

				pIPODManager->HandleiPodStatusChanged(rIPodNotify.rNotify.eiPodStatus);

				break;
			}

		default:
			break;
		}
	}
	return 1;
}


bool  CIPODManager::PostIPodMessage(UINT Msg,
									WPARAM wParam,
									LPARAM lParam)
{
	//for(int i=0;i<m_vechWnd.size();i++)
	{
		::SendMessage(HWND_BROADCAST,Msg,wParam,lParam);
	}
	return true;
}


void  CIPODManager::RegisterWindow(wstring strWindowName)
{
	HWND  hWindow=FindWindow(NULL,strWindowName.c_str());
	m_vechWnd.push_back(hWindow);
}
void CIPODManager::UnRegisterWindow(wstring strWindowName)
{
	HWND  hWindow=FindWindow(NULL,strWindowName.c_str());
	for(int i=0;i<m_vechWnd.size();i++)
	{
		if (m_vechWnd[i]==hWindow)
		{
			m_vechWnd.erase(m_vechWnd.begin()+i);
		}
	}

}
bool    CIPODManager::ReadDbCategory(UINT8 u1SelectDbCategory, UINT8 u1QueryDbCategory, int nRecIndex)
{

	if (!m_bConnected)
		return false;

	UINT32 u4RecCnt = 1;
	MUSB_IPOD_DB_RecItem_T	rRecItem;

	if(u1QueryDbCategory==0x1)
	{

		if (RET_IPOD_OK !=CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnRetrieveCategorizedDBRecords(u1SelectDbCategory, nRecIndex, u4RecCnt, (void*)&rRecItem))
		{

			return false;
		}

	}

	else

	{
		if (RET_IPOD_OK !=CIPODManager::GetInstance()->GetIPodCtrl()->m_pfnRetrieveCategorizedDBRecords(u1QueryDbCategory, nRecIndex, u4RecCnt, (void*)&rRecItem))
		{

			return false;
		}
	}





	m_pServceData->SendBuffer(&rRecItem,sizeof(MUSB_IPOD_DB_RecItem_T));



	return true;
}
UINT    CIPODManager::UpdataDbCategory(UINT8 u1SelectDbCategory, UINT8 u1QueryDbCategory, int nRecIndex)
{

	if (!m_bConnected)
		return false;
	if (!m_iPodSDKAgent->IsInit())
		return false;

	if(u1QueryDbCategory==0x1)
	{

		MUSB_IPOD_DB_RecItem_T	rRecItem;
		UINT32 u4RecCnt = 1;
		m_iPodSDKAgent->m_pfnResetDBSelection();

		UINT32  un4NumRecords = m_iPodSDKAgent->m_pfnGetNumberCategorizedDBRecords(u1SelectDbCategory);
		return  un4NumRecords;

	}
	else  //点击某个选项后进入下一级目录            
	{
		m_iPodSDKAgent->m_pfnSelectDBRecord(u1SelectDbCategory,nRecIndex);
		UINT32  un4NumRecords = m_iPodSDKAgent->m_pfnGetNumberCategorizedDBRecords(u1QueryDbCategory);
		return  un4NumRecords;

	}

}

void   CIPODManager::GetPlayStatu()
{
	if (!m_bConnected)
		return ;  
	if (!m_iPodSDKAgent->IsInit())
		return ;
	MUSB_IPOD_PLAY_ST_INFO_T rPbStatus;
	if (RET_IPOD_OK ==m_iPodSDKAgent->m_pfnGetPlayStatus(&rPbStatus))
		m_pServceData->SendBuffer(&rPbStatus,sizeof(MUSB_IPOD_PLAY_ST_INFO_T));
	else
		return;

}

UINT    CIPODManager::GetTrackID3Tag()
{
	if (!m_bConnected)
		return false;
	if (!m_iPodSDKAgent->IsInit())
		return false;
	PLAYBACK_ID3TAG  TrackID3Tag;
	UINT unPlaybackTrackIndex=0;
	if (RET_IPOD_OK !=m_iPodSDKAgent->m_pfnGetCurPlayingTrackIdx(&unPlaybackTrackIndex))
		return -1;
	TrackID3Tag.u4RecIdx=unPlaybackTrackIndex;

	if (RET_IPOD_OK !=m_iPodSDKAgent->m_pfnGetIndexedPlayingTrackTitle(unPlaybackTrackIndex, TrackID3Tag.TrackTitle))
		return -1;
	if (RET_IPOD_OK !=m_iPodSDKAgent->m_pfnGetIndexedPlayingTrackArtist(unPlaybackTrackIndex,TrackID3Tag.TrackArtist))
		return -1;

	if (RET_IPOD_OK !=m_iPodSDKAgent->m_pfnGetIndexedPlayingTrackAlbum(unPlaybackTrackIndex, TrackID3Tag.TrackAlbum))
		return -1;

	m_pServceData->SendBuffer(&TrackID3Tag,sizeof(PLAYBACK_ID3TAG));

	return  unPlaybackTrackIndex;

}

BOOL CIPODManager::GetTrackArtWork(UINT index, UIPOD_ARTWORKDATA* pdata)
{ 
	if (m_bConnected && pdata)
	{
		RETAILMSG(1, (L"++++++++++++ HandleTrackArtwork. begin \n"));
		if (HandleTrackArtwork(index, &(pdata->pImageData), &pdata->u4ImageLen,
			&pdata->u4PicWith, &pdata->u4PicHeight))
		{
			return TRUE;
		}
		RETAILMSG(1, (L"++++++++++++ HandleTrackArtwork. END \n"));
	}

	return FALSE;
}

void CIPODManager::SetSinkOutput(SWI_SINK_TYPE_T outPut)
{
	if (!m_iPodSDKAgent->IsInit())
		return ;

	if (m_bConnected)
	{
		RETAILMSG(1, (L"CIPODManager::=%d\n",outPut));
		m_iPodSDKAgent->m_pfnSetADecSinkInfo(outPut);
	}

}

/////////////////////////////////////////////////////////////////
void CIPODManager::LaunchSrc(DVP_SINK sink_type)
{
	if (m_bConnected)
	{
		m_iPodSDKAgent->m_pfnGetAudioDSP();
		m_iPodSDKAgent->m_pfnSetADecSinkInfo((SWI_SINK_TYPE_T)sink_type);
	}
}

void CIPODManager::StopSrc()
{
	if (m_bConnected)
	{
		MUSB_IPOD_PLAY_ST_INFO_T rPbStatus;
		ZeroMemory(&rPbStatus, sizeof(MUSB_IPOD_PLAY_ST_INFO_T));
		if(m_iPodSDKAgent->m_pfnGetPlayStatus(&rPbStatus) == RET_IPOD_OK
			&& rPbStatus.ePbStatus==PB_ST_EX_PLAY_0X01)
		{
			m_iPodSDKAgent->m_pfnPlayControl(PLAY_CTRL_PLAY_PAUSE);
		}  

		m_iPodSDKAgent->m_pfnReleaseAudioDSP();
	}
}

void CIPODManager::SetSink(DVP_SINK sink_type)
{
	if (m_bConnected)
	{
		m_iPodSDKAgent->m_pfnSetADecSinkInfo((SWI_SINK_TYPE_T)sink_type);
	}
}

void CIPODManager::ReleaseAudioDsp()
{
	if (m_bConnected)
	{
		m_iPodSDKAgent->m_pfnReleaseAudioDSP();
	}
}

void CIPODManager::GetAudioDsp()
{
	if (m_bConnected)
	{
		m_iPodSDKAgent->m_pfnGetAudioDSP();
	}
}


MUSB_IPOD_DB_CATEGORY_TYPE_E CIPODManager::GetDBType(MUSB_IPOD_DB_CATEGORY_TYPE_E type, UINT sub_type)
{
	MUSB_IPOD_DB_CATEGORY_TYPE_E ret_type;
	if (type == DB_TYPE_GENRE)
	{
		if (sub_type == DB_SUB_TYPE_GENRE_GENRE)
		{
			ret_type = DB_TYPE_GENRE;
		}
		else if (sub_type == DB_SUB_TYPE_GENRE_ARTIST)
		{
			ret_type = DB_TYPE_ARTIST;
		}
		else if (sub_type == DB_SUB_TYPE_GENRE_ALBUM)
		{
			ret_type = DB_TYPE_ALBUM;
		}
		else if (sub_type == DB_SUB_TYPE_GENRE_TRACK)
		{
			ret_type = DB_TYPE_TRACK;
		}
	}
	// ============================== Artist ==============================
	else if (type == DB_TYPE_ARTIST)
	{
		if (sub_type == DB_SUB_TYPE_ARTIST_ARTIST)
		{
			ret_type = DB_TYPE_ARTIST;
		}
		else if (sub_type == DB_SUB_TYPE_ARTIST_ALBUM)
		{
			ret_type = DB_TYPE_ALBUM;
		}
		else if (sub_type == DB_SUB_TYPE_ARTIST_TRACK)
		{
			ret_type = DB_TYPE_TRACK;
		}
	}
	// ============================== Album ==============================
	else if (type == DB_TYPE_ALBUM)
	{
		if (sub_type == DB_SUB_TYPE_ALBUM_ALBUM)
		{
			ret_type = DB_TYPE_ALBUM;
		}
		else if (sub_type == DB_SUB_TYPE_ALBUM_TRACK)
		{
			ret_type = DB_TYPE_TRACK;
		}
	}
	////////////////////////////////// Tack //////////////////////////////
	else if (type == DB_TYPE_TRACK)
	{
		if (sub_type == DB_SUB_TYPE_TRACK_TRACK)
		{
			ret_type = DB_TYPE_TRACK;
		}

	}
	else if (type == DB_TYPE_PLAYLIST)
	{
		if (sub_type == DB_SUB_TYPE_PLAYLIST_PLAYLIST)
		{
			ret_type = DB_TYPE_PLAYLIST;
		}
		else if (sub_type == DB_SUB_TYPE_PLAYLIST_TRACK)
		{
			ret_type = DB_TYPE_TRACK;
		}
	}

	return ret_type;
}