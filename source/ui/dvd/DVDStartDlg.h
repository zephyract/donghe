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
	
	void OnEject();		// 出碟
	void OnTrayIn();	// 进碟
protected:
	static void DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData);
	void InitCodepage();

	// 通知动画显示开始
	virtual void OnAnimateBegin();
	// 通知动画显示结束
	virtual void OnAnimateEnd();

	// 碟片动画
	void OnTimerMoveIn();	// 从屏幕外移动到屏幕内
	void OnTimerMoveOut();	// 从内移到外
	void OnTimerZoomIn();	// 放大
	void OnTimerZoomOut();	// 缩小
	void OnTimerSpeedAcc();
	void OnTimerSpeedDec();
	void OnTimerRotateLaser();

	float m_fScale;	// 缩放系数
	float m_fAngle;
	CRect m_rcOrginal;		// 碟片原始位置
	CAnimDiskLayer* m_pDiskLayer;

	// 进碟时会有这四个动作,某个动作过程中，有可能出碟,这里出碟动作只需执行部分
	BOOL m_bMoveIn;
	BOOL m_bZoomOut;
	BOOL m_bRotateLaser;
	BOOL m_bSpeedAcc;


	// laser转动数据
	int m_nRotateLaserCount;	// 保存需转动的次数，确保转动指定的角度
	int m_nCurAngle;			// 当前处于的角度
	int m_nAngleOnce;			// 每次转动的角度,控制速度
	int m_nAngleTotal;			// 需要转动到哪个角度
	CAnimDiskLayer* m_pLaserLayer;
	// 参数指定一次旋转的角度和最终的角度
	void AnimRotateLaser(int nAngleOnce, int nAngleTotal);	// 旋转到多少角度,如果之前有旋转,需计算还需旋转多少角度

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
const float DISK_MAX_SCALE  = 1.2f;	// 最大放大系数

class CAnimDiskLayer : public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CAnimDiskLayer, CWceUiLayer)
public:
	CAnimDiskLayer();
	~CAnimDiskLayer();

	virtual void OnTimer(WORD wIDEvent);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnInitLayer();

	// 角度越大，转的越快
	void SetRotateAngle(float fAngle);
	void StartRotate();
	void StopRotate();
	void Scale(float scale);
	void Reset();	// 回到原始状态

	void Rotate();

protected:
	virtual void OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw);

	float m_fScale;
	float m_fAngle;
	BOOL m_bRotate;

	float m_fAngleTotal;	// GDI的旋转角度需累加,与opengl不一样
	CWceUiRotateBitmap m_rbmpDisk;

};
WCEUI_DYNCREATE_END(CAnimDiskLayer, CWceUiLayer);
