///////////////////////////////////////////////////////////////////////////////
// Code Name : IWON TEMP FUNC
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_TASK.h"
#include "lcd_driver.h"

#ifndef IWON_TEMP_FUNC_H
#define IWON_TEMP_FUNC_H

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
		
	BOOL LowHigh_FLag;		
		
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
	
	VOID SuccessDisp(VOID);
	VOID FailDisp(VOID);
	VOID OkDisp(VOID);
	VOID VerDisp(VOID);

	VOID BuzzerCMD(BOOL state);

	VOID ClearDisp(VOID);

	VOID TempValueDisplay(INT16 value, BOOL fillZero);
	VOID TempValueDisplay(INT16 value);

private:   
	VOID Init(VOID);
};

#endif