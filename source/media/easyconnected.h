#pragma once


#define UM_NET_EASYCONN 0x7001		//��Ϣ����
// -- WPARAM
#define PHONE_PLUGIN 0x01			//Android �ֻ�����
#define PHONE_PLUGOUT 0x02			//Android �ֻ��γ�
#define PROCESS_START 0x03			//��������
#define PROCESS_EXIT 0x04			//�����˳�
#define IOSPHONE_PLUGIN 0x11		//IOS �ֻ�����
#define IOSPHONE_PLUGOUT 0x12		//IOS �ֻ��γ�
#define INTERACTIVE_ANDROID_START 0x05 //Android �ֻ�������ʼ
#define INTERACTIVE_ANDROID_EXIT 0x06 //Android �ֻ������˳�
#define ECST_NEED_RESIZE 0x07		//���������ⲿ�ı䴰�ڴ�С
#define ECST_GUI_MIN 0x08			//������С��������Ϣ
#define ECST_FOCUS_CHANGE 0x09		//�������򴰿ڵõ���ʧȥ����LPARAM:0 ʧȥ����  1 �õ�����
#define INTERACTIVE_IOS_START 0x45	//IOS �ֻ�������˫��
#define INTERACTIVE_IOS_EXIT 0x46	//IOS �ֻ������˳�˫��
#define ECST_NEED_ACTIVE 0x0A		//��Ҫ����


// �ֻ�����״̬֪ͨ�ⲿ
//::PostMessage(HWND_BROADCAST ,WM_COPYDATA, ECST_BLUE_TOOTH, (LPARAM)&cpData);
#define ECST_BLUE_TOOTH = 0x0B,
struct C2U_GET_BLUE_TOOTH
{
	char szAdr[32];//�ֻ�������ַ
	int nState; //-1 ������ģ�飬0 �����أ�1 �������������豸��2 ������δ�����豸
};

// �л�����
#define UM_EASYCONNET_LANG 0x7006 //��Ϣ����

//�����ⲿ��Ϣ����
#define UM_EASYCONNET_IN 0x7008 
// --����Ϣ����WPARAM �У���������LPARAM ��
#define VOLUME_UP 0x01 //�ֻ�������
#define VOLUME_DOWN 0x02 //�ֻ�������
#define HOME_KEY 0x03 //�ֻ�HOME ��
#define MENU_KEY 0x04 //�ֻ�MENU ��
#define BACK_KEY 0x05 //�ֻ�BACKE ��
#define OUTER_PLAY 0x06 //�ֻ����ֲ���
#define OUTER_PAUSE 0x07 /�ֻ�������ͣ
#define OUTER_PLAY_PREV 0x08 //�ֻ�������һ��
#define OUTER_PLAY_NEXT 0x09 //�ֻ�������һ��
#define INER_QUIT 0xFF01 //�ⲿ�˳�����
#define BLUETOOTH_STATUS 0xFF02 //����״̬֪ͨ����0�������Ͽ�1����������
#define DRIVE_MODE 0xFF03 //�г�ģʽ֪ͨ����0��ͣ��1���г�
// ios
#define STOP_IOS_MIRROR 0xFF05 //�ⲿֹͣIOS ����
#define OUTER_HIDE_VOLUME 0xFF06 //�ⲿ�ر�����
#define OUTER_SHOW_VOLUME 0xFF07 //�ⲿ��������

class CEasyConnected
{
public:
	CEasyConnected(void);
	~CEasyConnected(void);

public:
	static CEasyConnected* GetInstance();
	static void ShowWindow(BOOL bShow);
	static BOOL LoadEasyConnected();
	// ����EasyConnected����������Ϣ,��������˷���TRUE,���򷵻�FALSE
	BOOL HandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	// ��phonelink����Ϣ
	void PostMessage2EasyConnected(UINT message, WPARAM wParam, LPARAM lParam);

	void OnTimer(UINT_PTR nIDEvent);

	void OnConnect(BOOL bConnect);

	/* source control */
	void LaunchSrc();
	void StopSrc();

	// ��ʱ�򿪻�ر�����ͨ�� for RDS
	void OpenAudio();
	void CloseAudio();

	// ����ֵ: 0 ~ android,  1 ~ iOS
	int GetDeviceType();
	int m_nDeviceType;

};
