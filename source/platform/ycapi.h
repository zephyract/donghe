#pragma  once
#include <WinIoCtl.h>

// 要求FastcamerUI退出倒车(os倒车程序)
#define YC_MSG_EXIT_BACKCAR		(WM_APP+0x1200)
#define OS_BACKCAR_WINDOW_NAME	L"FastcamerUI"

// 是否允许夏令时
BOOL YC_SetDST(DWORD bEnable);

// 设置GPS定位后时间是否与系统时间同步.系统默认是同步的,如果APP需要配置此项功能,
// 需要每次开机的时候调用一下此接口
// 参数dwEnable : 0 ~ 不同步  1 ~ 同步
// -------------------------------------------------------------------------------------------------------------
BOOL YC_EnableGpsTimeAutoSet(DWORD dwEnable);

// 开机时获取是否处于倒车状态(APP此时还没与MCU通信)
// 此接口会释放COM通信口，所以APP打开串口前必须调用此接口
// 返回值: 1 ~ 正在倒车   0 ~ 没有倒车
DWORD YC_GetBootBackCarState();

// 此接口只用来查询OS是否处于倒车状态，该接口不会关闭arm2的串口
DWORD YC_GetBootBackCarState2();


void YC_DecodeEnable(DWORD dwIoControlCode, BOOL bEnable, DWORD size);

// -------------------------------------------------------------------------------------------------------------
// Function: YC_GetUUID
// 获取UUID号码
// 参数:
// pData:   存储UUID地址
// DataLen: 16
// -------------------------------------------------------------------------------------------------------------
VOID YC_GetUUID(CHAR *pData, DWORD DataLen);


// -------------------------------------------------------------------------------------------------------------
// Function: YC_UpdateLogo
// 更新Logo文件
// 参数:
// pData:   Logo源文件存储地址
// DataLen: 读取的Logo实际大小
// -------------------------------------------------------------------------------------------------------------
VOID YC_UpdateLogo(CHAR *pData, DWORD DataLen);



// -------------------------------------------------------------------------------------------------------------
// Function: YC_GetBootRev
// 获取Boot版本信息.(格式: E******A******)(******为日编译日期,例如:120523)
// 参数:
// pWData: 宽字符存储地址
// DataLen: >sizeof(L"E******A******")
// -------------------------------------------------------------------------------------------------------------
VOID YC_GetBootRev(WCHAR *pWData,DWORD DataLen);



// -------------------------------------------------------------------------------------------------------------
// Function: YC_GetOSRev
// 获取OS版本信息.(格式:R.YC002.******.******)
// 参数:
// pWData: 宽字符存储地址
// DataLen: >sizeof(L"R.YC002.******.******")
// -------------------------------------------------------------------------------------------------------------
VOID YC_GetOSRev(WCHAR *pWData,DWORD DataLen);


// --------------------------------------------------
// 使能背光,单独控制EN脚,不控制PWM脚.
// 参数: bEnable: 0:disable; 1:enable
// --------------------------------------------------
VOID YC_BackLightEnable(DWORD bEnable);

// -------------------------------------------------------------------------------------------------------------
// Function: YC_SetBackLightLevel
// 设置背光函数.
// 参数: 
// u4Level: 0-100
// -------------------------------------------------------------------------------------------------------------
VOID YC_SetBackLightLevel(DWORD u4Level);

// -------------------------------------------------------------------------------------------------------------
// Function: YC_GetBackLightLevel
// 获取背亮度光函数..
// 返回: 
// u4Level: 0-100; 0xff:失败
// -------------------------------------------------------------------------------------------------------------
DWORD YC_GetBackLightLevel();




// -------------------------------------------------------------------------------------------------------------
// Function: YC_EnableTouch
// 对触摸屏的一些操作
// 参数bType: 
// 0: 禁止触摸屏操作,同时关闭背光 (当点击触摸屏时,重新打开背光,并且激活触摸功能)
// 1: 重新激活触摸屏,并且打开背光
// 2: 禁止触摸屏操作,不对背光操作 (当点击触摸屏时,触摸功能也不会重新恢复),对应参数3
// 3: 重新激活触摸屏,
// -------------------------------------------------------------------------------------------------------------
VOID YC_EnableTouch(DWORD bType);



// -------------------------------------------------------------------------------------------------------------
// Function: YC_SetGpioMode
// 设置GPIO口为Output还是Input
// 参数: 
// PinNum: IO对应的Pin脚
// IOMode: 1:Output,0:Input
// -------------------------------------------------------------------------------------------------------------
VOID YC_SetGpioMode(DWORD PinNum,DWORD IOMode);



// -------------------------------------------------------------------------------------------------------------
// Function: YC_SetGpioData
// 设置GPIO口为高电平或低电平
// 参数:
// PinNum: IO对应的Pin脚
// PinLevel: 1:拉高; 0:拉低
// -------------------------------------------------------------------------------------------------------------
VOID YC_SetGpioData(DWORD PinNum, DWORD PinLevel);



// -------------------------------------------------------------------------------------------------------------
// Function: YC_GetGpioData
// 获取GPIO口的电平状态
// 参数:
// PinNum: IO对应的Pin脚
// 返回值:
// 当IO口做为输入时,返回高低电平状态 1:高电平;0:低电平
// -------------------------------------------------------------------------------------------------------------
DWORD YC_GetGpioData(DWORD PinNum);


// -------------------------------------------------------------------------------------------------------------
// Function: YC_EnableDebugLog
// 禁止使能DEBUG信息
// 参数:
// bEnable: 
// 1: 使能; 0: 禁止
// -------------------------------------------------------------------------------------------------------------
VOID YC_EnableDebugLog(BOOL bEnable);

// -------------------------------------------------------------------------------------------------------------
// Function: YC_GetDskAttach
// 检测5个DSK是否加载还是卸载(包括加载和卸载中...),
// 目的：在开机的时候，APP要快速检测U盘或SD卡有无插入或在加载，可以通过此函数判断
//       因为SD卡或U盘加载时间不等，会造成APP判断有误，此函数可解决此问题
// 返回值:
// 0xffffffff:失败
// SDMMC,StorageCard,ExternCard,USBDisk USBDisk1分别代表4-0位
// -------------------------------------------------------------------------------------------------------------
DWORD YC_GetDiskAttach();

// 是否配置成了USB1.1
BOOL YC_IsUSB11();

// 获取当前每个SD,或USB是否有插入盘
DWORD YC_GetDiskExistStatus();

#define FILE_DEVICE_FSC 					32768
#define FUNCTION_NOTIFY_APP_READY			2048
#define FUNCTION_GET_ARM2_VERSION			2049
#define FUNCTION_NOTIFY_ARM2_STOP			2050
#define FUNCTION_NOTIFY_ARM2_GIFSTOP		2051



#define IOCTL_FSC_NOTIFY_APP_READY          CTL_CODE(FILE_DEVICE_FSC, FUNCTION_NOTIFY_APP_READY, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_FSC_GET_ARM2_VERSION          CTL_CODE(FILE_DEVICE_FSC, FUNCTION_GET_ARM2_VERSION, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_FSC_NOTIFY_ARM2_STOP          CTL_CODE(FILE_DEVICE_FSC, FUNCTION_NOTIFY_ARM2_STOP, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_FSC_NOTIFY_ARM2_GIFSTOP       CTL_CODE(FILE_DEVICE_FSC, FUNCTION_NOTIFY_ARM2_GIFSTOP, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_YGI_TOUCH_ENABLE				CTL_CODE(FILE_DEVICE_UNKNOWN, 0x10C, METHOD_BUFFERED, FILE_ANY_ACCESS)




#define IOCTL_BKL_SET_ENABLE \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_BKL_GET_ENABLE \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_BKL_SET_INTENSITY \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_BKL_GET_INTENSITY \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x3, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define EFUSE_DISCLESS_BIT      (1<<15)


#define IOCTL_HAL_GET_CHIP_VERSION_MTK			CTL_CODE(FILE_DEVICE_UNKNOWN, 2120, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_GET_CHIP_FEATURE_MTK			CTL_CODE(FILE_DEVICE_UNKNOWN, 2121, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_HAL_ENABLE_WMA	 CTL_CODE(FILE_DEVICE_UNKNOWN, 2122, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_ENABLE_3D	 CTL_CODE(FILE_DEVICE_UNKNOWN, 2123, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_ENABLE_WMV	 CTL_CODE(FILE_DEVICE_UNKNOWN, 2124, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_ENABLE_DIVX	 CTL_CODE(FILE_DEVICE_UNKNOWN, 2125, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_ENABLE_AC3	 CTL_CODE(FILE_DEVICE_UNKNOWN, 2126, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_ENABLE_H264	 CTL_CODE(FILE_DEVICE_UNKNOWN, 2127, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_ENABLE_DVDDECODE				CTL_CODE(FILE_DEVICE_UNKNOWN, 2128, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_GET_FEATUREDISCLESS			CTL_CODE(FILE_DEVICE_UNKNOWN, 2129, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_YGI_GET_DSK_ATTACH   			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x113, METHOD_BUFFERED, FILE_ANY_ACCESS)	

#define CHIP_TYPE_UNKNOWN		0
#define CHIP_TYPE_MT3353BDG		1
#define CHIP_TYPE_MT3353CBDG	2
#define CHIP_TYPE_MT3353CBCG	3
#define CHIP_TYPE_MT3360MXDG	4
#define CHIP_TYPE_MT3360KNCG	5
#define CHIP_TYPE_MT3360BICG	6

// YC_GetDiskExistStatus()返回值中相应的位标识哪个盘符是存在的
#define  DSK_USB_DISK_1				0x01
#define  DSK_USB_DISK_2				0x02       //前面U盘
#define  DSK_USB_EXTERN_CARD		0x04
#define  DSK_STORAGE_CARD			0x08
#define	 DSK_SDMMC					0x10
