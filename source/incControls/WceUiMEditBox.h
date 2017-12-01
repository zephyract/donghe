#pragma once
#include "WceUiLayer.h"
#include "WceUiEditBox.h"
#include "WceUiLoadBitmap.h"

class CWceUiMEditBox : public CWceUiEditBox
{
	WCEUI_DYNCREATE_BEGIN(CWceUiMEditBox, CWceUiEditBox);
public:
	CWceUiMEditBox(void);
	virtual ~CWceUiMEditBox(void);

	void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnDrawText(CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap);


protected:
	void DrawScrollButton(CWceUiGenericBitmap* pDrawBitmap, const LPRECT lprc);
	CWceUiLoadBitmap m_bmpLineBg;
	CWceUiLoadBitmap m_bmpSBTop;
	CWceUiLoadBitmap m_bmpSBMiddle;
	CWceUiLoadBitmap m_bmpSBBottom;
};
WCEUI_DYNCREATE_END(CWceUiMEditBox, CWceUiEditBox);
