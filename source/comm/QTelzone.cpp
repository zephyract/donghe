#include "stdafx.h"
#include "QTelzone.h"
#include "WceUiXml.h"
#include "tzUtil.h"

const int MOBILE_FIELD_MOBILE		= 0;
const int MOBILE_FIELD_PROVINCE	= 1;
const int MOBILE_FIELD_CITY		= 2;
const int MOBILE_FIELD_PROVIDER	= 3;

const int TELZONE_FIELD_PROVINCE	= 0;
const int TELZONE_FIELD_CITY		= 1;
const int TELZONE_FIELD_TELZONE		= 2;


const int FIELD_MOBILE		= 0;
const int FIELD_PROVINCE	= 1;
const int FIELD_CITY		= 2;
const int FIELD_PROVIDER	= 3;
const int FIELD_TELZONE		= 4;

// 先调用CreateDB()从XML创建tel.db
// 再调用CreateDB2()从tel.db创建简化过的数据,有的数据从7位简化到6位，
// 并且把区号放到另外的表,建索引比较占空间，放到另外的表再建索引比较有效

CQTelzone::CQTelzone()
{
	TCHAR szFilepath[MAX_PATH] = L"";
	_stprintf(szFilepath, L"%s\\tel.db", tzutil::GetAppPath());
	m_db.open(szFilepath);
}


CQTelzone::~CQTelzone()
{
	m_db.close();
}

CQTelzone* CQTelzone::GetInstance()
{
	static CQTelzone qt;
	return &qt;
}
// 
// void CQTelzone::CreateDB2()
// {
// 	TCHAR szFilepath[MAX_PATH] = L"";
// 	_stprintf(szFilepath, L"%s\\tel2.db", tzutil::GetAppPath());
// 	CppSQLite3DB db;
// 
// 	// create db
// 	db.open(szFilepath);
// 	try
// 	{
// 		db.execDML(L"PRAGMA synchronous = OFF; ");
// 		if (!db.tableExists(L"telzone"))
// 		{
// 			db.execDML(L"create table telzone(mobile int, province nchar(4), city nchar(7), provider char(1), telzone smallint);");
// 			db.execDML(L"create index mobile_index on telzone(mobile)");
// 			db.execDML(L"create index telzone_index on telzone(telzone)");
// 		}
// 	}
// 	catch (CppSQLite3Exception& e)
// 	{
// 		RETAILMSG(1, (L"db create error code = %d %s \n", e.errorCode(), e.errorMessage()));
// 	}
// 
// 	// 
// 	// add to db
// 	try
// 	{
// 		db.execDML(L"begin transaction;");
// 		CppSQLite3Statement stmt = db.compileStatement(L"insert into telzone values (?, ?, ?, ?, ?);");
// 
// 
// 		for (int i=130000; i<189999; i++)
// 		{
// 			TCHAR szSQL[128];
// 			_stprintf(szSQL, L"select * from telzone where mobile >= %d and mobile <= %d ;", i*10, i*10+9);
// 
// 			CppSQLite3Query q = m_db.execQuery(szSQL);
// 			int count = 0;
// 			int mobile;
// 			TCHAR province[4]=L"";
// 			TCHAR city[7]=L"";
// 			char provider;
// 			short telzone;
// 			if (!q.eof())
// 			{
// 				mobile = q.getIntField(FIELD_MOBILE);
// 				_tcscpy(province, q.fieldValue(FIELD_PROVINCE));
// 				_tcscpy(city, q.fieldValue(FIELD_CITY));
// 				provider = q.getIntField(FIELD_PROVIDER);
// 				telzone = q.getIntField(FIELD_TELZONE);
// 				count++;
// 				q.nextRow();
// 			}
// 			while (!q.eof())
// 			{
// 				if (_tcscmp(province, q.fieldValue(FIELD_PROVINCE))!=0 ||  _tcscmp(city, q.fieldValue(FIELD_CITY))!=0)
// 				{
// 					break;
// 				}
// 				q.nextRow();
// 				count++;
// 			}
// 			q.finalize();
// 
// 			// 如果count ==10说明这10条记录可以合并为一条记录，都分配给了同一个地区
// 			if (count == 10)
// 			{
// 				// 添加DB数据
// 				stmt.bind(1, mobile/10);	// 用6位来表示这10条数据属于同一个地区
// 				stmt.bind(2, province);
// 				stmt.bind(3, city);
// 				stmt.bind(4, provider);
// 				stmt.bind(5, telzone);
// 				stmt.execDML();
// 				stmt.reset();
// 			}
// 			else	// 否则直接把查询到的数据导出到新数据库
// 			{
// 				CppSQLite3Query q = m_db.execQuery(szSQL);
// 				while (!q.eof())
// 				{
// 					mobile = q.getIntField(FIELD_MOBILE);
// 					_tcscpy(province, q.fieldValue(FIELD_PROVINCE));
// 					_tcscpy(city, q.fieldValue(FIELD_CITY));
// 					provider = q.getIntField(FIELD_PROVIDER);
// 					telzone = q.getIntField(FIELD_TELZONE);
// 					// 添加DB数据
// 					stmt.bind(1, mobile);	
// 					stmt.bind(2, province);
// 					stmt.bind(3, city);
// 					stmt.bind(4, provider);
// 					stmt.bind(5, telzone);
// 					stmt.execDML();
// 					stmt.reset();
// 
// 					q.nextRow();
// 				}
// 				q.finalize();
// 			}
// 		}
// 
// 
// 		db.execDML(L"commit transaction;");
// 	}
// 	catch (CppSQLite3Exception& e)
// 	{
// 		db.execDML(L"rollback transaction;");
// 		RETAILMSG(1, (L"db error code = %d %s \n", e.errorCode(), e.errorMessage()));
// 	}
// 
// 	
// 	// read it out
// 	try
// 	{
// 		TCHAR szSQL[64];
// 		_stprintf(szSQL, L"select * from telzone;");
// 		CppSQLite3Query q = db.execQuery(szSQL);
// 		while (!q.eof())
// 		{
// 			_stprintf(szSQL, L"%d, %s, %s, %d, %d ",q.getIntField(0), q.fieldValue(1), q.fieldValue(2), q.getIntField(3), q.getIntField(4));
// 			RETAILMSG(1, (L"%s\n", szSQL));
// 			q.nextRow();
// 		}
// 	}
// 	catch (CppSQLite3Exception& e)
// 	{
// 		RETAILMSG(1, (L"db select error code = %d %s \n", e.errorCode(), e.errorMessage()));
// 	}
// }


void CQTelzone::CreateDB2()
{
	TCHAR szFilepath[MAX_PATH] = L"";
	_stprintf(szFilepath, L"%s\\tel2.db", tzutil::GetAppPath());
	CppSQLite3DB db;

	// create db
	db.open(szFilepath);
	try
	{
		db.execDML(L"PRAGMA synchronous = OFF; ");
		if (!db.tableExists(L"mobile"))
		{
			db.execDML(L"create table mobile(mobile int, province nchar(4), city nchar(7), provider char(1));");
			db.execDML(L"create index mobile_index on mobile(mobile)");
		}

		if (!db.tableExists(L"telzone"))
		{
			db.execDML(L"create table telzone(province nchar(4), city nchar(7), telzone smallint);");
			db.execDML(L"create index telzone_index on telzone(telzone)");
		}
	}
	catch (CppSQLite3Exception& e)
	{
		RETAILMSG(1, (L"db create error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}

	// 
	// add to db
	try
	{
		db.execDML(L"begin transaction;");
		CppSQLite3Statement stmt = db.compileStatement(L"insert into mobile values (?, ?, ?, ?);");
		CppSQLite3Statement stmt1 = db.compileStatement(L"insert into telzone values (?, ?, ?);");

		for (int i=10; i<=999; i++)	// 区号表
		{
			TCHAR szSQL[128];
			_stprintf(szSQL, L"select * from telzone where telzone = %d ;", i);

			CppSQLite3Query q = m_db.execQuery(szSQL);
			if (!q.eof())
			{
				stmt1.bind(1, q.fieldValue(FIELD_PROVINCE));
				stmt1.bind(2, q.fieldValue(FIELD_CITY));
				stmt1.bind(3, q.getIntField(FIELD_TELZONE));
				stmt1.execDML();
				stmt1.reset();
			}
			q.finalize();
		}

		for (int i=130000; i<=189999; i++)
		{
			TCHAR szSQL[128];
			_stprintf(szSQL, L"select * from telzone where mobile >= %d and mobile <= %d ;", i*10, i*10+9);

			CppSQLite3Query q = m_db.execQuery(szSQL);
			int count = 0;
			int mobile;
			TCHAR province[4]=L"";
			TCHAR city[7]=L"";
			char provider;
			short telzone;
			if (!q.eof())
			{
				mobile = q.getIntField(FIELD_MOBILE);
				_tcscpy(province, q.fieldValue(FIELD_PROVINCE));
				_tcscpy(city, q.fieldValue(FIELD_CITY));
				provider = q.getIntField(FIELD_PROVIDER);
				telzone = q.getIntField(FIELD_TELZONE);
				count++;
				q.nextRow();
			}
			while (!q.eof())
			{
				if (_tcscmp(province, q.fieldValue(FIELD_PROVINCE))!=0 ||  _tcscmp(city, q.fieldValue(FIELD_CITY))!=0)
				{
					break;
				}
				q.nextRow();
				count++;
			}
			q.finalize();

			// 如果count ==10说明这10条记录可以合并为一条记录，都分配给了同一个地区
			if (count == 10)
			{
				// 添加DB数据
				stmt.bind(1, mobile/10);	// 用6位来表示这10条数据属于同一个地区
				stmt.bind(2, province);
				stmt.bind(3, city);
				stmt.bind(4, provider);
				stmt.execDML();
				stmt.reset();

			}
			else	// 否则直接把查询到的数据导出到新数据库
			{
				CppSQLite3Query q = m_db.execQuery(szSQL);
				while (!q.eof())
				{
					mobile = q.getIntField(FIELD_MOBILE);
					_tcscpy(province, q.fieldValue(FIELD_PROVINCE));
					_tcscpy(city, q.fieldValue(FIELD_CITY));
					provider = q.getIntField(FIELD_PROVIDER);
					telzone = q.getIntField(FIELD_TELZONE);
					// 添加DB数据
					stmt.bind(1, mobile);	
					stmt.bind(2, province);
					stmt.bind(3, city);
					stmt.bind(4, provider);
					stmt.execDML();
					stmt.reset();

					q.nextRow();
				}
				q.finalize();
			}
		}


		db.execDML(L"commit transaction;");
	}
	catch (CppSQLite3Exception& e)
	{
		db.execDML(L"rollback transaction;");
		RETAILMSG(1, (L"db error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}


	// read it out
	try
	{
		TCHAR szSQL[64];
		_stprintf(szSQL, L"select * from telzone;");
		CppSQLite3Query q = db.execQuery(szSQL);
		while (!q.eof())
		{
			_stprintf(szSQL, L"%s, %s, %d ",q.fieldValue(0), q.fieldValue(1), q.getIntField(2));
			RETAILMSG(1, (L"%s\n", szSQL));
			q.nextRow();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		RETAILMSG(1, (L"db select error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}

	// read it out
	try
	{
		TCHAR szSQL[64];
		_stprintf(szSQL, L"select * from mobile;");
		CppSQLite3Query q = db.execQuery(szSQL);
		while (!q.eof())
		{
			_stprintf(szSQL, L"%d, %s, %s, %d ",q.getIntField(0), q.fieldValue(1), q.fieldValue(2), q.getIntField(3));
			RETAILMSG(1, (L"%s\n", szSQL));
			q.nextRow();
		}
	}
	catch (CppSQLite3Exception& e)
	{
		RETAILMSG(1, (L"db select error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}
}

void CQTelzone::CreateDB()
{
	// create db
	try
	{
		m_db.execDML(L"PRAGMA synchronous = OFF; ");
		if (!m_db.tableExists(L"telzone"))
		{
			m_db.execDML(L"create table telzone(mobile int, province nchar(4), city nchar(7), provider char(1), telzone smallint);");
			m_db.execDML(L"create index mobile_index on telzone(mobile)");
			m_db.execDML(L"create index telzone_index on telzone(telzone)");
		}
	}
	catch (CppSQLite3Exception& e)
	{
		RETAILMSG(1, (L"db create error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}


	// open xml
	TCHAR szFilepath[MAX_PATH];
	_stprintf(szFilepath, L"%s\\tel2.xml", tzutil::GetAppPath());

	CWceUiXmlDoc xmlDoc;
	if (!xmlDoc.LoadXmlFile(szFilepath))
	{
		RETAILMSG(1, (L"[nvai_car]: could not find tel2.xml file.\n"));
		return ;
	}
	CWceUiXmlElem *pxmlElem = xmlDoc.RootElement();
	ASSERT(pxmlElem);

	// parse data
	LPCTSTR lpszValue = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	int mobile;
	TCHAR province[4];
	TCHAR city[7];
	char provider;
	short telzone;
	pXmlElemChild = pxmlElem->FirstChild();

	// add to db
	try
	{
			m_db.execDML(L"begin transaction;");
			CppSQLite3Statement stmt = m_db.compileStatement(L"insert into telzone values (?, ?, ?, ?, ?);");
			while (pXmlElemChild)
			{
				lpszValue = pXmlElemChild->GetAttr(L"A");
				ASSERT(lpszValue);
				mobile = _ttoi(lpszValue);
				if (_tcslen(lpszValue) != 7)
				{
					RETAILMSG(1, (L"------------ mobile = %s \n", lpszValue));
				}

				lpszValue = pXmlElemChild->GetAttr(L"B");
				ASSERT(lpszValue);
				ASSERT(_tcslen(lpszValue)<4);
				_tcscpy(province, lpszValue);

				lpszValue = pXmlElemChild->GetAttr(L"C");
				ASSERT(lpszValue);
				if (_tcslen(lpszValue)>=7)
				{
					RETAILMSG(1, (L"%s\n", lpszValue));
					pXmlElemChild = pxmlElem->NextChild(pXmlElemChild);
					continue;
				}
				ASSERT(_tcslen(lpszValue)<7);
				_tcscpy(city, lpszValue);

				lpszValue = pXmlElemChild->GetAttr(L"D");
				ASSERT(lpszValue);
				provider = lpszValue[0] - L'0';

				lpszValue = pXmlElemChild->GetAttr(L"E");
				telzone = lpszValue ? (SHORT)_ttoi(lpszValue) : 0;

				pXmlElemChild = pxmlElem->NextChild(pXmlElemChild);

				// 添加DB数据
				stmt.bind(1, mobile);
				stmt.bind(2, province);
				stmt.bind(3, city);
				stmt.bind(4, provider);
				stmt.bind(5, telzone);
				stmt.execDML();
				stmt.reset();
			}
			m_db.execDML(L"commit transaction;");
	}
	catch (CppSQLite3Exception& e)
	{
		m_db.execDML(L"rollback transaction;");
		RETAILMSG(1, (L"db error code = %d %s \n", e.errorCode(), e.errorMessage()));
	}


	// read it out
// 	try
// 	{
// 		TCHAR szSQL[64];
// 		_stprintf(szSQL, L"select * from telzone;");
// 		CppSQLite3Query q = m_db.execQuery(szSQL);
// 		while (!q.eof())
// 		{
// 			_stprintf(szSQL, L"%d, %s, %s, %d, %d ",q.getIntField(0), q.fieldValue(1), q.fieldValue(2), q.getIntField(3), q.getIntField(4));
// 			RETAILMSG(1, (L"%s\n", szSQL));
// 			q.nextRow();
// 		}
// 	}
// 	catch (CppSQLite3Exception& e)
// 	{
// 		RETAILMSG(1, (L"db select error code = %d %s \n", e.errorCode(), e.errorMessage()));
// 	}
}

static void _get_result(OUT LPTSTR result, IN LPCTSTR province, IN LPCTSTR city, int provider)
{
	if (result && province && city)
	{
		_tcscpy(result, province);
		if (_tcscmp(province, city) != 0)
		{
			_tcscat(result, city);
		}

		if (provider == 1)
		{
			_tcscat(result, L"移动");
		}
		else if (provider == 2)
		{
			_tcscat(result, L"联通");
		}
		else if (provider == 3)
		{
			_tcscat(result, L"电信");
		}
	}
}

LPCTSTR CQTelzone::GetTelZone(LPCTSTR tel)
{
	DWORD tick = GetTickCount();

	static TCHAR result[16];
	result[0] = L'\0';
	CString str = tel;

	_tcscpy(result, GetSpecialTel(tel));
	if ( result[0] != L'\0')	// 匹配到数据了
	{
		goto LEXIT;
	}

	if (_tcslen(tel) >= 6 && tel[0] == L'1')	// 如果是手机，并匹配到结果直接返回
	{
		try
		{
			TCHAR szSQL[128];
			int value = _ttoi(str.Left(6));
			int value2 = _ttoi(str.Left(7));
			_stprintf(szSQL, L"select * from mobile where mobile = %d or mobile = %d ;", value, value2);
			CppSQLite3Query q = m_db.execQuery(szSQL);
			if (!q.eof())
			{
				_get_result(result, q.fieldValue(MOBILE_FIELD_PROVINCE), q.fieldValue(MOBILE_FIELD_CITY), q.getIntField(MOBILE_FIELD_PROVIDER));
				goto LEXIT;
			}
			q.finalize();
		}
		catch (CppSQLite3Exception& e)
		{
			RETAILMSG(1, (L"db error code = %d %s \n", e.errorCode(), e.errorMessage()));
		}
	}

	// 检查区号
	str = str.TrimLeft(L'0');
	if (str.GetLength() >= 2)
	{
		try
		{
			TCHAR szSQL[128];
			int value1 = _ttoi(str.Left(2));
			int value2 = _ttoi(str.Left(3));
			_stprintf(szSQL, L"select * from telzone where telzone = %d or telzone = %d ;", value1, value2);
			CppSQLite3Query q = m_db.execQuery(szSQL);
			if (!q.eof())
			{
				//_stprintf(result, L"%s%s", q.fieldValue(FIELD_PROVINCE), q.fieldValue(FIELD_CITY));
				_get_result(result, q.fieldValue(TELZONE_FIELD_PROVINCE), q.fieldValue(TELZONE_FIELD_CITY), 0);
				goto LEXIT;
			}
			q.finalize();
		}
		catch (CppSQLite3Exception& e)
		{
			RETAILMSG(1, (L"db error code = %d %s \n", e.errorCode(), e.errorMessage()));
		}
	}

LEXIT:
	RETAILMSG(1, (L"tel=%s, result = %s, tick = %d ++++++++++++ \n",tel, result, GetTickCount()-tick));
	return result;
}

LPCTSTR CQTelzone::GetSpecialTel(LPCTSTR tel)
{
	struct SPECIAL_TEL 
	{
		LPCTSTR tel;
		LPCTSTR desc;
	};

	SPECIAL_TEL pst[] = {
		{L"110", L"公安报警"},
		{L"119", L"火警"},
		{L"114", L"查号台"},
		{L"160", L"160声讯台"},
		{L"10000", L"中国电信客服"},
		{L"10086", L"中国移动客服"},
		{L"10010", L"联通客服热线"},
		{L"122", L"交通事故"},
		{L"120", L"急救中心"},
		{L"121", L"天气预报"},
		{L"112", L"电话故障"},
		{L"4008005005", L"翼卡在线"},
		{L"4001050868", L"翼卡在线"},
		{L"075787807155", L"翼卡在线"},
		{L"075788303000", L"翼卡在线"},
		{L"075536860630", L"翼卡在线"}
	};

	if (tel)
	{
		for (int i=0; i<sizeof(pst)/sizeof(pst[0]); i++)
		{
			if (_tcscmp(tel, pst[i].tel) == 0)
			{
				return pst[i].desc;
			}
		}
	}

	return L"";
}