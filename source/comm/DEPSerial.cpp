// DEPSerial.cpp: implementation of the CDEPSerial class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "DEPSerial.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define XON			0x11
#define XOFF		0x13

CDEPSerial::CDEPSerial()
{
	// initialize member variables
	m_hFile			= 0;
	m_eEvent		= EEventNone;
	m_dwEventMask	= 0;
	m_lLastError	= ERROR_SUCCESS;
	m_fStopListen		= FALSE;
	m_hThread		= NULL;
	m_pfnEventCallback		= NULL;
	m_lParam		= 0;
	m_hevtOverlappedEvent = NULL;
	m_hevtOverlappedRead = NULL;
	m_hevtOverlappedWrite = NULL;
}

CDEPSerial::~CDEPSerial()
{
	if (m_hFile)
	{
		// Display a warning
		TRACE(L"CDEPSerial::~CSerial - Serial port not closed\n");

		// Close implicitly
		Close();
	}
}

LONG CDEPSerial::Open (LPCTSTR lpszDevice, DWORD dwInQueue, DWORD dwOutQueue, BOOL fOverlapped)
{
	// Reset error state
	m_lLastError = ERROR_SUCCESS;

	// Check if the port isn't already opened
	if (m_hFile)
	{
		m_lLastError = ERROR_ALREADY_INITIALIZED;
		TRACE(L"CDEPSerial::Open - Port already opened\n");
		return m_lLastError;
	}

	// Open the device
	m_hFile = ::CreateFile(lpszDevice,
						   GENERIC_READ|GENERIC_WRITE,
						   0,
						   0,
						   OPEN_EXISTING,
						   fOverlapped?FILE_FLAG_OVERLAPPED:0,
						   0);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		m_hFile = 0;

		m_lLastError = ::GetLastError();
		TRACE(L"CDEPSerial::Open - Unable to open port\n");
		return m_lLastError;
	}

	_ASSERTE(m_hevtOverlappedRead == 0);
	_ASSERTE(m_hevtOverlappedWrite == 0);
	_ASSERTE(m_hevtOverlappedEvent == 0);

	// Create the event handle for internal overlapped operations (manual reset)
	if (fOverlapped)
	{
		m_hevtOverlappedRead	= ::CreateEvent(0,TRUE,FALSE,0);
		m_hevtOverlappedWrite	= ::CreateEvent(0,TRUE,FALSE,0);
		m_hevtOverlappedEvent	= ::CreateEvent(0,TRUE,FALSE,0);
		if (m_hevtOverlappedEvent == 0
			|| m_hevtOverlappedRead == 0
			|| m_hevtOverlappedWrite == 0)
		{
			// Obtain the error information
			m_lLastError = ::GetLastError();
			TRACE(L"CDEPSerial::Open - Unable to create event\n");

			// Close the port
			::CloseHandle(m_hFile);
			m_hFile = 0;

			// Return the error
			return m_lLastError;
		}
	}

	// Setup the COM-port
	if (dwInQueue || dwOutQueue)
	{
		// Make sure the queue-sizes are reasonable sized. Win9X systems crash
		// if the input queue-size is zero. Both queues need to be at least
		// 16 bytes large.
		_ASSERTE(dwInQueue >= 16);
		_ASSERTE(dwOutQueue >= 16);

		if (!::SetupComm(m_hFile,dwInQueue,dwOutQueue))
		{
			long lLastError = ::GetLastError();
			TRACE(L"CDEPSerial::Open - Unable to setup the COM-port\n");

			Close();

			// Save last error from SetupComm
			m_lLastError = lLastError;
			return m_lLastError;	
		}
	}

	// Setup the default communication mask
	SetMask();

	// Blocking reads is default
	SetupReadTimeouts(EReadTimeoutBlocking);

// 	// Setup the device for default settings
//  	COMMCONFIG commConfig = {0};
// 	DWORD dwSize = sizeof(commConfig);
// 	commConfig.dwSize = dwSize;
// 	if (::GetDefaultCommConfig(lpszDevice,&commConfig,&dwSize))
// 	{
// 		// Set the default communication configuration
// 		if (!::SetCommConfig(m_hFile,&commConfig,dwSize))
// 		{
// 			// Display a warning
// 			TRACE(L"CDEPSerial::Open - Unable to set default communication configuration.\n");
// 		}
// 	}
// 	else
// 	{
// 		// Display a warning
// 		TRACE(L"CDEPSerial::Open - Unable to obtain default communication configuration.\n");
// 	}

	// set the default communication settings. (9600,8,N,1)
	Setup();

	return m_lLastError;
}


LONG CDEPSerial::Close()
{
	m_lLastError = ERROR_SUCCESS;

	// If the device is already closed,
	// then we don't need to do anything.
	if (m_hFile == 0)
	{
		TRACE(L"CDEPSerial::Close - Method called when device is not open\n");
		return m_lLastError;
	}

	// Stop listener
	StopListener();

	// Free event handle
	if (m_hevtOverlappedEvent)
	{
		::CloseHandle(m_hevtOverlappedEvent);
		m_hevtOverlappedEvent = 0;
	}
	if (m_hevtOverlappedRead)
	{
		::CloseHandle(m_hevtOverlappedRead);
		m_hevtOverlappedRead = 0;
	}
	if (m_hevtOverlappedWrite)
	{
		::CloseHandle(m_hevtOverlappedWrite);
		m_hevtOverlappedWrite = 0;
	}
	
	// Close COM port
	::CloseHandle(m_hFile);
	m_hFile = 0;

	return m_lLastError;
}

LONG CDEPSerial::SetupReadTimeouts (EReadTimeout eReadTimeout, 
									LPCOMMTIMEOUTS lpCommTimeouts)
{
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		TRACE(L"CDEPSerial::SetupReadTimeouts - Device is not opened\n");
		return m_lLastError;
	}

	// Determine the time-outs
	COMMTIMEOUTS cto;
	if (!::GetCommTimeouts(m_hFile,&cto))
	{
		m_lLastError = ::GetLastError();
		TRACE(L"CDEPSerial::SetupReadTimeouts - Unable to obtain timeout information\n");
		return m_lLastError;
	}

	// Set the new timeouts
	switch (eReadTimeout)
	{
	case EReadTimeoutBlocking:
		cto.ReadIntervalTimeout = 0;
		cto.ReadTotalTimeoutConstant = 0;
		cto.ReadTotalTimeoutMultiplier = 0;
		break;
	case EReadTimeoutNonblocking:
		cto.ReadIntervalTimeout = MAXDWORD;
		cto.ReadTotalTimeoutConstant = 0;
		cto.ReadTotalTimeoutMultiplier = 0;
		break;
	case EReadTimeoutCustom:
		if( lpCommTimeouts != NULL )
		{
			cto.ReadIntervalTimeout = lpCommTimeouts->ReadIntervalTimeout;
			cto.ReadTotalTimeoutConstant = lpCommTimeouts->ReadTotalTimeoutConstant;
			cto.ReadTotalTimeoutMultiplier = lpCommTimeouts->ReadTotalTimeoutMultiplier;
			break;
		} else
			// go through
	default:
		// This shouldn't be possible
		_ASSERTE(false);
		m_lLastError = ERROR_INVALID_PARAMETER;
		return m_lLastError;
	}

	if (!::SetCommTimeouts(m_hFile,&cto))
	{
		m_lLastError = ::GetLastError();
		TRACE(L"CDEPSerial::SetupReadTimeouts - Unable to set timeout information\n");
		return m_lLastError;
	}

	// Return successful
	return m_lLastError;
}

LONG CDEPSerial::Setup (EBaudrate eBaudrate, EDataBits eDataBits, EParity eParity, EStopBits eStopBits)
{
	m_lLastError = ERROR_SUCCESS;

	// Check if the device is open
	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		TRACE(L"CDEPSerial::Setup - Device is not opened\n");
		return m_lLastError;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		m_lLastError = ::GetLastError();
		TRACE(L"CDEPSerial::Setup - Unable to obtain DCB information\n");
		return m_lLastError;
	}

	// Set the new data
	dcb.BaudRate = DWORD(eBaudrate);
	dcb.ByteSize = BYTE(eDataBits);
	dcb.Parity   = BYTE(eParity);
	dcb.StopBits = BYTE(eStopBits);

	// Determine if parity is used
	dcb.fParity  = (eParity != EParNone);

	// Set the new DCB structure
	if (!::SetCommState(m_hFile,&dcb))
	{
		m_lLastError = ::GetLastError();
		TRACE(L"CDEPSerial::Setup - Unable to set DCB information\n");
		return m_lLastError;
	}

	return m_lLastError;
}


CDEPSerial::EBaudrate CDEPSerial::GetBaudrate (void)
{
	m_lLastError = ERROR_SUCCESS;

	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		TRACE(L"CDEPSerial::GetBaudrate - Device is not opened\n");
		return EBaudUnknown;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		m_lLastError = ::GetLastError();
		// Display a warning
		TRACE(L"CDEPSerial::GetBaudrate - Unable to obtain DCB information\n");
		return EBaudUnknown;
	}

	// Return the appropriate baudrate
	return EBaudrate(dcb.BaudRate);
}

CDEPSerial::EDataBits CDEPSerial::GetDataBits (void)
{
	m_lLastError = ERROR_SUCCESS;

	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		TRACE(L"CDEPSerial::GetDataBits - Device is not opened\n");
		return EDataUnknown;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		m_lLastError = ::GetLastError();
		TRACE(L"CDEPSerial::GetDataBits - Unable to obtain DCB information\n");
		return EDataUnknown;
	}

	// Return the appropriate bytesize
	return EDataBits(dcb.ByteSize);
}

CDEPSerial::EParity CDEPSerial::GetParity (void)
{
	m_lLastError = ERROR_SUCCESS;

	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		TRACE(L"CDEPSerial::GetParity - Device is not opened\n");
		return EParUnknown;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		m_lLastError = ::GetLastError();
		TRACE(L"CDEPSerial::GetParity - Unable to obtain DCB information\n");
		return EParUnknown;
	}

	// Check if parity is used
	if (!dcb.fParity)
	{
		// No parity
		return EParNone;
	}

	// Return the appropriate parity setting
	return EParity(dcb.Parity);
}

CDEPSerial::EStopBits CDEPSerial::GetStopBits (void)
{
	m_lLastError = ERROR_SUCCESS;

	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		TRACE(L"CDEPSerial::GetStopBits - Device is not opened\n");
		return EStopUnknown;
	}

	// Obtain the DCB structure for the device
	CDCB dcb;
	if (!::GetCommState(m_hFile,&dcb))
	{
		m_lLastError = ::GetLastError();
		TRACE(L"CDEPSerial::GetStopBits - Unable to obtain DCB information\n");
		return EStopUnknown;
	}

	// Return the appropriate stopbits
	return EStopBits(dcb.StopBits);
}

DWORD CDEPSerial::GetEventMask (void)
{
	m_lLastError = ERROR_SUCCESS;

	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		TRACE(L"CDEPSerial::GetEventMask - Device is not opened\n");
		return 0;
	}

	// Return the event mask
	return m_dwEventMask;
}

LONG CDEPSerial::Write (const void* pData, size_t iLen, DWORD* pdwWritten, 
						LPOVERLAPPED lpOverlapped, DWORD dwTimeout)
{
	m_lLastError = ERROR_SUCCESS;

	// Use our own variable for read count
	DWORD dwWritten;
	if (pdwWritten == 0)
		pdwWritten = &dwWritten;

	// Reset the number of bytes written
	*pdwWritten = 0;
	
	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		TRACE(L"CDEPSerial::Write - Device is not opened\n");
		return m_lLastError;
	}

	// Check if an overlapped structure has been specified
	if (!m_hevtOverlappedWrite && (lpOverlapped || (dwTimeout != INFINITE)))
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_FUNCTION;

		// Issue an error and quit
		TRACE(L"CDEPSerial::Write - Overlapped I/O is disabled, specified parameters are illegal.\n");
		return m_lLastError;
	}

	// Write the data
	if (!::WriteFile(m_hFile,pData,iLen,pdwWritten,/*lpOverlapped*/NULL))
	{
		// Set the internal error code
		long lLastError = ::GetLastError();

		// Save the error
		m_lLastError = lLastError;

		// Issue an error and quit
		TRACE(L"CDEPSerial::Write - Unable to write the data\n");
		return m_lLastError;
	}
	else
	{
		// The operation completed immediatly. Just to be sure
		// we'll set the overlapped structure's event handle.
		if (lpOverlapped)
			::SetEvent(lpOverlapped->hEvent);
	}

	if(m_lLastError == ERROR_SUCCESS)
		_ASSERTE(*pdwWritten == iLen);

	// Return successfully
	return m_lLastError;
}

LONG CDEPSerial::Read (void* pData, size_t iLen, DWORD* pdwRead, 
					   LPOVERLAPPED lpOverlapped, DWORD dwTimeout)
{
	m_lLastError = ERROR_SUCCESS;

	// Use our own variable for read count
	DWORD dwRead;
	if (pdwRead == 0)
		pdwRead = &dwRead;

	// Reset the number of bytes read
	*pdwRead = 0;
	
	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		TRACE(L"CDEPSerial::Read - Device is not opened\n");
		return m_lLastError;
	}

#ifdef _DEBUG
	// The debug version fills the entire data structure with
	// 0xDC bytes, to catch buffer errors as soon as possible.
	memset(pData,0xDC,iLen);
#endif

	// Check if an overlapped structure has been specified
	if (!m_hevtOverlappedRead && (lpOverlapped || (dwTimeout != INFINITE)))
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_FUNCTION;

		// Issue an error and quit
		TRACE(L"CDEPSerial::Read - Overlapped I/O is disabled, specified parameters are illegal.\n");
		return m_lLastError;
	}


	// Read the data
	if (!::ReadFile(m_hFile,pData,iLen,pdwRead,NULL))
	{
		long lLastError = ::GetLastError();

		m_lLastError = lLastError;
		TRACE(L"CDEPSerial::Read - Unable to read the data\n");
		return m_lLastError;
	}
	else
	{
		// The operation completed immediatly. Just to be sure
		// we'll set the overlapped structure's event handle.
		if (lpOverlapped)
			::SetEvent(lpOverlapped->hEvent);

	}

	return m_lLastError;
}

LONG CDEPSerial::Purge()
{
	m_lLastError = ERROR_SUCCESS;

	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		TRACE(L"CDEPSerial::Purge - Device is not opened\n");
		return m_lLastError;
	}

	// mcu回上来的数据时间和包都是随意的,有可能发下一命令时，缓冲区中有MCU主动上报的数据
	// 这里不清除接收缓冲区
	if (!::PurgeComm(m_hFile, PURGE_TXCLEAR /*| PURGE_RXCLEAR*/))
	{
		m_lLastError = ::GetLastError();
		TRACE(L"CDEPSerial::Purge - Overlapped completed without result\n");
	}

	return m_lLastError;
}

CDEPSerial::EEvent CDEPSerial::GetEventType (void)
{
#ifdef _DEBUG
	// Check if the event is within the mask
	if ((m_eEvent & m_dwEventMask) == 0)
		TRACE2("CDEPSerial::GetEventType - Event %08Xh not within mask %08Xh.\n", m_eEvent, m_dwEventMask);
#endif

	// Obtain the event (mask unwanted events out)
	EEvent eEvent = EEvent(m_eEvent & m_dwEventMask);

	// Reset internal event type
	m_eEvent = EEventNone;

	// Return the current cause
	return eEvent;
}

LONG CDEPSerial::SetMask (DWORD dwEventMask)
{
	m_lLastError = ERROR_SUCCESS;

	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		TRACE(L"CDEPSerial::SetMask - Device is not opened\n");
		return m_lLastError;
	}

	if (!::SetCommMask(m_hFile,dwEventMask))
	{
		m_lLastError = ::GetLastError();
		TRACE(L"CDEPSerial::SetMask - Unable to set event mask\n");
		return m_lLastError;
	}

	// Save event mask and return successful
	m_dwEventMask = dwEventMask;
	return m_lLastError;
}

LONG CDEPSerial::WaitEvent (LPOVERLAPPED lpOverlapped,DWORD dwTimeout)
{
	m_lLastError = ERROR_SUCCESS;

	// Check if an overlapped structure has been specified
	if (!m_hevtOverlappedEvent && (lpOverlapped || (dwTimeout != INFINITE)))
	{
		// Set the internal error code
		m_lLastError = ERROR_INVALID_FUNCTION;

		// Issue an error and quit
		TRACE(L"CDEPSerial::WaitEvent - Overlapped I/O is disabled, specified parameters are illegal.\n");
		return m_lLastError;
	}


	// Wait for the COM event
	if (!::WaitCommEvent(m_hFile,LPDWORD(&m_eEvent),NULL))
	{
		long lLastError = ::GetLastError();

		m_lLastError = lLastError;
		TRACE(L"CDEPSerial::WaitEvent - Unable to wait for COM event\n");
		return m_lLastError;
	}
	else
	{
		// The operation completed immediatly. Just to be sure
		// we'll set the overlapped structure's event handle.
		if (lpOverlapped)
			::SetEvent(lpOverlapped->hEvent);
	}

	return m_lLastError;
}



LONG CDEPSerial::StartListener (void)
{
	// Check if the watcher thread was already running
	if (m_hThread == 0)
	{
		// Make sure the thread has stopped
		_ASSERTE(!m_fStopListen);

		// Start the watcher thread
		m_hThread = ::CreateThread(0,0,ThreadProc,LPVOID(this),0,NULL);
		if (m_hThread == 0)
		{
			TRACE(L"CDEPSerial::StartListener - Unable to start COMM watcher thread\n");
			m_lLastError = ::GetLastError();
			return m_lLastError;
		}
	}

	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}


LONG CDEPSerial::StopListener (DWORD dwTimeout)
{
	BOOL bInfiniteFlag = FALSE;
	if (dwTimeout == INFINITE)
	{
		bInfiniteFlag = TRUE;
		dwTimeout = 100;
	}

	if (m_hThread)
	{
		// Set the flag that the thread must be stopped
		m_fStopListen = true;

		DWORD dwResult;

		// if set timeout internal to INFINITE, it will wait until thread exits.
		// otherwise, it will wait until thread exists or timeout(force to terminate
		// thread in this case).
		do 
		{
			// Cancel the pending WaitEvent, but we won't do this using
			// CancelIo. By setting the event mask again, the call will 
			// also be completed before the thread exits.
			SetMask(GetEventMask());
			
			// Wait until the watcher thread has stopped
			dwResult = ::WaitForSingleObject(m_hThread,dwTimeout);
			if (dwResult == WAIT_TIMEOUT && !bInfiniteFlag)
			{
				// force to terminate the thread
				::TerminateThread(m_hThread,0);
			}
		} while(dwResult == WAIT_TIMEOUT && bInfiniteFlag);

		// The thread has stopped
		m_fStopListen = false;

		// Close handle to the thread
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	// Return the error
	m_lLastError = ERROR_SUCCESS;
	return m_lLastError;
}

DWORD WINAPI CDEPSerial::ThreadProc (LPVOID lpArg)
{
	// Route the method to the actual object
	CDEPSerial* pThis = reinterpret_cast<CDEPSerial*>(lpArg);
	return pThis->ThreadProc();
}

DWORD CDEPSerial::ThreadProc (void)
{
	// Use overlapped structure
	LPOVERLAPPED lpOverlapped = 0;

	// Keep looping
	do
	{
		// Start the WaitEvent 
		if (WaitEvent() != ERROR_SUCCESS)
			return m_lLastError;

		// Wait until one of the events happens
		if (!m_fStopListen)
		{
			// Determine the event
			EEvent eEvent = GetEventType();

			// Obtain the error status during this event
			DWORD dwErrors = 0;
			COMSTAT	state;
			if (!::ClearCommError(m_hFile,&dwErrors,&state))
			{
				m_lLastError = ::GetLastError();
				TRACE(L"CDEPSerial::ThreadProc - Unable to obtain COM status\n");
			}

			// Convert the error
			EError eError = EError(dwErrors);

			// There was a COMM event, which needs handling. We'll call the
			// event handler. We can receive a "zero" event, when the
			// mask has been set. We won't pass this down.
			// if received EEventRecv and no data in received buff, ignore this event.
			if ( eEvent == EEventNone
				|| (eEvent == EEventRecv && state.cbInQue == 0) )
				continue;
			
			OnEvent(eEvent,eError);
		}
	}
	while (!m_fStopListen);

	return 0;
}


void CDEPSerial::OnEvent (EEvent eEvent, EError eError)
{
	// Event occurs, notify application by calling the registered
	// callback function.
	if( m_pfnEventCallback )
		(*m_pfnEventCallback)(eEvent, m_lParam);
}

UINT CDEPSerial::RegCallback(DEP_EVENT_CALLBACK proc, LPARAM lParam)
{
	m_lLastError = ERROR_SUCCESS;

	if (m_hFile == 0)
	{
		m_lLastError = ERROR_INVALID_HANDLE;
		TRACE(L"CDEPSerial::RegCallback - Device is not opened\n");
		return m_lLastError;
	}

	if( m_pfnEventCallback != NULL )
		TRACE(L"CDEPSerial::RegCallback - Registered a callback previously. It will be overwritten.\n");

	m_pfnEventCallback = proc;
	m_lParam = lParam;
	StartListener();
	return m_lLastError;
}

UINT CDEPSerial::UnRegCallback(DEP_EVENT_CALLBACK proc, LPARAM lParam)
{
	m_lLastError = ERROR_SUCCESS;

	if( m_pfnEventCallback == NULL )
	{
		TRACE(L"CDEPSerial::UnRegCallback - You haven't registered any callback yet.\n");
		return m_lLastError;
	}

	_ASSERTE(m_pfnEventCallback == proc && m_lParam == lParam);
	m_pfnEventCallback = 0;
	m_lParam = 0;
	StopListener();
	return m_lLastError;
}