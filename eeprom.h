#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "stm8l15x.h"

#define EEPROM_base  (uint16_t)0x1000


#define __EEPROM ((EEROM_TypeDef *) EEPROM_base)


#define memNumber_p   __EEPROM->memNumber
#define measureMode_p __EEPROM->measureMode
#define buzzerState_p __EEPROM->buzzerState
#define tempUnit_p    __EEPROM->tempUnit

typedef struct {
  
  int8_t memNumber;
  int8_t measureMode;
  int8_t buzzerState;
  int8_t tempUnit;
  
  int16_t memTempData[32];
  
}EEROM_TypeDef;



void EEPROM_init(void);



#endif