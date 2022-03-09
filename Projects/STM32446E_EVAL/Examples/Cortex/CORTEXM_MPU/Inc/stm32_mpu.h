/**
  ******************************************************************************
  * @file    Cortex/CORTEXM_MPU/Inc/stm32_mpu.h
  * @author  MCD Application Team
  * @brief   Header for stm32_mpu.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_MPU_H
#define __STM32_MPU_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define RAM_ADDRESS_START                        (0x20000000UL)
#define RAM_SIZE                                 MPU_REGION_SIZE_8KB
#define PERIPH_ADDRESS_START                     (0x40000000)
#define PERIPH_SIZE                              MPU_REGION_SIZE_512MB
#define FLASH_ADDRESS_START                      (0x08000000)
#define FLASH_SIZE                               MPU_REGION_SIZE_1MB
#define portMPU_REGION_READ_WRITE                MPU_REGION_FULL_ACCESS
#define portMPU_REGION_PRIVILEGED_READ_ONLY      MPU_REGION_PRIV_RO
#define portMPU_REGION_READ_ONLY                 MPU_REGION_PRIV_RO_URO
#define portMPU_REGION_PRIVILEGED_READ_WRITE     MPU_REGION_PRIV_RW
#define RAM_REGION_NUMBER                        MPU_REGION_NUMBER0
#define FLASH_REGION_NUMBER                      MPU_REGION_NUMBER1
#define PERIPH_REGION_NUMBER                     MPU_REGION_NUMBER2


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void MPU_Config(void);
void MPU_AccessPermConfig(void);

#endif /* __STM32_MPU_H */
