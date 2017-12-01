// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiWndAnimate.h"

//
//left, right, up, down, mid-lr, mid-ud is valid
//lr-mid, ud-mid is invalid
//


WCEUI_ADD_TO_RANDOM(CWceUiGoAni);
class CWceUiGoAni : public CWceUiWndAnimate
{
	WCEUI_DYNCREATE_BEGIN(CWceUiGoAni, CWceUiWndAnimate)
public:
	CWceUiGoAni();
	virtual ~CWceUiGoAni();

	virtual BOOL Init(PCWceUiXmlElem pElem);

	virtual BOOL Play(IN OUT CWceUiGenericBitmap* pMixedBitmap,
		IN CWceUiGenericBitmap* pFromBitmap, 
		IN CWceUiGenericBitmap* pToBitmap,
		int	  nCurFrame,
		OUT LPINT lpPlayTime);

	virtual void Randomize();

	void SetDirection(WCEUI_EIGHT_DIRECTION direction);
	WCEUI_EIGHT_DIRECTION GetDirection() const;

	void SetFrameLength(int nFrameLength);
	int  GetFrameLength() const;

private:
	void DrawLeft  (CWceUiGenericBitmap* pMixedBitmap, CWceUiGenericBitmap* pFromBitmap, CWceUiGenericBitmap* pToBitmap, int nCurFrame, LPCRECT lpRcValid);
	void DrawRight (CWceUiGenericBitmap* pMixedBitmap, CWceUiGenericBitmap* pFromBitmap, CWceUiGenericBitmap* pToBitmap, int nCurFrame, LPCRECT lpRcValid);
	void DrawUp    (CWceUiGenericBitmap* pMixedBitmap, CWceUiGenericBitmap* pFromBitmap, CWceUiGenericBitmap* pToBitmap, int nCurFrame, LPCRECT lpRcValid);
	void DrawDown  (CWceUiGenericBitmap* pMixedBitmap, CWceUiGenericBitmap* pFromBitmap, CWceUiGenericBitmap* pToBitmap, int nCurFrame, LPCRECT lpRcValid);
	void DrawMid2LR(CWceUiGenericBitmap* pMixedBitmap, CWceUiGenericBitmap* pFromBitmap, CWceUiGenericBitmap* pToBitmap, int nCurFrame, LPCRECT lpRcValid);
	void DrawMID2UD(CWceUiGenericBitmap* pMixedBitmap, CWceUiGenericBitmap* pFromBitmap, CWceUiGenericBitmap* pToBitmap, int nCurFrame, LPCRECT lpRcValid);

	int					  m_nFrameLength;
	WCEUI_EIGHT_DIRECTION  m_Direction;

private:
	CWceUiGoAni (const CWceUiGoAni &);
	CWceUiGoAni & operator=(const CWceUiGoAni &);
};
WCEUI_DYNCREATE_END(CWceUiGoAni, CWceUiWndAnimate)