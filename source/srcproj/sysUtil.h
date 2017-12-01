#pragma once

#include "uibase.h"

#define NS_SYSUTIL_BEGIN	namespace sysutil {
#define NS_SYSUTIL_END	}


NS_SYSUTIL_BEGIN
// ����NSSʵ��
NAVI_SYSTEM_STATUS* nss_get_instance();

// �Ӵ����ж�ȡNSS��Ϣ,����ʼ����ص�����
void nss_init();

// 
void nss_reset();	// �ָ���������

// ��NSS��Ϣ���浽������
BOOL nss_save(BOOL is_acc_off);

// �Ӵ��̶�ȡNSS��Ϣ
BOOL nss_read();

// ��������ָ���ı���,�����ͷ�֮ǰ���ص���Դ
BOOL nss_load_bkground(int index);

// ����Դ���ض�Ӧ�Ĵ洢��
MSDK_MEMORY* nss_get_msdk_memory(SOURCE_ID sid);

// ���������ļ���ϵͳ״̬��������ϵͳ���г�ʼ��,��Ϊ��ȡ��ֻ�����ݣ�ʹ����Ч��������һ������
void system_init();


// ��ȡSD,USB ID��
BOOL get_storageID(const CString &strDiskName, CString &strStorageID);

// ����һ��BMP�ļ�
HBITMAP CreateDibSectionFromDibFile(LPCTSTR szFileName, LPBYTE *pData );

NS_SYSUTIL_END