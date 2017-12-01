// MTD_f.cpp: implementation of the CMTD_fEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MTD_fEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMTD_fEngine::CMTD_fEngine()
{

}

CMTD_fEngine::~CMTD_fEngine()
{

}

void CMTD_fEngine::SearchAGoodMove(BYTE position[][9])
{
	int firstguess=0;

	memcpy(CurPosition,position,90);
	m_nMaxDepth=m_nSearchDepth;
	CalculateInitHashKey(CurPosition);//�����ʼ���̵Ĺ�ϣֵ

	//��������� MTD(f)����
	for(m_nMaxDepth=1;m_nMaxDepth<=m_nSearchDepth;m_nMaxDepth++)
		firstguess=Mtdf(firstguess,m_nMaxDepth);
	MakeMove(&m_cmBackupBM);
	memcpy(position,CurPosition,90);
	//return 0;
}

int CMTD_fEngine::Mtdf(int firstguess, int nDepth)
{
	int g,lowerbound,upperbound,beta;
	
	g=firstguess;
	//��ʼ������Χ-20000 - 20000
	upperbound=20000;
	lowerbound=-20000;
	while(lowerbound<upperbound)
	{
		m_cmBackupBM=m_cmBestMove;
		//��������Ŀ���ƶ�
		if(g==lowerbound)
			beta=g+1;
		else 
			beta=g;
		g=FAlphaBeta(nDepth,beta-1,beta);//�մ�̽��
		if(g<beta)
			upperbound=g;
		else
			lowerbound=g;
	}

	return g;
}

int CMTD_fEngine::FAlphaBeta(int nDepth, int alpha, int beta)
{
	int current=-19999 ;
	int score;
	int Count,i;
	BYTE type;

	i=IsGameOver(CurPosition, nDepth);
	if(i!=0)
		return i;

	score=LookUpHashTable(alpha,beta,nDepth,(m_nMaxDepth-nDepth)%2);//�쿴��ϣ�������޵�ǰ�ڵ����Ч����
	if(score!=66666)
		return score;//���У�ֱ�ӷ����û���������

	if(nDepth<=0)//Ҷ�ӽڵ�ȡ��ֵ
	{
		current=m_pEval->Eveluate(CurPosition,(m_nMaxDepth-nDepth)%2,m_nUserChessColor);
		EnterHashTable(exact,current,nDepth,(m_nMaxDepth-nDepth)%2);//����ֵ�����û���
		return current;
	}

	Count=m_pMG->CreatePossibleMove(CurPosition,nDepth,(m_nMaxDepth-nDepth)%2,m_nUserChessColor);
   
	if( nDepth==m_nMaxDepth && NULL!=m_pThinkProgress )
	{
		//�ڸ��ڵ��趨������
		m_pThinkProgress->SetRange(0,Count);
		m_pThinkProgress->SetStep(1);
	}

	int eval_is_exact=0;//�������ͱ�־��Ϊ 0

	for (i=0;i<Count;i++) 
	{
		if( nDepth==m_nMaxDepth && NULL!=m_pThinkProgress )
			
            { 
                m_pThinkProgress->StepIt();//�߽�����
            }
		Hash_MakeMove(&m_pMG->m_MoveList[nDepth][i],CurPosition);//�����ӽڵ�Ĺ�ϣֵ
		type=MakeMove(&m_pMG->m_MoveList[nDepth][i]);			//�����ӽڵ�

		score=-FAlphaBeta(nDepth-1,-beta,-alpha);//�ݹ������ӽڵ�
		
		Hash_UnMakeMove(&m_pMG->m_MoveList[nDepth][i],type, CurPosition); 
		UnMakeMove(&m_pMG->m_MoveList[nDepth][i],type); 

				
		if(score>current)
		{
			current=score;
			if(nDepth == m_nMaxDepth)
				m_cmBestMove = m_pMG->m_MoveList[nDepth][i];
			if(score>=beta)//beta��֦
			{
				EnterHashTable(lower_bound, score, nDepth,(m_nMaxDepth-nDepth)%2);//���±߽�װ���û���
				return current;
			}
			if(score>alpha)
			{
				eval_is_exact=1;//�趨ȷ��ֵ��־
				alpha=score;//�������ֵ
			}
		}
	}

	//��������������û���
	if(eval_is_exact) 
		EnterHashTable(exact,alpha,nDepth,(m_nMaxDepth-nDepth)%2);
    else 
		EnterHashTable(upper_bound,current,nDepth,(m_nMaxDepth-nDepth)%2);

	return current;//���ؼ���ֵ /�߽�
}
