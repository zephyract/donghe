#pragma once

// put the following code in header file or at the beginning of the source file.
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "


#define NS_TZUTIL_BEGIN	namespace tzutil {
#define NS_TZUTIL_END	}


NS_TZUTIL_BEGIN

BOOL LoadApp(LPCWSTR pExeFile, LPCWSTR cmdLine, PROCESS_INFORMATION* pInfo);

BOOL GetSubString(IN LPCTSTR lpszpData, OUT CStringArray &strArray, const TCHAR chSeparator);

// ���ص�ǰִ���ļ����ڵ�Ŀ¼, ������"\"
LPCTSTR   GetAppPath(void);

// Extract the file path from a full path.
// the out buffer must be large enough.
// the returned path not includes the last '\'
BOOL GetFilePath(IN LPCTSTR lpszFileFullPath, OUT LPTSTR lpszFilePath);

// Extract the file name from a full path.
// the out buffer must be large enough.
BOOL GetFileName(IN LPCTSTR lpszFileFullPath, OUT LPTSTR lpszFileName);

// Extract the extent file name from a full path.
BOOL GetFileExtentName(IN LPCTSTR lpszFileFullPath, OUT LPTSTR lpszFileExtentName);


typedef struct 
{
	TCHAR _title[30+1];
	TCHAR _artist[30+1];
	TCHAR _album[30+1];
	TCHAR _year[4+1];
	TCHAR _comment[30+1];
	TCHAR _genre[32];
} MP3_TAG;

BOOL GetMP3Tag(const TCHAR *pFileName, MP3_TAG *pMP3_Tag,UINT nCodePage);
BOOL GetMp3Image(LPCTSTR lpszFileName, OUT BYTE **ppBuffer, OUT int *pSize);
BOOL GetMp3TagV2(LPCTSTR lpszFileName, OUT MP3_TAG* pMP3_Tag,UINT nCodePage);
BOOL DecodeTextV2(IN char* pText, int nLen, OUT TCHAR* pDst, int nDstCnt,UINT nCodePage);
BOOL GetWMATag(const TCHAR *pFileName, MP3_TAG * pWMA_Tag);
// This function retrieves the data for a specified value name associated with an open registry key
// hKey -- Handle to a currently open key or any of the predefined reserved handle values.
// lpSubKey -- Pointer to a null-terminated string containing the name of the subkey to open.
// lpValueName -- Pointer to a string containing the name of the value to query.
// lpszData -- pointer to a buffer that receives the value's data.
// lpcbData -- Pointer to a variable that specifies the size, in bytes, 
//             of the buffer pointed to by the lpData parameter. When the function returns, 
//             this variable contains the size of the data copied to lpData. 
BOOL GetRegValue(HKEY hKey, LPCTSTR lpSubKey, LPCWSTR lpValueName, OUT LPBYTE lpszData, IN OUT LPDWORD lpcbData);

void CopyScreen(HBITMAP hbmp, LPRECT prcSrc);

int _W(LPRECT prc);
int _H(LPRECT prc);

int UTF8ToUnicode(LPCSTR pszSrc, LPWSTR pszDst);

// get current time zone info. it returns the registry name which one is used now.
// the buffer must be large enough
BOOL GetTimeZoneInformation(LPTSTR buffer);

BOOL GetTziByRegistry(IN LPCTSTR lpszRegistryName, OUT TIME_ZONE_INFORMATION* ptzi);
BOOL GetRegistryByTzi(OUT LPTSTR lpszRegistryName, IN const TIME_ZONE_INFORMATION* ptzi);

// �ж��Ƿ�ΪӢ����ĸ
BOOL IsAlphabet(TCHAR ch);
// ����һ���ַ���ƴ������ĸ
TCHAR GetCharPYCode(TCHAR ch);
// ����һ���ַ�����ƴ������ĸ��,��෵��8λ��
LPCTSTR GetStringPYCode(LPCTSTR lpszString);

// ��ȡָ�����̵�������
HWND GetProcessMainWnd(DWORD dwProcessId);

BOOL SetProcessForeground(DWORD dwPorcessID);

// �Ƚ϶���BOOLֵ�Ƿ����
BOOL IsBoolEqual(BOOL a, BOOL b);

// ������תΪ�ַ���
LPCTSTR Int2Ascii(int n);

BOOL DeleteDirectory(LPCTSTR lpszFilePath);

NS_TZUTIL_END