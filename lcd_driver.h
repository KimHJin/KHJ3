#ifndef __LCD_DRIVER_H__
#define __LCD_DRIVER_H__

#include "stm8l15x.h"
#include "stm8l15x_lcd.h"
#include "stm8l15x_conf.h"
#include "eeprom.h"
#include "IWON_ENV.h"
#include "IWON_FUNC.h"

#define setBitSegment(segment, position) \
	;                                    \
	LCD->segment##position = 1;

#define clsBitSegment(segment, position) \
	;                                    \
	LCD->segment##position = 0;

#define NUMBER_CLEAR_IN(position) \
	clsBitSegment(A, position);  \
	clsBitSegment(B, position);  \
	clsBitSegment(C, position);  \
	clsBitSegment(D, position);  \
	clsBitSegment(E, position);  \
	clsBitSegment(F, position);  \
	clsBitSegment(G, position);

#define NUMBER_0_IN(position)    \
	setBitSegment(A, position); \
	setBitSegment(B, position); \
	setBitSegment(C, position); \
	setBitSegment(D, position); \
	setBitSegment(E, position); \
	setBitSegment(F, position);

#define NUMBER_1_IN(position)    \
	setBitSegment(B, position); \
	setBitSegment(C, position);

#define NUMBER_2_IN(position)    \
	setBitSegment(A, position); \
	setBitSegment(B, position); \
	setBitSegment(D, position); \
	setBitSegment(E, position); \
	setBitSegment(G, position);

#define NUMBER_3_IN(position)    \
	setBitSegment(A, position); \
	setBitSegment(B, position); \
	setBitSegment(C, position); \
	setBitSegment(D, position); \
	setBitSegment(G, position);

#define NUMBER_4_IN(position)    \
	setBitSegment(B, position); \
	setBitSegment(C, position); \
	setBitSegment(F, position); \
	setBitSegment(G, position);

#define NUMBER_5_IN(position)    \
	setBitSegment(A, position); \
	setBitSegment(C, position); \
	setBitSegment(D, position); \
	setBitSegment(F, position); \
	setBitSegment(G, position);

#define NUMBER_6_IN(position)    \
	setBitSegment(A, position); \
	setBitSegment(C, position); \
	setBitSegment(D, position); \
	setBitSegment(E, position); \
	setBitSegment(F, position); \
	setBitSegment(G, position);

#define NUMBER_7_IN(position)    \
	setBitSegment(A, position); \
	setBitSegment(B, position); \
	setBitSegment(C, position); \
	setBitSegment(F, position);

#define NUMBER_8_IN(position)    \
	setBitSegment(A, position); \
	setBitSegment(B, position); \
	setBitSegment(C, position); \
	setBitSegment(D, position); \
	setBitSegment(E, position); \
	setBitSegment(F, position); \
	setBitSegment(G, position);

#define NUMBER_9_IN(position)    \
	setBitSegment(A, position); \
	setBitSegment(B, position); \
	setBitSegment(C, position); \
	setBitSegment(F, position); \
	setBitSegment(G, position);

void LCD_Display_init(IWON_TEMP_FUNC *IWonFunc);
void tempValueDisplay(INT16 value, BOOL fillZero);	// fillZero 를 true 로 하면 00.0 같이 왼쪽에 맨 왼쪽 0 이 표시된다.
void tempValueDisplay(INT16 value);					// 위의 fill 을 true 하여 동작
void displayNumber(INT8 number, INT8 position);
void memNumberDisplay(INT8 number);
void memTempDataDisplay(INT16 tempData, BOOL fillZero);
void memTempDataDisplay(INT16 tempData);
void measureModeSet(BOOL mode);
void tempUnitSet(BOOL unit);


VOID NUMBER_CLEAR(INT8 position);
VOID ALL_NUMBER_CLEAR(VOID);
VOID NUMBER_0(INT8 position);
VOID NUMBER_1(INT8 position);
VOID NUMBER_2(INT8 position);
VOID NUMBER_3(INT8 position);
VOID NUMBER_4(INT8 position);
VOID NUMBER_5(INT8 position);
VOID NUMBER_6(INT8 position);
VOID NUMBER_7(INT8 position);
VOID NUMBER_8(INT8 position);
VOID NUMBER_9(INT8 position);


#endif