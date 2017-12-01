#pragma once
#include "WceUiSlider.h"

class CCCSlider : public CWceUiSlider
{
	WCEUI_DYNCREATE_BEGIN(CCCSlider, CWceUiSlider);


public:
	int SetStep(int nStep);
	int StepIt();
	int StepToEnd();


protected:
	int m_nstep;
};


WCEUI_DYNCREATE_END(CCCSlider, CWceUiSlider);
