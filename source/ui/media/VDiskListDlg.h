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

	void OnRipInterrupt();	// ¼����;ֹͣ��(�����ǳ���,��Դ,�����)
protected:
	virtual void OnBnClick(CWceUiButton* pButton);
	// ֪ͨ������ʾ��ʼ
	virtual void OnAnimateBegin();
	// ֪ͨ������ʾ����
	virtual void OnAnimateEnd();
	void ShowStaticLayers(BOOL bShow);


	void PlayItem(int nDiskIndex);
	void PlayNext();

	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);
	void OnGraphBuilt();

	// ��ʼ������Ƭ״̬
	void InitActiveDisk();
	// �����Ƭ��¼��,��Ϊactive״̬, nIndex������0��ʼ����
	void ActiveDiskButton(int nIndex, BOOL bActive);
	// disk�����Ҳ�������¼�Ƶĵ�Ƭ,����Ϊ��active�ġ���ʾΪactive״̬���ɲ���
	BOOL IsDiskActive(int nIndex);

	// ���û���ڲ���disk,������Ϊ-1
	void SetPlayingDisk(int nDiskIndex);

	void ShowPlayingDlg();

	// rip
	void OnDvpMediaEvent(UINT media_event, UINT param1, UINT param2);
	void OnDiskError();
	void ShowTipMessage(LPCTSTR message);
	BOOL RipTrack(int nTrack);
	CString GetRipFileName(int nTrack); 
	void OnRipTrackFinish();	// track¼�����
	void OnRipDiskFinish();		// ���ŵ�¼�����
	void OnBnRecord();
	void DeleteDisk(int nDiskIndex);
	void ShowRipProgress(int nPercent);	// ����Ϊ��ǰRIP�����Ľ���
	void OnRipStart();

	// ¼�����ʱ����CD ID�ż�¼�Ƶ��˶�����
	BOOL SaveRipInfo(int nDiskIndex, int nTracks);	
	// ����CD ID�Ų����Ƿ��Ѿ�¼�ƹ�,���¼�ƹ�����TRUE,�����ص�Ƭ��¼���˶�������
	BOOL GetRipInfo(const BYTE *pDiskID, OUT int& nDiskIndex, OUT int& nTracks);

	BYTE m_DiskID[8];	// ��ǰCD����ID��
	int m_nRipDisk;		// RIP���ڼ��ŵ�
	int m_nCurRipTrack;	// ��ǰ����rip��track
//	BOOL m_bIsRiping;	// ��ʶ�Ƿ�Ҫ����rip����

	BOOL m_bIsPopup;	// ����DoModle()��������ʱ,��ΪTRUE
};
WCEUI_DYNCREATE_END(CVDiskListDlg, CDvpBaseDlg);
