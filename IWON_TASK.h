///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEMP TASK
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
// 2020/04/27 v1.1 by KHJ
// 2020/05/14 v1.2 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_SCAN.h"
#include "IWON_VAVG.h"

#ifndef IWON_TEMP_TASK_H
#define IWON_TEMP_TASK_H

// 펌웨어 버전의 : 10 이면 1.0 이다. 11 이면 1.1 이다.
#define DEFINED_FW_VER 13

#define DEFINED_adcVREFValue 1224

#define DEFINED_VDD 3300
#define DEFINED_R1 100000
#define DEFINED_R2 200000

#define DEFINED_ADC_CALC 50
#define DEFINED_ADC_DELAY 20
#define DEFINED_ADC_SUM_C 2

class IWON_TEMP_TASK : IWON_TEMP_SCAN
{

public:
	// 생성자
	IWON_TEMP_TASK();
	IWON_TEMP_TASK(INT8 tsumn); // 온도 평균값을 만들기 위하여 몇개의 방을 써야할지 결정

	// 소멸자
	~IWON_TEMP_TASK();

	BOOL Task(UINT MGInterval, UINT TTInterval);
	BOOL Task(VOID);
	VOID Time(VOID);

	BOOL Was_Calc(VOID);

	INT16 Get_AMB_TEMP(VOID);
	INT16 Get_OBJ_TEMP(VOID);
	INT16 Get_BDY_TEMP(VOID);
	
	INT16 Get_BAT_mV(VOID);
	INT16 Get_VDD_mV(VOID);
	INT16 Get_NTC_mV(VOID);
	INT32 Get_TPC_mV(VOID);

	UINT Get_ADC_CAL(VOID);

	VOID Clear_AVG(VOID);
	VOID Set_AdjValue(INT32 ADJV);

	BOOL NeedPowerDown(VOID);
	VOID ClearPowerDown(VOID);

	VOID YellowDisp(VOID);
	VOID lowBatteryDisp(VOID);
	

  	INT16 BATmV;
	INT16 VDDmV;

	INT16 DeviceTestModeWait;	// 테스트 가능 모드를 위해서 있는 변수
	INT16 DeviceTestModeValue;	// 테스트 가능 모드를 위해서 있는 변수

	BOOL Measuring;
	BOOL Measured;
	INT16 MeasredTemp;
	INT8 MeasredCount1;
	INT8 MeasredCount2;
	INT8 RetryCount;
	
	INT32 VoltmV;

private:
	VOID Init(VOID);

	VOID GPIO_init(VOID);

	VOID Delay_10us(INT16 us);
	VOID Delay_ms(INT16 ms);	
	
	VOID Init_Clock(VOID);
	VOID Init_TIM4(VOID);
	VOID Init_ADC(VOID);

	VOID DISABLE_ALL_ADC(VOID) ;

	DWORD GetTimeStamp(VOID);
	VOID SetTimeStamp(DWORD t);
	DWORD GetTimeOutStartTime(VOID);
	BOOL TimeOut(DWORD startTime, UINT delayTime);

	BOOL IS_BODY_CC(VOID);

	INT32 powerDown_msec;

	UINT adcVREFValue; // 1.224V
	UINT adcVREFINTvalue;
	UINT adcCalValue;

	UINT adc0value;
	float adc0volt;
	UINT adc1value;
	float adc1volt;
	UINT adc2value;
	float adc2volt;

	INT32 V0;
	DWORD R0;
	DWORD R1; // 100K
	DWORD R2; // 200K
	DWORD R3; // 측정된 써미스터 저항값

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

	int lowBattery_Count;
	
};

#endif