#pragma once

#define WM_CLOSE_EARMIC            WM_USER + 3001 //闭启耳机咪头
#define WM_OPEN_EARMIC             WM_USER + 3002 //开启耳机咪头
#define WM_UPDT_MAPINFO            WM_USER + 3003 //下发信息点
#define WM_UPDT_USERCALLNO         WM_USER + 3004 //下发代用户拨打号码 
#define WM_UPDT_CALLCENTERNO       WM_USER + 3005 //下发后台号码

#define WM_DTMFDECODE_ADCRATE      WM_USER + 3006 //ADC速率的回调
#define WM_DTMFDECODE_EVENT        WM_USER + 3007 //事件通知
#define WM_ECARONEKEY_START        WM_USER + 3008 //ECAR一键通广播启动
#define WM_ECARONEKEY_EXIT         WM_USER + 3014 //通知已启动的程序关闭退出

#define WM_UPDT_CALLBYNAME         WM_USER + 3010 //代用户拨号，根据下发的姓名，ADDBY jobs 2013/8/26


#define WM_BTHANGUP           WM_USER + 3011 //通知蓝牙挂机 add by jobs 2013/10/16 for 上传功能
#define WM_BTDIALEXTNUMBER    WM_USER + 3012 //拨打分机号 add by jobs 2013/10/16 for 上传功能

#define WM_ACTIVATIONCODE    WM_USER + 3013 //在线激活 add by jobs 2013/11/5 for 上传功能

typedef const char cchar;
typedef wchar_t wchar;

typedef struct
{
	//1：84坐标
	//2：道道通坐标
	char cFlagX;

	//导航方式	//0：系统推荐，
	//1：高速优先，
	//2：经济路线，
	//3：最短距离，
	char cFlagNavigation;

	//目的点纬度	
	//X为2时,道道通坐标；为1时，84坐标，
	//单位为度，小数部分保留5位，如果为南纬，则前面加‘-’表示负数
	double  idesLatitude;

	//目的点经度	
	//X为2时, 道道通坐标；为1时，84坐标，
	//单位为度，小数部分保留5位，如果为西经，则前面加‘-’表示负数
	double idesLongitude;

	//目的地名	//Unicode字符，属宽字节
	const wchar_t *pDesPlacenames;
}* PMAP_INFO, MAP_INFO;

//闭启耳机咪头
typedef BOOL (*ECARONEKEY_CLOSE_EARMIC_FUN)(LPVOID pParam);
//开启耳机咪头
typedef BOOL (*ECARONEKEY_OPEN_EARMIC_FUN)(LPVOID pParam);
//下发信息点PMAP_INFO是一个结构变量，请参考消息机制说明
typedef BOOL (*ECARONEKEY_UPDT_MAPINFO_FUN)(LPVOID pParam, PMAP_INFO pMapInfo, LPCSTR pTncCode);

//说明：1.0版本不提供以下功能
//下发代用户拨打号码
typedef BOOL (*ECARONEKEY_UPDT_USERCALLNO_FUN)(LPVOID pParam, LPCSTR pUserCallId);
//下发后台号码
typedef BOOL (*ECARONEKEY_UPDT_CALLCENTERNO_FUN)(LPVOID pParam, DWORD dwFlag, LPCSTR lpMsg);

//读ADC速率的回调,130322 add by jobs
typedef  BOOL  (*DTMFDECODE_ADCRATE_FUN)(LPVOID lpParam, DWORD dwRate);

//事件通知回调函数,130322 add by jobs
typedef  BOOL  (*DTMFDECODE_EVENT_FUN)(LPVOID lpParam, DWORD dwFlag, LPVOID lpMsg);

//代用户拨号，根据下发的姓名，ADDBY jobs 2013/8/26
typedef  BOOL  (*ECARONEKEY_UPDT_CALLCENTERNOBYNAME_FUN)(LPVOID lpParam, LPBYTE pName, DWORD dwLen);

//通知蓝牙挂机 add by jobs 2013/10/16 for 上传功能
typedef  BOOL  (*ECARONEKEY_BTHANGUP_FUN)(LPVOID lpParam);

//拨打分机号 add by jobs 2013/10/16 for 上传功能
typedef  BOOL  (*ECARONEKEY_BTDIALEXTNUMBER_FUN)(LPVOID lpParam, const char* pExtNumber);

//在线激活 add by jobs 2013/11/5 for 上传功能
typedef  BOOL  (*ECARONEKEY_ACTIVATIONCODE_FUN)(LPVOID lpParam, int nFlag);


class CECarObject
{
public:
	CECarObject(void){};
	virtual ~CECarObject(void){};


	//取得后台号码
	virtual BOOL GetCallCenterNo(unsigned char* pCallNo)=0;

	//关闭耳机咪头
	virtual BOOL RegisterCloseEarMicFun(ECARONEKEY_CLOSE_EARMIC_FUN pCloseEarMic)=0;

	//开启耳机咪头
	virtual BOOL RegisterOpenEarMicFun(ECARONEKEY_OPEN_EARMIC_FUN pOpenEarMic)=0;

	//下发信息点
	virtual BOOL RegisterUpdtMapInfoFun(ECARONEKEY_UPDT_MAPINFO_FUN pUpdtMapInfo)=0;

//说明：1.0版本不提供以下功能
	//下发代用户拨打号码
	virtual BOOL RegisterUpdtUserCallNoFun(ECARONEKEY_UPDT_USERCALLNO_FUN pUpdtUserCallNo)=0;

	//下发后台号码
	virtual BOOL RegisterUpdtCallCenterNoFun(ECARONEKEY_UPDT_CALLCENTERNO_FUN pUpdtCallCenterNo)=0;

	//读ADC速率的回调,130322 add by jobs
	virtual BOOL RegisterDtmfDeocdeAdcRateFun(DTMFDECODE_ADCRATE_FUN pDtmfDeocdeAdcRate)=0;

	//事件通知回调函数,130322 add by jobs
	virtual BOOL RegisterDtmfDeocdeEventFun(DTMFDECODE_EVENT_FUN pDtmfDeocdeEventFun)=0;

	//代用户拨号，根据下发的姓名，ADDBY jobs 2013/8/26
	virtual BOOL RegisterUpdtUserCallNoByNameFun(ECARONEKEY_UPDT_CALLCENTERNOBYNAME_FUN pUpdtUserCallNoByName)=0;

	//启动业务处理
	virtual BOOL Start(LPVOID pParam)=0;

	//停止业务处理
	virtual void Stop()=0;

	//DTMF码处理,by jobs 20130807
	virtual BOOL CheckDTMFCode(BYTE DtmfCode)=0;

	//通知蓝牙挂机 add by jobs 2013/10/16 for 上传功能
	virtual BOOL RegisterBTHangupFun(ECARONEKEY_BTHANGUP_FUN pBTHangupFun)=0;
	//拨打分机号 add by jobs 2013/10/16 for 上传功能
	virtual BOOL RegisterBTDialExtNumberFun(ECARONEKEY_BTDIALEXTNUMBER_FUN pDialExtNumber)=0;
	//输出DTMF码给后台 add by jobs 2013/10/16 for 上传功能
	//#上传方式，0：自动完成，1：MTK，2：MStar2531，3：MStar785，4:CSRA6
	virtual BOOL DtmfEncodeOut(int nFlag, const char* pDtmfData)=0;
	//在线激活 add by jobs 2013/11/5 for 上传功能
	virtual BOOL RegisterActivationCodeFun(ECARONEKEY_ACTIVATIONCODE_FUN pActivationCodeFun)=0;

	//在线激活,上传激活码
	virtual BOOL UploadActivationCode(cchar* ActiCode)=0;

	//设置蓝牙状态
	virtual BOOL SetBtState(int state)=0;

public:
	char     m_NotifyFlag; //沟通机制,1:消息，2：回调函数，有没父窗口时使用
	HWND     m_hwnParent;  //用于通知应层的父窗口消息

};

//蓝牙状态定义
#define CEBT_STA_INIT (0)//	~初始化
#define CEBT_STA_IDEL (1)//	~待机状态
#define CEBT_STA_CONN (2)//	~连接中
#define CEBT_STA_COOK (3)//	~连接成功
#define CEBT_STA_CAOU (4)//	~电话拨出
#define CEBT_STA_CAIN (5)//	~电话打入
#define CEBT_STA_SPKN (6)//	~通话中

//创建ECar 一键通对象
int WINAPI CreateEcarObject(void** ppObject);

//销毁ECar 一键通对象
int WINAPI DestroyEcarObject(void* pObject);

//取得ECAR库版本号
int WINAPI GetOneKeyDllVer(char* pOutFile, char* pOutProd);

//分离DTMF码
int WINAPI GetDtmfCode(BYTE data, BYTE* pOutDtmfCode);

//设定导航路径
void WINAPI SetNaviPath(const char* pNaviPath);

//将TNC码发送给地图
int WINAPI SendTNCToMap(const char* pTncCode);

//Byte类型的电话号码转换成ASC类型
BOOL WINAPI ByteToASC(byte CallNo[32]);

//设定功能码变量函数
//0：一键导航
//1：代用户拨号
//2：修改后台号码
//3：激活
void WINAPI SetFuncCode(int nCode);

//设定机器码函数
void WINAPI SetMachCode(char* pMachCode);

//是否已激活,TRUE:激活，FALSE:未激活
BOOL CheckClientActiveState();

//获取当前库工作路径
BOOL GetCurrentDir(wchar* pPathOut);
