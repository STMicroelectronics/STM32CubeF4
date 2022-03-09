/**
  ******************************************************************************
  * @file    SMARTCARD/SMARTCARD_T0/Src/stm32f4xx_hal_msp.c
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

/** @addtogroup SMARTCARD_T0
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
  * @brief SMARTCARD MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param hsmartcard: SmartCard handle pointer
  * @retval None
  *//**
  * @brief  Initializes I2C MSP.
  * @param  None
  * @retval None
  */
void HAL_SMARTCARD_MspInit(SMARTCARD_HandleTypeDef *hsc)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable Smartcard GPIO clocks */
  SC_USART_TX_CLK_ENABLE();
  SC_USART_CK_CLK_ENABLE();
                         
  /* Enable SmartCard clock */
  SC_USART_CLK_ENABLE();

  /* Configure USART Clock pin as alternate function push-pull */
  GPIO_InitStruct.Pin = SC_USART_CK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = SC_USART_CK_AF;
  HAL_GPIO_Init(SC_USART_CK_GPIO_PORT, &GPIO_InitStruct);
  
  /* Configure USART Tx pin as alternate function open-drain */
  GPIO_InitStruct.Pin = SC_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  HAL_GPIO_Init(SC_USART_TX_GPIO_PORT, &GPIO_InitStruct);

  /* Enable SC_USART IRQ */
  HAL_NVIC_SetPriority(SC_USART_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SC_USART_IRQn);
}

/**
  * @brief SMARTCARD MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_SMARTCARD_MspDeInit(SMARTCARD_HandleTypeDef *hsmartcard)
{
  /*##-1- Reset peripherals ##################################################*/
  SC_USART_FORCE_RESET(); 
  SC_USART_RELEASE_RESET();

  /* Disable SmartCard clock */
  SC_USART_CLK_DISABLE();
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
