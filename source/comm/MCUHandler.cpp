#include "StdAfx.h"
#include "MCUHandler.h"
#include "interface.h"
#include "MainWnd.h"
#include "CanBus.h"
#include "sysUtil.h"

UINT CMCUHandler::m_nDigitalKey = 0;
// 用来处理MCU主动发送上来的数据
CMCUHandler::CMCUHandler()
{
}

CMCUHandler::~CMCUHandler()
{

}

void WINAPI CMCUHandler::MCU_OnReceive(LPVOID pData,int nDataLen, LPARAM lParam)
{
	CMCUHandler* phandler = (CMCUHandler*)lParam;
	phandler->OnReceive((BYTE*)pData, nDataLen);

}
void _DebugData(int mode, const void* pdata, int len);

// 如果一个包中收到K_VOL_UP, K_VOL_DN，将包中的所有该键值处理完，再处理其他命令
// 这两个键值容易一次发送多个，处理速度有点慢
void CMCUHandler::OnReceive(BYTE* pData,int nDataLen)
{
	HandleVolumeKey(pData, nDataLen);

	int pos = 0;
	while (pos < nDataLen)
	{
		// 找到起始符并且是一个合法的包
		if (pData[pos] == 0x02 && CObexMCU::IsValidObject((OBEX_Object*)(pData+pos), nDataLen-pos))		// 起始符
		{
			int object_size = CObexMCU::GetObjectSize((OBEX_Object*)(pData+pos));	// 不包含校验码
			HandleObject((OBEX_Object*)(pData+pos));
			pos += object_size + 1;
		}
		else
		{
			RETAILMSG(1, (L"[nvai_car]: CMCUHandler::OnReceive(), discard some data.\n"));

			pos++;
		}
	}
}

void CMCUHandler::HandleVolumeKey(BYTE* pData,int nDataLen)
{
	BOOL bFound = FALSE;
	int volume = 0;
	int pos = 0;
	while (pos < nDataLen)
	{
		OBEX_Object *pobject = (OBEX_Object*)(pData+pos);
		// 找到起始符并且是一个合法的包
		if (pobject->STX == 0x02 && CObexMCU::IsValidObject(pobject, nDataLen-pos))		// 起始符
		{
			if (pobject->CmdCode == 0xB2)	// key
			{
				UINT key = (pobject->Data[0]<<8)+pobject->Data[1];
				if (key == K_VOL_UP)
				{
					bFound = TRUE;
					volume++;
				}
				else if (key == K_VOL_DN)
				{
					bFound = TRUE;
					volume--;
				}

			}
			int object_size = CObexMCU::GetObjectSize((OBEX_Object*)(pData+pos));	// 不包含校验码
			pos += object_size + 1;
		}
		else
		{
			pos++;
		}
	}

	if (bFound)
	{
		if (volume >= 0)
		{
			OnKeyCommand(K_VOL_UP, abs(volume));
		}
		else
		{
			OnKeyCommand(K_VOL_DN, abs(volume));
		}
	}
}

void CMCUHandler::HandleObject(OBEX_Object *pobject)
{
	ASSERT(pobject);
	switch (pobject->CmdCode)
	{
	case 0xA6:	// 设备状态表
		OnStatusChange((MCU_STATUS*)pobject->Data);
		break;
	case 0xB2:
		{
			UINT key = (pobject->Data[0]<<8)+pobject->Data[1];
			UINT param = (pobject->Data[2]<<24)+(pobject->Data[3]<<16)+(pobject->Data[4]<<8)+pobject->Data[5];
			if (key != K_VOL_UP && key != K_VOL_DN)	// 这两个键已经处理了
			{
				OnKeyCommand(key, param);
			}
		}
		break;
	case 0xB9:	// 系统参数表
		OnSysParaChange(pobject->Data);
		break;
	case 0xC2:	// ACC off or on
		OnAccChange(pobject->Data[0]);
		break;
	case 0xB4:
		OnRadioData(pobject->DataLength-1, pobject->Data[0], pobject->Data+1);
		break;
	case 0xBB:	// SWC映射表
		OnSWCTable((MCU_SWC_TABLE*)pobject->Data);
		break;	
	case 0xBE:	// 采样结果
		OnSWCKeyResult(pobject->Data);
		break;
	case 0x08:	// can bus数据
		OnCanBusData(pobject->DataLength, pobject->Data);
		break;
	default:
		break;
	}
}

void CMCUHandler::OnCanBusData(int nDataLen, BYTE* pdata)
{
	switch (sysutil::nss_get_instance()->sys_canbus_type)
	{
	case CANBUS_TYPE_RAISE_DASAUTO:
		OnCanBus_RaiseDasAuto(nDataLen, pdata);
		break;
	case CANBUS_TYPE_RAISE_FOCUS:
		OnCanBus_RaiseFocus(nDataLen, pdata);
		break;
	case CANBUS_TYPE_RAISE_HONDA:
		OnCanBus_RaiseHonda(nDataLen, pdata);
		break;
	case CANBUS_TYPE_RAISE_PEUGEOT:
		OnCanBus_RaisePeugeot(nDataLen, pdata);
		break;
	case CANBUS_TYPE_RAISE_CAMRY:
		OnCanBus_RaiseCamry(nDataLen, pdata);
		break;
	case CANBUS_TYPE_RAISE_HYUNDAI:
		OnCanBus_RaiseHyundai(nDataLen, pdata);
		break;
	default:
		break;
	}
	
}



void CMCUHandler::OnSWCKeyResult(BYTE result[2])
{
	CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_SWC_KEY_RESULT, (WPARAM)0, (LPARAM)MAKELPARAM(result[0], result[1]));
}

static void cbTimer(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if (idEvent == TIMER_ID_KEY_CONFIRM)
	{
		CMainWnd::GetInstance()->KillTimer(TIMER_ID_KEY_CONFIRM);
		if (CMCUHandler::m_nDigitalKey > 0)
		{
			//CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_KEY_COMMAND, (WPARAM)T_IR_GOTO, 
			//	(LPARAM)CMCUHandler::m_nDigitalKey);
			CMCUHandler::m_nDigitalKey = 0;
		}
	}
}

BOOL CMCUHandler::HandleDigitalKey(UINT key, UINT param)
{
	key = key & 0xFF;	// 忽略按键状态
	if (key >= K_0 && key <= K_9)
	{
		if (m_nDigitalKey > 100000)	// 只支持最多六位数
		{
			m_nDigitalKey = 0;
		}
		m_nDigitalKey = m_nDigitalKey*10 + (key - K_0);
		CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_DIGITAL_KEY, (WPARAM)0, (LPARAM)m_nDigitalKey);

		CMainWnd::GetInstance()->SetTimer(TIMER_ID_KEY_CONFIRM, 3000, cbTimer);
		return TRUE;
	}
	else if (key == T_IR_GOTO)	// goto键,确认数字键输入完毕, 直接发键值给上层应用
	{
		if (m_nDigitalKey > 0)
		{
			CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_KEY_COMMAND, (WPARAM)key, (LPARAM)m_nDigitalKey);
			m_nDigitalKey = 0;
		}
		else	// 如果之前没按数字键, GOTO键当Enter键用
		{
			CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_KEY_COMMAND, (WPARAM)K_ENTER, 0);
		}
		return TRUE;
	}
	else	// 其它键,将之前的数字键清除
	{
		m_nDigitalKey = 0;
		return FALSE;
	}
		
}

void CMCUHandler::OnKeyCommand(UINT key, UINT param)
{
	if (!HandleDigitalKey(key, param))
	{
		CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_KEY_COMMAND, (WPARAM)key, (LPARAM)param);
	}
}

void CMCUHandler::OnStatusChange(MCU_STATUS *pstatus)
{
	protocol::set_mcu_status(pstatus);
	CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_STATUS_CHANGE, 0, (LPARAM)protocol::get_mcu_status());
}

void CMCUHandler::OnSysParaChange(BYTE* pdata)
{
	protocol::set_mcu_sys_para(pdata[0], pdata+1);
	CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_SYSPARAMS_CHANGE, 0, pdata[0]);
}

void CMCUHandler::OnAccChange(BOOL bAccOn)
{
	::PostMessage(HWND_BROADCAST, bAccOn ? NAVI_MSG_ACC_ON : NAVI_MSG_ACC_OFF, 0, 0);
}

void CMCUHandler::OnRadioData(int nDataLen, int nInfoType, BYTE* pdata)
{
	if (nInfoType == 0)		// 频点信息
	{
		if (nDataLen == 9)
		{
			MCU_RADIO_STATUS* pstatus = protocol::get_mcu_radio_status();
			pstatus->SetData(pdata);
		}
		else
		{
			RETAILMSG(1, (L"[nvai_car]: MCU send data error,command code=0xB4.\n"));
		}
	}
	else if (nInfoType == 1)	// preset list信息
	{
		if (nDataLen == 16)
		{
			MCU_RADIO_PRESET_INFO* ppreset = protocol::get_mcu_radio_presetinfo();
			ppreset->SetData(pdata);
		}
		else
		{
			RETAILMSG(1, (L"[nvai_car]: MCU send data error,command code=0xB4.\n"));
		}
	}
	else if (nInfoType == 2)		// PS, PTY
	{
		if (nDataLen == 9)
		{
			MCU_RADIO_PS_PTY* pps_pty = protocol::get_mcu_radio_ps_pty();
			pps_pty->SetData(pdata);
		}
		else
		{
			RETAILMSG(1, (L"[nvai_car]: MCU send data error,command code=0xB4.\n"));
		}
	}
	else if (nInfoType == 3)		// RT
	{
		MCU_RADIO_RT_INFO* prt_info = protocol::get_mcu_radio_rt_info();
		prt_info->SetData(pdata);
	}
	else
	{
		;// error data
	}

	CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_RADIO_DATA, 0, (LPARAM)nInfoType);

}

void CMCUHandler::OnSWCTable(MCU_SWC_TABLE* pswc_table)
{
	protocol::set_mcu_swc_table(pswc_table);
	CMainWnd::GetInstance()->PostMessage(UI_MSG_MCU_SWC_TABLE, 0, (LPARAM)protocol::get_mcu_swc_table());
}