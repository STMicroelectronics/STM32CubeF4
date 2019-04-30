/**
  ******************************************************************************
  * @file    I2S/I2S_Audio/Inc/audio_if.h 
  * @author  MCD Application Team
  * @brief   Header for audio_if.c module
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
#ifndef __AUDIO_IF_H
#define __AUDIO_IF_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported constants --------------------------------------------------------*/
#define AUDIO_BUFFER_SIZE       1000
#define AUDIO_DEFAULT_VOLUME    70

/* Audio file size and start offset address are defined here since the audio wave file is 
   stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_FILE_SIZE         147500       /* Size of audio file */
#define AUDIO_FILE_ADDRESS      0x08080000   /* Audio file address */

/* Exported types ------------------------------------------------------------*/
typedef enum {
  AUDIO_STATE_IDLE = 0,
  AUDIO_STATE_INIT,    
  AUDIO_STATE_PLAYING,  
}AUDIO_PLAYBACK_StateTypeDef;

typedef enum {
  AUDIO_ERROR_NONE = 0, 
  AUDIO_ERROR_NOTREADY,     
  AUDIO_ERROR_IO,   
  AUDIO_ERROR_EOF,
}AUDIO_ErrorTypeDef;

typedef enum {
  BUFFER_OFFSET_NONE = 0,  
  BUFFER_OFFSET_HALF,  
  BUFFER_OFFSET_FULL,     
}BUFFER_StateTypeDef;

typedef struct {
  uint8_t buff[AUDIO_BUFFER_SIZE];
  uint32_t fptr;  
  BUFFER_StateTypeDef state;
}AUDIO_BufferTypeDef;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
AUDIO_ErrorTypeDef AUDIO_Init(void);
AUDIO_ErrorTypeDef AUDIO_Start(void);
AUDIO_ErrorTypeDef AUDIO_Process(void);
AUDIO_ErrorTypeDef AUDIO_Stop(void);
#endif /* __AUDIO_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
