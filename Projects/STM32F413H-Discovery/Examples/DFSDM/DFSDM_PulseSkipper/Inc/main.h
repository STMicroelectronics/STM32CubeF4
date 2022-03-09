/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_PulseSkipper/Inc/main.h
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
#include "audio.h"
#include "pulse_skipper.h"
#include "../Components/wm8994/wm8994.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

/* Unselect USE_CHANNEL_DELAY for normal use of the DFSDM */
#define USE_CHANNEL_DELAY

/* Select Mics */
//#define PLAY_DFSDM2_DATIN01     /* Use Mics: U2 and U3 from extension board */
#define PLAY_DFSDM2_DATIN06     /* Use Mics: U2 and U5 from extension board */
//#define PLAY_DFSDM2_DATIN07     /* Use Mics: U2 and U4 from extension board */
//#define PLAY_DFSDM2_DATIN16     /* Use Mics: U3 and U5 from extension board */
//#define PLAY_DFSDM2_DATIN17     /* Use Mics: U3 and U4 from extension board */
//#define PLAY_DFSDM2_DATIN67     /* Use Mics: U5 and U4 from extension board */
//#define PLAY_DFSDM12_DATIN10    /* Use Mics: U1 and U2 from extension board or U16 and U17 from discovery */
//#define PLAY_DFSDM12_DATIN11    /* Use Mics: U1 and U3 from extension board */
//#define PLAY_DFSDM12_DATIN16    /* Use Mics: U1 and U5 from extension board */
//#define PLAY_DFSDM12_DATIN17    /* Use Mics: U1 and U4 from extension board */



#if defined(USE_CHANNEL_DELAY)
#if defined(PLAY_DFSDM2_DATIN01)
/* Select channel to generate delay: either DFSDM2 CH0 or  DFSDM2 CH1 */
//#define GENERATE_DELAY_DFSDM2_CH1                          
//#define GENERATE_DELAY_DFSDM2_CH0                             
#elif defined(PLAY_DFSDM2_DATIN06)
/* Select channel to generate delay: either DFSDM2 CH0 or  DFSDM2 CH6 */
//#define GENERATE_DELAY_DFSDM2_CH6                       
//#define GENERATE_DELAY_DFSDM2_CH0                         
#elif defined(PLAY_DFSDM2_DATIN07)
/* Select channel to generate delay: either DFSDM2 CH0 or  DFSDM2 CH7 */
//#define GENERATE_DELAY_DFSDM2_CH7                        
//#define GENERATE_DELAY_DFSDM2_CH0                          
#elif defined(PLAY_DFSDM2_DATIN16)
/* Select channel to generate delay: either DFSDM2 CH1 or  DFSDM2 CH6 */
//#define GENERATE_DELAY_DFSDM2_CH1
//#define GENERATE_DELAY_DFSDM2_CH6
#elif defined(PLAY_DFSDM2_DATIN17)
/* Select channel to generate delay: either DFSDM2 CH1 or  DFSDM2 CH7 */
//#define GENERATE_DELAY_DFSDM2_CH1
#define GENERATE_DELAY_DFSDM2_CH7
#elif defined(PLAY_DFSDM2_DATIN67)
/* Select channel to generate delay: either DFSDM2 CH6 or  DFSDM2 CH7 */
#define GENERATE_DELAY_DFSDM2_CH6
//#define GENERATE_DELAY_DFSDM2_CH7
#elif defined(PLAY_DFSDM12_DATIN10)
/* Select channel to generate delay: either DFSDM1 CH1 or  DFSDM2 CH0 */
//#define GENERATE_DELAY_DFSDM1_CH1
#define GENERATE_DELAY_DFSDM2_CH0
#elif defined(PLAY_DFSDM12_DATIN11)
/* Select channel to generate delay: either DFSDM1 CH1 or  DFSDM2 CH1 */
//#define GENERATE_DELAY_DFSDM1_CH1
#define GENERATE_DELAY_DFSDM2_CH1                         /* OK */
#elif defined(PLAY_DFSDM12_DATIN16)
/* Select channel to generate delay: either DFSDM1 CH1 or  DFSDM2 CH6 */
//#define GENERATE_DELAY_DFSDM1_CH1
//#define GENERATE_DELAY_DFSDM2_CH6
#elif defined(PLAY_DFSDM12_DATIN17)
/* Select channel to generate delay: either DFSDM1 CH1 or  DFSDM2 CH7 */
//#define GENERATE_DELAY_DFSDM1_CH1
//#define GENERATE_DELAY_DFSDM2_CH7
#endif
#endif

#define DFSDM_DATIN0_CHANNEL                            DFSDM_CHANNEL_0
#define DFSDM_DATIN1_CHANNEL                            DFSDM_CHANNEL_1
#define DFSDM_DATIN2_CHANNEL                            DFSDM_CHANNEL_2
#define DFSDM_DATIN3_CHANNEL                            DFSDM_CHANNEL_3
#define DFSDM_DATIN4_CHANNEL                            DFSDM_CHANNEL_4
#define DFSDM_DATIN5_CHANNEL                            DFSDM_CHANNEL_5
#define DFSDM_DATIN6_CHANNEL                            DFSDM_CHANNEL_6
#define DFSDM_DATIN7_CHANNEL                            DFSDM_CHANNEL_7

#define DFSDM1_DATIN0_INSTANCE                          DFSDM1_Channel0
#define DFSDM1_DATIN1_INSTANCE                          DFSDM1_Channel1
#define DFSDM1_DATIN2_INSTANCE                          DFSDM1_Channel2
#define DFSDM1_DATIN3_INSTANCE                          DFSDM1_Channel3
#define DFSDM1_FILTER0                                  DFSDM1_Filter0
#define DFSDM1_FILTER1                                  DFSDM1_Filter1

#define DFSDM2_DATIN0_INSTANCE                          DFSDM2_Channel0
#define DFSDM2_DATIN1_INSTANCE                          DFSDM2_Channel1
#define DFSDM2_DATIN2_INSTANCE                          DFSDM2_Channel2
#define DFSDM2_DATIN3_INSTANCE                          DFSDM2_Channel3
#define DFSDM2_DATIN4_INSTANCE                          DFSDM2_Channel4
#define DFSDM2_DATIN5_INSTANCE                          DFSDM2_Channel5
#define DFSDM2_DATIN6_INSTANCE                          DFSDM2_Channel6
#define DFSDM2_DATIN7_INSTANCE                          DFSDM2_Channel7
#define DFSDM2_FILTER0                                  DFSDM2_Filter0
#define DFSDM2_FILTER1                                  DFSDM2_Filter1
#define DFSDM2_FILTER2                                  DFSDM2_Filter2
#define DFSDM2_FILTER3                                  DFSDM2_Filter3

#define DFSDM1_CKOUT_PIN                                GPIO_PIN_8
#define DFSDM1_CKOUT_PORT                               GPIOA
#define DFSDM1_CKOUT_ALTERNATE                          GPIO_AF6_DFSDM1
#define __DFSDM1_CKOUT_ENABLE()                         __HAL_RCC_GPIOA_CLK_ENABLE() 
#define __DFSDM1_CKOUT_DISABLE()                        __HAL_RCC_GPIOA_CLK_DISABLE()

#define DFSDM2_CKOUT_PIN                                GPIO_PIN_2
#define DFSDM2_CKOUT_PORT                               GPIOD
#define DFSDM2_CKOUT_ALTERNATE                          GPIO_AF3_DFSDM2
#define __DFSDM2_CKOUT_ENABLE()                         __HAL_RCC_GPIOD_CLK_ENABLE() 
#define __DFSDM2_CKOUT_DISABLE()                        __HAL_RCC_GPIOD_CLK_DISABLE()

#define DFSDM1_DATIN1_PIN                               GPIO_PIN_6
#define DFSDM1_DATIN1_PORT                              GPIOD
#define DFSDM1_DATIN1_ALTERNATE                         GPIO_AF6_DFSDM1
#define __DFSDM1_DATIN1_ENABLE()                        __HAL_RCC_GPIOD_CLK_ENABLE()
#define __DFSDM1_DATIN1_DISABLE()                       __HAL_RCC_GPIOD_CLK_DISABLE()

#define DFSDM2_DATIN1_PIN                               GPIO_PIN_7
#define DFSDM2_DATIN1_PORT                              GPIOA
#define DFSDM2_DATIN1_ALTERNATE                         GPIO_AF7_DFSDM2
#define __DFSDM2_DATIN1_ENABLE()                        __HAL_RCC_GPIOA_CLK_ENABLE()
#define __DFSDM2_DATIN1_DISABLE()                       __HAL_RCC_GPIOA_CLK_DISABLE()

#define DFSDM2_DATIN7_PIN                               GPIO_PIN_7
#define DFSDM2_DATIN7_PORT                              GPIOB
#define DFSDM2_DATIN7_ALTERNATE                         GPIO_AF6_DFSDM2
#define __DFSDM2_DATIN7_ENABLE()                        __HAL_RCC_GPIOB_CLK_ENABLE()
#define __DFSDM2_DATIN7_DISABLE()                       __HAL_RCC_GPIOB_CLK_DISABLE()

#define __HAL_RCC_DFSDMx_CLK_ENABLE()                   do {__HAL_RCC_DFSDM1_CLK_ENABLE(); \
                                                            __HAL_RCC_DFSDM2_CLK_ENABLE();    \
                                                        }while(0)
#define __HAL_RCC_DFSDMxAUDIO_CONFIG()                  do {__HAL_RCC_DFSDM1AUDIO_CONFIG(RCC_DFSDM1AUDIOCLKSOURCE_I2SAPB1); \
                                                            __HAL_RCC_DFSDM2AUDIO_CONFIG(RCC_DFSDM2AUDIOCLKSOURCE_I2SAPB1);    \
                                                        }while(0)                                                  

#endif /* __MAIN_H */
