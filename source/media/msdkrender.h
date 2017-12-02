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
	HMEDIAGRAPH hMediagraph;	// avinʱ��¼��ֵ,ֻ���Ͷ�Ӧavinͨ������Ϣ
};

// avin����·ͨ·����ͬ����ʹ��,����ö�ٸ�������,��Ӧ��ͨ�����ܻ�仯
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
	void FSeek(DWORD dwPosition);	// ��λ��
	E_MSDK_MGSTATE FGetState();
	DWORD FGetDuration();		// ����ֵ��λ��
	DWORD FGetCurPostion();		// ����ֵ��λ��
	void FLastMemoryStart(DWORD dwPosition);	// ��λ��
	void FStopnClear();		// ֹͣ�����mediagraph,���ú���ļ������ٲ���,�����µ���FSetSourceFile(). 
	// ���յ�graph build��Ϣ�󣬵��øú�����ȡ����Ƶ�ļ���֧�����
	// ����ֵ�ο�E_MSDK_CAPABILITY_T
	DWORD FGetCapabilites();
	// audio
	HAUDIOINFO FGetAudioInfoHandle();
	void FGetAudioInfo(OUT BYTE* pCount, OUT BYTE* pCurrent);	// ���ص�ǰaudio����audio����
	BOOL FSetCurrentAudio(BYTE current);						// ���õ�ǰ������
	// subtitle
	void FGetSubtitleInfo(OUT INT* pCount, OUT INT* pCurrent);
	BOOL FSetCurrentSubtitle(INT current);						// ���õ�ǰ����Ļ
	BOOL FSetSubtitleCodePage(E_MSDK_CODEPAGE_SET eCodePage);
	
	BOOL FIsMediaTypeSupport();
	BOOL FGetMediaInfo(MSDK_MEDIA_INFO_T* pinfo);
	BOOL FIsWmaPlaying();
	void FCloseAudio();		// ��ʱ�ر�audio,RDS����ʱ����
	void FOpenAudio();		// �ָ���audio,RDS����ʱ����

	// interface for camera
	void AOpenCameraChannel();	// �򿪵�����ͨ��
	void ACloseCameraChannel();
	void ASetCameraMirror(MSDK_AVIN_MIRROR_E mirror);
	BOOL AIsSignalReadyCamera();
	void SetVideoScaleCamera(RECT *prc);
	MSDK_AVIN_SIGNAL_MODE_T AGetSignalModeCamera();
	// ������ʱ��ͬ�����������ͬ�Ĵ���
	void AOnCamera(BOOL bStart, SOURCE_ID sid, DVP_SINK sink_type);


	/* source control */
	void FLaunchSrc(DVP_SINK sink_type);
	void FStopSrc();

	void ALaunchAvin1Src(DVP_SINK sink_type);
	void AStopAvin1Src();
	void ASetSinkAvin1(DVP_SINK sink_type);
	void AShowVideoAvin1(BOOL bShow, DVP_SINK sink_type, LPRECT prect=NULL);
	BOOL AIsSignalReadyAvin1();
	// ���ü�����,���Ϊ���򲻼���,ע�ⲻͬ��ʽ�ķֱ����ǲ�һ����
	// PAL����720X576
	// N��,PAL-M�� 720X480, 
	// ����е���ʽ��ʾ������,�п����Ƿֱ���Ū����,����ʱ��������ȷ�ķֱ��ʻ����ϼ���
	void SetVideoScaleAvin1(RECT *prc);
	MSDK_AVIN_SIGNAL_MODE_T AGetSignalModeAvin1();
	void ACloseAudioAvin1();		// ��ʱ�ر�audio,RDS����ʱ����
	void AOpenAudioAvin1();			// �ָ���audio,RDS����ʱ����


	void ALaunchAvin2Src(DVP_SINK sink_type);
	void AStopAvin2Src();
	void ASetSinkAvin2(DVP_SINK sink_type);
	void AShowVideoAvin2(BOOL bShow, DVP_SINK sink_type, LPRECT prect=NULL);
	BOOL AIsSignalReadyAvin2();
	void SetVideoScaleAvin2(RECT *prc);
	MSDK_AVIN_SIGNAL_MODE_T AGetSignalModeAvin2();
	void ACloseAudioAvin2();		// ��ʱ�ر�audio,RDS����ʱ����
	void AOpenAudioAvin2();			// �ָ���audio,RDS����ʱ����

	void ALaunchTVSrc(DVP_SINK sink_type);
	void AStopTVSrc();
	void ASetSinkTV(DVP_SINK sink_type);
	void AShowVideoTV(BOOL bShow, DVP_SINK sink_type, LPRECT prect=NULL);
	BOOL AIsSignalReadyTV();
	void SetVideoScaleTV(RECT *prc);
	MSDK_AVIN_SIGNAL_MODE_T AGetSignalModeTV();
	void ACloseAudioTV();		// ��ʱ�ر�audio,RDS����ʱ����
	void AOpenAudioTV();			// �ָ���audio,RDS����ʱ����

	void ALaunchFMSrc();
	void AStopFMSrc();

	// �����յ�ע�����Ϣ����øýӿڴ�����Ϣ
	void HandleEventF();		// for file
	void HandleEventWma();		// for wma
	// ���ע�������Ѿ��������ֵ���򷵻ش���
	BOOL RegisterEventHandlerF(PFN_MSDKRENDER_EVENT_HANDLER pfn, DWORD user_data);
	// ���δ�ҵ�����򷵻ش���
	BOOL UnRegisterEventHandlerF(PFN_MSDKRENDER_EVENT_HANDLER pfn, DWORD user_data);


	void HandleEventAvin(HMEDIAGRAPH hMediaGraph);		// for avin
	// ���ע�������Ѿ��������ֵ���򷵻ش���
	BOOL RegisterEventHandlerAvin(PFN_MSDKRENDER_EVENT_HANDLER pfn, DWORD user_data, MSDK_AVIN_TYPE type);
	// ���δ�ҵ�����򷵻ش���
	BOOL UnRegisterEventHandlerAvin(PFN_MSDKRENDER_EVENT_HANDLER pfn);

	void OnTimer(UINT_PTR nIDEvent);	// ��ʱ����������ת������



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
	// sink_typeָ���ǵ�ǰavinͨ����������ģʽ, ��ͬ��ģʽ,����ʽ���ܲ�һ��
	void ShowVideoAvin(BOOL bShow, HMEDIAGRAPH *phMediaGraphV, DVP_SINK sink_type, LPRECT prect=NULL);
	// ����ͼ��
	void SetVideoScaleAvin(HMEDIAGRAPH hMediaGraph, RECT *prc);
	MSDK_AVIN_SIGNAL_MODE_T GetSignalModeAvin(HMEDIAGRAPH hMediaGraph);

	// create mediagraph and initialize it
	BOOL CreateMediagraphFile();
	BOOL CreateMediagraphAvin(HMEDIAGRAPH *phMediaGraphV, HMEDIAGRAPH *phMediaGraphA);
	HMEDIAGRAPH GetAvinMediagraph(MSDK_AVIN_TYPE type);
protected:

	HWND m_hwnd;	// �����Ĵ���

	HMEDIAGRAPH m_hMediaGraphFile;	// ���������ļ�
	HMEDIAGRAPH m_hMediaGraphWma;	// ��������wma�ļ�

	HMEDIAGRAPH m_hMediaGraphAvin1V;	// ��������AVIN1��Ƶ
	HMEDIAGRAPH m_hMediaGraphAvin1A;	// ��������AVIN1��Ƶ

	HMEDIAGRAPH m_hMediaGraphAvin2V;	// ��������AVIN2��Ƶ
	HMEDIAGRAPH m_hMediaGraphAvin2A;	// ��������AVIN2��Ƶ

	HMEDIAGRAPH m_hMediaGraphTVV;	// �������ŵ�����Ƶ
	HMEDIAGRAPH m_hMediaGraphTVA;	// �������ŵ�����Ƶ

	HMEDIAGRAPH m_hMediaGraphFM;	// ��������FM��Ƶ

	HMEDIAGRAPH m_hMediaGraphCamera;// ��������


	// ���pfn����ָ��Ϊ�գ���ʾ����û��ʹ��
	EVENT_HANDLER_INFO m_eventHandlerFile[MAX_EVENT_HANDLER_COUNT];

	// ���pfn����ָ��Ϊ�գ���ʾ����û��ʹ��
	EVENT_HANDLER_INFO m_eventHandlerAvin[MAX_EVENT_HANDLER_COUNT];


	BOOL m_bIsWmaPlaying;	// ������ʶ�Ƿ�wma���ڲ���

};

typedef enum
{
	MSDK_CAP_FILE_SEEK_SUPPORT			= 0x01   ,		//�ļ��ܷ�time seek
	MSDK_CAP_FILE_FF_SUPPORT			= 0x01<<1,		//�ļ��ܷ���
	MSDK_CAP_VIDEO_RESOLUTION_SUPPORT   = 0x01<<2,		//�ļ���Ƶ��������Ƿ�֧�֣�����128M��1080p�Ĳ�֧�֣�
	MSDK_CAP_VIDEO_BITRATE_SUPPORT      = 0x01<<3,		//�ļ���Ƶ�������Ƿ�֧��
	MSDK_CAP_VIDEO_FRAMERATE_SUPPORT    = 0x01<<4,		//�ļ���Ƶ֡���Ƿ�֧��
	MSDK_CAP_VIDEO_CODEC_SUPPORT        = 0x01<<5,		//�ļ���Ƶ���뷽ʽ�Ƿ�֧��
	MSDK_CAP_VIDEO_PROFILE_LEVEL_SUPPORT= 0x01<<6,		//�ļ���Ƶ����汾�Ƿ�֧�֣�����DIVX,  ����ֻ֧��DIVX3��4��5��
	MSDK_CAP_AUDIO_BITRATE_SUPPOPRT     = 0x01<<7,		//�ļ���Ƶ�������Ƿ�֧��
	MSDK_CAP_AUDIO_SAMPLERATE_SUPPORT   = 0x01<<8,		//�ļ���Ƶ�������Ƿ�֧��
	MSDK_CAP_AUDIO_CODEC_SUPPORT        = 0x01<<9,		//�ļ���Ƶ���뷽ʽ�Ƿ�֧��
	MSDK_CAP_AUDIO_PROFILE_LEVEL_SUPPORT= 0x01<<10       //�ļ���Ƶ����汾�Ƿ�֧��
}E_MSDK_CAPABILITY_T;