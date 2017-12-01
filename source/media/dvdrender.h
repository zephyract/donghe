#pragma once

#include "msdk_defs.h"
#include "DVP_Agent.h"
#include "dvp_cmd.h"
#include "GErr.h"

#include "uibase.h"

#define  DVP_LASTMEMORY_FILE_PATH	L"\\residentflash3\\app\\lastMem_dvp.txt"
//const DWORD DVP_LM_START_ADDRESS = 0x006da300; 
const int DVP_LM_SIZE = 256;
typedef struct {
	GTCHAR DVPLMInfo[DVP_LM_SIZE];
	GUINT8 DVPLMFileTye;	// ֻ�����ݵ���Ч����¼�������ֹ���״̬�����֣���Ƶ��ͼƬ����ȫ��
} DVP_LMFileInfo_T;

// overlay Transparent color
#define COLOR_SPACE_LOW_VALUE	0xF81F
#define COLOR_SPACE_HIGH_VALUE	0xF81F

const int DVD_MAX_EVENT_HANDLER_COUNT = 5;
typedef void (*PFN_DVDRENDER_EVENT_HANDLER)(UINT media_event, UINT param1, UINT param2, DWORD user_data);

enum
{
	SETTINGS_DVD_DOLBY_OFF,
	SETTINGS_DVD_DOLBY_18,
	SETTINGS_DVD_DOLBY_14,
	SETTINGS_DVD_DOLBY_38,
	SETTINGS_DVD_DOLBY_12,
	SETTINGS_DVD_DOLBY_58,
	SETTINGS_DVD_DOLBY_34,
	SETTINGS_DVD_DOLBY_78,
	SETTINGS_DVD_DOLBY_FULL,
};

struct DVD_EVENT_HANDLER_INFO
{
	PFN_DVDRENDER_EVENT_HANDLER pfn;
	DWORD	user_data;
};


class CDvdRender
{
public:
	CDvdRender(void);
	virtual ~CDvdRender(void);

public:
	static CDvdRender* GetInstance();
	BOOL Initialize(HWND hWnd);
	void Uninitialize();

	// interface for rendering
	// prectΪ��ʱ��������ʾ
	void ShowVideo(BOOL bShow, RECT* prect=NULL, DVD_SINK_T sink=DVD_SINK_FRONT);
	void SetSink(DVP_SINK sink_type);
	void Play();
	void Pause();
	void Stop();
	void Next();
	void Previous();
	void CreateFileDatabase(GUINT8 uFilterType);
	void SetCodePage(GUINT32 u4CodePage);
	// ģʽ��ѭ���л������¼�֪ͨ���ѯ��ȥˢ�µ�ǰ��repeate״̬
	void SetPBRepeat();
	void SetPBRandom();
	UCHAR GetRepeatMode();
	UCHAR GetPBCUserMode();

	void GotoRootMenu();
	// 0 ~ for title, 1 ~ for chapter
	BOOL ChooseDigitalNum(GUINT8 uType, GUINT8 uDigitalNum);

	// ��MENU״̬�¿�������������˵�
	void MenuUp();
	void MenuDown();
	void MenuLeft();
	void MenuRight();
	void MenuEnter();

	void SwitchSubtitle();
	void SwitchPBCAngle();
	void GotoTitleMenu();
	void SwitchAudioChannel();
	void ZoomIn();
	void ZoomOut();
	void Rotate();

	void Seek(DWORD dwPosition);	// ��λ��
	BYTE GetState();
	DWORD GetDuration();		// ����ֵ��λ��
	DWORD GetCurPostion();		// ����ֵ��λ��

	// ����
	void Eject();

	// CD track info
	void InitCDTrackInfo(int nTotal);
	void SetCDTrackInfo(int nIndex, int nTrackLen);
	const CArray<int> &GetCDTrackInfo();
	void SetCDCurTrack(int nIndex);
	int GetCDCurTrack();
	int GetCDTotalTrack();

	// cd rip
	static const int RIP_BITRATE = 128;
	int GetCurFileCount();
	// ����¼����Ҫ���ֽ���
	UINT GetRipTrkLen(int nTrack);
	BOOL SetRipPath(LPCTSTR lpszFileName, UINT len);
	// ������ɹ�,���ش�����
	UINT StartRip(int nTrack);
	BOOL StopRip();

	/* source control */
	void LaunchSrc(DVP_SINK sink_type);
	void StopSrc();

	// ��ʱ�򿪻�ر�����ͨ�� for RDS
	void OpenAudio();
	void CloseAudio();

	// �����յ�ע�����Ϣ����øýӿڴ�����Ϣ
	void HandleEvent();
	// ���ע�������Ѿ��������ֵ���򷵻ش���
	BOOL RegisterEventHandler(PFN_DVDRENDER_EVENT_HANDLER pfn, DWORD user_data);
	// ���δ�ҵ�����򷵻ش���
	BOOL UnRegisterEventHandler(PFN_DVDRENDER_EVENT_HANDLER pfn);

	void OnTimer(UINT_PTR nIDEvent);	// ��ʱ����������ת������

	BOOL ReadDVPLM();
	BOOL WriteDVPLM2File();
	// bReset : TRUE ~ ��ʾ���������Ϣ,����г���,�����
	void SaveDVPLM(BOOL bReset = FALSE);
	DVP_LMFileInfo_T* GetDVPLMFileInfo();

protected:
	static GRESULT DvpAgentEventListener(GUINT32 u4Evt, GUINT32 u4Param1, GUINT32 u4Param2, GUINT32 u4CustomData);
	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);

protected:

	HWND m_hwnd;	// �����Ĵ���
	DVP_LMFileInfo_T m_DVPLMFileInfo;
	DVP_SINK m_SinkType;

	// ���pfn����ָ��Ϊ�գ���ʾ����û��ʹ��
	DVD_EVENT_HANDLER_INFO m_eventHandler[DVD_MAX_EVENT_HANDLER_COUNT];

	//  �����CD������¼CDÿ��track��ʱ��, ��¼������Ϊ��Ŀ����, ���ֵΪ0,������û�л�ȡ������
	CArray<int> m_cdTrackInfo;
	int         m_cdCurTrack;	// ��ǰ���ڲ��ŵ�track

	BOOL m_bIsRiping;	// �����ripʱ,�˳�Դʱ��ûstop,����stop

	// ���LaunchSrc()�ӿڴ�û�е��ù�,��ΪDVDû����ȫ��ʼ����,������Ϊ��ʼ����.
	// ���û����ȫ��ʼ����,���ó����ӿ�Eject(),�ᵼ����һ�ζ���������. 
	// �û��п��ܿ�����ֱ���ȳ������ٽ������ᵼ�´���
	BOOL m_bInitialized;	
};
