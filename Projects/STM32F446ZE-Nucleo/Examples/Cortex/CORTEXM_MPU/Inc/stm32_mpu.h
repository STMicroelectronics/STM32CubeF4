/**
  ******************************************************************************
  * @file    Cortex/CORTEXM_MPU/Inc/stm32_mpu.h
  * @author  MCD Application Team
  * @brief   Header for stm32_mpu.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_MPU_H
#define __STM32_MPU_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define EXAMPLE_RAM_ADDRESS_START                (0x20000000UL)
#define EXAMPLE_RAM_SIZE                         MPU_REGION_SIZE_32KB
#define EXAMPLE_PERIPH_ADDRESS_START             (0x40000000)
#define EXAMPLE_PERIPH_SIZE                      MPU_REGION_SIZE_512MB
#define EXAMPLE_FLASH_ADDRESS_START              (0x08000000)
#define EXAMPLE_FLASH_SIZE                       MPU_REGION_SIZE_256KB
#define EXAMPLE_RAM_REGION_NUMBER                MPU_REGION_NUMBER0
#define EXAMPLE_FLASH_REGION_NUMBER              MPU_REGION_NUMBER1
#define EXAMPLE_PERIPH_REGION_NUMBER             MPU_REGION_NUMBER2
#define portMPU_REGION_READ_WRITE                MPU_REGION_FULL_ACCESS
#define portMPU_REGION_PRIVILEGED_READ_ONLY      MPU_REGION_PRIV_RO
#define portMPU_REGION_READ_ONLY                 MPU_REGION_PRIV_RO_URO
#define portMPU_REGION_PRIVILEGED_READ_WRITE     MPU_REGION_PRIV_RW

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MPU_Config(void);
void MPU_AccessPermConfig(void);

#endif /* __STM32_MPU_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
