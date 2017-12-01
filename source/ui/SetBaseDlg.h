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
	virtual void OnReset();	// �ָ��������ú�,UI��ˢ�µĽ���Ҫ���ش˽ӿ�
	virtual void OnBnClick(CWceUiButton* pButton);

	virtual void OnDrawEnd();
	virtual void DoAnimate();
	virtual void OnAnimateBegin();

	// ����ɾ����־,��ܺ��ʵ�ʱ���ɾ��
	void DeleteDlg();

	void AnimGetParameters(OUT float* params, int nStep, int nTotal);

	//  ����ʱ��ʵ��top bar����ؼ���һЩ����Ч��
	void ResetTopLayers();
	CWceUiLayer* m_pLayerTop[TOP_LAYER_COUNT];
	RECT		 m_rcLayerTop[TOP_LAYER_COUNT];
};
WCEUI_DYNCREATE_END(CSetBaseDlg, CAnimDlg);
