/**
  ******************************************************************************
  * @file    Audio/Audio_playback_and_record/Inc/waveplayer.h
  * @author  MCD Application Team
  * @brief   Header for waveplayer.c module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WAVEPLAYER_H
#define __WAVEPLAYER_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
AUDIO_ErrorTypeDef AUDIO_PLAYER_Init(void);
AUDIO_ErrorTypeDef AUDIO_PLAYER_Start(uint8_t idx);
AUDIO_ErrorTypeDef AUDIO_PLAYER_Process(void);
AUDIO_ErrorTypeDef AUDIO_PLAYER_Stop(void);

#endif /* __WAVEPLAYER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
