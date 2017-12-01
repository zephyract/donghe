#ifndef INTERPROCESSCOMMUNICATION_H__
#define INTERPROCESSCOMMUNICATION_H__

#include <windows.h>
#include <tchar.h>

//---------------------------------------------------------------------
// Structure used by the class to manage its internal variables
// This struct is mapped into the common memory area between the two
// communicating processes
//
typedef struct
{
  DWORD ServerPID; // the PID of the process that last wrote
  DWORD ClientPID; // the first process
} tInterProcessCommunicationData;

//----------------------------------------------------------------------
// Class interface
//
class CInterProcessCommunication
{
public:
	DWORD  GetLastError() const;
	DWORD  GetOtherProcessId() const;
	tInterProcessCommunicationData * GetIpcInternalData() const;
	bool   InterConnect();
	inline HANDLE OpenFileMapping (DWORD dwAccess, BOOL bInherit, LPCTSTR lpName) 
	{
		return CreateFileMapping ((HANDLE)0xffffffff,
			NULL,
			PAGE_READWRITE,
			NULL,
			m_nSize + sizeof(tInterProcessCommunicationData), lpName);
	}

	HANDLE OpenMutex(
		DWORD dwDesiredAccess,
		BOOL bInheritHandle,
		LPCTSTR lpName
		)
	{
		return CreateMutex (NULL, bInheritHandle, lpName);
	}


	CInterProcessCommunication(LPCTSTR SharedName, DWORD Size, BOOL bHandlesInheritable = FALSE);
	CInterProcessCommunication(LPCTSTR SharedName, DWORD Size, BOOL bHandlesInheritable = FALSE,BOOL bIsServer= TRUE);
	~CInterProcessCommunication();
	void   SendBuffer(LPVOID Buffer, DWORD Size);
	void   ReceiveBuffer(LPVOID Buffer, DWORD Size);
private:
	void   EndRaceProtection();
	bool   BeginRaceProtection();

	DWORD  LastError;
	HANDLE FetchEvent(LPCTSTR szName);
	bool   m_bIsServer;
	tInterProcessCommunicationData *m_ipcd;
	void   MakeObjectsNames(LPCTSTR szSharedName);
	DWORD  m_nSize;
	DWORD  m_PID;
	HANDLE m_hFileMap;
	HANDLE m_hDataMutex; // memory protection mutex
	HANDLE m_hServerEvent; 
	HANDLE m_hClientEvent;
	HANDLE m_hRaceMutex;
	LPVOID m_lpMem;
	LPVOID m_lpMappedViewOfFile;
	TCHAR  m_szMutexName[80];
	TCHAR  m_szFileMapName[80];
	TCHAR  m_szSharedName[80];
	TCHAR  m_szRaceMutex[80];
	BOOL   m_bHandlesInheritable;

};

#endif