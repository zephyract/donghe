#pragma once
extern const WORD START_YEAR;
extern const WORD END_YEAR;
class CalendarData
{
public:
	CalendarData(void);
	~CalendarData(void);

public:
	WORD    m_iYear, m_iMonth, m_iDay;
	WORD GetYear(){return m_iYear;}
	WORD GetMonth(){return m_iMonth;}
	WORD GetDay(){return m_iDay;}
	void GetDate(WORD &iYear, WORD &iMonth, WORD &iDay);
	BOOL SetDate(WORD iYear, WORD iMonth, WORD iDay);

	//�ж�iYear�ǲ�������
	static BOOL IsLeapYear(WORD iYear)
	{return !(iYear%4)&&(iYear%100) || !(iYear%400);}

	//����iYear,iMonth,iDay��Ӧ�����ڼ� 1��1��1�� --- 65535��12��31��
	static WORD WeekDay(WORD iYear, WORD iMonth, WORD iDay);

	static int ReturnWeekDay( unsigned int iYear, unsigned int iMonth, unsigned int iDay );

	//����iYear��iMonth�µ����� 1��1�� --- 65535��12��
	static WORD MonthDays(WORD iYear, WORD iMonth);

	//��������iLunarYer������iLunarMonth�µ����������iLunarMonthΪ���£�
	//����Ϊ�ڶ���iLunarMonth�µ��������������Ϊ0 
	// 1901��1��---2050��12��
	static LONG LunarMonthDays(WORD iLunarYear, WORD iLunarMonth);

	//��������iLunarYear���������
	// 1901��1��---2050��12��
	static WORD LunarYearDays(WORD iLunarYear);

	//��������iLunarYear��������·ݣ���û�з���0
	// 1901��1��---2050��12��
	static WORD GetLeapMonth(WORD iLunarYear);

	//��iYear���ʽ������ɼ��귨��ʾ���ַ���
	static void FormatLunarYear(WORD  iYear, LPTSTR pBuffer);

	//��iMonth��ʽ���������ַ���
	static void FormatMonth(WORD iMonth, LPTSTR pBuffer, BOOL bLunar = TRUE);

	//��iDay��ʽ���������ַ���
	static void FormatLunarDay(WORD  iDay, LPTSTR pBuffer);

	//���㹫���������ڼ���������  1��1��1�� --- 65535��12��31��
	static LONG CalcDateDiff(WORD iEndYear, WORD iEndMonth, WORD iEndDay,
		WORD iStartYear = START_YEAR, 
		WORD iStartMonth =1, WORD iStartDay =1);

	//���㹫��iYear��iMonth��iDay�ն�Ӧ����������,���ض�Ӧ���������� 0-24
	//1901��1��1��---2050��12��31��
	static WORD GetLunarDate(WORD iYear, WORD iMonth, WORD iDay,
		WORD &iLunarYear, WORD &iLunarMonth, WORD &iLunarDay);

	void InitData();
	//�����1901��1��1�չ�iSpanDays������������
	static void   CalcLunarDate(WORD &iYear, WORD &iMonth ,WORD &iDay, LONG iSpanDays);
	//���㹫��iYear��iMonth��iDay�ն�Ӧ�Ľ��� 0-24��0���ǽ���
	static WORD   GetLunarHolDay(WORD iYear, WORD iMonth, WORD iDay);
};
