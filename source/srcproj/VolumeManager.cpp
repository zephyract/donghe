#include "StdAfx.h"
#include "SourceManager.h"
#include "sysUtil.h"
#include "volumeManager.h"
#include "DVP_Agent.h"
#include "MainWnd.h"
#include "WceUiLayerCtrl.h"
#include "uiddraw.h"
#include "OverlayDlg.h"
#include "tzUtil.h"
#include "config.h"
#include "easyconnected.h"

UINT DVP_VOLUME_MSDK_TABLE[UI_MAX_VOLUME_LEVEL+1] =
{
	0, 272, 370, 472, 572,
	672, 772, 872, 1072,
	1372, 1672, 1872, 2072,
	3072, 4072, 5072, 6072,
	8072, 10072, 12072, 14072,
	16072, 16572, 19072, 21072,
	24072, 27072, 30072, 33072,
	38072, 43072, 48072, 54072,
	57072, 60072, 64072, 68072,
	84072, 100072, 115072, 131072
};

UINT DVP_VOLUME_AUX_FRONT_TABLE[UI_MAX_VOLUME_LEVEL+1] =
{
	0, 272,  672, 800, 1072,
	2072, 2572, 3072, 4072,
	6072, 8072, 10072, 13072,
	18072, 23072, 29072, 36072,
	47072, 58072, 70072, 82072,
	93072, 104072, 115072, 127072,
	145072, 163072, 181072, 198072,
	230072, 262072, 294072, 326072,
	344072, 362072, 380072, 399072,
	499072, 599072, 699072, 799072
};

// 后排AVIN的音量表，最大不能超过0x20000
UINT DVP_VOLUME_AUX_REAR_TABLE[UI_MAX_VOLUME_LEVEL+1] =
{
	0, 272,  672, 800, 1072,
	2072, 2572, 3072, 4072,
	5072, 6072, 7072, 8072, 
	9072, 10072, 11072, 13072,
	15072, 18072, 20072, 23072,
	26072, 29072,33072, 36072,
	41072, 47072, 52072, 58072, 
	65072, 70072, 76072, 82072,
	87072, 93072, 96072, 104072,
	109072, 115072, 127072, 131072
};

BYTE BT_VOLUME_TABLE[UI_MAX_VOLUME_LEVEL+1] =
{
	0x00,	0x03,	0x05,	0x07,
	0x0f,	0x19,	0x25,	0x2A,
	0x2F,	0x35,	0x3a,	0x3f,
	0x45,	0x4a,	0x4f,	0x55,
	0x5a,	0x5f,	0x65,	0x6a,
	0x6f,	0x75,	0x7a,	0x7f,
	0x85,	0x8a,	0x8f,	0x95,
	0x9a,	0x9f,	0xa5,	0xaa,
	0xaf,	0xb5,	0xbf,	0xc5,
	0xcf,	0xdf,	0xed,	0xfd,
	0xff
};

WORD WINCE_VOLUME_TABLE[UI_MAX_VOLUME_LEVEL+1] = 
{
	0x0000,	0x0a00,	0x0d00,	0x1500,
	0x1A00,	0x1F00,	0x2500,	0x2A00,
	0x2F00,	0x3500,	0x3a00,	0x3f00,
	0x4500,	0x4a00,	0x4f00,	0x5500,
	0x5a00,	0x5f00,	0x6500,	0x6a00,
	0x6f00,	0x7500,	0x7a00,	0x7f00,
	0x8500,	0x8a00,	0x8f00,	0x9500,
	0x9a00,	0x9f00,	0xa500,	0xaa00,
	0xaf00,	0xb500,	0xbf00,	0xc500,
	0xcf00,	0xdf00,	0xed00,	0xfd00,
	0xffff
};

CVolumeManager::CVolumeManager(void)
: m_McuMuteFlag(0)
, m_McuUnmuteFlag(0)
{

}

CVolumeManager::~CVolumeManager(void)
{
}

CVolumeManager* CVolumeManager::GetInstance()
{
	static CVolumeManager volume_manager;
	return &volume_manager;
}


void CVolumeManager::SetVolume(UI_VOLUME_TYPE eVolumeType, int nVolume, DVP_SINK eSinkType)
{
	if ((eVolumeType != VOLUME_TYPE_MEDIA && eVolumeType != VOLUME_TYPE_BT && eVolumeType != VOLUME_TYPE_GPS)
		|| (nVolume<0 || nVolume>UI_MAX_VOLUME_LEVEL)
		|| (eSinkType != DVP_SINK_FRONT && eSinkType != DVP_SINK_REAR && eSinkType != DVP_SINK_FRONT_REAR))
	{
		RETAILMSG(1, (L"[nvai_car]: CVolumeManager::SetVolume, invalid parameters.\n"));
		return;
	}

	if (eVolumeType == VOLUME_TYPE_MEDIA)
	{
		if (eSinkType == DVP_SINK_FRONT)
		{
			sysutil::nss_get_instance()->audio_media_front_volume = nVolume;
		}
		else if (eSinkType == DVP_SINK_REAR)
		{
			sysutil::nss_get_instance()->audio_media_rear_volume = nVolume;
		}
		else if (eSinkType == DVP_SINK_FRONT_REAR)
		{
			sysutil::nss_get_instance()->audio_media_front_volume = nVolume;
			sysutil::nss_get_instance()->audio_media_rear_volume = nVolume;
		}
	}
	else if (eVolumeType == VOLUME_TYPE_BT)
	{
		sysutil::nss_get_instance()->audio_bt_volume = nVolume;
	}
	else if (eVolumeType == VOLUME_TYPE_GPS)
	{
		sysutil::nss_get_instance()->audio_gps_volume = nVolume;
	}
}


void CVolumeManager::SetVolumeMute(UI_VOLUME_TYPE eVolumeType, BOOL bMute, DVP_SINK eSinkType)
{
	if ((eVolumeType != VOLUME_TYPE_MEDIA && eVolumeType != VOLUME_TYPE_BT && eVolumeType != VOLUME_TYPE_GPS)
		|| (eSinkType != DVP_SINK_FRONT && eSinkType != DVP_SINK_REAR && eSinkType != DVP_SINK_FRONT_REAR))
	{
		RETAILMSG(1, (L"[nvai_car]: CVolumeManager::SetVolumeMute, invalid parameters.\n"));
		return;
	}

	if (eVolumeType == VOLUME_TYPE_MEDIA)
	{
		if (eSinkType == DVP_SINK_FRONT)
		{
			sysutil::nss_get_instance()->audio_is_media_front_mute = bMute;
		}
		else if (eSinkType == DVP_SINK_REAR)
		{
			sysutil::nss_get_instance()->audio_is_media_rear_mute = bMute;
		}
		else if (eSinkType == DVP_SINK_FRONT_REAR)
		{
			sysutil::nss_get_instance()->audio_is_media_front_mute = bMute;
			sysutil::nss_get_instance()->audio_is_media_rear_mute = bMute;
		}
	}
	else if (eVolumeType == VOLUME_TYPE_BT)
	{
		sysutil::nss_get_instance()->audio_is_bt_mute = bMute;
	}
	else if (eVolumeType == VOLUME_TYPE_GPS)
	{
		sysutil::nss_get_instance()->audio_is_gps_mute = bMute;
	}
}


void CVolumeManager::UpdateVolume()
{
	ValidateVolumeLevel();

	if (sysutil::nss_get_instance()->sys_status == UISS_ACC_OFF
		|| sysutil::nss_get_instance()->sys_status == UISS_SLEEP)	// acc off状态下,所有音量静音
	{
		SetDVPVolume(DVP_SINK_FRONT_REAR, 0);	// 媒体静音
		SetGPSVolumeGain(0);	// gps静音
		SetBTVolume(0);		// BT静音
	}
	else if (sysutil::nss_get_instance()->sys_rds_status)	// rds报警
	{
		// 后排静音,前排为AUX音量
		SetDVPVolume(DVP_SINK_REAR, 0);
		int gain = sysutil::nss_get_instance()->audio_is_media_front_mute ? 0 : 1;
		UINT volume = DVP_VOLUME_AUX_FRONT_TABLE[sysutil::nss_get_instance()->audio_media_front_volume];
		SetDVPVolume(DVP_SINK_FRONT, volume*gain);
		// 系统静音
		//SetWinceVolume(0);
		// BT静音
		SetBTVolume(0);
	}
	else if (sysutil::nss_get_instance()->sys_bt_call_status
		|| sysutil::nss_get_instance()->sys_ecar_status)	// 蓝牙来电 或者  ECar状态
	{
		SetDVPVolume(DVP_SINK_FRONT_REAR, 0);	// 媒体静音
		SetGPSVolumeGain(0);	// gps静音
		int gain = sysutil::nss_get_instance()->audio_is_bt_mute ? 0 : 1;
		SetBTVolume(BT_VOLUME_TABLE[sysutil::nss_get_instance()->audio_bt_volume] * gain);
		//SetWinceVolume(WINCE_VOLUME_TABLE[sysutil::nss_get_instance()->audio_bt_volume] * gain);	// 蓝牙铃声音量
	}
	else if (sysutil::nss_get_instance()->sys_camera_status)	// 倒车
	{
		SetDVPVolume(DVP_SINK_FRONT_REAR, 0);	// 媒体静音
		SetGPSVolumeGain(0);	// gps静音
	}
	else if (sysutil::nss_get_instance()->sys_status == UISS_SLEEP_AWAKE)
	{
		SetDVPVolume(DVP_SINK_FRONT_REAR, 0);	// 媒体静音
		SetGPSVolumeGain(0);	// gps静音
	}
	else if (sysutil::nss_get_instance()->sys_gps_guiding)	// gps语音播报
	{
		// 调整媒体音量
		UINT *pTable = GetMediaVolumeTable(DVP_SINK_REAR);
		int gps_gain = sysutil::nss_get_instance()->audio_gps_gain;
		if (sysutil::nss_get_instance()->sys_gps_foreground		// 如果gps前台而且设置了关闭导航背景音,媒体音量应为0
			&& sysutil::nss_get_instance()->audio_is_gps_gain_off)
		{
			gps_gain = 0;
		}
		int gain = sysutil::nss_get_instance()->audio_is_media_rear_mute ? 0 : 1;
		UINT volume = pTable[sysutil::nss_get_instance()->audio_media_rear_volume];
		SetDVPVolume(DVP_SINK_REAR, volume*gain*gps_gain/100);

		pTable = GetMediaVolumeTable(DVP_SINK_FRONT);
		gain = sysutil::nss_get_instance()->audio_is_media_front_mute ? 0 : 1;
		volume = pTable[sysutil::nss_get_instance()->audio_media_front_volume];
		SetDVPVolume(DVP_SINK_FRONT, volume*gain*gps_gain/100);

		// gps音量
		SetGPSVolumeGain((DWORD)-1);
		gain = sysutil::nss_get_instance()->audio_is_gps_mute ? 0 : 1;
		//SetWinceVolume(WINCE_VOLUME_TABLE[sysutil::nss_get_instance()->audio_gps_volume] * gain);
		SetWinceVolume(WINCE_VOLUME_TABLE[16]);	//该版本音量交由igo地图自己管理,系统音量调到一个固定值,igo地图音量调节好像是内部管理,不是通过调节系统音量实现的

		// 蓝牙优先级高,gps语音播报时,蓝牙不会发声,所以不用设置蓝牙音量
	}
	else	// 媒体播放 
	{
		UINT *pTable = GetMediaVolumeTable(DVP_SINK_REAR);
		int gain = sysutil::nss_get_instance()->audio_is_media_rear_mute ? 0 : 1;
		int gps_gain = 1;
		if (sysutil::nss_get_instance()->sys_gps_foreground		// 如果gps前台而且设置了关闭导航背景音,媒体音量应为0
			&& sysutil::nss_get_instance()->audio_is_gps_gain_off)
		{
			gps_gain = 0;
		}


		UINT volume = pTable[sysutil::nss_get_instance()->audio_media_rear_volume];
		SetDVPVolume(DVP_SINK_REAR, volume*gain*gps_gain);

		pTable = GetMediaVolumeTable(DVP_SINK_FRONT);
		gain = sysutil::nss_get_instance()->audio_is_media_front_mute ? 0 : 1;
		volume = pTable[sysutil::nss_get_instance()->audio_media_front_volume];

		// 如果MCU是mute状态,把前排的媒体音量也设置为0
		int mcu_mute_gain = protocol::get_mcu_volume_table()->is_mute ? 0 : 1;
		SetDVPVolume(DVP_SINK_FRONT, volume*gain*gps_gain*mcu_mute_gain);

		// 当前源为EasyConnected,且连接的是ios时，设置EasyConnected音量(系统音量)
		if (CSourceManager::GetInstance()->GetFrontSrcID() == SOURCE_EASY_CONNECTED
			&& CEasyConnected::GetInstance()->GetDeviceType() == 1)
		{
			volume = WINCE_VOLUME_TABLE[sysutil::nss_get_instance()->audio_media_front_volume];
			SetWinceVolume(volume*gain*gps_gain*mcu_mute_gain);
		}
	}
}

// 将volume值限制在合法范围内
void CVolumeManager::ValidateVolumeLevel(int* pVolume)
{
	if (pVolume)
	{
		if (*pVolume < 0)
		{
			*pVolume = 0;
		}
		else if (*pVolume > UI_MAX_VOLUME_LEVEL)
		{
			*pVolume = UI_MAX_VOLUME_LEVEL;
		}
	}
}

void CVolumeManager::ValidateVolumeLevel()
{
	ValidateVolumeLevel(&sysutil::nss_get_instance()->audio_media_front_volume);
	ValidateVolumeLevel(&sysutil::nss_get_instance()->audio_media_rear_volume);
	ValidateVolumeLevel(&sysutil::nss_get_instance()->audio_init_media_max_volume);
	ValidateVolumeLevel(&sysutil::nss_get_instance()->audio_bt_volume);
	ValidateVolumeLevel(&sysutil::nss_get_instance()->audio_gps_volume);
}

void CVolumeManager::SetDVPVolume(DVP_SINK type, UINT volume)
{
	GHANDLE hAudioDev = GOpenAudioDev();
	if (type == DVP_SINK_FRONT)
	{
		GClientSetVolume(hAudioDev, volume);
	}
	else if (type == DVP_SINK_REAR)
	{
		GClientSetRearVolume(hAudioDev, volume);
	}
	else if (type == DVP_SINK_FRONT_REAR)
	{
		GClientSetVolume(hAudioDev, volume);
		GClientSetRearVolume(hAudioDev, volume);
	}
	GCloseAudioDev(hAudioDev);
}

void CVolumeManager::SetWinceVolume(DWORD volume)
{
	volume = (volume<<16) + volume;
	waveOutSetVolume(0, volume);
}

void CVolumeManager::SetBTVolume(BYTE volume)
{
	waveOutMessage((HWAVEOUT)0, WAVMSG_BT_SET_VOLUME, (DWORD)volume, 0);
}

void CVolumeManager::SetGPSVolumeGain(DWORD gain)
{
	waveOutMessage((HWAVEOUT)0, WAVMSG_GPS_SET_VOLUME, 0, gain);
}

void CVolumeManager::SetGPSProcessID(DWORD dwProcessID)
{
	waveOutMessage( (HWAVEOUT)0, WAVMSG_GPS_SET_PROCESS_ID, 0, dwProcessID);	
}

void CVolumeManager::EnableGPSSetDeviceGain(BOOL bEnable)
{
	waveOutMessage( (HWAVEOUT)0, WAVMSG_GPS_ENABLE_SET_DEVICE_GAIN, (DWORD)bEnable, 0 );				
}

BOOL CVolumeManager::IsAuxSourceID(SOURCE_ID sid)
{
	if (sid == SOURCE_FM || sid == SOURCE_AVIN1 || sid == SOURCE_AVIN2 || sid == SOURCE_TV)		// 可能还有其它源
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

UINT* CVolumeManager::GetMediaVolumeTable(DVP_SINK eSinkType)
{
	if (IsAuxSourceID(CSourceManager::GetInstance()->GetFrontSrcID()))
	{
		if (eSinkType == DVP_SINK_REAR)
		{
			return DVP_VOLUME_AUX_REAR_TABLE;
		}
		else
		{
			return DVP_VOLUME_AUX_FRONT_TABLE;
		}
	}
	else
	{
		return DVP_VOLUME_MSDK_TABLE;
	}
}


void CVolumeManager::SetDAC(BOOL front_dac, BOOL rear_dac)
{
	if (front_dac)
	{
		GAudSelDacType(AUD_FRONT, AUD_DAC_EXT);
	}
	else
	{
		GAudSelDacType(AUD_FRONT, AUD_DAC_PWM);
	}

	if (rear_dac)
	{
		GAudSelDacType(AUD_REAR, AUD_DAC_EXT);
	}
	else
	{
		GAudSelDacType(AUD_REAR, AUD_DAC_PWM);
	}
}


void CVolumeManager::Init()
{
#define SPEAKER_LAYOUT_MASK 0xFF

	//setDownMix();
	//setUpmix();
	//return;

	if ( config::get_config_functions()->is_support(SOURCE_DVD) 
		&& CSourceManager::GetInstance()->GetFrontSrcID() != SOURCE_DVD
		&& CSourceManager::GetInstance()->GetRearSrcID() != SOURCE_DVD)
	{
		DVP_Init();
	}

	GHANDLE hAudioDev = GOpenAudioDev();

	UINT32 u4SpeakerSize = 0;
	UINT32 u4SpeakerLayout = SPEAKER_LAYOUT_STEREO;

	//SetFront();
	UINT32 u4ReturnValue;
	u4SpeakerSize |= (SPEAKER_LAYOUT_L_LARGE | SPEAKER_LAYOUT_R_LARGE | SPEAKER_LAYOUT_LS_LARGE | SPEAKER_LAYOUT_RS_LARGE);

	u4SpeakerSize |= (SPEAKER_LAYOUT_L_LARGE | SPEAKER_LAYOUT_R_LARGE);
	u4ReturnValue = GSetSpeakerLayout(hAudioDev, u4SpeakerLayout, u4SpeakerSize);
	u4ReturnValue = DVP_GSetSpeakerLayout(TRUE,u4SpeakerLayout,u4SpeakerSize);


	//SetRear();
	UINT32 u4TempSpeakerLayout;
	u4SpeakerSize &= (~(SPEAKER_LAYOUT_LS_LARGE | SPEAKER_LAYOUT_RS_LARGE));
	u4TempSpeakerLayout = SPEAKER_LAYOUT_LRLSRS; 
	u4SpeakerLayout = (u4SpeakerLayout & (~SPEAKER_LAYOUT_MASK)) | u4TempSpeakerLayout;
	u4ReturnValue = GSetSpeakerLayout(hAudioDev, u4SpeakerLayout, u4SpeakerSize);
	u4ReturnValue = DVP_GSetSpeakerLayout(TRUE,u4SpeakerLayout,u4SpeakerSize);


	//SetUpMix();
	INT32 i4Upmix;
	MISC_UPMIX_GAIN_T rUpmixGain;
	i4Upmix = AUD_UPMIX_ON;

	//1.5797835966168101567500723704814
	//1.7993405494535817053022720651029
	//1.6901960800285136614244325171853



	//rUpmixGain.u4Gain0 = 0x004CCCCC;
	//rUpmixGain.u4Gain1 = 0x00199999;
	//rUpmixGain.u4Gain2 = 0x00199999;
	//rUpmixGain.u4Gain3 = 0x004CCCCC;

	//rUpmixGain.u4Gain0 =   0x007fffff;
	//rUpmixGain.u4Gain3 =   0x007fffff;

	rUpmixGain.u4Gain0 =   0x00620000;
	rUpmixGain.u4Gain1 =   0x00000000;
	rUpmixGain.u4Gain2 =   0x00000000;
	rUpmixGain.u4Gain3 =   0x00620000;

	rUpmixGain.u4Gain4 = 0x00333333;
	rUpmixGain.u4Gain5 = 0x00333333;
	rUpmixGain.u4Gain6 = 0x00666666;
	rUpmixGain.u4Gain7 = 0x00666666;
	GClientSetUpMix(hAudioDev,(AUD_UPMIX_T) i4Upmix,rUpmixGain);


	//SetSub();
	u4SpeakerLayout |= SPEAKER_LAYOUT_SUBWOOFER;
	u4ReturnValue = GSetSpeakerLayout(hAudioDev, u4SpeakerLayout, u4SpeakerSize);
	u4ReturnValue = DVP_GSetSpeakerLayout(TRUE,u4SpeakerLayout,u4SpeakerSize);

	//SetAudioFuncOption();
	MISC_AUD_FUNC_OPTION_T rFuncOptionSet = {0};
	rFuncOptionSet.u4FuncOption0 = 1;
	GAudioFuncOptionSet(hAudioDev, rFuncOptionSet);

	GCloseAudioDev(hAudioDev);

	if ( config::get_config_functions()->is_support(SOURCE_DVD) 
		&& CSourceManager::GetInstance()->GetFrontSrcID() != SOURCE_DVD
		&& CSourceManager::GetInstance()->GetRearSrcID() != SOURCE_DVD)
	{
		DVP_Deinit();
	}

}

#include "WceUiLoadString.h"
void CVolumeManager::ShowUI(UI_VOLUME_TYPE eVolumeType, DWORD dwTime, int nStep)
{
	CWceUiLoadString load_str;
	int volume = 0;
	if (eVolumeType == VOLUME_TYPE_GPS)
	{
		load_str.SetTextResID(L"VOLUME_NAVIGATION_VOL");
		volume = sysutil::nss_get_instance()->audio_gps_volume;			
	}
	else if (eVolumeType == VOLUME_TYPE_BT)
	{
		load_str.SetTextResID(L"VOLUME_BLUETOOTH_VOL");
		volume = sysutil::nss_get_instance()->audio_bt_volume;
	}
	else if (eVolumeType == VOLUME_TYPE_MEDIA)
	{
		load_str.SetTextResID(L"VOLUME_MEDIA_VOL");
		volume = sysutil::nss_get_instance()->audio_media_front_volume;
	}
	else	// VOLUME_TYPE_NULL
	{
		// 没指定类型时显示前排音量,后面看要不要修改
		load_str.SetTextResID(L"VOLUME_MEDIA_VOL");
		volume = sysutil::nss_get_instance()->audio_media_front_volume;
	}

	COverlayDlg::GetInstance()->Vol_ShowUI(eVolumeType);
	int sign = nStep>=0 ? 1 : -1;
	for (int i=0; i<=abs(nStep); i++)
	{
		COverlayDlg::GetInstance()->Vol_SetData(load_str.GetString(), volume-nStep+i*sign, dwTime, nStep);

		int vol = sysutil::nss_get_instance()->audio_is_media_front_mute ? -1 : volume-nStep+i*sign;
		ui_led_show_volume(vol);
		CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
	}

	if (nStep == 0)	// 可能只是mute状态变化
	{
		int vol = sysutil::nss_get_instance()->audio_is_media_front_mute ? -1 : volume;
		ui_led_show_volume(vol);
		CRpcMCU::GetInstance()->RPC_LedDisplayData(ui_led_get_data());
	}
}

void CVolumeManager::HideUI()
{
	COverlayDlg::GetInstance()->Vol_HideUI();
}

void CVolumeManager::SetMcuUnmuteFlag(UNMUTE_MCU_MASK maskEvent, BOOL bSet)
{
	if (bSet)
	{
		m_McuUnmuteFlag |= maskEvent;
	}
	else
	{
		m_McuUnmuteFlag &= (~maskEvent);
	}
	UpdateMcuMute();
}

void CVolumeManager::SetMcuMuteFlag(MUTE_MCU_MASK maskEvent, BOOL bSet)
{
	if (bSet)
	{
		m_McuMuteFlag |= maskEvent;
	}
	else
	{
		m_McuMuteFlag &= (~maskEvent);
	}
	UpdateMcuMute();
}

void CVolumeManager::UpdateMcuMute()
{
	BOOL bNeedMute = FALSE;
	if (m_McuUnmuteFlag)	// 只要有unmute请求,就要解mute
	{
		bNeedMute = FALSE;
	}
	// MCU静音后,BEEP声发不出来，所以不能长时间MUTE,不能采用下面的设计。
// 	else if (sysutil::nss_get_instance()->is_audio_mute())	// 用户静音了
// 	{
// 		bNeedMute = TRUE;
// 	}
	else if (m_McuMuteFlag)	// 有静音请求
	{
		bNeedMute = TRUE;
	}
// 	else if (sysutil::nss_get_instance()->audio_media_front_volume == 0)	// 媒体音量为0时也静音
// 	{
// 		bNeedMute = TRUE;
// 	}
	else	// 一般情况是不静音的
	{
		bNeedMute = FALSE;
	}

	// mute状态有变化时才给MCU发命令
	if(!tzutil::IsBoolEqual(bNeedMute, protocol::get_mcu_volume_table()->is_mute))
	{
		protocol::get_mcu_volume_table()->is_mute = bNeedMute;
		if ( config::get_config_audio()->mute_by_app)
		{
			CRpcMCU::GetInstance()->RPC_SetVolume(protocol::get_mcu_volume_table());
		}
		UpdateVolume();
	}
}
