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
	int folder_index[MSDK_FILE_TYPE_COUNT];	// ѡ�е�Ŀ¼(Ϊ��Ŀ¼�����е�����)
	CStringArray file_list[MSDK_FILE_TYPE_COUNT];	// ����ѡ��Ŀ¼���ļ�
	BOOL is_loaded[MSDK_FILE_TYPE_COUNT];	// ��¼ѡ��Ŀ¼�µ��ļ��Ƿ��Ѿ���ʼ��,���Ƿ��Ѿ�����������
	int cur_file_index[MSDK_FILE_TYPE_COUNT];		// ѡ�е��ļ�(Ϊ���ļ������е�����)
};

class CMsdkFileManager
{
public:
	CMsdkFileManager(void);
	virtual ~CMsdkFileManager(void);

// interface
public:
	static CMsdkFileManager* GetInstance();

	// ���øýӿڳ�ʼ�����ݣ�ֻ�����һ�Σ�����̷����ˣ������ٴε���
	// �������ͬ�Ĳ������ã��ᵼ�����¼������ݡ�
	// lpszPath ~ ��������ݵ�·��������ָ�̷�
	// type ~ ���ȼ��ص��ļ����ͣ��������ļ�������Ҫ����ʱ���Ż����
	void InitLoadData(LPCTSTR lpszPath, MSDK_FILE_TYPE type);

	// ��ȡָ���ļ����͵�Ŀ¼����
	const CStringArray& GetFolderData(MSDK_FILE_TYPE type);

	//
	//***********    ����ļ��ӿ�   ****************/
	//
	// ��ȡ�ļ��б�, ֻҪ�����˸ýӿڣ�����Ϊָ����Ŀ¼Ϊ��ǰĿ¼(ָ��������)
	// ��ǰĿ¼���Ų��ŵĽ��У��п����л�Ϊ����Ŀ¼
	const CStringArray& GetBrowseFileData(int nFolderIndex, MSDK_FILE_TYPE type);

	// Ҫ�������ӿ�ʱ����֤�����͵������ȼ�����
	void SetBrowseFileIndex(int nFileIndex, MSDK_FILE_TYPE type);

	// ���ص�ǰĿ¼
	// �������û���ػ�������󷵻�-1
	int GetBrowseFolderIndex(MSDK_FILE_TYPE type);

	// ���ص�ǰѡ�е��ļ�
	// �������û���ػ�������󷵻�-1
	int GetBrowseFileIndex(MSDK_FILE_TYPE type);

	// ���ص�ǰ���������Ŀ¼���ļ�����
	// �������û���ػ�������󷵻�-1
	int GetBrowseFilesCount(MSDK_FILE_TYPE type);

	//
	//***********    �����ļ��ӿ�   ****************/
	//
	// nFolderIndex ~ ��Ŀ¼�����е�����
	// nFileIndex ~ ��ǰѡ�е��ļ��������ڲ��ŵ��ļ�
	// type ~ ���� (ͼƬ����������ͬʱ����)
	void SetPlayFileData(int nFolderIndex, int nFileIndex, MSDK_FILE_TYPE type);

	// ���ص�ǰĿ¼
	// �������û���ػ�������󷵻�-1
	int GetPlayFolderIndex(MSDK_FILE_TYPE type);

	// ���ص�ǰѡ�е��ļ�,�������ŵ��ļ�
	// �������û���ػ�������󷵻�-1
	int GetPlayingFileIndex(MSDK_FILE_TYPE type);

	// ���ص�ǰ���ڲ��ŵ�Ŀ¼���ļ�����
	// �������û���ػ�������󷵻�-1
	int GetPlayingFilesCount(MSDK_FILE_TYPE type);

	// ���ݲ����ļ��б�����ֵ�����ļ���
	LPCTSTR GetPlayFilePath(int nIndex, MSDK_FILE_TYPE type);

	// ����ģʽ
	void SetPlayRepeateMode(WORD wRepeateMode, MSDK_FILE_TYPE type);
	void SetPlayRandomMode(WORD wRandomMode, MSDK_FILE_TYPE type);
	WORD GetPlayRepeateMode(MSDK_FILE_TYPE type);
	WORD GetPlayRandomMode(MSDK_FILE_TYPE type);

	// ������������Ҫ���ŵ��ļ�����ֵ, ���ʧ�ܷ���-1
	// ���øýӿڻ���²����ļ��б����ݣ�����ֱ�ӵ�����ؽӿڻ�ȡ���µ����ݣ�
	// ����Ŀ¼��Ϣ(�ýӿڿ��ܻᵼ��Ŀ¼�л�)
	int PlayFileNext(MSDK_FILE_TYPE type);
	int PlayFilePrev(MSDK_FILE_TYPE type);

	// ���ŵ�ǰĿ¼��ָ�����ļ�
	// ���ָ���Ĳ����Ƿ�,��������²������ݣ�������FALSE
	BOOL PlayFileGoto(int nIndex, MSDK_FILE_TYPE type);

	//
	//***********    ��������   ****************/
	//
	// �����ļ��������ͣ���ѯ���ڵ�ǰ�����е�����λ�ã�����Ŀ¼���ļ�����
	// ���δ�ҵ���Ӧ�������FALSE
	BOOL GetFileIndexInfo(LPCTSTR lpszPath, MSDK_FILE_TYPE type, OUT int& nFolderIndex, OUT int& nFileIndex);
	// �Ӽ������ݵ��̷��ж����ĸ�Դ
	SOURCE_ID GetCurrentSrcID();

protected:
	// ��ָ��·���з���������Ŀ¼���ҳ���
	void LoadFolderData(LPCTSTR lpszPath, MSDK_FILE_TYPE type);

	// ��ָ��·���з����������ļ����ҳ���, �����ڵ�ǰ�ļ�����
	void LoadBrowseFileData(LPCTSTR lpszPath, MSDK_FILE_TYPE type);

	// ��ʼ���ļ����͵���չ��
	void InitFileType();
	CStringArray m_aFileType[MSDK_FILE_TYPE_COUNT];	// �ļ����͵���չ��

	// �����������
	void ClearData();

	// Ŀ¼����
	CStringArray m_aFolder[MSDK_FILE_TYPE_COUNT];	// �������������Ŀ¼
	BOOL m_aIsFolderLoaded[MSDK_FILE_TYPE_COUNT];	// ��¼���ļ����͵�Ŀ¼�Ƿ��Ѿ���ʼ��

	// ��ǰ������ļ�����
	MFM_FILE_INFO m_BrowseFileInfo;

	// ��ǰ�����ļ�����
	MFM_FILE_INFO m_PlayFileInfo;
	// high word��ʾrandom״̬ 0 ~ ����أ� 1 ~ �����
	// low word��ʾ�ظ�ģʽ    0 ~ repeat all  1 ~ repeat folder  2 ~ repeat one
	UINT		  m_PlayMode[MSDK_FILE_TYPE_COUNT];


	// ��ǰ������״̬
	TCHAR m_szFilepath2Load[MAX_PATH+1];			// ��ǰ��Ҫ���ص�·��������Ӧ��ָ�̷�

	// ����ͳ��Ŀ¼���������ܳ��������
	int m_nFolderCount;

	// helper functions
	void AddFolders(LPCTSTR lpszFilePath, MSDK_FILE_TYPE eFileType, OUT CStringArray& strArray);
	BOOL IsFindFolder(LPCTSTR lpszFilePath, MSDK_FILE_TYPE eFileType);
	BOOL IsFindFile(LPCTSTR lpszFileName, MSDK_FILE_TYPE eFileType);
	void AddFiles(LPCTSTR lpszFilePath, MSDK_FILE_TYPE type, OUT CStringArray& strArray);

	// ���ݵ�ǰ��ѭ��ģʽ�õ���һ��Ŀ¼(��Ŀ¼��Ҫ��תʱ)
	int GetPlayFolderNext(MSDK_FILE_TYPE type);
	// ���ݵ�ǰ��ѭ��ģʽ�õ���һ��Ŀ¼(��Ŀ¼��Ҫ��תʱ)
	int GetPlayFolderPrev(MSDK_FILE_TYPE type);
};
