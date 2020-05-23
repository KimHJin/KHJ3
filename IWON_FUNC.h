///////////////////////////////////////////////////////////////////////////////
// Code Name : IWON TEST
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "stm8l15x.h"
#include "stm8l15x_conf.h"
#include "lcd_driver.h"

#include "IWON_VAVG.h"

#define HAL_ADC_MODULE_ENABLED


#ifndef IWON_TEMP_FUNC_H
#define IWON_TEMP_FUNC_H

#define DEFINED_TADJ0 0
#define DEFINED_VADJ1 0
#define DEFINED_VADJ2 0

#define DEFINED_TSUMN 10

#define NTC_MIN -40
#define NTC_MAX 125

// 사물기준   (체온 계산용)
#define TB_MIN 281
#define TB_MAX 425

//#define ADC_CONVERT_RATIO	806		/* (3300mV / 0xFFF) x 1000 */
#define ADC_CONVERT_RATIO 804 /* (3287mV ~ 3300mV / 0xFFF) x 1000 */
//#define ADC_CONVERT_RATIO	802		/* (3287mV / 0xFFF)	x 1000 */

// 피부 방사율을 0.98 정도
// 환경 방사 온도 변경법 등에 의한 정확한 계측에서는, 사람의 피부 의 방사율은 0.95 ~ 0.97 정도
#define BODY_EMI 0.97

#define GPIO_HIGH(a, b) a->ODR |= b
#define GPIO_LOW(a, b) a->ODR &= ~b
#define GPIO_TOGGLE(a, b) a->ODR ^= b


#define SW_PWR_ON    !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
#define SW_LEFT_ON   !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6)
#define SW_RIGHT_ON  !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)
#define LED_ON()      GPIO_LOW(GPIOD, GPIO_Pin_6)  
#define LED_OFF()     GPIO_HIGH(GPIOD, GPIO_Pin_6)  

#define TEST_MODE_ON !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)

#define HIGH_FEVER 1
#define LIGHT_FEVER 2
#define NORMAL 3

#define CLEAR 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define CYAN 5
#define MAGENTA 6
#define WHITE 7




#include "IWON_TASK.h"

class IWON_TEMP_FUNC
{
public:
	// 생성자
	IWON_TEMP_FUNC();

	// 소멸자
	~IWON_TEMP_FUNC();

	

	VOID Delay_10us(INT16 us);
	VOID Delay_ms(INT16 ms);
	INT8 Delay_10us_with_key(INT16 us);
	INT8 Delay_ms_with_key(INT16 ms);	

	
	VOID Beep(INT16 length);
	VOID Beep(VOID);
	VOID BeepMode(INT16 mode);

	VOID POWER_DOWN(VOID);

	VOID LCD_clear(VOID);
	VOID DisplayRGB(INT8 color);
	VOID DisplayLOW(VOID);
	VOID DisplayHIGH(VOID);
	VOID DisplayError(VOID);
	VOID DisplayOFF(VOID);

	INT8 LowBatteryFlag;
	INT8 Measure_test_flag;
	INT8 LastMeasred;
	
	INT8 AutoCalStep;
	
	BOOL LowHigh_FLag;
	
	BOOL passFlag2;
	BOOL passFlag3;
	BOOL passFlagHigh;
	BOOL passFlagLow;
	
	BOOL measuredFlag;
	
	INT8 AutoCalDelayCount;
	
	INT16 UnitCalc(INT16 temp, BOOL unit);
	
	VOID LowBatteryDisplay_2v0(VOID);
	VOID LowBatteryDisplay_2v2(VOID);
	VOID LowBatteryDisplay_2v4(VOID);
	
	
	VOID BuzzerStateTask(VOID);
	VOID MeasureModeTask(VOID);

	INT8 TempUnitTask(BOOL inv);
	VOID TempLogDataTask(VOID);	
	
	VOID SpecialModeDisp(int16_t value);
	
	VOID CaliDone(IWON_TEMP_TASK *IWonTask);
	VOID SpecialMode(IWON_TEMP_TASK *IWonTask);
	VOID SpecialModeTask(IWON_TEMP_TASK *IWonTask);

	VOID TempLogDataSave(int16_t saveData);
	VOID TempLogDataClear(VOID);

	VOID SaveTemp(INT16 temp);

	VOID ObjTempDisp(INT16 temp);
	VOID BdyTempDisp(INT16 temp);

	VOID SystemError(VOID);
	VOID MeasuringDisp(VOID);
	
	VOID ALLCLEAR(VOID);
	
	VOID AUTOCAL(IWON_TEMP_TASK *IWonTask);
	INT8 GET_AutoCalStep(VOID);

	VOID SuccessDisp(VOID);
	VOID FailDisp(VOID);
	VOID BuzzerCMD(BOOL state);


private:   
	VOID Init(VOID);

	IWON_TEMP_VAVG *AutoCalAVG;
};

#endif