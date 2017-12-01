#pragma once
#include "uibase.h"
#include "WceUiButton.h"

#define VDISK_MAX_DISK_COUNT	12

class CVDiskFileManager
{
public:
	CVDiskFileManager(void);
	virtual ~CVDiskFileManager(void);

// interface
public:
	static CVDiskFileManager* GetInstance();

	// 初始化虚拟多碟目录中的文件数据
	// 根据设置的多碟目录来初始化CMsdkFileManager,此类帮助管理文件数据
	void InitLoadData();

	// 返回虚拟碟片是否有效,索引从0开始
	BOOL IsDiskExist(int nDiskIndex);

	// 返回第一个有效的disk,如果没有有效disk，返回-1
	int FirstDiskAvailable();
	int LastDiskAvailable();

	// 返回第一个无效的disk,如果没有，返回-1
	int FirstDiskUnavailable();

	// 下一个有效disk,如果已到最后一个，返回第一个
	int NextDiskAvailable(int nDiskIndex);
	int PrevDiskAvailable(int nDiskIndex);

	// 根据文件名及碟片ID，查询其在当前数据中的索引位置
	// 如果未找到对应的项，返回FALSE
	BOOL GetFileIndexInfo(LPCTSTR lpszPath, int nDiskIndex, OUT int& nFileIndex);

	// nDiskIndex ~ 在目录数据中的索引
	// nFileIndex ~ 当前选中的文件，即正在播放的文件
	void SetPlayFileData(int nDiskIndex, int nFileIndex);

	// 播放模式
	void SetPlayRepeateMode(WORD wRepeateMode);
	void SetPlayRandomMode(WORD wRandomMode);
	WORD GetPlayRepeateMode();
	WORD GetPlayRandomMode();
	int PlayFileNext();
	int PlayFilePrev();

	// 返回当前选中的文件,即正播放的文件
	// 如果数据没加载或参数错误返回-1
	int GetPlayingFileIndex();

	// 根据播放文件列表索引值返回文件名
	LPCTSTR GetPlayFilePath(int nIndex);

	//
	int GetPlayingDiskID();

	// 获取每张碟的路径
	CString GetVDiskPath(int nIndex);
	// 获取为虚拟碟片指定的路径
	CString GetVDiskPath();

protected:

	// 可能虚拟碟目录还有其它目录,diskindex与folderindex不一定是一一对应关系，这里作个转化
	// 如果转换不成功返回 -1 
	int DiskIndex2FolderIndex(int nDiskIndex);

	// 用一位来表示虚拟碟片是否有效，最低位表示第一张碟
	UINT m_uDiskValid;
	// 当前正在播放的disk,如果没有播放则为-1
	int  m_nPlayingDiskID;
};
