/**
  ******************************************************************************
  * @file    TIM/TIM_OnePulse/Inc/main.h
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


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Definition for TIMx clock resources */
#define TIMx                           TIM5
#define TIMx_CLK_ENABLE                __HAL_RCC_TIM5_CLK_ENABLE

/* Definition for TIMx Pins */
#define TIMx_CHANNEL_GPIO_PORT         __HAL_RCC_GPIOA_CLK_ENABLE
#define TIMx_GPIO_PORT                 GPIOA
#define GPIO_PIN_CHANNEL1              GPIO_PIN_0
#define GPIO_PIN_CHANNEL2              GPIO_PIN_1
#define GPIO_PUPD_CHANNEL1             GPIO_PULLUP
#define GPIO_PUPD_CHANNEL2             GPIO_PULLUP
#define GPIO_AF_TIMx                   GPIO_AF2_TIM5

/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
