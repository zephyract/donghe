#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpVideoBaseDlg.h"
#include "PhotoShowLayer.h"

class CPhotoPlayerDlg : public CDvpVideoBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CPhotoPlayerDlg, CDvpVideoBaseDlg)
public:
	CPhotoPlayerDlg(void);
	~CPhotoPlayerDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual LRESULT OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnDlgShow(BOOL bShow);
	virtual void OnKeyPlayControl(UINT key, UINT param);
	void AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData);
	void ShowInvalidIcon();
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void ShowVideo(BOOL bShow, RECT* prect = NULL);

	// ���ݵ�ǰ���ŵ��ļ����ɲ����ļ��б�
	// ���ɵ��ļ��б�������ǰĿ¼�е������ļ����Լ���һĿ¼�еĶ����ļ�����һĿ¼�еĶ����ļ������㻺�����ݡ�
	// ������ֵ�ǰ����ļ��Ѿ����ǵ�ǰĿ¼�е��ļ���ǰ������������ʱ�������������б�
	void RebuildFileList();

	void OnPlayFileChange(int nIndex);
	// ����ָ�����ļ�����ǰ��������ָ����,��ǰ����������Ŀ¼�������������,����Ҷ���Ŀ¼
	// nFolderIndex ~ ��ǰ���ڲ��ŵ�Ŀ¼����
	// nFileIndex ~ ��ǰ���ڲ��ŵ��ļ����Խ��Ĳ����б�(m_PlayFileList)�е�����
	// nPrevFolderIndex ~ ����ָ���ļ�����Ŀ¼������
	// nPrevFileIndex ~ ����ָ���ļ�����Ŀ¼�е�����
	BOOL GetPrevFileInfo(int nFolderIndex, int nFileIndex, OUT int& nPrevFolderIndex, OUT int& nPrevFileIndex);
	// ����ָ�����ļ�����ǰ��������ָ����,������������Ŀ¼�������������,����Ҷ���Ŀ¼
	BOOL GetNextFileInfo(int nFolderIndex, int nFileIndex, OUT int& nNextFolderIndex, OUT int& nNextFileIndex);


	void ShowFileTip();
	void ShowTipMessage(LPCTSTR message);
	void HideMessageTip();
	BOOL SaveBitmapToFile(LPCTSTR lpszFileName);

	CStringArray m_PlayFileList;	// ��ǰ���ڲ��ŵ��ļ��б�
	int m_nPlayIndex;				// ���ڲ��ŵ��ļ����б��е�����
	int m_nExtraFrontCount;			// ׷�����б�ǰ���ļ������ǵ�ǰĿ¼�е��ļ���
	int m_nExtraEndCount;			// ׷�����б����ļ������ǵ�ǰĿ¼�е��ļ���

	CPhotoShowLayer* m_pPhotoShowLayer;

};
WCEUI_DYNCREATE_END(CPhotoPlayerDlg, CDvpVideoBaseDlg);
