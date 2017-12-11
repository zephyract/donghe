#pragma once

class CBaseDlg;
class CWceglTexture;
class CWceUiGenericBitmap;

// nAnimMode: 显示时 0 ~ 不要动画, 1 ~ 从主界面弹出, 2 ~ 从上级界面弹出, 3 ~ 从其它界面弹出
//			  隐藏时 0 ~ 不要动画, 1 ~ 回主界面,	 2 ~ 回上级界面      3 ~ 回其它界面
enum ANIM_MODE
{
	ANIM_MODE_NONE,
	ANIM_MODE_MAIN,
	ANIM_MODE_UPLEVEL,
	ANIM_MODE_OTHER
};

// 注意,这里定义的枚举与源枚举可能没对应上,因为源的盘符是可通过配置文件配置的
// 有可能把SD源配成storage card, 导航卡配置在了SDMMC上
// 处理时要转换下
enum UI_MASS_STORAGE_TYPE
{
	UIMS_INVALID,
	UIMS_SDMMC,
	UIMS_STORAGE_CARD,
	UIMS_SD3,	// 这个SD卡一般没用
	UIMS_USB,
	UIMS_USB1,
};

enum UI_MASS_STORAGE_PLUG
{
	UIMS_PLUG_IN,
	UIMS_PLUG_OUT,
};

// 0 ~ 表示上下级关系的上级
// 1 ~ 表示上下级关系的下级
// 2 ~ 表示平级
#define ANIM_LEVEL_PARENT	0
#define ANIM_LEVEL_CHILD	1
#define ANIM_LEVEL_BROTHER	2

//媒体源定义
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
	SOURCE_TV     = 0x05,		// 通过AVIN输入的电视


	SOURCE_AVIN1 = 0x08,
	SOURCE_AVIN2 = 0x09,

	SOURCE_BT	= 0x0B,
	SOURCE_IPOD_USB = 0x13,
	SOURCE_USB_REAR = 0x14,
	SOURCE_VDISK    = 0x18,		// 虚拟多碟
	SOURCE_PHONE_LINK = 0x21,	// 外部程序, 手机互联
	SOURCE_EASY_CONNECTED = 0x22,	// 外部程序, 手机互联
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

	// 外部程序窗口
	ExternalApp_ID_MIN = 0x100,
	PhoneLink_ID,	// phonelink
	EasyConnected_ID, // 
};

struct DLG_INFO
{
	DLG_ID	dlg_id;		// 每个对话框类有唯一的ID号，方便查询，接口调用
	wstring dlg_path;	// 类对应的资源文件路径
	int		ui_level;	// 该对话框所处的UI层，不同的层切换执行不同的动画
	// 例如：maindlg为第0层，settingdlg为第一层，设置子项为第二层
	SOURCE_ID    source_id;	// 指定属于哪个源,对UI而言不一定是绝对的源，也可以是某一个类，比如所有的设置
	// 可以认为属于“设置”这个源。实际上就是对话框分类，每一类可能包含一系统对话框
	BOOL	is_created;	// 是否已经创建了该实例
	BOOL	is_show;	// 是否显示状态
	BOOL	is_active;	// 是否是该类别中的活动窗口,当在同一类别中的不同窗口间切换时，需维护该状态，一个类别
	// 只有一个窗口是活动的。当从主界面或其它界面切换到该类别时，应该切换到活动窗口
	CBaseDlg* pdlg;		// 指向dialog实例
	BOOL    to_delete;	// 如果为TRUE, 则不再使用时会尽快将其删除, 有些对话框即时删除会降低内存使用,有些是因为一个对话框在不同源在用，不想存在多个实例
};


const int NSS_UI_BK_INDEX_USER = 32;	// 用户自定义背景索引
const int NSS_UI_BK_INDEX_MAX = 16;		// 资源包中背景最大索引值
const float NSS_UI_BK_TRANSPARENT = 0.3f;	// 设置窗口中背景的透明度，用以淡化背景

// 用户自定义背景
#define USER_SKIN_PATH		L"\\Residentflash3\\App\\UserSkin.32a.conf"

// 系统状态保存文件
#define NSS_PATH			L"\\Residentflash3\\App\\nss.dat"

// ACC状态变化状态，正常情况为ON，当收到MCU的ACC OFF通知后状态为OFF，但此时ARM并没有真正掉电
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


// msdk播放断点信息及当前播放的状态信息
// 策略说明：
// 这里的变量不单作记忆用，有的还作为全局状态变量
// file_path, pos, repeate, random在退出源时保存CSourceManager::StopSrc_SD()
// type 在MMFileListDlg每次切换时就保存
// 还需进一步优化，逻辑不是很清晰(ACC OFF时也要保存，还未实现)
struct MSDK_MEMORY
{
	TCHAR serial_id[64];			// 存储设备的序列号manufactureID+serialNo
	MSDK_FILE_TYPE	type;			// 用户选中的过滤模式
	TCHAR file_path[MAX_PATH];		// 当前正在播放的文件全路径文件名
	UINT  pos;						// 单位秒
	int   repeate;					// 该值记录循环模式0 ~ repeat all  1 ~ repeat folder  2 ~ repeat one
	int   random;					// 该值记录是否随机0 ~ 随机关， 1 ~ 随机开
	BOOL  memory_play;				// 标识是否需要断点记忆播放,只有首次切源时才断点播放，SEEK到断点位置
};

// vdisk播放断点信息及当前播放的状态信息
// 策略说明：
// 这里的变量不单作记忆用，有的还作为全局状态变量
// file_path, pos, repeate, random在退出源时保存CSourceManager::StopSrc_SD()
struct VDISK_MEMORY
{
	int disk_id;					// 直接保存当前播放的disk,从0开始计数,不从路径来智能判断
	TCHAR file_path[MAX_PATH];		// 当前正在播放的文件全路径文件名
	UINT  pos;						// 单位秒
	int   repeate;					// 该值记录循环模式0 ~ repeat all  1 ~ repeat folder  2 ~ repeat one
	int   random;					// 该值记录是否随机0 ~ 随机关， 1 ~ 随机开
	BOOL  memory_play;				// 标识是否需要断点记忆播放,只有首次切源时才断点播放，SEEK到断点位置
};


// 整个系统当前所处的状态
enum UI_SYSTEM_STATUS
{
	UISS_NORMAL,			// 正常运行状态
	UISS_BACKLIGHT_OFF,		// 背光关闭状态
	UISS_ACC_OFF,			// MCU发送了ACC OFF后的状态,该状态一直维持到ARM掉电或MCU又发送了ACC ON
	UISS_SLEEP,				// 假关机状态(此状态下倒车或BT进入SLEEP_AWAKE状态, 或POWER键重新进入NORMAL状态)
	UISS_SLEEP_AWAKE,		// 假关机后唤醒状态(倒车或BT结束后重新进入SLEEP状态,没有其它条件进入其它状态)
};


typedef enum _DVP_SINK_
{
	DVP_SINK_NONE,
	DVP_SINK_FRONT,			// 前排播放
	DVP_SINK_REAR,			// 后排播放
	DVP_SINK_FRONT_REAR,	// 前后排播放
	DVP_SINK_FRONTV_REAR	// 前后排播放(声音在后排)
} DVP_SINK;

// 整个系统相关的全局状态
struct NAVI_SYSTEM_STATUS
{
	// UI相关
	// 0 ~ 16为资源包中的索引，32为用户自定义，将加载自定义的图片文件
	int				ui_bk_index;			// 用户指定的背景索引
	CWceUiGenericBitmap*	pui_bk_texture;			// 背景纹理索引

	int		ui_lan_id;		// 当前语言
	int		ui_timezone_id;	// 当前时区, 用时区表中的索引值业表示
	BOOL	ui_24hourtime;	// TRUE ~ 24小时制，FALSE ~ 12小时制
	BOOL	ui_time_autoset;// TRUE ~ 根据GPS时间自动设置时间 FALSE ~ 需手动校准时间
	BOOL	ui_time_need_autoset;	// 当需要GPS校正时间时有可能GPS没定位，用此变量记录此任务，当GPS定位后
									// 如果自动设置为TRUE而且该标志也为TRUE,则需校正时间，其它情况不需要
	BOOL    ui_time_dst;	// TRUE ~ 使用夏令时
	UINT    ui_screen_saver_mode;			// 0 ~ 关闭, 1 ~ 关屏, 2 ~ clock
	UINT	ui_screen_saver_time;	// 多长时间进入屏保,单位秒
	int     ui_clock_mode;			// 使用哪个时钟效果0, 1, 2目前有三种效果
	int		ui_front_source_id;		// 前排源ID
	int		ui_rear_source_id;		// 后排源ID
	BOOL    ui_dual_source_on;		// 双工功能是否打开,不支持双工,或者虽支持但双工功能关闭了,均关闭该标志
	BOOL    ui_capture_screen;		// 是否打开了屏幕截图功能,该标志不记忆,每次开机为关闭状态
	int     ui_rds_pty;				// 用户选定的PTY类型
	int     ui_colorful_light;		// 七彩灯类型 0 ~ 7, 缺省值为0表示auto
// 系统信息
	int   sys_acc_off;				// 如果是正常的acc off,该标志将置为1,如果检测到该标志为0,则表示掉B+或其它情况,这时要用出厂设置
	int   sys_mcu_port;				// 串口序列号 1 ~ COM1:  2 ~ COM2: 
	DWORD sys_mcu_baudrate;			// 波特率
	int	  sys_fastcamera_status;	// 开机倒车状态, 0 ~ 未倒车， 1 ~ 倒车
	int   sys_camera_status;		// 倒车状态      0 ~ 未倒车， 1 ~ 倒车
	int	  sys_acc_status;
	int   sys_rds_status;			// 0 ~ 无RDS, 1 ~ RDS报警
	int   sys_bt_call_status;		// bt的通话状态,与E_BZMWCALLMANAGER_CALLSTATE_T定义的值保持一致
	int   sys_ecar_status;			// 0 ~ ecar未激活  1 ~ ecar处于工作状态
	int   sys_gps_guiding;			// 0 ~ gps未语音播报, 1 ~ gps正在语音播报
	BOOL  sys_gps_foreground;		// 当GPS正在前台运行时置为TRUE
	BOOL  sys_light_check;			// 大灯检测
	UI_SYSTEM_STATUS   sys_status;	// 系统当前状态
	int   sys_calibrate_status;	    // 校屏状态, 0 ~ 未校屏, 1 ~ 处于校屏状态
	int   sys_canbus_type;			// 与_CANBUS_TYPE_定义一致,0表示无can

	// audio相关
	// 媒体音量
	int	  audio_media_front_volume;
	BOOL  audio_is_media_front_mute;		// 这里是指arm的mute状态，不是指MCU的mute状态
	int   audio_media_rear_volume;
	BOOL  audio_is_media_rear_mute;
	int   audio_init_media_max_volume;		// 开机时媒体的最大音量,其它音量暂时没这个属性
	// 蓝牙音量 
	int   audio_bt_volume;
	BOOL  audio_is_bt_mute;
	// gps 音量
	int   audio_gps_volume;
	BOOL  audio_is_gps_mute;
	int   audio_gps_gain;		// 导航增益,用来表示导航发声时媒体音量的混音增益,取值0~100表示(0%~100%)
	BOOL  audio_is_gps_gain_off;// 关闭导航背景音,导航前台发声时关闭媒体音量
	BOOL  audio_is_gps_set_gain;// 是否允许导航修改音量
	// 按键BEEP声
	BOOL  audio_key_beep;		// 按键是否有BEEP声
	// EQ
	UINT   audio_eq_type;		// 对应EQTYPE_T中的值，但增加了一种个性设置,值为CFG_EQ_SOFT+1 = 7
	UINT   audio_eq_custom[10];	// 个性设置中的一组频点值
	BOOL   audio_eq_subwoofer_enable;	// 打开或关闭打开重低音
	int    audio_eq_subwoofer;			// subwoofer的值, 0 ~ 40 (TRIM_LEVEL_MAX), 这里不能设这个值取中间值20.
	int    audio_eq_loudness;			// 0 ~ 19
	int    audio_eq_loudness_gain;		// 0或1，用来控制开关(遥控器上有这个按键时,可用来开关load
	char   audio_eq_balance_fr;			// 前后平衡 -20 ~ 10
	char   audio_eq_balance_lr;			// 左右平衡 -20 ~ 10

	// 视频相关
	// UI与DVD,SD的视频共用一组参数, AUX一组参数，倒车一组参数
	int video_rear_camera_mirror;	// 0 ~ 不镜像， 1 ~ 镜像
	BOOL video_parking;				// 是否行车禁止视频

	int video_mode;					// 0 ~ standard, 1 ~ colorful , 2 ~ soft 当重置时，该参数决定用配置文件中的哪组值
	                                // 目前UI没让用户选择模式,用缺省的standard
	int video_brightness;			// 亮度   视频参数取值范围0 ~100
	int video_contrast;				// 对比度
	int video_hue;					// 色度
	int video_saturation;			// 饱和度

	int video_aux_brightness;		// AUX
	int video_aux_contrast;	
	int video_aux_hue;		
	int video_aux_saturation;
	int video_camera_brightness;	// 倒车
	int video_camera_contrast;
	int video_camera_hue;	
	int video_camera_saturation;

	int video_backlight_normal;		// 白天背光10 ~ 100
	int video_backlight_night;		// 黑夜模式背光10 ~ 100

	// SD信息
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
	BOOL   gps_autorun;		// 开机是否自动运行
	int    gps_running_status;		// 关机前GPS的运行状态,0~未运行,1~前台运行,2~后台运行

	// VDisk虚拟多碟
	TCHAR vdisk_path[MAX_PATH];
	BOOL vdisk_is_ripping;		// 是否正在录制碟片
	DVP_SINK vdisk_sink;		// vdisk源状态, 因为vdisk录制时, 变成前排是vdisk播放,后排是DVD录制,这种特殊的状态
	                            // source manager无法管理，所以源状态变化时或切源时,需要上层应用来考虑这种状态

	// BT
	BOOL bt_auto_connect;	// 自动连接
	BOOL bt_auto_answer;	// 自动接听

	BOOL is_audio_mute();
	void set_audio_mute(BOOL is_mute);
	BOOL is_source_available(SOURCE_ID sid);

	// 从硬件层面检测源是否存在,主要针对SD卡和USB,OS有机制检测卡是否插在里面(但不保证盘符已经加载)
	// 如果硬件上都没检测到,没必要延时加载USB或SD
	BOOL is_source_available_hw(SOURCE_ID sid);

	// 根据指定的模式重置video参数值
	// mode : 0 ~ 标准  1 ~ colorful  2 ~ soft
	void reset_video_params(int mode = 0);
};

typedef enum _MCU_BEEP_MODE_
{
	MCU_BEEP_NORMAL,
	MCU_BEEP_ALARM,
} MCU_BEEP_MODE;


// *********  全局辅助函数     *********//

// 设置当前UI的语言
// lan_id ~ 语言表LANGUAGE_TABLE_INFO中指定的语言ID
void ui_set_language(int lan_id);

// 设置当前时区
// timezone_id ~ 时区表中时区的索引
void ui_set_timezone(int timezone_id);

// 获取当前使用的资源文件名
LPCTSTR ui_get_res_file();

// 根据当前的时间格式返回日期，时间字符串
// 参数指向的缓冲区需足够大
void ui_get_datetime_string(OUT LPTSTR pdate, OUT LPTSTR ptime);

// 根据电话号码格式化,在区号后加-,手机号码也分隔下
void ui_format_tel_number(IN OUT LPTSTR lpszNumber);

// 触摸校准
void ui_touch_calibrate();
void ui_exit_touch_calibrate();

// 加载地图
BOOL ui_load_navigation();
BOOL ui_is_navigation_run();
BOOL ui_terminate_navigation();

// 通知MCU发出BEEP音
// 设置mode让MCU发出不同的声音
// force为TRUE时,将忽略设置中关于BEEP音的设置
void ui_mcu_beep(int mode = MCU_BEEP_NORMAL);

// 根据sid返回全局变量中关于此源的记忆信息
MSDK_MEMORY* ui_msdk_get_memory(SOURCE_ID sid);

// 根据sid返回该源对应的路径(配置文件中可配置)
LPCTSTR ui_msdk_get_disk(SOURCE_ID sid);

// 当前是MSDK源时，返回确切的source id
SOURCE_ID ui_msdk_get_cursrc();

// 根据设置及当前的大灯状态更新背光值
void ui_update_backlight();

// 将路径格式化，如果太长了,将路径的中间目录用...代替,尽量保留盘符及最后的目录
void ui_format_file_path(HFONT hfont, const RECT* prc, LPCTSTR lpszPath, OUT LPTSTR pResult);

// 是否处于来电或通话状态
BOOL ui_is_on_bt_call();

// 得到定义的源的下一个源ID
SOURCE_ID ui_get_next_sid(SOURCE_ID sid);

void ui_close_backlight();

void ui_play_game_sound(LPCTSTR lpszFile);

void ui_set_foreground_window(HWND hwnd);


// 触摸按键
void ui_enable_touch_key(BOOL enable);

// 根据当前DST的设定,设置localtime,如果DST打开,会增加一个小时, 或减少一小时
void ui_set_systemtime_DST(__in CONST SYSTEMTIME *lpSystemTime, BOOL bDST);


// 将参数1指定的时间，调整其它参数指定的时间 (如果正数增加时间,如果负数减少时间)
void ui_time_calculate(IN OUT SYSTEMTIME *lpSystemTime, int hours, int minutes, int seconds);

// 设置显示模式
// 返回需显示的数据,可以直接发给MCU
BYTE* ui_led_get_data();
// 设置播放模式
void ui_led_set_playmode(BOOL random, BOOL repeat);
// 设置SD， USB， DVD是否存在
// 当前源会有闪烁效果,所以这里设置的只是显示效果,值为FALSE时并不一定是真实的源不存在
void ui_led_set_source_available(BYTE* data, BOOL sd, BOOL usb, BOOL dvd, BOOL bt);
// 设置LED需显示的字符串,长度必须是9个字符长度,否则不处理
// 字符只能是空格，数字，英文大写字母，点， 冒号
// 设置后直接发给LED屏显示了
// isTempData : 把音量，BT通话的信息显示，认为是临时数据,显示完成后，会恢复之前的数据显示
void ui_led_set_showdata(LPCTSTR toShow, BOOL isTempData=FALSE);
// 将收音信息转换成需显示的字符串
// band: 0 ~ FM1, 1 ~ FM2, 2 ~ FM3, 3 ~ AM1, 4 ~ AM2,
// freq: 转换成字符串后的频点
// preset: 当前的预设值, 取值0~6， 0表示当前频点不在预设值
LPCTSTR ui_led_fm_data(int band, LPCTSTR freq, int preset);

// index: 当前播放曲目的索引
// duration: 进度，单位秒
LPCTSTR ui_led_media_data(int index, int duration);

// 当显示音量信息时,不显示其它信息
// vol: -1 ~ mute,  0 ~ 40正常音量
void ui_led_show_volume(int vol);
void ui_led_hide_volume();

// 当显示BT通话状态时,不显示其它信息
 void ui_led_show_btcall(UINT nStatus);
 void ui_led_hide_btcall();


