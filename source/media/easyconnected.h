#pragma once


#define UM_NET_EASYCONN 0x7001		//消息定义
// -- WPARAM
#define PHONE_PLUGIN 0x01			//Android 手机插入
#define PHONE_PLUGOUT 0x02			//Android 手机拔出
#define PROCESS_START 0x03			//程序启动
#define PROCESS_EXIT 0x04			//程序退出
#define IOSPHONE_PLUGIN 0x11		//IOS 手机插入
#define IOSPHONE_PLUGOUT 0x12		//IOS 手机拔出
#define INTERACTIVE_ANDROID_START 0x05 //Android 手机互连开始
#define INTERACTIVE_ANDROID_EXIT 0x06 //Android 手机互连退出
#define ECST_NEED_RESIZE 0x07		//亿连请求外部改变窗口大小
#define ECST_GUI_MIN 0x08			//程序最小化窗口消息
#define ECST_FOCUS_CHANGE 0x09		//亿连程序窗口得到和失去焦点LPARAM:0 失去焦点  1 得到焦点
#define INTERACTIVE_IOS_START 0x45	//IOS 手机互联进双屏
#define INTERACTIVE_IOS_EXIT 0x46	//IOS 手机互联退出双屏
#define ECST_NEED_ACTIVE 0x0A		//需要激活


// 手机蓝牙状态通知外部
//::PostMessage(HWND_BROADCAST ,WM_COPYDATA, ECST_BLUE_TOOTH, (LPARAM)&cpData);
#define ECST_BLUE_TOOTH = 0x0B,
struct C2U_GET_BLUE_TOOTH
{
	char szAdr[32];//手机蓝牙地址
	int nState; //-1 无蓝牙模块，0 蓝牙关，1 蓝牙打开已连上设备，2 蓝牙打开未连上设备
};

// 切换语言
#define UM_EASYCONNET_LANG 0x7006 //消息定义

//车机外部消息定义
#define UM_EASYCONNET_IN 0x7008 
// --子消息放入WPARAM 中，参数放入LPARAM 中
#define VOLUME_UP 0x01 //手机音量加
#define VOLUME_DOWN 0x02 //手机音量减
#define HOME_KEY 0x03 //手机HOME 键
#define MENU_KEY 0x04 //手机MENU 键
#define BACK_KEY 0x05 //手机BACKE 键
#define OUTER_PLAY 0x06 //手机音乐播放
#define OUTER_PAUSE 0x07 /手机音乐暂停
#define OUTER_PLAY_PREV 0x08 //手机音乐上一曲
#define OUTER_PLAY_NEXT 0x09 //手机音乐下一曲
#define INER_QUIT 0xFF01 //外部退出亿连
#define BLUETOOTH_STATUS 0xFF02 //蓝牙状态通知参数0：蓝牙断开1：蓝牙连接
#define DRIVE_MODE 0xFF03 //行车模式通知参数0：停车1：行车
// ios
#define STOP_IOS_MIRROR 0xFF05 //外部停止IOS 互联
#define OUTER_HIDE_VOLUME 0xFF06 //外部关闭声音
#define OUTER_SHOW_VOLUME 0xFF07 //外部开启声音

class CEasyConnected
{
public:
	CEasyConnected(void);
	~CEasyConnected(void);

public:
	static CEasyConnected* GetInstance();
	static void ShowWindow(BOOL bShow);
	static BOOL LoadEasyConnected();
	// 处理EasyConnected程序发来的消息,如果处理了返回TRUE,否则返回FALSE
	BOOL HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	// 向phonelink发消息
	void PostMessage2EasyConnected(UINT message, WPARAM wParam, LPARAM lParam);

	void OnTimer(UINT_PTR nIDEvent);

	void OnConnect(BOOL bConnect);

	/* source control */
	void LaunchSrc();
	void StopSrc();

	// 临时打开或关闭声音通道 for RDS
	void OpenAudio();
	void CloseAudio();

	// 返回值: 0 ~ android,  1 ~ iOS
	int GetDeviceType();
	int m_nDeviceType;

};
