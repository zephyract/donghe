#pragma once
#include "dvpbasedlg.h"
#include "msdkFileManager.h"
#include "WceUiSEditBox.h"
#include "CppSQLite3U.h"

typedef enum
{
	VIDEO_ERROR_NONE,
	VIDEO_ERROR_FILE,
	VIDEO_ERROR_VIDEO,
	VIDEO_ERROR_AUDIO
} VIDEO_ERROR_CODE;

enum
{
	LISTBOX_FILE,
	LISTBOX_FOLDER
};

class CFileListBox;
class CMMFileListDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CMMFileListDlg, CDvpBaseDlg)
public:
	static void MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data);
	CMMFileListDlg(void);
	~CMMFileListDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);
	virtual void OnKeyPlayControl(UINT key, UINT param);
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);

	DLG_ID GetPlayerDlg(MSDK_FILE_TYPE type);

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	void OnGraphBuilt();
	void OnMsdkFinished();
	void OnErrorOccured(UINT param1, UINT param2);
	BOOL PlayNext();
	BOOL PlayPrev();
	// 播放当前文件列表框中指定的文件
	void PlayItem(int nIndex);

	
	void OnTypeChange(MSDK_FILE_TYPE type);

	// 选中指定的类型按钮,并清除其它按扭的选中状态
	void CheckButton(MSDK_FILE_TYPE type);

	CWceUiListBox *m_pListFolder;
	CFileListBox *m_pListFile;

	void SetFolderData(MSDK_FILE_TYPE type);
	// nFilderIndex ~ 需要显示哪个目录的数据
	// type ~ 类型
	// nFileIndex ~ 需选中的当前文件
	void SetFileData(int nFolderIndex, MSDK_FILE_TYPE type, int nFileIndex);
	void AddString2Listbox(const CStringArray& strArray, CWceUiListBox* plistbox, CWceUiGenericBitmap* pTexture);
	
	// nMode : 0 ~ 显示file listbox,  1 ~ 显示folder listbox
	void ShowListBox(int nMode);
	void ShowFileTip();
	void ShowTitle();

	// 根据当前播放类型显示对应的播放界面
	void ShowPlayingDlg();

	// 显示正在播放的数据
	void ShowPlayingData();

	MSDK_FILE_TYPE m_curType;	// 记录当前正在浏览的文件类型
	// 因为当前在浏览图片时,音乐视频可能在后台播放,这里需专门记录正在播放的类型,以便处理下一曲播放
	MSDK_FILE_TYPE m_curPlayingType;

	CWceUiLoadBitmap m_texMusic;
	CWceUiLoadBitmap m_texVideo;
	CWceUiLoadBitmap m_texPhoto;
	CWceUiLoadBitmap m_texFolder;
	CWceUiLoadBitmap m_texPlaying;	// 列表中用来标识正在播放的歌曲

	// key pad
	void ShowKeypad(BOOL bShow);
	void OnKeyEnter();
	BOOL OnBnKeypadClick(CWceUiButton* pButton);
	CWceUiLayer* m_pLayerFrameKeypad;

	//
	void CheckMediaInfo();
	VIDEO_ERROR_CODE m_nErrorCode;

	// 查询功能
	BOOL AddString2DB(const CStringArray& strArray);
	void UpdateQuery2Listbox();
	void OpenDB();
	void ShowQueryUI(BOOL bShow);
	BOOL m_bShowQueryUI;
	CppSQLite3DB m_db;
	CArray<INT> m_PBQueryResult;

	// 播放新文件时，提前刷新播放窗口的相关信息
	void UpdatePlayDlgMediaInfo(MSDK_FILE_TYPE type);

	void InitCodepage();

};
WCEUI_DYNCREATE_END(CMMFileListDlg, CDvpBaseDlg);



// 指定一条记录,显示正在播放的图标
class CFileListBox : public CWceUiListBox
{
	WCEUI_DYNCREATE_BEGIN(CFileListBox, CWceUiListBox)
public:
	CFileListBox();
	~CFileListBox();

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	// 指定当前播放的记录,如果为-1则没有播放记录
	void SetPlayIndex(int nIndex);
	// 只需指定X偏移，Y方向会居中
	void SetPlayImage(int nOffsetX, CWceUiGenericBitmap* pTexture);
protected:

	int m_nIndexPlay;
	int m_nOffsetX;
	CWceUiGenericBitmap* m_pTexturePlay;

};
WCEUI_DYNCREATE_END(CFileListBox, CWceUiListBox);



class CGotoEditBox : public CWceUiSEditBox
{
	WCEUI_DYNCREATE_BEGIN(CGotoEditBox, CWceUiSEditBox)

public:
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	void SetTotal(int nTotal);
protected:
	// 显示时,将数据格式化下,  1/4, 同时检测数据是否合法,如果不合法将其合法化,数据不能大于m_nTotal
	void FormatData(OUT LPTSTR buffer);
	int m_nTotal;
};

WCEUI_DYNCREATE_END(CGotoEditBox, CWceUiSEditBox);


