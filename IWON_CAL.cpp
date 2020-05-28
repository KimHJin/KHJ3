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
}

VOID IWON_TEMP_CAL::SUCCESS(IWON_TEMP_FUNC *IWonFunc)
{
	IWonFunc->DisplayRGB(GREEN);
	IWonFunc->SuccessDisp();
	IWonFunc->Delay_ms(1000);

	AutoCalStep++;
	IWonFunc->ClearDisp();
	IWonFunc->DisplayRGB(BLUE);
	memTempDataDisplay(AutoCalStep*10);
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
	return (target - AutoCalTorn <= measered && measered <= target + AutoCalTorn);
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
				memTempDataDisplay(AutoCalStep * 10);

				INT32 TPCmV = 0;
				IWonTask->ClearTSUM();

				IWON_TEMP_VAVG *AutoCalAVG = new IWON_TEMP_VAVG(10, 10);
				while(!AutoCalAVG->IsOC())
				{
					if (IWonTask->Task())
					{
						TPCmV = IWonTask->Get_TPC_mV();
						TPCmV = AutoCalAVG->AddCalc(TPCmV, 20);
					}
				}
				delete AutoCalAVG;

				INT32 VoltmV = IWonTask->CALC_TPC_mV(AutoCalTemp1);	// 첫번째 기준온도의 TPC 전압을 구해온다. 시간이 걸릴 수 있다.

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
				memTempDataDisplay(AutoCalStep * 10);

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
						AutoCaliFlag_p = 1;

						IWonFunc->DisplayRGB(GREEN);
						IWonFunc->OkDisp();
						IWonFunc->Delay_ms(500);
						IWonFunc->Beep();
						IWonFunc->Delay_ms(100);	   
						IWonFunc->Beep();

						memTempDataDisplay((AutoCalStep-1) * 10);

						AutoCalStep = 0;
					}
				}
				else 
				{
					FAIL(IWonFunc);
					IWonFunc->TempValueDisplay(IWonTask->MeasredTemp, false);
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
