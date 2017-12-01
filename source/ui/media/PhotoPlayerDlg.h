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

	// 根据当前播放的文件生成播放文件列表
	// 生成的文件列表将包含当前目录中的所有文件，以及上一目录中的二个文件，下一目录中的二个文件，方便缓冲数据。
	// 如果发现当前浏览文件已经不是当前目录中的文件（前二个或后二个）时，需重新生成列表。
	void RebuildFileList();

	void OnPlayFileChange(int nIndex);
	// 根据指定的文件（由前二个参数指定）,向前查找其所在目录及其自身的索引,最多找二级目录
	// nFolderIndex ~ 当前正在播放的目录索引
	// nFileIndex ~ 当前正在播放的文件在自建的播放列表(m_PlayFileList)中的索引
	// nPrevFolderIndex ~ 返回指定文件所在目录的索引
	// nPrevFileIndex ~ 返回指定文件在其目录中的索引
	BOOL GetPrevFileInfo(int nFolderIndex, int nFileIndex, OUT int& nPrevFolderIndex, OUT int& nPrevFileIndex);
	// 根据指定的文件（由前二个参数指定）,向后查找其所在目录及其自身的索引,最多找二级目录
	BOOL GetNextFileInfo(int nFolderIndex, int nFileIndex, OUT int& nNextFolderIndex, OUT int& nNextFileIndex);


	void ShowFileTip();
	void ShowTipMessage(LPCTSTR message);
	void HideMessageTip();
	BOOL SaveBitmapToFile(LPCTSTR lpszFileName);

	CStringArray m_PlayFileList;	// 当前正在播放的文件列表
	int m_nPlayIndex;				// 正在播放的文件在列表中的索引
	int m_nExtraFrontCount;			// 追加在列表前的文件数（非当前目录中的文件）
	int m_nExtraEndCount;			// 追加在列表后的文件数（非当前目录中的文件）

	CPhotoShowLayer* m_pPhotoShowLayer;

};
WCEUI_DYNCREATE_END(CPhotoPlayerDlg, CDvpVideoBaseDlg);
