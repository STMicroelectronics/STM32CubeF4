/**
  ******************************************************************************
  * @file    ADC/ADC_TripleModeInterleaved/Inc/main.h 
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
     
/* Definition for ADCz clock resources */
#define ADCz                             ADC3
#define ADCz_CLK_ENABLE()                __HAL_RCC_ADC3_CLK_ENABLE();
     
#define DMAxyz_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()     
#define ADCxyz_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
     
#define ADCxyz_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCxyz_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCxyz_CHANNEL_PIN                GPIO_PIN_2
#define ADCxyz_CHANNEL_GPIO_PORT          GPIOC 

/* Definition for ADCx's Channel */
#define ADCxyz_CHANNEL                    ADC_CHANNEL_12

/* Definition for ADCx's DMA */
#define ADCxyz_DMA_CHANNEL                DMA_CHANNEL_0
#define ADCxyz_DMA_STREAM                 DMA2_Stream0         

/* Definition for ADCx's NVIC */
#define ADCxyz_DMA_IRQn                   DMA2_Stream0_IRQn
#define ADCxyz_DMA_IRQHandler             DMA2_Stream0_IRQHandler


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */
