/**
  ******************************************************************************
  * @file    DFSDM/DFSDM_PulseSkipper/Inc/pulse_skipper.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for pulse skipper.
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
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t  DFSDM1PulseSkipperCh;    /*!< Channels selection to generate pulse skipper of DFSDM1.
                                         This parameter can be a value of @ref DFSDM1_PULSESKIPPER_CH */
  uint32_t  DFSDM2PulseSkipperCh;    /*!< Channels selection to generate pulse skipper of DFSDM2.
                                         This parameter can be a value of @ref DFSDM2_PULSESKIPPER_CH */
}PulseSkipper_InitTypeDef;
/* Exported constants --------------------------------------------------------*/
/** 
  * @brief  Skipper Delay parameters structure definition  
  */  

/**
  * @brief DFSDM1_PULSESKIPPER_CH
  */
#define DFSDM1_PULSE_SKIPPER_CH02         0x00000001U
#define DFSDM1_PULSE_SKIPPER_CH13         0x00000002U

/**
  * @brief DFSDM2_PULSESKIPPER_CH
  */
#define DFSDM2_PULSE_SKIPPER_CH04         0x00000010U
#define DFSDM2_PULSE_SKIPPER_CH15         0x00000020U
#define DFSDM2_PULSE_SKIPPER_CH26         0x00000040U
#define DFSDM2_PULSE_SKIPPER_CH37         0x00000080U


#define DFSDM1_TIM4_OC1                   TIM_CHANNEL_1
#define DFSDM1_TIM4_OC2                   TIM_CHANNEL_2

#define DFSDM2_TIM3_OC1                   TIM_CHANNEL_1
#define DFSDM2_TIM3_OC2                   TIM_CHANNEL_2
#define DFSDM2_TIM3_OC3                   TIM_CHANNEL_3
#define DFSDM2_TIM3_OC4                   TIM_CHANNEL_4

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void             Pulse_Skipper_Init(void);
void             Pulse_Skipper_Bitstream_Start(void);
void             Pulse_Skipper_Bitstream_Stop(void);
void             Pulse_Skipper_DeInit(void);
void             Pulse_Skipper_Generate_Pulse(PulseSkipper_InitTypeDef* skipperstruct);
