
#ifndef _MSDK_MEDIAEVENT_WMA_H_
#define _MSDK_MEDIAEVENT_WMA_H_

#include <MSDK_Defs.h>


#ifdef __cplusplus
extern "C" {
#endif
/**
 * @addtogroup PlaybackMechanism
 * @{
 *
 */


//msdk event enumrator
#if 0
enum
{
    EVT_MSDK_FINISHED,                  /**<play finished                                    */
    EVT_MSDK_PLAYBACK_POSITION_STATUS,  /**<playback position change                         */  
    EVT_MSDK_PLAYED,                    /**<play state                                       */
    EVT_MSDK_PAUSED,                    /**<pause state                                      */
    EVT_MSDK_STOPED,                    /**<stop state                                       */
    EVT_MSDK_GRAPH_CHANGED,             /**<media graph changed                              */
    EVT_MSDK_GRAPH_BUILT,               /**<graph build successful                           */
    EVT_MSDK_LYRICITEM_CHANGED,         /**<u4Param1 is the index of the lyric item          */
    EVT_MSDK_VIDEO_SIZE_CHANGED,        /**<vido size change                                 */
    EVT_MSDK_ERROR_OCCURED,             /**<error occurred                                   */
    EVT_MSDK_TIP_WARNING,               /**tip warning for app tips                          */

    EVT_MSDK_SET_SUBTITLE_SUCCEED,
    EVT_MSDK_SET_SUBTITLE_FAILED,
    EVT_MSDK_SET_AUDIO_SUCCEED,
    EVT_MSDK_SET_AUDIO_FAILED,
    EVT_MSDK_SEEK_SUCCEED,
    EVT_MSDK_SEEK_FAILED,
    EVT_MSDK_SET_RATE_SUCCEED,
    EVT_MSDK_SET_RATE_FAILED,
   
    EVT_MSDK_AVIN_SIGNAL_READY,         /**video in signal ready                             */
    EVT_MSDK_AVIN_SIGNAL_LOST,          /**video in signal lost                              */
};
#endif
/**
*   @brief   This method retrieves the next notification event. 
*
*   @param[in]  hMediaGraph  mediagraph handle.
*   @param[out] pu4EventCode next event notification.
*   @param[out] pu4Param1 First parameter of the event.
*   @param[out] pu4Param2 Second parameter of the event. 
*   @param[in]  i4TimeOut in milliseconds, to wait before assuming that there are no events. 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_Wma_GetEvent(HMEDIAGRAPH hMediaGraph, GUINT32 *pu4EventCode, 
                                GUINT32 *pu4Param1, GUINT32 *pu4Param2, 
                                GINT32 i4TimeOut);

/**
*   @brief   This method add listener to media graph. 
*
*   @param[in]  hMediaGraph  mediagraph handle.
*   @param[in]  pfnListener call back function as listener.
*   @param[in]  u4CustomData listener function parameter.
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    PFN_MSDK_MEDIAEVENT_LISTENER
**/
MRESULT     MediaGraph_Wma_AddListener(HMEDIAGRAPH hMediaGraph, 
                                   PFN_MSDK_MEDIAEVENT_LISTENER pfnListener,
                                   GUINT32 u4CustomData);

/**
*   @brief   This method remove listener from media graph. 
*
*   @param[in]  hMediaGraph  mediagraph handle.
*   @param[in]  pfnListener the istener will be removed.
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    PFN_MSDK_MEDIAEVENT_LISTENER
**/
MRESULT     MediaGraph_Wma_RemoveListener(HMEDIAGRAPH hMediaGraph, 
                                      PFN_MSDK_MEDIAEVENT_LISTENER pfnListener);

/**
*   @brief   This method dispatch event. 
*
*   @param[in]  hMediaGraph  mediagraph handle.
*   @param[in]  u4EventCode event notification code.
*   @param[in]  u4Param1 first event parameter
*   @param[in]  u4Param2 second event parameter
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_Wma_DispatchEvent(HMEDIAGRAPH hMediaGraph, GUINT32 u4EventCode,
                                     GUINT32 u4Param1, GUINT32 u4Param2);

/**
*   @brief   This method handle media graph event. 
*
*   @param[in]  hMediaGraph  mediagraph handle.
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_Wma_HandleEvent(HMEDIAGRAPH hMediaGraph);

/**
*   @brief   This method registers a window that will handle messages in response to all events from an object. 
*
*   @param[in]  hMediaGraph  mediagraph handle.
*   @param[in]  hWnd Handle of window to notify. Pass NULL to stop notification.
*   @param[in]  u4Message Window message to be passed as the notification. 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_Wma_RegisterNotifyWindow(HMEDIAGRAPH hMediaGraph, HWND hWnd, GUINT32 u4Message);

/**
*@}
*/

#ifdef __cplusplus
}
#endif

#endif //_MSDK_MEDIAEVENT_H_


