#include "StdAfx.h"
#include "msdkFileManager.h"
#include "vdiskFileManager.h"
#include "interface.h"
#include "filefind.h"
#include "tzUtil.h"
#include "config.h"
#include "sysUtil.h"

CVDiskFileManager::CVDiskFileManager(void)
: m_uDiskValid(0)
, m_nPlayingDiskID(-1)
{

}

CVDiskFileManager::~CVDiskFileManager(void)
{


}

CVDiskFileManager* CVDiskFileManager::GetInstance()
{
	static CVDiskFileManager file_manager;
	return &file_manager;
}

void CVDiskFileManager::InitLoadData()
{
	// ��ʼ��Ŀ¼����
	CMsdkFileManager::GetInstance()->InitLoadData(GetVDiskPath(), MSDK_FILE_MUSIC);

	// �ж������Ƭ�Ƿ���Ч
	m_uDiskValid = 0;
	const CStringArray& folder = CMsdkFileManager::GetInstance()->GetFolderData(MSDK_FILE_MUSIC);
	for (int i=0; i<VDISK_MAX_DISK_COUNT; i++)
	{
		CString vdisk_path = GetVDiskPath(i);
		for (int j=0; j<folder.GetCount(); j++)
		{
			if (vdisk_path.CompareNoCase(folder.GetAt(j)) == 0)	// Ŀ¼����,��Ϊ�õ���Ч
			{
				m_uDiskValid |= (1<<i);
				break;
			}
		}
	}
}

BOOL CVDiskFileManager::GetFileIndexInfo(LPCTSTR lpszPath, int nDiskIndex, OUT int& nFileIndex)
{
	int nFolderIndex = DiskIndex2FolderIndex(nDiskIndex);
	if (nFolderIndex >= 0)
	{
		int folder = 0, file = 0;
		if( CMsdkFileManager::GetInstance()->GetFileIndexInfo(lpszPath, MSDK_FILE_MUSIC, folder, file)
			&& nFolderIndex == folder)	// ֻ��ָ��Ŀ¼���Ҹ��ļ�,�����λ������Ŀ¼��Ҳ��Ϊ������
		{
			nFileIndex = file;
			return TRUE;
		}
	}

	return FALSE;
}

int CVDiskFileManager::DiskIndex2FolderIndex(int nDiskIndex)
{
	if (IsDiskExist(nDiskIndex))
	{
		const CStringArray& folder = CMsdkFileManager::GetInstance()->GetFolderData(MSDK_FILE_MUSIC);
		CString vdisk_path = GetVDiskPath(nDiskIndex);
		for (int j=0; j<folder.GetCount(); j++)
		{
			if (vdisk_path.CompareNoCase(folder.GetAt(j)) == 0)	// Ŀ¼����,��Ϊ�õ���Ч
			{
				return j;
			}
		}
	}

	return -1;
}

CString CVDiskFileManager::GetVDiskPath(int nIndex)
{
	CString path;
	path.Format(L"%s\\vdisk%d", GetVDiskPath(), nIndex);
	return path;
}


CString CVDiskFileManager::GetVDiskPath()
{
	CString path;
	path.Format(L"%s\\vdisk", sysutil::nss_get_instance()->vdisk_path);
	return path;
}


BOOL CVDiskFileManager::IsDiskExist(int nDiskIndex)
{
	if (nDiskIndex>=0 && nDiskIndex<VDISK_MAX_DISK_COUNT)
	{
		return (m_uDiskValid & (1<<nDiskIndex)) ? TRUE : FALSE;
	}

	return FALSE;
}

int CVDiskFileManager::FirstDiskAvailable()
{
	for (int i=0; i<VDISK_MAX_DISK_COUNT; i++)
	{
		if (IsDiskExist(i))
		{
			return i;
		}
	}

	return -1;
}


int CVDiskFileManager::FirstDiskUnavailable()
{
	for (int i=0; i<VDISK_MAX_DISK_COUNT; i++)
	{
		if (!IsDiskExist(i))
		{
			return i;
		}
	}

	return -1;
}


int CVDiskFileManager::LastDiskAvailable()
{
	for (int i=VDISK_MAX_DISK_COUNT-1; i>=0; i--)
	{
		if (IsDiskExist(i))
		{
			return i;
		}
	}

	return -1;
}

int CVDiskFileManager::PrevDiskAvailable(int nDiskIndex)
{
	if (nDiskIndex>=0 && nDiskIndex<VDISK_MAX_DISK_COUNT)
	{
		for (int i=nDiskIndex-1; i>=0; i--)
		{
			if (IsDiskExist(i))
			{
				return i;
			}
		}

		// ���û�ҵ�����ģ��������һ��
		return LastDiskAvailable();
	}

	return -1;
}

int CVDiskFileManager::NextDiskAvailable(int nDiskIndex)
{
	if (nDiskIndex>=0 && nDiskIndex<VDISK_MAX_DISK_COUNT)
	{
		for (int i=nDiskIndex+1; i<VDISK_MAX_DISK_COUNT; i++)
		{
			if (IsDiskExist(i))
			{
				return i;
			}
		}

		// ���û�ҵ�����ģ����ص�һ��
		return FirstDiskAvailable();
	}

	return -1;
}

void CVDiskFileManager::SetPlayFileData(int nDiskIndex, int nFileIndex)
{
	int nFolderIndex = DiskIndex2FolderIndex(nDiskIndex);
	if (nFolderIndex >= 0)
	{
		CMsdkFileManager::GetInstance()->SetPlayFileData(nFolderIndex, nFileIndex, MSDK_FILE_MUSIC);
		m_nPlayingDiskID = nDiskIndex;	// �Ϸ��Ĳż�¼
	}
	else
	{
		m_nPlayingDiskID = -1;
	}
}

int CVDiskFileManager::GetPlayingDiskID()
{
	return m_nPlayingDiskID;
}

void CVDiskFileManager::SetPlayRepeateMode(WORD wRepeateMode)
{
	CMsdkFileManager::GetInstance()->SetPlayRepeateMode(wRepeateMode, MSDK_FILE_MUSIC);
}

void CVDiskFileManager::SetPlayRandomMode(WORD wRandomMode)
{
	CMsdkFileManager::GetInstance()->SetPlayRandomMode(wRandomMode, MSDK_FILE_MUSIC);
}

WORD CVDiskFileManager::GetPlayRepeateMode()
{
	return CMsdkFileManager::GetInstance()->GetPlayRepeateMode(MSDK_FILE_MUSIC);
}

WORD CVDiskFileManager::GetPlayRandomMode()
{
	return CMsdkFileManager::GetInstance()->GetPlayRandomMode(MSDK_FILE_MUSIC);
}

int CVDiskFileManager::GetPlayingFileIndex()
{
	return CMsdkFileManager::GetInstance()->GetPlayingFileIndex(MSDK_FILE_MUSIC);
}

LPCTSTR CVDiskFileManager::GetPlayFilePath(int nIndex)
{
	return CMsdkFileManager::GetInstance()->GetPlayFilePath(nIndex, MSDK_FILE_MUSIC);
}

int CVDiskFileManager::PlayFileNext()
{
	return CMsdkFileManager::GetInstance()->PlayFileNext(MSDK_FILE_MUSIC);
}

int CVDiskFileManager::PlayFilePrev()
{
	return CMsdkFileManager::GetInstance()->PlayFilePrev(MSDK_FILE_MUSIC);
}