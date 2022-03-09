/**
  ******************************************************************************
  * @file    sdram_diskio.h
  * @author  MCD Application Team
  * @brief   Header for sdram_diskio.c module.
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
#ifndef __SDRAM_DISKIO_H
#define __SDRAM_DISKIO_H

/* Includes ------------------------------------------------------------------*/
#include  "stm32f429i_discovery_sdram.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern const Diskio_drvTypeDef  SDRAMDISK_Driver;

#endif /* __SDRAM_DISKIO_H */
