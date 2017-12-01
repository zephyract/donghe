#include "StdAfx.h"
#include "DVDStartDlg.h"
#include "WceUiButton.h"
#include "WceUiRealWnd.h"
#include <atlbase.h>
#include "wceuilayerctrl.h"
#include "dvdrender.h"
#include "SourceManager.h"
#include "uiddraw.h"
#include "MainWnd.h"
#include "interface.h"
#include "sysUtil.h"
#include "tzUtil.h"
#include "config.h"

CDVDStartDlg::CDVDStartDlg(void)
: m_fAngle(0)
, m_fScale(1.0f)
, m_bMoveIn(FALSE)
, m_bZoomOut(FALSE)
, m_bSpeedAcc(FALSE)
, m_bRotateLaser(FALSE)
, m_nRotateLaserCount(0)
, m_nCurAngle(0)
, m_nAngleOnce(0)
, m_nAngleTotal(0)
{
	CDvdRender::GetInstance()->RegisterEventHandler(CDVDStartDlg::DvpAgentEventListener, (DWORD)this);
}

CDVDStartDlg::~CDVDStartDlg(void)
{
	CDvdRender::GetInstance()->UnRegisterEventHandler(CDVDStartDlg::DvpAgentEventListener);
}

void CDVDStartDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_DVD_ANIM_LOADING);
	RegisterMsg(NAVI_MSG_LAN_CHANGE);

	__super::OnInitDialog();

	m_pDiskLayer = (CAnimDiskLayer*)GetChildByName(L"disk");
	m_pDiskLayer->GetWindowRect(m_rcOrginal);
	m_pLaserLayer = (CAnimDiskLayer*)GetChildByName(L"laser");

	AnimRotateLaser(-6, -30);

}


void CDVDStartDlg::OnBnClick(PCWceUiButton pButton)
{
	
	__super::OnBnClick(pButton);
}


LRESULT CDVDStartDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_DVD_ANIM_LOADING)
	{
		// 取消缺省旋转laser动作
		AnimRotateLaser(6, 0);
		KillTimer(TIMER_ID_ROTATE_LASER);

		// 将控件移到屏幕外
		CRect rc;
		m_pDiskLayer->GetWindowRect(rc);
		rc.OffsetRect(-rc.right, 0);
		m_pDiskLayer->MoveLayer(rc);

		// 静态的可以直接隐藏,
		CWceUiLayer* p3 = GetChildByName(L"disk_static");
		ASSERT(p3);
		if (p3)
		{
			p3->ShowLayer(FALSE);
		}


		SetTimer(TIMER_ID_MOVE_IN, DISK_ANIM_TIME);
		m_bMoveIn = TRUE;	// 启动计时器了,就认为状态为movein了

		// 放大光盘
		m_pDiskLayer->Scale(DISK_MAX_SCALE);
	}
	else if (message == NAVI_MSG_LAN_CHANGE)
	{
		InitCodepage();
	}
	else
	{
		return __super::OnMessage(message, wParam, lParam);
	}

	return 0L;
}

void CDVDStartDlg::ShowStatus(int nStatus)
{
	CWceUiLayer *pstatus = GetChildByName(L"title");
	if (!pstatus)
	{
		return;
	}

	switch (nStatus)
	{
	case SV_LOADING_DISC:
		pstatus->SetTextResID(L"DVD_LOADING");
		ui_led_set_showdata(L"LOADI NG ");
		break;
	case SV_TRAY_OPENING:
		pstatus->SetTextResID(L"DVD_EJECTING");
		ui_led_set_showdata(L" EJEC T  ");
		break;
	case SV_TRAY_CLOSING:
		break;
	case SV_TRAY_ERROR:
		break;
	case SV_NO_DISC:
	case SV_UNKNOWN_DISC:
		pstatus->SetTextResID(L"DVD_DISK_ERROR");
		SetTimer(TIMER_ID_AUTO_EXIT_DVD_SRC, 1000);
		break;
	case SV_DISC_IDENTIFIED:
		break;
	case SV_DISC_INITED:
		break;
	case SV_EMD_LOADING:
		break;
	case SV_NO_USB:
		break;
	default:
		break;
	}
}

void CDVDStartDlg::MyShowDlg(DLG_ID did)
{
	// 如果加载窗口可见才显示播放界面,否则在后台播放
	if (IsLayerVisible())
	{
		CDlgManager::GetInstance()->ShowDlg(did, SOURCE_DVD);
	}
	else
	{
		CDlgManager::GetInstance()->SetActiveDlg(did, SOURCE_DVD);
		// 如果last dlg刚好是该窗口,需主动更新last dlg信息，从其它窗口返回时,应返回这里设置的活动窗口
		if (CDlgManager::GetInstance()->GetLastDlgID() == CDVDStartDlg_ID)
		{
			DLG_INFO* pdi = CDlgManager::GetInstance()->GetDlgInfo(did, SOURCE_DVD);
			CDlgManager::GetInstance()->SetLastDlgInfo(pdi);
		}

	}
}

void CDVDStartDlg::LoadUI(UINT nDiskType)
{
	switch (nDiskType)
	{
	case SV_DVD_VIDEO:
	case SV_DVD_AUDIO:
	case SV_MINI_DVD:
		MyShowDlg(CDVDPlayerDlg_ID);
		break;
	case SV_CDDA:
		MyShowDlg(CCDFileListDlg_ID);
		break;
	case SV_DATA:
	case SV_HDCD:
	case SV_DTS_CS:
	case SV_SACD:
		MyShowDlg(CDVDFileListDlg_ID);
		break;
	case SV_SVCD:
	case SV_CVD:
	case SV_VCD3_0:
	case SV_VCD2_0:
	case SV_VCD1_1:
	case SV_VCD6_0:
	case SV_VCD6_1:
		MyShowDlg(CVCDPlayerDlg_ID);
		break;
	case SV_CDG:
		MyShowDlg(CCDGPlayerDlg_ID);
		break;
	default:
		break;
	}
}


void CDVDStartDlg::OnTimer(WORD wIDEvent)
{
	if (!IsAnimating())		// 如果窗口在作动画，这些动画效果暂停
	{
		if (wIDEvent == TIMER_ID_ROTATE_SPEED_ACC)
		{
			OnTimerSpeedAcc();
		}
		else if (wIDEvent == TIMER_ID_ROTATE_SPEED_DEC)
		{
			OnTimerSpeedDec();
		}
		else if (wIDEvent == TIMER_ID_MOVE_OUT)
		{
			OnTimerMoveOut();
		}
		else if (wIDEvent == TIMER_ID_MOVE_IN)
		{
			OnTimerMoveIn();
		}
		else if (wIDEvent == TIMER_ID_ZOOM_OUT)
		{
			OnTimerZoomOut();
		}
		else if (wIDEvent == TIMER_ID_ZOOM_IN)
		{
			OnTimerZoomIn();
		}
		else if (wIDEvent == TIMER_ID_ROTATE_LASER)
		{
			OnTimerRotateLaser();
		}
	}

	if (wIDEvent == TIMER_ID_DISK_EJECT)
	{
		KillTimer(TIMER_ID_DISK_EJECT);
		CRpcMCU::GetInstance()->RPC_KeyCommand(K_EJECT);
		CMainWnd::GetInstance()->PostMessage(UI_MSG_SOURCE_EXIT, 0, (LPARAM)SOURCE_DVD);
	}
	else if (wIDEvent == TIMER_ID_AUTO_EXIT_DVD_SRC)
	{
		KillTimer(TIMER_ID_AUTO_EXIT_DVD_SRC);
		CMainWnd::GetInstance()->PostMessage(UI_MSG_SOURCE_EXIT, 0, (LPARAM)SOURCE_DVD);
	}

	__super::OnTimer(wIDEvent);

}

void CDVDStartDlg::OnTimerRotateLaser()
{
	m_bRotateLaser = TRUE;

	if (m_nRotateLaserCount > 0)
	{
		m_nCurAngle += m_nAngleOnce;
		m_nRotateLaserCount--;
		m_pLaserLayer->Rotate();
		m_pLaserLayer->InvalidateRect(NULL);
	}
	else
	{
		KillTimer(TIMER_ID_ROTATE_LASER);
		m_bRotateLaser = FALSE;

		if (m_nCurAngle < 0)	// 旋转了,应该是启动碟
		{
			SetTimer(TIMER_ID_ROTATE_SPEED_ACC, 300);
			m_fAngle = 0.0f;
			m_pDiskLayer->StartRotate();
		}
		else	// 停止碟
		{
			// 碟片停止后,放大碟片
			SetTimer(TIMER_ID_ZOOM_IN, DISK_ANIM_TIME);
			m_fScale = 1.0f;
		}
	}
}

void CDVDStartDlg::OnTimerSpeedAcc()
{
	m_bSpeedAcc = TRUE;

	if (m_fAngle <= -50.0f)
	{
		KillTimer(TIMER_ID_ROTATE_SPEED_ACC);
		m_bSpeedAcc = FALSE;
	}
	else
	{
		CAnimDiskLayer* player = (CAnimDiskLayer*)GetChildByName(L"disk");
		m_fAngle -= 10.0f;
		player->SetRotateAngle(m_fAngle);
	}
}

void CDVDStartDlg::OnTimerSpeedDec()
{
	if (m_fAngle >= 0)
	{
		KillTimer(TIMER_ID_ROTATE_SPEED_DEC);

		m_pDiskLayer->StopRotate();
		AnimRotateLaser(6, 0);
// 		m_pDiskLayer->StopRotate();
// 		// 碟片停止后,放大碟片
// 		SetTimer(TIMER_ID_ZOOM_IN, DISK_ANIM_TIME);
// 		m_fScale = 1.0f;
	}
	else
	{
		CAnimDiskLayer* player = (CAnimDiskLayer*)GetChildByName(L"disk");
		if (m_fAngle < -40.0f)
		{
			m_fAngle += 2.5f;
		}
		else if (m_fAngle < -10.0f)
		{
			m_fAngle += 10.0f;
		}
		else
		{
			m_fAngle += 2.5f;
		}
		
		player->SetRotateAngle(m_fAngle);
	}
}

void CDVDStartDlg::OnTimerMoveIn()
{

	CRect rc;
	int offset = 20;
	m_pDiskLayer->GetWindowRect(rc);
	if (m_rcOrginal.left - rc.left < 20)
	{
		offset = m_rcOrginal.left - rc.left;
		KillTimer(TIMER_ID_MOVE_IN);
		m_bMoveIn = FALSE;

		// move in完成后,缩小碟片
		SetTimer(TIMER_ID_ZOOM_OUT, DISK_ANIM_TIME);
		m_fScale = DISK_MAX_SCALE;
	}
	rc.OffsetRect(offset, 0);
	m_pDiskLayer->MoveLayer(rc);
	m_pDiskLayer->InvalidateRect(NULL);
}

void CDVDStartDlg::OnTimerMoveOut()
{
	CRect rc;
	m_pDiskLayer->GetWindowRect(rc);
	rc.OffsetRect(-20, 0);
	m_pDiskLayer->MoveLayer(rc);

	if (rc.right <= 0)
	{
		KillTimer(TIMER_ID_MOVE_OUT);

		SetTimer(TIMER_ID_DISK_EJECT, 100);	// 动画结束后,基本可以出碟了
	}
	m_pDiskLayer->InvalidateRect(NULL);
}

void CDVDStartDlg::OnTimerZoomOut()
{
	m_bZoomOut = TRUE;

	if (m_fScale > 1.0f)
	{
		m_fScale -= 0.05f;
	}

	if (fabs(m_fScale - 1.0f) < 0.0001 )		// 动画完毕
	{
		KillTimer(TIMER_ID_ZOOM_OUT);
		m_fScale = 1.0f;
		m_bZoomOut = FALSE;

		// zoom out完成后,移动laser到读碟处
		AnimRotateLaser(-6, -30);
// 		SetTimer(TIMER_ID_ROTATE_SPEED_ACC, 300);
// 		m_fAngle = 0.0f;
// 		m_pDiskLayer->StartRotate();
	}

	m_pDiskLayer->Scale(m_fScale);
}


void CDVDStartDlg::OnTimerZoomIn()
{
	if (m_fScale < DISK_MAX_SCALE)
	{
		m_fScale += 0.05f;
	}

	if (fabs(m_fScale - DISK_MAX_SCALE) < 0.0001 )		// 动画完毕
	{
		KillTimer(TIMER_ID_ZOOM_IN);
		m_fScale = DISK_MAX_SCALE;

		// 放大碟片完成后,移出碟片
		SetTimer(TIMER_ID_MOVE_OUT, DISK_ANIM_TIME);
	}

	m_pDiskLayer->Scale(m_fScale);
}


void CDVDStartDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	switch (media_event)
	{
	case EVT_DVP_DISC_INSERT:
		break;
		// 	case EVT_DVP_DISC_ID:
		// 		{
		// 			GUINT8 uData;UINT32 u4Len = 64;
		// 			DVP_GetDiscID(&uData,u4Len);//u4Len>=8
		// 			RETAILMSG(TRUE, (L"[DVP] EVT_DVP_DISC_ID ******************%d \n",uData));	
		// 		}		
		// 		break;
	case EVT_DVP_INIT_STATE:
		RETAILMSG(TRUE, (L"[DVP] EVT_DVP_INIT_STATE ******************%d \n",param1));	
		if (param1 == SV_TRAY_OPENING)
		{
			param1 = SV_UNKNOWN_DISC;	// 不可能出现该事件,如果出现该事件,认为是碟片错误
		}
		ShowStatus(param1);	
		break;
	case EVT_DVP_DISC_TYPE:
		if (param1 >= 0 && param1 < SV_UNKNOWN)		// must be a valid disk type
		{
			LoadUI(param1);
			InitCodepage();		// 根据当前语言设置DVD模块的缺省语言,ID3信息获取时可能需要
		}
		break;

	case EVT_DVP_PBC_PLAY_STATE:
		// 可能已经被禁止视频了
		//		OnShowVideo(TRUE);
		break;
	default:
		break;
	}
}

void CDVDStartDlg::InitCodepage()
{
	int lan_id = sysutil::nss_get_instance()->ui_lan_id;
	LANGUAGE_TABLE_INFO* pinfo = config::get_language_info(lan_id);
	if (pinfo)
	{
		DVP_SetCodePage(pinfo->code_page);
	}
}

void CDVDStartDlg::DvpAgentEventListener(UINT u4Evt, UINT u4Param1, UINT u4Param2, DWORD u4CustomData)
{
	CDVDStartDlg* pthis = (CDVDStartDlg*)u4CustomData;
	pthis->OnMediaEvent(u4Evt, u4Param1, u4Param2);
}

void CDVDStartDlg::OnDlgShow(BOOL bShow)
{
	if (!bShow)
	{
		CDvdRender::GetInstance()->ShowVideo(FALSE, NULL);
	}

	__super::OnDlgShow(bShow);
}


void CDVDStartDlg::OnEject()
{
	ShowStatus(SV_TRAY_OPENING);

	CDlgManager::GetInstance()->ShowDlg(CDVDStartDlg_ID, SOURCE_DVD);
	CDvdRender::GetInstance()->StopSrc();
	CDvdRender::GetInstance()->Eject();

	// 出碟动画
	if (m_bMoveIn)
	{
		m_bMoveIn = FALSE;
		KillTimer(TIMER_ID_MOVE_IN);
		SetTimer(TIMER_ID_MOVE_OUT, DISK_ANIM_TIME);
	}
	else if (m_bZoomOut)
	{
		m_bZoomOut = FALSE;
		KillTimer(TIMER_ID_ZOOM_OUT);
		SetTimer(TIMER_ID_ZOOM_IN, DISK_ANIM_TIME);
	}
	else if (m_bRotateLaser)
	{
		m_bRotateLaser = FALSE;
		AnimRotateLaser(6, 0);
	}
	else
	{
		KillTimer(TIMER_ID_ROTATE_SPEED_ACC);

		SetTimer(TIMER_ID_ROTATE_SPEED_DEC, 50);
		//m_fAngle = -50.0f;	// 进源时,这个值应该是合适的值了
		m_pDiskLayer->SetRotateAngle(m_fAngle);
		m_pDiskLayer->StartRotate();
	}
}

void CDVDStartDlg::OnTrayIn()
{

}


void CDVDStartDlg::AnimRotateLaser(int nAngleOnce, int nAngleTotal)
{
	m_nAngleOnce = nAngleOnce;
	m_nAngleTotal = (nAngleTotal - m_nCurAngle);	// 有可能已经旋转了一个角度

	m_pLaserLayer->SetRotateAngle((float)nAngleOnce);

	m_nRotateLaserCount = m_nAngleTotal / nAngleOnce;	
	SetTimer(TIMER_ID_ROTATE_LASER, 100);
}

void CDVDStartDlg::AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData)
{
	// CDVDStartDlg <-> CDVDPlayerDlg  不作动画
	if (nAnimMode == ANIM_MODE_UPLEVEL)
	{
		ShowLayer(bShow);
	}
	else
	{
		__super::AnimShowWindow(bShow, nAnimMode, dwAnimData);
	}
}

void CDVDStartDlg::OnAnimateBegin()
{
	__super::OnAnimateBegin();
}

void CDVDStartDlg::OnAnimateEnd()
{
	__super::OnAnimateEnd();
}

//////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
CAnimDiskLayer::CAnimDiskLayer()
: m_fAngle(0)
, m_fAngleTotal(0)
, m_fScale(1.0f)
, m_bRotate(FALSE)
{

}

CAnimDiskLayer::~CAnimDiskLayer()
{

}

void CAnimDiskLayer::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);
	if (m_bRotate)
	{
		Rotate();
	}

	CRect rc;
	GetWindowRect(rc);
	int x = rc.left + (rc.Width()-m_rbmpDisk.GetBitmap()->GetWidth())/2;
	int y = rc.top + (rc.Height()-m_rbmpDisk.GetBitmap()->GetHeight())/2;
	
	BLENDFUNCTION bf;
	memset(&bf, 0, sizeof(BLENDFUNCTION));
	bf.AlphaFormat = AC_SRC_ALPHA;

	pWndBitmap->AlphaBlend(x, y, 
		m_rbmpDisk.GetBitmap()->GetWidth(), m_rbmpDisk.GetBitmap()->GetHeight(),
		m_rbmpDisk.GetBitmap(), 0, 0, bf);
}

void CAnimDiskLayer::Rotate()
{
	m_fAngleTotal += m_fAngle;
	m_rbmpDisk.Rotate(m_fAngleTotal, m_rbmpDisk.GetWidth()/2, m_rbmpDisk.GetHeight()/2, m_fScale, m_fScale);
}


void CAnimDiskLayer::Scale(float scale)
{
	m_fScale = scale;
	Rotate();
	InvalidateRect(NULL);
}


void CAnimDiskLayer::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_ANIM_DISK_LAYER)
	{
		if (IsLayerVisible())
		{
			InvalidateRect(NULL);
		}
	}
	else
	{
		__super::OnTimer(wIDEvent);
	}
}

void CAnimDiskLayer::SetProperties(PCWceUiXmlElem pCreateElem)
{
	LPCTSTR attr = pCreateElem->GetAttr(L"bitmap");
	if (attr)
	{
		CString str;
		str.Format(L"%s\\%s\\res\\bitmap\\%s", tzutil::GetAppPath(), ui_get_res_file(), attr);
		m_rbmpDisk.LoadFile(str);
	}

	__super::SetProperties(pCreateElem);
}

void CAnimDiskLayer::OnInitLayer()
{
	__super::OnInitLayer();
}


void CAnimDiskLayer::SetRotateAngle(float fAngle)
{
	m_fAngle = fAngle;
}

void CAnimDiskLayer::StartRotate()
{
	SetTimer(TIMER_ID_ANIM_DISK_LAYER, 20);
	m_bRotate = TRUE;
}

void CAnimDiskLayer::StopRotate()
{
	KillTimer(TIMER_ID_ANIM_DISK_LAYER);
	m_bRotate = FALSE;
}

void CAnimDiskLayer::Reset()
{
}