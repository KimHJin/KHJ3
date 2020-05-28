///////////////////////////////////////////////////////////////////////////////
// Code Name : IWON TEMP FUNC
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_ENV.h"
#include "IWON_FUNC.h"


// 생성자
IWON_TEMP_FUNC::IWON_TEMP_FUNC()
{
	Init();  
}
// 소멸자
IWON_TEMP_FUNC::~IWON_TEMP_FUNC()
{

}

VOID IWON_TEMP_FUNC::Init(VOID)
{
	LowBatteryFlag = 0;
	Measure_test_flag = 0;
	LastMeasred = 0;  
	LowHigh_FLag = false;

	AutoCalDelayCount = 0;
}

VOID IWON_TEMP_FUNC::Beep(INT16 length)
{
	if (!buzzerState_p) return;
	for (INT16 i = 0; i < length; i++)
	{
		GPIO_HIGH(GPIOD, GPIO_Pin_5);
		Delay_10us(20);
		GPIO_LOW(GPIOD, GPIO_Pin_5);
		Delay_10us(20);
	}
}

VOID IWON_TEMP_FUNC::Beep(VOID)
{
	Beep(300);
}

VOID IWON_TEMP_FUNC::BeepMode(INT16 mode)
{
	if (mode == HIGH_FEVER)	// 고열
	{
		Beep(2000);
		Delay_ms(400);
		Beep(2000);
		Delay_ms(400);
		Beep(2000);
	}
	else if (mode == LIGHT_FEVER)	// 미열
	{
		Beep(1000);
		Delay_ms(400);
		Beep(1000);
	}
	else if (mode == NORMAL)	// 정상
	{
		Beep();
	}
}


VOID IWON_TEMP_FUNC::LCD_clear(VOID)
{
	for (INT8 i = 0; i < 14; i++)
	{
		LCD->RAM[i] = 0;
	}
}

VOID IWON_TEMP_FUNC::POWER_DOWN(VOID)
{
	DisplayOFF();
	Delay_ms(1000);

	LCD_clear();
	while(1)	// 무조건 무한루프 돌면서 파워를 끈다.
	{
		GPIO_LOW(GPIOD, GPIO_Pin_7);
	}
}


VOID IWON_TEMP_FUNC::DisplayRGB(INT8 color)
{
	GPIO_HIGH(GPIOD, GPIO_Pin_4); // RED
	GPIO_HIGH(GPIOF, GPIO_Pin_0); // GREEN
	GPIO_HIGH(GPIOB, GPIO_Pin_7); // BLUE

	switch(color)
	{
		case RED:
			GPIO_LOW(GPIOD, GPIO_Pin_4);  // RED
			break;
		case GREEN:
			GPIO_LOW(GPIOF, GPIO_Pin_0);  // GREEN
			break;
		case BLUE:
			GPIO_LOW(GPIOB, GPIO_Pin_7);  // BLUE
			break;
		case YELLOW:
			GPIO_LOW(GPIOD, GPIO_Pin_4); // RED  
			GPIO_LOW(GPIOF, GPIO_Pin_0); // GREEN
			break;
		case CYAN:
			GPIO_LOW(GPIOB, GPIO_Pin_7);  // BLUE
			GPIO_LOW(GPIOF, GPIO_Pin_0);  // GREEN
			break;
		case MAGENTA:
			GPIO_LOW(GPIOD, GPIO_Pin_4);  // RED  
			GPIO_LOW(GPIOB, GPIO_Pin_7);  // BLUE
			break;
		case WHITE:
			GPIO_LOW(GPIOD, GPIO_Pin_4);  // RED  
			GPIO_LOW(GPIOF, GPIO_Pin_0);  // GREEN
			GPIO_LOW(GPIOB, GPIO_Pin_7);  // BLUE
			break;
	}
}

VOID IWON_TEMP_FUNC::DisplayLOW(VOID)
{
	LCD->X9 = 0;

	ClearDisp();

	LCD->F1 = 1; // L
	LCD->E1 = 1;
	LCD->D1 = 1;

	LCD->C2 = 1; // O
	LCD->D2 = 1;
	LCD->E2 = 1;
	LCD->G2 = 1;
	LowHigh_FLag = true;
}

VOID IWON_TEMP_FUNC::DisplayHIGH(VOID)
{
	LCD->X9 = 0;

	ClearDisp();

	LCD->F1 = 1; // H
	LCD->E1 = 1;
	LCD->G1 = 1;
	LCD->G1 = 1;
	LCD->B1 = 1;
	LCD->C1 = 1;

	LCD->F2 = 1; // I
	LCD->E2 = 1;
	LowHigh_FLag = true;
}

VOID IWON_TEMP_FUNC::DisplayError(VOID)
{
	ClearDisp();

	LCD->A1 = 1; // E
	LCD->D1 = 1;
	LCD->E1 = 1;
	LCD->F1 = 1;
	LCD->G1 = 1;

	LCD->G2 = 1; //r
	LCD->E2 = 1;

	LCD->G3 = 1; //r
	LCD->E3 = 1;
}


// LCD 에 oFF 표시
VOID IWON_TEMP_FUNC::DisplayOFF(VOID)
{
	DisplayRGB(CLEAR);
	LCD_clear();

	LCD->C1 = 1;// o
	LCD->D1 = 1;
	LCD->E1 = 1;
	LCD->G1 = 1;

	LCD->A2 = 1;// F
	LCD->E2 = 1;
	LCD->F2 = 1;
	LCD->G2 = 1;

	LCD->A3 = 1;// F
	LCD->E3 = 1;
	LCD->F3 = 1;
	LCD->G3 = 1;
}

VOID IWON_TEMP_FUNC::Delay_10us(INT16 us)
{
	us *= 16;
	for (INT16 i = 0; i < us; i++)
	{
		us = us;
	}
}
VOID IWON_TEMP_FUNC::Delay_ms(INT16 ms)
{
	for (INT16 i = 0; i < ms; i++)
	{
		Delay_10us(100);
	}
}

INT8 IWON_TEMP_FUNC::Delay_10us_with_key(INT16 us)
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
INT8 IWON_TEMP_FUNC::Delay_ms_with_key(INT16 ms)
{
  	INT8 key = 0;
	for (INT16 i = 0; i < ms; i++)
	{
		key = Delay_10us_with_key(100);
		if(key!=0)
		{
			return key;
		}
	}
	return 0;
}

INT16 IWON_TEMP_FUNC::UnitCalc(INT16 temp, BOOL unit)
{
	INT16 value = temp;
	if (!unit)
	{
		value = (INT16)((((float)temp / 10) * 1.8 + 32) * 10);
	}

	return value;
}


// 배터리 2.0 볼트 이하 , 경고음과 함께 전원을 끈다.
VOID IWON_TEMP_FUNC::LowBatteryDisplay_2v0(VOID)
{
	LowBatteryFlag = 1;
	DisplayOFF();
	BeepMode(LIGHT_FEVER);
	POWER_DOWN();
}

// 배터리 2.2 볼트 이하 , 저전압 배터리 아이콘을 깜빡이게 한다.
VOID IWON_TEMP_FUNC::LowBatteryDisplay_2v2(VOID)
{
	LowBatteryFlag = 1;
}

// 배터리 2.4 볼트 이하 , 저전압 배터리 아이콘을 켠다.
VOID IWON_TEMP_FUNC::LowBatteryDisplay_2v4(VOID)
{
	LCD->X5 = 1;
}



VOID IWON_TEMP_FUNC::TempLogDataTask(VOID)
{
	memNumber_p--;

	if (memNumber_p < 0)
		memNumber_p = 32;

	memNumberDisplay(memNumber_p);
	memTempDataDisplay(UnitCalc(__EEPROM->memTempData[memNumber_p - 1], tempUnit_p));
}



VOID IWON_TEMP_FUNC::BuzzerStateTask(VOID)
{
	buzzerState_p ^= 1;

	BuzzerCMD(buzzerState_p);
}

VOID IWON_TEMP_FUNC::MeasureModeTask(VOID)
{
	measureMode_p ^= 1;

	measureModeSet(measureMode_p);

	if (measureMode_p)
		DisplayRGB(BLUE);
	else
		DisplayRGB(GREEN);
}

INT8 IWON_TEMP_FUNC::TempUnitTask(BOOL inv)
{
	INT8 r = 0;

	if(inv)
	{
		tempUnit_p ^= 1;
	}

	tempUnitSet(tempUnit_p);
	if (LowHigh_FLag == false) 
	{
		if(LastMeasred==1)
		{
			TempValueDisplay(UnitCalc(TEMP, tempUnit_p));
			r = 1;
		}
	}
	memTempDataDisplay(UnitCalc(__EEPROM->memTempData[memNumber_p - 1], tempUnit_p));	
	return r;
}


/*************************************************************************************/
/*****************************   SPECIAL MODE   **************************************/
/*************************************************************************************/



VOID IWON_TEMP_FUNC::SpecialModeDisp(int16_t value)
{
    INT16 temp = 0;
  
	if(value < 0) 
		temp = -1 * value;
	else 
		temp = value;

	INT16 secondNumber = temp % 10;
	INT16 firstNumber = temp / 10;
	
	if(value < 0) 
		LCD->G1 = 1;
	else 
		LCD->G1 = 0;
	
	displayNumber(firstNumber, 2);
	displayNumber(secondNumber, 3);
}

VOID IWON_TEMP_FUNC::CaliDone(IWON_TEMP_TASK *IWonTask)
{
    IWonTask->Set_AdjValue(caliData_p); // <= 이 값을 저장하고 읽어서 여기에 적용 하세요.

	LCD_clear();

	TempValueDisplay(0);

	memNumberDisplay(memNumber_p);
	memTempDataDisplay(UnitCalc(__EEPROM->memTempData[memNumber_p - 1], tempUnit_p));
	measureModeSet(measureMode_p);
	BuzzerCMD(buzzerState_p);
	tempUnitSet(tempUnit_p);

	LCD->X8 = 1; // Display "LOG"

	if (measureMode_p)
		DisplayRGB(BLUE);
	else
		DisplayRGB(GREEN);	
}

VOID IWON_TEMP_FUNC::SpecialMode(IWON_TEMP_TASK *IWonTask)
{
	IWonTask->ClearPowerDown();

	if (SW_LEFT_ON)
	{
		Delay_ms(400);
		caliData_p--;
		
		if(caliData_p < -99)
			caliData_p = -99;
		  
		SpecialModeDisp(caliData_p);
	}	
	else 
	if (SW_RIGHT_ON)
	{
		Delay_ms(400);		
		caliData_p++;
		
		if(caliData_p > 99) 
	    	caliData_p = 99;

		SpecialModeDisp(caliData_p);
	}
}

VOID IWON_TEMP_FUNC::SpecialModeTask(IWON_TEMP_TASK *IWonTask)
{
	INT16 Count = 0;

	// 수동 보정모드에서는 바탕색을 CYAN (하늘색 비슷) 으로 한다.
    DisplayRGB(CYAN);
	LCD_clear();
	
	SpecialModeDisp(caliData_p);
	
	while(SW_LEFT_ON || SW_RIGHT_ON);

	while (!SW_PWR_ON)
	{
		SpecialMode(IWonTask);
	}

	BeepMode(NORMAL);

	// 수동보정모드를 빠져 나올때
	while(SW_PWR_ON)
	{
		Count++;
		Delay_ms(15);

		// 측정버튼을 4초 이상 누르고 있으면 모든 저장된 내용을 지우고 오토 캘리브레이션 모드로 진입할 수 있는 상태로 설정된다.
		if(Count == 350)
		{
			BeepMode(LIGHT_FEVER);
			ALLCLEAR();
			POWER_DOWN();
		}
	}
	
	CaliDone(IWonTask);
	Delay_ms(500);
}


VOID IWON_TEMP_FUNC::TempLogDataSave(int16_t saveData)
{
	for (INT8 i = 0; i < 31; i++)
	{
		memTemp_p(i) = memTemp_p(i + 1);
	}

	memTemp_p(31) = saveData;
}
VOID IWON_TEMP_FUNC::TempLogDataClear(VOID)
{
	for (INT8 i = 0; i < 32; i++)
	{
		memTemp_p(i) = 0;
	}
}


VOID IWON_TEMP_FUNC::SaveTemp(INT16 temp)
{
	LastMeasred = 1;

	TEMP = temp;

	memNumber_p = 32;

	TempLogDataSave(temp);
	memNumberDisplay(memNumber_p);
	memTempDataDisplay(UnitCalc(memTemp_p(memNumber_p - 1), tempUnit_p));
}

VOID IWON_TEMP_FUNC::ObjTempDisp(INT16 temp)
{
	Beep();								
	DisplayRGB(GREEN);
	
	if(temp < 0) // 사물 온도 모드에서 0도 미만
	{
		DisplayLOW();
	}
	else 
	if(temp > 850) // 사물 온도 모드에서 85도 초과
	{
		DisplayHIGH();
	}
	else
	{
		TempValueDisplay(UnitCalc(temp, tempUnit_p));
	
		SaveTemp(temp);
	}
}
VOID IWON_TEMP_FUNC::BdyTempDisp(INT16 temp)
{
	if (temp >= 381 && temp <= 425)
	{ // HIGH FEVER
		DisplayRGB(RED);
		TempValueDisplay(UnitCalc(temp, tempUnit_p)); // temp Display
		BeepMode(HIGH_FEVER);
	}
	else if (temp >= 371 && temp <= 380)
	{ // LIGHT FEVER
		DisplayRGB(YELLOW);
		TempValueDisplay(UnitCalc(temp, tempUnit_p)); // temp Display
		BeepMode(LIGHT_FEVER);
	}
	else if (temp >= 334 && temp <= 370)
	{ // NORMAL
		DisplayRGB(BLUE);
		TempValueDisplay(UnitCalc(temp, tempUnit_p)); // temp Display
		BeepMode(NORMAL);
	}

	SaveTemp(temp);
}


VOID IWON_TEMP_FUNC::SystemError(VOID)
{
	DisplayRGB(RED);
	DisplayError();

	Delay_ms(50);
	Beep(1000);
	Delay_ms(40);
	Beep(1000);
	Delay_ms(40);
	Beep(1000);
	Delay_ms(40);
	
	while(!SW_PWR_ON);
	
	POWER_DOWN();
}

VOID IWON_TEMP_FUNC::MeasuringDisp(VOID)
{  
  	LCD->X9 = 0;	// 맨 앞의 1 자리 꺼지는것
	  
	ClearDisp();

	// - - - 표시
	LCD->G1 = 1;
	LCD->G2 = 1;
	LCD->G3 = 1;
}

VOID IWON_TEMP_FUNC::ALLCLEAR(VOID)
{
	caliData_p     = 0; // 수동 보정값 0 으로 저장   
	TEMP           = 0;   
	memNumber_p    = 0;  
	measureMode_p  = 0;	// 사물 측정 모드  
	buzzerState_p  = 1;	// BUZZER ON
	tempUnit_p 	   = 1;	// 섭씨 모드
	AutoCaliFlag_p = 0;
	//offSetVolt_p   = 0;	// 자동 보정값 0 으로 저장
	
	TempLogDataClear();
}

VOID IWON_TEMP_FUNC::ClearDisp(VOID)
{
	ALL_NUMBER_CLEAR();
	LCD->DP1 = 0;
}

VOID IWON_TEMP_FUNC::SuccessDisp(VOID)
{
	ClearDisp();

	LCD->A1 = 1; // S
	LCD->C1 = 1;
	LCD->D1 = 1;
	LCD->F1 = 1;
	LCD->G1 = 1;
	
	LCD->C2 = 1; // u
	LCD->D2 = 1;
	LCD->E2 = 1;
}

VOID IWON_TEMP_FUNC::FailDisp(VOID)
{
	ClearDisp();
		
	LCD->A1 = 1; // F
	LCD->E1 = 1;
	LCD->F1 = 1;
	LCD->G1 = 1;

	LCD->A2 = 1; // A
	LCD->B2 = 1;
	LCD->C2 = 1;
	LCD->E2 = 1;
	LCD->F2 = 1;
	LCD->G2 = 1;
}

VOID IWON_TEMP_FUNC::OkDisp(VOID)
{
	ClearDisp();

	LCD->A2 = 1; // O
	LCD->B2 = 1;
	LCD->C2 = 1;
	LCD->D2 = 1;
	LCD->E2 = 1;
	LCD->F2 = 1;

	LCD->A3 = 1; // ㅋ
	LCD->B3 = 1;
	LCD->C3 = 1;
	LCD->G3 = 1;
/*
	LCD->A3 = 1; // !
	LCD->B3 = 1;
	LCD->D3 = 1;
	LCD->F3 = 1;
	LCD->G3 = 1;	
*/
}

VOID IWON_TEMP_FUNC::VerDisp(VOID)
{
	TempValueDisplay(DEFINED_FW_VER, false);		// <= 펌웨어 버전 표시

	LCD->C1 = 1; // (V)ER
	LCD->D1 = 1;
	LCD->E1 = 1;
}


VOID IWON_TEMP_FUNC::BuzzerCMD(BOOL state)
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

VOID IWON_TEMP_FUNC::TempValueDisplay(INT16 value, BOOL fillZero)
{
	tempValueDisplay(value, fillZero);
}
VOID IWON_TEMP_FUNC::TempValueDisplay(INT16 value)
{
	TempValueDisplay(value, true);
}

