///////////////////////////////////////////////////////////////////////////////
// Code Name : IWON TEMP TASK
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_TASK.h"
#include "lcd_driver.h"
#include "stm8l15x_clk.h"

IWON_TEMP_TASK::IWON_TEMP_TASK() : IWON_TEMP_SCAN()
{
	Init();
}
IWON_TEMP_TASK::IWON_TEMP_TASK(INT8 tsumn) : IWON_TEMP_SCAN(tsumn)
{ // 온도 평균값을 만들기 위하여 몇개의 방을 써야할지 결정
	Init();
}
// 소멸자
IWON_TEMP_TASK::~IWON_TEMP_TASK()
{
	delete VrefintAvg;
	delete VrefvddAvg;
	delete VrefbatAvg;
	delete VrefntcAvg;
	delete VreftpcAvg;
}

VOID IWON_TEMP_TASK::Init(VOID)
{
  	GPIO_init();
  
  	BATmV = 0;
	VDDmV = 0;

	DeviceTestModeWait = 0;		// 테스트 가능 모드를 위해서 있는 변수
	DeviceTestModeValue = 0;	// 테스트 가능 모드를 위해서 있는 변수

	Measuring = false;
	Measured = false;
	MeasredTemp = -999;
	MeasredCount1 = 0;
	MeasredCount2 = 0;
	RetryCount = 0;


	adcVREFValue = DEFINED_adcVREFValue; // 1.224V
	adcVREFINTvalue = 0;
	adcCalValue = 0;

	adc0value = 0;
	adc0volt = 0;
	adc1value = 0;
	adc1volt = 0;
	adc2value = 0;
	adc2volt = 0;

	V0 = 0;
	R0 = 0;
	R1 = DEFINED_R1; // 100K
	R2 = DEFINED_R2; // 200K
	R3 = 0;					 // 측정된 써미스터 저항값

	VrefintAvg = new IWON_TEMP_VAVG(5, 3);
	VrefvddAvg = new IWON_TEMP_VAVG(5, 3);
	VrefbatAvg = new IWON_TEMP_VAVG(5, 3);
	VrefntcAvg = new IWON_TEMP_VAVG(10, 30);
	VreftpcAvg = new IWON_TEMP_VAVG(10);

	ClearAllTemp();
	
	Init_Clock();
	Init_TIM4();
	Init_ADC();

	powerDown_msec = 0;
    lowBattery_Count = 0;
}

VOID IWON_TEMP_TASK::ClearAllTemp(VOID)
{
	Vrefint = 0;
	Vrefvdd = 0;
	Vrefbat = 0;
	Vrefntc = 0;
	Vreftpc = 0;
	
	ADJ_VALUE = 0;
	OFS_VALUE = 0;
	

	VrefintmV = 0;
	VrefvddmV = 0;
	VrefbatmV = 0;
	VrefntcmV = 0;
	VreftpcmV = 0;

	AMB_TEMP = 0;
	OBJ_TEMP = 0;
	BDY_TEMP = 0;

	tCC = 0;

	ClearTSUMB();

	VrefintAvg->Init();
	VrefvddAvg->Init();
	VrefbatAvg->Init();
	VrefntcAvg->Init();
	VreftpcAvg->Init();

	timeStamp = 0;
	startTime = GetTimeOutStartTime();
	TTtime = startTime;
	MGtime = startTime;
}


VOID IWON_TEMP_TASK::Init_Clock(VOID)
{
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

	/* Select HSE as system clock source */
	//CLK_SYSCLKSourceSwitchCmd(ENABLE);
	//CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
	CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
}
VOID IWON_TEMP_TASK::Init_TIM4(VOID)
{
	timeStamp = 0;

	TIM4_DeInit();
	TIM4_TimeBaseInit(TIM4_Prescaler_64, 250); // TimerClock = 16000000 / 64 / 250 = 1000Hz
	TIM4_ClearFlag(TIM4_FLAG_Update);
	TIM4_ITConfig(TIM4_IT_Update, ENABLE);

	enableInterrupts(); // global interrupt enable

	TIM4_Cmd(ENABLE); // Start Timer 4
}

DWORD IWON_TEMP_TASK::GetTimeStamp(VOID)
{
	return timeStamp;
}
VOID IWON_TEMP_TASK::SetTimeStamp(DWORD t)
{
	timeStamp = t;
}
DWORD IWON_TEMP_TASK::GetTimeOutStartTime(VOID)
{
	DWORD TimeOutStartTime;
	sim();
	TimeOutStartTime = GetTimeStamp();
	rim();
	return TimeOutStartTime;
}
VOID IWON_TEMP_TASK::Time(VOID)
{
	timeStamp++;
	powerDown_msec++;
}
BOOL IWON_TEMP_TASK::TimeOut(DWORD startTime, UINT delayTime)
{
	DWORD TimeStamp;
	DWORD PeriodStamp;

	sim();
	TimeStamp = GetTimeStamp();
	rim();

	if (startTime > TimeStamp)
		return TRUE;

	PeriodStamp = TimeStamp - startTime;
	if (PeriodStamp >= delayTime)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID IWON_TEMP_TASK::Init_ADC(VOID)
{

	ADC_DeInit(ADC1);

	/* Initialize and configure ADC1 */
	ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);
	ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
}

BOOL IWON_TEMP_TASK::Was_Calc(VOID) 
{
	return VrefntcAvg->IsOC();
}

VOID IWON_TEMP_TASK::DISABLE_ALL_ADC(VOID) 
{
	ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, DISABLE); 	 // REFINT
	ADC_ChannelCmd(ADC1, ADC_Channel_3, DISABLE);			 // REFVDD
	ADC_ChannelCmd(ADC1, ADC_Channel_4, DISABLE);			 // BAT
	ADC_ChannelCmd(ADC1, ADC_Channel_5, DISABLE);			 // NTC
	ADC_ChannelCmd(ADC1, ADC_Channel_6, DISABLE);			 // TPC
}

BOOL IWON_TEMP_TASK::IS_BODY_CC(VOID)
{
	return (VreftpcAvg->GetCC()>10);
}

INT32 IWON_TEMP_TASK::CALC_TPC_mV(INT16 ObjTemp)
{
	INT16 t = 0;
	for(INT32 TPCmV=800; TPCmV<1500; TPCmV++)	// 우선 800mV ~ 1500mV 사이를 찾아본다.
	{
		t = CALC_OBJTEMP(TPCmV);
		if(t==ObjTemp) return TPCmV;
	}
	for(INT32 TPCmV=800; TPCmV>0; TPCmV--)	// 우선 800mV 이하를 찾아본다
	{
		t = CALC_OBJTEMP(TPCmV);
		if(t==ObjTemp) return TPCmV;
	}
	for(INT32 TPCmV=1500; TPCmV<3300; TPCmV++)	// 우선 1500mV 이상을 찾아본다
	{
		t = CALC_OBJTEMP(TPCmV);
		if(t==ObjTemp) return TPCmV;
	}
	return -1;	// 찾지 못한 것이다.
}

INT16 IWON_TEMP_TASK::CALC_OBJTEMP(INT32 TPCmV)
{
	float ambtemp = (float)AMB_TEMP / 10.f;

	const float k = 0.00066f;
	const float n = 1.93f;
	const float reftemp = 25.f;		
	const float shiftv = 0.57f;
	const float A_v = 454.54f;
	/*						
	float comp = k * (pow(ambtemp, 4.f - delta) - pow(reftemp, 4.f - delta)); // equivalent thermopile V for amb temp			
	float v2 = (float)TPCmV / 1000.f + comp - shiftv;			
	float objtemp = pow((v2 + k * pow(ambtemp, 4.f - delta)) / k, 1.f / (4.f - delta)); // object temp			
	INT16 T_OBJ = (INT16)(objtemp * 10.f);
*/
	
	float constant = 1000.f / (A_v * k);
	float V_tp =  (float)TPCmV / 1000.f - shiftv;
	float objtemp = pow( ( constant * V_tp + pow( ambtemp, n ) ), 1.f / n );
	
	INT16 T_OBJ = (INT16) ( objtemp * 10.f );
	
	// TODO : - 값 고민 대상, 연산 속도 무지 느림.
	INT16 AMBADJ = (INT16)((ambtemp - reftemp) * 10.f);
	if(T_OBJ-AMBADJ < 0) {
		T_OBJ = AddTSUMB(0);
	} else {
		T_OBJ = AddTSUMB(T_OBJ - 300);
	}

	return T_OBJ;
}


BOOL IWON_TEMP_TASK::Task(VOID)
{
	return Task(DEFINED_ADC_CALC, DEFINED_ADC_DELAY);
}
BOOL IWON_TEMP_TASK::Task(UINT MGInterval, UINT TTInterval)
{ // MGInterval = Measure (200), TTInteval = Take Interval (50)

	if (TimeOut(MGtime, MGInterval) && IS_BODY_CC())
	{
		VrefintmV = (INT32)(((INT32)Vrefint * (INT32)ADC_CONVERT_RATIO) / 1000);
		VrefvddmV = (INT32)(((INT32)Vrefvdd * (INT32)ADC_CONVERT_RATIO) / 1000);
		VrefbatmV = (INT32)(((INT32)Vrefbat * (INT32)ADC_CONVERT_RATIO) / 1000);
		VrefntcmV = (INT32)(((INT32)Vrefntc * (INT32)ADC_CONVERT_RATIO) / 1000);
		VreftpcmV = (INT32)(((INT32)Vreftpc * (INT32)ADC_CONVERT_RATIO) / 1000) + ADJ_VALUE + OFS_VALUE;
		

		// V0 는 써미스터에 걸리는 전압
		// R0 는 써미스터와 병렬로 연결된 저항과 병렬 합계 저항 값
		V0 = VrefntcmV;
		R0 = ((DWORD)V0 * (R1 / 100)) / (DWORD)(DEFINED_VDD - V0) * 100;
		
		// R3 는 써미스터 저항 값
		// R3 = ((R2/10)*(R0/10)*10) / (R2-R0);
		R3 = ((R2/10)*R0) / (R2-R0);

		
		INT32 MRES = R3 * 10;
		INT16 ntcIndex = GetNTCIndex(MRES);
		if (ntcIndex == 0)
		{
			AMB_TEMP = -1; // Low
		}
		else if (ntcIndex == -1)
		{
			AMB_TEMP = -2; // High
		}
		else
		{
			INT16 PR = GetNTCValueRatio(MRES, ntcIndex);			
			AMB_TEMP = ((NTC_MIN + ntcIndex) * 100 - PR) / 10;

			OBJ_TEMP = CALC_OBJTEMP(VreftpcmV);

			INT16 BB = GetTSUMB();
			if (BB != -999)
			{
				BB = BB - OBJ_TEMP;
				if (ABS(BB) > 15)
				{
					ClearTSUMB();
				}
			}

				if(DEFINED_USE_BDY_TBL)
				{
					// 사물 to 인체 테이블 사용
					INT8 TBL = GetTBLValue(OBJ_TEMP);
					if (TBL == -1)
					{
						BDY_TEMP = -1; // LOW
					}
					else 
					if (TBL == -2)
					{
						BDY_TEMP = -2; // HIGH
					}
					else
					{
						BDY_TEMP = OBJ_TEMP + (INT16)TBL;				
					}
				}
				else
				{
					BDY_TEMP = (INT16)((float)OBJ_TEMP / DEFINED_BDY_EMI);
				}
		}

		MGtime = GetTimeOutStartTime();

		return (TSUMC>=DEFINED_ADC_SUM_C);
	}
	else if (TimeOut(TTtime, TTInterval))
	{
		if (tCC == 0 && VrefintAvg->IsOC())
			tCC++;
		if (tCC == 0)
		{
			DISABLE_ALL_ADC();
			ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, ENABLE);				// REFINT 
			ADC_SoftwareStartConv(ADC1);
			tCC++;
			TTtime = GetTimeOutStartTime();
			return FALSE;
		}

		if (tCC == 1 && VrefvddAvg->IsOC())
			tCC++;
		if (tCC == 1)
		{
			if (!VrefintAvg->IsOC())
			{
				//printf("VrefintAvg\r\n");
		        while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET){}
				Vrefint = VrefintAvg->AddCalc(ADC_GetConversionValue(ADC1), 30);
				// return FALSE;
			}

			DISABLE_ALL_ADC();
			ADC_ChannelCmd(ADC1, ADC_Channel_3, ENABLE);				// REFVDD
			ADC_SoftwareStartConv(ADC1);
			tCC++;
			TTtime = GetTimeOutStartTime();
			return FALSE;
		}

		if (tCC == 2 && VrefbatAvg->IsOC())
			tCC++;
		if (tCC == 2)
		{
			if (!VrefvddAvg->IsOC())
			{
		        while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET){}
				Vrefvdd = VrefvddAvg->AddCalc(ADC_GetConversionValue(ADC1), 30);
				VrefvddmV = (INT32)(((INT32)Vrefvdd * (INT32)ADC_CONVERT_RATIO) / 1000);
				//printf("VrefvddAvg VrefvddmV=%ld\r\n", VrefvddmV);
				if(VrefvddmV>1600) {
					// 전원 측정이 된 것이다.
					if(VrefvddAvg->IsOC()) {
						adcCalValue = DEFINED_VDD / 2 - VrefvddmV;
						VrefintAvg->SetOC();
					} else {
						tCC = 0;
						return FALSE;
					}
				} else {
					tCC = 0;
					MGtime = GetTimeOutStartTime();
					TTtime = GetTimeOutStartTime();
					return FALSE;
				}
			}

			DISABLE_ALL_ADC();
			ADC_ChannelCmd(ADC1, ADC_Channel_4, ENABLE);				// BAT
			ADC_SoftwareStartConv(ADC1);
			tCC++;
			TTtime = GetTimeOutStartTime();
			return FALSE;
		}

		if (tCC == 3 && VrefntcAvg->IsOC())
			tCC++;
		if (tCC == 3)
		{
			if (VrefvddAvg->IsOC() && !VrefbatAvg->IsOC())
			{
				//printf("VrefbatAvg\r\n");
		        while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET){}
				Vrefbat = VrefbatAvg->AddCalc(ADC_GetConversionValue(ADC1), 30);
				VrefbatAvg->SetOC();
			}

			DISABLE_ALL_ADC();
			ADC_ChannelCmd(ADC1, ADC_Channel_5, ENABLE);				// NTC
			ADC_SoftwareStartConv(ADC1);
			tCC++;
			TTtime = GetTimeOutStartTime();
			return FALSE;
		}

		if (tCC == 4 && VreftpcAvg->IsOC())
			tCC++;
		if (tCC == 4)
		{
			if (VrefvddAvg->IsOC() && !VrefntcAvg->IsOC())
			{
		        for(INT8 i=0;i<30;i++)
				{
			  		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET){}
					Vrefntc = VrefntcAvg->AddCalc(ADC_GetConversionValue(ADC1), 30);
					if(VrefntcAvg->IsOC()) break;
					ADC_SoftwareStartConv(ADC1);
					//Delay_ms(5);
				}								
				VrefntcAvg->SetOC();
			}

			DISABLE_ALL_ADC();
			ADC_ChannelCmd(ADC1, ADC_Channel_6, ENABLE);				// TPC
			ADC_SoftwareStartConv(ADC1);
			tCC++;
			TTtime = GetTimeOutStartTime();
			return FALSE;
		}

		if (tCC == 5)
		{
			if (VrefntcAvg->IsOC() && !VreftpcAvg->IsOC())
			{
				//printf("VreftpcAvg\r\n");
		        for(INT8 i=0;i<30;i++)
				{
			  		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET){}
					Vreftpc = VreftpcAvg->AddCalc(ADC_GetConversionValue(ADC1), 50);
					ADC_SoftwareStartConv(ADC1);
					if(IS_BODY_CC()) break;
				}
			}
			// tCC = 0;
		}

		TTtime = GetTimeOutStartTime();
		return FALSE;
	}

	return FALSE;
}

INT16 IWON_TEMP_TASK::Get_AMB_TEMP(VOID)
{
	return AMB_TEMP;
}
INT16 IWON_TEMP_TASK::Get_OBJ_TEMP(VOID)
{
	return OBJ_TEMP;
}
INT16 IWON_TEMP_TASK::Get_BDY_TEMP(VOID)
{
	return BDY_TEMP;
}
INT16 IWON_TEMP_TASK::Get_BAT_mV(VOID)
{
	return (INT16)(VrefbatmV*2);
}
INT16 IWON_TEMP_TASK::Get_VDD_mV(VOID)
{
	return (INT16)(VrefvddmV*2);
}
INT16 IWON_TEMP_TASK::Get_NTC_mV(VOID)
{
	return (INT16)VrefntcmV;
}
INT32 IWON_TEMP_TASK::Get_TPC_mV(VOID)
{
	return VreftpcmV;
}
UINT IWON_TEMP_TASK::Get_ADC_CAL(VOID)
{
	return adcCalValue;
}

VOID IWON_TEMP_TASK::Clear_AVG(VOID)
{
	VreftpcAvg->Init();
}


VOID IWON_TEMP_TASK::Set_AdjValue(INT16 ADJV)
{
	ADJ_VALUE = ADJV;
}
VOID IWON_TEMP_TASK::Set_OfsValue(INT32 OFSV)
{
	OFS_VALUE = OFSV;
}



BOOL IWON_TEMP_TASK::NeedPowerDown(VOID)
{
	return (powerDown_msec > 15000);
}
VOID IWON_TEMP_TASK::ClearPowerDown(VOID)
{
	powerDown_msec = 0;
}

VOID IWON_TEMP_TASK::LowBatteryDisp(VOID)
{
  lowBattery_Count++;
  
  if(lowBattery_Count < 500)
  {
	LCD->X5 = 1;
	
  }
  
  else if(lowBattery_Count < 1000)
  {
	LCD->X5 = 0;
  }
  
  else 
	lowBattery_Count = 0;
  
}

VOID IWON_TEMP_TASK::GPIO_init(VOID)
{
	GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast); //RED
    GPIO_Init(GPIOF, GPIO_Pin_0, GPIO_Mode_Out_PP_High_Fast); //GREEN
    GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //BLUE
    
    GPIO_Init(GPIOC, GPIO_Pin_5, GPIO_Mode_In_FL_No_IT); //SW_PWR
    GPIO_Init(GPIOE, GPIO_Pin_6, GPIO_Mode_In_FL_No_IT); //SW_LEFT
    GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT); //SW_RIGHT
    
    GPIO_Init(GPIOD, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Fast); //BUZZER
	
	GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast); //PWR_STATE
    
	GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_In_FL_No_IT); // TEST_MODE
	
	GPIO_Init(GPIOD, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast); // RESERVED_LED
	
	GPIO_HIGH(GPIOD, GPIO_Pin_7);
}

VOID IWON_TEMP_TASK::SetMeasredStates(VOID)
{
	this->Measuring = false;
	this->Measured = true;
	this->MeasredCount1 = 0;
	this->MeasredCount2 = 0;
}