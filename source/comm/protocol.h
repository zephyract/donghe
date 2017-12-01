#pragma once

#include "uibase.h"
#include <winbase.h>

#define NS_PROTOCOL_BEGIN	namespace protocol {
#define NS_PROTOCOL_END	}

struct MCU_STATUS
{
	BYTE data[9];
	BOOL is_CDC_exist();
	BOOL is_DISK_exist();
	BOOL is_DISK_autoin();	// TRUE:出碟片后没拔出自动吸入了，FALSE：由用户主动进碟
	BOOL is_camera_on();	// 是否在倒车
	BOOL is_light_on();		// 汽车大灯是否打开
	BOOL is_brake_on();		// 是否处于刹车状态
	int  get_source_id();	// MCU指定源，优先级比APP指定的源要高，如果指定，APP启动时进入该源
	BOOL is_sleep();		// 开机时,从MCU获取假关机状态
};



struct MCU_SYS_PARA
{
	typedef enum _E_PARA_INDEX{
		INDEX_TIME = 0x02,
		INDEX_GENERAL = 0X03,
		INDEX_VIDEO = 0x04,
		INDEX_FM   = 0x05,
		INDEX_RDS  = 0x06,
		INDEX_CAMERA = 0x08,
	} E_PARA_INDEX;
	BYTE fm_data[6];
	BYTE rds_data[3];
	BYTE time_data[8];
	BYTE video_data[10];		// 协议中的后四个字节没用
	BYTE general_data[2];		// 通用设置目前还没用到
	// fm
	int get_fm_cur_region();
	void set_fm_cur_region(BYTE region);
	int get_fm_default_region();
	// time
	void get_system_time(OUT SYSTEMTIME *ptm);
	void set_system_time(IN SYSTEMTIME *ptm);
	// video
	void set_video_data(int backlight, int brightness, int contrast, int hue, int satruation, int camera_mirror);
	//rds
	BOOL is_rds_AF_on();
	BOOL is_rds_TA_on();
	BOOL is_rds_EON_on();
	BOOL is_rds_REGION_on();
	// colorful light
	int get_colorful_light();
	void set_colorful_light(int value);
};

// MCU信息表格式 0xA2
struct MCU_INIT_TABLE
{
	BYTE data[64];			// 扩展信息不能太长，整个包不能超过64字节
	BYTE extra_data_len;	// 扩展信息长度
	void get_system_time(SYSTEMTIME *ptm);
	// 获取扩展信息, 长度通过返回值返回
	int  get_extra_data(OUT void** ppdata);
};

struct MCU_VOLUME_TABLE
{
	BYTE front_volume;
	BYTE rear_volume;
	BYTE fade_balance_LR;
	BYTE fade_balance_FR;
	BYTE is_mute;
	BYTE gps_gain;
	BYTE bt_volume;
	BYTE key_beep_on;
};

struct MCU_RADIO_STATUS
{
	WORD min_freq;		// 最小频点
	WORD max_freq;		// 最大频点
	BYTE granularity;	// 步长
	BYTE signal;		// 信号强度
	BYTE status[2];		// radio的状态参数，详细含义见协议
	BYTE rds_status;	// rds的状态参数，详细含义见协议
	BOOL is_use_stereo();	// 是否使用立体声播放
	BOOL is_stereo_status();	// 是否处于立体声播放状态
	BOOL is_DX_mode();	// 灵敏度是否为远程
	BOOL is_scan();
	BOOL is_preview_scan();	// 浏览预设电台 
	void SetData(BYTE* pdata);	// pdata指向MCU发上来的数据
	// rds
	BOOL is_EWS();		// RDS报警
	BOOL is_TA_Alarm();	// TA广播
	BOOL is_TA_seek();
	BOOL is_PTY_seek();
	BOOL is_AF_seek();
};

struct MCU_RADIO_PRESET_INFO
{
	BYTE band;				// 0 ~ FM1, 1 ~ FM2, 2 ~ FM3, 3 ~ AM1, 4 ~ AM2,
	BYTE preset_index;		// 当前的预设值, 取值0~6， 0表示当前频点不在预设值
	WORD cur_freq;			// 当前频点
	WORD preset_list[6];	// 预设频点信息表
	void SetData(BYTE* pdata);	// pdata指向MCU发上来的数据
};

struct MCU_RADIO_PS_PTY
{
	char PS[9];		// 节目名称,协议中是8个字节,这里定义9个字节,加上了字符串结尾符
	BYTE PTY;		// 节目类型
	void SetData(BYTE* pdata);	// pdata指向MCU发上来的数据
};

struct MCU_RADIO_RT_INFO
{
	BYTE len;		// 0 ~ 表示无RT信息，  n ~ 表示RT信息的长度(n只能为1~64)
	char RT[65];	// RT信息,最长为64字节,这里加上字符串结尾符
	void SetData(BYTE* pdata);	// pdata指向MCU发上来的数据
};


struct MCU_SWC_TABLE
{
	BYTE key[12][2];		// 12个键值, byte0 ~ flag, byte1 ~ IR key
};

struct MCU_ARM_STATUS
{
	BYTE data[3];
	void set_bt_active(BOOL is_active);		// 第三字节的Bit3来表示当前是否处于BT或BT通话的界面
	BOOL is_bt_active();	// 返回之前记忆的状态
};

NS_PROTOCOL_BEGIN
// MCU状态
MCU_STATUS* get_mcu_status();
void set_mcu_status(MCU_STATUS* pstatus);


MCU_SYS_PARA* get_mcu_sys_para();
// 设置系统参数，pdata不包括协议中定义的第一个字节，即索引
void set_mcu_sys_para(int index, LPVOID pdata);

MCU_INIT_TABLE* get_mcu_init_table();
void set_mcu_init_table(LPVOID pdata, int extra_data_len);

MCU_VOLUME_TABLE* get_mcu_volume_table();
void set_mcu_init_table(MCU_VOLUME_TABLE* pvolume_table);

// RADIO
MCU_RADIO_STATUS* get_mcu_radio_status();
void set_mcu_radio_status(MCU_RADIO_STATUS* pstatus);

MCU_RADIO_PRESET_INFO* get_mcu_radio_presetinfo();
void set_mcu_radio_presetinfo(MCU_RADIO_PRESET_INFO* pinfo);

MCU_RADIO_PS_PTY* get_mcu_radio_ps_pty();
void set_mcu_radio_ps_pty(MCU_RADIO_PS_PTY* pps_pty);

MCU_RADIO_RT_INFO* get_mcu_radio_rt_info();
void set_mcu_radio_rt_info(MCU_RADIO_RT_INFO* prt_info);

// swc
MCU_SWC_TABLE* get_mcu_swc_table();
void set_mcu_swc_table(MCU_SWC_TABLE* pswc_table);

// arm status
MCU_ARM_STATUS* get_mcu_arm_status();
NS_PROTOCOL_END



//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//--------------------------  KEY Define	---------------------------------------
#define K_NONE				0x00
#define K_POWER				0x01
#define K_MUTE				0x02
#define K_EQ				0x03
#define K_VOL_UP			0x04
#define K_VOL_DN			0x05
#define K_PHONE_ON			0x06
#define K_PHONE_OFF			0x07

#define	K_SOURCE_MODE		0x08
#define K_SOURCE_HOME		0x09

#define K_HOME_FRONTSRC		0x0A	


//#define	K_SOURCE_MENU		0x08
//#define	K_SOURCE_MODE		0x09


//#define K_PLAY			0x10
//#define K_PAUSE			0x11
//#define K_STOP			0x12
//#define K_NEXT			0x13
//#define K_PREV			0x14
//#define	K_FASTF			0x15
//#define K_FASTB			0x16
//#define K_SLOWF			0x17
//#define K_SLOWB			0x18
//#define K_STEP			0x19
//#define K_REPEAT		0x1A
//#define K_SHUFFLE		0x1B
//#define K_SCAN			0x1C
//#define K_TRACK			0x1E

#define K_EJECT			0x1D


#define K_0				0x20
#define K_1				0x21
#define K_2				0x22
#define K_3				0x23
#define K_4				0x24
#define K_5				0x25
#define K_6				0x26
#define K_7				0x27
#define K_8				0x28
#define K_9				0x29
#define K_UP			0x2A
#define K_DOWN			0x2B
#define K_LEFT			0x2C
#define K_RIGHT			0x2D
#define K_ENTER			0x2E
#define K_RETURN        0x2F
#define K_STAR			0x30
#define K_NUMBER        0x31
#define K_CLEAR			0x32
#define K_PLUS			0x33	// "+"号

#define K_MUSIC			0x40
#define K_PHOTO			0x41
#define K_VIDEO			0x42
#define K_NAVI			0x43
#define K_TUNER			0x44
#define K_DVD			0x45
#define K_BLUETOOTH		0x46
#define K_SETTING		0X47
#define K_TV			0x48
#define K_USB			0x49
#define K_SD_CARD		0x4A
#define K_PHONELINK		0x4B
#define K_AVIN1			0x4C
#define K_AVIN2			0x4D
#define K_TOUCH_MENU	0X4F



#define T_RADIO_BAND				0x50
#define T_RADIO_TUNE_UP				0x51
#define T_RADIO_TUNING_DOWN         0x52
#define T_RADIO_SEEK_UP				0x53
#define T_RADIO_SEEK_DOWN			0x54
#define T_RADIO_AS					0x55
#define T_RADIO_PS					0x56
#define T_RADIO_LOC					0x57
#define T_RADIO_TA					0x58
#define T_RADIO_PTY					0x59
#define T_RADIO_AF					0x5A
#define T_RADIO_REG					0x5B
#define T_RADIO_CT					0x5C
#define T_RADIO_EON					0x5D
#define T_RADIO_RDS					0x5E
#define T_RADIO_RDS_TYPE			0x5F
#define T_RADIO_FM_FREQ				0x60
#define T_RADIO_AM_FREQ				0x61
#define T_RADIO_PRESET_SAVE         0x62
#define T_RADIO_PRESET_LOAD         0x63
#define T_RADIO_PTY_SEEK				0x64
#define T_RADIO_STEREO_STATE_CHANGE		0x65

#define T_RADIO_PRESET_PRE				0x66		//选中前一个预存电台
#define T_RADIO_PRESET_NEXT				0x67		//选中下一个预存电台



//#define T_DVDC_STOP					0x70
//#define T_DVDC_PLAY					0x71
//#define T_DVDC_NEXT_CHAPTER			0x72
//#define T_DVDC_PREV_CHAPTER			0x73
//#define T_DVDC_FF					0x74
//#define T_DVDC_FB					0x75
//#define T_DVDC_GOTO_CHAPTER			0x76
//#define T_DVDC_GOTO_DISC			0x77
//#define T_DVDC_REPEAT				0x78
//#define T_DVDC_SHUFFLE				0x79
//#define T_DVDC_SCAN					0x7A
//#define T_DVDC_A_B					0x7B
//#define T_DVDC_ZOOM					0X7C
//#define T_DVDC_TITLE				0x7D
//#define T_DVDC_SUB_T				0x7E


#define T_STOP		0x70
#define T_PLAY		0x71
#define T_NEXT		0x72
#define T_PREV		0x73
#define T_FASTF		0x74
#define T_FASTB		0x75
#define T_REPEAT	0x76
#define T_SHUFFLE	0x77
#define T_SCAN		0x78
#define T_A_B		0x79
#define T_ZOOM		0X7A
#define T_TITLE		0x7B
#define T_SUB_T		0x7C
#define T_PBC		0x7D
#define T_ANGER		0x7E
#define T_STEP		0x7F
#define T_TRACK		0x80
#define T_FULL_SCREEN	0x81
#define T_DVD_MENU		0x82

#define K_EPG			0x90
#define K_TRANSFER		0x91

#define T_BT_AUDIO_MUTE				0XA0
#define T_BLACKOUT					0xA1
#define T_NAVI_MUTE					0xA2
#define T_BEEP						0xA3
#define T_SELECT					0xA4
#define T_SETTING_RESET				0xA5
#define T_SEND_WHEEL_STUDY			0xA6
#define	T_WHEEL_STUDY_OK			0xA7		//方向盘学习确认
#define	T_WHEEL_STUDY_RESET			0xA8		//方向盘学习复位
#define T_SEL_EQ					0xA9		//EQ	调节界面


#define T_LOUDNESS					0xAA
#define T_SUBWOOFER					0xAB





#define T_IR_GOTO					0xB1
#define T_KEY_DISP					0xB2
#define T_REARVIEW					0XB3		//遥控器后视按键
/*High-order word表示Y坐标，
low-order word 表示X坐标	触摸屏坐标
按顺序接收p1 p2 p3 p4
x = p1 + (p2<<8);
y = p3 + (p4<<8);
*/
#define T_CAMERA_RECORD				0XB5		/*行车记录仪按键*/

#define T_CLOSE_DOG					0XB6	
#define T_MOUSE_POS					0XB4	

#define K_USER						0xFF
#define USER_WPARAM_ACCENT			0x10
#define USER_WPARAM_STERE			0x20







#define K_POWER_STATE				0xFB		//关机根据MCU发送的状态进行相应的处理

#define K_BACKLIGHT_STATE			0xFC		//背光根据MCU发送的状态进行相应的处理(0 白天，1 关闭， 2 黑夜)

#define K_SAVE_ALL					0xFD		//掉ACC前发送此按键通知MCU

