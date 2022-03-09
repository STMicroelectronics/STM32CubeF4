/**
  ******************************************************************************
  * @file    stm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  *         
  @verbatim
  ===============================================================================
  ##### How to use this driver #####
  ===============================================================================
  [..]
  This file is generated automatically by STM32CubeMX and eventually modified 
  by the user
  
  @endverbatim
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
#include "stm32f4xx_hal.h"

/** @addtogroup STM32F4xx_HAL_Driver
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
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
  /* NOTE : This function is generated automatically by STM32CubeMX and eventually  
  modified by the user
  */
  /* Enable UART 4 clock  */
  __HAL_RCC_UART4_CLK_ENABLE();
  
  /* RNG Periph clock enable */
  __HAL_RCC_RNG_CLK_ENABLE();
  
#ifdef USE_STM32F4XX_HW_CRYPTO   
  /* Enable CRYP clock */
  __HAL_RCC_CRYP_CLK_ENABLE();
    
  /* Enable Hash clock */
  __HAL_RCC_HASH_CLK_ENABLE();

#endif
  
}

#ifdef USE_STM32F4XX_HW_CRYPTO  
/**
  * @brief  DeInitializes the HASH MSP.
  * @param  None  
  * @retval None
  */
void HAL_HASH_MspDeInit(HASH_HandleTypeDef *hhash)
{
  /* Force the HASH Periheral Clock Reset */  
  __HAL_RCC_HASH_FORCE_RESET(); 
  
  /* Release the HASH Periheral Clock Reset */  
  __HAL_RCC_HASH_RELEASE_RESET();
}

/**
  * @brief  DeInitializes the CRYP MSP.
  * @param  None  
  * @retval None
  */
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp)
{
  /* Force the CRYP Periheral Clock Reset */  
  __HAL_RCC_CRYP_FORCE_RESET(); 
  
  /* Release the CRYP Periheral Clock Reset */  
  __HAL_RCC_CRYP_RELEASE_RESET();
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
