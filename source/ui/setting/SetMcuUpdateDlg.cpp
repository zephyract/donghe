#include "stdafx.h"
#include "WceUiListBox.h"
#include "WceUiBase.h"
#include "SetMcuUpdateDlg.h"
#include "config.h"
#include "interface.h"
#include "sysUtil.h"
#include "RpcMCU.h"
#include "MainWnd.h"

CSetMcuUpdateDlg::CSetMcuUpdateDlg(void)
: m_bIsUpdating(FALSE)
, m_fp(NULL)
, m_file_size(0)
, m_bytes_written(0)
{
}

CSetMcuUpdateDlg::~CSetMcuUpdateDlg(void)
{

}

void CSetMcuUpdateDlg::OnInitDialog()
{
	CWceUiSlider* pslider = (CWceUiSlider*)GetChildByName(L"progress");
	if (pslider)
	{
		pslider->SetRange(0, 100);
		pslider->SetReadonly(TRUE);
	}

	__super::OnInitDialog();
}


void CSetMcuUpdateDlg::OnBnClick(CWceUiButton* pButton)
{
	if (m_bIsUpdating)		// 升级过程不响应任何事件
	{
		return;
	}

	if (pButton->IsEqualName(L"update"))
	{
		if (!IsMCUFileExist())
		{
			ShowTip(L"SET_MCU_UPDATE_NO_FILE");
		}
		else
		{
			if(Start())
			{
				ShowTip(L"SET_MCU_UPDATE_TIP");
			}
			else
			{
				ShowTip(L"SET_MCU_UPDATE_FAIL");
			}
		}
	}
	else if (pButton->IsEqualName(L"back"))
	{
		CDlgManager::GetInstance()->ShowDlg(CSetFactoryDlg_ID, SOURCE_SETTING);
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}

void CSetMcuUpdateDlg::OnDlgShow(BOOL bShow)
{
	if (bShow)
	{
		ShowTip(L"SET_MCU_UPDATE_TIP");
	}

	__super::OnDlgShow(bShow);
}

void CSetMcuUpdateDlg::ShowTip(LPCTSTR res_id)
{
	CWceUiLayer* ptip = GetChildByName(L"tip");
	if (ptip)
	{
		ptip->SetTextResID(res_id);
	}
}

void CSetMcuUpdateDlg::OnTimer(WORD wIDEvent)
{
	if (wIDEvent == TIMER_ID_UPDATE_MCU)
	{
		if (m_bIsUpdating)
		{
			if (!Updating())
			{
				ShowTip(L"SET_MCU_UPDATE_FAIL");
				End();
			}
			UpdateProgress();
		}
		else
		{
			KillTimer(TIMER_ID_UPDATE_MCU);
		}
	}
	else if (wIDEvent == TIMER_ID_UPDATE_MCU_END)
	{
		KillTimer(TIMER_ID_UPDATE_MCU_END);
		CRpcMCU::GetInstance()->RPC_UpdateMCU(FALSE);
	}

	__super::OnTimer(wIDEvent);
}

BOOL CSetMcuUpdateDlg::IsMCUFileExist()
{
	return GetFileAttributes(GetMCUFile()) != (DWORD)-1;
}

LPCTSTR CSetMcuUpdateDlg::GetMCUFile()
{
	return L"sdmmc\\stm8.s19";
}

BOOL CSetMcuUpdateDlg::Start()
{
	m_bIsUpdating = TRUE;
	// 开始升级MCU，不能给MCU发送其它数据
	CMainWnd::GetInstance()->KillTimer(TIMER_ID_REFRESH_LED);

	m_bytes_written = 0;
	m_file_size = GetFileSize(GetMCUFile());
	m_fp = _tfopen(GetMCUFile(), L"rb");
	if (m_fp)
	{
		RPCERR error;
  		if( ERROR_SUCCESS == CRpcMCU::GetInstance()->RPC_UpdateMCU(TRUE, &error)
  			&& error == ERROR_SUCCESS)
		{
			SetTimer(TIMER_ID_UPDATE_MCU, 1);
			return TRUE;
		}
	}

	if (m_fp)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
	m_bIsUpdating = FALSE;

	// 开始升级MCU，不能给MCU发送其它数据, 失败恢复
	CMainWnd::GetInstance()->SetTimer(TIMER_ID_REFRESH_LED, 500, NULL);


	return FALSE;
}

DWORD CSetMcuUpdateDlg::GetFileSize(LPCTSTR file_name)
{
	DWORD size = (DWORD)-1;
	HANDLE hfile = CreateFile(file_name,  GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hfile)
	{
		size = ::GetFileSize(hfile, NULL);
		CloseHandle(hfile);
	}

	return size;
}

BOOL CSetMcuUpdateDlg::Updating()
{
	DWORD dwTick = GetTickCount();
	if (m_fp)
	{
		char buffer[256];
		int len = 0;
		if (fgets(buffer, 256, m_fp))
		{
			len = 0;
			while (buffer[len] != '\0' && buffer[len] != '\r' && buffer[len] != '\n')
			{
				len++;		// 统计有效字符个数
			}
			if (len > 0)
			{
				RPCERR error;
  				if( ERROR_SUCCESS != CRpcMCU::GetInstance()->RPC_SendMCUData(buffer, len, &error)
  					|| ERROR_SUCCESS != error)
				{
					// 如果失败了,多尝试一次
					if( ERROR_SUCCESS != CRpcMCU::GetInstance()->RPC_SendMCUData(buffer, len, &error)
						|| ERROR_SUCCESS != error)
					{
						return FALSE;
					}

				}
			}

			m_bytes_written += (len+2);	// 换行符数目同,可能会有点误差,不过不影响升级进度的计算
		}
		else
		{
			if (!feof(m_fp))	// error occured...
			{
				return FALSE;
			}
			else	// 升級完成
			{
				ShowTip(L"SET_MCU_UPDATE_SUCCESS");
				End();
			}
		}
	}
	static int t=0;
	if (t++<10)
	{
		RETAILMSG(1, (L"send to mcu tick = %d \n", GetTickCount()-dwTick));
	}
	return TRUE;
}

BOOL CSetMcuUpdateDlg::End()
{
	m_bIsUpdating = FALSE;

	KillTimer(TIMER_ID_UPDATE_MCU);
	fclose(m_fp);
	m_fp = NULL;

	SetTimer(TIMER_ID_UPDATE_MCU_END, 1000);

	return TRUE;
}

void CSetMcuUpdateDlg::UpdateProgress()
{
	static int s_pos = -1;
	int pos = m_bytes_written*100/m_file_size;
	if (s_pos != pos)
	{
		s_pos = pos;

		CWceUiSlider* pslider = (CWceUiSlider*)GetChildByName(L"progress");
		if (pslider && m_file_size>0)
		{
			pslider->SetPos(m_bytes_written*100/m_file_size);
		}

		CWceUiLayer* player = GetChildByName(L"percent");
		if (player)
		{
			TCHAR buffer[16] = L"";
			if (m_bytes_written > 0)
			{
				_stprintf(buffer, L"%d%%", m_bytes_written*100/m_file_size);
			}
			player->SetText(buffer);
		}
	}

}