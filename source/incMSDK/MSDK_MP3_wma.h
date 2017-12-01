
#ifndef _MSDK_MP3_WMA_H_
#define _MSDK_MP3_WMA_H_

#include <MSDK_Defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup mediaInfo
 * @{
 */

/**
*   @brief get MP3 title information
*
*   @param[in]   hMP3Info  mp3info handle
*   @param[out]    szTitle GTCHAR*  pointer address of output title information 
*   @param[in]     u4MaxLen GUINT32 max length of szTitle
*
*   @return    MRESULT  define in msdk_error.h
*              
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
MRESULT     MP3Info_Wma_GetTitle(HMP3INFO hMP3Info, GTCHAR *szTitle, GUINT32 u4MaxLen);

/**
*   @brief get MP3 artist information
*
*   @param[in]   hMP3Info  mp3info handle
*   @param[out]    szArtist GTCHAR* address of output artist information 
*   @param[in]     u4MaxLen GUINT32 max length of szArtist
*
*   @return    MRESULT  define in msdk_error.h
*              
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
MRESULT     MP3Info_Wma_GetArtist(HMP3INFO hMP3Info, GTCHAR *szArtist, GUINT32 u4MaxLen);

/**
*   @brief get MP3 album information
*
*   @param[in]   hMP3Info  mp3info handle
*   @param[out]    szAlbum GTCHAR*  address of output album information
*   @param[in]     u4MaxLen GUINT32 max length of szAlbum
*
*   @return    MRESULT  define in msdk_error.h
*              
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
MRESULT     MP3Info_Wma_GetAlbum(HMP3INFO hMP3Info, GTCHAR *szAlbum, GUINT32 u4MaxLen);

/**
*   @brief get MP3 genre information
*
*   @param[in]   hMP3Info  mp3info handle
*   @param[out]    szGenre GTCHAR*  address of output genre information
*   @param[in]     u4MaxLen GUINT32 max length of szGenre
*
*   @return    MRESULT  define in msdk_error.h
*              
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
MRESULT     MP3Info_Wma_GetGenre(HMP3INFO hMP3Info, GTCHAR *szGenre, GUINT32 u4MaxLen);


/**
*   @brief get MP3 year information
*
*   @param[in]   hMP3Info  mp3info handle
*   @param[out]    szYear GTCHAR*  address of output year information
*   @param[in]    u4MaxLen GUINT32 max length  of szYear
*
*   @return    MRESULT  define in msdk_error.h
*              
*
*   @see     MediaGraph_QueryInterface()
*   @see     msdk_error.h
*
**/
MRESULT     MP3Info_Wma_GetYear(HMP3INFO hMP3Info, GTCHAR *szYear, GUINT32 u4MaxLen);

/**
*@}
*/

#ifdef __cplusplus
}
#endif

#endif //_MSDK_MP3_H_
