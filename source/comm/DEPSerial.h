// DEPSerial.h: interface for the CDEPSerial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEPSerial_H__AD7530BC_8390_4323_8D43_DC2D48E535C6__INCLUDED_)
#define AFX_DEPSerial_H__AD7530BC_8390_4323_8D43_DC2D48E535C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDEPSerial  
{
	public:
		// Communication event
		typedef enum
		{
			EEventUnknown  	   = -1,			// Unknown event
			EEventNone  	   = 0,				// Event trigged without cause
			EEventBreak 	   = EV_BREAK,		// A break was detected on input
			EEventCTS   	   = EV_CTS,		// The CTS signal changed state
			EEventDSR   	   = EV_DSR,		// The DSR signal changed state
			EEventError 	   = EV_ERR,		// A line-status error occurred
			EEventRing  	   = EV_RING,		// A ring indicator was detected
			EEventRLSD  	   = EV_RLSD,		// The RLSD signal changed state
			EEventRecv  	   = EV_RXCHAR,		// Data is received on input
			EEventRcvEv 	   = EV_RXFLAG,		// Event character was received on input
			EEventSend		   = EV_TXEMPTY,	// Last character on output was sent
			EEventPrinterError = EV_PERR,		// Printer error occured
			EEventRx80Full	   = EV_RX80FULL,	// Receive buffer is 80 percent full
			EEventProviderEvt1 = EV_EVENT1,		// Provider specific event 1
			EEventProviderEvt2 = EV_EVENT2,		// Provider specific event 2
		} 
		EEvent;

	// Baudrate
	typedef enum
	{
		EBaudUnknown = -1,			// Unknown
		EBaud110     = CBR_110,		// 110 bits/sec
		EBaud300     = CBR_300,		// 300 bits/sec
		EBaud600     = CBR_600,		// 600 bits/sec
		EBaud1200    = CBR_1200,	// 1200 bits/sec
		EBaud2400    = CBR_2400,	// 2400 bits/sec
		EBaud4800    = CBR_4800,	// 4800 bits/sec
		EBaud9600    = CBR_9600,	// 9600 bits/sec (default)
		EBaud14400   = CBR_14400,	// 14400 bits/sec
		EBaud19200   = CBR_19200,	// 19200 bits/sec 
		EBaud38400   = CBR_38400,	// 38400 bits/sec
		EBaud56000   = CBR_56000,	// 56000 bits/sec
		EBaud57600   = CBR_57600,	// 57600 bits/sec
		EBaud115200  = CBR_115200,	// 115200 bits/sec
		EBaud128000  = CBR_128000,	// 128000 bits/sec
		EBaud256000  = CBR_256000,	// 256000 bits/sec
	}
	EBaudrate;

	// Data bits (5-8)
	typedef enum
	{
		EDataUnknown = -1,			// Unknown
		EData5       =  5,			// 5 bits per byte
		EData6       =  6,			// 6 bits per byte
		EData7       =  7,			// 7 bits per byte
		EData8       =  8			// 8 bits per byte (default)
	}
	EDataBits;

	// Parity scheme
	typedef enum
	{
		EParUnknown = -1,			// Unknown
		EParNone    = NOPARITY,		// No parity (default)
		EParOdd     = ODDPARITY,	// Odd parity
		EParEven    = EVENPARITY,	// Even parity
		EParMark    = MARKPARITY,	// Mark parity
		EParSpace   = SPACEPARITY	// Space parity
	}
	EParity;

	// Stop bits
	typedef enum
	{
		EStopUnknown = -1,			// Unknown
		EStop1       = ONESTOPBIT,	// 1 stopbit (default)
		EStop1_5     = ONE5STOPBITS,// 1.5 stopbit
		EStop2       = TWOSTOPBITS	// 2 stopbits
	} 
	EStopBits;

	// Timeout settings
	typedef enum
	{
		EReadTimeoutUnknown		= -1,	// Unknown
		EReadTimeoutNonblocking	=  0,	// Always return immediately
		EReadTimeoutBlocking	=  1,	// Block until everything is retrieved
		EReadTimeoutCustom		=  2	// Set customed time-out parameters for read
	}
	EReadTimeout;

	// Communication errors
	typedef enum
	{
		EErrorUnknown = 0,			// Unknown
		EErrorBreak   = CE_BREAK,	// Break condition detected
		EErrorFrame   = CE_FRAME,	// Framing error
		EErrorIOE     = CE_IOE,		// I/O device error
		EErrorMode    = CE_MODE,	// Unsupported mode
		EErrorOverrun = CE_OVERRUN,	// Character buffer overrun, next byte is lost
		EErrorRxOver  = CE_RXOVER,	// Input buffer overflow, byte lost
		EErrorParity  = CE_RXPARITY,// Input parity error
		EErrorTxFull  = CE_TXFULL	// Output buffer full
	}
	EError;

	typedef void (WINAPI *DEP_EVENT_CALLBACK)(CDEPSerial::EEvent event, LPARAM lParam);

// construction
public:
	CDEPSerial();
	virtual ~CDEPSerial();

// Operations
public:
	// Open the serial communications for a particular COM port. You
	// need to use the full devicename (i.e. "COM1") to open the port.
	// It's possible to specify the size of the input/output queues, if
	// both of ther are zero, the default settings are apllied.
	// It will open the serial communications in overlapped mode in default.
	LONG Open (LPCTSTR lpszDevice, DWORD dwInQueue = 0, DWORD dwOutQueue = 0, BOOL fOverlapped=FALSE);

	// Close the serial port.
	LONG Close();
	
	// Read operations can be blocking or non-blocking. You can use
	// this method to setup wether to use blocking or non-blocking
	// reads. Blocking reads is the default, which is required
	// for most applications.
	//
	// 1) Blocking reads, which will cause the 'Read' method to block
	//    until the requested number of bytes have been read.
	//    set eReadTimeout = EReadTimeoutNonblocking.
	// 2) Non-blocking reads, which will read as many bytes into your
	//    buffer and returns almost immediately. This is often the
	//    preferred setting.
	//    set eReadTimeout = EReadTimeoutBlocking.
	// 3) Customed Blocking reads, which will cause the 'Read' method to block
	//    until the requested number of bytes have been read or time out.
	//    set eReadTimeout = EReadTimeoutCustom and lpCommTimeouts must not be NULL.
	//    the member about write in lpCommTimeouts will be ingnored. 
	LONG SetupReadTimeouts (EReadTimeout eReadTimeout, 
							LPCOMMTIMEOUTS lpCommTimeouts = NULL);

	// Setup the communication settings such as baudrate, databits,
	// parity and stopbits. The default settings are applied when the
	// device has been opened. Call this function if these settings do
	// not apply for your application.
	LONG Setup (EBaudrate eBaudrate = EBaud9600,
				EDataBits eDataBits = EData8,
				EParity   eParity   = EParNone,
				EStopBits eStopBits = EStop1);

	// Obtain communication settings
	EBaudrate  GetBaudrate    (void);
	EDataBits  GetDataBits    (void);
	EParity    GetParity      (void);
	EStopBits  GetStopBits    (void);
	DWORD      GetEventMask   (void);

	// Write data to the serial port. 
	LONG Write (const void* pData, size_t iLen, DWORD* pdwWritten = 0,
				LPOVERLAPPED lpOverlapped = 0, DWORD dwTimeout = INFINITE);

	// Read data from the serial port.
	LONG Read (void* pData, size_t iLen, DWORD* pdwRead = 0, 
				LPOVERLAPPED lpOverlapped = 0, DWORD dwTimeout = INFINITE);

	// Obtain the COMM and event handle
	HANDLE GetCommHandle (void)		{ return m_hFile; }

	// Check if com-port is opened
	BOOL IsOpen (void) const		{ return (m_hFile != 0); }

	// Purge all buffers
	LONG Purge();
	
	// Set the event mask, which indicates what events should be
	// monitored. The WaitEvent method can only monitor events that
	// have been enabled.
	LONG SetMask (DWORD dwMask = EEventBreak|EEventError|EEventRecv);

	// Start the listener thread
	LONG StartListener (void);

	// Stop the listener thread. Because the other thread might be
	// busy processing data it might take a while, so you can specify
	// a time-out.
	LONG StopListener (DWORD dwTimeout = INFINITE);


	// Register a callback function which will be called by the framework
	// when monitored event occurs.
	UINT RegCallback(DEP_EVENT_CALLBACK proc, LPARAM lParam);

	// Stop the event notification.
	UINT UnRegCallback(DEP_EVENT_CALLBACK proc, LPARAM lParam);

	// Get last error.
	LONG GetLastError(){ return m_lLastError; }

protected:
	// Each opened COM-port uses its own specific thread, which will
	// wait for one of the events to happen. When an event happens,
	// the registered callback function will be called.
	static DWORD WINAPI ThreadProc (LPVOID lpArg);
	
	DWORD ThreadProc (void);

public:
	// The WaitEvent method waits for one of the events that are
	// enabled (see SetMask).
	LONG WaitEvent (LPOVERLAPPED lpOverlapped = NULL,DWORD dwTimeout = INFINITE);

	// Determine what caused the event to trigger
	EEvent GetEventType (void);

	// Event handler, Called by the framework to notify that event occurs.
	virtual void OnEvent(EEvent eEvent, EError eError);

protected:
	class CDCB : public DCB
	{
	public:
		CDCB() { DCBlength = sizeof(DCB); }
	};
// Attributes
protected:
	HANDLE	m_hFile;			// File handle
	EEvent	m_eEvent;			// Event type
	DWORD	m_dwEventMask;		// Event mask
	LONG	m_lLastError;		// Last serial error

	HANDLE	m_hevtOverlappedEvent;	// Event handle for internal overlapped operations
	HANDLE	m_hevtOverlappedRead;
	HANDLE	m_hevtOverlappedWrite;
	bool	m_fStopListen;
	HANDLE	m_hThread;

	// Pointer to registered callback function.
	DEP_EVENT_CALLBACK m_pfnEventCallback;
	// The parameter for callback function.
	LPARAM m_lParam;
};


#endif // !defined(AFX_DEPSerial_H__AD7530BC_8390_4323_8D43_DC2D48E535C6__INCLUDED_)
