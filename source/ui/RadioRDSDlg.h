#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "dvpbasedlg.h"
#include "RadioDlg.h"

class CRadioRDSDlg : public CRadioBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CRadioRDSDlg, CRadioBaseDlg)
public:
	CRadioRDSDlg(void);
	~CRadioRDSDlg(void);	

	virtual void OnInitDialog();
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	virtual void OnDlgShow(BOOL bShow);
	virtual void OnKeyPlayControl(UINT key, UINT param);

	// ����ΪRDS������ʾʱ,����TRUE��������û��������ý����,����FALSE
	static BOOL IsTopMostDlg();
	static void SetTopMostDlg(BOOL bIsTopMostDlg);

	// ���ض�Ӧ���ַ�����,RDS��RBDS���ܲ�һ��
	static LPCTSTR GetPTYString(int type);
	// ����PTY�ĸ���
	static int GetPTYCount();

protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnBnLongClick(CWceUiButton* pButton);
	void OnPTYClick();

	void OnRadioStatus();
	void OnRDSAlarm(BOOL bAlarmOn);
	void OnRadioPresetInfo(BOOL bForceRefresh = FALSE);

	// TA alarm ʱ��ʾtraffic����
	void ShowTAAlarm(BOOL bShow);

	// ���ݵ�ǰ̨��PS,PTY��Ϣ����ʾPS,PTY��Ϣ
	void ShowPSPTYInfo();

	// ��ʾ�û�ѡ����PTY����
	void ShowPTYUser(int type);


	// ���pbutton���ĸ�value��ť������Ƿ�������(1~6), ������Ƿ���0
	int GetValueButtonIndex(CWceUiButton *pbutton);
	void InitLayers();


	static BOOL m_bIsTopMostDlg;
};
WCEUI_DYNCREATE_END(CRadioRDSDlg, CRadioBaseDlg);
