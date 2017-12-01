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
	// ����Ч����COPY��pBufferָ�Ļ����������ҷ����������ж����ֽ���extra data.
	// �����������㹻��
	LONG RPC_GetInitTable(OUT void* pBuffer, OUT BYTE* pExtraDataLen, RPCERR* pRetCode=NULL);
	// ��������С������nIndexָ��Ĳ������Ӧ�����򻺳����п���Խ��
	// nIndexָ���ĸ�������
	LONG RPC_GetSysParams(int nIndex, OUT void* pBuffer, int nBufferLen, RPCERR* pRetCode=NULL);
	// nIndexָ���ĸ�������
	LONG RPC_SetSysParams(int nIndex, void* pdata, int nLen, RPCERR* pRetCode=NULL);

	LONG RPC_GetStatus(MCU_STATUS* pMcuStatus, RPCERR* pRetCode=NULL);

	// ��������
	LONG RPC_SetVolume(IN MCU_VOLUME_TABLE* pVolumeTable, RPCERR* pRetCode=NULL);
	LONG RPC_GetVolume(OUT MCU_VOLUME_TABLE* pVolumeTable, RPCERR* pRetCode=NULL);

	// �л�Դ
	LONG RPC_SetSource(int nFrontSource, int nRearSource=0, RPCERR* pRetCode=NULL);

	// key ~ �������ͣ� param ~ ����
	LONG RPC_KeyCommand(UINT key, UINT param=0, RPCERR* pRetCode=NULL);

	// bufferҪ�㹻��(Ҫ���ڻ����33�ַ�, MCU�汾���32�ֽ�)
	LONG RPC_GetMcuVer(OUT TCHAR* pMcuVer, RPCERR* pRetCode=NULL);

	// ����SWCѧϰ���� 0 ~ ���������� 1 ~ ��ʼ������ 2 ~ ȷ�ϱ��棬 3 ~ ѧϰ��λ
	LONG RPC_SWCCommand(int cmd, RPCERR* pRetCode=NULL);
	// ������ȡswc table
	LONG RPC_SWCGetTable(MCU_SWC_TABLE* ptable, RPCERR* pRetCode=NULL);

	// MCU����
	// bStart : TRUE ~ ��ʼ����, FALSE ~ ��������
	LONG RPC_UpdateMCU(BOOL bStart, RPCERR* pRetCode=NULL);
	// ��MCU����MCU�����ļ�����,��ͬ��MCU���ܶԳ��ȵ����Ʋ�һ������ע��
	LONG RPC_SendMCUData(LPVOID pData, UINT nDataLen, RPCERR* pRetCode=NULL);

	// ������ȡradio��Ϣ
	// type : 0 ~ ��ǰƵ����Ϣ 1 ~ preset list��Ϣ 2 ~ PS��PTY  3 ~ RT��Ϣ
	LONG RPC_GetRadioData(int type, RPCERR* pRetCode=NULL);

	// ���õ�ǰʹ��canbus����
	LONG RPC_SetCanbusType(int type, RPCERR* pRetCode=NULL);
	// ��MCU����CAN����,��MCUת������CAN��
	LONG RPC_SetCanbusData(LPVOID pData, UINT nDataLen, RPCERR* pRetCode=NULL);

	// ��MCU����ARM״̬
	LONG RPC_SetArmStatus(MCU_ARM_STATUS* pstatus, RPCERR* pRetCode=NULL);

	// ��MCU����С����ʾ״̬
	// pData 16���ֽ�����
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
