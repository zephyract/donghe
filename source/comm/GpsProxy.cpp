#include "StdAfx.h"
#include "GpsProxy.h"

//MTK GPS NMEA Command 
static const char* const ColdCmd ="$PMTK103*30\r\n";
static const char* const WarmCmd ="$PMTK102*31\r\n";
static const char* const HotCmd ="$PMTK101*32\r\n";
static const char* const FactoryCmd ="$PMTK101*33\r\n";
static const char* const VersionCmd ="$PMTK605*31\r\n";


CGpsProxy::CGpsProxy(void)
{
	memset(&m_GpsStatus, 0, sizeof(m_GpsStatus));
}

CGpsProxy::~CGpsProxy(void)
{
// 	if (g_bRunning)
// 	{
// 		RETAILMSG(1, (L"[nvai_car]: ~CGpsProxy() warning: please call Uninitialize().\n"));
// 		Uninitialize();
// 	}
}

BOOL CGpsProxy::Initialize(HWND hWnd, LPCTSTR lpszPort, DWORD dwBaudrate)
{
	int n=1;
	BOOL bResult = FALSE;
	while(!bResult && n<6)
	{
		bResult = 	m_GpsCore.Open(hWnd, lpszPort, dwBaudrate);

		n++;
		Sleep(300);
	}

	return bResult;
};

void CGpsProxy::Uninitialize()
{
	m_GpsCore.Close();
}

GpsStatus* CGpsProxy::GetGpsStatus()
{
	// 更新数据后，再返回
	UpdateStatus();

	return &m_GpsStatus;
}


GPSInfo* CGpsProxy::GetGpsInfo()
{
	static GPSInfo info;
	memcpy(&info, m_GpsCore.GetGpsInfo(), sizeof(GPSInfo));
	return &info;
}

void CGpsProxy::UpdateStatus()
{
	if(m_GpsCore.IsSVUpdate())
	{
		m_GpsStatus.nSVCnt = min(m_GpsCore.GetGpsInfo()->SV_cnt, GPS_NUM_CH);
		// 将有效数据复制过来, 其它数据置0
		memcpy(m_GpsStatus.svInfo, m_GpsCore.GetGpsStatus(), m_GpsStatus.nSVCnt*sizeof(SVInfo));
		memset(m_GpsStatus.svInfo+m_GpsStatus.nSVCnt, 0, (GPS_NUM_CH-m_GpsStatus.nSVCnt)*sizeof(SVInfo));

		// 排序
		for(int i=0; i<m_GpsStatus.nSVCnt && i<GPS_NUM_CH; i++)
		{
			for (int j=0; j<m_GpsStatus.nSVCnt-i-1; j++)
			{
				if (m_GpsStatus.svInfo[j].SNR < m_GpsStatus.svInfo[j+1].SNR)
				{
					SVInfo temp = m_GpsStatus.svInfo[j];
					m_GpsStatus.svInfo[j] = m_GpsStatus.svInfo[j+1];
					m_GpsStatus.svInfo[j+1] = temp;
				}
			}
		}
	}
}

void CGpsProxy::ResetGps()
{
	// 不包含结尾符'\0'
	m_GpsCore.SendCmdToGPS(ColdCmd, strlen(ColdCmd));
}

int CGpsProxy::IsFixed()
{
	int count = 0;
	GpsStatus* pstatus = GetGpsStatus();

	for(int i=0;i<pstatus->nSVCnt && i<GPS_NUM_CH;i++)
	{
		if(pstatus->svInfo[i].Fix)
		{
			count++;
		}
	}

	return count;
}