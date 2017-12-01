#ifndef _MSDK_GLOBAL_WMA_H_
#define _MSDK_GLOBAL_WMA_H_


#include <MSDK_Defs.h>
#include "ddraw.h"
#define MSDK_FULLSCREEN 0

#ifdef __cplusplus
extern "C" {
#endif

/**
*  @defgroup PlaybackMechanism  PlaybackMechanism
*
*  @brief the control center of media playback,and the entry of msdk API.msdk initialize and unintialize must be called before enter or exit msdk module.
*  @{
*/

/**
*   @brief This method initializes msdk. 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*
*   @note   must be called before msdk api used
**/
MRESULT     MSDK_Wma_Init(GVOID);

/**
*   @brief This method uninitializes msdk. 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*
*   @note   must be called after app exit.sure that all resource be released
**/
MRESULT     MSDK_Wma_Uninit(GVOID);

/**
*   @brief This method initializes thread context. 
*
*   @return MRESULT  define in msdk_error.h
*              
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*
*   @note   not implemented
**/


/**
*@}
*/
#ifdef __cplusplus
}
#endif

#endif //_MSDK_GLOBAL_H_
