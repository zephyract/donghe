
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
 *   mtk70933
 *
 * Last changed:
 * ------------- 
 * $Author: wenchao.liu $ 
 *
 * $Modtime: $  
 *
 * $Revision: #1 $
****************************************************************************/
#ifndef _BZI_SEARCH_H_
#define _BZI_SEARCH_H_

#include "BZI_Comm.h"
//#include "BMSComm.h"
#include "BZS_Search.h"

HRESULT BZ_StartSearch();
HRESULT BZ_StopSearch();

HRESULT BZ_GetDeviceRec(UINT32 u4Idx, P_BT_DEVICE_INFO_NODE_T  *prDeviceRec);

void BZ_ConnectByDeviceRec(UINT32 i4Index);
void BZ_DetachByDeviceRec(UINT32 i4Index);
HRESULT BZ_GetSearchedRecCnt(UINT32 &u4Count);
HRESULT BZ_GetSearchState(E_DEVICE_SEARCH_STATE_T &eSearchState);

#endif
