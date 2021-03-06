#include "stdafx.h"
#include "SetFactoryDlg.h"
#include "RpcMCU.h"
#include "config.h"
#include "tzUtil.h"
#include "msgboxdlg.h"
#include "MainWnd.h"
#include "interface.h"
#include "sysUtil.h"

CSetFactoryDlg::CSetFactoryDlg(void)
{

}

CSetFactoryDlg::~CSetFactoryDlg(void)
{

}

void CSetFactoryDlg::OnInitDialog()
{
	__super::OnInitDialog();

	InitLayers();
	InitSButtonStatus();
}

void CSetFactoryDlg::InitLayers()
{
	CWceUiScrollLayer* pLayer = (CWceUiScrollLayer*)GetChildByName(L"frame");
	if (pLayer)
	{
		pLayer->SetScrollMode(SL_VERT);
		pLayer->SetExtraScroll(30);
		pLayer->SetScrollRange(0, 60);
	}


	CWceUiLayer* pLayerValue = GetLayerByName(L"value_mcu");
	if (pLayerValue)
	{
		//MCU VERSION	
		CString str;
		CRpcMCU::GetInstance()->RPC_GetMcuVer(str.GetBuffer(64));
		str.ReleaseBuffer();
		pLayerValue->SetText(str);
	}
}


void CSetFactoryDlg::OnBnClick(CWceUiButton* pButton)
{
	if (pButton->IsEqualName(L"mcu"))	// found it
	{
		CDlgManager::GetInstance()->ShowDlg(CSetMcuUpdateDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"logo"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetUpdateLogoDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"functions"))
	{
		CDlgManager::GetInstance()->ShowDlg(CFacFunctionsDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"language"))
	{
		CDlgManager::GetInstance()->ShowDlg(CFacLanguageDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"canbus"))
	{
		CDlgManager::GetInstance()->ShowDlg(CFacCanbusDlg_ID, SOURCE_SETTING);
	}
	else if (pButton->IsEqualName(L"save"))
	{
		OnSave();
	}
	else if (pButton->IsEqualName(L"savetosd"))
	{
		OnSaveToSD();
	}
	else if (pButton->IsEqualName(L"loadfromsd"))
	{
		OnLoadFromSD();
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}

void CSetFactoryDlg::OnSBnClick(CWceUiSButton* pSButton, int status)
{
	if (pSButton->IsEqualName(L"capture_switch"))
	{
		BOOL bEnable = (status == WCEUI_SBUTTON_ON) ? TRUE : FALSE;
		sysutil::nss_get_instance()->ui_capture_screen = bEnable;
	}
	else if (pSButton->IsEqualName(L"h264_switch"))
	{
		config::get_config_video()->h264 = (status == WCEUI_SBUTTON_ON) ? 1 : 0;
	}
}

void CSetFactoryDlg::InitSButtonStatus()
{
	// capture
	CWceUiSButton* psbutton = (CWceUiSButton*)GetLayerByName(L"capture_switch");
	if(psbutton)
	{
		psbutton->SetStatus(sysutil::nss_get_instance()->ui_capture_screen ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);
	}

	// h.24
	psbutton = (CWceUiSButton*)GetLayerByName(L"h264_switch");
	if(psbutton)
	{
		psbutton->SetStatus(config::get_config_video()->h264 ? WCEUI_SBUTTON_ON : WCEUI_SBUTTON_OFF);
	}
}

void CSetFactoryDlg::OnSave()
{
	if (SaveConfig())
	{
		CMsgBoxDlg::MessageBox(L"SET_FACTORY_SAVE_OK", TRUE);
	}
	else
	{
		CMsgBoxDlg::MessageBox(L"SET_FACTORY_SAVE_FAIL", TRUE);
	}
}

void CSetFactoryDlg::OnSaveToSD()
{
	CString path;
	path.Format(L"%s\\%s", ui_msdk_get_disk(SOURCE_SD), CONFIG_FILE_NAME);	// 保存到SD源对应的卡,不一定是SDMMC
	if (SaveConfig(path))
	{
		CMsgBoxDlg::MessageBox(L"SET_FACTORY_SAVE_SD_OK", TRUE);
	}
	else
	{
		CMsgBoxDlg::MessageBox(L"SET_FACTORY_SAVE_SD_FAIL", TRUE);
	}
}

void CSetFactoryDlg::OnLoadFromSD()
{
	CString sd_config_file;
	sd_config_file.Format(L"%s\\%s", ui_msdk_get_disk(SOURCE_SD), CONFIG_FILE_NAME);	// SD源对应的卡,不一定是SDMMC
	CString config_file;
	config_file.Format(L"%s\\%s", tzutil::GetAppPath(), CONFIG_FILE_NAME);

	if (GetFileAttributes(sd_config_file) != (DWORD)-1)	// 文件存在
	{
		CWceUiXmlDoc xmlDoc;
		if (xmlDoc.LoadXmlFile(sd_config_file)	// 文件的合法性由用户保证,这里不作太多校验,只要能成功加载就认为合法
			&& CopyFile(sd_config_file, config_file, FALSE))
		{
			config::config_init();
			// 有些更新项可以即时刷新，有些项可能需要重启
			CMainWnd::GetInstance()->PostMessage(UI_MSG_FACTORY_SET_CHANGE);

			CMsgBoxDlg::MessageBox(L"SET_FACTORY_LOAD_SD_OK", TRUE);
		}
		else
		{
			CMsgBoxDlg::MessageBox(L"SET_FACTORY_LOAD_SD_FAIL", TRUE);
		}
	}
	else
	{
		CMsgBoxDlg::MessageBox(L"SET_FACTORY_NO_CONFIG_FILE", TRUE);
	}
}


BOOL CSetFactoryDlg::SaveConfig(LPCTSTR lpszFileName)
{
	TCHAR szFilepath[MAX_PATH] = L"";
	_stprintf(szFilepath, L"%s\\%s", tzutil::GetAppPath(), CONFIG_FILE_NAME);

	CWceUiXmlDoc xmlDoc;
	if (!xmlDoc.LoadXmlFile(szFilepath))
	{
		RETAILMSG(1, (L"[nvai_car]: CSetFactoryDlg::SaveConfig, could not find config.xml file.\n"));
		return FALSE;
	}

	// 保存可修改项,这些项可能被修改了
	CWceUiXmlElem *pxmlElem = xmlDoc.RootElement();
	ASSERT(pxmlElem);
	XMLSaveFunctions(pxmlElem);


	if (lpszFileName)
	{
		return xmlDoc.SaveXmlFileAs(lpszFileName);
	}
	else
	{
		return xmlDoc.SaveXmlFile();
	}
}

BOOL CSetFactoryDlg::XMLSaveFunctions(CWceUiXmlElem *pxmlElemRoot)
{
	// functions
	CString str;
	for (int i=0; i<config::get_config_functions()->support_array.GetCount(); i++)
	{
		if (i!=0)
		{
			str += L",";
		}
		str += config::get_config_functions()->support_array.GetAt(i);
	}

	PCWceUiXmlElem pXmlElemChild = pxmlElemRoot->GetChildElem(L"Functions");
	if (pXmlElemChild)
	{
		pXmlElemChild->SetAttr(L"Value", str);
	}

	// language

	// h264
	pXmlElemChild = pxmlElemRoot->GetChildElem(L"Video");
	if (pXmlElemChild)
	{
		pXmlElemChild->SetAttr(L"h264", config::get_config_video()->h264 == 1 ? L"1" : L"0");
	}


	return TRUE;
}