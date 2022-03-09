/**
  ******************************************************************************
  * @file    ADC/ADC_DualModeInterleaved/Inc/main.h 
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
#include "stm324x9i_eval.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor ADCx instance used and associated 
   resources */
/* Definition for ADCx clock resources */
#define ADCx                             ADC1
#define ADCx_CLK_ENABLE()                __HAL_RCC_ADC1_CLK_ENABLE()

/* Definition for ADCy clock resources */
#define ADCy                             ADC2
#define ADCy_CLK_ENABLE()                __HAL_RCC_ADC2_CLK_ENABLE();
     
#define DMAxy_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()     
#define ADCxy_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
     
#define ADCxy_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCxy_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCxy_CHANNEL_PIN                GPIO_PIN_2
#define ADCxy_CHANNEL_GPIO_PORT          GPIOC 

/* Definition for ADCx's Channel */
#define ADCxy_CHANNEL                    ADC_CHANNEL_12

/* Definition for ADCx's DMA */
#define ADCxy_DMA_CHANNEL                DMA_CHANNEL_0
#define ADCxy_DMA_STREAM                 DMA2_Stream0         

/* Definition for ADCx's NVIC */
#define ADCxy_DMA_IRQn                   DMA2_Stream0_IRQn
#define ADCxy_DMA_IRQHandler             DMA2_Stream0_IRQHandler


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */
