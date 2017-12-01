#include "StdAfx.h"
#include "CalculatorDlg.h"
#include "WceUiStyle.h"
#include "WceUiDrawTextCtrl.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"


#define DBL_MAX        1.7976931348623158e+308 /* max value */
#define LDBL_MAX        DBL_MAX         /* max value */


CCalculatorDlg::CCalculatorDlg(void)
{

}

CCalculatorDlg::~CCalculatorDlg(void)
{
}

void CCalculatorDlg::OnInitDialog()
{
	__super::OnInitDialog();

    OnStandard();
	BtnInit();
}

void CCalculatorDlg::BtnInit()
{
   GetChildByName(L"xStep2")->SetText(L"x ^ 2");
   GetChildByName(L"xStepy")->SetText(L"x ^ y");
   GetChildByName(L"Factorial")->SetText(L"n!");
   GetChildByName(L"Exp")->SetText(L"Exp");
   GetChildByName(L"Ln")->SetText(L"ln");
   
   GetChildByName(L"Asin")->SetText(L"Asin");
   GetChildByName(L"Acos")->SetText(L"Acos");
   GetChildByName(L"Atan")->SetText(L"Atan");
   GetChildByName(L"Lg")->SetText(L"Lg");
   GetChildByName(L"Pi")->SetText(L"Pi");

   GetChildByName(L"Sin")->SetText(L"Sin");
   GetChildByName(L"Cos")->SetText(L"Cos");
   GetChildByName(L"Tan")->SetText(L"Tan");
   GetChildByName(L"Sqrt")->SetText(L"Sqrt");
   GetChildByName(L"Reciprocal")->SetText(L"1/X");

   GetChildByName(L"Percent")->SetText(L"%");
   GetChildByName(L"Num7")->SetText(L"7");
   GetChildByName(L"Num4")->SetText(L"4");
   GetChildByName(L"Num1")->SetText(L"1");
   GetChildByName(L"Num0")->SetText(L"0");

   GetChildByName(L"Clean")->SetText(L"CE");
   GetChildByName(L"Num8")->SetText(L"8");
   GetChildByName(L"Num5")->SetText(L"5");
   GetChildByName(L"Num2")->SetText(L"2");
   GetChildByName(L"Neg")->SetText(L"+/-");

   GetChildByName(L"Clear")->SetText(L"C");
   GetChildByName(L"Num9")->SetText(L"9");
   GetChildByName(L"Num6")->SetText(L"6");
   GetChildByName(L"Num3")->SetText(L"3");
   GetChildByName(L"Point")->SetText(L".");

   //only for cui3
   GetChildByName(L"Backspace")->SetText(L"Back\nspace");

   GetChildByName(L"Div")->SetText(L"/");
   GetChildByName(L"Mul")->SetText(L"*");
   GetChildByName(L"Sub")->SetText(L"-");
   GetChildByName(L"Add")->SetText(L"+");

}


void CCalculatorDlg::OnBnClick(PCWceUiButton pButton)
{
    DWORD IsOK = 1;
	if (pButton->IsEqualName(L"xStep2"))
	{
		OnXstep2();
	}
	else if (pButton->IsEqualName(L"xStepy"))
	{
		OnXstepy();
	}
	else if (pButton->IsEqualName(L"Factorial"))
	{
		OnFactorial();
	}
	else if (pButton->IsEqualName(L"Exp"))
	{
		OnExp();
	}
	else if (pButton->IsEqualName(L"Ln"))
	{
		OnLn();
	}
	else if (pButton->IsEqualName(L"Asin"))
	{
		OnArcsin();
	}
	else if (pButton->IsEqualName(L"Acos"))
	{
		OnArccos();
	}
	else if (pButton->IsEqualName(L"Atan"))
	{
		OnArctan();
	}
	else if (pButton->IsEqualName(L"Lg"))
	{
		OnLg();
	}
	else if (pButton->IsEqualName(L"Pi"))
	{
		OnPi();
	}
	else if (pButton->IsEqualName(L"Sin"))
	{
		OnSin();
	}
	else if (pButton->IsEqualName(L"Cos"))
	{
		OnCos();
	}
	else if (pButton->IsEqualName(L"Tan"))
	{
		OnTan();
	}
	else if (pButton->IsEqualName(L"Sqrt"))
	{
		OnSqrt();
	}
	else if (pButton->IsEqualName(L"Reciprocal"))
	{
		OnReciprocal();
	}
	else if (pButton->IsEqualName(L"Percent"))
	{
		OnPercent();
	}
	else if (pButton->IsEqualName(L"Num7"))
	{
		On7();
	}
	else if (pButton->IsEqualName(L"Num4"))
	{
		On4();
	}
	else if (pButton->IsEqualName(L"Num1"))
	{
		On1();
	}
	else if (pButton->IsEqualName(L"Num0"))
	{
		On0();
	}
	else if (pButton->IsEqualName(L"Clean"))
	{
		OnClean();
	}
	else if (pButton->IsEqualName(L"Num8"))
	{
		On8();
	}
	else if (pButton->IsEqualName(L"Num5"))
	{
		On5();
	}
	else if (pButton->IsEqualName(L"Num2"))
	{
		On2();
	}
	else if (pButton->IsEqualName(L"Neg"))
	{
		OnNeg();
	}
	else if (pButton->IsEqualName(L"Clear"))
	{
		OnClear();
	}
	else if (pButton->IsEqualName(L"Num9"))
	{
		On9();

	}
	else if (pButton->IsEqualName(L"Num6"))
	{
		On6();
	}
	else if(pButton->IsEqualName(L"Num3"))
	{
		On3();
	}
	else if (pButton->IsEqualName(L"Point"))
	{
		OnPoint();
	}
	else if (pButton->IsEqualName(L"Backspace"))
	{
		OnBackspace();
	}
	else if(pButton->IsEqualName(L"Div"))
	{
		OnDiv();
	}
	else if (pButton->IsEqualName(L"Mul"))
	{
		OnMul();
	}
	else if (pButton->IsEqualName(L"Sub"))
	{
		OnSub();
	}
	else if (pButton->IsEqualName(L"Add"))
	{
		OnAdd();
	}
	else if (pButton->IsEqualName(L"Equal"))
	{
		OnEqual();
	}
	else
	{
		__super::OnBnClick(pButton);
	}
}

long CCalculatorDlg::LengthOfString(const TCHAR *str)
{
	long i=0;
	while(*str++) i++;
	return i;
}

long CCalculatorDlg::GetCharPosFromLeft(const TCHAR *str1,TCHAR chr)
{
	long pos=0;
	while(*str1)
	{
		pos++;
		if(*str1++==chr) return pos;
	}
	return pos;
}

long CCalculatorDlg::GetUnCharPosFromRight(const TCHAR *str1,TCHAR chr)
{
	long strlen=LengthOfString(str1),pos=strlen,i;
	str1+=strlen-1;
	for(i=0;i<strlen;i++)
	{
		if(*str1--!=chr) return pos;
		pos--;
	}
	return pos;
}

void CCalculatorDlg::DoubleToString(double sum,TCHAR str[])
{
	int sumlen=1;
	double bk_sum=sum;
	BOOL bNeg = FALSE;
	if(!sum)
	{
		str[0]=' ';
		str[1]='0';
		str[2]='.';
		str[3]=0;
		return;
	}
	if(sum<0) 
	{
		sum=-sum;
		bNeg = TRUE;
	}

	if ( sum> LDBL_MAX )
	{
		sum = 0;
	}

	while(sum>=10)
	{
		sum/=10;
		sumlen++;
	}
	sum=bk_sum;
	if(sumlen>MAX_NUM_COUNT)
	{
		swprintf(str,_T("%E"),sum);
		return;
	}
	if(bNeg)
	{
		swprintf(str,_T("%f"),sum);
	}
	else
	{
		str[0]=' ';
		swprintf(str+1,_T("%f"),sum);
	}
	str[GetUnCharPosFromRight(str,'0')]=0;
}

void CCalculatorDlg::Delay(unsigned long ti)
{
	unsigned long oldti=GetTickCount();
	while(GetTickCount()<oldti+ti);
}

void CCalculatorDlg::Num10ToNum2(long num10,TCHAR *num2)
{
	TCHAR i=1,j;
	long _num10;
	if(!num10)
	{
		*num2++=' ';
		*num2++='0';
		*num2=0;
		return;
	}
	if(num10<0)
	{
		num10=-num10;
		*num2++='-';
	}
	else
		*num2++=' ';
	_num10=num10;
	while(num10/=2)
		i++;
	*(num2+i)=0;
	num10=_num10;
	for(j=i-1;j>=0;j--)
	{
		*(num2+j)=num10%2+48;
		num10/=2;
	}
	*(num2+MAX_NUM_COUNT)='.';
	*(num2+MAX_NUM_COUNT+1)=0;
}

void CCalculatorDlg::Num10ToNum8(long num10,TCHAR *num8)
{
	TCHAR i=1,j;
	long _num10;
	if(!num10)
	{
		*num8++=' ';
		*num8++='0';
		*num8=0;
		return;
	}
	if(num10<0)
	{
		num10=-num10;
		*num8++='-';
	}
	else
		*num8++=' ';
	_num10=num10;
	while(num10/=8)
		i++;
	num10=_num10;
	*(num8+i)=0;
	for(j=i-1;j>=0;j--)
	{
		*(num8+j)=num10%8+48;
		num10/=8;
	}
	*(num8+MAX_NUM_COUNT)='.';
	*(num8+MAX_NUM_COUNT+1)=0;
}

void CCalculatorDlg::Num10ToNum16(long num10,TCHAR *num16)
{
	TCHAR i=1,j;
	long _num10;
	if(!num10)
	{
		*num16++=' ';
		*num16++='0';
		*num16=0;
		return;
	}
	if(num10<0)
	{
		num10=-num10;
		*num16++='-';
	}
	else
		*num16++=' ';
	_num10=num10;
	while(num10/=16)
		i++;
	*(num16+i)=0;
	num10=_num10;
	for(j=i-1;j>=0;j--)
	{
		*(num16+j)=num10%16>9?num10%16-10+'A':num10%16+48;
		num10/=16;
	}
	*(num16+MAX_NUM_COUNT)='.';
	*(num16+MAX_NUM_COUNT+1)=0;
}

void CCalculatorDlg::On0() 
{
	// TODO: Add your control notification handler code here
	wcscpy(ScrText,GetChildByName(L"show_result")->GetText());
	ScrLen  = LengthOfString(ScrText);
	//ScrLen=m_stScreen.GetWindowTextLength();
	//m_stScreen.GetWindowText(ScrText,ScrLen+1);

	if ( ScrText[ScrLen-1] =='%' )
	{
		DoubleToString(ScrNum,ScrText);
		ScrLen = LengthOfString(ScrText);
	}

	if(Flag)
	{
		Flag=0;
		ScrNum=0;
		ScrText[0]=' ';
		ScrText[1]='0';
		ScrText[2]='.';
		ScrText[3]=0;
	}
	else if(ScrLen<MAX_NUM_COUNT)
	{
		if(Fpoint)
		{
			ScrNum+=0*pow((double)10,(int)(GetCharPosFromLeft(ScrText,'.')-ScrLen-1));
			ScrText[ScrLen]='0';
			ScrText[ScrLen+1]=0;
		}
		else 
		{
			if(ScrNum)
			{
				ScrText[ScrLen-1]='0';
				ScrText[ScrLen]='.';
				ScrText[ScrLen+1]=0;
			}
			else
			{
				ScrText[1]='0';
			}
			ScrNum=ScrNum*10+0;
		}
	}
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::On1() 
{
	// TODO: Add your control notification handler code here
	wcscpy(ScrText,GetChildByName(L"show_result")->GetText());
	ScrLen  = LengthOfString(ScrText);
	//ScrLen=m_stScreen.GetWindowTextLength();
	//m_stScreen.GetWindowText(ScrText,ScrLen+1);

	if ( ScrText[ScrLen-1] =='%' )
	{
		DoubleToString(ScrNum,ScrText);
		ScrLen = LengthOfString(ScrText);
	}

	if(Flag)
	{
		Flag=0;
		ScrNum=1;
		ScrText[0]=' ';
		ScrText[1]='1';
		ScrText[2]='.';
		ScrText[3]=0;
	}
	else if(ScrLen<MAX_NUM_COUNT)
	{
		if(Fpoint)
		{
			ScrNum+=1*pow((double)10,(int)(GetCharPosFromLeft(ScrText,'.')-ScrLen-1));
			ScrText[ScrLen]='1';
			ScrText[ScrLen+1]=0;
		}
		else 
		{
			if(ScrNum)
			{
				ScrText[ScrLen-1]='1';
				ScrText[ScrLen]='.';
				ScrText[ScrLen+1]=0;
			}
			else
			{
				ScrText[1]='1';
			}
			switch(FNum)
			{
			case FNUM16:
				ScrNum=ScrNum*16+1;
				break;
			case FNUM10:
				ScrNum=ScrNum*10+1;
				break;
			case FNUM8:
				ScrNum=ScrNum*8+1;
				break;
			case FNUM2:
				ScrNum=ScrNum*2+1;
			}
		}
	}
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::On2() 
{
	// TODO: Add your control notification handler code here
	wcscpy(ScrText,GetChildByName(L"show_result")->GetText());
	ScrLen  = LengthOfString(ScrText);
	//ScrLen=m_stScreen.GetWindowTextLength();
	//m_stScreen.GetWindowText(ScrText,ScrLen+1);

	if ( ScrText[ScrLen-1] =='%' )
	{
		DoubleToString(ScrNum,ScrText);
		ScrLen = LengthOfString(ScrText);
	}

	if(Flag)
	{
		Flag=0;
		ScrNum=2;
		ScrText[0]=' ';
		ScrText[1]='2';
		ScrText[2]='.';
		ScrText[3]=0;
	}
	else if(ScrLen<MAX_NUM_COUNT)
	{
		if(Fpoint)
		{
			ScrNum+=2*pow((double)10,(int)(GetCharPosFromLeft(ScrText,'.')-ScrLen-1));
			ScrText[ScrLen]='2';
			ScrText[ScrLen+1]=0;
		}
		else
		{
			if(ScrNum)
			{
				ScrText[ScrLen-1]='2';
				ScrText[ScrLen]='.';
				ScrText[ScrLen+1]=0;
			}
			else
			{
				ScrText[1]='2';
			}
			switch(FNum)
			{
			case FNUM16:
				ScrNum=ScrNum*16+2;
				break;
			case FNUM10:
				ScrNum=ScrNum*10+2;
				break;
			case FNUM8:
				ScrNum=ScrNum*8+2;
			}
		}
	}
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::On3() 
{
	// TODO: Add your control notification handler code here
	//CString strTemp; m_stScreen.GetWindowText(strTemp); //test
	wcscpy(ScrText,GetChildByName(L"show_result")->GetText());
	ScrLen  = LengthOfString(ScrText);
	//ScrLen=m_stScreen.GetWindowTextLength();
	//m_stScreen.GetWindowText(ScrText,ScrLen+1);

	if ( ScrText[ScrLen-1] =='%' )
	{
		DoubleToString(ScrNum,ScrText);
		ScrLen = LengthOfString(ScrText);
	}

	if(Flag)
	{
		Flag=0;
		ScrNum=3;
		ScrText[0]=' ';
		ScrText[1]='3';
		ScrText[2]='.';
		ScrText[3]=0;
	}
	else if(ScrLen<MAX_NUM_COUNT)
	{
		if(Fpoint)
		{
			ScrNum+=3*pow((double)10,(int)(GetCharPosFromLeft(ScrText,'.')-ScrLen-1));
			ScrText[ScrLen]='3';
			ScrText[ScrLen+1]=0;
		}
		else
		{
			if(ScrNum)
			{
				ScrText[ScrLen-1]='3';
				ScrText[ScrLen]='.';
				ScrText[ScrLen+1]=0;
			}
			else
			{
				ScrText[1]='3';
			}
			switch(FNum)
			{
			case FNUM16:
				ScrNum=ScrNum*16+3;
				break;
			case FNUM10:
				ScrNum=ScrNum*10+3;
				break;
			case FNUM8:
				ScrNum=ScrNum*8+3;
			}
		}
	}
	GetChildByName(L"show_result")->SetText(ScrText);
	//m_stScreen.GetWindowText(strTemp); //test
}

void CCalculatorDlg::On4() 
{
	// TODO: Add your control notification handler code here
	wcscpy(ScrText,GetChildByName(L"show_result")->GetText());
	ScrLen  = LengthOfString(ScrText);
//	ScrLen=m_stScreen.GetWindowTextLength();
//	m_stScreen.GetWindowText(ScrText,ScrLen+1);

	if ( ScrText[ScrLen-1] =='%' )
	{
		DoubleToString(ScrNum,ScrText);
		ScrLen = LengthOfString(ScrText);
	}

	if(Flag)
	{
		Flag=0;
		ScrNum=4;
		ScrText[0]=' ';
		ScrText[1]='4';
		ScrText[2]='.';
		ScrText[3]=0;
	}
	else if(ScrLen<MAX_NUM_COUNT)
	{
		if(Fpoint)
		{
			ScrNum+=4*pow((double)10,(int)(GetCharPosFromLeft(ScrText,'.')-ScrLen-1));
			ScrText[ScrLen]='4';
			ScrText[ScrLen+1]=0;
		}
		else
		{
			if(ScrNum)
			{
				ScrText[ScrLen-1]='4';
				ScrText[ScrLen]='.';
				ScrText[ScrLen+1]=0;
			}
			else
			{
				ScrText[1]='4';
			}
			switch(FNum)
			{
			case FNUM16:
				ScrNum=ScrNum*16+4;
				break;
			case FNUM10:
				ScrNum=ScrNum*10+4;
				break;
			case FNUM8:
				ScrNum=ScrNum*8+4;
			}
		}
	}
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::On5() 
{
	// TODO: Add your control notification handler code here
	wcscpy(ScrText,GetChildByName(L"show_result")->GetText());
	ScrLen  = LengthOfString(ScrText);
	//ScrLen=m_stScreen.GetWindowTextLength();
	//m_stScreen.GetWindowText(ScrText,ScrLen+1);

	if ( ScrText[ScrLen-1] =='%' )
	{
		DoubleToString(ScrNum,ScrText);
		ScrLen = LengthOfString(ScrText);
	}

	if(Flag)
	{
		Flag=0;
		ScrNum=5;
		ScrText[0]=' ';
		ScrText[1]='5';
		ScrText[2]='.';
		ScrText[3]=0;
	}
	else if(ScrLen<MAX_NUM_COUNT)
	{
		if(Fpoint)
		{
			ScrNum+=5*pow((double)10,(int)(GetCharPosFromLeft(ScrText,'.')-ScrLen-1));
			ScrText[ScrLen]='5';
			ScrText[ScrLen+1]=0;
		}
		else
		{
			if(ScrNum)
			{
				ScrText[ScrLen-1]='5';
				ScrText[ScrLen]='.';
				ScrText[ScrLen+1]=0;
			}
			else
			{
				ScrText[1]='5';
			}
			switch(FNum)
			{
			case FNUM16:
				ScrNum=ScrNum*16+5;
				break;
			case FNUM10:
				ScrNum=ScrNum*10+5;
				break;
			case FNUM8:
				ScrNum=ScrNum*8+5;
			}
		}
	}
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::On6() 
{
	// TODO: Add your control notification handler code here
	wcscpy(ScrText,GetChildByName(L"show_result")->GetText());
	ScrLen  = LengthOfString(ScrText);
	//ScrLen=m_stScreen.GetWindowTextLength();
	//m_stScreen.GetWindowText(ScrText,ScrLen+1);

	if ( ScrText[ScrLen-1] =='%' )
	{
		DoubleToString(ScrNum,ScrText);
		ScrLen = LengthOfString(ScrText);
	}

	if(Flag)
	{
		Flag=0;
		ScrNum=6;
		ScrText[0]=' ';
		ScrText[1]='6';
		ScrText[2]='.';
		ScrText[3]=0;
	}
	else if(ScrLen<MAX_NUM_COUNT)
	{
		if(Fpoint)
		{
			ScrNum+=6*pow((double)10,(int)(GetCharPosFromLeft(ScrText,'.')-ScrLen-1));
			ScrText[ScrLen]='6';
			ScrText[ScrLen+1]=0;
		}
		else
		{
			if(ScrNum)
			{
				ScrText[ScrLen-1]='6';
				ScrText[ScrLen]='.';
				ScrText[ScrLen+1]=0;
			}
			else
			{
				ScrText[1]='6';
			}
			switch(FNum)
			{
			case FNUM16:
				ScrNum=ScrNum*16+6;
				break;
			case FNUM10:
				ScrNum=ScrNum*10+6;
				break;
			case FNUM8:
				ScrNum=ScrNum*8+6;
			}
		}
	}
	GetChildByName(L"show_result")->SetText(ScrText);

//	CString strTemp; m_stScreen.GetWindowText(strTemp); //test
}

void CCalculatorDlg::On7() 
{
	// TODO: Add your control notification handler code here
	wcscpy(ScrText,GetChildByName(L"show_result")->GetText());
	ScrLen  = LengthOfString(ScrText);
	//ScrLen=m_stScreen.GetWindowTextLength();
	//m_stScreen.GetWindowText(ScrText,ScrLen+1);
	if ( ScrText[ScrLen-1] =='%' )
	{
		DoubleToString(ScrNum,ScrText);
		ScrLen = LengthOfString(ScrText);
	}

	if(Flag)
	{
		Flag=0;
		ScrNum=7;
		ScrText[0]=' ';
		ScrText[1]='7';
		ScrText[2]='.';
		ScrText[3]=0;
	}
	else if(ScrLen<MAX_NUM_COUNT)
	{
		if(Fpoint)
		{
			ScrNum+=7*pow((double)10,(int)(GetCharPosFromLeft(ScrText,'.')-ScrLen-1));
			ScrText[ScrLen]='7';
			ScrText[ScrLen+1]=0;
		}
		else
		{
			if(ScrNum)
			{
				ScrText[ScrLen-1]='7';
				ScrText[ScrLen]='.';
				ScrText[ScrLen+1]=0;
			}
			else
			{
				ScrText[1]='7';
			}
			switch(FNum)
			{
			case FNUM16:
				ScrNum=ScrNum*16+7;
				break;
			case FNUM10:
				ScrNum=ScrNum*10+7;
				break;
			case FNUM8:
				ScrNum=ScrNum*8+7;
			}
		}
	}
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::On8() 
{
	// TODO: Add your control notification handler code here
	wcscpy(ScrText,GetChildByName(L"show_result")->GetText());
	ScrLen  = LengthOfString(ScrText);
	//ScrLen=m_stScreen.GetWindowTextLength();
	//m_stScreen.GetWindowText(ScrText,ScrLen+1);
	if ( ScrText[ScrLen-1] =='%' )
	{
		DoubleToString(ScrNum,ScrText);
		ScrLen = LengthOfString(ScrText);
	}

	if(Flag)
	{
		Flag=0;
		ScrNum=8;
		ScrText[0]=' ';
		ScrText[1]='8';
		ScrText[2]='.';
		ScrText[3]=0;
	}
	else if(ScrLen<MAX_NUM_COUNT)
	{
		if(Fpoint)
		{
			ScrNum+=8*pow((double)10,(int)(GetCharPosFromLeft(ScrText,'.')-ScrLen-1));
			ScrText[ScrLen]='8';
			ScrText[ScrLen+1]=0;
		}
		else
		{
			if(ScrNum)
			{
				ScrText[ScrLen-1]='8';
				ScrText[ScrLen]='.';
				ScrText[ScrLen+1]=0;
			}
			else
			{
				ScrText[1]='8';
			}
			switch(FNum)
			{
			case FNUM16:
				ScrNum=ScrNum*16+8;
				break;
			case FNUM10:
				ScrNum=ScrNum*10+8;
			}
		}
	}
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::On9() 
{
	// TODO: Add your control notification handler code here
	wcscpy(ScrText,GetChildByName(L"show_result")->GetText());
	ScrLen  = LengthOfString(ScrText);
	//ScrLen=m_stScreen.GetWindowTextLength();
	//m_stScreen.GetWindowText(ScrText,ScrLen+1);
	if ( ScrText[ScrLen-1] =='%' )
	{
		DoubleToString(ScrNum,ScrText);
		ScrLen = LengthOfString(ScrText);
	}

	if(Flag)
	{
		Flag=0;
		ScrNum=9;
		ScrText[0]=' ';
		ScrText[1]='9';
		ScrText[2]='.';
		ScrText[3]=0;
	}
	else if(ScrLen<MAX_NUM_COUNT)
	{
		if(Fpoint)
		{
			ScrNum+=9*pow((double)10,(int)(GetCharPosFromLeft(ScrText,'.')-ScrLen-1));
			ScrText[ScrLen]='9';
			ScrText[ScrLen+1]=0;
		}
		else
		{
			if(ScrNum)
			{
				ScrText[ScrLen-1]='9';
				ScrText[ScrLen]='.';
				ScrText[ScrLen+1]=0;
			}
			else
			{
				ScrText[1]='9';
			}
			switch(FNum)
			{
			case FNUM16:
				ScrNum=ScrNum*16+9;
				break;
			case FNUM10:
				ScrNum=ScrNum*10+9;
			}
		}
	}
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	//CString strTemp; m_stScreen.GetWindowText(strTemp); //test

	m_bError = FALSE;
	OnEqual();
	if ( m_bError )
		return;

	Sign=FADD;
	Scr_Num=ScrNum;
	//m_stScreen.GetWindowText(strTemp); //test
}

void CCalculatorDlg::OnBackspace() 
{
	// TODO: Add your control notification handler code here
	//CString strTemp; m_stScreen.GetWindowText(strTemp); //test
	if(ScrNum)
	{
		wcscpy(ScrText,GetChildByName(L"show_result")->GetText());
		ScrLen  = LengthOfString(ScrText);
		//ScrLen=m_stScreen.GetWindowTextLength();
		//m_stScreen.GetWindowText(ScrText,ScrLen+1);
		if(ScrText[ScrLen-1]=='.')
		{
			Fpoint=0;
			if(ScrLen==3)
			{
				ScrNum=0;
				ScrText[1]='0';
				ScrText[2]='.';
				ScrText[3]=0;
			}
			else
			{
				ScrText[ScrLen-2]='.';
				ScrText[ScrLen-1]=0;
			}
		}
		else
		{
			ScrText[ScrLen-1]=0;
		}

		TCHAR p;
		switch(FNum)
		{
		case FNUM16:
			p=16;
			break;
		case FNUM10:
			p=10;
			break;
		case FNUM8:
			p=8;
			break;
		case FNUM2:
			p=2;
		}
		if(Fpoint)
		{
			int offset=GetCharPosFromLeft(ScrText,'.');
			ScrNum*=pow((double)p,(int)(ScrLen-offset-1));
			if((long) (ScrNum))
			{
				ScrNum=(long) ScrNum;
				ScrNum/=pow((double)p,(int)(ScrLen-offset-1));
			}
		}
		else
		{
			if((long) (ScrNum/p))
			{
				ScrNum=(long) (ScrNum/p);
			}
			else
			{
				ScrNum/=p;
			}
		}
		GetChildByName(L"show_result")->SetText(ScrText);
	}
}

void CCalculatorDlg::OnClean() 
{
	// TODO: Add your control notification handler code here
	ScrNum=0;
	Fpoint=0;
	GetChildByName(L"show_result")->SetText(_T(" 0."));
}

void CCalculatorDlg::OnClear() 
{
	// TODO: Add your control notification handler code here
	GetChildByName(L"show_result")->SetText(_T(" 0."));
	ScrNum=0;
	Scr_Num=0;
	Fpoint=0;
	Sign=0;
}

void CCalculatorDlg::OnDiv() 
{
	// TODO: Add your control notification handler code here
	m_bError = FALSE;
	OnEqual();
	if ( m_bError )
		return;
	Sign=FDIV;
	Scr_Num=ScrNum;
}

void CCalculatorDlg::OnEqual()
{
	// TODO: Add your control notification handler code here
//	CString strTemp; m_stScreen.GetWindowText(strTemp); //test
	double i,n,sum=1;
	Fpoint=0;
	Flag=1;
	if(Sign)
	{
		switch(Sign)
		{
		case FADD:
			ScrNum=Scr_Num+ScrNum;
			break;
		case FSUB:
			ScrNum=Scr_Num-ScrNum;
			break;
		case FMUL:
			if ( Scr_Num==0|| ScrNum==0 )
			{
				ScrNum=Scr_Num*ScrNum;
			}
			else
			{
				ScrNum=Scr_Num*ScrNum;
				if ( 2*ScrNum == ScrNum )
				{
					Sign=0;
					ScrNum = 0;
					//GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
					OnClear();
					GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
					//GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString(LG_DATAFLOOD) );
					m_bError = TRUE;
					return;				
				}
			}
			break;
		case FDIV:
			if(!ScrNum)
			{
				Sign=0;
				OnClear();
                GetChildByName(L"show_result")->SetTextResID(L"LG_ZEROERROR");
				//GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString( LG_ZEROERROR) );
				m_bError = TRUE;
				return;
			}
			ScrNum=Scr_Num/ScrNum;
			break;
		case FPOW:
			if ( ScrNum==0 )
			{
				Sign=0;
				//GetChildByName(L"show_result")->SetTextResID(L"LG_ZEROERROR");
				OnClear();
				GetChildByName(L"show_result")->SetTextResID(L"LG_ZEROERROR");
				//GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString( LG_ZEROERROR) );
				m_bError = TRUE;
				return;
			}
			n=333/ScrNum;

			for(i=0;i<n;i++)
			{
				sum*=10;
				if ( sum>1e+308 )
				{
					Sign=0;
					//GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
					OnClear();
					GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
					//GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString(LG_DATAFLOOD) );
					m_bError = TRUE;		
					return;
				}
			}
			if(Scr_Num>sum)
			{
				Sign=0;
				//GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
				OnClear();
				GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
			//	GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString(LG_DATAFLOOD) );
				m_bError = TRUE;
				return;
			}
			ScrNum=pow(Scr_Num,ScrNum);
			break;
		case FAND:
			ScrNum=(long) Scr_Num & (long) ScrNum;
			break;
		case FOR:
			ScrNum=(long) Scr_Num | (long) ScrNum;
			break;
		case FXOR:
			ScrNum=(long) Scr_Num ^ (long) ScrNum;
			break;
		}
		Sign=0;
		if(FNum!=FNUM10)
		{
			switch(FNum)
			{
			case FNUM16:
				Num10ToNum16((long) ScrNum,ScrText);
				break;
			case FNUM8:
				Num10ToNum8((long) ScrNum,ScrText);
				break;
			case FNUM2:
				Num10ToNum2((long) ScrNum,ScrText);
			}
			ScrLen=(TCHAR) LengthOfString(ScrText);
			ScrText[ScrLen]='.';
			ScrText[ScrLen+1]=0;
		}
		else
			DoubleToString(ScrNum,ScrText);
		GetChildByName(L"show_result")->SetText(ScrText);
	}
	//m_stScreen.GetWindowText(strTemp); //test
}

void CCalculatorDlg::OnMadd()
{
#if 0
	// TODO: Add your control notification handler code here
	if(ScrNum)
	{
		MNum+=ScrNum;
		m_stShow.SetWindowText(_T("M+"));
	}
#endif
}

void CCalculatorDlg::OnMclear()
{
#if 0	
	// TODO: Add your control notification handler code here
	MNum=0;
	m_stShow.SetWindowText(_T("MC"));
#endif
}

void CCalculatorDlg::OnMread() 
{
#if 0
	// TODO: Add your control notification handler code here
	if(MNum)
	{
		ScrNum=MNum;
		DoubleToString(ScrNum,ScrText);
		GetChildByName(L"show_result")->SetText(ScrText);
		m_stShow.SetWindowText(_T("MR"));
	}
#endif
}

void CCalculatorDlg::OnMsave() 
{
#if 0
	// TODO: Add your control notification handler code here
	MNum=ScrNum;
	m_stShow.SetWindowText(_T("MS"));
#endif
}

void CCalculatorDlg::OnMul() 
{
	// TODO: Add your control notification handler code here
	m_bError = FALSE;
	OnEqual();
	if ( m_bError )
		return;
	Sign=FMUL;
	Scr_Num=ScrNum;
}

void CCalculatorDlg::OnNeg() 
{
	// TODO: Add your control notification handler code here
	if(Flag)
	{
		ScrNum=0;
		GetChildByName(L"show_result")->SetText(_T(" 0."));
	}
	else
	{
		ScrNum=-ScrNum;
		wcscpy(ScrText,GetChildByName(L"show_result")->GetText());
		ScrLen  = LengthOfString(ScrText);
		//ScrLen=m_stScreen.GetWindowTextLength();
		//m_stScreen.GetWindowText(ScrText,ScrLen+1);
		if(ScrNum<0)
		{
			ScrText[0]='-';
		}
		else
		{
			ScrText[0]=' ';
		}
		GetChildByName(L"show_result")->SetText(ScrText);
	}
}

void CCalculatorDlg::OnPercent()
{
	// TODO: Add your control notification handler code here

	if(Sign)
	{
		m_bError = FALSE;
		OnEqual();
		if ( m_bError )
			return;
	}
	DoubleToString(ScrNum*100,ScrText);
	long len=LengthOfString(ScrText);
	ScrText[len]='%';
	ScrText[len+1]=0;
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnPoint() 
{
	// TODO: Add your control notification handler code here
	Fpoint=1;
	if(Flag)
	{
		ScrNum=0;
		GetChildByName(L"show_result")->SetText(_T(" 0."));
	}
}

void CCalculatorDlg::OnReciprocal() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
	if(!ScrNum)
	{
		//GetChildByName(L"show_result")->SetTextResID(L"LG_ZEROERROR");
		OnClear();
		GetChildByName(L"show_result")->SetTextResID(L"LG_ZEROERROR");
		//GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString( LG_ZEROERROR) );
		return;
	}
	if(FNum!=FNUM10)
	{
		ScrText[0]=' ';
		ScrText[1]=ScrNum==1?'1':'0';
		ScrText[2]='.';
		ScrText[3]=0;
	}
	else
	{
		ScrNum=1/ScrNum;
		DoubleToString(ScrNum,ScrText);
	}
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnSqrt() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
	if(ScrNum<0)
	{
		ScrNum=0;
		//GetChildByName(L"show_result")->SetTextResID(_T("LG_NEGATIVEERROR"));
		OnClear();
	
		GetChildByName(L"show_result")->SetTextResID(_T("LG_NEGATIVEERROR"));
		//GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString( LG_NEGATIVEERROR ) );
		return;
	}
	ScrNum=sqrt(ScrNum);
	DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnSub() 
{
	// TODO: Add your control notification handler code here
	m_bError = FALSE;
	OnEqual();
	if ( m_bError )
		return;
	Sign=FSUB;
	Scr_Num=ScrNum;
}

void CCalculatorDlg::OnStandard() 
{
	GetChildByName(L"show_result")->SetText(_T(" 0."));
	ScrNum=0;
	MNum=0;
	Scr_Num=0;
	Sign=0;
	Flag=0;
	Fpoint=0;;
	FNum=FNUM10;
}


void CCalculatorDlg::OnPi()
{
	ScrNum=3.141592653589793238462643383;
	GetChildByName(L"show_result")->SetText(_T("3.141592653589793238462643383"));
}

void CCalculatorDlg::OnAnd() 
{
	// TODO: Add your control notification handler code here
	m_bError = FALSE;
	OnEqual();
	if ( m_bError )
		return;
	Sign=FAND;
	Scr_Num=ScrNum;
}

void CCalculatorDlg::OnNot() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
	ScrNum=~(long) ScrNum;
	if(FNum!=FNUM10)
	{
		switch(FNum)
		{
		case FNUM16:
			Num10ToNum16((long) ScrNum,ScrText);
			break;
		case FNUM8:
			Num10ToNum8((long) ScrNum,ScrText);
			break;
		case FNUM2:
			Num10ToNum2((long) ScrNum,ScrText);
		}
		ScrLen=(TCHAR) LengthOfString(ScrText);
		ScrText[ScrLen]='.';
		ScrText[ScrLen+1]=0;
	}
	else
		DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnOr()
{
	// TODO: Add your control notification handler code here
	m_bError = FALSE;
	OnEqual();
	if ( m_bError )
		return;
	Sign=FOR;
	Scr_Num=ScrNum;
}

void CCalculatorDlg::OnXor()
{
	// TODO: Add your control notification handler code here
	m_bError = FALSE;
	OnEqual();
	if ( m_bError )
		return;
	Sign=FXOR;
	Scr_Num=ScrNum;
}

void CCalculatorDlg::OnArccos() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
#ifdef USE_ANGLE_TRIGONOMETRY
	ScrNum = ScrNum*M_PI/180;
#endif

	ScrNum=acos(ScrNum);
	DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnArcsin() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
#ifdef USE_ANGLE_TRIGONOMETRY
	ScrNum = ScrNum*M_PI/180;
#endif

	ScrNum=asin(ScrNum);
	DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnArctan() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
#ifdef USE_ANGLE_TRIGONOMETRY
	ScrNum = ScrNum*M_PI/180;
#endif

	ScrNum=atan(ScrNum);
	DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnCos() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
#ifdef USE_ANGLE_TRIGONOMETRY
	ScrNum = ScrNum*M_PI/180;
#endif

	ScrNum=cos(ScrNum);
	DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnExp() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
	if(ScrNum>=700)
	{
		//GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
		OnClear();
		GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
		//GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString(LG_DATAFLOOD) );
		return;
	}
	ScrNum=exp(ScrNum);
	DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnFactorial() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
	if(ScrNum>170)
	{
		//GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
		OnClear();
		GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
		//GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString(LG_DATAFLOOD) );
		return;
	}
	double Sum=1;
	for(DWORD i=2;i<=ScrNum;i++)
		Sum*=i;
	ScrNum=Sum;
	if(FNum!=FNUM10)
	{
		switch(FNum)
		{
		case FNUM16:
			Num10ToNum16((long) ScrNum,ScrText);
			break;
		case FNUM8:
			Num10ToNum8((long) ScrNum,ScrText);
			break;
		case FNUM2:
			Num10ToNum2((long) ScrNum,ScrText);
		}
		ScrLen=(TCHAR) LengthOfString(ScrText);
		ScrText[ScrLen]='.';
		ScrText[ScrLen+1]=0;
	}
	else
		DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnLg() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
	if(ScrNum<=0)
	{
		
		OnClear();
		
        GetChildByName(L"show_result")->SetTextResID(L"LG_FUNCERROR");
		//GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString( LG_FUNCERROR ) );
		return;
	}
	ScrNum=log10(ScrNum);
	DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnLn() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
	if(ScrNum<=0)
	{
		//GetChildByName(L"show_result")->SetTextResID(L"LG_FUNCERROR");;
		OnClear();
		GetChildByName(L"show_result")->SetTextResID(L"LG_FUNCERROR");;
		//GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString( LG_FUNCERROR ) );
		return;
	}
	ScrNum=log(ScrNum);
	DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnSin() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
#ifdef USE_ANGLE_TRIGONOMETRY
	ScrNum = ScrNum*M_PI/180;
#endif
	ScrNum=sin(ScrNum);
	DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnTan() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
#ifdef USE_ANGLE_TRIGONOMETRY
	ScrNum = ScrNum*M_PI/180;
#endif

	ScrNum=tan(ScrNum);
#define   EPSINON   1e-6    
	if(ScrNum>=-EPSINON && ScrNum<=EPSINON)   
	{   
		ScrNum = 0;		      
	}

	DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnXstep2() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
	if(ScrNum>1.0e+190)
	{
		//GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
		OnClear();
		GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
	//	GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString(LG_DATAFLOOD) );
		return;
	}
	ScrNum*=ScrNum;
	if(FNum!=FNUM10)
	{
		switch(FNum)
		{
		case FNUM16:
			Num10ToNum16((long) ScrNum,ScrText);
			break;
		case FNUM8:
			Num10ToNum8((long) ScrNum,ScrText);
			break;
		case FNUM2:
			Num10ToNum2((long) ScrNum,ScrText);
		}
		ScrLen=(TCHAR) LengthOfString(ScrText);
		ScrText[ScrLen]='.';
		ScrText[ScrLen+1]=0;
	}
	else
		DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnXstep3() 
{
	// TODO: Add your control notification handler code here
	Flag=1;
	if(ScrNum>1.0e+126)
	{
		//GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
		OnClear();
	//	GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
         GetChildByName(L"show_result")->SetTextResID(L"LG_DATAFLOOD");
		//GetChildByName(L"show_result")->SetText( g_LangMode.GetCtrlString(LG_DATAFLOOD) );
		return;
	}
	ScrNum*=ScrNum*ScrNum;
	if(FNum!=FNUM10)
	{
		switch(FNum)
		{
		case FNUM16:
			Num10ToNum16((long) ScrNum,ScrText);
			break;
		case FNUM8:
			Num10ToNum8((long) ScrNum,ScrText);
			break;
		case FNUM2:
			Num10ToNum2((long) ScrNum,ScrText);
		}
		ScrLen=(TCHAR) LengthOfString(ScrText);
		ScrText[ScrLen]='.';
		ScrText[ScrLen+1]=0;
	}
	else
		DoubleToString(ScrNum,ScrText);
	GetChildByName(L"show_result")->SetText(ScrText);
}

void CCalculatorDlg::OnXstepy() 
{
	// TODO: Add your control notification handler code here
	m_bError = FALSE;
	OnEqual();
	if ( m_bError )
		return;
	Sign=FPOW;
	Scr_Num=ScrNum;
}

void CCalculatorDlg::On10num() 
{
#if 0
	// TODO: Add your control notification handler code here
	if(FNum!=FNUM10)
	{
		FNum=FNUM10;
		DoubleToString(ScrNum,ScrText);
		GetChildByName(L"show_result")->SetText(ScrText);
		m_btnNum2.EnableWindow();
		m_btnNum3.EnableWindow();
		m_btnNum4.EnableWindow();
		m_btnNum5.EnableWindow();
		m_btnNum6.EnableWindow();
		m_btnNum7.EnableWindow();
		m_btnNum8.EnableWindow();
		m_btnNum9.EnableWindow();
		m_btnA.EnableWindow(FALSE);
		m_btnB.EnableWindow(FALSE);
		m_btnC.EnableWindow(FALSE);
		m_btnD.EnableWindow(FALSE);
		m_btnE.EnableWindow(FALSE);
		m_btnF.EnableWindow(FALSE);
		m_btnPoint.EnableWindow();
		m_btnSin.EnableWindow();
		m_btnCos.EnableWindow();
		m_btnTan.EnableWindow();
		m_btnAsin.EnableWindow();
		m_btnAcos.EnableWindow();
		m_btnAtan.EnableWindow();
		m_btnExp.EnableWindow();
		m_btnLn.EnableWindow();
		m_btnLg.EnableWindow();
		m_btnPi.EnableWindow();
		m_btnSqrt.EnableWindow();
		m_btnPercent.EnableWindow();
	}
#endif
}

void CCalculatorDlg::On16num() 
{
#if 0
	// TODO: Add your control notification handler code here
	if(FNum!=FNUM16)
	{
		FNum=FNUM16;
		Num10ToNum16((long) ScrNum,ScrText);
		ScrLen=(TCHAR) LengthOfString(ScrText);
		ScrText[ScrLen]='.';
		ScrText[ScrLen+1]=0;
		GetChildByName(L"show_result")->SetText(ScrText);
		m_btnNum2.EnableWindow();
		m_btnNum3.EnableWindow();
		m_btnNum4.EnableWindow();
		m_btnNum5.EnableWindow();
		m_btnNum6.EnableWindow();
		m_btnNum7.EnableWindow();
		m_btnNum8.EnableWindow();
		m_btnNum9.EnableWindow();
		m_btnA.EnableWindow();
		m_btnB.EnableWindow();
		m_btnC.EnableWindow();
		m_btnD.EnableWindow();
		m_btnE.EnableWindow();
		m_btnF.EnableWindow();
		m_btnPoint.EnableWindow(FALSE);
		m_btnSin.EnableWindow(FALSE);
		m_btnCos.EnableWindow(FALSE);
		m_btnTan.EnableWindow(FALSE);
		m_btnAsin.EnableWindow(FALSE);
		m_btnAcos.EnableWindow(FALSE);
		m_btnAtan.EnableWindow(FALSE);
		m_btnExp.EnableWindow(FALSE);
		m_btnLn.EnableWindow(FALSE);
		m_btnLg.EnableWindow(FALSE);
		m_btnPi.EnableWindow(FALSE);
		m_btnSqrt.EnableWindow(FALSE);
		m_btnPercent.EnableWindow(FALSE);
	}
#endif
}

void CCalculatorDlg::On8num() 
{
#if 0
	// TODO: Add your control notification handler code here
	if(FNum!=FNUM8)
	{
		FNum=FNUM8;
		Num10ToNum8((long) ScrNum,ScrText);
		ScrLen=(TCHAR) LengthOfString(ScrText);
		ScrText[ScrLen]='.';
		ScrText[ScrLen+1]=0;
		GetChildByName(L"show_result")->SetText(ScrText);
		m_btnNum2.EnableWindow();
		m_btnNum3.EnableWindow();
		m_btnNum4.EnableWindow();
		m_btnNum5.EnableWindow();
		m_btnNum6.EnableWindow();
		m_btnNum7.EnableWindow();
		m_btnNum8.EnableWindow(FALSE);
		m_btnNum9.EnableWindow(FALSE);
		m_btnA.EnableWindow(FALSE);
		m_btnB.EnableWindow(FALSE);
		m_btnC.EnableWindow(FALSE);
		m_btnD.EnableWindow(FALSE);
		m_btnE.EnableWindow(FALSE);
		m_btnF.EnableWindow(FALSE);
		m_btnNum9.EnableWindow(FALSE);
		m_btnNum8.EnableWindow(FALSE);
		m_btnPoint.EnableWindow(FALSE);
		m_btnSin.EnableWindow(FALSE);
		m_btnCos.EnableWindow(FALSE);
		m_btnTan.EnableWindow(FALSE);
		m_btnAsin.EnableWindow(FALSE);
		m_btnAcos.EnableWindow(FALSE);
		m_btnAtan.EnableWindow(FALSE);
		m_btnExp.EnableWindow(FALSE);
		m_btnLn.EnableWindow(FALSE);
		m_btnLg.EnableWindow(FALSE);
		m_btnPi.EnableWindow(FALSE);
		m_btnSqrt.EnableWindow(FALSE);
		m_btnPercent.EnableWindow(FALSE);
	}
#endif
}

void CCalculatorDlg::On2num() 
{
#if 0
	// TODO: Add your control notification handler code here
	if(FNum!=FNUM2)
	{
		FNum=FNUM2;
		Num10ToNum2((long) ScrNum,ScrText);
		ScrLen=(TCHAR) LengthOfString(ScrText);
		ScrText[ScrLen]='.';
		ScrText[ScrLen+1]=0;
		GetChildByName(L"show_result")->SetText(ScrText);
		m_btnNum2.EnableWindow(FALSE);
		m_btnNum3.EnableWindow(FALSE);
		m_btnNum4.EnableWindow(FALSE);
		m_btnNum5.EnableWindow(FALSE);
		m_btnNum6.EnableWindow(FALSE);
		m_btnNum7.EnableWindow(FALSE);
		m_btnNum8.EnableWindow(FALSE);
		m_btnNum9.EnableWindow(FALSE);
		m_btnA.EnableWindow(FALSE);
		m_btnB.EnableWindow(FALSE);
		m_btnC.EnableWindow(FALSE);
		m_btnD.EnableWindow(FALSE);
		m_btnE.EnableWindow(FALSE);
		m_btnF.EnableWindow(FALSE);
		m_btnNum9.EnableWindow(FALSE);
		m_btnNum8.EnableWindow(FALSE);
		m_btnNum7.EnableWindow(FALSE);
		m_btnNum6.EnableWindow(FALSE);
		m_btnNum5.EnableWindow(FALSE);
		m_btnNum4.EnableWindow(FALSE);
		m_btnNum3.EnableWindow(FALSE);
		m_btnNum2.EnableWindow(FALSE);
		m_btnPoint.EnableWindow(FALSE);
		m_btnSin.EnableWindow(FALSE);
		m_btnCos.EnableWindow(FALSE);
		m_btnTan.EnableWindow(FALSE);
		m_btnAsin.EnableWindow(FALSE);
		m_btnAcos.EnableWindow(FALSE);
		m_btnAtan.EnableWindow(FALSE);
		m_btnExp.EnableWindow(FALSE);
		m_btnLn.EnableWindow(FALSE);
		m_btnLg.EnableWindow(FALSE);
		m_btnPi.EnableWindow(FALSE);
		m_btnSqrt.EnableWindow(FALSE);
		m_btnPercent.EnableWindow(FALSE);
	}
#endif
}

