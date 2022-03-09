/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Raw/Src/tm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  *         
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
/**
  * @brief  ADC_MspInit
  * @param  None
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *heth)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   
   __HAL_RCC_GPIOF_CLK_ENABLE();
   
   /* Configure ADC Channel 7 as analog input */
  GPIO_InitStructure.Pin = GPIO_PIN_9;/**/
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
   
   /* ADC3 Periph clock enable */
   __HAL_RCC_ADC3_CLK_ENABLE();
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
