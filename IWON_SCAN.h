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

#define DEFINED_TADJ0 60
#define DEFINED_VADJ1 130
#define DEFINED_VADJ2 127

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
	// ������
	IWON_TEMP_SCAN();
	IWON_TEMP_SCAN(INT8 tsumn);   // �µ� ��հ��� ����� ���Ͽ� ��� ���� ������� ����

	// �Ҹ���
	~IWON_TEMP_SCAN();

	// TOBJ �� �� ����ŭ ���ش�.
	VOID SetTADJ0(INT16 tadj0);
	INT16 GetTADJ0(VOID);

	// delta �� ����
	VOID SetVADJ1(INT16 vadj1);
	INT16 GetVADJ1(void);

	// shaft v ��
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
	// NTC ���� ���̺� �ε��� �ѹ��� ã�´�.
	// �Է� : MRES = ������ ��̽��� ���� ��
	// ��� : MRES �� ��ġ�ϴ� NTC ���̺� �ε��� ��ġ
	//-----------------------------------------------------------------------------
	INT16 GetNTCIndex(INT32 MRES);

	// NTC ���� �ε��� ��ȣȭ ������ ���� �̿��ؼ� �ش� �������� ���� ��ġ�� �ۼ�Ʈ�� ����Ѵ�.
	// �� ������ ���� ������ �µ����� ���װ��� �ε����� �ش� NTC ������ 1�� ���ش��̱� ������
	// �� ������ ��Ȯ�� �µ��� ����ϱ� ���ؼ� ������ ���� �ش� ������ ��� ��ġ�� �ִ��� �˾ƾ� �ϱ� �����̴�.
	// �Է� : MRES ������ ��̽��� ���� ��
	//       MRES �� index �ش� NTC �ε��� ��ġ ��
	// ��� : �ش� NTC ������ �Էµ� MRES �� ��ġ �ۼ�Ʈ ��
	INT16 GetNTCValueRatio(INT32 MRES, INT16 index);


	// �繰 �µ��� ��ü �µ��� ��ȯ
	// �Է� : T_OBJ �� ������ �繰 �µ��� 10 ����� �����̴�.
	// ��� : -1 = LO
	//       -2 = HI
	INT8 GetTBLValue(INT16 T_OBJ);


	// ��հ��� ���ϴ� �� �޸� ���� �Լ�
	VOID ClearTSUMB();
	INT16 GetTSUMB();
	INT16 AddTSUMB(INT16 B);
};

#endif
