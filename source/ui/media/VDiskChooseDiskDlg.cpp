#include "stdafx.h"
#include "WceUiBase.h"
#include "VDiskChooseDiskDlg.h"
#include "WceUiLoadString.h"
#include "vdiskFileManager.h"
#include "WceUiLayerCtrl.h"
#include "MainWnd.h"
#include "interface.h"
#include "protocol.h"

CVDiskChooseDiskDlg::CVDiskChooseDiskDlg(void)
: m_nMode(0)
, m_nDiskID(-1)
, m_nDiskRip(-1)
{
}

CVDiskChooseDiskDlg::~CVDiskChooseDiskDlg(void)
{
}

void CVDiskChooseDiskDlg::OnInitDialog()
{
	__super::OnInitDialog();


	for (int i=0; i<VDISK_MAX_DISK_COUNT; i++)
	{
		TCHAR layer_name[16];
		TCHAR layer_text[8];
		_stprintf(layer_name, L"disk%d", i);
		_stprintf(layer_text, L"%02d", i+1);
		CWceUiButton* player = (CWceUiButton*)GetChildByName(layer_name);
		if (player)
		{
			player->SetText(layer_text);

			if (IsDiskActive(i))
			{
				ActiveDiskButton(i, TRUE);
			}
		}
	}

	CWceUiLayer *pLayer = GetChildByName(L"title");
	if (pLayer)
	{
		pLayer->SetTextResID(m_nMode == VDISK_RECORD ? L"VDISK_MSG_DISK_TO_RIP" : L"VDISK_MSG_DISK_TO_DELETE");
	}
}

BOOL CVDiskChooseDiskDlg::IsDiskActive(int nIndex)
{
	return (CVDiskFileManager::GetInstance()->IsDiskExist(nIndex) && nIndex!=m_nDiskRip);
}

void CVDiskChooseDiskDlg::ActiveDiskButton(int nIndex, BOOL bActive)
{
	TCHAR layer_name[16];
	_stprintf(layer_name, L"disk%d", nIndex);
	CWceUiButton* player = (CWceUiButton*)GetChildByName(layer_name);
	if (player)
	{
		TCHAR style_normal[MAX_TAG_NAME_LEN];
		TCHAR style_down[MAX_TAG_NAME_LEN];
		if (bActive)
		{
			_stprintf(style_normal, L"disk%d_active_normal", nIndex);
			_stprintf(style_down, L"disk%d_active_down", nIndex);
		}
		else
		{
			_stprintf(style_normal, L"disk%d_normal", nIndex);
			_stprintf(style_down, L"disk%d_down", nIndex);
		}

		player->SetStatusStyleName(style_normal, CWceUiButton::STATUS_NORMAL);
		player->SetStatusStyleName(style_down, CWceUiButton::STATUS_DOWN);
	}

}


void CVDiskChooseDiskDlg::OnBnClick(CWceUiButton* pButton)
{
	// 处理disk按钮
	int i = 0;
	for (; i<VDISK_MAX_DISK_COUNT; i++)
	{
		TCHAR layer_name[16];
		_stprintf(layer_name, L"disk%d", i);
		if (pButton->IsEqualName(layer_name))
		{
			break;
		}
	}

	if (i < VDISK_MAX_DISK_COUNT)	// 点击的是disk
	{
		if ((IsDiskActive(i) && m_nMode == VDISK_DELETE)
			|| (!CVDiskFileManager::GetInstance()->IsDiskExist(i) && m_nMode == VDISK_RECORD) )// 只能删除活动的, 或录制非活动的
		{
			if (i != m_nDiskID)
			{
				CheckDiskButton(m_nDiskID, FALSE);
				CheckDiskButton(i, TRUE);
				m_nDiskID = i;
			}
		}
	
		return;
	}

	// 
	if (pButton->IsEqualName(L"ok"))
	{
		if (m_nDiskID>=0)	// 已选中了一个disk
		{
			EndDialog(m_nDiskID);
		}
	}
	else if (pButton->IsEqualName(L"cancel"))
	{
		EndDialog(-1);
	}

	__super::OnBnClick(pButton);
}

void CVDiskChooseDiskDlg::CheckDiskButton(int nDiskID, BOOL bCheck)
{
	TCHAR layer_name[16];
	_stprintf(layer_name, L"disk%d", nDiskID);
	CWceUiButton* player = (CWceUiButton*)GetChildByName(layer_name);
	TCHAR style_down[MAX_TAG_NAME_LEN];

	if (nDiskID>=0 && nDiskID<VDISK_MAX_DISK_COUNT)
	{
		if (CVDiskFileManager::GetInstance()->IsDiskExist(nDiskID))
		{
			if (bCheck)
			{
				if (player)
				{
					_stprintf(style_down, L"disk%d_active_down", nDiskID);
					player->SetStatusStyleName(style_down, CWceUiButton::STATUS_NORMAL);
				}
			}
			else
			{
				ActiveDiskButton(nDiskID, TRUE);
			}
		}
		else
		{
			if (bCheck)
			{
				if (player)
				{
					_stprintf(style_down, L"disk%d_down", nDiskID);
					player->SetStatusStyleName(style_down, CWceUiButton::STATUS_NORMAL);
				}
			}
			else
			{
				ActiveDiskButton(nDiskID, FALSE);
			}
		}
	}
}

int CVDiskChooseDiskDlg::MessageBox(int nMode, int nDiskRip)
{
	CVDiskChooseDiskDlg *pDlg = (CVDiskChooseDiskDlg *)CMainWnd::GetInstance()->GetLayerCtrl()->CreateDlg(_T("media\\CVDiskChooseDiskDlg"));
	int ret = -1;
	if (pDlg)
	{
		pDlg->m_nMode = nMode;
		pDlg->m_nDiskRip = nDiskRip;
		ret = (int)pDlg->DoModal();
		CMainWnd::GetInstance()->GetLayerCtrl()->DeleteLayer(pDlg);
	}

	return ret;
}
