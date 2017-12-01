#pragma once
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpBaseDlg.h"

#define TIMER_ID_DELAY_ANIMATING	1010
#define TIMER_ID_RIP_START			1011

class CVDiskListDlg : public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CVDiskListDlg, CDvpBaseDlg)
public:
	static void MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data);
	static void DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData);

	CVDiskListDlg(void);
	~CVDiskListDlg(void);	

	virtual void OnInitDialog();
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

	void OnRipInterrupt();	// 录制中途停止了(可能是出碟,切源,出错等)
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	// 通知动画显示开始
	virtual void OnAnimateBegin();
	// 通知动画显示结束
	virtual void OnAnimateEnd();
	void ShowStaticLayers(BOOL bShow);


	void PlayItem(int nDiskIndex);
	void PlayNext();

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	void OnGraphBuilt();

	// 初始化各碟片状态
	void InitActiveDisk();
	// 如果碟片有录制,设为active状态, nIndex索引从0开始计数
	void ActiveDiskButton(int nIndex, BOOL bActive);
	// disk存在且不是正在录制的碟片,则认为是active的。显示为active状态并可播放
	BOOL IsDiskActive(int nIndex);

	// 如果没有在播放disk,参数设为-1
	void SetPlayingDisk(int nDiskIndex);

	void ShowPlayingDlg();

	// rip
	void OnDvpMediaEvent(UINT media_event, UINT param1, UINT param2);
	void OnDiskError();
	void ShowTipMessage(LPCTSTR message);
	BOOL RipTrack(int nTrack);
	CString GetRipFileName(int nTrack); 
	void OnRipTrackFinish();	// track录制完成
	void OnRipDiskFinish();		// 整张碟录制完成
	void OnBnRecord();
	void DeleteDisk(int nDiskIndex);
	void ShowRipProgress(int nPercent);	// 参数为当前RIP歌曲的进度
	void OnRipStart();

	// 录制完毕时保存CD ID号及录制到了多少首
	BOOL SaveRipInfo(int nDiskIndex, int nTracks);	
	// 根据CD ID号查找是否已经录制过,如果录制过返回TRUE,并返回碟片及录制了多少首了
	BOOL GetRipInfo(const BYTE *pDiskID, OUT int& nDiskIndex, OUT int& nTracks);

	BYTE m_DiskID[8];	// 当前CD碟的ID号
	int m_nRipDisk;		// RIP到第几张碟
	int m_nCurRipTrack;	// 当前正在rip的track
//	BOOL m_bIsRiping;	// 标识是否要进行rip操作

	BOOL m_bIsPopup;	// 当有DoModle()弹出窗口时,置为TRUE
};
WCEUI_DYNCREATE_END(CVDiskListDlg, CDvpBaseDlg);
