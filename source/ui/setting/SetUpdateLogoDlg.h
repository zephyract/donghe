#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

#define LOGO_COUNT	9

class CSetUpdateLogoDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetUpdateLogoDlg, CSetBaseDlg)
public:
	CSetUpdateLogoDlg(void);
	~CSetUpdateLogoDlg(void);	

	virtual void OnInitDialog();

protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	BOOL UpdateLogo(int nIndex);
	BOOL WriteLogo(LPVOID pbuffer, DWORD size);
	
	void CheckButton(int nIndex);
	// 0 ~ 无显示， 1 ~ 成功， 2 ~ 失败
	void ShowTip(int nMode);
protected:
	CWceUiGenericBitmap* m_pBkTexture[LOGO_COUNT];
	int m_nSelected;
};
WCEUI_DYNCREATE_END(CSetUpdateLogoDlg, CSetBaseDlg);

