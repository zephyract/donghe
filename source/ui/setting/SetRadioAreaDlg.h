#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

#define TIMER_ID_SET_MCU_PARAMS	1001

enum FREP_NATION { 
	FQ_EUROPE = 0,
	FQ_USA = 1,
	FQ_OIRT = 2,
	FQ_JAPAN = 3,
	FQ_M_EAST = 4,
	FQ_LATIN = 5,
	FQ_AUSTRALIA = 6,
	FQ_ASIA = 7,
	FQ_SAMER1 = 8,
	FQ_ARABIA = 9,
	FQ_CHINA = 10,
	FQ_SAMER2 = 11,
	FQ_KOREA = 12,
	FQ_RUSSIAN = 13,
	FQ_FM_ONLY = 14,
	FQ_TOTAL,
};

const int RADIO_AREA_COUNT = 8; // 目前UI要处理的地区个数

struct RADIO_AREA
{
	FREP_NATION index;		// 频点索引值
	LPCTSTR resid;			// 资源ID
};

class CSetRadioAreaDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetRadioAreaDlg, CSetBaseDlg)
public:
	CSetRadioAreaDlg(void);
	~CSetRadioAreaDlg(void);	

	virtual void OnInitDialog();
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDlgShow(BOOL bShow);
	virtual void OnTimer(WORD wIDEvent);
protected:
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);

	void InitListbox();
	void SetMCUParam();
	RADIO_AREA m_area[RADIO_AREA_COUNT];
	BOOL m_bNeedSetMCUParam;
};
WCEUI_DYNCREATE_END(CSetRadioAreaDlg, CSetBaseDlg);
