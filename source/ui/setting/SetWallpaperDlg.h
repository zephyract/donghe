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

	// ѡ�е�ǰѡ�еı���
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

	// ���ݱ�������ֵ�����ñ���
	void SetBackGround(CWceUiGenericBitmap* pTexture);
protected:
	CWceUiGenericBitmap* m_pBkTexture;
};
WCEUI_DYNCREATE_END(CWallPaperButton, CWceUiButton);
