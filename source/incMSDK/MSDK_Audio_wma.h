
#ifndef _MSDK_AUDIO_WMA_H_
#define _MSDK_AUDIO_WMA_H_

#include <MSDK_Defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup mediaInfo
 *
 * @{
 */


/**
 * @addtogroup mediaCtrl
 *
 * @{
 */

/**
*   @brief  set volume for current audio
*
*   @param[in]   hAudioCtrl handle of audio control
*   @param[in]     i4Volume integer value be used for set
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
MRESULT     AudioCtrl_Wma_SetVolume(HAUDIOCTRL hAudioCtrl, GINT32 i4Volume);

/**
*   @brief  get current audio volume
*
*   @param[in]   hAudioCtrl handle of audio control
*   @param[out]   pi4Volume integer pointer be used for store audio volume
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT     AudioCtrl_Wma_GetVolume(HAUDIOCTRL hAudioCtrl, GINT32 *pi4Volume);

/**
*   @brief  set balance for audio
*
*   @param[in]   hAudioCtrl handle of audio control
*   @param[out]   i4Balance integer value to be set
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT     AudioCtrl_Wma_SetBalance(HAUDIOCTRL hAudioCtrl, GINT32 i4Balance);

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
MRESULT     AudioCtrl_Wma_GetBalance(HAUDIOCTRL hAudioCtrl, GINT32 *pi4Balance);


/**
 * @addtogroup mediaInfo
 *
 * @{
 */

//ExtendAudio functions

/**
*   @brief  get audio spectrums of all band,values from 0 to 0x20000
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[out]   pu1SValues UINT8  array for low values
*   @param[out]   pu1LValues UINT8 array for  bar values
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioInfo_Wma_GetSourceSpectrums(HAUDIOINFO hAudioInfo,UINT32 *pu4SValues,UINT32 *pu4LValues);

/**
*   @brief  get audio spectrum of specify band,values from 0 to 0x20000
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[in]   u1Band  specify band
*   @param[out]   pu1SValue UINT8  pointer to low value
*   @param[out]   pu1LValue UINT8 pointer to bar value
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioInfo_Wma_GetSourceSpectrum(HAUDIOINFO hAudioInfo,UINT8 u1Band,UINT32 *pu4SValue,UINT32 *pu4LValue);

/**
*   @brief  get audio spectrum's bands number
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[out]   pu1BandsNum UINT8  pointer
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioInfo_Wma_GetSpectrumBands(HAUDIOINFO hAudioInfo,UINT8 *pu1BandsNum);


/**
*@}
*/

/**
 * @addtogroup mediaCtrl
 *
 * @{
 */

/**
*   @brief get  audio stream num
*
*   @param[in]   hAudioCtrl  audio control handle
*   @param[out]      u1Cnt UINT8  pointer
*
*   @return    MRESULT  define in msdk_error.h
*
*      one audio file might have multiple audio stream,like english france chinese and so on
*      you can obtain the count by the method
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioCtrl_Wma_GetCount(HAUDIOCTRL hAudioCtrl,UINT8 *u1Cnt);

/**
*   @brief get  audio stream index which is playing
*
*   @param[in]   hAudioCtrl  audio control handle
*   @param[out]    u1Index UINT8  pointer value in range(1~num)
*
*   @return    MRESULT  define in msdk_error.h
*
*   @see     AudioCtrl_GetCount()
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioCtrl_Wma_GetCurrentAudio(HAUDIOCTRL hAudioCtrl,UINT8 *u1Index);

/**
*   @brief set  play audio stream
*
*   @param[in]   hAudioCtrl  audio control handle
*   @param[in]     u1Index UINT8  value in range(1~num)
*
*   @return    MRESULT  define in msdk_error.h
*
*   @see     AudioCtrl_GetCount()
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioCtrl_Wma_SetCurrentAudio(HAUDIOCTRL hAudioCtrl,UINT8 u1Index);

/**
*   @brief get audio language code by index
*
*   @param[in]   hAudioCtrl  audio control handle
*   @param[in]     u1Index UINT8  value in range(1~stream num)
*   @param[out]     pu4LangID UINT32 pointer for language ID
*
*   @return    MRESULT  define in msdk_error.h
*
*   @see     AudioCtrl_GetCount()
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
*   @note   not implement
**/
MRESULT AudioCtrl_Wma_GetLanguage(HAUDIOCTRL hAudioCtrl,UINT8 u1Index,UINT32 *pu4LangID);
/**
*@}
*/






/*******************************************************************************
@Deprecate
以下函数不建议使用,其功能可以有其他函数替代，在以后的版本中将被取消
目前保留是为了兼容客户和公版
*******************************************************************************/


MRESULT AudioCtrl_Wma_GetAudFgSupport(HAUDIOCTRL hAudioCtrl, BOOL *pBfgAudSupport);

/**
*   @brief  get audio spectrum's max value
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[out]   pu1MaxVal UINT8  pointer
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/

MRESULT AudioInfo_Wma_GetSpectrumMaxVal(HAUDIOINFO hAudioInfo,UINT8 *pu1MaxVal);

/**
*   @brief set  max value for audio spectrum's
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[in]      u1MaxVal UINT8
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioInfo_Wma_SetSpectrumMaxVal(HAUDIOINFO hAudioInfo,UINT8 u1MaxVal);

/**
*   @brief  get audio spectrums of all band
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[out]   pu1SValues UINT8  array for low values
*   @param[out]   pu1LValues UINT8 array for  bar values
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioInfo_Wma_GetSpectrums(HAUDIOINFO hAudioInfo,UINT8 *pu1SValues,UINT8 *pu1LValues);

/**
*   @brief  get audio spectrum of specify band
*
*   @param[in]   hAudioInfo  audio info handle
*   @param[in]   u1Band  specify band
*   @param[out]   pu1SValue UINT8  pointer to low value
*   @param[out]   pu1LValue UINT8 pointer to bar value
*
*   @return    MRESULT  define in msdk_error.h
*
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
**/
MRESULT AudioInfo_Wma_GetSpectrum(HAUDIOINFO hAudioInfo,UINT8 u1Band,UINT8 *pu1SValue,UINT8 *pu1LValue);



#ifdef __cplusplus
}
#endif

#endif //_MSDK_AUDIO_H_
