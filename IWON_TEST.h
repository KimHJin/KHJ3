///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEST
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_SCAN.h"

#ifndef IWON_TEMP_TEST_H
#define IWON_TEMP_TEST_H


#define SW_PWR_ON    !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
#define SW_LEFT_ON   !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6)
#define SW_RIGHT_ON  !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)

#define TEST_MODE_ON !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)

#define HIGH_FEVER 1
#define LIGHT_FEVER 2
#define NORMAL 3

#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define CLEAR 5

class IWON_TEMP_TEST
{
public:
	// 생성자
	IWON_TEMP_TEST();

	// 소멸자
	~IWON_TEMP_TEST();

	INT8 GetTestCount(VOID);
	INT8 GetTestModeFlag(VOID);
	
	VOID SetTestCount(INT8 testCount);
	VOID SetTestModeFlag(INT8 testModeFlag);


	INT8 IncTestCount();
	INT8 DecTestCount();
		
	VOID VDD_Test(VOID);
	VOID BAT_Test(VOID);
	INT8 LCD_Test(VOID);
	INT8 BackLight_Test(VOID);

private:  
	INT8 TestCount;
	INT8 TestModeFlag;

	VOID Init(VOID);
	
	INT8 delay_10us_with_key(INT16 us);
	INT8 delay_ms_with_key(INT16 ms);
	
};

#endif
