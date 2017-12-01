#pragma once
#include "wceuidlg.h"
#include "BaseDlg.h"

#define TIMER_ID_ANIM_SHOWWINDOW		1

const int FRAME_ANMI_SHOWWINDOW = 8;
const int SPEED_ANIM_SHOWWINDOW = 1;	// �������ü�ʱ����ԽСԽ��

enum
{
	TOP_LAYER_BK = 0,
	TOP_LAYER_TOPBAR,
	TOP_LAYER_HOME,
	TOP_LAYER_BACK,
	TOP_LAYER_TITLE,
	TOP_LAYER_COUNT
};

class CAnimDlg : public CBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CAnimDlg, CBaseDlg)
public:
	CAnimDlg(void);
	~CAnimDlg(void);	

	virtual void OnTimer(WORD wIDEvent);

	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	// ��������ʵ�ִ˷���,DVD��MSDK��AVIN����Ƶ��������ʾ�ӿڲ�ͬ
	virtual void ShowVideo(BOOL bShow, RECT* prect = NULL);
protected:
	virtual void OnInitDialog();
	virtual void OnDrawBegin();
	virtual void OnDrawEnd();

	void OnActiveateWnd(BOOL bActive);

	// �����任����������������ش˺�����ɲ�ͬ�Ķ���
	// ����ֻ��ɴ������浯�������������Ķ���
	// ��ָ���ľ�������̬�Ŵ�ȫ�������ȫ����С��ָ���ľ�����
	virtual void DoAnimate();

	// ֪ͨ������ʾ��ʼ
	virtual void OnAnimateBegin();
	// ֪ͨ������ʾ����
	virtual void OnAnimateEnd();
	// ���ڴ�����������֡�����Է��ʳ�Ա����
	virtual void OnAnimating();

public:
	virtual void AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData);

	// ��������Ҫ��������Ч��ʱ�����øýӿڻ�ȡ��ǰ֡����͸����
	static int CalcTextTransparent(BOOL bShow, int nCurFrame, int nTotalFrame);

	BOOL IsAnimating();
	int GetCurrentFrame();

protected:
	// �����ĸ���������˳��ֱ��� zoomX, zoomY, offsetX, offsetY
	void AnimGetParameters(OUT float* params, int nStep, int nTotal);


	BOOL		m_bAnimShow;		// show window or hide window
	ANIM_MODE	m_nAnimMode;	// ������ʽ
	DWORD		m_dwAnimData;		// ��������ʼ�����λ��

	BOOL		m_bIsAnimating;	// TRUE��ʾ���ڶ�����FALSE��ʾ�Ѿ���������
	float		m_transparent;		// ����ʱ���޸�͸���ȣ���������֮ǰ��ֵ��������ɺ�ָ�֮ǰ��ֵ
	int			m_nCurrentFrame;	// ��0��ʼ����

	// ��������ʱ�ľ�����.
	RECT		m_rcAnimRect;

// ANIM_MODE_UPLEVEL���¼�֮�������һ�ֶ���Ч��
protected:
	void AnimGetParameters_uplevel(OUT float* params, int nStep, int nTotal);

	//  ����ʱ��ʵ��top bar����ؼ���һЩ����Ч��
	void ResetTopLayers();
	CWceUiLayer* m_pLayerTop[TOP_LAYER_COUNT];
	RECT		 m_rcLayerTop[TOP_LAYER_COUNT];

};
WCEUI_DYNCREATE_END(CAnimDlg, CBaseDlg);
