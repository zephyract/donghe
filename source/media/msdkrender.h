#pragma once

#include "msdk_defs.h"
#include "DVP_Agent.h"
#include "dvp_cmd.h"
#include "GErr.h"

#include "MSDK_Global.h"
#include "MSDK_Error.h"
#include "MSDK_Global.h"
#include "MSDK_Error.h"
#include "MSDK_MediaGraph.h"
#include "MSDK_MediaEvent.h"
#include "MSDK_MediaCtrl.h"
#include "MSDK_StrmIID.h"
#include "MSDK_Video.h"
#include "MSDK_Subtitle.h"
#include "MSDK_Audio.h"
#include "MSDK_AVIN.h"

// wma
#include "msdk_mediagraph_wma.h"
#include "msdk_global_wma.h"
#include "msdk_mediaevent_wma.h"
#include "MSDK_MediaCtrl_wma.h"
#include "MSDK_AudioID3Info_wma.h"


#include "uibase.h"

#define TIMER_ID_HIDE_MSDK_VIDEO_WIN	8020

const int MAX_EVENT_HANDLER_COUNT = 10;
typedef void (*PFN_MSDKRENDER_EVENT_HANDLER)(UINT media_event, UINT param1, UINT param2, DWORD user_data);

struct EVENT_HANDLER_INFO
{
	PFN_MSDKRENDER_EVENT_HANDLER pfn;
	DWORD	user_data;
	HMEDIAGRAPH hMediagraph;	// avin时记录该值,只发送对应avin通道的信息
};

// avin有五路通路供不同功能使用,这里枚举各功能项,对应的通道可能会变化
typedef enum _MSDK_AVIN_TYPE_
{
	MSDK_AVIN1,
	MSDK_AVIN2,
	MSDK_AVIN_TV,
	MSDK_AVIN_CAMERA,
} MSDK_AVIN_TYPE;

class CMsdkRender
{
public:
	CMsdkRender(void);
	virtual ~CMsdkRender(void);

public:
	static CMsdkRender* GetInstance();

	//start GPS initialize
	BOOL Initialize(HWND hWnd);
	void Uninitialize();

	// interface for rendering file
	BOOL FSetSourceFile(LPCTSTR lpszFilePath);
	void FShowVideo(BOOL bShow, LPRECT prect=NULL, E_MSDK_SINK_TYPE eSink=MSDK_SINK_FRONT);
	void FSetSink(DVP_SINK sink_type);
	void FPlay();
	void FPause();
	void FStop();
	void FSeek(DWORD dwPosition);	// 单位秒
	E_MSDK_MGSTATE FGetState();
	DWORD FGetDuration();		// 返回值单位秒
	DWORD FGetCurPostion();		// 返回值单位秒
	void FLastMemoryStart(DWORD dwPosition);	// 单位秒
	void FStopnClear();		// 停止并清除mediagraph,调用后该文件不能再播放,需重新调用FSetSourceFile(). 
	// 在收到graph build消息后，调用该函数获取该视频文件的支持情况
	// 返回值参考E_MSDK_CAPABILITY_T
	DWORD FGetCapabilites();
	// audio
	HAUDIOINFO FGetAudioInfoHandle();
	void FGetAudioInfo(OUT BYTE* pCount, OUT BYTE* pCurrent);	// 返回当前audio，及audio总数
	BOOL FSetCurrentAudio(BYTE current);						// 设置当前的声道
	// subtitle
	void FGetSubtitleInfo(OUT INT* pCount, OUT INT* pCurrent);
	BOOL FSetCurrentSubtitle(INT current);						// 设置当前的字幕
	BOOL FSetSubtitleCodePage(E_MSDK_CODEPAGE_SET eCodePage);
	
	BOOL FIsMediaTypeSupport();
	BOOL FGetMediaInfo(MSDK_MEDIA_INFO_T* pinfo);
	BOOL FIsWmaPlaying();
	void FCloseAudio();		// 临时关闭audio,RDS报警时会用
	void FOpenAudio();		// 恢复打开audio,RDS报警时会用

	// interface for camera
	void AOpenCameraChannel();	// 打开倒车的通道
	void ACloseCameraChannel();
	void ASetCameraMirror(MSDK_AVIN_MIRROR_E mirror);
	BOOL AIsSignalReadyCamera();
	void SetVideoScaleCamera(RECT *prc);
	MSDK_AVIN_SIGNAL_MODE_T AGetSignalModeCamera();
	// 倒车来时不同的情况需作不同的处理
	void AOnCamera(BOOL bStart, SOURCE_ID sid, DVP_SINK sink_type);


	/* source control */
	void FLaunchSrc(DVP_SINK sink_type);
	void FStopSrc();

	void ALaunchAvin1Src(DVP_SINK sink_type);
	void AStopAvin1Src();
	void ASetSinkAvin1(DVP_SINK sink_type);
	void AShowVideoAvin1(BOOL bShow, DVP_SINK sink_type, LPRECT prect=NULL);
	BOOL AIsSignalReadyAvin1();
	// 设置剪裁区,如果为空则不剪裁,注意不同制式的分辩率是不一样的
	// PAL制是720X576
	// N制,PAL-M是 720X480, 
	// 如果有的制式显示不出来,有可能是分辩率弄错了,剪裁时必须在正确的分辩率基础上剪裁
	void SetVideoScaleAvin1(RECT *prc);
	MSDK_AVIN_SIGNAL_MODE_T AGetSignalModeAvin1();
	void ACloseAudioAvin1();		// 临时关闭audio,RDS报警时会用
	void AOpenAudioAvin1();			// 恢复打开audio,RDS报警时会用


	void ALaunchAvin2Src(DVP_SINK sink_type);
	void AStopAvin2Src();
	void ASetSinkAvin2(DVP_SINK sink_type);
	void AShowVideoAvin2(BOOL bShow, DVP_SINK sink_type, LPRECT prect=NULL);
	BOOL AIsSignalReadyAvin2();
	void SetVideoScaleAvin2(RECT *prc);
	MSDK_AVIN_SIGNAL_MODE_T AGetSignalModeAvin2();
	void ACloseAudioAvin2();		// 临时关闭audio,RDS报警时会用
	void AOpenAudioAvin2();			// 恢复打开audio,RDS报警时会用

	void ALaunchTVSrc(DVP_SINK sink_type);
	void AStopTVSrc();
	void ASetSinkTV(DVP_SINK sink_type);
	void AShowVideoTV(BOOL bShow, DVP_SINK sink_type, LPRECT prect=NULL);
	BOOL AIsSignalReadyTV();
	void SetVideoScaleTV(RECT *prc);
	MSDK_AVIN_SIGNAL_MODE_T AGetSignalModeTV();
	void ACloseAudioTV();		// 临时关闭audio,RDS报警时会用
	void AOpenAudioTV();			// 恢复打开audio,RDS报警时会用

	void ALaunchFMSrc();
	void AStopFMSrc();

	// 窗口收到注册的消息后调用该接口处理消息
	void HandleEventF();		// for file
	void HandleEventWma();		// for wma
	// 如果注册数量已经超过最大值，则返回错误
	BOOL RegisterEventHandlerF(PFN_MSDKRENDER_EVENT_HANDLER pfn, DWORD user_data);
	// 如果未找到该项，则返回错误
	BOOL UnRegisterEventHandlerF(PFN_MSDKRENDER_EVENT_HANDLER pfn, DWORD user_data);


	void HandleEventAvin(HMEDIAGRAPH hMediaGraph);		// for avin
	// 如果注册数量已经超过最大值，则返回错误
	BOOL RegisterEventHandlerAvin(PFN_MSDKRENDER_EVENT_HANDLER pfn, DWORD user_data, MSDK_AVIN_TYPE type);
	// 如果未找到该项，则返回错误
	BOOL UnRegisterEventHandlerAvin(PFN_MSDKRENDER_EVENT_HANDLER pfn);

	void OnTimer(UINT_PTR nIDEvent);	// 计时器需主窗口转发过来



protected:

	// for file
	static MRESULT MediaEventListenerFile(HMEDIAGRAPH hMediaGraph, GUINT32 u4Evt, 
		GUINT32 u4Param1, GUINT32 u4Param2, 
		GUINT32 u4CustomData);
	void OnMediaEventFile(UINT media_event, UINT param1, UINT param2);
	//event handler
	void OnGraphBuiltFile();

	// for avin
	static MRESULT MediaEventListenerAvin(HMEDIAGRAPH hMediaGraph, GUINT32 u4Evt, 
		GUINT32 u4Param1, GUINT32 u4Param2, 
		GUINT32 u4CustomData);
	void OnMediaEventAvin(HMEDIAGRAPH hMediaGraph, UINT media_event, UINT param1, UINT param2);
	void OpenAvin(HMEDIAGRAPH *phMediaGraphV, E_MSDK_AVIN_VINDEX srcV, 
		HMEDIAGRAPH *phMediaGraphA, E_MSDK_AVIN_AINDEX srcA, DVP_SINK sink_type, BOOL bInitShow=FALSE);
	void CloseAvin(HMEDIAGRAPH *phMediaGraphV, HMEDIAGRAPH *phMediaGraphA);
	void SetSinkAvin(HMEDIAGRAPH *phMediaGraphV, HMEDIAGRAPH *phMediaGraphA, DVP_SINK sink_type);
	// sink_type指的是当前avin通道处在哪种模式, 不同的模式,处理方式可能不一样
	void ShowVideoAvin(BOOL bShow, HMEDIAGRAPH *phMediaGraphV, DVP_SINK sink_type, LPRECT prect=NULL);
	// 剪裁图像
	void SetVideoScaleAvin(HMEDIAGRAPH hMediaGraph, RECT *prc);
	MSDK_AVIN_SIGNAL_MODE_T GetSignalModeAvin(HMEDIAGRAPH hMediaGraph);

	// create mediagraph and initialize it
	BOOL CreateMediagraphFile();
	BOOL CreateMediagraphAvin(HMEDIAGRAPH *phMediaGraphV, HMEDIAGRAPH *phMediaGraphA);
	HMEDIAGRAPH GetAvinMediagraph(MSDK_AVIN_TYPE type);
protected:

	HWND m_hwnd;	// 关联的窗口

	HMEDIAGRAPH m_hMediaGraphFile;	// 用来播放文件
	HMEDIAGRAPH m_hMediaGraphWma;	// 用来播放wma文件

	HMEDIAGRAPH m_hMediaGraphAvin1V;	// 用来播放AVIN1视频
	HMEDIAGRAPH m_hMediaGraphAvin1A;	// 用来播放AVIN1音频

	HMEDIAGRAPH m_hMediaGraphAvin2V;	// 用来播放AVIN2视频
	HMEDIAGRAPH m_hMediaGraphAvin2A;	// 用来播放AVIN2音频

	HMEDIAGRAPH m_hMediaGraphTVV;	// 用来播放电视视频
	HMEDIAGRAPH m_hMediaGraphTVA;	// 用来播放电视音频

	HMEDIAGRAPH m_hMediaGraphFM;	// 用来播放FM音频

	HMEDIAGRAPH m_hMediaGraphCamera;// 用来倒车


	// 如果pfn函数指针为空，表示该项没有使用
	EVENT_HANDLER_INFO m_eventHandlerFile[MAX_EVENT_HANDLER_COUNT];

	// 如果pfn函数指针为空，表示该项没有使用
	EVENT_HANDLER_INFO m_eventHandlerAvin[MAX_EVENT_HANDLER_COUNT];


	BOOL m_bIsWmaPlaying;	// 用来标识是否wma正在播放

};

typedef enum
{
	MSDK_CAP_FILE_SEEK_SUPPORT			= 0x01   ,		//文件能否time seek
	MSDK_CAP_FILE_FF_SUPPORT			= 0x01<<1,		//文件能否快进
	MSDK_CAP_VIDEO_RESOLUTION_SUPPORT   = 0x01<<2,		//文件视频解决方案是否支持（比如128M的1080p的不支持）
	MSDK_CAP_VIDEO_BITRATE_SUPPORT      = 0x01<<3,		//文件视频比特率是否支持
	MSDK_CAP_VIDEO_FRAMERATE_SUPPORT    = 0x01<<4,		//文件视频帧率是否支持
	MSDK_CAP_VIDEO_CODEC_SUPPORT        = 0x01<<5,		//文件视频编码方式是否支持
	MSDK_CAP_VIDEO_PROFILE_LEVEL_SUPPORT= 0x01<<6,		//文件视频编码版本是否支持（比如DIVX,  我们只支持DIVX3、4、5）
	MSDK_CAP_AUDIO_BITRATE_SUPPOPRT     = 0x01<<7,		//文件音频比特率是否支持
	MSDK_CAP_AUDIO_SAMPLERATE_SUPPORT   = 0x01<<8,		//文件音频采样率是否支持
	MSDK_CAP_AUDIO_CODEC_SUPPORT        = 0x01<<9,		//文件音频编码方式是否支持
	MSDK_CAP_AUDIO_PROFILE_LEVEL_SUPPORT= 0x01<<10       //文件音频编码版本是否支持
}E_MSDK_CAPABILITY_T;