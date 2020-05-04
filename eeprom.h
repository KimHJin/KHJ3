#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "stm8l15x.h"
#include "IWON_FUNC.h"

#define EEPROM_base (uint16_t)0x1000

#define __EEPROM ((EEROM_TypeDef *)EEPROM_base)

#define PLUS_MINUS     __EEPROM->plusminus
#define caliData_p     __EEPROM->caliData
#define TEMP           __EEPROM->measureTemp
#define memNumber_p    __EEPROM->memNumber
#define measureMode_p  __EEPROM->measureMode
#define buzzerState_p  __EEPROM->buzzerState
#define tempUnit_p 	   __EEPROM->tempUnit
#define memTemp_p(num) __EEPROM->memTempData[num]

typedef struct
{

  INT8 memNumber;
  INT8 measureMode;
  INT8 buzzerState;
  INT8 tempUnit;

  INT16 memTempData[32];
  INT16 measureTemp;

  INT16 caliData;

} EEROM_TypeDef;

void EEPROM_init(void);

#endif