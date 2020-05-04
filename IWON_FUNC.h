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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>

#include "lcd_driver.h"

#define HAL_ADC_MODULE_ENABLED


#ifndef IWON_TEMP_FUNC_H
#define IWON_TEMP_FUNC_H

#define CHAR char
#define VOID void
#define CONST const
#define BOOL bool
#define LOCAL(type) static type
#define WINAPI
#define CALLBACK
#define FALSE 0
#define TRUE 1


typedef signed char INT8;
typedef int INT16;
typedef long INT32;
typedef long INT;
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef char *LPSTR;
typedef const char *LPCSTR;
typedef VOID *LPVOID;
typedef const VOID *LPCVOID;
typedef BYTE *LPBYTE;
typedef const BYTE *LPCBYTE;

#define lstrlen strlen
#define lstrcpy strcpy
#define lstrcat strcat
#define lstrcmp strcmp
#define lstrcmpi strcasecmp

#define CRLF "\r\n"
#define CRLFSP "     \r\n"

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



#define SW_PWR_ON    !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
#define SW_LEFT_ON   !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6)
#define SW_RIGHT_ON  !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)

#define TEST_MODE_ON !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)

#define HIGH_FEVER 1
#define LIGHT_FEVER 2
#define NORMAL 3

#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define CLEAR 5

#define GPIO_HIGH(a, b) a->ODR |= b
#define GPIO_LOW(a, b) a->ODR &= ~b
#define GPIO_TOGGLE(a, b) a->ODR ^= b


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

	INT8 YellowFlag;
	INT8 LowBatteryFlag;
	INT8 Measure_test_flag;
	INT8 LastMeasred;
	
	int16_t UnitCalc(int16_t temp, int unit);
	
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
	VOID SaveTemp(INT16 temp);
	VOID ObjTempDisp(INT16 temp);
	VOID BdyTempDisp(INT16 temp);

	VOID SystemError(VOID);
	VOID MeasuringDisp(VOID);

private:   
	VOID Init(VOID);
};

#endif