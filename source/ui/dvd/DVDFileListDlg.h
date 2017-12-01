#pragma once
#include "dvpbasedlg.h"
#include "DVDFolderListBox.h"

#define TIMER_ID_SHOW_WAIT_CUSOR	1001
const UINT YC_MESSAGE_LOAD_UI2 = RegisterWindowMessage(L"YC_MESSAGE_LOAD_UI2");

#define  DVD_MODE_COUNT		4
enum
{
	DVP_OP_CMD_NONE = 0x00,		//0
	DVP_OP_CMD_ADO_BACK2FL,
	DVP_OP_CMD_JPG_BACK2FL,
	DVP_OP_CMD_SEEK,
	DVP_OP_CMD_RIP_START,
	DVP_OP_CMD_RIP_EXIT,		//5
	DVP_OP_CMD_FL_FILTER,		//6
	DVP_OP_CMD_FL_CHG_DIR,		//7
	DVP_OP_CMD_FL_CLICK_ITEM,	//8
};


typedef enum
{
	FILE_FILTER_BTN_ALL = 0x00,	//0 all
	FILE_FILTER_BTN_AUDIO,		//1 audio
	FILE_FILTER_BTN_VIDEO,		//2 video
	FILE_FILTER_BTN_PIC,		//3 picture
	FILE_FILTER_BTN_TXT,		//4 text
	FILE_FILTER_BTN_MAX,		//5
}E_FILE_FILTER_BTN;


class CDVDFileListDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CDVDFileListDlg, CDvpBaseDlg)
public:
	static void DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData);
	CDVDFileListDlg(void);
	~CDVDFileListDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	BOOL HandleFLACK(UINT param1, UINT param2);
	BOOL HandleResponse(UINT param1, UINT param2);

	// 选中指定的类型按钮,并清除其它按扭的选中状态
//	void CheckButton(E_FILE_FILTER_BTN type);

	CDVDFolderListBox* m_pListFolder;

	// 根据当前播放类型显示对应的播放界面
	void ShowPlayingDlg();


	void RefreshMode(int nMode);
	void InitListbox();
	void ShowStatus(BOOL bShow);
	void RefreshListBox(int nMode);

	void LoadUI(int type);

	void ParamInit(int type);
	int GetCurrentDVDIndex();
	int GetFilterByType(UINT uFileType);

	void ShowFileTip();
	
	TCHAR m_szPlayFile[MAX_PATH];	// 保存当前播放文件的文件名
	E_FILE_FILTER_BTN m_curType;
	BOOL m_bPlay_continue;

public:
	int m_nRefreshMode;
	// 记忆跟DVD交互时，此时的命令状态
	int m_nCmdStatus;


};
WCEUI_DYNCREATE_END(CDVDFileListDlg, CDvpBaseDlg);
