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
//left, right, up, down, lr-mid, ud-mid is valid
//mid-lr, mid-ud is invalid
//

WCEUI_ADD_TO_RANDOM(CWceUiComeAni);
class CWceUiComeAni : public CWceUiWndAnimate
{
	WCEUI_DYNCREATE_BEGIN(CWceUiComeAni, CWceUiWndAnimate)
public:
	CWceUiComeAni();
	virtual ~CWceUiComeAni();

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
	void DrawLR2Mid(CWceUiGenericBitmap* pMixedBitmap, CWceUiGenericBitmap* pFromBitmap, CWceUiGenericBitmap* pToBitmap, int nCurFrame, LPCRECT lpRcValid);
	void DrawUD2Mid(CWceUiGenericBitmap* pMixedBitmap, CWceUiGenericBitmap* pFromBitmap, CWceUiGenericBitmap* pToBitmap, int nCurFrame, LPCRECT lpRcValid);

	int					  m_nFrameLength;
	WCEUI_EIGHT_DIRECTION  m_Direction;

private:
	CWceUiComeAni (const CWceUiComeAni &);
	CWceUiComeAni & operator=(const CWceUiComeAni &);
};
WCEUI_DYNCREATE_END(CWceUiComeAni, CWceUiWndAnimate)