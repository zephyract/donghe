// FAlphaBetaEngine.h: interface for the CFAlphaBetaEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FALPHABETAENGINE_H__5102001C_EF47_4C16_B8F7_59290815310D__INCLUDED_)
#define AFX_FALPHABETAENGINE_H__5102001C_EF47_4C16_B8F7_59290815310D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchEngine.h"

//fail-softalpha-beta��������
class CFAlphaBetaEngine:public CSearchEngine
{
public:
	CFAlphaBetaEngine();
	virtual ~CFAlphaBetaEngine();

public:
	virtual void SearchAGoodMove(BYTE position[10][9]);

protected:
	int FAlphaBeta(int depth,int alpha,int beta);//fail-softalpha-beta�������������ӿں�������
};

#endif // !defined(AFX_FALPHABETAENGINE_H__5102001C_EF47_4C16_B8F7_59290815310D__INCLUDED_)
