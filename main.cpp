///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEMP - NON CONTACT PROJECT
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY & KHJ
///////////////////////////////////////////////////////////////////////////////

#include "stm8l15x.h"
#include "lcd_driver.h"

#include "IWON_TASK.h"

// 아이원 온도계 테스크 클래스
IWON_TEMP_TASK *IWonTask = NULL;

#define SW_PWR_ON !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
#define SW_LEFT_ON !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6)
#define SW_RIGHT_ON !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)

int yellowFlag = 0;
int count = 0;

/************************************************************************/
/**
  * 1 msec 마다 호출되는 인터럽트이므로 응용해서 사용해도 좋다.
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler, 25)
{
	IWonTask->Time();
	if (yellowFlag)
		IWonTask->YellowDisp();

	//
	// 필요한 경우 여기에 코드를 넣어서 사용하세요.
	//

	// Clear TIM4 interrupt
	TIM4_ClearITPendingBit(TIM4_IT_Update);
}
/************************************************************************/

#define HIGH_FEVER 1
#define LIGHT_FEVER 2
#define NORMAL 3

#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define CLEAR 5

void LCD_clear()
{
	for (int i = 0; i < 14; i++)
	{
		LCD->RAM[i] = 0;
	}
}

void displayRGB(int color)
{
	if (color == RED)
	{
		GPIO_LOW(GPIOD, GPIO_Pin_4);
		GPIO_HIGH(GPIOF, GPIO_Pin_0);
		GPIO_HIGH(GPIOB, GPIO_Pin_7);
		yellowFlag = 0;
	}

	else if (color == GREEN)
	{
		GPIO_HIGH(GPIOD, GPIO_Pin_4);
		GPIO_LOW(GPIOF, GPIO_Pin_0);
		GPIO_HIGH(GPIOB, GPIO_Pin_7);
		yellowFlag = 0;
	}

	else if (color == BLUE)
	{
		GPIO_HIGH(GPIOD, GPIO_Pin_4);
		GPIO_HIGH(GPIOF, GPIO_Pin_0);
		GPIO_LOW(GPIOB, GPIO_Pin_7);
		yellowFlag = 0;
	}

	else if (color == YELLOW)
	{
		GPIO_HIGH(GPIOD, GPIO_Pin_4);
		GPIO_HIGH(GPIOF, GPIO_Pin_0);
		GPIO_HIGH(GPIOB, GPIO_Pin_7);
		yellowFlag = 1;
	}

	else if (color == CLEAR)
	{
		GPIO_HIGH(GPIOD, GPIO_Pin_4);
		GPIO_HIGH(GPIOF, GPIO_Pin_0);
		GPIO_HIGH(GPIOB, GPIO_Pin_7);
		yellowFlag = 0;
	}
}

void displayLOW(void)
{
	LCD->X9 = 0;
	LCD->DP1 = 0;
	NUMBER_CLEAR(1);
	NUMBER_CLEAR(2);
	NUMBER_CLEAR(3);

	LCD->F1 = 1; // L
	LCD->E1 = 1;
	LCD->D1 = 1;

	LCD->C2 = 1; // O
	LCD->D2 = 1;
	LCD->E2 = 1;
	LCD->G2 = 1;
}

void displayHIGH(void)
{

	LCD->X9 = 0;
	LCD->DP1 = 0;
	NUMBER_CLEAR(1);
	NUMBER_CLEAR(2);
	NUMBER_CLEAR(3);

	LCD->F1 = 1; // H
	LCD->E1 = 1;
	LCD->G1 = 1;
	LCD->G1 = 1;
	LCD->B1 = 1;
	LCD->C1 = 1;

	LCD->F2 = 1; // I
	LCD->E2 = 1;
}

void displayError(void)
{
	NUMBER_CLEAR(1);
	NUMBER_CLEAR(2);
	NUMBER_CLEAR(3);

	LCD->A1 = 1; // E
	LCD->D1 = 1;
	LCD->E1 = 1;
	LCD->F1 = 1;
	LCD->G1 = 1;

	LCD->G2 = 1; //r
	LCD->E2 = 1;

	LCD->G3 = 1; //r
	LCD->E3 = 1;

	LCD->DP1 = 0;
}

/************************************************************************/

void delay_10us(int us)
{
	us *= 16;
	for (int i = 0; i < us; i++)
	{
		us = us;
	}
}
void delay_ms(int ms)
{
	for (int i = 0; i < ms; i++)
	{
		delay_10us(100);
	}
}
/************************************************************************/

void Beep(int length)
{
	if (buzzerState_p)
		for (int i = 0; i < length; i++)
		{
			GPIO_HIGH(GPIOD, GPIO_Pin_5);
			delay_10us(20);
			GPIO_LOW(GPIOD, GPIO_Pin_5);
			delay_10us(20);
		}
}
void Beep()
{
	Beep(500);
}

void GPIO_init()
{
	GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast); //RED
	GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast); //GREEN
	GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //BLUE

	GPIO_Init(GPIOC, GPIO_Pin_5, GPIO_Mode_In_FL_No_IT); //SW_PWR
	GPIO_Init(GPIOE, GPIO_Pin_6, GPIO_Mode_In_FL_No_IT); //SW_LEFT
	GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT); //SW_RIGHT

	GPIO_Init(GPIOD, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Fast); //BUZZER

	GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //PWR_STATE

	GPIO_HIGH(GPIOD, GPIO_Pin_7);
}

int16_t unitCalc(int16_t temp, int unit)
{
	int16_t value = temp;
	if (!unit)
	{
		value = (int16_t)((((float)temp / 10) * 1.8 + 32) * 10);
	}

	return value;
}

/*********************************************/

void tempLogDataTask()
{
	memNumber_p++;

	if (memNumber_p > 32)
		memNumber_p = 1;

	memNumberDisplay(memNumber_p);
	memTempDataDisplay(memNumber_p, unitCalc(__EEPROM->memTempData[memNumber_p - 1], tempUnit_p));
}

void buzzerStateTask()
{
	buzzerState_p ^= 1;

	buzzerCMD(buzzerState_p);
}

void measureModeTask()
{
	measureMode_p ^= 1;

	measureModeSet(measureMode_p);

	if (measureMode_p)
		displayRGB(BLUE);
	else
		displayRGB(GREEN);
}

void tempUnitTask()
{
	tempUnit_p ^= 1;

	tempUnitSet(tempUnit_p);

	if ((334 <= TEMP && TEMP <= 425) || measureMode_p == 0)
		tempValueDisplay(unitCalc(TEMP, tempUnit_p));

	memTempDataDisplay(memNumber_p, unitCalc(__EEPROM->memTempData[memNumber_p - 1], tempUnit_p));
}

/*********************************************/

void specialModeDisp()
{
	int forthNumber = caliData_p % 10;
	int thirdNumber = (caliData_p / 10) % 10;
	int secondNumber = (caliData_p / 100) % 10;

	NUMBER_CLEAR(1);

	if (!PLUS_MINUS && caliData_p != 0)
	{
		NUMBER_CLEAR(1);
		LCD->G1 = 1;
	}
	else
		displayNumber(secondNumber, 1);
	displayNumber(thirdNumber, 2);
	displayNumber(forthNumber, 3);
}

void specialMode()
{

	if (SW_LEFT_ON)
	{
		delay_ms(400);
		displayRGB(RED);

		if (caliData_p == 0)
			PLUS_MINUS = 0;

		if (!PLUS_MINUS)
		{
			caliData_p++;
			if (caliData_p > 99)
				caliData_p = 99;
		}
		else
		{
			caliData_p--;
			if (caliData_p <= 0)
				PLUS_MINUS = 0;
		}

		specialModeDisp();
	}

	else if (SW_RIGHT_ON)
	{
		delay_ms(400);
		displayRGB(BLUE);

		if (caliData_p == 0)
			PLUS_MINUS = 1;

		if (PLUS_MINUS)
		{
			caliData_p++;
			if (caliData_p > 99)
				caliData_p = 99;
		}
		if (!PLUS_MINUS)
		{
			caliData_p--;

			if (caliData_p >= 0)
				PLUS_MINUS = 1;
		}

		specialModeDisp();
	}
}

void caliDone()
{

	int16_t value = caliData_p;

	if (!PLUS_MINUS)
		value = -1 * caliData_p;
	else
		value = caliData_p;

	IWonTask->Set_AdjValue(value); // <= 이 값을 저장하고 읽어서 여기에 적용 하세요.

	LCD_clear();

	displayNumber(0, 1);
	displayNumber(0, 2);
	displayNumber(0, 3);

	memNumberDisplay(memNumber_p);
	memTempDataDisplay(memNumber_p, unitCalc(__EEPROM->memTempData[memNumber_p - 1], tempUnit_p));
	measureModeSet(measureMode_p);
	buzzerCMD(buzzerState_p);
	tempUnitSet(tempUnit_p);

	LCD->X8 = 1; // Display "LOG"

	if (measureMode_p)
		displayRGB(BLUE);
	else
		displayRGB(GREEN);
}

void keyScan()
{
	if (SW_LEFT_ON) // SW_LEFT
	{
		IWonTask->ClearPowerDown();

		delay_ms(15);
		int delayCount = 0;

		while (SW_LEFT_ON)
		{

			delayCount++;
			delay_ms(15);

			if (delayCount == 350)
			{
				Beep();
				delay_ms(40);

				displayRGB(CLEAR);
				LCD_clear();

				while (!SW_PWR_ON)
				{
					specialMode();
				}

				caliDone();
			}

			if (delayCount == 60 && !SW_RIGHT_ON) // LONG_PRESS
			{
				Beep();
				measureModeTask(); // measure mode Set
			}
		}

		if (delayCount < 60) // SHORT_PRESS
		{
			Beep();
			tempLogDataTask(); // memory Data
		}
		delay_ms(10);
	}

	if (SW_RIGHT_ON) // SW_RIGHT
	{
		IWonTask->ClearPowerDown();

		delay_ms(15);
		int delayCount = 0;

		while (SW_RIGHT_ON)
		{

			delayCount++;
			delay_ms(15);

			if (delayCount == 350)
			{
				Beep();
				delay_ms(40);

				displayRGB(CLEAR);
				LCD_clear();

				while (!SW_PWR_ON)
				{
					specialMode();
				}

				caliDone();
			}

			if (delayCount == 60 && !SW_LEFT_ON) // LONG_PRESS
			{
				Beep();
				tempUnitTask(); // temp Unit set
			}
		}

		if (delayCount < 60) // SHORT_PRESS
		{
			buzzerStateTask(); // buzzer On / Off
			Beep();
		}
		delay_ms(10);
	}
}

void tempLogDataSave(int16_t saveData)
{

	for (int i = 0; i < 31; i++)
	{

		__EEPROM->memTempData[i] = __EEPROM->memTempData[i + 1];
	}

	__EEPROM->memTempData[31] = saveData;
}

void BeepMode(int mode)
{

	if (mode == HIGH_FEVER)
	{
		Beep(1500);
		delay_ms(30);
		Beep(1500);
		delay_ms(30);
		Beep(1500);
	}

	else if (mode == LIGHT_FEVER)
	{
		Beep(400);
		delay_ms(30);
		Beep(400);
		delay_ms(30);
		Beep(400);
	}
	else if (mode == NORMAL)
	{
		Beep(400);
	}
}

void saveTemp()
{
	memNumber_p = 32;

	tempLogDataSave(TEMP);

	memNumberDisplay(memNumber_p);

	memTempDataDisplay(memNumber_p, unitCalc(__EEPROM->memTempData[memNumber_p - 1], tempUnit_p));
}

int16_t getCaliValue()
{

	int16_t value = caliData_p;

	if (!PLUS_MINUS)
		value = -1 * caliData_p;

	return value;
}

void systemError(VOID)
{
	displayRGB(RED);
	displayError();

	delay_ms(50);
	Beep(1000);
	delay_ms(40);
	Beep(1000);
	delay_ms(40);
	Beep(1000);
	delay_ms(40);
}

/*********************************************/
/*****************  MAIN  ********************/
/*********************************************/

int main(void)
{
	GPIO_init();
	EEPROM_init();
	LCD_Display_init();

	if (caliData_p > 99 || caliData_p < -99)
		caliData_p = 0;

	if (measureMode_p)
		displayRGB(BLUE);
	else
		displayRGB(GREEN);

	IWonTask = new IWON_TEMP_TASK(10); // 온도를 10개 합산해서 평균낸다.

	int16_t value = caliData_p;
	if (!PLUS_MINUS)
		value = -1 * caliData_p;
	else
		value = caliData_p;

	IWonTask->Set_AdjValue(value); // <= 이 값을 저장하고 읽어서 여기에 적용 하세요.

	Beep();

	delay_ms(100);
	for (int i = 0; i < 1000; i++)
	{
		IWonTask->Task();
		delay_ms(2);
	}
	if (true)
	{
		INT32 AMB = IWonTask->Get_AMB_TEMP();
		if (AMB < 0 || 500 < AMB)
		{ // 사용 환경의 온도가 0 도 보다 낮고 50 도 보다 높으면 에러를 발생한다.
			systemError();
		}
	}

	BOOL Measuring = false;
	BOOL Measured = false;
	INT16 MeasredTemp = -999;
	INT8 MeasredCount1 = 0;
	INT8 MeasredCount2 = 0;

	while (SW_PWR_ON)
		;

	while (IWonTask->NeedPowerDown() == false)
	{

		if (Measuring == false)
		{
			keyScan();
		}

		if (Measuring == false && Measured == false && MeasredTemp != -100 && SW_PWR_ON)
		{

			delay_ms(40);

			if (SW_PWR_ON)
			{
				IWonTask->ClearPowerDown();

				Beep();

				if (measureMode_p)
					displayRGB(BLUE);

				MeasredTemp = -100; // 온도측정하라는 값
				IWonTask->Clear_AVG();
			}
		}
		if (Measuring == false && Measured && SW_PWR_ON == false)
		{
			Measured = false;
		}

		if (IWonTask->Task())
		{
			if (Measured == false && Measuring == false && MeasredTemp == -100)
			{ // 온도 측정 시작
				Measuring = true;
				Measured = false;
				MeasredCount1 = 0;
				MeasredCount2 = 0;
			}
			else if (Measuring)
			{ // 온도 측정

				INT32 AMB = IWonTask->Get_AMB_TEMP();
				if (AMB < 0 || 500 < AMB)
				{ // 사용 환경의 온도가 0 도 보다 낮고 50 도 보다 높으면 에러를 발생한다.
					systemError();

					MeasredTemp = 0;

					Measuring = false;
					Measured = true;
					MeasredCount1 = 0;
					MeasredCount2 = 0;
				}
				else if (AMB > 0)
				{
					if (measureMode_p == 1)
					{

						// 인체 측정
						TEMP = IWonTask->Get_BDY_TEMP();
						// TEMP += getCaliValue();

						if (TEMP != -2 && TEMP < 334)
						{ // LOW  Less Than 33.4 C

							delay_ms(2000);
							MeasredTemp = TEMP;

							Beep();

							displayRGB(BLUE);

							displayLOW();

							Measuring = false;
							Measured = true;
							MeasredCount1 = 0;
							MeasredCount2 = 0;
						}
						else if (TEMP == -2 || TEMP > 425)
						{ // HIGH Greater Than 42.5 C

							delay_ms(2000);

							MeasredTemp = TEMP;

							Beep();

							displayRGB(BLUE);

							displayHIGH();

							Measuring = false;
							Measured = true;
							MeasredCount1 = 0;
							MeasredCount2 = 0;
						}
						else
						{

							if (MeasredCount1 > 0 && MeasredCount2 < 50 && (TEMP - MeasredTemp > 2 || TEMP - MeasredTemp < -2))
							{

								MeasredTemp = TEMP;
								MeasredCount1 = 1;
							}
							else
							{

								MeasredTemp = TEMP;
								MeasredCount1++;
								MeasredCount2++;

								if (MeasredCount1 > 20 || MeasredCount2 >= 50)
								{

									tempValueDisplay(unitCalc(TEMP, tempUnit_p)); // temp Display

									saveTemp();

									if (TEMP >= 381 && TEMP <= 425)
									{ // HIGH FEVER
										displayRGB(RED);
										BeepMode(HIGH_FEVER);
									}
									else if (TEMP >= 371 && TEMP <= 380)
									{ // LIGHT FEVER
										displayRGB(YELLOW);
										BeepMode(LIGHT_FEVER);
									}
									else if (TEMP >= 334 && TEMP <= 370)
									{ // NORMAL
										displayRGB(BLUE);
										BeepMode(NORMAL);
									}

									Measuring = false;
									Measured = true;
									MeasredCount1 = 0;
									MeasredCount2 = 0;
								}
							}
						}
					}
					else
					{
						// 사물 측정
						TEMP = IWonTask->Get_OBJ_TEMP();

						if (MeasredCount1 > 0 && (TEMP - MeasredTemp > 3 || TEMP - MeasredTemp < -3))
						{

							MeasredTemp = TEMP;
							MeasredCount1 = 1;
						}
						else
						{

							MeasredTemp = TEMP;
							MeasredCount1++;
							MeasredCount2++;

							if (MeasredCount1 > 15 || MeasredCount2 > 25)
							{

								displayRGB(GREEN);

								tempValueDisplay(unitCalc(TEMP, tempUnit_p));

								saveTemp();

								Beep(850);

								Measuring = false;
								Measured = true;
								MeasredCount1 = 0;
								MeasredCount2 = 0;
							}
						}
					}
				}
			}
		}
	}

	LCD_clear();

	GPIO_LOW(GPIOD, GPIO_Pin_7);
}

/*********************************************/
/*********************************************/
