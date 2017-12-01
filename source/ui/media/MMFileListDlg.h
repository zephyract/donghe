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
	// ���ŵ�ǰ�ļ��б����ָ�����ļ�
	void PlayItem(int nIndex);

	
	void OnTypeChange(MSDK_FILE_TYPE type);

	// ѡ��ָ�������Ͱ�ť,�����������Ť��ѡ��״̬
	void CheckButton(MSDK_FILE_TYPE type);

	CWceUiListBox *m_pListFolder;
	CFileListBox *m_pListFile;

	void SetFolderData(MSDK_FILE_TYPE type);
	// nFilderIndex ~ ��Ҫ��ʾ�ĸ�Ŀ¼������
	// type ~ ����
	// nFileIndex ~ ��ѡ�еĵ�ǰ�ļ�
	void SetFileData(int nFolderIndex, MSDK_FILE_TYPE type, int nFileIndex);
	void AddString2Listbox(const CStringArray& strArray, CWceUiListBox* plistbox, CWceUiGenericBitmap* pTexture);
	
	// nMode : 0 ~ ��ʾfile listbox,  1 ~ ��ʾfolder listbox
	void ShowListBox(int nMode);
	void ShowFileTip();
	void ShowTitle();

	// ���ݵ�ǰ����������ʾ��Ӧ�Ĳ��Ž���
	void ShowPlayingDlg();

	// ��ʾ���ڲ��ŵ�����
	void ShowPlayingData();

	MSDK_FILE_TYPE m_curType;	// ��¼��ǰ����������ļ�����
	// ��Ϊ��ǰ�����ͼƬʱ,������Ƶ�����ں�̨����,������ר�ż�¼���ڲ��ŵ�����,�Ա㴦����һ������
	MSDK_FILE_TYPE m_curPlayingType;

	CWceUiLoadBitmap m_texMusic;
	CWceUiLoadBitmap m_texVideo;
	CWceUiLoadBitmap m_texPhoto;
	CWceUiLoadBitmap m_texFolder;
	CWceUiLoadBitmap m_texPlaying;	// �б���������ʶ���ڲ��ŵĸ���

	// key pad
	void ShowKeypad(BOOL bShow);
	void OnKeyEnter();
	BOOL OnBnKeypadClick(CWceUiButton* pButton);
	CWceUiLayer* m_pLayerFrameKeypad;

	//
	void CheckMediaInfo();
	VIDEO_ERROR_CODE m_nErrorCode;

	// ��ѯ����
	BOOL AddString2DB(const CStringArray& strArray);
	void UpdateQuery2Listbox();
	void OpenDB();
	void ShowQueryUI(BOOL bShow);
	BOOL m_bShowQueryUI;
	CppSQLite3DB m_db;
	CArray<INT> m_PBQueryResult;

	// �������ļ�ʱ����ǰˢ�²��Ŵ��ڵ������Ϣ
	void UpdatePlayDlgMediaInfo(MSDK_FILE_TYPE type);

	void InitCodepage();

};
WCEUI_DYNCREATE_END(CMMFileListDlg, CDvpBaseDlg);



// ָ��һ����¼,��ʾ���ڲ��ŵ�ͼ��
class CFileListBox : public CWceUiListBox
{
	WCEUI_DYNCREATE_BEGIN(CFileListBox, CWceUiListBox)
public:
	CFileListBox();
	~CFileListBox();

	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);
	// ָ����ǰ���ŵļ�¼,���Ϊ-1��û�в��ż�¼
	void SetPlayIndex(int nIndex);
	// ֻ��ָ��Xƫ�ƣ�Y��������
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
	// ��ʾʱ,�����ݸ�ʽ����,  1/4, ͬʱ��������Ƿ�Ϸ�,������Ϸ�����Ϸ���,���ݲ��ܴ���m_nTotal
	void FormatData(OUT LPTSTR buffer);
	int m_nTotal;
};

WCEUI_DYNCREATE_END(CGotoEditBox, CWceUiSEditBox);


