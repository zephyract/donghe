#pragma once
#include <vector>
#include "BaseDlg.h"
class CBaseDlg;

typedef void (*ExternalApp_Show_Fun)(BOOL bShow);


class CDlgManager
{
public:
	CDlgManager(void);
	~CDlgManager(void);

public:
	// 显示指定的对话框
	BOOL ShowDlg(DLG_ID did, SOURCE_ID sid);
	
	// 切换到指定的源对应的活动窗口,如果没有活动窗口，显示第一个匹配到的
	// 由于一个源可能对应多个窗口，但只有一个活动窗口，这样可以有记忆功能
	// 下次进入进，还是上次退出时的窗口
	BOOL GotoSource(SOURCE_ID sid);

	// “关闭”源的所有窗口（不一定是销毁，也可能是将其置为初始化状态）
	// 活动窗口置为缺省的(切源后之前的记忆无效了)，如果窗口不销毁，可能需要
	// 重新初始化窗口的数据（窗口需要统一的接口？）
	void CloseSource(SOURCE_ID sid);

	// 返回上一次显示的界面，如果没有记录到上一次的界面(这种情况基本不存在，主界面应该不会调用该接口)，
	// 不做界面切换，还是停留在当前界面
	// 只有某些界面可以从不同地方调用时，才调用该接口返回，不能二个这样的界面互相显示，形成死循环了，
	// 【目前不知有没有这样的情况，后续关注】
	void GotoLastDlg();
	// 返回上一个窗口的ID
	DLG_ID GetLastDlgID();
	DLG_ID GetCurDlgID();
	DLG_INFO* GetLastDlgInfo();
	DLG_INFO* GetCurDlgInfo();
	// 外部调用可以主动更新last dlg的信息, 注意参数必须是从本类获取的有效信息(指向本类的某项数据),不能是个临时指针
	void SetLastDlgInfo(DLG_INFO* pdi);	

	BOOL IsDlgCreated(DLG_ID did, SOURCE_ID sid);
	BOOL IsDlgShow(DLG_ID did, SOURCE_ID sid);
	BOOL IsDlgActive(DLG_ID did, SOURCE_ID sid);
	BOOL IsAnimating();	// 当前窗口是否在动画,动画时不要处理鼠标消息,(按键是否也要过滤下？)

	static CDlgManager* CDlgManager::GetInstance();

	static vector<DLG_INFO> m_dlg_info;


	// 删除一个窗口
	BOOL DeleteDlg(DLG_ID did, SOURCE_ID sid);

	// 删除一个源相关的所有窗口
	BOOL DeleteSrcDlg(SOURCE_ID sid);

	// 删除所有已创建的窗口,APP退出时可调用该接口删除dlg
	BOOL DeleteAllDlg();

	// 创建一个窗口
	CBaseDlg* CreateDlg(DLG_ID did, SOURCE_ID sid);

	// 通过dlg id找到信息表中的项, 找不到返回NULL
	DLG_INFO* GetDlgInfo(DLG_ID did, SOURCE_ID sid);

	// 通过dlg找到信息表中的项, 找不到返回NULL
	DLG_INFO* GetDlgInfo(CBaseDlg* pdlg);


	// 将指定的dlg设为活动状态，此时需把同类别的其它dlg设为非活动状态
	// 如果对话框没有创建，则先创建对话框
	void SetActiveDlg(DLG_ID did, SOURCE_ID sid);

	// 置了标志的dlg框架会在合适时候释放资源
	void SetToDeleteFlag(DLG_ID did, SOURCE_ID sid, BOOL bToDelete);

	// 在dlg info中查找指定源的活动窗口，找不到返回NULL
	// 如果没有指定活动窗口，返回匹配到的第一个项
	DLG_INFO* GetActiveDlgInfo(SOURCE_ID sid);

	// 判断是否为外部APP的窗口
	inline BOOL IsExternalApp(DLG_ID did, SOURCE_ID sid);
	inline BOOL IsExternalApp(const DLG_INFO* pdi);


protected:

	// 当一个dialog销毁后，相关信息必须重置
	void ResetDlgInfo(DLG_ID did, SOURCE_ID sid);


	// 获取二个对话框间切换时的动画效果
	BOOL GetAnimMode(IN DLG_INFO* pdi_show, IN DLG_INFO* pdi_hide, 
		OUT ANIM_MODE* pam_show, OUT ANIM_MODE* pam_hide);

	// 获取二个对话框间切换时的动画效果的辅助参数
	// 目前是返回主界面上图标对应的dlg id(需要时)
	BOOL GetAnimData(IN DLG_INFO* pdi_show, IN DLG_INFO* pdi_hide, 
		OUT DWORD* pdata_show, OUT DWORD* pdata_hide);

	// 获取指定源的最上层窗口ID(ui_level值最小)
	// 当有多个最上层窗口时(如SD卡的音乐视频图片界面),返回第一个
	DLG_ID GetTopDlgInSource(SOURCE_ID sid);

	// 将不再使用的dlg资源释放掉
	// 二个参数分别指向正在显示的与将要显示的,这二个dlg不能删除
	void ClearDlgResource(DLG_INFO* pdlgShow, DLG_INFO* pdlgToShow);

	// 窗口切换时,广播窗口切换消息
	void NotifyDlgChanged(DLG_INFO* pdi_show, DLG_INFO* pdi_hide);

	// BT，倒车，ECar窗口认为是顶层窗口，
	BOOL IsTopMostDlg(DLG_INFO* pdi);
	// 当工作在topmost模式(包括BT通话,倒车,ECar)时，其它窗口如果需要显示，只能在后显示
	BOOL IsInTopMostMode();
	// 后台启动对话框
	BOOL ShowDlgInBackground(DLG_ID did, SOURCE_ID sid);

	DLG_INFO* m_cur_dlginfo;	// 当前窗口信息

	// 上一个显示的窗口信息,有些窗口退出时逻辑为从哪里来，回哪里去
	DLG_INFO* m_last_dlginfo;

};
