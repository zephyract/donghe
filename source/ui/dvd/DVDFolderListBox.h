#pragma once
#include "WceUiListBox.h"

#define ID_TYPE_DISK		0
#define ID_TYPE_FOLDER		1
#define ID_TYPE_FILE		2
#define ID_TYPE_PARENT_FOLDER		3	// ..folder
struct FILE_LIST_ITEM_DATA 
{
	TCHAR szFilePath[MAX_PATH];		// full file path name
	int   nItemType;
};


class CDVDFolderListBox : public CWceUiListBox
{
	WCEUI_DYNCREATE_BEGIN(CDVDFolderListBox, CWceUiListBox);
public:
	CDVDFolderListBox();
	~CDVDFolderListBox();

	static int GetFilterByType(int type);

	// lpszFilePath ~ 需要显示的目录路径，控件将显示该目录下所有相关的子目录
	// aFileType    ~ 文件类型，只要包含任一文件类型的文件的目录，都显示
	void Init()
	{
		ResetListContent();
		AddFiles();
	}

	void AddFile(int nDVPIndex);
	void AddFiles();
	BOOL IsRootDir();
	// 选中指定索引的文件，参数为文件的索引，不包括前面的目录
	// 需将文件的索引转换为列表项的索引
	int SetCurSelFile(int nIndex);
	void ResetListContent();

	// 返回列表中当前选中的文件,索引为文件的索引,不包括前面的目录, 从0开始计数
	int GetCurSelFile();
	// 返回列表中所有文件数,不包括目录
	int GetFileCount();
	// 返回列表中目录数
	int GetFolderCount();

protected:


	CWceUiLoadBitmap m_bmpUpLevel;
	CWceUiLoadBitmap m_bmpFolder;
	CWceUiLoadBitmap m_bmpAudio;
	CWceUiLoadBitmap m_bmpVideo;
	CWceUiLoadBitmap m_bmpPicture;


};

WCEUI_DYNCREATE_END(CDVDFolderListBox, CWceUiListBox);
