/**
  ******************************************************************************
  * @file    ADC/ADC_InjectedConversion_Interrupt/Src/stm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
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

/** @defgroup ADC_InjectedConversion_Interrupt
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
  * @brief ADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* ADC3 Periph clock enable */
  ADCx_CLK_ENABLE();
  /* Enable GPIO clock ****************************************/
  ADCx_REG_CHANNEL_GPIO_CLK_ENABLE();
  ADCx_INJ_CHANNEL_GPIO_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* ADC ADC_CHANNEL_10 GPIO pin configuration */
  GPIO_InitStruct.Pin = ADCx_REG_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_REG_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

  /* ADC ADC_CHANNEL_12 GPIO pin configuration */
  GPIO_InitStruct.Pin = ADCx_INJ_CHANNEL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_INJ_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC #################################################*/
  /* NVIC configuration for conversion complete interrupt */
  HAL_NVIC_SetPriority(ADCx_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(ADCx_IRQn);
}

/**
  * @brief ADC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{

  /*##-1- Reset peripherals ##################################################*/
  ADCx_FORCE_RESET();
  ADCx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the ADC ADC_CHANNEL_10 GPIO pin */
  HAL_GPIO_DeInit(ADCx_REG_CHANNEL_GPIO_PORT, ADCx_REG_CHANNEL_PIN);
  /* De-initialize the ADC ADC_CHANNEL_12 GPIO pin */
  HAL_GPIO_DeInit(ADCx_INJ_CHANNEL_GPIO_PORT, ADCx_INJ_CHANNEL_PIN);
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
