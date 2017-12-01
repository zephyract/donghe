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
	// ֮ǰ�Ķ�����û����
	while (m_bIsAnimating)
	{
		OnDrawBegin();
		OnDrawEnd();

		if (m_nCurrentFrame >= FRAME_ANMI_SHOWWINDOW)
		{
			break;	// ���������
		}
	}


	m_nCurrentFrame = 0;
	m_bAnimShow = bShow;
	m_nAnimMode = nAnimMode;
	m_dwAnimData = dwAnimData;

	if (nAnimMode == ANIM_MODE_NONE || nAnimMode == ANIM_MODE_OTHER)	// ���������浯�������������ʱҲ��ʱ������, ������治�붯�����ô�����
	{
		ShowLayer(bShow);
	}
	else
	{
		// �����Ҫ��ʾ���ڣ����ô˽ӿ�ʱ��Ҫ������ʾ��
		// ��������ش��ڣ� Ҫ�ȶ�����ɺ�������
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
		params[3] = offsetY - (float)nStep*(offsetY)/(float)(nTotal-1);	// opengl Y����������
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
		else	// ��������
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
	// ����������,���ƶ��˵Ŀؼ��ָ���ԭʼλ��,
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
		// ֻҪ�ǻ��ڶ���,�����ǺϷ���֡,���һ֡�п��ܻ����
		// �п��ܲ��Ƕ�����timer�������ػ�,���һ֡��,�п������������ִ������ػ�
		// ��ʱ���������һ֡�ٻ�һ��,�����п�������������ʾ������,�������˸
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

	// ResetTopLayers() �ĵ���λ�ã����������Щ�Ź֣���Ϊÿ���Ի���Ķ����Ƕ����ģ���OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)ʱ
	// ��ͬ��֡����(��ʱ��û�취��֤֡ͬ������û����������Ļ���)�������ڵ�һ�ζ���ǰ���ã���ÿ��
	// ����������Ϊ��һ�ζ�����ǰ������
	// �����ڶ���������������(UI���Ѿ���ʼondraw��)����Ϊ�пؼ����ƶ�����Ӱ��UI��ļ���
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
	const int TRANSPARENT_FRAMES = 4;	// ���Ч�ʣ�ָ������֡����һ��͸����
	int transparent = 255;
	if (nTotalFrame > TRANSPARENT_FRAMES)	// ��֡������̫��
	{
		if (bShow)
		{
			transparent = 96*(nCurFrame/TRANSPARENT_FRAMES)/((nTotalFrame-1)/TRANSPARENT_FRAMES);
			if (nCurFrame == nTotalFrame-1)	// ���һ֡
			{
				transparent = 255;
			}
		}
		else
		{
			if (nCurFrame == 0)
			{
				transparent = 255;	// ��һ֡����ԭ��������,���㶯����ɺ�ָ�ԭʼֵ
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

	// ��Щ�ؼ���������ֵ�ƶ����꣬Ϊ�˲�������ȡ����
	params[2] = (float)((int)params[2]);
}



void CAnimDlg::ResetTopLayers()
{
	float param[4];
	AnimGetParameters_uplevel(param, m_nCurrentFrame, FRAME_ANMI_SHOWWINDOW);

	// �ƶ�TOPBAR�ϵĿؼ������俴��������ʱû���ƶ�
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

	// �������ֵ�͸����
	const int OFFSET_X = WceUiGetScreenWidth() / 2;
	const int TRANSPARENT_FRAMES = 4;	// ���Ч�ʣ�ָ������֡����һ��͸����
	if (m_pLayerTop[TOP_LAYER_TITLE])
	{
		// ��ȡ����ÿ֡��͸����
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
		// ������֡����Ϊ���һ֡,����������������,��ͬ���̼��л�ʱ(GPS���л�),���Զ���
		if (m_nAnimMode == ANIM_MODE_UPLEVEL)
		{
			while (m_nCurrentFrame<FRAME_ANMI_SHOWWINDOW )	// �����������л�,�ѿؼ�λ�õ�������ȷλ��
			{
				ResetTopLayers();
				m_nCurrentFrame++;
			}
		}
		else	// ��������Ķ���û�пؼ��ƶ�,ֱ���������һ֡�Ϳ�����
		{
			m_nCurrentFrame = FRAME_ANMI_SHOWWINDOW;
		}

		// ��Ƶ��overlayҲҪ����,Ҫ����ס���������̻���
		// CPhotoPlayerDlg, CClockDlg�Ǵ�CDvpVideoBaseDlg�̳е�,������ʾ��overlay��
		if (IsKindOf(L"CDvpVideoBaseDlg") && !IsKindOf(L"CPhotoPlayerDlg") && !IsKindOf(L"CClockDlg"))
		{
			if (CMainWnd::GetInstance()->IsForegroundWindow())	// ������������жϵ�ǰ������ǰ̨,������Ϊ��active��
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
