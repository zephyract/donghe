#pragma once
#include "uibase.h"

struct LANGUAGE_TABLE_INFO
{
	TCHAR name[16];
	TCHAR file[32];	// 不带扩展名.ini，配置文件中引用语言时，也以此为关键字,可不区要大小写
	int	  id;
	int	  code_page;
	int   msdk_code_page;	// MSDK用的code page是定义了一个枚举E_MSDK_CODEPAGE_SET
	int   support;	// 0 ~ 表示不支持， 1 ~ 表示支持
};

#define LAN_SUPPORT		1
#define LAN_UNSUPPORT	0

enum{
	LAN_ENGLISH = 0,
	LAN_CHINESEGB,
	LAN_CHINESEBIG5,
	LAN_FRENCH,
	LAN_RUSSIAN,
	LAN_SPANISH,
	LAN_GERMAN,
	LAN_PORTUGUESS,
	LAN_ARAB,
	LAN_THAI,
	LAN_JAPANESE,
	LAN_TURKIC
};


/////////////////  时区 //////////////////////
#define TIMEZONE_MAX_LEN	36
struct TIMEZONE_TABLE_INFO
{
	TCHAR name[TIMEZONE_MAX_LEN];
	int   support;	// 0 ~ 表示不支持， 1 ~ 表示支持
};
#define TIMEZONE_SUPPORT		1
#define TIMEZONE_UNSUPPORT		0

////////////////// AUDIO ////////////////////
struct CONFIG_AUDIO
{
	BOOL front_dac;
	BOOL rear_dac;
	int init_max_volume;
	BOOL key_beep;
	int media_front_volume;
	int meida_rear_volume;
	int bt_volume;
	int gps_volume;
	int gps_gain;
	BOOL gps_gain_off;
	BOOL gps_set_gain_enable;	// 是否允许GPS设置音量
	int dac_gpio;
	int subwoofer;		// 重低音缺省值
	int ui_delay;		// UI延时多长时间消失
	BOOL mute_by_app;	// 是否由APP控制MCU mute动作,如果控制的话,app mute arm音量时，会让MCU mute功放
};

////////////////// DISK ////////////////////
#define MAX_DISK_NAME_LEN	15
#define MAX_DISK_GPS_PATH	3
struct CONFIG_DISK 
{
	TCHAR sd[MAX_DISK_NAME_LEN+1];
	TCHAR usb1[MAX_DISK_NAME_LEN+1];
	TCHAR usb2[MAX_DISK_NAME_LEN+1];
	// GPS最多支持从三个盘符上搜索
	TCHAR gps[MAX_DISK_GPS_PATH][MAX_DISK_NAME_LEN+1];
	TCHAR vdisk[MAX_PATH];
	TCHAR gps_path[MAX_PATH];
};


//////////////// Video ////////////////////////
struct CONFIG_VIDEO
{
	BYTE standard_brightness;
	BYTE standard_contrast;
	BYTE standard_hue;
	BYTE standard_saturation;

	BYTE colorful_brightness;
	BYTE colorful_contrast;
	BYTE colorful_hue;
	BYTE colorful_saturation;

	BYTE soft_brightness;
	BYTE soft_contrast;
	BYTE soft_hue;
	BYTE soft_saturation;

	BYTE aux_brightness;
	BYTE aux_contrast;
	BYTE aux_hue;
	BYTE aux_saturation;

	BYTE camera_brightness;
	BYTE camera_contrast;
	BYTE camera_hue;
	BYTE camera_saturation;

	BYTE backlight_normal;
	BYTE backlight_night;
	BYTE backlight_min;

	BYTE camera_mirror;
	BYTE parking;

	BYTE h264;
};

// sys info
struct CONFIG_SYSINFO
{
	TCHAR app_ver[32];
	TCHAR manufacture[32];
};

// bluetooth
struct CONFIG_BLUETOOTH
{
	int mic_gain;
	BOOL auto_connect;
	BOOL auto_answer;
};

struct CONFIG_AVIN
{
	BYTE avin1_a;
	BYTE avin1_v;
	BYTE avin2_a;
	BYTE avin2_v;
	BYTE fm_a;
	BYTE camera_v;
	BYTE tv_a;
	BYTE tv_v;
};


struct CONFIG_FUNCTIONS
{
	CStringArray support_array;	// 主界面需显示支持的功能项,保存的是控件名称
	BOOL is_support(LPCTSTR lpszFunction);
	BOOL is_support(SOURCE_ID sid);
	BOOL remove(LPCTSTR lpszFunctions);
	BOOL add(LPCTSTR lpszFunctions);

	CStringArray mode_array;	// 切模时源切换的顺序
	SOURCE_ID next_sid(SOURCE_ID sid);	// 按顺序获取下一个源ID
	SOURCE_ID get_sid_by_sname(LPCTSTR sname);
	LPCTSTR get_sname_by_sid(SOURCE_ID sid);
};

struct CONFIG_UART
{
	int mcu_commport;	// MCU通信串口
	DWORD mcu_baudrate;	// MCU通信波特率
	int gps_commport;	// GPS通信串口
	DWORD gps_baudrate;	// GPS波特率
};

struct CONFIG_CANBUS
{
	int canbus_type;
};

// 触摸按键
struct CONFIG_TOUCHKEY
{
	WORD key[10][6];	// 最多支持十个按键
	int count;			// 有效数据个数
	int align;			// 0 表示horzontal(按键在二边), 1 表示vertical(按键在上下)
	BOOL enable;
	int get_count();
	int get_align();
	BOOL get_enable();
	RECT* get_rect(int index);	// 获取矩形区,如果是不合法索引,返回空指针
	WORD get_key(int index);	// 获取按键编码
	WORD get_key_long(int index);	// 获取按键(长按)编码
};

// 其它配置项
struct CONFIG_MISC 
{
	int fm_area;	// 收音区域
	int colorful_lights;	// 是否带七彩灯功能
	int parking;	// 行车禁止视频设置项
};

#define CONFIG_FILE_NAME	L"config.xml"


namespace config {

	// 读取配置文件，初始化配置信息
	BOOL config_init();

	// 获取支持的语言表
	LANGUAGE_TABLE_INFO* get_language_table();

	// 根据语言ID，返回表中该项的信息,如果没找到，返回FALSE
	LANGUAGE_TABLE_INFO* get_language_info(int id);

	// 获取语言表中总共有多少项
	int get_language_table_count();

	// 获取配置文件中指定的缺省语言ID
	int get_language_default();
	void set_language_default(int LanID);


	/////////////////// 时区 ////////////////////
	TIMEZONE_TABLE_INFO* get_timezone_table();

	// 获取时区表中总共有多少项
	int get_timezone_table_count();

	// 获取配置文件中的缺省时区
	// 返回是时区表中的索引值
	int get_timezone_default();

	// 是否支持DST
	BOOL get_timezone_dst_enable();


	// aduio配置信息
	CONFIG_AUDIO* get_config_audio();

	// disk配置信息
	CONFIG_DISK* get_config_disk();

	// video
	CONFIG_VIDEO* get_config_video();

	//sysinfo
	CONFIG_SYSINFO* get_config_sysinfo();

	// bluetooth
	CONFIG_BLUETOOTH* get_config_bluetooth();

	// avin
	CONFIG_AVIN* get_config_avin();

	// functions
	CONFIG_FUNCTIONS* get_config_functions();

	// uart
	CONFIG_UART* get_config_uart();

	// canbus
	CONFIG_CANBUS* get_config_canbus();

	// touchkey
	CONFIG_TOUCHKEY* get_config_touchkey();

	// misc
	CONFIG_MISC* get_config_misc();
}