#pragma once
#include "wceuidlg.h"
#include "uibase.h"
#include "DlgManager.h"
#include "WceUiButton.h"
#include "WceUiListBox.h"
#include "WceUiSlider.h"
#include "WceUiSButton.h"

// �Ի�����࣬���еĶԻ��򶼴Ӹ�����������Ӹ������������
class CBaseDlg : public CWceUiDlg
{
	WCEUI_DYNCREATE_BEGIN(CBaseDlg, CWceUiDlg)
public:
	CBaseDlg(void);
	~CBaseDlg(void);	

	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);
public:


	// nAnimMode: ����ģʽ
	// dwAnimData: ������Ҫ�Ĳ�����Ҳ�����Ǹ�ָ�룬��ͬ������ʽ���ܴ���ͬ���壬��ϸ��
	virtual void AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData);
	virtual BOOL IsAnimating();

	// event
	// ������ʾ������ʱ������øýӿ�
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



// ���д��ڵı����ؼ�����
// ϵͳ�б��������ɶԻ�����ı���ָ��������ͳһ�ɴ˿ؼ���������������֧��ĳЩ������
// ������ڲ���Ҫ��Щ���ԣ�Ҳ�ɲ������˿ؼ�


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


