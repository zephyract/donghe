#include "stdafx.h"
#include "CameraDlg.h"
#include "WceUiBase.h"
#include "DlgManager.h"
#include "MainWnd.h"
#include "msdkrender.h"
#include "uiddraw.h"
#include "SourceManager.h"
#include "sysUtil.h"
#include "tzUtil.h"
#include "SetVideoParams.h"
#include "CanBus.h"
#include "interface.h"
#include "config.h"

CCameraDlg::CCameraDlg(void)
: m_bIsSiganlReady(FALSE)
{
	CMsdkRender::GetInstance()->RegisterEventHandlerAvin(MediaEventHandler, (DWORD)this, MSDK_AVIN_CAMERA);
}

CCameraDlg::~CCameraDlg(void)
{
	CMsdkRender::GetInstance()->UnRegisterEventHandlerAvin(MediaEventHandler);

}

void CCameraDlg::MediaEventHandler(UINT media_event, UINT param1, UINT param2, DWORD user_data)
{
	CCameraDlg* pthis = (CCameraDlg*)user_data;
	if (pthis)
	{
		pthis->OnMediaEvent(media_event, param1, param2);
	}
}


void CCameraDlg::OnMediaEvent(UINT media_event, UINT param1, UINT param2)
{
	RECT rc;
	::SetRect(&rc, 10, 4, 714, 576);		// 720X576
	switch(media_event)
	{
	case AVIN_SIGNAL_CHANGE:
	case AVIN_SIGNAL_READY:
		m_bIsSiganlReady = TRUE;	
		KillTimer(TIMER_ID_SHOW_SIGNAL_FLAG);
		ShowNoSignalFlag();	
		CMsdkRender::GetInstance()->SetVideoScaleCamera(&rc);
		UpdateVideoParams();
		break;
	case AVIN_SIGNAL_LOST:
		m_bIsSiganlReady = FALSE;
		SetTimer(TIMER_ID_SHOW_SIGNAL_FLAG, 1000);
		UpdateVideoParams();
		break;
	default:
		break;
	}
}

void CCameraDlg::OnInitLayer()
{
	__super::OnInitLayer();
	if (WceUiGetBitsPixel() == 16)
	{
		m_pBmpBk.SetBitmapFromRes(L"media\\camera\\track_bk.565a.conf");
	}
	else
	{
		m_pBmpBk.SetBitmapFromRes(L"media\\camera\\track_bk.32a.conf");
	}

	WCHAR szPath[MAX_PATH] = {0};
	for (int i= 0;i< 8;i++)
	{
		for (int j= 0;j<= 10;j++)
		{
			if (WceUiGetBitsPixel() == 16)
			{
				swprintf(szPath, L"media\\camera\\radar\\%c%d.565a.conf",65+i,j); //65为A的ASCII码十进制值
			}
			else
			{
				swprintf(szPath, L"media\\camera\\radar\\%c%d.32a.conf",65+i,j); //65为A的ASCII码十进制值
			}

			m_pBmpRadar[i][j].SetBitmapFromRes(szPath);
		}	
	}
	if (WceUiGetBitsPixel() == 16)
	{
		m_pBmpCar.SetBitmapFromRes(L"media\\camera\\radar\\car.565a.conf");
	}
	else
	{
		m_pBmpCar.SetBitmapFromRes(L"media\\camera\\radar\\car.32a.conf");
	}

}

void CCameraDlg::OnInitDialog()
{
	__super::OnInitDialog();
	RegisterMsg(UI_MSG_MCU_CANBUS_DATA);
}

void CCameraDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_CHECK_SIGNAL)
	{
		KillTimer(TIMER_ID_CHECK_SIGNAL);
		m_bIsSiganlReady = CMsdkRender::GetInstance()->AIsSignalReadyCamera();
		SetTimer(TIMER_ID_SHOW_SIGNAL_FLAG, 1000);		
	}
	else if (wIDEvent == TIMER_ID_SHOW_SIGNAL_FLAG)
	{
		KillTimer(TIMER_ID_SHOW_SIGNAL_FLAG);	
		ShowNoSignalFlag();
	}

	__super::OnTimer(wIDEvent);
}




void CCameraDlg::OnDraw(CWceUiGenericBitmap* pWndBitmap, RECT rcNeedDraw)
{
	__super::OnDraw(pWndBitmap, rcNeedDraw);

	// 0表示不带CAN,不带CAN时不画轨迹及雷达
	if (config::get_config_canbus()->canbus_type != 0)
	{
	// 倒车轨迹
	DrawBackCarLine(pWndBitmap);
    // 雷达
	DrawRadar(pWndBitmap);
	}
}

LRESULT CCameraDlg::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == UI_MSG_MCU_CANBUS_DATA)
	{
		switch (lParam)
		{
		case CBDT_SWC_ANGLE:		
		case CBDT_FRONT_RADAR:
		case CBDT_REAR_RADAR:
			InvalidateRect();
			GetWindow()->UpdateWindow();
			break;
		}
	}

	return __super::OnMessage(message, wParam, lParam);
}

void CCameraDlg::OnDlgShow(BOOL bShow)
{
	CUiDDraw::GetInstance()->UpdateOverlay(bShow, 180);

	if (bShow)
	{
		// 如果导航在前台运行
		if (sysutil::nss_get_instance()->sys_gps_foreground)
		{
			::SetForegroundWindow(GetWindow()->m_hWnd);
		}

		SetTimer(TIMER_ID_CHECK_SIGNAL, 500);

	}
	else
	{
// 		if (ui_is_on_bt_call())
// 		{
// 			;	// 如果还在蓝牙通话,通话结束后会进入正确的状态,这里不用任何操作
// 		}
// 		else if (sysutil::nss_get_instance()->sys_ecar_status)
// 		{
// 			;   // 翼卡跟BT的处理一样
// 		}
// 		else
// 		{
// 			// 如果导航在前台运行
// 			if (sysutil::nss_get_instance()->sys_gps_foreground)
// 			{
// 				//tzutil::SetProcessForeground(sysutil::nss_get_instance()->gps_process_id);
// 				ui_load_navigation();
// 				Sleep(1000);
// 			}
// 		}
	}
	UpdateVideoParams();
}

void CCameraDlg::OnBnClick(CWceUiButton* pButton)
{
	__super::OnBnClick(pButton);
}

void CCameraDlg::ShowNoSignalFlag()
{
	CWceUiLayer *player = GetChildByName(L"nosignal");
	if(NULL != player)
	{
		player->ShowLayer(!m_bIsSiganlReady);
	}
}

void CCameraDlg::UpdateVideoParams()
{
	if (IsLayerVisible() 
		&& CMainWnd::GetInstance()->IsForegroundWindow()
		&& CMsdkRender::GetInstance()->AIsSignalReadyCamera())

	{
		CSetVideoParams::setBrightness((UINT*)&sysutil::nss_get_instance()->video_camera_brightness);
		CSetVideoParams::setContrast((UINT*)&sysutil::nss_get_instance()->video_camera_contrast);
		CSetVideoParams::setHue((UINT*)&sysutil::nss_get_instance()->video_camera_hue);
		CSetVideoParams::setSaturation((UINT*)&sysutil::nss_get_instance()->video_camera_saturation);
	}
	else
	{
		CSetVideoParams::setBrightness((UINT*)&sysutil::nss_get_instance()->video_brightness);
		CSetVideoParams::setContrast((UINT*)&sysutil::nss_get_instance()->video_contrast);
		CSetVideoParams::setHue((UINT*)&sysutil::nss_get_instance()->video_hue);
		CSetVideoParams::setSaturation((UINT*)&sysutil::nss_get_instance()->video_saturation);
	}
}

void CCameraDlg::DrawBmp(CWceUiGenericBitmap* pWndBitmap,int x,int y,CWceUiGenericBitmap* pSrcBitmap)
{	
	if (pSrcBitmap)
	{
		BLENDFUNCTION bf;
		memset(&bf, 0, sizeof(BLENDFUNCTION));
		bf.AlphaFormat = AC_SRC_ALPHA;
		CRect rc(x, y, x + pSrcBitmap->GetWidth(), y + pSrcBitmap->GetHeight());
		pWndBitmap->AlphaBlend(rc.left,rc.top,rc.Width(),rc.Height(),pSrcBitmap,0,0,bf);
	}
}

BOOL DrawBackCarTrack(HDC hDC,UINT8 u1Angle, BOOL bLeft);
void CCameraDlg::DrawBackCarLine(CWceUiGenericBitmap* pWndBitmap)
{	
	// 背景
	DrawBmp(pWndBitmap,0,0,m_pBmpBk.GetBitmap());

	CDC dc;
	dc.CreateCompatibleDC(NULL);
	LPBYTE pbb;
	HBITMAP hbmp = pWndBitmap->GetHBitmap(pbb);
	dc.SelectObject(hbmp);
	dc.SetBkMode(TRANSPARENT);

	INT nAngle = CANBUS_RAISE_DASAUTO::get_instance()->get_swc_angle();
	if (nAngle >= 0)
	{
		DrawBackCarTrack(dc.GetSafeHdc(),abs(nAngle), TRUE);
	} 
	else
	{
		DrawBackCarTrack(dc.GetSafeHdc(),abs(nAngle), FALSE);
	}
	dc.DeleteDC();
}

void CCameraDlg::DrawRadar(CWceUiGenericBitmap* pWndBitmap)
{	
	DrawBmp(pWndBitmap,701,96,m_pBmpCar.GetBitmap());

	//雷达信息 范围 0x00~0x0A
	int nIndex = CANBUS_RAISE_DASAUTO::get_instance()->get_radar_front_L();
	DrawBmp(pWndBitmap,658,44,m_pBmpRadar[0][nIndex].GetBitmap());
	nIndex = CANBUS_RAISE_DASAUTO::get_instance()->get_radar_front_L_C();
	DrawBmp(pWndBitmap,690,38,m_pBmpRadar[1][nIndex].GetBitmap());
	nIndex = CANBUS_RAISE_DASAUTO::get_instance()->get_radar_front_R_C();
	DrawBmp(pWndBitmap,727,38,m_pBmpRadar[2][nIndex].GetBitmap());
	nIndex = CANBUS_RAISE_DASAUTO::get_instance()->get_radar_front_R();   
	DrawBmp(pWndBitmap,744,44,m_pBmpRadar[3][nIndex].GetBitmap());

	nIndex = CANBUS_RAISE_DASAUTO::get_instance()->get_radar_rear_L();   
    DrawBmp(pWndBitmap,658,201,m_pBmpRadar[4][nIndex].GetBitmap());
	nIndex = CANBUS_RAISE_DASAUTO::get_instance()->get_radar_rear_L_C(); 
	DrawBmp(pWndBitmap,690,210,m_pBmpRadar[5][nIndex].GetBitmap());
	nIndex = CANBUS_RAISE_DASAUTO::get_instance()->get_radar_rear_R_C();
	DrawBmp(pWndBitmap,727,210,m_pBmpRadar[6][nIndex].GetBitmap());
	nIndex = CANBUS_RAISE_DASAUTO::get_instance()->get_radar_rear_R();
	DrawBmp(pWndBitmap,744,201,m_pBmpRadar[7][nIndex].GetBitmap());
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 track                                                          //
////////////////////////////////////////////////////////////////////////////////////////////////////

POINT L01_A[] = {{65,348},{311,79}};
POINT L01_B[] = {{722,352},{630,245},{608,220},{550,155},{482,83}};
POINT L02_A[] = {{65,348},{85,325},{118,287},{163,238},{193,205},{222,173},{248,144},{269,120},{292,93},{302,81}};
POINT L02_B[] = {{722,352},{708,335},{685,308},{659,278},{636,252},{607,220},{590,201},{582,193},{563,173},{527,137},{513,123},{502,113},{488,100},{470,83}};
POINT L03_A[] = {{65,347},{95,312},{115,288},{140,260},{168,228},{189,205},{206,186},{221,169},{235,154},{249,137},{265,118},{281,98},{293,82}};
POINT L03_B[] = {{721,351},{705,332},{687,309},{666,285},{648,264},{632,247},{616,230},{599,212},{577,190},{553,166},{536,151},{517,134},{494,113},{471,94},{456,82}};
POINT L04_A[] = {{65,347},{84,324},{103,300},{123,277},{142,255},{156,239},{173,219},{194,195},{214,172},{232,151},{256,122},{277,94},{284,84}};
POINT L04_B[] = {{721,351},{703,329},{684,306},{662,281},{644,261},{626,241},{604,219},{585,200},{564,180},{542,160},{520,141},{498,123},{471,102},{443,82}};
POINT L05_A[] = {{64,347},{80,327},{92,311},{110,289},{127,269},{147,245},{165,224},{186,199},{202,181},{219,161},{234,143},{250,122},{266,99},{275,85}};
POINT L05_B[] = {{721,351},{702,328},{688,311},{673,293},{659,277},{644,261},{628,244},{607,223},{590,206},{574,191},{553,172},{531,154},{519,144},{501,130},{476,112},{439,88},{429,81}};
POINT L06_A[] = {{63,347},{81,325},{96,303},{117,277},{131,260},{150,237},{172,211},{194,185},{216,159},{236,133},{255,105},{266,86}};
POINT L06_B[] = {{721,352},{699,324},{682,304},{663,282},{642,259},{621,237},{598,214},{574,192},{544,167},{515,144},{487,124},{454,103},{415,81}};
POINT L07_A[] = {{63,347},{76,329},{90,309},{108,286},{128,260},{150,234},{170,209},{188,187},{206,165},{236,125},{257,87}};
POINT L07_B[] = {{721,352},{703,329},{686,308},{667,286},{646,263},{618,234},{590,208},{559,181},{527,155},{488,128},{448,104},{422,90},{403,81}};
POINT L08_A[] = {{62,348},{81,320},{99,295},{121,266},{146,235},{169,206},{193,177},{213,150},{233,120},{243,101},{248,88}};
POINT L08_B[] = {{720,351},{693,317},{665,284},{643,260},{624,240},{605,222},{580,199},{552,176},{522,153},{489,131},{445,106},{407,88},{389,80}};
POINT L09_A[] = {{62,347},{77,324},{97,295},{117,268},{138,241},{159,214},{180,188},{204,157},{222,130},{233,108},{239,90}};
POINT L09_B[] = {{720,351},{696,321},{674,294},{650,267},{623,239},{590,208},{551,176},{520,153},{486,131},{446,109},{403,90},{375,80}};
POINT L10_A[] = {{61,348},{80,318},{99,290},{117,265},{136,240},{155,215},{176,189},{197,161},{214,135},{225,113},{230,91}};
POINT L10_B[] = {{720,352},{698,323},{675,295},{649,266},{619,235},{588,207},{553,178},{521,155},{483,131},{441,109},{363,80}};
POINT L11_A[] = {{61,347},{81,315},{102,283},{125,251},{149,219},{175,185},{202,147},{217,120},{221,106},{221,92}};
POINT L11_B[] = {{720,352},{699,324},{673,293},{646,263},{616,232},{589,208},{561,185},{531,162},{505,145},{474,127},{446,113},{413,99},{384,89},{349,79}};
POINT L12_A[] = {{60,348},{79,316},{97,288},{121,254},{144,223},{169,189},{196,150},{209,125},{213,112},{214,104},{213,93}};
POINT L12_B[] = {{720,352},{697,322},{676,296},{648,265},{621,237},{588,207},{552,178},{523,157},{490,137},{454,118},{415,101},{374,88},{335,79}};
POINT L13_A[] = {{60,347},{80,313},{98,284},{119,253},{141,222},{164,191},{186,159},{202,131},{206,119},{208,108},{204,96}};
POINT L13_B[] = {{720,352},{699,324},{679,300},{656,273},{629,245},{600,217},{573,194},{541,170},{508,148},{471,127},{438,111},{403,98},{362,86},{322,78}};
POINT L14_A[] = {{59,348},{78,314},{95,286},{117,253},{142,217},{167,182},{189,148},{196,134},{201,120},{201,109},{195,97}};
POINT L14_B[] = {{720,352},{691,314},{663,281},{636,252},{608,224},{578,198},{541,170},{504,145},{457,120},{413,102},{371,89},{331,81},{309,78}};
POINT L15_A[] = {{59,347},{77,314},{98,279},{120,245},{144,210},{167,177},{188,143},{192,134},{195,124},{196,116},{193,107},{186,98}};
POINT L15_B[] = {{720,352},{688,311},{662,280},{635,250},{607,223},{571,192},{529,161},{487,136},{445,115},{404,99},{355,86},{295,77}};
POINT L16_A[] = {{58,349},{75,315},{90,290},{105,265},{125,235},{175,160},{186,139},{190,126},{191,119},{185,106},{178,100}};
POINT L16_B[] = {{719,352},{699,325},{675,295},{665,283},{647,263},{632,248},{609,225},{588,207},{564,187},{542,171},{512,151},{475,130},{455,120},{420,105},{372,90},{347,85},{318,80},{282,77}};
POINT L17_A[] = {{58,348},{76,312},{100,270},{124,232},{147,198},{170,163},{184,134},{186,120},{184,114},{178,106},{169,101}};
POINT L17_B[] = {{719,352},{694,318},{667,285},{638,253},{609,225},{579,198},{542,170},{504,145},{467,125},{421,105},{378,91},{333,82},{268,77}};
POINT L18_A[] = {{57,348},{76,311},{97,273},{119,237},{145,196},{168,160},{178,140},{182,124},{178,112},{171,107},{161,102}};
POINT L18_B[] = {{718,351},{672,291},{644,259},{614,229},{580,199},{544,171},{503,144},{423,105},{349,84},{255,76}};
POINT L19_A[] = {{57,347},{69,322},{90,283},{113,243},{149,186},{166,158},{173,144},{178,127},{174,115},{165,108},{152,103}};
POINT L19_B[] = {{718,351},{680,301},{628,242},{586,203},{542,169},{483,132},{436,110},{388,93},{334,81},{311,78},{284,76},{242,76}};
POINT L20_A[] = {{56,348},{84,291},{101,260},{121,226},{147,184},{167,150},{172,137},{174,123},{166,112},{157,108},{143,104}};
POINT L20_B[] = {{718,351},{676,296},{632,246},{576,194},{520,153},{466,123},{412,100},{376,89},{350,83},{344,82},{333,80},{319,78},{310,77},{301,76},{286,75},{241,74},{228,75}};
POINT L21_A[] = {{55,348},{80,297},{98,264},{129,210},{158,161},{165,145},{168,128},{163,119},{150,112},{136,108}};
POINT L21_B[] = {{717,351},{681,304},{646,263},{617,234},{562,185},{508,148},{453,119},{420,105},{371,90},{338,83},{326,81},{299,78},{271,76},{240,77},{222,78}};
POINT L22_A[] = {{55,349},{73,309},{100,256},{154,161},{162,139},{161,128},{152,119},{142,115},{131,113}};
POINT L22_B[] = {{716,351},{659,279},{626,243},{567,191},{514,154},{446,118},{392,98},{358,89},{353,88},{332,84},{318,82},{310,81},{283,79},{240,79},{225,80},{216,80}};
POINT L23_A[] = {{54,349},{100,253},{144,174},{156,144},{156,133},{149,125},{143,122},{138,120},{133,119},{128,118},{123,117}};
POINT L23_B[] = {{715,351},{661,283},{616,235},{570,195},{508,152},{441,118},{374,95},{348,89},{325,85},{309,83},{286,81},{263,80},{238,81},{210,83}};
POINT L24_A[] = {{53,349},{81,287},{126,202},{147,160},{150,136},{144,129},{131,124},{116,121}};
POINT L24_B[] = {{714,351},{665,289},{619,240},{557,187},{519,161},{461,129},{413,109},{381,99},{353,92},{332,88},{310,85},{286,83},{234,83},{204,85}};
POINT L25_A[] = {{52,349},{96,254},{127,194},{142,162},{145,142},{140,134},{129,129},{109,125}};
POINT L25_B[] = {{713,351},{673,300},{618,241},{571,200},{516,161},{459,130},{395,105},{366,97},{349,93},{319,88},{289,85},{216,86},{197,87}};
POINT L26_A[] = {{52,349},{89,264},{117,209},{133,176},{139,159},{140,147},{135,138},{124,133},{103,129}};
POINT L26_B[] = {{712,351},{656,282},{599,225},{550,186},{503,155},{453,129},{419,115},{389,105},{358,98},{339,93},{320,91},{291,87},{225,87},{205,89},{191,90}};
POINT L27_A[] = {{51,349},{84,272},{114,210},{128,179},{132,167},{135,156},{133,146},{128,141},{122,138},{114,136},{96,133}};
POINT L27_B[] = {{711,351},{667,296},{621,247},{574,206},{516,165},{451,130},{413,115},{389,107},{363,100},{344,96},{334,94},{313,91},{280,88},{220,89},{210,90},{201,91},{194,92},{185,93}};
POINT L28_A[] = {{50,349},{87,262},{110,211},{125,177},{129,162},{127,149},{121,144},{114,141},{108,140},{89,138}};
POINT L28_B[] = {{710,351},{615,243},{563,199},{511,164},{458,135},{424,121},{386,108},{359,101},{329,95},{316,93},{298,91},{229,90},{215,91},{206,92},{197,93},{190,94},{184,95},{179,95}};
POINT L29_A[] = {{50,346},{76,286},{98,233},{118,186},{123,170},{123,156},{115,147},{109,145},{97,143},{83,142}};
POINT L29_B[] = {{709,351},{671,304},{628,257},{577,212},{525,174},{478,147},{435,127},{403,115},{383,109},{365,104},{347,100},{330,97},{317,95},{240,91},{210,93},{192,95},{173,98}};
POINT L30_A[] = {{48,350},{95,235},{113,190},{117,175},{118,162},{115,155},{105,150},{95,148},{90,147},{75,147}};
POINT L30_B[] = {{708,352},{667,301},{639,270},{595,228},{530,180},{499,160},{410,120},{360,105},{335,100},{295,95},{265,92},{240,93},{220,95},{195,97},{166,100}};
POINT L31_A[] = {{48,347},{86,251},{99,219},{108,195},{113,175},{113,164},{107,156},{96,152},{86,151},{69,150}};
POINT L31_B[] = {{707,351},{634,266},{594,229},{550,195},{499,162},{448,136},{392,115},{378,111},{362,107},{339,102},{328,100},{315,98},{286,95},{238,94},{209,96},{192,98},{178,100},{160,101}};
POINT L32_A[] = {{46,350},{68,290},{72,280},{80,260},{100,207},{106,191},{107,184},{108,168},{104,161},{92,156},{82,155},{62,155}};
POINT L32_B[] = {{706,352},{641,275},{605,240},{570,211},{533,185},{510,170},{480,153},{445,136},{410,123},{385,115},{353,107},{320,101},{292,97},{263,95},{226,96},{200,98},{180,101},{154,106}};
POINT L33_A[] = {{46,350},{60,308},{95,216},{101,195},{103,185},{103,173},{100,166},{95,162},{88,160},{55,159}};
POINT L33_B[] = {{705,351},{668,305},{635,270},{592,231},{556,202},{514,174},{490,160},{440,136},{402,122},{360,110},{325,103},{286,99},{269,97},{225,97},{200,99},{175,103},{148,108}};
POINT L34_A[] = {{45,349},{64,295},{87,230},{95,204},{97,174},{98,186},{94,169},{79,163},{67,162},{49,163}};
POINT L34_B[] = {{705,352},{659,297},{564,210},{521,180},{480,157},{452,143},{397,122},{366,113},{345,108},{323,104},{302,101},{279,99},{220,99},{198,101},{182,103},{170,105},{159,107},{142,110}};
POINT L35_A[] = {{45,349},{62,297},{80,244},{89,214},{93,195},{92,177},{84,169},{72,166},{57,166},{42,167}};
POINT L35_B[] = {{704,352},{632,270},{534,190},{465,151},{426,134},{407,127},{356,112},{337,108},{320,105},{298,102},{271,100},{226,100},{210,101},{191,103},{176,105},{160,108},{150,110},{136,113}};
POINT L36_A[] = {{44,349},{62,292},{80,236},{88,202},{88,184},{83,174},{74,171},{55,170},{44,171},{36,172}};
POINT L36_B[] = {{703,352},{663,304},{567,215},{519,182},{442,142},{394,124},{362,115},{344,111},{323,107},{302,104},{281,102},{262,101},{230,101},{212,102},{192,104},{177,106},{155,110},{130,115}};
POINT L37_A[] = {{43,349},{54,313},{66,275},{76,241},{83,211},{84,191},{78,179},{67,174},{52,173},{38,175},{29,176}};
POINT L37_B[] = {{702,352},{669,312},{634,275},{594,238},{547,202},{511,179},{469,156},{418,134},{364,117},{320,108},{275,103},{243,102},{216,103},{193,105},{165,109},{141,114},{125,118}};
POINT L38_A[] = {{43,349},{59,292},{74,239},{80,207},{77,186},{69,178},{55,176},{37,178},{22,180}};
POINT L38_B[] = {{701,352},{646,288},{601,246},{555,209},{500,174},{458,152},{414,134},{373,121},{340,113},{311,108},{270,104},{249,103},{231,103},{217,104},{194,106},{171,109},{150,113},{120,120}};
POINT L39_A[] = {{42,349},{54,305},{63,272},{71,241},{75,217},{75,198},{69,186},{60,181},{45,180},{30,182},{15,184}};
POINT L39_B[] = {{700,352},{667,312},{596,243},{564,217},{529,193},{499,175},{465,157},{440,146},{401,131},{364,120},{327,112},{292,107},{256,104},{220,105},{186,108},{149,114},{112,123}};
POINT L40_A[] = {{40,350},{63,267},{70,235},{71,203},{69,195},{66,190},{60,186},{51,184},{35,184},{8,189}};
POINT L40_B[] = {{700,352},{659,305},{624,270},{579,230},{545,205},{495,174},{440,147},{377,125},{354,119},{330,114},{298,109},{260,106},{215,106},{195,108},{174,111},{155,113},{134,118},{105,126}};

POINT R01_A[] = {{66,347},{94,318},{121,289},{150,259},{178,230},{209,199},{234,174},{266,142},{295,114},{333,77}};
POINT R01_B[] = {{723,352},{692,314},{664,280},{639,249},{611,215},{583,182},{556,149},{528,114},{504,84}};
POINT R02_A[] = {{67,347},{95,316},{123,287},{154,255},{185,224},{214,196},{244,168},{278,136},{313,106},{332,89},{346,77}};
POINT R02_B[] = {{724,352},{699,320},{678,294},{655,265},{630,234},{605,203},{582,175},{559,146},{537,117},{513,85}};
POINT R03_A[] = {{67,347},{118,292},{157,252},{187,223},{218,194},{248,167},{286,134},{325,102},{359,76}};
POINT R03_B[] = {{724,351},{696,314},{670,280},{641,243},{613,208},{583,170},{555,133},{528,95},{521,85}};
POINT R04_A[] = {{67,347},{97,314},{131,279},{166,244},{197,215},{233,182},{270,151},{307,122},{345,94},{372,75}};
POINT R04_B[] = {{725,351},{697,312},{671,278},{644,243},{617,208},{590,173},{566,141},{543,107},{530,86}};
POINT R05_A[] = {{67,347},{99,312},{129,281},{158,252},{184,227},{215,199},{243,175},{275,150},{308,125},{338,104},{356,92},{385,74}};
POINT R05_B[] = {{726,351},{704,320},{681,288},{660,260},{634,226},{612,197},{591,168},{568,136},{551,109},{538,86}};
POINT R06_A[] = {{67,347},{94,317},{121,288},{152,257},{182,229},{212,202},{245,175},{276,151},{308,128},{340,107},{375,86},{398,73}};
POINT R06_B[] = {{727,351},{704,317},{681,285},{659,255},{635,222},{611,190},{587,156},{569,129},{555,104},{547,87}};
POINT R07_A[] = {{67,347},{143,266},{189,223},{252,171},{303,134},{366,95},{412,72}};
POINT R07_B[] = {{728,351},{692,297},{640,224},{601,170},{568,117},{556,87}};
POINT R08_A[] = {{67,348},{114,295},{156,253},{206,208},{259,166},{320,125},{425,71}};
POINT R08_B[] = {{729,351},{676,270},{624,197},{591,148},{571,109},{564,86}};
POINT R09_A[] = {{67,348},{126,283},{174,236},{226,192},{291,145},{359,105},{438,70}};
POINT R09_B[] = {{730,351},{698,300},{647,225},{606,165},{580,117},{573,86}};
POINT R10_A[] = {{67,348},{119,290},{184,227},{243,179},{314,132},{382,96},{451,69}};
POINT R10_B[] = {{730,351},{688,281},{634,201},{599,146},{590,128},{584,111},{582,101},{581,87}};
POINT R11_A[] = {{67,348},{129,279},{186,225},{246,177},{301,140},{381,98},{465,68}};
POINT R11_B[] = {{731,351},{692,284},{633,195},{604,147},{596,130},{590,111},{589,99},{590,87}};
POINT R12_A[] = {{67,348},{134,274},{187,224},{260,167},{340,119},{426,82},{479,67}};
POINT R12_B[] = {{732,351},{674,252},{630,185},{610,150},{602,133},{597,117},{596,104},{597,95},{599,87}};
POINT R13_A[] = {{67,348},{145,263},{203,210},{278,155},{364,108},{434,81},{492,66}};
POINT R13_B[] = {{733,351},{693,278},{656,220},{643,199},{620,162},{610,142},{604,124},{602,108},{603,99},{607,88}};
POINT R14_A[] = {{68,348},{121,287},{172,237},{227,191},{288,149},{348,116},{437,81},{505,66}};
POINT R14_B[] = {{734,351},{682,256},{629,171},{620,154},{612,135},{610,128},{608,120},{608,107},{610,98},{616,88}};
POINT R15_A[] = {{68,348},{120,288},{167,241},{220,196},{279,154},{348,116},{422,86},{519,65}};
POINT R15_B[] = {{735,351},{697,278},{642,187},{618,141},{614,128},{613,113},{616,100},{625,88}};
POINT R16_A[] = {{68,348},{141,266},{193,218},{248,175},{289,148},{359,111},{402,93},{457,77},{510,67},{532,64}};
POINT R16_B[] = {{735,350},{700,280},{655,203},{626,151},{621,138},{618,125},{618,112},{622,101},{634,88}};
POINT R17_A[] = {{68,348},{100,310},{158,249},{212,202},{283,151},{369,106},{446,79},{511,66},{545,63}};
POINT R17_B[] = {{736,350},{695,267},{641,174},{630,151},{624,134},{623,120},{625,106},{631,97},{642,89}};
POINT R18_A[] = {{68,348},{111,298},{154,253},{212,201},{278,154},{355,112},{425,85},{475,72},{510,66},{558,62}};
POINT R18_B[] = {{737,350},{698,269},{655,193},{640,165},{630,142},{627,129},{627,119},{631,105},{639,96},{653,89}};
POINT R19_A[] = {{68,348},{120,287},{173,234},{248,174},{339,119},{397,94},{468,73},{522,64},{571,61}};
POINT R19_B[] = {{738,350},{707,282},{662,200},{645,168},{635,146},{632,135},{631,122},{634,109},{643,98},{662,90}};
POINT R20_A[] = {{68,348},{106,303},{154,252},{198,212},{271,157},{373,103},{438,80},{490,68},{546,61},{584,60}};
POINT R20_B[] = {{739,351},{710,285},{666,202},{646,164},{639,147},{635,132},{635,118},{641,105},{651,97},{670,90}};
POINT R21_A[] = {{69,348},{132,275},{176,232},{263,164},{345,117},{431,84},{490,70},{548,63},{590,62}};
POINT R21_B[] = {{740,351},{697,256},{664,194},{646,156},{642,145},{641,141},{640,127},{643,113},{651,104},{663,97},{675,94}};
POINT R22_A[] = {{69,348},{120,289},{172,237},{231,188},{300,142},{386,101},{452,80},{524,67},{596,64}};
POINT R22_B[] = {{740,350},{701,261},{659,178},{651,160},{645,140},{645,130},{648,116},{659,105},{679,98}};
POINT R23_A[] = {{70,348},{134,274},{190,222},{267,164},{335,125},{419,91},{483,75},{545,67},{602,67}};
POINT R23_B[] = {{741,351},{706,268},{665,185},{657,167},{650,144},{650,126},{656,115},{666,107},{683,101}};
POINT R24_A[] = {{71,348},{128,281},{179,232},{241,183},{297,147},{384,105},{450,84},{513,72},{563,68},{608,69}};
POINT R24_B[] = {{742,351},{723,304},{711,276},{706,265},{683,217},{669,187},{660,166},{655,149},{655,132},{660,119},{671,110},{687,105}};
POINT R25_A[] = {{71,348},{134,276},{188,225},{246,180},{306,143},{373,111},{434,90},{486,78},{540,71},{563,70},{614,71}};
POINT R25_B[] = {{742,350},{699,247},{667,176},{661,157},{659,142},{661,129},{670,117},{679,112},{692,109}};
POINT R26_A[] = {{72,348},{130,281},{194,221},{263,170},{372,113},{444,89},{518,75},{560,72},{592,72},{620,73}};
POINT R26_B[] = {{743,350},{727,308},{711,270},{694,232},{677,192},{668,169},{664,152},{665,134},{673,122},{683,116},{696,112}};
POINT R27_A[] = {{72,348},{147,264},{202,215},{266,169},{324,136},{391,107},{446,90},{507,78},{569,73},{625,75}};
POINT R27_B[] = {{744,351},{723,296},{705,253},{687,211},{674,177},{669,156},{669,142},{674,129},{682,122},{700,116}};
POINT R28_A[] = {{73,348},{127,285},{181,234},{245,184},{333,133},{410,102},{481,84},{542,76},{602,75},{631,77}};
POINT R28_B[] = {{745,351},{709,258},{700,236},{690,212},{680,185},{675,167},{674,157},{674,144},{677,135},{688,125},{705,120}};
POINT R29_A[] = {{74,348},{134,279},{183,233},{248,183},{310,146},{381,114},{447,93},{509,81},{551,77},{612,77},{637,79}};
POINT R29_B[] = {{745,350},{712,262},{697,224},{682,180},{679,167},{679,149},{682,138},{692,129},{709,123}};
POINT R30_A[] = {{74,348},{140,273},{192,227},{259,177},{328,138},{406,106},{464,91},{517,82},{551,79},{602,78},{642,81}};
POINT R30_B[] = {{746,351},{718,273},{691,199},{685,178},{683,163},{684,149},{690,138},{699,131},{713,127}};
POINT R31_A[] = {{75,348},{145,269},{191,228},{245,188},{309,149},{360,125},{425,102},{479,89},{526,82},{613,80},{649,84}};
POINT R31_B[] = {{747,351},{718,269},{695,202},{690,183},{688,169},{688,155},{693,143},{703,135},{717,131}};
POINT R32_A[] = {{75,348},{139,276},{187,232},{234,196},{280,166},{313,148},{378,119},{437,100},{476,91},{512,85},{543,82},{603,81},{634,83},{655,86}};
POINT R32_B[] = {{747,350},{733,308},{719,268},{698,202},{694,186},{692,170},{693,157},{698,146},{708,138},{722,135}};
POINT R33_A[] = {{76,348},{131,285},{170,247},{217,209},{264,177},{315,148},{359,128},{411,109},{454,97},{502,88},{535,84},{576,82},{616,83},{629,84},{641,85},{661,88}};
POINT R33_B[] = {{748,351},{735,311},{719,263},{705,217},{700,198},{697,180},{699,156},{706,146},{712,142},{725,138}};
POINT R34_A[] = {{77,347},{113,305},{139,277},{169,249},{205,219},{241,193},{289,163},{342,136},{374,123},{412,110},{461,97},{485,92},{520,87},{557,84},{610,84},{637,86},{667,90}};
POINT R34_B[] = {{749,351},{733,302},{720,261},{710,226},{704,201},{702,173},{705,157},{712,148},{723,143},{730,142}};
POINT R35_A[] = {{77,348},{105,314},{155,263},{217,211},{262,180},{301,158},{343,137},{370,126},{406,113},{449,101},{506,90},{533,87},{564,85},{603,85},{622,86},{635,87},{672,92}};
POINT R35_B[] = {{749,350},{731,292},{718,247},{707,199},{706,174},{710,159},{716,152},{723,148},{735,146}};
POINT R36_A[] = {{78,347},{106,314},{186,236},{236,198},{272,175},{320,149},{363,130},{414,112},{459,100},{496,93},{526,89},{550,87},{576,86},{611,87},{632,88},{656,91},{678,94}};
POINT R36_B[] = {{750,350},{737,308},{718,239},{710,191},{711,173},{715,162},{723,154},{729,151},{739,149}};
POINT R37_A[] = {{79,347},{134,284},{177,244},{225,207},{286,168},{346,138},{396,119},{439,106},{465,100},{504,93},{540,89},{578,88},{620,89},{650,91},{684,96}};
POINT R37_B[] = {{751,351},{736,301},{723,251},{717,221},{715,202},{716,174},{722,162},{730,156},{743,153}};
POINT R38_A[] = {{79,348},{128,291},{172,249},{223,209},{272,177},{314,154},{356,135},{396,120},{432,109},{476,99},{522,92},{575,89},{619,90},{657,93},{674,96},{689,98}};
POINT R38_B[] = {{751,349},{734,288},{724,246},{719,214},{719,185},{723,171},{730,162},{738,158},{747,157}};
POINT R39_A[] = {{80,348},{117,303},{165,256},{217,214},{267,181},{321,152},{374,129},{412,116},{467,102},{500,96},{534,92},{565,90},{609,90},{648,93},{671,96},{696,101}};
POINT R39_B[] = {{752,350},{740,307},{731,269},{725,237},{723,206},{724,184},{730,170},{741,162},{752,160}};
POINT R40_A[] = {{80,348},{122,299},{154,267},{192,234},{231,205},{277,176},{321,153},{383,127},{434,111},{473,102},{517,95},{569,91},{613,92},{647,94},{676,98},{702,103}};
POINT R40_B[] = {{753,351},{744,318},{737,290},{730,254},{727,227},{726,204},{729,185},{736,172},{744,166},{756,164}};
POINT R00_A[] = {{66,347},/*{117,293},*/{193,213},/*{288,112},*/{319,80}};
POINT R00_B[] = {{722,352},/*{656,274},*/{581,185},/*{533,128},*/{498,86}};


struct Line
{
	POINT* point;
	UINT8 len;		//线的实际坐标点数
};

#define GetTackItem(x) (x, sizeof(x)/sizeof(POINT))

Line LineLeftA[] =
{
	{R00_A, sizeof(R00_A)/sizeof(POINT)}, // 从1度开始，0度时为正中间
	{L01_A, sizeof(L01_A)/sizeof(POINT)},
	{L02_A, sizeof(L02_A)/sizeof(POINT)},
	{L03_A, sizeof(L03_A)/sizeof(POINT)},
	{L04_A, sizeof(L04_A)/sizeof(POINT)},
	{L05_A, sizeof(L05_A)/sizeof(POINT)},
	{L06_A, sizeof(L06_A)/sizeof(POINT)},
	{L07_A, sizeof(L07_A)/sizeof(POINT)},
	{L08_A, sizeof(L08_A)/sizeof(POINT)},
	{L09_A, sizeof(L09_A)/sizeof(POINT)},
	{L10_A, sizeof(L10_A)/sizeof(POINT)},
	{L11_A, sizeof(L11_A)/sizeof(POINT)},
	{L12_A, sizeof(L12_A)/sizeof(POINT)},
	{L13_A, sizeof(L13_A)/sizeof(POINT)},
	{L14_A, sizeof(L14_A)/sizeof(POINT)},
	{L15_A, sizeof(L15_A)/sizeof(POINT)},
	{L16_A, sizeof(L16_A)/sizeof(POINT)},
	{L17_A, sizeof(L17_A)/sizeof(POINT)},
	{L18_A, sizeof(L18_A)/sizeof(POINT)},
	{L19_A, sizeof(L19_A)/sizeof(POINT)},
	{L20_A, sizeof(L20_A)/sizeof(POINT)},
	{L21_A, sizeof(L21_A)/sizeof(POINT)},
	{L22_A, sizeof(L22_A)/sizeof(POINT)},
	{L23_A, sizeof(L23_A)/sizeof(POINT)},
	{L24_A, sizeof(L24_A)/sizeof(POINT)},
	{L25_A, sizeof(L25_A)/sizeof(POINT)},
	{L26_A, sizeof(L26_A)/sizeof(POINT)},
	{L27_A, sizeof(L27_A)/sizeof(POINT)},
	{L28_A, sizeof(L28_A)/sizeof(POINT)},
	{L29_A, sizeof(L29_A)/sizeof(POINT)},
	{L30_A, sizeof(L30_A)/sizeof(POINT)},
	{L31_A, sizeof(L31_A)/sizeof(POINT)},
	{L32_A, sizeof(L32_A)/sizeof(POINT)},
	{L33_A, sizeof(L33_A)/sizeof(POINT)},
	{L34_A, sizeof(L34_A)/sizeof(POINT)},
	{L35_A, sizeof(L35_A)/sizeof(POINT)},
	{L36_A, sizeof(L36_A)/sizeof(POINT)},
	{L37_A, sizeof(L37_A)/sizeof(POINT)},
	{L38_A, sizeof(L38_A)/sizeof(POINT)},
	{L39_A, sizeof(L39_A)/sizeof(POINT)},
	{L40_A, sizeof(L40_A)/sizeof(POINT)},
};

Line LineLeftB[] =
{
	{R00_B, sizeof(R00_B)/sizeof(POINT)}, // 从1度开始，0度时为正中间
	{L01_B, sizeof(L01_B)/sizeof(POINT)},
	{L02_B, sizeof(L02_B)/sizeof(POINT)},
	{L03_B, sizeof(L03_B)/sizeof(POINT)},
	{L04_B, sizeof(L04_B)/sizeof(POINT)},
	{L05_B, sizeof(L05_B)/sizeof(POINT)},
	{L06_B, sizeof(L06_B)/sizeof(POINT)},
	{L07_B, sizeof(L07_B)/sizeof(POINT)},
	{L08_B, sizeof(L08_B)/sizeof(POINT)},
	{L09_B, sizeof(L09_B)/sizeof(POINT)},
	{L10_B, sizeof(L10_B)/sizeof(POINT)},
	{L11_B, sizeof(L11_B)/sizeof(POINT)},
	{L12_B, sizeof(L12_B)/sizeof(POINT)},
	{L13_B, sizeof(L13_B)/sizeof(POINT)},
	{L14_B, sizeof(L14_B)/sizeof(POINT)},
	{L15_B, sizeof(L15_B)/sizeof(POINT)},
	{L16_B, sizeof(L16_B)/sizeof(POINT)},
	{L17_B, sizeof(L17_B)/sizeof(POINT)},
	{L18_B, sizeof(L18_B)/sizeof(POINT)},
	{L19_B, sizeof(L19_B)/sizeof(POINT)},
	{L20_B, sizeof(L20_B)/sizeof(POINT)},
	{L21_B, sizeof(L21_B)/sizeof(POINT)},
	{L22_B, sizeof(L22_B)/sizeof(POINT)},
	{L23_B, sizeof(L23_B)/sizeof(POINT)},
	{L24_B, sizeof(L24_B)/sizeof(POINT)},
	{L25_B, sizeof(L25_B)/sizeof(POINT)},
	{L26_B, sizeof(L26_B)/sizeof(POINT)},
	{L27_B, sizeof(L27_B)/sizeof(POINT)},
	{L28_B, sizeof(L28_B)/sizeof(POINT)},
	{L29_B, sizeof(L29_B)/sizeof(POINT)},
	{L30_B, sizeof(L30_B)/sizeof(POINT)},
	{L31_B, sizeof(L31_B)/sizeof(POINT)},
	{L32_B, sizeof(L32_B)/sizeof(POINT)},
	{L33_B, sizeof(L33_B)/sizeof(POINT)},
	{L34_B, sizeof(L34_B)/sizeof(POINT)},
	{L35_B, sizeof(L35_B)/sizeof(POINT)},
	{L36_B, sizeof(L36_B)/sizeof(POINT)},
	{L37_B, sizeof(L37_B)/sizeof(POINT)},
	{L38_B, sizeof(L38_B)/sizeof(POINT)},
	{L39_B, sizeof(L39_B)/sizeof(POINT)},
	{L40_B, sizeof(L40_B)/sizeof(POINT)},
};

Line LineRightA[] =
{
	{R00_A, sizeof(R00_A)/sizeof(POINT)}, // 从1度开始，0度时为正中间
	{R01_A, sizeof(R01_A)/sizeof(POINT)},
	{R02_A, sizeof(R02_A)/sizeof(POINT)},
	{R03_A, sizeof(R03_A)/sizeof(POINT)},
	{R04_A, sizeof(R04_A)/sizeof(POINT)},
	{R05_A, sizeof(R05_A)/sizeof(POINT)},
	{R06_A, sizeof(R06_A)/sizeof(POINT)},
	{R07_A, sizeof(R07_A)/sizeof(POINT)},
	{R08_A, sizeof(R08_A)/sizeof(POINT)},
	{R09_A, sizeof(R09_A)/sizeof(POINT)},
	{R10_A, sizeof(R10_A)/sizeof(POINT)},
	{R11_A, sizeof(R11_A)/sizeof(POINT)},
	{R12_A, sizeof(R12_A)/sizeof(POINT)},
	{R13_A, sizeof(R13_A)/sizeof(POINT)},
	{R14_A, sizeof(R14_A)/sizeof(POINT)},
	{R15_A, sizeof(R15_A)/sizeof(POINT)},
	{R16_A, sizeof(R16_A)/sizeof(POINT)},
	{R17_A, sizeof(R17_A)/sizeof(POINT)},
	{R18_A, sizeof(R18_A)/sizeof(POINT)},
	{R19_A, sizeof(R19_A)/sizeof(POINT)},
	{R20_A, sizeof(R20_A)/sizeof(POINT)},
	{R21_A, sizeof(R21_A)/sizeof(POINT)},
	{R22_A, sizeof(R22_A)/sizeof(POINT)},
	{R23_A, sizeof(R23_A)/sizeof(POINT)},
	{R24_A, sizeof(R24_A)/sizeof(POINT)},
	{R25_A, sizeof(R25_A)/sizeof(POINT)},
	{R26_A, sizeof(R26_A)/sizeof(POINT)},
	{R27_A, sizeof(R27_A)/sizeof(POINT)},
	{R28_A, sizeof(R28_A)/sizeof(POINT)},
	{R29_A, sizeof(R29_A)/sizeof(POINT)},
	{R30_A, sizeof(R30_A)/sizeof(POINT)},
	{R31_A, sizeof(R31_A)/sizeof(POINT)},
	{R32_A, sizeof(R32_A)/sizeof(POINT)},
	{R33_A, sizeof(R33_A)/sizeof(POINT)},
	{R34_A, sizeof(R34_A)/sizeof(POINT)},
	{R35_A, sizeof(R35_A)/sizeof(POINT)},
	{R36_A, sizeof(R36_A)/sizeof(POINT)},
	{R37_A, sizeof(R37_A)/sizeof(POINT)},
	{R38_A, sizeof(R38_A)/sizeof(POINT)},
	{R39_A, sizeof(R39_A)/sizeof(POINT)},
	{R40_A, sizeof(R40_A)/sizeof(POINT)},
};

Line LineRightB[] =
{
	{R00_B, sizeof(R00_B)/sizeof(POINT)}, // 从1度开始，0度时为正中间
	{R01_B, sizeof(R01_B)/sizeof(POINT)},
	{R02_B, sizeof(R02_B)/sizeof(POINT)},
	{R03_B, sizeof(R03_B)/sizeof(POINT)},
	{R04_B, sizeof(R04_B)/sizeof(POINT)},
	{R05_B, sizeof(R05_B)/sizeof(POINT)},
	{R06_B, sizeof(R06_B)/sizeof(POINT)},
	{R07_B, sizeof(R07_B)/sizeof(POINT)},
	{R08_B, sizeof(R08_B)/sizeof(POINT)},
	{R09_B, sizeof(R09_B)/sizeof(POINT)},
	{R10_B, sizeof(R10_B)/sizeof(POINT)},
	{R11_B, sizeof(R11_B)/sizeof(POINT)},
	{R12_B, sizeof(R12_B)/sizeof(POINT)},
	{R13_B, sizeof(R13_B)/sizeof(POINT)},
	{R14_B, sizeof(R14_B)/sizeof(POINT)},
	{R15_B, sizeof(R15_B)/sizeof(POINT)},
	{R16_B, sizeof(R16_B)/sizeof(POINT)},
	{R17_B, sizeof(R17_B)/sizeof(POINT)},
	{R18_B, sizeof(R18_B)/sizeof(POINT)},
	{R19_B, sizeof(R19_B)/sizeof(POINT)},
	{R20_B, sizeof(R20_B)/sizeof(POINT)},
	{R21_B, sizeof(R21_B)/sizeof(POINT)},
	{R22_B, sizeof(R22_B)/sizeof(POINT)},
	{R23_B, sizeof(R23_B)/sizeof(POINT)},
	{R24_B, sizeof(R24_B)/sizeof(POINT)},
	{R25_B, sizeof(R25_B)/sizeof(POINT)},
	{R26_B, sizeof(R26_B)/sizeof(POINT)},
	{R27_B, sizeof(R27_B)/sizeof(POINT)},
	{R28_B, sizeof(R28_B)/sizeof(POINT)},
	{R29_B, sizeof(R29_B)/sizeof(POINT)},
	{R30_B, sizeof(R30_B)/sizeof(POINT)},
	{R31_B, sizeof(R31_B)/sizeof(POINT)},
	{R32_B, sizeof(R32_B)/sizeof(POINT)},
	{R33_B, sizeof(R33_B)/sizeof(POINT)},
	{R34_B, sizeof(R34_B)/sizeof(POINT)},
	{R35_B, sizeof(R35_B)/sizeof(POINT)},
	{R36_B, sizeof(R36_B)/sizeof(POINT)},
	{R37_B, sizeof(R37_B)/sizeof(POINT)},
	{R38_B, sizeof(R38_B)/sizeof(POINT)},
	{R39_B, sizeof(R39_B)/sizeof(POINT)},
	{R40_B, sizeof(R40_B)/sizeof(POINT)},
};

BOOL DrawBackCarLine(HDC hDC,const POINT* pLeftPt, UINT8 u1LeftCount, const POINT* pRightPt, UINT8 u1RightCount, COLORREF color)
{
	POINT beginPt, endPt;
	HPEN hNewPen, hOldPen;

	hNewPen = ::CreatePen(PS_SOLID, 4, color);
	hOldPen = (HPEN)SelectObject(hDC, hNewPen);

	for(int i=0; i<u1LeftCount-1; i++)		//左侧轨迹
	{
		beginPt.x = pLeftPt[i].x;
		beginPt.y = pLeftPt[i].y;
		endPt.x   = pLeftPt[i+1].x;
		endPt.y   = pLeftPt[i+1].y;
		::MoveToEx(hDC, beginPt.x, beginPt.y, NULL);
		::LineTo(hDC, endPt.x, endPt.y);
	}

	for(int i=0; i<u1RightCount-1; i++)		//右侧轨迹
	{
		beginPt.x = pRightPt[i].x;
		beginPt.y = pRightPt[i].y;
		endPt.x   = pRightPt[i+1].x;
		endPt.y   = pRightPt[i+1].y;
		::MoveToEx(hDC, beginPt.x, beginPt.y, NULL);
		::LineTo(hDC, endPt.x, endPt.y);
	}

	SelectObject(hDC, hOldPen);
	DeleteObject(hNewPen);
	return TRUE;
}

BOOL DrawBackCarTrack(HDC hDC,UINT8 u1Angle, BOOL bLeft)
{
	if (u1Angle > 40)
	{
		u1Angle = 40;
	}

	if(bLeft)
	{
		if(u1Angle > 32)		//Red Track
		{
			DrawBackCarLine(hDC,(const POINT*)LineLeftA[u1Angle].point, LineLeftA[u1Angle].len, (const POINT*)LineLeftB[u1Angle].point, LineLeftB[u1Angle].len, RGB(255, 0, 0));
		}
		else					//Green Track
		{
			DrawBackCarLine(hDC,(const POINT*)LineLeftA[u1Angle].point, LineLeftA[u1Angle].len, (const POINT*)LineLeftB[u1Angle].point, LineLeftB[u1Angle].len, RGB(0, 255, 0));
		}
	}
	else
	{
		if(u1Angle > 32)		//Red Track
		{
			DrawBackCarLine(hDC,(const POINT*)LineRightA[u1Angle].point, LineRightA[u1Angle].len, (const POINT*)LineRightB[u1Angle].point, LineRightB[u1Angle].len, RGB(255, 0, 0));
		}
		else					//Green Track
		{
			DrawBackCarLine(hDC,(const POINT*)LineRightA[u1Angle].point, LineRightA[u1Angle].len, (const POINT*)LineRightB[u1Angle].point, LineRightB[u1Angle].len, RGB(0, 255, 0));
		}
	}
	return TRUE;
}


