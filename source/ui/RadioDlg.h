#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "dvpbasedlg.h"

#define TIMER_ID_PS_TIP				1019
#define TIMER_ID_SHOW_TA_SEEK		1020
#define TIMER_ID_SHOW_PTY_SEARCH	1021
#define TIMER_ID_SHOW_AF_SEEK		1022

class CRadioBaseDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CRadioBaseDlg, CDvpBaseDlg)
public:
	CRadioBaseDlg(void);
	virtual ~CRadioBaseDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

	// �����ز���
	void InitRuler(BYTE band, WORD wMinFreq, WORD wMaxFreq, WORD wGranularity);
	void RefreshRuler(WORD wFreq, WORD wMinFreq, WORD wMaxFreq);
	// ���������ʱ,��ʱ����ֻ����UI�ϸ���,ֻ�����̧��ʱ,�Ű����ݷ���MCU
	// bUpdateMCU : TRUE ~ ����ǰ��Ӧ��Ƶ����µ�MCU,���򲻸���,ֻ�ǽ�������ʾ����
	//              FALSE~ ֻ����UI
	void RulerPosChange(CWceUiScrollLayer* pLayer, BOOL bUpdateMCU);

protected:
	void OnBnClick(CWceUiButton* pButton);

	// ����ң���������ּ�
	void OnKeyDigital(UINT nDigitalKey);
	
	// ��Ϣ����
	virtual void OnRadioStatus();
	virtual void OnRadioPresetInfo(BOOL bForceRefresh=FALSE);
	virtual void OnRDSStatusChange();
	virtual void ShowPSPTYInfo();	// ���ݵ�ǰ̨��PS,PTY��Ϣ����ʾPS,PTY��Ϣ

	virtual void InitLayers();

	// TA seek ʱ��ʾta seek����
	void ShowTASeek(BOOL bShow);
	void OnTASeekBegin();
	void OnTASeekEnd();
	BOOL m_bTASeekVisible;

	// AF seek ʱ��ʾPI seek����
	void ShowAFSeek(BOOL bShow);
	void OnAFSeekBegin();
	void OnAFSeekEnd();
	BOOL m_bAFSeekVisible;

	// PTY search ʱ��ʾPTY Search����
	void ShowPTYSearch(BOOL bShow);
	void OnPTYSearchBegin();
	void OnPTYSearchEnd();
	BOOL m_bPTYSearchVisible;

	CWceUiScrollLayer* m_pRuler;
	CWceUiLayer* m_pLayerCurFreq;
	CWceUiLayer* m_pLayerFreqUnit;

	// seek״̬�仯ʱ,��������
	BOOL m_is_TA_seek;
	BOOL m_is_AF_seek;
	BOOL m_is_PTY_search;

	// ruler
	WORD m_ruler_min_freq;
	WORD m_ruler_max_freq;
	BOOL m_ruler_init;
};
WCEUI_DYNCREATE_END(CRadioBaseDlg, CDvpBaseDlg);



class CRadioDlg : public CRadioBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CRadioDlg, CRadioBaseDlg)
public:
	CRadioDlg(void);
	~CRadioDlg(void);	

	virtual void OnInitDialog();
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	virtual void OnTimer(WORD wIDEvent);

	virtual void OnKeyPlayControl(UINT key, UINT param);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnBnLongClick(CWceUiButton* pButton);

	void OnRadioStatus();
	void OnRadioPresetInfo(BOOL bForceRefresh=FALSE);

	// ֻ��bandΪFMʱ,����ʾstereo��ʶ��AMû��stereo״̬
	void ShowStereo(BOOL bShow);

	void ShowBand(int nBand);
	// ���pbutton���ĸ�value��ť������Ƿ�������(1~6), ������Ƿ���0
	int GetValueButtonIndex(CWceUiButton *pbutton);
	void InitLayers();

	void OnCurPresetChange(int nOld, int nCur);

	// PS��ʼ���������
	BOOL m_bPSTipVisible;
	void ShowPSTip(BOOL bShow);
	void OnPSBegin();
	void OnPSEnd();

	// ��������仯RDS UI����
	void OnRadioRegionChange();

	CWceUiButton* m_pLayerValue[6];
};
WCEUI_DYNCREATE_END(CRadioDlg, CRadioBaseDlg);


// ������
class CRadioScrollLayer : public CWceUiScrollLayer
{
	WCEUI_DYNCREATE_BEGIN(CRadioScrollLayer, CWceUiLayer)
public:
	CRadioScrollLayer();
	~CRadioScrollLayer();

	virtual void OnTimer(WORD wIDEvent);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnInitLayer();

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	virtual void OnLButtonDown(UINT nFlags,  POINT point);

protected:
	virtual void OnScrollWindow(int nAmount);
	virtual void OnScrollWindowEnd();
};
WCEUI_DYNCREATE_END(CRadioScrollLayer, CWceUiScrollLayer);


// �̶�, ����ȷ��λ��������̶�
class CRadioScaleLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CRadioScaleLayer, CWceUiLayer)
public:
	CRadioScaleLayer();
	~CRadioScaleLayer();
	static const int MAX_SCALE_COUNT = 16;

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	// ������ЧƵ��ķ�Χ,�÷�ΧӦ�ñȱ�ߵķ�ΧС
	void SetFreqRange(UINT min, UINT max);

	// ���ñ�ߵķ�Χ
	void SetRange(UINT min, UINT max);
	// pscale : ָ��̶�ֵ����
	// count  : �������
	void SetScale(UINT* pscale, UINT count);
protected:
	
	UINT m_minPos;
	UINT m_maxPos;
	UINT m_scale[MAX_SCALE_COUNT];		// �����16���̶�
	UINT m_scale_count;		// ��Ч�̶ȸ���

	UINT m_minFreq;
	UINT m_maxFreq;

};
WCEUI_DYNCREATE_END(CRadioScaleLayer, CWceUiLayer);
