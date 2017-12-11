#pragma once
#include "uibase.h"

struct LANGUAGE_TABLE_INFO
{
	TCHAR name[16];
	TCHAR file[32];	// ������չ��.ini�������ļ�����������ʱ��Ҳ�Դ�Ϊ�ؼ���,�ɲ���Ҫ��Сд
	int	  id;
	int	  code_page;
	int   msdk_code_page;	// MSDK�õ�code page�Ƕ�����һ��ö��E_MSDK_CODEPAGE_SET
	int   support;	// 0 ~ ��ʾ��֧�֣� 1 ~ ��ʾ֧��
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


/////////////////  ʱ�� //////////////////////
#define TIMEZONE_MAX_LEN	36
struct TIMEZONE_TABLE_INFO
{
	TCHAR name[TIMEZONE_MAX_LEN];
	int   support;	// 0 ~ ��ʾ��֧�֣� 1 ~ ��ʾ֧��
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
	BOOL gps_set_gain_enable;	// �Ƿ�����GPS��������
	int dac_gpio;
	int subwoofer;		// �ص���ȱʡֵ
	int ui_delay;		// UI��ʱ�೤ʱ����ʧ
	BOOL mute_by_app;	// �Ƿ���APP����MCU mute����,������ƵĻ�,app mute arm����ʱ������MCU mute����
};

////////////////// DISK ////////////////////
#define MAX_DISK_NAME_LEN	15
#define MAX_DISK_GPS_PATH	3
struct CONFIG_DISK 
{
	TCHAR sd[MAX_DISK_NAME_LEN+1];
	TCHAR usb1[MAX_DISK_NAME_LEN+1];
	TCHAR usb2[MAX_DISK_NAME_LEN+1];
	// GPS���֧�ִ������̷�������
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
	CStringArray support_array;	// ����������ʾ֧�ֵĹ�����,������ǿؼ�����
	BOOL is_support(LPCTSTR lpszFunction);
	BOOL is_support(SOURCE_ID sid);
	BOOL remove(LPCTSTR lpszFunctions);
	BOOL add(LPCTSTR lpszFunctions);

	CStringArray mode_array;	// ��ģʱԴ�л���˳��
	SOURCE_ID next_sid(SOURCE_ID sid);	// ��˳���ȡ��һ��ԴID
	SOURCE_ID get_sid_by_sname(LPCTSTR sname);
	LPCTSTR get_sname_by_sid(SOURCE_ID sid);
};

struct CONFIG_UART
{
	int mcu_commport;	// MCUͨ�Ŵ���
	DWORD mcu_baudrate;	// MCUͨ�Ų�����
	int gps_commport;	// GPSͨ�Ŵ���
	DWORD gps_baudrate;	// GPS������
};

struct CONFIG_CANBUS
{
	int canbus_type;
};

// ��������
struct CONFIG_TOUCHKEY
{
	WORD key[10][6];	// ���֧��ʮ������
	int count;			// ��Ч���ݸ���
	int align;			// 0 ��ʾhorzontal(�����ڶ���), 1 ��ʾvertical(����������)
	BOOL enable;
	int get_count();
	int get_align();
	BOOL get_enable();
	RECT* get_rect(int index);	// ��ȡ������,����ǲ��Ϸ�����,���ؿ�ָ��
	WORD get_key(int index);	// ��ȡ��������
	WORD get_key_long(int index);	// ��ȡ����(����)����
};

// ����������
struct CONFIG_MISC 
{
	int fm_area;	// ��������
	int colorful_lights;	// �Ƿ���߲ʵƹ���
	int parking;	// �г���ֹ��Ƶ������
};

#define CONFIG_FILE_NAME	L"config.xml"


namespace config {

	// ��ȡ�����ļ�����ʼ��������Ϣ
	BOOL config_init();

	// ��ȡ֧�ֵ����Ա�
	LANGUAGE_TABLE_INFO* get_language_table();

	// ��������ID�����ر��и������Ϣ,���û�ҵ�������FALSE
	LANGUAGE_TABLE_INFO* get_language_info(int id);

	// ��ȡ���Ա����ܹ��ж�����
	int get_language_table_count();

	// ��ȡ�����ļ���ָ����ȱʡ����ID
	int get_language_default();
	void set_language_default(int LanID);


	/////////////////// ʱ�� ////////////////////
	TIMEZONE_TABLE_INFO* get_timezone_table();

	// ��ȡʱ�������ܹ��ж�����
	int get_timezone_table_count();

	// ��ȡ�����ļ��е�ȱʡʱ��
	// ������ʱ�����е�����ֵ
	int get_timezone_default();

	// �Ƿ�֧��DST
	BOOL get_timezone_dst_enable();


	// aduio������Ϣ
	CONFIG_AUDIO* get_config_audio();

	// disk������Ϣ
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