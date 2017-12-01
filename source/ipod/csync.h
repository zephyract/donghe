#ifndef __CSYNC_H_
#define __CSYNC_H_

class CLockObject
{
private:
    CRITICAL_SECTION    m_CSection;
public:
    CLockObject()
    {
        InitializeCriticalSection( &m_CSection );
    };
    virtual ~CLockObject()
    {
        DeleteCriticalSection( &m_CSection );
    };
    void Lock( void )
    {
        EnterCriticalSection( &m_CSection );
    };
    void Unlock( void )
    {
        LeaveCriticalSection( &m_CSection );
    };
};

class CSemaphore
{
public:
    CSemaphore(LONG lMaximumCount, LONG lInitialCount = 0, LPCTSTR lpName = NULL, LPSECURITY_ATTRIBUTES lpSemaphoreAttributes = NULL)
    {
        m_hObject = CreateSemaphore(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName); 
    };
    virtual ~CSemaphore()
    {
        if( m_hObject != NULL )
        {
            ::CloseHandle( m_hObject );
            m_hObject = NULL;
        }
    };

    BOOL Release(LONG lReleaseCount, LPLONG lpPreviousCount = NULL)
    {   
        return TRUE == ReleaseSemaphore(m_hObject, lReleaseCount, lpPreviousCount);
    }

    BOOL Lock( DWORD dwTimeOut = INFINITE )
    {
        return (::WaitForSingleObject( m_hObject, dwTimeOut ) == WAIT_OBJECT_0);
    };
    BOOL Unlock( void )
    {
        return ::ReleaseSemaphore(m_hObject, 1, NULL);
    };
        
private:
    HANDLE m_hObject;
};

class CMuTexObject
{
private:
    HANDLE  m_hObject;
public:
    CMuTexObject( BOOL bInitiallyOwn = FALSE,
        LPCTSTR lpszName = NULL,
        LPSECURITY_ATTRIBUTES lpsaAttribute = NULL )
    {
        m_hObject = ::CreateMutex( lpsaAttribute, bInitiallyOwn, lpszName );
    }
    virtual ~CMuTexObject()
    {
        if( m_hObject != NULL )
        {
            ::CloseHandle( m_hObject );
            m_hObject = NULL;
        }
    };
    BOOL Lock( DWORD dwTimeOut = INFINITE )
    {
        //return TRUE;
        return (::WaitForSingleObject( m_hObject, dwTimeOut ) == WAIT_OBJECT_0);
    };
    BOOL Unlock( void )
    {
        //return TRUE;
        return ::ReleaseMutex( m_hObject );
    };
};


#endif
