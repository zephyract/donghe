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
* file MSDK_DRM.h
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

#ifndef _MSDK_DRM_H_
#define _MSDK_DRM_H_

#include <MSDK_Defs.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup DRM
 *
 * @{
 */


MRESULT     DRM_GetPBContext(HDRM *phDRM);

MRESULT     DRM_ReleaseContext(HDRM hDRM);


/**
*   @brief  Init DRM system
*
*
*   @return    MRESULT  define in msdk_error.h
*
*   @see     msdk_error.h
**/
MRESULT     DRM_InitSystem(HDRM hDRM);


MRESULT     DRM_SetRandomSample(HDRM hDRM);


/**
*   @brief  Get balance for audio
*
*   @param[in]   hAudioCtrl handle of audio control
*   @param[out]   pi4Balance integer pointer be used for store audio balance
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT     DRM_GetRegistrationCode(HDRM hDRM, TCHAR *szRegCode, GUINT32 u4BufSz);

MRESULT     DRM_GetDeactivationCode(HDRM hDRM, TCHAR *szDeactCode, GUINT32 u4BufSz);

MRESULT     DRM_QueryRentalStatus(HDRM hDRM, GUINT8 *puRentalMessageFlag,
                                  GUINT8 *puUseLimit, GUINT8 *puUseCount);

MRESULT     DRM_FinalizePlayback(HDRM hDRM);    


MRESULT     DRM_IsDeviceActivated(GVOID);
MRESULT     DRM_GetActivationStatus(GUINT8 *puUserId, GUINT32 *pu4UserIdLength);
MRESULT     DRM_IsDeviceNeverRegistered(GVOID);
MRESULT     DRM_GetRegCode(TCHAR *szRegCode, GUINT32 u4BufSz);
MRESULT     DRM_GetDeregCode(TCHAR *szDeregCode, GUINT32 u4BufSz);


/**
*@}
*/

#ifdef __cplusplus
}
#endif

#endif //_MSDK_DRM_H_
