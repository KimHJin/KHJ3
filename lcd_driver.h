#ifndef __LCD_DRIVER_H__
#define __LCD_DRIVER_H__

#include "IWON_SCAN.h"
#include "IWON_FUNC.h"

#include "stm8l15x.h"
#include "stm8l15x_lcd.h"
#include "eeprom.h"

#define setBitSegment(segment, position) \
	;                                      \
	LCD->segment##position = 1;

#define clsBitSegment(segment, position) \
	;                                      \
	LCD->segment##position = 0;

#define NUMBER_CLEAR(position) \
	clsBitSegment(A, position);  \
	clsBitSegment(B, position);  \
	clsBitSegment(C, position);  \
	clsBitSegment(D, position);  \
	clsBitSegment(E, position);  \
	clsBitSegment(F, position);  \
	clsBitSegment(G, position);

#define NUMBER_0(position)    \
	setBitSegment(A, position); \
	setBitSegment(B, position); \
	setBitSegment(C, position); \
	setBitSegment(D, position); \
	setBitSegment(E, position); \
	setBitSegment(F, position);

#define NUMBER_1(position)    \
	setBitSegment(B, position); \
	setBitSegment(C, position);

#define NUMBER_2(position)    \
	setBitSegment(A, position); \
	setBitSegment(B, position); \
	setBitSegment(D, position); \
	setBitSegment(E, position); \
	setBitSegment(G, position);

#define NUMBER_3(position)    \
	setBitSegment(A, position); \
	setBitSegment(B, position); \
	setBitSegment(C, position); \
	setBitSegment(D, position); \
	setBitSegment(G, position);

#define NUMBER_4(position)    \
	setBitSegment(B, position); \
	setBitSegment(C, position); \
	setBitSegment(F, position); \
	setBitSegment(G, position);

#define NUMBER_5(position)    \
	setBitSegment(A, position); \
	setBitSegment(C, position); \
	setBitSegment(D, position); \
	setBitSegment(F, position); \
	setBitSegment(G, position);

#define NUMBER_6(position)    \
	setBitSegment(A, position); \
	setBitSegment(C, position); \
	setBitSegment(D, position); \
	setBitSegment(E, position); \
	setBitSegment(F, position); \
	setBitSegment(G, position);

#define NUMBER_7(position)    \
	setBitSegment(A, position); \
	setBitSegment(B, position); \
	setBitSegment(C, position); \
	setBitSegment(F, position);

#define NUMBER_8(position)    \
	setBitSegment(A, position); \
	setBitSegment(B, position); \
	setBitSegment(C, position); \
	setBitSegment(D, position); \
	setBitSegment(E, position); \
	setBitSegment(F, position); \
	setBitSegment(G, position);

#define NUMBER_9(position)    \
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
void buzzerCMD(BOOL state);
void measureModeSet(BOOL mode);
void tempUnitSet(BOOL unit);
void successDisp(void);
void failDisp(void);


#endif