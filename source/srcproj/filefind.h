#pragma once 

#include <atltime.h>
using ATL::CTime;
using ATL::CTimeSpan;

class CFileFind : public CObject
{
public:
	CFileFind();
	virtual ~CFileFind();

	// Attributes
public:
	ULONGLONG GetLength() const;
	virtual CString GetFileName() const;
	virtual CString GetFilePath() const;
	virtual CString GetFileTitle() const;
	virtual CString GetFileURL() const;
	virtual CString GetRoot() const;

	virtual BOOL GetLastWriteTime(FILETIME* pTimeStamp) const;
	virtual BOOL GetLastAccessTime(FILETIME* pTimeStamp) const;
	virtual BOOL GetCreationTime(FILETIME* pTimeStamp) const;
	virtual BOOL GetLastWriteTime(CTime& refTime) const;
	virtual BOOL GetLastAccessTime(CTime& refTime) const;
	virtual BOOL GetCreationTime(CTime& refTime) const;

	virtual BOOL MatchesMask(DWORD dwMask) const;

	virtual BOOL IsDots() const;
	// these aren't virtual because they all use MatchesMask(), which is
	BOOL IsReadOnly() const;
	BOOL IsDirectory() const
	{
		return MatchesMask(FILE_ATTRIBUTE_DIRECTORY); 
	}
	BOOL IsCompressed() const;
	BOOL IsSystem() const;
	BOOL IsHidden() const;
	BOOL IsTemporary() const;
	BOOL IsNormal() const;
	BOOL IsArchived() const;

	// Operations
	void Close();
	virtual BOOL FindFile(LPCTSTR pstrName = NULL, DWORD dwUnused = 0);
	virtual BOOL FindNextFile();

protected:
	virtual void CloseContext();

	// Implementation
protected:
	void* m_pFoundInfo;
	void* m_pNextInfo;
	HANDLE m_hContext;
	CString m_strRoot;
	TCHAR m_chDirSeparator;     // not '\\' for Internet classes

	DECLARE_DYNAMIC(CFileFind)
};


