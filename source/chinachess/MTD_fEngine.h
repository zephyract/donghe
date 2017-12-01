// MTD_f.h: interface for the CMTD_fEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MTD_F_H__CFE83D8F_087F_496F_88E5_92C11F448F22__INCLUDED_)
#define AFX_MTD_F_H__CFE83D8F_087F_496F_88E5_92C11F448F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchEngine.h"
#include "TranspositionTable.h"
#include "HistoryHeuristic.h"

//MTD(f)��������
class CMTD_fEngine:public CSearchEngine,public CHistoryHeuristic,public CTranspositionTable  
{
public:
	CMTD_fEngine();
	virtual ~CMTD_fEngine();

public:
	virtual void SearchAGoodMove(BYTE position[10][9]);

protected:
	int FAlphaBeta(int nDepth,int alpha,int beta);//�����û����fail-softalpha-beta��������
	int Mtdf(int firstguess,int nDepth);		  //MTD(f)��������

protected:
	CHESSMOVE m_cmBackupBM;//���Ա�������߷��ı���
};

#endif // !defined(AFX_MTD_F_H__CFE83D8F_087F_496F_88E5_92C11F448F22__INCLUDED_)
