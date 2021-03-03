/**
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_AdvComIT/Slave/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor SPIx instance used and associated
   resources */
/* Definition for SPIx clock resources */
#define SPIx                             SPI2
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define SPIx_FORCE_RESET()               __HAL_RCC_SPI2_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI2_RELEASE_RESET()

/* Definition for SPIx Pins */
#define SPIx_SCK_PIN                     GPIO_PIN_13
#define SPIx_SCK_GPIO_PORT               GPIOB
#define SPIx_SCK_AF                      GPIO_AF5_SPI2
#define SPIx_MISO_PIN                    GPIO_PIN_14
#define SPIx_MISO_GPIO_PORT              GPIOB
#define SPIx_MISO_AF                     GPIO_AF5_SPI2
#define SPIx_MOSI_PIN                    GPIO_PIN_15
#define SPIx_MOSI_GPIO_PORT              GPIOB
#define SPIx_MOSI_AF                     GPIO_AF5_SPI2

/* Definition for SPIx's NVIC */
#define SPIx_IRQn                        SPI2_IRQn
#define SPIx_IRQHandler                  SPI2_IRQHandler

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
