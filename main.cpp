///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEMP - NON CONTACT PROJECT
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY & KHJ
// 2020/04/27 v1.1 by KHJ
// 2020/05/09 v2.0 by KHJ
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
	
	if(IWonFunc->LowBatteryFlag)
	    IWonTask->lowBatteryDisp();

	//
	// 필요한 경우 여기에 코드를 넣어서 사용하세요.
	//

	// Clear TIM4 interrupt
	TIM4_ClearITPendingBit(TIM4_IT_Update);
}
/************************************************************************/


/********************************* TEST MODE *******************************************/

void MEAS_Test(void)
{
	IWonFunc->Measure_test_flag = 1;
	IWonFunc->LCD_clear();
	memTempDataDisplay(50);
}

void testMode()
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
		  if(nowAction!=0) IWonTest->VDD_Test(IWonTask->VDDmV); 
		  break;

		case 2: 	// 배터리 전압 테스트
		  if(nowAction!=0) IWonTest->BAT_Test(IWonTask->BATmV); 
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
			
			while(SW_LEFT_ON && SW_RIGHT_ON)
			{
			  	delayCount++;
				IWonFunc->Delay_ms(15);
				if (delayCount == 350)
				{
					IWonFunc->Beep();
					IWonFunc->SpecialModeTask(IWonTask);
				}
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
			
			while(SW_LEFT_ON && SW_RIGHT_ON)
			{
			  	delayCount++;
				IWonFunc->Delay_ms(15);
				if (delayCount == 350)
				{
					IWonFunc->Beep();
					IWonFunc->SpecialModeTask(IWonTask);
				}
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

/*********************************************/
/*****************  MAIN  ********************/
/*********************************************/

int main(void)
{		
	IWonTask = new IWON_TEMP_TASK(10); // 온도를 10개 합산해서 평균낸다.
	IWonFunc = new IWON_TEMP_FUNC();
	
	IWON_TEMP_VAVG *TEMP_AVG = new IWON_TEMP_VAVG();
	
	IWonTask->GPIO_init();
	EEPROM_init();
	LCD_Display_init(IWonFunc);


	if (caliData_p > 99 || caliData_p < -99)
		caliData_p = 0;

	IWonTask->Set_AdjValue(caliData_p); // <= 이 값을 저장하고 읽어서 여기에 적용 하세요.

	IWonFunc->Beep();
	


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
	
		// 초기에 센서의 온도를 측정하게 된다.
	INT32 AMB = IWonTask->Get_AMB_TEMP();		
	if (AMB < 150 || 400 < AMB)
	{ // 사용 환경의 온도가 15 도 보다 낮고 40 도 보다 높으면 에러를 발생한다.
		IWonFunc->SystemError();
	}
		
	if( AutoCaliFlag_p == 1 ) // AUTO CAL 완료
	{
	  
	   	if (measureMode_p)
			IWonFunc->DisplayRGB(BLUE);
		else 
			IWonFunc->DisplayRGB(GREEN);
		
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
	  
	  
		IWonTask->BATmV = IWonTask->Get_BAT_mV();
		IWonTask->VDDmV = IWonTask->Get_VDD_mV();
		
		// 테스트 결과 1.1V 에서는 전원이 켜지지도 않음
		if(IWonTask->BATmV/100 <= 20)
			IWonFunc->LowBatteryDisplay_2v0();
		else if(IWonTask->BATmV/100 < 22)
			IWonFunc->LowBatteryDisplay_2v2();
		else if(IWonTask->BATmV/100 < 24)
			IWonFunc->LowBatteryDisplay_2v4();
		
		
		// 초기에 전원 버튼과 함께 왼쪽 혹은 오른쪽 버튼을 약 6초 이상 누르고 있으면 확인용 값이 표시된다.
		while (SW_PWR_ON) 
		{
			if(IWonTask->DeviceTestModeWait>500)
			{
				if(SW_LEFT_ON && SW_RIGHT_ON)  // Test Mode 진입
				{
					if(IWonTask->DeviceTestModeValue == 1)
					{
						IWonTest = new IWON_TEMP_TEST(IWonFunc, IWonTask);
						IWonTest->SetTestModeFlag(1);					
						IWonFunc->LCD_clear();
						IWonFunc->DisplayRGB(CLEAR);
						break;
					}
					IWonTask->DeviceTestModeValue++;
				}
				else 
				if(SW_RIGHT_ON)	// 전원 버튼과 오른쪽 버튼을 누르고 있으면 버전 표시
				{
					if(IWonTask->DeviceTestModeValue==1) 
					{
						tempValueDisplay(DEFINED_FW_VER, false);		// <= 펌웨어 버전 표시
					}
					IWonTask->DeviceTestModeValue++;
				}
				else	
				if(SW_LEFT_ON)	// 전원 버튼과 왼쪽 버튼을 누르고 있으면 내부 값 표시
				{
					// 숨은기능 (아래의 SW_PWR_ON 관련) : SW_PWR_ON 을 오래 누르고 있으면 배터리 값이 표시 된다.
					if(IWonTask->DeviceTestModeValue==1) 
					{
						tempValueDisplay(IWonTask->BATmV/100, false);		// <= 배터리 전압값 표시
					}
					else
					if(IWonTask->DeviceTestModeValue==150) 
					{
						tempValueDisplay(AMB);					// 센서 온도값 표시
					}
					else
					if(IWonTask->DeviceTestModeValue==300) 
					{
						tempValueDisplay(IWonTask->Get_NTC_mV(), false);		// <= 센서 온도의 전압 (NTC)
						//tempValueDisplay(IWonTask->Get_ADC_CAL(), false);		// <= ADC 보정 값					
					}
					else
					if(IWonTask->DeviceTestModeValue==450)	// - - - 표시
					{
						IWonFunc->MeasuringDisp();
					}
					else
					if(IWonTask->DeviceTestModeValue==550)
					{
						IWonTask->DeviceTestModeValue = 0;
					}
					IWonTask->DeviceTestModeValue++;
				}
			}
			else
			{
				IWonTask->DeviceTestModeWait++;
			}

			IWonFunc->Delay_ms(10);
			IWonTask->ClearPowerDown();
		}
		
		tempValueDisplay(0);
	}
	else 
	{
	 	IWonFunc->LCD_clear();
	    IWonFunc->DisplayRGB(CLEAR); 
		
		measureMode_p = 0; // 사물 측정 모드  
		buzzerState_p = 1; // BUZZER ON
		tempUnit_p    = 1; // 섭씨 모드
	}
	
	while (IWonTask->NeedPowerDown() == false)
	{
		INT16 MEASURED_TEMP = 0;
	  
		INT8 testModeFlag = (IWonTest==NULL) ? 0 : IWonTest->GetTestModeFlag();
		
		if (IWonTask->Measuring == false)
		{
		  	if(testModeFlag==1) // Test Mode
			{	
				testMode();
			}
			else if(AutoCaliFlag_p == 0)
			{
			  
			}
			else
			{
				keyScan();
			}
		}

		if (IWonTask->Measuring == false && IWonTask->Measured == false && IWonTask->MeasredTemp != -100 && ((SW_PWR_ON && testModeFlag==0) || IWonFunc->Measure_test_flag==1))
		{
			if (SW_PWR_ON || IWonFunc->Measure_test_flag==1)
			{
				IWonTask->ClearPowerDown();
				IWonTask->MeasredTemp = -100; // 온도측정하라는 값
				IWonTask->Clear_AVG();
				TEMP_AVG->Init();

				for(INT8 i=0;i<100;i++)
					IWonTask->Task();

				IWonFunc->Measure_test_flag = 0;
				IWonTask->RetryCount = 0;
			}
		}
		if (IWonTask->Measuring == false && IWonTask->Measured && SW_PWR_ON == false)
		{
			IWonTask->Measured = false;
		}

		if (IWonTask->Task())
		{
			if (IWonTask->Measured == false && IWonTask->Measuring == false && IWonTask->MeasredTemp == -100)
			{ // 온도 측정 시작
				IWonTask->Measuring = true;
				IWonTask->Measured = false;
				IWonTask->MeasredCount1 = 0;
				IWonTask->MeasredCount2 = 0;

				if (measureMode_p)
					IWonFunc->DisplayRGB(BLUE);
				else 
					IWonFunc->DisplayRGB(GREEN);

				IWonFunc->MeasuringDisp();
				IWonFunc->Beep();
			}
			else if (IWonTask->Measuring)
			{ // 온도 측정
				INT32 AMB = IWonTask->Get_AMB_TEMP();
				if (AMB < 150 || 400 < AMB)
				{ // 사용 환경의 온도가 15 도 보다 낮고 40 도 보다 높으면 에러를 발생한다.
					IWonFunc->SystemError();

					IWonTask->MeasredTemp = 0;

					IWonTask->Measuring = false;
					IWonTask->Measured = true;
					IWonTask->MeasredCount1 = 0;
					IWonTask->MeasredCount2 = 0;
				}
				else if (AMB > 0)
				{
				    IWonFunc->LowHigh_FLag = 0;
					if (measureMode_p == 1)
					{
						// 인체 측정
						MEASURED_TEMP = IWonTask->Get_BDY_TEMP();

						if (MEASURED_TEMP != -2 && MEASURED_TEMP < 334)
						{ // LOW  Less Than 33.4 C
							IWonTask->RetryCount++;
							if(IWonTask->RetryCount<3)	// 인체 측정에서 초기 한번 LOW 는 다시 측정 시도한다.
							{
								IWonTask->Measured = false;
								IWonTask->Measuring = true;
								IWonTask->MeasredTemp = -100;
								IWonTask->Clear_AVG();
								IWonFunc->Delay_ms(300);
								continue;
							}

							IWonTask->MeasredTemp = MEASURED_TEMP;
							IWonFunc->DisplayRGB(BLUE);
							IWonFunc->DisplayLOW();
							IWonFunc->Beep();
							IWonTask->Measuring = false;
							IWonTask->Measured = true;
							IWonTask->MeasredCount1 = 0;
							IWonTask->MeasredCount2 = 0;
						}
						else if (MEASURED_TEMP == -2 || MEASURED_TEMP > 425)
						{ // HIGH Greater Than 42.5 C
							IWonTask->MeasredTemp = MEASURED_TEMP;
							IWonFunc->DisplayRGB(BLUE);
							IWonFunc->DisplayHIGH();
							IWonFunc->Beep();
							IWonTask->Measuring = false;
							IWonTask->Measured = true;
							IWonTask->MeasredCount1 = 0;
							IWonTask->MeasredCount2 = 0;
						}
						else
						{
							if (IWonTask->MeasredCount1 > 0 && IWonTask->MeasredCount2 < 50 && (MEASURED_TEMP - IWonTask->MeasredTemp > 3 || MEASURED_TEMP - IWonTask->MeasredTemp < -3))
							{
								TEMP_AVG->Init();
								IWonTask->MeasredTemp = TEMP_AVG->AddCalc(MEASURED_TEMP);
								IWonTask->MeasredCount1 = 1;
							}
							else
							{
								IWonTask->MeasredTemp = TEMP_AVG->AddCalc(MEASURED_TEMP);
								IWonTask->MeasredCount1++;
								IWonTask->MeasredCount2++;

								if (IWonTask->MeasredCount1 > 10 || IWonTask->MeasredCount2 >= 20)
								{
									IWonFunc->BdyTempDisp(IWonTask->MeasredTemp);

									IWonTask->Measuring = false;
									IWonTask->Measured = true;
									IWonTask->MeasredCount1 = 0;
									IWonTask->MeasredCount2 = 0;
								}
							}
						}
					}
					else
					{
						// 사물 측정
						MEASURED_TEMP = IWonTask->Get_OBJ_TEMP();
						if (IWonTask->MeasredCount1 > 0 && ABS(MEASURED_TEMP - IWonTask->MeasredTemp) > 3)
						{
							TEMP_AVG->Init();
							IWonTask->MeasredTemp = TEMP_AVG->AddCalc(MEASURED_TEMP);
							IWonTask->MeasredCount1 = 1;
						}
						else
						{
							IWonTask->MeasredTemp = TEMP_AVG->AddCalc(MEASURED_TEMP);
							IWonTask->MeasredCount1++;
							IWonTask->MeasredCount2++;

							if (IWonTask->MeasredCount1 > 5 || IWonTask->MeasredCount2 > 10)
							{
								IWonFunc->ObjTempDisp(IWonTask->MeasredTemp);

								if(AutoCaliFlag_p == 0)
								{
									// 측정 된 온도 값을 받아 CAL
									// 3번 측정 후 파워다운		
									IWonFunc->AUTOCAL(IWonTask->MeasredTemp); // 실제 AUTO CAL 하는 부분
									
									if(IWonFunc->GET_AutoCal_Count() == 3) // 3번 측정 완료
									{
										AutoCaliFlag_p = 1; // AUTO CAL 완료
										IWonFunc->Delay_ms(2000);
										IWonFunc->POWER_DOWN(); // 파워다운
									}
								}
								
								IWonTask->Measuring = false;
								IWonTask->Measured = true;
								IWonTask->MeasredCount1 = 0;
								IWonTask->MeasredCount2 = 0;
								
							}
						}
					}
				}
			}
		}
		
		
		
		
	} //while
	
	IWonFunc->POWER_DOWN();	// 함후 안에서 while 로 무한루프 돈다.
}

/*********************************************/
/*********************************************/
