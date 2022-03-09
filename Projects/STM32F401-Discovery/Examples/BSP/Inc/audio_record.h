/**
  ******************************************************************************
  * @file    BSP/Inc/audio_record.h 
  * @author  MCD Application Team
  * @brief   Header for audio_record.c module
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
#ifndef __AUDIO_RECORD_H
#define __AUDIO_RECORD_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc/pdm2pcm_glo.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void AudioRecord_Test(void);
void AudioRecorderProcessingAudioBuffer(void);

#endif /* __AUDIO_RECORD_H */
