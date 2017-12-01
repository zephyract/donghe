// AlphaBetaAndTT.h: interface for the CAlphaBeta_TTEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALPHABETAANDTT_H__F7C6541D_B14D_4995_806D_34C19622DEE1__INCLUDED_)
#define AFX_ALPHABETAANDTT_H__F7C6541D_B14D_4995_806D_34C19622DEE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchEngine.h"
#include "TranspositionTable.h"

//加置换表的Alpha-Beta搜索引擎
class CAlphaBeta_TTEngine:public CSearchEngine,public CTranspositionTable   
{
public:
	CAlphaBeta_TTEngine();
	virtual ~CAlphaBeta_TTEngine();

public:
	virtual void SearchAGoodMove(BYTE position[10][9]);

protected:
	int AlphaBeta(int nDepth, int alpha, int beta);
};
 
#endif // !defined(AFX_ALPHABETAANDTT_H__F7C6541D_B14D_4995_806D_34C19622DEE1__INCLUDED_)
