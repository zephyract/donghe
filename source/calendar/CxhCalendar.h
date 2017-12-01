#pragma once
#include "wceuilayer.h"
#include "CalendarData.h"
#include "WceUiButton.h"

#define EACH_DAY_COUNT 42
#define SHOW_TXT_COUNT 60


class CWceUiStatic :	public CWceUiButton
{
	WCEUI_DYNCREATE_BEGIN(CWceUiStatic, CWceUiLayer)
public:
	CWceUiStatic(void){};
	virtual ~CWceUiStatic(void){};
};
WCEUI_DYNCREATE_END(CWceUiStatic, CWceUiLayer)


class CxhCalendar :	public CWceUiLayer
{
	WCEUI_DYNCREATE_BEGIN(CxhCalendar, CWceUiLayer)
public:
	CxhCalendar(void);
	virtual ~CxhCalendar(void);
	virtual void OnLMouseDown(PCWceUiStatic pStatic, UINT nFlags, POINT point);
	virtual void SetProperties(PCWceUiXmlElem pCreateElem);
	virtual void OnInitLayer();
	virtual void OnClick(PCWceUiButton pButton);
	void CreateAllDays();
	UINT GetLanguage(void);
	void CreateRDStaic();               //���½ǿؼ�����
	void DrawRDStatic();
	void ClearRDStatic();
    void OnMonthChange( int changeIndex );
    void OnDayClicked(CWceUiStatic* p);
	void OnYearChange( int changeIndex );
	void GetChinaHInfoStr( LPWSTR StrChinaInfo,LPWSTR StrLunarInfo ,LPWSTR StrHolInfo );
    void Beep( BOOL bIsOK );

	//
	//clear a item before draw item
	//
	virtual void OnClearItem(CWceUiStatic* pItem);


	//
	//draw a item
	//nDay:	current day
	//bSelected: cureent item is selected or not
	//
	virtual void OnDrawItem(int nDay, BOOL bSelected, CWceUiStatic* pItem);

private:
	int m_nLanguage;
protected:
	void UpdateEachDay();
	int m_xStart;
	int m_yStart;
	int m_nWidth;
	int m_nHeight;
	int m_nColorIndex;         //������ɫ 0 �ڣ�1.��
    int m_nlunarColorIndex;
    int m_nenColorIndex;

	TCHAR m_stryear[SHOW_TXT_COUNT];
	TCHAR m_strmonth[SHOW_TXT_COUNT];
	TCHAR m_strday[SHOW_TXT_COUNT];
	TCHAR m_strlunarday[SHOW_TXT_COUNT];
    CalendarData m_calendardate;
	CWceUiStatic* m_pEachDayArr[EACH_DAY_COUNT];

	TCHAR				m_strtoday[SHOW_TXT_COUNT];
	TCHAR				m_strchinainfo[SHOW_TXT_COUNT];
	//������Ϣ
	TCHAR				m_strholinfo[SHOW_TXT_COUNT]; 
	//ũ������
	TCHAR				m_strlunainfo[SHOW_TXT_COUNT];

	TCHAR               m_strenyearm[SHOW_TXT_COUNT];
    TCHAR               m_strenday[SHOW_TXT_COUNT];


	//���϶˵�������ʾ
	CWceUiStatic* m_pYear;
	CWceUiStatic* m_pMonth;
	//������ʱ���½ǵ���ʾ
	CWceUiStatic* m_pENYearM;
	CWceUiStatic* m_pENDay;
	//����ʱ���½ǵ���ʾ
	CWceUiStatic* m_pAnimal;//��Ф
	CWceUiStatic* m_plunarYear;//ũ�����
    CWceUiStatic* m_plunarMonthD;//ũ���·�
	CWceUiStatic* m_pHoliday;//�ڼ���
	CWceUiStatic* m_pCurday;

	RECT   RC_ANIMAL;
	RECT   RC_CHINESEYEAR;//
	RECT   RC_CHINESEMD;   //month and day
	RECT   RC_CHINESEFESTIVAL;   //�ڼ���
	RECT   RC_CURDAY;

	RECT   RC_ENYEARMONTH; //year and month
	RECT   RC_ENDAY;

};
WCEUI_DYNCREATE_END(CxhCalendar, CWceUiLayer)