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

	// ֱ�ӹ������ڣ���ʵ�ʹ��������п����������һ�£���Ϊ���ڹ�������һ����Χ��
	// SetScrollRange()�������ù�����Χ
	void ScrollWindow(int nAmount, BOOL bRedraw=TRUE);

	// ���ж���Ч���Ĵ��ڹ���
	void AnimateScrollWindow(int Amount);

	// �õ���ǰ�Ĺ���λ��
	int GetScrollPos() const ;
	// ���ù���λ��,���ú󴰿ڻ��Զ���������Ӧλ��
	void SetScrollPos(int nPos, BOOL bRedraw=TRUE);
	// ���ڵ���ʱ�ö�������ʽ
	void SetScrollPosAnim(int nPos, BOOL bRedraw=TRUE);

	// ���ù�����Χ
	void SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw=TRUE);
	// �õ�������Χ
	void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos ) const;
	// �����͵ľ��룬�����������Ч������ֵ��Ϊ0����
	void SetExtraScroll(UINT nExtraScroll);


	// ��������ʱ��ÿ֡���ĵ�ʱ�䣬��ֵ��ʵ�ʻ���ʱ���ʱ�������壬����
	// ���ϵͳ������һ��ʱ�䣨����ʱ�����ü�ʱ��������ʱ��
	// ��λ: ms
	void SetScrollFrameTime(DWORD nTime);

	// �ⲿ����AnimateScrollWindow()ʱ��������ָ������֡,�ؼ������������û�������ٶ�
	void SetScrollFrame(int nFrame);

	//mouse messages
	virtual void OnLButtonDown(UINT nFlags,  POINT point);
	virtual void OnLButtonUp(UINT nFlags,	 POINT point);
	virtual void OnMouseMove(UINT nFlags,	 POINT point);
	virtual void OnLoseCapture();
	virtual void OnTimer(WORD wIDEvent);

protected:
	// ����굯��ʱ�����𴰿ڹ�����У��λ�ã�������ֹͣ�����󣬻���øýӿ�
	virtual void OnScrollWindowEnd();

	// ÿ�ι������ں󣬻���øú����������Ҫ��������һЩ������������������ش˺���
	// ���� editbox���ڹ����󣬶Թ���λ�ý���ˢ��
	virtual void OnScrollWindow(int nAmount);

	// ��굯��ʱ(mouse up)���жϸò����������Ƿ��й�������������л��
	// OnMouseScrolled()�� �������OnMouseClicked().
	// ������¼�����ʱ��Ҫ֪ͨ�����ڻ���Ҫ��������ʱ�������ش˺���
	virtual void OnMouseScrolled();
	virtual void OnMouseClicked();

	// ÿ������������ɺ󣬻���øýӿ�
	// ���������ͷ�ˣ��ٵ�������,
	// ���ؿ����������������������λ�ñ������е�������
	virtual void ExtraScrollWindow();

	// ���ڹ����������¼����������λ��
	void RefreshScrollButtonPos();

	// ��ȡ����ʱ�ľ��룬�����������ٶ������㣬
	// ������Ҳ�����Լ���������, ����ÿ�ι���ʱ��������������е�������
	virtual int GetScrollDistance(DWORD speed, POINT point);

	// 
	int AdjustMouseMove(int nAmount);

	// ������굯��ʱ�Ļ����ٶ�, ֻ����굯��ʱ����
	int GetScrollSpeed(POINT ptMouseUp);

	// �����ǰλ�ô�������λ�ã�����TRUE������Ѿ��ƶ����߽����ˣ�Extra Pos������ҪУ��λ���򷵻�FALSE
	BOOL IsAtNormalPos();

	// �����ǰλ�ô�������λ�û�Extra Pos������TRUE������Ѿ��ƶ���Extra Pos�߽����ˣ��򷵻�FALSE
	BOOL IsAtNormalExtraPos();


	void OnTimerScroll();

	// scroll range and current position
	int m_nMinPos;
	int m_nMaxPos;
	int m_nCurrentPos;
	int m_nExtraScroll;	// �϶�ʱ�������ķ�Χ���ɿ��������޸ĵ�ǰλ��, ʵ������Ч��

	// �������ĳ����뵱ǰλ��
	int m_nScrollButtonLength;
	int m_nScrollButtonPos;

	// record the previous point of the mouse track when LButton is down.
	POINT m_PreMouseMovePoint;	// ��껬��ʱ��ǰһ��mouse moveλ��
	int   m_nMouseMoveDirect;	// 0 - backward,  1 - forward

	// mouse status
	BOOL m_bIsMouseDown;
	BOOL m_bIsMouseMoved;

	// scroll mode(SL_HORZ or SL_VERT)
	int m_nScrollMode;

	// ������갴��ʱ��ʱ�估λ�ã���������Զ��������ٶ�
	// ����Щ�����Ҫ����������Щֵ���Ա��׼ȷ�ļ���(�磺��������仯��ͣ��ʱ��̫��)
	DWORD m_dwTickMouseMoveStart;
	POINT m_MouseMoveStartPoint;	

	// ��������ʱ����������֡
	int m_nScrollFrame;

	// ÿ֡����ʱ��
	DWORD m_nScrollFrameTime;

	// ���ƶ�������, ���ü�ʱ��ʵ�ֵ�
	int m_nScrollDistance;	// �����ľ���
	int m_nScrollCurrentFrame;	// ����ʱ��¼��ǰ�ǵڼ�֡

	DWORD m_dwTickScrollBegin;		// ��ʼ����ʱ��tickcount
	int m_nSpeedScrollBegin;		// ��ʼ�ٶ�
	int m_nShiftScrolled;			// �ۼ��Ѿ������˵ľ���


	list<PCWceUiLayer> m_ChildList;
	void MoveLayer(PCWceUiLayer pLayer, int x, int y);
	void GetAllChilds();
};

WCEUI_DYNCREATE_END(CWceUiScrollLayer, CWceUiLayer);
