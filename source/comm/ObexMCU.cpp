// ObexCMU.cpp: implementation of the CObexMCU class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ObexMCU.h"


// mode : 0 ~ read, 1 ~ write
void _DebugData(int mode, const void* pdata, int len)
{
	LPCTSTR str = (mode==0) ? L"read:\n" : L"write:\n";
	RETAILMSG(1, (str));
	CString strData;
	TCHAR buffer[16];
	for (int i=0; i<len; i++)
	{
		if (i%16==0 && i>0)
		{
			strData += L"\n";
		}
		_stprintf(buffer, L"%02X ", ((BYTE*)pdata)[i]);
		strData += buffer;
	}
	strData += L"\n";
	RETAILMSG(1, (strData));
}

BYTE CalcLRC(BYTE *pStr, DWORD lLength)
{
	BYTE lrc = 0;
	for (int i=1; i<(int)lLength; i++)
	{
		lrc ^= pStr[i];
	}
	return lrc;
}

CObexMCU::CObexMCU()
{
	m_pObexObjectS = NULL;
	m_pObexObjectR = NULL;
	m_nResendCount = 0;
	m_pfnOBEXDataCB = NULL;
	m_lParamOBEX = 0;
	m_pfnExtraDataCB = NULL;
	m_lParamExtra = 0;
	m_hEventCmd = NULL;
	// create mutex
	m_hMutex = CreateMutex(NULL, FALSE, NULL);

}

CObexMCU::~CObexMCU()
{
	if( m_pObexObjectS )
	{
		TRACE(L"CObexMCU::~CObexMCU - OBEX not closed.\n");

		// Close implicitly
		Close();
	}

	if ( m_hMutex )
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
}

LONG CObexMCU::Open(LPCTSTR lpszDevice)
{
	m_lLastError = ERROR_SUCCESS;

	if( m_pObexObjectS != NULL || m_pObexObjectS != NULL )
	{
		m_lLastError = ERROR_ALREADY_INITIALIZED;
		TRACE(L"CObexMCU::Init - OBEX has been intialized.\n");
		return m_lLastError;
	}

	// Allocate memory for sending buffer and receiving buffer.
	m_pObexObjectS = (OBEX_Object*)malloc(MAX_OBEX_OBJECT);
	if( m_pObexObjectS == NULL )
	{
		m_lLastError = ERROR_OUTOFMEMORY;
		TRACE(L"CObexMCU::Init - No sufficient memroy.\n");
		return m_lLastError;
	}
	memset(m_pObexObjectS, 0, MAX_OBEX_OBJECT);

	m_pObexObjectR = (OBEX_Object*)malloc(MAX_OBEX_OBJECT);
	if( m_pObexObjectR == NULL )
	{
		m_lLastError = ERROR_OUTOFMEMORY;
		TRACE(L"CObexMCU::Init - No sufficient memroy.\n");
		free(m_pObexObjectS);
		return m_lLastError;
	}
	memset(m_pObexObjectR, 0, MAX_OBEX_OBJECT);

	// initialize dep
	m_lLastError = m_depSerial.Open(lpszDevice);
	if (m_lLastError != ERROR_SUCCESS)
		return m_lLastError;

	COMMTIMEOUTS cto;
	memset(&cto, 0 , sizeof(cto));
	cto.ReadIntervalTimeout = 20;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;
	m_depSerial.SetupReadTimeouts(CDEPSerial::EReadTimeoutCustom, &cto);

	m_depSerial.RegCallback(Obex_OnReceive, (LPARAM)this);

	// create event handle to serialize the obex operation.
	m_hEventCmd = CreateEvent(NULL, FALSE, TRUE, NULL);
	
	return m_lLastError;
}

LONG CObexMCU::Close()
{
	// Close DEP first.
	m_depSerial.Close();

	// avoid we close obex when it is processing received data.
	//WaitForSingleObject(m_hMutex,INFINITE);

	// Free allocated memory.
	if( m_pObexObjectS )
	{
		free(m_pObexObjectS);
		m_pObexObjectS = NULL;
	}

	if( m_pObexObjectR )
	{
		free(m_pObexObjectR);
		m_pObexObjectR = NULL;
	}

	// Close event
	if( m_hEventCmd )
	{
		CloseHandle(m_hEventCmd);
		m_hEventCmd = NULL;
	}

//	ReleaseMutex(m_hMutex);

	return ERROR_SUCCESS;
}

void CObexMCU::Obex_OnReceive(CDEPSerial::EEvent event, LPARAM lParam)
{
	// Data is received on input.
	if( event == CDEPSerial::EEventRecv )
	{
		CObexMCU *pObex = (CObexMCU*)lParam;
		pObex->OnReceive();
	}
	else
	{
		TRACE(L"CObexMCU::Obex_OnReceive - A line-status error occurred.\n");
	}

}

int CObexMCU::PreHandleData(int nDataLen)
{
	if( !IsCmdProcessing() )	// 如果没在命令处理中,不作任何处理
	{
		return nDataLen;
	}

	BYTE buffer[MAX_PACKAGE];
	int buffer_len = 0;
	BYTE* pData = (BYTE*)m_pObexObjectR;

	int pos = 0;
	while (pos < nDataLen)
	{
		OBEX_Object* pObject = (OBEX_Object*)(pData+pos);
		// 找到起始符并且是一个合法的包
		if (pData[pos] == 0x02 && CObexMCU::IsValidObject(pObject, nDataLen-pos))		// 起始符
		{
			int object_size = CObexMCU::GetObjectSize(pObject);	// 不包含校验码
			// 如果这个包是要处理的包,把数据保存起来,否则直接处理掉数据
			if ( (pObject->STX == m_pObexObjectS->STX
				&& pObject->CmdType == m_pObexObjectS->CmdType
				&& pObject->CmdCode == m_pObexObjectS->CmdCode)
				|| m_pObexObjectR->CmdCode == 0xC5)	// 协议设计有问题0xC4, 0xC6反复命令为0xC5,后面需更改MCU设计,这里对C5暂不判断
			{
				buffer_len = object_size+1;
				memcpy(buffer, pObject, buffer_len);
			}
			else
			{
				// 这种情况有可能是MCU没有回复该命令,直接发送了其它的数据,这种数据有可能是MCU主动上传的数据
				if (m_pfnExtraDataCB)
				{
					(*m_pfnExtraDataCB)(pObject, object_size+1, m_lParamExtra);
				}
			}
			pos += object_size + 1;
		}
		else
		{
			RETAILMSG(1, (L"[nvai_car]: CObexMCU::PreHandleData(), discard some data.\n"));
			pos++;
		}
	}

	if (buffer_len>0)
	{
		memcpy(m_pObexObjectR, buffer, buffer_len);
	}
	return buffer_len;
}

void CObexMCU::OnReceive()
{
	DWORD dwRead;
	void* pBuff;
	int nBuffLen = 0;
	
	WaitForSingleObject(m_hMutex,INFINITE);

	pBuff = m_pObexObjectR;
	nBuffLen = MAX_OBEX_OBJECT;

	m_lLastError = m_depSerial.Read(pBuff,nBuffLen,&dwRead);
	_ASSERTE(m_lLastError == ERROR_SUCCESS);

//	if (((BYTE*)pBuff)[2] != 0xC5)	// MCU升级回应包不打印
	{
		_DebugData(0, pBuff, dwRead);
	}


	// If we are not processing a command request, call extra data callback function
	// to handle it if registered one, otherwise discard all received data.
	if( !IsCmdProcessing() )
	{
		if (m_pfnExtraDataCB)
		{
			(*m_pfnExtraDataCB)(pBuff, dwRead, m_lParamExtra);
		}
		else
		{
			TRACE(L"CObexMCU::OnReceive - Data recieved,discard it because no cmd in request.\n");
			m_depSerial.Purge();
		}
		goto LEXIT;
	}	

	// Ok, read a full object, notify app to process it.
	dwRead = PreHandleData(dwRead);
	if( dwRead>0 && IsValidObject(m_pObexObjectR, dwRead) )
	{
		size_t object_size = GetObjectSize(m_pObexObjectR);	// 不包含校验码
		if ( (m_pObexObjectR->STX == m_pObexObjectS->STX
			&& m_pObexObjectR->CmdType == m_pObexObjectS->CmdType
			&& m_pObexObjectR->CmdCode == m_pObexObjectS->CmdCode)
			|| m_pObexObjectR->CmdCode == 0xC5)	// 协议设计有问题0xC4, 0xC6反复命令为0xC5,后面需更改MCU设计,这里对C5暂不判断
		{
			// call callback function.
			if( m_pfnOBEXDataCB )
				(*m_pfnOBEXDataCB)(m_pObexObjectR, object_size, m_lParamOBEX);
			else
				TRACE(L"CObexMCU::OnReceive - Object recieved but no callback function registered.\n");
		}
		else
		{
			TRACE(L"CObexMCU::OnReceive - Received Object mismatches command's.\n");
			// 这种情况有可能是MCU没有回复该命令,直接发送了其它的数据,这种数据有可能是MCU主动上传的数据
			if (m_pfnExtraDataCB)
			{
				(*m_pfnExtraDataCB)(pBuff, dwRead, m_lParamExtra);
			}
			goto LEXIT;
		}

		// 有可能接收的包比一个合法的包还要长，后面的数据作为Extra Data发送给上层
		int remain = dwRead - (object_size + 1);
		if (remain>0 && m_pfnExtraDataCB!=NULL)
		{
			// 保证RPC请求命令先执行,附加的数据后处理,这里延时下，让系统切到RPC进程处理
			// 否则可能后面的附加数据会先处理
			Sleep(100);
			(*m_pfnExtraDataCB)(((byte*)pBuff+object_size+1), remain, m_lParamExtra);
		}
	} 
	// Some errors occur, resend the cmd object but only try 3 times.
	else
	{
		if( ++m_nResendCount < 3 )
		{
//			Send(m_pObexObjectS);   // 取消重发机制,如果二个包上来的速度太快,可能引起RPC调用进程没机会处理上一个包,导致是否正在处理命令的状态不正确
			TRACE(L"CObexMCU::OnReceive - Received invalid Object,resend cmd.\n");
		}
	}

LEXIT:
	ReleaseMutex(m_hMutex);
}

BOOL CObexMCU::IsValidObject(OBEX_Object* pObject, UINT len)
{
	// It's too small, it must be an invalid object.
	if( len <= OBEX_OBJECT_HEAD_SIZE )
	{
		TRACE(L"CObexMCU::IsValidObject - Object is too small. len=%d\n",len);
		return FALSE;
	}

	size_t objext_size = GetObjectSize(pObject);
	// It must include a LRC check code in the end of the object.
	if( len < objext_size + 1 )
	{
		TRACE(L"CObexMCU::IsValidObject - Object does not include LRC or it's an invalid object.\n");
		TRACE(L" len = %d\n", len);
		return FALSE;
	}

	// Check if LRC is correct.
	BYTE lrc = CalcLRC(((BYTE*)pObject), objext_size);
	if( lrc != ((BYTE*)pObject)[objext_size])
	{
		TRACE(L"CObexMCU::IsValidObject - LRC check errors.\n");
		TRACE(L"LRC = %x, lrc in received package is %x\n", lrc, ((BYTE*)pObject)[len-1]);
		return FALSE;
	}

	return TRUE;
}

LONG CObexMCU::Send(OBEX_Object* pObject)
{
	m_lLastError = ERROR_SUCCESS;

	if ( m_pObexObjectS == NULL )
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		TRACE(L"CObexMCU::Send - Send buffer is empty, call Init() first.\n");
		return m_lLastError;
	}

	if( pObject == NULL )
	{
		m_lLastError = ERROR_INVALID_PARAMETER;
		TRACE(L"CObexMCU::Send - OBEX object cannot be NULL, specified parameters are illegal.\n");
		return m_lLastError;
	}
	
	// Clear buffers before send a new cmd.
	m_depSerial.Purge();
	
	// Save the sent object and calculat LRC.
	if( pObject != m_pObexObjectS)
	{
		// serailize this operation.
		switch( WaitForSingleObject(m_hEventCmd, OBEX_TIMEOUT) )
		{
		case WAIT_OBJECT_0:
			break;
		case WAIT_TIMEOUT:
			m_lLastError = ERROR_TIMEOUT;
			return m_lLastError;
		default:
			m_lLastError = GetLastError();
			return m_lLastError;
		}
		
		size_t size = GetObjectSize(pObject);
		memcpy(m_pObexObjectS, pObject, size);

		
		// Add LRC check at the end of the object.
		BYTE lrc = CalcLRC(((BYTE*)m_pObexObjectS), size);
		((BYTE*)m_pObexObjectS)[size] = lrc;

		m_nResendCount = 0;
	}

	// Send the object, including the LRC check code following 
	// the very last byte of the object.
	m_lLastError = m_depSerial.Write(m_pObexObjectS, GetObjectSize(m_pObexObjectS)+1);

	if (m_pObexObjectS->CmdCode != 0xC6) // 这个命令数据太多,不打印
	{
		_DebugData(1, m_pObexObjectS, GetObjectSize(m_pObexObjectS)+1);
	}

	return m_lLastError;
}

size_t CObexMCU::GetObjectSize(OBEX_Object *pObject)
{
	size_t size = 0;
	if ( pObject != NULL )
	{
		size = pObject->DataLength + OBEX_OBJECT_HEAD_SIZE;		// 包头+数据域长度，不包括校验码
	}
	return size;
}


LONG CObexMCU::RegOBEXDataCallback(OBEX_RECEIVE_CALLBACK proc, LPARAM lParam)
{
	m_lLastError = ERROR_SUCCESS;

	if( m_pfnOBEXDataCB != NULL )
	{
		TRACE(L"CObexMCU::RegCallback - Registered a callback previously. It will be overwritten.\n");
	}
	m_pfnOBEXDataCB = proc;
	m_lParamOBEX = lParam;
	return m_lLastError;
}



LONG CObexMCU::RegExtraDataCallback(EXTRADATA_RECEIVE_CALLBACK proc, LPARAM lParam)
{
	m_lLastError = ERROR_SUCCESS;

	if( m_pfnExtraDataCB != NULL )
	{
		TRACE(L"CObexMCU::RegExtraDataCallback - Registered a callback previously. It will be overwritten.\n");
	}
	m_pfnExtraDataCB = proc;
	m_lParamExtra = lParam;
	return m_lLastError;
}


BOOL CObexMCU::CancelIo()
{
	WaitForSingleObject(m_hMutex,INFINITE);

	if ( m_pObexObjectS == NULL )
	{
		m_lLastError = ERROR_INVALID_OPERATION;
		TRACE(L"CObexMCU::Send - Send buffer is empty, call Init() first.\n");
		return FALSE;
	}

	// Clear sending buffer. After calling CancelIo(), it will discard
	// all the received data.
	memset(m_pObexObjectS, 0, OBEX_OBJECT_HEAD_SIZE);

	// Clear buffers.
	m_depSerial.Purge();

	// now we can send another command object.
	SetEvent(m_hEventCmd);

	ReleaseMutex(m_hMutex);
	return TRUE;
}

// if OBEX is not in cmd processing, all data received will be discarded.
BOOL CObexMCU::IsCmdProcessing()
{
	char buff[OBEX_OBJECT_HEAD_SIZE] = {0,};
	BOOL ret;
	if (m_pObexObjectS != NULL )
		ret = memcmp(m_pObexObjectS, buff, OBEX_OBJECT_HEAD_SIZE);
	else	// maybe obex has been closed, or not initialized yet.
		ret = FALSE;

	return  ret; 
};