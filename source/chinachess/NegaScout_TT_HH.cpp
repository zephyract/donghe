// NegaScout_TT_HH.cpp: implementation of the CNegaScout_TT_HH class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NegaScout_TT_HH.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNegaScout_TT_HH::CNegaScout_TT_HH()
{

}

CNegaScout_TT_HH::~CNegaScout_TT_HH()
{

}

void CNegaScout_TT_HH::SearchAGoodMove(BYTE position[][9])
{
	memcpy(CurPosition,position,90);

	m_nMaxDepth=m_nSearchDepth;
	CalculateInitHashKey(CurPosition);
	ResetHistoryTable();
	NegaScout(m_nMaxDepth,-20000,20000);
	m_umUndoMove.cmChessMove=m_cmBestMove;
	m_umUndoMove.nChessID=MakeMove(&m_cmBestMove);	

	memcpy(position,CurPosition,90);
}

int CNegaScout_TT_HH::NegaScout(int depth, int alpha, int beta)
{
	int Count,i;
	BYTE type;
	int a,b,t;
	int side;
	int score;
	
	i=IsGameOver(CurPosition, depth);
	if(i!=0)
		return i;
	
	side=(m_nMaxDepth-depth)%2;//���㵱ǰ�ڵ�����ͣ����� 0/��С 1
	
	score=LookUpHashTable(alpha,beta,depth,side); 
	if(score!=66666) 
		return score;
	
	if(depth<=0)//Ҷ�ӽڵ�ȡ��ֵ
	{
		score=m_pEval->Eveluate(CurPosition,side,m_nUserChessColor);
		EnterHashTable(exact,score,depth,side);//����ֵ�����û���
		return score;
	}
	
	Count=m_pMG->CreatePossibleMove(CurPosition,depth,side,m_nUserChessColor);
	
	for(i=0;i<Count;i++) 
		m_pMG->m_MoveList[depth][i].Score=GetHistoryScore(&m_pMG->m_MoveList[depth][i]);
	MergeSort(m_pMG->m_MoveList[depth],Count,0);
	
	int bestmove=-1;
	
    a=alpha;
    b=beta;

    int eval_is_exact=0;

    for(i=0;i<Count;i++) 
	{
		Hash_MakeMove(&m_pMG->m_MoveList[depth][i],CurPosition);
		type=MakeMove(&m_pMG->m_MoveList[depth][i]);
		
		t=-NegaScout(depth-1,-b,-a);
		
		if(t>a && t<beta && i>0) 
		{
			//���ڵ�һ����Ľڵ㣬������������ failhigh
			a=-NegaScout(depth-1,-beta,-t);     /* re-search */
			eval_is_exact=1;//����������Ϊ��ȷֵ
			if(depth==m_nMaxDepth)
				m_cmBestMove=m_pMG->m_MoveList[depth][i];
			bestmove=i;
		}
		Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i],type,CurPosition); 
		UnMakeMove(&m_pMG->m_MoveList[depth][i],type); 
		if(a<t)
		{
			eval_is_exact=1;
			a=t;
			if(depth==m_nMaxDepth)
				m_cmBestMove=m_pMG->m_MoveList[depth][i];
		}
		if(a>= beta) 
		{
			EnterHashTable(lower_bound,a,depth,side);
			EnterHistoryScore(&m_pMG->m_MoveList[depth][i],depth);
			return a;
		}
		b=a+1;                      /* set new null window */
	}
	if(bestmove!=-1)
		EnterHistoryScore(&m_pMG->m_MoveList[depth][bestmove], depth);
	if (eval_is_exact) 
		EnterHashTable(exact,a,depth,side);
	else 
		EnterHashTable(upper_bound,a,depth,side);

	return a;
}
