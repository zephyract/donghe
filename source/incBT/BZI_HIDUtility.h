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
 *   mtk71275
 *
 * Last changed:
 * ------------- 
 * $Author: xiaozhou.huang $ 
 *
 * $Modtime: $  
 *
 * $Revision: #2 $
****************************************************************************/

#ifndef _BZI_HID_UTILITY_H_
#define _BZI_HID_UTILITY_H_

#include "BZI_HID.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HID_KEYBOARD_CODE_ESC       0x29


#define HID_CONTROL_CMD_HOME        0x0001
#define HID_CONTROL_CMD_BACK        0x0004
#define HID_CONTROL_CMD_MENU        0x0010
#define HID_CONTROL_CMD_PLAY_PAUSE  0x0020
#define HID_CONTROL_CMD_PREVIOUS    0x0040
#define HID_CONTROL_CMD_NEXT        0x0080
#define HID_CONTROL_CMD_STOP        0x0100
#define HID_CONTROL_CMD_VOLUME_UP   0x0200
#define HID_CONTROL_CMD_VOLUME_DOWN 0x0400

HRESULT BZ_HIDUtilityInit();

HRESULT BZ_HIDUtilityUninit();

HRESULT BZ_HIDSetInterruptLatency(DWORD u16LatencyMS);

HRESULT BZ_HIDMouseData(BOOL fgLButtonDown, int i16RelativeX, int i16RelativeY);

HRESULT BZ_HIDKeyboardData(BYTE bKeyCode);

HRESULT BZ_HIDControlCMDData(UINT16 u16CMD);

#ifdef __cplusplus
}
#endif


#endif /* _BZI_HID_UTILITY_H_ */

