#include "stdafx.h"
#include "DVDFolderListBox.h"
#include "filefind.h"
#include "DVDFileListDlg.h"
#include "tzUtil.h"
#include "GDef.h"
#include "DVP_Agent.h"
CDVDFolderListBox::CDVDFolderListBox()
{
	if (WceUiGetBitsPixel() == 16)
	{
		m_bmpUpLevel.SetBitmapFromRes(L"media\\filelist\\up_level.565a.conf");
		m_bmpFolder.SetBitmapFromRes(L"media\\filelist\\folder.565a.conf");
		m_bmpAudio.SetBitmapFromRes(L"media\\filelist\\music.565a.conf");
		m_bmpVideo.SetBitmapFromRes(L"media\\filelist\\video.565a.conf");
		m_bmpPicture.SetBitmapFromRes(L"media\\filelist\\photo.565a.conf");
	}
	else
	{
		m_bmpUpLevel.SetBitmapFromRes(L"media\\filelist\\up_level.32a.conf");
		m_bmpFolder.SetBitmapFromRes(L"media\\filelist\\folder.32a.conf");
		m_bmpAudio.SetBitmapFromRes(L"media\\filelist\\music.32a.conf");
		m_bmpVideo.SetBitmapFromRes(L"media\\filelist\\video.32a.conf");
		m_bmpPicture.SetBitmapFromRes(L"media\\filelist\\photo.32a.conf");
	}
}


CDVDFolderListBox::~CDVDFolderListBox()
{
	ResetListContent();
}


void CDVDFolderListBox::ResetListContent()
{
	for (int i=0; i<GetCount(); i++)
	{
		FILE_LIST_ITEM_DATA* pid = (FILE_LIST_ITEM_DATA*)GetItemData(i);
		delete pid;
	}
	ResetContent();
}

BOOL CDVDFolderListBox::IsRootDir()
{
	// 在其它的过滤模式时，没有目录的概念，直接返回TRUE，不存在有子目录的情况
	CDVDFileListDlg* pParent = (CDVDFileListDlg*)GetParent();
	if (pParent->m_nRefreshMode != FILE_FILTER_BTN_ALL)
	{
		return TRUE;
	}

	// if it's root dir, DVP_GetCurrentDir() returns L"\"
	TCHAR szDir[MAX_PATH] = L"\0";
	int count = 0;		// DVD模块有点BUG，有时无法获取目录信息，如果多次(50次)无法获取，认为是在根目录
	while (  DVP_GetCurrentDir(szDir, MAX_PATH) != RET_OK || _tcslen(szDir) == 0)
	{
		Sleep(10);
		if (count++ > 50)
		{
			return TRUE;
		}
	}
	if (_tcslen(szDir) > 1)
	{
		return FALSE;
	}
	else 
	{
		return TRUE;
	}
}

void CDVDFolderListBox::AddFiles()
{
	if (!IsRootDir())
	{
		// add .. folder
		FILE_LIST_ITEM_DATA *pid = new FILE_LIST_ITEM_DATA();
		pid->nItemType = ID_TYPE_PARENT_FOLDER;
		_tcscpy(pid->szFilePath, L"..");
		int nIndex = AddString(L"..");
		SetItemData(nIndex, (DWORD)pid);
		SetItemImage(nIndex, m_bmpUpLevel.GetBitmap());
	}


	GUINT16 nFileCount = 0;
	DVP_GetCurFileCount(&nFileCount);
	for (UINT32 i=0; i<nFileCount; i++)
	{
		AddFile(i);
	}
}

void CDVDFolderListBox::AddFile(int nDVPIndex)
{
	// 如果不是根目录且在追加第一条数据时，增加父目录项
// 	if (!IsRootDir() && nDVPIndex == 0)
// 	{
// 		// add .. folder
// 		FILE_LIST_ITEM_DATA *pid = new FILE_LIST_ITEM_DATA();
// 		pid->nItemType = ID_TYPE_PARENT_FOLDER;
// 		_tcscpy(pid->szFilePath, L"..");
// 		int nIndex = AddString(L"..");
// 		SetItemData(nIndex, (DWORD)pid);
// 		SetItemImage(nIndex, m_bmpUpLevel.GetBitmap());
// 	}

	DVP_FILEITEM_INFO_T dfi;
	DVP_GetDramFileItemInfo(nDVPIndex, &dfi);
	TCHAR szFolderName[MAX_PATH];
	tzutil::GetFileName(dfi.szFileName, szFolderName);
	if (dfi.uType == FTYPE_DIR || dfi.uType == FTYPE_PDIR)
	{
		int nIndex = AddString(szFolderName);
		if (nIndex != LB_ERR)
		{
			FILE_LIST_ITEM_DATA *pid = new FILE_LIST_ITEM_DATA();
			pid->nItemType = ID_TYPE_FOLDER;
			_tcscpy(pid->szFilePath, dfi.szFileName);
			SetItemData(nIndex, (DWORD)pid);
			SetItemImage(nIndex, m_bmpFolder.GetBitmap());
		}
	}
	else
	{
		int nIndex = AddString(szFolderName);
		if (nIndex != LB_ERR)
		{
			FILE_LIST_ITEM_DATA *pid = new FILE_LIST_ITEM_DATA();
			pid->nItemType = ID_TYPE_FILE;
			_tcscpy(pid->szFilePath, dfi.szFileName);
			SetItemData(nIndex, (DWORD)pid);

			switch (GetFilterByType(dfi.uType))
			{
			case FILE_FILTER_BTN_AUDIO:
				SetItemImage(nIndex, m_bmpAudio.GetBitmap());
				break;
			case FILE_FILTER_BTN_VIDEO:
				SetItemImage(nIndex, m_bmpVideo.GetBitmap());
				break;
			case FILE_FILTER_BTN_PIC:
				SetItemImage(nIndex, m_bmpPicture.GetBitmap());
				break;
			default:
				break;
			}
		}
	}
}

int CDVDFolderListBox::GetFilterByType(int type)
{
	int filter_type = 0;
	switch (type)
	{
	case FTYPE_MP1:
	case FTYPE_MP2:
	case FTYPE_MP3:
	case FTYPE_WMA:
	case FTYPE_ASF:
	case FTYPE_DST:
	case FTYPE_DSD:
	case FTYPE_AAC:
	case FTYPE_CDA:
	case FTYPE_DTS:
	case FTYPE_OGG:
		filter_type = FILE_FILTER_BTN_AUDIO;
		break;

	case FTYPE_MPG:
	case FTYPE_DAT:
	case FTYPE_VOB:
	case FTYPE_AVI:
	case FTYPE_TS1:
	case FTYPE_TS2:
	case FTYPE_TS4:
	case FTYPE_MP4:
	case FTYPE_3GP:
	case 0x0E: /*FTYPE_OGM:*/
		filter_type = FILE_FILTER_BTN_VIDEO;
		break;

	case FTYPE_JPG:
		filter_type = FILE_FILTER_BTN_PIC;
		break;

	default:
		break;
	}

	return filter_type;
}

int CDVDFolderListBox::GetFolderCount()
{
	// 该列表，目录在前，文件在后
	int folder_count = 0;
	for (int i=0; i<GetCount(); i++)
	{
		FILE_LIST_ITEM_DATA *pid = (FILE_LIST_ITEM_DATA*)GetItemData(i);

		if (pid->nItemType != ID_TYPE_FILE)
		{
			folder_count++;
		}
		else
		{
			break;		// 出现文件后，后面不再可能有目录
		}
	}

	return folder_count;
}

int CDVDFolderListBox::SetCurSelFile(int nIndex)
{
	int folder_count = GetFolderCount();

	SetCurSel(nIndex+folder_count);
	if (!IsItemVisible(nIndex))
	{
		SetTopIndex(nIndex+folder_count);
	}
	return nIndex+folder_count;
}

int CDVDFolderListBox::GetCurSelFile()
{
	int folder_count = GetFolderCount();
	int nIndex = GetCurSel();
	if (nIndex >= folder_count)
	{
		return nIndex-folder_count;
	}
	else
	{
		return -1;
	}
}

int CDVDFolderListBox::GetFileCount()
{
	return GetCount() - GetFolderCount();
}