/**
  ******************************************************************************
  * @file    TIM/TIM_DMA/Inc/main.h
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
#include "stm32f413h_discovery.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition of TIM instance */
#define TIMx                             TIM3

/* Definition for TIMx clock resources */
#define TIMx_CLK_ENABLE                  __HAL_RCC_TIM3_CLK_ENABLE
#define DMAx_CLK_ENABLE                  __HAL_RCC_DMA1_CLK_ENABLE

/* Definition for TIMx Pins */
#define TIMx_CHANNEL3_GPIO_CLK_ENABLE    __HAL_RCC_GPIOB_CLK_ENABLE

#define TIMx_GPIO_CHANNEL3_PORT          GPIOB
#define GPIO_PIN_CHANNEL3                GPIO_PIN_0


#define GPIO_AF_TIMx                     GPIO_AF2_TIM3

/* Definition for TIMx's DMA */
#define TIMx_CC3_DMA_INST                DMA1_Stream7

/* Definition for ADCx's NVIC */
#define TIMx_DMA_IRQn                    DMA1_Stream7_IRQn
#define TIMx_DMA_IRQHandler              DMA1_Stream7_IRQHandler

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
