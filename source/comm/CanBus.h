#pragma once

// �ܹ�֧�ֵ�canbus������
enum _CANBUS_TYPE_
{
	CANBUS_TYPE_RAISE_DASAUTO = 1,
	CANBUS_TYPE_RAISE_FOCUS,
	CANBUS_TYPE_RAISE_HONDA,
	CANBUS_TYPE_RAISE_PEUGEOT,
	CANBUS_TYPE_RAISE_CAMRY,
	CANBUS_TYPE_RAISE_HYUNDAI,

};

void OnCanBus_RaiseDasAuto(int nDataLen, BYTE* pdata);
void OnCanBus_RaiseFocus(int nDataLen, BYTE* pdata);
void OnCanBus_RaiseHonda(int nDataLen, BYTE* pdata);
void OnCanBus_RaisePeugeot(int nDataLen, BYTE* pdata);
void OnCanBus_RaiseCamry(int nDataLen, BYTE* pdata);
void OnCanBus_RaiseHyundai(int nDataLen, BYTE* pdata);


class ParseCanBus
{
public:
	ParseCanBus(void){};
	virtual ~ParseCanBus(void){};
public:
    //�յ���Ϣ
	virtual BOOL  is_climatronic_Switch()        = 0;  // �յ�����ָʾ
	virtual BOOL  is_climatronic_AC()           = 0;  // A/Cָʾ
	virtual BOOL  is_climatronic_Cycle()        = 0;  // ����ѭ��ָʾ 0:outer 1:inner
	virtual BOOL  is_climatronic_HighWind()     = 0;  // ����ָʾ
	virtual BOOL  is_climatronic_SmallWind()    = 0;  // С���ָʾ
	virtual BOOL  is_climatronic_Dual()         = 0;  // ˫������
	virtual BOOL  is_climatronic_FDemister()    = 0;  // ǰ����������(����)FDemister
	virtual BOOL  is_climatronic_RDemister()    = 0;  // �󵲲�������(����)RDemister
	virtual BOOL  is_climatronic_Airdir_head()  = 0;  // �����ͷ翪��ָʾ
	virtual BOOL  is_climatronic_Airdir_hands() = 0;  // ƽ���ͷ翪��ָʾ
	virtual BOOL  is_climatronic_Airdir_feet()  = 0;  // �����ͷ翪��ָʾ
	virtual BOOL  is_climatronic_REQ_show()     = 0;  // �յ�������ʾ
	virtual BYTE  get_climatronic_FanSpeed()    = 0;  // ������С 0~7��
	virtual FLOAT get_climatronic_LTemp()       = 0;  // ��ʻλ�ô��¶� 0x00:LO 0x1F:HI 18��~26��
	virtual FLOAT get_climatronic_RTemp()       = 0;  // ����ʻλ�ô��¶�
	virtual BOOL  get_climatronic_AQS()         = 0;  // AQS��ѭ��
	virtual BYTE  get_climatronic_LHotSeat()    = 0;  // ������μ��� 0:����ʾ 1~3: 1~3���¶�
	virtual BYTE  get_climatronic_RHotSeat()    = 0;  // �ұ����μ��� 

	//�״���Ϣ
	virtual BYTE  get_radar_front_L()           = 0;  // ǰ�״��� ��Χ 0x00~0x0A
	virtual BYTE  get_radar_front_L_C()         = 0;  // ǰ�״�����
	virtual BYTE  get_radar_front_R_C()         = 0;  // ǰ�״����� 
	virtual BYTE  get_radar_front_R()           = 0;  // ǰ�״���
	virtual BYTE  get_radar_rear_L()            = 0;  // ���״���
	virtual BYTE  get_radar_rear_L_C()          = 0;  // ���״�����
	virtual BYTE  get_radar_rear_R_C()          = 0;  // ���״����� 
	virtual BYTE  get_radar_rear_R()            = 0;  // ���״���

    //������ת��
	virtual INT   get_swc_angle()               = 0;  //�����̽Ƕ�  < 0 ��ʾ��ת

    //������Ϣ
	virtual BOOL  is_car_info_SeatBelt_off()     = 0;  // ��ȫ��״̬  0:���� 1:δϵ
	virtual BOOL  is_car_info_CleanWater_low()   = 0;  // ��ϴҺ״̬  0:normal 1:low 
	virtual BOOL  is_car_info_Brake_hung()       = 0;  // ��ɲ״̬  0:���� 1:δ����
	virtual BOOL  is_car_info_Trunk_Open()       = 0;  // β���״̬  0:off 1:on
	virtual BOOL  is_car_info_Door_RR()          = 0;  // ������  0:off 1:on
	virtual BOOL  is_car_info_Door_RL()          = 0;  // ������  0:off 1:on 
	virtual BOOL  is_car_info_Door_FR()          = 0;  // ǰ����  0:off 1:on 
	virtual BOOL  is_car_info_Door_FL()          = 0;  // ǰ����  0:off 1:on 
	virtual WORD  get_car_info_EngineSpeed()     = 0;  // ������ת�� RPM
	virtual WORD  get_car_info_InstantaneousVelocity()= 0;  // ˲ʱ�ٶ� km/h
	virtual WORD  get_car_info_BattVtg()         = 0;  // ��ص�ѹ V
	virtual INT   get_car_info_TempOutside()     = 0;  // �����¶� ��
	virtual WORD  get_car_info_Mileage()         = 0;  // �г���� Km
	virtual WORD  get_car_info_RemainL()         = 0;  // ʣ������ L
	virtual BOOL  is_car_info_RemainL_low()      = 0;  // ���������־  0:normal 1:low 
	virtual BOOL  is_car_info_BattVtg_low()      = 0;  // ��ѹ�����־  0:normal 1:low 

	//ͣ������״̬
	virtual BOOL  is_parking_info_Switch()       = 0;  // ����������Ϣ״̬  0:off 1:on 
	virtual BOOL  is_parking_info_Radar_beep()   = 0;  // �״﷢��״̬  0:off 1:on 

	//�����̰�����Ϣ
	virtual BYTE  get_swc_key_Value()             = 0;  // 0x00:û�а������»��ͷ� 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
	virtual BYTE  get_swc_key_State()            = 0;  // ����״̬ 0:�ͷ� 1:���� 2:����������Ч
    
	//����
	virtual WORD  get_car_Speed()               = 0;  // ���� Km/h

	// ��������can�ĳ�ʼ��Ϣ
	virtual void request_can_data(BYTE type, BYTE info_id)             = 0;


};

struct CANBUS_RAISE_DASAUTO : public ParseCanBus
{
	BYTE speed[2];		    // ���� 0x16
	BYTE swc_key[2];	    // ��������Ϣ 0x20
	BYTE climatronic[5];    // �յ���Ϣ 0x21
	BYTE rear_radar[4];     // ���״� 0x22
	BYTE front_radar[4];    // ǰ�״� 0x23
	BYTE basic_info[1];     // ������Ϣ 0x24
	BYTE parking_info[2];   // ͣ������״̬ 0x25
	BYTE swc_angle[2];      // ������ת�� 0x26

	BYTE car_info1[2];      // ������Ϣ 0x41 -- 01
	BYTE car_info2[13];     // ������Ϣ 0x41 -- 02
	BYTE car_info3[2];      // ������Ϣ 0x41 -- 03

	BYTE start_end[1];      // START/END 0x81
	BYTE req_ctrl_info[2];  // �����������Ϣ 0x90
	BYTE source[2];         // SOURCE 0xC0
	BYTE icon[1];           // ICON 0xC1
	BYTE radio_info[4];     // ������Ϣ 0xC2
	BYTE media_play_info[6];// ý�岥����Ϣ 0xC3
	BYTE vol_show_ctrl[1];	// ������ʾ���� 0xC4
	BYTE radar_vol_ctrl[2];	// �״��������� 0xC6

	static CANBUS_RAISE_DASAUTO* get_instance();

    //�յ���Ϣ
	BOOL  is_climatronic_Switch();       // �յ�����ָʾ
	BOOL  is_climatronic_AC();           // A/Cָʾ
	BOOL  is_climatronic_Cycle();        // ����ѭ��ָʾ 0:outer 1:inner
	BOOL  is_climatronic_HighWind();     // ����ָʾ
	BOOL  is_climatronic_SmallWind();    // С���ָʾ
	BOOL  is_climatronic_Dual();         // ˫������
	BOOL  is_climatronic_FDemister();    // ǰ����������(����)FDemister
	BOOL  is_climatronic_RDemister();    // �󵲲�������(����)RDemister
	BOOL  is_climatronic_Airdir_head();  // �����ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_hands(); // ƽ���ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_feet();  // �����ͷ翪��ָʾ
	BOOL  is_climatronic_REQ_show();     // �յ�������ʾ
	BYTE  get_climatronic_FanSpeed();    // ������С 0~7��
	FLOAT get_climatronic_LTemp();       // ��ʻλ�ô��¶� 0x00:LO 0x1F:HI 18��~26��
	FLOAT get_climatronic_RTemp();       // ����ʻλ�ô��¶�
	BOOL  get_climatronic_AQS();         // AQS��ѭ��
	BYTE  get_climatronic_LHotSeat();    // ������μ��� 0:����ʾ 1~3: 1~3���¶�
	BYTE  get_climatronic_RHotSeat();    // �ұ����μ��� 

	//�״���Ϣ
	BYTE  get_radar_front_L();           // ǰ�״��� ��Χ 0x00~0x0A
	BYTE  get_radar_front_L_C();         // ǰ�״�����
	BYTE  get_radar_front_R_C();         // ǰ�״����� 
	BYTE  get_radar_front_R();           // ǰ�״���
	BYTE  get_radar_rear_L();            // ���״���
	BYTE  get_radar_rear_L_C();          // ���״�����
	BYTE  get_radar_rear_R_C();          // ���״����� 
	BYTE  get_radar_rear_R();            // ���״���

    //������ת��
	INT   get_swc_angle();               //�����̽Ƕ�  < 0 ��ʾ��ת

    //������Ϣ
	BOOL  is_car_info_SeatBelt_off();     // ��ȫ��״̬  0:���� 1:δϵ
	BOOL  is_car_info_CleanWater_low();   // ��ϴҺ״̬  0:normal 1:low 
	BOOL  is_car_info_Brake_hung();       // ��ɲ״̬  0:���� 1:δ����
	BOOL  is_car_info_Trunk_Open();       // β���״̬  0:off 1:on
	BOOL  is_car_info_Door_RR();          // ������  0:off 1:on
	BOOL  is_car_info_Door_RL();          // ������  0:off 1:on 
	BOOL  is_car_info_Door_FR();          // ǰ����  0:off 1:on 
	BOOL  is_car_info_Door_FL();          // ǰ����  0:off 1:on 
	WORD  get_car_info_EngineSpeed();     // ������ת�� RPM
	WORD  get_car_info_InstantaneousVelocity();// ˲ʱ�ٶ� km/h
	WORD  get_car_info_BattVtg();         // ��ص�ѹ V
	INT   get_car_info_TempOutside();     // �����¶� ��
	WORD  get_car_info_Mileage();         // �г���� Km
	WORD  get_car_info_RemainL();         // ʣ������ L
	BOOL  is_car_info_RemainL_low();      // ���������־  0:normal 1:low 
	BOOL  is_car_info_BattVtg_low();      // ��ѹ�����־  0:normal 1:low 

	//ͣ������״̬
	BOOL  is_parking_info_Switch();       // ����������Ϣ״̬  0:off 1:on 
	BOOL  is_parking_info_Radar_beep();   // �״﷢��״̬  0:off 1:on 

	//�����̰�����Ϣ
	BYTE  get_swc_key_Value();             // 0x00:û�а������»��ͷ� 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
	BYTE  get_swc_key_State();            // ����״̬ 0:�ͷ� 1:���� 2:����������Ч
    
	//����
	WORD  get_car_Speed();               // ���� Km/h

	// ��������can�ĳ�ʼ��Ϣ
	// ��������:
	// 0x16--������Ϣ, 0x21--�յ���Ϣ, 0x24--������Ϣ, 
	// 0x25--��������״̬��Ϣ, 0x41--������Ϣ(��������Ϣʱ��ָ��info_id)
	void request_can_data(BYTE type, BYTE info_id);
};


//////////////////FOCUS/////////////////////////
struct CANBUS_RAISE_FOCUS 
{
	BYTE backlight[1];		    // ���� 0x14
	BYTE swc_key[2];	    // ��������Ϣ 0x20
	BYTE climatronic[6];    // �յ���Ϣ 0x21
	BYTE rear_radar[4];     // ���״� 0x22
	BYTE front_radar[6];    // ǰ�״� 0x23
	BYTE basic_info[2];     // ������Ϣ 0x24
	BYTE parking_info[2];   // ͣ������״̬ 0x25
	BYTE datetime_info[6];		//����ʱ����Ϣ0x26
	BYTE car_version[10];	//����汾��Ϣ0x30
	BYTE sync_srt_up[14];    // 0x70
	BYTE sync_srt_down[14];    // 0x71
	BYTE sync_srt_short[5];    // 0x72
	BYTE sync_srt_status[5];    // 0x78
	BYTE audio_change[1];       //0x79 Audio�л�����
	BYTE car_langue_set[2];		//0x27
	BYTE sync_version[1];       //0x40
	BYTE sync_V3_menu[8];       //0x50
	BYTE sync_V3_menuitem[256];       //0x51
	BYTE sync_media_tracks_tm[3];       //0x52
	BYTE sync_talk_tm[3];       //0x53
	BYTE key_cmd[1];               // ��������0x01


	BYTE ctrl_cmd[2];  // �����������Ϣ 0xC6
	BYTE start_end[1];      // START/END 0x81
	BYTE car_datetime_set[6];//0x82
	BYTE req_decode_status[1];//0x90

// 	BYTE swc_angle[2];      // ������ת�� 0x26
// 	BYTE car_info[13];      // ������Ϣ 0x41
// 	BYTE source[2];         // SOURCE 0xC0
// 	BYTE icon[1];           // ICON 0xC1
// 	BYTE radio_info[4];     // ������Ϣ 0xC2
// 	BYTE media_play_info[6];// ý�岥����Ϣ 0xC3
// 	BYTE vol_show_ctrl[1];	// ������ʾ���� 0xC4
// 	BYTE radar_vol_ctrl[2];	// �״��������� 0xC6

	static CANBUS_RAISE_FOCUS* get_instance();

	//�յ���Ϣ
	BOOL  is_climatronic_Switch();       // �յ�����ָʾ
	BOOL  is_climatronic_AC();           // A/Cָʾ
	BOOL  is_climatronic_Cycle();        // ����ѭ��ָʾ 0:outer 1:inner
	BOOL  is_climatronic_SmallWind();    // С���ָʾ
	BOOL  is_climatronic_Dual();         // ˫������
	BOOL  is_climatronic_MaxFornt();     // MAX FORNT��ָʾ

// 	BOOL  is_climatronic_FDemister();    // ǰ����������(����)FDemister
// 	BOOL  is_climatronic_RDemister();    // �󵲲�������(����)RDemister
	BOOL  is_climatronic_Airdir_head();  // �����ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_hands(); // ƽ���ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_feet();  // �����ͷ翪��ָʾ
 	BOOL  is_climatronic_REQ_show();     // �յ�������ʾ
	BYTE  get_climatronic_FanSpeed();    // ������С 0~7��
	FLOAT get_climatronic_LTemp();       // ��ʻλ�ô��¶� 0x00:LO 0x1F:HI 18��~26��
	FLOAT get_climatronic_RTemp();       // ����ʻλ�ô��¶�
	BOOL  is_climatronic_AC_max();       // AC_max
	BYTE  get_climatronic_tempunit();// AC_max

// 	BOOL  get_climatronic_AQS();         // AQS��ѭ��
// 	BYTE  get_climatronic_LHotSeat();    // ������μ��� 0:����ʾ 1~3: 1~3���¶�
// 	BYTE  get_climatronic_RHotSeat();    // �ұ����μ��� 

	//�״���Ϣ
	BYTE  get_radar_front_L();           // ǰ�״��� ��Χ 0x00~0x0A
	BYTE  get_radar_front_L_C();         // ǰ�״�����
	BYTE  get_radar_front_R_C();         // ǰ�״����� 
	BYTE  get_radar_front_R();           // ǰ�״���
	BYTE  get_radar_rear_L();            // ���״���
	BYTE  get_radar_rear_L_C();          // ���״�����
	BYTE  get_radar_rear_R_C();          // ���״����� 
	BYTE  get_radar_rear_R();            // ���״���

	//������ת��
// 	INT   get_swc_angle();               //�����̽Ƕ�  < 0 ��ʾ��ת

	//������Ϣ
	BOOL  is_car_info_SeatBelt_off();     // ��ȫ��״̬  0:���� 1:δϵ
// 	BOOL  is_car_info_CleanWater_low();   // ��ϴҺ״̬  0:normal 1:low 
	BOOL  is_car_info_Brake_hung();       // ��ɲ״̬  0:���� 1:δ����
	BOOL  is_car_info_Trunk_Open();       // β���״̬  0:off 1:on
	BOOL  is_car_info_Door_RR();          // ������  0:off 1:on
	BOOL  is_car_info_Door_RL();          // ������  0:off 1:on 
	BOOL  is_car_info_Door_FR();          // ǰ����  0:off 1:on 
	BOOL  is_car_info_Door_FL();          // ǰ����  0:off 1:on 
// 	WORD  get_car_info_EngineSpeed();     // ������ת�� RPM
// 	WORD  get_car_info_InstantaneousVelocity();// ˲ʱ�ٶ� km/h
// 	WORD  get_car_info_BattVtg();         // ��ص�ѹ V
	INT   get_car_info_TempOutside();     // �����¶� ��
// 	WORD  get_car_info_Mileage();         // �г���� Km
// 	WORD  get_car_info_RemainL();         // ʣ������ L
// 	BOOL  is_car_info_RemainL_low();      // ���������־  0:normal 1:low 
// 	BOOL  is_car_info_BattVtg_low();      // ��ѹ�����־  0:normal 1:low 

	//ͣ������״̬
// 	BOOL  is_parking_info_Switch();       // ����������Ϣ״̬  0:off 1:on 
// 	BOOL  is_parking_info_Radar_beep();   // �״﷢��״̬  0:off 1:on 

	//�����̰�����Ϣ
    BYTE  get_swc_key_Value();             // 0x00:û�а������»��ͷ� 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
    BYTE  get_swc_key_State();            // ����״̬ 0:�ͷ� 1:���� 2:����������Ч

	//����
// 	WORD  get_car_Speed();               // ���� Km/h

};


/////////////////////////////HONDA////////////////////////////////
struct CANBUS_RAISE_HONDA
{
	BYTE swc_key[2];	    // ��������Ϣ 0x20
	BYTE climatronic[6];    // �յ���Ϣ 0x21
	BYTE basic_info[1];     // ������Ϣ 0x24
	BYTE version_info[10];   // �汾��Ϣ 0x30
	BYTE car_info[4];      // ������Ϣ 0x32
	BYTE car_Mileage[15];      // �ͺ���� 0x33
	BYTE swc_angle[2];      // ������ת�� 0x29
	BYTE car_set_ack[2];      // ��������״̬���� 0xD0
	BYTE backlight[1];		    //���������Ϣ 0x14
	BYTE rear_radar[4];     // ���״� 0x22
// 	BYTE front_radar[4];    // ǰ�״� 0x23
	BYTE parking_info[2];   // ͣ������״̬ 0x25


	BYTE start_end[1];      // START/END 0x81
	BYTE set_cmd[2];  // �趨���� 0xC6
	BYTE query_cmd[2];  // �趨���� 0x90


	static CANBUS_RAISE_HONDA* get_instance();

	//�յ���Ϣ
	BOOL  is_climatronic_Switch();       // �յ�����ָʾ
	BOOL  is_climatronic_AC();           // A/Cָʾ
	BOOL  is_climatronic_Cycle();        // ����ѭ��ָʾ 0:outer 1:inner
	BOOL  is_climatronic_HighWind();     // ����ָʾ
	BOOL  is_climatronic_SmallWind();    // С���ָʾ
	BOOL  is_climatronic_Dual();         // ˫������
	BOOL  is_climatronic_FDemister();    // ǰ����������(����)FDemister
	BOOL  is_climatronic_RDemister();    // �󵲲�������(����)RDemister
	BOOL  is_climatronic_Airdir_head();  // �����ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_hands(); // ƽ���ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_feet();  // �����ͷ翪��ָʾ
	BOOL  is_climatronic_REQ_show();     // �յ�������ʾ
	BYTE  get_climatronic_FanSpeed();    // ������С 0~7��
	FLOAT get_climatronic_LTemp();       // ��ʻλ�ô��¶� 0x00:LO 0x1F:HI 18��~26��
	FLOAT get_climatronic_RTemp();       // ����ʻλ�ô��¶�
	BOOL  get_climatronic_AQS();         // AQS��ѭ��
	BYTE  get_climatronic_LHotSeat();    // ������μ��� 0:����ʾ 1~3: 1~3���¶�
	BYTE  get_climatronic_RHotSeat();    // �ұ����μ��� 

	//�״���Ϣ
// 	BYTE  get_radar_front_L();           // ǰ�״��� ��Χ 0x00~0x0A
// 	BYTE  get_radar_front_L_C();         // ǰ�״�����
// 	BYTE  get_radar_front_R_C();         // ǰ�״����� 
// 	BYTE  get_radar_front_R();           // ǰ�״���
	BYTE  get_radar_rear_L();            // ���״���
	BYTE  get_radar_rear_L_C();          // ���״�����
	BYTE  get_radar_rear_R_C();          // ���״����� 
	BYTE  get_radar_rear_R();            // ���״���

	//������ת��
	INT   get_swc_angle();               //�����̽Ƕ�  < 0 ��ʾ��ת

	//������Ϣ
	BOOL  is_car_info_SeatBelt_off();     // ��ȫ��״̬  0:���� 1:δϵ
	BOOL  is_car_info_CleanWater_low();   // ��ϴҺ״̬  0:normal 1:low 
	BOOL  is_car_info_Brake_hung();       // ��ɲ״̬  0:���� 1:δ����
	BOOL  is_car_info_Trunk_Open();       // β���״̬  0:off 1:on
	BOOL  is_car_info_Door_RR();          // ������  0:off 1:on
	BOOL  is_car_info_Door_RL();          // ������  0:off 1:on 
	BOOL  is_car_info_Door_FR();          // ǰ����  0:off 1:on 
	BOOL  is_car_info_Door_FL();          // ǰ����  0:off 1:on 
	WORD  get_car_info_EngineSpeed();     // ������ת�� RPM
	WORD  get_car_info_InstantaneousVelocity();// ˲ʱ�ٶ� km/h
	WORD  get_car_info_BattVtg();         // ��ص�ѹ V
// 	INT   get_car_info_TempOutside();     // �����¶� ��
	WORD  get_car_info_Mileage();         // �г���� Km
	WORD  get_car_info_RemainL();         // ʣ������ L
	BOOL  is_car_info_RemainL_low();      // ���������־  0:normal 1:low 
	BOOL  is_car_info_BattVtg_low();      // ��ѹ�����־  0:normal 1:low 

	//ͣ������״̬
	BOOL  is_parking_info_Switch();       // ����������Ϣ״̬  0:off 1:on 
	BOOL  is_parking_info_Radar_beep();   // �״﷢��״̬  0:off 1:on 

	//�����̰�����Ϣ
	BYTE  get_swc_key_Value();             // 0x00:û�а������»��ͷ� 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
	BYTE  get_swc_key_State();            // ����״̬ 0:�ͷ� 1:���� 2:����������Ч

	//����
	WORD  get_car_Speed();               // ���� Km/h

};


//////////////////FOCUS/////////////////////////
struct CANBUS_RAISE_CAMRY
{
	BYTE swc_key[2];	    // ��������Ϣ 0x20
	BYTE per_min_oil[7];		//ÿ�����ͺ�0x21
	BYTE instant_oil[3];		//˲ʱ�ͺ�0x22
	BYTE history_oil[13];		//��ʷ�ͺ�0x23
	BYTE basic_info[2];     // ������Ϣ 0x24
	BYTE TPMS[6];             //0x25
	BYTE vehicle_setting[4];   //0x26
	BYTE per_15min_oil[31];		//0x27//ÿ15�����ͺ�0x21
	BYTE climatronic[5];    // �յ���Ϣ 0x28
	BYTE swc_angle[2];      // ������ת�� 0x29 >0 ��ת
	BYTE amplifier_info[5];  //������Ϣ0x31
	BYTE sys_info[1];     //ϵͳ��Ϣ0x32
	BYTE hybrid_electric[2];//0x1f�͵�����Ϣ
	BYTE car_set_status[2];     //0x50
	BYTE car_version[10];   //0x30
	BYTE rear_radar[5];     // ���״� 0x1E
	BYTE front_radar[4];    // ǰ�״� 0x1D


	BYTE start_end[1];      // START/END 0x81
	BYTE setting_cmd[2];  // �趨���� 0x83
	BYTE setting_cmd_baodao[2];  // �趨���� 0x84
	BYTE ctrl_cmd[2];//0x90�����������Ϣ


	static CANBUS_RAISE_CAMRY* get_instance();

	//�յ���Ϣ
	BOOL  is_climatronic_Switch();       // �յ�����ָʾ
	BOOL  is_climatronic_AC();           // A/Cָʾ
// 	BOOL  is_climatronic_Cycle();        // ����ѭ��ָʾ 0:outer 1:inner
	BOOL  is_climatronic_SmallWind();    // С���ָʾ
	BOOL  is_climatronic_Dual();         // ˫������
// 	BOOL  is_climatronic_MaxFornt();     // MAX FORNT��ָʾ

	BOOL  is_climatronic_FDemister();    // ǰ����������(����)FDemister
	// 	BOOL  is_climatronic_RDemister();    // �󵲲�������(����)RDemister
	BOOL  is_climatronic_RearHeat();      // �Ӵ�����״ָ̬ʾ
	BOOL  is_climatronic_Airdir_head();  // �����ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_hands(); // ƽ���ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_feet();  // �����ͷ翪��ָʾ
	BOOL  is_climatronic_REQ_show();     // �յ�������ʾ
	BYTE  get_climatronic_FanSpeed();    // ������С 0~7��
	FLOAT get_climatronic_LTemp();       // ��ʻλ�ô��¶� 0x00:LO 0x1F:HI 18��~26��
	FLOAT get_climatronic_RTemp();       // ����ʻλ�ô��¶�
// 	BOOL  is_climatronic_AC_max();       // AC_max
	BYTE  get_climatronic_tempunit();// AC_max

	//�״���Ϣ
	BYTE  get_radar_front_L();           // ǰ�״��� ��Χ 0x00~0x0A
	BYTE  get_radar_front_L_C();         // ǰ�״�����
	BYTE  get_radar_front_R_C();         // ǰ�״����� 
	BYTE  get_radar_front_R();           // ǰ�״���
	BYTE  get_radar_rear_L();            // ���״���
	BYTE  get_radar_rear_L_C();          // ���״�����
	BYTE  get_radar_rear_R_C();          // ���״����� 
	BYTE  get_radar_rear_R();            // ���״���
	BOOL  is_radar_rear_show();            // ��ʾ״̬
	BYTE  get_radar_rear_range();            // ���� 0:�� 1:Զ
	BOOL  is_radar_rear_switch();		 //�״￪��
	BYTE  get_radar_rear_vol();			//����ǿ��
	//������ת��
	 	INT   get_swc_angle();               //�����̽Ƕ�  < 0 ��ʾ��ת

	//������Ϣ
// 	BOOL  is_car_info_SeatBelt_off();     // ��ȫ��״̬  0:���� 1:δϵ
	// 	BOOL  is_car_info_CleanWater_low();   // ��ϴҺ״̬  0:normal 1:low 
// 	BOOL  is_car_info_Brake_hung();       // ��ɲ״̬  0:���� 1:δ����
	BOOL  is_car_info_Trunk_Open();       // β���״̬  0:off 1:on
	BOOL  is_car_info_Door_RR();          // ������  0:off 1:on
	BOOL  is_car_info_Door_RL();          // ������  0:off 1:on 
	BOOL  is_car_info_Door_FR();          // ǰ����  0:off 1:on 
	BOOL  is_car_info_Door_FL();          // ǰ����  0:off 1:on 
	// 	WORD  get_car_info_EngineSpeed();     // ������ת�� RPM
	// 	WORD  get_car_info_InstantaneousVelocity();// ˲ʱ�ٶ� km/h
	// 	WORD  get_car_info_BattVtg();         // ��ص�ѹ V
// 	INT   get_car_info_TempOutside();     // �����¶� ��
	// 	WORD  get_car_info_Mileage();         // �г���� Km
	// 	WORD  get_car_info_RemainL();         // ʣ������ L
	// 	BOOL  is_car_info_RemainL_low();      // ���������־  0:normal 1:low 
	// 	BOOL  is_car_info_BattVtg_low();      // ��ѹ�����־  0:normal 1:low 

	//ͣ������״̬
	// 	BOOL  is_parking_info_Switch();       // ����������Ϣ״̬  0:off 1:on 
	// 	BOOL  is_parking_info_Radar_beep();   // �״﷢��״̬  0:off 1:on 

	//�����̰�����Ϣ
	BYTE  get_swc_key_Value();             // 0x00:û�а������»��ͷ� 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
	BYTE  get_swc_key_State();            // ����״̬ 0:�ͷ� 1:���� 2:����������Ч

	//����
	// 	WORD  get_car_Speed();               // ���� Km/h

};

/////////////////////////////////////����ѩ����////////////////////////////////////
struct CANBUS_RAISE_PEUGEOT /*: public ParseCanBus*/
{
	BYTE control_cmd[3];           //��������0x01
	BYTE key_cmd[1];               // ��������0x02
	BYTE climatronic[7];          //�յ���Ϣ0x21
	BYTE rear_radar_info[7];      //�����״���Ϣ0x32
	BYTE radar_all_info[6];        //ȫ��λ�״���Ϣ 0x30
	BYTE ecu_info0[6];             //�г�������Ϣ0x33
	BYTE ecu_info1[6];             //�г�������Ϣ0x34
	BYTE ecu_info2[6];             //�г�������Ϣ0x35
	BYTE outside_temp[1];          //�����¶� 0x36
	BYTE car_alarm_info[1024];	   //����������Ϣ0x37
	BYTE car_info[6];			   //����״̬0x38
	BYTE car_action_info[1024];    //��������״̬��Ϣ0x39
	BYTE diagnose_info[1024];	   //�����Ϣ0x3a
	BYTE clear_ecu_data[3];        //�г���������(0x34 0x35)����0x3d
	BYTE memorize_speed[7];       //�����ٶ�ֵ0x3B
	BYTE cruise_speed[7];		   //Ѳ���ٶ�ֵ0x3d
	BYTE radarvol_change_flag[1];  //�״������л���ʶ0x3E


	BYTE car_param_set[2];		   //������������0x80
	BYTE ecu_param_set[3];		   //�г����Բ�������0x82
	BYTE req_alarm_record_info[1]; //���󱨾���¼��Ϣ0x85
	BYTE req_action_state_info[1]; //������״̬��Ϣ0x86
	BYTE req_diagnose_info[1];     //�������������Ϣ0x87
	BYTE memorize_speed_set[7];   //���ü����ٶ�ֵ0x88
	BYTE cruise_speed_set[7];     //����Ѳ���ٶ�ֵ0x89
	BYTE climatronic_set[2];       //�յ�����0x8a
	BYTE req_showinfo_cmd[1];      //������ʾ��Ϣ����0x8f
	
	static CANBUS_RAISE_PEUGEOT* get_instance();

	//�յ���Ϣ
	BOOL  is_climatronic_Switch();       // �յ�����ָʾ
	BOOL  is_climatronic_AC();           // A/Cָʾ
	BOOL  is_climatronic_Cycle();        // ����ѭ��ָʾ 0:outer 1:inner
	BOOL  is_climatronic_RearHeat();     // �󴰼���
	BOOL  is_climatronic_Auto();      // С���ָʾ
	BOOL  is_climatronic_Dual();         // ˫������
	BOOL  is_climatronic_FDemister();    // ǰ����������(����)FDemister
	BOOL  is_climatronic_AC_max();    // �󵲲�������(����)RDemister
	BOOL  is_climatronic_Airdir_head();  // �����ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_hands(); // ƽ���ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_feet();  // �����ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_auto();  //�ֶ��Զ��յ�״̬��ʶ0:�Զ�, 1:�ֶ�

	BOOL  is_climatronic_REQ_show();     // �յ�������ʾ
	BYTE  get_climatronic_FanSpeed();    // ������С 0~7��
	BYTE  get_climatronic_FanStrength();    //����ǿ�� 0:�� 0x01:��  0x10 ��
	FLOAT get_climatronic_LTemp();       // ��ʻλ�ô��¶� 0x00:LO 0x1F:HI 18��~26��
	FLOAT get_climatronic_RTemp();       // ����ʻλ�ô��¶�
	BYTE  get_climatronic_tempunit();// �¶ȵ�λ
	BYTE  get_climatronic_temp_step();// �¶Ȳ���


	//�״���Ϣ
	BYTE  get_radar_work_status();       //�״﹤��״̬
	BYTE  get_radar_front_L();           // ǰ�״��� ��Χ 0x00~0x0A
	BYTE  get_radar_front_C();           // ǰ�״���
	BYTE  get_radar_front_R();           // ǰ�״���
	BYTE  get_radar_rear_L();            // ���״���
	BYTE  get_radar_rear_C();            // ���״���
	BYTE  get_radar_rear_R();            // ���״���


	//������ת��
	INT   get_swc_angle();               //�����̽Ƕ�  < 0 ��ʾ��ת

	//������Ϣ
	BOOL  is_car_info_Door_FL();          // ǰ����  0:off 1:on 
	BOOL  is_car_info_Door_FR();          // ǰ����  0:off 1:on 
	BOOL  is_car_info_Door_RL();          // ������  0:off 1:on 
	BOOL  is_car_info_Door_RR();          // ������  0:off 1:on
	BOOL  is_car_info_Trunk_Open();       // β���״̬  0:off 1:on
	BOOL  is_car_info_SeatBelt_off();     // ��ȫ��״̬  0:���� 1:δϵ
	BOOL  is_car_info_SeatBelt2_off();     // ��ȫ��״̬������ʻ��  0:���� 1:δϵ
	BOOL  is_car_info_RemainL_low();      // ���������־  0:normal 1:low 
	BOOL  is_car_info_Door_locking();	  //��������״̬ 0:ͣ�� 1������
	BOOL  is_parking_info_Switch();       // ����������Ϣ״̬  0:off 1:on 

	BYTE  get_car_info_lighting();       //����״̬0,�ر� 1~7��Ӧ����ֵ0~6
	BOOL  is_car_Rear_radar_Enable();    //�����״�״̬0:ͣ�� 1:����
	BYTE  get_car_parking_status();		//����״̬ 0:���� 1 �ǵ���
	BYTE  get_car_P_shift_status();            //P��״̬ 0:p��  1 ��p��

// 	BOOL  is_car_info_CleanWater_low();   // ��ϴҺ״̬  0:normal 1:low 
// 	WORD  get_car_info_EngineSpeed();     // ������ת�� RPM
// 	WORD  get_car_info_InstantaneousVelocity();// ˲ʱ�ٶ� km/h
// 	WORD  get_car_info_BattVtg();         // ��ص�ѹ V
// 	INT   get_car_info_TempOutside();     // �����¶� ��
// 	WORD  get_car_info_Mileage();         // �г���� Km
// 	WORD  get_car_info_RemainL();         // ʣ������ L
// 	BOOL  is_car_info_BattVtg_low();      // ��ѹ�����־  0:normal 1:low 


	BOOL  is_parking_info_Radar_beep();   // �״﷢��״̬  0:off 1:on 
// 
// 	//�����̰�����Ϣ
// 	BYTE  get_swc_key_Value();             // 0x00:û�а������»��ͷ� 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
// 	BYTE  get_swc_key_State();            // ����״̬ 0:�ͷ� 1:���� 2:����������Ч
// 
// 	//����
// 	WORD  get_car_Speed();               // ���� Km/h

};


/////////////////////////////////////�����ִ�////////////////////////////////////
struct CANBUS_RAISE_HYUNDAI 
{
	BYTE outside_temp[1];           //��������0x01
	BYTE swc_key[1];				// ��������Ϣ 0x02
	BYTE climatronic[4];           //�յ���Ϣ0x03
	BYTE radar_all_info[2];        //ȫ��λ�״���Ϣ 0x04

	BYTE ON_OFF_cmd[1];           //���ػ�����0x04
	BYTE eqBalance_cmd[2];        // 0x07
	BYTE audioset_cmd[3];         //��Ч��������
	BYTE host_info[20];          //DVD����״̬��Ϣ0x09
	BYTE radio_info[4];           //��������Ϣ0x09
	BYTE media_info[6];			  //ý����Ϣ0x09
	BYTE bt_info[2];			  //����״̬��Ϣ0x09

	static CANBUS_RAISE_HYUNDAI* get_instance();

	//�յ���Ϣ
	BOOL  is_climatronic_Switch();       // �յ�����ָʾ
	BOOL  is_climatronic_AC();           // A/Cָʾ
	BOOL  is_climatronic_Cycle();        // ����ѭ��ָʾ 0:outer 1:inner
	BOOL  is_climatronic_Auto();      // С���ָʾ
	BOOL  is_climatronic_Dual();         // ˫������
	BOOL  is_climatronic_FDemister();    // ǰ����������(����)FDemister
	BOOL  is_climatronic_RDemister();    // �󵲲�������(����)FDemister
// 	BOOL  is_climatronic_Airdir_head();  // �����ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_hands(); // ƽ���ͷ翪��ָʾ
	BOOL  is_climatronic_Airdir_feet();  // �����ͷ翪��ָʾ

	BOOL  is_climatronic_REQ_show();     // �յ�������ʾ
	BYTE  get_climatronic_FanSpeed();    // ������С 0~7��
	FLOAT get_climatronic_LTemp();       // ��ʻλ�ô��¶� 0x00:LO 0x1F:HI 18��~26��
	FLOAT get_climatronic_RTemp();       // ����ʻλ�ô��¶�
// 	BYTE  get_climatronic_tempunit();// �¶ȵ�λ


	//�״���Ϣ
	BYTE  get_radar_front_L();           // ǰ���״� ��Χ 0x00~0x0A
	BYTE  get_radar_front_C();           // ǰ���״�
	BYTE  get_radar_front_R();           // ǰ���״�
	BYTE  get_radar_rear_L();            // �����״�
	BYTE  get_radar_rear_C();            // �����״�
	BYTE  get_radar_rear_R();            // �����״�


	//������ת��
// 	INT   get_swc_angle();               //�����̽Ƕ�  < 0 ��ʾ��ת

	//������Ϣ

	INT   get_car_info_TempOutside();     // �����¶� ��


	// 
	// 	//�����̰�����Ϣ
	// 	BYTE  get_swc_key_Value();             // 0x00:û�а������»��ͷ� 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
	// 	BYTE  get_swc_key_State();            // ����״̬ 0:�ͷ� 1:���� 2:����������Ч

};

// ���Զ�����������ͼ������е�can��Ϣ,������ݲ���,�ٿ���ô����
typedef enum _CANBUS_DATA_TYPE_
{
	CBDT_SPEED,			 // ������Ϣ
	CBDT_SWC_KEY,		 // ��������Ϣ
	CBDT_CLIMATRONIC,	 // �յ���Ϣ
	CBDT_REAR_RADAR,	 // ���״�
	CBDT_FRONT_RADAR,	 // ǰ�״�
	CBDT_BASIC_INFO,	 // ������Ϣ
	CBDT_PARKING_INFO,   // ͣ������״̬
	CBDT_SWC_ANGLE,		 // ������ת��
	CBDT_CAR_INFO,		 // ������Ϣ
	                    
	CBDT_START_END,      // START/END
	CBDT_REQ_CTRL_INFO,  // �����������Ϣ
	CBDT_SOURCE,         // SOURCE
	CBDT_ICON,           // ICON
	CBDT_RADIO_INFO,     // ������Ϣ
	CBDT_MEDIA_PLAY_INFO,// ý�岥����Ϣ
	CBDT_VOL_SHOW_CTRL,  // ������ʾ����
	CBDT_RADAR_VOL_CTRL, // �״���������

	CBDT_INVALID = 0xFF,
}CANBUS_DATA_TYPE;