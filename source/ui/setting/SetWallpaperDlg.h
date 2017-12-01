#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "SetBaseDlg.h"

#define WALLPAPER_COUNT		9

class CSetWallpaperDlg : public CSetBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CSetWallpaperDlg, CSetBaseDlg)
public:
	CSetWallpaperDlg(void);
	~CSetWallpaperDlg(void);	

	virtual void OnInitDialog();

protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnReset();

	// 选中当前选中的背景
	void CheckButton(int nIndex);
protected:
	CWceUiGenericBitmap* m_pBkTexture[WALLPAPER_COUNT];
};
WCEUI_DYNCREATE_END(CSetWallpaperDlg, CSetBaseDlg);


//******************** CWallPaperButton  ***********************//

class CWallPaperButton : public CWceUiButton
{

	WCEUI_DYNCREATE_BEGIN(CWallPaperButton, CWceUiButton)
public:
	CWallPaperButton(void);
	~CWallPaperButton(void);	

	virtual void OnDrawBackGround(CWceUiGenericBitmap* pDrawBitmap, BOOL bLayerBitmap);

	// 根据背景索引值来设置背景
	void SetBackGround(CWceUiGenericBitmap* pTexture);
protected:
	CWceUiGenericBitmap* m_pBkTexture;
};
WCEUI_DYNCREATE_END(CWallPaperButton, CWceUiButton);
