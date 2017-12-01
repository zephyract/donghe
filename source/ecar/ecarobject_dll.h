#pragma once

#define WM_CLOSE_EARMIC            WM_USER + 3001 //����������ͷ
#define WM_OPEN_EARMIC             WM_USER + 3002 //����������ͷ
#define WM_UPDT_MAPINFO            WM_USER + 3003 //�·���Ϣ��
#define WM_UPDT_USERCALLNO         WM_USER + 3004 //�·����û�������� 
#define WM_UPDT_CALLCENTERNO       WM_USER + 3005 //�·���̨����

#define WM_DTMFDECODE_ADCRATE      WM_USER + 3006 //ADC���ʵĻص�
#define WM_DTMFDECODE_EVENT        WM_USER + 3007 //�¼�֪ͨ
#define WM_ECARONEKEY_START        WM_USER + 3008 //ECARһ��ͨ�㲥����
#define WM_ECARONEKEY_EXIT         WM_USER + 3014 //֪ͨ�������ĳ���ر��˳�

#define WM_UPDT_CALLBYNAME         WM_USER + 3010 //���û����ţ������·���������ADDBY jobs 2013/8/26


#define WM_BTHANGUP           WM_USER + 3011 //֪ͨ�����һ� add by jobs 2013/10/16 for �ϴ�����
#define WM_BTDIALEXTNUMBER    WM_USER + 3012 //����ֻ��� add by jobs 2013/10/16 for �ϴ�����

#define WM_ACTIVATIONCODE    WM_USER + 3013 //���߼��� add by jobs 2013/11/5 for �ϴ�����

typedef const char cchar;
typedef wchar_t wchar;

typedef struct
{
	//1��84����
	//2������ͨ����
	char cFlagX;

	//������ʽ	//0��ϵͳ�Ƽ���
	//1���������ȣ�
	//2������·�ߣ�
	//3����̾��룬
	char cFlagNavigation;

	//Ŀ�ĵ�γ��	
	//XΪ2ʱ,����ͨ���ꣻΪ1ʱ��84���꣬
	//��λΪ�ȣ�С�����ֱ���5λ�����Ϊ��γ����ǰ��ӡ�-����ʾ����
	double  idesLatitude;

	//Ŀ�ĵ㾭��	
	//XΪ2ʱ, ����ͨ���ꣻΪ1ʱ��84���꣬
	//��λΪ�ȣ�С�����ֱ���5λ�����Ϊ��������ǰ��ӡ�-����ʾ����
	double idesLongitude;

	//Ŀ�ĵ���	//Unicode�ַ��������ֽ�
	const wchar_t *pDesPlacenames;
}* PMAP_INFO, MAP_INFO;

//����������ͷ
typedef BOOL (*ECARONEKEY_CLOSE_EARMIC_FUN)(LPVOID pParam);
//����������ͷ
typedef BOOL (*ECARONEKEY_OPEN_EARMIC_FUN)(LPVOID pParam);
//�·���Ϣ��PMAP_INFO��һ���ṹ��������ο���Ϣ����˵��
typedef BOOL (*ECARONEKEY_UPDT_MAPINFO_FUN)(LPVOID pParam, PMAP_INFO pMapInfo, LPCSTR pTncCode);

//˵����1.0�汾���ṩ���¹���
//�·����û��������
typedef BOOL (*ECARONEKEY_UPDT_USERCALLNO_FUN)(LPVOID pParam, LPCSTR pUserCallId);
//�·���̨����
typedef BOOL (*ECARONEKEY_UPDT_CALLCENTERNO_FUN)(LPVOID pParam, DWORD dwFlag, LPCSTR lpMsg);

//��ADC���ʵĻص�,130322 add by jobs
typedef  BOOL  (*DTMFDECODE_ADCRATE_FUN)(LPVOID lpParam, DWORD dwRate);

//�¼�֪ͨ�ص�����,130322 add by jobs
typedef  BOOL  (*DTMFDECODE_EVENT_FUN)(LPVOID lpParam, DWORD dwFlag, LPVOID lpMsg);

//���û����ţ������·���������ADDBY jobs 2013/8/26
typedef  BOOL  (*ECARONEKEY_UPDT_CALLCENTERNOBYNAME_FUN)(LPVOID lpParam, LPBYTE pName, DWORD dwLen);

//֪ͨ�����һ� add by jobs 2013/10/16 for �ϴ�����
typedef  BOOL  (*ECARONEKEY_BTHANGUP_FUN)(LPVOID lpParam);

//����ֻ��� add by jobs 2013/10/16 for �ϴ�����
typedef  BOOL  (*ECARONEKEY_BTDIALEXTNUMBER_FUN)(LPVOID lpParam, const char* pExtNumber);

//���߼��� add by jobs 2013/11/5 for �ϴ�����
typedef  BOOL  (*ECARONEKEY_ACTIVATIONCODE_FUN)(LPVOID lpParam, int nFlag);


class CECarObject
{
public:
	CECarObject(void){};
	virtual ~CECarObject(void){};


	//ȡ�ú�̨����
	virtual BOOL GetCallCenterNo(unsigned char* pCallNo)=0;

	//�رն�����ͷ
	virtual BOOL RegisterCloseEarMicFun(ECARONEKEY_CLOSE_EARMIC_FUN pCloseEarMic)=0;

	//����������ͷ
	virtual BOOL RegisterOpenEarMicFun(ECARONEKEY_OPEN_EARMIC_FUN pOpenEarMic)=0;

	//�·���Ϣ��
	virtual BOOL RegisterUpdtMapInfoFun(ECARONEKEY_UPDT_MAPINFO_FUN pUpdtMapInfo)=0;

//˵����1.0�汾���ṩ���¹���
	//�·����û��������
	virtual BOOL RegisterUpdtUserCallNoFun(ECARONEKEY_UPDT_USERCALLNO_FUN pUpdtUserCallNo)=0;

	//�·���̨����
	virtual BOOL RegisterUpdtCallCenterNoFun(ECARONEKEY_UPDT_CALLCENTERNO_FUN pUpdtCallCenterNo)=0;

	//��ADC���ʵĻص�,130322 add by jobs
	virtual BOOL RegisterDtmfDeocdeAdcRateFun(DTMFDECODE_ADCRATE_FUN pDtmfDeocdeAdcRate)=0;

	//�¼�֪ͨ�ص�����,130322 add by jobs
	virtual BOOL RegisterDtmfDeocdeEventFun(DTMFDECODE_EVENT_FUN pDtmfDeocdeEventFun)=0;

	//���û����ţ������·���������ADDBY jobs 2013/8/26
	virtual BOOL RegisterUpdtUserCallNoByNameFun(ECARONEKEY_UPDT_CALLCENTERNOBYNAME_FUN pUpdtUserCallNoByName)=0;

	//����ҵ����
	virtual BOOL Start(LPVOID pParam)=0;

	//ֹͣҵ����
	virtual void Stop()=0;

	//DTMF�봦��,by jobs 20130807
	virtual BOOL CheckDTMFCode(BYTE DtmfCode)=0;

	//֪ͨ�����һ� add by jobs 2013/10/16 for �ϴ�����
	virtual BOOL RegisterBTHangupFun(ECARONEKEY_BTHANGUP_FUN pBTHangupFun)=0;
	//����ֻ��� add by jobs 2013/10/16 for �ϴ�����
	virtual BOOL RegisterBTDialExtNumberFun(ECARONEKEY_BTDIALEXTNUMBER_FUN pDialExtNumber)=0;
	//���DTMF�����̨ add by jobs 2013/10/16 for �ϴ�����
	//#�ϴ���ʽ��0���Զ���ɣ�1��MTK��2��MStar2531��3��MStar785��4:CSRA6
	virtual BOOL DtmfEncodeOut(int nFlag, const char* pDtmfData)=0;
	//���߼��� add by jobs 2013/11/5 for �ϴ�����
	virtual BOOL RegisterActivationCodeFun(ECARONEKEY_ACTIVATIONCODE_FUN pActivationCodeFun)=0;

	//���߼���,�ϴ�������
	virtual BOOL UploadActivationCode(cchar* ActiCode)=0;

	//��������״̬
	virtual BOOL SetBtState(int state)=0;

public:
	char     m_NotifyFlag; //��ͨ����,1:��Ϣ��2���ص���������û������ʱʹ��
	HWND     m_hwnParent;  //����֪ͨӦ��ĸ�������Ϣ

};

//����״̬����
#define CEBT_STA_INIT (0)//	~��ʼ��
#define CEBT_STA_IDEL (1)//	~����״̬
#define CEBT_STA_CONN (2)//	~������
#define CEBT_STA_COOK (3)//	~���ӳɹ�
#define CEBT_STA_CAOU (4)//	~�绰����
#define CEBT_STA_CAIN (5)//	~�绰����
#define CEBT_STA_SPKN (6)//	~ͨ����

//����ECar һ��ͨ����
int WINAPI CreateEcarObject(void** ppObject);

//����ECar һ��ͨ����
int WINAPI DestroyEcarObject(void* pObject);

//ȡ��ECAR��汾��
int WINAPI GetOneKeyDllVer(char* pOutFile, char* pOutProd);

//����DTMF��
int WINAPI GetDtmfCode(BYTE data, BYTE* pOutDtmfCode);

//�趨����·��
void WINAPI SetNaviPath(const char* pNaviPath);

//��TNC�뷢�͸���ͼ
int WINAPI SendTNCToMap(const char* pTncCode);

//Byte���͵ĵ绰����ת����ASC����
BOOL WINAPI ByteToASC(byte CallNo[32]);

//�趨�������������
//0��һ������
//1�����û�����
//2���޸ĺ�̨����
//3������
void WINAPI SetFuncCode(int nCode);

//�趨�����뺯��
void WINAPI SetMachCode(char* pMachCode);

//�Ƿ��Ѽ���,TRUE:���FALSE:δ����
BOOL CheckClientActiveState();

//��ȡ��ǰ�⹤��·��
BOOL GetCurrentDir(wchar* pPathOut);
