#include "lcd_driver.h"



VOID NUMBER_CLEAR(INT8 position)
{
	switch(position)
	{
		case 1:
			NUMBER_CLEAR_IN(1);
			break;
		case 2:
			NUMBER_CLEAR_IN(2);
			break;
		case 3:
			NUMBER_CLEAR_IN(3);
			break;
		case 4:
			NUMBER_CLEAR_IN(4);
			break;
		case 5:
			NUMBER_CLEAR_IN(5);
			break;
		case 6:
			NUMBER_CLEAR_IN(6);
			break;
		case 7:
			NUMBER_CLEAR_IN(7);
			break;
		case 8:
			NUMBER_CLEAR_IN(8);
			break;
	}
}	


VOID ALL_NUMBER_CLEAR(VOID)
{
	NUMBER_CLEAR(1);
	NUMBER_CLEAR(2);
	NUMBER_CLEAR(3);
}

VOID NUMBER_0(INT8 position)
{
	NUMBER_CLEAR(position);
	switch(position)
	{
		case 1:
			NUMBER_0_IN(1);
			break;
		case 2:
			NUMBER_0_IN(2);
			break;
		case 3:
			NUMBER_0_IN(3);
			break;
		case 4:
			NUMBER_0_IN(4);
			break;
		case 5:
			NUMBER_0_IN(5);
			break;
		case 6:
			NUMBER_0_IN(6);
			break;
		case 7:
			NUMBER_0_IN(7);
			break;
		case 8:
			NUMBER_0_IN(8);
			break;
	}
}

VOID NUMBER_1(INT8 position)
{
	NUMBER_CLEAR(position);
	switch(position)
	{
		case 1:
			NUMBER_1_IN(1);
			break;
		case 2:
			NUMBER_1_IN(2);
			break;
		case 3:
			NUMBER_1_IN(3);
			break;
		case 4:
			NUMBER_1_IN(4);
			break;
		case 5:
			NUMBER_1_IN(5);
			break;
		case 6:
			NUMBER_1_IN(6);
			break;
		case 7:
			NUMBER_1_IN(7);
			break;
		case 8:
			NUMBER_1_IN(8);
			break;
	}
}


VOID NUMBER_2(INT8 position)
{
	NUMBER_CLEAR(position);
	switch(position)
	{
		case 1:
			NUMBER_2_IN(1);
			break;
		case 2:
			NUMBER_2_IN(2);
			break;
		case 3:
			NUMBER_2_IN(3);
			break;
		case 4:
			NUMBER_2_IN(4);
			break;
		case 5:
			NUMBER_2_IN(5);
			break;
		case 6:
			NUMBER_2_IN(6);
			break;
		case 7:
			NUMBER_2_IN(7);
			break;
		case 8:
			NUMBER_2_IN(8);
			break;
	}
}

VOID NUMBER_3(INT8 position)
{
	NUMBER_CLEAR(position);
	switch(position)
	{
		case 1:
			NUMBER_3_IN(1);
			break;
		case 2:
			NUMBER_3_IN(2);
			break;
		case 3:
			NUMBER_3_IN(3);
			break;
		case 4:
			NUMBER_3_IN(4);
			break;
		case 5:
			NUMBER_3_IN(5);
			break;
		case 6:
			NUMBER_3_IN(6);
			break;
		case 7:
			NUMBER_3_IN(7);
			break;
		case 8:
			NUMBER_3_IN(8);
			break;
	}
}

VOID NUMBER_4(INT8 position)
{
	NUMBER_CLEAR(position);
	switch(position)
	{
		case 1:
			NUMBER_4_IN(1);
			break;
		case 2:
			NUMBER_4_IN(2);
			break;
		case 3:
			NUMBER_4_IN(3);
			break;
		case 4:
			NUMBER_4_IN(4);
			break;
		case 5:
			NUMBER_4_IN(5);
			break;
		case 6:
			NUMBER_4_IN(6);
			break;
		case 7:
			NUMBER_4_IN(7);
			break;
		case 8:
			NUMBER_4_IN(8);
			break;
	}
}

VOID NUMBER_5(INT8 position)
{
	NUMBER_CLEAR(position);
	switch(position)
	{
		case 1:
			NUMBER_5_IN(1);
			break;
		case 2:
			NUMBER_5_IN(2);
			break;
		case 3:
			NUMBER_5_IN(3);
			break;
		case 4:
			NUMBER_5_IN(4);
			break;
		case 5:
			NUMBER_5_IN(5);
			break;
		case 6:
			NUMBER_5_IN(6);
			break;
		case 7:
			NUMBER_5_IN(7);
			break;
		case 8:
			NUMBER_5_IN(8);
			break;
	}
}

VOID NUMBER_6(INT8 position)
{
	NUMBER_CLEAR(position);
	switch(position)
	{
		case 1:
			NUMBER_6_IN(1);
			break;
		case 2:
			NUMBER_6_IN(2);
			break;
		case 3:
			NUMBER_6_IN(3);
			break;
		case 4:
			NUMBER_6_IN(4);
			break;
		case 5:
			NUMBER_6_IN(5);
			break;
		case 6:
			NUMBER_6_IN(6);
			break;
		case 7:
			NUMBER_6_IN(7);
			break;
		case 8:
			NUMBER_6_IN(8);
			break;
	}
}

VOID NUMBER_7(INT8 position)
{
	NUMBER_CLEAR(position);
	switch(position)
	{
		case 1:
			NUMBER_7_IN(1);
			break;
		case 2:
			NUMBER_7_IN(2);
			break;
		case 3:
			NUMBER_7_IN(3);
			break;
		case 4:
			NUMBER_7_IN(4);
			break;
		case 5:
			NUMBER_7_IN(5);
			break;
		case 6:
			NUMBER_7_IN(6);
			break;
		case 7:
			NUMBER_7_IN(7);
			break;
		case 8:
			NUMBER_7_IN(8);
			break;
	}
}

VOID NUMBER_8(INT8 position)
{
	NUMBER_CLEAR(position);
	switch(position)
	{
		case 1:
			NUMBER_8_IN(1);
			break;
		case 2:
			NUMBER_8_IN(2);
			break;
		case 3:
			NUMBER_8_IN(3);
			break;
		case 4:
			NUMBER_8_IN(4);
			break;
		case 5:
			NUMBER_8_IN(5);
			break;
		case 6:
			NUMBER_8_IN(6);
			break;
		case 7:
			NUMBER_8_IN(7);
			break;
		case 8:
			NUMBER_8_IN(8);
			break;
	}
}

VOID NUMBER_9(INT8 position)
{
	NUMBER_CLEAR(position);
	switch(position)
	{
		case 1:
			NUMBER_9_IN(1);
			break;
		case 2:
			NUMBER_9_IN(2);
			break;
		case 3:
			NUMBER_9_IN(3);
			break;
		case 4:
			NUMBER_9_IN(4);
			break;
		case 5:
			NUMBER_9_IN(5);
			break;
		case 6:
			NUMBER_9_IN(6);
			break;
		case 7:
			NUMBER_9_IN(7);
			break;
		case 8:
			NUMBER_9_IN(8);
			break;
	}
}


VOID LCD_Display_init(IWON_TEMP_FUNC *IWonFunc)
{
	CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

	CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);

	LCD_Init(LCD_Prescaler_1, LCD_Divider_22, LCD_Duty_1_4, LCD_Bias_1_3, LCD_VoltageSource_Internal);

	LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
	LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF);
	LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0x01);

	LCD_ContrastConfig(LCD_Contrast_3V0);

	LCD_DeadTimeConfig(LCD_DeadTime_0);
	LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);

	LCD_HighDriveCmd(ENABLE);

	LCD_Cmd(ENABLE);

	ALL_NUMBER_CLEAR();
	
	memNumberDisplay(memNumber_p);
	memTempDataDisplay(IWonFunc->UnitCalc(memTemp_p(memNumber_p - 1), tempUnit_p));
	measureModeSet(measureMode_p);
	IWonFunc->BuzzerCMD(buzzerState_p);
	tempUnitSet(tempUnit_p);

	LCD->X8 = 1;   // Display "LOG"
	//LCD->DP1 = 1;  // Display "."
	//LCD->DP2 = 1;  // Display "."
}

void displayNumber(INT8 number, INT8 position)
{
	switch (number)
	{
		case -1:
			NUMBER_CLEAR(position);
			break;

		case 0:
			NUMBER_0(position);
			break;

		case 1:
			NUMBER_1(position);
			break;

		case 2:
			NUMBER_2(position);
			break;

		case 3:
			NUMBER_3(position);
			break;

		case 4:
			NUMBER_4(position);
			break;

		case 5:
			NUMBER_5(position);
			break;

		case 6:
			NUMBER_6(position);
			break;

		case 7:
			NUMBER_7(position);
			break;

		case 8:
			NUMBER_8(position);
			break;

		case 9:
			NUMBER_9(position);
			break;
	}
}

void tempValueDisplay(INT16 value, BOOL fillZero)
{	
	ALL_NUMBER_CLEAR();

	// 소수점
	LCD->DP1 = 1;

	// 마이너스 부호표시 (테스트용)
	//if(value < 0) 
		//LCD->DP1 = 0;

	// 음수도 양수로 처리 (마이너스는 위해서 표시됨)
	if(value<0) value = ABS(value);

	INT8 forthNumber = (INT8)(value % 10);
	INT8 thirdNumber = (INT8)((value / 10) % 10);
	INT8 secondNumber = (INT8)((value / 100) % 10);
	INT8 firstNumber = (INT8)(value / 1000);

	if (firstNumber == 1)
		LCD->X9 = 1;
	else
		LCD->X9 = 0;

	if (firstNumber == 1 || secondNumber != 0)
	{
		displayNumber(secondNumber, 1);
	}
	else
	{
		if(fillZero)
		{
			displayNumber(0, 1);
		}
		else
		{
			NUMBER_CLEAR(1);
		}		
	}
	
	displayNumber(thirdNumber, 2);
	displayNumber(forthNumber, 3);
}

void tempValueDisplay(INT16 value)
{
	tempValueDisplay(value, true);
}

void memNumberDisplay(INT8 number)
{
	INT8 secondNum = number % 10;
	INT8 firstNum = number / 10;

	displayNumber(firstNum, 4);
	displayNumber(secondNum, 5);
}

void memTempDataDisplay(INT16 tempData, BOOL fillZero)
{
	INT8 forthNumber = (INT8)(tempData % 10);
	INT8 thirdNumber = (INT8)((tempData / 10) % 10);
	INT8 secondNumber = (INT8)((tempData / 100) % 10);
	INT8 firstNumber = (INT8)(tempData / 1000);
	
    NUMBER_CLEAR(6);
	NUMBER_CLEAR(7);
	NUMBER_CLEAR(8);
	

	if (firstNumber == 1)
		LCD->X10 = 1;
	else
		LCD->X10 = 0;

	if (firstNumber == 1 || (firstNumber == 0 && secondNumber != 0))
	{
		displayNumber(secondNumber, 6);
	}
	else
	{
		if(fillZero)
		{
			displayNumber(0, 6);
		}
		else
		{
			NUMBER_CLEAR(6);
		}		
	}
	
	displayNumber(thirdNumber, 7);
	displayNumber(forthNumber, 8);
	LCD->DP2 = 1;
}

void memTempDataDisplay(INT16 tempData)
{
	memTempDataDisplay(tempData, true);
}

void measureModeSet(BOOL mode)
{
	if (mode) // HUMAN
	{
		LCD->X1 = 0;
		LCD->X2 = 1;
	}
	else // OBJECT
	{
		LCD->X1 = 1;
		LCD->X2 = 0;
	}
}


void tempUnitSet(BOOL unit)
{
	if (unit) // Celsius
	{
		LCD->X6 = 1;
		LCD->X7 = 0;
	}

	else // Fahrenheit
	{
		LCD->X6 = 0;
		LCD->X7 = 1;
	}
}
