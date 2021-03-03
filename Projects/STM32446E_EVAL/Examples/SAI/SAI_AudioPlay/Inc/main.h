/**
  ******************************************************************************
  * @file    SAI/SAI_AudioPlay/Inc/main.h
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
#include "stm32f4xx_hal.h"
#include "stm32446e_eval.h"
#include "audio.h"
#include "../Components/wm8994/wm8994.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

  
/* SAI peripheral configuration defines */
#define AUDIO_SAIx                           SAI2_Block_B
#define AUDIO_SAIx_CLK_ENABLE()              __HAL_RCC_SAI2_CLK_ENABLE()

#define AUDIO_SAIx_FS_GPIO_PORT              GPIOG
#define AUDIO_SAIx_FS_AF                     GPIO_AF10_SAI2
#define AUDIO_SAIx_FS_PIN                    GPIO_PIN_9
#define AUDIO_SAIx_SCK_GPIO_PORT             GPIOA
#define AUDIO_SAIx_SCK_AF                    GPIO_AF8_SAI2
#define AUDIO_SAIx_SCK_PIN                   GPIO_PIN_2
#define AUDIO_SAIx_SD_GPIO_PORT              GPIOG
#define AUDIO_SAIx_SD_AF                     GPIO_AF10_SAI2
#define AUDIO_SAIx_SD_PIN                    GPIO_PIN_10
#define AUDIO_SAIx_MCLK_GPIO_PORT            GPIOA
#define AUDIO_SAIx_MCLK_AF                   GPIO_AF10_SAI2
#define AUDIO_SAIx_MCLK_PIN                  GPIO_PIN_1
   
#define AUDIO_SAIx_MCLK_ENABLE()             __HAL_RCC_GPIOA_CLK_ENABLE()
#define AUDIO_SAIx_SCK_ENABLE()              __HAL_RCC_GPIOA_CLK_ENABLE()
#define AUDIO_SAIx_FS_ENABLE()               __HAL_RCC_GPIOG_CLK_ENABLE()
#define AUDIO_SAIx_SD_ENABLE()               __HAL_RCC_GPIOG_CLK_ENABLE()
   

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
