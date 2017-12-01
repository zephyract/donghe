
#ifndef _MSDK_MEDIAGRAPH_WMA_H_
#define _MSDK_MEDIAGRAPH_WMA_H_


#include <MSDK_Defs.h>



#ifdef __cplusplus
extern "C" {
#endif

/**
  *@addtogroup PlaybackMechanism
 * @{
 *
 */

/**
*   @brief This method create media graph.
*
*   @param[out]     phMediaGraph pointer to media graph handle.
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_Wma_Create(HMEDIAGRAPH *phMediaGraph);

/**
*   @brief This method release media graph.
*
*   @param[in]  hMediaGraph media graph handle.
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*
*   @note the method might be called while app exit.media graph will be destroyed,all inteface be released also.
**/
MRESULT     MediaGraph_Wma_Release(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method clear media graph.
*
*   @param[in]  hMediaGraph media graph handle.
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*
*   @note the method might be called while stop play .mediagraph not be destroyed,but some members will be released .
**/
MRESULT     MediaGraph_Wma_Clear(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method set media graph config.repeat play and notify frequency
*
*   @param[in]  hMediaGraph media graph handle.
*   @param[in]  prConfig config param for mediagraph
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    MG_CONFIG_T
**/
MRESULT     MediaGraph_Wma_SetConfig(HMEDIAGRAPH hMediaGraph, MG_CONFIG_T *prConfig);

/**
*   @brief This method retrieves media graph config.
*
*   @param[in]  hMediaGraph media graph handle.
*   @param[out] prConfig current mediagraph config
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    MG_CONFIG_T
**/
MRESULT     MediaGraph_Wma_GetConfig(HMEDIAGRAPH hMediaGraph, MG_CONFIG_T *prConfig);

/**
*   @brief This method retrieves media graph state(play pause stop).
*
*   @param[in]  hMediaGraph media graph handle.
*   @param[out] peState current media graph state
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    E_MSDK_MGSTATE
**/
MRESULT     MediaGraph_Wma_GetState(HMEDIAGRAPH hMediaGraph, E_MSDK_MGSTATE *peState);

/**
*   @brief This method query interface by IID.
*
*   @param[in]  hMediaGraph media graph handle.
*   @param[in]  pIID MSDK_IID marked
*   @param[out] ppvInterface address of interface pointer
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
*   @see    MSDK_IID
**/
MRESULT     MediaGraph_Wma_QueryInterface(HMEDIAGRAPH hMediaGraph, const MSDK_IID *pIID,
                                      GVOID **ppvInterface);

/**
*   @brief This method set media graph  source file.
*
*   @param[in]  hMediaGraph media graph handle.
*   @param[in]  szFileName file path will be render
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_Wma_SetSourceFile(HMEDIAGRAPH hMediaGraph, GTCHAR *szFileName);


/**
*   @brief This method force media progress stop.
*
*   @param[in]  hMediaGraph media graph handle.
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/
MRESULT     MediaGraph_Wma_TerminateLoad(HMEDIAGRAPH hMediaGraph);

/**
*   @brief This method set media graph display target.
*
*   @param[in]  hMediaGraph media graph handle.
*   @param[in]    eSiType  video display target to front ,rear or both
*
*   @return MRESULT  define in msdk_error.h
*
*   @see    MediaGraph_Create()
*   @see    msdk_error.h
**/

MRESULT     MediaGraph_Wma_SetDataSink(HMEDIAGRAPH hMediaGraph,E_MSDK_SINK_TYPE eSiType);

/**
*     @brief This method set media graph  display information.
*
*     @param[in]    hMediaGraph media graph handle.
*     @param[in]    prSinkInfo config param for source
*
*     @return   MRESULT  define in msdk_error.h
*
*     @see       MediaGraph_Create()
*     @see       msdk_error.h
**/
MRESULT     MediaGraph_Wma_CloseAudio(HMEDIAGRAPH hMediaGraph, E_MSDK_SINK_TYPE eSiType);

MRESULT     MediaGraph_Wma_OpenAudio(HMEDIAGRAPH hMediaGraph, E_MSDK_SINK_TYPE eSiType);

MRESULT     MediaGraph_Wma_SetSinkInfo (HMEDIAGRAPH hMediaGraph,MSDK_SINK_INFO_T *prSinkInfo);

/**
*     @brief This method retrieves media infomation .
*
*     @param[in]    hMediaGraph media graph handle.
*     @param[in]    prMediaInfo media infomation struct
*
*     @return   MRESULT  define in msdk_error.h
*
*     @see       MediaGraph_Create()
*     @see       msdk_error.h
**/
MRESULT     MediaGraph_Wma_GetMediaInfo(HMEDIAGRAPH hMeidaGraph,MSDK_MEDIA_INFO_T *prMediaInfo);



/**********************************************************************************************
以下函数不建议使用,其功能可以有其他函数替代，在以后的版本中将被取消
目前保留是为了兼容客户和公版
************************************************************************************************/

/**
/*@Deprecate

* @See Video
*
**/

MRESULT MediaGraph_Wma_GetVStmCnt(HMEDIAGRAPH hMediaGraph, UINT8 *u1VideoStreamCnt);
MRESULT MediaGraph_Wma_SetVideoInSource(HMEDIAGRAPH hMediaGraph, E_MSDK_AVIN_VINDEX eVIndex,
                                    MSDK_AVIN_SINK_INFO_T *prSinkInfo);
MRESULT MediaGraph_Wma_SetAudioInSource(HMEDIAGRAPH hMediaGraph,E_MSDK_AVIN_AINDEX eAIndex,
                                    MSDK_AVIN_SINK_INFO_T *prSinkInfo);
MRESULT MediaGraph_Wma_SetAVInSource(HMEDIAGRAPH hMediaGraph,E_MSDK_AVIN_VINDEX eVIndex,
                                 E_MSDK_AVIN_AINDEX eAIndex, MSDK_AVIN_SINK_INFO_T *prSinkInfo);

MRESULT MediaGraph_Wma_SetVideoInInfo(HMEDIAGRAPH hMediaGraph, MSDK_AVIN_SINK_INFO_T *prSinkInfo);
MRESULT MediaGraph_Wma_SwitchAVInSink(HMEDIAGRAPH hMediaGraph,E_MSDK_SINK_TYPE eSinkType,E_MSDK_MODE eMode);

MRESULT MediaGraph_Wma_VdoInGetIsSignalReady(HMEDIAGRAPH hMediaGraph, BOOL *fgIsSignalReady);
MRESULT MediaGraph_Wma_SetScale(HMEDIAGRAPH hMediaGraph,MSDK_SCALE_INFO_T *prScaleInfo);

MRESULT MediaGraph_Wma_GetAudInSpec(HMEDIAGRAPH hMediaGraph, UINT32 *pu1SValues, UINT32 *pu1LValues);
MRESULT MediaGraph_Wma_SwitchVideo(HMEDIAGRAPH hMediaGraph, E_MSDK_SINK_TYPE eSiType);

BOOL fgWmaFile(GTCHAR *szExtName,GTCHAR *szFileName);
/**
*@}
*/

#ifdef __cplusplus
}
#endif

#endif //_MSDK_MEDIAGRAPH_H_


