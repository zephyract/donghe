// Alphabeta_HH.cpp: implementation of the CAlphabeta_HHEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Alphabeta_HH.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlphabeta_HHEngine::CAlphabeta_HHEngine()
{

}

CAlphabeta_HHEngine::~CAlphabeta_HHEngine()
{

}

void CAlphabeta_HHEngine::SearchAGoodMove(BYTE position[][9])
{
	memcpy(CurPosition,position,90);

	m_nMaxDepth=m_nSearchDepth;
	ResetHistoryTable();//��ʼ����ʷ��¼��
	AlphaBeta(m_nMaxDepth,-20000,20000);
	m_umUndoMove.cmChessMove=m_cmBestMove;
	m_umUndoMove.nChessID=MakeMove(&m_cmBestMove);	

	memcpy(position,CurPosition,90);
}
#define  MAX_STEP 15
int CAlphabeta_HHEngine::AlphaBeta(int nDepth, int alpha,int beta)
{
	int score;
	int Count,i;
	BYTE type;

	i=IsGameOver(CurPosition, nDepth);
	if(i!=0)
		return i;

	if(nDepth<=0)//Ҷ�ӽڵ�ȡ��ֵ
		return m_pEval->Eveluate(CurPosition,(m_nMaxDepth-nDepth)%2,m_nUserChessColor);
	
	Count=m_pMG->CreatePossibleMove(CurPosition,nDepth,(m_nMaxDepth-nDepth)%2,m_nUserChessColor);
    int m_nstep = 0 ;
	if(nDepth==m_nMaxDepth && NULL!=m_pThinkProgress )
	{
		//�ڸ��ڵ��趨������
		m_pThinkProgress->SetRange(0,Count);
       
        if (Count > MAX_STEP)
        {
            m_nstep = Count /MAX_STEP;
            if (Count <MAX_STEP*2)
            {
                m_nstep = 2;
            }
        }
        else
        {
            m_nstep = 1;
        }
    //    RETAILMSG(1,(TEXT("Count  =  %d, m_nstep = %d\r\n"),Count,m_nstep));
		m_pThinkProgress->SetStep(m_nstep);
	}

	//ȡ�����߷�����ʷ�÷�
	for(i=0;i<Count;i++) 
		m_pMG->m_MoveList[nDepth][i].Score=GetHistoryScore(&m_pMG->m_MoveList[nDepth][i]);

	MergeSort(m_pMG->m_MoveList[nDepth],Count,0);//��Count���߷�����ʷ�÷ִ�С����

	int bestmove=-1;//��¼����߷��ı���

	for(i=0;i<Count;i++) 
	{
		if( nDepth==m_nMaxDepth && NULL!=m_pThinkProgress &&Count%m_nstep == 0)
            
            {
               m_pThinkProgress->StepIt();
            }
			

		type=MakeMove(&m_pMG->m_MoveList[nDepth][i]);
		score=-AlphaBeta(nDepth-1,-beta,-alpha);
		UnMakeMove(&m_pMG->m_MoveList[nDepth][i],type); 

		if(score>alpha)
		{
			alpha=score;
			if(nDepth==m_nMaxDepth)
				m_cmBestMove=m_pMG->m_MoveList[nDepth][i];
			bestmove=i;
		}
        if(alpha>=beta) 
		{
			bestmove=i;
			break;
		}			
	}
	if(bestmove!=-1)
		EnterHistoryScore(&m_pMG->m_MoveList[nDepth][bestmove],nDepth);//������߷�������ʷ��¼��

	return alpha;
}
