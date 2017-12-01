#include "StdAfx.h"
#include "RpcMCU.h"

CRpcMCU::CRpcMCU()
{
	m_pObexObjectS		= NULL;
	m_pObexObjectR		= NULL;
	m_hEventReply		= NULL;
	m_hEventCancelCmd	= NULL;
	m_pObex			= NULL;
	m_pfnMsgHandlerCallback = NULL;
}

CRpcMCU::~CRpcMCU()
{
	// close RPC if it's still open.
	if( IsOpen() )
	{
		RETAILMSG(1, (L"CRpcMCULD::~CRpcMCULD - RPC not closed.\n"));
		// Close implicitly
		Close();
	}
}

CRpcMCU* CRpcMCU::GetInstance()
{
	static CRpcMCU rpc;
	return &rpc;
}

LONG CRpcMCU::Open(LPCTSTR lpszDevive)
{
	m_lLastError = ERROR_SUCCESS;
	// Check if it has already opened.
	if( IsOpen() )
	{
		m_lLastError = ERROR_ALREADY_INITIALIZED;
		RETAILMSG(1, (L"CRpcMCULD::Open - RpcLD has opened, close it first.\n"));
		return m_lLastError;
	}

	// Allocate output buffer.
	m_pObexObjectS = (OBEX_Object*)malloc(MAX_OBEX_OBJECT);
	if(m_pObexObjectS == NULL)
	{
		m_lLastError = ERROR_OUTOFMEMORY;
		RETAILMSG(1, (L"CRpcMCULD::CRpcMCULD - Nosufficient memory.\n"));
		goto LERRORS;
	}
	memset(m_pObexObjectS,0,MAX_OBEX_OBJECT);

	// Allocate input buffer.
	m_pObexObjectR = (OBEX_Object*)malloc(MAX_OBEX_OBJECT);
	if(m_pObexObjectR == NULL)
	{
		m_lLastError = ERROR_OUTOFMEMORY;
		RETAILMSG(1, (L"CRpcMCULD::CRpcMCULD - Nosufficient memory.\n"));
		goto LERRORS;
	}
	memset(m_pObexObjectR,0,MAX_OBEX_OBJECT);

	// Create the event handle for internal control.
	m_hEventReply = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hEventCancelCmd = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_hEventReply == 0 || m_hEventCancelCmd == 0)
	{
		m_lLastError = ::GetLastError();
		RETAILMSG(1, (L"CDEPSerial::Open - Unable to create event\n"));
		goto LERRORS;
	}

	// Construct OBEX and open it.
	m_pObex = new CObexMCU;
	if( m_pObex == NULL )
	{
		m_lLastError = ERROR_OUTOFMEMORY;
		RETAILMSG(1, (L"CRpcMCULD::Open - No sufficient memroy.\n"));
		goto LERRORS;
	}

	m_lLastError = m_pObex->Open(lpszDevive);
	if( m_lLastError != ERROR_SUCCESS)
	{
		RETAILMSG(1, (L"CRpcMCULD::Open - Cannot open ObexLD.\n"));
		goto LERRORS;
	}
	m_pObex->RegOBEXDataCallback(RPC_OnReceive, (LPARAM)this);


	m_pObex->RegExtraDataCallback(m_MCUHandler.MCU_OnReceive, (LPARAM)&m_MCUHandler);
LERRORS:
	LONG err = m_lLastError;
	if (err != ERROR_SUCCESS)
		Close();

	return err;
}

LONG CRpcMCU::Close()
{
	m_lLastError = ERROR_SUCCESS;

	// Check if it has already opened.
	if( !IsOpen() )
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		RETAILMSG(1, (L"CRpcMCULD::Close - RpcLD has not opened.\n"));
		return m_lLastError;
	}

	// Close ObexLD	
	if( m_pObex != NULL)
	{
		m_pObex->Close();
		delete m_pObex;
		m_pObex = NULL;
	}

	// Unregister MSG callback
	m_pfnMsgHandlerCallback = NULL;

	// free memory
	free(m_pObexObjectR);
	m_pObexObjectR = NULL;
	free(m_pObexObjectS);
	m_pObexObjectS = NULL;
	
	// close event
	if(m_hEventReply)
	{
		CloseHandle(m_hEventReply);
		m_hEventReply = NULL;
	}
	if(m_hEventCancelCmd)
	{
		CloseHandle(m_hEventCancelCmd);
		m_hEventCancelCmd = NULL;
	}

	return m_lLastError;
}

CDEPSerial* CRpcMCU::GetDEP()
{
	if (m_pObex)
		return m_pObex->GetDEP();

	return NULL;
}


void WINAPI CRpcMCU::RPC_OnReceive(OBEX_Object* pData,int nDataLen, LPARAM lParam)
{
	CRpcMCU* pRpcLD = (CRpcMCU*)lParam;
	pRpcLD->OnReceive(pData, nDataLen);
}

void CRpcMCU::OnReceive(OBEX_Object* pData,int nDataLen)
{
	// Copy received data.
	memcpy(m_pObexObjectR, pData, nDataLen);

	// Command request is replied, signal the reply event.
	SetEvent(m_hEventReply);
}

LONG CRpcMCU::CmdRequest(DWORD dwTimeout)
{
	const int TIMEOUT = 100;
	// the time out value could not be too small, or it will always fail to call RPC.
	// because MCU may take time longer than 100ms to send reply back.
//	_ASSERTE(dwTimeout >= TIMEOUT);
	// Send cmd package
	m_lLastError = m_pObex->Send(m_pObexObjectS);
	if(m_lLastError != ERROR_SUCCESS)
	{
		// Cancel the command so that obex can process next command.
		m_pObex->CancelIo();
		return m_lLastError;
	}

	// Reset the cancel command event.
	//ResetEvent(m_hEventCancelCmd);

	// Wait for reply from pos
	DWORD dwTickCount = GetTickCount();
	BOOL fLoop = TRUE;
	HANDLE pHandle[2] = {m_hEventReply, m_hEventCancelCmd};
	MSG msg;
	while( fLoop )
	{
		switch( MsgWaitForMultipleObjects(2,pHandle,FALSE,TIMEOUT,QS_ALLEVENTS) )
		{
		case WAIT_OBJECT_0:			// get the reply.
			fLoop = FALSE;
			break;
		case WAIT_OBJECT_0 + 1:		// cancel the command
			fLoop = FALSE;
			m_lLastError = ERROR_CANCELLED;
			break;
		case WAIT_OBJECT_0 + 2:		// message in queue, handle it.
			// handle message by calling callback function
			if( m_pfnMsgHandlerCallback )
				(*m_pfnMsgHandlerCallback)(m_lParam);

			// make sure marking msg queue from unread state to read state.
			// cause m_pfnMsgHandlerCallback may be NULL.
			PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);

			if( GetTickCount() - dwTickCount > dwTimeout )
			{
				// the total time consumed by handling message has exceed
				// dwTimeout, so we must exit the loop and return ERROR_TIMEOUT
				dwTimeout = 0;
				// go through next case section.
			}
			else
				break;
		case WAIT_TIMEOUT:			// time out
			if( dwTimeout > TIMEOUT )
				dwTimeout -= TIMEOUT;
			else
			{
				fLoop = FALSE;
				m_lLastError = ERROR_TIMEOUT;
			}
			break;
		default:
			fLoop = FALSE;
			m_lLastError = GetLastError();
			break;
		}
	}
	
	// Cancel the command so that obex can process next command.
	m_pObex->CancelIo();

	return m_lLastError;
}

void CRpcMCU::RegMSGCallback(RPC_MSGHANDLER_CALLBACK pfnCallback, LPARAM lParam) 
{ 
	if( m_pfnMsgHandlerCallback != NULL )
	{
		RETAILMSG(1, (L"CRpcMCULD::RegCallback - Registered a callback previously. It will be overwritten.\n"));
	}

	m_pfnMsgHandlerCallback = pfnCallback;
	m_lParam = lParam;
	return;
}


LONG CRpcMCU::RPC_ShakeHand(RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 500;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xA0;
	m_pObexObjectS->DataLength = 0;

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		*pRetCode = (m_pObexObjectR->Data[0] == 0x00) ? RPC_SUCCESS : RPC_FAIL;
	}

	return m_lLastError;
}


LONG CRpcMCU::RPC_GetInitTable(OUT void* pbuffer, OUT BYTE* pextra_data_len, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xA2;
	m_pObexObjectS->DataLength = 2;
	memcpy(m_pObexObjectS->Data, "\x01\x00",2);	// 协议版本号

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	//RETAILMSG(1, (L"******** RPC_GetInitTable() return %d. \n", m_lLastError));
	if( m_lLastError == ERROR_SUCCESS )
	{
		if(m_pObexObjectR->Data[0] == 0x00)
		{
			*pRetCode = RPC_SUCCESS;
			if (pbuffer && pextra_data_len)
			{
				if (m_pObexObjectR->DataLength >= 18)		// 根据协议超过了这个长度才有扩展信息
				{
					*pextra_data_len = m_pObexObjectR->DataLength-18;
				}
				else
				{
					*pextra_data_len = 0;
				}
				memcpy(pbuffer, m_pObexObjectR->Data+1, m_pObexObjectR->DataLength -1);
			}
		}
		else
		{
			*pRetCode = RPC_FAIL;
		}
	}
	else 
	{
		RETAILMSG(1, (L"[nvai_car]: RPC_GetInitTable() fail. errcode = %d. \n", m_lLastError));
	}


	return m_lLastError;
}

LONG CRpcMCU::RPC_GetStatus(MCU_STATUS* pMcuStatus, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xA4;
	m_pObexObjectS->DataLength = 0;

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		if(m_pObexObjectR->Data[0] == 0x00)
		{
			*pRetCode = RPC_SUCCESS;
			if (pMcuStatus)
			{
				memcpy(pMcuStatus, m_pObexObjectR->Data+1, sizeof(MCU_STATUS));
			}
			if (m_pObexObjectR->DataLength-1 != sizeof(MCU_STATUS))
			{
				RETAILMSG(1, (L"[nvai_car]: MCU send data error,command code=0xA4.\n"));
			}

			//protocol::set_mcu_status((MCU_STATUS *)m_pObexObjectR->Data[1]);
		}
		else
		{
			*pRetCode = RPC_FAIL;
		}
	}


	return m_lLastError;
}


LONG CRpcMCU::RPC_GetSysParams(int nIndex, OUT void* pBuffer, int nBufferLen, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xB7;
	m_pObexObjectS->DataLength = 1;
	m_pObexObjectS->Data[0] = (BYTE)nIndex;	// 要获取的参数的索引

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		if(m_pObexObjectR->Data[0] == 0x00)
		{
			*pRetCode = RPC_SUCCESS;
			if (pBuffer)
			{
				memcpy(pBuffer, m_pObexObjectR->Data+2, nBufferLen);
				if (nBufferLen != m_pObexObjectR->DataLength-2)
				{
					RETAILMSG(1, (L"[nvai_car]: MCU send data error,command code=0xB7.\n"));
				}
			}
			//protocol::set_mcu_sys_para(m_pObexObjectR->Data[1], m_pObexObjectR->Data+2);	// 表的第一个字节为索引
		}
		else
		{
			*pRetCode = RPC_FAIL;
		}
	}


	return m_lLastError;
}

LONG CRpcMCU::RPC_SetSysParams(int nIndex, void* pdata, int nLen, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL || pdata == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xB8;
	m_pObexObjectS->DataLength = nLen+1;	// 加上索引的长度
	m_pObexObjectS->Data[0] = (BYTE)nIndex;	// 参数的索引
	memcpy(m_pObexObjectS->Data+1, pdata, nLen);
	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		*pRetCode = (m_pObexObjectR->Data[0] == 0x00) ? RPC_SUCCESS : RPC_FAIL;
	}


	return m_lLastError;
}

LONG CRpcMCU::RPC_SetVolume(IN MCU_VOLUME_TABLE* pVolumeTable, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL || pVolumeTable == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xAA;
	m_pObexObjectS->DataLength = sizeof(MCU_VOLUME_TABLE);
	memcpy(m_pObexObjectS->Data, pVolumeTable, sizeof(MCU_VOLUME_TABLE));
	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		*pRetCode = (m_pObexObjectR->Data[0] == 0x00) ? RPC_SUCCESS : RPC_FAIL;
	}


	return m_lLastError;
}

LONG CRpcMCU::RPC_GetVolume(OUT MCU_VOLUME_TABLE* pVolumeTable, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xA9;
	m_pObexObjectS->DataLength = 0;

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		if(m_pObexObjectR->Data[0] == 0x00)
		{
			*pRetCode = RPC_SUCCESS;
			if (pVolumeTable)
			{
				memcpy(pVolumeTable, m_pObexObjectR->Data+1, sizeof(MCU_VOLUME_TABLE));
			}
		}
		else
		{
			*pRetCode = RPC_FAIL;
		}
	}

	return m_lLastError;
}


LONG CRpcMCU::RPC_SetSource(int nFrontSource, int nRearSource, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xA7;
	m_pObexObjectS->DataLength = 2;
	m_pObexObjectS->Data[0] = (BYTE)nFrontSource;
	m_pObexObjectS->Data[1] = (BYTE)nRearSource;

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		*pRetCode = (m_pObexObjectR->Data[0] == 0x00) ? RPC_SUCCESS : RPC_FAIL;
	}

	return m_lLastError;
}

LONG CRpcMCU::RPC_KeyCommand(UINT key, UINT param, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xB0;
	m_pObexObjectS->DataLength = 5;
	m_pObexObjectS->Data[0] = (BYTE)key;
	m_pObexObjectS->Data[1] = (param>>24) & 0xFF;
	m_pObexObjectS->Data[2] = (param>>16) & 0xFF;
	m_pObexObjectS->Data[3] = (param>>8) & 0xFF;
	m_pObexObjectS->Data[4] = (param) & 0xFF;

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		*pRetCode = (m_pObexObjectR->Data[0] == 0x00) ? RPC_SUCCESS : RPC_FAIL;
	}

	return m_lLastError;
}


LONG CRpcMCU::RPC_GetMcuVer(OUT TCHAR* pMcuVer, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xC3;
	m_pObexObjectS->DataLength = 0;

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		*pRetCode = (m_pObexObjectR->Data[0] == 0x00) ? RPC_SUCCESS : RPC_FAIL;

		if (pMcuVer)
		{
			char temp[33];
			int count = min(32, m_pObexObjectR->DataLength-1);	// 长度包含了返回码
			strncpy(temp, (char*)m_pObexObjectR->Data+1, count);
			temp[count] = '\0';
			MultiByteToWideChar (CP_ACP, 0, temp, -1, pMcuVer, 33);
		}
	}

	return m_lLastError;
}


LONG CRpcMCU::RPC_SWCCommand(int cmd, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 200;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xBD;
	m_pObexObjectS->DataLength = 1;
	m_pObexObjectS->Data[0] = (BYTE)cmd;

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		*pRetCode = (m_pObexObjectR->Data[0] == 0x00) ? RPC_SUCCESS : RPC_FAIL;
	}

	return m_lLastError;
}

LONG CRpcMCU::RPC_SWCGetTable(MCU_SWC_TABLE* ptable, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xBA;
	m_pObexObjectS->DataLength = 0;

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		if(m_pObexObjectR->Data[0] == 0x00)
		{
			*pRetCode = RPC_SUCCESS;
			if (ptable)
			{
				memcpy(ptable, m_pObexObjectR->Data+1, sizeof(MCU_SWC_TABLE));
			}
			if (m_pObexObjectR->DataLength-1 != sizeof(MCU_SWC_TABLE))
			{
				RETAILMSG(1, (L"[nvai_car]: MCU send data error,command code=0xBA.\n"));
			}
		}
		else
		{
			*pRetCode = RPC_FAIL;
		}
	}


	return m_lLastError;
}


LONG CRpcMCU::RPC_UpdateMCU(BOOL bStart, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 5000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xC4;
	m_pObexObjectS->DataLength = 1;
	m_pObexObjectS->Data[0] = bStart ? 1 : 0;

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		if(m_pObexObjectR->Data[0] == 0x01)	// 已准备好
		{
			*pRetCode = RPC_SUCCESS;
		}
		else
		{
			*pRetCode = RPC_FAIL;
		}
	}


	return m_lLastError;
}

LONG CRpcMCU::RPC_SendMCUData(LPVOID pData, UINT nDataLen, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL || pData == NULL || nDataLen == 0 || nDataLen > MAX_PACKAGE)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xC6;
	m_pObexObjectS->DataLength = nDataLen;
	memcpy(m_pObexObjectS->Data, pData, nDataLen);

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		if(m_pObexObjectR->Data[0] == 0x01)	// 继续发送后续数据
		{
			*pRetCode = RPC_SUCCESS;
		}
		else
		{
			*pRetCode = RPC_FAIL;
		}
	}


	return m_lLastError;
}


LONG CRpcMCU::RPC_GetRadioData(int type, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL )
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xB3;
	m_pObexObjectS->DataLength = 1;
	m_pObexObjectS->Data[0] = (BYTE)type;
	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		if(m_pObexObjectR->Data[0] == 0)	
		{
			*pRetCode = RPC_SUCCESS;
			if (m_pObexObjectR->Data[1] == 0)  // 频点信息
			{
				MCU_RADIO_STATUS* pstatus = protocol::get_mcu_radio_status();
				pstatus->SetData(m_pObexObjectR->Data + 2);
// 				BYTE* pdata = m_pObexObjectR->Data + 2;
// 				pstatus->min_freq = (pdata[0] << 8) + (WORD)pdata[1];
// 				pstatus->max_freq = (pdata[2] << 8) + pdata[3];
// 				pstatus->granularity = pdata[4];
// 				pstatus->signal = pdata[5];
// 				memcpy(pstatus->status, pdata+6, sizeof(pstatus->status));
// 				pstatus->rds_status = pdata[8];
			}
			else if (m_pObexObjectR->Data[1] == 1)  // preset list
			{
				MCU_RADIO_PRESET_INFO* ppreset = protocol::get_mcu_radio_presetinfo();
				ppreset->SetData(m_pObexObjectR->Data + 2);
// 				BYTE* pdata = m_pObexObjectR->Data + 2;
// 				ppreset->band = pdata[0];
// 				ppreset->preset_index = pdata[1];
// 				ppreset->cur_freq = (pdata[2] << 8) + pdata[3];
// 				for (int i=0; i<6; i++)
// 				{
// 					ppreset->preset_list[i] = (pdata[4+i*2] << 8) + pdata[5+i*2];
// 				}
			}
		}
		else
		{
			*pRetCode = RPC_FAIL;
		}
	}


	return m_lLastError;
}

LONG CRpcMCU::RPC_SetCanbusType(int type, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}

	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xCE;
	m_pObexObjectS->DataLength = 4;
	m_pObexObjectS->Data[0] = (BYTE)type;
	memset(m_pObexObjectS->Data+1, 0, 3);	// 高三个字节置0(保留字节)

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		if(m_pObexObjectR->Data[0] == 0x00)	
		{
			*pRetCode = RPC_SUCCESS;
		}
		else
		{
			*pRetCode = RPC_FAIL;
		}
	}


	return m_lLastError;
}

LONG CRpcMCU::RPC_SetCanbusData(LPVOID pData, UINT nDataLen, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 1000;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL || pData == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}


	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0x08;
	m_pObexObjectS->DataLength = nDataLen;
	memcpy(m_pObexObjectS->Data, pData, nDataLen);

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		if(m_pObexObjectR->Data[0] == 0x00)	
		{
			*pRetCode = RPC_SUCCESS;
		}
		else
		{
			*pRetCode = RPC_FAIL;
		}
	}

	return m_lLastError;
}


LONG CRpcMCU::RPC_SetArmStatus(MCU_ARM_STATUS* pstatus, RPCERR* pRetCode)
{
	const DWORD CMD_TIME_OUT = 200;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}


	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xA5;
	m_pObexObjectS->DataLength = sizeof(MCU_ARM_STATUS);
	memcpy(m_pObexObjectS->Data, pstatus, sizeof(MCU_ARM_STATUS));

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		if(m_pObexObjectR->Data[0] == 0x00)	
		{
			*pRetCode = RPC_SUCCESS;
		}
		else
		{
			*pRetCode = RPC_FAIL;
		}
	}

	return m_lLastError;
}


BYTE CalcLRC(BYTE *pStr, DWORD lLength);
LONG CRpcMCU::RPC_LedDisplayData(LPVOID pData, RPCERR* pRetCode)
{
	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xC7;
	m_pObexObjectS->DataLength = 16;
	memcpy(m_pObexObjectS->Data, pData, 16);
	// Add LRC check at the end of the object.
	DWORD size = m_pObexObjectS->DataLength + OBEX_OBJECT_HEAD_SIZE;
	BYTE lrc = CalcLRC(((BYTE*)m_pObexObjectS), size);
	((BYTE*)m_pObexObjectS)[size] = lrc;
	GetDEP()->Write(m_pObexObjectS, size+1);
	return 0L;



	const DWORD CMD_TIME_OUT = 1;
	m_lLastError = ERROR_SUCCESS;

	RPCERR RPCRet;
	pRetCode = (pRetCode != NULL) ? pRetCode : &RPCRet;

	// Check if RPC is open
	if( m_pObex == NULL || pData == NULL)
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		return m_lLastError;
	}


	// Assemble the cmd package
	m_pObexObjectS->STX = 0x02;
	m_pObexObjectS->CmdType = 0x80;
	m_pObexObjectS->CmdCode = 0xC7;
	m_pObexObjectS->DataLength = 16;
	memcpy(m_pObexObjectS->Data, pData, 16);

	// send command and wait result
	m_lLastError = CmdRequest(CMD_TIME_OUT);
	if( m_lLastError == ERROR_SUCCESS )
	{
		if(m_pObexObjectR->Data[0] == 0x00)	
		{
			*pRetCode = RPC_SUCCESS;
		}
		else
		{
			*pRetCode = RPC_FAIL;
		}
	}

	return m_lLastError;
}
