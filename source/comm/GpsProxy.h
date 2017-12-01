#pragma once

#include "GpsCore.h"

struct  GpsStatus
{
	SVInfo		svInfo[GPS_NUM_CH];
	int			nSVCnt;
};

class CGpsProxy
{
public:
	CGpsProxy(void);
	virtual ~CGpsProxy(void);

public:
	//start GPS initialize
	BOOL Initialize(HWND hWnd, LPCTSTR strPortL=L"COM1:", DWORD dwBaudrate=9600);

	void Uninitialize();

	//get position 
	GPSInfo* GetGpsInfo();

	//get signal 
	GpsStatus* GetGpsStatus();

	void ResetGps();

	// �˿��Ƿ��Ѿ���λ�ɹ�, ��λ�ɹ���ʱ����Ч(�Ƿ�����������)
	// ���δ��λ������ 0�� �����λ���ˣ� �����ж��ٿ���Ч����
	int IsFixed();

protected:
	//update gps data
	void UpdateStatus();

	GpsStatus m_GpsStatus;

	CGpsCore m_GpsCore;
};
