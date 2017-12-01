// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"

#include "WceUiWndAnimate.h"

WCEUI_ADD_TO_RANDOM(CWceUiBlendAni);
class CWceUiBlendAni : public CWceUiWndAnimate
{
	WCEUI_DYNCREATE_BEGIN(CWceUiBlendAni, CWceUiWndAnimate)
public:
	CWceUiBlendAni();
	virtual ~CWceUiBlendAni();

	virtual BOOL Init(PCWceUiXmlElem pElem);

	virtual BOOL Play(IN OUT CWceUiGenericBitmap* pMixedBitmap,
		IN CWceUiGenericBitmap* pFromBitmap, 
		IN CWceUiGenericBitmap* pToBitmap,
		int	  nCurFrame,
		OUT LPINT lpPlayTime);

	virtual void Randomize();

	void SetFrameLength(int nFrameLength);
	int  GetFrameLength() const;
private:
	int		m_nFrameLength;

private:
	CWceUiBlendAni (const CWceUiBlendAni &);
	CWceUiBlendAni & operator=(const CWceUiBlendAni &);
};
WCEUI_DYNCREATE_END(CWceUiBlendAni, CWceUiWndAnimate)