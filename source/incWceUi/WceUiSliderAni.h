// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiWndAnimate.h"

WCEUI_ADD_TO_RANDOM(CWceUiSliderAni);
class CWceUiSliderAni : public CWceUiWndAnimate
{
	WCEUI_DYNCREATE_BEGIN(CWceUiSliderAni, CWceUiWndAnimate)
public:
	CWceUiSliderAni();
	virtual ~CWceUiSliderAni();

	virtual BOOL Init(PCWceUiXmlElem pElem);

	virtual BOOL Play(IN OUT CWceUiGenericBitmap* pMixedBitmap,
		IN CWceUiGenericBitmap* pFromBitmap, 
		IN CWceUiGenericBitmap* pToBitmap,
		int	  nCurFrame,
		OUT LPINT lpPlayTime);

	virtual void Randomize();

	void SetDirection(WCEUI_DIRECTION direction);
	WCEUI_DIRECTION GetDirection() const;


	void SetFrameLength(int nFrameLength);
	int  GetFrameLength() const;
private:
	WCEUI_DIRECTION m_Direction;
	int				m_nFrameLength;

private:
	CWceUiSliderAni (const CWceUiSliderAni &);
	CWceUiSliderAni & operator=(const CWceUiSliderAni &);
};
WCEUI_DYNCREATE_END(CWceUiSliderAni, CWceUiWndAnimate)