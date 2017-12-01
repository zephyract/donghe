#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"
#include "WceUiSButton.h"

#define EQ_FREQ_COUNT 10
#define EQ_TYPE_COUNT 8

enum EQ_FUN_BUTTON_TYPE
{
	EQ_FUN_AUDIO,
	EQ_FUN_BALANCE,
	EQ_FUN_INVALID = -1
};

class CSetAudioDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetAudioDlg, CSetBaseDlg)
public:
	CSetAudioDlg(void);
	~CSetAudioDlg(void);	

	virtual void OnInitDialog();
	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);
	virtual void OnDlgShow(BOOL bShow);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

	void OnBack(); // �˳�����
protected:
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);
	virtual void OnBnClick(CWceUiButton* pButton);

	BOOL OnBnEQTypeClick(CWceUiButton* pButton);

	void InitLayer();
	// �����л�
	void OnFunChange(EQ_FUN_BUTTON_TYPE type);
	// ѡ��ָ���Ĺ��ܰ�ť
	void CheckFunButton(EQ_FUN_BUTTON_TYPE type);
	// ��ʾָ���Ĺ���ҳ��
	void ShowFuncFrame(EQ_FUN_BUTTON_TYPE type);
	// ѡ��ָ����EQ��ť
	void CheckEQButton(UINT type);
	// �ص�������
	void CheckSubwooferButton(BOOL bCheck);

	EQ_FUN_BUTTON_TYPE m_CurFunType;		// ��ǰѡ�еĹ���

	// eq values
	CWceUiSlider* m_pSliderEQ[EQ_FREQ_COUNT];
	void UpdateEQValues(UINT* pvalue);	// ����slider������, pvalueҪָ��һ������
	void GetEQValues(UINT* pvalue);		// ��slider��ȡ����, pvalueҪָ��һ������
	CWceUiButton* m_pButtonEQ[EQ_TYPE_COUNT];

	// balance
	void UpdateBalance(POINT pt);
	void UpdateBSlider(int offsetx, int offsety);
	void UpdateBValue(POINT pt);

	//
	DLG_ID m_LastDlgID;
	SOURCE_ID m_LastDlgSourceID;
};
WCEUI_DYNCREATE_END(CSetAudioDlg, CSetBaseDlg);


// 
// class CSetBalanceLayer : public CWceUiLayer
// {
// 	WCEUI_DYNCREATE_BEGIN(CSetBalanceLayer, CWceUiLayer)
// public:
// 	CSetBalanceLayer(void);
// 	~CSetBalanceLayer(void);	
// 
// 	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
// 	virtual void OnInitLayer();
// 
// 	virtual void OnLButtonDown(UINT nFlags,  POINT point);
// 	virtual void OnLButtonUp  (UINT nFlags,	 POINT point);
// 	virtual void OnMouseMove  (UINT nFlags,	 POINT point);
// 	virtual void OnLoseCapture();
// 
// protected:
// 	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
// 
// 
// };
// WCEUI_DYNCREATE_END(CSetBalanceLayer, CWceUiLayer);
// 
