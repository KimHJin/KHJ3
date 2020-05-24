///////////////////////////////////////////////////////////////////////////////
// Code Name : IWON TEMP TEST
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_TEST.h"

// 생성자
IWON_TEMP_TEST::IWON_TEMP_TEST(IWON_TEMP_FUNC *iWonFunc, IWON_TEMP_TASK *iWonTask)
{
	Init();
	IWonFunc = iWonFunc;
	IWonTask = iWonTask;
}
// 소멸자
IWON_TEMP_TEST::~IWON_TEMP_TEST()
{
  
}

VOID IWON_TEMP_TEST::Init(VOID)
{
	IWonFunc = NULL;
	TestCount = 0;
	TestModeFlag = 0;
}


INT8 IWON_TEMP_TEST::GetTestCount(VOID)
{
	return TestCount;
}
INT8 IWON_TEMP_TEST::GetTestModeFlag(VOID)
{
	return TestModeFlag;
}
VOID IWON_TEMP_TEST::SetTestCount(INT8 testCount)
{
	TestCount = testCount;
}
VOID IWON_TEMP_TEST::SetTestModeFlag(INT8 testModeFlag)
{
	TestModeFlag = testModeFlag;
}
INT8 IWON_TEMP_TEST::IncTestCount(VOID)
{
	TestCount++;
	return TestCount;
}
INT8 IWON_TEMP_TEST::DecTestCount(VOID)
{
	TestCount--;
	if(TestCount<0) TestCount = 0;
	return TestCount;
}





VOID IWON_TEMP_TEST::VDD_Test(INT16 VDDmV)
{
    IWonFunc->DisplayRGB(CLEAR);
	IWonFunc->LCD_clear();
    memTempDataDisplay(10);
  	tempValueDisplay(VDDmV/100, false);

	if(VDDmV >= 3000 && VDDmV <= 3300)
	{
		IWonFunc->DisplayRGB(GREEN);
	}
	else 	
	{
		IWonFunc->DisplayRGB(RED);
	}	
}

VOID IWON_TEMP_TEST::BAT_Test(INT16 BATmV)
{  
    IWonFunc->DisplayRGB(CLEAR);
	IWonFunc->LCD_clear();
    memTempDataDisplay(20);
	tempValueDisplay(BATmV/100, false);

	if(BATmV >= 2600 && BATmV <= 3000)
	{
		IWonFunc->DisplayRGB(GREEN);
	}
	else 	
	{
		IWonFunc->DisplayRGB(RED);
	}
}

INT8 IWON_TEMP_TEST::LCD_Test(VOID)
{
	IWonFunc->DisplayRGB(CLEAR);
	IWonFunc->LCD_clear();

	INT8 key = 0;
	while(key==0) 
	{	  
		IWonTask->ClearPowerDown();
	  
		for(INT8 i=0; i<14; i++)	// 모든 아이콘 표시
		  LCD->RAM[i] = 0xFF;		
		
		key = IWonFunc->Delay_ms_with_key(100);
		if(key!=0) break;
		
		IWonFunc->LCD_clear();		
		key = IWonFunc->Delay_ms_with_key(100);
	}
	
	IWonFunc->LCD_clear();		  		
	return key;
}

INT8 IWON_TEMP_TEST::BackLight_Test(VOID)
{
    IWonFunc->LCD_clear();
    memTempDataDisplay(40);

	INT8 key = 0;
	while(key==0) 
	{	  
		IWonTask->ClearPowerDown();
	  
		IWonFunc->DisplayRGB(RED);
		key = IWonFunc->Delay_ms_with_key(100);
		if(key!=0) break;

		IWonFunc->DisplayRGB(GREEN);
		key = IWonFunc->Delay_ms_with_key(100);
		if(key!=0) break;

		IWonFunc->DisplayRGB(BLUE);
		key = IWonFunc->Delay_ms_with_key(100);
		if(key!=0) break;

		IWonFunc->DisplayRGB(CLEAR);	
		key = IWonFunc->Delay_ms_with_key(100);
		if(key!=0) break;
	}	
	IWonFunc->DisplayRGB(CLEAR);
	
	return key;
}

VOID IWON_TEMP_TEST::MEAS_Test(VOID)
{
	IWonFunc->Measure_test_flag = 1;
	IWonFunc->LCD_clear();
	memTempDataDisplay(50);
}


void IWON_TEMP_TEST::TestTask(VOID)
{		
	INT8 nowAction = 0;
	
    IWonTask->ClearPowerDown();

	if(SW_LEFT_ON)
	{
	    IWonFunc->Beep();
	    IWonFunc->Delay_ms(500);	   
		this->DecTestCount();
		nowAction = 1;
		while(SW_LEFT_ON);
	}	
	else 
	if(SW_RIGHT_ON)
	{
	    IWonFunc->Beep();
	    IWonFunc->Delay_ms(500);
		this->IncTestCount();		
		nowAction = 2;
		while(SW_RIGHT_ON);
	}
	
	switch(this->GetTestCount())
	{
		case 0: 	// 부저 테스트
		  IWonFunc->Beep(); 
		  this->IncTestCount();
		  IWonFunc->Delay_ms(500);		  
		  nowAction = 2;
		case 1: 	// 전원 테스트
		  if(nowAction!=0) this->VDD_Test(IWonTask->VDDmV); 
		  break;

		case 2: 	// 배터리 전압 테스트
		  if(nowAction!=0) this->BAT_Test(IWonTask->BATmV); 
		  break;

		case 3: 
		  if(nowAction!=0) this->LCD_Test(); 
		  break;

		case 4: 	// 백라이트 테스트
		  if(nowAction!=0) this->BackLight_Test(); 
		  break;

		case 5: 	// 온도 측정 테스트
		  if(nowAction!=0) this->MEAS_Test();
		  break;

		default: 	// 파워다운 테스트
		  
			IWonFunc->TempLogDataClear();	// 모든 로그 데이터 제거
			IWonFunc->POWER_DOWN();			// 안에서 while 로 무한루프 돈다.
			break;			
	}
}
