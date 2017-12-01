#ifndef _IPODSDK_DEF_H_
#define _IPODSDK_DEF_H_

#define IPOD_INFO_MAX_STR_LEN	(256)

#define RET_IPOD_OK				0
#define RET_IPOD_FAIL			1
#define RET_IPOD_BUSY			2
#define RET_IPOD_BAD_PARAM		3


#define QUEUE_NOTIFY_EVENT_NAME     _T("IPOD_QUEUE_NOTIFY")
#define IPOD_INSERT_EVENT_NAME     _T("IPOD_AUTH_DONE_EVENT")

typedef enum
{
	IPOD_AUTH_STATE,
	IPOD_PB_STATUS,
	IPOD_DIGITALAUIDO_CFG,
	IPOD_STATUS_NOTIFY
}E_IPOD_NOTIFY_INFO_E;

typedef enum
{
	IPOD_AUTHENTICATION_PASS,
	IPOD_AUTHENTICATION_NONE,
	IPOD_AUTHENTICATION_FAIL    
}E_IPOD_AUTHENTICATION_STATUS;


typedef enum
{
	IPOD_IDENTIFY_DEVICE_LINGO_1,
	IPOD_IDENTIFY_DEVICE_LINGO_2,
	IPOD_IDENTIFY_DEVICE_LINGO_3,
	IPOD_IDENTIFY_DEVICE_LINGO_MAX
}E_IPOD_IDENTIFY_DEVICE_LINGO;

// MUSB_IPOD_GetAuthStatus
typedef struct
{
	UINT32	u4AuthResult; //E_IPOD_AUTHENTICATION_STATUS
	BOOL	fgIdpMode;
	UINT8	u1CategoryMode;
	BOOL	fgInAuthoring;
	BOOL	fgCanReTry;
}MUSB_AUTH_RESULT_T;

typedef struct
{
	BYTE bPreferenceClassID;
	BYTE bPreferenceSettingID;
}MUSB_IPOD_PREFERENCES_T;

#define IPOD_IDPS_ERROR_CATEGORY      0
#define IPOD_IDPS_PHOTO_CATEGORY      1
#define IPOD_IDPS_MUSIC_OSD_CATEGORY  2
#define IPOD_IDPS_MUSIC_GUPA_CATEGORY 3
#define IPOD_IDPS_VIDEO_CATEGORY      4


typedef struct
{
	UINT8  fgIdpMode;
	UINT8  u1CategoryMode;
}MUSB_IPOD_START_PARAM_T;

typedef struct
{
	UINT32 u4InParam0;
	UINT32 u4InParam1;
	UINT32 u4InParam2;
	UINT32 u4InParam3; 
	UINT32 u4InParam4;    
	struct
	{
		MUSB_IPOD_START_PARAM_T tStartParamter;
		void *pv_req;
		UINT32 u4ReqSize;
	}uContent;

	INT32 i4Return;
}MUSB_IPOD_REQUEST_T;


typedef struct
{
	UINT8     u1BusNO;
	UINT8     u1DeviceNO;
	UINT8     u1HiddevNO;
	UINT32    u4ModeInfo;
}MUSB_IPOD_CONN_PARM_T;

typedef struct
{
	UINT8  u1KeyCode;
}MUSB_IPOD_SEND_KEY_PARM_T;


//this information is getted by getIndexedPlayingTrackInfo
typedef enum
{
	TRACK_CAP_INF				= 0x00, //track capabilities & information.
	TRACK_LOCALCOASTING_NAME	= 0x01, //track localcasting name.
	TRACK_DATE_INFOTYPE_CODE	= 0x02, //track released date infotype code.
	TRACK_DESCRIPTION			= 0x03, //track description.
	TRACK_SONG_LYRICS			= 0x04, //track Sony Lyric.
	TRACK_GENRE					= 0x05, //track Genre.
	TRACK_COMPOSER				= 0x06, //track composer.
	TRACK_ARTWORK_CNT			= 0x07, //track artwork count.
	TRACK_INF_NONE
}MUSB_TRACK_INF_TYPE_E;

//this information is getted by getIndexedPlayingTrackInfo
typedef struct
{
	BOOL fgQueue2PlayVideo;	//bit 8 ,track is currently queued to play as video
	BOOL fgContainVideo;	//track contains video.
	BOOL fgHasDescription;	//track has description.
	BOOL fgHasReleaseDate;	//track has release date.
	BOOL fgHasLocalCasting;	//track has localcastiong episode.
	BOOL fgHasLyrics;		//track has song lyrics.
	BOOL fgHasAlbumArtWork;	//Track has Album art works.
	BOOL fghasChapter;		//track has chapters.
	BOOL fgIsAudioBook;		//track is audio book.
	UINT32 u4TImeLen;		//track length in millisecons.
	UINT16 u2ChapterCount;	// track chaper count.
}MUSB_IPOD_TRACK_CAP_INF_T; //track capabilities information

//this information is getted by getIndexedPlayingTrackInfo
typedef struct
{
	UINT8 u1Second;
	UINT8 u1Minute;
	UINT8 u1Hour;
	UINT8 u1Day;
	UINT8 u1Month;
	UINT8 u1WeekDay;  
	UINT16 u2Year;  
}MUSB_IPOD_TRACK_RELEASE_DATE_T; //track capabilities information

#define MAX_ARTWORK_INFO_NUM		8
#define MAX_ARTWORK_FTM_NUM			8
#define MAX_ARTWORK_TIME_NUM		8

//this information is getted by getIndexedPlayingTrackInfo
typedef struct
{
	UINT16 u2FormatID;
	UINT16 u2ArtWorkCnt;
}MUSB_IPOD_ARTWORK_CNT_T;

typedef struct
{
	UINT16 u2InfoNum;
	MUSB_IPOD_ARTWORK_CNT_T *prArtworkCnt;
}MUSB_IPOD_ARTWORK_CNT_INFO_T;

//this information is getted by getIndexedPlayingTrackInfo
typedef struct
{
	UINT16 u2FormatID;
	UINT8 u1PixFormat;
	UINT16 u2ImgWidth;
	UINT16 u2ImgHeight;
}MUSB_IPOD_ARTWORK_FMTS;

typedef struct
{
	UINT32 u4FmtNum;
	MUSB_IPOD_ARTWORK_FMTS* prArtworkFmt;
}MUSB_IPOD_ARTWORK_FMTS_INFO_T;


typedef struct
{
	UINT32 u4InfoNum;
	UINT32 *pi4MsTimePos;
}
MUSB_IPOD_ARTWORK_TIMES_T;

// This information is got by MUSB_IPOD_GetCurPlayingTrackChapterInfo
typedef struct
{
	UINT32  u4ChapterCnt;
	UINT32 u4ChapterIndex;
}MUSB_IPOD_CHAPTER_INFO_T;

// This information is got by MUSB_IPOD_GetTrackArtWorkData
typedef struct
{
	UINT16 u2DescritorIndex;
	UINT8 u1DisplayPixFmtCode;
	UINT16 u2ImgWidth;
	UINT16 u2ImgHeight;
	UINT16 u2TopLeftX;
	UINT16 u2TopLeftY;
	UINT16 u2BotRightX;
	UINT16 u2BotRightY;
	UINT32 u4RowSizeInByte;
	UINT32 u4DataBufSize;
	BYTE *pvData;
}MUSB_IPOD_ARTWORK_DATA_T;


typedef enum
{
	PB_ST_EX_STOP_0X00		= 0x00,
	PB_ST_EX_PLAY_0X01		= 0x01,
	PB_ST_EX_PAUSE_0X02		= 0x02, 
	PB_ST_EX_PF_0X03		= 0x03,
	PB_ST_EX_FR_0X04		= 0x04,
	PB_ST_EX_REWIND_0X05	= 0x05,   
	//added for notify
	PB_ST_EX_FFW_SEEK_STOP,
	PB_ST_EX_REW_SEEK_STOP,
	PB_ST_EX_FFW_SEEK_START,
	PB_ST_EX_REW_SEEK_START,
	//PB_ST_EX_SEEK_STOP,   
	PB_ST_EX_ERR_0XFF   =0xFF,   	
}MUSB_IPOD_PLAY_ST_EX_E;

typedef struct
{
	UINT32 u4TrackLen;
	UINT32 u4TrackPos;
	MUSB_IPOD_PLAY_ST_EX_E ePbStatus;
}MUSB_IPOD_PLAY_ST_INFO_EX_T;

typedef struct
{
	UINT32 u4TrackIdx;
	UINT32 u4TrackLen;
	UINT32 u4TrackPos;
	MUSB_IPOD_PLAY_ST_EX_E ePbStatus;
}  MUSB_IPOD_PLAY_ST_INFO_T;

typedef struct DBRecordItem
{
	UINT32 u4RecIdx;
	CHAR szRecInfo[IPOD_INFO_MAX_STR_LEN];
}MUSB_IPOD_DB_RecItem_T;

// Used by MUSB_IPOD_GetDbTrackInfo MAST BIT Define.
#define DB_TRK_INF_MASKBIT_00_CAP                          0//  got  MUSB_IPOD_DB_TRACK_CAP_INFO_T
#define DB_TRK_INF_MASKBIT_01_NAME                         1//  got MUSB_IPOD_STR_INFO_T
#define DB_TRK_INF_MASKBIT_02_NAME_ARTIST                  2//  got MUSB_IPOD_STR_INFO_T
#define DB_TRK_INF_MASKBIT_03_NAME_ALBUM                   3//  got MUSB_IPOD_STR_INFO_T
#define DB_TRK_INF_MASKBIT_04_NAME_GENRE                   4//  got MUSB_IPOD_STR_INFO_T
#define DB_TRK_INF_MASKBIT_05_NAME_COMPOSER                5//  got MUSB_IPOD_STR_INFO_T
#define DB_TRK_INF_MASKBIT_06_TRK_DURATION_TIME            6//  got UINT32 value.
#define DB_TRK_INF_MASKBIT_07_TRK_UINQUE_ID                7//  got UINT64 value.
#define DB_TRK_INF_MASKBIT_08_CHAPT_CNT                    8//  got UINT16 value.
#define DB_TRK_INF_MASKBIT_09_CHAPT_TIME                   9//  got MUSB_IPOD_CHAPTER_TIME_T only get per track
#define DB_TRK_INF_MASKBIT_10_CHAP_NAME                   10//  got MUSB_IPOD_STR_INFO_T
#define DB_TRK_INF_MASKBIT_11_PLAYING_LYRIC               11//  Got MUSB_IPOD_TRACK_CUR_PLAY_LYRIC_T
#define DB_TRK_INF_MASKBIT_12_DESCRIPTION                 12//  got MUSB_IPOD_STR_INFO_T
#define DB_TRK_INF_MASKBIT_13_ALBUM_TRK_IDX               13//  got UINT16
#define DB_TRK_INF_MASKBIT_14_DISC_ABLUM_IDX              14//  got UINT16
#define DB_TRK_INF_MASKBIT_15_PLAY_CNT                    15//  got UINT32
#define DB_TRK_INF_MASKBIT_16_SKIP_CNT                    16//  got UINT32
#define DB_TRK_INF_MASKBIT_17_LOCAL_RELEASE_DATE          17//  got MUSB_IPOD_DATE_TIME_T
#define DB_TRK_INF_MASKBIT_18_LAST_PLAYED_DATE            18//  got MUSB_IPOD_DATE_TIME_T
#define DB_TRK_INF_MASKBIT_19_RELEASE_YEAR                19//  got UINT16
#define DB_TRK_INF_MASKBIT_20_STAR_RATING                 20//  got MSB_IPOD_TRAK_STAR_RATING_E
#define DB_TRK_INF_MASKBIT_21_SERIES_NAME                 21//  got MUSB_IPOD_STR_INFO_T
#define DB_TRK_INF_MASKBIT_22_SEASON_NUM                  22//  got unit16 type info
#define DB_TRK_INF_MASKBIT_23_VOLUE_AJUST                 23//  got uint8 /0x00 nu adjust 0x64 +1005 0x9c-100%
#define DB_TRK_INF_MASKBIT_24_EQ_RESET                    24//  got unint16 information
#define DB_TRK_INF_MASKBIT_25_DATA_RATE                   25//  got UINT32 data rate per second 
#define DB_TRK_INF_MASKBIT_26_BOOKMARK_OFFSET             26//  got UINT32 
#define DB_TRK_INF_MASKBIT_27_START_STOP_TIME             27//  got MUSB_IPOD_TRACK_TIME_T  

// For GetDBTrackInfo
/******************************************************

Bit Description
0 Capabilities (media kind, skip when shuffle, has artwork, has bookmark, has lyrics, is audiobook, etc.)
1 Track name
2 Artist name
3 Album name
4 Genre name
5 Composer name
6 Total track time duration
7 Unique track identifier
8 Chapter count
9 Chapter times
10 Chapter names
11 Lyrics of the song currently playing in the Playback Engine
12 Description
13 Album track index
14 Disc set album index
15 Play count
16 Skip count
17 Podcast release date
18 Last played date/time
19 Year (release date)
20 Star rating
21 Series name
22 Season number
23 Track volume adjust
24 Track EQ preset
25 Track sample rate
26 Bookmark offset
27 Start/stop time offset
28 Reserved
	
*******************************************************/
	 typedef enum
    {
        DB_TRACK_TYPE_00_CAPABILITIES 		= 0, 	// Capabilities (media kind, skip when shuffle, has artwork, has bookmark, has lyrics, is audiobook, etc.)
        DB_TRACK_TYPE_01_TRACK_NAME			= 1,	// Track name
		DB_TRACK_TYPE_02_ARTIST_NAME		= 2,	// Artist name
		DB_TRACK_TYPE_03_ALBUM_NAME			= 3,	// Album name
		DB_TRACK_TYPE_04_GENRE_NAME			= 4, 	// Genre name
		DB_TRACK_TYPE_05_COMPOSER_NAME		= 5, 	// Composer name
		DB_TRACK_TYPE_06_TRACK_TIME			= 6, 	// Total track time duration
		DB_TRACK_TYPE_07_TARCK_UID			= 7, 	// Unique track identifier
		DB_TRACK_TYPE_08_CHAPTER_COUNT		= 8, 	// Chapter count
		DB_TRACK_TYPE_09_CHAPTER_TIMES		= 9, 	// Chapter times
		DB_TRACK_TYPE_10_CHAPTER_NAMES		= 10, 	// Chapter names
		DB_TRACK_TYPE_11_PLAYING_LYRICS		= 11, 	// Lyrics of the song currently playing in the Playback Engine
		DB_TRACK_TYPE_12_DESCRIPTION		= 12, 	// Description
		DB_TRACK_TYPE_13_ALBUM_TRACK_INDEX	= 13, 	// Album track index
		DB_TRACK_TYPE_14_DISC_ALBUM_INDEX	= 14, 	// Disc set album index
		DB_TRACK_TYPE_15_PLAY_CONUT			= 15, 	// Play count
		DB_TRACK_TYPE_16_SKIP_COUNT			= 16, 	// Skip count
		DB_TRACK_TYPE_17_PODCAST_DATE		= 17, 	// Podcast release date
		DB_TRACK_TYPE_18_LAST_PLAY_DATE		= 18, 	// Last played date/time
		DB_TRACK_TYPE_19_YEAR_RELEASE		= 19, 	// Year (release date)
		DB_TRACK_TYPE_20_STAR_RATING		= 20, 	// Star rating
		DB_TRACK_TYPE_21_SERIES_NAME		= 21, 	// Series name
		DB_TRACK_TYPE_22_SEASON_NUMBER		= 22, 	// Season number
		DB_TRACK_TYPE_23_TRACK_VOL_ADJUST	= 23, 	// Track volume adjust
		DB_TRACK_TYPE_24_TRACK_EQ_PRESET	= 24, 	// Track EQ preset
		DB_TRACK_TYPE_25_TRACK_SAMPLE_RATE	= 25, 	// Track sample rate
		DB_TRACK_TYPE_26_BOOKMARK_OFFSET	= 26, 	// Bookmark offset
		DB_TRACK_TYPE_27_START_STOP_TIME_OFFSET	= 27, // Start/stop time offset
		DB_TRACK_TYPE_28_RESERVED			= 28  	// Reserved
    }
    MUSB_DB_TRACK_INFO_TYPE_E;



//used by set notification mask bits. for 4 byte status change event mask bit
#define PB_NOTIFY_INFO_MASKBIT_00_BASIC_STATUS     (1<<0)// basic notify status(Stop,stop FFW seek stop, REW seek stop)
#define PB_NOTIFY_INFO_MASKBIT_01_EXTEND_STATUS    (1<<1)// extend  notify status
#define PB_NOTIFY_INFO_MASKBIT_02_TRK_INDEX        (1<<2)// track index.
#define PB_NOTIFY_INFO_MASKBIT_03_TRK_MS_TIME      (1<<3)// track time offset (ms)  
#define PB_NOTIFY_INFO_MASKBIT_04_TRK_SEC_TIME     (1<<4)// track time offset (second)  
#define PB_NOTIFY_INFO_MASKBIT_05_TRK_INDEX        (1<<5)// chapter index.  
#define PB_NOTIFY_INFO_MASKBIT_06_CHP_MS_TIME      (1<<6)// chapter time offset (ms)  
#define PB_NOTIFY_INFO_MASKBIT_07_CHP_SEC_TIME     (1<<7)// chapter time offset (second)  
#define PB_NOTIFY_INFO_MASKBIT_08_TRK_UID          (1<<8)// track unique  ID  
#define PB_NOTIFY_INFO_MASKBIT_09_TRK_TYPE         (1<<9)//track media type(audio/video)
#define PB_NOTIFY_INFO_MASKBIT_10_LYRIC_READY      (1<<10)// track lyrics ready
#define PB_NOTIFY_INFO_MASKBIT_10_TRK_CAPABILITY_CHGED       (1<<11)// track cabability changed

//PlayStatusChangeNotification
typedef enum
{
	PLAY_STATUS_NOTIFICATION_MODE_ONE_BYTE	= 0x01,
	PLAY_STATUS_NOTIFICATION_MODE_FOUR_BYTE = 0x04

}MUSB_IPOD_PLAY_STATUS_NOTIFICATION_MODE_E;

//got by GetDbTrackinfo
typedef struct
{
	BOOL fgIsAudioBook;
	BOOL fgHasChapters;
	BOOL fgHasArtWork;
	BOOL fgHasLyric;
	BOOL fgIsLocalCastingEpisode;
	BOOL fgHasReleaseDate;
	BOOL fgHasDescription;
	BOOL fgIsVideo;
	BOOL fgIsQueuedVideo;
}MUSB_IPOD_DB_TRACK_CAP_INFO_T;


typedef struct
{
	UINT8 u1Second;
	UINT8 u1Minute;
	UINT8 u1Hour;
	UINT8 u1Day;
	UINT8 u1Month;
	UINT16 u2Year;
}   MUSB_IPOD_DATE_TIME_T;

typedef struct
{
	TCHAR szInfo[IPOD_INFO_MAX_STR_LEN];
}MUSB_IPOD_STR_INFO_T;

typedef struct
{
	UINT16  u2ChapterIndex;
	MUSB_IPOD_STR_INFO_T rName;
}MUSB_IPOD_TRACK_CHAPT_NAME_T;

typedef struct
{
	UINT16  u2ChapterIndex;
	TCHAR *szLyric; //should be allocate by upper layer.
}MUSB_IPOD_TRACK_CUR_PLAY_LYRIC_T;

typedef enum
{
	STAR_RATE_NO_STAR		= 0,
	STAR_RATE_ONE_STAR		= 20,
	STAR_RATE_TWO_STAR		= 40,
	STAR_RATE_THREE_STAR	= 60,
	STAR_RATE_FOUR_STAR		= 80,
	STAR_RATE_FIVE_STAR		= 100  
}MUSB_IPOD_TRAK_STAR_RATING_E;

typedef struct
{
	UINT32 u4TrackStartTime;
	UINT32 u4TrackStopTime;
}MUSB_IPOD_TRACK_TIME_T;

typedef struct
{
	UINT16 u2ChapterIndex;
	UINT32 u4MilliSec;
}MUSB_IPOD_CHAPTER_TIME_T;

#define PB_ST_REMOTE_EVENT 1
#define PB_ST_NOTI_EVENT 2
typedef struct
{
	UINT8  fgPbStatuValid; //PB_ST_REMOTE_EVENT or PB_ST_NOTI_EVENT
	MUSB_IPOD_PLAY_ST_EX_E eStatus;

	BOOL fgTrackIndexValid;
	UINT32 u4TrackIndex;

	BOOL fgTrackMsOffsetValid;
	UINT32 u4TrackMsOffset;

	BOOL fgChapterIndexValid;
	UINT32 u4ChapterIndex;

	BOOL fgTrackSecOffsetValid;
	UINT32 u4TrackSecOffset;

	BOOL fgChaptMsOffsetValid;
	UINT32 u4ChapsetMsOffset;

	BOOL fgChaptSecOffsetValid;
	UINT32 u4ChaptSecOffset;

	BOOL fgTrackUIDValid;
	UINT64 u8TrackUID;

	BOOL fgTrackPlayModeValid;
	UINT8 u4TrackPlayMode; //0 audio 1, other video

	BOOL fgLyricInfoValid;
	BOOL fgLyricReady;  

	BOOL fgTrackCapa;
	UINT32 u4TrackCapaBits;
}MUSB_IPOD_PB_NOTIFY_STATUS_INF_T;

typedef struct
{
   DWORD u4SampleRateSupportFromIpod;
}
MUSB_DIGITALAUDIO_CONFIGUE_T;
   
typedef enum
{
	IPOD_STATUS_PLUG_IN = 0,
	IPOD_STATUS_PLUG_OUT
}MUSB_IPOD_STATUS_E;
	   

typedef union
{
	MUSB_AUTH_RESULT_T rAuthResult;
	MUSB_IPOD_PB_NOTIFY_STATUS_INF_T rPbInfo;
	MUSB_DIGITALAUDIO_CONFIGUE_T rDigitalAudio;
	MUSB_IPOD_STATUS_E	eiPodStatus;
}NOTIFY_INF_A;

typedef struct
{
	E_IPOD_NOTIFY_INFO_E eNotifyType;
	NOTIFY_INF_A rNotify;
} MUSB_IPOD_NOTIFY_INFO_T;

typedef struct
{
	UINT8 u1ClassID; //preference class ID
	UINT8 u1SetingID; //preference setting ID.
}MUSB_IPOD_REFERENCE_INFO;

typedef struct
{
	UINT32	u4ModelD; //preference class ID
	TCHAR   szModelNum[IPOD_INFO_MAX_STR_LEN];
}MUSB_IPOD_MODEL_INFO;

/*
static TCHAR * g_szPlayControl[] =
{	
	_T("Reserved"), 
	_T("Toggle Play/Pause"),  
	_T("Stop"),
	_T("Next Track"), 
	_T("Previous Track"), 
	_T("Start FF"), 
	_T("Start Rew"), 
	_T("End FF/Rew"), 
	_T("Next"), 
	_T("Previous"),
	_T("Play"),
	_T("Pause"),
	_T("Next Chapter"),
	_T("Previous Chapter")
};
*/
typedef enum
{
	PLAY_CTRL_RESERVED		= 0,
	PLAY_CTRL_PLAY_PAUSE,
	PLAY_CTRL_STOP,
	PLAY_CTRL_NEXT_TRACK,
	PLAY_CTRL_PRE_TRACK,
	PLAY_CTRL_START_FF,
	PLAY_CTRL_START_REW,
	PLAY_CTRL_END_FF_REW,
	PLAY_CTRL_NEXT,
	PLAY_CTRL_PRE,
	PLAY_CTRL_PLAY,
	PLAY_CTRL_PAUSE,
	PLAY_CTRL_NEXT_CHAP,
	PLAY_CTRL_PRE_CHAP
}MUSB_IPOD_PLAY_CTRL_CODE_E;

// Database category types for commands
/**********************
Top-level 0x00 1.14
Playlist 0x01
Artist 0x02
Album 0x03
Genre 0x04
Track 0x05
Composer 0x06
Audiobook 0x07 
Podcast 0x08 
Nested playlist 0x09 
Genius Mixes 0x0A
***********************/
typedef enum
{
	DB_TYPE_TOP_LEVEL		= 0,
	DB_TYPE_PLAYLIST,
	DB_TYPE_ARTIST,
	DB_TYPE_ALBUM,
	DB_TYPE_GENRE,
	DB_TYPE_TRACK,
	DB_TYPE_COMPOSER,
	DB_TYPE_AUDIOBOOK,
	DB_TYPE_PODCAST,
	DB_TYPE_NESTED_PLAYLIST,
	DB_TYPE_GENIUS_MIXES
}MUSB_IPOD_DB_CATEGORY_TYPE_E;

#endif //_IPODSDK_DEF_H_
