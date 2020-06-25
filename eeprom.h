#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "stm8l15x.h"
#include "IWON_FUNC.h"

#define EEPROM_base (uint16_t)0x1000

#define __EEPROM ((EEROM_TypeDef *)EEPROM_base)

#define caliData_p     __EEPROM->caliData
#define TEMP           __EEPROM->measureTemp
#define memNumber_p    __EEPROM->memNumber
#define measureMode_p  __EEPROM->measureMode
#define buzzerState_p  __EEPROM->buzzerState
#define tempUnit_p 	   __EEPROM->tempUnit
#define memTemp_p(num) __EEPROM->memTempData[num]
#define AutoCaliFlag_p __EEPROM->AutoCaliFlag
#define offSetVolt_p   __EEPROM->offSetVolt
#define ambRef_p       __EEPROM->ambRef
#define AutoCaliVer_p  __EEPROM->AutoCaliVer
#define AutoCaliMode_p __EEPROM->AutoCaliMode

typedef struct
{
  INT8 memNumber;
  INT8 measureMode;
  INT8 buzzerState;
  INT8 tempUnit;

  INT16 memTempData[32];
  INT16 measureTemp;

  INT8 caliData;  		// 수동 캘리브레이션 값
  INT8 AutoCaliFlag;	// 0 은 AutoCAL 필요, 1은 첫번째 공식, 2는 두번째 공식, 3은 세번째 공식  
  INT32 offSetVolt;		// 자동 캘리브레이션 값

  INT16 ambRef;			// AutoCal 할 때 센서의 AMB 온도

  INT16 AutoCaliVer;	// AutoCal Version. 이 값이 다르면 무조건 자동 캘리브레이션 모드로 들어간다.
  INT8 AutoCaliMode;	// 0: 기본5단계 모드 1: 2단계 60도 모드 등등...
} EEROM_TypeDef;

void EEPROM_init(void);

#endif