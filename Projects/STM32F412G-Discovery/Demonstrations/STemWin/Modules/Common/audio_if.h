/**
  ******************************************************************************
  * @file    audioplayer_app.h
  * @author  MCD Application Team
  * @brief   header of audio player application file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_IF_H
#define __AUDIO_IF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported constants --------------------------------------------------------*/
#define AUDIO_OUT_BUFFER_SIZE                       (24 * 1024)
#define AUDIO_IN_BUFFER_SIZE                        (24 * 1024) /* buffer size in half-word */
#define AUDIO_IN_PDM_BUFFER_SIZE                    (256)
/* Exported types ------------------------------------------------------------*/
 /* Audio buffer control struct */
   
typedef enum
{
    AUDIO_RECORDER_IDLE   =           0,
    AUDIO_RECORDER_SUSPENDED =        1,    
    AUDIO_RECORDER_RECORDING =        2,
    AUDIO_RECORDER_PLAYING   =        3,
    AUDIO_RECORDER_RECORD_PAUSE =     4,
    AUDIO_RECORDER_PLAY_PAUSE   =     5,
    AUDIO_RECORDER_RECORD_EOF =       6,
    AUDIO_RECORDER_PLAY_EOF =         7,
    AUDIO_RECORDER_ERROR        =     0xFF,      
  
}IN_StateTypdef;

typedef enum
{
  BUFFER_OFFSET_NONE = 0,  
  REC_BUFFER_OFFSET_HALF,  
  REC_BUFFER_OFFSET_FULL,   
  PLAY_BUFFER_OFFSET_HALF,  
  PLAY_BUFFER_OFFSET_FULL,     
}
BUFFER_StateTypeDef;


typedef enum
{
  AUDIOPLAYER_STOP = 0, 
  AUDIOPLAYER_START,   
  AUDIOPLAYER_PLAY,
  AUDIOPLAYER_PAUSE, 
  AUDIOPLAYER_EOF,   
  AUDIOPLAYER_ERROR,  
  
}OUT_StateTypdef;

typedef struct {
  uint32_t        volume;
  uint32_t        mute;   
  OUT_StateTypdef state;    
}OUT_Process_t;   

typedef enum {
  BUFFER_EMPTY = 0,  
  BUFFER_FULL,     
}WR_BUFFER_StateTypeDef;

typedef struct {
  uint32_t        volume;
  uint32_t        fptr;  
  IN_StateTypdef  state;  
}IN_Process_t;

#define INTERNAL_BUFF_SIZE                  (128*DEFAULT_AUDIO_IN_FREQ/16000*DEFAULT_AUDIO_IN_CHANNEL_NBR)
typedef struct 
{
  uint8_t          buff[AUDIO_OUT_BUFFER_SIZE];
  uint16_t         pdm[AUDIO_IN_PDM_BUFFER_SIZE]; 
  uint32_t         ppcm;    
  OUT_Process_t    out;  
  IN_Process_t     in;


}AUDIO_ProcessTypdef ;


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

}WAV_InfoTypedef ;   
   
   
typedef struct
{
  void  (*TransferComplete_CallBack)(void);
  void  (*HalfTransfer_CallBack)(void);  
  void  (*Error_CallBack)(void);
  
}AUDIO_IFTypeDef;

typedef  void (*pFunc)(void);               

/* Exported macros -----------------------------------------------------------*/
extern AUDIO_ProcessTypdef haudio;
/* Exported functions ------------------------------------------------------- */
void AUDIO_IF_RegisterCallbacks(pFunc  tc_cb, 
                                pFunc  ht_cb, 
                                pFunc  err_cb);
#ifdef __cplusplus
}
#endif

#endif /*__AUDIO_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
