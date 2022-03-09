/**
  ******************************************************************************
  * @file    IWDG/IWDG_Example/Inc/main.h
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
#include "stm32f4xx_hal.h"
#include "stm32469i_discovery.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition of TIM instance */
#define TIMx                              TIM5

/* Definition for TIMx clock resources */
#define TIMx_CLK_ENABLE()                 __HAL_RCC_TIM5_CLK_ENABLE()
#define TIMx_CLK_DISABLE()                __HAL_RCC_TIM5_CLK_DISABLE()

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                         TIM5_IRQn

/* Definition for TIMx remap */
#define TIMx_REMAP                        TIM_TIM5_LSI
#define TIMx_REMAP_NONE                   TIM_TIM5_NONE
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
