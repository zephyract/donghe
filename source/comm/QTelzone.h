#pragma once
#include "CppSQLite3U.h"

class CQTelzone
{
public:
	CQTelzone();
	~CQTelzone();

	// interface
	static CQTelzone* GetInstance();
	LPCTSTR GetTelZone(LPCTSTR tel);

	void CreateDB2();
	void CreateDB();
protected:
	LPCTSTR GetSpecialTel(LPCTSTR tel);
	CppSQLite3DB m_db;
};