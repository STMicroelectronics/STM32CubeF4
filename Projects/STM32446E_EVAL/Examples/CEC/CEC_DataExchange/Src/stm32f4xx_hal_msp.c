
/**
  ******************************************************************************
  * @file    CEC/CEC_DataExchange/Src/stm32f0xx_hal_msp.c 
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

/** @defgroup CEC_DataExchange
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
  * @brief CEC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hcec: CEC handle pointer
  * @retval None
  */  
void HAL_CEC_MspInit(CEC_HandleTypeDef *hcec)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
  
  /* Enable CEC clock */
  __HAL_RCC_CEC_CLK_ENABLE();  
    
  /* Enable GPIO clock and initialize GPIO */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF3_CEC;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Enable and set CEC Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(CEC_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(CEC_IRQn);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select LSE as CEC source clock */
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_CEC;
  RCC_PeriphClkInit.CecClockSelection = RCC_CECCLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
}

/**
  * @brief CEC MSP De-Initialization 
  *        This function disable the Peripheral's clock
  * @param hcec: CEC handle pointer
  * @retval None
  */
void HAL_CEC_MspDeInit(CEC_HandleTypeDef *hcec)
{ 
  /* Disable CEC clock */ 
  __HAL_RCC_CEC_CLK_DISABLE();
  /* Disable GPIO TX/RX clock */
  __HAL_RCC_GPIOB_CLK_DISABLE();
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
