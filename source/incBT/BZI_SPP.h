 /*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc.   2008 MediaTek Inc.
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
 * szFileName:
 * ---------
 *   $Workfile:  $ 
 *
 * Project:
 * --------
 *   MTKBluetooth
 *
 * Description:
 * ------------
 *   
 *   
 * Author:
 * -------
 *   mtk40144
 *
 * Last changed:
 * ------------- 
 * $Author: xiaoxia.pan $ 
 *
 * $Modtime: $  
 *
 * $Revision: #4 $
****************************************************************************/


#pragma once

#include "BZI_Comm.h"
#include "BZS_SPP.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPP_COMNAME_LENGTH_MAX 6

HRESULT BZ_SPPCreateConnectByBDAddr(BT_ADDR addr);
HRESULT BZ_SPPCreateConnect();


HRESULT BZ_SPPRemoveConnect();

BOOL BZ_SPPGetConnectStatus();

//************************************
//function:get spp device address
//parameter:[in&out]szName  wchar_t 
//          [in]u4Len  u4Len >= (MAX_LEN_DEVICE_ADDR + 1)
//return: success return ok,or return fail
//************************************
HRESULT BZ_SPPGetAddr(wchar_t *szName, UINT32 u4Len);

//************************************
//function:get spp device name
//parameter:[in&out]szName  wchar_t
//          [in]u4Len  u4Len >= (MAX_LEN_DEVICE_NAME + 1)
//return: success return ok,or return fail
//************************************
HRESULT BZ_SPPGetName(wchar_t *szName,UINT32 u4Len);

//************************************
//function:get spp status
//parameter:[in&out]eState  wchar_t
//return: success return ok,or return fail
//useage:recv BZ_IND_SPP_CONNETC_STATUS_CHANGE call this fun
//************************************
HRESULT BZ_SPPGetStatus(E_SPP_CONNECT_STATUS_T &eState);

typedef struct 
{
    WCHAR  wszCOMName[SPP_COMNAME_LENGTH_MAX+1];
    BT_ADDR bta;
} SPPVCOMINFO_T, *P_SPPVCOMINFO_T;

#ifdef __cplusplus
}
#endif


