// 这里定义与外部的接口

#pragma once

#define UI_MAIN_WINDOW_NAME		L"Appolo_Main_Window"

// LPARAM ~ 标识language ID
// 0 ~ english, 1 ~ chinesegb, 2 ~ chinesebig5
const UINT NAVI_MSG_LAN_CHANGE = ::RegisterWindowMessage(L"NAVI_MSG_LAN_CHANGE");

// LPARAM ~ 标识背景索引
const UINT NAVI_MSG_BK_CHANGE = ::RegisterWindowMessage(L"NAVI_MSG_BK_CHANGE");


// ACC OFF时会广播该消息, 广播消息后MCU会延时一段时间掉电(缺省情况下约为4秒)
const UINT NAVI_MSG_ACC_OFF		= ::RegisterWindowMessage(L"NAVI_MSG_ACC_OFF");

// ACC OFF后，ARM还没掉电时，ACC又上电了，系统会广播该消息通知该事件
// 掉电状态下上电，是不会广播该消息的，这种情况MCU也没办法通知APP，系统都没启动
const UINT NAVI_MSG_ACC_ON	= ::RegisterWindowMessage(L"NAVI_MSG_ACC_ON");

// 收到该消息程序会退出, 单独升级APP时可能会用到该消息
const UINT NAVI_MSG_CLOSE_PROCESS = ::RegisterWindowMessage(L"NAVI_MSG_CLOSE_PROCESS");

// 时间显示格式变化, LPARAM ~ 1 表示24小时制， 0表示12小时制
const UINT NAVI_MSG_24HOURTIME_CHANGE = ::RegisterWindowMessage(L"NAVI_MSG_24HOURTIME_CHANGE");

// GPS定位状态与未定位状态变化时，会触发该消息
// LPARAM : 0 ~ 未定位， 1 ~ 定到位
const UINT NAVI_MSG_GPS_FIXED = ::RegisterWindowMessage(L"NAVI_MSG_GPS_FIXED");

// 开机倒车退出时会广播该消息
// LPARAM : 0 ~ 未检测到开机倒车， 1 ~ 检测到开机倒车，退出时触发该消息
const UINT NAVI_MSG_FASTCAMERA_EXIT = ::RegisterWindowMessage(L"NAVI_MSG_FASTCAMERA_EXIT");

// 收到该消息APP将退出
const UINT NAVI_MSG_APP_EXIT = ::RegisterWindowMessage(L"NAVI_MSG_APP_EXIT");
// 升级工具要升级APP时会先发这个消息出来,收到该消息后程序退出,升级工具好复制文件升级
const UINT NAVI_MSG_APP_UPDATE_START = ::RegisterWindowMessage(L"GPSAPP_UPDATE_START");


/***************************************************************************
*                     内部消息定义 (WM_USER ~ WM_USER+100)                 *
***************************************************************************/
// GPS数据更新后，会触发该消息
#define UI_MSG_GPS_STATUS_UPDATE		(WM_USER + 1)
#define UI_MSG_GPS_FIRMWARE_VERSION		(WM_USER + 2)

// 时区改变了，也会引起时间变化
#define UI_MSG_TIMEZONE_CHANGE			(WM_USER + 3)

// mainwnd收到该消息，尝试用GPS时间设置系统时间，如果GPS没定位，则不设置，GPS定位后再触发设置时间
#define UI_MSG_GPS_SETTIME				(WM_USER + 4)

// EQ类型变化了,发送该消息
// lParam: EQ类型
#define UI_MSG_EQ_CHANGE				(WM_USER + 5)

// 通知AUX窗口,视频参数有变化
#define UI_MSG_AUX_VIDEO_PARAMS_CHANGE	(WM_USER + 6)

// 恢复出厂设置后,广播该消息,有些窗口需刷新UI
#define UI_MSG_SYSTEM_RESET					(WM_USER + 7)

//  当窗口管理模块弹出新的窗口时，会广播该消息
//  LPARAM : High word为之前的dlg id，Low word为要显示的dlg id
#define UI_MSG_DLG_CHANGE					(WM_USER + 8)

// 工厂设置有变化,有些UI或状态可能需要更新
#define UI_MSG_FACTORY_SET_CHANGE			(WM_USER + 9)

/***************************************************************************
*                               MCU              	                       *
***************************************************************************/
// MCU按键或命令消息
// wParam : byte0 为协议中定义按键类别，即按键值key_id, byte1为按键状态
// lParam : 参数
#define UI_MSG_MCU_KEY_COMMAND			(WM_USER + 10)

// 收到MCU的radio数据了
// lParam: 信息的类型(参考协议0xB4) 0 ~ radio status, 1 ~ preset list ...
#define UI_MSG_MCU_RADIO_DATA			(WM_USER + 11)

// MCU的设备状态变化了
// lParam: MCU_STATUS指针,包含倒车,大灯,刹车状态等
#define UI_MSG_MCU_STATUS_CHANGE		(WM_USER + 12)

// MCU报告SWC表
// lParam: MCU_SWC_TABLE指针,
#define UI_MSG_MCU_SWC_TABLE			(WM_USER + 13)

// MCU报告采样结果
// lParam: LOWORD 表示是否保存(对应协议byte0), HIWORD 表示keyID(对应协议byte1)
#define UI_MSG_MCU_SWC_KEY_RESULT		(WM_USER + 14)

// CAN BUS数据有更新
// lParam: 信息类型
#define UI_MSG_MCU_CANBUS_DATA			(WM_USER + 15)

// MCU的系统参数变化了
// lParam: 参数索引,即哪个参数变化了
#define UI_MSG_MCU_SYSPARAMS_CHANGE		(WM_USER + 16)

// 用户按下遥控器数字键会收到这个消息
// lParam: 组合后的数字内容
#define UI_MSG_MCU_DIGITAL_KEY			(WM_USER + 17)


/***************************************************************************
*                               SOURCE相关         	                       *
***************************************************************************/
// 启动一个源时通知该消息
// wParam : DVP_SINK 指定sink类型
// lParam : SOURCE_ID 源id来指定哪个源要停止
#define UI_MSG_LOAD_SOURCE_NOTIFY		(WM_USER + 18)

// 停止一个源时通知该消息
// lParam : SOURCE_ID 源id来指定哪个源要停止
#define UI_MSG_STOP_SOURCE_NOTIFY		(WM_USER + 19)

// 当一个源不在可用(如拔卡,出碟,出错,或卡为空时),该源可能主动要求退出源,用此消息通知主窗口
// 由主窗口去加载上一个可用源
// lParam : SOURCE_ID 源id来指定哪个源要退出
#define UI_MSG_SOURCE_EXIT				(WM_USER + 20)

// DVD进出碟消息通知
// wParam : 0 ~ 有进出碟动作但不需要切源， 1 ~ 有进出碟动作，而且需切源
// lParam : 0 ~ 出碟， 1 ~ 进碟
#define UI_MSG_DVD_EJECT_NOTIFY				(WM_USER + 21)

// 专门发给DVD加载画面的,需执行加载动画
#define UI_MSG_DVD_ANIM_LOADING				(WM_USER + 22)

// 触摸禁止后(消息被HOOK截获了),如果有点击,会向主窗口发送该消息通知
// 参数与HOOK处理程序的参数一样
// 按面板按键也可能触发该消息,但二个参数均为0
#define UI_MSG_TP_CLICK						(WM_USER + 23)

// 只要鼠标被点击了,就会发送该消息
#define UI_MSG_LBUTTON_DOWN				(WM_USER + 24)

// 只要鼠标被点击了,就会发送该消息
#define UI_MSG_LBUTTON_UP				(WM_USER + 25)

/***************************************************************************
*                     MSDK文件播放时，事件通知消息	                       *
***************************************************************************/
#define UI_MSG_MSDK_MEDIAGRAPH_FILE		(WM_USER + 50)
// MSDK AVIN时，事件通知消息
#define UI_MSG_MSDK_MEDIAGRAPH_AVIN		(WM_USER + 51)
// DVD播放时时，事件通知消息
#define UI_MSG_DVP_AGENT				(WM_USER + 52)
// wma
#define UI_MSG_MSDK_MEDIAGRAPH_WMA		(WM_USER + 53)

// 通知有个文件要播放了
#define UI_MSG_MSDK_PLAY_FILE			(WM_USER + 54)

/***************************************************************************
*                                  BT 	                                   *
***************************************************************************/
// 连接状态变化时，会通知窗口
#define UI_MSG_BT_CONNECT_STATUS_CHANGE		(WM_USER + 60)
// 来电事件通知
// lParam: ~ 事件的类型
#define UI_MSG_BT_CALL_EVENT				(WM_USER + 61)

// 同步数据时事件通知
// wParam: ~ 高WORD为需下载记录的总数,低WORD为下载数据的各类E_BT_PBAP_SYNC_PATH_T(电话本，未接来电等)
// lParam: ~ 事件的类型
#define UI_MSG_BT_PBMGR_SYNC_EVENT			(WM_USER + 62)

// 音乐事件通知
// lParam: ~ 事件的类型
#define UI_MSG_BT_MUSIC_EVENT				(WM_USER + 63)

// 本地通话记录变化通知
// lParam: ~ 记录的类型E_BT_PBAP_SYNC_PATH_T, 如果为录音数据为0xFF(RECENTS_TYPE_RECORD)
#define UI_MSG_BT_LOCAL_CALLINFO_CHANGE			(WM_USER + 64)

// 蓝牙电话本搜索条件变化时发送该消息
#define UI_MSG_BT_PB_QUERY_CHANGE			(WM_USER + 65)

/***************************************************************************
*                               USB IPOD	                               *
***************************************************************************/
// ipod认证成功并连接上后或拔出IPOD时,发送该消息
// lParam: 0 ~ 断开连接, 1 ~ 连接成功
#define UI_MSG_USB_IPOD_CONNECT				(WM_USER + 70)

// ipod Playback 状态信息
// lParam ~ 当前播放位置信息
#define UI_MSG_USB_IPOD_PB_POS_CHANGED		(WM_USER + 71)

// lParam ~ 当前播放曲目索引
#define UI_MSG_USB_IPOD_PB_TRACK_CHANGED	(WM_USER + 72)

// lParam ~ MUSB_IPOD_PLAY_ST_EX_E类型,描述当前播放状态(播放,暂停等)
#define UI_MSG_USB_IPOD_PB_STATUS_CHANGED		(WM_USER + 73)


/***************************************************************************
*                               Storage card                               *
***************************************************************************/

// wParam : UI_MASS_STORAGE_TYPE来描述是哪个卡被插拔
// lParam : UI_MASS_STORAGE_PLUG来描述是插入还是拔出
#define UI_MSG_STORAGE_CARD_PLUG				(WM_USER + 80)


/***************************************************************************
*         内部控件命令码定义 (WCEUI_CMD_USER ~ WCEUI_CMD_USER+100)         *
***************************************************************************/

/*
* CPhotoShowLayer 引起当前播放文件变化时，发送该命令
* lParam: 当前播放文件索引
*/
#define UI_CMD_PHOTOSHOW_SELCHANGE		(WCEUI_CMD_USER + 1)

/*
* CPhotoShowLayer 全屏状态切换时，发送该命令
* lParam: 0 ~ 非全屏， 1 ~ 全屏
*/
#define UI_CMD_PHOTOSHOW_FULLSCREEN		(WCEUI_CMD_USER + 2)

/*
* CPhotoShowLayer 播放状态切换时，发送该命令
* lParam: 0 ~ 停止播放， 1 ~ 播放
*/
#define UI_CMD_PHOTOSHOW_PLAY_STATUS	(WCEUI_CMD_USER + 4)

/*
* CBalanceSlider 全屏状态切换时，发送该命令
* lParam: hight word为当前鼠标的状态，low word为当前的位置
*/
#define UI_CMD_BSLIDER		(WCEUI_CMD_USER + 3)

/***************************************************************************
*                           其它消息定义                              *
***************************************************************************/

// 通知导航警告窗口退出
#define UI_MSG_NAVI_WARNGINDLG_EXIT				(WM_USER + 0x800)