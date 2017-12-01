#include "stdafx.h"
#include "tzUtil.h"
#include "filefind.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BOOL tzutil::GetSubString(IN LPCTSTR lpszpData, OUT CStringArray& strArray, const TCHAR chSeparator)
{
	int start_pos = 0;
	int end_pos = -1;
	CString data(lpszpData);
	while ( (end_pos=data.Find(chSeparator, start_pos)) != -1)
	{
		CString sub_string = data.Mid(start_pos, end_pos-start_pos);
		sub_string.Trim();	// 去掉前后空格
		if (sub_string.GetLength() > 0)	// 路径第一个字符为"\"时,第一个子串为空
		{
			strArray.Add(sub_string);
		}
		start_pos = end_pos+1;
	}

	// 只有一个或最后一个没有分号时
	if (start_pos < data.GetLength())
	{
		CString sub_string = data.Mid(start_pos, data.GetLength()-start_pos);
		sub_string.Trim();	// 去掉前后空格
		strArray.Add(sub_string);
	}

	return TRUE;
}

LPCTSTR tzutil::GetAppPath(void)
{
	static TCHAR szAppPath[MAX_PATH] = {_T('\0')};
	if ( GetModuleFileName(GetModuleHandle(NULL), szAppPath, MAX_PATH))
	{
		GetFilePath(szAppPath, szAppPath);
	}

	return szAppPath;
}

BOOL tzutil::LoadApp(LPCWSTR pExeFile, LPCWSTR cmdLine, PROCESS_INFORMATION* pInfo)
{
	PROCESS_INFORMATION pi = {0};
	if (CreateProcess(pExeFile, cmdLine, NULL, NULL, FALSE, 0,NULL, NULL, NULL, &pi))
	{
		if (pInfo)
		{
			*pInfo = pi;
		}
		else
		{
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
		return TRUE;
	}

	return FALSE;
}


BOOL tzutil::GetFilePath(IN LPCTSTR lpszFileFullPath, OUT LPTSTR lpszFilePath)
{
	if ( !lpszFileFullPath || !lpszFilePath )
	{
		return FALSE;
	}

	// return null string if filepath is null.
	if ( lpszFileFullPath[0] == '\0')
	{
		lpszFilePath[0] = 0;
		return TRUE;
	}

	_tcscpy(lpszFilePath, lpszFileFullPath);

	// Find the last '\' character in filepath.
	LPTSTR pos = NULL, temppos = NULL;
	pos = lpszFilePath;
	temppos = _tcsstr(lpszFilePath, _T("\\"));
	while (temppos)
	{
		pos = temppos;
		temppos = _tcsstr(pos+1, _T("\\"));
	}

	*pos = 0;

	return TRUE;
}

BOOL tzutil::GetFileName(IN LPCTSTR lpszFileFullPath, OUT LPTSTR lpszFileName)
{
	if ( !lpszFileFullPath || !lpszFileName )
	{
		return FALSE;
	}

	// return null string if filepath is null.
	if ( lpszFileFullPath[0] == '\0')
	{
		lpszFileName[0] = 0;
		return TRUE;
	}



	// Find the last '\' character in filepath.
	LPCTSTR temppos = _tcsrchr(lpszFileFullPath, _T('\\'));
	if (temppos)
	{
		_tcscpy(lpszFileName, temppos+1);
	}
	else
	{
		_tcscpy(lpszFileName, lpszFileFullPath);
	}


	return TRUE;
}

BOOL tzutil::GetFileExtentName(IN LPCTSTR lpszFileFullPath, OUT LPTSTR lpszFileExtentName)
{
	if ( !lpszFileFullPath || !lpszFileExtentName )
	{
		return FALSE;
	}

	// return null string if filepath is null.
	if ( lpszFileFullPath[0] == L'\0')
	{
		lpszFileExtentName[0] = 0;
		return TRUE;
	}



	// Find the last '.' character in filepath.
	LPCTSTR temppos = _tcsrchr(lpszFileFullPath, _T('.'));
	if (temppos)
	{
		_tcscpy(lpszFileExtentName, temppos+1);
	}
	else
	{
		lpszFileExtentName[0] = 0;
	}


	return TRUE;
}



/*
*****************************************************************
******************** Get MP3 file information *******************
*****************************************************************
*/
const static char MP3_GENRE[128][32] = {
	"Blues","Classic Rock","Country","Dance","Disco","Funk","Grunge",
	"Hip-Hop","Jazz","Metal","New Age","Oldies","Other","Pop","R&B",
	"Rap","Reggae","Rock","Techno","Industrial","Alternative","Ska","Death Metal",
	"Pranks","Soundtrack","Euro-Techno","Ambient","Trip-Hop","Vocal","Jazz+Funk","Fusion",
	"Trance","Classical","Instrumental","Acid","House","Game","Sound Clip","Gospel",
	"Noise","AlternRock","Bass","Soul","Punk","Space","Meditative","Instrumental Pop",
	"Instrumental Rock","Ethnic","Gothic","Darkwave","Techno-Industrial","Electronic","Pop-Folk","Eurodance",
	"Dream","Southern Rock","Comedy","Cult","Gangsta",
	"Top 40","Christian Rap","Pop/Funk","Jungle","Native American","Cabaret","New Wave","Psychadelic","Rave","Showtunes","Trailer","Lo-Fi","Tribal","Acid Punk","Acid Jazz","Polka","Retro","Musical","Rock & Roll","Hard Rock",
	"Folk","Folk-Rock","National Folk","Swing","Fast Fusion","Bebob","Latin","Revival","Celtic","Bluegrass","Avantgarde","Gothic Rock",
	"Progressive Rock","Psychedelic Rock","Symphonic Rock","Slow Rock","Big Band","Chorus","Easy Listening","Acoustic","Humour","Speech","Chanson","Opera",
	"Chamber Music","Sonata","Symphony","Booty Brass","Primus","Porn Groove","Satire","Slow Jam","Club","Tango","Samba","Folklore",
	"Ballad","Poweer Ballad","Rhytmic Soul","Freestyle","Duet","Punk Rock","Drum Solo","A Capela","Euro-House","Dance Hall"

};

BOOL tzutil::GetMP3Tag(const TCHAR *pFileName, MP3_TAG *pMP3_Tag,UINT nCodePage)
{
	const int MP3_TAG_LEN = 128;
	memset(pMP3_Tag, 0, sizeof(MP3_TAG));

	if( GetMp3TagV2(pFileName, pMP3_Tag, nCodePage) )	// V2优先，跟windows一样
	{
		return TRUE;
	}

	FILE *fp = _tfopen(pFileName, _T("rb"));
	if( NULL == fp )
	{
		TRACE(_T("open file %s fail.\n "));
		return FALSE;
	}
	fseek(fp, 0, SEEK_END);
	if ( ftell(fp) <= MP3_TAG_LEN )	// file is too small
	{
		TRACE(_T("File is too small, no tag informaiton.\n"));
		fclose(fp);
		return FALSE;
	}

	// read the tag data from file.
	char buf[MP3_TAG_LEN];
	fseek(fp, -MP3_TAG_LEN, SEEK_END);
	if( ferror(fp) )
	{
		fclose(fp);
		return FALSE;
	}

	size_t iread = fread(buf, MP3_TAG_LEN, 1, fp);
	if( ferror(fp) || iread != 1 )
	{
		fclose(fp);
		return FALSE;
	}

	// invalid tag data
	if(buf[0] != 'T' || buf[1] != 'A' || buf[2] != 'G')
	{
		TRACE(_T("the file doesn't contain tag information.\n"));
		fclose(fp);
		
		return FALSE;
	}

    
	// convert string and copy it to output buffer 
	MultiByteToWideChar(nCodePage, 0, buf+3, 30, (LPWSTR)pMP3_Tag->_title, 30);//ChineseGB 936
	MultiByteToWideChar(nCodePage, 0, buf+33, 30, (LPWSTR)pMP3_Tag->_artist, 30);
	MultiByteToWideChar(nCodePage, 0, buf+63, 30, (LPWSTR)pMP3_Tag->_album, 30);
	MultiByteToWideChar(nCodePage, 0, buf+93, 4, (LPWSTR)pMP3_Tag->_year, 4);
	MultiByteToWideChar(nCodePage, 0, buf+97, 30, (LPWSTR)pMP3_Tag->_comment, 30);
	char *p = (char*)MP3_GENRE[(byte)(*(buf+127))];
	MultiByteToWideChar(nCodePage, 0, p, (int)strlen(p), (LPWSTR)pMP3_Tag->_genre, 32);

	fclose(fp);
	return TRUE;
}


struct ID3_HEAD
{
	BYTE Header[3];    /*必须为"ID3"否则认为标签不存在*/
	BYTE Ver;    /*版本号ID3V2.3就记录3*/
	BYTE Revision;    /*副版本号此版本记录为0*/
	BYTE Flag;    /*存放标志的字节，这个版本只定义了三位，稍后详细解说*/
	BYTE Size[4];    /*标签大小，包括标签头的10个字节和所有的标签帧的大小*/
};

struct ID3_FRAME 
{
	BYTE FrameID[4];  /*用四个字符标识一个帧，说明其内容，稍后有常用的标识对照表*/
	BYTE Size[4];   /*帧内容的大小，不包括帧头，不得小于1*/ 
	BYTE Flags[2];   /*存放标志，只定义了6位，稍后详细解说*/
};

// lpszFileName -- mp3 file name
// ppBuffer -- the buffer in which the image data are saved.
// pSize  -- pointer to the buffer size
BOOL tzutil::GetMp3Image(LPCTSTR lpszFileName, OUT BYTE **ppBuffer, OUT int *pSize)
{
	BOOL bRet = FALSE;
	DWORD dwFileSize = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	ID3_HEAD head;
	ID3_FRAME *pframe;
	DWORD dwTotalSize;		// 标签大小，包括标签头的10字节
	DWORD bytes_read;
	BYTE *pFrameBuffer = NULL;		// hold all frame data
	DWORD offset = 0;

	hFile = CreateFile(lpszFileName,GENERIC_READ, 
		FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		goto EXIT;
	}

	dwFileSize=GetFileSize(hFile,NULL);//获取文件字节数
	if (dwFileSize == INVALID_FILE_SIZE) 
	{
		goto EXIT;
	}

	if(!ReadFile(hFile, &head, sizeof(head), &bytes_read, NULL) 
		|| bytes_read != sizeof(head))
	{
		goto EXIT;
	}

	if (memcmp(head.Header, "ID3", 3) != 0 )	// no id3
	{
		goto EXIT;
	}

	dwTotalSize = (head.Size[0]&0x7F)*0x200000 + (head.Size[1]&0x7F)*0x4000
		+ (head.Size[2]&0x7F)*0x80 +(head.Size[3]&0x7F);

	if (dwTotalSize <= 0)
	{
		goto EXIT;
	}

	pFrameBuffer = (BYTE*)malloc(dwTotalSize);
	if (pFrameBuffer == NULL)
	{
		goto EXIT;
	}

	// read frame data
	if(!ReadFile(hFile, pFrameBuffer, dwTotalSize, &bytes_read, NULL) 
		|| bytes_read != dwTotalSize)
	{
		goto EXIT;
	}

	while (offset + sizeof(ID3_FRAME) < dwTotalSize )	// 余下的字节数已经不够一帧数据了
	{
		pframe = (ID3_FRAME*)(pFrameBuffer+offset);
		DWORD nFrameSize;
		nFrameSize =  (pframe->Size[0]<<24) + (pframe->Size[1]<<16)
			+ (pframe->Size[2]<<8) + pframe->Size[3];
		if (nFrameSize == 0 || nFrameSize > dwTotalSize-offset-sizeof(ID3_FRAME))	// data error
		{
			goto EXIT;
		}

		if (memcmp(pframe->FrameID, "APIC", 4) == 0)	// 专辑封面
		{
			*ppBuffer = (BYTE*)malloc(nFrameSize-14);
			if (*ppBuffer == NULL)
			{
				goto EXIT;
			}

			BYTE byFlag = 0;
			byFlag = *(pFrameBuffer+offset+sizeof(ID3_FRAME));

			int nHeadLen = 13;
			UCHAR uc;
			WORD w;

			if(1 == byFlag)
			{
				while(1)
				{
					memcpy(&w, pFrameBuffer+offset+sizeof(ID3_FRAME) + nHeadLen, 2);

					nHeadLen += 2;

					if(0 == w)
					{	
						break;
					}
				}
			}
			else
			{
				while(1)
				{
					memcpy(&uc, pFrameBuffer+offset+sizeof(ID3_FRAME) + nHeadLen, 1);

					nHeadLen++;

					if(0 == uc)
					{	
						break;
					}
				}
			}

			memset(*ppBuffer, 0, nFrameSize-nHeadLen);
			memcpy(*ppBuffer, pFrameBuffer+offset+sizeof(ID3_FRAME)+nHeadLen, nFrameSize-nHeadLen);
			*pSize = nFrameSize-nHeadLen;
			bRet = TRUE;
			break;
		}
		else
		{
			offset += nFrameSize + sizeof(ID3_FRAME);
		}
	}


EXIT:
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}
	if (pFrameBuffer)
	{
		free(pFrameBuffer);
		pFrameBuffer = NULL;
	}

	return bRet;
}

BOOL tzutil::GetMp3TagV2(LPCTSTR lpszFileName, OUT MP3_TAG* pMP3_Tag,UINT nCodePage)
{
	if(NULL == pMP3_Tag)
	{
		return FALSE;
	}

	BOOL bRet = FALSE;
	DWORD dwFileSize = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	ID3_HEAD head;
	ID3_FRAME *pframe;
	DWORD dwTotalSize;		// 标签大小，包括标签头的10字节
	DWORD bytes_read;
	BYTE *pFrameBuffer = NULL;		// hold all frame data
	DWORD offset = 0;


	int nInfoCnt = 0;
	int nLen = 0;
	int nCnt = 0;
	char szBuf[64] = {0};

	hFile = CreateFile(lpszFileName,GENERIC_READ, 
		FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		goto EXIT;
	}

	dwFileSize=GetFileSize(hFile,NULL);//获取文件字节数
	if (dwFileSize == INVALID_FILE_SIZE) 
	{
		goto EXIT;
	}

	if(!ReadFile(hFile, &head, sizeof(head), &bytes_read, NULL) 
		|| bytes_read != sizeof(head))
	{
		goto EXIT;
	}

	if (memcmp(head.Header, "ID3", 3) != 0 )	// no id3
	{
		goto EXIT;
	}

	dwTotalSize = (head.Size[0]&0x7F)*0x200000 + (head.Size[1]&0x7F)*0x4000
		+ (head.Size[2]&0x7F)*0x80 +(head.Size[3]&0x7F);

	if (dwTotalSize <= 0 )
	{
		goto EXIT;
	}

	pFrameBuffer = (BYTE*)malloc(dwTotalSize);
	if (pFrameBuffer == NULL)
	{
		goto EXIT;
	}

	// read frame data
	if(!ReadFile(hFile, pFrameBuffer, dwTotalSize, &bytes_read, NULL) 
		|| bytes_read != dwTotalSize)
	{
		goto EXIT;
	}
	while (offset + sizeof(ID3_FRAME) < dwTotalSize)	// 余下的字节数已经不够一帧数据了
	{
		pframe = (ID3_FRAME*)(pFrameBuffer+offset);
		DWORD nFrameSize;
		nFrameSize =  (pframe->Size[0]<<24) + (pframe->Size[1]<<16)
			+ (pframe->Size[2]<<8) + pframe->Size[3];
		if (nFrameSize == 0 || nFrameSize > dwTotalSize-offset-sizeof(ID3_FRAME))	// data error
		{
			goto EXIT;
		}

		if(memcmp(pframe->FrameID, "TIT2", 4) == 0)	// 标题
		{
			nLen = nFrameSize < 60 ? nFrameSize:60;

			memcpy(szBuf, pFrameBuffer + offset + sizeof(ID3_FRAME) , nLen);
			szBuf[nLen] = 0;

			DecodeTextV2(szBuf, nLen, pMP3_Tag->_title, sizeof(pMP3_Tag->_title)/sizeof(TCHAR),nCodePage);

			nInfoCnt++;
		}
		else if(memcmp(pframe->FrameID, "TPE1", 4) == 0)//　作者
		{
			nLen = nFrameSize<60 ? nFrameSize:60;
			memcpy(szBuf, pFrameBuffer + offset + sizeof(ID3_FRAME), nLen);
			szBuf[nLen] = 0;

			DecodeTextV2(szBuf, nLen, pMP3_Tag->_artist, sizeof(pMP3_Tag->_title)/sizeof(TCHAR),nCodePage);

			nInfoCnt++;
		}
		else if(memcmp(pframe->FrameID, "TALB", 4) == 0)// 专辑
		{
			
			nLen = nFrameSize<60 ? nFrameSize:60;
			memcpy(szBuf, pFrameBuffer + offset + sizeof(ID3_FRAME), nLen);
			szBuf[nLen] = 0;

			DecodeTextV2(szBuf, nLen, pMP3_Tag->_album, sizeof(pMP3_Tag->_title)/sizeof(TCHAR),nCodePage);

			nInfoCnt++;
		}
		else if(memcmp(pframe->FrameID, "TYER", 4) == 0)// 年代
		{
		}
		else if(memcmp(pframe->FrameID, "TCON", 4) == 0)// 类型
		{
		}
		else if(memcmp(pframe->FrameID, "COMM", 4) == 0)	// 备注
		{
		}

		offset += nFrameSize + sizeof(ID3_FRAME);
		if(nInfoCnt >= 3)
		{
			break;
		}
	}

EXIT:
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}
	if (pFrameBuffer)
	{
		free(pFrameBuffer);
		pFrameBuffer = NULL;
	}

	if(nInfoCnt > 0)
	{
		bRet = TRUE;
	}

	return bRet;
}

BOOL tzutil::DecodeTextV2(IN char* pText, int nLen, OUT TCHAR* pDst, int nMaxDstCnt, UINT nCodePage)
{
	int nCnt = 0;
	if(0 == pText[0] || 3 == pText[0])	// 3为UTF8编码
	{
		TCHAR *pBuffer = new TCHAR[nLen+1];
		nCnt = MultiByteToWideChar((0 == pText[0]) ? nCodePage : CP_UTF8, 0, pText+1, -1, (LPWSTR)pBuffer, nLen);//ChineseGB 936
		if(nCnt >= 0)
		{
			pBuffer[nCnt] = 0;
		}
		_tcsncpy(pDst, pBuffer, nMaxDstCnt);
		pDst[nMaxDstCnt-1] = L'\0';
		delete pBuffer;
	}
	else if(1==pText[0])
	{
		if(0xff ==(UCHAR)pText[1] && 0xfe==(UCHAR)pText[2])
		{
			memset(pDst, 0, nMaxDstCnt*sizeof(TCHAR));
			memcpy(pDst, pText+3, nLen-3);
		}
		else if(0xfe==(UCHAR)pText[1] && 0xff==(UCHAR)pText[2])
		{
			memset(pDst, 0, nMaxDstCnt*sizeof(TCHAR));
			int i=3;
			int j=0;
			for(i=3; i<nLen; i+=2)
			{
				pDst[j++] = ((UCHAR)pText[i+1]) + ((UCHAR)pText[i]<<8);
			}
		}
		else
		{
			memset(pDst, 0, nMaxDstCnt);
			memcpy(pDst, pText+1, nLen-1);
		}
	}
	else if(2==pText[0])
	{
		memset(pDst, 0, nMaxDstCnt*sizeof(TCHAR));
		int i=1;
		int j=0;
		for(i=1; i<nLen; i+=2)
		{
			pDst[j++] = (pText[i+1]<<8) + pText[i];
		}
	}
	else
	{
		pDst[0] = 0;
	}

	return TRUE;
}

BOOL tzutil::GetRegValue(HKEY hKey, LPCTSTR lpSubKey, LPCWSTR lpValueName, OUT LPBYTE lpszData, IN OUT LPDWORD lpcbData)
{
	LONG result = -1;
	HKEY hmykey = 0;
	if(ERROR_SUCCESS == RegOpenKeyEx(hKey, lpSubKey, 0, 0, &hmykey))
	{
		result = RegQueryValueEx(hmykey, lpValueName, NULL, NULL, lpszData, lpcbData);
		RegCloseKey(hmykey);
	}

	return result == ERROR_SUCCESS;
}

int tzutil::UTF8ToUnicode(LPCSTR pszSrc, LPWSTR pszDst)
{
	if ((pszSrc == NULL) || (pszDst == NULL))
	{
		return 0;
	}

	int nSize = MultiByteToWideChar(CP_UTF8, 0, pszSrc, -1, NULL, 0);
	return MultiByteToWideChar(CP_UTF8, 0, pszSrc, -1, pszDst, nSize);
}
void tzutil::CopyScreen(HBITMAP hbmp, LPRECT prcSrc)
{
	HDC hdcSrc = ::CreateDC(L"DISPLAY", NULL, NULL, NULL);
	HDC hdcDest = ::CreateCompatibleDC(NULL);
	HBITMAP oldbmp = (HBITMAP)::SelectObject(hdcDest, hbmp);
	::BitBlt(hdcDest, 0, 0, _W(prcSrc), _H(prcSrc), hdcSrc, prcSrc->left, prcSrc->top, SRCCOPY);
	::SelectObject(hdcDest, oldbmp);
	DeleteDC(hdcDest);
}

int tzutil::_W(LPRECT prc)
{
	if (prc)
	{
		return prc->right - prc->left;
	}
	else
	{
		return 0;
	}
}

int tzutil::_H(LPRECT prc)
{
	if (prc)
	{
		return prc->bottom - prc->top;
	}
	else
	{
		return 0;
	}
}




BOOL tzutil::GetTimeZoneInformation(LPTSTR buffer)
{
	BOOL ret = FALSE;
	TIME_ZONE_INFORMATION tzi;
	::GetTimeZoneInformation(&tzi);

	// enumurate
	int i = 0;
	HKEY hkey;
	TCHAR reg_name[128];
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Time Zones"), 0, 0, &hkey);
	DWORD len = 128;
	while (ERROR_SUCCESS == RegEnumKeyEx(hkey, i++, reg_name, &len,NULL,NULL,NULL,NULL))
	{
		CRegKey key;
		key.Open(hkey, reg_name);
		TCHAR stdName[32];
		ULONG len2 = 32;
		key.QueryStringValue(L"Std", stdName, &len2);
		if (_tcscmp(stdName, tzi.StandardName) == 0)
		{
			ret = TRUE;
			_tcscpy(buffer, reg_name);
			break;
		}
		len = 128;
	}
	RegCloseKey(hkey);

	return ret;
}


typedef struct sysutil_tagTZREG {
	LONG    Bias;
	LONG    StandardBias;
	LONG    DaylightBias;
	SYSTEMTIME StandardDate;
	SYSTEMTIME DaylightDate;
} sysutil_TZREG;

// BOOL tzutil::SetTimeZoneInformation(LPCTSTR lpszTimeZoneRegname)
// {
// 	BOOL ret = FALSE;
// 	// write sys reg
// 	CRegKey key;
// 	CString reg_name(L"Time Zones\\");
// 	reg_name += lpszTimeZoneRegname;
// 	if ( ERROR_SUCCESS == key.Open(HKEY_LOCAL_MACHINE, reg_name))
// 	{
// 		BYTE value[128];
// 		DWORD len = 128;
// 		if (ERROR_SUCCESS == key.QueryBinaryValue(L"TZI", &value, &len))
// 		{
// 			if (len == sizeof(sysutil_TZREG))
// 			{
// 				sysutil_TZREG *ptz = (sysutil_TZREG*)value;
// 				TIME_ZONE_INFORMATION tzi;
// 
// 				tzi.Bias = ptz->Bias;
// 				tzi.StandardBias = ptz->StandardBias;
// 				tzi.DaylightBias = ptz->DaylightBias;
// 				tzi.StandardDate = ptz->StandardDate;
// 				tzi.DaylightDate = ptz->DaylightDate;
// 				ULONG len1 = sizeof(tzi.DaylightName);
// 				ULONG len2 = sizeof(tzi.StandardName);
// 				key.QueryStringValue(L"Dlt", tzi.DaylightName, &len1);
// 				key.QueryStringValue(L"Std", tzi.StandardName, &len2);
// 
// 				ret = ::SetTimeZoneInformation(&tzi);
// 			}
// 		}
// 	}
// 
// 	return ret;
// }

BOOL tzutil::GetTziByRegistry(IN LPCTSTR lpszRegistryName, OUT TIME_ZONE_INFORMATION* ptzi)
{
	BOOL ret = FALSE;
	// write sys reg
	CRegKey key;
	CString reg_name(L"Time Zones\\");
	reg_name += lpszRegistryName;
	if ( ERROR_SUCCESS == key.Open(HKEY_LOCAL_MACHINE, reg_name))
	{
		BYTE value[128];
		DWORD len = 128;
		if (ERROR_SUCCESS == key.QueryBinaryValue(L"TZI", &value, &len))
		{
			if (len == sizeof(sysutil_TZREG))
			{
				sysutil_TZREG *ptz = (sysutil_TZREG*)value;
				if (ptzi)
				{
					ptzi->Bias = ptz->Bias;
					ptzi->StandardBias = ptz->StandardBias;
					ptzi->DaylightBias = ptz->DaylightBias;
					ptzi->StandardDate = ptz->StandardDate;
					ptzi->DaylightDate = ptz->DaylightDate;
					ULONG len1 = sizeof(ptzi->DaylightName);
					ULONG len2 = sizeof(ptzi->StandardName);
					key.QueryStringValue(L"Dlt", ptzi->DaylightName, &len1);
					key.QueryStringValue(L"Std", ptzi->StandardName, &len2);

					ret = TRUE;
				}

			}
		}
	}

	return ret;
}
BOOL tzutil::GetRegistryByTzi(OUT LPTSTR lpszRegistryName, IN const TIME_ZONE_INFORMATION* ptzi)
{
	ASSERT(ptzi);
	BOOL ret = FALSE;

	// enumurate
	int i = 0;
	HKEY hkey;
	TCHAR reg_name[128];
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Time Zones"), 0, 0, &hkey);
	DWORD len = 128;
	while (ERROR_SUCCESS == RegEnumKeyEx(hkey, i++, reg_name, &len,NULL,NULL,NULL,NULL))
	{
		CRegKey key;
		key.Open(hkey, reg_name);
		TCHAR stdName[32];
		ULONG len2 = 32;
		key.QueryStringValue(L"Std", stdName, &len2);
		if (_tcscmp(stdName, ptzi->StandardName) == 0)
		{
			ret = TRUE;
			_tcscpy(lpszRegistryName, reg_name);
			break;
		}
		len = 128;
	}
	RegCloseKey(hkey);

	return ret;
}

BOOL tzutil::IsAlphabet(TCHAR ch)
{
	return ( (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z'));
}

TCHAR tzutil::GetCharPYCode(TCHAR ch)
{
	TCHAR result;
	BYTE chTrans[2];

	if (ch >= L'a' && ch <= L'z')
	{
		result = ch - L'a' + L'A'; 
	}
	else if (ch >= L'A' && ch <= L'Z')
	{
		result = ch;
	}
	else if (ch >= 0x4e00 && ch <= 0x9FA5)
	{
		memset(chTrans,0,sizeof(chTrans));

		WideCharToMultiByte(936,NULL,&ch,1,(LPSTR)chTrans,2,NULL,NULL);

		//WORD iCnChar = ((chTrans[0] - 160) * 100) + (chTrans[1] - 160); //计算区位码
		WORD iCnChar = ((WORD)chTrans[0] << 8) | (chTrans[1]);
		if ((iCnChar >= 45217) && (iCnChar <= 45252))
		{
			result = L'A';
		}
		else if ((iCnChar >= 45253) && (iCnChar <= 45760))
		{
			result = L'B';
		}
		else if ((iCnChar >= 45761) && (iCnChar <= 46317))
		{
			result = L'C';
		}
		else if ((iCnChar >= 46318) && (iCnChar <= 46825))
		{
			result = L'D';
		}
		else if ((iCnChar >= 46826) && (iCnChar <= 47009))
		{
			result = L'E';
		}
		else if ((iCnChar >= 47010) && (iCnChar <= 47296))
		{
			result = L'F';
		}
		else if ((iCnChar >= 47297) && (iCnChar <= 47613))
		{
			result = L'G';
		}
		else if ((iCnChar >= 47614) && (iCnChar <= 48118))
		{
			result = L'H';
		}
		else if ((iCnChar >= 48119) && (iCnChar <= 49061))
		{
			result = L'J';
		}
		else if ((iCnChar >= 49062) && (iCnChar <= 49323))
		{
			result = L'K';
		}
		else if ((iCnChar >= 49324) && (iCnChar <= 49895))
		{
			result = L'L';
		}
		else if ((iCnChar >= 49896) && (iCnChar <= 50370))
		{
			result = L'M';
		}

		else if ((iCnChar >= 50371) && (iCnChar <= 50613))
		{
			result = L'N';
		}
		else if ((iCnChar >= 50614) && (iCnChar <= 50621))
		{
			result = L'O';
		}
		else if ((iCnChar >= 50622) && (iCnChar <= 50905))
		{
			result = L'P';
		}
		else if ((iCnChar >= 50906) && (iCnChar <= 51386))
		{
			result = L'Q';
		}
		else if ((iCnChar >= 51387) && (iCnChar <= 51445))
		{
			result = L'R';
		}
		else if ((iCnChar >= 51446) && (iCnChar <= 52217))
		{
			result = L'S';
		}
		else if ((iCnChar >= 52218) && (iCnChar <= 52697))
		{
			result = L'T';
		}
		else if ((iCnChar >= 52698) && (iCnChar <= 52979))
		{
			result = L'W';
		}
		else if ((iCnChar >= 52980) && (iCnChar <= 53640))
		{
			result = L'X';
		}
		else if ((iCnChar >= 53689) && (iCnChar <= 54480))
		{
			result = L'Y';
		}
		else if ((iCnChar >= 54481) && (iCnChar <= 55289))
		{
			result = L'Z';
		}
	}
	else
	{
		result = L'#';
	}

	return result;
}

// 当是英语时词时,需用空格分隔
// 识别英文词算法策略：如果当前为英文字符,则上一个字符为字母时，该字符为单词中的字母，否则为首字符 
LPCTSTR tzutil::GetStringPYCode(LPCTSTR lpszString)
{
	const int MAX_COUNT = 8;
	static TCHAR py[MAX_COUNT+1];
	py[0] = L'\0';
	int count = 0;
	BOOL bIsAlphabet = FALSE;
	if (lpszString)
	{
		for (int i=0; i<(int)_tcslen(lpszString); i++)
		{
			if (bIsAlphabet && IsAlphabet(lpszString[i]))
			{
				continue;	// 前一字符和当前字符都是英文字符,该字符不需处理
			}

			if (lpszString[i] == L' ')	// 忽略空格
			{
				bIsAlphabet = FALSE;
				continue;
			}

			bIsAlphabet = IsAlphabet(lpszString[i]);
			py[count++] = GetCharPYCode(lpszString[i]);
			if (count >= MAX_COUNT)
			{
				break;
			}
		}

		py[count] = L'\0';
	}

	return py;
}


struct TZ_WNDINFO
{
	DWORD dwProcessId;
	HWND hWnd;
};

static BOOL CALLBACK WndEnumProc(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	TZ_WNDINFO* pInfo = (TZ_WNDINFO*)lParam;
	if(dwProcessId == pInfo->dwProcessId)
	{
		pInfo->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

HWND tzutil::GetProcessMainWnd(DWORD dwProcessId)
{
	TZ_WNDINFO wi;
	wi.dwProcessId = dwProcessId;
	wi.hWnd = NULL;
	EnumWindows(WndEnumProc,(LPARAM)&wi);
	return wi.hWnd;
}


static BOOL CALLBACK WEP_SetForeground(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if(dwProcessId == (DWORD)lParam)
	{
		::ShowWindow(hWnd, SW_SHOW);
		::SetForegroundWindow(hWnd);
		return FALSE;
	}
	return TRUE;
}

BOOL tzutil::SetProcessForeground(DWORD dwPorcessID)
{
	return EnumWindows(WEP_SetForeground,(LPARAM)dwPorcessID);
}

BOOL tzutil::IsBoolEqual(BOOL a, BOOL b)
{
	return ((a && b) || (!a && !b));
}

LPCTSTR tzutil::Int2Ascii(int n)
{
	static TCHAR buffer[16];
	buffer[0] = L'\0';
	_stprintf(buffer, L"%d", n);
	return buffer;
}

BOOL tzutil::GetWMATag(const TCHAR *pFileName, MP3_TAG * pWMA_Tag)
{
	ASSERT(pFileName != NULL);
	ASSERT(_tcslen(pFileName) < MAX_PATH);

	memset(pWMA_Tag,NULL,sizeof(MP3_TAG));
	BOOL bRet = FALSE;
	//CFile file;
	DWORD   num = 0;
	HANDLE   hFile = NULL;
	if((hFile = CreateFile(pFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL)) !=
		INVALID_HANDLE_VALUE)
	{
		const BYTE btWma[16] = {0x30,0x26,0xB2,0x75,0x8E,0x66,0xCF,0x11,0xA6,0xD9,0x00,0xAA,0x00,0x62,0xCE,0x6C};
		BYTE btCheckWma[16];
		::ReadFile(hFile ,btCheckWma ,16 ,NULL ,NULL);
		// is wma tag?
		if(memcmp(btCheckWma, btWma, 16) == 0)
		{
			printf("###File is Wma \n");
			__int64 llTagsize = 0;
			::ReadFile(hFile , &llTagsize ,8 ,NULL ,NULL);//get head size
			const BYTE btTag_head[16]= {0x33,0x26,0xB2,0x75,0x8E,0x66,0xCF,0x11,0xA6,0xD9,0x00,0xAA,0x00,0x62,0xCE,0x6C};
			const BYTE btExTag_head[16] = {0x40,0xA4,0xD0,0xD2,0x07,0xE3,0xD2,0x11,0x97,0xF0,0x00,0xA0,0xC9,0x5E,0xA8,0x50};

			::SetFilePointer(hFile ,30 ,NULL ,FILE_BEGIN);
			::SetFilePointer(hFile ,0, (PLONG)&num ,FILE_CURRENT);//Get the cur pos
			while((__int64)num < llTagsize)
			{
				LONG llSize = 0;
				::ReadFile(hFile,btCheckWma ,16 ,NULL ,NULL);
				::ReadFile(hFile ,&llSize ,8 ,NULL ,NULL);
				if(llSize == 0)
				{
					break;
				}
				// is tag head ?
				if(memcmp(btCheckWma, btTag_head, 16) == 0)
				{
					int nTagSize[5] = {0};
					TCHAR szTemp[100] = _T("");

					for(int i=0; i<5; i++)
					{
						::ReadFile(hFile ,&nTagSize[i] ,2 ,NULL ,NULL);
					}
					for(int i=0; i<5; i++)
					{
						if(nTagSize[i] >= 100)
						{
							break;
						}
						::ReadFile(hFile,(BYTE*)szTemp ,nTagSize[i] ,NULL ,NULL);
						switch(i) 
						{
						case 0: // title
							_tcscpy(pWMA_Tag->_title, szTemp);
							if(_tcsicmp(pWMA_Tag->_title,L"\0") != 0)
							{
								bRet = TRUE;
							}
							break;
						case 1: // artist
							_tcscpy(pWMA_Tag->_artist, szTemp);
							if(_tcsicmp(pWMA_Tag->_title,L"\0") != 0)
							{
								bRet = TRUE;
							}
							break;
						default:
							break;
						}
					}
				}
				else if(memcmp(btCheckWma, btExTag_head, 16) == 0) // is extend tag head?
				{
					int nExNo = 0;
					int nFlag = 0;
					int nValueSize = 0;
					::ReadFile(hFile , &nExNo ,2 ,NULL ,NULL);

					for(int i=0; i<nExNo; i++)
					{
						int nExSize = 0;
						TCHAR szExName[100] = _T("");
						TCHAR szExValue[100] = _T("");
						::ReadFile(hFile ,&nExSize ,2 ,NULL ,NULL);
						if(nExSize == 0 || nExSize > 100)
						{
							break;
						}

						::ReadFile(hFile ,szExName ,nExSize,NULL ,NULL);
						::ReadFile(hFile ,&nFlag ,2,NULL ,NULL);
						::ReadFile(hFile ,&nValueSize ,2,NULL ,NULL);

						if(nFlag == 3)
						{
							::SetFilePointer(hFile ,4 ,NULL ,FILE_CURRENT);
						}
						else
						{
							// ablum
							if(_tcsicmp(szExName, _T("WM/AlbumTitle")) == 0)
							{
								::ReadFile(hFile ,szExValue ,nValueSize,NULL ,NULL);
								wprintf(_T("Alubm\n"));
								_tcscpy(pWMA_Tag->_album, szExValue);

								if(_tcsicmp(pWMA_Tag->_album,L"\0") != 0)
								{
									//pFileTag->bTag = TRUE;
									bRet = TRUE;
								}
							}
							//Year
							else if (_tcsicmp(szExName,_T("WM/Year")) == 0)
							{
								::ReadFile(hFile,szExName,nValueSize,NULL ,NULL);
								wprintf(_T("Years\n"));
								_tcscpy(pWMA_Tag->_year,szExName);

								if(_tcsicmp(pWMA_Tag->_year,L"\0") != 0)
								{
									bRet = TRUE;
								}
							}
							//TrackNum
							else if (_tcsicmp(szExName,_T("WM/TrackNumber")) == 0)
							{
								::ReadFile(hFile,szExName,nValueSize,NULL ,NULL);
								TCHAR   track[10];
								int j= 0;
								for (int i=0; i< nValueSize;)
								{
									if (szExName[i] >= _T('\0') &&szExName[i] <= _T('9'))
									{
										track[j] = szExName[i];
										j++;
									}
									i++;
								}
								int k = _ttoi(track);
								if (k > 0)
								{
									//pWMA_Tag->_TrackID =k;
									printf("Track ID is %d\n",k);
								}
							}
							else if (_tcsicmp(szExName,_T("WM/Genre")) == 0)
							{
								::ReadFile(hFile,szExName,nValueSize,NULL ,NULL);
								wprintf(_T("Genre is %s\n"),szExName);
								_tcscpy(pWMA_Tag->_genre,szExName);
								if(_tcsicmp(pWMA_Tag->_genre,L"\0") != 0)
								{
									bRet = TRUE;
								}
							}
							else if (_tcsicmp(szExName,_T("WM/Picture")) == 0)
							{
								//pWMA_Tag->_havecover = TRUE;
								::SetFilePointer(hFile,nValueSize,0,FILE_CURRENT);
							}
							else
							{
								::SetFilePointer(hFile ,nValueSize ,NULL ,FILE_CURRENT);
							}
						}
					}
				}// end for else if
				else
				{
					::SetFilePointer(hFile , llSize-24 ,NULL ,FILE_CURRENT);
				}
			}
		}
	}		
	CloseHandle(hFile);
	return bRet;
}

BOOL tzutil::DeleteDirectory(LPCTSTR lpszFilePath)
{
	CFileFind finder;
	CString strWildcard(lpszFilePath);
	strWildcard += L"\\*.*";

	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile(); 
		if (finder.IsDirectory() && !finder.IsDots())
		{
			DeleteDirectory(finder.GetFilePath());		// call it recursively
		}
		else
		{
			DeleteFile(finder.GetFilePath());
		}
	}
	finder.Close();

	RemoveDirectory(lpszFilePath);
	return TRUE;
}