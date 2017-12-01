// IDAlphabeta.h: interface for the CIDAlphabetaEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDALPHABETA_H__0DE91721_FF04_4487_AFE1_7CB884A22775__INCLUDED_)
#define AFX_IDALPHABETA_H__0DE91721_FF04_4487_AFE1_7CB884A22775__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchEngine.h"

//�������������
class CIDAlphabetaEngine:public CSearchEngine   
{
public:
	CIDAlphabetaEngine();
	virtual ~CIDAlphabetaEngine();

public:
	virtual void SearchAGoodMove(BYTE position[10][9]);

protected:
	int AlphaBeta(int nDepth,int alpha,int beta);

protected:
	int m_nTimeCount;//��ʱ����
};

#endif // !defined(AFX_IDALPHABETA_H__0DE91721_FF04_4487_AFE1_7CB884A22775__INCLUDED_)
