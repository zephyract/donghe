#include "stdafx.h"
#include "LyricParser.h"
#include <Mlang.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CLyricParser::CLyricParser()
: m_uCodePage(CP_ACP)
{
}

CLyricParser::~CLyricParser()
{
	UnInitialize();
}

BOOL CLyricParser::Initialize(LPCTSTR lpszFileName)
{
	UnInitialize();
	m_uCodePage = TestCodePage(lpszFileName);


	if (ParseFile(lpszFileName))
	{
		SortLyric();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	 
}

void CLyricParser::UnInitialize()
{
	memset(&m_head, 0, sizeof(LYRIC_HEAD));
	for (int i=0; i<m_lyric.GetCount(); i++)
	{
		delete m_lyric[i];
	}
	m_lyric.RemoveAll();
	m_uCodePage = CP_ACP;
}

// BOOL CLyricParser::Is_nobom_utf8(const unsigned char *str)
// {
// 	int i = 0;
// 	int size = strlen((const char*)str);
// 
// 	printf("size = %d\r\n",size);
// 	while(i < size)
// 	{
// 		int step = 0;
// 		if((str[i] & 0x80) == 0x00)
// 		{
// 			step = 1;
// 		}
// 		else if((str[i] & 0xe0) == 0xc0)
// 		{
// 			if(i + 1 >= size) return FALSE;
// 			if((str[i + 1] & 0xc0) != 0x80) return FALSE;
// 
// 			step = 2;
// 		}
// 		else if((str[i] & 0xf0) == 0xe0)
// 		{
// 			if(i + 2 >= size) return FALSE;
// 			if((str[i + 1] & 0xc0) != 0x80) return FALSE;
// 			if((str[i + 2] & 0xc0) != 0x80) return FALSE;
// 
// 			step = 3;
// 		}
// 		else
// 		{
// 			return FALSE;
// 		}
// 
// 		i += step;
// 	}
// 
// 	if(i == size) return TRUE;
// 
// 
// 	return FALSE;
// }


BOOL CLyricParser::Is_nobom_utf8(const unsigned char * str)
{
	int i = 0;
	int size = strlen((const char*)str);

//	printf("size = %d\r\n",size);
	while(i < size)
	{
		int step = 0;
		if((str[i] & 0x80) == 0x00)
		{
			step = 1;
		}
		else if((str[i] & 0xe0) == 0xc0)
		{
			if(i + 1 >= size) return FALSE;
			if((str[i + 1] & 0xc0) != 0x80) return FALSE;

			step = 2;
		}
		else if((str[i] & 0xf0) == 0xe0)
		{
			if(i + 2 >= size) return FALSE;
			if((str[i + 1] & 0xc0) != 0x80) return FALSE;
			if((str[i + 2] & 0xc0) != 0x80) return FALSE;

			step = 3;
		}
		else
		{
			return FALSE;
		}

		i += step;
	}

	if(i == size) return TRUE;


	return FALSE;
}


BOOL CLyricParser::ParseFile(LPCTSTR lpszFileName)
{
	const int BUFF_SIZE = 1024;
	char szTemp[BUFF_SIZE];
	CString buffer;
	FILE *fp = NULL;
	if(lpszFileName == NULL
		|| (fp=_tfopen (lpszFileName, _T("rb"))) == NULL)
	{
		return FALSE;
	}
	if( fread(szTemp, 1, 3, fp) != 3 )
	{
		fclose(fp);
		return FALSE;
	}

	int nCode = 0;
	if(szTemp[0]==((char)0xFF) && szTemp[1]==((char)0xFE))
	{
		nCode = 1;	// UNICODE
		fseek(fp, 2, SEEK_SET);
	}
	else if(szTemp[0]==((char)0xEF) && szTemp[1]==((char)0xBB) && szTemp[2]==((char)0xBF))
	{
		nCode = 2;	// UTF8
		fseek(fp, 3, SEEK_SET);
	}
	else
	{
		nCode = 0;	// ASCII
		fseek(fp, 0, SEEK_SET);
	}


	while(!feof(fp))
	{
		memset(szTemp, 0, sizeof(szTemp));

		if (nCode == 0)	// ASCII
		{
			fgets(szTemp, BUFF_SIZE, fp);
			if(Is_nobom_utf8((const unsigned char *)szTemp))
			{
				// UTF8 no bom
				MultiByteToWideChar( CP_UTF8,  0,  szTemp,  -1,  buffer.GetBuffer(BUFF_SIZE), BUFF_SIZE); 
				buffer.ReleaseBuffer();
			}
			else
			{			
				MultiByteToWideChar( m_uCodePage,  MB_COMPOSITE,  szTemp,  -1,  buffer.GetBuffer(BUFF_SIZE), BUFF_SIZE); 
				buffer.ReleaseBuffer();
			}

		} 
		else if(nCode == 1)	// UNICODE
		{
			fgetws((LPTSTR)szTemp, BUFF_SIZE, fp);
			buffer = (LPTSTR)szTemp;
		}
		else if (nCode == 2)	// UTF8, not support completely
		{
			fgets(szTemp, BUFF_SIZE, fp);
			MultiByteToWideChar( /*m_uCodePage*/CP_UTF8,  0,  szTemp,  -1,  buffer.GetBuffer(BUFF_SIZE), BUFF_SIZE); 
			buffer.ReleaseBuffer();
		}
		// cut \r\n in the end of the string
		buffer.TrimRight();

		// to get better performance, if we have parsed a line including lyric, 
		// we then don't consider the following lines including head info.
		if (m_lyric.GetCount()<=0)
		{
			ParseLine(buffer, TRUE);
			continue;
		}
		else
		{
			ParseLine(buffer, FALSE);
		}
	}

	fclose( fp );
	return (m_lyric.GetCount()>0);
}

void CLyricParser::SetHeadInfo(int nIndex, LPCTSTR lpszInfo)
{
	const int MAX_HEAD_CHARS = 32;
	if (nIndex>=0 && nIndex<4 && lpszInfo)
	{
		switch(nIndex)
		{
		case 0:
			_tcsncpy(m_head.title, lpszInfo, MAX_HEAD_CHARS-1);
			m_head.title[MAX_HEAD_CHARS-1] = _T('\0');
			break;
		case 1:
			_tcsncpy(m_head.artist, lpszInfo, MAX_HEAD_CHARS-1);
			m_head.artist[MAX_HEAD_CHARS-1] = _T('\0');
			break;
		case 2:
			_tcsncpy(m_head.album, lpszInfo, MAX_HEAD_CHARS-1);
			m_head.album[MAX_HEAD_CHARS-1] = _T('\0');
			break;
		case 3:
			_tcsncpy(m_head.author, lpszInfo, MAX_HEAD_CHARS-1);
			m_head.author[MAX_HEAD_CHARS-1] = _T('\0');
			break;
		default:
			break;
		}
	}
}

BOOL CLyricParser::ParseLine(LPCTSTR lpszLine, BOOL bCheckHead)
{
	TCHAR *LYRIC_HEAD_TAG[] = {_T("[ti:"), _T("[ar:"), _T("[al:"), _T("[by:") };
	CString strLine(lpszLine);
	int posStart = 0;
	int posEnd = 0;
	//*** parse head info
	if (bCheckHead)
	{
		for (int i=0; i<4; i++)
		{
			posStart = strLine.Find(LYRIC_HEAD_TAG[i]);
			if (posStart != -1)	// found the tag
			{
				posEnd = strLine.Find(_T(']'));
				if (posEnd != -1 && posEnd>posStart)
				{
					CString data = strLine.Mid(posStart+4, posEnd-posStart-4);
					SetHeadInfo(i, data);
					return TRUE;
				} 
				else	// error format
				{
					return FALSE;
				}
			}
		}
	}

	//*** parse lyric
	// get the lyric
	if ((posStart = strLine.ReverseFind(_T(']'))) == -1)		// no ']' found, error format
	{
		return FALSE;
	}
	CString strLyric = strLine.Mid(posStart+1);

	// get time info and add to lyric array.
	BOOL bRet = FALSE;
	posEnd = 0;
	posStart = strLine.Find(_T('['), posEnd);
	while (posStart != -1)
	{
		posEnd = strLine.Find(_T(']'), posStart);
		if (posEnd != -1)
		{
			CString data = strLine.Mid(posStart+1, posEnd-posStart-1);
			data.TrimLeft();
			data.TrimRight();
			int time = String2Time(data);
			if (time>=0)
			{
				LYRIC* lyric = new LYRIC();
				lyric->time = time;
				_tcsncpy(lyric->szLyric, strLyric, MAX_LYRIC_LENGTH-1);
				lyric->szLyric[MAX_LYRIC_LENGTH-1] = _T('\0');
				m_lyric.Add(lyric);
				bRet = TRUE;	// add one lyric successfully, it returns TRUE
			}
		}
		else
		{
			break;		// no one ']' found, unnecessary to search.
		}

		posStart = strLine.Find(_T('['), posEnd);
	}

	return bRet;
}

int CLyricParser::String2Time(LPCTSTR str)
{
	if (_tcslen(str) != 8 || str[2] != _T(':') || str[5] != _T('.'))
	{
		return -1;
	}

	TCHAR temp[3];
	_tcsncpy(temp, str, 2);
	int nMin = _ttoi(temp);
	_tcsncpy(temp, str+3, 2);
	int nSec = _ttoi(temp);
	_tcsncpy(temp, str+6, 2);
	int nMillSec = _ttoi(temp);
	if (nMillSec >= 50)
	{
		nSec++;
	}

	return (nMin*60+nSec);
}

UINT CLyricParser::TestCodePage(LPCTSTR lpszFileName)
{
	const int BUFF_SIZE = 1024;
	char buffer[BUFF_SIZE];
	DWORD bytes_read = 0;	
	int nCodePage = CP_ACP;
	HANDLE fh = INVALID_HANDLE_VALUE;

	memset( buffer,0,sizeof(buffer) );
	fh = CreateFile(lpszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if (INVALID_HANDLE_VALUE == fh)
	{
		goto EXIT;
	}

	if (ReadFile(fh,buffer,BUFF_SIZE,&bytes_read,NULL) && bytes_read>0)
	{	
		DWORD dwvalue =0;
		int nBig5Count = 0; 
		int nGBKCount=0;
		//  Big5   第一节字，行码A1-FE，第二节字，列码40-7E，0xA1-FE，
		int ch =0;
		int nHigh=0;
		int nLow=0;
		for (int i=0; i<(int)bytes_read; i++)
		{
			ch = buffer[i];
			dwvalue = (dwvalue<<8) | ch;
			nHigh = nLow;
			nLow = dwvalue & 0xff;
			if((nHigh>=0xa1 && nHigh<=0xfe) && (nLow>=0x40 && nLow<=0x7e))
				nBig5Count++;

			if(nHigh>=0x81 && nHigh<=0xfe)
				nGBKCount++;
		}
		if(nBig5Count > bytes_read*0.1)
			nCodePage = 950;
		else if(nGBKCount > bytes_read*0.1)
			nCodePage = 936;
	}

/*
	IMultiLanguage3   *pMultiLanguage = NULL; 
	HRESULT hr =CoCreateInstance(CLSID_CMultiLanguage, NULL, CLSCTX_INPROC_SERVER,
		IID_IMultiLanguage3, (void**)&pMultiLanguage); 

	INT size = bytes_read;
	DetectEncodingInfo*   encoding = NULL;
	INT score = 1;

	encoding = (DetectEncodingInfo*)CoTaskMemAlloc(sizeof(DetectEncodingInfo)*10);
	memset(encoding, 0, sizeof(DetectEncodingInfo)*1);

	hr = pMultiLanguage->DetectInputCodepage(MLDETECTCP_DBCS, 0, buffer, &size, encoding, &score);

	pMultiLanguage->Release();
	nCodePage = encoding[0].nCodePage;
	CoTaskMemFree(encoding);
*/

EXIT:
	if (fh != INVALID_HANDLE_VALUE)
	{
		CloseHandle(fh);
		fh = INVALID_HANDLE_VALUE;
	}
	
	return nCodePage;
}

void CLyricParser::SortLyric()
{
	LYRIC* prev = NULL;
	LYRIC* next = NULL;
	int nCount = m_lyric.GetCount();
	for (int i=0; i<nCount; i++)
	{
		prev = (LYRIC*)m_lyric.GetAt(i);
		for (int j=i+1; j<nCount; j++)
		{
			next = (LYRIC*)m_lyric.GetAt(j);
			if (prev->time > next->time)	// switch two
			{
				m_lyric.SetAt(i, next);
				m_lyric.SetAt(j, prev);
				prev = next;
			}
		}
	}
}

int CLyricParser::GetLyricIndex(int time)
{
	if (m_lyric.GetCount() <= 0)
	{
		return -1;
	}

	int i = 0;
	for (i=0; i<m_lyric.GetCount(); i++)
	{
		LYRIC *plyric = (LYRIC*)m_lyric.GetAt(i);
		if (plyric->time > time)
		{
			break;
		}
	}

	// found one, return its previous one if it's not the first one.
	// not found one, return the last one.
	if (i>0)
	{
		i--;
	}

	return i;
}

LPCTSTR CLyricParser::GetLyric(int time)
{
	int i = GetLyricIndex(time);
	
	if (i != -1)
	{
		LYRIC *plyric = (LYRIC*)m_lyric.GetAt(i);
		return plyric->szLyric;
	}
	else
	{
		return NULL;
	}
}