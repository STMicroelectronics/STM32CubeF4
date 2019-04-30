/**
  ******************************************************************************
  * @file    HASH/HASH_SHA1MD5_DMA/Src/stm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.    
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
  * @brief  Initializes the HASH MSP.
  *        This function configures the hardware resources used in this example: 
  *           - HASH's clock enable
  * @param  hhash: HASH handle pointer
  * @retval None
  */
void HAL_HASH_MspInit(HASH_HandleTypeDef *hhash)
{
  static DMA_HandleTypeDef  hdma_hash;
  
  /* Enable HASH clock */
  __HAL_RCC_HASH_CLK_ENABLE();
  
  /* Enable DMA clocks */
  DMA_CLK_ENABLE();
  
  /***************** Configure common DMA In parameters ***********************/
  hdma_hash.Init.Channel             = HASH_DMA_CHANNEL;
  hdma_hash.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_hash.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_hash.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_hash.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_hash.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_hash.Init.Mode                = DMA_NORMAL;
  hdma_hash.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_hash.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
  hdma_hash.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_HALFFULL;
  hdma_hash.Init.MemBurst            = DMA_MBURST_SINGLE;
  hdma_hash.Init.PeriphBurst         = DMA_PBURST_SINGLE; 
  hdma_hash.Instance = HASH_DMA_STREAM;
  
  /* Associate the DMA handle */
  __HAL_LINKDMA(hhash, hdmain, hdma_hash);
  
  /* Deinitialize the Stream for new transfer */
  HAL_DMA_DeInit(hhash->hdmain);
  
  /* Configure the DMA Stream */
  HAL_DMA_Init(hhash->hdmain);      
  
  /* NVIC configuration for DMA Input data interrupt */
  HAL_NVIC_SetPriority(HASH_DMA_IRQn, 0x0F, 0x0f);
  HAL_NVIC_EnableIRQ(HASH_DMA_IRQn);
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
