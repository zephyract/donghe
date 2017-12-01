#pragma once

#include "ObexMCU.h"
#include "protocol.h"

#define TIMER_ID_KEY_CONFIRM	0xAB0D3

// ��������MCU������������������
class CMCUHandler
{
public:
	CMCUHandler();
	~CMCUHandler();

	static void WINAPI MCU_OnReceive(LPVOID pData,int nDataLen, LPARAM lParam);
	// �����洢�������ּ�,�簴��1,2,3�򱣴���123
	static UINT m_nDigitalKey;

protected:
	void OnReceive(BYTE* pData,int nDataLen);
	// ����һ�����������ݰ�
	void HandleObject(OBEX_Object *pobject);
	// ��������������
	void HandleVolumeKey(BYTE* pData,int nDataLen);


	void OnStatusChange(MCU_STATUS *pstatus);
	void OnSysParaChange(BYTE* pdata);
	void OnAccChange(BOOL bAccOn);
	void OnKeyCommand(UINT key, UINT param);
	void OnRadioData(int nDataLen, int nInfoType, BYTE* pdata);
	void OnSWCTable(MCU_SWC_TABLE* pswc_table);
	void OnSWCKeyResult(BYTE result[2]);

	void OnCanBusData(int nDataLen, BYTE* pdata);

	// ��������ּ�,���д�������TRUE�����򷵻�FALSE
	BOOL HandleDigitalKey(UINT key, UINT param);
};

