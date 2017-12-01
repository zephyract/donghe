#pragma once
#include "uibase.h"
#include "WceUi32Bitmap.h"
#include "WceUi565Bitmap.h"

typedef enum _UI_VOLUME_TYPE_
{
	VOLUME_TYPE_NULL,	// ����ǶԼ���������Ч,�羲��,������ʾTYPE,�������ô�ö��
	VOLUME_TYPE_MEDIA,
	VOLUME_TYPE_GPS,
	VOLUME_TYPE_BT,
	VOLUME_TYPE_TA,
} UI_VOLUME_TYPE;

// ������ΧΪ0 ~ 40
#define UI_MAX_VOLUME_LEVEL	40
extern UINT DVP_VOLUME_MSDK_TABLE[UI_MAX_VOLUME_LEVEL+1];
extern UINT DVP_VOLUME_AUX_FRONT_TABLE[UI_MAX_VOLUME_LEVEL+1];
extern UINT DVP_VOLUME_AUX_REAR_TABLE[UI_MAX_VOLUME_LEVEL+1];
extern BYTE BT_VOLUME_TABLE[UI_MAX_VOLUME_LEVEL+1];
extern WORD WINCE_VOLUME_TABLE[UI_MAX_VOLUME_LEVEL+1];

#define WAVMSG_BT_SET_VOLUME				0x412
#define	WAVMSG_GPS_SET_VOLUME				(WM_USER + 0X15)
#define	WAVMSG_GPS_SET_PROCESS_ID			(WM_USER + 0X13)
#define	WAVMSG_GPS_ENABLE_SET_DEVICE_GAIN	(WM_USER + 0x52)

#define	WAVMSG_GPS_STREAMING_NOTIFY			0x3644	// gps����ʱ֪ͨ����Ϣ, ����ָ֪ͨ�����ڣ����ٶ�ϵͳ����


// Ϊ����POP�����룬���Ը�MCU����mute���������
// ���ԣ�
// ��mute����ʱ,���ö�Ӧ�ı�־,mute��Ϻ�Ӧ�����Ӧ�ı�־
// ��Щ�¼�����Ե�ǰ��mute״̬�����뷢����,���ö�Ӧ��unmute��־��������ɺ�,�����Ӧ�ı�־
// ϵͳ������������־����������ǰMCU��mute״̬��
typedef enum _UNMUTE_MCU_MASK_
{
	UNMUTE_MCU_MASK_BT   = 0x01,		// ͨ��
	UNMUTE_MCU_MASK_NAVI = (0x01 << 1),	// ��������
	UNMUTE_MCU_MASK_RDS  = (0x01 << 2), // RDS Alarm
	// �����������
}UNMUTE_MCU_MASK;

typedef enum _MUTE_MCU_MASK_
{
	MUTE_MCU_MASK_INVALID_SRC   = 0x00,		// ��Դ״̬(�ɱ��ⱳ������)		-- �������ʱ���ܾ���,Ӱ��BEEP��
	MUTE_MCU_MASK_SRC_LOADING	= (0x01 << 1),	// ����Դʱ(�ս�Դʱ���ܻ���pop��,��mute 500ms), Ŀǰֻ���ǰ��Դ
	// �����������
}MUTE_MCU_MASK;


class CVolumeManager
{

public:
	CVolumeManager(void);
	~CVolumeManager(void);

public:
	static CVolumeManager* CVolumeManager::GetInstance();

	static void Init();
	static void SetDAC(BOOL front_dac, BOOL rear_dac);

	// ����ϵͳ�ĵ�ǰ״̬���������������ʵ�ֵ
	static void UpdateVolume();

	// �趨ָ�����͵�����
	// ǰ���Ų���ֻ��ý��������Ч,����ָ������ǰ����������������������ǰ����һ������
	static void SetVolume(UI_VOLUME_TYPE eVolumeType, int nVolume, DVP_SINK eSinkType = DVP_SINK_FRONT_REAR);
	static void SetVolumeMute(UI_VOLUME_TYPE eVolumeType, BOOL bMute, DVP_SINK eSinkType = DVP_SINK_FRONT_REAR);

	// ����DVP����
	static void SetDVPVolume(DVP_SINK type, UINT volume);
	// ����winceϵͳ����
	static void SetWinceVolume(DWORD volume);
	// ��������ͨ������
	static void SetBTVolume(BYTE volume);
	// ����GPS����ֵ 0 ~ ʹGPS����, 0xFFFFFFFF ~ GPS�Ե�ǰϵͳ������������
	static void SetGPSVolumeGain(DWORD gain);	// 0 ~ 0xFFFFFFFF
	// �������ص�GPS����
	static void SetGPSProcessID(DWORD dwProcessID);


	// ������ֹ�����޸�ϵͳ����
	static void EnableGPSSetDeviceGain(BOOL bEnable);

	// ��ֹ����ֵԽ��,���Խ����,����Ϊ�Ϸ�ֵ
	static void ValidateVolumeLevel();

	// ���������Ĳ���ֵ����Ϊ�Ϸ�ֵ
	static void ValidateVolumeLevel(int* pVolume);

protected:
	// ���ݵ�ǰ��Դ״̬,������ȷ��������,�����AUX��ط���AUX�ı�,���򷵻�MSDK��
	static UINT* GetMediaVolumeTable(DVP_SINK eSinkType);

	// �жϸ�Դ�Ƿ���AUX����Դ
	static BOOL IsAuxSourceID(SOURCE_ID sid);

// UI Manager
public:
	// ��ʾ����UI,
	// eVolumeType ~ ��ʾ�ĸ�����ֵ
	// dwTime ~ ��ʾ�೤ʱ��(��λms), 0xFFFFFFFF��ʾһֱ��ʾ
	// nStep ~ ���������˴������仯�˶��٣������Ż�UI��ʾ
	void ShowUI(UI_VOLUME_TYPE eVolumeType, DWORD dwTime, int nStep);
	void HideUI();

	// bSet : TRUE ~ ���ñ�־  FALSE ~ �����־
	void SetMcuUnmuteFlag(UNMUTE_MCU_MASK maskEvent, BOOL bSet);
	void SetMcuMuteFlag(MUTE_MCU_MASK maskEvent, BOOL bSet);
	void UpdateMcuMute();	// ���ݵ�ǰ��״̬,����MCU��mute״̬
protected:
	DWORD m_McuMuteFlag;
	DWORD m_McuUnmuteFlag;
};
