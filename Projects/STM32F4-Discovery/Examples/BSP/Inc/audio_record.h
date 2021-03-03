/**
  ******************************************************************************
  * @file    BSP/Inc/audio_record.h 
  * @author  MCD Application Team
  * @brief   Header for audio_record.c module
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
