#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "AnimDlg.h"


class CSetBaseDlg : public CAnimDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetBaseDlg, CAnimDlg)
public:
	CSetBaseDlg(void);
	~CSetBaseDlg(void);	

	virtual void OnInitDialog();
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDlgShow(BOOL bShow);
protected:
	virtual void OnReset();	// 恢复出厂设置后,UI需刷新的界面要重载此接口
	virtual void OnBnClick(CWceUiButton* pButton);

	virtual void OnDrawEnd();
	virtual void DoAnimate();
	virtual void OnAnimateBegin();

	// 设置删除标志,框架合适的时候会删除
	void DeleteDlg();

	void AnimGetParameters(OUT float* params, int nStep, int nTotal);

	//  动画时，实现top bar上面控件的一些特殊效果
	void ResetTopLayers();
	CWceUiLayer* m_pLayerTop[TOP_LAYER_COUNT];
	RECT		 m_rcLayerTop[TOP_LAYER_COUNT];
};
WCEUI_DYNCREATE_END(CSetBaseDlg, CAnimDlg);
