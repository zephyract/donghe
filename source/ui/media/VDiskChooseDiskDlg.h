#pragma once
#include "basedlg.h"
#include "WceUiButton.h"
#include "msgboxdlg.h"

#define TIMER_ID_DELAY_ANIMATING	1010
#define TIMER_ID_RIP_START			1011

#define VDISK_RECORD 0
#define VDISK_DELETE 1
class CVDiskChooseDiskDlg : public CPopupDlg
{
	WCEUI_DYNCREATE_BEGIN(CVDiskChooseDiskDlg, CPopupDlg)
public:
	CVDiskChooseDiskDlg(void);
	~CVDiskChooseDiskDlg(void);	

	virtual void OnInitDialog();

	// interface
	// nMode : 0 ~ ¼�ƣ� 1 ~ ɾ��
	// nDiskRip: ����rip�ĵ�Ƭ
	// ����-1��ʾ�û�ȡ������,���򷵻�disk ID
	static int MessageBox(int nMode, int nDiskRip);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	void ActiveDiskButton(int nIndex, BOOL bActive);
	BOOL IsDiskActive(int nIndex);

	void CheckDiskButton(int nDiskID, BOOL bCheck);

	// 0 ~ ¼�ƣ� 1 ~ ɾ��
	int m_nMode;
	int m_nDiskID;	// ��ǰѡ�е�disk
	int m_nDiskRip;
};
WCEUI_DYNCREATE_END(CVDiskChooseDiskDlg, CPopupDlg);
