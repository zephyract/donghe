#pragma once
#ifndef _IPOD_SDK_AGENT_
#define _IPOD_SDK_AGENT_

#include "iPodSDK_def.h"
#include "csync.h"
#include "HW_Adec.h"
#define IPOD_SDK_DLL_NAME		_T("M_iPodSDK.dll")
#define IPOD_SDK_MSG_TITLE		_T("IPOD SDK")	

typedef BOOL (* TYPE_MUSB_IPOD_Init)();
typedef BOOL (* TYPE_MUSB_IPOD_DeInit)();

typedef BOOL (* TYPE_MUSB_IPOD_StartPlayAudio)();
typedef BOOL (* TYPE_MUSB_IPOD_StopPlayAudio)();

typedef BOOL (* TYPE_MUSB_IPOD_GetAudioDSP)();
typedef BOOL (* TYPE_MUSB_IPOD_ReleaseAudioDSP)();

typedef BOOL (* TYPE_MUSB_IPOD_SetADecSinkInfo)(SWI_SINK_TYPE_T eSink);

typedef UINT32 (* TYPE_MUSB_IPOD_EnterRemoteUIMode)();
typedef UINT32 (* TYPE_MUSB_IPOD_GetPlayStatus)(MUSB_IPOD_PLAY_ST_INFO_T *prPbStatus);

typedef UINT32 (* TYPE_MUSB_IPOD_ExitRemoteUIMode)();
typedef UINT32 (* TYPE_MUSB_IPOD_ResetDBSelection)();

typedef UINT32 (* TYPE_MUSB_IPOD_SetRepeatMode)(UINT8 u1RepeatMode, BOOL fgRestore);

typedef UINT32 (* TYPE_MUSB_IPOD_GetNumEQProfile)(UINT32* pu4NumEQProfile);
typedef UINT32 (* TYPE_MUSB_IPOD_GetCurEQProfileIndex)(UINT32* pu4EQProfileIndex);

typedef UINT32 (* TYPE_MUSB_IPOD_SetShuffleMode)(UINT8 u1ShuffleMode, BOOL fgRestore);

typedef UINT32 (* TYPE_MUSB_IPOD_PlayCurrentSelection)(UINT32 u4SelectTrackRecIndex);
typedef UINT32 (* TYPE_MUSB_IPOD_SetCurrentPlayingTrack)(UINT32 u4CurPlayingTrackIndex);

typedef UINT32 (* TYPE_MUSB_IPOD_SelectDBRecord)(UINT8 u1DbCategaroy, UINT32 u4RecIndex );
typedef UINT32 (* TYPE_MUSB_IPOD_SelectSortDBRecord)(UINT8 u1DbCategaroy, UINT32 u4RecIndex, UINT8 u1SortOrder);

typedef UINT32 (* TYPE_MUSB_IPOD_GetNumberCategorizedDBRecords)(UINT8 u1DbCategory);


typedef UINT32 (* TYPE_MUSB_IPOD_RetrieveCategorizedDBRecords)(UINT8 u1DbCategory,
                                                               UINT32 u4StartRecIdx, 
                                                               UINT32 u4RecCnt,
                                                               VOID *pvInfo);

typedef UINT32 (* TYPE_MUSB_IPOD_GetShuffleSetting)(UINT8 *pu1ShuffMode); 

typedef UINT32 (* TYPE_MUSB_IPOD_GetRepeatSetting)(UINT8 *pu1RepeatMode) ;

typedef UINT32 (* TYPE_MUSB_IPOD_GetCurPlayingTrackIdx)(UINT32 *pu4CurTrackIndex) ;

typedef UINT32 (* TYPE_MUSB_IPOD_GetIndexedPlayingTrackTitle)(UINT32 u4PlaybackTrackIndex,
                                                              TCHAR *pszCurTrackTitle) ;

typedef UINT32 (* TYPE_MUSB_IPOD_GetIndexedPlayingTrackArtist)(UINT32 u4PlaybackTrackIndex, 
                                                               TCHAR *pszCurTrackArtist) ;

typedef UINT32 (* TYPE_MUSB_IPOD_GetIndexedPlayingTrackAlbum)(UINT32 u4PlaybackTrackIndex,
                                                              TCHAR *pszCurTrackAlbum)  ;

typedef UINT32 (* TYPE_MUSB_IPOD_RequestLingoProtocolVersion)(UINT8 u1LingoID,                                  
                                                              UINT32 *pu4Version) ;

typedef UINT32 (* TYPE_MUSB_IPOD_GetCurPlayingTrackChapterInfo)(VOID *pvCharpterInfo);

typedef UINT32 (* TYPE_MUSB_IPOD_GetDbTrackInfo)(UINT32 u4TrackDbStarIndex,
                                                 UINT32 u4TrackCount,
                                                 UINT8 u1TrackInfoBit,
                                                 VOID *pvGotInfo);

typedef UINT32 (* TYPE_MUSB_IPOD_GetPlayStatus_Ex)(MUSB_IPOD_PLAY_ST_INFO_EX_T *prPbStatus);

typedef UINT32 (* TYPE_MUSB_IPOD_GetTrackArtWorkData)(UINT32 u4TrackIndex,
                                                      UINT16 u2FmtID,
                                                      UINT32 u4MsTime,
                                                      VOID *pvArtWorkData);

typedef UINT32 (* TYPE_MUSB_IPOD_GetArtWorkFormats)(VOID *pvGotInfo); //MUSB_IPOD_ARTWORK_FMT

typedef UINT32 (* TYPE_MUSB_IPOD_GetAuthStatus)(MUSB_AUTH_RESULT_T *prAuthResult);

typedef void (* TYPE_MUSB_IPOD_PlayControl)(UINT8 u1CtrlCode);

typedef UINT32 (* TYPE_MUSB_IPOD_GetiPodPreferences)(UINT8 u1ClassID, MUSB_IPOD_PREFERENCES_T* prIPodPreferences);

typedef UINT32 (* TYPE_MUSB_IPOD_SetiPodPreferences)(UINT8 u1ClassID, UINT8 u1SettingID);

typedef UINT32 (* TYPE_MUSB_IPOD_SetVolume)(UINT32 u4Volume);

typedef UINT32 (* TYPE_MUSB_IPOD_GetVolume)(UINT32* pu4Volume);

typedef UINT32 (* TYPE_MUSB_IPOD_SetRemoteEvent)(UINT32 u4EventMask);

typedef UINT32 (* TYPE_MUSB_IPOD_SetPlayStatusChangeNotification)(UINT32 u4NotificationType, UINT32 u4NotificationMask);

typedef UINT32 (* TYPE_MUSB_IPOD_GetIndexedPlayingTrackInfo)(UINT32 u4PlaybackTrackIndex, 
                                                             UINT32 u4ChapIndex, 
                                                             MUSB_TRACK_INF_TYPE_E eTrackInfoType, 
                                                             VOID* pInfo);

typedef UINT32 (* TYPE_MUSB_IPOD_GetTrackArtWorkTimes)(UINT32 u4TrackIndex,
                                                       UINT16 u2FmtID,
                                                       UINT16 u2ArtWorkIndex,
                                                       UINT16 u2ArtWorkCount,
                                                       VOID *pvGotInfo);

typedef UINT32 (* TYPE_MUSB_IPOD_GetNumPlayingTracks)(DWORD* pdwTracksNum);

typedef UINT32 (* TYPE_MUSB_IPOD_ResetDBSelectionHierarchy)(UINT8 u1Hierarchy);

typedef UINT32 (* TYPE_MUSB_IPOD_SetVideoDelay)(UINT32 u4VideoDelayMs);

typedef UINT32 (* TYPE_MUSB_IPOD_GetiPodOptionsForLingo)(UINT8 u1LingoID, UINT64* pu8Options);

typedef UINT32 (* TYPE_MUSB_IPOD_GetiPodOptions)(UINT64* pu8Options);



class CiPodSDKAgent : public CMuTexObject
{
public:
    CiPodSDKAgent(void);
    ~CiPodSDKAgent(void);

public:
    BOOL Init();
    BOOL Deinit();
    BOOL ReInit();

  	BOOL IsInit(){return m_bInited;}
    BOOL m_pfnIPOD_Init();
    BOOL m_pfnIPOD_DeInit();

    BOOL m_pfnStartPlayAudio();
    BOOL m_pfnStopPlayAudio();

    BOOL m_pfnGetAudioDSP();
    BOOL m_pfnReleaseAudioDSP();

    BOOL m_pfnSetADecSinkInfo(SWI_SINK_TYPE_T eSink);

    UINT32 m_pfnEnterRemoteUIMode();
    UINT32 m_pfnGetPlayStatus(MUSB_IPOD_PLAY_ST_INFO_T *prPbStatus);

    UINT32 m_pfnExitRemoteUIMode();
    UINT32 m_pfnResetDBSelection();

    UINT32 m_pfnSetRepeatMode(UINT8 u1RepeatMode, BOOL fgRestore);

    UINT32 m_pfnGetNumEQProfile(UINT32* pu4NumEQProfile);
    UINT32 m_pfnGetCurEQProfileIndex(UINT32* pu4EQProfileIndex);

    UINT32 m_pfnSetShuffleMode(UINT8 u1ShuffleMode, BOOL fgRestore);

    UINT32 m_pfnPlayCurrentSelection(UINT32 u4SelectTrackRecIndex);
    UINT32 m_pfnSetCurrentPlayingTrack(UINT32 u4CurPlayingTrackIndex);

    UINT32 m_pfnSelectDBRecord(UINT8 u1DbCategaroy, UINT32 u4RecIndex );
    UINT32 m_pfnSelectSortDBRecord(UINT8 u1DbCategaroy, UINT32 u4RecIndex, UINT8 u1SortOrder);

    UINT32 m_pfnGetNumberCategorizedDBRecords(UINT8 u1DbCategory);


    UINT32 m_pfnRetrieveCategorizedDBRecords(UINT8 u1DbCategory,
        UINT32 u4StartRecIdx, 
        UINT32 u4RecCnt,
        VOID *pvInfo);

    UINT32 m_pfnGetShuffleSetting(UINT8 *pu1ShuffMode); 

    UINT32 m_pfnGetRepeatSetting(UINT8 *pu1RepeatMode) ;

    UINT32 m_pfnGetCurPlayingTrackIdx(UINT32 *pu4CurTrackIndex) ;

    UINT32 m_pfnGetIndexedPlayingTrackTitle(UINT32 u4PlaybackTrackIndex,
        TCHAR *pszCurTrackTitle) ;

    UINT32 m_pfnGetIndexedPlayingTrackArtist(UINT32 u4PlaybackTrackIndex, 
        TCHAR *pszCurTrackArtist) ;

    UINT32 m_pfnGetIndexedPlayingTrackAlbum(UINT32 u4PlaybackTrackIndex,
        TCHAR *pszCurTrackAlbum)  ;

    UINT32 m_pfnRequestLingoProtocolVersion(UINT8 u1LingoID,                                  
        UINT32 *pu4Version) ;

    UINT32 m_pfnGetCurPlayingTrackChapterInfo(VOID *pvCharpterInfo);

    UINT32 m_pfnGetDbTrackInfo(UINT32 u4TrackDbStarIndex,
        UINT32 u4TrackCount,
        UINT8 u1TrackInfoBit,
        VOID *pvGotInfo);

    UINT32 m_pfnGetPlayStatus_Ex(MUSB_IPOD_PLAY_ST_INFO_EX_T *prPbStatus);

    UINT32 m_pfnGetTrackArtWorkData(UINT32 u4TrackIndex,
        UINT16 u2FmtID,
        UINT32 u4MsTime,
        VOID *pvArtWorkData);

    UINT32 m_pfnGetArtWorkFormats(VOID *pvGotInfo); //MUSB_IPOD_ARTWORK_FMT

    UINT32 m_pfnGetAuthStatus(MUSB_AUTH_RESULT_T *prAuthResult);

    void m_pfnPlayControl(UINT8 u1CtrlCode);

    UINT32 m_pfnGetiPodPreferences(UINT8 u1ClassID, MUSB_IPOD_PREFERENCES_T* prIPodPreferences);

    UINT32 m_pfnSetiPodPreferences(UINT8 u1ClassID, UINT8 u1SettingID);

    UINT32 m_pfnSetVolume(UINT32 u4Volume);

    UINT32 m_pfnGetVolume(UINT32* pu4Volume);

    UINT32 m_pfnSetRemoteEvent(UINT32 u4EventMask);

    UINT32 m_pfnSetPlayStatusChangeNotification(UINT32 u4NotificationType, UINT32 u4NotificationMask);

    UINT32 m_pfnGetIndexedPlayingTrackInfo(UINT32 u4PlaybackTrackIndex, 
        UINT32 u4ChapIndex, 
        MUSB_TRACK_INF_TYPE_E eTrackInfoType, 
        VOID* pInfo);

    UINT32 m_pfnGetTrackArtWorkTimes(UINT32 u4TrackIndex,
        UINT16 u2FmtID,
        UINT16 u2ArtWorkIndex,
        UINT16 u2ArtWorkCount,
        VOID *pvGotInfo);

    UINT32 m_pfnGetNumPlayingTracks(DWORD* pdwTracksNum);

    UINT32 m_pfnResetDBSelectionHierarchy(UINT8 u1Hierarchy);

    UINT32 m_pfnSetVideoDelay(UINT32 u4VideoDelayMs);

    UINT32 m_pfnGetiPodOptionsForLingo(UINT8 u1LingoID, UINT64* pu8Options);

    UINT32 m_pfnGetiPodOptions(UINT64* pu8Options);

private:
    HMODULE m_hIPodSDKDll;
    void ResetAllFuncPointers();
    BOOL m_bInited;

private:

    TYPE_MUSB_IPOD_Init		pfnIPOD_Init;
    TYPE_MUSB_IPOD_DeInit	pfnIPOD_DeInit;

    TYPE_MUSB_IPOD_StartPlayAudio	pfnStartPlayAudio;
    TYPE_MUSB_IPOD_StopPlayAudio	pfnStopPlayAudio;
    TYPE_MUSB_IPOD_GetAudioDSP		pfnGetAudioDSP;
    TYPE_MUSB_IPOD_ReleaseAudioDSP	pfnReleaseAudioDSP;

    TYPE_MUSB_IPOD_EnterRemoteUIMode			pfnEnterRemoteUIMode;
    TYPE_MUSB_IPOD_GetPlayStatus				pfnGetPlayStatus;
    TYPE_MUSB_IPOD_ExitRemoteUIMode				pfnExitRemoteUIMode;
    TYPE_MUSB_IPOD_ResetDBSelection				pfnResetDBSelection;
    TYPE_MUSB_IPOD_SetRepeatMode				pfnSetRepeatMode;
    TYPE_MUSB_IPOD_GetNumEQProfile				pfnGetNumEQProfile;
    TYPE_MUSB_IPOD_GetCurEQProfileIndex			pfnGetCurEQProfileIndex;
    TYPE_MUSB_IPOD_SetShuffleMode				pfnSetShuffleMode;
    TYPE_MUSB_IPOD_PlayCurrentSelection			pfnPlayCurrentSelection;
    TYPE_MUSB_IPOD_SetCurrentPlayingTrack		pfnSetCurrentPlayingTrack;
    TYPE_MUSB_IPOD_SelectDBRecord				pfnSelectDBRecord;
    TYPE_MUSB_IPOD_SelectSortDBRecord			pfnSelectSortDBRecord;
    TYPE_MUSB_IPOD_GetNumberCategorizedDBRecords pfnGetNumberCategorizedDBRecords;
    TYPE_MUSB_IPOD_RetrieveCategorizedDBRecords	pfnRetrieveCategorizedDBRecords;
    TYPE_MUSB_IPOD_GetShuffleSetting			pfnGetShuffleSetting;
    TYPE_MUSB_IPOD_GetRepeatSetting				pfnGetRepeatSetting;
    TYPE_MUSB_IPOD_GetCurPlayingTrackIdx		pfnGetCurPlayingTrackIdx;
    TYPE_MUSB_IPOD_GetIndexedPlayingTrackTitle	pfnGetIndexedPlayingTrackTitle;
    TYPE_MUSB_IPOD_GetIndexedPlayingTrackArtist pfnGetIndexedPlayingTrackArtist;
    TYPE_MUSB_IPOD_GetIndexedPlayingTrackAlbum	pfnGetIndexedPlayingTrackAlbum;
    TYPE_MUSB_IPOD_RequestLingoProtocolVersion	pfnRequestLingoProtocolVersion;                         
    TYPE_MUSB_IPOD_GetCurPlayingTrackChapterInfo pfnGetCurPlayingTrackChapterInfo;
    TYPE_MUSB_IPOD_GetDbTrackInfo				pfnGetDbTrackInfo;
    TYPE_MUSB_IPOD_GetPlayStatus_Ex				pfnGetPlayStatus_Ex;
    TYPE_MUSB_IPOD_GetTrackArtWorkData			pfnGetTrackArtWorkData;
    TYPE_MUSB_IPOD_GetArtWorkFormats			pfnGetArtWorkFormats;
    TYPE_MUSB_IPOD_GetAuthStatus				pfnGetAuthStatus;
    TYPE_MUSB_IPOD_PlayControl					pfnPlayControl;
    TYPE_MUSB_IPOD_GetiPodPreferences			pfnGetiPodPreferences;
    TYPE_MUSB_IPOD_SetiPodPreferences			pfnSetiPodPreferences;
    TYPE_MUSB_IPOD_SetVolume					pfnSetVolume;
    TYPE_MUSB_IPOD_GetVolume					pfnGetVolume;
    TYPE_MUSB_IPOD_SetRemoteEvent				pfnSetRemoteEvent;
    TYPE_MUSB_IPOD_SetPlayStatusChangeNotification	pfnSetPlayStatusChangeNotification;
    TYPE_MUSB_IPOD_GetIndexedPlayingTrackInfo	pfnGetIndexedPlayingTrackInfo;
    TYPE_MUSB_IPOD_GetTrackArtWorkTimes			pfnGetTrackArtWorkTimes;
    TYPE_MUSB_IPOD_GetNumPlayingTracks			pfnGetNumPlayingTracks;
    TYPE_MUSB_IPOD_ResetDBSelectionHierarchy	pfnResetDBSelectionHierarchy;
    TYPE_MUSB_IPOD_SetVideoDelay				pfnSetVideoDelay;
    TYPE_MUSB_IPOD_GetiPodOptionsForLingo		pfnGetiPodOptionsForLingo;
    TYPE_MUSB_IPOD_GetiPodOptions				pfnGetiPodOptions;
    TYPE_MUSB_IPOD_SetADecSinkInfo				pfnSetADecSinkInfo;
public:
    int GetCurPlayIndex();
	BOOL IsPlaying();
	BOOL PlayCurrentSelection(int index=0);
	void  SetSuffle();
	void  SetRptOne();
	void  SetRptAll();

};

#endif // _IPOD_SDK_AGENT_