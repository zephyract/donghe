#pragma once
#include "CiPodSDKAgent.h"
#include "IPodMessage_def.h"
#include "InterProcessCommunication.h"
#include "uibase.h"
#include <vector>
using namespace  std;

#define  TIMER_CONNECTDEVICE  0X2011


#define CALL_IPOD_API(x)		\
	if (RET_IPOD_OK != (x))		\
{							\
	return;					\
}							\

#define CALL_IPOD_API_BOOL(x)	\
	if (RET_IPOD_OK != (x))		\
{							\
	return FALSE;			\
}



// Genre
#define DB_SUB_TYPE_GENRE_GENRE				0x01
#define DB_SUB_TYPE_GENRE_ARTIST			0x02
#define DB_SUB_TYPE_GENRE_ALBUM				0x03
#define DB_SUB_TYPE_GENRE_TRACK				0x04

// Playlist
#define DB_SUB_TYPE_PLAYLIST_PLAYLIST		0x01
#define DB_SUB_TYPE_PLAYLIST_TRACK			0x02

// Artist
#define DB_SUB_TYPE_ARTIST_ARTIST			0x01
#define DB_SUB_TYPE_ARTIST_ALBUM			0x02
#define DB_SUB_TYPE_ARTIST_TRACK			0x03

// Album
#define DB_SUB_TYPE_ALBUM_ALBUM				0x01
#define DB_SUB_TYPE_ALBUM_TRACK				0x02

// Track
#define DB_SUB_TYPE_TRACK_TRACK				0x01

// Audiobooks
#define DB_SUB_TYPE_AUDIOBOOK_TRACK			0x01

// Podcast
#define DB_SUB_TYPE_PODCAST_PODCAST			0x01
#define DB_SUB_TYPE_PODCAST_TRACK			0x02

// Composer
#define DB_SUB_TYPE_COMPOSER_COMPOSER		0x01
#define DB_SUB_TYPE_COMPOSER_TRACK			0x02


#define DB_VIDEO_SUB_TYPE_GENRE_MOVIES				0x00
#define DB_VIDEO_SUB_TYPE_GENRE_MUSIC_VIDEOS		0x01
#define DB_VIDEO_SUB_TYPE_GENRE_TV_SHOWS			0x02
#define DB_VIDEO_SUB_TYPE_GENRE_VIDEO_PODCASTS		0x03
#define DB_VIDEO_SUB_TYPE_GENRE_RENTALS				0x04





class CIPODManager
{
public:
	CIPODManager(void);
	~CIPODManager(void);
	static CIPODManager m_Instance;
	static CIPODManager * GetInstance () { return &m_Instance; };
	CiPodSDKAgent *m_iPodSDKAgent;


	// my interface
	/* source control */
	void LaunchSrc(DVP_SINK sink_type);
	void StopSrc();
	void SetSink(DVP_SINK sink_type);
	// 根据当前的类型,及所处的子目录,确定当前数据的类型
	// 传进来的参数必须合法,否则返回结果未定义,函数本身不作参数校验
	MUSB_IPOD_DB_CATEGORY_TYPE_E GetDBType(MUSB_IPOD_DB_CATEGORY_TYPE_E type, UINT sub_type);
	BOOL IsConnect(){return m_bConnected;}


public:
	BOOL    IsIPODSDKInitlized();
	BOOL    InitIPodSDK(HWND hwnd);
	void    OnPlugOut();
	void    OnPlugIn();
	void    ReleaseAudioDsp();
	void    GetAudioDsp();
	void    ConnectDevice();
//     BOOL    HandleAuthDone(UINT32 u4AuthState);
	void    HandlePlaybackPositionChanged(UINT32 u4Offset);
	void    HandleiPodStatusChanged(UINT32 u4IPodStatus);
	bool    GetiPodArtworkFormats();
	bool    CreateArtworkDataBuffer();
	UINT    UpdataDbCategory(UINT8 u1SelectDbCategory, UINT8 u1QueryDbCategory=0x1, int nRecIndex=0);
	bool    ReadDbCategory(UINT8 u1SelectDbCategory, UINT8 u1QueryDbCategory=0x1, int nRecIndex=0);
	void    HandleTrackIndexChanged(UINT32 u4TrackIndex);
	void    HandlePlaybackStatusChanged(UINT32 u4Status);
	bool    HandleTrackArtwork(int index,UINT8 **puId3Pic, UINT32 *u4PicLen,UINT32 *u4PicWith,UINT32 *u4PicHeight);
	void    GetPlayStatu();
    UINT    GetTrackID3Tag();
	BOOL    GetTrackArtWork(UINT index, UIPOD_ARTWORKDATA *pdata);
	void    SetSinkOutput(SWI_SINK_TYPE_T outPut) ;//设置前后排输出
	void    SetIPodMute();
		 

	CiPodSDKAgent *GetIPodCtrl(){return m_iPodSDKAgent;}
	bool   PostIPodMessage(UINT Msg,
		WPARAM wParam,
		LPARAM lParam);
	void  RegisterWindow(wstring strWindowName);
	void  UnRegisterWindow(wstring strWindowName);

   	static DWORD WINAPI NotifyThreadProc(LPVOID lpParm);
public:
	BOOL        m_bConnected;
	BOOL        m_bInsertPowerOn;
	HANDLE		m_hEvtQueueNotify;
	BOOL		m_biPodInited;
	HANDLE		m_hReadMsgQueue;
	BOOL		m_bExitNotifyThread;
	HANDLE		m_hEvtNotifyThreadExit;
	HANDLE		m_hNotifyThread;
//	BOOL		m_bAuthPassed;
	//BOOL		m_bInsert;
	UINT        m_iPodVol;
	CInterProcessCommunication  *m_pServceData;
	CInterProcessCommunication  *m_pServerArtWork;

	MUSB_IPOD_ARTWORK_FMTS_INFO_T m_rArtWorkFmtInfo;
	MUSB_IPOD_ARTWORK_FMTS m_rArtWorkFmts[MAX_ARTWORK_FTM_NUM];
	UINT32 m_u4MaxArtworkHeight;

	LPBYTE	m_pbTrackArtworkData;
	UINT32	m_u4ArtworkDataSize;

	MUSB_IPOD_PLAY_ST_INFO_T m_rPbStatus;
    HWND   m_hMainWnd;
	vector<HWND> m_vechWnd;

protected:
	BOOL m_bInit;

};
