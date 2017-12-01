
#pragma once

#include "Windev.h"
#include<windows.h>



#define FILE_DEVICE_MTKPNPMSSRV 				  32769
#define MTKPNPMSSRV_GET_VOLUMENAME			      2048
#define IOCTL_MTKPNPMSSRV_GET_VOLUMENAME          CTL_CODE(FILE_DEVICE_MTKPNPMSSRV, MTKPNPMSSRV_GET_VOLUMENAME, METHOD_BUFFERED, FILE_ANY_ACCESS)

#ifndef VOLUMENAMESIZE
#define VOLUMENAMESIZE                            64
#endif
typedef struct _MTKPNPMSSRV_DSK_VOLUMENAME {
	TCHAR szVolumeName[VOLUMENAMESIZE];
} MTKPNPMSSRV_DSK_VOLUMENAME, *PMTKPNPMSSRV_DSK_VOLUMENAME;

//This message informs the window registered that an event has occurred due to PnP Mass Storage Mount/Unmount event.
#define     WM_USER_MTKPNPMS_RESERVED              (WM_USER + 3360)
//Syntax
//WM_USER_MTKPNPMS_RESERVED dwMessage = (DWORD) wParam; szDiskVolumeName = (dwHash) lParam;
// please use GetPnpMassStorageVolumeName to get disk volume name as soon as possible after you accessed hash value.

// dwMessage   Hi-Word bits decode information
#define     SDH_SLOT_ZERO                            0x0001UL
#define     SDH_SLOT_ONE                             0x0002UL
#define     SDH_SLOT_TWO                             0x0004UL
#define     HDC_PORT_ZERO                            0x0100UL
#define     HDC_PORT_ONE                             0x0200UL



#define 	FS_DAEMON_UNKNOW                         0  //设备当前为位置状态
#define 	FS_DAEMON_MOUNT_SINCEBABBLE              1  //设备在干扰消失后恢复加载
#define 	FS_DAEMON_DISMOUNT_SINCEBABBLE           2  //设备由于干扰卸载
#define 	FS_DAEMON_MOUNT                          3  //设备在EMI程序启动后正常加载
#define 	FS_DAEMON_DISMOUNT                       4  //设备正常卸载
#define 	FS_DAEMON_MOUNTED                        5  //设备在EMI程序启动前加载
#define 	FS_DAEMON_INVALID                        6  //非法值

#define     MSG_FS_DAEMON_CAUSEMASK                  0x0000FFFF
#define     MSG_FS_DAEMON_SDHSLOT0(dwCause)          (( SDH_SLOT_ZERO << 16) | ((dwCause)&MSG_FS_DAEMON_CAUSEMASK)) //SD0发生插拔事件
#define     MSG_FS_DAEMON_SDHSLOT1(dwCause)          (( SDH_SLOT_ONE  << 16) | ((dwCause)&MSG_FS_DAEMON_CAUSEMASK)) //SD1发生插拔事件
#define     MSG_FS_DAEMON_SDHSLOT2(dwCause)          (( SDH_SLOT_TWO  << 16) | ((dwCause)&MSG_FS_DAEMON_CAUSEMASK)) //SD2发生插拔事件

#define     MSG_FS_DAEMON_HCDPORT0(dwCause)          (( HDC_PORT_ZERO << 16) | ((dwCause)&MSG_FS_DAEMON_CAUSEMASK)) //USB0发生插拔事件
#define     MSG_FS_DAEMON_HCDPORT1(dwCause)          (( HDC_PORT_ONE  << 16) | ((dwCause)&MSG_FS_DAEMON_CAUSEMASK)) //USB1发生插拔事件

#define     GET_MSG_CAUSE(uMsg)                      (    (uMsg) & ( MSG_FS_DAEMON_CAUSEMASK)         ) //获取设备当前状态
#define     GET_WHICH_USB_PORTS(uMsg)                ( (  (uMsg) & (~MSG_FS_DAEMON_CAUSEMASK)  ) >> 16) //判断当前是否是USB设备
#define     GET_WHICH_SDH_SLOTS(uMsg)                ( (  (uMsg) & (~MSG_FS_DAEMON_CAUSEMASK)  ) >> 16) //判断当前是否是SD设备

#define     IS_USBPORT0_PRESENT(uMsg)                ((GET_WHICH_USB_PORTS(uMsg) & HDC_PORT_ZERO) ? TRUE : FALSE) //判断当前设备是否在USB Port0
#define     IS_USBPORT1_PRESENT(uMsg)                ((GET_WHICH_USB_PORTS(uMsg) & HDC_PORT_ONE ) ? TRUE : FALSE) //判断当前设备是否在USB Port1
#define     IS_SDCSLOT0_PRESENT(uMsg)                ((GET_WHICH_SDH_SLOTS(uMsg) & SDH_SLOT_ZERO) ? TRUE : FALSE) //判断当前设备是否在SD0
#define     IS_SDCSLOT1_PRESENT(uMsg)                ((GET_WHICH_SDH_SLOTS(uMsg) & SDH_SLOT_ONE ) ? TRUE : FALSE) //判断当前设备是否在SD1
#define     IS_SDCSLOT2_PRESENT(uMsg)                ((GET_WHICH_SDH_SLOTS(uMsg) & SDH_SLOT_TWO ) ? TRUE : FALSE) //判断当前设备是否在SD2


#define		FILE_DEVICE_MTKPNPMSSRV 				  32769
#define		MTKPNPMSSRV_GET_VOLUMENAME			      2048
#define		IOCTL_MTKPNPMSSRV_GET_VOLUMENAME          CTL_CODE(FILE_DEVICE_MTKPNPMSSRV, MTKPNPMSSRV_GET_VOLUMENAME, METHOD_BUFFERED, FILE_ANY_ACCESS)




DWORD		GetPnpMassStorageVolumeName(DWORD dwHash,PMTKPNPMSSRV_DSK_VOLUMENAME prDskVolumeName);
