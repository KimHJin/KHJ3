#ifndef __LCD_DRIVER_H__
#define __LCD_DRIVER_H__

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

void LCD_Display_init();
void tempValueDisplay(int16_t value);
void displayNumber(int number, int position);
void memNumberDisplay(int number);
void memTempDataDisplay(int tempData);
void buzzerCMD(int state);
void measureModeSet(int mode);
void tempUnitSet(int unit);

#endif