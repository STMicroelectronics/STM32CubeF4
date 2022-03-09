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
#include "stm32446e_eval.h"
#include "stm32446e_eval_ts.h"
#include "stm32446e_eval_io.h"
#include "stm32446e_eval_lcd.h"
#include "stm32446e_eval_sd.h"
#include "stm32446e_eval_sdram.h"
#include "stm32446e_eval_eeprom.h"
#include "stm32446e_eval_camera.h"
#include "stm32446e_eval_audio.h"
#include "stm32446e_eval_qspi.h"

/* Macros --------------------------------------------------------------------*/




/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[50]; 
  uint32_t DemoIndex;
}BSP_DemoTypedef;

extern const unsigned char stlogo[];
/* Exported global variables ---------------------------------------------------*/
extern volatile uint8_t MfxExtiReceived;
#ifndef USE_FULL_ASSERT
extern uint16_t ErrorCounter;
#endif

/* Exported constants --------------------------------------------------------*/

/* Camera buffer requested space (at max resolution) */
#define CAMERA_RES_MAX_X          480
#define CAMERA_RES_MAX_Y          272
#define RGB565_BYTE_PER_PIXEL     2
/* Memories addresses */
#define FLASH_CODE_ADDRESS         0x08000000   
#define FLASH_DATA_ADDRESS         0x08040000   
#define SRAM_WRITE_READ_ADDR       0x2000b000
#define SDRAM_WRITE_READ_ADDR      SDRAM_DEVICE_ADDR + (CAMERA_RES_MAX_X * CAMERA_RES_MAX_Y * RGB565_BYTE_PER_PIXEL)

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
void Joystick_polling_demo (void);
void Joystick_exti_demo (void);
void Joystick_SetCursorPosition(void);
void Touchscreen_polling_demo (void);
void AudioPlay_demo (void);
void LCD_demo (void);
void SD_demo (void);
void SD_exti_demo (void);
void Log_demo(void);
void SDRAM_demo(void);
void EEPROM_demo(void);
void Camera_demo(void);
void Touchscreen_Calibration (void);
uint16_t Calibration_GetX(uint16_t x);
uint16_t Calibration_GetY(uint16_t y);
uint8_t IsCalibrationDone(void);
uint8_t AUDIO_Process(void);
void QSPI_demo(void);
uint8_t CheckForUserInput(void);
void Toggle_Leds(void);

#endif /* __MAIN_H */
