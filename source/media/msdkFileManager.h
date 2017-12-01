#pragma once
#include "uibase.h"

#define MAX_FOLDER_COUNT 1000
#define MAX_FILE_COUNT	2000


#define MSDK_RANDOM_ON		1
#define MSDK_RANDOM_OFF		0


#define MSDK_REPEATE_OFF	0
#define MSDK_REPEATE_ONE	1
#define MSDK_REPEATE_FOLDER	2
#define MSDK_REPEATE_ALL	3

struct MFM_FILE_INFO
{
	int folder_index[MSDK_FILE_TYPE_COUNT];	// 选中的目录(为在目录数据中的索引)
	CStringArray file_list[MSDK_FILE_TYPE_COUNT];	// 保存选中目录的文件
	BOOL is_loaded[MSDK_FILE_TYPE_COUNT];	// 记录选中目录下的文件是否已经初始化,即是否已经遍历出来了
	int cur_file_index[MSDK_FILE_TYPE_COUNT];		// 选中的文件(为在文件数据中的索引)
};

class CMsdkFileManager
{
public:
	CMsdkFileManager(void);
	virtual ~CMsdkFileManager(void);

// interface
public:
	static CMsdkFileManager* GetInstance();

	// 调用该接口初始化数据，只需调用一次，如果盘符变了，才需再次调用
	// 如果用相同的参数调用，会导致重新加载数据。
	// lpszPath ~ 需加载数据的路径，这里指盘符
	// type ~ 优先加载的文件类型，其它的文件类型需要数据时，才会加载
	void InitLoadData(LPCTSTR lpszPath, MSDK_FILE_TYPE type);

	// 获取指定文件类型的目录数据
	const CStringArray& GetFolderData(MSDK_FILE_TYPE type);

	//
	//***********    浏览文件接口   ****************/
	//
	// 获取文件列表, 只要调用了该接口，就认为指定的目录为当前目录(指定的类型)
	// 当前目录随着播放的进行，有可能切换为其它目录
	const CStringArray& GetBrowseFileData(int nFolderIndex, MSDK_FILE_TYPE type);

	// 要求调用这接口时，保证该类型的数据先加载了
	void SetBrowseFileIndex(int nFileIndex, MSDK_FILE_TYPE type);

	// 返回当前目录
	// 如果数据没加载或参数错误返回-1
	int GetBrowseFolderIndex(MSDK_FILE_TYPE type);

	// 返回当前选中的文件
	// 如果数据没加载或参数错误返回-1
	int GetBrowseFileIndex(MSDK_FILE_TYPE type);

	// 返回当前正在浏览的目录中文件个数
	// 如果数据没加载或参数错误返回-1
	int GetBrowseFilesCount(MSDK_FILE_TYPE type);

	//
	//***********    播放文件接口   ****************/
	//
	// nFolderIndex ~ 在目录数据中的索引
	// nFileIndex ~ 当前选中的文件，即正在播放的文件
	// type ~ 类型 (图片可以与音乐同时播放)
	void SetPlayFileData(int nFolderIndex, int nFileIndex, MSDK_FILE_TYPE type);

	// 返回当前目录
	// 如果数据没加载或参数错误返回-1
	int GetPlayFolderIndex(MSDK_FILE_TYPE type);

	// 返回当前选中的文件,即正播放的文件
	// 如果数据没加载或参数错误返回-1
	int GetPlayingFileIndex(MSDK_FILE_TYPE type);

	// 返回当前正在播放的目录中文件个数
	// 如果数据没加载或参数错误返回-1
	int GetPlayingFilesCount(MSDK_FILE_TYPE type);

	// 根据播放文件列表索引值返回文件名
	LPCTSTR GetPlayFilePath(int nIndex, MSDK_FILE_TYPE type);

	// 播放模式
	void SetPlayRepeateMode(WORD wRepeateMode, MSDK_FILE_TYPE type);
	void SetPlayRandomMode(WORD wRandomMode, MSDK_FILE_TYPE type);
	WORD GetPlayRepeateMode(MSDK_FILE_TYPE type);
	WORD GetPlayRandomMode(MSDK_FILE_TYPE type);

	// 上下曲，返回要播放的文件索引值, 如果失败返回-1
	// 调用该接口会更新播放文件列表数据，可以直接调用相关接口获取最新的数据，
	// 包括目录信息(该接口可能会导致目录切换)
	int PlayFileNext(MSDK_FILE_TYPE type);
	int PlayFilePrev(MSDK_FILE_TYPE type);

	// 播放当前目录中指定的文件
	// 如果指定的参数非法,将不会更新播放数据，并返回FALSE
	BOOL PlayFileGoto(int nIndex, MSDK_FILE_TYPE type);

	//
	//***********    辅助函数   ****************/
	//
	// 根据文件名及类型，查询其在当前数据中的索引位置，包括目录及文件索引
	// 如果未找到对应的项，返回FALSE
	BOOL GetFileIndexInfo(LPCTSTR lpszPath, MSDK_FILE_TYPE type, OUT int& nFolderIndex, OUT int& nFileIndex);
	// 从加载数据的盘符判断是哪个源
	SOURCE_ID GetCurrentSrcID();

protected:
	// 将指定路径中符合条件的目录查找出来
	void LoadFolderData(LPCTSTR lpszPath, MSDK_FILE_TYPE type);

	// 将指定路径中符合条件的文件查找出来, 保存在当前文件数据
	void LoadBrowseFileData(LPCTSTR lpszPath, MSDK_FILE_TYPE type);

	// 初始化文件类型的扩展名
	void InitFileType();
	CStringArray m_aFileType[MSDK_FILE_TYPE_COUNT];	// 文件类型的扩展名

	// 清除所有数据
	void ClearData();

	// 目录数据
	CStringArray m_aFolder[MSDK_FILE_TYPE_COUNT];	// 保存符合条件的目录
	BOOL m_aIsFolderLoaded[MSDK_FILE_TYPE_COUNT];	// 记录该文件类型的目录是否已经初始化

	// 当前浏览的文件数据
	MFM_FILE_INFO m_BrowseFileInfo;

	// 当前播放文件数据
	MFM_FILE_INFO m_PlayFileInfo;
	// high word表示random状态 0 ~ 随机关， 1 ~ 随机开
	// low word表示重复模式    0 ~ repeat all  1 ~ repeat folder  2 ~ repeat one
	UINT		  m_PlayMode[MSDK_FILE_TYPE_COUNT];


	// 当前的数据状态
	TCHAR m_szFilepath2Load[MAX_PATH+1];			// 当前需要加载的路径，这里应是指盘符

	// 用来统计目录个数，不能超过最大数
	int m_nFolderCount;

	// helper functions
	void AddFolders(LPCTSTR lpszFilePath, MSDK_FILE_TYPE eFileType, OUT CStringArray& strArray);
	BOOL IsFindFolder(LPCTSTR lpszFilePath, MSDK_FILE_TYPE eFileType);
	BOOL IsFindFile(LPCTSTR lpszFileName, MSDK_FILE_TYPE eFileType);
	void AddFiles(LPCTSTR lpszFilePath, MSDK_FILE_TYPE type, OUT CStringArray& strArray);

	// 跟据当前的循环模式得到下一个目录(当目录需要跳转时)
	int GetPlayFolderNext(MSDK_FILE_TYPE type);
	// 跟据当前的循环模式得到上一个目录(当目录需要跳转时)
	int GetPlayFolderPrev(MSDK_FILE_TYPE type);
};
