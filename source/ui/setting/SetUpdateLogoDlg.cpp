#include "stdafx.h"
#include "SetUpdateLogoDlg.h"
#include "tzUtil.h"
#include "wceUiPageLayer.h"
#include "sysUtil.h"
#include "interface.h"
#include "SetWallpaperDlg.h"
#include "WceUi32Bitmap.h"

#include <WinIoCtl.h>
#include "diskio.h"

CSetUpdateLogoDlg::CSetUpdateLogoDlg(void)
{
	for (int i=0; i<LOGO_COUNT; i++)
	{
		if (WceUiGetBitsPixel() == 16)
		{
			m_pBkTexture[i] = new CWceUi565Bitmap();
		}
		else
		{
			m_pBkTexture[i] = new CWceUi32Bitmap();
		}
	}

	m_nSelected = -1;
}

CSetUpdateLogoDlg::~CSetUpdateLogoDlg(void)
{
	for (int i=0; i<LOGO_COUNT; i++)
	{
		if (m_pBkTexture[i] != NULL)
		{
			delete m_pBkTexture[i];
			m_pBkTexture[i] = NULL;
		}
	}
}

void CSetUpdateLogoDlg::OnInitDialog()
{
	__super::OnInitDialog();

	TCHAR szFilepath[MAX_PATH] = L"";
	TCHAR szLayerName[32];
	CWallPaperButton* pbutton = NULL;
	for (int i=0; i<LOGO_COUNT; i++)
	{
		_stprintf(szFilepath, L"%s\\logo.pack\\s%d.bmp", tzutil::GetAppPath(), i);
		if (WceUiGetBitsPixel() == 16)
		{
			((CWceUi565Bitmap*)m_pBkTexture[i])->LoadFromFile(szFilepath);
		}
		else
		{
			((CWceUi32Bitmap*)m_pBkTexture[i])->LoadFromFile(szFilepath);
		}

		_stprintf(szLayerName, L"bk%d", i);
		pbutton = (CWallPaperButton*)GetLayerByName(szLayerName);
		if (pbutton)
		{
			pbutton->SetBackGround(m_pBkTexture[i]);
		}
	}
	CheckButton(0);
	m_nSelected = 0;

	CWceUiPageLayer* ppage = (CWceUiPageLayer*)GetChildByName(L"frame");
	if (ppage)
	{
		ppage->SetExtraScroll(30);
		ppage->SetScrollRange(0, 1600);
	}
}

void CSetUpdateLogoDlg::CheckButton(int nIndex)
{
	TCHAR buffer[32];
	CWceUiButton* pbutton = NULL;
	for (int i=0; i<LOGO_COUNT; i++)
	{
		_stprintf(buffer, L"bk%d", i);
		pbutton = (CWceUiButton*)GetLayerByName(buffer);
		if (pbutton)
		{
			if (i == nIndex)
			{
				_stprintf(buffer, L"bk%d_down", i);
			}
			else
			{
				_stprintf(buffer, L"bk%d_normal", i);
			}
			pbutton->SetStatusStyleName(buffer, CWceUiButton::STATUS_NORMAL);
		}
	}
}


void CSetUpdateLogoDlg::OnBnClick(CWceUiButton* pButton)
{
	TCHAR layer_name[][16] = {L"bk0", L"bk1", L"bk2", L"bk3", L"bk4", L"bk5", L"bk6", L"bk7", L"bk8" };
	int count = sizeof(layer_name)/sizeof(layer_name[0]);
	int i = 0;
	for (; i<count; i++)
	{
		if (pButton->IsEqualName(layer_name[i]))
		{
			m_nSelected = i;
			CheckButton(i);
			ShowTip(0);
			return;
		}
	}

	if (pButton->IsEqualName(L"update"))
	{
		BOOL b = UpdateLogo(m_nSelected); 
		ShowTip(b ? 1 : 2);
	}
	else if (pButton->IsEqualName(L"back"))
	{
		ShowTip(0);
		CDlgManager::GetInstance()->ShowDlg(CSetFactoryDlg_ID, SOURCE_SETTING);
	}
	else
	{
		__super::OnBnClick(pButton);
	}

}

void CSetUpdateLogoDlg::ShowTip(int nMode)
{
	CWceUiLayer* ptip = GetChildByName(L"tip");
	if (ptip)
	{
		if (nMode == 0)
		{
			ptip->SetText(L"");
		}
		else if (nMode == 1)
		{
			ptip->SetTextResID(L"SET_UPDATE_SUCCESS");
		}
		else if(nMode == 2)
		{
			ptip->SetTextResID(L"SET_UPDATE_FAIL");
		}
	}
}

BOOL CSetUpdateLogoDlg::UpdateLogo(int nIndex)
{
	if (nIndex < 0 || nIndex>=LOGO_COUNT)
	{
		return FALSE;
	}

	TCHAR szFilepath[MAX_PATH] = L"";
	DWORD size = 0;
	LPVOID pbuffer = NULL;
	_stprintf(szFilepath, L"%s\\logo.pack\\logo%d.bmp", tzutil::GetAppPath(), nIndex);
	pbuffer = WceUiReadFile(szFilepath, &size, L"abcdefg");
	if (pbuffer)
	{
		BOOL b = WriteLogo(pbuffer, size);
		delete[] pbuffer;
		pbuffer = NULL;
		return b;
	}

	return FALSE;
}

#define IOCTL_UPDATE_LOGO					CTL_CODE(FILE_DEVICE_UNKNOWN, 0x108, METHOD_BUFFERED, FILE_ANY_ACCESS)

BOOL CSetUpdateLogoDlg::WriteLogo(LPVOID pbuffer, DWORD size)
{
	HANDLE	handle = CreateFile(L"YGI1:", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(INVALID_HANDLE_VALUE == handle)
	{
		return  FALSE;
	}

	//Write Logo
	DeviceIoControl(handle, IOCTL_UPDATE_LOGO, pbuffer, size, NULL, 0, NULL, NULL);
	CloseHandle(handle);

	return TRUE;
}