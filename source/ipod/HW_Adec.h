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
* file HW_Adec.h
*
* Project:
* --------
*   MT33xx
*
* Description:
* ------------
*   In this file, define API for Audio Stream playback through audio decoder driver.
*
* Author:
* -------
*   MTK40043, daowen.deng@mediatek.com
*
*------------------------------------------------------------------------------
* $Revision: #1 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _MSDK_ADEC_H_
#define _MSDK_ADEC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "GDef.h"

	typedef GVOID* HADECINST;

	typedef struct STREAMBUFFER
	{
		GBYTE   *pbData;                //pointer to the buffer of data to be play. 
		GUINT32 u4BufferLen;            //the length of buffer
		GUINT32 u4BytesRecorded;        //reserved, no use
		GUINT32 u4User;                 //reserved, no use
		GUINT32 dwFlags;                //reserved, no use
		struct  STREAMBUFFER *lpNext;   // point to next structure to create link table when app send buffer one by one
	}STREAMBUFFER_T, *PSTREAMBUFFER_T;

	typedef enum ACODECID
	{
		AUD_DEC_FMT_UNKNOWN = 0,
		AUD_DEC_FMT_MPEG,
		AUD_DEC_FMT_AC3,
		AUD_DEC_FMT_PCM,
		AUD_DEC_FMT_MP3,
		AUD_DEC_FMT_AAC,                    // 5
		AUD_DEC_FMT_DTS,
		AUD_DEC_FMT_WMA,
		AUD_DEC_FMT_RA
	} CODECID_T;

	typedef enum
	{
		AUD_DEC_TYPE_UNKNOWN = 0,
		AUD_DEC_TYPE_MONO,                 /* 1/0 */
		AUD_DEC_TYPE_MONO_SUB,             /* 1+sub-language */
		AUD_DEC_TYPE_DUAL_MONO,            /* 1+1 */
		AUD_DEC_TYPE_STEREO,               /* 2/0 */
		AUD_DEC_TYPE_STEREO_SUB,           /* 2+sub-language */
		AUD_DEC_TYPE_STEREO_DOLBY_SURROUND,/* 2/0, dolby surround */
		AUD_DEC_TYPE_SURROUND_2CH,         /* 2/1 */
		AUD_DEC_TYPE_SURROUND,             /* 3/1 */
	} AUD_DEC_TYPE_T;

	typedef enum
	{
		AUD_DEC_PCM_FMT_WAVE = 1,    /* for AVI etc.*/
		AUD_DEC_PCM_FMT_ADPCM,       /* for ADPCM IMA*/
		AUD_DEC_PCM_FMT_ADPCM_MS,    /* for ADPCM MS*/
		AUD_DEC_PCM_FMT_PCM_NORMAL,  /* for ASF, AVI, MPS...etc */
	}AUD_DEC_PCM_FMT_T;

	typedef struct
	{
		CODECID_T       eAudFmt;            //the audio codec, eg. PCM is AUD_DEC_FMT_PCM
		AUD_DEC_TYPE_T  eDecType;           //if channel = 1, then choose AUD_DEC_TYPE_MONO
		GUINT8          ui1_bit_depth;      // bit depth of one sample
		GUINT32         ui4_data_rate;      // bit rate of audio stream
		GUINT32         ui4SampleRate;      // Sample rate of audio stream

		AUD_DEC_PCM_FMT_T   ePcmFmt;        // PCM format, please ref AUD_DEC_PCM_FMT_T
		UINT16              u2BlockAlign;   //block align, only necessary for ADPCM
		BOOL                b_de_emphasis;  //de-emphasis bit... 0 or 1
		BOOL                b_dlna_exist;   //DLNA exist or no
	} ADEC_PARAM_T, *PADEC_PARAM_T;

	typedef enum
	{
		SWI_SINK_TYPE_NONE	= 0,
		SWI_SINK_TYPE_FRONT,
		SWI_SINK_TYPE_REAR,
		SWI_SINK_TYPE_FRONT_REAR,
	} SWI_SINK_TYPE_T;

	typedef struct
	{
		UINT BUFFER_DONE;
		UINT OPEN_SUCCESS;
		UINT CLOSE_SUCCESS;
	}AUD_DEC_MSG_T;

	typedef void (__cdecl DRVCALLBACK)(HANDLE hdrvr, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

	/*
	* This function for open audio decoder driver. In this function, all submodules
	* should be init, MSDKCore and Audio Decoder Driver Instance should be created,
	* and all global memory will be allocated.
	*
	*@param hWnd -- Handle to the window whose window procedure is to receive the message. 
	* @param eCodecId --the codec of audio that will be passed into audio driver
	*
	* @return Returns a handle to the device context created if successful. Returns
	*         NULL if not successful.
	*/
	HADECINST ADec_CreateInstance(HANDLE hApp, DRVCALLBACK *fnCB, CODECID_T eCodecId);

	/*
	* This function for close audio decoder driver. If you do not use the handle created by 
	* function Adec_CreateInstance, Please close it.
	* Instance should be destroy and all global memory will be free.
	*
	* @param hInst -- Handle to the device context created by Adec_CreateInstance.
	*
	* @return Returns true if close successful, or else return false.
	*/
	GBOOL ADec_ReleaseInstance(HADECINST hInst);

	/*
	* This function for waitting End of Stream CMD to audio driver.
	*
	* @param hInst -- Handle to the device context created by Adec_CreateInstance.

	*
	* @return true if set CMD successfully, or else return false.
	*/
	GBOOL ADec_WaitEOS(HADECINST hInst);

	/*
	* This function for setting audio volume.
	*
	* @param hInst -- Handle to the device context created by Adec_CreateInstance.
	* @param eSink -- select seting the front, rear, or both volume.
	* @param SetFrontVolume -- set the front volume value
	* @param SetRearVolume  -- set the rear volume value
	* @return true if set setting successfully, or else return false.
	*/
	GBOOL ADec_SetVolume(HADECINST hInst, SWI_SINK_TYPE_T eSink, UINT8 SetFrontVolume, UINT8 SetRearVolume);

	/*
	* This function for getting audio volume.
	*
	* @param hInst -- Handle to the device context created by Adec_CreateInstance.
	* @param eSink -- select getting the front, rear, or both volume.
	* @param GetFrontVolume -- get the front volume value
	* @param GetRearVolume  -- get the rear volume value
	* @return true if set setting successfully, or else return false.
	*/
	GBOOL ADec_GetVolume(HADECINST hInst, SWI_SINK_TYPE_T eSink, UINT8* GetFrontVolume, UINT8* GetRearVolume);

	/*
	* This function for setting parameters about the audio which will be decoded.
	*
	* @param hInst -- Handle to the device context created by Adec_CreateInstance.
	* @param prParam -- point the parameter strcture 
	*
	* @return true if set parameters successfully, or else return false.
	*/
	GBOOL ADec_SetParam(HADECINST hInst, PADEC_PARAM_T prParam);

	/*
	* This function for send audio stream buffer to afifo of audio decoder.
	* Only one buffer once, and after send buffer, APP will wait for a event.
	*
	* @param hInst    -- Handle to the device context created by Adec_Open.
	* @param prStmBuf -- point the data passed by the app 
	*
	* @return Returns true if send buffer data successful, or else return false.
	*/
	GBOOL ADec_WriteData(HADECINST hInst, PSTREAMBUFFER_T prBuf);

	/*
	* This function for set DTV to pause.
	*
	* @param hInst    -- Handle to the device context created by Adec_Open.
	*
	* @return Returns true if pause successful, or else return false.
	*/
	GBOOL ADec_Pause(HADECINST hInst);

	/*
	* This function for set DTV to resume.
	*
	* @param hInst    -- Handle to the device context created by Adec_Open.
	*
	* @return Returns true if resume successful, or else return false.
	*/
	GBOOL ADec_Resume(HADECINST hInst);

	/*
	* This function for set DTV to flush.
	*
	* @param hInst    -- Handle to the device context created by Adec_Open.
	*
	* @return Returns true if flush successful, or else return false.
	*/
	GBOOL ADec_Flush(HADECINST hInst);

	/*
	* This function for set DTV to stop.
	*
	* @param hInst    -- Handle to the device context created by Adec_Open.
	*
	* @return Returns true if stop successful, or else return false.
	*/
	GBOOL ADec_Stop(HADECINST hInst);

	/*
	* This function for set DTV to front, rear or front and rear.
	*
	* @param hInst    -- Handle to the device context created by Adec_Open.
	* @param prStmBuf -- 
	*
	* @return Returns true if send buffer data successful, or else return false.
	*/
	GBOOL ADec_SetSinkInfo(HADECINST hInst, SWI_SINK_TYPE_T eSink);
	/*
	* This function for getting the Message ID that registered in ADec_CreateInstance
	*
	* @param hInst -- Handle to the device context created by Adec_CreateInstance.
	* @param pAudMsg -- point to the structure that can save the message ID from HW_Adec
	*
	* @return a pointer that point to the structure of AUD_DEC_MSG_T.
	*/
	GBOOL ADec_GetMsgID(HADECINST hInst, AUD_DEC_MSG_T *pAudMsg);


#ifdef __cplusplus
}
#endif

#endif //_MSDK_ADEC_H_
