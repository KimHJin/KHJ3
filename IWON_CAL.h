
///////////////////////////////////////////////////////////////////////////////
// Code Name : IWON TEMP CAL
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/05/24 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_TASK.h"
#include "IWON_FUNC.h"


#ifndef IWON_TEMP_CAL_H
#define IWON_TEMP_CAL_H

/**
 * 오토 캘리브레이션
 */

// 오토 캘리브레이션 타겟 온도
#define AutoCalTemp1 330	/* STEP 1 : 사물온도 33.0 도 = 인체 36.5 도 */

//#define AutoCalTemp2 400	/* STEP 2 : 사물온도 40.0 도 = 인체 41.0 도 */
#define AutoCalTemp2 330	/* STEP 2 : 사물온도 - 개발용 */

//#define AutoCalTemp3 405	/* STEP 3 : 인체온도 미열 37.1 ~ 38.0 */
#define AutoCalTemp3 363	/* STEP 3 : 인체온도 미열 - 개발용*/

//#define AutoCalTemp4 405	/* STEP 4 : 인체온도 고열 38.1 ~ 42.5 */
#define AutoCalTemp4 363	/* STEP 4 : 인체온도 고열 - 개발용*/

//#define AutoCalTemp5 405	/* STEP 5 : 인체온도 정상 33.0 ~ 37.0 */
#define AutoCalTemp5 363	/* STEP 5 : 인체온도 정상 - 개발용*/

#define AutoCalTorn 5		/* 허용오차 */



class IWON_TEMP_CAL
{
public:
	// 생성자
	IWON_TEMP_CAL();

	// 소멸자
	~IWON_TEMP_CAL();

	VOID AUTOCAL(IWON_TEMP_TASK *IWonTask, IWON_TEMP_FUNC *IWonFunc);
	INT8 GET_AutoCalStep(VOID);

private:   
	VOID Init(VOID);	
	VOID SUCCESS(IWON_TEMP_FUNC *IWonFunc);
	VOID FAIL(IWON_TEMP_FUNC *IWonFunc);
	BOOL IS_SUCCESS(INT32 target, INT32 measered);

	INT8 AutoCalStep;
};


#endif