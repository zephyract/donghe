#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpBaseDlg.h"
#include "bluetooth.h"
#include "CppSQLite3U.h"

const int FIELD_ID			= 0;
const int FIELD_NAME		= 1;
const int FIELD_NUMBER		= 2;
const int FIELD_PYNAME		= 3;
const int FIELD_LONGSTRING	= 4;
const int FIELD_SHORTSTRING = 5;


#define TIMER_ID_BT_CONNECTED	2001
#define TIMER_ID_BT_UNCONNECTED	2002
#define TIMER_ID_BT_UPDATE_DB_STRING	2003

enum BT_FUN_BUTTON_TYPE
{
	BT_FUN_CONNECT,
	BT_FUN_DIAL,
	BT_FUN_RECENTS,
	BT_FUN_PHONEBOOK,
	BT_FUN_MUSIC,
	BT_FUN_INVALID = -1
};

class CBluetoothDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CBluetoothDlg, CDvpBaseDlg)
public:
	CBluetoothDlg(void);
	~CBluetoothDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDlgShow(BOOL bShow);
	virtual void OnKeyPlayControl(UINT key, UINT param);
	static void PlayControl(UINT key, UINT param);

	// 点击拨号键
	void OnKeyCall();
	// 功能切换
	void OnFunChange(BT_FUN_BUTTON_TYPE type);
	BT_FUN_BUTTON_TYPE GetFunType() {return m_CurFunType;}

// 根据连接状态,标题栏信息也变化
	static void GetTitleInfo(CString &strTitle);
protected:
	void InitLayer();
	void OpenDB();
	// helper
	// 拨打指定的记录中的电话
	// nIndex : 记录中的索引
	// SyncPath: 指定数据类别
	void Call(int nIndex, E_BT_PBAP_SYNC_PATH_T SyncPath);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	virtual void OnBnLongClick(CWceUiButton* pButton);


	// 按钮处理分下类, 如果处理了返回TRUE，否则返回FALSE
	BOOL OnBnFunClick(CWceUiButton* pButton);			// 功能切换按钮
	BOOL OnBnPhonebookClick(CWceUiButton* pButton);		// 电话本中的按钮
	BOOL OnBnRecentsClick(CWceUiButton* pButton);		// 通话记录中的按钮
	BOOL OnBnKeypadClick(CWceUiButton* pButton);		// keypad
	BOOL OnBnMusicClick(CWceUiButton* pButton);			// music


	// 选中指定的功能按钮
	void CheckFunButton(BT_FUN_BUTTON_TYPE type);
	// 显示指定的功能页面
	void ShowFuncFrame(BT_FUN_BUTTON_TYPE type);
	// 控制通话记录中按钮与列表的显示
	void ShowRecentsListbox(BOOL bShow);
	BOOL m_bShowRecentsListUI;	// 用来标识通话记录列表数据是否显示了

	// 控制电话本的搜索界面显示与否
	void ShowPBQueryUI(BOOL bShow);
	BOOL m_bShowQueryUI;	// 用来标识搜索界面是否显示了
	CStringArray m_PBQueryResult;		// 保存搜索结果中的电话号码,需要拔号时,通过listbox索引直接可在数组中查到号码
	
	// 将历史连接设备添加到列表
	void InitPairedRecsListbox();
	// 根据选中的项,调整子控件的位置
	void UpdatePairedLBChilds();
	void UpdateLBChilds(LPCTSTR lpszListboxName, LPCTSTR lpszChildName);
	void DeletePairedItem();

	// CALL事件处理
	void OnCallEvent(DWORD dwEventID);
	// CALL事件处理
	void OnMusicEvent(DWORD dwEventID);
	// PGMGR事件处理
	void OnPBMGRSyncEvent(DWORD dwEventID, int nTotalRecords, E_BT_PBAP_SYNC_PATH_T SyncPath);

	void OnConnectEvent(DWORD dwEventID);
	void OnTimerBTConnect();
	void OnTimerBTDisconnect();

	void OnLocalCallInfoChange(int nRecentsType);

	void ShowDeviceInfo();
	void ShowTitleInfo();
	void ShowID3Tag();
	void UpgradeProgress();

	BT_FUN_BUTTON_TYPE m_CurFunType;		// 当前选中的功能

	// 打出拨号键盘中的号码后,记下旧号码,方便下次重拨
	// 即使窗口退出，下次进来时该数据还是有效
	static CString m_strOldDialNumber;
	BOOL m_bIsDialNumber;	// 如果是通过拨号打出的号码,置此标志为TRUE

	// 最近通话记录加到列表
	void AddRecentsCall2Listbox(E_BT_PBAP_SYNC_PATH_T eSyncPath);
	// 通话录音加到列表
	void AddRecentsRecord2Listbox();
	// 根据选中的项,调整子控件的位置
	void UpdateRecentsLBChilds();
	// 删除当前选中的通话记录
	void DeleteRecentsItem();
	// 通话记录与E_BT_PBAP_SYNC_PATH_T中定义保持一致
	// 0xFF用来表示录音
	static const int RECENTS_TYPE_RECORD = 0xFF;
	int m_nRecentsType;

	// 将电话本添加到listbox
	// nFirst ~ 从第几条开始添加,  nCount ~ 总共添加几条
	BOOL AddPB2Listbox(int nFirst, int nCount);
	// 将下载的电话本添加到数据库,数据库还保留了格式化串(用来显示),但格式化的动作异步执行(需要较长时间),
	// 数据添加到数据库后,启动一个计时器分步格式化数据
	BOOL AddPB2DB(int nFirst, int nCount);
	// 格式化数据库的字符串数据用来显示
	// 每次格式化固定数目的数据,以免耗时太长
	// 如果已经格式化了全部数据,返回TRUE,如果返回FALSE,表示后面还有数据未格式化
	BOOL UpdateDBString();
	void OnTimerUpdateDBString();
	// 记录已经格式化了的记录的数量
	int m_nUpdatedRecords;
	// 更新电话本的查询结果到listbox
	void UpdatePBQ2Listbox();
	// 同步电话本数据
	// bForceResync ~ TRUE : 强制重新下载数据， FALSE : 优先使用本地数据,如果没有本地数据则下载
	void SyncPBData(BOOL bForceResync);
	// 下载电话本时,显示正在下载动画
	void ShowWaitCursor(BOOL bShow);

	// 指定当前需同步数据的类型
//	E_BT_PBAP_SYNC_PATH_T m_eSyncPath;

	// 显示上下曲，播放暂停等提示信息
	void ShowTipMessage(LPCTSTR message);

	// 处理遥控器的数字键
	void OnKeyDigital(UINT nDigitalKey);

	CppSQLite3DB m_db;
};
WCEUI_DYNCREATE_END(CBluetoothDlg, CDvpBaseDlg);


#include "WceUiSEditBox.h"

class CTelNumberEditBox : public CWceUiSEditBox
{
	WCEUI_DYNCREATE_BEGIN(CTelNumberEditBox, CWceUiSEditBox)

public:
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

};

WCEUI_DYNCREATE_END(CTelNumberEditBox, CWceUiSEditBox);



class CKeypadLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CKeypadLayer, CWceUiLayer)

public:
	CKeypadLayer();
	~CKeypadLayer();

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	virtual void OnLButtonDown(UINT nFlags, POINT point);
	virtual void OnLButtonUp(UINT nFlags,	 POINT point);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);

	// 设置第一个矩形区, 列间隔， 行间隔， 每行个数
	void SetKeyRect(RECT *pRect, int nColGap, int nRowGap, int nColCount);
protected:
	static const int KEY_COUNT = 26;
	RECT m_rc[KEY_COUNT];
	CWceUiLoadBitmap m_texSelect; 
	BOOL m_bMouseDown;
	int m_nClickItem;
	int HitTest(POINT pt);
};

WCEUI_DYNCREATE_END(CKeypadLayer, CWceUiLayer);
