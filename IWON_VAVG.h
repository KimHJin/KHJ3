///////////////////////////////////////////////////////////////////////////////
//Code Name : IWON VAVG
//
// For STM8L0
//
// 2020/04/18 I-WON SOLUTION CO LTD
// 2020/04/18 v1.0 by KGY
///////////////////////////////////////////////////////////////////////////////

#ifndef IWON_TEMP_VAVG_H
#define IWON_TEMP_VAVG_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdbool.h>


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

#define ABS(x) ((x) > 0 ? (x) : -(x))

class IWON_TEMP_VAVG
{

public:
	// 생성자
	IWON_TEMP_VAVG();
	IWON_TEMP_VAVG(INT8 sumNum); // 평균값을 만들기 위하여 몇개의 방을 써야할지 결정
	IWON_TEMP_VAVG(INT8 sumNum, INT8 OC);

	// 소멸자
	~IWON_TEMP_VAVG();

	VOID Init(VOID);
	VOID Add(INT32 v);
	VOID Rm(VOID);
	INT32 Calc(VOID);
	INT32 AddCalcVOID();
	INT32 AddCalc(INT32 v);
	INT32 AddCalc(INT32 v, INT32 cv);
	BOOL IsOC(VOID);
	VOID SetOC(VOID);
	INT8 GetCC(VOID);

private:
	INT32 *val;
	INT8 num;
	INT8 pos;
	INT8 erc;
	INT8 oc;
	INT8 cc;
};

#endif