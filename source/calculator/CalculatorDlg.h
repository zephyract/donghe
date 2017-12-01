#pragma once
#include "wceuidlg.h"
#include "WceUiLayer.h"
#include "WceUiButton.h"
#include "wceuirealwnd.h"
#include "AnimDlg.h"
#include <vector>
#define		FADD	1
#define		FSUB	2
#define		FMUL	3
#define		FDIV	4
#define		FPOW	5
#define		FAND	6
#define		FOR		7
#define		FXOR	8
#define		FNUM16	9
#define		FNUM10	10
#define		FNUM8	11
#define		FNUM2	12


#define USE_ANGLE_TRIGONOMETRY
#define M_PI 3.14159265358979323846 
#define  MAX_NUM_COUNT   18

class CCalculatorDlg : public CAnimDlg
{
	WCEUI_DYNCREATE_BEGIN(CCalculatorDlg, CAnimDlg)

public:
	CCalculatorDlg(void);
	~CCalculatorDlg(void);
	void BtnInit();
	virtual void OnInitDialog();
	virtual void OnBnClick(PCWceUiButton pButton);
	
	long LengthOfString(const TCHAR *str);
	long GetCharPosFromLeft(const TCHAR *str1,TCHAR chr);
	long GetUnCharPosFromRight(const TCHAR *str1,TCHAR chr);
	void DoubleToString(double sum,TCHAR *str);
	void Delay(unsigned long ti);
	void Num10ToNum2(long num10,TCHAR *num2);
	void Num10ToNum8(long num10,TCHAR *num8);
	void Num10ToNum16(long num10,TCHAR *num16);
	void OnStandard();
	void OnExtend();
	void On0();
	void On1();
	void On2();
	void On3();
	void On4();
	void On5();
	void On6();
	void On7();
	void On8();
	void On9();
	void OnAdd();
	void OnBackspace();
	void OnClean();
	void OnClear();
	void OnDiv();
	void OnEqual();
	void OnMadd();
	void OnMclear();
	void OnMread();
	void OnMsave();
	void OnMul();
	void OnNeg();
	void OnPercent();
	void OnPoint();
	void OnReciprocal();
	void OnSqrt();
	void OnSub();
	void OnPi();
	void OnAnd();
	void OnNot();
	void OnOr();
	void OnXor();
	void OnArccos();
	void OnArcsin();
	void OnArctan();
	void OnCos();
	void OnExp();
	void OnFactorial();
	void OnLg();
	void OnLn();
	void OnSin();
	void OnTan();
	void OnXstep2();
	void OnXstep3();
	void OnXstepy();
	void On10num();
	void On16num();
	void On8num();
	void On2num();
	void OnExit();
private:
	long double ScrNum,Scr_Num,MNum;
	UINT				m_nMenuKeyMsg;
	TCHAR ScrLen,ScrText[40],Sign,Fpoint,Flag,FNum;
	BYTE Key;
	BOOL  m_bError;			//判断计算过程是否有错误，onequal没有返回值



};
WCEUI_DYNCREATE_END(CCalculatorDlg, CAnimDlg)