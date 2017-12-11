#pragma once

class CBaseDlg;
class CWceglTexture;
class CWceUiGenericBitmap;

// nAnimMode: ��ʾʱ 0 ~ ��Ҫ����, 1 ~ �������浯��, 2 ~ ���ϼ����浯��, 3 ~ ���������浯��
//			  ����ʱ 0 ~ ��Ҫ����, 1 ~ ��������,	 2 ~ ���ϼ�����      3 ~ ����������
enum ANIM_MODE
{
	ANIM_MODE_NONE,
	ANIM_MODE_MAIN,
	ANIM_MODE_UPLEVEL,
	ANIM_MODE_OTHER
};

// ע��,���ﶨ���ö����Դö�ٿ���û��Ӧ��,��ΪԴ���̷��ǿ�ͨ�������ļ����õ�
// �п��ܰ�SDԴ���storage card, ��������������SDMMC��
// ����ʱҪת����
enum UI_MASS_STORAGE_TYPE
{
	UIMS_INVALID,
	UIMS_SDMMC,
	UIMS_STORAGE_CARD,
	UIMS_SD3,	// ���SD��һ��û��
	UIMS_USB,
	UIMS_USB1,
};

enum UI_MASS_STORAGE_PLUG
{
	UIMS_PLUG_IN,
	UIMS_PLUG_OUT,
};

// 0 ~ ��ʾ���¼���ϵ���ϼ�
// 1 ~ ��ʾ���¼���ϵ���¼�
// 2 ~ ��ʾƽ��
#define ANIM_LEVEL_PARENT	0
#define ANIM_LEVEL_CHILD	1
#define ANIM_LEVEL_BROTHER	2

//ý��Դ����
// enum  SOURCE_ID { SRC_OFF=0X00,SRC_DISC=0X01,SRC_SD=0X02,SRC_USB=0X03,SRC_RADIO=0X04,SRC_TV=0X05,SRC_CMMB=0X06,
// SRC_DVB_T=0X07,SRC_AUX_1=0X08,SRC_AUX_2=0X09,SRC_CDC=0X0A,SRC_BT=0X0B,SRC_CAMERA=0X0C,
// SRC_NAVIGATION=0X0D, SRC_MAIN=0X0F,SRC_IPOD=0X10,SRC_MCUTV=0X11, SRC_ARM_UI=0x12,SRC_IPOD_USB=0x13,
// SOURCE_USB_REAR=0x14, SRC_ANALOGUE_TV=0x16,SRC_DRIVE_RECORD=0x17,SRC_ALL ,SRC_MCU_STANDBY=0XFF, };


enum SOURCE_ID
{
	INVALID_SOURCE_ID = 0,

	SOURCE_DVD = 0x01,
	SOURCE_SD  = 0x02,
	SOURCE_USB = 0x03,
	SOURCE_FM  = 0x04,
	SOURCE_TV     = 0x05,		// ͨ��AVIN����ĵ���


	SOURCE_AVIN1 = 0x08,
	SOURCE_AVIN2 = 0x09,

	SOURCE_BT	= 0x0B,
	SOURCE_IPOD_USB = 0x13,
	SOURCE_USB_REAR = 0x14,
	SOURCE_VDISK    = 0x18,		// ������
	SOURCE_PHONE_LINK = 0x21,	// �ⲿ����, �ֻ�����
	SOURCE_EASY_CONNECTED = 0x22,	// �ⲿ����, �ֻ�����
	SOURCE_MAX_ID   = 0x30,


	SOURCE_MAIN = 0x80,
	SOURCE_SETTING,
	SOURCE_BT_CALL,
	SOURCE_EQ,
	SOURCE_CAMERA,

	SOURCE_CALENDAR,
	SOURCE_CALCULATOR,
	SOURCE_LINK,
	SOURCE_GOBANG,
	SOURCE_OTHELLO,
	SOURCE_BOXMAN,
	SOURCE_CHINACHESS,
	SOURCE_RUSSIAN,
	SOURCE_CLOCK,
	SOURCE_QTELZONE,
	// ecar
	SOURCE_ECAR,
	// 
	SOURCE_CAR_INFO,
	SOURCE_NAVI_WARNING, 
};

enum DLG_ID
{
	INVALID_DLG_ID = 0,
	//
	CMainDlg_ID,
	//
	CSettingDlg_ID,
	CSetGeneralDlg_ID,
	CSetLanguageDlg_ID,
	CSetAboutDlg_ID,
	CSetColorfulLightDlg_ID,
	CSetScreenSaverDlg_ID,
	CSetWallpaperDlg_ID,
	CSetDateTimeDlg_ID,
	CSetTimezoneDlg_ID,
	CGpsViewerDlg_ID,
	CSetGpsDlg_ID,
	CSetGpsPathDlg_ID,
	CSetRadioAreaDlg_ID,
	CSetVideoDlg_ID,
	CSetSWCDlg_ID,
	CSetAudioDlg_ID,
	CSetBluetoothDlg_ID,

	CSetFactoryDlg_ID,
	CSetMcuUpdateDlg_ID,
	CSetUpdateLogoDlg_ID,
	CFacFunctionsDlg_ID,
	CFacLanguageDlg_ID,
	CFacDefaultLanDlg_ID,
	CFacCanbusDlg_ID,
	//
	CRadioDlg_ID,
	CRadioRDSDlg_ID,
	//
	CBluetoothDlg_ID,
	CBTCallDlg_ID,
	//
	CVideoPlayerDlg_ID,
	CMusicPlayerDlg_ID,
	CPhotoPlayerDlg_ID,
	CMMFileListDlg_ID,
	//
	CDVDStartDlg_ID,
	CDVDPlayerDlg_ID,
	CCDPlayerDlg_ID,
	CCDFileListDlg_ID,
	CVCDPlayerDlg_ID,
	CCDGPlayerDlg_ID,
	CDVDFileListDlg_ID,
	CDVDMPSPlayerDlg_ID,
	CDVDAudioPlayerDlg_ID,
	CDVDPhotoPlayerDlg_ID,
	// ipod
	CUIPodFileListDlg_ID,
	CUIPodPlayerDlg_ID,
	//
	CAvin1Dlg_ID,
	CAvin2Dlg_ID,
	CATVDlg_ID,
	CCameraDlg_ID,
	//
	CClockDlg_ID,
	// 
	CCalendarDlg_ID,
	CCalculatorDlg_ID,
	CLinkDlg_ID,
	CGoBangDlg_ID,
	COthelloDlg_ID,
	CBoxManDlg_ID,
	CChinaChessDlg_ID,
	CRussianDlg_ID,
	CQTelzone_ID,
	// ecar
	CEcarDlg_ID,
	// vdisk
	CVDiskListDlg_ID,
	CVDiskPlayerDlg_ID,
	// car inof
	CCarInfoDlg_ID,
	// phonelik
	CPhonelinkStartDlg_ID,
	CPhonelinkWarningDlg_ID,
	//
	CNaviWarningDlg_ID,

	// �ⲿ���򴰿�
	ExternalApp_ID_MIN = 0x100,
	PhoneLink_ID,	// phonelink
	EasyConnected_ID, // 
};

struct DLG_INFO
{
	DLG_ID	dlg_id;		// ÿ���Ի�������Ψһ��ID�ţ������ѯ���ӿڵ���
	wstring dlg_path;	// ���Ӧ����Դ�ļ�·��
	int		ui_level;	// �öԻ���������UI�㣬��ͬ�Ĳ��л�ִ�в�ͬ�Ķ���
	// ���磺maindlgΪ��0�㣬settingdlgΪ��һ�㣬��������Ϊ�ڶ���
	SOURCE_ID    source_id;	// ָ�������ĸ�Դ,��UI���Բ�һ���Ǿ��Ե�Դ��Ҳ������ĳһ���࣬�������е�����
	// ������Ϊ���ڡ����á����Դ��ʵ���Ͼ��ǶԻ�����࣬ÿһ����ܰ���һϵͳ�Ի���
	BOOL	is_created;	// �Ƿ��Ѿ������˸�ʵ��
	BOOL	is_show;	// �Ƿ���ʾ״̬
	BOOL	is_active;	// �Ƿ��Ǹ�����еĻ����,����ͬһ����еĲ�ͬ���ڼ��л�ʱ����ά����״̬��һ�����
	// ֻ��һ�������ǻ�ġ���������������������л��������ʱ��Ӧ���л��������
	CBaseDlg* pdlg;		// ָ��dialogʵ��
	BOOL    to_delete;	// ���ΪTRUE, ����ʹ��ʱ�ᾡ�콫��ɾ��, ��Щ�Ի���ʱɾ���ή���ڴ�ʹ��,��Щ����Ϊһ���Ի����ڲ�ͬԴ���ã�������ڶ��ʵ��
};


const int NSS_UI_BK_INDEX_USER = 32;	// �û��Զ��屳������
const int NSS_UI_BK_INDEX_MAX = 16;		// ��Դ���б����������ֵ
const float NSS_UI_BK_TRANSPARENT = 0.3f;	// ���ô����б�����͸���ȣ����Ե�������

// �û��Զ��屳��
#define USER_SKIN_PATH		L"\\Residentflash3\\App\\UserSkin.32a.conf"

// ϵͳ״̬�����ļ�
#define NSS_PATH			L"\\Residentflash3\\App\\nss.dat"

// ACC״̬�仯״̬���������ΪON�����յ�MCU��ACC OFF֪ͨ��״̬ΪOFF������ʱARM��û����������
#define NSS_ACC_ON			1
#define NSS_ACC_OFF			0

typedef enum 
{
	MSDK_FILE_MUSIC = 0,
	MSDK_FILE_VIDEO,
	MSDK_FILE_PHOTO,
	MSDK_FILE_ALL,
	MSDK_FILE_INVALID = 0xFF,
} MSDK_FILE_TYPE;
const int MSDK_FILE_TYPE_COUNT = 4;


// msdk���Ŷϵ���Ϣ����ǰ���ŵ�״̬��Ϣ
// ����˵����
// ����ı��������������ã��еĻ���Ϊȫ��״̬����
// file_path, pos, repeate, random���˳�Դʱ����CSourceManager::StopSrc_SD()
// type ��MMFileListDlgÿ���л�ʱ�ͱ���
// �����һ���Ż����߼����Ǻ�����(ACC OFFʱҲҪ���棬��δʵ��)
struct MSDK_MEMORY
{
	TCHAR serial_id[64];			// �洢�豸�����к�manufactureID+serialNo
	MSDK_FILE_TYPE	type;			// �û�ѡ�еĹ���ģʽ
	TCHAR file_path[MAX_PATH];		// ��ǰ���ڲ��ŵ��ļ�ȫ·���ļ���
	UINT  pos;						// ��λ��
	int   repeate;					// ��ֵ��¼ѭ��ģʽ0 ~ repeat all  1 ~ repeat folder  2 ~ repeat one
	int   random;					// ��ֵ��¼�Ƿ����0 ~ ����أ� 1 ~ �����
	BOOL  memory_play;				// ��ʶ�Ƿ���Ҫ�ϵ���䲥��,ֻ���״���Դʱ�Ŷϵ㲥�ţ�SEEK���ϵ�λ��
};

// vdisk���Ŷϵ���Ϣ����ǰ���ŵ�״̬��Ϣ
// ����˵����
// ����ı��������������ã��еĻ���Ϊȫ��״̬����
// file_path, pos, repeate, random���˳�Դʱ����CSourceManager::StopSrc_SD()
struct VDISK_MEMORY
{
	int disk_id;					// ֱ�ӱ��浱ǰ���ŵ�disk,��0��ʼ����,����·���������ж�
	TCHAR file_path[MAX_PATH];		// ��ǰ���ڲ��ŵ��ļ�ȫ·���ļ���
	UINT  pos;						// ��λ��
	int   repeate;					// ��ֵ��¼ѭ��ģʽ0 ~ repeat all  1 ~ repeat folder  2 ~ repeat one
	int   random;					// ��ֵ��¼�Ƿ����0 ~ ����أ� 1 ~ �����
	BOOL  memory_play;				// ��ʶ�Ƿ���Ҫ�ϵ���䲥��,ֻ���״���Դʱ�Ŷϵ㲥�ţ�SEEK���ϵ�λ��
};


// ����ϵͳ��ǰ������״̬
enum UI_SYSTEM_STATUS
{
	UISS_NORMAL,			// ��������״̬
	UISS_BACKLIGHT_OFF,		// ����ر�״̬
	UISS_ACC_OFF,			// MCU������ACC OFF���״̬,��״̬һֱά�ֵ�ARM�����MCU�ַ�����ACC ON
	UISS_SLEEP,				// �ٹػ�״̬(��״̬�µ�����BT����SLEEP_AWAKE״̬, ��POWER�����½���NORMAL״̬)
	UISS_SLEEP_AWAKE,		// �ٹػ�����״̬(������BT���������½���SLEEP״̬,û������������������״̬)
};


typedef enum _DVP_SINK_
{
	DVP_SINK_NONE,
	DVP_SINK_FRONT,			// ǰ�Ų���
	DVP_SINK_REAR,			// ���Ų���
	DVP_SINK_FRONT_REAR,	// ǰ���Ų���
	DVP_SINK_FRONTV_REAR	// ǰ���Ų���(�����ں���)
} DVP_SINK;

// ����ϵͳ��ص�ȫ��״̬
struct NAVI_SYSTEM_STATUS
{
	// UI���
	// 0 ~ 16Ϊ��Դ���е�������32Ϊ�û��Զ��壬�������Զ����ͼƬ�ļ�
	int				ui_bk_index;			// �û�ָ���ı�������
	CWceUiGenericBitmap*	pui_bk_texture;			// ������������

	int		ui_lan_id;		// ��ǰ����
	int		ui_timezone_id;	// ��ǰʱ��, ��ʱ�����е�����ֵҵ��ʾ
	BOOL	ui_24hourtime;	// TRUE ~ 24Сʱ�ƣ�FALSE ~ 12Сʱ��
	BOOL	ui_time_autoset;// TRUE ~ ����GPSʱ���Զ�����ʱ�� FALSE ~ ���ֶ�У׼ʱ��
	BOOL	ui_time_need_autoset;	// ����ҪGPSУ��ʱ��ʱ�п���GPSû��λ���ô˱�����¼�����񣬵�GPS��λ��
									// ����Զ�����ΪTRUE���Ҹñ�־ҲΪTRUE,����У��ʱ�䣬�����������Ҫ
	BOOL    ui_time_dst;	// TRUE ~ ʹ������ʱ
	UINT    ui_screen_saver_mode;			// 0 ~ �ر�, 1 ~ ����, 2 ~ clock
	UINT	ui_screen_saver_time;	// �೤ʱ���������,��λ��
	int     ui_clock_mode;			// ʹ���ĸ�ʱ��Ч��0, 1, 2Ŀǰ������Ч��
	int		ui_front_source_id;		// ǰ��ԴID
	int		ui_rear_source_id;		// ����ԴID
	BOOL    ui_dual_source_on;		// ˫�������Ƿ��,��֧��˫��,������֧�ֵ�˫�����ܹر���,���رոñ�־
	BOOL    ui_capture_screen;		// �Ƿ������Ļ��ͼ����,�ñ�־������,ÿ�ο���Ϊ�ر�״̬
	int     ui_rds_pty;				// �û�ѡ����PTY����
	int     ui_colorful_light;		// �߲ʵ����� 0 ~ 7, ȱʡֵΪ0��ʾauto
// ϵͳ��Ϣ
	int   sys_acc_off;				// �����������acc off,�ñ�־����Ϊ1,�����⵽�ñ�־Ϊ0,���ʾ��B+���������,��ʱҪ�ó�������
	int   sys_mcu_port;				// �������к� 1 ~ COM1:  2 ~ COM2: 
	DWORD sys_mcu_baudrate;			// ������
	int	  sys_fastcamera_status;	// ��������״̬, 0 ~ δ������ 1 ~ ����
	int   sys_camera_status;		// ����״̬      0 ~ δ������ 1 ~ ����
	int	  sys_acc_status;
	int   sys_rds_status;			// 0 ~ ��RDS, 1 ~ RDS����
	int   sys_bt_call_status;		// bt��ͨ��״̬,��E_BZMWCALLMANAGER_CALLSTATE_T�����ֵ����һ��
	int   sys_ecar_status;			// 0 ~ ecarδ����  1 ~ ecar���ڹ���״̬
	int   sys_gps_guiding;			// 0 ~ gpsδ��������, 1 ~ gps������������
	BOOL  sys_gps_foreground;		// ��GPS����ǰ̨����ʱ��ΪTRUE
	BOOL  sys_light_check;			// ��Ƽ��
	UI_SYSTEM_STATUS   sys_status;	// ϵͳ��ǰ״̬
	int   sys_calibrate_status;	    // У��״̬, 0 ~ δУ��, 1 ~ ����У��״̬
	int   sys_canbus_type;			// ��_CANBUS_TYPE_����һ��,0��ʾ��can

	// audio���
	// ý������
	int	  audio_media_front_volume;
	BOOL  audio_is_media_front_mute;		// ������ָarm��mute״̬������ָMCU��mute״̬
	int   audio_media_rear_volume;
	BOOL  audio_is_media_rear_mute;
	int   audio_init_media_max_volume;		// ����ʱý����������,����������ʱû�������
	// �������� 
	int   audio_bt_volume;
	BOOL  audio_is_bt_mute;
	// gps ����
	int   audio_gps_volume;
	BOOL  audio_is_gps_mute;
	int   audio_gps_gain;		// ��������,������ʾ��������ʱý�������Ļ�������,ȡֵ0~100��ʾ(0%~100%)
	BOOL  audio_is_gps_gain_off;// �رյ���������,����ǰ̨����ʱ�ر�ý������
	BOOL  audio_is_gps_set_gain;// �Ƿ��������޸�����
	// ����BEEP��
	BOOL  audio_key_beep;		// �����Ƿ���BEEP��
	// EQ
	UINT   audio_eq_type;		// ��ӦEQTYPE_T�е�ֵ����������һ�ָ�������,ֵΪCFG_EQ_SOFT+1 = 7
	UINT   audio_eq_custom[10];	// ���������е�һ��Ƶ��ֵ
	BOOL   audio_eq_subwoofer_enable;	// �򿪻�رմ��ص���
	int    audio_eq_subwoofer;			// subwoofer��ֵ, 0 ~ 40 (TRIM_LEVEL_MAX), ���ﲻ�������ֵȡ�м�ֵ20.
	int    audio_eq_loudness;			// 0 ~ 19
	int    audio_eq_loudness_gain;		// 0��1���������ƿ���(ң���������������ʱ,����������load
	char   audio_eq_balance_fr;			// ǰ��ƽ�� -20 ~ 10
	char   audio_eq_balance_lr;			// ����ƽ�� -20 ~ 10

	// ��Ƶ���
	// UI��DVD,SD����Ƶ����һ�����, AUXһ�����������һ�����
	int video_rear_camera_mirror;	// 0 ~ ������ 1 ~ ����
	BOOL video_parking;				// �Ƿ��г���ֹ��Ƶ

	int video_mode;					// 0 ~ standard, 1 ~ colorful , 2 ~ soft ������ʱ���ò��������������ļ��е�����ֵ
	                                // ĿǰUIû���û�ѡ��ģʽ,��ȱʡ��standard
	int video_brightness;			// ����   ��Ƶ����ȡֵ��Χ0 ~100
	int video_contrast;				// �Աȶ�
	int video_hue;					// ɫ��
	int video_saturation;			// ���Ͷ�

	int video_aux_brightness;		// AUX
	int video_aux_contrast;	
	int video_aux_hue;		
	int video_aux_saturation;
	int video_camera_brightness;	// ����
	int video_camera_contrast;
	int video_camera_hue;	
	int video_camera_saturation;

	int video_backlight_normal;		// ���챳��10 ~ 100
	int video_backlight_night;		// ��ҹģʽ����10 ~ 100

	// SD��Ϣ
	MSDK_MEMORY	sd_memory;

	// USB1
	MSDK_MEMORY	usb1_memory;

	// USB2
	MSDK_MEMORY	usb2_memory;

	// VDisk
	VDISK_MEMORY vdisk_memory;

	// GPS
	TCHAR gps_path[MAX_PATH];
	HANDLE gps_process_handle;
	DWORD  gps_process_id;
	BOOL   gps_autorun;		// �����Ƿ��Զ�����
	int    gps_running_status;		// �ػ�ǰGPS������״̬,0~δ����,1~ǰ̨����,2~��̨����

	// VDisk������
	TCHAR vdisk_path[MAX_PATH];
	BOOL vdisk_is_ripping;		// �Ƿ�����¼�Ƶ�Ƭ
	DVP_SINK vdisk_sink;		// vdiskԴ״̬, ��Ϊvdisk¼��ʱ, ���ǰ����vdisk����,������DVD¼��,���������״̬
	                            // source manager�޷���������Դ״̬�仯ʱ����Դʱ,��Ҫ�ϲ�Ӧ������������״̬

	// BT
	BOOL bt_auto_connect;	// �Զ�����
	BOOL bt_auto_answer;	// �Զ�����

	BOOL is_audio_mute();
	void set_audio_mute(BOOL is_mute);
	BOOL is_source_available(SOURCE_ID sid);

	// ��Ӳ��������Դ�Ƿ����,��Ҫ���SD����USB,OS�л��Ƽ�⿨�Ƿ��������(������֤�̷��Ѿ�����)
	// ���Ӳ���϶�û��⵽,û��Ҫ��ʱ����USB��SD
	BOOL is_source_available_hw(SOURCE_ID sid);

	// ����ָ����ģʽ����video����ֵ
	// mode : 0 ~ ��׼  1 ~ colorful  2 ~ soft
	void reset_video_params(int mode = 0);
};

typedef enum _MCU_BEEP_MODE_
{
	MCU_BEEP_NORMAL,
	MCU_BEEP_ALARM,
} MCU_BEEP_MODE;


// *********  ȫ�ָ�������     *********//

// ���õ�ǰUI������
// lan_id ~ ���Ա�LANGUAGE_TABLE_INFO��ָ��������ID
void ui_set_language(int lan_id);

// ���õ�ǰʱ��
// timezone_id ~ ʱ������ʱ��������
void ui_set_timezone(int timezone_id);

// ��ȡ��ǰʹ�õ���Դ�ļ���
LPCTSTR ui_get_res_file();

// ���ݵ�ǰ��ʱ���ʽ�������ڣ�ʱ���ַ���
// ����ָ��Ļ��������㹻��
void ui_get_datetime_string(OUT LPTSTR pdate, OUT LPTSTR ptime);

// ���ݵ绰�����ʽ��,�����ź��-,�ֻ�����Ҳ�ָ���
void ui_format_tel_number(IN OUT LPTSTR lpszNumber);

// ����У׼
void ui_touch_calibrate();
void ui_exit_touch_calibrate();

// ���ص�ͼ
BOOL ui_load_navigation();
BOOL ui_is_navigation_run();
BOOL ui_terminate_navigation();

// ֪ͨMCU����BEEP��
// ����mode��MCU������ͬ������
// forceΪTRUEʱ,�����������й���BEEP��������
void ui_mcu_beep(int mode = MCU_BEEP_NORMAL);

// ����sid����ȫ�ֱ����й��ڴ�Դ�ļ�����Ϣ
MSDK_MEMORY* ui_msdk_get_memory(SOURCE_ID sid);

// ����sid���ظ�Դ��Ӧ��·��(�����ļ��п�����)
LPCTSTR ui_msdk_get_disk(SOURCE_ID sid);

// ��ǰ��MSDKԴʱ������ȷ�е�source id
SOURCE_ID ui_msdk_get_cursrc();

// �������ü���ǰ�Ĵ��״̬���±���ֵ
void ui_update_backlight();

// ��·����ʽ�������̫����,��·�����м�Ŀ¼��...����,���������̷�������Ŀ¼
void ui_format_file_path(HFONT hfont, const RECT* prc, LPCTSTR lpszPath, OUT LPTSTR pResult);

// �Ƿ��������ͨ��״̬
BOOL ui_is_on_bt_call();

// �õ������Դ����һ��ԴID
SOURCE_ID ui_get_next_sid(SOURCE_ID sid);

void ui_close_backlight();

void ui_play_game_sound(LPCTSTR lpszFile);

void ui_set_foreground_window(HWND hwnd);


// ��������
void ui_enable_touch_key(BOOL enable);

// ���ݵ�ǰDST���趨,����localtime,���DST��,������һ��Сʱ, �����һСʱ
void ui_set_systemtime_DST(__in CONST SYSTEMTIME *lpSystemTime, BOOL bDST);


// ������1ָ����ʱ�䣬������������ָ����ʱ�� (�����������ʱ��,�����������ʱ��)
void ui_time_calculate(IN OUT SYSTEMTIME *lpSystemTime, int hours, int minutes, int seconds);

// ������ʾģʽ
// ��������ʾ������,����ֱ�ӷ���MCU
BYTE* ui_led_get_data();
// ���ò���ģʽ
void ui_led_set_playmode(BOOL random, BOOL repeat);
// ����SD�� USB�� DVD�Ƿ����
// ��ǰԴ������˸Ч��,�����������õ�ֻ����ʾЧ��,ֵΪFALSEʱ����һ������ʵ��Դ������
void ui_led_set_source_available(BYTE* data, BOOL sd, BOOL usb, BOOL dvd, BOOL bt);
// ����LED����ʾ���ַ���,���ȱ�����9���ַ�����,���򲻴���
// �ַ�ֻ���ǿո����֣�Ӣ�Ĵ�д��ĸ���㣬 ð��
// ���ú�ֱ�ӷ���LED����ʾ��
// isTempData : ��������BTͨ������Ϣ��ʾ����Ϊ����ʱ����,��ʾ��ɺ󣬻�ָ�֮ǰ��������ʾ
void ui_led_set_showdata(LPCTSTR toShow, BOOL isTempData=FALSE);
// ��������Ϣת��������ʾ���ַ���
// band: 0 ~ FM1, 1 ~ FM2, 2 ~ FM3, 3 ~ AM1, 4 ~ AM2,
// freq: ת�����ַ������Ƶ��
// preset: ��ǰ��Ԥ��ֵ, ȡֵ0~6�� 0��ʾ��ǰƵ�㲻��Ԥ��ֵ
LPCTSTR ui_led_fm_data(int band, LPCTSTR freq, int preset);

// index: ��ǰ������Ŀ������
// duration: ���ȣ���λ��
LPCTSTR ui_led_media_data(int index, int duration);

// ����ʾ������Ϣʱ,����ʾ������Ϣ
// vol: -1 ~ mute,  0 ~ 40��������
void ui_led_show_volume(int vol);
void ui_led_hide_volume();

// ����ʾBTͨ��״̬ʱ,����ʾ������Ϣ
 void ui_led_show_btcall(UINT nStatus);
 void ui_led_hide_btcall();


