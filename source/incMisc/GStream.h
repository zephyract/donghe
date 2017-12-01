/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008 MediaTek Inc.
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
*
* Filename:
* ---------
* file GStream.h
*
* Project:
* --------
*   CNB
*
* Description:
* ------------
*
*
* Author:
* -------
*
*
*------------------------------------------------------------------------------
* $Revision: #1 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _GSTREAM_H_
#define _GSTREAM_H_

#include <GDef.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief  creat stream from file
*
* @param[in] szFileName file name
*
* @return GHANDLE define in GDef.h
*			   
**/
GHANDLE     GCreateStreamFromFile(GTCHAR *szFileName);
/**
* @brief  creat stream from buffer
*
* @param[in] pvBuf buf address
* @param[in] u4Size buf size
*
* @return GHANDLE define in GDef.h
*			   
**/
GHANDLE     GCreateStreamFromBuffer(CONST VOID *pvBuf, GUINT32 u4Size);

/**
* @brief  destroy stream
*
* @param[in] hStream stream handle
*
* @return NONE
*			   
**/
GVOID       GDestroyStream(GHANDLE hStream);
#include <winioctl.h>

//add for mtp file support by mtk40601
#define  IOCTL_MTP_GET_FILE_NUM                CTL_CODE(FILE_DEVICE_UNKNOWN, 0xBB8,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define  IOCTL_MTP_GET_FILE_NAME               CTL_CODE(FILE_DEVICE_UNKNOWN, 0xBB9,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define  IOCTL_MTP_SET_READ_POSITION           CTL_CODE(FILE_DEVICE_UNKNOWN, 0xBBA,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define  IOCTL_MTP_GET_FILE_DATA               CTL_CODE(FILE_DEVICE_UNKNOWN, 0xBBB,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define  IOCTL_MTP_GET_HANDLE_LIST             CTL_CODE(FILE_DEVICE_UNKNOWN, 0xBBC,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define  IOCTL_MTP_GET_FILE_ATTRIBUTE          CTL_CODE(FILE_DEVICE_UNKNOWN, 0xBBD,METHOD_BUFFERED,FILE_ANY_ACCESS)                   
#define  IOCTL_MTP_GET_FILE_SIZE               CTL_CODE(FILE_DEVICE_UNKNOWN, 0xBBE,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define  IOCTL_MTP_OPEN_FILE                   CTL_CODE(FILE_DEVICE_UNKNOWN, 0xBBF,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define  IOCTL_MTP_GET_READ_POSITION           CTL_CODE(FILE_DEVICE_UNKNOWN, 0xBC0,METHOD_BUFFERED,FILE_ANY_ACCESS)


typedef struct _MTP_FILE_POINTERIFO
{
    DWORD dwFileHandle;
    LONG lDistanceToMove;
    PLONG lpDistanceToMoveHigh;
    DWORD dwMoveMethod;
}MTPFilePointerInfo,*pMTPFilePointerInfo;

#define READ_FILE(a,b,c,d,e) READ_FILE_I(a,b,c,d,e,__FILE__,__LINE__)
#define SET_FILE_POINTER(a,b,c,d) SET_FILE_POINTER_I(a,b,c,d,__FILE__,__LINE__)


BOOL  READ_FILE_I(HANDLE*  hFile,
                        VOID * pvOutBuffer,
                        UINT32 u4ReadDataSz, 
                        UINT32 * pu4BytesReturned,
                        VOID * lpOverlapped,
                        CHAR * file,
                        UINT32 u4Line);

BOOL SET_FILE_POINTER_I(HANDLE  hFile, 
                                LONG    lLow32Bits,
                                LONG  * pu4High32Bits,
                                DWORD   dwMoveMethod,
                                CHAR *  file,
                                UINT32  u4Line);


//end for mtp file support by mtk40601
#ifdef __cplusplus
}
#endif

#endif //_GSTREAM_H_
