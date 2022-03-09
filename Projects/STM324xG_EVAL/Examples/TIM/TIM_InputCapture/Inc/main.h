/**
  ******************************************************************************
  * @file    TIM/TIM_InputCapture/Inc/main.h 
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
#include "stm324xg_eval.h"
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* User can use this section to tailor TIMx instance used and associated 
   resources */
/* Definition for TIMx clock resources */
#define TIMx                           TIM1
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM1_CLK_ENABLE()

/* Definition for TIMx Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOE_CLK_ENABLE()
#define GPIO_PORT                      GPIOE
#define GPIO_PIN_CHANNEL2              GPIO_PIN_11
#define GPIO_AF_TIMx                   GPIO_AF1_TIM1

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                      TIM1_CC_IRQn
#define TIMx_IRQHandler                TIM1_CC_IRQHandler


/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */
