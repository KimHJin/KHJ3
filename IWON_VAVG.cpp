///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON VAVG
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_VAVG.h"

IWON_TEMP_VAVG::IWON_TEMP_VAVG()
{
	num = 20;
	pos = 0;
	erc = 0;
	oc = 0;
	cc = 0;
	val = (INT32 *)malloc(sizeof(INT32) * num);
	Init();
}

IWON_TEMP_VAVG::IWON_TEMP_VAVG(INT8 sumNum)
{ // 평균값을 만들기 위하여 몇개의 방을 써야할지 결정
	num = sumNum;
	pos = 0;
	erc = 0;
	oc = 0;
	cc = 0;
	val = (INT32 *)malloc(sizeof(INT32) * num);
	Init();
}
IWON_TEMP_VAVG::IWON_TEMP_VAVG(INT8 sumNum, INT8 OC)
{
	num = sumNum;
	pos = 0;
	erc = 0;
	oc = OC;
	cc = 0;
	val = (INT32 *)malloc(sizeof(INT32) * num);
	Init();
}

// 소멸자
IWON_TEMP_VAVG::~IWON_TEMP_VAVG(VOID)
{
	free(val);
}

VOID IWON_TEMP_VAVG::Init(VOID)
{
	for (INT8 i = 0; i < num; i++)
	{
		val[i] = 0xffffffff;
	}
	pos = 0;
	erc = 0;
	cc = 0;
}

VOID IWON_TEMP_VAVG::Add(INT32 v)
{
	val[pos] = v;
	pos++;
	if (pos == num)
		pos = 0;
	cc++;
}

VOID IWON_TEMP_VAVG::Rm()
{
	pos--;
	if (pos < 0)
		pos = num - 1;
	val[pos] = 0xffffffff;
	cc--;
}
INT32 IWON_TEMP_VAVG::Calc(VOID)
{
	DWORD d = 0;
	INT8 c = 0;
	for (INT8 i = 0; i < num; i++)
	{
		if (val[i] == 0xffffffff)
			continue;
		d += val[i];
		c++;
	}
	if (c == 0)
		return 0;
	return (INT32)(d / c);
}
INT32 IWON_TEMP_VAVG::AddCalc(INT32 v)
{
	Add(v);
	return Calc();
}
INT32 IWON_TEMP_VAVG::AddCalc(INT32 v, INT32 cv)
{
	INT32 vv = AddCalc(v);
	if ((vv - v) > cv || cv > (vv - v))
	{
		erc++;
		if (erc < 1)
		{
			Rm();
			return Calc();
		}
		Init();
		Add(v);
		return v;
	}
	return vv;
}

BOOL IWON_TEMP_VAVG::IsOC(VOID)
{
	return (oc > 0 && oc < cc);
}
VOID IWON_TEMP_VAVG::SetOC(VOID)
{
	oc = 1;
	cc = 2;
}
INT8 IWON_TEMP_VAVG::GetCC(VOID)
{
	return cc;
}
