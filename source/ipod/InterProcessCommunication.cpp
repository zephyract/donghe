#include "stdafx.h"
#include "windows.h"
#include "interprocesscommunication.h"

/*
  This library is (c) Elias bachaalany <elias_bachaalany@yahoo.com>
  You may use it freely as long as you give credits to me.

  How it works?
  ================

  I am using a file mapping as the shared memory area
  Mutex is used to lock the memory access, although it is not neccessary...check next why:

  I am using two events to handle send/recv.
  A client cannot send and receive from itself nor a client can exist alone.
  Same for the server.

  This same Class can tell whether its current instance is a client or a server by checking who
  first initialzed the memory mapped file.

  How send/recv synchorination currently works?
  ===============================================

  Server
  ---------
  Read request->read when client_event = on
  Write request->signal server_event

  Client
  ----------
  Read request->read when server_event = on
  Write request->signal client_event

  See this interconnected logic ?
  Server cannot read from itself nor can the client.

  Current Limitations
  =======================
  Multiple sends will be satisfied with only when receive because I cannot queue buffers.
  Last sent buffer is the buffer that will be received.



  History
  ===========
  04/10/2003         * Added race protection mutex as suggested by Peter Chen.
                     * I want to stress that users are check for CInterProcessCommunication.GetLastError()
                     after the constructor is called.
                     LastError has same values that may be returned by Win32.GetLastError()
*/

CInterProcessCommunication::~CInterProcessCommunication()
{
  if (m_bIsServer)
    m_ipcd->ServerPID = NULL;
  else
    m_ipcd->ClientPID = NULL;

  if (m_lpMappedViewOfFile)
    UnmapViewOfFile(m_lpMappedViewOfFile);
  if (m_hFileMap)
    CloseHandle(m_hFileMap);
  if (m_hDataMutex)
    CloseHandle(m_hDataMutex);
  if (m_hClientEvent)
    CloseHandle(m_hClientEvent);
  if (m_hServerEvent)
    CloseHandle(m_hServerEvent);
}


CInterProcessCommunication::CInterProcessCommunication(LPCTSTR SharedName, DWORD Size,BOOL bHandlesInheritable, BOOL bIsServer)
: m_nSize(Size), m_bHandlesInheritable(bHandlesInheritable),m_bIsServer(bIsServer)
{

	// Initialize variables
	m_hDataMutex = 
		m_lpMem = 
		m_hFileMap = 
		m_lpMappedViewOfFile = 
		m_hClientEvent = 
		m_hServerEvent = 
		m_hRaceMutex = NULL;
	    m_bHandlesInheritable=FALSE;   

	// copy shared name
	_tcscpy(m_szSharedName, SharedName);

	// make object names
	MakeObjectsNames(SharedName);

	// Begin race protection
	if (!BeginRaceProtection())
		return;

	// First try to open mapped file
	m_hFileMap = OpenFileMapping(FILE_MAP_ALL_ACCESS,
		m_bHandlesInheritable,
		m_szFileMapName);

	// mapping object not there?
	// Assume that we are the server
	//m_bIsServer = (m_hFileMap == NULL);

	if (m_hFileMap == NULL)
	{
		// allocate a mapped file area for internal data storage
		// and for IPC buffer exchange
		m_hFileMap = CreateFileMapping((HANDLE)-1, 
			NULL,
			PAGE_READWRITE,
			NULL,
			Size + sizeof(tInterProcessCommunicationData),
			m_szFileMapName);
		LastError = ::GetLastError();
		if (m_hFileMap == NULL)
		{
			EndRaceProtection();
			return;
		}
	}

	// Now map the view of file
	m_lpMappedViewOfFile = 
		MapViewOfFile(m_hFileMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		m_nSize);

	LastError = ::GetLastError();

	// failed to map view of file?
	if (m_lpMappedViewOfFile == NULL)
	{
		CloseHandle(m_hFileMap);
		m_hFileMap = NULL;
		EndRaceProtection();
		return;
	}

	// Initialize internal data structure
	m_ipcd = (tInterProcessCommunicationData *) m_lpMappedViewOfFile;

	// Initialize buffer exchange memory
	m_lpMem  = (LPVOID)((DWORD)m_lpMappedViewOfFile + sizeof(tInterProcessCommunicationData));

	// Try to open mutex
	m_hDataMutex = OpenMutex(NULL,
		m_bHandlesInheritable, 
		m_szMutexName);

	// Mutex not found ? Create it
	if (m_hDataMutex == NULL)
	{
		LastError = ::GetLastError();

		// create mutex
		m_hDataMutex = CreateMutex(NULL,
			FALSE,
			m_szMutexName);

		LastError = ::GetLastError();

		// Could not create mutex?
		if (m_hDataMutex == NULL)
		{
			// close all objects ...
			UnmapViewOfFile(m_lpMappedViewOfFile);
			CloseHandle(m_hFileMap);
			EndRaceProtection();
			return;
		}
	}

	// store our process ID
	m_PID = GetCurrentProcessId();

	// initialize ipc data
	if (m_bIsServer)
		m_ipcd->ServerPID = m_PID;  
	else
		m_ipcd->ClientPID = m_PID;

	LastError = ERROR_SUCCESS;
	EndRaceProtection();
}



CInterProcessCommunication::CInterProcessCommunication(LPCTSTR SharedName, DWORD Size, BOOL bHandlesInheritable)
 : m_nSize(Size), m_bHandlesInheritable(bHandlesInheritable)
{
  
  // Initialize variables
  m_hDataMutex = 
  m_lpMem = 
  m_hFileMap = 
  m_lpMappedViewOfFile = 
  m_hClientEvent = 
  m_hServerEvent = 
  m_hRaceMutex = NULL;

  // copy shared name
  _tcscpy(m_szSharedName, SharedName);

  // make object names
  MakeObjectsNames(SharedName);

  // Begin race protection
  if (!BeginRaceProtection())
    return;

  // First try to open mapped file
  m_hFileMap = OpenFileMapping(FILE_MAP_ALL_ACCESS,
                               m_bHandlesInheritable,
                               m_szFileMapName);

  // mapping object not there?
  // Assume that we are the server
  m_bIsServer = (m_hFileMap == NULL);

  if (m_hFileMap == NULL)
  {
    // allocate a mapped file area for internal data storage
    // and for IPC buffer exchange
    m_hFileMap = CreateFileMapping((HANDLE)-1, 
                                   NULL,
                                   PAGE_READWRITE,
                                   NULL,
                                   Size + sizeof(tInterProcessCommunicationData),
                                   m_szFileMapName);
    LastError = ::GetLastError();
    if (m_hFileMap == NULL)
    {
      EndRaceProtection();
      return;
    }
  }

  // Now map the view of file
  m_lpMappedViewOfFile = 
            MapViewOfFile(m_hFileMap,
                          FILE_MAP_ALL_ACCESS,
                          0,
                          0,
                          m_nSize);

  LastError = ::GetLastError();

  // failed to map view of file?
  if (m_lpMappedViewOfFile == NULL)
  {
    CloseHandle(m_hFileMap);
    m_hFileMap = NULL;
    EndRaceProtection();
    return;
  }

  // Initialize internal data structure
  m_ipcd = (tInterProcessCommunicationData *) m_lpMappedViewOfFile;

  // Initialize buffer exchange memory
  m_lpMem  = (LPVOID)((DWORD)m_lpMappedViewOfFile + sizeof(tInterProcessCommunicationData));

  // Try to open mutex
  m_hDataMutex = OpenMutex(NULL,
                       m_bHandlesInheritable, 
                       m_szMutexName);

  // Mutex not found ? Create it
  if (m_hDataMutex == NULL)
  {
    LastError = ::GetLastError();

    // create mutex
    m_hDataMutex = CreateMutex(NULL,
                           FALSE,
                           m_szMutexName);

    LastError = ::GetLastError();

    // Could not create mutex?
    if (m_hDataMutex == NULL)
    {
      // close all objects ...
      UnmapViewOfFile(m_lpMappedViewOfFile);
      CloseHandle(m_hFileMap);
      EndRaceProtection();
      return;
    }
  }

  // store our process ID
  m_PID = GetCurrentProcessId();

  // initialize ipc data
  if (m_bIsServer)
    m_ipcd->ServerPID = m_PID;  
  else
    m_ipcd->ClientPID = m_PID;

  LastError = ERROR_SUCCESS;
  EndRaceProtection();
}

void CInterProcessCommunication::MakeObjectsNames(LPCTSTR szSharedName)
{
  wsprintf(m_szFileMapName, _T("FILEMAP_%s"), szSharedName);
  wsprintf(m_szMutexName, _T("MUTEX_%s"), szSharedName);

  // Set name of Mutex that will protect against call race
  _tcscpy(m_szRaceMutex, _T("IPC_RACE_MUTEX{DF9F578A-D4F8-45f3-8229-674094C60ECE}"));
}


void CInterProcessCommunication::ReceiveBuffer(LPVOID Buffer, DWORD Size)
{
  // Anything there to read?
  if (m_bIsServer)
    WaitForSingleObject(m_hClientEvent, INFINITE);
  else
    WaitForSingleObject(m_hServerEvent, INFINITE);

  // Acquire mutex
  WaitForSingleObject(m_hDataMutex, INFINITE);

  // Read from mapped memory
  memcpy(Buffer, m_lpMem, Size);

  // Release mutex
  ReleaseMutex(m_hDataMutex);
}

void CInterProcessCommunication::SendBuffer(LPVOID Buffer, DWORD Size)
{
  // Acquire mutex
  WaitForSingleObject(m_hDataMutex, INFINITE);

  // Write mapped memory
  memcpy(m_lpMem, Buffer, Size);

  // Release mutex
  ReleaseMutex(m_hDataMutex);

  // Signal that there is something to read
  if (m_bIsServer)
    SetEvent(m_hServerEvent);
  else
    SetEvent(m_hClientEvent);
}


bool CInterProcessCommunication::InterConnect()
{
  TCHAR  szEventName[40];
  bool   rc = false; 

  // Only one process can InterConnect() at a time
  BeginRaceProtection();

  // Update m_bFirstInst
  m_bIsServer = m_PID == m_ipcd->ServerPID;

  // Either client or server died!
  if (!m_ipcd->ClientPID || !m_ipcd->ServerPID)
    goto cleanup;

  // Create client
  wsprintf(szEventName, _T("CLIENTEVENT_%s_%d"), m_szSharedName, m_ipcd->ClientPID);
  m_hClientEvent = FetchEvent(szEventName);

  // failed to create event ?
  if (m_hClientEvent == NULL)
    goto cleanup;

  // Create server event
  wsprintf(szEventName, _T("SERVEREVENT_%s_%d"), m_szSharedName, m_ipcd->ServerPID);
  m_hServerEvent = FetchEvent(szEventName);

  // failed to create event ?
  if (m_hServerEvent == NULL)
    goto cleanup;

  rc = true;

cleanup:
  EndRaceProtection();
  return rc;
}

HANDLE CInterProcessCommunication::FetchEvent(LPCTSTR szName)
{
  HANDLE h;

  // Try to open mutex
  h = OpenEvent(EVENT_ALL_ACCESS,
                m_bHandlesInheritable, 
                szName);

  LastError = ::GetLastError();

  // event not found ? create it
  if (h == NULL)
  {
    // create new event
    h = CreateEvent(NULL,
                    FALSE,
                    FALSE,
                    szName);
    LastError = ::GetLastError();
  }
  return h;
}

inline tInterProcessCommunicationData * CInterProcessCommunication::GetIpcInternalData() const
{
  return m_ipcd;
}

DWORD CInterProcessCommunication::GetOtherProcessId() const
{
  if (m_ipcd->ClientPID == m_PID)
    return m_ipcd->ServerPID;
  else
    return m_ipcd->ClientPID;
}

DWORD CInterProcessCommunication::GetLastError() const
{
  return LastError;
}


bool CInterProcessCommunication::BeginRaceProtection()
{
  bool rc = true;

  // Try to open mutex
  m_hRaceMutex = OpenMutex(NULL, FALSE, m_szRaceMutex);

  // Mutex not found ? Create it
  if (m_hRaceMutex == NULL)
  {
    // create mutex
    m_hRaceMutex = CreateMutex(NULL, FALSE, m_szRaceMutex);
    LastError    = ::GetLastError();
    rc           = (LastError == ERROR_SUCCESS);
  }
  else
    LastError = ERROR_SUCCESS;

  // Acquire mutex
  if (rc)
    WaitForSingleObject(m_hRaceMutex, INFINITE);
  return rc;
}


void CInterProcessCommunication::EndRaceProtection()
{
  ReleaseMutex(m_hRaceMutex);
  CloseHandle(m_hRaceMutex);
  // Invalidate handle
  m_hRaceMutex = NULL;
}
