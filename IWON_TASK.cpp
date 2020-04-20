///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEMP TASK
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_TASK.h"


IWON_TEMP_TASK::IWON_TEMP_TASK() : IWON_TEMP_SCAN() {
	Init();
}
IWON_TEMP_TASK::IWON_TEMP_TASK(INT8 tsumn) : IWON_TEMP_SCAN(tsumn) {   // 온도 평균값을 만들기 위하여 몇개의 방을 써야할지 결정
	Init();  
}
  // 소멸자
IWON_TEMP_TASK::~IWON_TEMP_TASK() {
	delete VrefintAvg;
	delete VrefvddAvg;
	delete VrefbatAvg;
	delete VrefntcAvg;
	delete VreftpcAvg;  
}

VOID IWON_TEMP_TASK::Init(VOID) {
	adcVREFValue = DEFINED_adcVREFValue;   // 1.224V
	adcVREFINTvalue = 0;
	adcCalValue = 0;

	adc0value = 0;
	adc0volt = 0;
	adc1value = 0;
	adc1volt = 0;
	adc2value = 0;
	adc2volt = 0;

	VDDREF = DEFINED_VDDREF;	// 설계상의 VDD 전원 3.3V
	VDD = DEFINED_VDD;			// 측정하여 보정된 VDD 전원
	VCAL = 0;					// 설계상의 VDD 전원에 대한 측정된 ADC 옵셋
	V0 = 0;
	R0 = 0;
	R1 = DEFINED_R1;	// 100K
	R2 = DEFINED_R2;	// 200K
	R3 = 0;        		// 측정된 써미스터 저항값  
			
	Vrefint = 0;
	Vrefvdd = 0;
	Vrefbat = 0;
	Vrefntc = 0;
	Vreftpc = 0;
	
	ADJ_VALUE = 0;
	  
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
		
	timeStamp = 0;	
	startTime = GetTimeOutStartTime();
	TTtime = startTime;
	MGtime = startTime;
	
	VrefintAvg = new IWON_TEMP_VAVG(5, 5);
	VrefvddAvg = new IWON_TEMP_VAVG(5, 5);
	VrefbatAvg = new IWON_TEMP_VAVG(5, 5);
	VrefntcAvg = new IWON_TEMP_VAVG(2);
	VreftpcAvg = new IWON_TEMP_VAVG(2);	
	
  	Init_Clock();
	Init_TIM4();
	Init_ADC();	
	
	powerDown_msec = 0;
        count = 0;
}


VOID IWON_TEMP_TASK::Init_Clock(VOID) {
	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
	CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

	/* Select HSE as system clock source */
	//CLK_SYSCLKSourceSwitchCmd(ENABLE);
	//CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
	CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
}
VOID IWON_TEMP_TASK::Init_TIM4(VOID) {
	timeStamp = 0;
	
	TIM4_DeInit();
	TIM4_TimeBaseInit(TIM4_Prescaler_64, 250); // TimerClock = 16000000 / 64 / 250 = 1000Hz
	TIM4_ClearFlag(TIM4_FLAG_Update);
	TIM4_ITConfig(TIM4_IT_Update, ENABLE);
	
	enableInterrupts();	// global interrupt enable
	
	TIM4_Cmd(ENABLE);	// Start Timer 4
}

DWORD IWON_TEMP_TASK::GetTimeStamp(VOID) {
	return timeStamp;
}
VOID IWON_TEMP_TASK::SetTimeStamp(DWORD t) {
	timeStamp = t;
}
DWORD IWON_TEMP_TASK::GetTimeOutStartTime(VOID) {
	DWORD TimeOutStartTime;
	sim();
	TimeOutStartTime = GetTimeStamp();
	rim();
	return TimeOutStartTime;
}
VOID IWON_TEMP_TASK::Time(VOID) {
	timeStamp++;	
	powerDown_msec++;         
}
BOOL IWON_TEMP_TASK::TimeOut(DWORD startTime, UINT delayTime) {
	DWORD TimeStamp;
	DWORD PeriodStamp;

	sim();
	TimeStamp = GetTimeStamp();
	rim();
	
	if(startTime>TimeStamp) return TRUE;

	PeriodStamp = TimeStamp - startTime;
	if (PeriodStamp >= delayTime) {
		return TRUE;
	} else {
		return FALSE;
	}
}

VOID IWON_TEMP_TASK::Init_ADC(VOID) {

	ADC_DeInit(ADC1);

	/* Initialize and configure ADC1 */
	ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);
	ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
}

BOOL IWON_TEMP_TASK::Task(VOID) {
	return Task(250, 60);
}

BOOL IWON_TEMP_TASK::Task(UINT MGInterval, UINT TTInterval) {	// MGInterval = Measure (200), TTInteval = Take Interval (50)
	if(TimeOut(MGtime, MGInterval)) {		
		VrefintmV = (INT32)(((INT32)Vrefint * (INT32)ADC_CONVERT_RATIO) / 1000);
		VrefvddmV = (INT32)(((INT32)Vrefvdd * (INT32)ADC_CONVERT_RATIO) / 1000);
		VrefbatmV = (INT32)(((INT32)Vrefbat * (INT32)ADC_CONVERT_RATIO) / 1000);
		VrefntcmV = (INT32)(((INT32)Vrefntc * (INT32)ADC_CONVERT_RATIO) / 1000);
		VreftpcmV = (INT32)(((INT32)Vreftpc * (INT32)ADC_CONVERT_RATIO) / 1000) + (ADJ_VALUE * 10);
		
		//printf("int=%dmV,vdd=%dmV,bat=%dmV\r\n", (uint16_t)VrefintmV, (uint16_t)VrefvddmV, (uint16_t)VrefbatmV);
		//printf("ntc=%dmV,tpc=%dmV\r\n\r\n", (uint16_t)VrefntcmV, (uint16_t)VreftpcmV);				

		// V0 는 써미스터에 걸리는 전압 
		// R0 는 써미스터와 병렬로 연결된 저항과 병렬 합계 저항 값
		V0 = VrefntcmV;
		R0 = ((DWORD)V0 * (DWORD)R1) / (VDD - V0);
	
		// R3 는 써미스터 저항 값
		R3 = ((R2/100 * R0/100) * 100) / (R2 - R0);
	
		
		INT32 MRES = R3*100;
		INT16 ntcIndex = GetNTCIndex(MRES);
		if(ntcIndex==0) {
			AMB_TEMP = -1;	// Low
			//printf("AMB_TEMP LOW\r\n");
		}
		else
		if(ntcIndex==-1) {
			AMB_TEMP = -2;	// High
			//printf("AMB_TEMP HIGH\r\n");
		}
		else {
			INT16 PR = GetNTCValueRatio(MRES, ntcIndex);
			AMB_TEMP = ((NTC_MIN + ntcIndex) * 100 + PR) / 10;
			//printf("AMB_TEMP=%d.%d\r\n", AMB_TEMP/10, AMB_TEMP%10);
								
			
			// constants for the thermopile calculation
			const float k = 0.004313f; 
			//const float k = 0.004313f + 0.0002; 	// 값을 높이면 측정온도가 내려간다. 0.0001 당 0.7~0.8도
			// 값을 키우면 TOBJ 온도가 올라간다.
			float delta = 2.468f + 0.150f + (float)VADJ1 / 1000.f;

			
			AMB_TEMP = 280;
			
			float ambtemp = (float)AMB_TEMP / 10.f;
			//float reftemp = 23.f;       // 값을 낮추면 온도가 올라간다.
			//reftemp += 32.0f - ambtemp; // 즉, 이값을 높이면 온도가 올라간다.
			float reftemp = 23.f;

			// 값을 높이면 TOBJ 온도가 내려간다.
			// 값을 높이면 낮은 쪽의 온도차가 높은쪽의 온도차 감소량보다 많이 감소한다.
			float shiftv = 0.81f + (float)VADJ2 / 100.f;
			float comp = k * (pow(ambtemp,4.f-delta)-pow(reftemp,4.f-delta));  // equivalent thermopile V for amb temp

			VreftpcmV = 2250;	// 72.0 oC
			//VreftpcmV = 2185;	// 70.0 oC
			//VreftpcmV = 2100;	// 68.0 oC
			//VreftpcmV = 1950;	// 62.2 oC

			VreftpcmV = 1071;	// 30.8 oC
			
			
			float v2 = (float)VreftpcmV / 1000.f + comp - shiftv;
			float objtemp = pow((v2+k*pow(ambtemp,4.f-delta))/k, 1.f/(4.f-delta)); // object temp                    
			INT16 T_OBJ = (INT16)(objtemp * 10.f) - TADJ0;
			
			INT16 BB = GetTSUMB();						
			//printf("BB=%d\r\n", BB);			
			
			if(BB!=-999) {
				BB = BB-T_OBJ;
				if(BB > 15 || BB < -15) ClearTSUMB();
			}			
			OBJ_TEMP = AddTSUMB(T_OBJ);
			//printf("BB=%d\r\n", BB);
			OBJ_TEMP = T_OBJ;
			
			
			INT8 TBL = GetTBLValue(OBJ_TEMP);
			if(TBL==-1) {
				BDY_TEMP = -1;	// LOW
				//printf("BDY_TEMP LOW\r\n");
			} else
			if(TBL==-2) {
				BDY_TEMP = -2;	// HIGH
				//printf("BDY_TEMP HIGH\r\n");
			} else {
				BDY_TEMP = OBJ_TEMP + (INT16)TBL;
				//printf("BDY_TEMP %d.%d\r\n", BDY_TEMP/10, BDY_TEMP%10);
			}	
			
			BDY_TEMP = OBJ_TEMP;
		}

		MGtime = GetTimeOutStartTime();
		return TRUE;
	} 
	else 
	if(TimeOut(TTtime, TTInterval)) {
	  
		if(tCC==0 && VrefintAvg->IsOC()) tCC++;
		if(tCC==0) {
			ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, ENABLE);	// REFINT
			ADC_ChannelCmd(ADC1, ADC_Channel_3, DISABLE);		// REFVDD
			ADC_ChannelCmd(ADC1, ADC_Channel_4, DISABLE);		// BAT
			ADC_ChannelCmd(ADC1, ADC_Channel_5, DISABLE);		// NTC
			ADC_ChannelCmd(ADC1, ADC_Channel_6, DISABLE);		// TPC
			ADC_SoftwareStartConv(ADC1);
			tCC ++;
			TTtime = GetTimeOutStartTime();
			return TRUE;
		} 
		
		if(tCC==1 && VrefvddAvg->IsOC()) tCC++;
		if(tCC==1) {
			if(!VrefintAvg->IsOC()) {
				Vrefint = VrefintAvg->AddCalc(ADC_GetConversionValue(ADC1), 500);
			}

			ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, DISABLE);	// REFINT
			ADC_ChannelCmd(ADC1, ADC_Channel_3, ENABLE);		// REFVDD
			ADC_ChannelCmd(ADC1, ADC_Channel_4, DISABLE);		// BAT
			ADC_ChannelCmd(ADC1, ADC_Channel_5, DISABLE);		// NTC
			ADC_ChannelCmd(ADC1, ADC_Channel_6, DISABLE);		// TPC
			ADC_SoftwareStartConv(ADC1);	
			tCC ++;				  
			TTtime = GetTimeOutStartTime();
			return TRUE;
		} 

		if(tCC==2 && VrefbatAvg->IsOC()) tCC++;
		if(tCC==2) {
			if(!VrefvddAvg->IsOC()) {
				Vrefvdd = VrefvddAvg->AddCalc(ADC_GetConversionValue(ADC1), 500);
			}
													
			ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, DISABLE);	// REFINT
			ADC_ChannelCmd(ADC1, ADC_Channel_3, DISABLE);		// REFVDD
			ADC_ChannelCmd(ADC1, ADC_Channel_4, ENABLE);		// BAT
			ADC_ChannelCmd(ADC1, ADC_Channel_5, DISABLE);		// NTC
			ADC_ChannelCmd(ADC1, ADC_Channel_6, DISABLE);		// TPC
			ADC_SoftwareStartConv(ADC1);	
			tCC ++;
			TTtime = GetTimeOutStartTime();
			return TRUE;
		} 

		if(tCC==3 && VrefntcAvg->IsOC()) tCC++;
		if(tCC==3) {
			if(!VrefbatAvg->IsOC()) {
				Vrefbat = VrefbatAvg->AddCalc(ADC_GetConversionValue(ADC1), 500);
			}
													
			ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, DISABLE);	// REFINT
			ADC_ChannelCmd(ADC1, ADC_Channel_3, DISABLE);		// REFVDD
			ADC_ChannelCmd(ADC1, ADC_Channel_4, DISABLE);		// BAT
			ADC_ChannelCmd(ADC1, ADC_Channel_5, ENABLE);		// NTC
			ADC_ChannelCmd(ADC1, ADC_Channel_6, DISABLE);		// TPC
			ADC_SoftwareStartConv(ADC1);	
			tCC ++;
			TTtime = GetTimeOutStartTime();
			return TRUE;
		} 

		if(tCC==4 && VreftpcAvg->IsOC()) tCC++;
		if(tCC==4) {
			if(!VrefntcAvg->IsOC()) {
				Vrefntc = VrefntcAvg->AddCalc(ADC_GetConversionValue(ADC1), 500);
			}
													
			ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, DISABLE);	// REFINT
			ADC_ChannelCmd(ADC1, ADC_Channel_3, DISABLE);		// REFVDD
			ADC_ChannelCmd(ADC1, ADC_Channel_4, DISABLE);		// BAT
			ADC_ChannelCmd(ADC1, ADC_Channel_5, DISABLE);		// NTC
			ADC_ChannelCmd(ADC1, ADC_Channel_6, ENABLE);		// TPC
			ADC_SoftwareStartConv(ADC1);	
			tCC ++;
			TTtime = GetTimeOutStartTime();
			return TRUE;
		} 

		if(tCC==5) {
			if(!VreftpcAvg->IsOC()) {
				Vreftpc = VreftpcAvg->AddCalc(ADC_GetConversionValue(ADC1), 500);
			}
			tCC = 0;
		}

		TTtime = GetTimeOutStartTime();
		return TRUE;
	}
	
	return FALSE;
}

INT16 IWON_TEMP_TASK::Get_AMB_TEMP(VOID) {
 	return AMB_TEMP;
}
INT16 IWON_TEMP_TASK::Get_OBJ_TEMP(VOID) {
 	return OBJ_TEMP;
}
INT16 IWON_TEMP_TASK::Get_BDY_TEMP(VOID) {
 	return BDY_TEMP;
}

VOID IWON_TEMP_TASK::Clear_AVG(VOID) {
  	VreftpcAvg->Init();
}

VOID IWON_TEMP_TASK::Set_AdjValue(INT32 ADJV) {
  	ADJ_VALUE = ADJV;
}


BOOL IWON_TEMP_TASK::NeedPowerDown(VOID) {
  	return (powerDown_msec>1500000);
}
VOID IWON_TEMP_TASK::ClearPowerDown(VOID) {
  	powerDown_msec = 0;
}

VOID IWON_TEMP_TASK::YellowDisp(VOID)
{
           count++;            
            if(count < 5)
            {
              GPIO_LOW(GPIOD, GPIO_Pin_4);  //RED
              
              GPIO_HIGH(GPIOF, GPIO_Pin_0); //GREEN
            }
            else 
            {
               GPIO_LOW(GPIOF, GPIO_Pin_0); //GREEN
               
               GPIO_HIGH(GPIOD, GPIO_Pin_4);  //RED
               
               count = 0;
            }
}
