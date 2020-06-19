///////////////////////////////////////////////////////////////////////////////
// Code Name : IWON TEMP CAL
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/05/24 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_ENV.h"
#include "IWON_CAL.h"
#include "IWON_VAVG.h"

// 생성자
IWON_TEMP_CAL::IWON_TEMP_CAL()
{
	Init();  
}
// 소멸자
IWON_TEMP_CAL::~IWON_TEMP_CAL()
{

}

VOID IWON_TEMP_CAL::Init(VOID)
{
	AutoCalStep = 1;	// 오토 캘리브레이션 스탭1 부터 시작
	AutoCalFlag = 1;
}

VOID IWON_TEMP_CAL::SUCCESS(IWON_TEMP_FUNC *IWonFunc)
{
	IWonFunc->DisplayRGB(GREEN);
	IWonFunc->SuccessDisp();
	IWonFunc->Delay_ms(1000);

	AutoCalStep++;
	IWonFunc->ClearDisp();
	IWonFunc->DisplayRGB(BLUE);
	memTempDataDisplay(AutoCalStep*100 + AutoCalFlag);
}

VOID IWON_TEMP_CAL::TRY(IWON_TEMP_FUNC *IWonFunc)
{
	IWonFunc->DisplayRGB(YELLOW);
	IWonFunc->FailDisp();
	IWonFunc->BeepMode(LIGHT_FEVER);

	IWonFunc->ALLCLEAR();
	AutoCalStep = 1;
}

VOID IWON_TEMP_CAL::FAIL(IWON_TEMP_FUNC *IWonFunc)
{
	IWonFunc->DisplayRGB(RED);
	IWonFunc->FailDisp();
	IWonFunc->BeepMode(HIGH_FEVER);
	IWonFunc->Delay_ms(1000);

	IWonFunc->ALLCLEAR();
	AutoCalStep = 0;
}

BOOL IWON_TEMP_CAL::IS_SUCCESS(INT32 target, INT32 measered)
{
	if(AutoCalStep==2)
	{
		return (DIST(target,measered)<=2);
	}
	return (DIST(target,measered)<=AutoCalTorn);
}

VOID IWON_TEMP_CAL::AUTOCAL(IWON_TEMP_TASK *IWonTask, IWON_TEMP_FUNC *IWonFunc)
{
	switch(AutoCalStep)
	{
		case 1:
			{
				// AUTO CAL STEP 1
				// 사물모드
				// 특정 온도를 측정하여 측정된 전압과 기준이 되는 전압 차이를 offSetVolt_p 에 저장한다.

				IWonTask->InitNTC();
				
				// 약간의 시간을 준다...
				IWonFunc->Delay_ms(300);
				for (BYTE i = 0; i < 150; i++)	// 추가 계산을 위해서 충분한 루프를 돌리고
				{
					IWonTask->Task(AutoCalFlag);
					IWonFunc->Delay_ms(DEFINED_ADC_DELAY);
					if(IWonTask->Was_Calc()) break;
				}
				IWonFunc->Delay_ms(100);
				for (BYTE i = 0; i < 20; i++)	// 추가 계산을 위해서 충분한 루프를 돌리고
				{
					IWonTask->Task(AutoCalFlag);
					IWonFunc->Delay_ms(DEFINED_ADC_DELAY);
				}
				
				ambRef_p = IWonTask->Get_AMB_TEMP();
				IWonTask->AMB_REF = ambRef_p;			// 자동 캘리브레이션 할 때 센서의 온도
				memTempDataDisplay(IWonTask->AMB_REF);	// 측정할 때의 AMB 값을 표시한다.

				INT32 TPCmV = 0;
				IWonTask->ClearTSUM();

				IWON_TEMP_VAVG *AutoCalAVG = new IWON_TEMP_VAVG(10, 10);
				while(!AutoCalAVG->IsOC())
				{
					if (IWonTask->Task(AutoCalFlag))
					{
						TPCmV = IWonTask->Get_TPC_mV();
						TPCmV = AutoCalAVG->AddCalc(TPCmV, 20);
					}
				}
				delete AutoCalAVG;

				INT32 VoltmV = IWonTask->CALC_TPC_mV(AutoCalTemp1, AutoCalFlag);	// 첫번째 기준온도의 TPC 전압을 구해온다. 시간이 걸릴 수 있다.

				INT32 ADJmV = VoltmV - TPCmV;
				offSetVolt_p = ADJmV;

				IWonTask->Set_OfsValue(offSetVolt_p);	// 자동 보정값 읽어서 적용

				SUCCESS(IWonFunc);
			}
			break;

		case 2:
		case 3:
		case 4:
		case 5:
			{
				// AUTO CAL STEP 2		: 사물모드
				// AUTO CAL STEP 3 ~ 5	: 체온모드
				// 특정 온도를 측정하여 해당 오차 범위안에 있는지 확인한다.
				memTempDataDisplay(AutoCalStep * 100 + AutoCalFlag);

				if(AutoCalStep==2)
				{
					// 약간의 시간을 준다...
					IWonFunc->Delay_ms(500);
					for (BYTE i = 0; i < 40; i++)	// 추가 계산을 위해서 충분한 루프를 돌리고
					{
						IWonTask->Task(AutoCalFlag);
						IWonFunc->Delay_ms(DEFINED_ADC_DELAY);
					}
				}

				INT32 target = AutoCalTemp2;				// 사물모드
				if(AutoCalStep==3) target = AutoCalTemp3;	// 체온모드
				if(AutoCalStep==4) target = AutoCalTemp4;	// 체온모드
				if(AutoCalStep==5) target = AutoCalTemp5;	// 체온모드

				// 정상 범위 안 : 녹색 램프 => 다음 단계
				// 정상 범위 바깥 : 적색 램프, 저장된 보정값 삭제 => 전원 OFF
				if(IS_SUCCESS(target, IWonTask->MeasredTemp))
				{
					SUCCESS(IWonFunc);
					
					if(AutoCalStep==3)
					{
						// 인체모드로 변경해야 한다.
						IWonFunc->MeasureModeTask();
					}
					if(AutoCalStep==6)
					{
						// 오토 캘리브레이션 완료 저장
						AutoCaliFlag_p = AutoCalFlag;
						AutoCaliVer_p = DEFINED_CALI_VER;

						IWonFunc->DisplayRGB(GREEN);
						IWonFunc->OkDisp();
						IWonFunc->Delay_ms(500);
						IWonFunc->Beep();
						IWonFunc->Delay_ms(100);	   
						IWonFunc->Beep();

						memTempDataDisplay((AutoCalStep-1) * 100 + AutoCalFlag);

						AutoCalStep = 0;
					}
				}
				else 
				{
					// STEP 2 에서 온도차가 있으면 이를 보정하도록 한다.
					if(AutoCalStep==2 && AutoCalFlag<31)	// 두번째 측정에서 에러가 있으면
					{
						TRY(IWonFunc);
						IWonTask->Set_OfsValue(0);
						IWonTask->Set_AdjValue(0);
						IWonFunc->TempValueDisplay(IWonTask->MeasredTemp, false);

						if(AutoCalFlag==1)
						{
							// target : 450
							if(IWonTask->MeasredTemp > target)
							{
								AutoCalFlag = 1;	// 홀수로 시작 (내려가는 방향)
							}
							else
							{
								AutoCalFlag = 0;	// 짝수로 시작 (올라가는 방향)
							}
						}
						
						INT16 dist = (INT16)target - IWonTask->MeasredTemp;
						if(dist<0)
						{
							dist = IWonTask->MeasredTemp - (INT16)target;
						}
						if(dist!=0)
						{
							dist /= 2;
							dist *= 2;
							if(dist<2) dist = 2;
							AutoCalFlag += dist;
						}
						
						memTempDataDisplay(AutoCalStep * 100 + AutoCalFlag);
						AutoCalStep = 1;
					}
					else
					{
						FAIL(IWonFunc);
						IWonTask->Set_OfsValue(0);
						IWonTask->Set_AdjValue(0);
						IWonFunc->TempValueDisplay(IWonTask->MeasredTemp, false);
					}
					
				}
			}
			break;
		default:
			IWonFunc->POWER_DOWN(); // 파워다운									
			break;
	}
}        

INT8 IWON_TEMP_CAL::GET_AutoCalStep(VOID)
{
	return AutoCalStep;
}
