///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEMP TEST
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_TEST.h"


void displayRGB(int color);
void LCD_clear();


// 생성자
IWON_TEMP_TEST::IWON_TEMP_TEST()
{
	Init();  
}
// 소멸자
IWON_TEMP_TEST::~IWON_TEMP_TEST()
{
  
}

VOID IWON_TEMP_TEST::Init(VOID)
{
	TestCount = 0;
	TestModeFlag = 0;
}


INT8 IWON_TEMP_TEST::GetTestCount(VOID)
{
	return TestCount;
}
INT8 IWON_TEMP_TEST::GetTestModeFlag(VOID)
{
	return TestModeFlag;
}
VOID IWON_TEMP_TEST::SetTestCount(INT8 testCount)
{
	TestCount = testCount;
}
VOID IWON_TEMP_TEST::SetTestModeFlag(INT8 testModeFlag)
{
	TestModeFlag = testModeFlag;
}
INT8 IWON_TEMP_TEST::IncTestCount()
{
	TestCount++;
	return TestCount;
}
INT8 IWON_TEMP_TEST::DecTestCount()
{
	TestCount--;
	if(TestCount<0) TestCount = 0;
	return TestCount;
}



INT8 IWON_TEMP_TEST::delay_10us_with_key(INT16 us)
{
	us *= 16;
	for (INT16 i = 0; i < us; i++)
	{
		us = us;
		if(SW_LEFT_ON) return 1;
		if(SW_RIGHT_ON) return 2;
		if(SW_PWR_ON) return 3;
	}
	return 0;
}
INT8 IWON_TEMP_TEST::delay_ms_with_key(INT16 ms)
{
  	INT8 key = 0;
	for (INT16 i = 0; i < ms; i++)
	{
		key = delay_10us_with_key(100);
		if(key!=0)
		{
			return key;
		}
	}
	return 0;
}



VOID IWON_TEMP_TEST::VDD_Test(VOID)
{
    displayRGB(CLEAR);
	LCD_clear();
    memTempDataDisplay(10);
  	tempValueDisplay(VDDmV/100, false);

	if(VDDmV >= 3000 && VDDmV <= 3300)
	{
		displayRGB(GREEN);
	}
	else 	
	{
		displayRGB(RED);
	}	
}

VOID IWON_TEMP_TEST::BAT_Test(VOID)
{  
    displayRGB(CLEAR);
	LCD_clear();
    memTempDataDisplay(20);
	tempValueDisplay(BATmV/100, false);

	if(BATmV >= 2600 && BATmV <= 3000)
	{
		displayRGB(GREEN);
	}
	else 	
	{
		displayRGB(RED);
	}
}

INT8 IWON_TEMP_TEST::LCD_Test(VOID)
{
	displayRGB(CLEAR);
	LCD_clear();

	INT8 key = 0;
	while(key==0) 
	{	  
		IWonTask->ClearPowerDown();
	  
		for(INT8 i=0; i<14; i++)	// 모든 아이콘 표시
		  LCD->RAM[i] = 0xFF;		
		
		key = delay_ms_with_key(100);
		if(key!=0) break;
		
		LCD_clear();		
		key = delay_ms_with_key(100);
	}
	
	LCD_clear();		  		
	return key;
}

INT8 IWON_TEMP_TEST::BackLight_Test(VOID)
{
    LCD_clear();
    memTempDataDisplay(40);

	INT8 key = 0;
	while(key==0) 
	{	  
		IWonTask->ClearPowerDown();
	  
		displayRGB(RED);
		key = delay_ms_with_key(100);
		if(key!=0) break;

		displayRGB(GREEN);
		key = delay_ms_with_key(100);
		if(key!=0) break;

		displayRGB(BLUE);
		key = delay_ms_with_key(100);
		if(key!=0) break;

		displayRGB(CLEAR);	
		key = delay_ms_with_key(100);
		if(key!=0) break;
	}	
	displayRGB(CLEAR);
	
	return key;
}

