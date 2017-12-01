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

	// 设置当前页,第二个参数可指定是否动画滚动
	BOOL SetToPage(int nPage, BOOL bAnim);
	// 获取当前页
	int GetCurPage();
protected:
	// 当鼠标弹起时会引起窗口滚动或校正位置，当窗口停止滚动后，会调用该接口
	virtual void OnScrollWindowEnd();


	// 获取滚动时的距离，根据鼠标滚动速度来计算，
	// 重载它也可以自己决定规则, 比如每次滚动时滚动距离必须是行的整数倍
	virtual int GetScrollDistance(DWORD speed, POINT point);

	DWORD m_dwSpeed;


};

WCEUI_DYNCREATE_END(CWceUiPageLayer, CWceUiScrollLayer);
