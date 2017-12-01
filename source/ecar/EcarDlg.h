#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "AnimDlg.h"
#include "CppSQLite3U.h"
#include "ecarobject_dll.h"

#define TIMER_ID_DELAY_DIAL		1020
#define TIMER_ID_SEND_TNC_CODE	1021
#define TIMER_ID_DELAY_DIAL_LEFT_COUNT	1022	// 拨号倒计时
enum ECAR_STAUS
{
	ES_IDLE,			// 没有执行特别指令
	ES_TO_DIAL,			// 匹配到唯一数据,拨号倒计时状态
};

class CEcarDlg : public CAnimDlg
{
	WCEUI_DYNCREATE_BEGIN(CEcarDlg, CAnimDlg)
public:
	CEcarDlg(void);
	~CEcarDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	
	virtual void OnDlgShow(BOOL bShow);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

	static BOOL IsEcarNumber(const TCHAR *pTelNumber);

protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);

	// button click
	void OnEcar();
	void OnCall();
	void OnCancel();
	void OnBack();
	void OnHome();

	// timer
	void OnTimerLeftCount();

	void OnUserCallNo(char* pTelNumber);	// 代拨电话
	void OnCallByName(LPCTSTR pTelName);	// 根据名字代拨电话
	void OnUpdateMapInfo(const char* pTncCode);	// 设置导航目的地
	void OnCallEvent(DWORD dwEventID);

	BOOL SendTncCodeToNavi();
	// 通过名字或电话号码查询数据库, 
	// bByName :  TRUE ~ by name,  FALSE ~ by number
	void GetContactsByName(LPCTSTR pTelName, LPCTSTR pTelNumber, BOOL bByName, CWceUiListBox* pListbox, CStringArray& PBQueryResult);
	void ShowMessageTip(LPCTSTR tip, BOOL bAutoHide=TRUE);
	void ShowMessage(LPCTSTR message);

	// 触发了翼卡,调用这接口,让BT通话结束后,激活ECar窗口
	void SetLastDlgInfoToThis();

	// 取消到计时拨号
	void CancelDelayDial();
	// Ecar是否处于激活状态
	// 规则：当ECAR引擎下发了有效命令时认为Ecar处于激活状态, 当ECar画面处于当前画面时,也认为是Ecar处理激活状态
	//       只有Ecar Dlg主动退出了， 或者自动拔号出去了,才认为Ecar处于非激活状态
	void SetEcarActive(BOOL bActive);	

	CECarObject* m_pECarObject;
	// 根据通话的电话号码判断是否接通的是ecar的呼叫中心
	BOOL m_bIsEcarCallCenter;
	// 下发的代拨电话
	CString m_strTelNumberToDial;
	// 导航信息
	TCHAR m_szTncCode[256];
	int   m_nSendTncCodeCount;	// 尝试向导航发送TNC的次数,因为导航有可能不成功，达到次数限制就不再发送

	// 下发名字时,匹配到的电话号码记录,名字显示在listbox
	CStringArray m_PBQueryResult;

	// 拨号倒计时
	int m_LeftCount;
	ECAR_STAUS m_eStatus;

	// 记录下启动翼卡服务时的当前界面
	DLG_INFO* m_pdiLastDlg;
};
WCEUI_DYNCREATE_END(CEcarDlg, CAnimDlg);
