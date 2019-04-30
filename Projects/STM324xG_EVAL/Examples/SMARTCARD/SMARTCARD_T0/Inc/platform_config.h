/**
  ******************************************************************************
  * @file    SMARTCARD/SMARTCARD_T0/Inc/platform_config.h 
  * @author  MCD Application Team
  * @brief   Evaluation board specific configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
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


/* Smartcard Inteface GPIO pins */
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
