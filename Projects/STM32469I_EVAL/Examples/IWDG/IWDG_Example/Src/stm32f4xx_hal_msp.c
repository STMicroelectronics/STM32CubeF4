/**
  ******************************************************************************
  * @file    IWDG/IWDG_Example/Src/stm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  /* TIMx Peripheral clock enable */
  TIMx_CLK_ENABLE();

  /* Configure the NVIC for TIMx */
  HAL_NVIC_SetPriority(TIMx_IRQn, 0, 0);

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIMx_IRQn);
}

/**
  * @brief  DeInitializes TIM Input Capture MSP.
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim)
{
   /* Enable the TIMx global Interrupt */
  HAL_NVIC_DisableIRQ(TIMx_IRQn);
  
  /* TIMx Peripheral clock disable */
  TIMx_CLK_DISABLE();

}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
