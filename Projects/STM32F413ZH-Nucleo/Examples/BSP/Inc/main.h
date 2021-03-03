/**
  ******************************************************************************
  * @file    main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_144.h"

#include "stm32_adafruit_sd.h"
#include "stm32_adafruit_lcd.h"
#include <stdio.h>
#include <stdlib.h>


/* Exported types ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void   (*DemoFunc)(void);
  uint8_t DemoName[30]; 
  uint32_t DemoIndex;
}BSP_DemoTypedef;

extern const unsigned char stlogo[];

/* Exported constants --------------------------------------------------------*/
#define KEY_NOT_PRESSED 0x00
#define KEY_PRESSED     0x01

/* TIM2 Autoreload and Capture Compare register values */
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
void Error_Handler(void);
uint8_t CheckForUserInput(void);

void Joystick_demo (void);
void LCD_demo (void);
void SD_demo (void);


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 

