// This is a part of the WceUi library.
// Copyright (C) WceUi Team
// All rights reserved.
//
// 
//
#pragma once
#include "WceUiCommon.h"
#include "WceUiObject.h"
#include "WceUiGenericBitmap.h"
#include "WceUiXml.h"

struct tWCEUI_ADD_TO_RANDOM
{
	tWCEUI_ADD_TO_RANDOM(LPCTSTR pAnimateClassName);
};

#define WCEUI_ADD_TO_RANDOM(class_name) \
	static tWCEUI_ADD_TO_RANDOM _add_to_random##class_name(\
	WCEUI_CLASS_NAME(class_name)\
	);

#define WCEUI_DEFAUlT_FRAME_LEN   33
#define WCEUI_DEFAUlT_FRAME_COUNT 12

class CWceUiWndAnimate : public CWceUiObject
{
	WCEUI_DYNAMIC_BEGIN(CWceUiWndAnimate, CWceUiObject)
public:
	CWceUiWndAnimate();
	~CWceUiWndAnimate();

	void Enable(BOOL bEnable);
	BOOL IsEnabled() const;

	void SetName(LPCTSTR pName);
	LPCTSTR GetName() const;

	void SetRect(LPCRECT lpRcAnimate);
	void GetRect(LPRECT lpRcAnimate) const;

	void SetFrameCount(int nFrameCount);
	int  GetFrameCount() const;
	
	//init from style file
	virtual BOOL Init(PCWceUiXmlElem pElem);

	virtual BOOL ReStart(IN OUT CWceUiGenericBitmap* pMixedBitmap,
		IN CWceUiGenericBitmap* pFromBitmap, 
		IN CWceUiGenericBitmap* pToBitmap);
	
	virtual BOOL Play(IN OUT CWceUiGenericBitmap* pMixedBitmap,
		IN CWceUiGenericBitmap* pFromBitmap, 
		IN CWceUiGenericBitmap* pToBitmap,
		int	  nCurFrame,
		OUT LPINT lpPlayTime) = 0;

	virtual void Stop();

	virtual void Randomize();

	static CWceUiWndAnimate* GetRandomAnimate();


private:
	BOOL			m_bEnabled;		//is animate enabled?
	RECT			m_rcAnimate;	//the animate rect.
	int				m_nFrameCount;  //the frame count
	TCHAR			m_szName[MAX_TAG_NAME_LEN];

protected:
	BOOL CheckParameter(CWceUiGenericBitmap* pMixedBitmap, 
		CWceUiGenericBitmap* pFromBitmap, 
		CWceUiGenericBitmap* pToBitmap);

private:
	CWceUiWndAnimate (const CWceUiWndAnimate &);
	CWceUiWndAnimate & operator=(const CWceUiWndAnimate &);
};

WCEUI_DYNAMIC_END(CWceUiWndAnimate, CWceUiObject)
