#include "stdafx.h"
#include "AnimDlg.h"
#include "WceUiBase.h"
#include "tzUtil.h"
#include "MainDlg.h"
#include "uiddraw.h"
#include "MainWnd.h"

using namespace tzutil;

CAnimDlg::CAnimDlg(void)
{
	m_nCurrentFrame = 0;
	m_bIsAnimating = FALSE;
	m_bAnimShow = FALSE;
	::SetRectEmpty(&m_rcAnimRect);
	m_transparent = 1.0;

	for (int i=0; i<TOP_LAYER_COUNT; i++)
	{
		m_pLayerTop[i] = NULL;
	}
	::memset(m_rcLayerTop, 0, sizeof(m_rcLayerTop));
}

CAnimDlg::~CAnimDlg(void)
{

}


void CAnimDlg::OnInitDialog()
{
	__super::OnInitDialog();

	RegisterMsg(WM_ACTIVATE);


	LPCTSTR layer_name[TOP_LAYER_COUNT] = 
	{
		L"bk",
		L"top_bar",
		L"home",
		L"back",
		L"title"
	};
	for (int i=0; i<TOP_LAYER_COUNT; i++)
	{
		m_pLayerTop[i] = GetChildByName(layer_name[i]);
		if (m_pLayerTop[i])
		{
			m_pLayerTop[i]->GetWindowRect(&m_rcLayerTop[i]);
		}
	}
}

void CAnimDlg::AnimShowWindow(BOOL bShow, ANIM_MODE nAnimMode, DWORD dwAnimData)
{
	// 之前的动画还没做完
	while (m_bIsAnimating)
	{
		OnDrawBegin();
		OnDrawEnd();

		if (m_nCurrentFrame >= FRAME_ANMI_SHOWWINDOW)
		{
			break;	// 动画已完成
		}
	}


	m_nCurrentFrame = 0;
	m_bAnimShow = bShow;
	m_nAnimMode = nAnimMode;
	m_dwAnimData = dwAnimData;

	if (nAnimMode == ANIM_MODE_NONE || nAnimMode == ANIM_MODE_OTHER)	// 从其它界面弹出或回其它界面时也暂时不动画, 如果界面不想动画可用此特性
	{
		ShowLayer(bShow);
	}
	else
	{
		// 如果是要显示窗口，调用此接口时就要马上显示，
		// 如果是隐藏窗口， 要等动画完成后，再隐藏
		if (bShow)
		{
			ShowLayer(TRUE);
		}

		OnAnimateBegin();
		SetTimer(TIMER_ID_ANIM_SHOWWINDOW, SPEED_ANIM_SHOWWINDOW);
		InvalidateRect(NULL);

		::CopyRect(&m_rcAnimRect, (RECT*)CMainDlg::GetInstance()->GetAnimData((DLG_ID)dwAnimData));
		m_bIsAnimating = TRUE;
	} 
}

void CAnimDlg::DoAnimate()
{
	if (m_nAnimMode == ANIM_MODE_UPLEVEL)
	{
		float param[4];
		AnimGetParameters_uplevel(param, m_nCurrentFrame, FRAME_ANMI_SHOWWINDOW);
// 		wceui::wceglLoadIdentity();
// 		wceui::wceglTranslate(param[2], param[3], 0);
		m_nCurrentFrame++;
	}
	else
	{
		float param[4];
		AnimGetParameters(param, m_nCurrentFrame, FRAME_ANMI_SHOWWINDOW);
// 		wceui::wceglLoadIdentity();
// 		wceui::wceglScale(param[0], param[1], 1);
// 		wceui::wceglTranslate(param[2], param[3], 0);

		if (!m_bAnimShow && m_nCurrentFrame > FRAME_ANMI_SHOWWINDOW/2)
		{
//			wceui::wceglSetTransparent((float)(FRAME_ANMI_SHOWWINDOW-m_nCurrentFrame/2)/(float)FRAME_ANMI_SHOWWINDOW);
		}
		m_nCurrentFrame++;
	}


}

void CAnimDlg::AnimGetParameters(OUT float* params, int nStep, int nTotal)
{
	ASSERT(params);
	float zoomX = (float)_W(&m_rcAnimRect) / (float)WceUiGetScreenWidth();
	float zoomY = (float)_H(&m_rcAnimRect) / (float)WceUiGetScreenHeight();
	float offsetX = (float)WceUiGetScreenWidth()/2.0f - (float)(m_rcAnimRect.left+m_rcAnimRect.right)/2.0f;
	float offsetY = (float)WceUiGetScreenHeight()/2.0f - (float)(m_rcAnimRect.top+m_rcAnimRect.bottom)/2.0f;
	if (m_bAnimShow)
	{
		params[0] = zoomX + (float)nStep*(1.0f - zoomX)/(float)(nTotal-1);
		params[1] = zoomY + (float)nStep*(1.0f - zoomY)/(float)(nTotal-1);
		params[2] = -offsetX + (float)nStep*(offsetX)/(float)(nTotal-1);
		params[3] = offsetY - (float)nStep*(offsetY)/(float)(nTotal-1);	// opengl Y轴向上增长
	} 
	else
	{
		params[0] = 1 - (float)nStep*(1.0f - zoomX)/(float)(nTotal-1);
		params[1] = 1 - (float)nStep*(1.0f - zoomY)/(float)(nTotal-1);
		params[2] = (float)nStep*(-offsetX)/(float)(nTotal-1);
		params[3] = (float)nStep*(offsetY)/(float)(nTotal-1);
	}

}

void CAnimDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_ANIM_SHOWWINDOW)
	{
		if (m_nCurrentFrame < FRAME_ANMI_SHOWWINDOW)
		{
			OnAnimating();
			InvalidateRect(NULL);
		}
		else	// 动画结束
		{
			KillTimer(TIMER_ID_ANIM_SHOWWINDOW);

			if (!m_bAnimShow)
			{
				ShowLayer(FALSE);
			}
			m_bIsAnimating = FALSE;

			OnAnimateEnd();
		}
	}


	__super::OnTimer(wIDEvent);
}

void CAnimDlg::OnAnimateBegin()
{
	if (m_nAnimMode == ANIM_MODE_UPLEVEL)
	{
		ResetTopLayers();
	}
}

void CAnimDlg::OnAnimateEnd()
{
	// 动画结束后,将移动了的控件恢复到原始位置,
	if (m_nAnimMode == ANIM_MODE_UPLEVEL)
	{
		if (!m_bAnimShow)
		{
			m_nCurrentFrame = 0;
			ResetTopLayers();
		}
	}
}

void CAnimDlg::OnAnimating()
{

}

void CAnimDlg::OnDrawBegin()
{
	if (m_bIsAnimating)
	{
		// 只要是还在动画,必须是合法的帧,最后一帧有可能画多次
		// 有可能不是动画的timer在驱动重画,最后一帧后,有可能其它条件又触发了重画
		// 此时必须让最后一帧再画一次,否则有可能整个画面显示出来了,画面会闪烁
		if (m_nCurrentFrame >= FRAME_ANMI_SHOWWINDOW)
		{
			m_nCurrentFrame = FRAME_ANMI_SHOWWINDOW - 1;
		}
		//wceui::wceglPushMatrix();
		//m_transparent = wceui::wceglGetTransparent();
		DoAnimate();
	}

}

void CAnimDlg::OnDrawEnd()
{
	if (m_bIsAnimating)
	{
// 		wceui::wceglPopMatrix();
// 		wceui::wceglSetTransparent(m_transparent);
	}

	// ResetTopLayers() 的调用位置，这里设计有些古怪，因为每个对话框的动画是独立的，靠OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)时
	// 来同步帧数，(计时器没办法保证帧同步，又没有设计其它的机制)，所以在第一次动画前调用，或每次
	// 动画结束后为下一次动画提前做计算
	// 不能在动画过程中做计算(UI库已经开始ondraw了)，因为有控件的移动，会影响UI库的计算
	if (m_nCurrentFrame<FRAME_ANMI_SHOWWINDOW && m_nAnimMode==ANIM_MODE_UPLEVEL )
	{
		ResetTopLayers();
	}
}

BOOL CAnimDlg::IsAnimating()
{
	return m_bIsAnimating;
}

int CAnimDlg::GetCurrentFrame()
{
	return m_nCurrentFrame;
}

int CAnimDlg::CalcTextTransparent(BOOL bShow, int nCurFrame, int nTotalFrame)
{
	const int TRANSPARENT_FRAMES = 4;	// 提高效率，指定多少帧共用一个透明度
	int transparent = 255;
	if (nTotalFrame > TRANSPARENT_FRAMES)	// 总帧数不能太少
	{
		if (bShow)
		{
			transparent = 96*(nCurFrame/TRANSPARENT_FRAMES)/((nTotalFrame-1)/TRANSPARENT_FRAMES);
			if (nCurFrame == nTotalFrame-1)	// 最后一帧
			{
				transparent = 255;
			}
		}
		else
		{
			if (nCurFrame == 0)
			{
				transparent = 255;	// 第一帧保持原来的数据,方便动画完成后恢复原始值
			}
			else
			{
				transparent = 96-96*(nCurFrame/TRANSPARENT_FRAMES)/((nTotalFrame-1)/TRANSPARENT_FRAMES);
			}
		}
	}

	return transparent;
}


void CAnimDlg::AnimGetParameters_uplevel(OUT float* params, int nStep, int nTotal)
{
	ASSERT(params);
	float offsetX = (float)WceUiGetScreenWidth();
	//	float offsetY = (float)WceUiGetScreenHeight()/2.0f - (float)(m_rcAnimRect.top+m_rcAnimRect.bottom)/2.0f;
	if (m_bAnimShow)
	{
		if (m_dwAnimData == ANIM_LEVEL_CHILD || m_dwAnimData == ANIM_LEVEL_BROTHER)
		{
			params[2] = offsetX - (float)nStep*(offsetX)/(float)(nTotal-1);
		}
		else if (m_dwAnimData == ANIM_LEVEL_PARENT)
		{
			params[2] = -offsetX/4 + (float)nStep*(offsetX/4)/(float)(nTotal-1);
		}
		else
		{
			;	// impossible
		}
	} 
	else
	{
		if (m_dwAnimData == ANIM_LEVEL_CHILD)
		{
			params[2] = (float)nStep*(offsetX)/(float)(nTotal-1);
		}
		else if (m_dwAnimData == ANIM_LEVEL_PARENT)
		{
			params[2] = (float)nStep*(-offsetX/4)/(float)(nTotal-1);
		}
		else	// ANIM_LEVEL_BROTHER
		{
			params[2] = (float)nStep*(-offsetX)/(float)(nTotal-1);
		}


	}

	// 有些控件需根据这个值移动坐标，为了不产生误差，取整数
	params[2] = (float)((int)params[2]);
}



void CAnimDlg::ResetTopLayers()
{
	float param[4];
	AnimGetParameters_uplevel(param, m_nCurrentFrame, FRAME_ANMI_SHOWWINDOW);

	// 移动TOPBAR上的控件，让其看起来动画时没有移动
	// 	TOP_LAYER_BK, TOP_LAYER_TOPBAR,	TOP_LAYER_HOME,	TOP_LAYER_BACK,
	for (int i=0; i<=TOP_LAYER_BACK; i++)
	{
		if (m_pLayerTop[i])
		{
			RECT rc;
			::CopyRect(&rc, &m_rcLayerTop[i]);
			::OffsetRect(&rc, (int)-param[2], 0);
			m_pLayerTop[i]->MoveLayer(&rc);
		}
	}

	// 设置文字的透明度
	const int OFFSET_X = WceUiGetScreenWidth() / 2;
	const int TRANSPARENT_FRAMES = 4;	// 提高效率，指定多少帧共用一个透明度
	if (m_pLayerTop[TOP_LAYER_TITLE])
	{
		// 获取文字每帧的透明度
		int transparent = CalcTextTransparent(m_bAnimShow, m_nCurrentFrame, FRAME_ANMI_SHOWWINDOW);
		if (m_bAnimShow)
		{
			if (m_dwAnimData == ANIM_LEVEL_CHILD)
			{
				RECT rc;
				::CopyRect(&rc, &m_rcLayerTop[TOP_LAYER_TITLE]);
				::OffsetRect(&rc,  -OFFSET_X+OFFSET_X*m_nCurrentFrame/(FRAME_ANMI_SHOWWINDOW-1), 0);
				m_pLayerTop[TOP_LAYER_TITLE]->MoveLayer(&rc);
			}
//			m_pLayerTop[TOP_LAYER_TITLE]->GetStyle()->SetTextTransparent(transparent);
		}
		else
		{
			if (m_dwAnimData == ANIM_LEVEL_CHILD)
			{
				RECT rc;
				::CopyRect(&rc, &m_rcLayerTop[TOP_LAYER_TITLE]);
				::OffsetRect(&rc,  -OFFSET_X*m_nCurrentFrame/(FRAME_ANMI_SHOWWINDOW-1), 0);
				m_pLayerTop[TOP_LAYER_TITLE]->MoveLayer(&rc);
			}
//			m_pLayerTop[TOP_LAYER_TITLE]->GetStyle()->SetTextTransparent(transparent);
		}
	}
}

LRESULT CAnimDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_ACTIVATE)
	{
		OnActiveateWnd(LOWORD(wParam));

		RETAILMSG(1, (L"WM_ACTIVATE +++++++++++++    wParam = %d \n", LOWORD(wParam)));
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CAnimDlg::OnActiveateWnd(BOOL bActive)
{
	if (IsLayerVisible())
	{
		// 将动画帧数设为最后一帧,让其跳过动画过程,不同进程间切换时(GPS间切换),忽略动画
		if (m_nAnimMode == ANIM_MODE_UPLEVEL)
		{
			while (m_nCurrentFrame<FRAME_ANMI_SHOWWINDOW )	// 动画过程中切换,把控件位置调整到正确位置
			{
				ResetTopLayers();
				m_nCurrentFrame++;
			}
		}
		else	// 与主界面的动画没有控件移动,直接跳到最后一帧就可以了
		{
			m_nCurrentFrame = FRAME_ANMI_SHOWWINDOW;
		}

		// 视频及overlay也要控制,要不挡住了其它进程画面
		// CPhotoPlayerDlg, CClockDlg是从CDvpVideoBaseDlg继承的,但不显示在overlay上
		if (IsKindOf(L"CDvpVideoBaseDlg") && !IsKindOf(L"CPhotoPlayerDlg") && !IsKindOf(L"CClockDlg"))
		{
			if (CMainWnd::GetInstance()->IsForegroundWindow())	// 如果复合条件判断当前窗口在前台,还是认为是active的
			{
				bActive = TRUE;
			}
			CUiDDraw::GetInstance()->UpdateOverlay(bActive, 180);
			ShowVideo(bActive);
		}
	}
}


void CAnimDlg::ShowVideo(BOOL bShow, RECT* prect)
{

}
