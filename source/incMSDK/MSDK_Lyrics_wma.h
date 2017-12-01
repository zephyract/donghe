
#ifndef _MSDK_LYRICS_WMA_H_
#define _MSDK_LYRICS_WMA_H_

#include <MSDK_Defs.h>

#ifdef __cplusplus
extern "C" {
#endif

MRESULT LyricInfo_Wma_GetCount(HLYRICINFO hLyricInfo, UINT32 *pu4Count);

MRESULT LyricInfo_Wma_GetPts(HLYRICINFO hLyricInfo, INT32 i4Index, UINT32 *pu4Pts);


MRESULT LyricInfo_Wma_GetText(HLYRICINFO hLyricInfo, INT32 i4Index, GTCHAR *szLyric, UINT32 u4MaxLen);

MRESULT LyricInfo_Wma_GetIndexByPts(HLYRICINFO hLyricInfo, GUINT64 u8CurTime, UINT32 *pu4Index);


#ifdef __cplusplus
}
#endif

#endif //_MSDK_LYRICS_H_


