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
* file AVSwi_Pub.h
*
* Project:
* --------
*   CNB
*
* Description:
* ------------
*
*
* Author: jian liang mtk40099
* -------
*
*
*------------------------------------------------------------------------------
* $Revision: #3 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _AVSWI_PUB_H_
#define _AVSWI_PUB_H_

#include <GKal.h>
#include "AVSwi_Def.h"

#define FULLSCREEN 0
#define DDOVER_SHOWATREAR           0x00200000

#define SWI_INFO_SHOW               0x00000001
#define SWI_INFO_HIDE               0x00000002
#define SWI_INFO_DESTCOLKEY         0x00000004
#define SWI_INFO_SRCCOLKEY          0x00000008
#define SWI_INFO_ALPHA              0x00000010
#define SWI_INFO_RESTORE            0x00000020


typedef void *LPVOID;
typedef GBOOL (*PFN_SWI_EVENT)(GUINT32 u4Event, GUINT32 u4Param1, GUINT32 u4Param2);


typedef enum
{
    SWI_SOURCE_NONE,
    SWI_SOURCE_USBSD,
    SWI_SOURCE_DTV,
    SWI_SOURCE_DVD,
    SWI_SOURCE_MAX
} SWI_SOURCE_T;


typedef enum
{
    SWI_SINK_NONE,
    SWI_SINK_FRONT,
    SWI_SINK_REAR,
    SWI_SINK_FRONT_REAR,
} SWI_SINK_T;


typedef enum
{
    SWI_SF_VIDEO,
    SWI_SF_SP,
    SWI_SF_ST,
    SWI_SF_CC,
} SWI_SF_TYPE_T;


typedef struct
{
    SWI_SINK_T      eSink;
    RECT            DestRect;
    UINT32          u4Flag;
    DDOVERLAYFX     ovfx;
} SWI_SINK_INFO_T;


typedef struct 
{
    SWI_SINK_T eSinkType;
    RECT           rRect;
    GBOOL          fgNotScale;
}SWI_SCALE_INFO_T;

#define DVD_DO_SCALE_FRONT   0X01
#define DVD_DO_SCALE_REAR    0X02
#define USBSD_DO_SCALE_FRONT 0X04
#define USBSD_DO_SCALE_REAR  0X08

#ifdef __cplusplus
extern "C" {
#endif

// For MSDK/App
#if FULLSCREEN
GRESULT AVSwi_Init(HWND hWnd,LPDIRECTDRAWSURFACE pddPrimary);
#else
GRESULT AVSwi_Init(GVOID);
#endif

GRESULT AVSwi_Deinit(GVOID);

GRESULT AVSwi_SetScaleInfo(SWI_SOURCE_T eSource, SWI_SCALE_INFO_T *prScaleInfo);

GRESULT AVSwi_SetSinkInfo(SWI_SINK_INFO_T *prSinkInfo);

GRESULT AVSwi_SetCCSinkInfo(SWI_SINK_INFO_T *prSinkInfo);

GRESULT AVSwi_Open(SWI_SOURCE_T eSource, SWI_SINK_T eSink);

GRESULT AVSwi_Switch(SWI_SINK_T eSink);
GRESULT AVSwi_SwitchVideo(SWI_SINK_T eSink);


GRESULT AVSwi_CloseAudio(SWI_SINK_T eSink);

GRESULT AVSwi_OpenAudio(SWI_SINK_T eSink);
GRESULT AVSwi_Close();

GRESULT AVSwi_CreateSurface(GSURFACEDESC_T *prSurfaceDesc, GHANDLE *phSurface, SWI_SF_TYPE_T eSfType);

GRESULT AVSwi_EnumAttachedSurfaces(GHANDLE hSurface, LPVOID lpContext,
                                   LPGENUMSURFACESCALLBACK lpEnumSurfacesCallback);
GRESULT AVSwi_UpdateOverlay(GHANDLE hSurface,
                            RECT *prSrcRect,
                            GHANDLE hDestSurface,
                            RECT *prDestRect,
                            GDWORD dwFlags,
                            GOVERLAYFX_T *prOverlayFx);
GRESULT AVSwi_Flip(GHANDLE hSurface,
                   GHANDLE hSurfaceTargetOverride,
                   GDWORD dwFlags);
GRESULT AVSwi_ReleaseSurface(GHANDLE hSurface);

GRESULT AVSwi_Lock(GHANDLE hSurface, RECT *prDestRect,
                   GSURFACEDESC_T *prSurfaceDesc, DWORD dwFlags, HANDLE hEvent);
GRESULT AVSwi_Unlock(GHANDLE hSurface, LPRECT lpRect);

GRESULT AVSwi_GetSurfaceDesc(GHANDLE hSurface, GSURFACEDESC_T *prSurfaceDesc);

GRESULT AVSwi_GetCaps(GCAPS_T *prGDriverCaps, GCAPS_T *prGHELCaps);

GRESULT AVSwi_Restore(GHANDLE hSurface);

GRESULT AVSwi_GetDC(GHANDLE hSurface, HDC *phdc);

GRESULT AVSwi_ReleaseDC(GHANDLE hSurface, HDC hdc);

GRESULT AVSwi_RegCB(PFN_SWI_EVENT pfnSwiEvent);

#ifdef __cplusplus
}
#endif

#endif //_AVSWI_PUB_H_


