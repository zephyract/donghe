#pragma once
#include "WceUiLayer.h"
#include "WceUiScrollLayer.h"
#include "WceUiEditBox.h"
#include "WceUiLoadBitmap.h"

// single line edit box control
class CWceUiSEditBox : public CWceUiEditBox
{
	WCEUI_DYNCREATE_BEGIN(CWceUiSEditBox, CWceUiEditBox);
public:
	CWceUiSEditBox(void);
	virtual ~CWceUiSEditBox(void);

	void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnDrawText(CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap);


protected:
	void DrawScrollButton(CWceUiGenericBitmap* pDrawBitmap, const LPRECT lprc);
	CWceUiLoadBitmap m_bmpSBLeft;
	CWceUiLoadBitmap m_bmpSBCenter;
	CWceUiLoadBitmap m_bmpSBRight;	
};
WCEUI_DYNCREATE_END(CWceUiSEditBox, CWceUiEditBox);
