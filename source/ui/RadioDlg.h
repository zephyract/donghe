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

	// 标尺相关操作
	void InitRuler(BYTE band, WORD wMinFreq, WORD wMaxFreq, WORD wGranularity);
	void RefreshRuler(WORD wFreq, WORD wMinFreq, WORD wMaxFreq);
	// 当滚动标尺时,即时数据只是在UI上更新,只有鼠标抬起时,才把数据发给MCU
	// bUpdateMCU : TRUE ~ 将当前对应的频点更新到MCU,否则不更新,只是将数据显示出来
	//              FALSE~ 只更新UI
	void RulerPosChange(CWceUiScrollLayer* pLayer, BOOL bUpdateMCU);

protected:
	void OnBnClick(CWceUiButton* pButton);

	// 处理遥控器的数字键
	void OnKeyDigital(UINT nDigitalKey);
	
	// 消息处理
	virtual void OnRadioStatus();
	virtual void OnRadioPresetInfo(BOOL bForceRefresh=FALSE);
	virtual void OnRDSStatusChange();
	virtual void ShowPSPTYInfo();	// 根据当前台的PS,PTY信息，显示PS,PTY信息

	virtual void InitLayers();

	// TA seek 时显示ta seek字样
	void ShowTASeek(BOOL bShow);
	void OnTASeekBegin();
	void OnTASeekEnd();
	BOOL m_bTASeekVisible;

	// AF seek 时显示PI seek字样
	void ShowAFSeek(BOOL bShow);
	void OnAFSeekBegin();
	void OnAFSeekEnd();
	BOOL m_bAFSeekVisible;

	// PTY search 时显示PTY Search字样
	void ShowPTYSearch(BOOL bShow);
	void OnPTYSearchBegin();
	void OnPTYSearchEnd();
	BOOL m_bPTYSearchVisible;

	CWceUiScrollLayer* m_pRuler;
	CWceUiLayer* m_pLayerCurFreq;
	CWceUiLayer* m_pLayerFreqUnit;

	// seek状态变化时,才做动作
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

	// 只有band为FM时,才显示stereo标识，AM没有stereo状态
	void ShowStereo(BOOL bShow);

	void ShowBand(int nBand);
	// 检测pbutton是哪个value按钮，如果是返回索引(1~6), 如果不是返回0
	int GetValueButtonIndex(CWceUiButton *pbutton);
	void InitLayers();

	void OnCurPresetChange(int nOld, int nCur);

	// PS开始或结束触发
	BOOL m_bPSTipVisible;
	void ShowPSTip(BOOL bShow);
	void OnPSBegin();
	void OnPSEnd();

	// 收音区域变化RDS UI调整
	void OnRadioRegionChange();

	CWceUiButton* m_pLayerValue[6];
};
WCEUI_DYNCREATE_END(CRadioDlg, CRadioBaseDlg);


// 滚动尺
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


// 刻度, 在正确的位置上输出刻度
class CRadioScaleLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CRadioScaleLayer, CWceUiLayer)
public:
	CRadioScaleLayer();
	~CRadioScaleLayer();
	static const int MAX_SCALE_COUNT = 16;

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	// 设置有效频点的范围,该范围应该比标尺的范围小
	void SetFreqRange(UINT min, UINT max);

	// 设置标尺的范围
	void SetRange(UINT min, UINT max);
	// pscale : 指向刻度值数组
	// count  : 数组个数
	void SetScale(UINT* pscale, UINT count);
protected:
	
	UINT m_minPos;
	UINT m_maxPos;
	UINT m_scale[MAX_SCALE_COUNT];		// 最多存放16个刻度
	UINT m_scale_count;		// 有效刻度个数

	UINT m_minFreq;
	UINT m_maxFreq;

};
WCEUI_DYNCREATE_END(CRadioScaleLayer, CWceUiLayer);
