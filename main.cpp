///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEMP - NON CONTACT PROJECT
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY & KHJ
// 2020/04/27 v1.1 by KHJ
// 2020/05/09 v1.3 by KHJ
///////////////////////////////////////////////////////////////////////////////

#include "stm8l15x.h"

#include "IWON_TASK.h"
#include "IWON_TEST.h"
#include "IWON_CAL.h"

// 아이원 온도계 테스크 클래스
IWON_TEMP_TASK *IWonTask = NULL;
IWON_TEMP_FUNC *IWonFunc = NULL;

/************************************************************************/
/**
  * 1 msec 마다 호출되는 인터럽트이므로 응용해서 사용해도 좋다.
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler, 25)
{
	IWonTask->Time();
	
	if(IWonFunc->LowBatteryFlag)
	    IWonTask->LowBatteryDisp();

	//
	// 필요한 경우 여기에 코드를 넣어서 사용하세요.
	//

	// Clear TIM4 interrupt
	TIM4_ClearITPendingBit(TIM4_IT_Update);
}
/************************************************************************/
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
					// 수동 보정모드 진입시킴
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
					// 수동 보정모드 진입시킴
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

	if(IWonTask->medicalTestMode!=0)
	{
		if(SW_PWR_ON)
		{
			IWonFunc->Beep();
			IWonFunc->Delay_ms(10);
			IWonFunc->CheckMedicalTestMode(IWonTask);
		}
	}
}

/*********************************************/
/*****************  MAIN  ********************/
/*********************************************/

int main(void)
{
	IWON_TEMP_TEST *IWonTest = NULL;
	IWON_TEMP_CAL *IWonCal = NULL;

	IWON_TEMP_VAVG *TEMP_AVG = new IWON_TEMP_VAVG();
	IWonFunc = new IWON_TEMP_FUNC();
	
	EEPROM_init();
	LCD_Display_init(IWonFunc);


	// 수동 캘리브레이션 저장소의 값이 허용되는 이외의 값은 0 값으로 바꿔준다.
	if (caliData_p > 99 || caliData_p < -99)
		caliData_p = 0;

	IWonTask = new IWON_TEMP_TASK(10); 		// 온도를 10개 합산해서 평균낸다.
	IWonTask->Set_OfsValue(offSetVolt_p);	// 자동 보정값 읽어서 적용
	IWonTask->Set_AdjValue(caliData_p); 	// 수동 보정값 읽어서 적용

	// HW 버전이 의료용인지 확인하여 변수를 초기화 한다.
	if(IS_MEDICAL_VER)
	{
		IWonTask->HWVersion = 0x01; // 01: 의료용
		IWonTask->SetSensorType(1);	// 1: 독일센서
	}

	IWonTask->AMB_REF = ambRef_p;			// 자동 캘리브레이션 할 때 센서의 온도
	if(IWonTask->AMB_REF<50 || IWonTask->AMB_REF>800)
	{
		if(IWonTask->SENSOR_TYPE==1)	// 독일센서
		{
			// 공식 산출시 24.4도 였다.
			ambRef_p = 244;
			IWonTask->AMB_REF = ambRef_p;
		}
		else
		{
			// 만약 없으면 생산이 대략 24.0 도에서 생산하고 있다.
			ambRef_p = 240;
			IWonTask->AMB_REF = ambRef_p;
		}
	}

//	IWonFunc->TempValueDisplay(IWonTask->AMB_REF, false);
//	IWonFunc->Delay_ms(3000);

	// 인체 모드일때 BLUE 배경	=> 버전 4.2 부터 GREEN 으로 바뀜
	// 사물 모드일때 GREEN 배경	=> 버전 4.2 부터 BLUE 로 바뀜
	IWonFunc->DisplayRGB((measureMode_p==1)?GREEN:BLUE);	// 인체 | 사물

	//IWonFunc->TempValueDisplay(0);
	IWonFunc->Beep();

	// 전원 진입 초기에 ADC 의 기본 동작이 되도록 Task 루프를 처리한다.
	for (INT16 i = 0; i < 200; i++)	// 200 값은 충분한 값이다. 중간에 완료되면 Was_Calc 에 의해서 빠져 나간다.
	{
		IWonTask->Task(AutoCaliFlag_p);
		if(IWonTask->Was_Calc())
		{	// ADC 의 기초 계산이 완료되면...
			for (BYTE i = 0; i < 15; i++)	// 추가 계산을 위해서 충분한 루프를 돌리고
			{
				IWonTask->Task(AutoCaliFlag_p);
				IWonFunc->Delay_ms(DEFINED_ADC_DELAY);
			}
			break;	// 빠져나가게 된다.
		}
		IWonFunc->Delay_ms(50);
	}
	
	// 초기에 센서의 온도를 측정하게 된다.
	INT32 AMB = IWonTask->Get_AMB_TEMP();

#ifdef MYTEST	
	IWonFunc->TempValueDisplay(AMB, false);
	IWonFunc->Delay_ms(3000);
#endif

	// 사용 환경의 온도가 0 도 보다 낮고 50 도 보다 높으면 에러를 발생한다.
	if (IWonTask->IsBadAMB(AMB))
	{ 
		IWonFunc->SystemError();
	}
	

	// caliVer_p 이 DEFINED_CALI_VER 값보다 작으면 무조건 오토 캘리브레이션을 가종 시킨다.
	BOOL IsAutoCalCompleted = (AutoCaliFlag_p!=0) && (AutoCaliVer_p>=DEFINED_CALI_VER);
	// BOOL IsAutoCalCompleted = (AutoCaliFlag_p!=0);
	if( IsAutoCalCompleted ) // AUTO CAL 완료인가?
	{
		// 기본 동작모드 진입

		// 지그용 온도계
		#ifdef JIG
		IWonTask->medicalTestMode = 1;
		IWonTask->medicalTestTimerCount = DEFINED_JIG_TEST_INTEVAL;
		#endif
		
		
		// 가장 마지막 측정 값
		if(TEMP>0 && TEMP<800) 
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
		
		// 초기에 전원 버튼과 함께 왼쪽 혹은 오른쪽 버튼을 약 6초 이상 누르고 있으면 확인용 값이 표시된다.
		while (SW_PWR_ON) 
		{
			if(IWonTask->DeviceTestModeWait>500)
			{
				if(!SW_LEFT_ON && !SW_RIGHT_ON)  // 배터리 레벨 체크
				{
					if(IWonTask->IsMedicalVer())
					{
						if(IWonTask->DeviceTestModeValue == 1)
						{
							IWonFunc->Beep();
							IWonFunc->LCD_clear();
							IWonFunc->BetteryPercentDisp(IWonTask);
						}					
						IWonTask->DeviceTestModeValue++;
					}
				}
				else
				if(SW_LEFT_ON && SW_RIGHT_ON)  // LCD 테스트 등의 Test Mode 진입
				{
					if(IWonTask->DeviceTestModeValue == 1)
					{
						IWonTest = new IWON_TEMP_TEST(IWonFunc, IWonTask);
						IWonFunc->Beep();
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
						IWonFunc->VerDisp();		// <= 펌웨어 버전 표시
					}
					IWonTask->DeviceTestModeValue++;
				}
				else	
				if(SW_LEFT_ON)	// 전원 버튼과 왼쪽 버튼을 누르고 있으면 내부 값 표시
				{
					// 숨은기능 (아래의 SW_PWR_ON 관련) : SW_PWR_ON 을 오래 누르고 있으면 배터리 값이 표시 된다.
					if(IWonTask->DeviceTestModeValue==1) 
					{
						IWonFunc->TempValueDisplay(IWonTask->BATmV/100, false);		// <= 배터리 전압값 표시
					}
					else
					if(IWonTask->DeviceTestModeValue==150) 
					{
						IWonFunc->TempValueDisplay(AMB);					// 센서 온도값 표시
					}
					else
					if(IWonTask->DeviceTestModeValue==300) 
					{
						IWonFunc->TempValueDisplay(IWonTask->Get_NTC_mV(), false);		// <= 센서 온도의 전압 (NTC)
						//IWonFunc->TempValueDisplay(IWonTask->Get_ADC_CAL(), false);		// <= ADC 보정 값					
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

		LCD_Display_init(IWonFunc);
		IWonFunc->TempValueDisplay(0);		

		// 테스트 결과 1.1V 에서는 전원이 켜지지도 않음
		if(IWonTask->BATmV/100 <= 20)
			IWonFunc->LowBatteryDisplay_2v0();
		else if(IWonTask->BATmV/100 < 22)
			IWonFunc->LowBatteryDisplay_2v2();
		else if(IWonTask->BATmV/100 < 24)
			IWonFunc->LowBatteryDisplay_2v4();
		else IWonFunc->LowBatteryDisplay_Off();
	}
	else 
	{
		// 오토 캘리브레이션 동작으로 진입
		IWonCal = new IWON_TEMP_CAL();
		IWonFunc->AutoCalDisp();		
		IWonTask->Set_OfsValue(offSetVolt_p);	// 자동 보정값 읽어서 적용
		IWonTask->Set_AdjValue(caliData_p); 	// 수동 보정값 읽어서 적용
	}
	
	while (IWonTask->medicalTestMode!=0 || IWonTask->NeedPowerDown() == false)
	{
		INT16 MEASURED_TEMP = 0;	  
		INT8 testModeFlag = (IWonTest==NULL) ? 0 : IWonTest->GetTestModeFlag();

		// 의료용 테스트모드에서 BEAM LED 를 계속 켜놓는 모드
		if(IWonTask->medicalTestMode==2)
		{
			BEAM_ON();
			keyScan();
			continue;
		}

		if (IWonTask->Measuring == false)
		{
		  	if(testModeFlag==1) // Test Mode
			{	
				IWonTest->TestTask();
			}
			else 
			if(IsAutoCalCompleted==false)
			{
				// 오토 캘리브레이션 모드로 동작 중...
				IWonTask->ClearPowerDown();
				while(SW_LEFT_ON && SW_RIGHT_ON)
				{
					IWonFunc->AutoCalDelayCount++;
					IWonFunc->Delay_ms(100);
					
					if(IWonFunc->AutoCalDelayCount == 40)
					{
						// 오토 캘리브레이션 모드에서 LEFT+RIGHT 버튼을 5초 이상 누르고 있으면 오토캘리브레이션 모드를 빠져나오게 된다.
						AutoCaliFlag_p = 1;
						AutoCaliVer_p = DEFINED_CALI_VER;

						// 이때 오토옵셋값 하고 수동보정값을 0 으로 저장된다.
						offSetVolt_p = 0;
						caliData_p = 0;
						ambRef_p = 0;
						
						IWonFunc->Beep();
						IWonFunc->Delay_ms(100);	   
						IWonFunc->POWER_DOWN();						
					}
				}

				if(SW_LEFT_ON || SW_RIGHT_ON)
				{
					IWonFunc->Delay_ms(150);
				}

				if(SW_LEFT_ON && !SW_RIGHT_ON)
				{
					IWonFunc->Beep();
					IWonFunc->LeftBtnDisp();
					IWonFunc->DisplayRGB(WHITE);
					IWonFunc->Delay_ms(1000);
					while(SW_LEFT_ON && !SW_RIGHT_ON){};					
					IWonFunc->AutoCalDisp();
				}
				else
				if(!SW_LEFT_ON && SW_RIGHT_ON)
				{
					IWonFunc->Beep();
					IWonFunc->RightBtnDisp();
					IWonFunc->DisplayRGB(WHITE);
					IWonFunc->Delay_ms(1000);
					while(!SW_LEFT_ON && SW_RIGHT_ON){};
					IWonFunc->AutoCalDisp();
				}
				
				IWonFunc->AutoCalDelayCount = 0;
			}
			else
			{
				// 측정 모드로 동작 중...
				keyScan();
			}
		}

		if (IWonTask->Measuring == false && IWonTask->Measured == false && IWonTask->MeasredTemp != -1000 && ((SW_PWR_ON && testModeFlag==0) || IWonFunc->Measure_test_flag==1 || IWonTask->IsMedicalTestModeAction()))
		{
			if (SW_PWR_ON || IWonFunc->Measure_test_flag==1 || IWonTask->IsMedicalTestModeAction())
			{
				// IWonTask->medicalTestMode 값이 0 이 아니라는 것은 의료용 테스트 모드인 것이다.
				IWonTask->medicalTestTimerCount = 0;
				
				IWonTask->ClearPowerDown();
				IWonTask->MeasredTemp = -1000; // 온도측정하라는 값

				// 측정 할 때마다 NTC 를 측정한다.
				if(IsAutoCalCompleted)
				{
					INT16 PREAMB = IWonTask->Get_AMB_TEMP();

					IWonTask->InitNTC();
					while(1)
					{
						IWonTask->Task(AutoCaliFlag_p);
						if(IWonTask->Was_Calc())
						{
							AMB = IWonTask->Get_AMB_TEMP();
							if(DIST(PREAMB,AMB)<5)
							{
								break;	// 차이가 0.5도 미만이면 빠져나가게 된다.
							}
							IWonTask->InitNTC();
							PREAMB = AMB;
						}
						IWonFunc->Delay_ms(50);
					}

					if (IWonTask->IsBadAMB(AMB))
					{ 
						BEAM_OFF();
						IWonFunc->SystemError();
					}

					#ifdef JIG	
					memTempDataDisplay(AMB);
					#endif
				}

				IWonTask->Clear_AVG();

				// TODO : 이곳 결정 필요
				// IWonTask->ClearTSUM();

				IWonTask->TSUMBerrCount = 0;

				TEMP_AVG->Init();

				IWonFunc->Measure_test_flag = 0;
				IWonTask->RetryCount = 0;

#ifdef MYTEST				
				// 측정할 때마다 주변온도 표시
				INT32 AMB_TEMP = IWonTask->Get_AMB_TEMP();
				memTempDataDisplay(AMB_TEMP);	// 측정할 때의 AMB 값을 표시한다.
#endif				
			}
		}
		if (IWonTask->Measuring == false && IWonTask->Measured && !SW_PWR_ON)
		{
			IWonTask->Measured = false;
		}

		if (IWonTask->Task(IsAutoCalCompleted ? AutoCaliFlag_p : IWonCal->AutoCalFlag))
		{
			if (IWonTask->Measured == false && IWonTask->Measuring == false && IWonTask->MeasredTemp == -1000)
			{ // 온도 측정 시작
				IWonTask->Measuring = true;
				IWonTask->Measured = false;
				IWonTask->MeasredCount1 = 0;
				IWonTask->MeasredCount2 = 0;

				// 버전 4.2 부터 측정시 별도 배경색을 처리 안하고, 인체모드=GREEN 사물모드=BLUE 로 한다.
				IWonFunc->DisplayRGB((measureMode_p==1)?GREEN:BLUE);	// 인체 | 사물

				#ifdef JIG
				BEAM_ON();
				IWonFunc->Beep();
				#else
				IWonFunc->MeasuringDisp();
				BEAM_ON();
				IWonFunc->Beep();
				#endif
			}
			else 
			if (IWonTask->Measuring)
			{ // 온도 측정
				INT32 AMB = IWonTask->Get_AMB_TEMP();
				if (IWonTask->IsBadAMB(AMB))
				{ // 사용 환경의 온도가 10 도 보다 낮고 50 도 보다 높으면 에러를 발생한다.
					BEAM_OFF();
					IWonFunc->SystemError();
					IWonTask->MeasredTemp = 0;
					IWonTask->SetMeasredStates();
				}
				else 
				if (AMB > 0)
				{
				    IWonFunc->LowHigh_FLag = 0;
					if (measureMode_p == 1)
					{
						// 인체 측정
						MEASURED_TEMP = IWonTask->Get_BDY_TEMP();

						if (IsAutoCalCompleted || (IWonCal!=NULL && IWonTask->Measured==false && IWonCal->GET_AutoCalStep()>0))
						{
							// -120 은 HI 를 뜻함.
							if(MEASURED_TEMP==-120 && IWonTask->Get_TPC_mV()<900)
							{
								MEASURED_TEMP = -110;
							}
							if (MEASURED_TEMP != -120 && MEASURED_TEMP < 334)
							{ 
								// LOW  Less Than 33.4 C
								IWonTask->RetryCount++;
								if(IWonCal==NULL && IWonTask->RetryCount<3)	// 인체 측정에서 초기 한번 LOW 는 다시 측정 시도한다.
								{
									IWonTask->Measured = false;
									IWonTask->Measuring = true;
									IWonTask->MeasredTemp = -1000;
									IWonTask->Clear_AVG();
									IWonFunc->Delay_ms(300);
									continue;
								}

								IWonTask->MeasredTemp = MEASURED_TEMP;
								
								BEAM_OFF();

								// 의료용의 LOW 는 GREEN 바탕
								IWonFunc->DisplayRGB(IWonTask->IsMedicalVer() ? GREEN : BLUE);
								IWonFunc->DisplayLOW();								
								IWonFunc->Beep();								
								IWonTask->SetMeasredStates();
								IWonFunc->CheckMedicalTestMode(IWonTask);
							}
							else 
							if (MEASURED_TEMP == -120 || MEASURED_TEMP > TB_MAX)
							{ 
								// HIGH Greater Than 43.0 C  (42.5 => 43.0 으로 변경 2020.06.04)
								IWonTask->MeasredTemp = MEASURED_TEMP;
								
								BEAM_OFF();

								// 의료용의 HIGH 는 GREEN 바탕
								IWonFunc->DisplayRGB(IWonTask->IsMedicalVer() ? GREEN : BLUE);
								IWonFunc->DisplayHIGH();								
								IWonFunc->Beep();
								IWonTask->SetMeasredStates();
								IWonFunc->CheckMedicalTestMode(IWonTask);
							}
							else
							{
								INT16 MT = MEASURED_TEMP - IWonTask->MeasredTemp;
								if(MT<0) MT = IWonTask->MeasredTemp - MEASURED_TEMP;

								if (IWonTask->MeasredCount1 > 0 && IWonTask->MeasredCount2 < 50 && MT > 3)
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
										BEAM_OFF();
										IWonFunc->BdyTempDisp(IWonTask->MeasredTemp, IsAutoCalCompleted);																				
										IWonTask->SetMeasredStates();
										IWonFunc->CheckMedicalTestMode(IWonTask);
									}
								}
							}
						}

						if (!IsAutoCalCompleted && (IWonTask->Measured || IWonCal->GET_AutoCalStep()==0))
						{
							if(IWonCal->GET_AutoCalStep()>2)
							{
							    IWonFunc->Delay_ms(2000);
							}

							// 측정 된 온도 값을 받아 CAL
							IWonCal->AUTOCAL(IWonTask, IWonFunc); // 실제 AUTO CAL 하는 부분
							BEAM_OFF();
							IWonTask->SetMeasredStates();
						}
					}
					else
					{
						// 사물 측정
						MEASURED_TEMP = IWonTask->Get_OBJ_TEMP();

						INT16 MT = DIST(MEASURED_TEMP, IWonTask->MeasredTemp);
						if (IWonTask->MeasredCount1 > 0 && MT > 3)
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
								BEAM_OFF();

								if(!IsAutoCalCompleted)
								{
									// 측정 된 온도 값을 받아 CAL
									IWonCal->AUTOCAL(IWonTask, IWonFunc); // 실제 AUTO CAL 하는 부분
								}
								else 
								{
									#ifdef JIG	
									//IWonFunc->ObjTempDisp(AMB);	
									IWonFunc->ObjTempDisp(IWonTask->MeasredTemp);	
									#else
									if(IWonTask->MeasredTemp>850 && IWonTask->Get_TPC_mV()<900)
									{
										MEASURED_TEMP = -120;
										IWonTask->MeasredTemp = MEASURED_TEMP;
									}
									IWonFunc->ObjTempDisp(IWonTask->MeasredTemp);	
									#endif									
								}

								IWonTask->SetMeasredStates();								
								IWonFunc->CheckMedicalTestMode(IWonTask);
							}
						}
					}
				}
			}
		}
		else
		{
			// memTempDataDisplay(IWonTask->TSUMBerrCount);
			// 센서의 자체 온도가 급격하게 바뀌거나 (냉동 혹은 가열) 시 측정키(POWER키) 동작 안하는 행업 걸릴때
			if(IWonTask->TSUMBerrCount>120)
			{
				IWonFunc->SystemError();
			}
		}
		
	} //while

	IWonFunc->Beep();
	IWonFunc->Delay_ms(100);	   
	IWonFunc->Beep();
	IWonFunc->POWER_DOWN();	// 함후 안에서 while 로 무한루프 돈다.
}

/*********************************************/
/*********************************************/
