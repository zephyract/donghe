// AlphaBetaEngine.h: interface for the CAlphaBetaEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALPHABETAENGINE_H__C39E0A9F_C8EA_42E7_A56F_B286548405F6__INCLUDED_)
#define AFX_ALPHABETAENGINE_H__C39E0A9F_C8EA_42E7_A56F_B286548405F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchEngine.h"

//alpha-beta��������
class CAlphaBetaEngine:public CSearchEngine
{
public:
	CAlphaBetaEngine();
	virtual ~CAlphaBetaEngine();

public:
	virtual void SearchAGoodMove(BYTE position[10][9]);//��������õĽӿڣ�Ϊ��ǰ�������һ������

protected:
	int AlphaBeta(int depth,int alpha,int beta);//alpha-beta����������
};

#endif // !defined(AFX_ALPHABETAENGINE_H__C39E0A9F_C8EA_42E7_A56F_B286548405F6__INCLUDED_)
