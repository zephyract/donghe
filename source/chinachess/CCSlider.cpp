#include "stdafx.h"
#include "CCSlider.h"

int CCCSlider::SetStep( int nStep )
{
	m_nstep =nStep;
	return true;
}

int CCCSlider::StepIt()
{
	int nPos = m_nPos + m_nstep;
	if (nPos>m_nMaxPos)
	{
		nPos = m_nMaxPos;
	}
	SetPos(nPos);
	return true;
}

int CCCSlider::StepToEnd()
{
	SetPos(m_nMaxPos);
	return true;
}