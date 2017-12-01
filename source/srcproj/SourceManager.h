#pragma once
#include "uibase.h"

// 加载源时mute住MCU, 500ms后解mute
#define TIMER_ID_SOURCE_LONDING_MUTE	8040


struct SOURCE_INFO
{
	SOURCE_ID	sid;
	DVP_SINK	sink;
};

typedef enum _PLAYBACK_MASK_
{
	PLAYBACK_MASK_BT     = 0x01,		// 通话
	PLAYBACK_MASK_CAMERA = (0x01 << 1),	// 倒车
	PLAYBACK_MASK_RDS = (0x01 << 2), // RDS Alarm
	// 增加其它情况
}PLAYBACK_MASK;

class CSourceManager
{

public:
	CSourceManager(void);
	~CSourceManager(void);

public:
	static CSourceManager* CSourceManager::GetInstance();
	void OnTimer(UINT_PTR nIDEvent);	// 计时器需主窗口转发过来
	// 指定前排源
	void SetFrontSrc(SOURCE_ID sid);
	// 指定后排源
	void SetRearSrc(SOURCE_ID sid);
	// 指定前后排为同一个源
	void SetFrontRearSrc(SOURCE_ID sid);
	// 当前后排不一样时，后排源有时需在DVP_SINK_REAR,DVP_SINK_FRONTV_REAR间切换
	void SetRearSrcSink(DVP_SINK sink);
	// 同时指定前后排源
	//void SetFrontRearSrc(SOURCE_ID sid);
	// 返回当前前排源信息, 如果没有源，返回NULL
	SOURCE_INFO* GetFrontSrc();
	// 返回当前后排源信息, 如果没有源，返回NULL
	SOURCE_INFO* GetRearSrc();

	// 返回源的ID号，如果没有源，返回INVALID_SOURCE_ID
	SOURCE_ID GetFrontSrcID();
	SOURCE_ID GetRearSrcID();

	// 返回上一个源的ID号，如果没有源，返回收音机源ID
	SOURCE_ID GetLastFrontSrcID();
	// 为简单起见，后排不记录上一个源,返回当前排源ID,如果前排源ID不支持后排,则返回INVALID_SOURCE_ID
	SOURCE_ID GetLastRearSrcID();

	void SaveMemory_MSDK();	// 保存断点
	void SaveMemory_VDisk();	// 保存断点

	// 参数为什么原因引起的挂起或恢复
	void HangupPlayback(PLAYBACK_MASK maskEvent);
	void RestorePlayback(PLAYBACK_MASK maskEvent);

	// RDS报警时，需先关闭其它源的audio，报警完成后再打开,可以调用下面的接口实现
	void CloseAudio();
	void OpenAudio();

protected:
	// 下面三个函数直接对指定的源进行操作,启动或停止一个源，或转换源的状态
	void StopSrc(SOURCE_ID sid);
	void LaunchSrc(SOURCE_ID sid, DVP_SINK sink);
	void SetSrcSink(SOURCE_ID sid, DVP_SINK sink);

	// 返回sid是否是一个合法的ID号
	BOOL IsValidSrc(SOURCE_ID sid);

	// 启动退出MSDK源(SD, USB, USB_REAR)
	// 将文件的初始化，及断点保存功能放在这里完成
	void LaunchSrc_MSDK(SOURCE_ID sid, DVP_SINK sink);
	void StopSrc_MSDK(SOURCE_ID sid);

	//
	void LaunchSrc_VDisk(DVP_SINK sink);
	void StopSrc_VDisk();

	// 如果是VDisk启动的后排DVD源,则返回TRUE
	BOOL IsDVDSourceByVdisk(SOURCE_ID sid);
protected:
	// 支持前后排，有可能同时存在二个源
	// 每个源是负责前排或后排，或前后排是动态的，随时可以转换
	SOURCE_INFO m_src1;
	SOURCE_INFO m_src2;

	// 记录前排的上一个源ID,当当前源不可用时,可以回到上一个源,
	// 如果上一个源也不可用,回到收音机
	SOURCE_ID	m_srcLastFront;

	// 用来描述播放动作挂起的原因,如果标志为0,表示挂起标志清除了,应该继续播放
	// 不同的原因(BT通话,倒车等)用不同的位来表示，参见PLAYBACK_MASK
	UINT m_playback_hangup_flag;
	UINT m_playback_status;		// 挂起时,当前的播放状态,挂起结束后,恢复为该状态0~表示停止或暂停了,1~表示播放
	// 获取当前源播放状态
	UINT GetPlaybackStatus();
	// 当前源播放
	void PlaybackPlay();
	// 当前源暂停
	void PlaybackPause();
};
