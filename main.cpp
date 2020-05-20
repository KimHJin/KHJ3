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
		  
		    for(int i=0; i<32; i++)
				memTemp_p(i) = 0;
			
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
	IWON_TEMP_VAVG *TEMP_AVG = new IWON_TEMP_VAVG();
	IWonFunc = new IWON_TEMP_FUNC();
	
	EEPROM_init();
	LCD_Display_init(IWonFunc);
	
	IWonTask = new IWON_TEMP_TASK(10);
	
	for (INT8 i = 0; i < 14; i++)
	{
		LCD->RAM[i] = 0xFF;
	}
	
	
	while(1)
	{
		IWonFunc->DisplayRGB(RED);
		IWonFunc->Delay_ms(1000);
	    IWonFunc->DisplayRGB(GREEN);
		IWonFunc->Delay_ms(1000);
		IWonFunc->DisplayRGB(BLUE);
		IWonFunc->Delay_ms(1000);
		IWonFunc->DisplayRGB(YELLOW);
		IWonFunc->Delay_ms(1000);
		
	} //while

	
}

/*********************************************/
/*********************************************/
