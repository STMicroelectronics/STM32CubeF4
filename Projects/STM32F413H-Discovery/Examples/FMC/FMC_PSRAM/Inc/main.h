/**
  ******************************************************************************
  * @file    FMC/FMC_PSRAM/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f413h_discovery.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define PSRAM_BANK_ADDR                 ((uint32_t)0x60000000)

/* #define PSRAM_MEMORY_WIDTH            FMC_NORSRAM_MEM_BUS_WIDTH_8  */
#define PSRAM_MEMORY_WIDTH               FMC_NORSRAM_MEM_BUS_WIDTH_16

#define PSRAM_TIMEOUT     ((uint32_t)0xFFFF)

#endif /* __MAIN_H */
