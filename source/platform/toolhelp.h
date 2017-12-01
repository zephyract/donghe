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
#ifndef __TOOLHELP_H__
#define __TOOLHELP_H__

#include "tlhelp32.h"

#ifdef __cplusplus
extern "C" {
#endif

#define THSNAP_RESERVE (1024*1024)

#define TH32CS_SNAPNOHEAPS	0x40000000	// optimization for text shell to not snapshot heaps

//
// We need to make Toolhelp snapshot position independent so we can easily pass it from one
// process to the other to fill heap informations. Here's the new layout:
//
//
//          ----------------------------
//          | Snapshot header (THSNAP) |
//          ----------------------------
//          |                          |
//          | array of PROCESSENTRY32  |
//          |   dwProcCnt elements     |
//          |                          |
//          ----------------------------
//          |                          |
//          |  array of MODULEENTYR32  |
//          |   dwModCnt elements      |
//          |                          |
//          ----------------------------
//          |                          |
//          |  array of THREADENTRY32  |
//          |   dwThrdCnt elements     |
//          |                          |
//          ----------------------------
//          |                          |
//          |    TH32HEAPLISTs         |
//          |  dwHeapListCnt elements  |
//          |                          |
//          ----------------------------


// pNextHeapList = (PTH32HEAPLIST) ((DWORD) pCurrHeapList + pCurHeap->dwTotalSize)
// Validation: pheapList->dwTotalSize == sizeof (TH32HEAPLIST) + dwHeapEntryCnt * sizeof (HEAPENTRY32)
typedef struct TH32HEAPLIST {
	HEAPLIST32  heaplist;
    DWORD       dwHeapEntryCnt;
    DWORD       dwTotalSize;
//  HEAPENTRY32 heapEntries[0];         // heap entries follow
} TH32HEAPLIST, *PTH32HEAPLIST;	

typedef struct THSNAP {
    DWORD   cbSize;             // == sizeof (THSNAP)
	DWORD   cbInuse;            // inuse count == total size of the snapshot
	DWORD   cbCommit;           // total size committed, >= cbInuse
	DWORD   cbReserved;         // total size reserved, >= cbCommit
    DWORD   dwProcCnt;          // # of process snap (array of TH32PROC right after snap header)
    DWORD   dwModCnt;           // # of Module snap (array of MODULEENTRY32 after Processes)
    DWORD   dwThrdCnt;          // # of Thread snap (array of THREADENTRY32 after Modules)
    DWORD   dwFlags;            // flags of this snap

    // the following are never touched by kernel when snapshot is created, it'll be updated in toolhelp function
    // to add heap entries and keep track of what to return on xxxNext functions

    DWORD   dwHeapListCnt;      // # of Heap list snap (variable size, use formula above to calculate next entry)

    DWORD   dwProcIdx;          // index of process 
    DWORD   dwModIdx;           // index of module
    DWORD   dwThrdIdx;          // index of thread

    DWORD   dwOfstHeapList;     // offset of heap list (it's variable length, can't simply use index)
    DWORD   dwOfstEntryList;    // the ofst of HeapList that is found in Heap32First
    DWORD   dwHeapEntryIdx;     // index of heap entry

} THSNAP, *PTHSNAP;

// useful macros
#define TH32FIRSTPROC(pSnap)    ((PPROCESSENTRY32) ((pSnap) + 1))
#define TH32FIRSTMOD(pSnap)     ((PMODULEENTRY32) (TH32FIRSTPROC(pSnap) + ((pSnap)->dwProcCnt)))
#define TH32FIRSTTHRD(pSnap)    ((PTHREADENTRY32) (TH32FIRSTMOD(pSnap) + ((pSnap)->dwModCnt)))
#define TH32FIRSTHEAP(pSnap)    ((PTH32HEAPLIST) (TH32FIRSTTHRD(pSnap) + (pSnap)->dwThrdCnt))

#define TH32NEXTPROC(pprc)      ((pprc)+1)
#define TH32NEXTTHRD(pth)       ((pth)+1)
#define TH32NEXTMOD(pMod)       ((pMod)+1)
#define TH32NEXTHEAPLIST(php)   ((PTH32HEAPLIST) ((DWORD) (php) + (php)->dwTotalSize))

#define TH32FIRSTHEAPITEM(php)  ((PHEAPENTRY32) (php+1))


#ifdef __cplusplus
}
#endif

#endif

