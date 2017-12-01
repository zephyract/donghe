#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpVideoBaseDlg.h"
#include "VolumeManager.h"

#define TIMER_ID_HIDE_VOLUME_UI			1001
#define TIMER_ID_HIDE_CLIMATRONIC_UI	1002
#define TIMER_ID_HIDE_MESSAGE_TIP_UI	1003

const UINT HIDE_CLIMATRONIC_UI_TIME = 5000;
const UINT HIDE_MESSAGE_TIP_UI_TIME = 3000;

class COverlayDlg : public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(COverlayDlg, CDvpVideoBaseDlg)
public:
	COverlayDlg(void);
	~COverlayDlg(void);	
	static COverlayDlg* GetInstance();

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	
	virtual void OnDlgShow(BOOL bShow);
	virtual void OnSliderChange(CWceUiSlider* pslider, UINT status, UINT pos);

	// 显示音量UI,
	// lpszType ~ 显示哪个音量值
	// nValue ~ 音量值
	// dwTime ~ 显示多长时间(单位ms), 0xFFFFFFFF表示一直显示
	// nStep ~ 用来描述此次音量变化了多少，用来优化UI显示
	void Vol_SetData(LPCTSTR lpszType, int nValue, DWORD dwTime, int nStep);
	void Vol_ShowUI(UI_VOLUME_TYPE eVolumeType);
	void Vol_HideUI();
	// 音量显示出来时,让音量条可以操作,需要hook鼠标消息
	// dwUserData :  为回传的this指针
	// wParam: 为鼠标消息WM_LBUTTONDOWN, WM_MOUSEMOVE, WM_LBUTTONUP
	// lParam: 为坐标值
	// 返回值: TRUE ~ 表示要截获鼠标消息   FALSE ~ 表示不截获 (可以对坐标值进行判断,不符合条件时不要截获)
	static BOOL Vol_MouseLL_Hook_CB(DWORD dwUserData, WPARAM wParam, LPARAM lParam);

	// 空调信息
	void Climatronic_ShowUI();
	void Climatronic_HideUI();

	// 提示信息, 
	// bIsResID :  TRUE ~ 字符串为资源ID， FALSE ~ 直接显示字符串内容
	void MessageTip_ShowUI(LPCTSTR lpszMessage, BOOL bIsResID=TRUE, UINT nShowDuration=HIDE_MESSAGE_TIP_UI_TIME);
	void MessageTip_HideUI();

	// 当有UI数据更新时,调用这个接口更新overlay
	void UpdateOverlay();
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	void Vol_ShowMuteIcon(BOOL bMute);
	CWceUiGenericBitmap *m_bmp;

	CWceUiSlider* m_pVolSlider;
	RECT m_rcVolume;	// volume的显示区域
	DWORD m_dwVolTime;	// volume显示时间
	UI_VOLUME_TYPE m_eVolumeType;

	// 用来描述各功能的UI显示状态
	BOOL m_bIsVolumeUIShow;
	BOOL m_bIsCanUIShow;	// can 信息
	BOOL m_bIsTipUIShow;	// 提示信息
};
WCEUI_DYNCREATE_END(COverlayDlg, CDvpVideoBaseDlg);
