#pragma once
#include "ObexMCU.h"
#include "protocol.h"
#include "MCUHandler.h"

typedef void (WINAPI *RPC_MSGHANDLER_CALLBACK)(LPARAM);

typedef enum
{
	RPC_SUCCESS						= 0x00 ,
	RPC_FAIL				= 0x01 ,
} RPCERR;



class CRpcMCU  
{
public:
	CRpcMCU();
	virtual ~CRpcMCU();
// Opreations
public:
	static CRpcMCU* GetInstance();
	static void WINAPI RPC_OnReceive(OBEX_Object* pData,int nDataLen, LPARAM lParam);
	LONG Open(LPCTSTR lpszDevive);
	LONG Close();
	BOOL IsOpen() { return m_pObex != NULL;}

	// Register a callback function which will be called when RPC is executing and 
	// there are messages in the calling thread message queue.
	void RegMSGCallback(RPC_MSGHANDLER_CALLBACK pfnCallback, LPARAM lParam);

	CObexMCU* GetObex() { return m_pObex; }
	CDEPSerial* GetDEP();
	void CancelCmd() { SetEvent(m_hEventCancelCmd); }


	LONG RPC_ShakeHand(RPCERR* pRetCode=NULL);
	// 将有效数据COPY到pBuffer指的缓冲区，并且返回数据中有多少字节是extra data.
	// 缓冲区必须足够大
	LONG RPC_GetInitTable(OUT void* pBuffer, OUT BYTE* pExtraDataLen, RPCERR* pRetCode=NULL);
	// 缓冲区大小必须与nIndex指向的参数表对应，否则缓冲区有可能越界
	// nIndex指定哪个参数表
	LONG RPC_GetSysParams(int nIndex, OUT void* pBuffer, int nBufferLen, RPCERR* pRetCode=NULL);
	// nIndex指定哪个参数表
	LONG RPC_SetSysParams(int nIndex, void* pdata, int nLen, RPCERR* pRetCode=NULL);

	LONG RPC_GetStatus(MCU_STATUS* pMcuStatus, RPCERR* pRetCode=NULL);

	// 设置音量
	LONG RPC_SetVolume(IN MCU_VOLUME_TABLE* pVolumeTable, RPCERR* pRetCode=NULL);
	LONG RPC_GetVolume(OUT MCU_VOLUME_TABLE* pVolumeTable, RPCERR* pRetCode=NULL);

	// 切换源
	LONG RPC_SetSource(int nFrontSource, int nRearSource=0, RPCERR* pRetCode=NULL);

	// key ~ 按键类型， param ~ 参数
	LONG RPC_KeyCommand(UINT key, UINT param=0, RPCERR* pRetCode=NULL);

	// buffer要足够大(要大于或等于33字符, MCU版本号最长32字节)
	LONG RPC_GetMcuVer(OUT TCHAR* pMcuVer, RPCERR* pRetCode=NULL);

	// 发送SWC学习请求 0 ~ 采样结束， 1 ~ 开始采样， 2 ~ 确认保存， 3 ~ 学习复位
	LONG RPC_SWCCommand(int cmd, RPCERR* pRetCode=NULL);
	// 主动获取swc table
	LONG RPC_SWCGetTable(MCU_SWC_TABLE* ptable, RPCERR* pRetCode=NULL);

	// MCU升级
	// bStart : TRUE ~ 开始升级, FALSE ~ 结束升级
	LONG RPC_UpdateMCU(BOOL bStart, RPCERR* pRetCode=NULL);
	// 向MCU发送MCU升级文件数据,不同的MCU可能对长度的限制不一样，需注意
	LONG RPC_SendMCUData(LPVOID pData, UINT nDataLen, RPCERR* pRetCode=NULL);

	// 主动获取radio信息
	// type : 0 ~ 当前频点信息 1 ~ preset list信息 2 ~ PS，PTY  3 ~ RT信息
	LONG RPC_GetRadioData(int type, RPCERR* pRetCode=NULL);

	// 设置当前使用canbus类型
	LONG RPC_SetCanbusType(int type, RPCERR* pRetCode=NULL);
	// 向MCU发送CAN数据,由MCU转发给以CAN盒
	LONG RPC_SetCanbusData(LPVOID pData, UINT nDataLen, RPCERR* pRetCode=NULL);

	// 向MCU发送ARM状态
	LONG RPC_SetArmStatus(MCU_ARM_STATUS* pstatus, RPCERR* pRetCode=NULL);

	// 向MCU发送小屏显示状态
	// pData 16个字节数据
	LONG RPC_LedDisplayData(LPVOID pData, RPCERR* pRetCode=NULL);

protected:
	void OnReceive(OBEX_Object* pData,int nDataLen);
	LONG CmdRequest(DWORD dwTimeout);
	
// Attributes
protected:
	CObexMCU* m_pObex;
	LONG m_lLastError;
	OBEX_Object* m_pObexObjectS;
	OBEX_Object* m_pObexObjectR;

	HANDLE m_hEventReply;
	HANDLE m_hEventCancelCmd;
	
	RPC_MSGHANDLER_CALLBACK m_pfnMsgHandlerCallback;
	LPARAM m_lParam;	// parameter for above callback function

	CMCUHandler m_MCUHandler;
};
