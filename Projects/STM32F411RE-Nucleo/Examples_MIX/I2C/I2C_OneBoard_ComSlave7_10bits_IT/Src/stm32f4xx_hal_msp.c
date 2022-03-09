/**
  ******************************************************************************
  * @file    Examples_MIX/I2C/I2C_OneBoard_ComSlave7_10bits_IT/Src/stm32f4xx_hal_msp.c
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

/** @addtogroup STM32F4xx_MIX_Examples
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
  * @brief I2C MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - I2C NVIC configuration
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  if(hi2c->Instance == I2Cx_MASTER)  
  {
    /*##-1- The I2C clock source is derived from the PCLK #*/
    /* The independent clock switching is not supported by this instance */

    /*##-2- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    I2Cx_MASTER_SCL_GPIO_CLK_ENABLE();
    I2Cx_MASTER_SDA_GPIO_CLK_ENABLE();
    /* Enable I2Cx_MASTER clock */
    I2Cx_MASTER_CLK_ENABLE(); 

    /*##-3- Configure peripheral GPIO ##########################################*/  
    /* I2C TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = I2Cx_MASTER_SCL_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = I2Cx_MASTER_SCL_AF;
    HAL_GPIO_Init(I2Cx_MASTER_SCL_GPIO_PORT, &GPIO_InitStruct);
      
    /* I2C RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = I2Cx_MASTER_SDA_PIN;
    GPIO_InitStruct.Alternate = I2Cx_MASTER_SDA_AF;
    HAL_GPIO_Init(I2Cx_MASTER_SDA_GPIO_PORT, &GPIO_InitStruct);
      
    /*##-4- Configure the NVIC for I2C ########################################*/   
    /* NVIC for I2Cx_MASTER */
    HAL_NVIC_SetPriority(I2Cx_MASTER_ER_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(I2Cx_MASTER_ER_IRQn);
    HAL_NVIC_SetPriority(I2Cx_MASTER_EV_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(I2Cx_MASTER_EV_IRQn);
  }
  else /* I2Cx_SLAVE */
  {
    /*##-1- The I2C clock source is derived from the PCLK #*/
    /* The independent clock switching is not supported by this instance */

    /*##-2- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    I2Cx_SLAVE_SCL_GPIO_CLK_ENABLE();
    I2Cx_SLAVE_SDA_GPIO_CLK_ENABLE();
    /* Enable I2Cx_SLAVE clock */
    I2Cx_SLAVE_CLK_ENABLE(); 

    /*##-3- Configure peripheral GPIO ##########################################*/  
    /* I2C TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = I2Cx_SLAVE_SCL_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = I2Cx_SLAVE_SCL_SDA_AF;
    HAL_GPIO_Init(I2Cx_SLAVE_SCL_GPIO_PORT, &GPIO_InitStruct);
      
    /* I2C RX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = I2Cx_SLAVE_SDA_PIN;
    GPIO_InitStruct.Alternate = I2Cx_SLAVE_SCL_SDA_AF;
    HAL_GPIO_Init(I2Cx_SLAVE_SDA_GPIO_PORT, &GPIO_InitStruct);
      
    /*##-4- Configure the NVIC for I2C ########################################*/   
    /* NVIC for I2Cx_SLAVE */
    HAL_NVIC_SetPriority(I2Cx_SLAVE_ER_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(I2Cx_SLAVE_ER_IRQn);
    HAL_NVIC_SetPriority(I2Cx_SLAVE_EV_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(I2Cx_SLAVE_EV_IRQn);
  }
}

/**
  * @brief I2C MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
  if(hi2c->Instance == I2Cx_MASTER)  
  {
    /*##-1- Reset peripherals ##################################################*/
    I2Cx_MASTER_FORCE_RESET();
    I2Cx_MASTER_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks #################################*/
    /* Configure I2C Tx as alternate function  */
    HAL_GPIO_DeInit(I2Cx_MASTER_SCL_GPIO_PORT, I2Cx_MASTER_SCL_PIN);
    /* Configure I2C Rx as alternate function  */
    HAL_GPIO_DeInit(I2Cx_MASTER_SDA_GPIO_PORT, I2Cx_MASTER_SDA_PIN);
    
    /*##-3- Disable the NVIC for I2C ##########################################*/
    HAL_NVIC_DisableIRQ(I2Cx_MASTER_ER_IRQn);
    HAL_NVIC_DisableIRQ(I2Cx_MASTER_EV_IRQn);
  }
  else /* I2Cx_SLAVE */
  {
    /*##-1- Reset peripherals ##################################################*/
    I2Cx_SLAVE_FORCE_RESET();
    I2Cx_SLAVE_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks #################################*/
    /* Configure I2C Tx as alternate function  */
    HAL_GPIO_DeInit(I2Cx_SLAVE_SCL_GPIO_PORT, I2Cx_SLAVE_SCL_PIN);
    /* Configure I2C Rx as alternate function  */
    HAL_GPIO_DeInit(I2Cx_SLAVE_SDA_GPIO_PORT, I2Cx_SLAVE_SDA_PIN);
    
    /*##-3- Disable the NVIC for I2C ##########################################*/
    HAL_NVIC_DisableIRQ(I2Cx_SLAVE_ER_IRQn);
    HAL_NVIC_DisableIRQ(I2Cx_SLAVE_EV_IRQn);
  }
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
