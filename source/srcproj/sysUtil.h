#pragma once

#include "uibase.h"

#define NS_SYSUTIL_BEGIN	namespace sysutil {
#define NS_SYSUTIL_END	}


NS_SYSUTIL_BEGIN
// 返回NSS实例
NAVI_SYSTEM_STATUS* nss_get_instance();

// 从磁盘中读取NSS信息,并初始化相关的内容
void nss_init();

// 
void nss_reset();	// 恢复出厂设置

// 将NSS信息保存到磁盘中
BOOL nss_save(BOOL is_acc_off);

// 从磁盘读取NSS信息
BOOL nss_read();

// 加载索引指定的背景,并会释放之前加载的资源
BOOL nss_load_bkground(int index);

// 根据源返回对应的存储项
MSDK_MEMORY* nss_get_msdk_memory(SOURCE_ID sid);

// 根据配置文件及系统状态，对整个系统进行初始化,因为读取的只是数据，使其生效还需作进一步处理
void system_init();


// 读取SD,USB ID号
BOOL get_storageID(const CString &strDiskName, CString &strStorageID);

// 加载一个BMP文件
HBITMAP CreateDibSectionFromDibFile(LPCTSTR szFileName, LPBYTE *pData );

NS_SYSUTIL_END