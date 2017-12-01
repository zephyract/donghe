#pragma once
#include "uibase.h"

// ����ԴʱmuteסMCU, 500ms���mute
#define TIMER_ID_SOURCE_LONDING_MUTE	8040


struct SOURCE_INFO
{
	SOURCE_ID	sid;
	DVP_SINK	sink;
};

typedef enum _PLAYBACK_MASK_
{
	PLAYBACK_MASK_BT     = 0x01,		// ͨ��
	PLAYBACK_MASK_CAMERA = (0x01 << 1),	// ����
	PLAYBACK_MASK_RDS = (0x01 << 2), // RDS Alarm
	// �����������
}PLAYBACK_MASK;

class CSourceManager
{

public:
	CSourceManager(void);
	~CSourceManager(void);

public:
	static CSourceManager* CSourceManager::GetInstance();
	void OnTimer(UINT_PTR nIDEvent);	// ��ʱ����������ת������
	// ָ��ǰ��Դ
	void SetFrontSrc(SOURCE_ID sid);
	// ָ������Դ
	void SetRearSrc(SOURCE_ID sid);
	// ָ��ǰ����Ϊͬһ��Դ
	void SetFrontRearSrc(SOURCE_ID sid);
	// ��ǰ���Ų�һ��ʱ������Դ��ʱ����DVP_SINK_REAR,DVP_SINK_FRONTV_REAR���л�
	void SetRearSrcSink(DVP_SINK sink);
	// ͬʱָ��ǰ����Դ
	//void SetFrontRearSrc(SOURCE_ID sid);
	// ���ص�ǰǰ��Դ��Ϣ, ���û��Դ������NULL
	SOURCE_INFO* GetFrontSrc();
	// ���ص�ǰ����Դ��Ϣ, ���û��Դ������NULL
	SOURCE_INFO* GetRearSrc();

	// ����Դ��ID�ţ����û��Դ������INVALID_SOURCE_ID
	SOURCE_ID GetFrontSrcID();
	SOURCE_ID GetRearSrcID();

	// ������һ��Դ��ID�ţ����û��Դ������������ԴID
	SOURCE_ID GetLastFrontSrcID();
	// Ϊ����������Ų���¼��һ��Դ,���ص�ǰ��ԴID,���ǰ��ԴID��֧�ֺ���,�򷵻�INVALID_SOURCE_ID
	SOURCE_ID GetLastRearSrcID();

	void SaveMemory_MSDK();	// ����ϵ�
	void SaveMemory_VDisk();	// ����ϵ�

	// ����Ϊʲôԭ������Ĺ����ָ�
	void HangupPlayback(PLAYBACK_MASK maskEvent);
	void RestorePlayback(PLAYBACK_MASK maskEvent);

	// RDS����ʱ�����ȹر�����Դ��audio��������ɺ��ٴ�,���Ե�������Ľӿ�ʵ��
	void CloseAudio();
	void OpenAudio();

protected:
	// ������������ֱ�Ӷ�ָ����Դ���в���,������ֹͣһ��Դ����ת��Դ��״̬
	void StopSrc(SOURCE_ID sid);
	void LaunchSrc(SOURCE_ID sid, DVP_SINK sink);
	void SetSrcSink(SOURCE_ID sid, DVP_SINK sink);

	// ����sid�Ƿ���һ���Ϸ���ID��
	BOOL IsValidSrc(SOURCE_ID sid);

	// �����˳�MSDKԴ(SD, USB, USB_REAR)
	// ���ļ��ĳ�ʼ�������ϵ㱣�湦�ܷ����������
	void LaunchSrc_MSDK(SOURCE_ID sid, DVP_SINK sink);
	void StopSrc_MSDK(SOURCE_ID sid);

	//
	void LaunchSrc_VDisk(DVP_SINK sink);
	void StopSrc_VDisk();

	// �����VDisk�����ĺ���DVDԴ,�򷵻�TRUE
	BOOL IsDVDSourceByVdisk(SOURCE_ID sid);
protected:
	// ֧��ǰ���ţ��п���ͬʱ���ڶ���Դ
	// ÿ��Դ�Ǹ���ǰ�Ż���ţ���ǰ�����Ƕ�̬�ģ���ʱ����ת��
	SOURCE_INFO m_src1;
	SOURCE_INFO m_src2;

	// ��¼ǰ�ŵ���һ��ԴID,����ǰԴ������ʱ,���Իص���һ��Դ,
	// �����һ��ԴҲ������,�ص�������
	SOURCE_ID	m_srcLastFront;

	// �����������Ŷ��������ԭ��,�����־Ϊ0,��ʾ�����־�����,Ӧ�ü�������
	// ��ͬ��ԭ��(BTͨ��,������)�ò�ͬ��λ����ʾ���μ�PLAYBACK_MASK
	UINT m_playback_hangup_flag;
	UINT m_playback_status;		// ����ʱ,��ǰ�Ĳ���״̬,���������,�ָ�Ϊ��״̬0~��ʾֹͣ����ͣ��,1~��ʾ����
	// ��ȡ��ǰԴ����״̬
	UINT GetPlaybackStatus();
	// ��ǰԴ����
	void PlaybackPlay();
	// ��ǰԴ��ͣ
	void PlaybackPause();
};
