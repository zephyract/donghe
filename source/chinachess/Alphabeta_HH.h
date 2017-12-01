// Alphabeta_HH.h: interface for the CAlphabeta_HHEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALPHABETA_HH_H__150772CA_2A04_457E_8B7A_3B9DBB6DCB66__INCLUDED_)
#define AFX_ALPHABETA_HH_H__150772CA_2A04_457E_8B7A_3B9DBB6DCB66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchEngine.h"
#include "HistoryHeuristic.h"

//����ʷ������Alpha-Beta��������
class CAlphabeta_HHEngine:public CSearchEngine,public CHistoryHeuristic  
{
public:
	CAlphabeta_HHEngine();
	virtual ~CAlphabeta_HHEngine();

public:
	virtual void SearchAGoodMove(BYTE position[10][9]);

protected:
	int AlphaBeta(int nDepth,int alpha,int beta);
};

#endif // !defined(AFX_ALPHABETA_HH_H__150772CA_2A04_457E_8B7A_3B9DBB6DCB66__INCLUDED_)
