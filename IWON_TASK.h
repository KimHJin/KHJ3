///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEMP TASK
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_SCAN.h"
#include "IWON_VAVG.h"

#ifndef IWON_TEMP_TASK_H
#define IWON_TEMP_TASK_H


#define DEFINED_adcVREFValue 1224

#define DEFINED_VDDREF 3300
#define DEFINED_VDD 3300
#define DEFINED_R1 100000
#define DEFINED_R2 200000


class IWON_TEMP_TASK : IWON_TEMP_SCAN{
    
public:
	// ������
	IWON_TEMP_TASK();
	IWON_TEMP_TASK(INT8 tsumn);   // �µ� ��հ��� ����� ���Ͽ� ��� ���� ������� ����

	// �Ҹ���
	~IWON_TEMP_TASK();

	BOOL Task(UINT MGInterval, UINT TTInterval);
	BOOL Task(VOID);
	VOID Time(VOID);

	INT16 Get_AMB_TEMP(VOID);
	INT16 Get_OBJ_TEMP(VOID);
	INT16 Get_BDY_TEMP(VOID);
	
	VOID Clear_AVG(VOID);
	VOID Set_AdjValue(INT32 ADJV);
	
	BOOL NeedPowerDown(VOID);
	VOID ClearPowerDown(VOID);

private:  
	VOID Init(VOID);

	VOID Init_Clock(VOID);
	VOID Init_TIM4(VOID);
	VOID Init_ADC(VOID);	
	
	DWORD GetTimeStamp(VOID);
	VOID SetTimeStamp(DWORD t);
	DWORD GetTimeOutStartTime(VOID);
	BOOL TimeOut(DWORD startTime, UINT delayTime);
	
	INT32 powerDown_msec;

	UINT adcVREFValue;   // 1.224V
	UINT adcVREFINTvalue;
	UINT adcCalValue;

	UINT adc0value;
	float adc0volt;
	UINT adc1value;
	float adc1volt;
	UINT adc2value;
	float adc2volt;

	INT32 VDDREF;  // ������� VDD ���� 3.3V
	INT32 VDD;     // �����Ͽ� ������ VDD ����
	INT32 VCAL;    // ������� VDD ������ ���� ������ ADC �ɼ�
	INT32 V0;
	DWORD R0;
	DWORD R1;   // 100K
	DWORD R2;   // 200K
	DWORD R3;   // ������ ��̽��� ���װ�
	
	INT32 sumCount;
	
	DWORD timeStamp;
	DWORD startTime;
	
	INT32 Vrefint;
	INT32 Vrefvdd;
	INT32 Vrefbat;
	INT32 Vrefntc;
	INT32 Vreftpc;
	
	DWORD TTtime;
	DWORD MGtime;

	INT8 tCC;
	
	IWON_TEMP_VAVG *VrefintAvg;
	IWON_TEMP_VAVG *VrefvddAvg;
	IWON_TEMP_VAVG *VrefbatAvg;
	IWON_TEMP_VAVG *VrefntcAvg;
	IWON_TEMP_VAVG *VreftpcAvg;
	
	INT32 VrefintmV;
	INT32 VrefvddmV;
	INT32 VrefbatmV;
	INT32 VrefntcmV;
	INT32 VreftpcmV;
	
	INT16 AMB_TEMP;
	INT16 OBJ_TEMP;
	INT16 BDY_TEMP;
	
	INT16 ADJ_VALUE;
};

#endif