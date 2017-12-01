#include "StdAfx.h"
#include "msdkFileManager.h"
#include "interface.h"
#include "filefind.h"
#include "tzUtil.h"
#include "config.h"
#include <time.h>

static int _myrand()
{
	srand( GetTickCount() );
	return rand();
}

CMsdkFileManager::CMsdkFileManager(void)
{
	InitFileType();
	ClearData();
}

CMsdkFileManager::~CMsdkFileManager(void)
{


}

CMsdkFileManager* CMsdkFileManager::GetInstance()
{
	static CMsdkFileManager file_manager;
	return &file_manager;
}

void CMsdkFileManager::ClearData()
{
	for (int i = 0; i < MSDK_FILE_TYPE_COUNT; i++)
	{
		m_aFolder[i].RemoveAll();
		m_aIsFolderLoaded[i] = FALSE;

		m_BrowseFileInfo.folder_index[i] = 0;
		m_BrowseFileInfo.is_loaded[i] = FALSE;
		m_BrowseFileInfo.cur_file_index[i] = 0;
		m_BrowseFileInfo.file_list->RemoveAll();

		m_PlayFileInfo.folder_index[i] = 0;
		m_PlayFileInfo.is_loaded[i] = FALSE;
		m_PlayFileInfo.cur_file_index[i] = 0;
		m_PlayFileInfo.file_list->RemoveAll();

	}

	m_szFilepath2Load[0] = L'\0';
	m_nFolderCount = 0;

}

void CMsdkFileManager::InitLoadData(LPCTSTR lpszPath, MSDK_FILE_TYPE type)
{
	// 清楚之前的数据
	ClearData();
	_tcsncpy(m_szFilepath2Load, lpszPath, MAX_PATH);
	m_szFilepath2Load[MAX_PATH] = L'\0';

	// 加载数据
	LoadFolderData(lpszPath, type);
}

const CStringArray& CMsdkFileManager::GetFolderData(MSDK_FILE_TYPE type)
{
	if (type<0 || type>=MSDK_FILE_TYPE_COUNT)
	{
		static CStringArray aNULL;
		ASSERT(FALSE);
		return aNULL;
	}

	if (!m_aIsFolderLoaded[type])
	{
		LoadFolderData(m_szFilepath2Load, type);
	}

	return m_aFolder[type];
}

void CMsdkFileManager::LoadFolderData(LPCTSTR lpszPath, MSDK_FILE_TYPE type)
{
	if (type<0 || type>=MSDK_FILE_TYPE_COUNT)
	{
		ASSERT(FALSE);
		return;		// invalid parameters
	}

	m_aIsFolderLoaded[type] = TRUE;
	AddFolders(lpszPath, type, m_aFolder[type]);

}

void CMsdkFileManager::AddFolders(LPCTSTR lpszFilePath, MSDK_FILE_TYPE eFileType, OUT CStringArray& strArray)
{
	// 如果是符合条件的目录，加到listbox项数组中
	if (IsFindFolder(lpszFilePath, eFileType))
	{
		strArray.Add(lpszFilePath);
	}

	// 找出目录下符合条件的子目录
	CFileFind finder;
	// build a string with wildcards
	CString strWildcard(lpszFilePath);
	strWildcard += L"\\*.*";

	// start working for directory files
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile(); 
		if (finder.IsDirectory() && !finder.IsDots())
		{
			AddFolders(finder.GetFilePath(), eFileType, strArray);		// call it recursively

			if (m_nFolderCount++ > MAX_FOLDER_COUNT)
			{
				break;
			}
		}
	}

	finder.Close();
}

BOOL CMsdkFileManager::IsFindFolder(LPCTSTR lpszFilePath, MSDK_FILE_TYPE eFileType)
{
	BOOL bRet = FALSE;
	CFileFind finder;
	// build a string with wildcards
	CString strWildcard(lpszFilePath);
	strWildcard += L"\\*.*";

	// search files
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory())		// it's file
		{
			if (IsFindFile(finder.GetFileName(), eFileType))
			{
				bRet = TRUE;
				break;
			}
		}
	}
	finder.Close();

	return bRet;
}

BOOL CMsdkFileManager::IsFindFile(LPCTSTR lpszFileName, MSDK_FILE_TYPE eFileType)
{
	CString str(lpszFileName);
	int pos = str.ReverseFind(L'.');
	if (pos != -1)
	{
		str = str.Right(str.GetLength()-pos-1);
		str.MakeLower();
		for (int i=0; i<m_aFileType[eFileType].GetCount(); i++)
		{
			if (str == m_aFileType[eFileType][i])	// found, assume file type always be lowercase
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CMsdkFileManager::InitFileType()
{
	// music
	m_aFileType[MSDK_FILE_MUSIC].Add(L"mp3");
	m_aFileType[MSDK_FILE_MUSIC].Add(L"wma");
	m_aFileType[MSDK_FILE_MUSIC].Add(L"ac3");	
	m_aFileType[MSDK_FILE_MUSIC].Add(L"ra");
	m_aFileType[MSDK_FILE_MUSIC].Add(L"ram");
	m_aFileType[MSDK_FILE_MUSIC].Add(L"rmm");
	m_aFileType[MSDK_FILE_MUSIC].Add(L"pcm");
	m_aFileType[MSDK_FILE_MUSIC].Add(L"wav");
	m_aFileType[MSDK_FILE_MUSIC].Add(L"mp1");
	m_aFileType[MSDK_FILE_MUSIC].Add(L"mp2");	
	m_aFileType[MSDK_FILE_MUSIC].Add(L"ape");
	m_aFileType[MSDK_FILE_MUSIC].Add(L"m4a");
	m_aFileType[MSDK_FILE_MUSIC].Add(L"ogg");
	m_aFileType[MSDK_FILE_MUSIC].Add(L"aac");
	m_aFileType[MSDK_FILE_MUSIC].Add(L"flac");


	// video
	m_aFileType[MSDK_FILE_VIDEO].Add(L"asf");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"3gp");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"avi");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"vid");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"divx");	
	m_aFileType[MSDK_FILE_VIDEO].Add(L"mp4");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"mpg");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"mpeg");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"dat");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"vob");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"m2v");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"3g2");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"m4v");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"ogm");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"rm");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"rmv");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"rmvb");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"flv");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"mkv");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"mov");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"wmv");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"ts");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"m2ts");
	m_aFileType[MSDK_FILE_VIDEO].Add(L"mfs");

	// photo
	m_aFileType[MSDK_FILE_PHOTO].Add(L"jpg");
	m_aFileType[MSDK_FILE_PHOTO].Add(L"jpe");
	m_aFileType[MSDK_FILE_PHOTO].Add(L"jpeg");
	m_aFileType[MSDK_FILE_PHOTO].Add(L"bmp");
	m_aFileType[MSDK_FILE_PHOTO].Add(L"gif");
	m_aFileType[MSDK_FILE_PHOTO].Add(L"png");

	// all files
	m_aFileType[MSDK_FILE_ALL].Append(m_aFileType[MSDK_FILE_MUSIC]);
	m_aFileType[MSDK_FILE_ALL].Append(m_aFileType[MSDK_FILE_VIDEO]);
	m_aFileType[MSDK_FILE_ALL].Append(m_aFileType[MSDK_FILE_PHOTO]);
}

const CStringArray& CMsdkFileManager::GetBrowseFileData(int nFolderIndex, MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT
		&& nFolderIndex>=0 && nFolderIndex<m_aFolder[type].GetCount())
	{
		// 如果播放数据列表中正好是要请求的数据，直接COPY过来，不用重新遍历, 提高性能
		if (m_PlayFileInfo.folder_index[type] == nFolderIndex && m_PlayFileInfo.is_loaded[type])
		{
			m_BrowseFileInfo.file_list[type].Copy(m_PlayFileInfo.file_list[type]);
		}
		// 没有加载，或者是新的目录, 更新文件数据
		else if (m_BrowseFileInfo.folder_index[type] != nFolderIndex || !m_BrowseFileInfo.is_loaded[type])
		{
			LoadBrowseFileData(m_aFolder[type].GetAt(nFolderIndex), type);
		}
		else	// 如果已经加载了的数据，直接返回数据
		{
			;
		}
		m_BrowseFileInfo.folder_index[type] = nFolderIndex;
		m_BrowseFileInfo.is_loaded[type] = TRUE;

		return m_BrowseFileInfo.file_list[type];
	}
	else	// 有可能无任何数据,返回空数组
	{
		static CStringArray aNULL;
		return aNULL; 
	}
}

void CMsdkFileManager::LoadBrowseFileData(LPCTSTR lpszPath, MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT)
	{
		m_BrowseFileInfo.file_list[type].RemoveAll();
		AddFiles(lpszPath, type, m_BrowseFileInfo.file_list[type]);
	}
}

int CMsdkFileManager::GetBrowseFolderIndex(MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT && m_BrowseFileInfo.is_loaded[type])
	{
		return m_BrowseFileInfo.folder_index[type];
	}

	return 0;
}

void CMsdkFileManager::SetBrowseFileIndex(int nFileIndex, MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT)
	{
		// 指定的索引必须在文件列表范围内
		// 这就要求调用这接口时，保证数据先加载了
		if (nFileIndex>=0 && nFileIndex<m_BrowseFileInfo.file_list[type].GetCount())
		{
		}
		m_BrowseFileInfo.cur_file_index[type] = nFileIndex;
	}
}

int CMsdkFileManager::GetBrowseFileIndex(MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT && m_BrowseFileInfo.is_loaded[type])
	{
		return m_BrowseFileInfo.cur_file_index[type];
	}

	return 0;
}


int CMsdkFileManager::GetBrowseFilesCount(MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT && m_BrowseFileInfo.is_loaded[type])
	{
		return m_BrowseFileInfo.file_list[type].GetCount();
	}

	return -1;
}

void CMsdkFileManager::AddFiles(LPCTSTR lpszFilePath, MSDK_FILE_TYPE type, OUT CStringArray& strArray)
{
	int count = 0;
	CFileFind finder;
	// build a string with wildcards
	CString strWildcard(lpszFilePath);
	strWildcard += L"\\*.*";

	// start working for directory files
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		count++;
		if (count > MAX_FILE_COUNT)
		{
			break;
		}


		bWorking = finder.FindNextFile();
		if (finder.IsDirectory())
			continue;

		CString str = finder.GetFilePath();

		if (!IsFindFile(str, type))
			continue;

		strArray.Add(str);
	}
	finder.Close();
}


void CMsdkFileManager::SetPlayFileData(int nFolderIndex, int nFileIndex, MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT)
	{
		// 如果传进来的参数有误，需校正，确保数据合法
		// 索引有误，则直接选中第一项
		if (nFolderIndex<0 || nFolderIndex>=m_aFolder[type].GetCount())
		{
			nFolderIndex = 0;
		}

		// 如果数据已经加载了，不用重新加载数据
		if (m_PlayFileInfo.folder_index[type] == nFolderIndex && m_PlayFileInfo.is_loaded[type])
		{
			; // nothing to do
		}
		// 如果浏览文件信息中刚好是要请求的数据，直接将浏览的数据COPY过来，不用重新遍历
		else if (m_BrowseFileInfo.folder_index[type] == nFolderIndex && m_BrowseFileInfo.is_loaded[type])	
		{
			m_PlayFileInfo.file_list[type].Copy(m_BrowseFileInfo.file_list[type]);
		}
		else	// 遍历数据
		{
			m_PlayFileInfo.file_list[type].RemoveAll();
			if (m_aFolder[type].GetCount() > 0)	// 有可能没有任何数据,防止访问非法数据
			{
				AddFiles(m_aFolder[type][nFolderIndex], type, m_PlayFileInfo.file_list[type]);
			}
		}

		// 如果传进来的参数有误，需校正，确保数据合法
		// 索引有误，则直接选中第一项
		if (nFileIndex<0 || nFileIndex>=m_PlayFileInfo.file_list[type].GetCount())
		{
			nFileIndex = 0;
		}

		m_PlayFileInfo.cur_file_index[type] = nFileIndex;
		m_PlayFileInfo.folder_index[type] = nFolderIndex;
		m_PlayFileInfo.is_loaded[type] = TRUE;
	}

// 	for (int i=0; i<m_PlayFileInfo.file_list[type].GetCount(); i++)
// 	{
// 		RETAILMSG(1, (L"%s \n", m_PlayFileInfo.file_list[type].GetAt(i)));
// 	}
	
}

int CMsdkFileManager::GetPlayFolderIndex(MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT && m_PlayFileInfo.is_loaded[type])
	{
		return m_PlayFileInfo.folder_index[type];
	}

	return -1;
}

int CMsdkFileManager::GetPlayingFileIndex(MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT && m_PlayFileInfo.is_loaded[type])
	{
		return m_PlayFileInfo.cur_file_index[type];
	}

	return -1;
}

int CMsdkFileManager::GetPlayingFilesCount(MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT && m_PlayFileInfo.is_loaded[type])
	{
		return m_PlayFileInfo.file_list[type].GetCount();
	}

	return -1;
}

LPCTSTR CMsdkFileManager::GetPlayFilePath(int nIndex, MSDK_FILE_TYPE type)
{
	static TCHAR file_path[MAX_PATH];
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT && m_PlayFileInfo.is_loaded[type])
	{
		if (nIndex>=0 && nIndex<m_PlayFileInfo.file_list[type].GetCount())
		{
			return m_PlayFileInfo.file_list[type].GetAt(nIndex);
		}
	}

	return NULL;
}


void CMsdkFileManager::SetPlayRepeateMode(WORD wRepeateMode, MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT)
	{
		if (wRepeateMode == MSDK_REPEATE_ONE
			|| wRepeateMode == MSDK_REPEATE_ALL
			|| wRepeateMode == MSDK_REPEATE_FOLDER
			|| wRepeateMode == MSDK_REPEATE_OFF)
		{
			m_PlayMode[type] &= 0xFFFF0000;
			m_PlayMode[type] |= wRepeateMode;

			// 如果repeate模式处于MSDK_REPEATE_ONE, 需要关闭random
			if (wRepeateMode == MSDK_REPEATE_ONE
				&& GetPlayRandomMode(type) == MSDK_RANDOM_ON)
			{
				SetPlayRandomMode(MSDK_RANDOM_OFF, type);
			}

		}
	}
}

void CMsdkFileManager::SetPlayRandomMode(WORD wRandomMode, MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT)
	{
		if (wRandomMode == MSDK_RANDOM_OFF
			|| wRandomMode == MSDK_RANDOM_ON)
		{
			m_PlayMode[type] &= 0x0000FFFF;
			m_PlayMode[type] |= (wRandomMode<<16);

			// 如果random打开了,repeate模式处于MSDK_REPEATE_ONE,需要repeate模式切换为MSDK_REPEATE_OFF
			if (wRandomMode == MSDK_RANDOM_ON
				&& GetPlayRepeateMode(type) == MSDK_REPEATE_ONE)
			{
				SetPlayRepeateMode(MSDK_REPEATE_OFF, type);
			}
		}
	}

}

WORD CMsdkFileManager::GetPlayRepeateMode(MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT)
	{
		return (WORD)m_PlayMode[type];
	}

	return MSDK_REPEATE_ALL;
}

WORD CMsdkFileManager::GetPlayRandomMode(MSDK_FILE_TYPE type)
{
	if (type>=0 && type<MSDK_FILE_TYPE_COUNT)
	{
		return (WORD)(m_PlayMode[type]>>16);
	}

	return MSDK_RANDOM_OFF;
}

int CMsdkFileManager::GetPlayFolderNext(MSDK_FILE_TYPE type)
{
	WORD random_mode = GetPlayRandomMode(type);
	int count = m_aFolder[type].GetCount();
	int next = m_PlayFileInfo.folder_index[type];
	if (count <= 0)	// 确保有数据
	{
		return 0;
	}

	if (random_mode == MSDK_RANDOM_OFF)
	{
		next++;
		if (next >= count)
		{
			next = 0;
		}
	}
	else
	{
		next = _myrand() % count;
	}

	return next;
}

int CMsdkFileManager::PlayFileNext(MSDK_FILE_TYPE type)
{
	if (type<0 && type>=MSDK_FILE_TYPE_COUNT)
	{
		return -1;	// 参数错误
	}

	// 还没加载到正确的数据
	if (m_PlayFileInfo.file_list[type].GetCount() <= 0)
	{
		return -1;
	}

	WORD repeate_mode = GetPlayRepeateMode(type);
	WORD random_mode = GetPlayRandomMode(type);
	int index = GetPlayingFileIndex(type);
	int next = index;
	switch (repeate_mode)
	{
	case MSDK_REPEATE_ONE:
		// 播放同一首
		break;
	case MSDK_REPEATE_FOLDER:
		if (random_mode == MSDK_RANDOM_OFF)
		{
			next++;
			if (next >= m_PlayFileInfo.file_list[type].GetCount())	// 已经到目录最后一个了
			{
				next = 0;
			}
		}
		else
		{
			next = _myrand() % m_PlayFileInfo.file_list[type].GetCount();
		}
		break;
	case MSDK_REPEATE_ALL:
		if (random_mode == MSDK_RANDOM_OFF)
		{
			next++;
			if (next >= m_PlayFileInfo.file_list[type].GetCount())	// 已经到目录最后一个了
			{
				next = 0;
				int folder_index = GetPlayFolderNext(type);
				SetPlayFileData(folder_index, next, type);	// 这里可能更改了播放目录
			}
		}
		else
		{
			int folder_index = GetPlayFolderNext(type);
			SetPlayFileData(folder_index, 0, type);	// 这里可能更改了播放目录
			ASSERT(m_PlayFileInfo.file_list[type].GetCount() > 0);
			next = _myrand() % m_PlayFileInfo.file_list[type].GetCount();
		}
		break;
	case MSDK_REPEATE_OFF:
		if (random_mode == MSDK_RANDOM_OFF)	// 这种情况就是顺序播放到最后一曲,停下来
		{
			next++;
			if (next >= m_PlayFileInfo.file_list[type].GetCount())	// 已经到目录最后一个了
			{
				next = 0;
				int folder_index = GetPlayFolderNext(type);
				if (folder_index<=GetPlayFolderIndex(type))	// 如果下一个目录不是在当前目录后面,说明当前目录已经是最后一个目录了
				{
					// 当前播放信息设为第一首
					SetPlayFileData(0, 0, type);
					return -1;	// 停止播放
				}
				SetPlayFileData(folder_index, next, type);	// 这里可能更改了播放目录
			}
		}
		else	// random_on时应该与 MSDK_REPEATE_ALL一样的行为
		{
			int folder_index = GetPlayFolderNext(type);
			SetPlayFileData(folder_index, 0, type);	// 这里可能更改了播放目录
			ASSERT(m_PlayFileInfo.file_list[type].GetCount() > 0);
			next = _myrand() % m_PlayFileInfo.file_list[type].GetCount();
		}
		break;
	default:
		break;
	}

	if (next >=0)	// 数据正确
	{
		m_PlayFileInfo.cur_file_index[type] = next;
	}

	return next;
}


int CMsdkFileManager::GetPlayFolderPrev(MSDK_FILE_TYPE type)
{
	WORD random_mode = GetPlayRandomMode(type);
	int count = m_aFolder[type].GetCount();
	int next = m_PlayFileInfo.folder_index[type];
	if (count <= 0)	// 确保有数据
	{
		return 0;
	}

	if (random_mode == MSDK_RANDOM_OFF)
	{
		next--;
		if (next < 0)
		{
			next = count-1;	// 最后一个目录
		}
	}
	else
	{
		next = _myrand() % count;
	}

	return next;
}


int CMsdkFileManager::PlayFilePrev(MSDK_FILE_TYPE type)
{
	if (type<0 && type>=MSDK_FILE_TYPE_COUNT)
	{
		return -1;	// 参数错误
	}

	// 还没加载到正确的数据
	if (m_PlayFileInfo.file_list[type].GetCount() <= 0)
	{
		return -1;
	}

	WORD repeate_mode = GetPlayRepeateMode(type);
	WORD random_mode = GetPlayRandomMode(type);
	int index = GetPlayingFileIndex(type);
	int next = index;
	switch (repeate_mode)
	{
	case MSDK_REPEATE_ONE:
		// 播放同一首
		break;
	case MSDK_REPEATE_FOLDER:
		if (random_mode == MSDK_RANDOM_OFF)
		{
			next--;
			if (next<0)	// 已经到目录第一个了
			{
				next =  m_PlayFileInfo.file_list[type].GetCount()-1;
			}
		}
		else
		{
			next = _myrand() % m_PlayFileInfo.file_list[type].GetCount();
		}
		break;
	case MSDK_REPEATE_ALL:
	case MSDK_REPEATE_OFF:
		if (random_mode == MSDK_RANDOM_OFF)	// 一直往前找上一首
		{
			next--;
			if (next < 0)	// 已经到目录第一个了, 需要切到上一个目录的最后一首
			{
				int folder_index = GetPlayFolderPrev(type);
				SetPlayFileData(folder_index, 0, type);	// 这里可能更改了播放目录
				next = m_PlayFileInfo.file_list[type].GetCount()-1;
			}
		}
		else	// random_on
		{
			int folder_index = GetPlayFolderPrev(type);
			SetPlayFileData(folder_index, 0, type);	// 这里可能更改了播放目录
			ASSERT(m_PlayFileInfo.file_list[type].GetCount() > 0);
			next = _myrand() % m_PlayFileInfo.file_list[type].GetCount();
		}
		break;
	default:
		break;
	}

	if (next >=0)	// 数据正确
	{
		m_PlayFileInfo.cur_file_index[type] = next;
	}

	return next;
}

BOOL CMsdkFileManager::PlayFileGoto(int nIndex, MSDK_FILE_TYPE type)
{
	if ( nIndex>=0 && nIndex<m_PlayFileInfo.file_list[type].GetCount())
	{
		if (nIndex != GetPlayingFileIndex(type))
		{
			m_PlayFileInfo.cur_file_index[type] = nIndex;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMsdkFileManager::GetFileIndexInfo(LPCTSTR lpszPath, MSDK_FILE_TYPE type, 
									   OUT int& nFolderIndex, OUT int& nFileIndex)
{
	if (type<0 || type>=MSDK_FILE_TYPE_COUNT || !lpszPath)
	{
		return FALSE;	// 参数错误
	}

	// 查找folder
	TCHAR szFolderName[MAX_PATH] = L"";
	tzutil::GetFilePath(lpszPath, szFolderName);
	int i = 0;
	BOOL bFolderFound = FALSE;

	const CStringArray& afolder = GetFolderData(type);

	for (i=0; i<afolder.GetCount(); i++)
	{
		if (_tcsicmp(szFolderName, afolder.GetAt(i)) == 0)
		{
			bFolderFound = TRUE;
			break;
		}
	}

	// 查找file
	int j = 0;
	BOOL bFileFound = FALSE;
	if (bFolderFound)
	{
		const CStringArray& afile = GetBrowseFileData(i, type);
		for (j=0; j<afile.GetCount(); j++)
		{
			if (_tcsicmp(lpszPath, afile.GetAt(j)) == 0)
			{
				bFileFound = TRUE;
				break;
			}
		}
	}

	if (bFolderFound && bFileFound)
	{
		nFolderIndex = i;
		nFileIndex = j;
	}

	return (bFolderFound && bFileFound);
}

SOURCE_ID CMsdkFileManager::GetCurrentSrcID()
{
	if (_tcsicmp(m_szFilepath2Load, config::get_config_disk()->sd) == 0)
	{
		return SOURCE_SD;
	}
	else if (_tcsicmp(m_szFilepath2Load, config::get_config_disk()->usb1) == 0)
	{
		return SOURCE_USB;
	}
	else if (_tcsicmp(m_szFilepath2Load, config::get_config_disk()->usb2) == 0)
	{
		return SOURCE_USB_REAR;
	}
	else
	{
		return INVALID_SOURCE_ID;
	}
}
