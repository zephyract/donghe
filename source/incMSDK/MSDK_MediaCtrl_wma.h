

#ifndef _MSDK_MEDIACTRL_WMA_H_
#define _MSDK_MEDIACTRL_WMA_H_

#include <MSDK_Defs.h>


#ifdef __cplusplus
extern "C" {
#endif


/**@defgroup PlaybackControl PlaybackControl
*  
*  @brief the api of this module used as play back control. play pause stop seek and so on.
*  @{
*/

/**
*   @brief This method switches the entire filter graph into a running state. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_Wma_Play(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method switches the entire filter graph into a pause state. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_Wma_Pause(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method switches the entire filter graph into a stop state. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_Wma_Stop(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method sets playback rate. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[in]  rRate new playback rate.where 1 is the normal rate, 2 is twice as fast, and so on. 
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_Wma_SetRate(HMEDIAGRAPH hMediaGraph, E_MSDK_PLAY_SPEED rRate);

/**
*   @brief This method retrieves playback rate. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[out] prRate current rate 1  is normal play,2 is twice as fast, and so on. 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_Wma_GetRate(HMEDIAGRAPH hMediaGraph, E_MSDK_PLAY_SPEED *prRate);

/**
*   @brief This method retrieves playback capabilities. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[out] pCapabilities if zero,the media can not seek,otherise can use MediaCtrl_Seek 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaCtrl_Wma_GetCapabilites(HMEDIAGRAPH hMediaGraph, DWORD *pCapabilities);
/**
*   @brief This method set playback position. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[in]  eCurTimeFmt time format
*   @param[in]  u4Current new position set
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    E_MSDK_TIME_FORMAT
**/
MRESULT     MediaCtrl_Wma_Seek(HMEDIAGRAPH hMediaGraph, 
                           E_MSDK_TIME_FORMAT eCurTimeFmt,
                           UINT64 u8Current);

/**
*   @brief This method set playback start position and end position. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[in]  eStartTimeFmt start time format
*   @param[in]  u8Start new start position set
*   @param[in]  eEndTimeFmt   end time format
*   @param[in]  u8End new end position set
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    E_MSDK_TIME_FORMAT
**/
MRESULT     MediaCtrl_Wma_SetPlaybackRange(HMEDIAGRAPH hMediaGraph,  
                                       E_MSDK_TIME_FORMAT eStartTimeFmt,
                                       GUINT64 u8Start, 
                                       E_MSDK_TIME_FORMAT eEndTimeFmt, 
                                       GUINT64 u8End);

/**
*   @brief This method retrieves the length of time that the media stream will play. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[out] pu8Duration current media stream length in time.
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    E_MSDK_TIME_FORMAT
**/
MRESULT     MediaCtrl_Wma_GetDuration(HMEDIAGRAPH hMediaGraph, GUINT64 *pu8Duration);

/**
*   @brief This method retrieves the current position in terms of the total length of the media stream. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[out] pu8Current Current position in current time format units. 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    E_MSDK_TIME_FORMAT
**/
MRESULT     MediaCtrl_Wma_GetCurrentPosition(HMEDIAGRAPH hMediaGraph, 
                                         GUINT64 *pu8Current);

/**
*   @brief This method set playback position. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   @param[in]  eCurTimeFmt time format
*   @param[in]  u4Current new position set
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    E_MSDK_TIME_FORMAT
**/
MRESULT MediaCtrl_Wma_LastMemStart(HMEDIAGRAPH hMediaGraph, E_MSDK_TIME_FORMAT eCurTimeFmt, GUINT64 u8Current);

/**
*@}
*/





/*******************************************************************************
@Deprecate
以下函数不建议使用,其功能可以有其他函数替代，在以后的版本中将被取消
目前保留是为了兼容客户和公版
*******************************************************************************/

MRESULT MediaCtrl_Wma_GetFgIsNoSupportFF(HMEDIAGRAPH hMediaGraph, BOOL *fgIsNoSupportFF);

/**
*   @brief This method switches the AVIN into a running state. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*/
MRESULT MediaCtrl_Wma_PlayAvin(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method switches the AVIN into a pause state. 
*
*   @param[in]  hMediaGraph  media graph handle.
*   
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*/
MRESULT MediaCtrl_Wma_StopAvin(HMEDIAGRAPH hMediaGraph);


#ifdef __cplusplus
}
#endif

#endif //_MSDK_MEDIACTRL_H_


