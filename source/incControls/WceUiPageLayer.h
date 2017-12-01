#pragma once
#include "WceUiScrollLayer.h"
#include "WceUiHTList.h"
#include <list>
using namespace std;

#define SL_HORZ		0
#define SL_VERT		1

#define ID_TIMER_SCROLL		1

class CWceUiPageLayer : public CWceUiScrollLayer
{
	WCEUI_DYNCREATE_BEGIN(CWceUiPageLayer, CWceUiScrollLayer);
public:
	CWceUiPageLayer(void);
	virtual ~CWceUiPageLayer(void);


	virtual void OnLButtonUp(UINT nFlags,	 POINT point);

	// ���õ�ǰҳ,�ڶ���������ָ���Ƿ񶯻�����
	BOOL SetToPage(int nPage, BOOL bAnim);
	// ��ȡ��ǰҳ
	int GetCurPage();
protected:
	// ����굯��ʱ�����𴰿ڹ�����У��λ�ã�������ֹͣ�����󣬻���øýӿ�
	virtual void OnScrollWindowEnd();


	// ��ȡ����ʱ�ľ��룬�����������ٶ������㣬
	// ������Ҳ�����Լ���������, ����ÿ�ι���ʱ��������������е�������
	virtual int GetScrollDistance(DWORD speed, POINT point);

	DWORD m_dwSpeed;


};

WCEUI_DYNCREATE_END(CWceUiPageLayer, CWceUiScrollLayer);
