///////////////////////////////////////////////////////////////////////////////
// Code Name : IWON TEST
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_TASK.h"

#ifndef IWON_TEMP_TEST_H
#define IWON_TEMP_TEST_H


class IWON_TEMP_TEST
{
public:
	// 생성자
	IWON_TEMP_TEST(IWON_TEMP_FUNC *iWonFunc, IWON_TEMP_TASK *iWonTask);

	// 소멸자
	~IWON_TEMP_TEST();

	INT8 GetTestCount(VOID);
	INT8 GetTestModeFlag(VOID);
	
	VOID SetTestCount(INT8 testCount);
	VOID SetTestModeFlag(INT8 testModeFlag);


	INT8 IncTestCount();
	INT8 DecTestCount();
		
	VOID VDD_Test(INT16 VDDmV);
	VOID BAT_Test(INT16 BATmV);
	INT8 LCD_Test(VOID);
	INT8 BackLight_Test(VOID);

private:  
  	IWON_TEMP_FUNC *IWonFunc;
  	IWON_TEMP_TASK *IWonTask;
  
	INT8 TestCount;
	INT8 TestModeFlag;

	VOID Init(VOID);	
};

#endif
