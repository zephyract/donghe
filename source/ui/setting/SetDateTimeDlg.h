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
	void OnAutosetChange();	// ʱ���Զ����ñ仯ʱ��UI������£�
	void ShowDateTimeLayers(BOOL bShow);	// ��ʾ�������������ÿؼ�

	// get or set time from controls.
	void GetTime(LPSYSTEMTIME lpSystemTime);
	void SetTime(LPSYSTEMTIME lpSystemTime);
	// �����ؼ���ʱ�䣬��Ϊ��Щ��仯ʱ�����������Ҳ����ű仯������ʱ����ɣ�
	void AdjustTime();
	// ����ǰ���ĵ�ʱ�����ó�ϵͳʱ��
	void SaveTime();

	void UpdateTimezone();	// ����ʱ����Ϣ

	// �û��ı���DST״̬���Ƿ�ʹ��DST��
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

	// ����һҳ������ʾ������,����һ����5��
	int GetItemsPerpage();

	CWceUiLoadBitmap m_bmpSelected;
	static CSetDateTimeListBox *m_pFocusListBox;

	int m_nItemsVisible;
	int m_nTopIndex;
};
WCEUI_DYNCREATE_END(CSetDateTimeListBox, CWceUiListBox);


