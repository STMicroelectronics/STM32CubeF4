/**
  ******************************************************************************
  * @file    Examples_MIX/DMA/DMA_FLASHToRAM/Inc/main.h
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
#include "stm32f4xx_nucleo.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_pwr.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* DMA stream parameters definitions. You can modify these parameters to select*/
/*a different DMA channel.*/
#define DMA_INSTANCE               DMA2_Stream0
#define DMA_INSTANCE_IRQ           DMA2_Stream0_IRQn
#define DMA_INSTANCE_IRQHANDLER    DMA2_Stream0_IRQHandler

#define BUFFER_SIZE                32

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/* IRQ Handler treatment. */
void TransferComplete(DMA_HandleTypeDef *DmaHandle);
void TransferError(DMA_HandleTypeDef *DmaHandle);


#endif /* __MAIN_H */
