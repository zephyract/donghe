#include "stdafx.h"
#include "CanBus.h"
#include "MainWnd.h"
#include "interface.h"

void OnCanBus_RaiseDasAuto(int nDataLen, BYTE* pdata)
{
	// 数据包合法校验
	if (nDataLen<=0 || !pdata || pdata[0] != 0x2E || (pdata[2]+4)!=nDataLen)
	{
		return;
	}

	BYTE check_sum = pdata[1] + pdata[2];
	for (int i=0; i<pdata[2]; i++)
	{
		check_sum += pdata[3+i];
	}

	check_sum ^= 0xFF;

	if (check_sum != pdata[3+pdata[2]])		// 确保检验位是正确的
	{
		RETAILMSG(1, (L"[navi car]: OnCanBus_RaiseDasAuto() ~ check sum error.\n"));
		return;
	}

	CANBUS_RAISE_DASAUTO* pcan = CANBUS_RAISE_DASAUTO::get_instance();
	CANBUS_DATA_TYPE cbdt = CBDT_INVALID;
	switch (pdata[1])
	{
	case 0x16:
		memcpy(pcan->speed, pdata+3, sizeof(pcan->speed));
		cbdt = CBDT_SPEED;
		break;
	case 0x20:
		memcpy(pcan->swc_key, pdata+3, sizeof(pcan->swc_key));
		cbdt = CBDT_SWC_KEY;
#ifdef  _DEBUG
		pcan->get_swc_key_Value();             // SRC
		pcan->get_swc_key_State();            // 按键状态 0:释放 1:按下 2:连续按键有效
#endif
		break;
	case 0x21:
		memcpy(pcan->climatronic, pdata+3,sizeof(pcan->climatronic));
		cbdt = CBDT_CLIMATRONIC;
		break;
	case 0x22:
		memcpy(pcan->rear_radar, pdata+3,sizeof(pcan->rear_radar));
		cbdt = CBDT_REAR_RADAR;
		break;
	case 0x23:
		memcpy(pcan->front_radar, pdata+3,sizeof(pcan->front_radar));
		cbdt = CBDT_FRONT_RADAR;
		break;
	case 0x24:
		memcpy(pcan->basic_info, pdata+3,sizeof(pcan->basic_info));
		cbdt = CBDT_BASIC_INFO;
		break;
	case 0x25:
		memcpy(pcan->parking_info, pdata+3,sizeof(pcan->parking_info));
		cbdt = CBDT_PARKING_INFO;
		break;
	case 0x26:
		memcpy(pcan->swc_angle, pdata+3,sizeof(pcan->swc_angle));
		cbdt = CBDT_SWC_ANGLE;
		break;
	case 0x41:
		if (pdata[3] == 1)	memcpy(pcan->car_info1, pdata+3,sizeof(pcan->car_info1));
		else if (pdata[3] == 2)	memcpy(pcan->car_info2, pdata+3,sizeof(pcan->car_info2));
		else if (pdata[3] == 3)	memcpy(pcan->car_info3, pdata+3,sizeof(pcan->car_info3));

		cbdt = CBDT_CAR_INFO;		
		break;
	case 0x81:
		memcpy(pcan->start_end, pdata+3,sizeof(pcan->start_end));
		cbdt = CBDT_START_END;
		break;
	case 0x90:
		memcpy(pcan->req_ctrl_info, pdata+3,sizeof(pcan->req_ctrl_info));
		cbdt = CBDT_REQ_CTRL_INFO;
		break;
	case 0xC0:
		memcpy(pcan->source, pdata+3,sizeof(pcan->source));
		cbdt = CBDT_SOURCE;
		break;
	case 0xC1:
		memcpy(pcan->icon, pdata+3,sizeof(pcan->icon));
		cbdt = CBDT_ICON;
		break;
	case 0xC2:
		memcpy(pcan->radio_info, pdata+3,sizeof(pcan->radio_info));
		cbdt = CBDT_RADIO_INFO;
		break;
	case 0xC3:
		memcpy(pcan->media_play_info, pdata+3,sizeof(pcan->media_play_info));
		cbdt = CBDT_MEDIA_PLAY_INFO;
		break;
	case 0xC4:
		memcpy(pcan->vol_show_ctrl, pdata+3,sizeof(pcan->vol_show_ctrl));
		cbdt = CBDT_VOL_SHOW_CTRL;
		break;
	case 0xC6:
		memcpy(pcan->radar_vol_ctrl, pdata+3,sizeof(pcan->radar_vol_ctrl));
		cbdt = CBDT_RADAR_VOL_CTRL;
		break;
	}

	if (cbdt != CBDT_INVALID)
	{
		CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_CANBUS_DATA, 0, (LPARAM)cbdt);
	}
}

void OnCanBus_RaiseFocus(int nDataLen, BYTE* pdata)
{
	// 数据包合法校验
	if (nDataLen<=0 || !pdata || pdata[0] != 0x2E || (pdata[2]+4)!=nDataLen)
	{
		return;
	}

	BYTE check_sum = pdata[1] + pdata[2];
	for (int i=0; i<pdata[2]; i++)
	{
		check_sum += pdata[3+i];
	}

	check_sum ^= 0xFF;

	if (check_sum != pdata[3+pdata[2]])		// 确保检验位是正确的
	{
		RETAILMSG(1, (L"[navi car]: OnCanBus_RaiseDasAuto() ~ check sum error.\n"));
		return;
	}

	CANBUS_RAISE_FOCUS* pcan = CANBUS_RAISE_FOCUS::get_instance();
	CANBUS_DATA_TYPE cbdt = CBDT_INVALID;
	switch (pdata[1])
	{
	case 0x01:
		memcpy(pcan->key_cmd, pdata+3, sizeof(pcan->key_cmd));
		break;
	case 0x14:
		memcpy(pcan->backlight, pdata+3, sizeof(pcan->backlight));
// 		cbdt = CBDT_SPEED;
		break;
	case 0x20:
		memcpy(pcan->swc_key, pdata+3, sizeof(pcan->swc_key));
		cbdt = CBDT_SWC_KEY;
#ifdef  _DEBUG
		pcan->get_swc_key_Value();             // SRC
		pcan->get_swc_key_State();            // 按键状态 0:释放 1:按下 2:连续按键有效
#endif
		break;
	case 0x21:
		memcpy(pcan->climatronic, pdata+3,sizeof(pcan->climatronic));
		cbdt = CBDT_CLIMATRONIC;
		break;
	case 0x22:
		memcpy(pcan->rear_radar, pdata+3,sizeof(pcan->rear_radar));
		cbdt = CBDT_REAR_RADAR;
		break;
	case 0x23:
		memcpy(pcan->front_radar, pdata+3,sizeof(pcan->front_radar));
		cbdt = CBDT_FRONT_RADAR;
		break;
	case 0x24:
		memcpy(pcan->basic_info, pdata+3,sizeof(pcan->basic_info));
		cbdt = CBDT_BASIC_INFO;
		break;
	case 0x25:
		memcpy(pcan->parking_info, pdata+3,sizeof(pcan->parking_info));
		cbdt = CBDT_PARKING_INFO;
		break;
	case 0x26:
		memcpy(pcan->datetime_info, pdata+3,sizeof(pcan->datetime_info));
		cbdt = CBDT_PARKING_INFO;
		break;
	case 0x30:
		memcpy(pcan->car_version, pdata+3,sizeof(pcan->car_version));
		cbdt = CBDT_SWC_ANGLE;
		break;
	case 0x70:
		memcpy(pcan->sync_srt_up, pdata+3,sizeof(pcan->sync_srt_up));
		cbdt = CBDT_CAR_INFO;
		break;
	case 0x71:
		memcpy(pcan->sync_srt_down, pdata+3,sizeof(pcan->sync_srt_down));
		cbdt = CBDT_CAR_INFO;
		break;
	case 0x72:
		memcpy(pcan->sync_srt_short, pdata+3,sizeof(pcan->sync_srt_short));
		cbdt = CBDT_CAR_INFO;
		break;
	case 0x78:
		memcpy(pcan->sync_srt_status, pdata+3,sizeof(pcan->sync_srt_status));
		cbdt = CBDT_CAR_INFO;
		break;
	case 0x79:
		memcpy(pcan->audio_change, pdata+3,sizeof(pcan->audio_change));
		cbdt = CBDT_REQ_CTRL_INFO;
		break;
	case 0x81:
		memcpy(pcan->start_end, pdata+3,sizeof(pcan->start_end));
		cbdt = CBDT_START_END;
		break;
	case 0x27:
		memcpy(pcan->car_langue_set, pdata+3,sizeof(pcan->car_langue_set));
		cbdt = CBDT_SOURCE;
		break;
	case 0x40:
		memcpy(pcan->sync_version, pdata+3,sizeof(pcan->sync_version));
		cbdt = CBDT_ICON;
		break;
	case 0x50:
		memcpy(pcan->sync_V3_menu, pdata+3,sizeof(pcan->sync_V3_menu));
		cbdt = CBDT_RADIO_INFO;
		break;
	case 0x51:
		memcpy(pcan->sync_V3_menuitem, pdata+3,sizeof(pcan->sync_V3_menuitem));
		cbdt = CBDT_MEDIA_PLAY_INFO;
		break;
	case 0x52:
		memcpy(pcan->sync_media_tracks_tm, pdata+3,sizeof(pcan->sync_media_tracks_tm));
		break;
	case 0x53:
		memcpy(pcan->sync_talk_tm, pdata+3,sizeof(pcan->sync_talk_tm));
		break;

	case 0xC6:
		memcpy(pcan->ctrl_cmd, pdata+3,sizeof(pcan->ctrl_cmd));
		break;
	case 0x82:
		memcpy(pcan->car_datetime_set, pdata+3,sizeof(pcan->car_datetime_set));
		break;
	case 0x90:
		memcpy(pcan->req_decode_status, pdata+3,sizeof(pcan->req_decode_status));
		break;
	}

	if (cbdt != CBDT_INVALID)
	{
		CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_CANBUS_DATA, 0, (LPARAM)cbdt);

	}
}

/////////////////////////////////HONDA///////////////////////////////////////////
void OnCanBus_RaiseHonda(int nDataLen, BYTE* pdata)
{
	// 数据包合法校验
	if (nDataLen<=0 || !pdata || pdata[0] != 0x2E || (pdata[2]+4)!=nDataLen)
	{
		return;
	}

	BYTE check_sum = pdata[1] + pdata[2];
	for (int i=0; i<pdata[2]; i++)
	{
		check_sum += pdata[3+i];
	}

	check_sum ^= 0xFF;

	if (check_sum != pdata[3+pdata[2]])		// 确保检验位是正确的
	{
		RETAILMSG(1, (L"[navi car]: OnCanBus_RaiseDasAuto() ~ check sum error.\n"));
		return;
	}

	CANBUS_RAISE_HONDA* pcan = CANBUS_RAISE_HONDA::get_instance();
	CANBUS_DATA_TYPE cbdt = CBDT_INVALID;
	switch (pdata[1])
	{
	case 0x14:
		memcpy(pcan->backlight, pdata+3, sizeof(pcan->backlight));
		cbdt = CBDT_SPEED;
		break;
	case 0x20:
		memcpy(pcan->swc_key, pdata+3, sizeof(pcan->swc_key));
		cbdt = CBDT_SWC_KEY;
#ifdef  _DEBUG
		pcan->get_swc_key_Value();             // SRC
		pcan->get_swc_key_State();            // 按键状态 0:释放 1:按下 2:连续按键有效
#endif
		break;
	case 0x21:
		memcpy(pcan->climatronic, pdata+3,sizeof(pcan->climatronic));
		cbdt = CBDT_CLIMATRONIC;
		break;
	case 0x22:
		memcpy(pcan->rear_radar, pdata+3,sizeof(pcan->rear_radar));
		cbdt = CBDT_REAR_RADAR;
		break;
// 	case 0x23:
// 		memcpy(pcan->front_radar, pdata+3,sizeof(pcan->front_radar));
// 		cbdt = CBDT_FRONT_RADAR;
// 		break;
	case 0x24:
		memcpy(pcan->basic_info, pdata+3,sizeof(pcan->basic_info));
		cbdt = CBDT_BASIC_INFO;
		break;
	case 0x25:
		memcpy(pcan->parking_info, pdata+3,sizeof(pcan->parking_info));
		cbdt = CBDT_PARKING_INFO;
		break;
	case 0x29:
		memcpy(pcan->swc_angle, pdata+3,sizeof(pcan->swc_angle));
		cbdt = CBDT_SWC_ANGLE;
		break;
	case 0x30:
		memcpy(pcan->version_info, pdata+3,sizeof(pcan->version_info));
		cbdt = CBDT_CAR_INFO;
		break;
	case 0x32:
		memcpy(pcan->car_info, pdata+3,sizeof(pcan->car_info));
		cbdt = CBDT_CAR_INFO;		
		break;
	case 0x33:
		memcpy(pcan->car_Mileage, pdata+3,sizeof(pcan->car_Mileage));
		cbdt = CBDT_CAR_INFO;
		break;
	case 0xD0:
		memcpy(pcan->car_set_ack, pdata+3,sizeof(pcan->car_set_ack));
		cbdt = CBDT_CAR_INFO;
		break;

	case 0x81:
		memcpy(pcan->start_end, pdata+3,sizeof(pcan->start_end));
		cbdt = CBDT_START_END;
		break;
	case 0x90:
		memcpy(pcan->query_cmd, pdata+3,sizeof(pcan->query_cmd));
		cbdt = CBDT_REQ_CTRL_INFO;
		break;
	case 0xC6:
		memcpy(pcan->set_cmd, pdata+3,sizeof(pcan->set_cmd));
		cbdt = CBDT_SOURCE;
		break;
	}

	if (cbdt != CBDT_INVALID)
	{
		CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_CANBUS_DATA, 0, (LPARAM)cbdt);

	}
}
/////////////////////////CAMRY///////////////////////////////
void OnCanBus_RaiseCamry(int nDataLen, BYTE* pdata)
{
	// 数据包合法校验
	if (nDataLen<=0 || !pdata || pdata[0] != 0x2E || (pdata[2]+4)!=nDataLen)
	{
		return;
	}

	BYTE check_sum = pdata[1] + pdata[2];
	for (int i=0; i<pdata[2]; i++)
	{
		check_sum += pdata[3+i];
	}

	check_sum ^= 0xFF;

	if (check_sum != pdata[3+pdata[2]])		// 确保检验位是正确的
	{
		RETAILMSG(1, (L"[navi car]: OnCanBus_RaiseDasAuto() ~ check sum error.\n"));
		return;
	}

	CANBUS_RAISE_CAMRY* pcan = CANBUS_RAISE_CAMRY::get_instance();
	CANBUS_DATA_TYPE cbdt = CBDT_INVALID;
	switch (pdata[1])
	{
	case 0x20:
		memcpy(pcan->swc_key, pdata+3, sizeof(pcan->swc_key));
		cbdt = CBDT_SWC_KEY;
#ifdef  _DEBUG
		pcan->get_swc_key_Value();             // SRC
		pcan->get_swc_key_State();            // 按键状态 0:释放 1:按下 2:连续按键有效
#endif
		break;
	case 0x21:
		memcpy(pcan->per_min_oil, pdata+3,sizeof(pcan->per_min_oil));
		break;
	case 0x22:
		memcpy(pcan->instant_oil, pdata+3,sizeof(pcan->instant_oil));
		break;
	case 0x23:
		memcpy(pcan->history_oil, pdata+3,sizeof(pcan->history_oil));
		cbdt = CBDT_FRONT_RADAR;
		break;
	case 0x24:
		memcpy(pcan->basic_info, pdata+3,sizeof(pcan->basic_info));
		cbdt = CBDT_BASIC_INFO;
		break;
	case 0x25:
		memcpy(pcan->TPMS, pdata+3,sizeof(pcan->TPMS));
		cbdt = CBDT_PARKING_INFO;
		break;
	case 0x26:
		memcpy(pcan->vehicle_setting, pdata+3,sizeof(pcan->vehicle_setting));
		cbdt = CBDT_PARKING_INFO;
		break;
	case 0x27:
		memcpy(pcan->per_15min_oil, pdata+3,sizeof(pcan->per_15min_oil));
		cbdt = CBDT_CLIMATRONIC;
		break;
	case 0x28:
		memcpy(pcan->climatronic, pdata+3,sizeof(pcan->climatronic));
		cbdt = CBDT_PARKING_INFO;
		break;
	case 0x29:
		memcpy(pcan->swc_angle, pdata+3,sizeof(pcan->swc_angle));
		cbdt = CBDT_PARKING_INFO;
		break;
	case 0x30:
		memcpy(pcan->car_version, pdata+3,sizeof(pcan->car_version));
		cbdt = CBDT_SWC_ANGLE;
		break;
	case 0x31:
		memcpy(pcan->amplifier_info, pdata+3,sizeof(pcan->amplifier_info));
		cbdt = CBDT_CAR_INFO;
		break;
	case 0x32:
		memcpy(pcan->sys_info, pdata+3,sizeof(pcan->sys_info));
		cbdt = CBDT_CAR_INFO;
		break;
	case 0x50:
		memcpy(pcan->car_set_status, pdata+3,sizeof(pcan->car_set_status));
		cbdt = CBDT_CAR_INFO;
		break;
	case 0x1D:
		memcpy(pcan->front_radar, pdata+3,sizeof(pcan->front_radar));
		cbdt = CBDT_CAR_INFO;
		break;
	case 0x1E:
		memcpy(pcan->rear_radar, pdata+3,sizeof(pcan->rear_radar));
		cbdt = CBDT_CAR_INFO;
		break;
	case 0x1F:
		memcpy(pcan->hybrid_electric, pdata+3,sizeof(pcan->hybrid_electric));
		cbdt = CBDT_CAR_INFO;
		break;
	case 0x81:
		memcpy(pcan->start_end, pdata+3,sizeof(pcan->start_end));
		cbdt = CBDT_START_END;
		break;
	case 0x83:
		memcpy(pcan->setting_cmd, pdata+3,sizeof(pcan->setting_cmd));
		cbdt = CBDT_SOURCE;
		break;
	case 0x84:
		memcpy(pcan->setting_cmd_baodao, pdata+3,sizeof(pcan->setting_cmd_baodao));
		break;
	case 0x90:
		memcpy(pcan->ctrl_cmd, pdata+3,sizeof(pcan->ctrl_cmd));
		break;
	}

	if (cbdt != CBDT_INVALID)
	{
		CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_CANBUS_DATA, 0, (LPARAM)cbdt);
	}
}

//////////////////////////////// 标致雪铁龙//////////////////////////////////////////
void OnCanBus_RaisePeugeot(int nDataLen, BYTE* pdata)
{
	// 数据包合法校验 pdata[1]数据长度
	if (nDataLen<=0 || !pdata || pdata[0] != 0xfd || (pdata[1]+1)!=nDataLen)
	{
		return;
	}

	int check_temp = 0;
	for (int i=1; i<pdata[1]; i++)
	{
		check_temp += pdata[i];
	}
	BYTE check_sum = (check_temp>0xFF)? (check_temp&0xFF):check_temp;

// 	check_sum ^= 0xFF;

	if (check_sum != pdata[pdata[1]])		// 确保检验位是正确的
	{
		RETAILMSG(1, (L"[navi car]: OnCanBus_RaiseDasAuto() ~ check sum error.\n"));
		return;
	}

	CANBUS_RAISE_PEUGEOT* pcan = CANBUS_RAISE_PEUGEOT::get_instance();
	CANBUS_DATA_TYPE cbdt = CBDT_INVALID;
	switch (pdata[1])
	{
	case 0x01:
		memcpy(pcan->control_cmd, pdata+3, sizeof(pcan->control_cmd));
		break;
	case 0x02:
		memcpy(pcan->key_cmd,pdata+3,sizeof(pcan->key_cmd));
		break;
	case 0x21:
		memcpy(pcan->climatronic,pdata+3,sizeof(pcan->climatronic));
		cbdt = CBDT_CLIMATRONIC;
		break;
	case 0x32:
		memcpy(pcan->rear_radar_info,pdata+3,sizeof(pcan->rear_radar_info));
		break;
	case 0x30:
		memcpy(pcan->radar_all_info,pdata+3,sizeof(pcan->radar_all_info));
		break;
	case 0x33:
		memcpy(pcan->ecu_info0,pdata+3,sizeof(pcan->ecu_info0));
		break;
	case 0x34:
		memcpy(pcan->ecu_info1,pdata+3,sizeof(pcan->ecu_info1));
		break;
	case 0x35:
		memcpy(pcan->ecu_info2,pdata+3,sizeof(pcan->ecu_info2));
		break;
	case 0x36:
		memcpy(pcan->outside_temp,pdata+3,sizeof(pcan->outside_temp));
		break;
	case 0x37:
		memcpy(pcan->car_alarm_info,pdata+3,sizeof(pcan->car_alarm_info));
		break;
	case 0x38:
		memcpy(pcan->car_info,pdata+3,sizeof(pcan->car_info));
		break;
	case 0x39:
		memcpy(pcan->car_action_info,pdata+3,sizeof(pcan->car_action_info));
		break;
	case 0x3a:
		memcpy(pcan->diagnose_info,pdata+3,sizeof(pcan->diagnose_info));
		break;
	case 0x3b:
		memcpy(pcan->memorize_speed,pdata+3,sizeof(pcan->memorize_speed));
		break;
#ifdef PEUGEOT_408//标致408专用
	case 0x3d:
		memcpy(pcan->cruise_speed,pdata+3,sizeof(pcan->cruise_speed));
		break;
#else
	case 0x3d:
		memcpy(pcan->clear_ecu_data,pdata+3,sizeof(pcan->clear_ecu_data));
		break;
#endif
	case 0x3E:
		memcpy(pcan->radarvol_change_flag,pdata+3,sizeof(pcan->radarvol_change_flag));
		break;

	case 0x80:
		memcpy(pcan->car_param_set,pdata+3,sizeof(pcan->car_param_set));
		break;
	case 0x82:
		memcpy(pcan->ecu_param_set,pdata+3,sizeof(pcan->ecu_param_set));
		break;
	case 0x85:
		memcpy(pcan->req_alarm_record_info,pdata+3,sizeof(pcan->req_alarm_record_info));
		break;
	case 0x86:
		memcpy(pcan->req_action_state_info,pdata+3,sizeof(pcan->req_action_state_info));
		break;
	case 0x87:
		memcpy(pcan->req_diagnose_info,pdata+3,sizeof(pcan->req_diagnose_info));
		break;
	case 0x88:
		memcpy(pcan->memorize_speed_set,pdata+3,sizeof(pcan->memorize_speed_set));
		break;
	case 0x89:
		memcpy(pcan->cruise_speed_set,pdata+3,sizeof(pcan->cruise_speed_set));
		break;
	case 0x8a:
		memcpy(pcan->climatronic_set,pdata+3,sizeof(pcan->climatronic_set));
		break;
	case 0x8f:
		memcpy(pcan->req_showinfo_cmd,pdata+3,sizeof(pcan->req_showinfo_cmd));
		break;
	}
		if (cbdt != CBDT_INVALID)
		{
			CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_CANBUS_DATA, 0, (LPARAM)cbdt);
		}
}


//////////////////////////////// 标致雪铁龙//////////////////////////////////////////
void OnCanBus_RaiseHyundai(int nDataLen, BYTE* pdata)
{
	// 数据包合法校验 pdata[1]数据长度
	if (nDataLen<=0 || !pdata || pdata[0] != 0xfd || (pdata[1]+1)!=nDataLen)
	{
		return;
	}

	//校验和两个字节
	int check_temp = 0;
	for (int i=1; i<pdata[1]-1; i++)
	{
		check_temp += pdata[i];
	}

	if ( ((check_temp & 0xFF) != pdata[pdata[1]]) || ((check_temp & 0xFF00) != pdata[pdata[1]-1]) )		// 确保检验位是正确的
	{
		RETAILMSG(1, (L"[navi car]: OnCanBus_RaiseDasAuto() ~ check sum error.\n"));
		return;
	}

	CANBUS_RAISE_HYUNDAI* pcan = CANBUS_RAISE_HYUNDAI::get_instance();
	CANBUS_DATA_TYPE cbdt = CBDT_INVALID;
	switch (pdata[1])
	{
	case 0x01:
		memcpy(pcan->outside_temp, pdata+3, sizeof(pcan->outside_temp));
		break;
	case 0x02:
		memcpy(pcan->swc_key,pdata+3,sizeof(pcan->swc_key));
		break;
	case 0x03:
		memcpy(pcan->climatronic,pdata+3,sizeof(pcan->climatronic));
		cbdt = CBDT_CLIMATRONIC;
		break;
	case 0x04:
		memcpy(pcan->radar_all_info,pdata+3,sizeof(pcan->radar_all_info));
		break;
// 	case 0x36:
// 		memcpy(pcan->outside_temp,pdata+3,sizeof(pcan->outside_temp));
// 		break;
// 	case 0x37:
// 		memcpy(pcan->car_alarm_info,pdata+3,sizeof(pcan->car_alarm_info));
// 		break;
// 	case 0x38:
// 		memcpy(pcan->car_info,pdata+3,sizeof(pcan->car_info));
// 		break;
	}
	if (cbdt != CBDT_INVALID)
	{
		CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_CANBUS_DATA, 0, (LPARAM)cbdt);
	}
}

//////////////////////////// CANBUS_RAISE_DASAUTO /////////////////////////////////


CANBUS_RAISE_DASAUTO*  CANBUS_RAISE_DASAUTO::get_instance()
{
	static CANBUS_RAISE_DASAUTO dasauto;
	return &dasauto;
}

BOOL CANBUS_RAISE_DASAUTO::is_climatronic_Switch()
{
   return climatronic[0] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_climatronic_AC()
{
	return climatronic[0] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_climatronic_Cycle()
{
	return climatronic[0] >> 5 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_climatronic_HighWind()
{
	return climatronic[0] >> 4 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_climatronic_SmallWind()
{
	return climatronic[0] >> 3 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_climatronic_Dual()
{
	return climatronic[0] >> 2& 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_climatronic_FDemister()
{
	return climatronic[0] >> 1 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_climatronic_RDemister()
{
	return climatronic[0] & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_climatronic_Airdir_head()
{
	return climatronic[1] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_climatronic_Airdir_hands()
{
	return climatronic[1] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_climatronic_Airdir_feet()
{
	return climatronic[1] >> 5 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_climatronic_REQ_show()
{
	return climatronic[1] >> 4 & 0x01;
}

BYTE  CANBUS_RAISE_DASAUTO::get_climatronic_FanSpeed()
{
	return (climatronic[1] & 0x0F);
}

FLOAT CANBUS_RAISE_DASAUTO::get_climatronic_LTemp()
{
	// 驾驶位置处温度 0x00:LO 0x1F:HI 0x01-0x11:18℃ ~ 26℃,步进0.5℃
	FLOAT fValue;
	BYTE nValue = climatronic[2];
 
	if (nValue >= 0x01 && nValue <= 0x11)
	{
        fValue = 18 + 0.5f*(nValue-0x01);           
	}
	else
	{
        fValue = nValue;
	}
	//RETAILMSG( 1,(L"[CANBUS_RAISE_DASAUTO]..LTemp .......................... = %f \r\n",fValue ) );
	return  fValue;
}

FLOAT CANBUS_RAISE_DASAUTO::get_climatronic_RTemp()
{
	FLOAT fValue;
	BYTE nValue = climatronic[3];

	if (nValue >= 0x01 && nValue <= 0x11)
	{
		fValue = 18 + 0.5f*(nValue-0x01);  
	}
	else
	{
		fValue = nValue;
	}
	return  fValue;
}

BOOL CANBUS_RAISE_DASAUTO::get_climatronic_AQS()
{
	return climatronic[4]  >> 7 & 0x01;
}

BYTE CANBUS_RAISE_DASAUTO::get_climatronic_LHotSeat()
{
	return (climatronic[4] >> 4 & 0x03);
}

BYTE CANBUS_RAISE_DASAUTO::get_climatronic_RHotSeat()
{
	return climatronic[4] & 0x03;
}


//雷达信息
BYTE CANBUS_RAISE_DASAUTO::get_radar_front_L()
{
	return front_radar[0];
}

BYTE CANBUS_RAISE_DASAUTO::get_radar_front_L_C()
{
	return front_radar[1];
}

BYTE CANBUS_RAISE_DASAUTO::get_radar_front_R_C()
{
	return front_radar[2];
}

BYTE CANBUS_RAISE_DASAUTO::get_radar_front_R()
{
	return front_radar[3];
}

BYTE CANBUS_RAISE_DASAUTO::get_radar_rear_L()
{
	return rear_radar[0];
}

BYTE CANBUS_RAISE_DASAUTO::get_radar_rear_L_C()
{
	return rear_radar[1];
}

BYTE CANBUS_RAISE_DASAUTO::get_radar_rear_R_C()
{
	return rear_radar[2];
}

BYTE CANBUS_RAISE_DASAUTO::get_radar_rear_R()
{
	return rear_radar[3];
}

//方向盘转角
INT CANBUS_RAISE_DASAUTO::get_swc_angle()
{
	BYTE ESP1 = swc_angle[0];
	BYTE ESP2 = swc_angle[1];
	WORD ESP = MAKEWORD(ESP1,ESP2);
	BOOL bFlag = ESP2 & 0x80;
	INT nAngle = 0;
	if (bFlag)
	{
		ESP = ~ (ESP - 1);
		nAngle = -ESP;
	}
	else
	{
		nAngle = ESP;
	}
	return nAngle;
}

//车身信息
BOOL CANBUS_RAISE_DASAUTO::is_car_info_SeatBelt_off()
{
	return car_info1[1] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_car_info_CleanWater_low()
{
	return car_info1[1] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_car_info_Brake_hung()
{
	return car_info1[1] >> 5 & 0x01;
}


BOOL CANBUS_RAISE_DASAUTO::is_car_info_Trunk_Open()
{
	return car_info1[1] >> 4 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_car_info_Door_RR()
{
	return car_info1[1] >> 3 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_car_info_Door_RL()
{
	return car_info1[1] >> 2 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_car_info_Door_FR()
{
	return car_info1[1] >> 1 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_car_info_Door_FL()
{
	return car_info1[1] & 0x01;
}

WORD CANBUS_RAISE_DASAUTO::get_car_info_EngineSpeed()
{
	return car_info2[1] * 256 + car_info2[2];
}

WORD CANBUS_RAISE_DASAUTO::get_car_info_InstantaneousVelocity()
{
	return (car_info2[3] * 256 + car_info2[4]) * 0.01;
}

WORD CANBUS_RAISE_DASAUTO::get_car_info_BattVtg()
{
	return (car_info2[5] * 256 + car_info2[6]) * 0.01;
}

INT CANBUS_RAISE_DASAUTO::get_car_info_TempOutside()
{
	INT nValue;
	WORD wdValue = car_info2[7] * 256 + car_info2[8];
	BOOL bFlag = wdValue & 0x8000;
	if (bFlag)
	{
		wdValue = ~ (wdValue - 1);
		nValue = - wdValue * 0.1;
	} 
	else
	{
		nValue = wdValue * 0.1;
	}	
	return  nValue;
}


WORD CANBUS_RAISE_DASAUTO::get_car_info_Mileage()
{
	return car_info2[9] * 65536 +car_info2[10] *256 + car_info2[11];
}

WORD CANBUS_RAISE_DASAUTO::get_car_info_RemainL()
{
	return car_info2[12];
}

BOOL CANBUS_RAISE_DASAUTO::is_car_info_RemainL_low()
{
	return car_info3[1] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_car_info_BattVtg_low()
{
	return car_info3[1] >> 6 & 0x01;
}

// 停车辅助状态
BOOL CANBUS_RAISE_DASAUTO::is_parking_info_Switch()
{
	return parking_info[0] >> 1 & 0x01;
}

BOOL CANBUS_RAISE_DASAUTO::is_parking_info_Radar_beep()
{
	return parking_info[0] & 0x01;
}

//方向盘按键信息

BYTE CANBUS_RAISE_DASAUTO::get_swc_key_Value()
{
	return swc_key[0];
}

BYTE CANBUS_RAISE_DASAUTO::get_swc_key_State()
{
	return swc_key[1];
}

//车速
WORD  CANBUS_RAISE_DASAUTO::get_car_Speed()
{
	return MAKEWORD(speed[0], speed[1]);
}

static void _raise_dasauto_assemble_pack(IN BYTE* data, IN UINT data_len, OUT BYTE* send_data, OUT UINT* psend_data_len)
{
	if (!data || !send_data || !psend_data_len)
	{
		return;
	}

	send_data[0] = 0x2E;
	memcpy(send_data+1, data, data_len);

	BYTE check_sum = 0;
	for (int i=0; i<data_len; i++)
	{
		check_sum += data[i];
	}
	check_sum ^= 0xFF;

	send_data[data_len+1] = check_sum;
	*psend_data_len = data_len + 2;
}

void CANBUS_RAISE_DASAUTO::request_can_data(BYTE type, BYTE info_id)
{
	if (type != 0x16 && type != 0x21 && type != 0x24 && type != 0x25 && type != 0x41)
	{
		RETAILMSG(1, (L"[navi_car]: request_can_data() paramters error type=%x.\n", type));
	}

	//datatype, length, data0(请求内容), data1(车身信息中命令)
	BYTE data[4] = {0x90, 1, 0x16, 0x00};
	BYTE send_data[16];
	UINT send_len;

	if (type == 0x41)
	{
		data[1] = 2;
		data[2] = type;
		data[3] = info_id;
		_raise_dasauto_assemble_pack(data, 4, send_data, &send_len);
		CRpcMCU::GetInstance()->RPC_SetCanbusData(send_data, send_len);
	}
	else if (type == 0x16 || type == 0x21 || type == 0x24 || type == 0x25)
	{
		data[1] = 1;
		data[2] = type;
		_raise_dasauto_assemble_pack(data, 3, send_data, &send_len);
		CRpcMCU::GetInstance()->RPC_SetCanbusData(send_data, send_len);
	}
	else
	{
		RETAILMSG(1, (L"[navi_car]: request_can_data() paramters error type=%x.\n", type));
	}

// 	_raise_dasauto_assemble_pack(data, 3, send_data, &send_len);
// 	CRpcMCU::GetInstance()->RPC_SetCanbusData(send_data, send_len);
// 	data[2] = 0x21;
// 	_raise_dasauto_assemble_pack(data, 3, send_data, &send_len);
// 	CRpcMCU::GetInstance()->RPC_SetCanbusData(send_data, send_len);
// 	data[2] = 0x24;
// 	_raise_dasauto_assemble_pack(data, 3, send_data, &send_len);
// 	CRpcMCU::GetInstance()->RPC_SetCanbusData(send_data, send_len);
// 	data[2] = 0x25;
// 	_raise_dasauto_assemble_pack(data, 3, send_data, &send_len);
// 	CRpcMCU::GetInstance()->RPC_SetCanbusData(send_data, send_len);
// 
// 	// 车身信息 0x41
// 	data[1] = 2;
// 	data[2] = 0x41;
// 	data[3] = 01;
// 	_raise_dasauto_assemble_pack(data, 4, send_data, &send_len);
// 	CRpcMCU::GetInstance()->RPC_SetCanbusData(send_data, send_len);
// 	data[3] = 02;
// 	_raise_dasauto_assemble_pack(data, 4, send_data, &send_len);
// 	CRpcMCU::GetInstance()->RPC_SetCanbusData(send_data, send_len);
// 	data[3] = 03;
// 	_raise_dasauto_assemble_pack(data, 4, send_data, &send_len);
// 	CRpcMCU::GetInstance()->RPC_SetCanbusData(send_data, send_len);
}


//////////////////////////// CANBUS_RAISE_FOUCS /////////////////////////////////

CANBUS_RAISE_FOCUS*  CANBUS_RAISE_FOCUS::get_instance()
{
	static CANBUS_RAISE_FOCUS dasauto;
	return &dasauto;
}

//方向盘按键信息

BYTE CANBUS_RAISE_FOCUS::get_swc_key_Value()
{
	return swc_key[0];
}

BYTE CANBUS_RAISE_FOCUS::get_swc_key_State()
{
	return swc_key[1];
}

BOOL CANBUS_RAISE_FOCUS::is_climatronic_Switch()
{
	return climatronic[0] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_FOCUS::is_climatronic_AC()
{
	return climatronic[0] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_FOCUS::is_climatronic_Cycle()
{
	return climatronic[0] >> 5 & 0x01;
}


BOOL CANBUS_RAISE_FOCUS::is_climatronic_SmallWind()
{
	return climatronic[0] >> 3 & 0x01;
}

BOOL CANBUS_RAISE_FOCUS::is_climatronic_Dual()
{
	return climatronic[0] >> 2 & 0x01;
}

BOOL CANBUS_RAISE_FOCUS::is_climatronic_MaxFornt()
{
	return climatronic[0] >> 1 & 0x01;
}

// BOOL CANBUS_RAISE_FOCUS::is_climatronic_FDemister()
// {
// 	return climatronic[0] >> 1 & 0x01;
// }
// 
// BOOL CANBUS_RAISE_DASAUTO::is_climatronic_RDemister()
// {
// 	return climatronic[0] & 0x01;
// }

BOOL CANBUS_RAISE_FOCUS::is_climatronic_Airdir_head()
{
	return climatronic[1] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_FOCUS::is_climatronic_Airdir_hands()
{
	return climatronic[1] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_FOCUS::is_climatronic_Airdir_feet()
{
	return climatronic[1] >> 5 & 0x01;
}

BOOL CANBUS_RAISE_FOCUS::is_climatronic_REQ_show()
{
	BYTE bb = climatronic[1] >> 4 & 0x01;
	return climatronic[1] >> 4 & 0x01;
}

BYTE  CANBUS_RAISE_FOCUS::get_climatronic_FanSpeed()
{
	return (climatronic[1] & 0x0F);
}

FLOAT CANBUS_RAISE_FOCUS::get_climatronic_LTemp()
{
	// 驾驶位置处温度 0x00:LO 0x7F:HI 0x1F-0x3B:15.5℃ ~ 29.5℃,步进0.5℃
	FLOAT fValue;
	BYTE nValue = climatronic[2];

	if (nValue >= 0x1F && nValue <= 0x3B)
	{
		fValue = 15.5 + 0.5f*(nValue-0x1F);
		if (get_climatronic_tempunit())
		{
			fValue = (fValue *9/5) + 32.0;
		}

	}
	else
	{
		fValue = nValue;
	}
	//RETAILMSG( 1,(L"[CANBUS_RAISE_DASAUTO]..LTemp .......................... = %f \r\n",fValue ) );
	return  fValue;
}

FLOAT CANBUS_RAISE_FOCUS::get_climatronic_RTemp()
{
	FLOAT fValue;
	BYTE nValue = climatronic[3];

	if (nValue >= 0x1F && nValue <= 0x3B)
	{
		fValue = 15.5 + 0.5f*(nValue-0x1F);
		if (get_climatronic_tempunit())
		{
			fValue = (fValue *9/5) + 32.0;
		}
	}
	else
	{
		fValue = nValue;
	}
	return  fValue;
}

BOOL  CANBUS_RAISE_FOCUS::is_climatronic_AC_max()// AC_max
{
	return climatronic[4] >> 2 & 0x01;
}

BYTE  CANBUS_RAISE_FOCUS::get_climatronic_tempunit()// AC_max
{
	return climatronic[4] >> 6 & 0x01;
}


//雷达信息
BYTE CANBUS_RAISE_FOCUS::get_radar_front_L()
{
	return front_radar[0];
}

BYTE CANBUS_RAISE_FOCUS::get_radar_front_L_C()
{
	return front_radar[1];
}

BYTE CANBUS_RAISE_FOCUS::get_radar_front_R_C()
{
	return front_radar[2];
}

BYTE CANBUS_RAISE_FOCUS::get_radar_front_R()
{
	return front_radar[3];
}

BYTE CANBUS_RAISE_FOCUS::get_radar_rear_L()
{
	return rear_radar[0];
}

BYTE CANBUS_RAISE_FOCUS::get_radar_rear_L_C()
{
	return rear_radar[1];
}

BYTE CANBUS_RAISE_FOCUS::get_radar_rear_R_C()
{
	return rear_radar[2];
}

BYTE CANBUS_RAISE_FOCUS::get_radar_rear_R()
{
	return rear_radar[3];
}


//车身信息
BOOL CANBUS_RAISE_FOCUS::is_car_info_SeatBelt_off()
{
	return basic_info[1] >> 7 & 0x01; 
}

// BOOL CANBUS_RAISE_FOCUS::is_car_info_CleanWater_low()
// {
// 	static BOOL bValue;
// 	if (car_info[0] != 0x01) return bValue;
// 	bValue = car_info[1] >> 6 & 0x01;
// 	return  bValue;
// }

BOOL CANBUS_RAISE_FOCUS::is_car_info_Brake_hung()
{
	return basic_info[1] >> 3 & 0x01; 
}


BOOL CANBUS_RAISE_FOCUS::is_car_info_Trunk_Open()
{
	return basic_info[0] >> 3 & 0x01;
}

BOOL CANBUS_RAISE_FOCUS::is_car_info_Door_RR()
{
	return basic_info[0] >> 5 & 0x01;
}

BOOL CANBUS_RAISE_FOCUS::is_car_info_Door_RL()
{
	return basic_info[0] >> 4 & 0x01;
}

BOOL CANBUS_RAISE_FOCUS::is_car_info_Door_FR()
{
	return basic_info[0] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_FOCUS::is_car_info_Door_FL()
{
	return basic_info[0] >> 6 & 0x01;
}

// WORD CANBUS_RAISE_FOCUS::get_car_info_EngineSpeed()
// {
// 	static WORD wdValue;
// 	if (car_info[0] != 0x02) return wdValue;
// 	wdValue = car_info[1] * 256 + car_info[2];
// 	return  wdValue;
// }
// 
// WORD CANBUS_RAISE_FOCUS::get_car_info_InstantaneousVelocity()
// {
// 	static WORD wdValue;
// 	if (car_info[0] != 0x02) return wdValue;
// 	wdValue = (car_info[3] * 256 + car_info[4]) * 0.01;
// 	return  wdValue;
// }
// 
// WORD CANBUS_RAISE_FOCUS::get_car_info_BattVtg()
// {
// 	static WORD wdValue;
// 	if (car_info[0] != 0x02) return wdValue;
// 	wdValue = (car_info[5] * 256 + car_info[6]) * 0.01;
// 	return  wdValue;
// }

INT CANBUS_RAISE_FOCUS::get_car_info_TempOutside()
{
	static INT nValue;
// 	if (car_info[0] != 0x02) return nValue;
// 	WORD wdValue = car_info[7] * 256 + car_info[8];
// 	BOOL bFlag = wdValue & 0x8000;
// 	if (bFlag)
// 	{
// 		wdValue = ~ (wdValue - 1);
// 		nValue = - wdValue * 0.1;
// 	} 
// 	else
// 	{
// 		nValue = wdValue * 0.1;
// 	}	
	return  nValue;
}


// WORD CANBUS_RAISE_FOCUS::get_car_info_Mileage()
// {
// 	static WORD wdValue;
// 	if (car_info[0] != 0x02) return wdValue;
// 	wdValue = car_info[9] * 65536 +car_info[10] *256 + car_info[11];
// 	return  wdValue;
// }
// 
// WORD CANBUS_RAISE_FOCUS::get_car_info_RemainL()
// {
// 	static WORD wdValue;
// 	if (car_info[0] != 0x02) return wdValue;
// 	wdValue = car_info[12];
// 	return  wdValue;
// }
// 
// BOOL CANBUS_RAISE_FOCUS::is_car_info_RemainL_low()
// {
// 	static BOOL bValue;
// 	if (car_info[0] != 0x03) return bValue;
// 	bValue = car_info[1] >> 7 & 0x01;
// 	return  bValue;
// }
// 
// BOOL CANBUS_RAISE_FOCUS::is_car_info_BattVtg_low()
// {
// 	static BOOL bValue;
// 	if (car_info[0] != 0x03) return bValue;
// 	bValue = car_info[1] >> 6 & 0x01;
// 	return  bValue;
// }

///////////////////////////本田/////////////////////////////
CANBUS_RAISE_HONDA*  CANBUS_RAISE_HONDA::get_instance()
{
	static CANBUS_RAISE_HONDA dasauto;
	return &dasauto;
}

//方向盘按键信息

BYTE CANBUS_RAISE_HONDA::get_swc_key_Value()
{
	return swc_key[0];
}

BYTE CANBUS_RAISE_HONDA::get_swc_key_State()
{
	return swc_key[1];
}

BOOL CANBUS_RAISE_HONDA::is_climatronic_Switch()
{
	return climatronic[0] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_climatronic_AC()
{
	return climatronic[0] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_climatronic_Cycle()
{
	return climatronic[0] >> 5 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_climatronic_HighWind()
{
	return climatronic[0] >> 4 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_climatronic_SmallWind()
{
	return climatronic[0] >> 3 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_climatronic_Dual()
{
	return climatronic[0] >> 2& 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_climatronic_FDemister()
{
	return climatronic[0] >> 1 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_climatronic_RDemister()
{
	return climatronic[0] & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_climatronic_Airdir_head()
{
	return climatronic[1] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_climatronic_Airdir_hands()
{
	return climatronic[1] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_climatronic_Airdir_feet()
{
	return climatronic[1] >> 5 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_climatronic_REQ_show()
{
	return climatronic[1] >> 4 & 0x01;
}

BYTE  CANBUS_RAISE_HONDA::get_climatronic_FanSpeed()
{
	return (climatronic[1] & 0x0F);
}

FLOAT CANBUS_RAISE_HONDA::get_climatronic_LTemp()
{
	// 驾驶位置处温度 0x00:LO 0x1F:HI 0x01-0x11:18℃ ~ 26℃,步进0.5℃
	FLOAT fValue;
	BYTE nValue = climatronic[2];

	if (nValue >= 0x01 && nValue <= 0x11)
	{
		fValue = 18 + 0.5f*(nValue-0x01);           
	}
	else
	{
		fValue = nValue;
	}
	//RETAILMSG( 1,(L"[CANBUS_RAISE_DASAUTO]..LTemp .......................... = %f \r\n",fValue ) );
	return  fValue;
}

FLOAT CANBUS_RAISE_HONDA::get_climatronic_RTemp()
{
	FLOAT fValue;
	BYTE nValue = climatronic[3];

	if (nValue >= 0x01 && nValue <= 0x11)
	{
		fValue = 18 + 0.5f*(nValue-0x01);  
	}
	else
	{
		fValue = nValue;
	}
	return  fValue;
}


//雷达信息
// BYTE CANBUS_RAISE_HONDA::get_radar_front_L()
// {
// 	return front_radar[0];
// }
// 
// BYTE CANBUS_RAISE_HONDA::get_radar_front_L_C()
// {
// 	return front_radar[1];
// }
// 
// BYTE CANBUS_RAISE_HONDA::get_radar_front_R_C()
// {
// 	return front_radar[2];
// }
// 
// BYTE CANBUS_RAISE_HONDA::get_radar_front_R()
// {
// 	return front_radar[3];
// }

BYTE CANBUS_RAISE_HONDA::get_radar_rear_L()
{
	return rear_radar[0];
}

BYTE CANBUS_RAISE_HONDA::get_radar_rear_L_C()
{
	return rear_radar[1];
}

BYTE CANBUS_RAISE_HONDA::get_radar_rear_R_C()
{
	return rear_radar[2];
}

BYTE CANBUS_RAISE_HONDA::get_radar_rear_R()
{
	return rear_radar[3];
}

//方向盘转角 
INT CANBUS_RAISE_HONDA::get_swc_angle()
{
	BYTE ESP1 = swc_angle[0];
	BYTE ESP2 = swc_angle[1];
	WORD ESP = MAKEWORD(ESP1,ESP2);
	BOOL bFlag = ESP2 & 0x80;
	INT nAngle = 0;
	if (bFlag)
	{
		ESP = ~ (ESP - 1);
		nAngle = -ESP;
	}
	else
	{
		nAngle = ESP;
	}
	return nAngle;//
}

//车身信息
// BOOL CANBUS_RAISE_HONDA::is_car_info_SeatBelt_off()
// {
// 	static BOOL bValue;
// 	if (car_info[0] != 0x01) return bValue;
// 	bValue = car_info[1] >> 7 & 0x01;
// 	return  bValue;
// }
// 
// BOOL CANBUS_RAISE_HONDA::is_car_info_CleanWater_low()
// {
// 	static BOOL bValue;
// 	if (car_info[0] != 0x01) return bValue;
// 	bValue = car_info[1] >> 6 & 0x01;
// 	return  bValue;
// }

BOOL CANBUS_RAISE_HONDA::is_car_info_Brake_hung()
{
	return  basic_info[1] >> 1 & 0x01;
}


BOOL CANBUS_RAISE_HONDA::is_car_info_Trunk_Open()
{
	return  basic_info[0] >> 3 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_car_info_Door_RR()
{
	return  basic_info[0] >> 5 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_car_info_Door_RL()
{
	return  basic_info[0] >> 4 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_car_info_Door_FR()
{
	return  basic_info[0] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_HONDA::is_car_info_Door_FL()
{
	return  basic_info[0] >> 6 & 0x01;
}

// WORD CANBUS_RAISE_HONDA::get_car_info_EngineSpeed()
// {
// 	static WORD wdValue;
// 	if (car_info[0] != 0x02) return wdValue;
// 	wdValue = car_info[1] * 256 + car_info[2];
// 	return  wdValue;
// }
// 
// WORD CANBUS_RAISE_HONDA::get_car_info_InstantaneousVelocity()
// {
// 	static WORD wdValue;
// 	if (car_info[0] != 0x02) return wdValue;
// 	wdValue = (car_info[3] * 256 + car_info[4]) * 0.01;
// 	return  wdValue;
// }
// 
// WORD CANBUS_RAISE_HONDA::get_car_info_BattVtg()
// {
// 	static WORD wdValue;
// 	if (car_info[0] != 0x02) return wdValue;
// 	wdValue = (car_info[5] * 256 + car_info[6]) * 0.01;
// 	return  wdValue;
// }
// 
// INT CANBUS_RAISE_HONDA::get_car_info_TempOutside()
// {
// 	static INT nValue;
// 	if (car_info[0] != 0x02) return nValue;
// 	WORD wdValue = car_info[7] * 256 + car_info[8];
// 	BOOL bFlag = wdValue & 0x8000;
// 	if (bFlag)
// 	{
// 		wdValue = ~ (wdValue - 1);
// 		nValue = - wdValue * 0.1;
// 	} 
// 	else
// 	{
// 		nValue = wdValue * 0.1;
// 	}	
// 	return  nValue;
// }
// 
// 
// WORD CANBUS_RAISE_HONDA::get_car_info_Mileage()
// {
// 	static WORD wdValue;
// 	if (car_info[0] != 0x02) return wdValue;
// 	wdValue = car_info[9] * 65536 +car_info[10] *256 + car_info[11];
// 	return  wdValue;
// }
// 
// WORD CANBUS_RAISE_HONDA::get_car_info_RemainL()
// {
// 	static WORD wdValue;
// 	if (car_info[0] != 0x02) return wdValue;
// 	wdValue = car_info[12];
// 	return  wdValue;
// }
// 
// BOOL CANBUS_RAISE_HONDA::is_car_info_RemainL_low()
// {
// 	static BOOL bValue;
// 	if (car_info[0] != 0x03) return bValue;
// 	bValue = car_info[1] >> 7 & 0x01;
// 	return  bValue;
// }
// 
// BOOL CANBUS_RAISE_HONDA::is_car_info_BattVtg_low()
// {
// 	static BOOL bValue;
// 	if (car_info[0] != 0x03) return bValue;
// 	bValue = car_info[1] >> 6 & 0x01;
// 	return  bValue;
// }
// 
// // 停车辅助状态
// BOOL CANBUS_RAISE_HONDA::is_parking_info_Switch()
// {
// 	return parking_info[0] >> 1 & 0x01;
// }
// 
// BOOL CANBUS_RAISE_HONDA::is_parking_info_Radar_beep()
// {
// 	return parking_info[0] & 0x01;
// }
// 
// 
// //车速
// WORD  CANBUS_RAISE_HONDA::get_car_Speed()
// {
// 	return MAKEWORD(speed[0], speed[1]);
// }


//////////////////////////// CANBUS_RAISE_CAMRY /////////////////////////////////

CANBUS_RAISE_CAMRY*  CANBUS_RAISE_CAMRY::get_instance()
{
	static CANBUS_RAISE_CAMRY dasauto;
	return &dasauto;
}

//方向盘按键信息

BYTE CANBUS_RAISE_CAMRY::get_swc_key_Value()
{
	return swc_key[0];
}

BYTE CANBUS_RAISE_CAMRY::get_swc_key_State()
{
	return swc_key[1];
}

BOOL CANBUS_RAISE_CAMRY::is_climatronic_Switch()
{
	return climatronic[0] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_CAMRY::is_climatronic_AC()
{
	return climatronic[0] >> 6 & 0x01;
}

// BOOL CANBUS_RAISE_CAMRY::is_climatronic_Cycle()
// {
// 	return climatronic[0] >> 5 & 0x01;
// }


BOOL CANBUS_RAISE_CAMRY::is_climatronic_SmallWind()
{
	return climatronic[0] >> 3 & 0x01;
}

BOOL CANBUS_RAISE_CAMRY::is_climatronic_Dual()
{
	return climatronic[0] >> 2 & 0x01;
}

BOOL CANBUS_RAISE_CAMRY::is_climatronic_RearHeat()
{
	return climatronic[4] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_CAMRY::is_climatronic_FDemister()
{
	return climatronic[4] >> 7 & 0x01;
}

// BOOL CANBUS_RAISE_CAMRY::is_climatronic_RDemister()
// {
// 	return climatronic[0] & 0x01;
// }

BOOL CANBUS_RAISE_CAMRY::is_climatronic_Airdir_head()
{
	return climatronic[1] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_CAMRY::is_climatronic_Airdir_hands()
{
	return climatronic[1] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_CAMRY::is_climatronic_Airdir_feet()
{
	return climatronic[1] >> 5 & 0x01;
}

BOOL CANBUS_RAISE_CAMRY::is_climatronic_REQ_show()
{
	BYTE bb = climatronic[1] >> 4 & 0x01;
	return climatronic[1] >> 4 & 0x01;
}

BYTE  CANBUS_RAISE_CAMRY::get_climatronic_FanSpeed()
{
	return (climatronic[1] & 0x0F);
}

FLOAT CANBUS_RAISE_CAMRY::get_climatronic_LTemp()
{
	// 驾驶位置处温度 0x00:LO 0x1F:HI 0x01-0x1D:16℃ ~ 32℃,步进0.5℃
	FLOAT fValue;
	BYTE nValue = climatronic[2];

	if (nValue >= 0x20 && nValue <= 0x23)
	{
		fValue = 16 + 0.5f*(nValue-0x20);
		if (get_climatronic_tempunit())
		{
			fValue = (fValue *9/5) + 32.0;
		}
	}
	else if (nValue >= 0x01 && nValue <= 0x1D)
	{
		fValue = 18 + 0.5f*(nValue-0x01);
		if (get_climatronic_tempunit())
		{
			fValue = (fValue *9/5) + 32.0;
		}
	}
	else
	{
		fValue = nValue;
	}
	//RETAILMSG( 1,(L"[CANBUS_RAISE_DASAUTO]..LTemp .......................... = %f \r\n",fValue ) );
	return  fValue;
}

FLOAT CANBUS_RAISE_CAMRY::get_climatronic_RTemp()
{
	FLOAT fValue;
	BYTE nValue = climatronic[3];

	if (nValue >= 0x20 && nValue <= 0x23)
	{
		fValue = 16 + 0.5f*(nValue-0x20);
		if (get_climatronic_tempunit())
		{
			fValue = (fValue *9/5) + 32.0;
		}
	}
	else if (nValue >= 0x01 && nValue <= 0x1D)
	{
		fValue = 18 + 0.5f*(nValue-0x01);
		if (get_climatronic_tempunit())
		{
			fValue = (fValue *9/5) + 32.0;
		}
	}
	else
	{
		fValue = nValue;
	}
	return  fValue;
}

// BOOL  CANBUS_RAISE_CAMRY::is_climatronic_AC_max()// AC_max
// {
// 	return climatronic[4] >> 2 & 0x01;
// }

BYTE  CANBUS_RAISE_CAMRY::get_climatronic_tempunit()//
{
	return climatronic[4]  & 0x01;//0 摄氏  1 华氏
}


//雷达信息
BYTE CANBUS_RAISE_CAMRY::get_radar_front_L()
{
	return front_radar[0];
}

BYTE CANBUS_RAISE_CAMRY::get_radar_front_L_C()
{
	return front_radar[1];
}

BYTE CANBUS_RAISE_CAMRY::get_radar_front_R_C()
{
	return front_radar[2];
}

BYTE CANBUS_RAISE_CAMRY::get_radar_front_R()
{
	return front_radar[3];
}

BYTE CANBUS_RAISE_CAMRY::get_radar_rear_L()
{
	return rear_radar[0];
}

BYTE CANBUS_RAISE_CAMRY::get_radar_rear_L_C()
{
	return rear_radar[1];
}

BYTE CANBUS_RAISE_CAMRY::get_radar_rear_R_C()
{
	return rear_radar[2];
}

BYTE CANBUS_RAISE_CAMRY::get_radar_rear_R()
{
	return rear_radar[3];
}

BOOL  CANBUS_RAISE_CAMRY::is_radar_rear_show()           // 显示状态
{
	return rear_radar[4] >> 7 & 0x01;
}

BYTE  CANBUS_RAISE_CAMRY::get_radar_rear_range()            // 距离 0:近 1:远
{
	return rear_radar[4] >> 6 & 0x01;
}

BOOL  CANBUS_RAISE_CAMRY::is_radar_rear_switch()		 //雷达开关
{
	return rear_radar[4] >> 5 & 0x01;
}

BYTE  CANBUS_RAISE_CAMRY::get_radar_rear_vol()		//音量强度
{
	return rear_radar[4]  & 0x7;
}

//方向盘转角
INT CANBUS_RAISE_CAMRY::get_swc_angle()
{
	BYTE ESP1 = swc_angle[0];
	BYTE ESP2 = swc_angle[1];
	WORD ESP = MAKEWORD(ESP1,ESP2);
	BOOL bFlag = ESP2 & 0x80;
	INT nAngle = 0;
	if (bFlag)
	{
		ESP = ~ (ESP - 1);
		nAngle = -ESP;
	}
	else
	{
		nAngle = ESP;
	}
	return nAngle;
}

BOOL CANBUS_RAISE_CAMRY::is_car_info_Trunk_Open()
{
	return  basic_info[0] >> 3 & 0x01;
}

BOOL CANBUS_RAISE_CAMRY::is_car_info_Door_RR()
{
	return  basic_info[0] >> 5 & 0x01;
}

BOOL CANBUS_RAISE_CAMRY::is_car_info_Door_RL()
{
	return  basic_info[0] >> 4 & 0x01;
}

BOOL CANBUS_RAISE_CAMRY::is_car_info_Door_FR()
{
	return  basic_info[0] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_CAMRY::is_car_info_Door_FL()
{
	return  basic_info[0] >> 6 & 0x01;
}

/////////////////////////////////////标致雪铁龙///////////////////////////////////////////////////
CANBUS_RAISE_PEUGEOT*  CANBUS_RAISE_PEUGEOT::get_instance()
{
	static CANBUS_RAISE_PEUGEOT dasauto;
	return &dasauto;
}

BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_Switch()
{
	return climatronic[0] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_AC()
{
	return climatronic[0] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_Cycle()
{
	return climatronic[0] >> 5 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_RearHeat()
{
	return climatronic[0]  & 0x01;
// 	return 0;
}

BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_Auto()
{
	return climatronic[0] >> 3 & 0x01;
// 	return 0;
}

BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_Dual()
{
	return climatronic[0] >> 2& 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_FDemister()
{
// 	return climatronic[0] >> 1 & 0x01;
	return climatronic[4] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_AC_max()
{
	return climatronic[4] >> 3 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_Airdir_auto()
{
	return climatronic[4]  >> 1 & 0x01;
}

BYTE CANBUS_RAISE_PEUGEOT::get_climatronic_tempunit()
{
	return (climatronic[4]  & 0x01);
}

BYTE CANBUS_RAISE_PEUGEOT::get_climatronic_temp_step()
{
	return climatronic[5]  >> 7 & 0x01; //0步距1 1：步距0.5

}
BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_REQ_show()
{
// 	 return climatronic[4] >> 1 & 0x01;
	return 1;//协议里没有这个定义
}

BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_Airdir_head()
{
	return climatronic[1] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_Airdir_hands()
{
	return climatronic[1] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_climatronic_Airdir_feet()
{
	return climatronic[1] >> 5 & 0x01;
}


BYTE  CANBUS_RAISE_PEUGEOT::get_climatronic_FanSpeed()
{
	return (climatronic[1] & 0x0F);
}

BYTE  CANBUS_RAISE_PEUGEOT::get_climatronic_FanStrength()
{
	return (climatronic[6] & 0xC0);
}

FLOAT CANBUS_RAISE_PEUGEOT::get_climatronic_LTemp()
{
	// 驾驶位置处温度 0x00:LO 0xFF:HI 0x01-0x11:16℃ ~ 30℃,步进0.5℃
	FLOAT fValue;
	BYTE nValue = climatronic[2];

	if (nValue >= 0x20 && nValue <= 0x3c)
	{
		fValue = 16 + 0.5f*(nValue-0x01);
	}
	else
	{
		fValue = nValue;
	}
	RETAILMSG( 1,(L"[CANBUS_RAISE_DASAUTO]..LTemp .......................... = %f \r\n",fValue ) );
	return  fValue;
}

FLOAT CANBUS_RAISE_PEUGEOT::get_climatronic_RTemp()
{
	FLOAT fValue;
	BYTE nValue = climatronic[3];

	if (nValue >= 0x20 && nValue <= 0x3c)
	{
		fValue = 16 + 0.5f*(nValue-0x01);
	}
	else
	{
		fValue = nValue;
	}
	return  fValue;
}


// 
// BYTE CANBUS_RAISE_PEUGEOT::get_climatronic_RHotSeat()
// {
// 	return climatronic[4] & 0x03;
// }


//雷达信息

BYTE CANBUS_RAISE_PEUGEOT::get_radar_work_status()
{
	return rear_radar_info[0];
}

BYTE CANBUS_RAISE_PEUGEOT::get_radar_rear_L()
{
	return rear_radar_info[1];
}

BYTE CANBUS_RAISE_PEUGEOT::get_radar_rear_C()
{
	return rear_radar_info[2];
}


BYTE CANBUS_RAISE_PEUGEOT::get_radar_rear_R()
{
	return rear_radar_info[3];
}


BYTE CANBUS_RAISE_PEUGEOT::get_radar_front_L()
{
	return rear_radar_info[4];
}

BYTE CANBUS_RAISE_PEUGEOT::get_radar_front_C()
{
	return rear_radar_info[5];
}

BYTE CANBUS_RAISE_PEUGEOT::get_radar_front_R()
{
	return rear_radar_info[6];
}


//方向盘转角
// INT CANBUS_RAISE_PEUGEOT::get_swc_angle()
// {
// 	BYTE ESP1 = swc_angle[0];
// 	BYTE ESP2 = swc_angle[1];
// 	WORD ESP = MAKEWORD(ESP1,ESP2);
// 	BOOL bFlag = ESP2 & 0x80;
// 	INT nAngle = 0;
// 	if (bFlag)
// 	{
// 		ESP = ~ (ESP - 1);
// 		nAngle = -ESP;
// 	}
// 	else
// 	{
// 		nAngle = ESP;
// 	}
// 	return nAngle;
// }

//车身信息

BOOL CANBUS_RAISE_PEUGEOT::is_car_info_Door_FL()
{
	return  car_info[0] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_car_info_Door_FR()
{
	return  car_info[0] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_car_info_Door_RL()
{
	return  car_info[0] >> 5 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_car_info_Door_RR()
{
	return  car_info[0] >> 4 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_car_info_Trunk_Open()
{
	return  car_info[0] >> 3 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_car_info_SeatBelt_off()
{
	return  car_info[0] >> 1 & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_car_info_SeatBelt2_off()
{
	return  car_info[0]  & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_car_info_RemainL_low()
{
	return  car_info[1] >> 5  & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_car_info_Door_locking()
{
	return  car_info[1] >> 4  & 0x01;
}

// 停车辅助状态
BOOL CANBUS_RAISE_PEUGEOT::is_parking_info_Switch()
{
	return car_info[1] >> 3  & 0x01;
}

BYTE CANBUS_RAISE_PEUGEOT::get_car_info_lighting()
{
	return car_info[3] >> 7  & 0x01;
}

BOOL  CANBUS_RAISE_PEUGEOT::is_car_Rear_radar_Enable()    //倒车雷达状态0:停用 1:启用
{
	return car_info[3] >> 3  & 0x01;
}

BYTE  CANBUS_RAISE_PEUGEOT::get_car_parking_status()		//倒车状态 0:倒车 1 非倒车
{
	return car_info[3] >> 2  & 0x01;
}

BYTE  CANBUS_RAISE_PEUGEOT::get_car_P_shift_status()           //P档状态 0:p档  1 非p档
{
	return car_info[3] >> 1  & 0x01;
}

BOOL CANBUS_RAISE_PEUGEOT::is_parking_info_Radar_beep()
{
	return radarvol_change_flag[0] >> 7 & 0x01;
}


/////////////////////////////////////现代///////////////////////////////////////////////////
CANBUS_RAISE_HYUNDAI*  CANBUS_RAISE_HYUNDAI::get_instance()
{
	static CANBUS_RAISE_HYUNDAI dasauto;
	return &dasauto;
}

BOOL CANBUS_RAISE_HYUNDAI::is_climatronic_Switch()
{
	return climatronic[2];//0表示空调关闭
}

BOOL CANBUS_RAISE_HYUNDAI::is_climatronic_AC()
{
	return climatronic[3]  & 0x01;
}

BOOL CANBUS_RAISE_HYUNDAI::is_climatronic_Cycle()
{
	return climatronic[3] >> 7 & 0x01;
}

BOOL CANBUS_RAISE_HYUNDAI::is_climatronic_Auto()
{
	return climatronic[3] >> 2 & 0x01;
	// 	return 0;
}

BOOL CANBUS_RAISE_HYUNDAI::is_climatronic_Dual()
{
	return climatronic[3] >> 1 & 0x01;
}

BOOL CANBUS_RAISE_HYUNDAI::is_climatronic_FDemister()
{
	return climatronic[3] >> 6 & 0x01;
}

BOOL CANBUS_RAISE_HYUNDAI::is_climatronic_RDemister()
{
	return climatronic[3] >> 5 & 0x01;
}



BOOL CANBUS_RAISE_HYUNDAI::is_climatronic_REQ_show()
{
	// 	 return climatronic[4] >> 1 & 0x01;
	return 1;//协议里没有这个定义
}

// BOOL CANBUS_RAISE_HYUNDAI::is_climatronic_Airdir_head()
// {
// 	return climatronic[1] >> 7 & 0x01;
// }

BOOL CANBUS_RAISE_HYUNDAI::is_climatronic_Airdir_hands()//正面吹风
{
	return climatronic[3] >> 3 & 0x01;
}

BOOL CANBUS_RAISE_HYUNDAI::is_climatronic_Airdir_feet()
{
	return climatronic[3] >> 4 & 0x01;
}


BYTE  CANBUS_RAISE_HYUNDAI::get_climatronic_FanSpeed()
{
	return climatronic[2];
}


FLOAT CANBUS_RAISE_HYUNDAI::get_climatronic_LTemp()
{
	// 驾驶位置处温度 0x00:LO 0x1E:HI 0x01-0x11:17.5℃ ~ 31.5℃,步进0.5℃
	FLOAT fValue;
	BYTE nValue = climatronic[0];

	if (nValue >= 0x01 && nValue <= 0x1D)
	{
		fValue = 17.5 + 0.5f*(nValue-0x01);
	}
	else
	{
		fValue = nValue;
	}
	RETAILMSG( 1,(L"[CANBUS_RAISE_DASAUTO]..LTemp .......................... = %f \r\n",fValue ) );
	return  fValue;
}

FLOAT CANBUS_RAISE_HYUNDAI::get_climatronic_RTemp()
{
	FLOAT fValue;
	BYTE nValue = climatronic[1];

	if (nValue >= 0x01 && nValue <= 0x1D)
	{
		fValue = 17.5 + 0.5f*(nValue-0x01);
	}
	else
	{
		fValue = nValue;
	}
	return  fValue;
}

BYTE CANBUS_RAISE_HYUNDAI::get_radar_front_L()
{
	return radar_all_info[0] & 0xC0; //0x00不在范围内，0x01刚进入范围 0x10	离车太近
}

BYTE CANBUS_RAISE_HYUNDAI::get_radar_front_C()
{
	return radar_all_info[0] & 0x3c; //0x0000不在范围内，0x0001刚进入范围 显示一格0x0010	显示二格 0x0011离车太近
}

BYTE CANBUS_RAISE_HYUNDAI::get_radar_front_R()
{
	return radar_all_info[0] & 0x03;
}


BYTE CANBUS_RAISE_HYUNDAI::get_radar_rear_L()
{
	return radar_all_info[1] & 0xC0;
}

BYTE CANBUS_RAISE_HYUNDAI::get_radar_rear_C()
{
	return radar_all_info[1] & 0x3c;
}


BYTE CANBUS_RAISE_HYUNDAI::get_radar_rear_R()
{
	return radar_all_info[1] & 0x03;
}

INT  CANBUS_RAISE_HYUNDAI::get_car_info_TempOutside()     // 车外温度 ℃
{
	int nTemp = 0;
	nTemp = outside_temp[0] & 0x7F;
	
	return (outside_temp[0] >> 7 & 0x01) ? -nTemp: nTemp;//最高位为符号位0为正1为负
}
