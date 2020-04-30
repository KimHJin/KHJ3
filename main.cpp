///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEMP - NON CONTACT PROJECT
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY & KHJ
// 2020/04/27 v1.1 by KHJ
///////////////////////////////////////////////////////////////////////////////

#include "stm8l15x.h"
#include "lcd_driver.h"

#include "IWON_TASK.h"
#include "IWON_TEST.h"

// 아이원 온도계 테스크 클래스
IWON_TEMP_TASK *IWonTask = NULL;
IWON_TEMP_TEST *IWonTest = NULL;
IWON_TEMP_FUNC *IWonFunc = NULL;

/************************************************************************/
/**
  * 1 msec 마다 호출되는 인터럽트이므로 응용해서 사용해도 좋다.
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler, 25)
{
	IWonTask->Time();
	if (IWonFunc->YellowFlag)
		IWonTask->YellowDisp();
	
	if(IWonFunc->LowBatteryFlag)
	    IWonTask->lowBatteryDisp();

	//
	// 필요한 경우 여기에 코드를 넣어서 사용하세요.
	//

	// Clear TIM4 interrupt
	TIM4_ClearITPendingBit(TIM4_IT_Update);
}
/************************************************************************/


/***************************************************************************************/
/********************************* TEST MODE *******************************************/
/***************************************************************************************/

void MEAS_Test(void)
{
	IWonFunc->Measure_test_flag = 1;
	IWonFunc->LCD_clear();
	memTempDataDisplay(50);
}

void testMode(INT16 VDDmV, INT16 BATmV)
{	
    IWonTask->ClearPowerDown();
	
	INT8 nowAction = 0;
	
	if(SW_LEFT_ON)
	{
	    IWonFunc->Beep();
	    IWonFunc->Delay_ms(500);	   
		IWonTest->DecTestCount();
		nowAction = 1;
		while(SW_LEFT_ON);
	}	
	else 
	if(SW_RIGHT_ON)
	{
	    IWonFunc->Beep();
	    IWonFunc->Delay_ms(500);
		IWonTest->IncTestCount();		
		nowAction = 2;
		while(SW_RIGHT_ON);
	}
	
	switch(IWonTest->GetTestCount())
	{
		case 0: 	// 부저 테스트
		  IWonFunc->Beep(); 
		  IWonTest->IncTestCount();
		  IWonFunc->Delay_ms(500);		  
		  nowAction = 2;
		case 1: 	// 전원 테스트
		  if(nowAction!=0) IWonTest->VDD_Test(VDDmV); 
		  break;

		case 2: 	// 배터리 전압 테스트
		  if(nowAction!=0) IWonTest->BAT_Test(BATmV); 
		  break;

		case 3: 
		  if(nowAction!=0) IWonTest->LCD_Test(); 
		  break;

		case 4: 	// 백라이트 테스트
		  if(nowAction!=0) IWonTest->BackLight_Test(); 
		  break;

		case 5: 	// 온도 측정 테스트
		  if(nowAction!=0) MEAS_Test();
		  break;

		default: 	// 파워다운 테스트
			IWonFunc->DisplayOFF();
			IWonFunc->Delay_ms(1000);
			IWonFunc->POWER_DOWN();	// 함후 안에서 while 로 무한루프 돈다.
			break;			
	}
}

/***************************************************************************************/
/***************************************************************************************/

void keyScan()
{
	if (SW_LEFT_ON) // SW_LEFT
	{
		IWonTask->ClearPowerDown();

		IWonFunc->Delay_ms(15);
		int delayCount = 0;

		while (SW_LEFT_ON)
		{
			delayCount++;
			IWonFunc->Delay_ms(15);
			
			if (delayCount == 350)
			{
			    IWonFunc->Beep();
			    IWonFunc->SpecialModeTask(IWonTask);
			}

			if (delayCount == 100 && !SW_RIGHT_ON) // LONG_PRESS
			{
				IWonFunc->Beep();
				IWonFunc->MeasureModeTask(); // measure mode Set
			}
		}

		if (delayCount < 100) // SHORT_PRESS
		{
			IWonFunc->Beep();
			IWonFunc->TempLogDataTask(); // memory Data
		}
		IWonFunc->Delay_ms(10);
	}

	if (SW_RIGHT_ON) // SW_RIGHT
	{
		// POWER_DOWN();	// 개발용

		IWonTask->ClearPowerDown();

		IWonFunc->Delay_ms(15);
		INT16 delayCount = 0;

		while (SW_RIGHT_ON)
		{
			delayCount++;
			IWonFunc->Delay_ms(15);
			
			if (delayCount == 350)
			{
				IWonFunc->Beep();
				IWonFunc->SpecialModeTask(IWonTask);		
			}

			if (delayCount == 100 && !SW_LEFT_ON) // LONG_PRESS
			{
				IWonFunc->Beep();
				IWonFunc->TempUnitTask(true); // temp Unit set
			}
		}

		if (delayCount < 100) // SHORT_PRESS
		{
			IWonFunc->BuzzerStateTask(); // buzzer On / Off
			IWonFunc->Beep();
		}
		IWonFunc->Delay_ms(10);
	}
}

void tempLogDataSave(int16_t saveData)
{
	for (INT8 i = 0; i < 31; i++)
	{
		__EEPROM->memTempData[i] = __EEPROM->memTempData[i + 1];
	}

	__EEPROM->memTempData[31] = saveData;
}

void saveTemp(INT16 temp)
{
	IWonFunc->LastMeasred = 1;

	TEMP = temp;

	memNumber_p = 32;

	tempLogDataSave(temp);

	memNumberDisplay(memNumber_p);

	memTempDataDisplay(IWonFunc->UnitCalc(__EEPROM->memTempData[memNumber_p - 1], tempUnit_p));
}


void systemError(VOID)
{
	IWonFunc->DisplayRGB(RED);
	IWonFunc->DisplayError();

	IWonFunc->Delay_ms(50);
	IWonFunc->Beep(1000);
	IWonFunc->Delay_ms(40);
	IWonFunc->Beep(1000);
	IWonFunc->Delay_ms(40);
	IWonFunc->Beep(1000);
	IWonFunc->Delay_ms(40);
}

void measuringDisp(void)
{  
  	LCD->X9 = 0;	// 맨 앞의 1 자리 꺼지는것
	  
	NUMBER_CLEAR(1);
	NUMBER_CLEAR(2);
	NUMBER_CLEAR(3);

	LCD->G1 = 1;
	LCD->G2 = 1;
	LCD->G3 = 1;

	LCD->DP1 = 0;
}

/*********************************************/
/*****************  MAIN  ********************/
/*********************************************/

int main(void)
{
  	INT16 BATmV = 0;
	INT16 VDDmV = 0;

	INT16 MEASURED_TEMP = 0;
	INT16 DeviceTestModeWait = 0;	// 테스트 가능 모드를 위해서 있는 변수
	INT16 DeviceTestModeValue = 0;	// 테스트 가능 모드를 위해서 있는 변수

	IWonTask = new IWON_TEMP_TASK(10); // 온도를 10개 합산해서 평균낸다.
	IWonFunc = new IWON_TEMP_FUNC();
	
	IWON_TEMP_VAVG *TEMP_AVG = new IWON_TEMP_VAVG();
	
	IWonTask->GPIO_init();
	EEPROM_init();
	LCD_Display_init(IWonFunc);

	if (measureMode_p)
		IWonFunc->DisplayRGB(BLUE);
	else 
		IWonFunc->DisplayRGB(GREEN);

	if (caliData_p > 99 || caliData_p < -99)
		caliData_p = 0;

	IWonTask->Set_AdjValue(caliData_p); // <= 이 값을 저장하고 읽어서 여기에 적용 하세요.

	IWonFunc->Beep();

	// 가장 마지막 측정 값
	if(TEMP>0 && TEMP<500) 
	{
		IWonFunc->LastMeasred = 1;
		if(IWonFunc->TempUnitTask(false)==1)
		{
			IWonFunc->Delay_ms(1200);
		}
		IWonFunc->LastMeasred = 0;
	}

/*	
	delay_ms(2000);	
	BeepMode(NORMAL); 	
	delay_ms(2000);
	BeepMode(LIGHT_FEVER); 
	delay_ms(2000);
	BeepMode(HIGH_FEVER); 
	delay_ms(2000);
*/
	
	// 전원 진입 초기에 ADC 의 기본 동작이 되도록 Task 루프를 처리한다.
	for (BYTE i = 0; i < 200; i++)	// 200 값은 충분한 값이다. 중간에 완료되면 Was_Calc 에 의해서 빠져 나간다.
	{
		IWonTask->Task();
		if(IWonTask->Was_Calc()) {	// ADC 의 기초 계산이 완료된면...
			for (BYTE i = 0; i < 12; i++)	// 추가 계산을 위해서 충분한 루프를 돌리고
			{
				IWonTask->Task();			  
				IWonFunc->Delay_ms(DEFINED_ADC_DELAY);
			}
			break;	// 빠져나가게 된다.
		}
		IWonFunc->Delay_ms(30);
	}

	
	BOOL Measuring = false;
	BOOL Measured = false;
	INT16 MeasredTemp = -999;
	INT8 MeasredCount1 = 0;
	INT8 MeasredCount2 = 0;
	INT8 RetryCount = 0;

	BATmV = IWonTask->Get_BAT_mV();
	VDDmV = IWonTask->Get_VDD_mV();
	
	// 테스트 결과 1.1V 에서는 전원이 켜지지도 않음
	if(BATmV/100 <= 20)
	    IWonFunc->LowBatteryDisplay_2v0();
	else if(BATmV/100 < 22)
		IWonFunc->LowBatteryDisplay_2v2();
    else if(BATmV/100 < 24)
	    IWonFunc->LowBatteryDisplay_2v4();
	
	
	// 초기에 센서의 온도를 측정하게 된다.
	INT32 AMB = IWonTask->Get_AMB_TEMP();		
	if (AMB < 0 || 500 < AMB)
	{ // 사용 환경의 온도가 0 도 보다 낮고 50 도 보다 높으면 에러를 발생한다.
		systemError();
	}


	// 초기에 전원 버튼과 함께 왼쪽 혹은 오른쪽 버튼을 약 6초 이상 누르고 있으면 확인용 값이 표시된다.
	while (SW_PWR_ON) 
	{
		if(DeviceTestModeWait>500)
		{
		  	if(SW_LEFT_ON && SW_RIGHT_ON)  // Test Mode 진입
			{
				if(DeviceTestModeValue == 1)
				{
					IWonTest = new IWON_TEMP_TEST(IWonFunc, IWonTask);
					IWonTest->SetTestModeFlag(1);					
					IWonFunc->LCD_clear();
					IWonFunc->DisplayRGB(CLEAR);
					break;
				}
				DeviceTestModeValue++;
			}
		    else 
			if(SW_RIGHT_ON)	// 전원 버튼과 오른쪽 버튼을 누르고 있으면 버전 표시
			{
				if(DeviceTestModeValue==1) 
				{
					tempValueDisplay(DEFINED_FW_VER, false);		// <= 펌웨어 버전 표시
				}
				DeviceTestModeValue++;
			}
			else	
			if(SW_LEFT_ON)	// 전원 버튼과 왼쪽 버튼을 누르고 있으면 내부 값 표시
			{
				// 숨은기능 (아래의 SW_PWR_ON 관련) : SW_PWR_ON 을 오래 누르고 있으면 배터리 값이 표시 된다.
				if(DeviceTestModeValue==1) 
				{
					tempValueDisplay(BATmV/100, false);		// <= 배터리 전압값 표시
				}
				else
				if(DeviceTestModeValue==150) 
				{
					tempValueDisplay(AMB);					// 센서 온도값 표시
				}
				else
				if(DeviceTestModeValue==300) 
				{
					tempValueDisplay(IWonTask->Get_NTC_mV(), false);		// <= 센서 온도의 전압 (NTC)
					//tempValueDisplay(IWonTask->Get_ADC_CAL(), false);		// <= ADC 보정 값					
				}
				else
				if(DeviceTestModeValue==450)	// - - - 표시
				{
					measuringDisp();
				}
				else
				if(DeviceTestModeValue==550)
				{
					DeviceTestModeValue = 0;
				}
				DeviceTestModeValue++;
			}
		}
		else
		{
			DeviceTestModeWait++;
		}

		IWonFunc->Delay_ms(10);
		IWonTask->ClearPowerDown();
	}
	
	tempValueDisplay(0);

	while (IWonTask->NeedPowerDown() == false)
	{
		INT8 testModeFlag = (IWonTest==NULL) ? 0 : IWonTest->GetTestModeFlag();
		
		if (Measuring == false)
		{
		  	if(testModeFlag==1) // Test Mode
				testMode(VDDmV, BATmV);			
			else 
				keyScan();
		}

		if (Measuring == false && Measured == false && MeasredTemp != -100 && ((SW_PWR_ON && testModeFlag==0) || IWonFunc->Measure_test_flag==1))
		{
			if (SW_PWR_ON || IWonFunc->Measure_test_flag==1)
			{
				IWonTask->ClearPowerDown();
				MeasredTemp = -100; // 온도측정하라는 값
				IWonTask->Clear_AVG();
				TEMP_AVG->Init();

				for(INT8 i=0;i<100;i++)
					IWonTask->Task();

				IWonFunc->Measure_test_flag = 0;
				RetryCount = 0;
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

				if (measureMode_p)
					IWonFunc->DisplayRGB(BLUE);
				else 
					IWonFunc->DisplayRGB(GREEN);

				measuringDisp();
				IWonFunc->Beep();
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
						MEASURED_TEMP = IWonTask->Get_BDY_TEMP();

						if (MEASURED_TEMP != -2 && MEASURED_TEMP < 334)
						{ // LOW  Less Than 33.4 C
							RetryCount++;
							if(RetryCount<3)	// 인체 측정에서 초기 한번 LOW 는 다시 측정 시도한다.
							{
								Measured = false;
								Measuring = true;
								MeasredTemp = -100;
								IWonTask->Clear_AVG();
								IWonFunc->Delay_ms(300);
								continue;
							}

							MeasredTemp = MEASURED_TEMP;
							IWonFunc->DisplayRGB(BLUE);
							IWonFunc->DisplayLOW();
							IWonFunc->Beep();
							Measuring = false;
							Measured = true;
							MeasredCount1 = 0;
							MeasredCount2 = 0;
						}
						else if (MEASURED_TEMP == -2 || MEASURED_TEMP > 425)
						{ // HIGH Greater Than 42.5 C
							MeasredTemp = MEASURED_TEMP;
							IWonFunc->DisplayRGB(BLUE);
							IWonFunc->DisplayHIGH();
							IWonFunc->Beep();
							Measuring = false;
							Measured = true;
							MeasredCount1 = 0;
							MeasredCount2 = 0;
						}
						else
						{
							if (MeasredCount1 > 0 && MeasredCount2 < 50 && (MEASURED_TEMP - MeasredTemp > 3 || MEASURED_TEMP - MeasredTemp < -3))
							{
								TEMP_AVG->Init();
								MeasredTemp = TEMP_AVG->AddCalc(MEASURED_TEMP);
								MeasredCount1 = 1;
							}
							else
							{
								MeasredTemp = TEMP_AVG->AddCalc(MEASURED_TEMP);
								MeasredCount1++;
								MeasredCount2++;

								if (MeasredCount1 > 10 || MeasredCount2 >= 20)
								{
									if (MEASURED_TEMP >= 381 && MEASURED_TEMP <= 425)
									{ // HIGH FEVER
										IWonFunc->DisplayRGB(RED);
										tempValueDisplay(IWonFunc->UnitCalc(MeasredTemp, tempUnit_p)); // temp Display
										IWonFunc->BeepMode(HIGH_FEVER);
									}
									else if (MEASURED_TEMP >= 371 && MEASURED_TEMP <= 380)
									{ // LIGHT FEVER
										IWonFunc->DisplayRGB(YELLOW);
										tempValueDisplay(IWonFunc->UnitCalc(MeasredTemp, tempUnit_p)); // temp Display
										IWonFunc->BeepMode(LIGHT_FEVER);
									}
									else if (MEASURED_TEMP >= 334 && MEASURED_TEMP <= 370)
									{ // NORMAL
										IWonFunc->DisplayRGB(BLUE);
										tempValueDisplay(IWonFunc->UnitCalc(MeasredTemp, tempUnit_p)); // temp Display
										IWonFunc->BeepMode(NORMAL);
									}

									saveTemp(MeasredTemp);

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
						MEASURED_TEMP = IWonTask->Get_OBJ_TEMP();
						if (MeasredCount1 > 0 && (MEASURED_TEMP - MeasredTemp > 3 || MEASURED_TEMP - MeasredTemp < -3))
						{
							TEMP_AVG->Init();
							MeasredTemp = TEMP_AVG->AddCalc(MEASURED_TEMP);
							MeasredCount1 = 1;
						}
						else
						{
							MeasredTemp = TEMP_AVG->AddCalc(MEASURED_TEMP);
							MeasredCount1++;
							MeasredCount2++;

							if (MeasredCount1 > 10 || MeasredCount2 > 20)
							{
								IWonFunc->Beep();

								tempValueDisplay(IWonFunc->UnitCalc(MeasredTemp, tempUnit_p));
								IWonFunc->DisplayRGB(GREEN);
								saveTemp(MeasredTemp);

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
	} //while
	
    IWonFunc->DisplayOFF();
	IWonFunc->Delay_ms(1000);
	IWonFunc->POWER_DOWN();	// 함후 안에서 while 로 무한루프 돈다.
}

/*********************************************/
/*********************************************/
