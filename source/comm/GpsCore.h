#pragma once

#include <string>
using namespace std;


//GPS Channels definitions
#define GPS_NUM_CH 20
#define PSEUDO_CH 32

//GPS relate constants
#define Knot2Kmhr 1.8532

//GPS FixType
#define NoFix 0
#define SPS 1
#define DGPS 2
#define Estimate 6

typedef struct SVInfo
{
    int SVid;            // PRN
    int SNR;
    int elv;             // elevation angle : 0~90
    int azimuth;         // azimuth : 0~360
    unsigned char Fix;   // 0:None , 1:FixSV
} SVInfo;

typedef struct ChInfo
{
    int SVid;            // PRN
    int SNR;             // SNR
    unsigned char Status;// Status(0:Idle, 1:Search, 2:Tracking)
} ChInfo;

typedef struct Version
{
    char MCore[30];      // MCore Version
    char Customer[30];   // Customer Version
} Version;

typedef struct GPSInfo
{
	int year;
	int mon;
    int day;
    int hour;
    int min;
    float sec;

    float Lat; // Position, +:E,N -:W,S
    float Lon;
    float Alt;
    unsigned char FixService;  // NoFix:0, SPS:1, DGPS:2, Estimate:6
    unsigned char FixType;     // None:0, 2D:1, 3D:2
    float Speed;  // km/hr
    float Track;  // 0~360
    float PDOP;   //DOP
    float HDOP;
    float VDOP;

    int SV_cnt;
    int fixSV[GPS_NUM_CH];
} GPSInfo;


const int MAX_GPS_OBJECT_R = 1024;
const int MAX_GPS_OBJECT_S = 64;

#include "DEPSerial.h"
class CGpsCore
{
public:
	CGpsCore();
	~CGpsCore();


	BOOL Open(HWND hwnd, LPCTSTR lpszDevice, DWORD dwBaudrate=9600);
	void Close();
	void SendCmdToGPS(const void* lpBuffer, size_t size);

	GPSInfo* GetGpsInfo();
	SVInfo* GetGpsStatus();
	BOOL IsSVUpdate();

protected:
	static void WINAPI Obex_OnReceive(CDEPSerial::EEvent event, LPARAM lParam);
	void OnReceive();
	void InitialGPSData(void);

protected:
	void NMEA_Combine(char *buf);
	void NMEA_Parse(void);

	void Channel_Parse( char *head);
	void Ack_Parse(char *head);
	void GLL_Parse( char *head);
	void RMC_Parse( char *head);

	void VTG_Parse( char *head);
	void GSA_Parse( char *head);
	void GSV_Parse( char *head);
	void GGA_Parse( char *head);
	void ZDA_Parse( char *head);
	void VTm_Parse( char *head);

	void FixSVMapping(void);
protected:
	char*	m_pObexObjectS;
	char*	m_pObexObjectR;
	CDEPSerial		m_depSerial;

	// the window to notify
	HWND m_hwnd;

	// GPS data
	char m_NMEARemain[2048];
	char m_NMEAbuf[2048];   
	GPSInfo m_gpsInfo;
	SVInfo  m_svInfo[GPS_NUM_CH];
	ChInfo  m_chInfo[PSEUDO_CH];
	Version m_FW;
	int  m_Ack;
	int  m_Fail_Ack;
	bool m_fgSVUpdate;
	bool m_fgChUpdate;
	long m_RTCM_Baudrate;
	char m_NMEASetting[75];
	wstring m_strFirmwareVer;
};