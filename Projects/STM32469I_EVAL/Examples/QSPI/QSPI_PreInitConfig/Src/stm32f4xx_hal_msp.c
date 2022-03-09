/**
  ******************************************************************************
  * @file    QSPI/QSPI_ExecuteInPlace/Src/stm32f4xx_hal_msp.c
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
  * @brief QSPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for requests by peripheral 
  *           - NVIC configuration for DMA and QSPI interrupts
  * @param hqspi: QSPI handle pointer
  * @retval None
  */
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{

//  /*##-1- Enable peripherals and GPIO Clocks #################################*/
//  /* Enable the QuadSPI memory interface clock */
//  QSPI_CLK_ENABLE();
//  /* Reset the QuadSPI memory interface */
//  QSPI_FORCE_RESET();
//  QSPI_RELEASE_RESET();
//  /* Enable GPIO clocks */
//  QSPI_CS_GPIO_CLK_ENABLE();
//  QSPI_CLK_GPIO_CLK_ENABLE();
//  QSPI_D0_GPIO_CLK_ENABLE();
//  QSPI_D1_GPIO_CLK_ENABLE();
//  QSPI_D2_GPIO_CLK_ENABLE();
//  QSPI_D3_GPIO_CLK_ENABLE();



//  /*##-2- Configure peripheral GPIO ##########################################*/
//  /* QSPI CS GPIO pin configuration  */
//  GPIO_InitStruct.Pin       = QSPI_CS_PIN;
//  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull      = GPIO_PULLUP;
//  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
//  GPIO_InitStruct.Alternate = GPIO_AF10_QSPI;
//  HAL_GPIO_Init(QSPI_CS_GPIO_PORT, &GPIO_InitStruct);
//
//  /* QSPI CLK GPIO pin configuration  */
//  GPIO_InitStruct.Pin       = QSPI_CLK_PIN;
//  GPIO_InitStruct.Pull      = GPIO_NOPULL;
//  GPIO_InitStruct.Alternate = GPIO_AF9_QSPI;
//  HAL_GPIO_Init(QSPI_CLK_GPIO_PORT, &GPIO_InitStruct);
//
//  /* QSPI D0 GPIO pin configuration  */
//  GPIO_InitStruct.Pin       = QSPI_D0_PIN;
//  GPIO_InitStruct.Alternate = GPIO_AF10_QSPI;
//  HAL_GPIO_Init(QSPI_D0_GPIO_PORT, &GPIO_InitStruct);
//
//  /* QSPI D1 GPIO pin configuration  */
//  GPIO_InitStruct.Pin       = QSPI_D1_PIN;
//  GPIO_InitStruct.Alternate = GPIO_AF10_QSPI;
//  HAL_GPIO_Init(QSPI_D1_GPIO_PORT, &GPIO_InitStruct);
//
//  /* QSPI D2 GPIO pin configuration  */
//  GPIO_InitStruct.Pin       = QSPI_D2_PIN;
//  GPIO_InitStruct.Alternate = GPIO_AF9_QSPI;
//  HAL_GPIO_Init(QSPI_D2_GPIO_PORT, &GPIO_InitStruct);
//
//  /* QSPI D3 GPIO pin configuration  */
//  GPIO_InitStruct.Pin       = QSPI_D3_PIN;
//  GPIO_InitStruct.Alternate = GPIO_AF9_QSPI;
//  HAL_GPIO_Init(QSPI_D3_GPIO_PORT, &GPIO_InitStruct);
//  HAL_GPIO_Init(QSPI_D3_GPIO_PORT, &GPIO_InitStruct);
  
    /*--------------------------------------------------------------------------*/
  /*------------------ Activation of the peripheral clocks -------------------*/
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /*--------------------- Configuration of the I/O pins ----------------------*/
  /*--------------------------------------------------------------------------*/
  /* Configure alternate function selection for IO pins */
  /* - GPIOF09 and GPIOF08 as AF10 */
  GPIOF->AFR[1] = 0x000009AA;
  /* - GPIOF06 and GPIOF07 as AF09 */
  GPIOF->AFR[0] = 0x99000000;
  /* - GPIOB02 as AF09 */
  GPIOB->AFR[0] = 0x0A000000;


  /* Configure alternate function mode for IO pins */
  /* - GPIOF09, GPIOF08, GPIOF07 and GPIOF06 Alternate function mode */
  GPIOF->MODER = 0x002AA000;
  /* - GPIOB02 Alternate function mode */
  GPIOB->MODER = 0x00002280;
  
  /* Configure output speed for IO pins */
  /* - GPIOF09, GPIOF08, GPIOF07 and GPIOF06 as Very high speed */
  GPIOF->OSPEEDR = 0x003FF000;
  /* - GPIOB02 as Very high speed */
  GPIOB->OSPEEDR = 0x000030C0;
 
  /* Configure IDR register */
  GPIOF->IDR = 0x000003C0;
  GPIOB->IDR = 0x000003D8;
  
  /* Configure pull-up or pull-down for IO pins */
  GPIOF->PUPDR = 0x00000000;
  GPIOB->PUPDR = 0x00001100;
//
//  /*##-3- Configure the NVIC for QSPI #########################################*/
//  /* NVIC configuration for QSPI interrupt */
//  HAL_NVIC_SetPriority(QUADSPI_IRQn, 0x0F, 0);
//  HAL_NVIC_EnableIRQ(QUADSPI_IRQn);
//
//  /*##-4- Configure the DMA channel ###########################################*/
//  /* QSPI DMA channel configuration */
//  hdma.Init.Channel             = QSPI_DMA_CHANNEL;                     
//  hdma.Init.PeriphInc           = DMA_PINC_DISABLE;
//  hdma.Init.MemInc              = DMA_MINC_ENABLE;
//  hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//  hdma.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
//  hdma.Init.Mode                = DMA_NORMAL;
//  hdma.Init.Priority            = DMA_PRIORITY_LOW;
//  hdma.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;        /* FIFO mode disabled     */
//  hdma.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
//  hdma.Init.MemBurst            = DMA_MBURST_SINGLE;           /* Memory burst           */
//  hdma.Init.PeriphBurst         = DMA_PBURST_SINGLE;           /* Peripheral burst       */
//  hdma.Instance                 = QSPI_DMA_INSTANCE;
//
//  __HAL_LINKDMA(hqspi, hdma, hdma);
//  HAL_DMA_Init(&hdma);
//
//  /* NVIC configuration for DMA interrupt */
//  HAL_NVIC_SetPriority(QSPI_DMA_IRQ, 0x00, 0);
//  HAL_NVIC_EnableIRQ(QSPI_DMA_IRQ);

}

/**
  * @brief QSPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hqspi: QSPI handle pointer
  * @retval None
  */
void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef *hqspi)
{
  static DMA_HandleTypeDef hdma;

  /*##-1- Disable the NVIC for QSPI and DMA ##################################*/
  HAL_NVIC_DisableIRQ(QSPI_DMA_IRQ);
  HAL_NVIC_DisableIRQ(QUADSPI_IRQn);

  /*##-2- Disable peripherals ################################################*/
  /* De-configure DMA channel */
  hdma.Instance = QSPI_DMA_INSTANCE;
  HAL_DMA_DeInit(&hdma);
  /* De-Configure QSPI pins */
  HAL_GPIO_DeInit(QSPI_CS_GPIO_PORT, QSPI_CS_PIN);
  HAL_GPIO_DeInit(QSPI_CLK_GPIO_PORT, QSPI_CLK_PIN);
  HAL_GPIO_DeInit(QSPI_D0_GPIO_PORT, QSPI_D0_PIN);
  HAL_GPIO_DeInit(QSPI_D1_GPIO_PORT, QSPI_D1_PIN);
  HAL_GPIO_DeInit(QSPI_D2_GPIO_PORT, QSPI_D2_PIN);
  HAL_GPIO_DeInit(QSPI_D3_GPIO_PORT, QSPI_D3_PIN);

  /*##-3- Reset peripherals ##################################################*/
  /* Reset the QuadSPI memory interface */
  QSPI_FORCE_RESET();
  QSPI_RELEASE_RESET();

  /* Disable the QuadSPI memory interface clock */
  QSPI_CLK_DISABLE();
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
