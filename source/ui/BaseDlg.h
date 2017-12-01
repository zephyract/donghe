#pragma once
#include "wceuidlg.h"
#include "uibase.h"
#include "DlgManager.h"
#include "WceUiButton.h"
#include "WceUiListBox.h"
#include "WceUiSlider.h"
#include "WceUiSButton.h"

// 对话框基类，所有的对话框都从该类派生，或从该类的子类派生
class CBaseDlg : public CWceUiDlg
{
	WCEUI_DYNCREATE_BEGIN(CBaseDlg, CWceUiDlg)
public:
	CBaseDlg(void);
	~CBaseDlg(void);	

	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);
public:


	// nAnimMode: 动画模式
	// dwAnimData: 动画需要的参数，也可以是个指针，不同动画方式可能代表不同含义，需细化
	virtual void AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData);
	virtual BOOL IsAnimating();

	// event
	// 窗口显示或隐藏时，会调用该接口
	virtual void OnDlgShow(BOOL bShow);
protected:
	// command handler
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnBnLongClick(CWceUiButton* pButton);
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);
	virtual void OnSBnClick(CWceUiSButton* pSButton, int status);


};
WCEUI_DYNCREATE_END(CBaseDlg, CWceUiDlg);



// 所有窗口的背景控件基类
// 系统中背景不是由对话框本身的背景指定，而是统一由此控件来管理，换背景，支持某些动画等
// 如果窗口不需要这些特性，也可不创建此控件


class CBKLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CBKLayer, CWceUiLayer)
public:
	virtual void OnDrawBackGround(CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap);

};
WCEUI_DYNCREATE_END(CBKLayer, CWceUiLayer);




class CSetBKLayer : public CBKLayer
{
	WCEUI_DYNCREATE_BEGIN(CSetBKLayer, CBKLayer)
public:
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
};
WCEUI_DYNCREATE_END(CSetBKLayer, CBKLayer);




class CMainBKLayer : public CBKLayer
{
	WCEUI_DYNCREATE_BEGIN(CMainBKLayer, CBKLayer)
public:
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
};
WCEUI_DYNCREATE_END(CMainBKLayer, CBKLayer);


