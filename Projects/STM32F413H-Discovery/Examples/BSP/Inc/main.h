/**
  ******************************************************************************
  * @file    BSP/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stm32f413h_discovery.h"
#include "stm32f413h_discovery_lcd.h"
#include "stm32f413h_discovery_sd.h"
#include "stm32f413h_discovery_qspi.h"
#include "stm32f413h_discovery_ts.h"
#include "stm32f413h_discovery_psram.h"
#include "stm32f413h_discovery_audio.h"
/* Macros --------------------------------------------------------------------*/




/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[50]; 
  uint32_t DemoIndex;
}BSP_DemoTypedef;

typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_NOTREADY,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
}AUDIO_ErrorTypeDef;

extern const unsigned char stlogo[];
/* Exported global variables ---------------------------------------------------*/
#ifndef USE_FULL_ASSERT
extern uint16_t ErrorCounter;
#endif
extern uint32_t SdmmcTest;

#ifdef USE_FULL_ASSERT
/* Assert activated */
#define BSP_TEST_APPLI_ASSERT(__error_condition__)    do { if(__error_condition__) \
                                                           {  while(1);  \
                                                           } \
                                                          } while(0)
#else
/* Assert not activated : macro has no effect */
#define BSP_TEST_APPLI_ASSERT(__error_condition__)    do { if(__error_condition__) \
                                                           {  ;  \
                                                           } \
                                                         } while(0)
#endif /* USE_FULL_ASSERT */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

#ifdef USE_FULL_ASSERT
/* Assert activated */
#define ASSERT(__condition__)                do { if(__condition__) \
                                                   {  assert_failed(__FILE__, __LINE__); \
                                                      while(1);  \
                                                    } \
                                              }while(0)
#else
/* Assert not activated : macro has no effect */
#define ASSERT(__condition__)                  do { if(__condition__) \
                                                   {  ErrorCounter++; \
                                                    } \
                                              }while(0)
#endif /* USE_FULL_ASSERT */

/* Exported functions ------------------------------------------------------- */

void Touchscreen_demo1(void);
void Touchscreen_demo2 (void);
void Touchscreen_demo3(void);
void Joystick_demo (void);
void Joystick_exti_demo (void);
void Joystick_SetCursorPosition(void);
void AudioPlay_demo (void);
void AudioRecDfsdm_demo(void);
void LCD_demo (void);
void SD_demo (void);
void SD_DMA_demo (void);
void SD_exti_demo(void);
void QSPI_demo(void);
void PSRAM_demo (void);
void PSRAM_DMA_demo (void);
void Log_demo(void);
uint8_t AUDIO_Process(void);
uint8_t  Touchscreen_Calibration (void);
uint16_t TouchScreen_Get_Calibrated_X(uint16_t x);
uint16_t TouchScreen_Get_Calibrated_Y(uint16_t y);
uint8_t  TouchScreen_IsCalibrationDone(void);
uint8_t TouchScreen_GetTouchPosition(void);
void Touchscreen_DrawBackground_Circles(uint8_t state);
void Touchscreen_DrawControl_Buttons(void);
uint8_t TouchScreen_GetTouchButtonPosition(void);
uint8_t CheckForUserInput(void);
void Toggle_Leds(void);

#endif /* __MAIN_H */
