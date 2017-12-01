#pragma once
#include "dvpbasedlg.h"
#include "dvdrender.h"
#include "MMFileListDlg.h"

#define TIMER_ID_START_PLAY_CD		1001

class CCDFileListDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CCDFileListDlg, CDvpBaseDlg)
public:
	CCDFileListDlg(void);
	~CCDFileListDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual void OnListSelChange(CWceUiListBox* plist, int nItemClicked, BOOL bClickChecked);
	
	void InitListbox();

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	static void DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData);

	void ShowFileTip();
	void ShowPlayingDlg();

	CFileListBox *m_pListFile;
	CWceUiLoadBitmap m_texPlaying;
	CWceUiLoadBitmap m_texMusic;
};
WCEUI_DYNCREATE_END(CCDFileListDlg, CDvpBaseDlg);
