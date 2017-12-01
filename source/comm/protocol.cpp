#include "stdafx.h"
#include "protocol.h"
#include "MainWnd.h"
#include "interface.h"

static int _BCD2Int(IN BYTE* pdata, int len)
{
	int sum = 0;
	if (pdata && len>0)
	{
		for (int i=0; i<len; i++)
		{
			int n = (pdata[i] & 0xF0) >> 4;
			int m = (pdata[i] & 0x0F);

			sum = sum*10 + n;
			sum = sum*10 + m;
		}
	}

	return sum;
}

// 返回转成BCD码后占用了多少字节， pbuffer指向的缓冲区必须足够大
static int _Int2BCD(int number, OUT BYTE* pbuffer)
{
	int count = 0;
	while (pbuffer && number > 0)
	{
		int m = number % 10;
		number /= 10;
		int n = number % 10;
		number /= 10;

		pbuffer[count++] = ((n & 0x0F) << 4) + m;
	}

	// 逆序一下
	for (int i=0; i<count/2; i++)
	{
		BYTE temp = pbuffer[i];
		pbuffer[i] = pbuffer[count-1-i];
		pbuffer[count-1-i] = temp;
	}

	return count;
}

///////////////// status ///////////////////////

BOOL MCU_STATUS::is_CDC_exist()
{
	return data[0] & 0x01;
}

BOOL MCU_STATUS::is_DISK_exist()
{
	return data[0] & 0x02;
}

BOOL MCU_STATUS::is_DISK_autoin()
{
	return data[0] & 0x04;
}

BOOL MCU_STATUS::is_camera_on()
{
	return data[1] & 0x01;
}

BOOL MCU_STATUS::is_light_on()
{
	return data[1] & 0x02;
}

BOOL MCU_STATUS::is_brake_on()
{
	return data[1] & 0x04 ? FALSE : TRUE;	// 0 ~ 刹车启动
}

int  MCU_STATUS::get_source_id()
{
	return data[3];
}

BOOL MCU_STATUS::is_sleep()
{
	return data[3] & 0x01 ? TRUE : FALSE; // bit0 表示假关机状态
}

//////////////////////////// system parameters ///////////////////////////


int MCU_SYS_PARA::get_fm_cur_region()
{
	return protocol::get_mcu_sys_para()->fm_data[0] & 0x0F;
}

void MCU_SYS_PARA::set_fm_cur_region(BYTE region)
{
	protocol::get_mcu_sys_para()->fm_data[0] &= 0xF0;
	protocol::get_mcu_sys_para()->fm_data[0] |= (region & 0x0F);
}


int MCU_SYS_PARA::get_fm_default_region()
{
	return (protocol::get_mcu_sys_para()->fm_data[0]>>4) & 0x0F;
}

void MCU_SYS_PARA::get_system_time(SYSTEMTIME *ptm)
{
	if (ptm)
	{
		ptm->wYear = _BCD2Int(time_data, 2);
		ptm->wMonth = _BCD2Int(time_data+2, 1);
		ptm->wDay = _BCD2Int(time_data+3, 1);
		ptm->wHour = _BCD2Int(time_data+4, 1);
		ptm->wMinute = _BCD2Int(time_data+5, 1);
		ptm->wSecond = _BCD2Int(time_data+6, 1);
	}
}


void MCU_SYS_PARA::set_system_time(IN SYSTEMTIME *ptm)
{
	if (ptm)
	{
		_Int2BCD(ptm->wYear, time_data);
		_Int2BCD(ptm->wMonth, time_data+2);
		_Int2BCD(ptm->wDay, time_data+3);
		_Int2BCD(ptm->wHour, time_data+4);
		_Int2BCD(ptm->wMinute, time_data+5);
		_Int2BCD(ptm->wSecond, time_data+6);
	}
}

void MCU_SYS_PARA::set_video_data(int backlight, int brightness, int contrast, int hue, int satruation, int camera_mirror)
{
	video_data[0] = (BYTE)brightness;
	video_data[1] = (BYTE)contrast;
	video_data[2] = (BYTE)hue;
	video_data[3] = (BYTE)satruation;
	video_data[4] = (BYTE)backlight;
	video_data[5] = (BYTE)camera_mirror;
}

BOOL MCU_SYS_PARA::is_rds_AF_on()
{
	return (rds_data[0] & (1<<3));
}

BOOL MCU_SYS_PARA::is_rds_TA_on()
{
	return (rds_data[0] & (1<<4));
}

BOOL MCU_SYS_PARA::is_rds_EON_on()
{
	return (rds_data[0] & (1<<5));
}

BOOL MCU_SYS_PARA::is_rds_REGION_on()
{
	return (rds_data[0] & (1<<6));
}

int MCU_SYS_PARA::get_colorful_light()
{
	return general_data[1];
}

void MCU_SYS_PARA::set_colorful_light(int value)
{
	general_data[1] = value;
}


//////////////////////////// MCU_INIT_TABLE  ///////////////////////////
void MCU_INIT_TABLE::get_system_time(SYSTEMTIME *ptm)
{
	if (ptm)
	{
		ptm->wYear = _BCD2Int(data+9, 2);
		ptm->wMonth = _BCD2Int(data+11, 1);
		ptm->wDay = _BCD2Int(data+12, 1);
		ptm->wHour = _BCD2Int(data+13, 1);
		ptm->wMinute = _BCD2Int(data+14, 1);
		ptm->wSecond = _BCD2Int(data+15, 1);
	}
}


int  MCU_INIT_TABLE::get_extra_data(OUT void** ppdata)
{
	if (extra_data_len > 0)
	{
		*ppdata = data + 17;
	}
	return extra_data_len;
}

//////////////////////////// MCU_RADIO_STATUS  ///////////////////////////
BOOL MCU_RADIO_STATUS::is_use_stereo()
{
	return (status[0] & 0x20);
}

BOOL MCU_RADIO_STATUS::is_stereo_status()
{
	return (status[0] & 0x10);
}


BOOL MCU_RADIO_STATUS::is_scan()
{
	return (status[0] & 0x04);	// 协议上的文字描述有点问题，这个状态其实叫scan
}

BOOL MCU_RADIO_STATUS::is_DX_mode()
{
	return (status[0] & 0x80);
}

BOOL MCU_RADIO_STATUS::is_preview_scan()
{
	return (status[1] & 0x01);
}

void MCU_RADIO_STATUS::SetData(BYTE* pdata)
{
	if (pdata)
	{
		min_freq = (pdata[0] << 8) + (WORD)pdata[1];
		max_freq = (pdata[2] << 8) + pdata[3];
		granularity = pdata[4];
		signal = pdata[5];
		memcpy(status, pdata+6, sizeof(status));
		rds_status = pdata[8];
	}
}

// rds
BOOL MCU_RADIO_STATUS::is_EWS()
{
	return (rds_status & (1<<3)) ? TRUE : FALSE;
}

BOOL MCU_RADIO_STATUS::is_TA_Alarm()
{
	return (rds_status & (1<<2)) ? TRUE : FALSE;
}

BOOL MCU_RADIO_STATUS::is_TA_seek()
{
	return (rds_status & (1<<4)) ? TRUE : FALSE;
}

BOOL MCU_RADIO_STATUS::is_PTY_seek()
{
	return (rds_status & (1<<5)) ? TRUE : FALSE;
}

BOOL MCU_RADIO_STATUS::is_AF_seek()
{
	return (rds_status & (1<<6)) ? TRUE : FALSE;
}

//////////////////////////// MCU_RADIO_PRESET_INFO  ///////////////////////////

void MCU_RADIO_PRESET_INFO::SetData(BYTE* pdata)
{
	if (pdata)
	{
		band = pdata[0];
		preset_index = pdata[1];
		cur_freq = (pdata[2] << 8) + pdata[3];
		for (int i=0; i<6; i++)
		{
			preset_list[i] = (pdata[4+i*2] << 8) + pdata[5+i*2];
		}
	}
}


//////////////////////////// MCU_RADIO_PS_PTY  ///////////////////////////

void MCU_RADIO_PS_PTY::SetData(BYTE* pdata)
{
	if (pdata)
	{
		memcpy(PS, pdata, 8);
		PS[8] = '\0';
		PTY = pdata[8];
	}
}

//////////////////////////// MCU_RADIO_RT_INFO  ///////////////////////////

void MCU_RADIO_RT_INFO::SetData(BYTE* pdata)
{
	if (pdata)
	{
		len = pdata[0];
		memset(RT, 0, sizeof(RT));
		if (len>=1 && len<=64)
		{
			memcpy(RT, pdata+1, len);
		}
	}
}
////////////////////////////  MCU_ARM_STATUS  ///////////////////////////
void MCU_ARM_STATUS::set_bt_active(BOOL is_active)
{
	if (is_active)
	{
		data[2] |= (1<<3); 
	}
	else
	{
		data[2] &= ~(1<<3);
	}
}

BOOL MCU_ARM_STATUS::is_bt_active()
{
	return (data[2] & (1<<3)) ? TRUE : FALSE;
}

//////////////////////////// protocol /////////////////////////////

MCU_STATUS* protocol::get_mcu_status()
{
	static MCU_STATUS status;
	return &status;
}


static void _notify_mcu_status_change(MCU_STATUS* pstatusOld, MCU_STATUS* pstatusNew)
{
	if (!pstatusOld || !pstatusNew)
	{
		return;
	}

	if (pstatusOld->is_DISK_exist() && !pstatusNew->is_DISK_exist())	// 出碟
	{
		CMainWnd::GetInstance()->PostMessage(UI_MSG_DVD_EJECT_NOTIFY, 1, 0);
	}
	else if (!pstatusOld->is_DISK_exist() && pstatusNew->is_DISK_exist() )	
	{
		if (!pstatusNew->is_DISK_autoin())
		{
			CMainWnd::GetInstance()->PostMessage(UI_MSG_DVD_EJECT_NOTIFY, 1, 1);// 进碟(自动吸入的不触发该消息,也不切源)
		} 
		else
		{
			CMainWnd::GetInstance()->PostMessage(UI_MSG_DVD_EJECT_NOTIFY, 0, 1);	// wParam为0表示不需要切源
		}
	}
}

void protocol::set_mcu_status(MCU_STATUS* pstatus)
{
	if (pstatus)
	{
		MCU_STATUS old_status;
		memcpy(&old_status, get_mcu_status(), sizeof(MCU_STATUS));

		memcpy(get_mcu_status(), pstatus, sizeof(MCU_STATUS));	// 数据提前更新,不要放在下面代码后面
		_notify_mcu_status_change(&old_status, pstatus);
	}
}


MCU_SYS_PARA* protocol::get_mcu_sys_para()
{
	static MCU_SYS_PARA sys_para;
	return &sys_para;
}

void protocol::set_mcu_sys_para(int index, LPVOID pdata)
{
	if (!pdata)
	{
		return;
	}

	switch (index)
	{
	case MCU_SYS_PARA::INDEX_FM:	// FM
		memcpy(get_mcu_sys_para()->fm_data, pdata, sizeof(get_mcu_sys_para()->fm_data));
		break;
	case MCU_SYS_PARA::INDEX_RDS:	// RDS
		memcpy(get_mcu_sys_para()->rds_data, pdata, sizeof(get_mcu_sys_para()->rds_data));
		break;
	case MCU_SYS_PARA::INDEX_TIME:
		memcpy(get_mcu_sys_para()->time_data, pdata, sizeof(get_mcu_sys_para()->time_data));
		break;
	case MCU_SYS_PARA::INDEX_VIDEO:
		memcpy(get_mcu_sys_para()->video_data, pdata, sizeof(get_mcu_sys_para()->video_data));
		break;
	case MCU_SYS_PARA::INDEX_GENERAL:
		memcpy(get_mcu_sys_para()->general_data, pdata, sizeof(get_mcu_sys_para()->general_data));
		break;
	}
}


MCU_INIT_TABLE* protocol::get_mcu_init_table()
{
	static MCU_INIT_TABLE init_talbe;
	return &init_talbe;
}

void protocol::set_mcu_init_table(LPVOID pdata, int extra_data_len)
{
	if (pdata && extra_data_len>=0)
	{
		memcpy(get_mcu_init_table()->data, pdata, 17+extra_data_len);
		get_mcu_init_table()->extra_data_len = extra_data_len;
	}
}


MCU_VOLUME_TABLE* protocol::get_mcu_volume_table()
{
	static MCU_VOLUME_TABLE voume_table;
	return &voume_table;
}

void protocol::set_mcu_init_table(MCU_VOLUME_TABLE* pvolume_table)
{
	if (pvolume_table)
	{
		memcpy(get_mcu_volume_table(), pvolume_table, sizeof(MCU_VOLUME_TABLE));
	}
}

MCU_RADIO_STATUS* protocol::get_mcu_radio_status()
{
	static MCU_RADIO_STATUS radio_status;
	return &radio_status;
}

void protocol::set_mcu_radio_status(MCU_RADIO_STATUS* pstatus)
{
	if (pstatus)
	{
		memcpy(get_mcu_radio_status(), pstatus, sizeof(MCU_RADIO_STATUS));
	}
}

MCU_RADIO_PRESET_INFO* protocol::get_mcu_radio_presetinfo()
{
	static MCU_RADIO_PRESET_INFO preset_info;
	return &preset_info;
}

void protocol::set_mcu_radio_presetinfo(MCU_RADIO_PRESET_INFO* pinfo)
{
	if (pinfo)
	{
		memcpy(get_mcu_radio_status(), pinfo, sizeof(MCU_RADIO_PRESET_INFO));
	}
}

MCU_RADIO_PS_PTY* protocol::get_mcu_radio_ps_pty()
{
	static MCU_RADIO_PS_PTY radio_ps_pty;
	return &radio_ps_pty;
}

void protocol::set_mcu_radio_ps_pty(MCU_RADIO_PS_PTY* pps_pty)
{
	if (pps_pty)
	{
		memcpy(get_mcu_radio_ps_pty(), pps_pty, sizeof(MCU_RADIO_PS_PTY));
	}
}


MCU_RADIO_RT_INFO* protocol::get_mcu_radio_rt_info()
{
	static MCU_RADIO_RT_INFO radio_rt_info;
	return &radio_rt_info;
}

void protocol::set_mcu_radio_rt_info(MCU_RADIO_RT_INFO* prt_info)
{
	if (prt_info)
	{
		memcpy(get_mcu_radio_rt_info(), prt_info, sizeof(MCU_RADIO_RT_INFO));
	}
}


MCU_SWC_TABLE* protocol::get_mcu_swc_table()
{
	static MCU_SWC_TABLE swc_table;
	return &swc_table;
}

void protocol::set_mcu_swc_table(MCU_SWC_TABLE* pswc_table)
{
	if (pswc_table)
	{
		memcpy(get_mcu_swc_table(), pswc_table, sizeof(MCU_SWC_TABLE));
	}
}

MCU_ARM_STATUS* protocol::get_mcu_arm_status()
{
	static MCU_ARM_STATUS arm_status;
	return &arm_status;
}