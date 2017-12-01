#pragma once
#include "dvpbasedlg.h"
#include "IPodManager.h"


class CUIPodFileListDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CUIPodFileListDlg, CDvpBaseDlg)
public:
	CUIPodFileListDlg(void);
	~CUIPodFileListDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	
	void OnTypeChange(MUSB_IPOD_DB_CATEGORY_TYPE_E type);
	// ���ݵ�ǰ�����ͼ������͸����б�
	void UpdateListbox(MUSB_IPOD_DB_CATEGORY_TYPE_E type, UINT sub_type);

	// ѡ��ָ�������Ͱ�ť,�����������Ť��ѡ��״̬
	void CheckButton(MUSB_IPOD_DB_CATEGORY_TYPE_E type);
	void ShowPlayingDlg();

	CWceUiListBox *m_pListbox;

	// ��ʾ���ڲ��ŵ�����
	void ShowPlayingData();

	MUSB_IPOD_DB_CATEGORY_TYPE_E m_curType;
	UINT	m_curSubType;

};
WCEUI_DYNCREATE_END(CUIPodFileListDlg, CDvpBaseDlg);
