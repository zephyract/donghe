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

	// ��ʾ����UI,
	// lpszType ~ ��ʾ�ĸ�����ֵ
	// nValue ~ ����ֵ
	// dwTime ~ ��ʾ�೤ʱ��(��λms), 0xFFFFFFFF��ʾһֱ��ʾ
	// nStep ~ ���������˴������仯�˶��٣������Ż�UI��ʾ
	void Vol_SetData(LPCTSTR lpszType, int nValue, DWORD dwTime, int nStep);
	void Vol_ShowUI(UI_VOLUME_TYPE eVolumeType);
	void Vol_HideUI();
	// ������ʾ����ʱ,�����������Բ���,��Ҫhook�����Ϣ
	// dwUserData :  Ϊ�ش���thisָ��
	// wParam: Ϊ�����ϢWM_LBUTTONDOWN, WM_MOUSEMOVE, WM_LBUTTONUP
	// lParam: Ϊ����ֵ
	// ����ֵ: TRUE ~ ��ʾҪ�ػ������Ϣ   FALSE ~ ��ʾ���ػ� (���Զ�����ֵ�����ж�,����������ʱ��Ҫ�ػ�)
	static BOOL Vol_MouseLL_Hook_CB(DWORD dwUserData, WPARAM wParam, LPARAM lParam);

	// �յ���Ϣ
	void Climatronic_ShowUI();
	void Climatronic_HideUI();

	// ��ʾ��Ϣ, 
	// bIsResID :  TRUE ~ �ַ���Ϊ��ԴID�� FALSE ~ ֱ����ʾ�ַ�������
	void MessageTip_ShowUI(LPCTSTR lpszMessage, BOOL bIsResID=TRUE, UINT nShowDuration=HIDE_MESSAGE_TIP_UI_TIME);
	void MessageTip_HideUI();

	// ����UI���ݸ���ʱ,��������ӿڸ���overlay
	void UpdateOverlay();
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	void Vol_ShowMuteIcon(BOOL bMute);
	CWceUiGenericBitmap *m_bmp;

	CWceUiSlider* m_pVolSlider;
	RECT m_rcVolume;	// volume����ʾ����
	DWORD m_dwVolTime;	// volume��ʾʱ��
	UI_VOLUME_TYPE m_eVolumeType;

	// �������������ܵ�UI��ʾ״̬
	BOOL m_bIsVolumeUIShow;
	BOOL m_bIsCanUIShow;	// can ��Ϣ
	BOOL m_bIsTipUIShow;	// ��ʾ��Ϣ
};
WCEUI_DYNCREATE_END(COverlayDlg, CDvpVideoBaseDlg);
