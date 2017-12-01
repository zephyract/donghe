//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//

// @CESYSGEN IF SQLCE_MODULES_EDB

#define EDB_MOUNT_FLAG 0x80000000

#ifndef INOUT
#define INOUT
#endif
//-------------------------------------------------------------------------
// property flags
//
#define DB_PROP_NOTNULL                         (0x00000001)
#define DB_PROP_COMPRESSED                      (0x00000002)

//--------------------------------------------------------------------------

// Option Ids
//
#define DBOPT_SESSION_MAX_CHANGES              (0x01)
#define DBOPT_SESSION_LOCK_TIMEOUT             (0x02)

//--------------------------------------------------------------------------
// New tracking types
//
#define DB_TRACK_INSERT                         0x00000001 
#define DB_TRACK_UPDATE                         0x00000002
#define DB_TRACK_DELETE                         0x00000003

// tracking modes
//
#define DB_TRACK_LEVEL                          0x0000007C //1111100
#define DB_TRACK_LEVEL_RECORD                   0x00000004
#define DB_TRACK_LEVEL_PROPINCLUSIVE            0x00000008
#define DB_TRACK_LEVEL_PROPEXCLUSIVE            0x00000010
#define DB_TRACK_LEVEL_PROPVALCHANGE            0x00000020

// row identifier for tracking
//
#define DB_TRACK_ROWIDENTITY                    0x00000003 // 000011
#define DB_TRACK_ROWIDENTITY_PK                 0x00000001 

// sync session flags
//
#define DB_SYNCSESSION_COMPLETE                 0x00000000
#define DB_SYNCSESSION_PARTIAL                  0x00000001
#define DB_SYNCSESSION_FAILED                   0x00000002

#define DB_SYNCSESSION_TRACKSERVER              0x00000000
#define DB_SYNCSESSION_NOTRACK                  0x00000001
#define DB_SYNCSESSION_OVERRRIDELOCALCHANGES    0x00000002

//--------------------------------------------------------------------------
// tracking generations: CEDBGEN
//
typedef ULONGLONG   CEDBGEN;

//--------------------------------------------------------------------------
// default generation values
//
#define CEDBGEN_DEFAULT                         0xFFFFFFFFFFFFFFFF


//--------------------------------------------------------------------------
// CECHANGEINFO
//
typedef struct _CECHANGEINFO {
    DWORD               dwChangeType;
    SYSTEMTIME          stLastModified;    
    CEDBGEN             genLastModified;
} CECHANGEINFO, *PCECHANGEINFO;

//--------------------------------------------------------------------------
// CEDBISOLATIONLEVEL
//
typedef int CEDBISOLATIONLEVEL;

#define CEDB_ISOLEVEL_DEFAULT            1
#define CEDB_ISOLEVEL_READCOMMITTED      1
#define CEDB_ISOLEVEL_REPEATABLEREAD     2
#define CEDB_ISOLEVEL_SERIALIZABLE       3
#define CEDB_ISOLEVEL_LAST   CEDB_ISOLEVEL_SERIALIZABLE

//--------------------------------------------------------------------------
// Limits - apply to various volume options
//
#define CEDB_CB_MIN_BUFFER_POOL         0x00040000
#define CEDB_CB_MAX_BUFFER_POOL         0x0FFFF000
#define CEDB_AUTO_SHRINK_OFF            0x0
#define CEDB_AUTO_SHRINK_THRESHOLD_MAX  100
#define CEDB_FLUSH_INTERVAL_MIN         1
#define CEDB_FLUSH_INTERVAL_MAX         1000
#define CEDB_C_MIN_MAX_NOTIFY_CHANGES   0
#define CEDB_C_MAX_MAX_NOTIFY_CHANGES   0xFFFFFFFF


//--------------------------------------------------------------------------
// CCH_MAX_PASSWORD
//
#if !defined(CCH_MAX_PASSWORD)
#define CCH_MAX_PASSWORD             40
#endif


//--------------------------------------------------------------------------
// CEVOLUMEOPTIONS
//
// wVersion         -   Structure version
// cbBufferPool     -   Count of bytes in the buffer pool
// dwAutoShrinkPercent- Percent free used to kick off autoshrink
// dwFlushInterval  -   Interval between two flushes
// cMaxNotifyChanges-   Max number of notifications on a database to track
// dwDefaultTimeout -   Interval in milliseconds to 
//                      wait on an unavailable lock
// wszPassword      -   Volume password
// dwFlags          -   Contains a bitflag that specifies which of the
//                      fields contain valid data (see above for values)
// cMaxSize         -   Contains the maximum database size for the database in MB
//

#define CEVOLUMEOPTIONS_VERSION 1
#define CEVOLUMEOPTIONS_VERSIONEX 2

typedef struct _CEVOLUMEOPTIONS {
    WORD                wVersion;    
    DWORD               cbBufferPool;
    DWORD               dwAutoShrinkPercent;
    DWORD               dwFlushInterval;
    DWORD               cMaxNotifyChanges;
    DWORD               dwDefaultTimeout;
    WCHAR               wszPassword[CCH_MAX_PASSWORD + 1];
    DWORD               dwFlags;
    DWORD               cMaxSize;
} CEVOLUMEOPTIONS, *PCEVOLUMEOPTIONS;

//-------------------------------------------------------------------------
// Flags for CeMountDBVolEx
//-------------------------------------------------------------------------
// Used to overwrite the default buffer pool size
//
#define CEDB_BUFFERPOOL         0x00000001

// Used to overwrite the default auto shrink percent.
//
#define CEDB_AUTOSHRINKPERCENT  0x00000002

// Used to overwrite the default flush interval.
//
#define CEDB_FLUSHINTERVAL      0x00000004

// Used to overwrite the default maximum number of 
// individual notifications changes to track.
//
#define CEDB_MAXNOTIFYCHANGES   0x00000008

// Used to overwrite the default lock time out.
//
#define CEDB_DEFAULTTIMEOUT     0x00000010

// Used to indicate that the password field contains a password
//
#define CEDB_PASSWORD           0x00000020

// Used to overwrite the max pages value
//
#define CEDB_MAXSIZE           0x00000040

//-------------------------------------------------------------------------
// CEPROPSPEC
//
// wVersion - Structure version
// propid   - Property id
// dwFlags  - Combination of the following:
//              DB_PROP_NOTNULL     - the column does not accept nulls
//              DB_PROP_COMPRESSED  - the column should be compressed 
//                                    (valid for long values only)
// pwszPropName - The readable name of the property
// cchPropName  - The user will fill this field with the number of 
//              wide characters available in pwszPropName.
//
#define CEPROPSPEC_VERSION 1
typedef struct _CEPROPSPEC {
   WORD     wVersion;
   CEPROPID propid;
   DWORD    dwFlags;
   LPWSTR   pwszPropName;
   DWORD    cchPropName;
} CEPROPSPEC, *PCEPROPSPEC;


//--------------------------------------------------------------------------
// New functions
//
CEOID CeCreateDatabaseWithProps( 
    IN PCEGUID pGuid, 
    IN CEDBASEINFOEX* pInfo,
    IN DWORD cProps,
    IN CEPROPSPEC* prgProps);

BOOL CeAddDatabaseProps(
    IN PCEGUID pGuid, 
    IN CEOID oidDb,
    IN DWORD cProps,    
    IN CEPROPSPEC* prgProps);

BOOL CeGetDatabaseProps(
    IN HANDLE hHandle,
    INOUT WORD* pcPropId,
    IN CEPROPID* prgPropId,
    OUT CEPROPSPEC* prgProps);

BOOL CeRemoveDatabaseProps(
    IN PCEGUID pGuid, 
    IN CEOID oidDb,
    IN DWORD cPropID,
    IN CEPROPID* prgPropID);


BOOL CeMountDBVolEx( 
    INOUT PCEGUID pGuid,
    IN LPWSTR pwszDBVol,
    IN CEVOLUMEOPTIONS* pOptions,
    IN DWORD dwFlags);


// Session API's
HANDLE CeCreateSession(
    IN CEGUID* pGuid);

BOOL CeSetSessionOption(
    IN HANDLE hSession,
    IN ULONG ulOptionId,
    IN DWORD dwValue);

HANDLE CeGetDatabaseSession(
    IN HANDLE hDatabase);

BOOL CeBeginTransaction(
    IN HANDLE hSession,
    IN CEDBISOLATIONLEVEL isoLevel);

BOOL CeEndTransaction(
    IN HANDLE hSession,
    IN BOOL fCommit);

HANDLE CeOpenDatabaseInSession( 
    IN HANDLE hSession,
    IN PCEGUID pGuid, 
    IN PCEOID poid,
    IN LPWSTR pwszName, 
    IN SORTORDERSPECEX* pSort,
    IN DWORD dwFlags,
    IN CENOTIFYREQUEST* pRequest);


// Stream API's    

HANDLE CeOpenStream(
    IN HANDLE hDatabase,
    IN CEPROPID propid,
    IN DWORD dwMode);

BOOL CeStreamRead(
    IN HANDLE hStream,
    OUT BYTE* prgbBuffer,
    IN DWORD cbRead,
    OUT DWORD* pcbRead);

BOOL CeStreamWrite(
    IN HANDLE hStream,
    IN BYTE* prgbBuffer,
    IN DWORD cbWrite,
    OUT DWORD* pcbWritten);

BOOL CeStreamSaveChanges(
    IN HANDLE hStream);

BOOL CeStreamSeek(
    IN HANDLE hStream,
    IN DWORD cbMove,
    IN DWORD dwOrigin,
    OUT DWORD* pcbNewOffset);

BOOL CeStreamSetSize(
    IN HANDLE hStream,
    IN DWORD cbSize);


//-----------------------------------------------------------------------
// Tracking functions
//
BOOL CeAddSyncPartner(
    IN  PCEGUID     pVolGuid,
    IN  PCEGUID     pSyncPartnerGuid,
    IN  LPCWSTR     pwszFriendlyName,
    IN  LPCWSTR     pwszFullName  );
         
BOOL CeRemoveSyncPartner(
    IN  PCEGUID     pVolGuid,
    IN  PCEGUID     pSyncPartnerGuid );

BOOL CeTrackDatabase(
    IN  PCEGUID     pVolGuid,
    IN  PCEGUID     pSyncPartnerGuid,
    IN  CEOID       oidDB,
    IN  DWORD       dwTrackingFlags );

BOOL CeRemoveDatabaseTracking(
    IN  PCEGUID     pVolGuid,
    IN  PCEGUID     pSyncPartnerGuid,
    IN  CEOID       oidDB );
    
BOOL CeTrackProperty(
    IN  PCEGUID     pVolGuid,
    IN  CEOID       oidDB,
    IN  CEPROPID    propid,
    IN  BOOL        fAddToScheme );

BOOL CeBeginSyncSession(
    IN  HANDLE      hSession,
    IN  PCEGUID     pSyncPartnerGuid,
    IN  CEDBGEN     genFrom,
    IN  CEDBGEN     genTo,
    IN  DWORD       dwFlags,
    OUT CEDBGEN*    pGenCur );
    
BOOL CeEndSyncSession(
    IN  HANDLE      hSession,
    IN  DWORD       dwOutcome );

BOOL CeGetChangedRecordCnt(
    IN  HANDLE      hSession,
    IN  CEOID       oidDB,
    OUT DWORD*      pdwCnt );

HANDLE CeGetChangedRecords(
    IN  HANDLE      hSession,
    IN  CEOID       oidDB,
    IN  DWORD       dwChangeType  );

CEOID CeFindNextChangedRecord(
    IN  HANDLE      hChangeEnum );

BOOL CeGetPropChangeInfo(
    IN  HANDLE          hChangeEnum,
    IN  CEOID           oidChangedRecord,
    IN  CEPROPID        propid,
    OUT BOOL*           pfPropChanged );

BOOL CeGetRecordChangeInfo(
    IN  HANDLE           hChangeEnum,
    IN  CEOID            oidChangedRecord,
    OUT CECHANGEINFO* pInfo );

BOOL CeMarkRecord(
    IN  HANDLE          hChangeEnum,
    IN  CEOID           oidChangedRecord,
    IN  BOOL            fChanged );

BOOL CeAttachCustomTrackingData(
    IN  HANDLE          hDB,
    IN  CEOID           oidChangedRecord,
    IN  BYTE*           rgbData,
    IN  DWORD           ccb );
    
BOOL CeGetCustomTrackingData(
    IN      HANDLE          hChangeEnum,
    IN      CEOID           oidChangedRecord,
    INOUT   BYTE*           rgbData,
    INOUT   DWORD*          pccb );

BOOL CePurgeTrackingData(
    IN  PCEGUID     pVolGuid,
    IN  PCEGUID     pSyncPartnerGuid,
    IN  SYSTEMTIME* pstThreshold );

BOOL CePurgeTrackingGenerations(
    IN  PCEGUID     pVolGuid,
    IN  PCEGUID     pSyncPartnerGuid,
    IN  CEDBGEN     genThreshold);

//--------------------------------------------------------------------------
//

// @CESYSGEN ENDIF

