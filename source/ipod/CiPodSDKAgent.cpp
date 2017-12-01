#include "StdAfx.h"
#include "CiPodSDKAgent.h"

CiPodSDKAgent::CiPodSDKAgent(void)
{
    m_hIPodSDKDll = NULL;
    ResetAllFuncPointers();
    m_bInited = FALSE;
}

CiPodSDKAgent::~CiPodSDKAgent(void)
{
    Deinit();
}

void CiPodSDKAgent::ResetAllFuncPointers()
{
    pfnIPOD_Init = NULL;
    pfnIPOD_DeInit = NULL;

    pfnStartPlayAudio = NULL;
    pfnStopPlayAudio = NULL;
    pfnGetAudioDSP = NULL;
    pfnReleaseAudioDSP = NULL;

    pfnEnterRemoteUIMode = NULL;
    pfnGetPlayStatus = NULL;
    pfnExitRemoteUIMode = NULL;
    pfnResetDBSelection = NULL;
    pfnSetRepeatMode = NULL;
    pfnGetNumEQProfile = NULL;
    pfnGetCurEQProfileIndex = NULL;
    pfnSetShuffleMode = NULL;
    pfnPlayCurrentSelection = NULL;
    pfnSetCurrentPlayingTrack = NULL;
    pfnSelectDBRecord = NULL;
    pfnSelectSortDBRecord = NULL;
    pfnGetNumberCategorizedDBRecords = NULL;
    pfnRetrieveCategorizedDBRecords = NULL;
    pfnGetShuffleSetting = NULL;
    pfnGetRepeatSetting = NULL;
    pfnGetCurPlayingTrackIdx = NULL;
    pfnGetIndexedPlayingTrackTitle = NULL;
    pfnGetIndexedPlayingTrackArtist = NULL;
    pfnGetIndexedPlayingTrackAlbum = NULL;
    pfnRequestLingoProtocolVersion = NULL;                         
    pfnGetCurPlayingTrackChapterInfo = NULL;
    pfnGetDbTrackInfo = NULL;
    pfnGetPlayStatus_Ex = NULL;
    pfnGetTrackArtWorkData = NULL;
    pfnGetArtWorkFormats = NULL;
    pfnGetAuthStatus = NULL;
    pfnPlayControl = NULL;
    pfnGetiPodPreferences = NULL;
    pfnSetiPodPreferences = NULL;
    pfnSetVolume = NULL;
    pfnGetVolume = NULL;
    pfnSetRemoteEvent = NULL;
    pfnSetPlayStatusChangeNotification = NULL;
    pfnGetIndexedPlayingTrackInfo = NULL;
    pfnGetTrackArtWorkTimes = NULL;
    pfnGetNumPlayingTracks = NULL;
    pfnResetDBSelectionHierarchy = NULL;
    pfnSetVideoDelay = NULL;
    pfnGetiPodOptionsForLingo = NULL;
    pfnGetiPodOptions = NULL;
    pfnSetADecSinkInfo = NULL;
}

BOOL CiPodSDKAgent::Init()
{
    BOOL bRet = TRUE;
    if (m_bInited)
    {
        return TRUE;
    }

	Lock();
    m_hIPodSDKDll = LoadLibrary(IPOD_SDK_DLL_NAME);
    if (m_hIPodSDKDll == NULL)
    {
        Unlock();
        return FALSE;
    }

    // Get Function Pointers
    pfnIPOD_Init = (TYPE_MUSB_IPOD_Init) GetProcAddress(m_hIPodSDKDll, _T("MUSB_IPOD_Init"));

    pfnIPOD_DeInit = (TYPE_MUSB_IPOD_DeInit) GetProcAddress(m_hIPodSDKDll, _T("MUSB_IPOD_DeInit"));

    pfnStartPlayAudio = (TYPE_MUSB_IPOD_StartPlayAudio)GetProcAddress(m_hIPodSDKDll, _T("MUSB_IPOD_StartPlayAudio"));

    pfnStopPlayAudio = (TYPE_MUSB_IPOD_StopPlayAudio)GetProcAddress(m_hIPodSDKDll, _T("MUSB_IPOD_StopPlayAudio"));

    pfnGetAudioDSP = (TYPE_MUSB_IPOD_GetAudioDSP)GetProcAddress(m_hIPodSDKDll, _T("MUSB_IPOD_GetAudioDSP"));

    pfnReleaseAudioDSP = (TYPE_MUSB_IPOD_ReleaseAudioDSP)GetProcAddress(m_hIPodSDKDll, _T("MUSB_IPOD_ReleaseAudioDSP"));

    pfnEnterRemoteUIMode = (TYPE_MUSB_IPOD_EnterRemoteUIMode)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_EnterRemoteUIMode"));

    pfnGetPlayStatus = (TYPE_MUSB_IPOD_GetPlayStatus)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetPlayStatus"));

    pfnExitRemoteUIMode = (TYPE_MUSB_IPOD_ExitRemoteUIMode)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_ExitRemoteUIMode"));

    pfnResetDBSelection = (TYPE_MUSB_IPOD_ResetDBSelection)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_ResetDBSelection"));

    pfnSetRepeatMode = (TYPE_MUSB_IPOD_SetRepeatMode)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_SetRepeatMode"));

    pfnGetNumEQProfile = (TYPE_MUSB_IPOD_GetNumEQProfile)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetNumEQProfile"));

    pfnGetCurEQProfileIndex = (TYPE_MUSB_IPOD_GetCurEQProfileIndex)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetCurEQProfileIndex"));

    pfnSetShuffleMode = (TYPE_MUSB_IPOD_SetShuffleMode)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_SetShuffleMode"));

    pfnPlayCurrentSelection = (TYPE_MUSB_IPOD_PlayCurrentSelection)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_PlayCurrentSelection"));

    pfnSetCurrentPlayingTrack = (TYPE_MUSB_IPOD_SetCurrentPlayingTrack)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_SetCurrentPlayingTrack"));

    pfnSelectDBRecord = (TYPE_MUSB_IPOD_SelectDBRecord)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_SelectDBRecord"));

    pfnSelectSortDBRecord = (TYPE_MUSB_IPOD_SelectSortDBRecord)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_SelectSortDBRecord"));

    pfnGetNumberCategorizedDBRecords = (TYPE_MUSB_IPOD_GetNumberCategorizedDBRecords)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetNumberCategorizedDBRecords"));

    pfnRetrieveCategorizedDBRecords = (TYPE_MUSB_IPOD_RetrieveCategorizedDBRecords)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_RetrieveCategorizedDBRecords"));

    pfnGetShuffleSetting = (TYPE_MUSB_IPOD_GetShuffleSetting)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetShuffleSetting"));

    pfnGetRepeatSetting = (TYPE_MUSB_IPOD_GetRepeatSetting)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetRepeatSetting"));

    pfnGetCurPlayingTrackIdx = (TYPE_MUSB_IPOD_GetCurPlayingTrackIdx)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetCurPlayingTrackIdx"));

    pfnGetIndexedPlayingTrackTitle = (TYPE_MUSB_IPOD_GetIndexedPlayingTrackTitle)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetIndexedPlayingTrackTitle"));

    pfnGetIndexedPlayingTrackArtist = (TYPE_MUSB_IPOD_GetIndexedPlayingTrackArtist)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetIndexedPlayingTrackArtist"));

    pfnGetIndexedPlayingTrackAlbum = (TYPE_MUSB_IPOD_GetIndexedPlayingTrackAlbum)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetIndexedPlayingTrackAlbum"));

    pfnRequestLingoProtocolVersion = (TYPE_MUSB_IPOD_RequestLingoProtocolVersion)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_RequestLingoProtocolVersion"));   

    pfnGetCurPlayingTrackChapterInfo = (TYPE_MUSB_IPOD_GetCurPlayingTrackChapterInfo)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetCurPlayingTrackChapterInfo"));

    pfnGetDbTrackInfo = (TYPE_MUSB_IPOD_GetDbTrackInfo)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetDbTrackInfo"));

    pfnGetPlayStatus_Ex = (TYPE_MUSB_IPOD_GetPlayStatus_Ex)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetPlayStatus_Ex"));

    pfnGetTrackArtWorkData = (TYPE_MUSB_IPOD_GetTrackArtWorkData)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetTrackArtWorkData"));

    pfnGetArtWorkFormats = (TYPE_MUSB_IPOD_GetArtWorkFormats)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetArtWorkFormats"));

    pfnGetAuthStatus = (TYPE_MUSB_IPOD_GetAuthStatus)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetAuthStatus"));

    pfnPlayControl = (TYPE_MUSB_IPOD_PlayControl)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_PlayControl"));

    pfnGetiPodPreferences = (TYPE_MUSB_IPOD_GetiPodPreferences)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetiPodPreferences"));

    pfnSetiPodPreferences = (TYPE_MUSB_IPOD_SetiPodPreferences)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_SetiPodPreferences"));

    pfnSetVolume = (TYPE_MUSB_IPOD_SetVolume)GetProcAddress(m_hIPodSDKDll, _T("MUSB_IPOD_SetVolume"));

    pfnGetVolume = (TYPE_MUSB_IPOD_GetVolume)GetProcAddress(m_hIPodSDKDll, _T("MUSB_IPOD_GetVolume"));

    pfnSetRemoteEvent = (TYPE_MUSB_IPOD_SetRemoteEvent)GetProcAddress(m_hIPodSDKDll, _T("MUSB_IPOD_SetRemoteEvent"));

    pfnSetPlayStatusChangeNotification = (TYPE_MUSB_IPOD_SetPlayStatusChangeNotification)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_SetPlayStatusChangeNotification"));

    pfnGetIndexedPlayingTrackInfo = (TYPE_MUSB_IPOD_GetIndexedPlayingTrackInfo)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetIndexedPlayingTrackInfo"));

    pfnGetTrackArtWorkTimes = (TYPE_MUSB_IPOD_GetTrackArtWorkTimes)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetTrackArtWorkTimes"));

    pfnGetNumPlayingTracks = (TYPE_MUSB_IPOD_GetNumPlayingTracks)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetNumPlayingTracks"));

    pfnResetDBSelectionHierarchy	= (TYPE_MUSB_IPOD_ResetDBSelectionHierarchy)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_ResetDBSelectionHierarchy"));

    pfnSetVideoDelay	= (TYPE_MUSB_IPOD_SetVideoDelay)GetProcAddress(m_hIPodSDKDll, _T("MUSB_IPOD_SetVideoDelay"));

    pfnGetiPodOptionsForLingo	= (TYPE_MUSB_IPOD_GetiPodOptionsForLingo)GetProcAddress(m_hIPodSDKDll, 
        _T("MUSB_IPOD_GetiPodOptionsForLingo"));

    pfnGetiPodOptions	= (TYPE_MUSB_IPOD_GetiPodOptions)GetProcAddress(m_hIPodSDKDll, _T("MUSB_IPOD_GetiPodOptions"));

    pfnSetADecSinkInfo = (TYPE_MUSB_IPOD_SetADecSinkInfo)GetProcAddress(m_hIPodSDKDll, _T("MUSB_IPOD_SetADecSinkInfo"));


    // Check Function Pointers
    if ((pfnIPOD_Init == NULL) ||	(pfnIPOD_DeInit == NULL) ||
        (pfnStartPlayAudio == NULL) || (pfnStopPlayAudio == NULL) ||
        (pfnGetAudioDSP == NULL) || (pfnReleaseAudioDSP == NULL) ||
        (pfnEnterRemoteUIMode == NULL) || (pfnGetPlayStatus == NULL) ||
        (pfnExitRemoteUIMode == NULL) || (pfnResetDBSelection == NULL) ||
        (pfnSetRepeatMode == NULL) || (pfnGetNumEQProfile == NULL) ||
        (pfnSetShuffleMode == NULL) || (pfnGetCurEQProfileIndex == NULL) ||
        (pfnPlayCurrentSelection == NULL) || (pfnSetCurrentPlayingTrack == NULL) ||
        (pfnSelectDBRecord == NULL) || (pfnSelectSortDBRecord == NULL) ||
        (pfnGetNumberCategorizedDBRecords == NULL) || (pfnGetShuffleSetting == NULL) ||
        (pfnRetrieveCategorizedDBRecords == NULL) || (pfnGetRepeatSetting == NULL) ||
        (pfnGetCurPlayingTrackIdx == NULL) || (pfnGetIndexedPlayingTrackTitle == NULL) ||
        (pfnGetArtWorkFormats == NULL) || (pfnGetTrackArtWorkData == NULL) ||
        (pfnGetAuthStatus == NULL) || (pfnGetIndexedPlayingTrackArtist == NULL) ||
        (pfnGetIndexedPlayingTrackAlbum == NULL) || (pfnRequestLingoProtocolVersion == NULL) ||
        (pfnGetCurPlayingTrackChapterInfo == NULL) || (pfnGetDbTrackInfo == NULL) ||
        (pfnPlayControl == NULL) || (pfnGetiPodPreferences == NULL) ||
        (pfnSetiPodPreferences == NULL) || (pfnSetVolume == NULL) || (pfnGetVolume == NULL) ||
        (pfnSetRemoteEvent == NULL) || (pfnSetPlayStatusChangeNotification == NULL) ||
        (pfnGetIndexedPlayingTrackInfo == NULL) || (pfnGetTrackArtWorkTimes == NULL) ||
        (pfnGetNumPlayingTracks == NULL) || (pfnResetDBSelectionHierarchy == NULL) ||
        (pfnSetVideoDelay == NULL) || (pfnGetiPodOptionsForLingo == NULL) ||
        (pfnGetiPodOptions == NULL) || (pfnSetADecSinkInfo == NULL))
    {
        bRet = FALSE;
		m_bInited = FALSE;
	}
	else
	{
		m_bInited = TRUE;
	}

    RETAILMSG(0, (_T("==============================>>> m_hIPodSDKDll = 0x%08X <<<==============================\r\n"), m_hIPodSDKDll));
    Unlock();
    return  bRet;
}

BOOL CiPodSDKAgent::Deinit()
{
    Lock();
    m_bInited = FALSE;
    if (m_hIPodSDKDll)
    {
        if (pfnIPOD_DeInit)
        {
            pfnIPOD_DeInit();
        }
        FreeLibrary(m_hIPodSDKDll);
        m_hIPodSDKDll = NULL;
        ResetAllFuncPointers();
    }
    Unlock();
    return TRUE;
}

BOOL CiPodSDKAgent::ReInit()
{
    Deinit();
    return Init();
}

BOOL CiPodSDKAgent::m_pfnIPOD_Init()
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    BOOL bRet = FALSE;

    Lock();
    if (pfnIPOD_Init)
    {
        bRet = pfnIPOD_Init();
    }

    Unlock();
    return bRet;
}

BOOL CiPodSDKAgent::m_pfnIPOD_DeInit()
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    BOOL bRet = FALSE;

    Lock();
    if (pfnIPOD_DeInit)
    {
        bRet = pfnIPOD_DeInit();
    }
    Unlock();
    return bRet;
}

BOOL CiPodSDKAgent::m_pfnStartPlayAudio()
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    BOOL bRet = FALSE;
    Lock();
    if (pfnStartPlayAudio)
    {
        bRet = pfnStartPlayAudio();
    }
    Unlock();
    return bRet;
}

BOOL CiPodSDKAgent::m_pfnStopPlayAudio()
{
    BOOL bRet = FALSE;
    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    Lock();
    if (pfnStopPlayAudio)
    {
        bRet = pfnStopPlayAudio();
    }
    Unlock();

    return bRet;
}

BOOL CiPodSDKAgent::m_pfnGetAudioDSP()
{
    BOOL bRet = FALSE;
    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    Lock();
    if (pfnGetAudioDSP)
    {
        bRet = pfnGetAudioDSP();
    }
    Unlock();
    return bRet;
}
BOOL CiPodSDKAgent::m_pfnReleaseAudioDSP()
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    BOOL bRet = FALSE;
    Lock();
    if (pfnReleaseAudioDSP)
    {
        bRet = pfnReleaseAudioDSP();
    }
    Unlock();
    return bRet;
}

BOOL CiPodSDKAgent::m_pfnSetADecSinkInfo(SWI_SINK_TYPE_T eSink)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    BOOL bRet = FALSE;
    Lock();
    if (pfnSetADecSinkInfo)
    {
        bRet = pfnSetADecSinkInfo(eSink);
    }
    Unlock();
    return bRet;
}


UINT32 CiPodSDKAgent::m_pfnEnterRemoteUIMode()
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnEnterRemoteUIMode)
    {
        u4Ret = pfnEnterRemoteUIMode();
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetPlayStatus(MUSB_IPOD_PLAY_ST_INFO_T *prPbStatus)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetPlayStatus)
    {
        u4Ret = pfnGetPlayStatus(prPbStatus);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnExitRemoteUIMode()
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnExitRemoteUIMode)
    {
        u4Ret = pfnExitRemoteUIMode();
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnResetDBSelection()
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnResetDBSelection)
    {
        u4Ret = pfnResetDBSelection();
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnSetRepeatMode(UINT8 u1RepeatMode, BOOL fgRestore)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnSetRepeatMode)
    {
        u4Ret = pfnSetRepeatMode(u1RepeatMode, fgRestore);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetNumEQProfile(UINT32* pu4NumEQProfile)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetNumEQProfile)
    {
        u4Ret = pfnGetNumEQProfile(pu4NumEQProfile);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetCurEQProfileIndex(UINT32* pu4EQProfileIndex)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetCurEQProfileIndex)
    {
        u4Ret = pfnGetCurEQProfileIndex(pu4EQProfileIndex);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnSetShuffleMode(UINT8 u1ShuffleMode, BOOL fgRestore)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnSetShuffleMode)
    {
        u4Ret = pfnSetShuffleMode(u1ShuffleMode, fgRestore);
    }
    Unlock();
    return u4Ret;
}


UINT32 CiPodSDKAgent::m_pfnPlayCurrentSelection(UINT32 u4SelectTrackRecIndex)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnPlayCurrentSelection)
    {
        u4Ret = pfnPlayCurrentSelection(u4SelectTrackRecIndex);
    }
    Unlock();
    return u4Ret;
}
UINT32 CiPodSDKAgent::m_pfnSetCurrentPlayingTrack(UINT32 u4CurPlayingTrackIndex)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnSetCurrentPlayingTrack)
    {
        u4Ret = pfnSetCurrentPlayingTrack(u4CurPlayingTrackIndex);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnSelectDBRecord(UINT8 u1DbCategaroy, UINT32 u4RecIndex )
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnSelectDBRecord)
    {
        u4Ret = pfnSelectDBRecord(u1DbCategaroy, u4RecIndex);
    }
    Unlock();
    return u4Ret;
}
UINT32 CiPodSDKAgent::m_pfnSelectSortDBRecord(UINT8 u1DbCategaroy, UINT32 u4RecIndex, UINT8 u1SortOrder)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnSelectSortDBRecord)
    {
        u4Ret = pfnSelectSortDBRecord(u1DbCategaroy, u4RecIndex, u1SortOrder);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetNumberCategorizedDBRecords(UINT8 u1DbCategory)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetNumberCategorizedDBRecords)
    {
        u4Ret = pfnGetNumberCategorizedDBRecords(u1DbCategory);
    }
    Unlock();
    return u4Ret;
}


UINT32 CiPodSDKAgent::m_pfnRetrieveCategorizedDBRecords(UINT8 u1DbCategory,
                                                        UINT32 u4StartRecIdx, 
                                                        UINT32 u4RecCnt,
                                                        VOID *pvInfo)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnRetrieveCategorizedDBRecords)
    {
        u4Ret = pfnRetrieveCategorizedDBRecords(u1DbCategory, u4StartRecIdx, u4RecCnt, pvInfo);
    }
    Unlock();
    return u4Ret;
}


UINT32 CiPodSDKAgent::m_pfnGetShuffleSetting(UINT8 *pu1ShuffMode)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetShuffleSetting)
    {
        u4Ret = pfnGetShuffleSetting(pu1ShuffMode);
    }
    Unlock();
    return u4Ret;
}


UINT32 CiPodSDKAgent::m_pfnGetRepeatSetting(UINT8 *pu1RepeatMode)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetRepeatSetting)
    {
        u4Ret = pfnGetRepeatSetting(pu1RepeatMode);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetCurPlayingTrackIdx(UINT32 *pu4CurTrackIndex)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetCurPlayingTrackIdx)
    {
        u4Ret = pfnGetCurPlayingTrackIdx(pu4CurTrackIndex);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetIndexedPlayingTrackTitle(UINT32 u4PlaybackTrackIndex,
                                                       TCHAR *pszCurTrackTitle)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetIndexedPlayingTrackTitle)
    {
        u4Ret = pfnGetIndexedPlayingTrackTitle(u4PlaybackTrackIndex, pszCurTrackTitle);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetIndexedPlayingTrackArtist(UINT32 u4PlaybackTrackIndex, 
                                                        TCHAR *pszCurTrackArtist)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetIndexedPlayingTrackArtist)
    {
        u4Ret = pfnGetIndexedPlayingTrackArtist(u4PlaybackTrackIndex, pszCurTrackArtist);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetIndexedPlayingTrackAlbum(UINT32 u4PlaybackTrackIndex,
                                                       TCHAR *pszCurTrackAlbum)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetIndexedPlayingTrackAlbum)
    {
        u4Ret = pfnGetIndexedPlayingTrackAlbum(u4PlaybackTrackIndex, pszCurTrackAlbum);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnRequestLingoProtocolVersion(UINT8 u1LingoID,                                  
                                                       UINT32 *pu4Version)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnRequestLingoProtocolVersion)
    {
        u4Ret = pfnRequestLingoProtocolVersion(u1LingoID, pu4Version);
    }
    Unlock();
    return u4Ret;
}


UINT32 CiPodSDKAgent::m_pfnGetCurPlayingTrackChapterInfo(VOID *pvCharpterInfo)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetCurPlayingTrackChapterInfo)
    {
        u4Ret = pfnGetCurPlayingTrackChapterInfo(pvCharpterInfo);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetDbTrackInfo(UINT32 u4TrackDbStarIndex,
                                          UINT32 u4TrackCount,
                                          UINT8 u1TrackInfoBit,
                                          VOID *pvGotInfo)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetDbTrackInfo)
    {
        u4Ret = pfnGetDbTrackInfo(u4TrackDbStarIndex, u4TrackCount, u1TrackInfoBit, pvGotInfo);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetPlayStatus_Ex(MUSB_IPOD_PLAY_ST_INFO_EX_T *prPbStatus)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetPlayStatus_Ex)
    {
        u4Ret = pfnGetPlayStatus_Ex(prPbStatus);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetTrackArtWorkData(UINT32 u4TrackIndex,
                                               UINT16 u2FmtID,
                                               UINT32 u4MsTime,
                                               VOID *pvArtWorkData)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetTrackArtWorkData)
    {
        u4Ret = pfnGetTrackArtWorkData(u4TrackIndex, u2FmtID, u4MsTime, pvArtWorkData);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetArtWorkFormats(VOID *pvGotInfo) //MUSB_IPOD_ARTWORK_FMT
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetArtWorkFormats)
    {
        u4Ret = pfnGetArtWorkFormats(pvGotInfo);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetAuthStatus(MUSB_AUTH_RESULT_T *prAuthResult)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = RET_IPOD_FAIL;
    Lock();
    if (pfnGetAuthStatus)
    {
        u4Ret = pfnGetAuthStatus(prAuthResult);
    }
    Unlock();
    return u4Ret;
}

void CiPodSDKAgent::m_pfnPlayControl(UINT8 u1CtrlCode)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    Lock();
    if (pfnPlayControl)
    {
        pfnPlayControl(u1CtrlCode);
    }
    Unlock();
}

UINT32 CiPodSDKAgent::m_pfnGetiPodPreferences(UINT8 u1ClassID, MUSB_IPOD_PREFERENCES_T* prIPodPreferences)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetiPodPreferences)
    {
        u4Ret = pfnGetiPodPreferences(u1ClassID, prIPodPreferences);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnSetiPodPreferences(UINT8 u1ClassID, UINT8 u1SettingID)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnSetiPodPreferences)
    {
        u4Ret = pfnSetiPodPreferences(u1ClassID, u1SettingID);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnSetVolume(UINT32 u4Volume)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnSetVolume)
    {
        u4Ret = pfnSetVolume(u4Volume);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetVolume(UINT32* pu4Volume)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetVolume)
    {
        u4Ret = pfnGetVolume(pu4Volume);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnSetRemoteEvent(UINT32 u4EventMask)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnSetRemoteEvent)
    {
        u4Ret = pfnSetRemoteEvent(u4EventMask);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnSetPlayStatusChangeNotification(UINT32 u4NotificationType, UINT32 u4NotificationMask)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnSetPlayStatusChangeNotification)
    {
        u4Ret = pfnSetPlayStatusChangeNotification(u4NotificationType, u4NotificationMask);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetIndexedPlayingTrackInfo(UINT32 u4PlaybackTrackIndex, 
                                                      UINT32 u4ChapIndex, 
                                                      MUSB_TRACK_INF_TYPE_E eTrackInfoType, 
                                                      VOID* pInfo)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetIndexedPlayingTrackInfo)
    {
        u4Ret = pfnGetIndexedPlayingTrackInfo(u4PlaybackTrackIndex, u4ChapIndex, eTrackInfoType,pInfo);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetTrackArtWorkTimes(UINT32 u4TrackIndex,
                                                UINT16 u2FmtID,
                                                UINT16 u2ArtWorkIndex,
                                                UINT16 u2ArtWorkCount,
                                                VOID *pvGotInfo)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetTrackArtWorkTimes)
    {
        u4Ret = pfnGetTrackArtWorkTimes(u4TrackIndex, u2FmtID, u2ArtWorkIndex, u2ArtWorkCount, pvGotInfo);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetNumPlayingTracks(DWORD* pdwTracksNum)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetNumPlayingTracks)
    {
        u4Ret = pfnGetNumPlayingTracks(pdwTracksNum);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnResetDBSelectionHierarchy(UINT8 u1Hierarchy)
{
    UINT32 u4Ret = 0;
    Lock();
    if (pfnResetDBSelectionHierarchy)
    {
        u4Ret = pfnResetDBSelectionHierarchy(u1Hierarchy);
    }
    Unlock();
    return u4Ret;
}


UINT32 CiPodSDKAgent::m_pfnSetVideoDelay(UINT32 u4VideoDelayMs)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnSetVideoDelay)
    {
        u4Ret = pfnSetVideoDelay(u4VideoDelayMs);
    }
    Unlock();
    return u4Ret;
}

UINT32 CiPodSDKAgent::m_pfnGetiPodOptionsForLingo(UINT8 u1LingoID, UINT64* pu8Options)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetiPodOptionsForLingo)
    {
        u4Ret = pfnGetiPodOptionsForLingo(u1LingoID, pu8Options);
    }
    Unlock();
    return u4Ret;
}


UINT32 CiPodSDKAgent::m_pfnGetiPodOptions(UINT64* pu8Options)
{    //RETAILMSG(1, (L"%d\r\n", __LINE__));
    UINT32 u4Ret = 0;
    Lock();
    if (pfnGetiPodOptions)
    {
        u4Ret = pfnGetiPodOptions(pu8Options);
    }
    Unlock();
    return u4Ret;
}
void  CiPodSDKAgent::SetSuffle()
{
	if (!m_bInited)
	{
		return ;
	}
	m_pfnSetRepeatMode(2,true);
	Sleep(20);
	m_pfnSetShuffleMode(1,true);
}
void  CiPodSDKAgent::SetRptOne()
{
	if (!m_bInited)
	{
		return ;
	}
	m_pfnSetRepeatMode(1,true);
	Sleep(20);
   	m_pfnSetShuffleMode(0,true);
}
void  CiPodSDKAgent::SetRptAll()
{
	if (!m_bInited)
	{
		return ;
	}
	m_pfnSetRepeatMode(2,true);
	Sleep(20);
   	m_pfnSetShuffleMode(0,true);

}
BOOL CiPodSDKAgent::IsPlaying()
{
	if (!m_bInited)
	{
		return  false;
	}
	MUSB_IPOD_PLAY_ST_INFO_T rPbStatus;
	if (RET_IPOD_OK ==m_pfnGetPlayStatus(&rPbStatus))
	{
	   if (rPbStatus.ePbStatus==PB_ST_EX_PLAY_0X01)
	   {
		   return true;
	   }
	   else
		   return false;
	}
	return false;
}
int CiPodSDKAgent::GetCurPlayIndex()
{
	if (!m_bInited)
	{
		return -1;
	}
   UINT8 u1ShuffleMode=1;
   UINT8 u1RepeatMode=0;
   UINT u4PlaybackTrackIndex=0;


   m_pfnGetShuffleSetting(&u1ShuffleMode);	
	Sleep(50);

	if (u1ShuffleMode)
	{
        m_pfnSetShuffleMode(0,false);
		 Sleep(50);
         m_pfnGetCurPlayingTrackIdx(&u4PlaybackTrackIndex);
		 Sleep(50);
	    m_pfnSetShuffleMode(u1ShuffleMode,true);
		 Sleep(50); 
	}
	else
	{
      m_pfnGetCurPlayingTrackIdx(&u4PlaybackTrackIndex);
	}
   
	return  u4PlaybackTrackIndex;
			

			
}

BOOL CiPodSDKAgent::PlayCurrentSelection(int index)
{
	UINT8 u1ShuffleMode=0;
	UINT8 u1RepeatMode=0;
	UINT u4PlaybackTrackIndex=0;
	if (!m_bInited)
	{
		return  false;
	}
	m_pfnGetShuffleSetting(&u1ShuffleMode);	
	Sleep(80);
	m_pfnGetRepeatSetting(&u1RepeatMode);
	Sleep(80); 

	if (RET_IPOD_OK ==m_pfnPlayCurrentSelection(index))
	{
	 
		Sleep(80);
		m_pfnSetShuffleMode(u1ShuffleMode,true);
		Sleep(80);
		m_pfnSetRepeatMode(u1RepeatMode,true);
		return true;
	}	

	return false;
}
