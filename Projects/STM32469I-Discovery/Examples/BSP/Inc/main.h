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
#include <stdio.h>
#include "stm32469i_discovery.h"
#include "stm32469i_discovery_ts.h"
#include "stm32469i_discovery_lcd.h"
#include "stm32469i_discovery_sd.h"
#include "stm32469i_discovery_sdram.h"
#include "stm32469i_discovery_eeprom.h"
#include "stm32469i_discovery_audio.h"
#include "stm32469i_discovery_qspi.h"

/* Macros --------------------------------------------------------------------*/

/* Macros */
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

#define ARBG8888_BYTE_PER_PIXEL   4

/**
  * @brief  LCD FB_StartAddress
  * LCD Frame buffer start address : starts at beginning of SDRAM
  */
#define LCD_FB_LENGTH              ((uint32_t)(OTM8009A_480X800_HEIGHT * OTM8009A_480X800_WIDTH * ARBG8888_BYTE_PER_PIXEL))

/**
  * @brief  SDRAM Write read buffer start address after LCD Frame buffer
  */
#define SDRAM_WRITE_READ_ADDR        ((uint32_t)(LCD_FB_START_ADDRESS + LCD_FB_LENGTH))

#define SDRAM_WRITE_READ_ADDR_OFFSET ((uint32_t)0x0800)

#define AUDIO_REC_START_ADDR         SDRAM_WRITE_READ_ADDR
#define AUDIO_REC_TOTAL_SIZE         ((uint32_t) 0x0000E000)
#define AUDIO_RECPDM_START_ADDR      (AUDIO_REC_START_ADDR+AUDIO_REC_TOTAL_SIZE)

/* The Audio file is flashed with ST-Link Utility @ flash address =  AUDIO_SRC_FILE_ADDRESS */
#define AUDIO_SRC_FILE_ADDRESS       0x08080000   /* Audio file address in flash */
#define AUDIO_FILE_SIZE               0x80000

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[50];
  uint32_t DemoIndex;

} BSP_DemoTypedef;

extern const unsigned char stlogo[];

typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_NOTREADY,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
}AUDIO_ErrorTypeDef;

/* Exported variables ---------------------------------------------------*/
extern volatile uint8_t exti0_received;
extern volatile uint8_t exti2_received;
extern volatile uint8_t exti5_received;
extern LCD_OrientationTypeDef lcd_display_orientation;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define COUNT_OF_EXAMPLE(x)    (sizeof(x)/sizeof(BSP_DemoTypedef))

/* Exported functions ------------------------------------------------------- */
#ifdef JOYSTICK
void Joystick_gpio_demo (void);
void Joystick_exti_demo (void);
void Joystick_SetCursorPosition(void);
#endif

void AudioPlay_demo (void);
void AudioRec_demo (void);

uint8_t  Touchscreen_Calibration (void);
uint16_t TouchScreen_Get_Calibrated_X(uint16_t x);
uint16_t TouchScreen_Get_Calibrated_Y(uint16_t y);
uint8_t  TouchScreen_IsCalibrationDone(void);
void     Touchscreen_DrawBackground_Circle_Buttons(uint8_t state);
uint8_t  TouchScreen_GetTouchPosition(void);
void     Touchscreen_demo1 (void);
void     Touchscreen_demo2 (void);
void     Touchscreen_demo3 (void);

void LCD_demo (void);
void SD_demo (void);
void SD_exti_demo (void);
void Log_demo(void);
void SDRAM_demo(void);
void SDRAM_DMA_demo(void);
void EEPROM_demo(void);
void Animation_demo(void);
uint8_t AUDIO_Play_Process(void);
AUDIO_ErrorTypeDef AUDIO_Play_Start(uint32_t *psrc_address, uint32_t file_size);
void QSPI_demo(void);
uint8_t CheckForUserInput(void);
uint8_t CheckForUserEarlyExit(void);
void Toggle_Leds(void);

#endif /* __MAIN_H */
