// AspirationSearch.h: interface for the CAspirationSearch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASPIRATIONSEARCH_H__9F3E2321_09AB_4FD4_BCB7_E3E2C8B24E0A__INCLUDED_)
#define AFX_ASPIRATIONSEARCH_H__9F3E2321_09AB_4FD4_BCB7_E3E2C8B24E0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FAlphaBetaEngine.h"

//¿ÊÍûËÑË÷ÒýÇæ
class CAspirationSearch:public CFAlphaBetaEngine
{
public:
	CAspirationSearch();
	virtual ~CAspirationSearch();

public:
	virtual void SearchAGoodMove(BYTE position[10][9]);
};

#endif // !defined(AFX_ASPIRATIONSEARCH_H__9F3E2321_09AB_4FD4_BCB7_E3E2C8B24E0A__INCLUDED_)
