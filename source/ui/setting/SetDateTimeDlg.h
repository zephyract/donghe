#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"
#include "WceUiSButton.h"
#include "wceuilistbox.h"

class CSetDateTimeListBox;

class CSetDateTimeDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetDateTimeDlg, CSetBaseDlg)
public:
	CSetDateTimeDlg(void);
	~CSetDateTimeDlg(void);	

	virtual void OnInitDialog();
	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnReset();
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	virtual void OnDlgShow(BOOL bShow);

	void OnSBnClick(CWceUiSButton* pSButton, int status);
protected:
	void InitLayers();
	void InitLayersStatus();
	void OnAutosetChange();	// 时间自动设置变化时，UI需调整下，
	void ShowDateTimeLayers(BOOL bShow);	// 显示或隐藏日期设置控件

	// get or set time from controls.
	void GetTime(LPSYSTEMTIME lpSystemTime);
	void SetTime(LPSYSTEMTIME lpSystemTime);
	// 调整控件的时间，因为有些项变化时，其它项可能也需跟着变化，符合时间规律，
	void AdjustTime();
	// 将当前更改的时间设置成系统时间
	void SaveTime();

	void UpdateTimezone();	// 更新时区信息

	// 用户改变了DST状态（是否使用DST）
	void OnDstChange();

	CSetDateTimeListBox *m_pListYear;
	CSetDateTimeListBox *m_pListMonth;
	CSetDateTimeListBox *m_pListDay;
	CSetDateTimeListBox *m_pListHour;
	CSetDateTimeListBox *m_pListMinute;

};
WCEUI_DYNCREATE_END(CSetDateTimeDlg, CSetBaseDlg);


/////////////////// CSetDateTimeListBox  //////////////////////////

class CSetDateTimeListBox : public CWceUiListBox
{
	WCEUI_DYNCREATE_BEGIN(CSetDateTimeListBox, CWceUiListBox)
public:
	CSetDateTimeListBox();
	~CSetDateTimeListBox(void);

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	virtual void OnScrollWindow();
	virtual void OnLButtonDown(UINT nFlags, POINT point);
	virtual void OnLButtonUp(UINT nFlags, POINT point);

	void SetItemsVisible(int nItemsVisible) { m_nItemsVisible = nItemsVisible; }
protected:
	virtual int GetScrollDistance(DWORD speed, POINT point);
	virtual void OnScrollWindowEnd();

	// 计算一页可以显示的行数,这里一般是5行
	int GetItemsPerpage();

	CWceUiLoadBitmap m_bmpSelected;
	static CSetDateTimeListBox *m_pFocusListBox;

	int m_nItemsVisible;
	int m_nTopIndex;
};
WCEUI_DYNCREATE_END(CSetDateTimeListBox, CWceUiListBox);


