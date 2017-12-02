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
*   mtk40390
*
* Last changed:
* ------------- 
* $Author: xiaozhou.huang $ 
*
* $Modtime: $  

* $Revision: #13 $
***************************************************************************/

#ifndef _BT_CONFIG_H_
#define _BT_CONFIG_H_

#define BT_DEFUALT_SETTING  (MW_ENABLE_NOISE_DEPRESS | CONNECTING_SDP_SEARCH_SERVICES_ASYNC | AUTO_HANGUP_MULTIPLE_CALL)

#define BT_KEEP_DISCOVERABLE_IF_UNCONNECTED     (0x00000001 << 0)
#define BT_TRY_DOWNLOAD_PB_ONLY_ONCE            (0x00000001 << 1)
#define BT_CONNECT_A2DP_AVRCP_AFTER_HFP         (0x00000001 << 2)
#define BT_QUERY_MANUFACTOR_ONLY_ONCE           (0x00000001 << 3)
#define BT_PB_NOT_USE_SPP_DUN                   (0x00000001 << 4)
#define BT_ALWAYS_TRY_DOWNLOAD_PB               (0x00000001 << 5)
#define BT_NO_PB_TESTDOWLOAD                    (0x00000001 << 6)
#define MW_ENABLE_NOISE_DEPRESS                 (0x00000001 << 7)
#define BT_ADJUST_TO_PC_TOOL_HSP_TEST           (0x00000001 << 8)

// if the app is used for PND, set it, otherwise do not define it
#define NOKIA_SWITCH_SCO_NOT_DISCONNECT         (0x00000001 << 9)

#define BT_MUSIC_STATUS_SYNC_WITH_AVDTP         (0x00000001 << 10)

#define SAVE_LINK_INFORMATION_TO_FILE           //Always enable. Use Bluetooth.ini to enable or disable saving linkkey in file.

#define BT_AVRCP_GET_MUSIC_ID3                  (0x00000001 << 11)

#define	BT_MUSIC_USE_NEW_STATUS                 //Always enable.

#define AUTO_HANGUP_MULTIPLE_CALL               (0x00000001 << 12)

#define CONNECTING_SDP_SEARCH_SERVICES_ASYNC    (0x00000001 << 13)

#define INCOMING_CALL_DO_NOT_MUTE_SCO           (0x00000001 << 14)

#define CHANGE_APPLE_CHARSET_TO_UTF8            (0x00000001 << 15)

#define SORT_PHONEBOOK_CONTACT_NAME             (0x00000001 << 16)

#define SUPPORT_HID_PROFILE                     (0x00000001 << 17)

#define ENABLE_SSP								(0x00000001 << 18)

#define THREE_WAY_CALLING_SUPPORT				(0x00000001 << 19)
#endif

