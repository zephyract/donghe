#pragma once
#include "uibase.h"
#include "WceUi32Bitmap.h"
#include "WceUi565Bitmap.h"

typedef enum _UI_VOLUME_TYPE_
{
	VOLUME_TYPE_NULL,	// 如果是对几种音量有效,如静音,不需显示TYPE,可以设置此枚举
	VOLUME_TYPE_MEDIA,
	VOLUME_TYPE_GPS,
	VOLUME_TYPE_BT,
	VOLUME_TYPE_TA,
} UI_VOLUME_TYPE;

// 音量范围为0 ~ 40
#define UI_MAX_VOLUME_LEVEL	40
extern UINT DVP_VOLUME_MSDK_TABLE[UI_MAX_VOLUME_LEVEL+1];
extern UINT DVP_VOLUME_AUX_FRONT_TABLE[UI_MAX_VOLUME_LEVEL+1];
extern UINT DVP_VOLUME_AUX_REAR_TABLE[UI_MAX_VOLUME_LEVEL+1];
extern BYTE BT_VOLUME_TABLE[UI_MAX_VOLUME_LEVEL+1];
extern WORD WINCE_VOLUME_TABLE[UI_MAX_VOLUME_LEVEL+1];

#define WAVMSG_BT_SET_VOLUME				0x412
#define	WAVMSG_GPS_SET_VOLUME				(WM_USER + 0X15)
#define	WAVMSG_GPS_SET_PROCESS_ID			(WM_USER + 0X13)
#define	WAVMSG_GPS_ENABLE_SET_DEVICE_GAIN	(WM_USER + 0x52)

#define	WAVMSG_GPS_STREAMING_NOTIFY			0x3644	// gps发声时通知该消息, 让他通知指定窗口，减少对系统干扰


// 为避免POP声或背噪，可以给MCU发送mute命令来解决
// 策略：
// 有mute请求时,设置对应的标志,mute完毕后，应清除相应的标志
// 有些事件需忽略当前的mute状态，必须发声的,设置对应的unmute标志，动作完成后,解除对应的标志
// 系统会根据这二个标志，来决定当前MCU的mute状态。
typedef enum _UNMUTE_MCU_MASK_
{
	UNMUTE_MCU_MASK_BT   = 0x01,		// 通话
	UNMUTE_MCU_MASK_NAVI = (0x01 << 1),	// 导航播报
	UNMUTE_MCU_MASK_RDS  = (0x01 << 2), // RDS Alarm
	// 增加其它情况
}UNMUTE_MCU_MASK;

typedef enum _MUTE_MCU_MASK_
{
	MUTE_MCU_MASK_INVALID_SRC   = 0x00,		// 无源状态(可避免背噪声音)		-- 该情况暂时不能静音,影响BEEP声
	MUTE_MCU_MASK_SRC_LOADING	= (0x01 << 1),	// 加载源时(刚进源时可能会有pop声,可mute 500ms), 目前只针对前排源
	// 增加其它情况
}MUTE_MCU_MASK;


class CVolumeManager
{

public:
	CVolumeManager(void);
	~CVolumeManager(void);

public:
	static CVolumeManager* CVolumeManager::GetInstance();

	static void Init();
	static void SetDAC(BOOL front_dac, BOOL rear_dac);

	// 根据系统的当前状态将音量调整到合适的值
	static void UpdateVolume();

	// 设定指定类型的音量
	// 前后排参数只对媒体音量有效,用来指定设置前排音量，后排音量，还是前后排一起设置
	static void SetVolume(UI_VOLUME_TYPE eVolumeType, int nVolume, DVP_SINK eSinkType = DVP_SINK_FRONT_REAR);
	static void SetVolumeMute(UI_VOLUME_TYPE eVolumeType, BOOL bMute, DVP_SINK eSinkType = DVP_SINK_FRONT_REAR);

	// 设置DVP音量
	static void SetDVPVolume(DVP_SINK type, UINT volume);
	// 设置wince系统音量
	static void SetWinceVolume(DWORD volume);
	// 设置蓝牙通话音量
	static void SetBTVolume(BYTE volume);
	// 设置GPS增益值 0 ~ 使GPS静音, 0xFFFFFFFF ~ GPS以当前系统音量正常发音
	static void SetGPSVolumeGain(DWORD gain);	// 0 ~ 0xFFFFFFFF
	// 设置需监控的GPS进程
	static void SetGPSProcessID(DWORD dwProcessID);


	// 允许开禁止导航修改系统音量
	static void EnableGPSSetDeviceGain(BOOL bEnable);

	// 防止音量值越界,如果越界了,调整为合法值
	static void ValidateVolumeLevel();

	// 将传进来的参数值调整为合法值
	static void ValidateVolumeLevel(int* pVolume);

protected:
	// 根据当前的源状态,返回正确的音量表,如果与AUX相关返回AUX的表,否则返回MSDK表
	static UINT* GetMediaVolumeTable(DVP_SINK eSinkType);

	// 判断该源是否是AUX类别的源
	static BOOL IsAuxSourceID(SOURCE_ID sid);

// UI Manager
public:
	// 显示音量UI,
	// eVolumeType ~ 显示哪个音量值
	// dwTime ~ 显示多长时间(单位ms), 0xFFFFFFFF表示一直显示
	// nStep ~ 用来描述此次音量变化了多少，用来优化UI显示
	void ShowUI(UI_VOLUME_TYPE eVolumeType, DWORD dwTime, int nStep);
	void HideUI();

	// bSet : TRUE ~ 设置标志  FALSE ~ 清除标志
	void SetMcuUnmuteFlag(UNMUTE_MCU_MASK maskEvent, BOOL bSet);
	void SetMcuMuteFlag(MUTE_MCU_MASK maskEvent, BOOL bSet);
	void UpdateMcuMute();	// 根据当前的状态,更新MCU的mute状态
protected:
	DWORD m_McuMuteFlag;
	DWORD m_McuUnmuteFlag;
};
