#pragma once

#include "ObexMCU.h"
#include "protocol.h"

#define TIMER_ID_KEY_CONFIRM	0xAB0D3

// 用来处理MCU主动发送上来的数据
class CMCUHandler
{
public:
	CMCUHandler();
	~CMCUHandler();

	static void WINAPI MCU_OnReceive(LPVOID pData,int nDataLen, LPARAM lParam);
	// 用来存储复合数字键,如按了1,2,3则保存了123
	static UINT m_nDigitalKey;

protected:
	void OnReceive(BYTE* pData,int nDataLen);
	// 处理一个完整的数据包
	void HandleObject(OBEX_Object *pobject);
	// 单独处理音量键
	void HandleVolumeKey(BYTE* pData,int nDataLen);


	void OnStatusChange(MCU_STATUS *pstatus);
	void OnSysParaChange(BYTE* pdata);
	void OnAccChange(BOOL bAccOn);
	void OnKeyCommand(UINT key, UINT param);
	void OnRadioData(int nDataLen, int nInfoType, BYTE* pdata);
	void OnSWCTable(MCU_SWC_TABLE* pswc_table);
	void OnSWCKeyResult(BYTE result[2]);

	void OnCanBusData(int nDataLen, BYTE* pdata);

	// 如果是数字键,进行处理并返回TRUE，否则返回FALSE
	BOOL HandleDigitalKey(UINT key, UINT param);
};

