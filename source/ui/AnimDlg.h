#pragma once
#include "wceuidlg.h"
#include "BaseDlg.h"

#define TIMER_ID_ANIM_SHOWWINDOW		1

const int FRAME_ANMI_SHOWWINDOW = 8;
const int SPEED_ANIM_SHOWWINDOW = 1;	// 用来设置计时器，越小越快

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
	// 派生类需实现此方法,DVD，MSDK，AVIN对视频的隐藏显示接口不同
	virtual void ShowVideo(BOOL bShow, RECT* prect = NULL);
protected:
	virtual void OnInitDialog();
	virtual void OnDrawBegin();
	virtual void OnDrawEnd();

	void OnActiveateWnd(BOOL bActive);

	// 动画变换函数，派生类可重载此函数完成不同的动画
	// 基类只完成从主界面弹出，或回主界面的动画
	// 从指定的矩形区动态放大到全屏，或从全屏缩小到指定的矩形区
	virtual void DoAnimate();

	// 通知动画显示开始
	virtual void OnAnimateBegin();
	// 通知动画显示结束
	virtual void OnAnimateEnd();
	// 正在处理动画，动画帧数可以访问成员变量
	virtual void OnAnimating();

public:
	virtual void AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData);

	// 当文字需要渐隐渐现效果时，调用该接口获取当前帧数的透明度
	static int CalcTextTransparent(BOOL bShow, int nCurFrame, int nTotalFrame);

	BOOL IsAnimating();
	int GetCurrentFrame();

protected:
	// 返回四个参数，按顺序分别是 zoomX, zoomY, offsetX, offsetY
	void AnimGetParameters(OUT float* params, int nStep, int nTotal);


	BOOL		m_bAnimShow;		// show window or hide window
	ANIM_MODE	m_nAnimMode;	// 动画方式
	DWORD		m_dwAnimData;		// 动画的起始或结束位置

	BOOL		m_bIsAnimating;	// TRUE表示正在动画，FALSE表示已经结束动画
	float		m_transparent;		// 动画时会修改透明度，用来保存之前的值，绘制完成后恢复之前的值
	int			m_nCurrentFrame;	// 从0开始计数

	// 回主界面时的矩形区.
	RECT		m_rcAnimRect;

// ANIM_MODE_UPLEVEL上下级之间的另外一种动画效果
protected:
	void AnimGetParameters_uplevel(OUT float* params, int nStep, int nTotal);

	//  动画时，实现top bar上面控件的一些特殊效果
	void ResetTopLayers();
	CWceUiLayer* m_pLayerTop[TOP_LAYER_COUNT];
	RECT		 m_rcLayerTop[TOP_LAYER_COUNT];

};
WCEUI_DYNCREATE_END(CAnimDlg, CBaseDlg);
