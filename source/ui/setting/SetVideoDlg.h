#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"
#include "WceUiSButton.h"
#include "wceuilistbox.h"

class CSetVideoDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetVideoDlg, CSetBaseDlg)
public:
	CSetVideoDlg(void);
	~CSetVideoDlg(void);	

	virtual void OnInitDialog();
	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDlgShow(BOOL bShow);
protected:
	virtual void OnReset();
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);

	void OnSBnClick(CWceUiSButton* pSButton, int status);

	// 根据指定的模式重置video参数值
	// mode : 0 ~ 标准  1 ~ colorful  2 ~ soft
	void OnResetVideo(int mode);
protected:
	void InitLayers();

	// 更新UI上的值
	void UpdateVideoValue();
	void UpdateSlider();

};
WCEUI_DYNCREATE_END(CSetVideoDlg, CSetBaseDlg);

