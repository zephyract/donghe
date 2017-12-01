#pragma once

#include "msdk_defs.h"
#include "DVP_Agent.h"
#include "dvp_cmd.h"
#include "GErr.h"

#include "uibase.h"

#define  DVP_LASTMEMORY_FILE_PATH	L"\\residentflash3\\app\\lastMem_dvp.txt"
//const DWORD DVP_LM_START_ADDRESS = 0x006da300; 
const int DVP_LM_SIZE = 256;
typedef struct {
	GTCHAR DVPLMInfo[DVP_LM_SIZE];
	GUINT8 DVPLMFileTye;	// 只对数据碟有效，记录处于哪种过滤状态，音乐，视频，图片，或全部
} DVP_LMFileInfo_T;

// overlay Transparent color
#define COLOR_SPACE_LOW_VALUE	0xF81F
#define COLOR_SPACE_HIGH_VALUE	0xF81F

const int DVD_MAX_EVENT_HANDLER_COUNT = 5;
typedef void (*PFN_DVDRENDER_EVENT_HANDLER)(UINT media_event, UINT param1, UINT param2, DWORD user_data);

enum
{
	SETTINGS_DVD_DOLBY_OFF,
	SETTINGS_DVD_DOLBY_18,
	SETTINGS_DVD_DOLBY_14,
	SETTINGS_DVD_DOLBY_38,
	SETTINGS_DVD_DOLBY_12,
	SETTINGS_DVD_DOLBY_58,
	SETTINGS_DVD_DOLBY_34,
	SETTINGS_DVD_DOLBY_78,
	SETTINGS_DVD_DOLBY_FULL,
};

struct DVD_EVENT_HANDLER_INFO
{
	PFN_DVDRENDER_EVENT_HANDLER pfn;
	DWORD	user_data;
};


class CDvdRender
{
public:
	CDvdRender(void);
	virtual ~CDvdRender(void);

public:
	static CDvdRender* GetInstance();
	BOOL Initialize(HWND hWnd);
	void Uninitialize();

	// interface for rendering
	// prect为空时，满屏显示
	void ShowVideo(BOOL bShow, RECT* prect=NULL, DVD_SINK_T sink=DVD_SINK_FRONT);
	void SetSink(DVP_SINK sink_type);
	void Play();
	void Pause();
	void Stop();
	void Next();
	void Previous();
	void CreateFileDatabase(GUINT8 uFilterType);
	void SetCodePage(GUINT32 u4CodePage);
	// 模式是循环切换，有事件通知或查询，去刷新当前的repeate状态
	void SetPBRepeat();
	void SetPBRandom();
	UCHAR GetRepeatMode();
	UCHAR GetPBCUserMode();

	void GotoRootMenu();
	// 0 ~ for title, 1 ~ for chapter
	BOOL ChooseDigitalNum(GUINT8 uType, GUINT8 uDigitalNum);

	// 在MENU状态下可以用命令操作菜单
	void MenuUp();
	void MenuDown();
	void MenuLeft();
	void MenuRight();
	void MenuEnter();

	void SwitchSubtitle();
	void SwitchPBCAngle();
	void GotoTitleMenu();
	void SwitchAudioChannel();
	void ZoomIn();
	void ZoomOut();
	void Rotate();

	void Seek(DWORD dwPosition);	// 单位秒
	BYTE GetState();
	DWORD GetDuration();		// 返回值单位秒
	DWORD GetCurPostion();		// 返回值单位秒

	// 出碟
	void Eject();

	// CD track info
	void InitCDTrackInfo(int nTotal);
	void SetCDTrackInfo(int nIndex, int nTrackLen);
	const CArray<int> &GetCDTrackInfo();
	void SetCDCurTrack(int nIndex);
	int GetCDCurTrack();
	int GetCDTotalTrack();

	// cd rip
	static const int RIP_BITRATE = 128;
	int GetCurFileCount();
	// 返回录制需要的字节数
	UINT GetRipTrkLen(int nTrack);
	BOOL SetRipPath(LPCTSTR lpszFileName, UINT len);
	// 如果不成功,返回错误码
	UINT StartRip(int nTrack);
	BOOL StopRip();

	/* source control */
	void LaunchSrc(DVP_SINK sink_type);
	void StopSrc();

	// 临时打开或关闭声音通道 for RDS
	void OpenAudio();
	void CloseAudio();

	// 窗口收到注册的消息后调用该接口处理消息
	void HandleEvent();
	// 如果注册数量已经超过最大值，则返回错误
	BOOL RegisterEventHandler(PFN_DVDRENDER_EVENT_HANDLER pfn, DWORD user_data);
	// 如果未找到该项，则返回错误
	BOOL UnRegisterEventHandler(PFN_DVDRENDER_EVENT_HANDLER pfn);

	void OnTimer(UINT_PTR nIDEvent);	// 计时器需主窗口转发过来

	BOOL ReadDVPLM();
	BOOL WriteDVPLM2File();
	// bReset : TRUE ~ 表示清除记忆信息,如果有出碟,需清除
	void SaveDVPLM(BOOL bReset = FALSE);
	DVP_LMFileInfo_T* GetDVPLMFileInfo();

protected:
	static GRESULT DvpAgentEventListener(GUINT32 u4Evt, GUINT32 u4Param1, GUINT32 u4Param2, GUINT32 u4CustomData);
	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);

protected:

	HWND m_hwnd;	// 关联的窗口
	DVP_LMFileInfo_T m_DVPLMFileInfo;
	DVP_SINK m_SinkType;

	// 如果pfn函数指针为空，表示该项没有使用
	DVD_EVENT_HANDLER_INFO m_eventHandler[DVD_MAX_EVENT_HANDLER_COUNT];

	//  如果是CD碟，记录CD每个track的时长, 记录总数即为曲目数量, 如果值为0,表明还没有获取到数据
	CArray<int> m_cdTrackInfo;
	int         m_cdCurTrack;	// 当前正在播放的track

	BOOL m_bIsRiping;	// 如果在rip时,退出源时还没stop,必须stop

	// 如果LaunchSrc()接口从没有调用过,认为DVD没有完全初始化过,否则认为初始化过.
	// 如果没有完全初始化过,调用出碟接口Eject(),会导致下一次读碟不正常. 
	// 用户有可能开机后，直接先出碟，再进碟，会导致错误。
	BOOL m_bInitialized;	
};
