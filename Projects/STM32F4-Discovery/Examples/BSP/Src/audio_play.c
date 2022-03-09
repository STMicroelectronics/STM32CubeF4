/**
  ******************************************************************************
  * @file    BSP/Src/audio_play.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use AUDIO features for the play.
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
#include "audio_play.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

typedef struct
{
  uint32_t   ChunkID;       /* 0 */ 
  uint32_t   FileSize;      /* 4 */
  uint32_t   FileFormat;    /* 8 */
  uint32_t   SubChunk1ID;   /* 12 */
  uint32_t   SubChunk1Size; /* 16 */  
  uint16_t   AudioFormat;   /* 20 */ 
  uint16_t   NbrChannels;   /* 22 */   
  uint32_t   SampleRate;    /* 24 */
  
  uint32_t   ByteRate;      /* 28 */
  uint16_t   BlockAlign;    /* 32 */  
  uint16_t   BitPerSample;  /* 34 */  
  uint32_t   SubChunk2ID;   /* 36 */   
  uint32_t   SubChunk2Size; /* 40 */    

}WAVE_FormatTypeDef;

/* Private define ------------------------------------------------------------*/

/* Audio file size and start address are defined here since the Audio file is 
   stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_FILE_SIZE               0x70000
#define AUDIO_FILE_ADDRESS            0x08080000   /* Audio file address */  
  
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t UserPressButton;
extern __IO uint32_t PauseResumeStatus;

WAVE_FormatTypeDef *waveformat =  NULL;

/* Variable used to replay audio sample (from play or record test)*/
__IO uint32_t AudioTest = 0;

/* Variables used in normal mode to manage audio file during DMA transfer */
uint32_t AudioTotalSize           = 0xFFFF; /* This variable holds the total size of the audio file */
uint32_t AudioRemSize             = 0xFFFF; /* This variable holds the remaining data in audio file */
uint16_t *CurrentPos ;             /* This variable holds the current position of audio pointer */

extern uint16_t WrBuffer[WR_BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Test Audio Hardware.
  *   The main objective of this test is to check the hardware connection of the 
  *   Audio peripheral.
  * @param  None
  * @retval None
  */
void AudioPlay_Test(void)
{  
  /* Initial volume level (from 0 (Mute) to 100 (Max)) */
  __IO uint8_t volume = 70;
  
  /* Initialize MEMS Accelerometer mounted on STM32F4-Discovery board */
  if(BSP_ACCELERO_Init() != ACCELERO_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Enable MEMS click feature only for STM32F4-DISCO rev B board */
  if (BSP_ACCELERO_ReadID() == I_AM_LIS302DL)
  {
    /* MEMS Accelerometer configure to manage PAUSE, RESUME operations */
    BSP_ACCELERO_Click_ITConfig();
    
    /* Clear MEMS click interruption */
    BSP_ACCELERO_Click_ITClear();
  }
  
  /* Turn ON LED6: start of Audio file play */
  BSP_LED_On(LED6);
  
  /* Retrieve Wave Sample rate */
  waveformat = (WAVE_FormatTypeDef*) AUDIO_FILE_ADDRESS;

  /* Initialize Audio Device */
  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, volume, waveformat->SampleRate) != 0)
  {
    Error_Handler();
  }
  
  /* Set variable used to stop player before starting */
  UserPressButton = 0;
  AudioTest = 0;
  
  /* Set the total number of data to be played */
  AudioTotalSize = AUDIO_FILE_SIZE;  
  /* Set the current audio pointer position */
  CurrentPos = (uint16_t *)(AUDIO_FILE_ADDRESS);
  /* Start the audio player */
  BSP_AUDIO_OUT_Play(CurrentPos, AudioTotalSize);  
  /* Update the remaining number of data to be played */
  AudioRemSize = AudioTotalSize - AUDIODATA_SIZE * DMA_MAX(AudioTotalSize);  
  /* Update the current audio pointer position */
  CurrentPos += DMA_MAX(AudioTotalSize);
  
  /* Infinite loop */
  while(!UserPressButton)
  { 
    if (PauseResumeStatus == PAUSE_STATUS)
    {
      /* Turn ON LED4: Audio play in pause */
      BSP_LED_On(LED4);
      
      /* Pause playing */
      BSP_AUDIO_OUT_Pause();
      PauseResumeStatus = IDLE_STATUS;
    }
    else if (PauseResumeStatus == RESUME_STATUS)
    {
      /* Turn OFF LED4: Audio play running */
      BSP_LED_Off(LED4);
      
      /* Resume playing */
      BSP_AUDIO_OUT_Resume();
      PauseResumeStatus = IDLE_STATUS;
    }
  }
  
  /* Stop Player before close Test */
  if (BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW) != AUDIO_OK)
  {
    /* Audio Stop error */
    Error_Handler();
  }
}

/*--------------------------------
Callbacks implementation:
The callbacks prototypes are defined in the stm32f4_discovery_audio.h file
and their implementation should be done in the user code if they are needed.
Below some examples of callback implementations.
--------------------------------------------------------*/
/**
* @brief  Calculates the remaining file size and new position of the pointer.
* @param  None
* @retval None
*/
void BSP_AUDIO_OUT_TransferComplete_CallBack()
{
  uint32_t replay = 0;
  
  if (AudioRemSize > 0)
  {
    /* Replay from the current position */
    BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)CurrentPos, DMA_MAX(AudioRemSize/AUDIODATA_SIZE));
    
    /* Update the current pointer position */
    CurrentPos += DMA_MAX(AudioRemSize);        
    
    /* Update the remaining number of data to be played */
    AudioRemSize -= AUDIODATA_SIZE * DMA_MAX(AudioRemSize/AUDIODATA_SIZE);  
  }
  else
  {
    /* Request to replay audio file from beginning */
    replay = 1;
  }
  
  /* Audio sample used for play */
  if((AudioTest == 0) && (replay == 1))
  {
    /* Replay from the beginning */
    /* Set the current audio pointer position */
    CurrentPos = (uint16_t *)(AUDIO_FILE_ADDRESS);
    /* Replay from the beginning */
    BSP_AUDIO_OUT_Play(CurrentPos, AudioTotalSize);
    /* Update the remaining number of data to be played */
    AudioRemSize = AudioTotalSize - AUDIODATA_SIZE * DMA_MAX(AudioTotalSize);  
    /* Update the current audio pointer position */
    CurrentPos += DMA_MAX(AudioTotalSize);
  }
  
  /* Audio sample saved during record */
  if((AudioTest == 1) && (replay == 1))
  {
    /* Replay from the beginning */
    BSP_AUDIO_OUT_Play(WrBuffer, AudioTotalSize);
  }
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(void)
{
  /* Stop the program with an infinite loop */
  Error_Handler();
}

/**
  * @}
  */ 

/**
  * @}
  */
