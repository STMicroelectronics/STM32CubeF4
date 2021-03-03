/**
  ******************************************************************************
  * @file    IWDG/IWDG_Example/Inc/main.h
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
#include "stm32f4xx_nucleo_144.h"


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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
