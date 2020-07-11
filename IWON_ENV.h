
///////////////////////////////////////////////////////////////////////////////
// Code Name : IWON VAR
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>

#ifndef IWON_TEMP_VAR_H
#define IWON_TEMP_VAR_H


//#define MYTEST
//#define JIG
#define DEFINED_JIG_TEST_INTEVAL 500

// 펌웨어 버전의 : 10 이면 1.0 이다. 11 이면 1.1 이다.
// 버전 1.4 : OPAMP 측정 버그 수정
// 버전 1.5 : 오토 캘리브레이션 기능 변경, 테스트 전압, 테스트별 램프색상, 펌웨어 버전표시 등
// 버전 1.6 : 새로운 온도 공식 및 오토캘
// 버전 1.7 : 미듬 보정값 135 개를 필요한 갯수인 136 개로 변경 , 온도 합산을 10개에서 6개로 변경
// 버전 2.2 : 센서 보정값 다변화 (n 값)
// 버전 2.4 : 센서 보정값의 지능화 (STEP 2 의 차이가 나는 온도에 따른 보정값 적용)
// 버전 2.5 : 오토캘 모드에서 왼쪽 오른쪽 버튼 기능 확인, 의료(검증)용 기능 추가, 고열 42.5 => 43.0 으로 수정
// 버전 2.6 : LEFT, RIGHT 버튼 테스트 완료하지 않아도 오토캘 가능하도록 함.
// 버전 3.8 : AMB 보정, 오토캘에서 미열, 고열 일때 시간 세이브, 오토캘리브레이션 버전 저장, DEFINED_CALI_VER 값 38
// 버전 3.9 : 센서 노이즈로 인하여 15 오차에서 35 로 변경, DEFINED_CALI_VER 값 38
// 버전 4.0 : TAMB 차이를 TSUM 내부에 입력 (15로 재조정), 여기까지 DEFINED_CALI_VER 값 38
// 버전 4.1 : 의료용, 독일센서 추가 (아직 개발 중...), DEFINED_MEDICAL_TEST_ENABLE
// 버전 4.11 : 새로운 캘리브레이션 모드 (NEWCALMODE) <= 의료용 부터 시작이다...
// 버전 4.2 : 배터리 레벨(퍼센트) 표시할 때 배터리 아이콘 표시, 
//           LED 램프 색상 조정
//			 인체모드 : 녹색, 결과 : 정상 녹색, 미열 황색, 고열 적색, 에러 적색
//           사물모드 : 청색, 결과 : 청색
// 버전 4.3 : 센서 AMB 온도 테스트
// 버전 4.4 : 캘리브레이션 모드 (0:5단계, 1:2단계,60도)
// 버전 4.9 : 새로운 온도공식 적용 (의료용, 독일센서2,3)
// 버전 5.1 : 새로운 체온 변환공식 적용 (의료용, 독일센서2,3)
#define DEFINED_FW_VER 51

// VER 4.4
//#define DEFINED_CALI_VER_1 39
//#define DEFINED_CALI_VER_0 38

// From VER 4.9
#define DEFINED_CALI_VER_1 45
#define DEFINED_CALI_VER_0 45


#define DEFINED_adcVREFValue 1224

#define DEFINED_VDD 3300
#define DEFINED_R1 100000
#define DEFINED_R2 200000

#define DEFINED_ADC_CALC 50
#define DEFINED_ADC_DELAY 20
#define DEFINED_ADC_SUM_C 2

// 의료용 테스트모드를 지원하는 테스트 코드 DEFINE (양산용은 주석처리해야 한다. *** 미듬에 다시 물어봐야 한다.)
#define DEFINED_MEDICAL_TEST_ENABLE
// 의료용 테스트 모드 연속 측정 인터벌
#define DEFINED_MEDICAL_TEST_INTEVAL 4000
// 의료용 테스트 모드 진입의 버튼 누르고 있는 시간 약 15초 이상
#define DEFINED_MEDICAL_TEST_BTN_TIME 1900

#define HAL_ADC_MODULE_ENABLED

#define DEFINED_TSUMN 10

// 사물기준 (체온 계산용)
#define TB_MIN 281
#define TB_MAX 422

//#define ADC_CONVERT_RATIO	806		/* (3300mV / 0xFFF) x 1000 */
#define ADC_CONVERT_RATIO 804 /* (3287mV ~ 3300mV / 0xFFF) x 1000 */
//#define ADC_CONVERT_RATIO	802		/* (3287mV / 0xFFF)	x 1000 */

// 피부 방사율을 0.98 정도
// 환경 방사 온도 변경법 등에 의한 정확한 계측에서는, 사람의 피부 의 방사율은 0.95 ~ 0.97 정도
#define BODY_EMI 0.97

#define GPIO_HIGH(a, b) a->ODR |= b
#define GPIO_LOW(a, b) a->ODR &= ~b
#define GPIO_TOGGLE(a, b) a->ODR ^= b


#define SW_PWR_ON    !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
#define SW_LEFT_ON   !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6)
#define SW_RIGHT_ON  !GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)
#define BEAM_ON()     GPIO_LOW(GPIOD, GPIO_Pin_6)  
#define BEAM_OFF()    GPIO_HIGH(GPIOD, GPIO_Pin_6)  

// R9 => 100K (저항 삽입하면 TRUE 저항 없으면 FALSE)
#define HW_VER_BIT0  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)
// R6 => 100K (저항 삽입하면 TRUE 저항 없으면 FALSE)
#define HW_VER_BIT1  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)

// 의료용(독일센서)은 R9 를 장착한다.
#define IS_MEDICAL_VER (HW_VER_BIT0 && !HW_VER_BIT1)


#define TEST_MODE_ON !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)

#define HIGH_FEVER 1
#define LIGHT_FEVER 2
#define NORMAL 3

#define CLEAR 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define CYAN 5
#define MAGENTA 6
#define WHITE 7


#define CHAR char
#define VOID void
#define CONST const
#define BOOL bool
#define LOCAL(type) static type
#define WINAPI
#define CALLBACK
#define FALSE 0
#define TRUE 1


typedef signed char INT8;
typedef int INT16;
typedef long INT32;
typedef long INT;
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef char *LPSTR;
typedef const char *LPCSTR;
typedef VOID *LPVOID;
typedef const VOID *LPCVOID;
typedef BYTE *LPBYTE;
typedef const BYTE *LPCBYTE;

#define lstrlen strlen
#define lstrcpy strcpy
#define lstrcat strcat
#define lstrcmp strcmp
#define lstrcmpi strcasecmp

#define CRLF "\r\n"
#define CRLFSP "     \r\n"

#define ABS(x) ((x > 0) ? x : -x)
#define DIST(a,b) ((a > b) ? a-b : b-a)

#endif