#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "WceUiSlider.h"
#include "DvpBaseDlg.h"
#include "iPodSDK_def.h"

#define TIMER_ID_SHOW_ARMWORK	1001
#define TIMER_ID_SHOW_PLAYMODE	1002

class CUIPodPlayerDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CUIPodPlayerDlg, CDvpBaseDlg)
public:
	CUIPodPlayerDlg(void);
	~CUIPodPlayerDlg(void);	

	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	
	virtual void OnDlgShow(BOOL bShow);
	virtual void OnKeyPlayControl(UINT key, UINT param);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnBnLongClick(CWceUiButton* pButton);

	void OnPosChange(UINT param1, UINT param2);
	void OnStatusChange(MUSB_IPOD_PLAY_ST_EX_E status);
	void ShowMp3Tag();
	void ShowMp3Image();
	void ShowFileTip();
	void ShowMessageTip(LPCTSTR tip);

	// random : 0 ~ off, 1 ~ on
	// repeat : 0 ~ off, 1 ~ repeat one, 2 ~ repeat all
	// 0xFF, 不更新相应的状态
	void ShowPlayMode(BYTE random, BYTE repeat);
	BYTE m_random;
	BYTE m_repeat;

	// 如果有快进或快退置为TRUE
	BOOL m_FF_REW;
};
WCEUI_DYNCREATE_END(CUIPodPlayerDlg, CDvpBaseDlg);

struct BITMAPINFO_RGB565 
{
	BITMAPINFO_RGB565()
	{
		memset(&bmiHeader, 0, sizeof(BITMAPINFOHEADER));
		memset(arrBitMasks, 0, sizeof(arrBitMasks));

		bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  
		bmiHeader.biWidth = 0;  
		bmiHeader.biHeight = 0;  
		bmiHeader.biPlanes = 1;  
		bmiHeader.biBitCount = 16;  
		bmiHeader.biCompression = BI_BITFIELDS;  
		bmiHeader.biSizeImage = 0;  
		bmiHeader.biXPelsPerMeter = 0;  
		bmiHeader.biYPelsPerMeter = 0;  
		bmiHeader.biClrUsed = 0;  
		bmiHeader.biClrImportant = 0;
		arrBitMasks[0] = 0x0000F800;
		arrBitMasks[1] = 0x000007E0;
		arrBitMasks[2] = 0x0000001F;
	};

	BITMAPINFOHEADER    bmiHeader;
	DWORD			arrBitMasks[3];
};
