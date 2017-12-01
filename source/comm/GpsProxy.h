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

	// 此刻是否已经定位成功, 定位成功后，时间有效(是否真是这样？)
	// 如果未定位到返回 0， 如果定位到了， 返回有多少颗有效卫星
	int IsFixed();

protected:
	//update gps data
	void UpdateStatus();

	GpsStatus m_GpsStatus;

	CGpsCore m_GpsCore;
};
