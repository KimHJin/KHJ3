#include "lcd_driver.h"


void LCD_Display_init(IWON_TEMP_FUNC *IWonFunc)
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

	NUMBER_CLEAR(1);
	NUMBER_CLEAR(2);
	NUMBER_CLEAR(3);

	memNumberDisplay(memNumber_p);
	memTempDataDisplay(IWonFunc->UnitCalc(memTemp_p(memNumber_p - 1), tempUnit_p));
	measureModeSet(measureMode_p);
	buzzerCMD(buzzerState_p);
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

		if (position == 1)
		{
			NUMBER_CLEAR(1);
		}
		else if (position == 2)
		{
			NUMBER_CLEAR(2);
		}
		else if (position == 3)
		{
			NUMBER_CLEAR(3);
		}
		else if (position == 4)
		{
			NUMBER_CLEAR(4);
		}
		else if (position == 5)
		{
			NUMBER_CLEAR(5);
		}
		else if (position == 6)
		{
			NUMBER_CLEAR(6);
		}
		else if (position == 7)
		{
			NUMBER_CLEAR(7);
		}
		else if (position == 8)
		{
			NUMBER_CLEAR(8);
		}

		break;

	case 0:

		if (position == 1)
		{
			NUMBER_CLEAR(1);
			NUMBER_0(1);
		}
		else if (position == 2)
		{
			NUMBER_CLEAR(2);
			NUMBER_0(2);
		}
		else if (position == 3)
		{
			NUMBER_CLEAR(3);
			NUMBER_0(3);
		}
		else if (position == 4)
		{
			NUMBER_CLEAR(4);
			NUMBER_0(4);
		}
		else if (position == 5)
		{
			NUMBER_CLEAR(5);
			NUMBER_0(5);
		}
		else if (position == 6)
		{
			NUMBER_CLEAR(6);
			NUMBER_0(6);
		}
		else if (position == 7)
		{
			NUMBER_CLEAR(7);
			NUMBER_0(7);
		}
		else if (position == 8)
		{
			NUMBER_CLEAR(8);
			NUMBER_0(8);
		}

		break;

	case 1:
		if (position == 1)
		{
			NUMBER_CLEAR(1);
			NUMBER_1(1);
		}
		else if (position == 2)
		{
			NUMBER_CLEAR(2);
			NUMBER_1(2);
		}
		else if (position == 3)
		{
			NUMBER_CLEAR(3);
			NUMBER_1(3);
		}
		else if (position == 4)
		{
			NUMBER_CLEAR(4);
			NUMBER_1(4);
		}
		else if (position == 5)
		{
			NUMBER_CLEAR(5);
			NUMBER_1(5);
		}
		else if (position == 6)
		{
			NUMBER_CLEAR(6);
			NUMBER_1(6);
		}
		else if (position == 7)
		{
			NUMBER_CLEAR(7);
			NUMBER_1(7);
		}
		else if (position == 8)
		{
			NUMBER_CLEAR(8);
			NUMBER_1(8);
		}

		break;

	case 2:
		if (position == 1)
		{
			NUMBER_CLEAR(1);
			NUMBER_2(1);
		}
		else if (position == 2)
		{
			NUMBER_CLEAR(2);
			NUMBER_2(2);
		}
		else if (position == 3)
		{
			NUMBER_CLEAR(3);
			NUMBER_2(3);
		}
		else if (position == 4)
		{
			NUMBER_CLEAR(4);
			NUMBER_2(4);
		}
		else if (position == 5)
		{
			NUMBER_CLEAR(5);
			NUMBER_2(5);
		}
		else if (position == 6)
		{
			NUMBER_CLEAR(6);
			NUMBER_2(6);
		}
		else if (position == 7)
		{
			NUMBER_CLEAR(7);
			NUMBER_2(7);
		}
		else if (position == 8)
		{
			NUMBER_CLEAR(8);
			NUMBER_2(8);
		}
		break;

	case 3:
		if (position == 1)
		{
			NUMBER_CLEAR(1);
			NUMBER_3(1);
		}
		else if (position == 2)
		{
			NUMBER_CLEAR(2);
			NUMBER_3(2);
		}
		else if (position == 3)
		{
			NUMBER_CLEAR(3);
			NUMBER_3(3);
		}
		else if (position == 4)
		{
			NUMBER_CLEAR(4);
			NUMBER_3(4);
		}
		else if (position == 5)
		{
			NUMBER_CLEAR(5);
			NUMBER_3(5);
		}
		else if (position == 6)
		{
			NUMBER_CLEAR(6);
			NUMBER_3(6);
		}
		else if (position == 7)
		{
			NUMBER_CLEAR(7);
			NUMBER_3(7);
		}
		else if (position == 8)
		{
			NUMBER_CLEAR(8);
			NUMBER_3(8);
		}
		break;

	case 4:
		if (position == 1)
		{
			NUMBER_CLEAR(1);
			NUMBER_4(1);
		}
		else if (position == 2)
		{
			NUMBER_CLEAR(2);
			NUMBER_4(2);
		}
		else if (position == 3)
		{
			NUMBER_CLEAR(3);
			NUMBER_4(3);
		}
		else if (position == 4)
		{
			NUMBER_CLEAR(4);
			NUMBER_4(4);
		}
		else if (position == 5)
		{
			NUMBER_CLEAR(5);
			NUMBER_4(5);
		}
		else if (position == 6)
		{
			NUMBER_CLEAR(6);
			NUMBER_4(6);
		}
		else if (position == 7)
		{
			NUMBER_CLEAR(7);
			NUMBER_4(7);
		}
		else if (position == 8)
		{
			NUMBER_CLEAR(8);
			NUMBER_4(8);
		}
		break;

	case 5:
		if (position == 1)
		{
			NUMBER_CLEAR(1);
			NUMBER_5(1);
		}
		else if (position == 2)
		{
			NUMBER_CLEAR(2);
			NUMBER_5(2);
		}
		else if (position == 3)
		{
			NUMBER_CLEAR(3);
			NUMBER_5(3);
		}
		else if (position == 4)
		{
			NUMBER_CLEAR(4);
			NUMBER_5(4);
		}
		else if (position == 5)
		{
			NUMBER_CLEAR(5);
			NUMBER_5(5);
		}
		else if (position == 6)
		{
			NUMBER_CLEAR(6);
			NUMBER_5(6);
		}
		else if (position == 7)
		{
			NUMBER_CLEAR(7);
			NUMBER_5(7);
		}
		else if (position == 8)
		{
			NUMBER_CLEAR(8);
			NUMBER_5(8);
		}
		break;

	case 6:
		if (position == 1)
		{
			NUMBER_CLEAR(1);
			NUMBER_6(1);
		}
		else if (position == 2)
		{
			NUMBER_CLEAR(2);
			NUMBER_6(2);
		}
		else if (position == 3)
		{
			NUMBER_CLEAR(3);
			NUMBER_6(3);
		}
		else if (position == 4)
		{
			NUMBER_CLEAR(4);
			NUMBER_6(4);
		}
		else if (position == 5)
		{
			NUMBER_CLEAR(5);
			NUMBER_6(5);
		}
		else if (position == 6)
		{
			NUMBER_CLEAR(6);
			NUMBER_6(6);
		}
		else if (position == 7)
		{
			NUMBER_CLEAR(7);
			NUMBER_6(7);
		}
		else if (position == 8)
		{
			NUMBER_CLEAR(8);
			NUMBER_6(8);
		}
		break;

	case 7:
		if (position == 1)
		{
			NUMBER_CLEAR(1);
			NUMBER_7(1);
		}
		else if (position == 2)
		{
			NUMBER_CLEAR(2);
			NUMBER_7(2);
		}
		else if (position == 3)
		{
			NUMBER_CLEAR(3);
			NUMBER_7(3);
		}
		else if (position == 4)
		{
			NUMBER_CLEAR(4);
			NUMBER_7(4);
		}
		else if (position == 5)
		{
			NUMBER_CLEAR(5);
			NUMBER_7(5);
		}
		else if (position == 6)
		{
			NUMBER_CLEAR(6);
			NUMBER_7(6);
		}
		else if (position == 7)
		{
			NUMBER_CLEAR(7);
			NUMBER_7(7);
		}
		else if (position == 8)
		{
			NUMBER_CLEAR(8);
			NUMBER_7(8);
		}
		break;

	case 8:
		if (position == 1)
		{
			NUMBER_CLEAR(1);
			NUMBER_8(1);
		}
		else if (position == 2)
		{
			NUMBER_CLEAR(2);
			NUMBER_8(2);
		}
		else if (position == 3)
		{
			NUMBER_CLEAR(3);
			NUMBER_8(3);
		}
		else if (position == 4)
		{
			NUMBER_CLEAR(4);
			NUMBER_8(4);
		}
		else if (position == 5)
		{
			NUMBER_CLEAR(5);
			NUMBER_8(5);
		}
		else if (position == 6)
		{
			NUMBER_CLEAR(6);
			NUMBER_8(6);
		}
		else if (position == 7)
		{
			NUMBER_CLEAR(7);
			NUMBER_8(7);
		}
		else if (position == 8)
		{
			NUMBER_CLEAR(8);
			NUMBER_8(8);
		}
		break;

	case 9:
		if (position == 1)
		{
			NUMBER_CLEAR(1);
			NUMBER_9(1);
		}
		else if (position == 2)
		{
			NUMBER_CLEAR(2);
			NUMBER_9(2);
		}
		else if (position == 3)
		{
			NUMBER_CLEAR(3);
			NUMBER_9(3);
		}
		else if (position == 4)
		{
			NUMBER_CLEAR(4);
			NUMBER_9(4);
		}
		else if (position == 5)
		{
			NUMBER_CLEAR(5);
			NUMBER_9(5);
		}
		else if (position == 6)
		{
			NUMBER_CLEAR(6);
			NUMBER_9(6);
		}
		else if (position == 7)
		{
			NUMBER_CLEAR(7);
			NUMBER_9(7);
		}
		else if (position == 8)
		{
			NUMBER_CLEAR(8);
			NUMBER_9(8);
		}

		break;
	}
}

void tempValueDisplay(INT16 value, BOOL fillZero)
{
	INT8 forthNumber = (INT8)(value % 10);
	INT8 thirdNumber = (INT8)((value / 10) % 10);
	INT8 secondNumber = (INT8)((value / 100) % 10);
	INT8 firstNumber = (INT8)(value / 1000);
	
	NUMBER_CLEAR(1);
	NUMBER_CLEAR(2);
	NUMBER_CLEAR(3);
	
	if (firstNumber == 1)
		LCD->X9 = 1;
	else
		LCD->X9 = 0;

	if (firstNumber == 1 || (firstNumber == 0 && secondNumber != 0))
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
	LCD->DP1 = 1;
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

void buzzerCMD(BOOL state)
{
	if (state) // MUTE OFF
	{
		LCD->X3 = 1;
		LCD->X4 = 0;
	}

	else // MUTE ON
	{
		LCD->X3 = 0;
		LCD->X4 = 1;
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
