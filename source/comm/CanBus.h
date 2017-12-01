#pragma once

// 总共支持的canbus的类型
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
    //空调信息
	virtual BOOL  is_climatronic_Switch()        = 0;  // 空调开关指示
	virtual BOOL  is_climatronic_AC()           = 0;  // A/C指示
	virtual BOOL  is_climatronic_Cycle()        = 0;  // 内外循环指示 0:outer 1:inner
	virtual BOOL  is_climatronic_HighWind()     = 0;  // 大风灯指示
	virtual BOOL  is_climatronic_SmallWind()    = 0;  // 小风灯指示
	virtual BOOL  is_climatronic_Dual()         = 0;  // 双区控制
	virtual BOOL  is_climatronic_FDemister()    = 0;  // 前挡玻璃吹风(除雾)FDemister
	virtual BOOL  is_climatronic_RDemister()    = 0;  // 后挡玻璃吹风(除雾)RDemister
	virtual BOOL  is_climatronic_Airdir_head()  = 0;  // 向上送风开关指示
	virtual BOOL  is_climatronic_Airdir_hands() = 0;  // 平行送风开关指示
	virtual BOOL  is_climatronic_Airdir_feet()  = 0;  // 向下送风开关指示
	virtual BOOL  is_climatronic_REQ_show()     = 0;  // 空调请求显示
	virtual BYTE  get_climatronic_FanSpeed()    = 0;  // 风量大小 0~7级
	virtual FLOAT get_climatronic_LTemp()       = 0;  // 驾驶位置处温度 0x00:LO 0x1F:HI 18℃~26℃
	virtual FLOAT get_climatronic_RTemp()       = 0;  // 副驾驶位置处温度
	virtual BOOL  get_climatronic_AQS()         = 0;  // AQS内循环
	virtual BYTE  get_climatronic_LHotSeat()    = 0;  // 左边座椅加热 0:不显示 1~3: 1~3级温度
	virtual BYTE  get_climatronic_RHotSeat()    = 0;  // 右边座椅加热 

	//雷达信息
	virtual BYTE  get_radar_front_L()           = 0;  // 前雷达左 范围 0x00~0x0A
	virtual BYTE  get_radar_front_L_C()         = 0;  // 前雷达左中
	virtual BYTE  get_radar_front_R_C()         = 0;  // 前雷达右中 
	virtual BYTE  get_radar_front_R()           = 0;  // 前雷达右
	virtual BYTE  get_radar_rear_L()            = 0;  // 后雷达左
	virtual BYTE  get_radar_rear_L_C()          = 0;  // 后雷达左中
	virtual BYTE  get_radar_rear_R_C()          = 0;  // 后雷达右中 
	virtual BYTE  get_radar_rear_R()            = 0;  // 后雷达右

    //方向盘转角
	virtual INT   get_swc_angle()               = 0;  //方向盘角度  < 0 表示右转

    //车身信息
	virtual BOOL  is_car_info_SeatBelt_off()     = 0;  // 安全带状态  0:正常 1:未系
	virtual BOOL  is_car_info_CleanWater_low()   = 0;  // 清洗液状态  0:normal 1:low 
	virtual BOOL  is_car_info_Brake_hung()       = 0;  // 手刹状态  0:正常 1:未放下
	virtual BOOL  is_car_info_Trunk_Open()       = 0;  // 尾箱盖状态  0:off 1:on
	virtual BOOL  is_car_info_Door_RR()          = 0;  // 后右门  0:off 1:on
	virtual BOOL  is_car_info_Door_RL()          = 0;  // 后左门  0:off 1:on 
	virtual BOOL  is_car_info_Door_FR()          = 0;  // 前右门  0:off 1:on 
	virtual BOOL  is_car_info_Door_FL()          = 0;  // 前左门  0:off 1:on 
	virtual WORD  get_car_info_EngineSpeed()     = 0;  // 发动机转速 RPM
	virtual WORD  get_car_info_InstantaneousVelocity()= 0;  // 瞬时速度 km/h
	virtual WORD  get_car_info_BattVtg()         = 0;  // 电池电压 V
	virtual INT   get_car_info_TempOutside()     = 0;  // 车外温度 ℃
	virtual WORD  get_car_info_Mileage()         = 0;  // 行车里程 Km
	virtual WORD  get_car_info_RemainL()         = 0;  // 剩余油量 L
	virtual BOOL  is_car_info_RemainL_low()      = 0;  // 油量警告标志  0:normal 1:low 
	virtual BOOL  is_car_info_BattVtg_low()      = 0;  // 电压警告标志  0:normal 1:low 

	//停车辅助状态
	virtual BOOL  is_parking_info_Switch()       = 0;  // 泊车辅助信息状态  0:off 1:on 
	virtual BOOL  is_parking_info_Radar_beep()   = 0;  // 雷达发声状态  0:off 1:on 

	//方向盘按键信息
	virtual BYTE  get_swc_key_Value()             = 0;  // 0x00:没有按键按下或释放 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
	virtual BYTE  get_swc_key_State()            = 0;  // 按键状态 0:释放 1:按下 2:连续按键有效
    
	//车速
	virtual WORD  get_car_Speed()               = 0;  // 车速 Km/h

	// 主动请求can的初始信息
	virtual void request_can_data(BYTE type, BYTE info_id)             = 0;


};

struct CANBUS_RAISE_DASAUTO : public ParseCanBus
{
	BYTE speed[2];		    // 车速 0x16
	BYTE swc_key[2];	    // 方向盘信息 0x20
	BYTE climatronic[5];    // 空调信息 0x21
	BYTE rear_radar[4];     // 后雷达 0x22
	BYTE front_radar[4];    // 前雷达 0x23
	BYTE basic_info[1];     // 基本信息 0x24
	BYTE parking_info[2];   // 停车辅助状态 0x25
	BYTE swc_angle[2];      // 方向盘转角 0x26

	BYTE car_info1[2];      // 车身信息 0x41 -- 01
	BYTE car_info2[13];     // 车身信息 0x41 -- 02
	BYTE car_info3[2];      // 车身信息 0x41 -- 03

	BYTE start_end[1];      // START/END 0x81
	BYTE req_ctrl_info[2];  // 请求控制器信息 0x90
	BYTE source[2];         // SOURCE 0xC0
	BYTE icon[1];           // ICON 0xC1
	BYTE radio_info[4];     // 收音信息 0xC2
	BYTE media_play_info[6];// 媒体播放信息 0xC3
	BYTE vol_show_ctrl[1];	// 音量显示控制 0xC4
	BYTE radar_vol_ctrl[2];	// 雷达音量控制 0xC6

	static CANBUS_RAISE_DASAUTO* get_instance();

    //空调信息
	BOOL  is_climatronic_Switch();       // 空调开关指示
	BOOL  is_climatronic_AC();           // A/C指示
	BOOL  is_climatronic_Cycle();        // 内外循环指示 0:outer 1:inner
	BOOL  is_climatronic_HighWind();     // 大风灯指示
	BOOL  is_climatronic_SmallWind();    // 小风灯指示
	BOOL  is_climatronic_Dual();         // 双区控制
	BOOL  is_climatronic_FDemister();    // 前挡玻璃吹风(除雾)FDemister
	BOOL  is_climatronic_RDemister();    // 后挡玻璃吹风(除雾)RDemister
	BOOL  is_climatronic_Airdir_head();  // 向上送风开关指示
	BOOL  is_climatronic_Airdir_hands(); // 平行送风开关指示
	BOOL  is_climatronic_Airdir_feet();  // 向下送风开关指示
	BOOL  is_climatronic_REQ_show();     // 空调请求显示
	BYTE  get_climatronic_FanSpeed();    // 风量大小 0~7级
	FLOAT get_climatronic_LTemp();       // 驾驶位置处温度 0x00:LO 0x1F:HI 18℃~26℃
	FLOAT get_climatronic_RTemp();       // 副驾驶位置处温度
	BOOL  get_climatronic_AQS();         // AQS内循环
	BYTE  get_climatronic_LHotSeat();    // 左边座椅加热 0:不显示 1~3: 1~3级温度
	BYTE  get_climatronic_RHotSeat();    // 右边座椅加热 

	//雷达信息
	BYTE  get_radar_front_L();           // 前雷达左 范围 0x00~0x0A
	BYTE  get_radar_front_L_C();         // 前雷达左中
	BYTE  get_radar_front_R_C();         // 前雷达右中 
	BYTE  get_radar_front_R();           // 前雷达右
	BYTE  get_radar_rear_L();            // 后雷达左
	BYTE  get_radar_rear_L_C();          // 后雷达左中
	BYTE  get_radar_rear_R_C();          // 后雷达右中 
	BYTE  get_radar_rear_R();            // 后雷达右

    //方向盘转角
	INT   get_swc_angle();               //方向盘角度  < 0 表示右转

    //车身信息
	BOOL  is_car_info_SeatBelt_off();     // 安全带状态  0:正常 1:未系
	BOOL  is_car_info_CleanWater_low();   // 清洗液状态  0:normal 1:low 
	BOOL  is_car_info_Brake_hung();       // 手刹状态  0:正常 1:未放下
	BOOL  is_car_info_Trunk_Open();       // 尾箱盖状态  0:off 1:on
	BOOL  is_car_info_Door_RR();          // 后右门  0:off 1:on
	BOOL  is_car_info_Door_RL();          // 后左门  0:off 1:on 
	BOOL  is_car_info_Door_FR();          // 前右门  0:off 1:on 
	BOOL  is_car_info_Door_FL();          // 前左门  0:off 1:on 
	WORD  get_car_info_EngineSpeed();     // 发动机转速 RPM
	WORD  get_car_info_InstantaneousVelocity();// 瞬时速度 km/h
	WORD  get_car_info_BattVtg();         // 电池电压 V
	INT   get_car_info_TempOutside();     // 车外温度 ℃
	WORD  get_car_info_Mileage();         // 行车里程 Km
	WORD  get_car_info_RemainL();         // 剩余油量 L
	BOOL  is_car_info_RemainL_low();      // 油量警告标志  0:normal 1:low 
	BOOL  is_car_info_BattVtg_low();      // 电压警告标志  0:normal 1:low 

	//停车辅助状态
	BOOL  is_parking_info_Switch();       // 泊车辅助信息状态  0:off 1:on 
	BOOL  is_parking_info_Radar_beep();   // 雷达发声状态  0:off 1:on 

	//方向盘按键信息
	BYTE  get_swc_key_Value();             // 0x00:没有按键按下或释放 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
	BYTE  get_swc_key_State();            // 按键状态 0:释放 1:按下 2:连续按键有效
    
	//车速
	WORD  get_car_Speed();               // 车速 Km/h

	// 主动请求can的初始信息
	// 参数含义:
	// 0x16--车速信息, 0x21--空调信息, 0x24--基本信息, 
	// 0x25--泊车辅助状态信息, 0x41--车身信息(请求车身信息时需指定info_id)
	void request_can_data(BYTE type, BYTE info_id);
};


//////////////////FOCUS/////////////////////////
struct CANBUS_RAISE_FOCUS 
{
	BYTE backlight[1];		    // 车速 0x14
	BYTE swc_key[2];	    // 方向盘信息 0x20
	BYTE climatronic[6];    // 空调信息 0x21
	BYTE rear_radar[4];     // 后雷达 0x22
	BYTE front_radar[6];    // 前雷达 0x23
	BYTE basic_info[2];     // 基本信息 0x24
	BYTE parking_info[2];   // 停车辅助状态 0x25
	BYTE datetime_info[6];		//车身时间信息0x26
	BYTE car_version[10];	//车身版本信息0x30
	BYTE sync_srt_up[14];    // 0x70
	BYTE sync_srt_down[14];    // 0x71
	BYTE sync_srt_short[5];    // 0x72
	BYTE sync_srt_status[5];    // 0x78
	BYTE audio_change[1];       //0x79 Audio切换请求
	BYTE car_langue_set[2];		//0x27
	BYTE sync_version[1];       //0x40
	BYTE sync_V3_menu[8];       //0x50
	BYTE sync_V3_menuitem[256];       //0x51
	BYTE sync_media_tracks_tm[3];       //0x52
	BYTE sync_talk_tm[3];       //0x53
	BYTE key_cmd[1];               // 按键命令0x01


	BYTE ctrl_cmd[2];  // 请求控制器信息 0xC6
	BYTE start_end[1];      // START/END 0x81
	BYTE car_datetime_set[6];//0x82
	BYTE req_decode_status[1];//0x90

// 	BYTE swc_angle[2];      // 方向盘转角 0x26
// 	BYTE car_info[13];      // 车身信息 0x41
// 	BYTE source[2];         // SOURCE 0xC0
// 	BYTE icon[1];           // ICON 0xC1
// 	BYTE radio_info[4];     // 收音信息 0xC2
// 	BYTE media_play_info[6];// 媒体播放信息 0xC3
// 	BYTE vol_show_ctrl[1];	// 音量显示控制 0xC4
// 	BYTE radar_vol_ctrl[2];	// 雷达音量控制 0xC6

	static CANBUS_RAISE_FOCUS* get_instance();

	//空调信息
	BOOL  is_climatronic_Switch();       // 空调开关指示
	BOOL  is_climatronic_AC();           // A/C指示
	BOOL  is_climatronic_Cycle();        // 内外循环指示 0:outer 1:inner
	BOOL  is_climatronic_SmallWind();    // 小风灯指示
	BOOL  is_climatronic_Dual();         // 双区控制
	BOOL  is_climatronic_MaxFornt();     // MAX FORNT灯指示

// 	BOOL  is_climatronic_FDemister();    // 前挡玻璃吹风(除雾)FDemister
// 	BOOL  is_climatronic_RDemister();    // 后挡玻璃吹风(除雾)RDemister
	BOOL  is_climatronic_Airdir_head();  // 向上送风开关指示
	BOOL  is_climatronic_Airdir_hands(); // 平行送风开关指示
	BOOL  is_climatronic_Airdir_feet();  // 向下送风开关指示
 	BOOL  is_climatronic_REQ_show();     // 空调请求显示
	BYTE  get_climatronic_FanSpeed();    // 风量大小 0~7级
	FLOAT get_climatronic_LTemp();       // 驾驶位置处温度 0x00:LO 0x1F:HI 18℃~26℃
	FLOAT get_climatronic_RTemp();       // 副驾驶位置处温度
	BOOL  is_climatronic_AC_max();       // AC_max
	BYTE  get_climatronic_tempunit();// AC_max

// 	BOOL  get_climatronic_AQS();         // AQS内循环
// 	BYTE  get_climatronic_LHotSeat();    // 左边座椅加热 0:不显示 1~3: 1~3级温度
// 	BYTE  get_climatronic_RHotSeat();    // 右边座椅加热 

	//雷达信息
	BYTE  get_radar_front_L();           // 前雷达左 范围 0x00~0x0A
	BYTE  get_radar_front_L_C();         // 前雷达左中
	BYTE  get_radar_front_R_C();         // 前雷达右中 
	BYTE  get_radar_front_R();           // 前雷达右
	BYTE  get_radar_rear_L();            // 后雷达左
	BYTE  get_radar_rear_L_C();          // 后雷达左中
	BYTE  get_radar_rear_R_C();          // 后雷达右中 
	BYTE  get_radar_rear_R();            // 后雷达右

	//方向盘转角
// 	INT   get_swc_angle();               //方向盘角度  < 0 表示右转

	//车身信息
	BOOL  is_car_info_SeatBelt_off();     // 安全带状态  0:正常 1:未系
// 	BOOL  is_car_info_CleanWater_low();   // 清洗液状态  0:normal 1:low 
	BOOL  is_car_info_Brake_hung();       // 手刹状态  0:正常 1:未放下
	BOOL  is_car_info_Trunk_Open();       // 尾箱盖状态  0:off 1:on
	BOOL  is_car_info_Door_RR();          // 后右门  0:off 1:on
	BOOL  is_car_info_Door_RL();          // 后左门  0:off 1:on 
	BOOL  is_car_info_Door_FR();          // 前右门  0:off 1:on 
	BOOL  is_car_info_Door_FL();          // 前左门  0:off 1:on 
// 	WORD  get_car_info_EngineSpeed();     // 发动机转速 RPM
// 	WORD  get_car_info_InstantaneousVelocity();// 瞬时速度 km/h
// 	WORD  get_car_info_BattVtg();         // 电池电压 V
	INT   get_car_info_TempOutside();     // 车外温度 ℃
// 	WORD  get_car_info_Mileage();         // 行车里程 Km
// 	WORD  get_car_info_RemainL();         // 剩余油量 L
// 	BOOL  is_car_info_RemainL_low();      // 油量警告标志  0:normal 1:low 
// 	BOOL  is_car_info_BattVtg_low();      // 电压警告标志  0:normal 1:low 

	//停车辅助状态
// 	BOOL  is_parking_info_Switch();       // 泊车辅助信息状态  0:off 1:on 
// 	BOOL  is_parking_info_Radar_beep();   // 雷达发声状态  0:off 1:on 

	//方向盘按键信息
    BYTE  get_swc_key_Value();             // 0x00:没有按键按下或释放 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
    BYTE  get_swc_key_State();            // 按键状态 0:释放 1:按下 2:连续按键有效

	//车速
// 	WORD  get_car_Speed();               // 车速 Km/h

};


/////////////////////////////HONDA////////////////////////////////
struct CANBUS_RAISE_HONDA
{
	BYTE swc_key[2];	    // 方向盘信息 0x20
	BYTE climatronic[6];    // 空调信息 0x21
	BYTE basic_info[1];     // 基本信息 0x24
	BYTE version_info[10];   // 版本信息 0x30
	BYTE car_info[4];      // 车身信息 0x32
	BYTE car_Mileage[15];      // 油耗里程 0x33
	BYTE swc_angle[2];      // 方向盘转角 0x29
	BYTE car_set_ack[2];      // 车辆设置状态反馈 0xD0
	BYTE backlight[1];		    //背光调节信息 0x14
	BYTE rear_radar[4];     // 后雷达 0x22
// 	BYTE front_radar[4];    // 前雷达 0x23
	BYTE parking_info[2];   // 停车辅助状态 0x25


	BYTE start_end[1];      // START/END 0x81
	BYTE set_cmd[2];  // 设定命令 0xC6
	BYTE query_cmd[2];  // 设定命令 0x90


	static CANBUS_RAISE_HONDA* get_instance();

	//空调信息
	BOOL  is_climatronic_Switch();       // 空调开关指示
	BOOL  is_climatronic_AC();           // A/C指示
	BOOL  is_climatronic_Cycle();        // 内外循环指示 0:outer 1:inner
	BOOL  is_climatronic_HighWind();     // 大风灯指示
	BOOL  is_climatronic_SmallWind();    // 小风灯指示
	BOOL  is_climatronic_Dual();         // 双区控制
	BOOL  is_climatronic_FDemister();    // 前挡玻璃吹风(除雾)FDemister
	BOOL  is_climatronic_RDemister();    // 后挡玻璃吹风(除雾)RDemister
	BOOL  is_climatronic_Airdir_head();  // 向上送风开关指示
	BOOL  is_climatronic_Airdir_hands(); // 平行送风开关指示
	BOOL  is_climatronic_Airdir_feet();  // 向下送风开关指示
	BOOL  is_climatronic_REQ_show();     // 空调请求显示
	BYTE  get_climatronic_FanSpeed();    // 风量大小 0~7级
	FLOAT get_climatronic_LTemp();       // 驾驶位置处温度 0x00:LO 0x1F:HI 18℃~26℃
	FLOAT get_climatronic_RTemp();       // 副驾驶位置处温度
	BOOL  get_climatronic_AQS();         // AQS内循环
	BYTE  get_climatronic_LHotSeat();    // 左边座椅加热 0:不显示 1~3: 1~3级温度
	BYTE  get_climatronic_RHotSeat();    // 右边座椅加热 

	//雷达信息
// 	BYTE  get_radar_front_L();           // 前雷达左 范围 0x00~0x0A
// 	BYTE  get_radar_front_L_C();         // 前雷达左中
// 	BYTE  get_radar_front_R_C();         // 前雷达右中 
// 	BYTE  get_radar_front_R();           // 前雷达右
	BYTE  get_radar_rear_L();            // 后雷达左
	BYTE  get_radar_rear_L_C();          // 后雷达左中
	BYTE  get_radar_rear_R_C();          // 后雷达右中 
	BYTE  get_radar_rear_R();            // 后雷达右

	//方向盘转角
	INT   get_swc_angle();               //方向盘角度  < 0 表示右转

	//车身信息
	BOOL  is_car_info_SeatBelt_off();     // 安全带状态  0:正常 1:未系
	BOOL  is_car_info_CleanWater_low();   // 清洗液状态  0:normal 1:low 
	BOOL  is_car_info_Brake_hung();       // 手刹状态  0:正常 1:未放下
	BOOL  is_car_info_Trunk_Open();       // 尾箱盖状态  0:off 1:on
	BOOL  is_car_info_Door_RR();          // 后右门  0:off 1:on
	BOOL  is_car_info_Door_RL();          // 后左门  0:off 1:on 
	BOOL  is_car_info_Door_FR();          // 前右门  0:off 1:on 
	BOOL  is_car_info_Door_FL();          // 前左门  0:off 1:on 
	WORD  get_car_info_EngineSpeed();     // 发动机转速 RPM
	WORD  get_car_info_InstantaneousVelocity();// 瞬时速度 km/h
	WORD  get_car_info_BattVtg();         // 电池电压 V
// 	INT   get_car_info_TempOutside();     // 车外温度 ℃
	WORD  get_car_info_Mileage();         // 行车里程 Km
	WORD  get_car_info_RemainL();         // 剩余油量 L
	BOOL  is_car_info_RemainL_low();      // 油量警告标志  0:normal 1:low 
	BOOL  is_car_info_BattVtg_low();      // 电压警告标志  0:normal 1:low 

	//停车辅助状态
	BOOL  is_parking_info_Switch();       // 泊车辅助信息状态  0:off 1:on 
	BOOL  is_parking_info_Radar_beep();   // 雷达发声状态  0:off 1:on 

	//方向盘按键信息
	BYTE  get_swc_key_Value();             // 0x00:没有按键按下或释放 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
	BYTE  get_swc_key_State();            // 按键状态 0:释放 1:按下 2:连续按键有效

	//车速
	WORD  get_car_Speed();               // 车速 Km/h

};


//////////////////FOCUS/////////////////////////
struct CANBUS_RAISE_CAMRY
{
	BYTE swc_key[2];	    // 方向盘信息 0x20
	BYTE per_min_oil[7];		//每分钟油耗0x21
	BYTE instant_oil[3];		//瞬时油耗0x22
	BYTE history_oil[13];		//历史油耗0x23
	BYTE basic_info[2];     // 基本信息 0x24
	BYTE TPMS[6];             //0x25
	BYTE vehicle_setting[4];   //0x26
	BYTE per_15min_oil[31];		//0x27//每15分钟油耗0x21
	BYTE climatronic[5];    // 空调信息 0x28
	BYTE swc_angle[2];      // 方向盘转角 0x29 >0 右转
	BYTE amplifier_info[5];  //功放信息0x31
	BYTE sys_info[1];     //系统信息0x32
	BYTE hybrid_electric[2];//0x1f油电混合信息
	BYTE car_set_status[2];     //0x50
	BYTE car_version[10];   //0x30
	BYTE rear_radar[5];     // 后雷达 0x1E
	BYTE front_radar[4];    // 前雷达 0x1D


	BYTE start_end[1];      // START/END 0x81
	BYTE setting_cmd[2];  // 设定命令 0x83
	BYTE setting_cmd_baodao[2];  // 设定命令 0x84
	BYTE ctrl_cmd[2];//0x90请求控制器信息


	static CANBUS_RAISE_CAMRY* get_instance();

	//空调信息
	BOOL  is_climatronic_Switch();       // 空调开关指示
	BOOL  is_climatronic_AC();           // A/C指示
// 	BOOL  is_climatronic_Cycle();        // 内外循环指示 0:outer 1:inner
	BOOL  is_climatronic_SmallWind();    // 小风灯指示
	BOOL  is_climatronic_Dual();         // 双区控制
// 	BOOL  is_climatronic_MaxFornt();     // MAX FORNT灯指示

	BOOL  is_climatronic_FDemister();    // 前挡玻璃吹风(除雾)FDemister
	// 	BOOL  is_climatronic_RDemister();    // 后挡玻璃吹风(除雾)RDemister
	BOOL  is_climatronic_RearHeat();      // 加窗加热状态指示
	BOOL  is_climatronic_Airdir_head();  // 向上送风开关指示
	BOOL  is_climatronic_Airdir_hands(); // 平行送风开关指示
	BOOL  is_climatronic_Airdir_feet();  // 向下送风开关指示
	BOOL  is_climatronic_REQ_show();     // 空调请求显示
	BYTE  get_climatronic_FanSpeed();    // 风量大小 0~7级
	FLOAT get_climatronic_LTemp();       // 驾驶位置处温度 0x00:LO 0x1F:HI 18℃~26℃
	FLOAT get_climatronic_RTemp();       // 副驾驶位置处温度
// 	BOOL  is_climatronic_AC_max();       // AC_max
	BYTE  get_climatronic_tempunit();// AC_max

	//雷达信息
	BYTE  get_radar_front_L();           // 前雷达左 范围 0x00~0x0A
	BYTE  get_radar_front_L_C();         // 前雷达左中
	BYTE  get_radar_front_R_C();         // 前雷达右中 
	BYTE  get_radar_front_R();           // 前雷达右
	BYTE  get_radar_rear_L();            // 后雷达左
	BYTE  get_radar_rear_L_C();          // 后雷达左中
	BYTE  get_radar_rear_R_C();          // 后雷达右中 
	BYTE  get_radar_rear_R();            // 后雷达右
	BOOL  is_radar_rear_show();            // 显示状态
	BYTE  get_radar_rear_range();            // 距离 0:近 1:远
	BOOL  is_radar_rear_switch();		 //雷达开关
	BYTE  get_radar_rear_vol();			//音量强度
	//方向盘转角
	 	INT   get_swc_angle();               //方向盘角度  < 0 表示右转

	//车身信息
// 	BOOL  is_car_info_SeatBelt_off();     // 安全带状态  0:正常 1:未系
	// 	BOOL  is_car_info_CleanWater_low();   // 清洗液状态  0:normal 1:low 
// 	BOOL  is_car_info_Brake_hung();       // 手刹状态  0:正常 1:未放下
	BOOL  is_car_info_Trunk_Open();       // 尾箱盖状态  0:off 1:on
	BOOL  is_car_info_Door_RR();          // 后右门  0:off 1:on
	BOOL  is_car_info_Door_RL();          // 后左门  0:off 1:on 
	BOOL  is_car_info_Door_FR();          // 前右门  0:off 1:on 
	BOOL  is_car_info_Door_FL();          // 前左门  0:off 1:on 
	// 	WORD  get_car_info_EngineSpeed();     // 发动机转速 RPM
	// 	WORD  get_car_info_InstantaneousVelocity();// 瞬时速度 km/h
	// 	WORD  get_car_info_BattVtg();         // 电池电压 V
// 	INT   get_car_info_TempOutside();     // 车外温度 ℃
	// 	WORD  get_car_info_Mileage();         // 行车里程 Km
	// 	WORD  get_car_info_RemainL();         // 剩余油量 L
	// 	BOOL  is_car_info_RemainL_low();      // 油量警告标志  0:normal 1:low 
	// 	BOOL  is_car_info_BattVtg_low();      // 电压警告标志  0:normal 1:low 

	//停车辅助状态
	// 	BOOL  is_parking_info_Switch();       // 泊车辅助信息状态  0:off 1:on 
	// 	BOOL  is_parking_info_Radar_beep();   // 雷达发声状态  0:off 1:on 

	//方向盘按键信息
	BYTE  get_swc_key_Value();             // 0x00:没有按键按下或释放 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
	BYTE  get_swc_key_State();            // 按键状态 0:释放 1:按下 2:连续按键有效

	//车速
	// 	WORD  get_car_Speed();               // 车速 Km/h

};

/////////////////////////////////////标致雪铁龙////////////////////////////////////
struct CANBUS_RAISE_PEUGEOT /*: public ParseCanBus*/
{
	BYTE control_cmd[3];           //控制命令0x01
	BYTE key_cmd[1];               // 按键命令0x02
	BYTE climatronic[7];          //空调信息0x21
	BYTE rear_radar_info[7];      //倒车雷达信息0x32
	BYTE radar_all_info[6];        //全方位雷达信息 0x30
	BYTE ecu_info0[6];             //行车电脑信息0x33
	BYTE ecu_info1[6];             //行车电脑信息0x34
	BYTE ecu_info2[6];             //行车电脑信息0x35
	BYTE outside_temp[1];          //室外温度 0x36
	BYTE car_alarm_info[1024];	   //汽车报警信息0x37
	BYTE car_info[6];			   //汽车状态0x38
	BYTE car_action_info[1024];    //汽车功能状态信息0x39
	BYTE diagnose_info[1024];	   //诊断信息0x3a
	BYTE clear_ecu_data[3];        //行车电脑数据(0x34 0x35)清零0x3d
	BYTE memorize_speed[7];       //记忆速度值0x3B
	BYTE cruise_speed[7];		   //巡航速度值0x3d
	BYTE radarvol_change_flag[1];  //雷达声音切换标识0x3E


	BYTE car_param_set[2];		   //车辆参数设置0x80
	BYTE ecu_param_set[3];		   //行车电脑参数设置0x82
	BYTE req_alarm_record_info[1]; //请求报警记录信息0x85
	BYTE req_action_state_info[1]; //请求功能状态信息0x86
	BYTE req_diagnose_info[1];     //请求汽车诊断信息0x87
	BYTE memorize_speed_set[7];   //设置记忆速度值0x88
	BYTE cruise_speed_set[7];     //设置巡航速度值0x89
	BYTE climatronic_set[2];       //空调设置0x8a
	BYTE req_showinfo_cmd[1];      //请求显示信息命令0x8f
	
	static CANBUS_RAISE_PEUGEOT* get_instance();

	//空调信息
	BOOL  is_climatronic_Switch();       // 空调开关指示
	BOOL  is_climatronic_AC();           // A/C指示
	BOOL  is_climatronic_Cycle();        // 内外循环指示 0:outer 1:inner
	BOOL  is_climatronic_RearHeat();     // 后窗加热
	BOOL  is_climatronic_Auto();      // 小风灯指示
	BOOL  is_climatronic_Dual();         // 双区控制
	BOOL  is_climatronic_FDemister();    // 前挡玻璃吹风(除雾)FDemister
	BOOL  is_climatronic_AC_max();    // 后挡玻璃吹风(除雾)RDemister
	BOOL  is_climatronic_Airdir_head();  // 向上送风开关指示
	BOOL  is_climatronic_Airdir_hands(); // 平行送风开关指示
	BOOL  is_climatronic_Airdir_feet();  // 向下送风开关指示
	BOOL  is_climatronic_Airdir_auto();  //手动自动空调状态标识0:自动, 1:手动

	BOOL  is_climatronic_REQ_show();     // 空调请求显示
	BYTE  get_climatronic_FanSpeed();    // 风量大小 0~7级
	BYTE  get_climatronic_FanStrength();    //风量强度 0:低 0x01:中  0x10 高
	FLOAT get_climatronic_LTemp();       // 驾驶位置处温度 0x00:LO 0x1F:HI 18℃~26℃
	FLOAT get_climatronic_RTemp();       // 副驾驶位置处温度
	BYTE  get_climatronic_tempunit();// 温度单位
	BYTE  get_climatronic_temp_step();// 温度步长


	//雷达信息
	BYTE  get_radar_work_status();       //雷达工作状态
	BYTE  get_radar_front_L();           // 前雷达左 范围 0x00~0x0A
	BYTE  get_radar_front_C();           // 前雷达中
	BYTE  get_radar_front_R();           // 前雷达右
	BYTE  get_radar_rear_L();            // 后雷达左
	BYTE  get_radar_rear_C();            // 后雷达中
	BYTE  get_radar_rear_R();            // 后雷达右


	//方向盘转角
	INT   get_swc_angle();               //方向盘角度  < 0 表示右转

	//车身信息
	BOOL  is_car_info_Door_FL();          // 前左门  0:off 1:on 
	BOOL  is_car_info_Door_FR();          // 前右门  0:off 1:on 
	BOOL  is_car_info_Door_RL();          // 后左门  0:off 1:on 
	BOOL  is_car_info_Door_RR();          // 后右门  0:off 1:on
	BOOL  is_car_info_Trunk_Open();       // 尾箱盖状态  0:off 1:on
	BOOL  is_car_info_SeatBelt_off();     // 安全带状态  0:正常 1:未系
	BOOL  is_car_info_SeatBelt2_off();     // 安全带状态（副驾驶）  0:正常 1:未系
	BOOL  is_car_info_RemainL_low();      // 油量警告标志  0:normal 1:low 
	BOOL  is_car_info_Door_locking();	  //车门自锁状态 0:停用 1：启用
	BOOL  is_parking_info_Switch();       // 泊车辅助信息状态  0:off 1:on 

	BYTE  get_car_info_lighting();       //照明状态0,关闭 1~7对应亮度值0~6
	BOOL  is_car_Rear_radar_Enable();    //倒车雷达状态0:停用 1:启用
	BYTE  get_car_parking_status();		//倒车状态 0:倒车 1 非倒车
	BYTE  get_car_P_shift_status();            //P档状态 0:p档  1 非p档

// 	BOOL  is_car_info_CleanWater_low();   // 清洗液状态  0:normal 1:low 
// 	WORD  get_car_info_EngineSpeed();     // 发动机转速 RPM
// 	WORD  get_car_info_InstantaneousVelocity();// 瞬时速度 km/h
// 	WORD  get_car_info_BattVtg();         // 电池电压 V
// 	INT   get_car_info_TempOutside();     // 车外温度 ℃
// 	WORD  get_car_info_Mileage();         // 行车里程 Km
// 	WORD  get_car_info_RemainL();         // 剩余油量 L
// 	BOOL  is_car_info_BattVtg_low();      // 电压警告标志  0:normal 1:low 


	BOOL  is_parking_info_Radar_beep();   // 雷达发声状态  0:off 1:on 
// 
// 	//方向盘按键信息
// 	BYTE  get_swc_key_Value();             // 0x00:没有按键按下或释放 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
// 	BYTE  get_swc_key_State();            // 按键状态 0:释放 1:按下 2:连续按键有效
// 
// 	//车速
// 	WORD  get_car_Speed();               // 车速 Km/h

};


/////////////////////////////////////韩国现代////////////////////////////////////
struct CANBUS_RAISE_HYUNDAI 
{
	BYTE outside_temp[1];           //控制命令0x01
	BYTE swc_key[1];				// 方向盘信息 0x02
	BYTE climatronic[4];           //空调信息0x03
	BYTE radar_all_info[2];        //全方位雷达信息 0x04

	BYTE ON_OFF_cmd[1];           //开关机命令0x04
	BYTE eqBalance_cmd[2];        // 0x07
	BYTE audioset_cmd[3];         //音效调节命令
	BYTE host_info[20];          //DVD主机状态信息0x09
	BYTE radio_info[4];           //收音机信息0x09
	BYTE media_info[6];			  //媒体信息0x09
	BYTE bt_info[2];			  //蓝牙状态信息0x09

	static CANBUS_RAISE_HYUNDAI* get_instance();

	//空调信息
	BOOL  is_climatronic_Switch();       // 空调开关指示
	BOOL  is_climatronic_AC();           // A/C指示
	BOOL  is_climatronic_Cycle();        // 内外循环指示 0:outer 1:inner
	BOOL  is_climatronic_Auto();      // 小风灯指示
	BOOL  is_climatronic_Dual();         // 双区控制
	BOOL  is_climatronic_FDemister();    // 前挡玻璃吹风(除雾)FDemister
	BOOL  is_climatronic_RDemister();    // 后挡玻璃吹风(除雾)FDemister
// 	BOOL  is_climatronic_Airdir_head();  // 向上送风开关指示
	BOOL  is_climatronic_Airdir_hands(); // 平行送风开关指示
	BOOL  is_climatronic_Airdir_feet();  // 向下送风开关指示

	BOOL  is_climatronic_REQ_show();     // 空调请求显示
	BYTE  get_climatronic_FanSpeed();    // 风量大小 0~7级
	FLOAT get_climatronic_LTemp();       // 驾驶位置处温度 0x00:LO 0x1F:HI 18℃~26℃
	FLOAT get_climatronic_RTemp();       // 副驾驶位置处温度
// 	BYTE  get_climatronic_tempunit();// 温度单位


	//雷达信息
	BYTE  get_radar_front_L();           // 前左雷达 范围 0x00~0x0A
	BYTE  get_radar_front_C();           // 前中雷达
	BYTE  get_radar_front_R();           // 前右雷达
	BYTE  get_radar_rear_L();            // 后左雷达
	BYTE  get_radar_rear_C();            // 后中雷达
	BYTE  get_radar_rear_R();            // 后右雷达


	//方向盘转角
// 	INT   get_swc_angle();               //方向盘角度  < 0 表示右转

	//车身信息

	INT   get_car_info_TempOutside();     // 车外温度 ℃


	// 
	// 	//方向盘按键信息
	// 	BYTE  get_swc_key_Value();             // 0x00:没有按键按下或释放 0x01:VOL+ 0x02:VOL- 0x03:MENU UP 0x04:MENU DOWN 0x05: TEL 0x06: MUTE 0x07:SRC
	// 	BYTE  get_swc_key_State();            // 按键状态 0:释放 1:按下 2:连续按键有效

};

// 尝试定义的数据类型兼容所有的can信息,如果兼容不了,再看怎么处理
typedef enum _CANBUS_DATA_TYPE_
{
	CBDT_SPEED,			 // 车速信息
	CBDT_SWC_KEY,		 // 方向盘信息
	CBDT_CLIMATRONIC,	 // 空调信息
	CBDT_REAR_RADAR,	 // 后雷达
	CBDT_FRONT_RADAR,	 // 前雷达
	CBDT_BASIC_INFO,	 // 基本信息
	CBDT_PARKING_INFO,   // 停车辅助状态
	CBDT_SWC_ANGLE,		 // 方向盘转角
	CBDT_CAR_INFO,		 // 车身信息
	                    
	CBDT_START_END,      // START/END
	CBDT_REQ_CTRL_INFO,  // 请求控制器信息
	CBDT_SOURCE,         // SOURCE
	CBDT_ICON,           // ICON
	CBDT_RADIO_INFO,     // 收音信息
	CBDT_MEDIA_PLAY_INFO,// 媒体播放信息
	CBDT_VOL_SHOW_CTRL,  // 音量显示控制
	CBDT_RADAR_VOL_CTRL, // 雷达音量控制

	CBDT_INVALID = 0xFF,
}CANBUS_DATA_TYPE;