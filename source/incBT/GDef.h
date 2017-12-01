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


/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile:  $ 
 *
 * Project:
 * --------
 *   MiGO
 *
 * Description:
 * ------------
 *   
 *   
 * Author:
 * -------
 *   mcn03001
 *
 * Last changed:
 * ------------- 
 * $Author: wenzong.peng $ 
 *
 * $Modtime: $  
 *
 * $Revision: #4 $
****************************************************************************/
#ifndef _GDEF_H_
#define _GDEF_H_

//#include "GCfg.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifndef GBASEVER
#define GBASEVER    0x0003
#endif /* GBASEVER */

/***************************************************************************/
/*                      INCLUDE HEADER FILES                               */
/***************************************************************************/
#if defined(MTK_FP)
    #include "MMIDataType.h"

#elif defined(WIN32) || defined(_WIN32_WCE)
    #include <windows.h>
    #include <tchar.h>
    #include <malloc.h>

    #ifdef WIN32
        #include <assert.h>
        #include <string.h>
        #include <stdio.h>
    #else
        #include <stdlib.h>
        #include <memory.h>
    #endif

#endif



/***************************************************************************/
/*                    PROPRIETARY TYPE DEFINITIONS                         */
/***************************************************************************/
#define MIGO_MAX_PATH       260
typedef unsigned long       GRESULT;

/***************************************************************************/
/*                PLATFORM INDEPENDENT DEFINITIONS                         */
/***************************************************************************/
#ifdef NULL
  #undef NULL
#endif

//#ifdef VOID
//  #undef VOID
//#endif
//
//#ifdef BOOL
//  #undef BOOL
//#endif

#ifdef IN
  #undef IN
#endif

#ifdef OUT
  #undef OUT
#endif

#ifdef INOUT
  #undef INOUT
#endif

#ifdef TRUE
  #undef TRUE
#endif

#ifdef FALSE
  #undef FALSE
#endif

#ifdef ON
  #undef ON
#endif

#ifdef OFF
  #define OFF
#endif

#ifdef GAPI
  #undef GAPI
#endif

#ifdef EXTERN
  #undef  EXTERN
#endif

#ifdef LOCAL
  #undef LOCAL
#endif

#ifdef ASSERT
  #undef ASSERT
#endif

#ifdef CONST
  #undef CONST
#endif

#ifdef GLOBAL
  #undef GLOBAL
#endif

#define LOAD_MRF_RESOURCE    0
#define IN
#define OUT
#define INOUT
#define TRUE            1
#define FALSE           0
#define ON              1
#define OFF             0
#define NULL            0
#define GVOID           void
#define LOCAL           static
#define CONST           const
#define GLOBAL

#undef _T
#undef __T
#undef TEXT

#ifdef _UNICODE
#define _T(x)           L##x
#else
#define _T(x)           x
#endif

#define __T             _T
#define TEXT            __T

/***************************************************************************/
/*               HW PLATFORM DEPENDENT DEFINITIONS                         */
/***************************************************************************/
#if defined(__arm)
    #define INLINE      __inline
    #define PACKED      __packed
    #define ALIGN(a)    __align(a)

#elif defined(WIN32)
    #define INLINE      __inline
    #define PACKED
    #define ALIGN(a)

#else
    #define INLINE
    #define PACKED
    #define ALIGN(a)
    
#endif

/***************************************************************************/
/*               SW PLATFORM DEPENDENT DEFINITIONS                         */
/***************************************************************************/
#if defined(MTK_FP)
    #define DLLEXPORT
    #define CALLBACK
    #define WINAPI              __stdcall
    #define GHWND                void*
    #define GTCHAR               GWCHAR

#ifndef GMAX_PATH
    #define GMAX_PATH      MIGO_MAX_PATH
#endif

    typedef          char       GCHAR;
    typedef unsigned char       GUCHAR;
    typedef unsigned short        GWCHAR;

    typedef signed   int        GINT;    
    typedef unsigned int        GUINT;

    typedef signed   char       GINT8;
    typedef unsigned char       GUINT8;

    typedef signed short int    GINT16;
    typedef unsigned short int  GUINT16;

    typedef signed long         GINT32;
    typedef unsigned long       GUINT32;

    typedef signed __int64      GINT64;
    typedef unsigned __int64    GUINT64;

    typedef unsigned char       GBYTE;
    typedef unsigned short      GWORD;
    typedef unsigned int        GDWORD;

    typedef const GCHAR *      GLPCSTR;
    typedef const GWCHAR *     GLPCWSTR;
    typedef const GTCHAR *     GLPCTSTR;
    typedef GTCHAR *           GLPTSTR;
    typedef const void *        GLPCVOID;
    typedef GUINT32            GLPARAM;
    typedef GUINT32            GWPARAM;
    typedef GUINT32            GLRESULT;

    typedef float               GFLOAT;
    typedef void *              GHANDLE;
    typedef double              GDOUBLE;

    #if defined(KAL_ON_OSCAR)
        typedef unsigned char   GBOOL;
    #else // KAL_IN_NUCLEUS
        typedef unsigned char   GBOOL;
    #endif

#elif defined(WIN32)
    typedef          char       GCHAR;
    typedef unsigned char       GUCHAR;
    typedef signed   int        GINT;    
    typedef unsigned int        GUINT;

    typedef signed   char       GINT8;
    typedef signed short        GINT16;
    typedef signed int          GINT32;
    typedef signed __int64      GINT64;

    typedef unsigned char       GUINT8;
    typedef unsigned short      GUINT16;
    typedef unsigned int        GUINT32;
    typedef unsigned __int64    GUINT64;

    typedef float               GFLOAT;
    typedef void *              GHANDLE;
    typedef double              GDOUBLE;

    #define GHWND              HWND //void*
    #define DLLEXPORT           _declspec(dllexport)

    typedef wchar_t             GWCHAR;
    typedef GWCHAR              GTCHAR;

    typedef unsigned char       GBOOL; 
    typedef unsigned char       GBYTE;
    typedef unsigned short      GWORD;
    typedef unsigned long       GDWORD;

    typedef const GCHAR *      GLPCSTR;
    typedef const GWCHAR *     GLPCWSTR;
    typedef const GTCHAR *     GLPCTSTR;
    typedef GTCHAR *           GLPTSTR;
    typedef const void *        GLPCVOID;
    typedef GUINT32            GLPARAM;
    typedef GUINT32            GWPARAM;
    typedef GUINT32            GLRESULT;

#ifndef GMAX_PATH
#define GMAX_PATH      MIGO_MAX_PATH
#endif

#else
    // T.B.D. some other platform in the future
#endif

/***************************************************************************/
/*                PLATFORM INDEPENDENT DEFINITIONS                         */
/***************************************************************************/
#ifdef __cplusplus
  #define GAPI          DLLEXPORT
  #define EXTERN        extern "C"
#else
  #define GAPI          extern DLLEXPORT
  #define EXTERN        extern
#endif

/************************************************************************/
/*           EXCEPTION AND ASSERTION  DEFINITIONS                       */
/************************************************************************/
/********  GB_ASSERT_DEBUG == 0  ********/
#if !GB_ASSERT_DEBUG 
  #if !defined(KAL_ON_OSCAR) && defined(MTK_FP)//defined(KAL_ON_OSCAR) || defined(WIN32)
    #define ASSERT(...)
  #else
    #define ASSERT
  #endif

/********  GB_ASSERT_DEBUG == 1  ********/
#else                       
    #include "GLog.h"
      
    GAPI  void   GAssert(GCHAR* , GCHAR* , GUINT32);
    GAPI  GCHAR *GStrrChrA(const GCHAR *pString, GCHAR c);

    #define GET_FILE(file)  (const char *)(((char*)GStrrChrA((file),'\\')) ? ((char*)GStrrChrA((file),'\\')) + 1 : (file))
    #define ASSERT(exp)     do{ if((exp)==FALSE) {LOG_Cmsg("<ASSERT> FILE(%s) LINE(%d) %s", GET_FILE(__FILE__), __LINE__, (#exp)); GAssert(#exp, __FILE__, __LINE__);} }while(0)
#endif /* _DEBUG */


#ifdef __cplusplus
}
#endif

#endif  // _GDEF_H_

