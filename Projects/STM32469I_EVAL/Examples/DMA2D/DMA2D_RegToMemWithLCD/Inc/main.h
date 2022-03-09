/**
  ******************************************************************************
  * @file    DMA2D/DMA2D_RegToMemWithLCD/Inc/main.h 
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
#include "stm32469i_eval.h"
#include "stm32469i_eval_lcd.h"
#include "stm32469i_eval_io.h"
#include "stm32469i_eval_sdram.h"

#define QVGA_RES_X  320
#define QVGA_RES_Y  240

#define LAYER_SIZE_X    QVGA_RES_X
#define LAYER_SIZE_Y    QVGA_RES_Y

#define ARGB8888_BYTE_PER_PIXEL  4
#define ARGB4444_BYTE_PER_PIXEL  2

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
