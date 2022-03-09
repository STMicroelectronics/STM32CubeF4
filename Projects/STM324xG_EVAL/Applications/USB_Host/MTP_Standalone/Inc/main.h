/**
  ******************************************************************************
  * @file    USB_Host/MTP_Standalone/Inc/main.h 
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
#include "stdio.h"
#include "usbh_core.h"
#include "usbh_mtp.h" 
#include "stm324xg_eval_io.h"
#include "stm324xg_eval_audio.h"
#include "lcd_log.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
  MTP_DEMO_IDLE = 0,
  MTP_DEMO_WAIT,  
  MTP_DEMO_EXPLORE,
  MTP_DEMO_PLAYBACK,
  MTP_REENUMERATE,  
}MTP_Demo_State;

typedef struct _DemoStateMachine {
  __IO MTP_Demo_State              state;
  __IO uint8_t                     select;  
}MTP_DEMO_StateMachine;

typedef struct AUDIO_Info_t {
  uint32_t ChunkID;       /* 0  */ 
  uint32_t FileSize;      /* 4  */
  uint32_t FileFormat;    /* 8  */
  uint32_t SubChunk1ID;   /* 12 */
  uint32_t SubChunk1Size; /* 16 */  
  uint16_t AudioFormat;   /* 20 */ 
  uint16_t NbrChannels;   /* 22 */   
  uint32_t SampleRate;    /* 24 */ 
  uint32_t ByteRate;      /* 28 */
  uint16_t BlockAlign;    /* 32 */  
  uint16_t BitPerSample;  /* 34 */  
  uint32_t SubChunk2ID;   /* 36 */   
  uint32_t SubChunk2Size; /* 40 */    
}WAV_InfoTypedef;

typedef enum {
  AUDIO_DEMO_IDLE = 0,
  AUDIO_DEMO_WAIT,  
  AUDIO_DEMO_EXPLORE,
  AUDIO_DEMO_PLAYBACK,
  AUDIO_REENUMERATE  
}AUDIO_Demo_State;

typedef enum {
  APPLICATION_IDLE = 0,  
  APPLICATION_START,   
  APPLICATION_READY,
  APPLICATION_DISCONNECT,
}MTP_ApplicationTypeDef;

typedef enum {
  AUDIO_STATE_IDLE = 0,
  AUDIO_STATE_WAIT,    
  AUDIO_STATE_INIT,    
  AUDIO_STATE_CONFIG,    
  AUDIO_STATE_PLAY,  
  AUDIO_STATE_NEXT,  
  AUDIO_STATE_PREVIOUS,
  AUDIO_STATE_FORWARD,   
  AUDIO_STATE_BACKWARD,
  AUDIO_STATE_STOP,   
  AUDIO_STATE_PAUSE,
  AUDIO_STATE_RESUME,
  AUDIO_STATE_VOLUME_UP,
  AUDIO_STATE_VOLUME_DOWN,  
}AUDIO_PLAYBACK_StateTypeDef;

typedef enum {
  AUDIO_ERROR_NONE = 0,  
  AUDIO_ERROR_IO,   
  AUDIO_ERROR_EOF,
}AUDIO_ErrorTypeDef;

typedef enum {
  MTP_ERROR_NONE = 0,  
  MTP_ERROR_IO,  
  MTP_ERROR_INVALID_VALUE,     
}MTP_ErrorTypeDef;

typedef enum {
  MTP_SELECT_MENU = 0,
  MTP_PLAYBACK_CONTROL ,  
}MTP_DEMO_SelectMode;

extern USBH_HandleTypeDef hUSBHost;
extern MTP_ApplicationTypeDef Appli_state;
extern AUDIO_PLAYBACK_StateTypeDef audio_state;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Toggle_Leds(void);
void MTP_MenuInit(void);
void MTP_MenuProcess(void);
void AUDIO_PlaybackProbeKey(JOYState_TypeDef state);
uint8_t MTP_ExploreWavFile(void);
uint8_t MTP_GetData(uint32_t file_idx, uint32_t offset, uint32_t maxbytes, 
                    uint8_t *object, uint32_t *len);
uint8_t MTP_GetWavObjectName(uint16_t object_index, uint8_t *filename);
uint8_t MTP_Init(void);
uint16_t MTP_GetWavObjectNumber(void);
AUDIO_ErrorTypeDef AUDIO_Init(void);
AUDIO_ErrorTypeDef AUDIO_Start(uint8_t idx);
AUDIO_ErrorTypeDef AUDIO_Process(void);
AUDIO_ErrorTypeDef AUDIO_Stop(void);
#endif /* __MAIN_H */
