// NegaMaxEngine.h: interface for the CNegaMaxEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEGAMAXENGINE_H__06536E97_58ED_40DD_8581_2D2E1D3CE9B8__INCLUDED_)
#define AFX_NEGAMAXENGINE_H__06536E97_58ED_40DD_8581_2D2E1D3CE9B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchEngine.h"

//负极大值搜索引擎
class CNegaMaxEngine:public CSearchEngine
{
public:	
	CNegaMaxEngine();
	virtual ~CNegaMaxEngine();
	
public:
	virtual void SearchAGoodMove(BYTE position[10][9]);//走下一步

protected:
	int NegaMax(int nDepth);//负极大值搜索函数
};

#endif // !defined(AFX_NEGAMAXENGINE_H__06536E97_58ED_40DD_8581_2D2E1D3CE9B8__INCLUDED_)
