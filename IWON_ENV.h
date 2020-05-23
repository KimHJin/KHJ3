
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


// 펌웨어 버전의 : 10 이면 1.0 이다. 11 이면 1.1 이다.
// 버전 1.4 : AMP 측정 버그 수정
#define DEFINED_FW_VER 14

#define DEFINED_adcVREFValue 1224

#define DEFINED_VDD 3300
#define DEFINED_R1 100000
#define DEFINED_R2 200000

#define DEFINED_ADC_CALC 50
#define DEFINED_ADC_DELAY 20
#define DEFINED_ADC_SUM_C 2


#define HAL_ADC_MODULE_ENABLED

#define DEFINED_TADJ0 0
#define DEFINED_VADJ1 0
#define DEFINED_VADJ2 0

#define DEFINED_TSUMN 10

#define NTC_MIN -40
#define NTC_MAX 125

// 사물기준   (체온 계산용)
#define TB_MIN 281
#define TB_MAX 425

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
#define LED_ON()      GPIO_LOW(GPIOD, GPIO_Pin_6)  
#define LED_OFF()     GPIO_HIGH(GPIOD, GPIO_Pin_6)  

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



#endif