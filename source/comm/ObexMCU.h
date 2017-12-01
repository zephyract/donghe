// ObexLD.h: interface for the CObexMCU class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBEXLD_H__86D67C3D_7AC9_4D78_8367_20FFEC1A6552__INCLUDED_)
#define AFX_OBEXLD_H__86D67C3D_7AC9_4D78_8367_20FFEC1A6552__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DEPSerial.h"

const size_t OBEX_OBJECT_HEAD_SIZE = 4;
const size_t MAX_PACKAGE	= 1024;	// 要大点，MCU有时会连续发多个包上来
// There is a LRC at the end. 
const size_t MAX_OBEX_OBJECT = OBEX_OBJECT_HEAD_SIZE + MAX_PACKAGE +1;	
const DWORD	 OBEX_TIMEOUT = 3000;


typedef struct 
{
	BYTE STX;
	BYTE CmdType;
	BYTE CmdCode;
	BYTE DataLength;
	// the actual size of Data field is determined by DataLength field.
	// put the LRC after the very last byte of Data.
	BYTE Data[1];
}OBEX_Object;

// para1 -- pointer to buffer that receive data
// para2 -- the number of bytes received
// para3 -- user defined value, set its value when calling RegCallback()
// refer the comment of RegCallback() function.
typedef void (WINAPI *OBEX_RECEIVE_CALLBACK)(OBEX_Object*, int, LPARAM);

// Refer the comment of RegExtraDataCallback() function
typedef void (WINAPI *EXTRADATA_RECEIVE_CALLBACK)(LPVOID, int, LPARAM);

class CObexMCU  
{
// Constructor
public:
	CObexMCU();
	virtual ~CObexMCU();

// Operations
public:
	static void WINAPI Obex_OnReceive(CDEPSerial::EEvent event, LPARAM lParam);
	// Initialize OBEX, it must be called before calling any other member functions.
	LONG Open(LPCTSTR lpszDevice);

	// Close OBEX.
	LONG Close();

	// Send object.
	LONG Send(OBEX_Object* pObject);

	// When recieved a full object, call this member to retreieve it.
	// LONG Receive(OBEX_Object* pObject);

	// 对收到的数据作个预处理,mcu上来的数据有可能包括之前超时命令的数据,
	// 或主动发上来的数据,把这部分数据先处理掉
	// 参数为接收缓冲区中接收数据的长度, 返回值为处理后缓冲区中的有效数据长度
	int PreHandleData(int nDataLen);

	// Called by framework when there is data in the buffer.
	void OnReceive();

	// Cancel the outstanding command request.
	BOOL CancelIo();

	// Register a callback function which will be called when received 
	// a valid object(waiting for a command reply).
	LONG RegOBEXDataCallback(OBEX_RECEIVE_CALLBACK fnCallback, LPARAM lParam);

	// Register a callback function which will be called when received 
	// extra data(it isnot waiting for a command reply, but some data received).
	LONG RegExtraDataCallback(EXTRADATA_RECEIVE_CALLBACK fnCallback, LPARAM lParam);

	// Get the pointer to DEP
	CDEPSerial* GetDEP(){ return &m_depSerial; };

	static size_t GetObjectSize(OBEX_Object *pObject);

	// Check if an object pointed to by pObject is valid. 
	// The parameter len here indicates the object's actual size according to PROTOCOL.
	// That's to say, it include a LRC check code at the end of the object.
	static BOOL IsValidObject(OBEX_Object* pObject, UINT len);

protected:
	// Check if it's processing a command request.
	BOOL IsCmdProcessing();
// Attributes
protected:
	LONG			m_lLastError;		// Last OBEX error
	OBEX_Object*	m_pObexObjectS;
	OBEX_Object*	m_pObexObjectR;
	CDEPSerial		m_depSerial;
	UINT			m_nResendCount;
	
	// Pointer to registered callback function.
	OBEX_RECEIVE_CALLBACK m_pfnOBEXDataCB;
	// The parameter for callback function.
	LPARAM m_lParamOBEX;

	EXTRADATA_RECEIVE_CALLBACK m_pfnExtraDataCB;
	LPARAM m_lParamExtra;

	HANDLE m_hMutex;	// for onReceive and Close operation's synchronize.
public:
	// Serailize obex operation. OBEX cannot send another object until the last
	// one has been processed or timeout.
	HANDLE m_hEventCmd;

};

#endif // !defined(AFX_OBEXLD_H__86D67C3D_7AC9_4D78_8367_20FFEC1A6552__INCLUDED_)
