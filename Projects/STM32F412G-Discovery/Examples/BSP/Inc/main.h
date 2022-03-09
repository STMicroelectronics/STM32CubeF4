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
#include "stm32412g_discovery.h"
#include "stm32412g_discovery_lcd.h"
#include "stm32412g_discovery_sd.h"
#include "stm32412g_discovery_eeprom.h"
#include "stm32412g_discovery_audio.h"
#include "stm32412g_discovery_qspi.h"
#include "stm32412g_discovery_ts.h"
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
/* Exported constants --------------------------------------------------------*/

/* Memories addresses */
#define FLASH_CODE_ADDRESS         0x08000000   
#define FLASH_DATA_ADDRESS         0x08040000   
#define SRAM_WRITE_READ_ADDR       0x2000b000

#define AUDIO_REC_START_ADDR       0x20030000


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
void Joystick_demo (void);
void Joystick_SetCursorPosition(void);
void AudioPlay_demo (void);
void AudioRecAnalog_demo(void);
void LCD_demo (void);
void TS_demo(void);
void SD_demo (void);
void SD_DMA_demo (void);
void SD_exti_demo(void);
void Log_demo(void);
void EEPROM_demo(void);
uint8_t AUDIO_Process(void);
void QSPI_demo(void);
uint8_t CheckForUserInput(void);
void Toggle_Leds(void);
void AudioRecDfsdm_demo(void);
#endif /* __MAIN_H */
