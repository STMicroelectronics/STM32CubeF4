/**
  ******************************************************************************
  * @file    TIM/TIM_OnePulse/Inc/main.h
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
#include "stm32f4xx_nucleo_144.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Definition for TIMx clock resources */
#define TIMx                           TIM3
#define TIMx_CLK_ENABLE                __HAL_RCC_TIM3_CLK_ENABLE

/* Definition for TIMx Pins */
#define TIMx_CHANNEL_GPIO_PORT         __HAL_RCC_GPIOB_CLK_ENABLE
#define TIMx_GPIO_PORT                 GPIOB
#define GPIO_PIN_CHANNEL1              GPIO_PIN_4
#define GPIO_PIN_CHANNEL2              GPIO_PIN_5
#define GPIO_PUPD_CHANNEL1             GPIO_PULLUP
#define GPIO_PUPD_CHANNEL2             GPIO_PULLUP
#define GPIO_AF_TIMx                   GPIO_AF2_TIM3

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
