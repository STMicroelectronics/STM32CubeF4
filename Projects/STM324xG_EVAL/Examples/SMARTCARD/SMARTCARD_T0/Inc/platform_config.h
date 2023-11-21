/**
  ******************************************************************************
  * @file    SMARTCARD/SMARTCARD_T0/Inc/platform_config.h 
  * @author  MCD Application Team
  * @brief   Evaluation board specific configuration file.
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
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define SC_USART                           USART6
#define SC_USART_CLK_ENABLE()              __HAL_RCC_USART6_CLK_ENABLE()
#define SC_USART_CLK_DISABLE();            __HAL_RCC_USART6_CLK_DISABLE()
#define SC_USART_FORCE_RESET()             __HAL_RCC_USART6_FORCE_RESET()
#define SC_USART_RELEASE_RESET()           __HAL_RCC_USART6_RELEASE_RESET()
#define SC_USART_IRQn                      USART6_IRQn
#define SC_USART_IRQHandler                USART6_IRQHandler

#define SC_USART_TX_PIN                    GPIO_PIN_6                
#define SC_USART_TX_GPIO_PORT              GPIOC                       
#define SC_USART_TX_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
#define SC_USART_TX_AF                     GPIO_AF8_USART6

#define SC_USART_CK_PIN                    GPIO_PIN_7                
#define SC_USART_CK_GPIO_PORT              GPIOG
#define SC_USART_CK_CLK_ENABLE()           __HAL_RCC_GPIOG_CLK_ENABLE()
#define SC_USART_CK_AF                     GPIO_AF8_USART6


/* Smartcard Interface GPIO pins */
#define SC_3_5V_PIN                        GPIO_PIN_15
#define SC_3_5V_GPIO_PORT                  GPIOH
#define SC_3_5V_CLK_ENABLE()               __HAL_RCC_GPIOH_CLK_ENABLE()

#define SC_RESET_PIN                       GPIO_PIN_7
#define SC_RESET_GPIO_PORT                 GPIOF
#define SC_RESET_CLK_ENABLE()              __HAL_RCC_GPIOF_CLK_ENABLE()

#define SC_CMDVCC_PIN                      GPIO_PIN_12
#define SC_CMDVCC_GPIO_PORT                GPIOG
#define SC_CMDVCC_CLK_ENABLE()             __HAL_RCC_GPIOG_CLK_ENABLE()

#define SC_OFF_PIN                         GPIO_PIN_6
#define SC_OFF_GPIO_PORT                   GPIOF
#define SC_OFF_CLK_ENABLE()                __HAL_RCC_GPIOF_CLK_ENABLE()

#define SC_OFF_EXTI_IRQn                   EXTI9_5_IRQn 
#define SC_OFF_EXTI_IRQHandler             EXTI9_5_IRQHandler

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */
