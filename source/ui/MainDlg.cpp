#include "stdafx.h"
#include "MainDlg.h"
#include "WceUiBase.h"
#include "wceUiPageLayer.h"
#include "tzUtil.h"
#include "MainWnd.h"
#include "interface.h"
#include "sysUtil.h"
#include "WceUiLoadString.h"
#include "SourceManager.h"
#include "msdkFileManager.h"
#include "WceUiLayerCtrl.h"
#include "msdkrender.h"
#include "bluetooth.h"
#include "DvpBaseDlg.h"
#include "OverlayDlg.h"

#include "WceUi32Bitmap.h"
#include "uiddraw.h"
#include "ycapi.h"
#include "config.h"

CMainDlg* CMainDlg::m_pthis = NULL;
BOOL CMainDlg::m_bBottomShow = FALSE;

using namespace tzutil;

CMainDlg::CMainDlg(void)
{
	CMainDlg::m_pthis = this;
	m_pAnimLayer = NULL;
}

CMainDlg::~CMainDlg(void)
{
	CMainDlg::m_pthis = NULL;
}

void CMainDlg::OnInitDialog()
{
	RegisterMsg(UI_MSG_BT_CONNECT_STATUS_CHANGE);
	RegisterMsg(UI_MSG_FACTORY_SET_CHANGE);
	RegisterMsg(UI_MSG_DVD_EJECT_NOTIFY);
	RegisterMsg(UI_MSG_STORAGE_CARD_PLUG);

	InitButtonsRect();
	RelayoutLayers();

	m_pDate = GetChildByName(L"Date");
	m_pTime = GetChildByName(L"Time");

	UpdateDateTime();
	SetTimer(TIMER_ID_REFRESH_TIME, 1000);
	CheckPage(0);

	// 为了提供启动速度,main需要时才创建,如果是后启动的,移动到最前面,
	CWceUiLayer *player = GetWindow()->GetLayerCtrl()->RootLayer()->FirstChild();
	if (player && player != this)
	{
		MoveBefore(player);
	}

	// 创建时蓝牙可能已经连上了,蓝牙状态标识也要刷新, 但前提是BT已经初始化了
	if (CBluetooth::GetInstance()->IsInitialized())
	{
		OnConnectEvent();
	}

	// dvd status
	OnDvdStatusChange();
	OnMassStoragePlug();

	ShowBottom(m_bBottomShow);

	__super::OnInitDialog();
}

CMainDlg* CMainDlg::GetInstance()
{
	if (m_pthis == NULL)
	{
		CDlgManager::GetInstance()->CreateDlg(CMainDlg_ID, SOURCE_MAIN);
	}

	return m_pthis;
}

void CMainDlg::InitButtonsRect()
{
	int total_buttons = 0;	// 总共按钮个数
	CWceUiPageLayer* pframe = (CWceUiPageLayer*)GetChildByName(L"frame");
	if (pframe)
	{
		CWceUiButton* pInd = (CWceUiButton*)pframe->FirstChild();
		while (pInd)
		{
			WCEUI_POSITION_WAY way = pInd->GetStyle()->GetPosition(&m_rcButtons[total_buttons]);// 得到坐标
			total_buttons++;
			pInd = (CWceUiButton*)pframe->NextChild(pInd);
		}
	}
}

void CMainDlg::RelayoutLayers()
{
	CWceUiPageLayer* pframe = (CWceUiPageLayer*)GetChildByName(L"frame");
	if (!pframe)
	{
		return;
	}

	// 因为重新初始化了子控件的位置,frame必须也处于原始位置
	pframe->SetScrollPos(0);
	CheckPage(0);

	int total_buttons_visible = 0;	// 总共可见的按钮个数

	// 遍历每个按钮,根据配置设置每个按钮的属性(位置及是否可见)
	CWceUiButton* pInd = (CWceUiButton*)pframe->FirstChild();
	while (pInd)
	{
		if (config::get_config_functions()->is_support(pInd->GetName()))
		{
			pInd->ShowLayer(TRUE);
			pInd->GetStyle()->SetPosition(m_rcButtons+total_buttons_visible, WCEUI_RELATIVE_POSITION);
			pInd->GetCommonStyle()->SetPosition(m_rcButtons+total_buttons_visible, WCEUI_RELATIVE_POSITION);
			pInd->GetCommonStyle()->SetUsePosition(TRUE);

			total_buttons_visible++;
		}
		else
		{
			pInd->ShowLayer(FALSE);
		}

		pInd = (CWceUiButton*)pframe->NextChild(pInd);
	}

	// 根据页数显示对应的页切换按钮
	int pages = (total_buttons_visible+MAIN_BUTTONS_PER_PAGE-1)/MAIN_BUTTONS_PER_PAGE;

	// 根据页数设置滚动范围
	if (pframe)
	{
		if (pages > 1)
		{
			pframe->SetExtraScroll(30);		// 如果只有一页，不允许滚动
		}
		pframe->SetScrollRange(0, (total_buttons_visible-1)/MAIN_BUTTONS_PER_PAGE*800);
	}

	for (int i=0; i<MAIN_MAX_BUTTONS/MAIN_BUTTONS_PER_PAGE; i++)
	{
		TCHAR layer_name[16];
		_stprintf(layer_name, L"page%d", i);
		CWceUiLayer* player = GetChildByName(layer_name);
		if (player)
		{
			player->ShowLayer((i<pages && pages>1) ? TRUE : FALSE);		// 如果只有一页不用显示这个
		}
	}
   
   for (int i=0;i<pages;i++)
   {
	   TCHAR layer_name[16];
	   CRect rc;
	   _stprintf(layer_name, L"page%d",i);
	   CWceUiLayer* player = GetChildByName(layer_name);
	   player->GetWindowRect(rc);
	   static int nWidth = rc.Width();
	   const int SPACE_W = 8;

	   if (pages%2)
	   {
		   rc.left = WceUiGetScreenWidth()/2 - nWidth/2 - (nWidth + SPACE_W) *(pages/2) + (nWidth + SPACE_W) * i;
	   }
	   else
	   {
		  rc.left = WceUiGetScreenWidth()/2 - SPACE_W/2 - (nWidth * (pages/2) + SPACE_W * (pages/2 -1))  + (nWidth + SPACE_W) * i;
	   }
       rc.right =  rc.left + nWidth;
	   
	   player->GetStyle()->SetPosition(rc, WCEUI_ABSOLUTE_POSITION);
	   player->GetCommonStyle()->SetPosition(rc, WCEUI_ABSOLUTE_POSITION);
	   player->GetCommonStyle()->SetUsePosition(TRUE);
   }


   // 这套UI，AVIN，AUX放在下面一排了frame_bottom,所以这里也处理下功能配置项,主要是avin1，avin2
	CWceUiLayer *pavin1 = GetLayerByName(L"avin1");
	CWceUiLayer *pavin2 = GetLayerByName(L"avin2");
   if (pavin1!=NULL) 
   {
	   pavin1->ShowLayer(config::get_config_functions()->is_support(pavin1->GetName()) ? TRUE : FALSE);
   }

   if (pavin2!=NULL) 
   {
	   pavin2->ShowLayer(config::get_config_functions()->is_support(pavin2->GetName()) ? TRUE : FALSE);
   }

}

#include "QTelzone.h"
#include "winhook.h"

#define MM_WAVE_FILE_PLAYER_PLAY_SOUND (WM_USER + 0xF1)
void CMainDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"setting"))
	{
		CDlgManager::GetInstance()->GotoSource(SOURCE_SETTING);
	}
	if (pButton->IsEqualName(L"navigation"))
	{
		CMainWnd::GetInstance()->LoadNavigation();
	}
	else if (pButton->IsEqualName(L"fm"))
	{
		CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_FM);
	}
	else if (pButton->IsEqualName(L"vdisk"))
	{
		CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_VDISK);
	}
	else if (pButton->IsEqualName(L"sd"))
	{
		if(sysutil::nss_get_instance()->is_source_available(SOURCE_SD))
		{
			CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_SD);
		}
		else if (!config::get_config_functions()->is_support(SOURCE_DVD))
		{
			; // 如果不带DVD功能,不作任何提示
		}
		else
		{
			ShowMessageTip(L"MIS_SD_NOT_EXIST");
		}
	}
	else if (pButton->IsEqualName(L"usb"))
	{
		if(sysutil::nss_get_instance()->is_source_available(SOURCE_USB))
		{
			CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_USB);
		}
		else
		{
			ShowMessageTip(L"MIS_USB_NOT_EXIST");
		}
	}
	else if (pButton->IsEqualName(L"usb_rear"))
	{
		if(sysutil::nss_get_instance()->is_source_available(SOURCE_USB_REAR))
		{
			CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_USB_REAR);
		}
		else
		{
			ShowMessageTip(L"MIS_USB_NOT_EXIST");
		}
	}
	else if (pButton->IsEqualName(L"tv"))
	{
		CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_TV);
	}
	else if (pButton->IsEqualName(L"bt"))
	{
		//CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_BT);
		CDlgManager::GetInstance()->ShowDlg(CBluetoothDlg_ID, SOURCE_BT);
	}
	else if (pButton->IsEqualName(L"avin1"))
	{
		CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_AVIN1);
	}
	else if (pButton->IsEqualName(L"avin2"))
	{
		CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_AVIN2);
	}
	else if (pButton->IsEqualName(L"dvd"))
	{
		if(sysutil::nss_get_instance()->is_source_available(SOURCE_DVD))
		{
			CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_DVD);
		}
		else
		{
			ShowMessageTip(L"MIS_DISK_NOT_EXIST");
		}
	}
	else if (pButton->IsEqualName(L"ipod"))
	{
		if(sysutil::nss_get_instance()->is_source_available(SOURCE_IPOD_USB))
		{
			CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_IPOD_USB);
		}
		else
		{
			ShowMessageTip(L"MIS_IPOD_NOT_EXIST");
		}
	}
	else if (pButton->IsEqualName(L"phonelink"))
	{
		if(sysutil::nss_get_instance()->is_source_available(SOURCE_PHONE_LINK))
		{
			// 由phonelinkstardlg自己加载源,因为做了USB端口拉低动作
			CDlgManager::GetInstance()->GotoSource(SOURCE_PHONE_LINK);
		}
		else
		{
			ShowMessageTip(L"MIS_PHONELINK_NOT_EXIST");
		}
	}
	else if (pButton->IsEqualName(L"easyconnected"))
	{
		if(sysutil::nss_get_instance()->is_source_available(SOURCE_EASY_CONNECTED))
		{
			CMainWnd::GetInstance()->LoadUiFrontSource(SOURCE_EASY_CONNECTED);
		}
		else
		{
			ShowMessageTip(L"MIS_PHONELINK_NOT_EXIST");
		}
	}
	else if (pButton->IsEqualName(L"link"))
	{
 		CDlgManager::GetInstance()->DeleteDlg(CLinkDlg_ID, SOURCE_LINK);
 		CDlgManager::GetInstance()->ShowDlg(CLinkDlg_ID, SOURCE_LINK);
	}
	else if (pButton->IsEqualName(L"gobang"))
	{
		CDlgManager::GetInstance()->DeleteDlg(CGoBangDlg_ID, SOURCE_GOBANG);
		CDlgManager::GetInstance()->ShowDlg(CGoBangDlg_ID, SOURCE_GOBANG);
	}
	else if (pButton->IsEqualName(L"othello"))
	{
		CDlgManager::GetInstance()->DeleteDlg(COthelloDlg_ID, SOURCE_OTHELLO);
		CDlgManager::GetInstance()->ShowDlg(COthelloDlg_ID, SOURCE_OTHELLO);
	}
	else if (pButton->IsEqualName(L"boxman"))
	{
		CDlgManager::GetInstance()->DeleteDlg(CBoxManDlg_ID, SOURCE_BOXMAN);
		CDlgManager::GetInstance()->ShowDlg(CBoxManDlg_ID, SOURCE_BOXMAN);
	}
	else if (pButton->IsEqualName(L"chinachess"))
	{
		CDlgManager::GetInstance()->DeleteDlg(CChinaChessDlg_ID, SOURCE_CHINACHESS);
		CDlgManager::GetInstance()->ShowDlg(CChinaChessDlg_ID, SOURCE_CHINACHESS);
	}
	else if (pButton->IsEqualName(L"russian"))
	{
		CDlgManager::GetInstance()->DeleteDlg(CRussianDlg_ID, SOURCE_RUSSIAN);
		CDlgManager::GetInstance()->ShowDlg(CRussianDlg_ID, SOURCE_RUSSIAN);
	}
	else if (pButton->IsEqualName(L"calendar"))
	{
		CDlgManager::GetInstance()->ShowDlg(CCalendarDlg_ID, SOURCE_CALENDAR);
	}
	else if (pButton->IsEqualName(L"calculator"))
	{
		CDlgManager::GetInstance()->ShowDlg(CCalculatorDlg_ID, SOURCE_CALCULATOR);
	}
	else if (pButton->IsEqualName(L"clock"))
	{
		CDlgManager::GetInstance()->ShowDlg(CClockDlg_ID, SOURCE_CLOCK);
	}
	else if (pButton->IsEqualName(L"telzone"))
	{
		CDlgManager::GetInstance()->ShowDlg(CQTelzone_ID, SOURCE_QTELZONE);
	}
	else if (pButton->IsEqualName(L"ecar"))
	{
		OnEcarClick();
	}
	else if (pButton->IsEqualName(L"carinfo"))
	{
		CDlgManager::GetInstance()->ShowDlg(CCarInfoDlg_ID, SOURCE_CAR_INFO);
	}
	else if (pButton->IsEqualName(L"backlight"))
	{
		ui_close_backlight();
	}
	else if (pButton->IsEqualName(L"page0"))
	{
		OnPageClick(0);
	}
	else if (pButton->IsEqualName(L"page1"))
	{
		OnPageClick(1);
	}
	else if (pButton->IsEqualName(L"page2"))
	{
		OnPageClick(2);
	}
	else if (pButton->IsEqualName(L"page3"))
	{
		OnPageClick(3);
	}
	else if (pButton->IsEqualName(L"page_prev"))
	{
		OnPagePrevClick();
	}
	else if (pButton->IsEqualName(L"page_next"))
	{
		OnPageNextClick();
	}
	else if (pButton->IsEqualName(L"EQ"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetAudioDlg_ID, SOURCE_EQ);
	}
	else if (pButton->IsEqualName(L"hide_bottom"))
	{
		ShowBottom(FALSE);
	}
	else if (pButton->IsEqualName(L"show_bottom"))
	{
		ShowBottom(TRUE);
	}
}

void CMainDlg::ShowBottom(BOOL bShow)
{
	CWceUiLayer* pShow = GetChildByName(L"show_bottom");
	CWceUiLayer* pHide = GetChildByName(L"frame_bottom");

	m_bBottomShow = bShow;
	if (pShow && pHide)
	{
// 		pShow->ShowLayer(!bShow);
// 		pHide->ShowLayer(bShow);
		pShow->ShowLayer(FALSE);
		pHide->ShowLayer(FALSE);
	}
}

void CMainDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_REFRESH_TIME)
	{
		UpdateDateTime();
	}
	else if (wIDEvent == TIMER_ID_HIDE_MESSAGE_TIP)
	{
		KillTimer(TIMER_ID_HIDE_MESSAGE_TIP);
		CWceUiLayer* player = GetLayerByName(L"message_tip");
		if (player)
		{
			player->ShowLayer(FALSE);

			CWceUiLayer* player = GetLayerByName(L"digital_clock");
			if (player)
			{
				player->ShowLayer(TRUE);
			}
		}
	}

	__super::OnTimer(wIDEvent);
}

void CMainDlg::UpdateDateTime()
{
	TCHAR szDate[32];
	TCHAR szTime[32];
	ui_get_datetime_string(szDate, szTime);
	if (m_pDate)
	{
		m_pDate->SetText(szDate);
	}
	if (m_pTime)
	{
		m_pTime->SetText(szTime);
	}
}

void CMainDlg::DoAnimate()
{
	m_nCurrentFrame++;
}

void CMainDlg::OnAnimateEnd()
{
	if (m_pAnimLayer)
	{
// 		m_pAnimLayer->GetStyle()->SetTextTransparent(255);
	}
}

void CMainDlg::AnimGetParameters(OUT float* params, int nStep, int nTotal)
{
	ASSERT(params);
	float zoomX = (float)WceUiGetScreenWidth() / (float)_W(&m_rcAnimRect);
	float zoomY = (float)WceUiGetScreenHeight() / (float)_H(&m_rcAnimRect);
	float offsetX = (float)WceUiGetScreenWidth()/2.0f - (float)(m_rcAnimRect.left+m_rcAnimRect.right)/2.0f;
		  offsetX *= zoomX;
	float offsetY = (float)WceUiGetScreenHeight()/2.0f - (float)(m_rcAnimRect.top+m_rcAnimRect.bottom)/2.0f;
	      offsetY *= zoomY;
	if (m_bAnimShow)
	{
		params[0] = zoomX + (float)nStep*(1.0f - zoomX)/(float)(nTotal-1);
		params[1] = zoomY + (float)nStep*(1.0f - zoomY)/(float)(nTotal-1);
		params[2] = offsetX - (float)nStep*(offsetX)/(float)(nTotal-1);
		params[3] = -offsetY + (float)nStep*(offsetY)/(float)(nTotal-1);	// opengl Y轴向上增长
	} 
	else
	{
		params[0] = 1 + (float)nStep*(zoomX - 1.0f)/(float)(nTotal-1);
		params[1] = 1 + (float)nStep*(zoomY - 1.0f)/(float)(nTotal-1);
		params[2] = (float)nStep*(offsetX)/(float)(nTotal-1);
		params[3] = (float)nStep*(-offsetY)/(float)(nTotal-1);
	}

}


DWORD CMainDlg::GetAnimData(DLG_ID did)
{
	struct LAYER_TABLE
	{
		DLG_ID dlg_id;
		TCHAR  layer_name[MAX_TAG_NAME_LEN];
	};
	
	LAYER_TABLE layer_table[] = 
	{
		{CSettingDlg_ID,	L"setting"	},
		{CRadioDlg_ID,		L"fm"		},
		{CVDiskListDlg_ID,	L"vdisk"	},
		{CBluetoothDlg_ID,	L"bt"		},
		{CBTCallDlg_ID,		L"bt"		},
		{CAvin1Dlg_ID,		L"avin1"	},
		{CAvin2Dlg_ID,		L"avin2"	},
		{CMMFileListDlg_ID, L"sd"		},
		{CDVDStartDlg_ID,   L"dvd"		},
		{CUIPodFileListDlg_ID, L"ipod"	},

		{CCalendarDlg_ID, L"calendar"	},
		{CCalculatorDlg_ID, L"calculator"	},
		{CQTelzone_ID, L"telzone"	},

		
		{CLinkDlg_ID, L"link"	},
		{CGoBangDlg_ID, L"gobang"	},
		{COthelloDlg_ID, L"othello"	},
		{CBoxManDlg_ID, L"boxman"	},
		{CChinaChessDlg_ID, L"chinachess"	},
		{CRussianDlg_ID, L"russian"	},

		{CATVDlg_ID, L"tv"	},
		{CClockDlg_ID, L"clock"	},
		{CCarInfoDlg_ID, L"carinfo"	},
		{CPhonelinkStartDlg_ID, L"phonelink"},
		{EasyConnected_ID, L"easyconnected"},
	};

	// 有可能上一个动画没完成，又要做下一动画,导致透明度没设回原始值,这里主动设置下可保证不会停留在中间状态
	if (m_pAnimLayer)
	{
//		m_pAnimLayer->GetStyle()->SetTextTransparent(255);
	}


	static RECT rc;
	m_pAnimLayer = NULL;
	for (int i=0; i<sizeof(layer_table)/sizeof(layer_table[0]); i++)
	{
		if (did == layer_table[i].dlg_id)
		{
			m_pAnimLayer = GetLayerByName(layer_table[i].layer_name);
		}
	}

	// CMMFileListDlg_ID给SD,USB,USB_REAR共用，这里需区分开来
	if (did == CMMFileListDlg_ID)
	{
		if (CSourceManager::GetInstance()->GetFrontSrcID()==SOURCE_USB 
			|| CSourceManager::GetInstance()->GetRearSrcID()==SOURCE_USB)
		{
			m_pAnimLayer = GetLayerByName(L"usb");
		}
		else if (CSourceManager::GetInstance()->GetFrontSrcID()==SOURCE_USB_REAR 
			|| CSourceManager::GetInstance()->GetRearSrcID()==SOURCE_USB_REAR)
		{
			m_pAnimLayer = GetLayerByName(L"usb_rear");
		}
	}

	if (m_pAnimLayer)
	{
		m_pAnimLayer->GetWindowRect(&rc);
		RECT rc_screen = {0, 0, WceUiGetScreenWidth(), WceUiGetScreenHeight()};
		RECT rc_interset;
		::IntersectRect(&rc_interset, &rc, &rc_screen);
		if (!::IsRectEmpty(&rc_interset))	// 必须位于屏幕内,主界面有几页,有可能当前的图标被翻到其它页了
		{
			rc.bottom = rc.top+130;
			return (DWORD)&rc;
		}
	}
	

	// 未找到对应的layer,或layer处于屏幕外,返回中心小矩形区
	int cx = WceUiGetScreenWidth() / 2;
	int cy = WceUiGetScreenHeight() / 2;
	::SetRect(&rc, cx-20, cy-20, cx+20, cy+20);

	return (DWORD)&rc;
}


LRESULT CMainDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_BT_CONNECT_STATUS_CHANGE)
	{
		OnConnectEvent();
	}
	else if (message == UI_MSG_FACTORY_SET_CHANGE)
	{
		RelayoutLayers();
	}
	else if (message == UI_MSG_DVD_EJECT_NOTIFY)
	{
		OnDvdStatusChange();
	}
	else if (message == UI_MSG_STORAGE_CARD_PLUG)
	{
		OnMassStoragePlug();
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CMainDlg::OnMassStoragePlug()
{
	CWceUiLayer *player = GetLayerByName(L"sd_status");
	if (player)
	{
		player->SetStyle(sysutil::nss_get_instance()->is_source_available(SOURCE_SD) ? L"sd_in" : L"sd_out");
	}

	player = GetLayerByName(L"usb_status");
	if (player)
	{
		BOOL b = sysutil::nss_get_instance()->is_source_available(SOURCE_USB)
			|| sysutil::nss_get_instance()->is_source_available(SOURCE_USB_REAR);
		player->SetStyle(b ? L"usb_in" : L"usb_out");
	}
}

void CMainDlg::OnDvdStatusChange()
{
	CWceUiLayer *player = GetLayerByName(L"dvd_status");
	if (player)
	{
		if ( config::get_config_functions()->is_support(SOURCE_DVD) )	
		{
			if (protocol::get_mcu_status()->is_DISK_exist())
			{
				player->SetStyle(L"dvd_in");
			}
			else
			{
				player->SetStyle(L"dvd_out");
			}
		}
		else
		{
			player->ShowLayer(FALSE);
		}
	}
}


LRESULT CMainDlg::OnCommand(UINT uCommand, WPARAM wParam, LPARAM lParam)
{
	if (uCommand == WCEUI_CMD_PAGELAYER_CHANGE)
	{
		CheckPage((int)lParam);
	}

	return __super::OnCommand(uCommand, wParam, lParam);
}

void CMainDlg::CheckPage(int nPage)
{
	if (nPage<0 || nPage>3)	// 指定的页超过了支持的范围
	{
		return;
	}

	CWceUiButton* pPage0 = (CWceUiButton*)GetChildByName(L"page0");
	CWceUiButton* pPage1 = (CWceUiButton*)GetChildByName(L"page1");
	CWceUiButton* pPage2 = (CWceUiButton*)GetChildByName(L"page2");
	CWceUiButton* pPage3 = (CWceUiButton*)GetChildByName(L"page3");


	if(pPage0) pPage0->SetStatusStyleName(L"page0_normal", CWceUiButton::STATUS_NORMAL);
	if(pPage1) pPage1->SetStatusStyleName(L"page1_normal", CWceUiButton::STATUS_NORMAL);
	if(pPage2) pPage2->SetStatusStyleName(L"page2_normal", CWceUiButton::STATUS_NORMAL);
	if(pPage3) pPage3->SetStatusStyleName(L"page3_normal", CWceUiButton::STATUS_NORMAL);
	
	switch (nPage)
	{
	case 0:
		if(pPage0) pPage0->SetStatusStyleName(L"page0_down", CWceUiButton::STATUS_NORMAL);
		break;
	case 1:
		if(pPage1) pPage1->SetStatusStyleName(L"page1_down", CWceUiButton::STATUS_NORMAL);
		break;
	case 2:
		if(pPage2) pPage2->SetStatusStyleName(L"page2_down", CWceUiButton::STATUS_NORMAL);
		break;
	case 3:
		if(pPage3) pPage3->SetStatusStyleName(L"page3_down", CWceUiButton::STATUS_NORMAL);
		break;
	default:
		break;
	}
}

void CMainDlg::OnPagePrevClick()
{
	CWceUiPageLayer* pPage = (CWceUiPageLayer*)GetChildByName(L"frame");
	if (pPage)
	{
		int nPage = pPage->GetCurPage();
		nPage--;
		OnPageClick(nPage);
	}
}


void CMainDlg::OnPageNextClick()
{
	CWceUiPageLayer* pPage = (CWceUiPageLayer*)GetChildByName(L"frame");
	if (pPage)
	{
		int nPage = pPage->GetCurPage();
		nPage++;
		OnPageClick(nPage);
	}
}

void CMainDlg::OnPageClick(int nPage)
{
	CWceUiPageLayer* pPage = (CWceUiPageLayer*)GetChildByName(L"frame");
	if (pPage)
	{
		pPage->SetToPage(nPage, TRUE);
	}

	CheckPage(nPage);
}


void CMainDlg::OnConnectEvent()
{
	CWceUiLayer* player = GetChildByName(L"bt_status");
	if (player)
	{
		player->SetStyle(CBluetooth::GetInstance()->IsConnected() ? L"bt_connect" : L"bt_disconnect");
	}
}

void CMainDlg::OnEcarClick()
{
	if (CBluetooth::GetInstance()->IsConnected())
	{
		CBluetooth::GetInstance()->Dial(L"4008005005");
	}
	else
	{
		ShowMessageTip(L"MIS_BT_NOT_CONNECTED");
	}
}


void CMainDlg::ShowMessageTip(LPCTSTR tip)
{
// 	CWceUiLayer* player = GetLayerByName(L"message_tip");
// 	if (player && tip)
// 	{
// 		CWceUiLoadString str(tip);
// 		player->ShowLayer(TRUE);
// 		player->SetText(str.GetString());
// 
// 		CWceUiLayer* player = GetLayerByName(L"digital_clock");
// 		if (player)
// 		{
// 			player->ShowLayer(FALSE);
// 		}
// 
// 		SetTimer(TIMER_ID_HIDE_MESSAGE_TIP, 2000);
// 
// 	}

	// 显示在overlay上 
	COverlayDlg::GetInstance()->MessageTip_ShowUI(tip);
}