///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON VAVG
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_SCAN.h"

#ifndef IWON_TEMP_VAVG_H
#define IWON_TEMP_VAVG_H


class IWON_TEMP_VAVG {
    
public:
	// ������
	IWON_TEMP_VAVG();
	IWON_TEMP_VAVG(INT8 sumNum);   // ��հ��� ����� ���Ͽ� ��� ���� ������� ����
	IWON_TEMP_VAVG(INT8 sumNum, INT8 OC);

	// �Ҹ���
	~IWON_TEMP_VAVG();

	VOID Init(VOID);
	VOID Add(INT32 v);
	VOID Rm(VOID);
	INT32 Calc(VOID);
	INT32 AddCalcVOID();
	INT32 AddCalc(INT32 v);
	INT32 AddCalc(INT32 v, INT32 cv);
	BOOL IsOC(VOID);

private:  	
	INT32 *val;	
	INT8 num;
	INT8 pos;
	INT8 erc;
	INT8 oc;
	INT8 cc;
};

#endif