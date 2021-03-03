/**
  ******************************************************************************
  * @file    TIM/TIM_OCInactive/Src/stm32f4xx_hal_msp.c
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
  *           - Peripheral's GPIO Configuration  
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* TIM1 Peripheral clock enable */
  TIMx_CLK_ENABLE();
  
    /* Enable GPIO Clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /* Configure the GPIO pins: used to display the 4 wave forms */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  /* Configure PB.00 to display wave form of channel1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Configure PB.01 to display wave form of channel2 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Configure PB.03 to display wave form of channel3 */  
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Configure PB.05 to display wave form of channel4 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Enable the TIM1 global Interrupt & set priority */
  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
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
