#pragma once
#include "WceUiLayer.h"
#include "MSDK_Audio.h"
#include "MSDK_Audio_wma.h"

#define EQ_MAX_FREQ_CNT		16
#define SRC_MODE_DVD		0
#define SRC_MODE_MSK		1
#define SRC_MODE_MSK_WMA	2


class CSpectrumLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CSpectrumLayer, CWceUiLayer);
public:
	CSpectrumLayer();
	~CSpectrumLayer();
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	void SetAudioInfo(HAUDIOINFO hAudioInfo) { m_hAudioInfo = hAudioInfo; }
	void SetSrcMode(int nMode) { m_nMode = nMode; }
	
protected:
	void DrawBmp(CWceUiGenericBitmap* pWndBitmap,int x,int y,CWceUiGenericBitmap* pSrcBitmap);

protected:
	HAUDIOINFO  m_hAudioInfo;
	int			m_nMode;	// SRC_MODE_MSK in default
};

WCEUI_DYNCREATE_END(CSpectrumLayer, CWceUiLayer);
