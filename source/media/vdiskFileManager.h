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

	// ��ʼ��������Ŀ¼�е��ļ�����
	// �������õĶ��Ŀ¼����ʼ��CMsdkFileManager,������������ļ�����
	void InitLoadData();

	// ���������Ƭ�Ƿ���Ч,������0��ʼ
	BOOL IsDiskExist(int nDiskIndex);

	// ���ص�һ����Ч��disk,���û����Чdisk������-1
	int FirstDiskAvailable();
	int LastDiskAvailable();

	// ���ص�һ����Ч��disk,���û�У�����-1
	int FirstDiskUnavailable();

	// ��һ����Чdisk,����ѵ����һ�������ص�һ��
	int NextDiskAvailable(int nDiskIndex);
	int PrevDiskAvailable(int nDiskIndex);

	// �����ļ�������ƬID����ѯ���ڵ�ǰ�����е�����λ��
	// ���δ�ҵ���Ӧ�������FALSE
	BOOL GetFileIndexInfo(LPCTSTR lpszPath, int nDiskIndex, OUT int& nFileIndex);

	// nDiskIndex ~ ��Ŀ¼�����е�����
	// nFileIndex ~ ��ǰѡ�е��ļ��������ڲ��ŵ��ļ�
	void SetPlayFileData(int nDiskIndex, int nFileIndex);

	// ����ģʽ
	void SetPlayRepeateMode(WORD wRepeateMode);
	void SetPlayRandomMode(WORD wRandomMode);
	WORD GetPlayRepeateMode();
	WORD GetPlayRandomMode();
	int PlayFileNext();
	int PlayFilePrev();

	// ���ص�ǰѡ�е��ļ�,�������ŵ��ļ�
	// �������û���ػ�������󷵻�-1
	int GetPlayingFileIndex();

	// ���ݲ����ļ��б�����ֵ�����ļ���
	LPCTSTR GetPlayFilePath(int nIndex);

	//
	int GetPlayingDiskID();

	// ��ȡÿ�ŵ���·��
	CString GetVDiskPath(int nIndex);
	// ��ȡΪ�����Ƭָ����·��
	CString GetVDiskPath();

protected:

	// ���������Ŀ¼��������Ŀ¼,diskindex��folderindex��һ����һһ��Ӧ��ϵ����������ת��
	// ���ת�����ɹ����� -1 
	int DiskIndex2FolderIndex(int nDiskIndex);

	// ��һλ����ʾ�����Ƭ�Ƿ���Ч�����λ��ʾ��һ�ŵ�
	UINT m_uDiskValid;
	// ��ǰ���ڲ��ŵ�disk,���û�в�����Ϊ-1
	int  m_nPlayingDiskID;
};
