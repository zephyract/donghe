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

	// lpszFilePath ~ ��Ҫ��ʾ��Ŀ¼·�����ؼ�����ʾ��Ŀ¼��������ص���Ŀ¼
	// aFileType    ~ �ļ����ͣ�ֻҪ������һ�ļ����͵��ļ���Ŀ¼������ʾ
	void Init()
	{
		ResetListContent();
		AddFiles();
	}

	void AddFile(int nDVPIndex);
	void AddFiles();
	BOOL IsRootDir();
	// ѡ��ָ���������ļ�������Ϊ�ļ���������������ǰ���Ŀ¼
	// �轫�ļ�������ת��Ϊ�б��������
	int SetCurSelFile(int nIndex);
	void ResetListContent();

	// �����б��е�ǰѡ�е��ļ�,����Ϊ�ļ�������,������ǰ���Ŀ¼, ��0��ʼ����
	int GetCurSelFile();
	// �����б��������ļ���,������Ŀ¼
	int GetFileCount();
	// �����б���Ŀ¼��
	int GetFolderCount();

protected:


	CWceUiLoadBitmap m_bmpUpLevel;
	CWceUiLoadBitmap m_bmpFolder;
	CWceUiLoadBitmap m_bmpAudio;
	CWceUiLoadBitmap m_bmpVideo;
	CWceUiLoadBitmap m_bmpPicture;


};

WCEUI_DYNCREATE_END(CDVDFolderListBox, CWceUiListBox);
