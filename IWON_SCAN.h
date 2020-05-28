///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEMP SCAN
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include "IWON_ENV.h"

#ifndef IWON_TEMP_SCAN_H
#define IWON_TEMP_SCAN_H

class IWON_TEMP_SCAN
{

public:
	// 생성자
	IWON_TEMP_SCAN();
	IWON_TEMP_SCAN(INT8 tsumn); // 온도 평균값을 만들기 위하여 몇개의 방을 써야할지 결정

	// 소멸자
	~IWON_TEMP_SCAN();

/*
	// TOBJ 를 이 값만큼 빼준다.
	VOID SetTADJ0(INT16 tadj0);
	INT16 GetTADJ0(VOID);

	// delta 값 변경
	VOID SetVADJ1(INT16 vadj1);
	INT16 GetVADJ1(void);

	// shaft v 값
	VOID SetVADJ2(INT16 vadj2);
	INT16 GetVADJ2(VOID);
*/
	
protected:
/*
	INT16 TADJ0;
	INT16 VADJ1;
	INT16 VADJ2;
*/

	INT8 TSUMN;
	INT8 TSUMC;
	INT16 *TSUMB;

	VOID InitTSUMB(void);

	//-----------------------------------------------------------------------------
	// NTC 기준 테이블 인덱스 넘버를 찾는다.
	// 입력 : MRES = 센서의 써미스터 저항 값
	// 출력 : MRES 가 위치하는 NTC 테이블 인덱스 위치
	//-----------------------------------------------------------------------------
	INT16 GetNTCIndex(INT32 MRES);

	// NTC 기준 인덱스 번호화 실측된 값을 이용해서 해당 구간내의 보정 위치의 퍼센트를 계산한다.
	// 그 이유는 실제 측정된 온도값의 저항값의 인덱스는 해당 NTC 구간의 1도 분해능이기 때문에
	// 그 사이의 정확한 온도를 계산하기 위해서 측정된 값이 해당 구간의 어느 위치에 있는지 알아야 하기 때문이다.
	// 입력 : MRES 센서의 써미스터 저항 값
	//       MRES 의 index 해당 NTC 인덱스 위치 값
	// 출력 : 해당 NTC 구간의 입력된 MRES 의 위치 퍼센트 값
	INT16 GetNTCValueRatio(INT32 MRES, INT16 index);

	// 사물 온도를 인체 온도로 변환
	// 입력 : T_OBJ 는 측정된 사물 온도를 10 배수한 정수이다.
	// 출력 : -1 = LO
	//       -2 = HI
	INT8 GetTBLValue(INT16 T_OBJ);

	// 평균값을 구하는 링 메모리 관련 함수
	INT16 GetTSUMB();
	INT16 AddTSUMB(INT16 B);

	VOID ClearTSUMB(VOID);
};

#endif
