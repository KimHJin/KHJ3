/**
  ******************************************************************************
  * @file    stm8l15x_clk.h
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    07/14/2010
  * @brief   This file contains all the functions prototypes for the CLK firmware
  *          library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8L15x_CLK_H
#define __STM8L15x_CLK_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/** @addtogroup STM8L15x_StdPeriph_Driver
  * @{
  */
/* Exported macros -----------------------------------------------------------*/
/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/

/** @addtogroup CLK_Exported_Types
  * @{
  */
/**
   * @brief HSE configuration.
   */
typedef enum {
  CLK_HSE_OFF    = (uint8_t)0x00, /*!< HSE Diasble */
  CLK_HSE_ON     = (uint8_t)0x01, /*!< HSE Enable */
  CLK_HSE_Bypass = (uint8_t)0x11  /*!< HSE Bypass and enable */
} CLK_HSE_TypeDef;

/**
   * @brief LSE configuration.
   */
typedef enum {
  CLK_LSE_OFF    = (uint8_t)0x00, /*!< LSE Diasble */
  CLK_LSE_ON     = (uint8_t)0x04, /*!< LSE Enable */
  CLK_LSE_Bypass = (uint8_t)0x24  /*!< LSE Bypass and enable */
} CLK_LSE_TypeDef;

/**
   * @brief System Clock Source.
   */
typedef enum {
  CLK_SYSCLKSource_HSI = (uint8_t)0x01, /*!< System Clock Source HSI */
  CLK_SYSCLKSource_LSI = (uint8_t)0x02, /*!< System Clock Source LSI */
  CLK_SYSCLKSource_HSE = (uint8_t)0x04, /*!< System Clock Source HSE */
  CLK_SYSCLKSource_LSE = (uint8_t)0x08  /*!< System Clock Source LSE */
} CLK_SYSCLKSource_TypeDef;

/**
  * @brief Clock Output Selection
  */
typedef enum {
  CLK_CCOSource_Off = (uint8_t)0x00, /*!< Clock Output Off */
  CLK_CCOSource_HSI = (uint8_t)0x02, /*!< HSI Clock Output  */
  CLK_CCOSource_LSI = (uint8_t)0x04, /*!< LSI Clock Output */
  CLK_CCOSource_HSE = (uint8_t)0x08, /*!< HSE Clock Output */
  CLK_CCOSource_LSE = (uint8_t)0x10  /*!< LSE Clock Output */
} CLK_CCOSource_TypeDef;

/**
  * @brief Clock Output Prescaler
  */
typedef enum {
  CLK_CCODiv_1  = (uint8_t)0x00, /*!< Clock Output Div 1 */
  CLK_CCODiv_2  = (uint8_t)0x20, /*!< Clock Output Div 2 */
  CLK_CCODiv_4  = (uint8_t)0x40, /*!< Clock Output Div 4 */
  CLK_CCODiv_8  = (uint8_t)0x60, /*!< Clock Output Div 8 */
  CLK_CCODiv_16 = (uint8_t)0x80, /*!< Clock Output Div 16 */
  CLK_CCODiv_32 = (uint8_t)0xA0, /*!< Clock Output Div 32 */
  CLK_CCODiv_64 = (uint8_t)0xC0  /*!< Clock Output Div 64 */
} CLK_CCODiv_TypeDef;

/**
  * @brief Clock BEEP Selection
  */
typedef enum {
  CLK_BEEPCLKSource_Off = (uint8_t)0x00, /*!< Clock BEEP Off */
  CLK_BEEPCLKSource_LSI = (uint8_t)0x02, /*!< Clock BEEP : LSI */
  CLK_BEEPCLKSource_LSE = (uint8_t)0x04  /*!< Clock BEEP : LSE */
} CLK_BEEPCLKSource_TypeDef;

/**
  * @brief Clock RTC Selection
  */
typedef enum {
  CLK_RTCCLKSource_Off = (uint8_t)0x00, /*!< Clock RTC Off */
  CLK_RTCCLKSource_HSI = (uint8_t)0x02, /*!< Clock RTC : HSI */
  CLK_RTCCLKSource_LSI = (uint8_t)0x04, /*!< Clock RTC : LSI */
  CLK_RTCCLKSource_HSE = (uint8_t)0x08, /*!< Clock RTC : HSE */
  CLK_RTCCLKSource_LSE = (uint8_t)0x10  /*!< Clock RTC : LSE */
} CLK_RTCCLKSource_TypeDef;

/**
  * @brief Clock RTC Prescaler
  */
typedef enum {
  CLK_RTCCLKDiv_1  = (uint8_t)0x00, /*!< Clock RTC Div 1 */
  CLK_RTCCLKDiv_2  = (uint8_t)0x20, /*!< Clock RTC Div 2  */
  CLK_RTCCLKDiv_4  = (uint8_t)0x40, /*!< Clock RTC Div 4 */
  CLK_RTCCLKDiv_8  = (uint8_t)0x60, /*!< Clock RTC Div 8 */
  CLK_RTCCLKDiv_16 = (uint8_t)0x80, /*!< Clock RTC Div 16 */
  CLK_RTCCLKDiv_32 = (uint8_t)0xA0, /*!< Clock RTC  Div 32 */
  CLK_RTCCLKDiv_64 = (uint8_t)0xC0  /*!< Clock RTC  Div 64 */
} CLK_RTCCLKDiv_TypeDef;


/**
  * @brief Enable peripheral PCKEN
  */
/* Elements values convention: 0xXY
    X = choice between the peripheral registers
        X = 0 : PCKENR1
        X = 1 : PCKENR2
    Y = Peripheral position in the register
*/
typedef enum {
  CLK_Peripheral_TIM2    = (uint8_t)0x00, /*!< Peripheral Clock Enable 1, TIM2 */
  CLK_Peripheral_TIM3    = (uint8_t)0x01, /*!< Peripheral Clock Enable 1, TIM3 */
  CLK_Peripheral_TIM4    = (uint8_t)0x02, /*!< Peripheral Clock Enable 1, TIM4 */
  CLK_Peripheral_I2C1    = (uint8_t)0x03, /*!< Peripheral Clock Enable 1, I2C1 */
  CLK_Peripheral_SPI1    = (uint8_t)0x04, /*!< Peripheral Clock Enable 1, SPI1 */
  CLK_Peripheral_USART1  = (uint8_t)0x05, /*!< Peripheral Clock Enable 1, USART1 */
  CLK_Peripheral_BEEP    = (uint8_t)0x06, /*!< Peripheral Clock Enable 1, BEEP */
  CLK_Peripheral_DAC     = (uint8_t)0x07, /*!< Peripheral Clock Enable 1, DAC */
  CLK_Peripheral_ADC1    = (uint8_t)0x10, /*!< Peripheral Clock Enable 2, ADC1 */
  CLK_Peripheral_TIM1    = (uint8_t)0x11, /*!< Peripheral Clock Enable 2, TIM1 */
  CLK_Peripheral_RTC     = (uint8_t)0x12, /*!< Peripheral Clock Enable 2, RTC */
  CLK_Peripheral_LCD     = (uint8_t)0x13, /*!< Peripheral Clock Enable 2, LCD */
  CLK_Peripheral_DMA1    = (uint8_t)0x14, /*!< Peripheral Clock Enable 2, DMA1 */
  CLK_Peripheral_COMP    = (uint8_t)0x15, /*!< Peripheral Clock Enable 2, COMP1 and COMP2 */
  CLK_Peripheral_BOOTROM = (uint8_t)0x17 /*!< Peripheral Clock Enable 2, Boot ROM */
} CLK_Peripheral_TypeDef;

/**
  * @brief System Clock Divider
  */
typedef enum {
  CLK_SYSCLKDiv_1   = (uint8_t)0x00, /*!< System Clock Divider: 1 */
  CLK_SYSCLKDiv_2   = (uint8_t)0x01, /*!< System Clock Divider: 2 */
  CLK_SYSCLKDiv_4   = (uint8_t)0x02, /*!< System Clock Divider: 4 */
  CLK_SYSCLKDiv_8   = (uint8_t)0x03, /*!< System Clock Divider: 8 */
  CLK_SYSCLKDiv_16  = (uint8_t)0x04, /*!< System Clock Divider: 16 */
  CLK_SYSCLKDiv_32  = (uint8_t)0x05, /*!< System Clock Divider: 32 */
  CLK_SYSCLKDiv_64  = (uint8_t)0x06, /*!< System Clock Divider: 64 */
  CLK_SYSCLKDiv_128 = (uint8_t)0x07  /*!< System Clock Divider: 128 */
} CLK_SYSCLKDiv_TypeDef;

/**
   * @brief Flags.
   */
/* Elements values convention: 0xXY
    X = choice between the register's flags
       X = 0 : CRTCR
       X = 1 : ICKCR
       X = 2 : CCOR
       X = 3 : ECKCR
       X = 4 : SWCR
       X = 5 : CSSR
       X = 6 : CBEEPR
       X = 7 : REGCSRR
  Y = flag position in the register
*/
typedef enum {
  CLK_FLAG_RTCSWBSY  = (uint8_t)0x00, /*!< RTC clock busy in switch Flag */
  CLK_FLAG_HSIRDY    = (uint8_t)0x11, /*!< High speed internal oscillator ready Flag */
  CLK_FLAG_LSIRDY    = (uint8_t)0x13, /*!< Low speed internal oscillator ready Flag */
  CLK_FLAG_CCOBSY    = (uint8_t)0x20, /*!< Configurable clock output busy */
  CLK_FLAG_HSERDY    = (uint8_t)0x31, /*!< High speed external oscillator ready Flag */
  CLK_FLAG_LSERDY    = (uint8_t)0x33, /*!< Low speed external oscillator ready Flag */
  CLK_FLAG_SWBSY     = (uint8_t)0x40, /*!< Switch busy Flag */
  CLK_FLAG_AUX       = (uint8_t)0x51, /*!< Auxiliary oscillator connected to master clock */
  CLK_FLAG_CSSD      = (uint8_t)0x53, /*!< Clock security system detection Flag */
  CLK_FLAG_BEEPSWBSY = (uint8_t)0x60, /*!< BEEP clock busy in switch Flag*/
  CLK_FLAG_EEREADY   = (uint8_t)0x77, /*!< Flash program memory and Data EEPROM ready Flag */
  CLK_FLAG_EEBUSY    = (uint8_t)0x76, /*!< Flash program memory and Data EEPROM busy Flag */
  CLK_FLAG_LSEPD     = (uint8_t)0x75, /*!< LSE power-down Flag */
  CLK_FLAG_HSEPD     = (uint8_t)0x74, /*!< HSE power-down Flag */
  CLK_FLAG_LSIPD     = (uint8_t)0x73, /*!< LSI power-down Flag */
  CLK_FLAG_HSIPD     = (uint8_t)0x72, /*!< HSI power-down Flag */
  CLK_FLAG_REGREADY  = (uint8_t)0x70  /*!< REGREADY Flag */
}CLK_FLAG_TypeDef;

/**
   * @brief interrupt configuration and Flags cleared by software.
   */
typedef enum {
  CLK_IT_CSSD = (uint8_t)0x0C, /*!< Clock security system detection Flag */
  CLK_IT_SWIF = (uint8_t)0x1C  /*!< Clock switch interrupt Flag */
}CLK_IT_TypeDef;

/**
   * @brief Halt wake up configuration.
   */
typedef enum {
  CLK_Halt_BEEPRunning = (uint8_t)0x40, /*!< BEEP clock Halt/Active-halt mode */
  CLK_Halt_FastWakeup  = (uint8_t)0x20, /*!< Fast wakeup from Halt/Active-halt modes */
  CLK_Halt_SlowWakeup  = (uint8_t)0x10  /*!< Slow Active-halt mode */
}
CLK_Halt_TypeDef;

/**
  * @}
  */


/** @addtogroup CLK_Private_Macros
  * @{
  */

/**
  * @brief Macro used by the assert function to check the different functions parameters.
  */

/**
 * @brief Macros used by the assert function in order to check the different HSE clock source configuration.
 */
#define IS_CLK_HSE(CONFIG) (((CONFIG) == CLK_HSE_ON) ||\
                            ((CONFIG) == CLK_HSE_OFF) ||\
                            ((CONFIG) == CLK_HSE_Bypass))


/**
 * @brief Macros used by the assert function in order to check the different LSE clock source configuration.
 */
#define IS_CLK_LSE(CONFIG) (((CONFIG) == CLK_LSE_OFF) ||\
                            ((CONFIG) == CLK_LSE_ON) ||\
                            ((CONFIG) == CLK_LSE_Bypass))

/**
 * @brief Macros used by the assert function in order to check the different clock sources.
 */
#define IS_CLK_SOURCE(SOURCE) (((SOURCE) == CLK_SYSCLKSource_HSI) ||\
                               ((SOURCE) == CLK_SYSCLKSource_LSI) ||\
                               ((SOURCE) == CLK_SYSCLKSource_HSE) ||\
                               ((SOURCE) == CLK_SYSCLKSource_LSE))

/**
  * @brief  Macro used by the assert function to check the different clock to output.
  */
#define IS_CLK_OUTPUT(OUTPUT) \
  (((OUTPUT) == CLK_CCOSource_Off)  ||\
   ((OUTPUT) == CLK_CCOSource_HSI)  ||\
   ((OUTPUT) == CLK_CCOSource_LSI) ||\
   ((OUTPUT) == CLK_CCOSource_HSE) ||\
   ((OUTPUT) == CLK_CCOSource_LSE))

/**
  * @brief  Macro used by the assert function to check the different clock output dividers.
  */
#define IS_CLK_OUTPUT_DIVIDER(PRESCALER) \
  (((PRESCALER) == CLK_CCODiv_1)  ||\
   ((PRESCALER) == CLK_CCODiv_2)  ||\
   ((PRESCALER) == CLK_CCODiv_4) ||\
   ((PRESCALER) == CLK_CCODiv_8) ||\
   ((PRESCALER) == CLK_CCODiv_16) ||\
   ((PRESCALER) == CLK_CCODiv_32) ||\
   ((PRESCALER) == CLK_CCODiv_64))

/**
  * @brief  Macro used by the assert function to check the different BEEP clock sources.
  */
#define IS_CLK_CLOCK_BEEP(OUTPUT) \
  (((OUTPUT) == CLK_BEEPCLKSource_Off)  ||\
   ((OUTPUT) == CLK_BEEPCLKSource_LSI) ||\
   ((OUTPUT) == CLK_BEEPCLKSource_LSE))

/**
  * @brief  Macro used by the assert function to check the different RTC clock sources.
  */
#define IS_CLK_CLOCK_RTC(OUTPUT) \
  (((OUTPUT) == CLK_RTCCLKSource_Off)  ||\
   ((OUTPUT) == CLK_RTCCLKSource_HSI)  ||\
   ((OUTPUT) == CLK_RTCCLKSource_LSI) ||\
   ((OUTPUT) == CLK_RTCCLKSource_HSE) ||\
   ((OUTPUT) == CLK_RTCCLKSource_LSE))

/**
  * @brief  Macro used by the assert function to check the different RTC clock dividers.
  */
#define IS_CLK_CLOCK_RTC_DIV(DIV) \
  (((DIV) == CLK_RTCCLKDiv_1)  ||\
   ((DIV) == CLK_RTCCLKDiv_2)  ||\
   ((DIV) == CLK_RTCCLKDiv_4) ||\
   ((DIV) == CLK_RTCCLKDiv_8) ||\
   ((DIV) == CLK_RTCCLKDiv_16) ||\
   ((DIV) == CLK_RTCCLKDiv_32) ||\
   ((DIV) == CLK_RTCCLKDiv_64))


/**
  * @brief  Macro used by the assert function to check the different periheral to clock.
  */
#define IS_CLK_PERIPHERAL(PERIPHERAL) \
  (((PERIPHERAL) == CLK_Peripheral_DAC)    ||\
   ((PERIPHERAL) == CLK_Peripheral_ADC1)    ||\
   ((PERIPHERAL) == CLK_Peripheral_DMA1)    ||\
   ((PERIPHERAL) == CLK_Peripheral_RTC)    ||\
   ((PERIPHERAL) == CLK_Peripheral_LCD)    ||\
   ((PERIPHERAL) == CLK_Peripheral_COMP)    ||\
   ((PERIPHERAL) == CLK_Peripheral_TIM1)    ||\
   ((PERIPHERAL) == CLK_Peripheral_USART1)  ||\
   ((PERIPHERAL) == CLK_Peripheral_SPI1)    ||\
   ((PERIPHERAL) == CLK_Peripheral_I2C1)    ||\
   ((PERIPHERAL) == CLK_Peripheral_TIM4) ||\
   ((PERIPHERAL) == CLK_Peripheral_TIM3) ||\
   ((PERIPHERAL) == CLK_Peripheral_BEEP)   ||\
   ((PERIPHERAL) == CLK_Peripheral_BOOTROM)   ||\
   ((PERIPHERAL) == CLK_Peripheral_TIM2))

/**
  * @brief  Macro used by the assert function to check the different clock prescaler.
  */
#define IS_CLK_SYSTEM_DIVIDER(DIV) \
  (((DIV) == CLK_SYSCLKDiv_1) ||\
   ((DIV) == CLK_SYSCLKDiv_2) ||\
   ((DIV) == CLK_SYSCLKDiv_4) ||\
   ((DIV) == CLK_SYSCLKDiv_8) ||\
   ((DIV) == CLK_SYSCLKDiv_16) ||\
   ((DIV) == CLK_SYSCLKDiv_32) ||\
   ((DIV) == CLK_SYSCLKDiv_64) ||\
   ((DIV) == CLK_SYSCLKDiv_128))

/**
  * @brief Macros used by the assert function in order to check the different clock flags.
  */
#define IS_CLK_FLAGS(FLAG) (((FLAG) == CLK_FLAG_LSIRDY) ||\
                            ((FLAG) == CLK_FLAG_HSIRDY) ||\
                            ((FLAG) == CLK_FLAG_HSERDY) ||\
                            ((FLAG) == CLK_FLAG_SWBSY) ||\
                            ((FLAG) == CLK_FLAG_CSSD) ||\
                            ((FLAG) == CLK_FLAG_AUX) ||\
                            ((FLAG) == CLK_FLAG_LSERDY) ||\
                            ((FLAG) == CLK_FLAG_CCOBSY) ||\
                            ((FLAG) == CLK_FLAG_RTCSWBSY) ||\
                            ((FLAG) == CLK_FLAG_EEREADY) ||\
                            ((FLAG) == CLK_FLAG_EEBUSY) ||\
                            ((FLAG) == CLK_FLAG_LSEPD) ||\
                            ((FLAG) == CLK_FLAG_LSIPD) ||\
                            ((FLAG) == CLK_FLAG_HSEPD) ||\
                            ((FLAG) == CLK_FLAG_HSIPD) ||\
                            ((FLAG) == CLK_FLAG_REGREADY) ||\
                            ((FLAG) == CLK_FLAG_BEEPSWBSY))

/**
  * @brief Macros used by the assert function in order to check the different clock interrupt sources.
  */
#define IS_CLK_IT(IT) (((IT) == CLK_IT_CSSD) || ((IT) == CLK_IT_SWIF))

/**
  * @brief Macros used by the assert function in order to check the different clock IT pending bits.
  */
#define IS_CLK_CLEAR_IT(IT) ((IT) == CLK_IT_SWIF)

/**
  * @brief Macros used by the assert function in order to check the different clock Halt config.
  */
#define IS_CLK_HALT(HALT) (((HALT) == CLK_Halt_BEEPRunning) ||\
                           ((HALT) == CLK_Halt_FastWakeup) ||\
                           ((HALT) == CLK_Halt_SlowWakeup))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */
/** @addtogroup CLK_Exported_Functions
  * @{
  */
void CLK_DeInit(void);
void CLK_HSICmd(FunctionalState NewState);
void CLK_AdjustHSICalibrationValue(uint8_t CLK_HSICalibrationValue);
void CLK_LSICmd(FunctionalState NewState);
void CLK_HSEConfig(CLK_HSE_TypeDef CLK_HSE);
void CLK_LSEConfig(CLK_LSE_TypeDef CLK_LSE);
void CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_TypeDef CLK_SYSCLKSource);
void CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_TypeDef CLK_SYSCLKDiv);
void CLK_SYSCLKSourceSwitchCmd(FunctionalState NewState);
CLK_SYSCLKSource_TypeDef CLK_GetSYSCLKSource(void);
void CLK_ClockSecuritySystemEnable(void);
uint32_t CLK_GetClockFreq(void);
void CLK_ITConfig(CLK_IT_TypeDef CLK_IT, FunctionalState NewState);
void CLK_CCOConfig(CLK_CCOSource_TypeDef CLK_CCOSource, CLK_CCODiv_TypeDef CLK_CCODiv);
void CLK_RTCClockConfig(CLK_RTCCLKSource_TypeDef CLK_RTCCLKSource, CLK_RTCCLKDiv_TypeDef CLK_RTCCLKDiv);
void CLK_BEEPClockConfig(CLK_BEEPCLKSource_TypeDef CLK_BEEPCLKSource);
void CLK_PeripheralClockConfig(CLK_Peripheral_TypeDef CLK_Peripheral, FunctionalState NewState);
void CLK_HaltConfig(CLK_Halt_TypeDef CLK_Halt, FunctionalState NewState);
void CLK_MainRegulatorCmd(FunctionalState NewState);
FlagStatus CLK_GetFlagStatus(CLK_FLAG_TypeDef CLK_FLAG);
ITStatus CLK_GetITStatus(CLK_IT_TypeDef CLK_IT);
void CLK_ClearITPendingBit(CLK_IT_TypeDef CLK_IT);

/**
  * @}
  */

#endif /* __STM8L15x_CLK_H */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
