#pragma once
#include "WceUiLayer.h"
#include "WceUiHTList.h"
#include <list>
using namespace std;

#define SL_HORZ		0
#define SL_VERT		1

#define ID_TIMER_SCROLL		1

enum
{
	WCEUI_MOUSE_MOVE_BACKWARD,
	WCEUI_MOUSE_MOVE_FORWARD
};


class CWceUiScrollLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CWceUiScrollLayer, CWceUiLayer);
public:
	CWceUiScrollLayer(void);
	virtual ~CWceUiScrollLayer(void);

	// Horizontal or vertical mode.
	int GetScrollMode();
	void SetScrollMode(int nMode);

	// 直接滚动窗口，但实际滚动距离有可能与参数不一致，因为窗口滚动是有一定范围的
	// SetScrollRange()用来设置滚动范围
	void ScrollWindow(int nAmount, BOOL bRedraw=TRUE);

	// 带有动画效果的窗口滚动
	void AnimateScrollWindow(int Amount);

	// 得到当前的滚动位置
	int GetScrollPos() const ;
	// 设置滚动位置,设置后窗口会自动调整到相应位置
	void SetScrollPos(int nPos, BOOL bRedraw=TRUE);
	// 窗口调整时用动画的形式
	void SetScrollPosAnim(int nPos, BOOL bRedraw=TRUE);

	// 设置滚动范围
	void SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw=TRUE);
	// 得到滚动范围
	void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos ) const;
	// 可推送的距离，如果不需这种效果，将值设为0即可
	void SetExtraScroll(UINT nExtraScroll);


	// 期望滑动时画每帧所耗的时间，该值比实际画的时间大时才有意义，这种
	// 情况系统会休眠一段时间（阻塞时）或将用记时器来控制时长
	// 单位: ms
	void SetScrollFrameTime(DWORD nTime);

	// 外部调用AnimateScrollWindow()时，可能需指定多少帧,控件本身这种情况没法计算速度
	void SetScrollFrame(int nFrame);

	//mouse messages
	virtual void OnLButtonDown(UINT nFlags,  POINT point);
	virtual void OnLButtonUp(UINT nFlags,	 POINT point);
	virtual void OnMouseMove(UINT nFlags,	 POINT point);
	virtual void OnLoseCapture();
	virtual void OnTimer(WORD wIDEvent);

protected:
	// 当鼠标弹起时会引起窗口滚动或校正位置，当窗口停止滚动后，会调用该接口
	virtual void OnScrollWindowEnd();

	// 每次滚动窗口后，会调用该函数，如果需要滚动后作一些操作，派生类可以重载此函数
	// 比如 editbox会在滚动后，对光标的位置进行刷新
	virtual void OnScrollWindow(int nAmount);

	// 鼠标弹起时(mouse up)会判断该操作过程中是否有滚动操作，如果有会调
	// OnMouseScrolled()， 否则调用OnMouseClicked().
	// 如果该事件发生时需要通知父窗口或需要其它处理时，可重载此函数
	virtual void OnMouseScrolled();
	virtual void OnMouseClicked();

	// 每次正常滚动完成后，会调用该接口
	// 如果滚动过头了，再调整回来,
	// 重载可以作其它调整，比如结束位置必须是行的整数倍
	virtual void ExtraScrollWindow();

	// 窗口滚动后，需重新计算滚动条的位置
	void RefreshScrollButtonPos();

	// 获取滚动时的距离，根据鼠标滚动速度来计算，
	// 重载它也可以自己决定规则, 比如每次滚动时滚动距离必须是行的整数倍
	virtual int GetScrollDistance(DWORD speed, POINT point);

	// 
	int AdjustMouseMove(int nAmount);

	// 计算鼠标弹起时的滑动速度, 只在鼠标弹起时调用
	int GetScrollSpeed(POINT ptMouseUp);

	// 如果当前位置处于正常位置，返回TRUE，如果已经移动到边界外了（Extra Pos），需要校正位置则返回FALSE
	BOOL IsAtNormalPos();

	// 如果当前位置处于正常位置或Extra Pos，返回TRUE，如果已经移动到Extra Pos边界外了，则返回FALSE
	BOOL IsAtNormalExtraPos();


	void OnTimerScroll();

	// scroll range and current position
	int m_nMinPos;
	int m_nMaxPos;
	int m_nCurrentPos;
	int m_nExtraScroll;	// 拖动时允许超过的范围，松开鼠标后再修改当前位置, 实现推送效果

	// 滚动条的长度与当前位置
	int m_nScrollButtonLength;
	int m_nScrollButtonPos;

	// record the previous point of the mouse track when LButton is down.
	POINT m_PreMouseMovePoint;	// 鼠标滑动时，前一个mouse move位置
	int   m_nMouseMoveDirect;	// 0 - backward,  1 - forward

	// mouse status
	BOOL m_bIsMouseDown;
	BOOL m_bIsMouseMoved;

	// scroll mode(SL_HORZ or SL_VERT)
	int m_nScrollMode;

	// 记下鼠标按下时的时间及位置，方便计算自动滚动的速度
	// 但有些情况需要重新设置这些值，以便更准确的计算(如：滚动方向变化或停留时间太长)
	DWORD m_dwTickMouseMoveStart;
	POINT m_MouseMoveStartPoint;	

	// 动画滚动时，滚动多少帧
	int m_nScrollFrame;

	// 每帧花的时间
	DWORD m_nScrollFrameTime;

	// 控制动画滚动, 是用计时器实现的
	int m_nScrollDistance;	// 滑动的距离
	int m_nScrollCurrentFrame;	// 滑动时记录当前是第几帧

	DWORD m_dwTickScrollBegin;		// 开始滚动时的tickcount
	int m_nSpeedScrollBegin;		// 初始速度
	int m_nShiftScrolled;			// 累计已经滚动了的距离


	list<PCWceUiLayer> m_ChildList;
	void MoveLayer(PCWceUiLayer pLayer, int x, int y);
	void GetAllChilds();
};

WCEUI_DYNCREATE_END(CWceUiScrollLayer, CWceUiLayer);
