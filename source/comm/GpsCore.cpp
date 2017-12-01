#include "StdAfx.h"
#include "gpscore.h"
#include "interface.h"

void FixSVMapping(void);
bool FetchField(char *start, char *result);
void NMEA_Parse(void);


static BOOL MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize)
{
   // Get the required size of the buffer that receives the Unicode 
   // string. 
   DWORD dwMinSize;
   dwMinSize = MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, NULL, 0);
  
   if(dwSize < dwMinSize)
   {
     return FALSE;
   }    
   // Convert headers from ASCII to Unicode.
   MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);  
   return TRUE;
}

BOOL static WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize)
{
   DWORD dwMinSize;
   dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE);
   if(dwSize < dwMinSize)
   {
    return FALSE;
   }
   WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwSize,NULL,FALSE);
   return TRUE;
}

void CGpsCore::InitialGPSData(void)
{
	memset( &m_gpsInfo, NULL, sizeof(GPSInfo));
	memset( &m_svInfo[0], NULL, sizeof(m_svInfo));
	memset( &m_FW, NULL, sizeof(Version));
	memset( &m_chInfo[0], NULL, sizeof(m_chInfo));
	memset( m_NMEASetting, NULL, sizeof(m_NMEASetting));
	memset( m_NMEAbuf, NULL, sizeof(m_NMEAbuf));
	memset( m_NMEARemain, NULL, sizeof(m_NMEARemain));

	m_fgChUpdate=false;
	m_Ack=0;
	m_Fail_Ack = 0;
	m_fgSVUpdate=false;
	m_RTCM_Baudrate=2;		
}

//the Sentence availability-----------------------------------------------------------------------
static unsigned char CheckSum(char *buf, int size)
{
   int i;
   char chksum=0, chksum2=0;

   if(size < 5)
      return false;

   chksum = buf[1];
   for(i = 2; i < (size - 2); i++)
   {
      if(buf[i] != '*')
      {
        chksum ^= buf[i];
      }
      else
      {
        if(buf[i + 1] >= 'A')
        {
          chksum2 = (buf[i+1]-'A'+10)<<4;
        }
        else
        {
          chksum2 = (buf[i+1]-'0')<<4;
        }

        if(buf[i + 2] >= 'A')
        {
          chksum2 += buf[i+2]-'A'+10;
        }
        else
        {
          chksum2 += buf[i+2]-'0';
        }
        break;
      }
    }

   /* if not found character '*' */
   if(i == (size - 2))
   {
      return (false);
   }

   if(chksum == chksum2)
   {
     return (true);
   }
   else
   {
     return (false);
   }
}
//---------------------------------------------------------------------------
static bool FetchField(char *start, char *result)
{
   char *end;

   if(start == NULL)
      return false;

   end = strstr( start, ",");
   // the end of sentence
   if(end == NULL)
      end = strstr(start, "*");

   if(end-start>0)
   {
     strncpy( result, start, end-start);
     result[end-start]='\0';
   }
   else   // no data
   {
     result[0]='\0';
     return false;
   }

   return true;
}
//NEMA FORMAT PARSE
//---------------------------------------------------------------------------
void CGpsCore::Channel_Parse( char *head)
{
   char *start, result[20], tmp[20];

   // check checksum
   if(CheckSum(head, strlen(head)))
   {
      start = strstr( head, ",");
      if(start != NULL)
         start = start +1;
      else
         return;

      for(int i=0; i<PSEUDO_CH ; i++)
      {
         if(!FetchField( start, result))
            return;

         // SVid
         strncpy( tmp, result, 2);
         tmp[2]='\0';
         m_chInfo[i].SVid = atoi(tmp);

         // SNR
         strncpy( tmp, result+2, 2);
         tmp[2]='\0';
         m_chInfo[i].SNR = atoi(tmp);

         // Status
         if(result[4]=='0')
            m_chInfo[i].Status = 0;
         else if(result[4]=='1')
            m_chInfo[i].Status = 1;
         else
            m_chInfo[i].Status = 2;

         start = strstr( start, ",");
         if(start != NULL)
            start = start +1;
         else                // find '*'
            break;
      }
      m_fgChUpdate = true;
   }
}

void CGpsCore::Ack_Parse(char *head)
{
	int CmdAck=0, type=0;
	char *start, result[20];
	TCHAR strTemp[256];
	wstring str;
	// check checksum
	if(CheckSum(head, strlen(head)))
	{
		if(strncmp(head, "$PMTK001", 8)==0)
		{
			start = strstr( head, ",")+1;
			FetchField( start, result);
			CmdAck = atoi(result);
			start = strstr( start, ",")+1;
			FetchField( start, result);
			type = atoi(result);
			// Success
			if(type==3)
			{
				m_Ack = CmdAck;
			}
			else if(type ==2)
			{
				m_Fail_Ack = CmdAck;
			}	
		}
		else if(strncmp(head, "$PMTK500", 8)==0)
		{
			int Fix_Int=0;
			int FixRate=0;
			start = strstr(head, ",")+1;
			FetchField( start, result);
			Fix_Int = atoi(result);
			if(Fix_Int <= 1000)
            FixRate = 1000/Fix_Int;
			memset(strTemp,0,sizeof(TCHAR)*256);
			swprintf(strTemp,L"%d",FixRate);
			str = strTemp;
		//	str.Format(_T("%d"),FixRate);	
		}
		else if(strncmp(head, "$PMTK501", 8)==0)
		{
			int DGPSMode=0;
			start = strstr(head, ",")+1;
			FetchField( start, result);
			DGPSMode = atoi(result);
		}
		else if(strncmp(head, "$PMTK514", 8)==0)
		{
			start = strstr(head, ",")+1;
			strcpy(m_NMEASetting, start);
			
		}
		else if(strncmp(head, "$PMTK519", 8)==0)
		{
			int IntegrityMode=0;
			start = strstr(head, ",")+1;
			FetchField( start, result);
			IntegrityMode = atoi(result);		
		}
		else if(strncmp(head, "$PMTK530", 8)==0)
		{
			int Datum_index;
			start = strstr(head, ",")+1;
			FetchField( start, result);
			Datum_index = atoi(result);
		}
		else if(strncmp(head, "$PMTK556", 8)==0)
		{
			float TCXO=1000;
			start = strstr(head, ",")+1;
			start = strstr(start, ",")+1;
			FetchField( start, result);
			TCXO = float(atof(result) * 1e6);
		}
		else if(strncmp(head, "$PMTK590", 8)==0)
		{
			int flash_times=0;
			start = strstr(head, ",")+1;
			FetchField( start, result);
			memset(strTemp,0,sizeof(TCHAR)*256);
			mbstowcs(strTemp,result,20);
			str=strTemp;
		}
		else if(strncmp(head, "$PMTK702", 8)==0)
		{
			if(strncmp(head, "$PMTK702,1,0,", 13)==0)
			{
				start = strstr(head, ",")+1;
				start = strstr(start, ",")+1;
				start = strstr(start, ",")+1;
				FetchField( start, result);
				m_RTCM_Baudrate=atol(result);
				//RTCM_Update
				memset(strTemp,0,sizeof(TCHAR)*256);
				mbstowcs(strTemp,result,20);
				str=strTemp;
			}
		    else
			{
				m_RTCM_Baudrate = 0;
			}
		}
		else if(strncmp(head, "$PMTK705", 8)==0)
		{
			// MCore Version
			start = strstr( head, ",")+1;
			FetchField( start, m_FW.MCore);
			// Customer Version
			start = strstr( start, ",")+1;
			FetchField( start, m_FW.Customer);
		
			memset(strTemp,0,sizeof(TCHAR)*256);
			TCHAR strTemp1[256],strTemp2[256];
			memset(strTemp1,0,sizeof(TCHAR)*256);
			memset(strTemp2,0,sizeof(TCHAR)*256);
			mbstowcs(strTemp1,m_FW.MCore,30);	
			mbstowcs(strTemp2,m_FW.Customer,30);
			swprintf(strTemp,L"%s %s",strTemp1,strTemp2);
			m_strFirmwareVer = strTemp;
			::PostMessage(m_hwnd, UI_MSG_GPS_FIRMWARE_VERSION, 0, 0);
		}
	}
}
//---------------------------------------------------------------------------
void CGpsCore::GLL_Parse( char *head)
{
   // $GPGLL,2446.367638,N,12101.356226,E,144437.000,A,A*56
   char *start, result[20], tmp[20], *point;
   int len=0;
   char FixService;

   // check checksum
   if(CheckSum(head, strlen(head)))
   {
      // Position(Lat)
      start = strstr( head, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField(start, result))
      {
         point = strstr( result, ".");
         len = (point-2)-result;
         strncpy(tmp, result, len);
         tmp[len]='\0';
         m_gpsInfo.Lat = float(atof(tmp));
         strncpy(tmp, result+len, strlen(result)-len);
         tmp[strlen(result)-len]='\0';
         m_gpsInfo.Lat += float(atof(tmp)/60.0);
      }

      // N or S
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField(start, result))
      {
         if(*result=='S')
            m_gpsInfo.Lat = -m_gpsInfo.Lat;
      }

      // Position(Lon)
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField(start, result))
      {
         point = strstr( result, ".");
         len = (point-2)-result;
         strncpy(tmp, result, len);
         tmp[len]='\0';
         m_gpsInfo.Lon = float(atof(tmp));
         strncpy(tmp, result+len, strlen(result)-len);
         tmp[strlen(result)-len]='\0';
         m_gpsInfo.Lon += float(atof(tmp)/60.0);
      }

      // E or W
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         if(*result=='W')
            m_gpsInfo.Lon = -m_gpsInfo.Lon;
      }

      // UTC Time
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         // Hour
         strncpy( tmp, result, 2);
         tmp[2]='\0';
         m_gpsInfo.hour = atoi(tmp);
         // Min
         strncpy( tmp, result+2, 2);
         tmp[2]='\0';
         m_gpsInfo.min = atoi(tmp);
         // Sec
         strncpy( tmp, result+4, strlen(result)-4);
         tmp[strlen(result)-4]='\0';
         m_gpsInfo.sec = float(atof(tmp));
      }   

      // The positioning system Mode Indicator and Status fields shall not be null fields.
      // Data valid
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;

      if(!FetchField( start, result))
         return;
      
      if(*result=='A')
      {
         // Fix Type
         if(m_gpsInfo.FixType == 0)
            m_gpsInfo.FixType = 1;   // Assume 2D, if there's no other info.

         // Fix Service
         start = strstr( start, ",");
         if(start != NULL)
             start = start +1;
         else
            return;

         if(!FetchField( start, result))
            return;

         FixService = *result;

         switch(FixService)
         {
            case 'A':
            {
               m_gpsInfo.FixService = 1;
               break;
            }
            case 'D':
            {
               m_gpsInfo.FixService = 2;
               break;
            }
            case 'E':
            {
               m_gpsInfo.FixService = 6;
               break;
            }
         }
      }
      else // Data invalid
      {
         m_gpsInfo.FixType = 0;    // NoFix
         m_gpsInfo.FixService = 0; // NoFix
      }
   }
}
//---------------------------------------------------------------------------
void CGpsCore::RMC_Parse( char *head)
{
   // $GPRMC,073446.000,A,2446.3752,N,12101.3708,E,0.002,22.08,121006,,,A*6C

   char *start, result[20], tmp[20], *point;
   int len=0;

   // check checksum
   if(CheckSum(head, strlen(head)))
   {
      // UTC time : 161229.487
      start = strstr( head, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, tmp))
      {
         // Hour
         strncpy( result, tmp, 2);
         result[2]='\0';
         m_gpsInfo.hour = atoi(result);
         // Min
         strncpy( result, tmp+2, 2);
         result[2]='\0';
         m_gpsInfo.min = atoi(result);
         // Sec
         strncpy( result, tmp+4, strlen(tmp)-4);
         result[strlen(tmp)-4]='\0';
         m_gpsInfo.sec = float(atof(result));
      }

      // valid
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(!FetchField( start, result))
         return;

      if(*result == 'A')
      {
         if(m_gpsInfo.FixType == 0)
            m_gpsInfo.FixType = 1;      // Assume 2D

         if(m_gpsInfo.FixService == 0)
            m_gpsInfo.FixService = 1;   // Assume SPS
      }
      else
      {
         m_gpsInfo.FixType = 0;    // NoFix
         m_gpsInfo.FixService = 0; // NoFix
      }

      // Position(Lat) : 3723.2475(N)
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         point = strstr( result, ".");
         len = (point-2)-result;
         strncpy(tmp, result, len);
         tmp[len]='\0';
         m_gpsInfo.Lat = float(atoi(tmp));
         strncpy(tmp, result+len, strlen(result)-len);
         tmp[strlen(result)-len]='\0';
         m_gpsInfo.Lat += float(atof(tmp)/60.0);
      }
	  else  //Can not fetch Lat field
	  {
	     m_gpsInfo.Lat = 0;
	  }

      // N or S
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result) && m_gpsInfo.Lat!=0)
      {
         if(*result=='S')
            m_gpsInfo.Lat = -m_gpsInfo.Lat;
      }

      // Position(Lon) : 12158.3416(W)
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         point = strstr( result, ".");
         len = (point-2)-result;
         strncpy(tmp, result, len);
         tmp[len]='\0';
         m_gpsInfo.Lon = float(atoi(tmp));
         strncpy(tmp, result+len, strlen(result)-len);
         tmp[strlen(result)-len]='\0';
         m_gpsInfo.Lon += float(atof(tmp)/60.0);
      }
	  else  //Can not fetch Lat field
	  {
	     m_gpsInfo.Lon = 0;
	  }

      // E or W
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result) && m_gpsInfo.Lat!=0)
      {
         if(*result=='W')
            m_gpsInfo.Lon = -m_gpsInfo.Lon;
      }

      // Speed : 0.13
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         m_gpsInfo.Speed = float(atof(result) * Knot2Kmhr);
      }

      // Track : 309.62
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         m_gpsInfo.Track = float(atof(result));
      }

      // Date : 120598
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         // Day
         strncpy(tmp, result, 2);
         tmp[2]='\0';
         m_gpsInfo.day=atoi(tmp);

         // Month
         strncpy(tmp, result+2, 2);
         tmp[2]='\0';
         m_gpsInfo.mon=atoi(tmp);

         // Year
         strncpy(tmp, result+4, 2);
         tmp[2]='\0';
         m_gpsInfo.year=atoi(tmp)+2000;
      }

      // skip Magnetic variation
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;

      // mode indicator
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;

      if(!FetchField( start, result))
         return;

      if(m_gpsInfo.FixType > 0)
      {
         switch(result[0])
         {
            case 'A':
            {
               m_gpsInfo.FixService = 1;
               break;
            }
            case 'D':
            {
               m_gpsInfo.FixService = 2;
               break;
            }
            case 'E':
            {
               m_gpsInfo.FixService = 6;
               break;
            }
         }
      }
   }
}
//---------------------------------------------------------------------------
void CGpsCore::VTm_Parse( char *head)
{
   //$GPVTG,159.16,T,,M,0.013,N,0.023,K,A*34
   char *start, result[20];
   char FixService;

   // check checksum
   if(CheckSum(head, strlen(head)))
   {
      // Track
      start = strstr( head, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         m_gpsInfo.Track = float(atof(result));
      }

      // ignore
      start = strstr( start, ",");     // T
      if(start != NULL)
         start = start +1;
      else
         return;

      start = strstr( start, ",");     // NULL
      if(start != NULL)
         start = start +1;
      else
         return;

      start = strstr( start, ",");     // M
      if(start != NULL)
         start = start +1;
      else
         return;

      // Speed
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         m_gpsInfo.Speed = float(atof(result) * Knot2Kmhr);
      }

      // ignore
      start = strstr( start, ",");     // N
      if(start != NULL)
         start = start +1;
      else
         return;

      start = strstr( start, ",");     // 0.023
      if(start != NULL)
         start = start +1;
      else
         return;

      start = strstr( start, ",");     // K
      if(start != NULL)
         start = start +1;
      else
         return;

      // Fix Service
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;

      if(!FetchField( start, result))
         return;
      FixService = *result;

      if(FixService != 'N')
      {
         if(m_gpsInfo.FixType == 0)
            m_gpsInfo.FixType = 1;  //Assume 2D

         switch(FixService)
         {
            case 'A':
            {
               m_gpsInfo.FixService = 1;
               break;
            }
            case 'D':
            {
               m_gpsInfo.FixService = 2;
               break;
            }
            case 'E':
            {
               m_gpsInfo.FixService = 6;
               break;
            }
         }
      }
      else // NoFix
      {
         m_gpsInfo.FixType = 0;    // NoFix
         m_gpsInfo.FixService = 0; // NoFix
      }
   }
}
//---------------------------------------------------------------------------
void CGpsCore::GSA_Parse( char *head)
{
   // $GPGSA,A,3,03,19,27,23,13,16,15,11,07,,,,1.63,0.95,1.32*03
   char *start, result[20];
   int sv_cnt=0, i;

   if(CheckSum(head, strlen(head)))
   {
      //Fix SV
      memset(&m_gpsInfo.fixSV, NULL, sizeof(m_gpsInfo.fixSV));

      //Valid
      start = strstr( head, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(!FetchField( start, result))
         return;

      if((*result == 'A') || (*result == 'M'))
      {
         // Fix Type
         start = strstr( start, ",");
         if(start != NULL)
            start = start +1;
         else
            return;
         if(!FetchField( start, result))
            return;
         m_gpsInfo.FixType = atoi(result)-1;

         if(m_gpsInfo.FixType > 0)          // Fix
         {
            if(m_gpsInfo.FixService == 0)
               m_gpsInfo.FixService = 1;    //Assume SPS FixSerivce
         }
         else
         {
            m_gpsInfo.FixType = 0;    // NoFix
            m_gpsInfo.FixService = 0; // NoFix
         }
      }
      else
      {
         m_gpsInfo.FixType = 0;    // NoFix
         m_gpsInfo.FixService = 0; // NoFix
      }

      for(i=0 ; i<12 ; i++)
      {
         start = strstr( start, ",");
         if(start != NULL)
            start = start +1;
         else
            return;

         FetchField( start, result);

         if(strlen(result)>0)
            m_gpsInfo.fixSV[sv_cnt++] = atoi(result);
      }

      //PDOP
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(!FetchField( start, result))
         return;
      m_gpsInfo.PDOP = float(atof(result));

      //HDOP
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(!FetchField( start, result))
         return;
      m_gpsInfo.HDOP = float(atof(result));

      //VDOP
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(!FetchField( start, result))
         return;
      m_gpsInfo.VDOP = float(atof(result));
   }
}
//---------------------------------------------------------------------------
void CGpsCore::GSV_Parse( char *head)
{
   // $GPGSV,3,1,09,03,63,020,43,19,76,257,37,27,14,320,30,23,39,228,37*79
   // $GPGSV,3,2,09,13,38,274,38,16,31,058,37,15,16,055,34,11,16,192,32*76
   // $GPGSV,3,3,09,07,15,043,26*40

   char *start, result[20];
   int sv_cnt=0, base=0, i;

   // check checksum
   if(CheckSum(head, strlen(head)))
   {
      // ignore
      start = strstr( head, ",");
      if(start != NULL)
         start = start +1;
      else
         return;

      //first Message
      if(*(start+2)=='1')
      {
         memset( &m_svInfo, NULL, sizeof(m_svInfo));
         m_fgSVUpdate = false;
      }

      // Last Message
      if(*start == *(start+2))
         m_fgSVUpdate = true;

      //base
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(!FetchField( start, result))
         return;
      base = (atoi(result)-1)*4;

      //total
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(!FetchField( start, result))
         return;
      m_gpsInfo.SV_cnt = atoi(result);
      if(m_gpsInfo.SV_cnt == 0)
      {
         return;
      }

      for( i=0 ; i<4 ; i++)
      {
         //SVid
         start = strstr( start, ",");
         if(start != NULL)
            start = start +1;
         else
            return;
         FetchField( start, result);
         if(strlen(result)>0)
            m_svInfo[base+sv_cnt].SVid = atoi(result);
         else
            m_svInfo[base+sv_cnt].SVid = 0;

         //elev
         start = strstr( start, ",");
         if(start != NULL)
            start = start +1;
         else
            return;
         FetchField( start, result);
         if(strlen(result)>0)
            m_svInfo[base+sv_cnt].elv = atoi(result);
         else
            m_svInfo[base+sv_cnt].elv = 0;

         //azimuth
         start = strstr( start, ",");
         if(start != NULL)
            start = start +1;
         else
            return;
         FetchField( start, result);
         if(strlen(result)>0)
            m_svInfo[base+sv_cnt].azimuth = atoi(result);
         else
            m_svInfo[base+sv_cnt].azimuth = 0;

         //SNR
         start = strstr( start, ",");
         if(start != NULL)
            start = start +1;
         else
            return;
         if(*start == '*')
            m_svInfo[base+sv_cnt].SNR = 0;
         else
         {
            FetchField( start, result);
            if(strlen(result)>0)
               m_svInfo[base+sv_cnt].SNR = atoi(result);
            else
               m_svInfo[base+sv_cnt].SNR = 0;
         }

         sv_cnt++;

         if(base+sv_cnt == m_gpsInfo.SV_cnt)
            break;
      }
   }
}
//---------------------------------------------------------------------------
void CGpsCore::GGA_Parse( char *head)
{
   //$GPGGA,144437.000,2446.367638,N,12101.356226,E,1,9,0.95,155.696,M,15.057,M,,*58
   char *start, result[20], tmp[20], *point;
   int len=0;

   // check checksum
   if(CheckSum(head, strlen(head)))
   {
      // UTC time : 144437.000
      start = strstr( head, ",");
      if(start != NULL)
         start = start +1;
      else
         return;

      if(FetchField( start, result))
      {
         // Hour
         strncpy( tmp, result, 2);
         tmp[2]='\0';
         m_gpsInfo.hour = atoi(tmp);
         // Min
         strncpy( tmp, result+2, 2);
         tmp[2]='\0';
         m_gpsInfo.min = atoi(tmp);
         // Sec
         strncpy( tmp, result+4, strlen(result)-4);
         tmp[strlen(result)-4]='\0';
         m_gpsInfo.sec = float(atof(tmp));
      }

      // Position(Lat)
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         point = strstr( result, ".");
         len = (point-2)-result;
         strncpy(tmp, result, len);
         tmp[len]='\0';
         m_gpsInfo.Lat = float(atoi(tmp));
         strncpy(tmp, result+len, strlen(result)-len);
         tmp[strlen(result)-len]='\0';
         m_gpsInfo.Lat += float(atof(tmp)/60.0);
      }

      // N or S
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         if(*result=='S')
            m_gpsInfo.Lat = -m_gpsInfo.Lat;
      }

      // Position(Lon)
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         point = strstr( result, ".");
         len = (point-2)-result;
         strncpy(tmp, result, len);
         tmp[len]='\0';
         m_gpsInfo.Lon = float(atoi(tmp));
         strncpy(tmp, result+len, strlen(result)-len);
         tmp[strlen(result)-len]='\0';
         m_gpsInfo.Lon += float(atof(tmp)/60.0);
      }

      // E or W
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         if(*result=='W')
            m_gpsInfo.Lon = -m_gpsInfo.Lon;
      }

      //GPS Fix Type and Service
      // 0: NoFix, 1:SPS, 2:DGPS, 6:Estimate
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(!FetchField( start, result))
         return;
      m_gpsInfo.FixService = atoi(result);

      // Fix
      if(m_gpsInfo.FixService > 0)
      {
         if(m_gpsInfo.FixType == 0)
            m_gpsInfo.FixType = 1; // Assume 2D
      }

      start = strstr( start, ",");   // Number of SV in use , ex :9
      if(start != NULL)
         start = start +1;
      else
         return;

      // HDOP
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         m_gpsInfo.HDOP = float(atof(result));
      }

      //Altitude (mean sea level)
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
      {
         m_gpsInfo.Alt = float(atof(result));
      }

      //Altitude unit (bypass)
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;

      //Altitude (Geoidal separation)
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      
      if(FetchField( start, result))
      {
         m_gpsInfo.Alt += float(atof(result));
      }   
   }
}
//---------------------------------------------------------------------------
void CGpsCore::ZDA_Parse( char *head)
{
   // $GPZDA,000007.123,06,01,2000,,*50
   char *start, result[20], tmp[20];

   // check checksum
   if(CheckSum(head, strlen(head)))
   {
      // UTC time : 000007.123
      start = strstr( head, ",");
      if(start != NULL)
         start = start +1;
      else
         return;

      if(FetchField( start, result))
      {
         // Hour
         strncpy( tmp, result, 2);
         tmp[2]='\0';
         m_gpsInfo.hour = atoi(tmp);

         // Min
         strncpy( tmp, result+2, 2);
         tmp[2]='\0';
         m_gpsInfo.min = atoi(tmp);

         // Sec
         strncpy( tmp, result+4, strlen(result)-4);
         tmp[strlen(result)-4]='\0';
         m_gpsInfo.sec = float(atof(tmp));
      }

      // Day
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
         m_gpsInfo.day = atoi(result);

      // Month
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
         m_gpsInfo.mon = atoi(result);

      // Year
      start = strstr( start, ",");
      if(start != NULL)
         start = start +1;
      else
         return;
      if(FetchField( start, result))
         m_gpsInfo.year = atoi(result);
   }
}

void CGpsCore::FixSVMapping(void)
{
   int i, j;

   //Clear
   for(i=0 ; i<GPS_NUM_CH ; i++)
   {
      m_svInfo[i].Fix = 0;
   }

   for(i=0 ; i<m_gpsInfo.SV_cnt ; i++)
   {
      j=0;
      while(m_gpsInfo.fixSV[j] > 0)
      {
         if(m_svInfo[i].SVid == m_gpsInfo.fixSV[j])
         {
            m_svInfo[i].Fix = 1;
            break;
         }
         j++;
      }
   }
}

int SVidSort( const void *a, const void *b)
{
   SVInfo *sv1, *sv2;

   sv1 = (SVInfo *)a;
   sv2 = (SVInfo *)b;

   if(sv1->SVid >= sv2->SVid)
      return 1;
   else
      return -1;
}

void GenerateCkSum(char *buf, char *result)
{
   char chksum;
   int i,size;

   chksum = buf[1];
   size = strlen(buf);

   for(i=2; i<size ; i++)
      chksum ^= buf[i];

   sprintf(result, "%02X", chksum);
}
bool GenerateFullCmd(char *buf, UINT len)
{
   char CkSum[3];

   ASSERT( strlen(buf) < len-5 );

   if ( strlen(buf) < len-5)
   {
	  GenerateCkSum(buf,CkSum);
	  strcat(buf,"*");
	  strcat(buf,CkSum);
      strcat(buf,"\r\n");
	  return true;
   }
   else
   {
	  return false;
   }
}



/////////////////////////////////////////////////////////////////////////

CGpsCore::CGpsCore()
: m_pObexObjectR(NULL),
m_pObexObjectS(NULL)
{
	InitialGPSData();
}

CGpsCore::~CGpsCore()
{

}

BOOL CGpsCore::Open(HWND hwnd, LPCTSTR lpszDevice, DWORD dwBaudrate/* 9600 */)
{
	if( m_pObexObjectR != NULL || m_pObexObjectS != NULL )
	{
		TRACE(L"CGpsCore::Open - it has been intialized.\n");
		return FALSE;
	}

	// Allocate memory for sending buffer and receiving buffer.
	m_pObexObjectS = (char*)malloc(MAX_GPS_OBJECT_S+1);
	if( m_pObexObjectS == NULL )
	{
		TRACE(L"CObexLD::Init - No sufficient memroy.\n");
		return FALSE;
	}
	memset(m_pObexObjectS, 0, MAX_GPS_OBJECT_S);

	m_pObexObjectR = (char*)malloc(MAX_GPS_OBJECT_R+1);
	if( m_pObexObjectR == NULL )
	{
		TRACE(L"CObexLD::Init - No sufficient memroy.\n");
		free(m_pObexObjectS);
		m_pObexObjectS = NULL;
		return FALSE;
	}
	memset(m_pObexObjectR, 0, MAX_GPS_OBJECT_R);

	// initialize dep
	LONG result = m_depSerial.Open(lpszDevice);
	if (result != ERROR_SUCCESS)
	{
		free(m_pObexObjectS);
		m_pObexObjectS = NULL;
		free(m_pObexObjectR);
		m_pObexObjectR = NULL;
		return FALSE;
	}

	COMMTIMEOUTS cto;
	memset(&cto, 0 , sizeof(cto));
	cto.ReadIntervalTimeout = 50;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;
	m_depSerial.SetupReadTimeouts(CDEPSerial::EReadTimeoutCustom, &cto);

	m_depSerial.RegCallback(Obex_OnReceive, (LPARAM)this);

	m_hwnd = hwnd;
	return TRUE;
}

void CGpsCore::Close()
{
	// Close DEP first.
	m_depSerial.Close();

	// Free allocated memory.
	if( m_pObexObjectS )
	{
		free(m_pObexObjectS);
		m_pObexObjectS = NULL;
	}

	if( m_pObexObjectR )
	{
		free(m_pObexObjectR);
		m_pObexObjectR = NULL;
	}
}

void CGpsCore::Obex_OnReceive(CDEPSerial::EEvent event, LPARAM lParam)
{
	if( event == CDEPSerial::EEventRecv )
	{
		CGpsCore *pObex = (CGpsCore*)lParam;
		pObex->OnReceive();
	}
}

void CGpsCore::OnReceive()
{
	DWORD dwRead;
	m_depSerial.Read(m_pObexObjectR, MAX_GPS_OBJECT_R, &dwRead);
	if (dwRead > 0)
	{
		m_pObexObjectR[dwRead] = '\0';

		// NMEA combine
		NMEA_Combine(m_pObexObjectR);
		// NMEA parsing
		NMEA_Parse();		
		FixSVMapping();

		if (m_hwnd)
		{
			::PostMessage(m_hwnd, UI_MSG_GPS_STATUS_UPDATE, 0, 0);
		}

	}
}

void CGpsCore::NMEA_Combine(char *buf)
{
	char *GP, *lastGP, *ptr;

	// avoid overflowing while no NMEA(GP) sentences output
	if(strlen(m_NMEARemain) > sizeof(m_NMEARemain) * 0.75)
		memset(m_NMEARemain, NULL, sizeof(m_NMEARemain));

	memset(m_NMEAbuf,NULL,sizeof(m_NMEAbuf));
	strcpy(m_NMEAbuf, m_NMEARemain);
	strcat(m_NMEAbuf, buf);

	//initialize
	GP = lastGP = NULL;
	ptr = m_NMEAbuf;

	// there must be GP sentences, couldn't change to PMTK setences
	// because there aren't always PMTK sentences.
	while((GP=strstr( ptr, "$GP")) != NULL)
	{
		lastGP = GP;
		ptr = GP+1;
	}

	while((GP=strstr( ptr, "$PMTK")) != NULL)
	{
		lastGP = GP;
		ptr = GP+1;
	}

	if(lastGP != NULL)
	{
		strcpy(m_NMEARemain, lastGP);
		*(lastGP) = '\0';
	}
	// lastGP == NULL means NMEAbuf doesn't contain any NMEA sentence
}


void CGpsCore::NMEA_Parse(void)
{
	char *GP;
	char type[4];
	char TempBuf[2048];
	memset(TempBuf,0,sizeof(char)*2048);
	strcpy(TempBuf, m_NMEAbuf);
	GP=strtok(TempBuf, "\r\n");

	if(GP == NULL)
		return;
	do
	{
		// Channel Status
		if(strncmp(GP, "$PMTKCHN", 8) == 0)
		{
			Channel_Parse(GP);
		}
		//Ack Parse
		else if(strncmp(GP, "$PMTK", 5) == 0)
		{
			Ack_Parse(GP);
		}
		// NMEA Parse
		else if((strncmp(GP, "$GP", 3) == 0) && (strlen(GP) > 10))
		{
			// skip "$GP" char to fetch Message Type
			strncpy ( type, GP+3, 3);
			type[3]='\0';
			if(strcmp( type, "GLL")==0)
			{
				GLL_Parse( GP);
			}
			else if(strcmp( type, "RMC")==0)
			{
				RMC_Parse( GP);
			}
			else if(strcmp( type, "VTG")==0)
			{
				VTm_Parse( GP);
			}
			else if(strcmp( type, "GSA")==0)
			{
				GSA_Parse( GP);
			}
			else if(strcmp( type, "GSV")==0)
			{
				GSV_Parse( GP);
			}
			else if(strcmp( type, "GGA")==0)
			{
				GGA_Parse( GP);
			}
			else if(strcmp( type, "ZDA")==0)
			{
				ZDA_Parse( GP);
			}
		}
	}while( (GP = strtok( NULL, "\r\n")) != NULL );
}

GPSInfo* CGpsCore::GetGpsInfo()
{
	return &m_gpsInfo;
}

SVInfo* CGpsCore::GetGpsStatus()
{
	return &m_svInfo[0];
}

BOOL CGpsCore::IsSVUpdate()
{
	return m_fgSVUpdate;
}

void CGpsCore::SendCmdToGPS(const void* lpBuffer, size_t size)
{
	m_depSerial.Write(lpBuffer, size);
}