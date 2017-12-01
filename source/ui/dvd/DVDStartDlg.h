#pragma once
#include "stdafx.h"
#include "wceuidlg.h"
#include "WceUiButton.h"
#include "DvpBaseDlg.h"
#include "WceUiRotateBitmap.h"

#define TIMER_ID_AUTO_EXIT_DVD_SRC 1005

class CAnimDiskLayer;
class CDVDStartDlg: public CDvpBaseDlg
{
	WCEUI_DYNCREATE_BEGIN(CDVDStartDlg, CDvpBaseDlg)
public:
	CDVDStartDlg(void);
	~CDVDStartDlg(void);

	virtual void OnInitDialog();
	virtual void OnBnClick(CWceUiButton* pButton);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnTimer(WORD wIDEvent);
	virtual void OnDlgShow(BOOL bShow);
	void AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData);
	
	void OnMediaEvent(UINT media_event, UINT param1, UINT param2);

	// when receive EVT_DVP_INIT_STATE event, show current status.
	void ShowStatus(int nStatus);
	// load corresponding UI for specified disktype(DVD, CD, etc...)
	void LoadUI(UINT nDiskType);
	void MyShowDlg(DLG_ID did);	
	
	void OnEject();		// ����
	void OnTrayIn();	// ����
protected:
	static void DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData);
	void InitCodepage();

	// ֪ͨ������ʾ��ʼ
	virtual void OnAnimateBegin();
	// ֪ͨ������ʾ����
	virtual void OnAnimateEnd();

	// ��Ƭ����
	void OnTimerMoveIn();	// ����Ļ���ƶ�����Ļ��
	void OnTimerMoveOut();	// �����Ƶ���
	void OnTimerZoomIn();	// �Ŵ�
	void OnTimerZoomOut();	// ��С
	void OnTimerSpeedAcc();
	void OnTimerSpeedDec();
	void OnTimerRotateLaser();

	float m_fScale;	// ����ϵ��
	float m_fAngle;
	CRect m_rcOrginal;		// ��Ƭԭʼλ��
	CAnimDiskLayer* m_pDiskLayer;

	// ����ʱ�������ĸ�����,ĳ�����������У��п��ܳ���,�����������ֻ��ִ�в���
	BOOL m_bMoveIn;
	BOOL m_bZoomOut;
	BOOL m_bRotateLaser;
	BOOL m_bSpeedAcc;


	// laserת������
	int m_nRotateLaserCount;	// ������ת���Ĵ�����ȷ��ת��ָ���ĽǶ�
	int m_nCurAngle;			// ��ǰ���ڵĽǶ�
	int m_nAngleOnce;			// ÿ��ת���ĽǶ�,�����ٶ�
	int m_nAngleTotal;			// ��Ҫת�����ĸ��Ƕ�
	CAnimDiskLayer* m_pLaserLayer;
	// ����ָ��һ����ת�ĽǶȺ����յĽǶ�
	void AnimRotateLaser(int nAngleOnce, int nAngleTotal);	// ��ת�����ٽǶ�,���֮ǰ����ת,����㻹����ת���ٽǶ�

};

WCEUI_DYNCREATE_END(CDVDStartDlg, CDvpBaseDlg);

#define TIMER_ID_DISK_EJECT			201
#define TIMER_ID_ANIM_DISK_LAYER	2002

#define TIMER_ID_MOVE_OUT			204
#define TIMER_ID_MOVE_IN			205
#define TIMER_ID_ZOOM_IN			206
#define TIMER_ID_ZOOM_OUT			207
#define TIMER_ID_ROTATE_LASER				208

#define TIMER_ID_ROTATE_SPEED_ACC	209
#define TIMER_ID_ROTATE_SPEED_DEC	210

const UINT DISK_ANIM_TIME	= 20;	// 20ms
const float DISK_MAX_SCALE  = 1.2f;	// ���Ŵ�ϵ��

class CAnimDiskLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CAnimDiskLayer, CWceUiLayer)
public:
	CAnimDiskLayer();
	~CAnimDiskLayer();

	virtual void OnTimer(WORD wIDEvent);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnInitLayer();

	// �Ƕ�Խ��ת��Խ��
	void SetRotateAngle(float fAngle);
	void StartRotate();
	void StopRotate();
	void Scale(float scale);
	void Reset();	// �ص�ԭʼ״̬

	void Rotate();

protected:
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	float m_fScale;
	float m_fAngle;
	BOOL m_bRotate;

	float m_fAngleTotal;	// GDI����ת�Ƕ����ۼ�,��opengl��һ��
	CWceUiRotateBitmap m_rbmpDisk;

};
WCEUI_DYNCREATE_END(CAnimDiskLayer, CWceUiLayer);
