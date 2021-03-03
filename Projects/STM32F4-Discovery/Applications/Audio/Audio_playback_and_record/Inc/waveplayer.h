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

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  BUFFER_OFFSET_NONE = 0,  
  BUFFER_OFFSET_HALF,  
  BUFFER_OFFSET_FULL,     
}BUFFER_StateTypeDef;

typedef struct
{
  uint32_t   ChunkID;       /* 0 */ 
  uint32_t   FileSize;      /* 4 */
  uint32_t   FileFormat;    /* 8 */
  uint32_t   SubChunk1ID;   /* 12 */
  uint32_t   SubChunk1Size; /* 16*/  
  uint16_t   AudioFormat;   /* 20 */ 
  uint16_t   NbrChannels;   /* 22 */   
  uint32_t   SampleRate;    /* 24 */
  
  uint32_t   ByteRate;      /* 28 */
  uint16_t   BlockAlign;    /* 32 */  
  uint16_t   BitPerSample;  /* 34 */  
  uint32_t   SubChunk2ID;   /* 36 */   
  uint32_t   SubChunk2Size; /* 40 */    

}WAVE_FormatTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void  WavePlayBack(uint32_t AudioFreq);
int   WavePlayerInit(uint32_t AudioFreq);
void  WavePlayerStop(void);
void  WavePlayerPauseResume(uint32_t state);
void  WavePlayerStart(void);
void  WavePlayer_CallBack(void);
#endif /* __WAVEPLAYER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
