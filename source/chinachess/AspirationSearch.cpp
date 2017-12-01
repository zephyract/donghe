// AspirationSearch.cpp: implementation of the CAspirationSearch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AspirationSearch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAspirationSearch::CAspirationSearch()
{

}

CAspirationSearch::~CAspirationSearch()
{

}

void CAspirationSearch::SearchAGoodMove(BYTE position[][9])
{
	int x,y;

	memcpy(CurPosition,position,90);
	
	//�Ƚ���ǳ���������²�Ŀ��ֵ��Χ
	m_nMaxDepth=m_nSearchDepth-1;
	x=FAlphaBeta(m_nMaxDepth,-20000,20000);
	
	if ( NULL!=m_pThinkProgress )
	{
		m_pThinkProgress->SetPos(0);
	}
	//��Ŀ��ֵ������С���ڽ�������
	m_nMaxDepth=m_nSearchDepth;
	y=FAlphaBeta(m_nMaxDepth,x-50,x+50);

	if(y<x-50)//fail-low research
	{
		if( NULL!=m_pThinkProgress ) m_pThinkProgress->SetPos(0);
		FAlphaBeta(m_nMaxDepth,-20000,y);
	}
	if(y>x+50)//fail-high research
	{
		if( NULL!=m_pThinkProgress ) m_pThinkProgress->SetPos(0);
		FAlphaBeta(m_nMaxDepth,y,20000);
	}
	
	m_umUndoMove.cmChessMove=m_cmBestMove;
	m_umUndoMove.nChessID=MakeMove(&m_cmBestMove);	
	
	memcpy(position,CurPosition,90);//���߹������̴���
}
