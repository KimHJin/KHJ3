///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON TEMP SCAN
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#define HAL_ADC_MODULE_ENABLED

#include "stm8l15x.h"
#include "stm8l15x_conf.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>


#ifndef IWON_TEMP_SCAN_H
#define IWON_TEMP_SCAN_H


#define CHAR            char
#define VOID            void
#define CONST           const
#define BOOL            bool
#define LOCAL(type) static type
#define WINAPI
#define CALLBACK
#define FALSE           0
#define TRUE            1

typedef signed char	INT8;
typedef int			INT16;
typedef long			INT32;
typedef long			INT;
typedef unsigned int		UINT;
typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef char *			LPSTR;
typedef const char *	LPCSTR;
typedef VOID *			LPVOID;
typedef const VOID *	LPCVOID;
typedef BYTE *			LPBYTE;
typedef const BYTE *	LPCBYTE;

#define lstrlen         strlen
#define lstrcpy         strcpy
#define lstrcat         strcat
#define lstrcmp         strcmp
#define lstrcmpi        strcasecmp


#define CRLF		"\r\n"
#define CRLFSP		"     \r\n"

#define DEFINED_TADJ0 0
#define DEFINED_VADJ1 0
#define DEFINED_VADJ2 0

#define DEFINED_TSUMN 10


#define NTC_MIN -40
#define NTC_MAX 125

#define TB_MIN 281
#define TB_MAX 389


//#define ADC_CONVERT_RATIO	806		/* (3300mV / 0xFFF) x 1000 */
#define ADC_CONVERT_RATIO	804		/* (3287mV ~ 3300mV / 0xFFF) x 1000 */
//#define ADC_CONVERT_RATIO	802		/* (3287mV / 0xFFF)	x 1000 */


class IWON_TEMP_SCAN {
    
public:
	// 생성자
	IWON_TEMP_SCAN();
	IWON_TEMP_SCAN(INT8 tsumn);   // 온도 평균값을 만들기 위하여 몇개의 방을 써야할지 결정

	// 소멸자
	~IWON_TEMP_SCAN();

	// TOBJ 를 이 값만큼 빼준다.
	VOID SetTADJ0(INT16 tadj0);
	INT16 GetTADJ0(VOID);

	// delta 값 변경
	VOID SetVADJ1(INT16 vadj1);
	INT16 GetVADJ1(void);

	// shaft v 값
	VOID SetVADJ2(INT16 vadj2);
	INT16 GetVADJ2(VOID);
  
protected:
	INT16 TADJ0;
	INT16 VADJ1;
	INT16 VADJ2;      

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
	VOID ClearTSUMB();
	INT16 GetTSUMB();
	INT16 AddTSUMB(INT16 B);
};

#endif
