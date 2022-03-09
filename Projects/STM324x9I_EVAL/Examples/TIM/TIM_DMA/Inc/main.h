/**
  ******************************************************************************
  * @file    TIM/TIM_DMA/Inc/main.h 
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
#include "stm324x9i_eval.h"
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition of TIM instance */
#define TIMx                              TIM1

/* Definition for TIMx clock resources */
#define TIMx_CLK_ENABLE                  __HAL_RCC_TIM1_CLK_ENABLE
#define DMAx_CLK_ENABLE                  __HAL_RCC_DMA2_CLK_ENABLE();

/* Definition for TIMx Pins */
#define TIMx_CHANNEL3_GPIOCLK_ENABLE    __HAL_RCC_GPIOE_CLK_ENABLE
#define TIMx_CHANNEL3N_GPIOCLK_ENABLE   __HAL_RCC_GPIOB_CLK_ENABLE

#define TIMx_GPIO_CHANNEL3_PORT          GPIOE
#define GPIO_PIN_CHANNEL3                GPIO_PIN_13

#define TIMx_GPIO_CHANNEL3N_PORT         GPIOB
#define GPIO_PIN_CHANNEL3N               GPIO_PIN_15

#define GPIO_AF_TIMx                     GPIO_AF1_TIM1


/* Definition for TIMx's DMA */
#define DMA_CHANNEL_CC3                  DMA_CHANNEL_6
#define TIMx_CC3_DMA_STREAM              DMA2_Stream6

/* Definition for ADCx's NVIC */
#define TIMx_DMA_IRQn                   DMA2_Stream6_IRQn
#define TIMx_DMA_IRQHandler             DMA2_Stream6_IRQHandler

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */
