/**
  ******************************************************************************
  * @file    BSP/Src/audio_record.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use AUDIO features for the record.
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
#include "audio_record.h"
#include "string.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
#define AUDIO_BUFFER_SIZE   8192

typedef struct {
  int32_t offset;
  uint32_t fptr;
}Audio_BufferTypeDef;

typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF,
  BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t pHeaderBuff[44];
uint16_t WrBuffer[WR_BUFFER_SIZE];

static uint16_t RecBuf[2*PCM_OUT_SIZE];
static uint16_t InternalBuffer[INTERNAL_BUFF_SIZE];
__IO uint32_t ITCounter = 0;
Audio_BufferTypeDef  BufferCtl;

/* Temporary data sample */
__IO uint32_t AUDIODataReady = 0, AUDIOBuffOffset = 0;

/* Variable used to replay audio sample (from play or record test) */
extern uint32_t AudioTest;

/* Variable used for play in infinite loop */
extern __IO uint8_t UserPressButton;

/* Variables used in normal mode to manage audio file during DMA transfer */
extern uint32_t AudioTotalSize; /* This variable holds the total size of the audio file */
extern uint32_t AudioRemSize;   /* This variable holds the remaining data in audio file */
extern uint16_t *CurrentPos ;   /* This variable holds the current position of audio pointer */

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief Test Audio Hardware.
  *   The main objective of this test is to check the hardware connection of the 
  *   Audio peripheral.
  * @param  None
  * @retval None
  */
void AudioRecord_Test(void)
{
  BufferCtl.offset = BUFFER_OFFSET_NONE;
  if(BSP_AUDIO_IN_Init(DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) != AUDIO_OK)
  {
    /* Record Error */
    Error_Handler();
  }  

  /* Turn ON LED3: start record */
  BSP_LED_On(LED3);
  
  /* Start the record */
  if (BSP_AUDIO_IN_Record((uint16_t*)&InternalBuffer[0], INTERNAL_BUFF_SIZE) != AUDIO_OK)
  {
    /* Record Error */
    Error_Handler();
  }
  BufferCtl.fptr = 0;
  
  AUDIODataReady = 0; 

  /* Wait for the data to be ready with PCM form */
  while (AUDIODataReady != 2) 
  {
    if(BufferCtl.offset == BUFFER_OFFSET_HALF)
    {
      /* PDM to PCM data convert */
      BSP_AUDIO_IN_PDMToPCM((uint16_t*)&InternalBuffer[0], (uint16_t*)&RecBuf[0]);
      
      /* Copy PCM data in internal buffer */
      memcpy((uint16_t*)&WrBuffer[ITCounter * (PCM_OUT_SIZE*2)], RecBuf, PCM_OUT_SIZE*4);
      
      BufferCtl.offset = BUFFER_OFFSET_NONE;
      
      if(ITCounter == (WR_BUFFER_SIZE/(PCM_OUT_SIZE*4))-1)
      {
        AUDIODataReady = 1;
        AUDIOBuffOffset = 0;
        ITCounter++;
      }
      else if(ITCounter == (WR_BUFFER_SIZE/(PCM_OUT_SIZE*2))-1)
      {
        AUDIODataReady = 2;
        AUDIOBuffOffset = WR_BUFFER_SIZE/2;
        ITCounter = 0;
      }
      else
      {
        ITCounter++;
      }     
      
    }
    
    if(BufferCtl.offset == BUFFER_OFFSET_FULL)
    {
      /* PDM to PCM data convert */
      BSP_AUDIO_IN_PDMToPCM((uint16_t*)&InternalBuffer[INTERNAL_BUFF_SIZE/2], (uint16_t*)&RecBuf[0]);
      
      /* Copy PCM data in internal buffer */
      memcpy((uint16_t*)&WrBuffer[ITCounter * (PCM_OUT_SIZE*2)], RecBuf, PCM_OUT_SIZE*4);
      
      BufferCtl.offset = BUFFER_OFFSET_NONE;
      
      if(ITCounter == (WR_BUFFER_SIZE/(PCM_OUT_SIZE*4))-1)
      {
        AUDIODataReady = 1;
        AUDIOBuffOffset = 0;
        ITCounter++;
      }
      else if(ITCounter == (WR_BUFFER_SIZE/(PCM_OUT_SIZE*2))-1)
      {
        AUDIODataReady = 2;
        AUDIOBuffOffset = WR_BUFFER_SIZE/2;
        ITCounter = 0;
      }
      else
      {
        ITCounter++;
      }  
    }   
  };
  
  /* Stop audio record */
  if (BSP_AUDIO_IN_Stop() != AUDIO_OK)
  {
    /* Record Error */
    Error_Handler();
  }

  /* Turn OFF LED3: record stopped */
  BSP_LED_Off(LED3);
  /* Turn ON LED6: play recorded file */
  BSP_LED_On(LED6);
  
  /* Play in the loop the recorded file */

  /* Set variable to indicate play from record buffer */ 
  AudioTest = 1;
  
  /*Set variable used to stop player before starting */
  UserPressButton = 0;

  /* Initialize audio IN at REC_FREQ */ 
  BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 70, DEFAULT_AUDIO_IN_FREQ);  
  
  /* Set the total number of data to be played */
  AudioTotalSize = AUDIODATA_SIZE * WR_BUFFER_SIZE;  
  /* Update the remaining number of data to be played */
  AudioRemSize = 0;  
  /* Update the WrBuffer audio pointer position */
  CurrentPos = (uint16_t *)(WrBuffer);
  
  /* Play the recorded buffer */
  BSP_AUDIO_OUT_Play(WrBuffer , AudioTotalSize);
  
  while(!UserPressButton)
  { 
  }
  
  /* Stop Player before close Test */
  if (BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW) != AUDIO_OK)
  {
    /* Audio Stop error */
    Error_Handler();
  }
}

/**
  * @brief Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  BufferCtl.offset = BUFFER_OFFSET_FULL;
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  BufferCtl.offset = BUFFER_OFFSET_HALF;
}

/**
  * @brief  Audio IN Error callback function
  * @param  pData
  * @retval None
  */
void BSP_AUDIO_IN_Error_Callback(void)
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
