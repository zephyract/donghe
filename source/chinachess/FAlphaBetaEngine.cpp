// FAlphaBetaEngine.cpp: implementation of the CFAlphaBetaEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FAlphaBetaEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFAlphaBetaEngine::CFAlphaBetaEngine()
{

}

CFAlphaBetaEngine::~CFAlphaBetaEngine()
{

}

void CFAlphaBetaEngine::SearchAGoodMove(BYTE position[][9])
{
	memcpy(CurPosition,position,90);	 //���ƴ��������

	m_nMaxDepth=m_nSearchDepth;			 //�趨�������
	FAlphaBeta(m_nMaxDepth,-20000,20000);//����fail-softalpha-beta
	m_umUndoMove.cmChessMove=m_cmBestMove;
	m_umUndoMove.nChessID=MakeMove(&m_cmBestMove);	

	memcpy(position,CurPosition,90);     //���߹������̴���
}

int CFAlphaBetaEngine::FAlphaBeta(int nDepth,int alpha,int beta)
{
	int current=-20000;//current��ʼΪ��Сֵ
	int score;
	int Count,i;
	BYTE type;
	
	i=IsGameOver(CurPosition,nDepth);//���˽ڵ���Ϸ�Ƿ����
	if(i!=0)
		return i;//�˽ڵ���Ϸ���������ؼ���/��Сֵ

	//�����Ҷ�ӽڵ�ȡ��ֵ
	if(nDepth<=0)
		return m_pEval->Eveluate(CurPosition,(m_nMaxDepth-nDepth)%2,m_nUserChessColor);
	
	Count=m_pMG->CreatePossibleMove(CurPosition,nDepth,(m_nMaxDepth-nDepth)%2,m_nUserChessColor);
   
	if(nDepth==m_nMaxDepth && NULL!=m_pThinkProgress )
	{
		//�ڸ��ڵ��趨������
		m_pThinkProgress->SetRange(0,Count);
		m_pThinkProgress->SetStep(1);
	}

	for(i=0;i<Count;i++)
	{
		if(nDepth==m_nMaxDepth && NULL!=m_pThinkProgress )
			
            { 
                m_pThinkProgress->StepIt();//�߽�����
            }
		type=MakeMove(&m_pMG->m_MoveList[nDepth][i]);
		score=-FAlphaBeta(nDepth-1,-beta,-alpha);
		UnMakeMove(&m_pMG->m_MoveList[nDepth][i],type);

		if(score>current)
		{
			current=score;//�������ֵ

			//���������ڵ�ʱ��¼����߷�
			if(nDepth==m_nMaxDepth)
				m_cmBestMove=m_pMG->m_MoveList[nDepth][i];

			if(score>=alpha)
				alpha=score;//�޸�alpha�߽�

			if(score>=beta)
				break;//beta��֦
		}
	}

	return current;//���ؼ���ֵ��߽�ֵ
}
