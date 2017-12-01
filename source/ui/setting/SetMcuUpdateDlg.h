#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

#define TIMER_ID_UPDATE_MCU		1008
#define TIMER_ID_UPDATE_MCU_END	1009

class CSetMcuUpdateDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetMcuUpdateDlg, CSetBaseDlg)
public:
	CSetMcuUpdateDlg(void);
	~CSetMcuUpdateDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);
protected:
	void OnBnClick(CWceUiButton* pButton);
	void UpdateProgress();

	BOOL Start();
	BOOL Updating();
	BOOL End();

	void ShowTip(LPCTSTR res_id);
	BOOL IsMCUFileExist();
	LPCTSTR GetMCUFile();

	DWORD GetFileSize(LPCTSTR file_name);

	FILE* m_fp;
	DWORD m_file_size;	// 升级文件大小
	DWORD m_bytes_written;	// 已经写入数据的字节数
	BOOL m_bIsUpdating;
};
WCEUI_DYNCREATE_END(CSetMcuUpdateDlg, CSetBaseDlg);
