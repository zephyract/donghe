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
 * $Author: yongguang.hong $ 
 *
 * $Modtime: $  
 *
 * $Revision: #4 $
****************************************************************************/

#ifndef _BZI_MUSIC_H_
#define _BZI_MUSIC_H_

#include "BZI_Comm.h"

#include "BZS_Music.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Play music
 */
void BZ_MusicPlay();

/**
 * Pause Music's playing
 */
void BZ_MusicPause();

/**
 * Stop Music's playing
 */
void BZ_MusicStop();

/**
 * Play next music
 */
void BZ_MusicPlayNext();    

/**
 * Play previous music
 */
void BZ_MusicPlayPrev();

/**
 * Get Music Play Status: Disconnected, playing, pause, stop
 */
HRESULT BZ_GetMusicPlayStatus(E_BT_MUSICPLAYSTATUS_T &eStatus);

/**
 * Get the AVRCP Status
 */
HRESULT BZ_GetAVRCPStatus(E_BT_AVRCP_CONNECT_STATUS_T &eStatus);

/**
 * Get the A2DP Status
 */
HRESULT BZ_GetA2DPStatus(E_BT_A2DP_CONNECT_STATUS_T &eStatus);       


HRESULT BZ_A2DPMuteIn();

HRESULT BZ_A2DPActive(BOOL fgActiveA2DP);

void BZ_A2DPConnect();

void BZ_A2DPDisConnect();

#if 1 //mtk71195
#define AUDIO_SPECTRUM_BAR_NUM       16
#define AUDIO_SPECTRUM_MAX_VAL       128
#define IOCTL_AUDIO_GET_SPECTRUM     CTL_CODE(FILE_DEVICE_UNKNOWN, 0x811, METHOD_BUFFERED, FILE_ANY_ACCESS)

/* Audio spectrum info. */
typedef struct _AUD_DEC_SPECTRUM_INFO_T
{
    UINT32    u4_aud_spectrum[16];
    UINT32    u4_aud_spectrum_bar[16];
} AUD_DEC_SPECTRUM_INFO_T;

HRESULT BZ_MusicGetSpectrum(DWORD dwShortLen, PDWORD pdwShortVal, DWORD dwLongLen, PDWORD pdwLongVal);
HRESULT BZ_MusicGetAudioData(DWORD dwShortLen, PDWORD pdwShortVal, DWORD dwLongLen, PDWORD pdwLongVal);
#endif

HRESULT BZ_GetID3Info(MediaAttribute_T* prMediaAttribute);

HRESULT BZ_GetPlayStateInfo(A2DPPlayState_T* prPlayStateInfo);

void BZ_ConnectA2DP(BT_ADDR *pbtAddr);
void BZ_DisConnectA2DP(BT_ADDR *pbtAddr);




#ifdef __cplusplus
}
#endif


#endif /* _BZI_MUSIC_H_ */
