/**
  ******************************************************************************
  * @file    TIM/TIM_DMABurst/Inc/main.h 
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
#include "stm324xg_eval.h"
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition of TIM instance */
#define TIMx                              TIM1

/* Definition for TIMx clock resources */
#define TIMx_CLK_ENABLE                  __HAL_RCC_TIM1_CLK_ENABLE
#define DMAx_CLK_ENABLE                  __HAL_RCC_DMA2_CLK_ENABLE();

/* Definition for TIMx Pins */
#define TIMx_CHANNEL1_GPIO_CLK_ENABLE    __HAL_RCC_GPIOA_CLK_ENABLE

#define TIMx_GPIO_CHANNEL1_PORT          GPIOA
#define GPIO_PIN_CHANNEL1                GPIO_PIN_8


#define GPIO_AF_TIMx                     GPIO_AF1_TIM1

/* Definition for TIMx's DMA */
#define DMA_CHANNEL_CC1                  DMA_CHANNEL_6
#define TIMx_CC1_DMA_STREAM              DMA2_Stream5

/* Definition for ADCx's NVIC */
#define TIMx_DMA_IRQn                   DMA2_Stream5_IRQn
#define TIMx_DMA_IRQHandler             DMA2_Stream5_IRQHandler

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
