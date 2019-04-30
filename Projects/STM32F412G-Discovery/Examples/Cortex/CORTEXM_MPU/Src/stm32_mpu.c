/**
  ******************************************************************************
  * @file    Cortex/CORTEXM_MPU/Src/stm32_mpu.c
  * @author  MCD Application Team
  * @brief   Access rights configuration using Cortex-M3 MPU regions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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
#include "stm32_mpu.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup CORTEXM_MPU
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ARRAY_ADDRESS_START    (0x20002000UL)
#define ARRAY_SIZE             MPU_REGION_SIZE_32B
#define ARRAY_REGION_NUMBER    MPU_REGION_NUMBER3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined ( __CC_ARM   )
uint8_t PrivilegedReadOnlyArray[32] __attribute__((at(0x20002000)));

#elif defined ( __ICCARM__ )
#pragma location=0x20002000
__no_init uint8_t PrivilegedReadOnlyArray[32];

#elif defined   (  __GNUC__  )
uint8_t PrivilegedReadOnlyArray[32] __attribute__((section(".ROarraySection")));

#endif


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configures the main MPU regions.
  * @param  None
  * @retval None
  */
void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable MPU */
  HAL_MPU_Disable();

  /* Configure RAM region as Region N°0, 32KB of size and R/W region */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = EXAMPLE_RAM_ADDRESS_START;
  MPU_InitStruct.Size = EXAMPLE_RAM_SIZE;
  MPU_InitStruct.AccessPermission = portMPU_REGION_READ_WRITE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = EXAMPLE_RAM_REGION_NUMBER;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure FLASH region as REGION N°1, 256KB of size and R/W region */
  MPU_InitStruct.BaseAddress = EXAMPLE_FLASH_ADDRESS_START;
  MPU_InitStruct.Size = EXAMPLE_FLASH_SIZE;
  MPU_InitStruct.Number = EXAMPLE_FLASH_REGION_NUMBER;
  
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure Peripheral region as REGION N°2, 512MB of size, R/W and Execute
  Never region */
  MPU_InitStruct.BaseAddress = EXAMPLE_PERIPH_ADDRESS_START;
  MPU_InitStruct.Size = EXAMPLE_PERIPH_SIZE;
  MPU_InitStruct.Number = EXAMPLE_PERIPH_REGION_NUMBER;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable MPU (any access not covered by any enabled region will cause a fault) */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  This function configures the access right using Cortex-M4 MPU regions.
  * @param  None
  * @retval None
  */
void MPU_AccessPermConfig(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Configure region for PrivilegedReadOnlyArray as REGION N°3, 32byte and R
     only in privileged mode */
  /* Disable MPU */
  HAL_MPU_Disable();

  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = ARRAY_ADDRESS_START;
  MPU_InitStruct.Size = ARRAY_SIZE;
  MPU_InitStruct.AccessPermission = portMPU_REGION_PRIVILEGED_READ_ONLY;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = ARRAY_REGION_NUMBER;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  
  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  
  /* Enable MPU (any access not covered by any enabled region will cause a fault) */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

  /* Read from PrivilegedReadOnlyArray. This will not generate error */
  (void)PrivilegedReadOnlyArray[0];

  /* Uncomment the following line to write to PrivilegedReadOnlyArray. This will
     generate error */
  /* PrivilegedReadOnlyArray[0] = 'e'; */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
