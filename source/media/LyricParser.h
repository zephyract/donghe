#pragma once

#define MAX_LYRIC_LENGTH	255
struct LYRIC
{
	int time;
	TCHAR szLyric[MAX_LYRIC_LENGTH];
};

struct LYRIC_HEAD
{
	TCHAR title[32];
	TCHAR artist[32];
	TCHAR album[32];
	TCHAR author[32];
};


class CLyricParser
{
public:
	CLyricParser();
	~CLyricParser();

	// interface
	BOOL Initialize(LPCTSTR lpszFileName);
	void UnInitialize();
	// return the corresponding lyric with specified time.
	LPCTSTR GetLyric(int time);
	// return the corresponding lyric line index in array with specified time.
	int GetLyricIndex(int time);
	// get lyric head info
	LYRIC_HEAD* GetHead() {return &m_head;}
	// get all lyric data.
	// all the data saved in a CPtrArray. the pointer in this array
	// points a struct LYRIC.
	CPtrArray* GetLyricData(){return &m_lyric;}

protected:
	BOOL ParseFile(LPCTSTR lpszFileName);
	// if bCheckHead is TRUE, it will consider if this line includes head info or not.
	// if it's FALSE, it will not consider this condition.
	BOOL ParseLine(LPCTSTR lpszLine, BOOL bCheckHead=FALSE);
	// return the file's codepage, if any errors occur, it return CP_ACP(0)
	UINT TestCodePage(LPCTSTR lpszFileName);
	// sort the lyric array by time
	void SortLyric();
	// nIndex -- 0 ~ set title,  1 ~ set artist, 2 ~ set album, 3 ~ set author.
	// other values are invalid.
	void SetHeadInfo(int nIndex, LPCTSTR lpszInfo);
	// convert string(format: [00:35.00]) to time(in seconds)
	// if success, it returns time in seconds.
	// if fail, it returns -1.
	int String2Time(LPCTSTR str);
	BOOL Is_nobom_utf8(const unsigned char *str);

	LYRIC_HEAD m_head;
	CPtrArray m_lyric;

	UINT m_uCodePage;	// auto test the file's code page.
};