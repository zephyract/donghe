#include "stdafx.h"
#include "config.h"
#include "tzUtil.h"
#include "WceUiXml.h"
#include "sysUtil.h"
#include "MSDK_Defs.h"
#include "volumemanager.h"

void _init_language(CWceUiXmlElem *pxmlElem );
void _init_timezone(CWceUiXmlElem *pxmlElem );
void _init_uart(CWceUiXmlElem *pxmlElem );
void _init_audio(CWceUiXmlElem *pxmlElem);
void _init_disk(CWceUiXmlElem *pxmlElem);
void _init_video(CWceUiXmlElem *pxmlElem);
void _init_sysinfo(CWceUiXmlElem *pxmlElem);
void _init_bluetooth(CWceUiXmlElem *pxmlElem);
void _init_avin(CWceUiXmlElem *pxmlElem);
void _init_functions(CWceUiXmlElem *pxmlElem);
void _init_canbus(CWceUiXmlElem *pxmlElem);
void _init_touchkey(CWceUiXmlElem *pxmlElem);
void _init_misc(CWceUiXmlElem *pxmlElem);
// 该表列出系统中所有可支持的语言，但呈现给用户的支持语言时，还需根据配置文件中的配置来决定
static LANGUAGE_TABLE_INFO	g_language_table[] =
{
	{ L"English",		L"English",		LAN_ENGLISH		, 437, MSDK_CPAGE_1252, LAN_UNSUPPORT},
	{ L"简体中文",		L"ChineseGB",	LAN_CHINESEGB	, 936, MSDK_CPAGE_936, LAN_UNSUPPORT},
	{ L"繁体中文",		L"ChineseBIG5",	LAN_CHINESEBIG5 , 950, MSDK_CPAGE_950, LAN_UNSUPPORT},
	{ L"Français",		L"French",		LAN_FRENCH		, 850, MSDK_CPAGE_8859_1, LAN_UNSUPPORT},
	{ L"Русский",	L"Russian",	LAN_RUSSIAN		, 866, MSDK_CPAGE_8859_2, LAN_UNSUPPORT},
	{ L"Español",		L"Spanish",		LAN_SPANISH		, 850, MSDK_CPAGE_8859_2, LAN_UNSUPPORT},
	{ L"Deutsch",		L"German",		LAN_GERMAN		, 850, MSDK_CPAGE_8859_1, LAN_UNSUPPORT},
	{ L"Português",		L"Portuguess",	LAN_PORTUGUESS  , 860, MSDK_CPAGE_8859_1, LAN_UNSUPPORT},
	{ L"Arab",			L"Arab",		LAN_ARAB		, 720, MSDK_CPAGE_1256, LAN_UNSUPPORT},
	{ L"Thai",			L"Thai",		LAN_THAI		, 874, MSDK_CPAGE_874, LAN_UNSUPPORT},
	{ L"日本語",		L"Japanese",	LAN_JAPANESE	, 932, MSDK_CPAGE_KANJI, LAN_UNSUPPORT},
	{ L"Turkic",		L"Turkic",		LAN_TURKIC		, 857, MSDK_CPAGE_8859_9, LAN_UNSUPPORT},
};

// 配置文件中指定缺省的语言
static int g_language_default = LAN_ENGLISH;

// 该表列出所有支持的时区项
// 配置表中暂时不支持配置支持哪些时区，如果需要此功能，后续可增加，这里缺省全部支持
static TIMEZONE_TABLE_INFO g_timezone_table[] =
{
	{ L"Dateline Standard Time"			  , TIMEZONE_UNSUPPORT },
	{ L"Samoa Standard Time"              , TIMEZONE_UNSUPPORT },
	{ L"Hawaiian Standard Time"           , TIMEZONE_UNSUPPORT },
	{ L"Alaskan Standard Time"            , TIMEZONE_UNSUPPORT },
	{ L"Pacific Standard Time"            , TIMEZONE_UNSUPPORT },
	{ L"Mountain Standard Time"           , TIMEZONE_UNSUPPORT },
	{ L"Mexico Standard Time"             , TIMEZONE_UNSUPPORT },
	{ L"US Mountain Standard Time"        , TIMEZONE_UNSUPPORT },
	{ L"Central Standard Time"            , TIMEZONE_UNSUPPORT },
	{ L"Canada Central Standard Time"     , TIMEZONE_UNSUPPORT },
	{ L"Mexico Standard Time"             , TIMEZONE_UNSUPPORT },
	{ L"Central America Standard Time"    , TIMEZONE_UNSUPPORT },
	{ L"Eastern Standard Time"            , TIMEZONE_UNSUPPORT },
	{ L"US Eastern Standard Time"         , TIMEZONE_UNSUPPORT },
	{ L"SA Pacific Standard Time"         , TIMEZONE_UNSUPPORT },
	{ L"Atlantic Standard Time"           , TIMEZONE_UNSUPPORT },
	{ L"SA Western Standard Time"         , TIMEZONE_UNSUPPORT },
	{ L"Pacific SA Standard Time"         , TIMEZONE_UNSUPPORT },
	{ L"Newfoundland Standard Time"		  , TIMEZONE_UNSUPPORT },
	{ L"E. South America Standard Time"   , TIMEZONE_UNSUPPORT },
	{ L"SA Eastern Standard Time"         , TIMEZONE_UNSUPPORT },
	{ L"Greenland Standard Time"          , TIMEZONE_UNSUPPORT },
	{ L"Mid-Atlantic Standard Time"       , TIMEZONE_UNSUPPORT },
	{ L"Azores Standard Time"             , TIMEZONE_UNSUPPORT },
	{ L"Cape Verde Standard Time"         , TIMEZONE_UNSUPPORT },
	{ L"GMT Standard Time"                , TIMEZONE_UNSUPPORT },
	{ L"Greenwich Standard Time"          , TIMEZONE_UNSUPPORT },
	{ L"Central Europe Standard Time"     , TIMEZONE_UNSUPPORT },
	{ L"Central European Standard Time"   , TIMEZONE_UNSUPPORT },
	{ L"Romance Standard Time"            , TIMEZONE_UNSUPPORT },
	{ L"W. Europe Standard Time"          , TIMEZONE_UNSUPPORT },
	{ L"W. Central Africa Standard Time"  , TIMEZONE_UNSUPPORT },
	{ L"E. Europe Standard Time"          , TIMEZONE_UNSUPPORT },
	{ L"Egypt Standard Time"              , TIMEZONE_UNSUPPORT },
	{ L"FLE Standard Time"                , TIMEZONE_UNSUPPORT },
	{ L"GTB Standard Time"                , TIMEZONE_UNSUPPORT },
	{ L"Jerusalem Standard Time"          , TIMEZONE_UNSUPPORT },
	{ L"South Africa Standard Time"       , TIMEZONE_UNSUPPORT },
	{ L"Russian Standard Time"            , TIMEZONE_UNSUPPORT },
	{ L"Arab Standard Time"               , TIMEZONE_UNSUPPORT },
	{ L"E. Africa Standard Time"          , TIMEZONE_UNSUPPORT },
	{ L"Arabic Standard Time"             , TIMEZONE_UNSUPPORT },
	{ L"Iran Standard Time"               , TIMEZONE_UNSUPPORT },
	{ L"Arabian Standard Time"            , TIMEZONE_UNSUPPORT },
	{ L"Caucasus Standard Time"           , TIMEZONE_UNSUPPORT },
	{ L"Afghanistan Standard Time"        , TIMEZONE_UNSUPPORT },
	{ L"Ekaterinburg Standard Time"       , TIMEZONE_UNSUPPORT },
	{ L"West Asia Standard Time"          , TIMEZONE_UNSUPPORT },
	{ L"India Standard Time"              , TIMEZONE_UNSUPPORT },
	{ L"Nepal Standard Time"              , TIMEZONE_UNSUPPORT },
	{ L"Central Asia Standard Time"       , TIMEZONE_UNSUPPORT },
	{ L"Sri Lanka Standard Time"          , TIMEZONE_UNSUPPORT },
	{ L"N. Central Asia Standard Time"    , TIMEZONE_UNSUPPORT },
	{ L"Myanmar Standard Time"            , TIMEZONE_UNSUPPORT },
	{ L"SE Asia Standard Time"            , TIMEZONE_UNSUPPORT },
	{ L"North Asia Standard Time"         , TIMEZONE_UNSUPPORT },
	{ L"China Standard Time"              , TIMEZONE_UNSUPPORT },
	{ L"Malay Peninsula Standard Time"    , TIMEZONE_UNSUPPORT },
	{ L"Taipei Standard Time"             , TIMEZONE_UNSUPPORT },
	{ L"W. Australia Standard Time"       , TIMEZONE_UNSUPPORT },
	{ L"North Asia East Standard Time"    , TIMEZONE_UNSUPPORT },
	{ L"Korea Standard Time"              , TIMEZONE_UNSUPPORT },
	{ L"Tokyo Standard Time"              , TIMEZONE_UNSUPPORT },
	{ L"Yakutsk Standard Time"            , TIMEZONE_UNSUPPORT },
	{ L"AUS Central Standard Time"        , TIMEZONE_UNSUPPORT },
	{ L"Cen. Australia Standard Time"     , TIMEZONE_UNSUPPORT },
	{ L"AUS Eastern Standard Time"        , TIMEZONE_UNSUPPORT },
	{ L"E. Australia Standard Time"       , TIMEZONE_UNSUPPORT },
	{ L"Tasmania Standard Time"           , TIMEZONE_UNSUPPORT },
	{ L"Vladivostok Standard Time"        , TIMEZONE_UNSUPPORT },
	{ L"West Pacific Standard Time"       , TIMEZONE_UNSUPPORT },
	{ L"Central Pacific Standard Time"    , TIMEZONE_UNSUPPORT },
	{ L"Fiji Standard Time"               , TIMEZONE_UNSUPPORT },
	{ L"New Zealand Standard Time"        , TIMEZONE_UNSUPPORT },
	{ L"Tonga Standard Time"              , TIMEZONE_UNSUPPORT },
};

// 配置文件中指定缺省的时区,如果配置文件没配置，则缺省为北京时区
static int g_timezone_default = 56; // 是表中的一个索引值，指向 L"China Standard Time"
static BOOL g_timezone_dst_enable = FALSE;

static CONFIG_AUDIO	g_config_audio;
static CONFIG_DISK g_config_disk = {L"sdmmc", L"usb", L"usb1", {L"storage card", L"", L""}};
static CONFIG_VIDEO g_config_video;
static CONFIG_SYSINFO g_config_sysinfo;
static CONFIG_BLUETOOTH g_config_bluetooth;
static CONFIG_AVIN		g_config_avin;
static CONFIG_FUNCTIONS g_config_functions;
static CONFIG_UART		g_config_uart = {4, 57600, 1, 9600};
static CONFIG_CANBUS    g_config_canbus;
static CONFIG_TOUCHKEY  g_config_touchkey;
static CONFIG_MISC  g_config_misc;

// 配置文件中可配置支持的功能项,是用主界面的layer name来标识的,这里建一张表来映射源与其对应关系
// 如果有新增加的源,该表也一定要对应起来,否则循环切模时,不能正常工作(因为无法知道目前支持哪些源)
struct SID2FuncName 
{
	SOURCE_ID sid;
	LPCTSTR lpszFuncName;
};

static SID2FuncName g_sid_table[] = 
{
	{SOURCE_DVD,	 L"dvd"	},
	{SOURCE_SD,		L"sd"	},
	{SOURCE_USB,	L"usb"	},
	{SOURCE_FM,		L"fm"	},
	{SOURCE_TV,		L"tv"	},
	{SOURCE_AVIN1,	L"avin1"	},
	{SOURCE_AVIN2,	L"avin2"	},
	{SOURCE_BT,		L"bt"	},
	{SOURCE_IPOD_USB,	L"ipod"	},
	{SOURCE_USB_REAR,	L"usb_rear"	},
	{SOURCE_VDISK,		L"vdisk"	},
	{SOURCE_PHONE_LINK,		L"phonelink"	},
	{SOURCE_EASY_CONNECTED,	L"easyconnected"	},
};

LANGUAGE_TABLE_INFO* config::get_language_table()
{
	return &g_language_table[0];
}

LANGUAGE_TABLE_INFO* config::get_language_info(int id)
{
	for (int i=0; i<get_language_table_count(); i++)
	{
		if (id == g_language_table[i].id)
		{
			return &g_language_table[i];
		}
	}

	return NULL;
}

int config::get_language_table_count()
{
	return sizeof(g_language_table)/sizeof(g_language_table[0]);
}

int config::get_language_default()
{
	return g_language_default;
}

void config::set_language_default(int LanID)
{
	if (LanID>=0 && LanID<get_language_table_count())
	{
		g_language_default = LanID;
	}
}


BOOL config::config_init()
{
	TCHAR szFilepath[MAX_PATH] = L"";
	_stprintf(szFilepath, L"%s\\%s", tzutil::GetAppPath(), CONFIG_FILE_NAME);

	CWceUiXmlDoc xmlDoc;
	if (!xmlDoc.LoadXmlFile(szFilepath))
	{
		RETAILMSG(1, (L"[nvai_car]: config::config_init(), could not find config.xml file.\n"));
		return FALSE;
	}

	CWceUiXmlElem *pxmlElem = xmlDoc.RootElement();
	ASSERT(pxmlElem);

	// 语言
	_init_language(pxmlElem);

	// 时区
	_init_timezone(pxmlElem);

	// uart communication port
	_init_uart(pxmlElem);

	// audio
	_init_audio(pxmlElem);

	// disk
	_init_disk(pxmlElem);

	// video
	_init_video(pxmlElem);

	// sysinfo
	_init_sysinfo(pxmlElem);

	// bluetooth
	_init_bluetooth(pxmlElem);

	// avin
	_init_avin(pxmlElem);

	// functions
	_init_functions(pxmlElem);

	// canbus
	_init_canbus(pxmlElem);

	// touchkey
	_init_touchkey(pxmlElem);

	// misc
	_init_misc(pxmlElem);

	// other item

	//

	return TRUE;
}


TIMEZONE_TABLE_INFO* config::get_timezone_table()
{
	return &g_timezone_table[0];
}

int config::get_timezone_table_count()
{
	return sizeof(g_timezone_table)/sizeof(g_timezone_table[0]);
}

// 获取配置文件中的缺省时区
int config::get_timezone_default()
{
	return g_timezone_default;
}

BOOL config::get_timezone_dst_enable()
{
	return g_timezone_dst_enable;
}

CONFIG_AUDIO* config::get_config_audio()
{
	return &g_config_audio;
}

CONFIG_DISK* config::get_config_disk()
{
	return &g_config_disk;
}

CONFIG_VIDEO* config::get_config_video()
{
	return &g_config_video;
}

CONFIG_SYSINFO* config::get_config_sysinfo()
{
	return &g_config_sysinfo;
}


CONFIG_BLUETOOTH* config::get_config_bluetooth()
{
	return &g_config_bluetooth;
}

CONFIG_AVIN* config::get_config_avin()
{
	return &g_config_avin;
}

CONFIG_FUNCTIONS* config::get_config_functions()
{
	return &g_config_functions;
}

CONFIG_CANBUS* config::get_config_canbus()
{
	return &g_config_canbus;
}

CONFIG_TOUCHKEY* config::get_config_touchkey()
{
	return &g_config_touchkey;
}

CONFIG_MISC* config::get_config_misc()
{
	return &g_config_misc;
}

////////////////////// static helper functions /////////////////////


// 初始化语言表等
static void _init_language(CWceUiXmlElem *pxmlElem )
{
	LPCTSTR lpszValue = NULL;
	LPCTSTR lpszDefault = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	// language
	if (pXmlElemChild = pxmlElem->GetChildElem(L"Language"))
	{
		lpszValue = pXmlElemChild->GetAttr(L"value");
		lpszDefault = pXmlElemChild->GetAttr(L"default");
	}

	if (lpszValue)	// support language
	{
		CStringArray strKeyVector;
		tzutil::GetSubString(lpszValue,strKeyVector,',');
		for (int i=0; i<strKeyVector.GetSize(); i++)
		{
			BOOL bfound = FALSE;
			for (int j=0; j<config::get_language_table_count(); j++)
			{
				if (_tcscmp(strKeyVector.GetAt(i), g_language_table[j].file) == 0)
				{
					g_language_table[j].support = LAN_SUPPORT;
					bfound = TRUE;
					break;
				}
			}
			if (!bfound)
			{
				RETAILMSG(1, (L"[nvai_car]: config::config_init(), error value(%s) in config.xml.\n", strKeyVector.GetAt(i)));
			}
		}
	}

	// 如果没有配置该项或配置为空，则表示支持所有项
	if (lpszValue == NULL || _tcslen(lpszValue)==0)
	{
		for (int j=0; j<config::get_language_table_count(); j++)
		{
			g_language_table[j].support = LAN_SUPPORT;
		}
	}

	if (lpszDefault)	// default language
	{
		for (int j=0; j<config::get_language_table_count(); j++)
		{
			if (_tcscmp(lpszDefault, g_language_table[j].file) == 0)
			{
				g_language_default = g_language_table[j].id;
				break;
			}
		}
	}
}


// 初始化时区表等
static void _init_timezone(CWceUiXmlElem *pxmlElem )
{
	LPCTSTR lpszValue = NULL;
	LPCTSTR lpszDefault = NULL;
	LPCTSTR lpszDST = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	// timezone
	if (pXmlElemChild = pxmlElem->GetChildElem(L"Timezone"))
	{
		lpszValue = pXmlElemChild->GetAttr(L"value");
		lpszDefault = pXmlElemChild->GetAttr(L"default");
		lpszDST = pXmlElemChild->GetAttr(L"dst_enable");
	}

	if (lpszValue)	// support timezone
	{
		CStringArray strKeyVector;
		tzutil::GetSubString(lpszValue,strKeyVector,',');
		for (int i=0; i<strKeyVector.GetSize(); i++)
		{
			BOOL bfound = FALSE;
			for (int j=0; j<config::get_timezone_table_count(); j++)
			{
				if (_tcscmp(strKeyVector.GetAt(i), g_timezone_table[j].name) == 0)
				{
					g_timezone_table[j].support = TIMEZONE_SUPPORT;
					bfound = TRUE;
					break;
				}
			}
			if (!bfound)
			{
				RETAILMSG(1, (L"[nvai_car]: config::config_init(), error value(%s) in config.xml.\n", strKeyVector.GetAt(i)));
			}
		}
	}

	// 如果没有配置该项或配置为空，则表示支持所有项
	if (lpszValue == NULL || _tcslen(lpszValue)==0)
	{
		for (int j=0; j<config::get_timezone_table_count(); j++)
		{
			g_timezone_table[j].support = TIMEZONE_SUPPORT;
		}
	}

	if (lpszDefault == NULL)
	{
		lpszDefault = L"China Standard Time";	// 如果没配置，用北京时区
	}

	for (int j=0; j<config::get_timezone_table_count(); j++)
	{
		if (_tcscmp(lpszDefault, g_timezone_table[j].name) == 0)
		{
			g_timezone_default = j;		// 直接用索引值就可以了
			break;
		}
	}

	// DST
	if (lpszDST)
	{
		g_timezone_dst_enable = (_ttoi(lpszDST) != 0);
	}
}


// 获取MCU通信口及波特率
static void _init_uart(CWceUiXmlElem *pxmlElem )
{
	LPCTSTR lpszPort = NULL;
	LPCTSTR lpszBaudrate = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	if (pXmlElemChild = pxmlElem->GetChildElem(L"Uart"))
	{
		//mcu
		lpszPort = pXmlElemChild->GetAttr(L"mcu_port");
		lpszBaudrate = pXmlElemChild->GetAttr(L"mcu_baudrate");
		if (lpszPort)
		{
			g_config_uart.mcu_commport = _ttoi(lpszPort);
		}

		if (lpszBaudrate)
		{
			g_config_uart.mcu_baudrate = _ttoi(lpszBaudrate);	
		}

		// uart
		lpszPort = pXmlElemChild->GetAttr(L"gps_port");
		lpszBaudrate = pXmlElemChild->GetAttr(L"gps_baudrate");
		if (lpszPort)
		{
			g_config_uart.gps_commport = _ttoi(lpszPort);
		}

		if (lpszBaudrate)
		{
			g_config_uart.gps_baudrate = _ttoi(lpszBaudrate);	
		}
	}
}

// nIndexTable :  0 ~ extern UINT DVP_VOLUME_MSDK_TABLE[UI_MAX_VOLUME_LEVEL+1];
// 1~ extern UINT DVP_VOLUME_AUX_FRONT_TABLE[UI_MAX_VOLUME_LEVEL+1];
// 2 ~ extern UINT DVP_VOLUME_AUX_REAR_TABLE[UI_MAX_VOLUME_LEVEL+1];
// 3 ~ extern BYTE BT_VOLUME_TABLE[UI_MAX_VOLUME_LEVEL+1];
// 4 ~ extern WORD WINCE_VOLUME_TABLE[UI_MAX_VOLUME_LEVEL+1];

static void _init_volume_table(LPCTSTR lpszValue, int nIndexTable)
{
	if (lpszValue)
	{
		CStringArray strValueVector;
		tzutil::GetSubString(lpszValue,strValueVector,',');
		if (strValueVector.GetCount() == UI_MAX_VOLUME_LEVEL+1)	// 合法检查，如果表的数目不对，忽略
		{
			for (int i=0; i<UI_MAX_VOLUME_LEVEL+1; i++)
			{
				if (nIndexTable == 0)
				{
					DVP_VOLUME_MSDK_TABLE[i] = _wtoi(strValueVector.GetAt(i));
				}
				else if (nIndexTable == 1)
				{
					DVP_VOLUME_AUX_FRONT_TABLE[i] = _wtoi(strValueVector.GetAt(i));
				}
				else if (nIndexTable == 2)
				{
					DVP_VOLUME_AUX_REAR_TABLE[i] = _wtoi(strValueVector.GetAt(i));
				}
				else if (nIndexTable == 3)
				{
					BT_VOLUME_TABLE[i] = (BYTE)_wtoi(strValueVector.GetAt(i));
				}
				else if (nIndexTable == 4)
				{
					WINCE_VOLUME_TABLE[i] = (WORD)_wtoi(strValueVector.GetAt(i));
				}
			}
		}
	}
}

static void _init_audio(CWceUiXmlElem *pxmlElem)
{
	LPCTSTR lpszValue = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	// audio
	if (pXmlElemChild = pxmlElem->GetChildElem(L"Audio"))
	{
		lpszValue = pXmlElemChild->GetAttr(L"dac_on");
		if (lpszValue)
		{
			BYTE dac = (BYTE)_ttoi(lpszValue);
			g_config_audio.front_dac = dac & 0x01;
			g_config_audio.rear_dac = dac & 0x02;
		}

		lpszValue = pXmlElemChild->GetAttr(L"volume");
		if (lpszValue)
		{
			CStringArray strArray;
			tzutil::GetSubString(lpszValue, strArray, L',');
			if (strArray.GetCount() >= 4)
			{
				g_config_audio.media_front_volume = _ttoi(strArray.GetAt(0));
				g_config_audio.meida_rear_volume = _ttoi(strArray.GetAt(0));	// 后排媒体音量不提供单独配置
				g_config_audio.bt_volume = _ttoi(strArray.GetAt(1));
				g_config_audio.gps_volume = _ttoi(strArray.GetAt(2));
				g_config_audio.ta_volume =  _ttoi(strArray.GetAt(3));

			}
			else
			{
				RETAILMSG(1, (L"[nvai_car]: config::_init_audio(), error volume values in config.xml.\n"));
			}
		}

		lpszValue = pXmlElemChild->GetAttr(L"init_max_volume");
		if (lpszValue)	{ g_config_audio.init_max_volume = _ttoi(lpszValue);	}

		lpszValue = pXmlElemChild->GetAttr(L"key_beep");
		if (lpszValue)	{ g_config_audio.key_beep = (BOOL)_ttoi(lpszValue);	}

		lpszValue = pXmlElemChild->GetAttr(L"gps_gain");
		if (lpszValue)	{ g_config_audio.gps_gain = _ttoi(lpszValue);	}

		lpszValue = pXmlElemChild->GetAttr(L"gps_gain_off");
		if (lpszValue)	{ g_config_audio.gps_gain_off = (BOOL)_ttoi(lpszValue);	}

		lpszValue = pXmlElemChild->GetAttr(L"gps_set_gain_enable");
		if (lpszValue)	{ g_config_audio.gps_set_gain_enable = (BOOL)_ttoi(lpszValue);	}

		lpszValue = pXmlElemChild->GetAttr(L"dac_gpio");
		if (lpszValue)	{ g_config_audio.dac_gpio = (BOOL)_ttoi(lpszValue);	}

		lpszValue = pXmlElemChild->GetAttr(L"subwoofer");
		g_config_audio.subwoofer = lpszValue ? _ttoi(lpszValue) : 20;	// 缺省20

		lpszValue = pXmlElemChild->GetAttr(L"ui_delay");
		g_config_audio.ui_delay = lpszValue ?  _ttoi(lpszValue) : 3000;	// 缺省3秒

		lpszValue = pXmlElemChild->GetAttr(L"mute_by_app");
		g_config_audio.mute_by_app = lpszValue ?  _ttoi(lpszValue) : 0;	// 缺省不控制MCU mute

		// audio table
		lpszValue = pXmlElemChild->GetAttr(L"sd_audio_table");
		_init_volume_table(lpszValue, 0);
	
		lpszValue = pXmlElemChild->GetAttr(L"aux_front_audio_table");
		_init_volume_table(lpszValue, 1);

		lpszValue = pXmlElemChild->GetAttr(L"aux_rear_audio_table");
		_init_volume_table(lpszValue, 2);

		lpszValue = pXmlElemChild->GetAttr(L"bt_audio_table");
		_init_volume_table(lpszValue, 3);

		lpszValue = pXmlElemChild->GetAttr(L"system_audio_table");
		_init_volume_table(lpszValue, 4);


	}
}



static void _init_disk(CWceUiXmlElem *pxmlElem)
{
	LPCTSTR lpszValue = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	// audio
	if (pXmlElemChild = pxmlElem->GetChildElem(L"Disk"))
	{
		lpszValue = pXmlElemChild->GetAttr(L"sd");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			_tcsncpy(g_config_disk.sd, lpszValue, MAX_DISK_NAME_LEN);
			g_config_disk.sd[MAX_DISK_NAME_LEN] = L'\0';
		}

		lpszValue = pXmlElemChild->GetAttr(L"usb1");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			_tcsncpy(g_config_disk.usb1, lpszValue, MAX_DISK_NAME_LEN);
			g_config_disk.usb1[MAX_DISK_NAME_LEN] = L'\0';
		}

		lpszValue = pXmlElemChild->GetAttr(L"usb2");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			_tcsncpy(g_config_disk.usb2, lpszValue, MAX_DISK_NAME_LEN);
			g_config_disk.usb2[MAX_DISK_NAME_LEN] = L'\0';
		}

		lpszValue = pXmlElemChild->GetAttr(L"gps");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			CStringArray str_array;
			tzutil::GetSubString(lpszValue, str_array, L',');
			int count = min(str_array.GetCount(), MAX_DISK_GPS_PATH);	// 最多支持三个导航盘符
			for (int i=0; i<count; i++)
			{
				str_array.GetAt(i).Trim();
				_tcsncpy(g_config_disk.gps[i], str_array.GetAt(i), MAX_DISK_NAME_LEN);
				g_config_disk.gps[i][MAX_DISK_NAME_LEN] = L'\0';
			}
		}

		lpszValue = pXmlElemChild->GetAttr(L"vdisk");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			_tcsncpy(g_config_disk.vdisk, lpszValue, MAX_PATH);
			g_config_disk.vdisk[MAX_PATH-1] = L'\0';
		}

		lpszValue = pXmlElemChild->GetAttr(L"gps_path");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			_tcsncpy(g_config_disk.gps_path, lpszValue, MAX_PATH);
			g_config_disk.gps_path[MAX_PATH-1] = L'\0';
		}
	}
}

// 将字符串中逗号分隔的数字解析出来,赋值给参数
// standard="40,25,50,50"
static BOOL _get_video_params(LPCTSTR lpszValue, OUT BYTE* pbyte)
{
	if (!lpszValue || !pbyte)
	{
		return FALSE;
	}

	CStringArray strArray;
	tzutil::GetSubString(lpszValue, strArray, L',');
	if (strArray.GetCount() < 4)
	{
		return FALSE;
	}

	for (int i=0; i<4; i++)
	{
		*(pbyte+i) = (BYTE)_ttoi(strArray.GetAt(i));
	}

	return TRUE;
}

static void _init_video(CWceUiXmlElem *pxmlElem)
{
	LPCTSTR lpszValue = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	// video
	if (pXmlElemChild = pxmlElem->GetChildElem(L"Video"))
	{
		lpszValue = pXmlElemChild->GetAttr(L"standard");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			// 四个参数一定要连续且顺序存放才能这么调用,注意结构体的定义
			_get_video_params(lpszValue, &g_config_video.standard_brightness);
		}

		lpszValue = pXmlElemChild->GetAttr(L"colorful");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			_get_video_params(lpszValue, &g_config_video.colorful_brightness);
		}

		lpszValue = pXmlElemChild->GetAttr(L"soft");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			_get_video_params(lpszValue, &g_config_video.soft_brightness);
		}

		lpszValue = pXmlElemChild->GetAttr(L"aux");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			_get_video_params(lpszValue, &g_config_video.aux_brightness);
		}

		lpszValue = pXmlElemChild->GetAttr(L"camera");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			_get_video_params(lpszValue, &g_config_video.camera_brightness);
		}

		lpszValue = pXmlElemChild->GetAttr(L"camera_mirror");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_video.camera_mirror = (BYTE)_ttoi(lpszValue);
		}

		lpszValue = pXmlElemChild->GetAttr(L"parking");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_video.parking = (BYTE)_ttoi(lpszValue);
		}

		lpszValue = pXmlElemChild->GetAttr(L"h264");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_video.h264 = (BYTE)_ttoi(lpszValue);
		}
	}

	// backlight
	if (pXmlElemChild = pxmlElem->GetChildElem(L"Backlight"))
	{
		lpszValue = pXmlElemChild->GetAttr(L"normal");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_video.backlight_normal = (BYTE)_ttoi(lpszValue);
		}

		lpszValue = pXmlElemChild->GetAttr(L"night");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_video.backlight_night = (BYTE)_ttoi(lpszValue);
		}

		lpszValue = pXmlElemChild->GetAttr(L"min");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_video.backlight_min = (BYTE)_ttoi(lpszValue);
		}
	}

}


static void _init_sysinfo(CWceUiXmlElem *pxmlElem)
{
	LPCTSTR lpszValue = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	// audio
	if (pXmlElemChild = pxmlElem->GetChildElem(L"Version"))
	{
		lpszValue = pXmlElemChild->GetAttr(L"app");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			_tcsncpy(g_config_sysinfo.app_ver, lpszValue, sizeof(g_config_sysinfo.app_ver)/sizeof(TCHAR));
		}

		lpszValue = pXmlElemChild->GetAttr(L"manufacture");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			_tcsncpy(g_config_sysinfo.manufacture, lpszValue, sizeof(g_config_sysinfo.manufacture)/sizeof(TCHAR));
		}


	}
}

static void _init_bluetooth(CWceUiXmlElem *pxmlElem)
{
	LPCTSTR lpszValue = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	if (pXmlElemChild = pxmlElem->GetChildElem(L"Bluetooth"))
	{
		lpszValue = pXmlElemChild->GetAttr(L"mic_gain");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_bluetooth.mic_gain = _ttoi(lpszValue);
		}

		lpszValue = pXmlElemChild->GetAttr(L"auto_connect");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_bluetooth.auto_connect = (_ttoi(lpszValue) != 0);
		}

		lpszValue = pXmlElemChild->GetAttr(L"auto_answer");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_bluetooth.auto_answer = (_ttoi(lpszValue) != 0);
		}
	}
}


// 将字符串中逗号分隔的数字解析出来,赋值给参数
// avin="1,1"
static BOOL _get_avin_params(LPCTSTR lpszValue, OUT BYTE* pbyte)
{
	if (!lpszValue || !pbyte)
	{
		return FALSE;
	}

	CStringArray strArray;
	tzutil::GetSubString(lpszValue, strArray, L',');
	if (strArray.GetCount() < 2)
	{
		return FALSE;
	}

	for (int i=0; i<2; i++)
	{
		*(pbyte+i) = (BYTE)_ttoi(strArray.GetAt(i));
	}

	return TRUE;
}

static void _init_avin(CWceUiXmlElem *pxmlElem)
{
	LPCTSTR lpszValue = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	if (pXmlElemChild = pxmlElem->GetChildElem(L"Avin"))
	{
		BYTE params[2] = {0, 0};
		lpszValue = pXmlElemChild->GetAttr(L"avin1");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			if (_get_avin_params(lpszValue, params))
			{
				g_config_avin.avin1_a = params[0];
				g_config_avin.avin1_v = params[1];
			}
		}

		lpszValue = pXmlElemChild->GetAttr(L"avin2");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			if (_get_avin_params(lpszValue, params))
			{
				g_config_avin.avin2_a = params[0];
				g_config_avin.avin2_v = params[1];
			}
		}

		lpszValue = pXmlElemChild->GetAttr(L"fm");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			if (_get_avin_params(lpszValue, params))
			{
				g_config_avin.fm_a = params[0];
			}
		}

		lpszValue = pXmlElemChild->GetAttr(L"camera");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			if (_get_avin_params(lpszValue, params))
			{
				g_config_avin.camera_v = params[1];
			}
		}

		lpszValue = pXmlElemChild->GetAttr(L"tv");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			if (_get_avin_params(lpszValue, params))
			{
				g_config_avin.tv_a = params[0];
				g_config_avin.tv_v = params[1];
			}
		}
	}
}

void _init_functions(CWceUiXmlElem *pxmlElem)
{
	LPCTSTR lpszValue = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	if (pXmlElemChild = pxmlElem->GetChildElem(L"Functions"))
	{
		lpszValue = pXmlElemChild->GetAttr(L"value");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			tzutil::GetSubString(lpszValue, g_config_functions.support_array, L',');
		}

		lpszValue = pXmlElemChild->GetAttr(L"mode");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			tzutil::GetSubString(lpszValue, g_config_functions.mode_array, L',');
		}
	}
}

void _init_canbus(CWceUiXmlElem *pxmlElem)
{
	LPCTSTR lpszValue = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	if (pXmlElemChild = pxmlElem->GetChildElem(L"Canbus"))
	{
		lpszValue = pXmlElemChild->GetAttr(L"type");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_canbus.canbus_type = _ttoi(lpszValue);
		}
	}
}


void _init_touchkey(CWceUiXmlElem *pxmlElem)
{
	LPCTSTR lpszValue = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	if (pXmlElemChild = pxmlElem->GetChildElem(L"Touchkey"))
	{
		lpszValue = pXmlElemChild->GetAttr(L"key");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			CStringArray keyArray;
			tzutil::GetSubString(lpszValue, keyArray, L';');

			g_config_touchkey.count = min(keyArray.GetCount(), 
				sizeof(g_config_touchkey.key)/sizeof(g_config_touchkey.key[0]));
			for (int i=0; i<g_config_touchkey.count; i++)
			{
				CStringArray valueArray;
				tzutil::GetSubString(keyArray.GetAt(i), valueArray, L',');
				if (valueArray.GetCount() == 6)		// 数据要完整
				{
					for (int j=0; j<6; j++)
					{
						g_config_touchkey.key[i][j] = (WORD)_ttoi(valueArray.GetAt(j));
					}
				}
				else
				{
					memset(g_config_touchkey.key[i], 0, sizeof(g_config_touchkey.key[i]));
				}
			}
			
		}

		lpszValue = pXmlElemChild->GetAttr(L"align");
		g_config_touchkey.align = 0;	// 缺省横向
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_touchkey.align = _ttoi(lpszValue) ? 1 : 0;
		}

		lpszValue = pXmlElemChild->GetAttr(L"enable");
		g_config_touchkey.enable = 0;	// 缺省不支持
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_touchkey.enable = _ttoi(lpszValue) ? 1 : 0;
		}

	}
}



void _init_misc(CWceUiXmlElem *pxmlElem)
{
	LPCTSTR lpszValue = NULL;
	PCWceUiXmlElem pXmlElemChild = NULL;

	if (pXmlElemChild = pxmlElem->GetChildElem(L"Misc"))
	{
		lpszValue = pXmlElemChild->GetAttr(L"fm_area");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_misc.fm_area = _ttoi(lpszValue);
		}

		lpszValue = pXmlElemChild->GetAttr(L"colorful_lights");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_misc.colorful_lights = _ttoi(lpszValue);
		}

		g_config_misc.calibrate = 1;	// 缺省支持触摸校准
		lpszValue = pXmlElemChild->GetAttr(L"calibrate");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_misc.calibrate = _ttoi(lpszValue);
		}

		lpszValue = pXmlElemChild->GetAttr(L"parking");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_misc.parking = _ttoi(lpszValue);
		}

		lpszValue = pXmlElemChild->GetAttr(L"rds");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_misc.rds = _ttoi(lpszValue);
		}

		lpszValue = pXmlElemChild->GetAttr(L"24hourtime");
		if (lpszValue && _tcslen(lpszValue)>0)
		{
			g_config_misc.hourtime24 = _ttoi(lpszValue);
		}
	}
}

BOOL CONFIG_FUNCTIONS::is_support(LPCTSTR lpszFunctions)
{
	for (int i=0; i<support_array.GetCount(); i++)
	{
		if (lpszFunctions && _tcscmp(lpszFunctions, support_array.GetAt(i)) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CONFIG_FUNCTIONS::is_support(SOURCE_ID sid)
{
	LPCTSTR source_name = NULL;
	for (int i=0; i<sizeof(g_sid_table)/sizeof(g_sid_table[0]); i++)
	{
		if (sid == g_sid_table[i].sid)
		{
			source_name = g_sid_table[i].lpszFuncName;
			break;
		}
	}

	if (source_name)
	{
		for (int i=0; i<support_array.GetCount(); i++)
		{
			if (_tcscmp(source_name, support_array.GetAt(i)) == 0)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CONFIG_FUNCTIONS::remove(LPCTSTR lpszFunctions)
{
	for (int i=0; i<support_array.GetCount(); i++)
	{
		if (lpszFunctions && _tcscmp(lpszFunctions, support_array.GetAt(i)) == 0)
		{
			support_array.RemoveAt(i);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CONFIG_FUNCTIONS::add(LPCTSTR lpszFunctions)
{
	if (lpszFunctions)
	{
		for (int i=0; i<support_array.GetCount(); i++)
		{
			if (lpszFunctions && _tcscmp(lpszFunctions, support_array.GetAt(i)) == 0)
			{
				RETAILMSG(1, (L"[navi car]: warning ~ Functions already supported.\n"));
				return TRUE;
			}
		}

		support_array.Add(lpszFunctions);
		return TRUE;
	}

	return FALSE;
}

struct SID_SNAME_TABLE
{
	SOURCE_ID sid;
	LPCTSTR sname;
};

// 这个表定义的是sid与配置文件中配置的源名称的对应关系
static SID_SNAME_TABLE sid_sname_table[] = { 
	{SOURCE_DVD, L"dvd"}, {SOURCE_SD, L"sd"},		{SOURCE_USB, L"usb"},	 {SOURCE_FM, L"fm"},
	{SOURCE_TV, L"tv"},   {SOURCE_AVIN1,L"avin1"},	{SOURCE_AVIN2, L"avin2"},{SOURCE_BT, L"bt"},
	{SOURCE_IPOD_USB, L"ipod"},	{SOURCE_USB_REAR, L"usb_rear"}, {SOURCE_VDISK, L"vdisk"}, {SOURCE_PHONE_LINK, L"phonelink"}	};

SOURCE_ID CONFIG_FUNCTIONS::get_sid_by_sname(LPCTSTR sname)
{
	if (sname)
	{
		for (int i=0; i<sizeof(sid_sname_table)/sizeof(sid_sname_table[0]); i++)
		{
			if (_tcscmp(sname, sid_sname_table[i].sname) == 0)
			{
				return sid_sname_table[i].sid;
			}
		}
	}

	return INVALID_SOURCE_ID;
}

LPCTSTR CONFIG_FUNCTIONS::get_sname_by_sid(SOURCE_ID sid)
{
	for (int i=0; i<sizeof(sid_sname_table)/sizeof(sid_sname_table[0]); i++)
	{
		if (sid == sid_sname_table[i].sid)
		{
			return sid_sname_table[i].sname;
		}
	}

	return NULL;
}


SOURCE_ID CONFIG_FUNCTIONS::next_sid(SOURCE_ID sid)
{
	LPCTSTR sname = get_sname_by_sid(sid);
	int index = 0;
	if (sname)
	{
		for (int i=0; i<mode_array.GetCount(); i++)
		{
			if (_tcscmp(sname, mode_array.GetAt(i)) == 0)
			{
				index = i+1;	// 取下一个源
				break;
			}
		}
	}

	if (index>=mode_array.GetCount())
	{
		index = 0;
	}

	if (mode_array.GetCount()>0)	// 有定义源顺序
	{
		return get_sid_by_sname(mode_array.GetAt(index));
	}
	else
	{
		return INVALID_SOURCE_ID;
	}
}


CONFIG_UART* config::get_config_uart()
{
	return &g_config_uart;
}

// 触摸按键
int CONFIG_TOUCHKEY::get_count()
{
	return count;
}

RECT* CONFIG_TOUCHKEY::get_rect(int index)
{
	static RECT rc = {0,0,0,0};
	if (index>=0 && index<count)
	{
		rc.left = key[index][0];
		rc.top = key[index][1];
		rc.right = key[index][2];
		rc.bottom = key[index][3];
		return &rc;
	}
	else
	{
		return NULL;
	}
}

WORD CONFIG_TOUCHKEY::get_key(int index)
{
	if (index>=0 && index<count)
	{
		return key[index][4];
	}
	else
	{
		return 0;
	}

}

WORD CONFIG_TOUCHKEY::get_key_long(int index)
{
	if (index>=0 && index<count)
	{
		return key[index][5];
	}
	else
	{
		return 0;
	}

}


int CONFIG_TOUCHKEY::get_align()
{
	return align;
}

BOOL CONFIG_TOUCHKEY::get_enable()
{
	return enable;
}