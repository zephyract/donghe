#pragma  once
#include <WinIoCtl.h>

// Ҫ��FastcamerUI�˳�����(os��������)
#define YC_MSG_EXIT_BACKCAR		(WM_APP+0x1200)
#define OS_BACKCAR_WINDOW_NAME	L"FastcamerUI"

// �Ƿ���������ʱ
BOOL YC_SetDST(DWORD bEnable);

// ����GPS��λ��ʱ���Ƿ���ϵͳʱ��ͬ��.ϵͳĬ����ͬ����,���APP��Ҫ���ô����,
// ��Ҫÿ�ο�����ʱ�����һ�´˽ӿ�
// ����dwEnable : 0 ~ ��ͬ��  1 ~ ͬ��
// -------------------------------------------------------------------------------------------------------------
BOOL YC_EnableGpsTimeAutoSet(DWORD dwEnable);

// ����ʱ��ȡ�Ƿ��ڵ���״̬(APP��ʱ��û��MCUͨ��)
// �˽ӿڻ��ͷ�COMͨ�ſڣ�����APP�򿪴���ǰ������ô˽ӿ�
// ����ֵ: 1 ~ ���ڵ���   0 ~ û�е���
DWORD YC_GetBootBackCarState();

// �˽ӿ�ֻ������ѯOS�Ƿ��ڵ���״̬���ýӿڲ���ر�arm2�Ĵ���
DWORD YC_GetBootBackCarState2();


void YC_DecodeEnable(DWORD dwIoControlCode, BOOL bEnable, DWORD size);

// -------------------------------------------------------------------------------------------------------------
// Function: YC_GetUUID
// ��ȡUUID����
// ����:
// pData:   �洢UUID��ַ
// DataLen: 16
// -------------------------------------------------------------------------------------------------------------
VOID YC_GetUUID(CHAR *pData, DWORD DataLen);


// -------------------------------------------------------------------------------------------------------------
// Function: YC_UpdateLogo
// ����Logo�ļ�
// ����:
// pData:   LogoԴ�ļ��洢��ַ
// DataLen: ��ȡ��Logoʵ�ʴ�С
// -------------------------------------------------------------------------------------------------------------
VOID YC_UpdateLogo(CHAR *pData, DWORD DataLen);



// -------------------------------------------------------------------------------------------------------------
// Function: YC_GetBootRev
// ��ȡBoot�汾��Ϣ.(��ʽ: E******A******)(******Ϊ�ձ�������,����:120523)
// ����:
// pWData: ���ַ��洢��ַ
// DataLen: >sizeof(L"E******A******")
// -------------------------------------------------------------------------------------------------------------
VOID YC_GetBootRev(WCHAR *pWData,DWORD DataLen);



// -------------------------------------------------------------------------------------------------------------
// Function: YC_GetOSRev
// ��ȡOS�汾��Ϣ.(��ʽ:R.YC002.******.******)
// ����:
// pWData: ���ַ��洢��ַ
// DataLen: >sizeof(L"R.YC002.******.******")
// -------------------------------------------------------------------------------------------------------------
VOID YC_GetOSRev(WCHAR *pWData,DWORD DataLen);


// --------------------------------------------------
// ʹ�ܱ���,��������EN��,������PWM��.
// ����: bEnable: 0:disable; 1:enable
// --------------------------------------------------
VOID YC_BackLightEnable(DWORD bEnable);

// -------------------------------------------------------------------------------------------------------------
// Function: YC_SetBackLightLevel
// ���ñ��⺯��.
// ����: 
// u4Level: 0-100
// -------------------------------------------------------------------------------------------------------------
VOID YC_SetBackLightLevel(DWORD u4Level);

// -------------------------------------------------------------------------------------------------------------
// Function: YC_GetBackLightLevel
// ��ȡ�����ȹ⺯��..
// ����: 
// u4Level: 0-100; 0xff:ʧ��
// -------------------------------------------------------------------------------------------------------------
DWORD YC_GetBackLightLevel();




// -------------------------------------------------------------------------------------------------------------
// Function: YC_EnableTouch
// �Դ�������һЩ����
// ����bType: 
// 0: ��ֹ����������,ͬʱ�رձ��� (�����������ʱ,���´򿪱���,���Ҽ��������)
// 1: ���¼������,���Ҵ򿪱���
// 2: ��ֹ����������,���Ա������ (�����������ʱ,��������Ҳ�������»ָ�),��Ӧ����3
// 3: ���¼������,
// -------------------------------------------------------------------------------------------------------------
VOID YC_EnableTouch(DWORD bType);



// -------------------------------------------------------------------------------------------------------------
// Function: YC_SetGpioMode
// ����GPIO��ΪOutput����Input
// ����: 
// PinNum: IO��Ӧ��Pin��
// IOMode: 1:Output,0:Input
// -------------------------------------------------------------------------------------------------------------
VOID YC_SetGpioMode(DWORD PinNum,DWORD IOMode);



// -------------------------------------------------------------------------------------------------------------
// Function: YC_SetGpioData
// ����GPIO��Ϊ�ߵ�ƽ��͵�ƽ
// ����:
// PinNum: IO��Ӧ��Pin��
// PinLevel: 1:����; 0:����
// -------------------------------------------------------------------------------------------------------------
VOID YC_SetGpioData(DWORD PinNum, DWORD PinLevel);



// -------------------------------------------------------------------------------------------------------------
// Function: YC_GetGpioData
// ��ȡGPIO�ڵĵ�ƽ״̬
// ����:
// PinNum: IO��Ӧ��Pin��
// ����ֵ:
// ��IO����Ϊ����ʱ,���ظߵ͵�ƽ״̬ 1:�ߵ�ƽ;0:�͵�ƽ
// -------------------------------------------------------------------------------------------------------------
DWORD YC_GetGpioData(DWORD PinNum);


// -------------------------------------------------------------------------------------------------------------
// Function: YC_EnableDebugLog
// ��ֹʹ��DEBUG��Ϣ
// ����:
// bEnable: 
// 1: ʹ��; 0: ��ֹ
// -------------------------------------------------------------------------------------------------------------
VOID YC_EnableDebugLog(BOOL bEnable);

// -------------------------------------------------------------------------------------------------------------
// Function: YC_GetDskAttach
// ���5��DSK�Ƿ���ػ���ж��(�������غ�ж����...),
// Ŀ�ģ��ڿ�����ʱ��APPҪ���ټ��U�̻�SD�����޲�����ڼ��أ�����ͨ���˺����ж�
//       ��ΪSD����U�̼���ʱ�䲻�ȣ������APP�ж����󣬴˺����ɽ��������
// ����ֵ:
// 0xffffffff:ʧ��
// SDMMC,StorageCard,ExternCard,USBDisk USBDisk1�ֱ����4-0λ
// -------------------------------------------------------------------------------------------------------------
DWORD YC_GetDiskAttach();

// �Ƿ����ó���USB1.1
BOOL YC_IsUSB11();

// ��ȡ��ǰÿ��SD,��USB�Ƿ��в�����
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

// YC_GetDiskExistStatus()����ֵ����Ӧ��λ��ʶ�ĸ��̷��Ǵ��ڵ�
#define  DSK_USB_DISK_1				0x01
#define  DSK_USB_DISK_2				0x02       //ǰ��U��
#define  DSK_USB_EXTERN_CARD		0x04
#define  DSK_STORAGE_CARD			0x08
#define	 DSK_SDMMC					0x10
