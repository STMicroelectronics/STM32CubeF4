/**
  ******************************************************************************
  * @file    USB_Host/MTP_Standalone/Src/audio.c 
  * @author  MCD Application Team
  * @brief   This file provides the Audio Out (playback) interface API
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
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define AUDIO_BUFFER_SIZE   4096
#define FILE_NAME_SIZE      255

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
    
typedef enum {
  BUFFER_OFFSET_NONE = 0,  
  BUFFER_OFFSET_HALF,  
  BUFFER_OFFSET_FULL,     
}BUFFER_StateTypeDef;

typedef struct {
  uint8_t buff[AUDIO_BUFFER_SIZE];
  int32_t offset;
  uint32_t fptr;
}Audio_BufferTypeDef;

/* Private variables ---------------------------------------------------------*/
static WAV_InfoTypedef wav_info;
static Audio_BufferTypeDef  buffer_ctl;
static int16_t file_pos = 0;
static __IO uint32_t uwVolume = 70;

/* Private function prototypes -----------------------------------------------*/
static AUDIO_ErrorTypeDef AUDIO_GetFileInfo(uint16_t file_idx, WAV_InfoTypedef *info);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes Audio Interface.
  * @param  None
  * @retval Audio error
  */
AUDIO_ErrorTypeDef AUDIO_Init(void)
{
 if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, uwVolume, I2S_AUDIOFREQ_44K) == 0)
 {
   return AUDIO_ERROR_NONE;
 }
  return AUDIO_ERROR_IO;
}

/**
  * @brief  Starts Audio streaming.    
  * @param  idx: File index
  * @retval Audio error
  */ 
AUDIO_ErrorTypeDef AUDIO_Start(uint8_t idx)
{
  uint32_t bytesread;
  
  if(MTP_GetWavObjectNumber() > idx)
  { 
    AUDIO_GetFileInfo(idx, &wav_info);
    
    BSP_AUDIO_OUT_SetFrequency(wav_info.SampleRate); 
    
    buffer_ctl.offset = BUFFER_OFFSET_NONE;
    
    if(MTP_GetData(idx, 
                   44,
                   AUDIO_BUFFER_SIZE, 
                   &buffer_ctl.buff[0],
                   &bytesread) == 0)
    {
      audio_state = AUDIO_STATE_PLAY;
      BSP_LCD_DisplayStringAt(250, LINE(14), (uint8_t *)"  [PLAY ]", LEFT_MODE);
      { 
        if(bytesread != 0)
        {
          BSP_AUDIO_OUT_Play((uint16_t*)&buffer_ctl.buff[0], AUDIO_BUFFER_SIZE);
          buffer_ctl.fptr = bytesread;
          return AUDIO_ERROR_NONE;
        }
      }
    }
  }
  return AUDIO_ERROR_IO;
}

/**
  * @brief  Manages Audio process. 
  * @param  None
  * @retval Audio error
  */
AUDIO_ErrorTypeDef AUDIO_Process(void)
{
  uint32_t bytesread, elapsed_time; 
  static uint32_t prev_elapsed_time = 0xFFFFFFFF;
  uint8_t str[14];
  AUDIO_ErrorTypeDef error_state = AUDIO_ERROR_NONE;  
  
  switch(audio_state)
  {
  case AUDIO_STATE_PLAY:
    if(buffer_ctl.fptr >= wav_info.FileSize)
    {
      error_state = AUDIO_ERROR_EOF;
    }
    
    if(buffer_ctl.offset == BUFFER_OFFSET_HALF)
    {
      if(MTP_GetData(file_pos, 
                     buffer_ctl.fptr,
                     AUDIO_BUFFER_SIZE /2, 
                     &buffer_ctl.buff[0],
                     &bytesread)> 0)
      { 
        error_state = AUDIO_ERROR_IO;       
      } 
      buffer_ctl.offset = BUFFER_OFFSET_NONE;
      buffer_ctl.fptr += bytesread; 
    }
    
    if(buffer_ctl.offset == BUFFER_OFFSET_FULL)
    {
      if(MTP_GetData(file_pos, 
                     buffer_ctl.fptr,
                     AUDIO_BUFFER_SIZE /2, 
                     &buffer_ctl.buff[AUDIO_BUFFER_SIZE /2],
                     &bytesread)> 0)
      { 
        error_state = AUDIO_ERROR_IO;       
      } 
      buffer_ctl.offset = BUFFER_OFFSET_NONE;
      buffer_ctl.fptr += bytesread; 
    }
    
    /* Display elapsed time */
    elapsed_time = buffer_ctl.fptr / wav_info.ByteRate; 
    if(prev_elapsed_time != elapsed_time)
    {
      prev_elapsed_time = elapsed_time;
      sprintf((char *)str, "[%02lu:%02lu]", elapsed_time /60, elapsed_time%60);
      BSP_LCD_SetTextColor(LCD_COLOR_CYAN); 
      BSP_LCD_DisplayStringAt(263, LINE(8), str, LEFT_MODE);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    }
    break;
    
  case AUDIO_STATE_STOP:
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
    audio_state = AUDIO_STATE_IDLE; 
    error_state = AUDIO_ERROR_IO;
    break;
    
  case AUDIO_STATE_NEXT:
    if(++file_pos >= MTP_GetWavObjectNumber())
    {
      file_pos = 0; 
    }
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
    AUDIO_Start(file_pos);
    break;    
    
  case AUDIO_STATE_PREVIOUS:
    if(--file_pos < 0)
    {
      file_pos = MTP_GetWavObjectNumber() - 1; 
    }
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
    AUDIO_Start(file_pos);
    break;   
    
  case AUDIO_STATE_PAUSE:
    BSP_LCD_DisplayStringAt(250, LINE(14), (uint8_t *)"  [PAUSE]", LEFT_MODE);
    BSP_AUDIO_OUT_Pause();
    audio_state = AUDIO_STATE_WAIT;
    break;
    
  case AUDIO_STATE_RESUME:
    BSP_LCD_DisplayStringAt(250, LINE(14), (uint8_t *)"  [PLAY ]", LEFT_MODE);
    BSP_AUDIO_OUT_Resume();
    audio_state = AUDIO_STATE_PLAY;
    break;
    
  case AUDIO_STATE_VOLUME_UP: 
    if(uwVolume <= 90)
    {
      uwVolume += 10;
    }
    BSP_AUDIO_OUT_SetVolume(uwVolume);
    audio_state = AUDIO_STATE_PLAY;
    break;
    
  case AUDIO_STATE_VOLUME_DOWN:    
    if(uwVolume >= 10)
    {
      uwVolume -= 10;
    }
    BSP_AUDIO_OUT_SetVolume(uwVolume);
    audio_state = AUDIO_STATE_PLAY;
    break;
    
  case AUDIO_STATE_WAIT:
  case AUDIO_STATE_CONFIG:    
  case AUDIO_STATE_IDLE:
  case AUDIO_STATE_INIT:    
  default:
    /* Do Nothing */
    break;
  }
  return error_state;
}

/**
  * @brief  Stops Audio streaming.
  * @param  None
  * @retval Audio error
  */
AUDIO_ErrorTypeDef AUDIO_Stop(void)
{
  audio_state = AUDIO_STATE_STOP;
  file_pos = 0;
  return AUDIO_ERROR_NONE;
}

/**
  * @brief  Probes the play back joystick state.
  * @param  state: Joystick state
  * @retval None
  */
void AUDIO_PlaybackProbeKey(JOYState_TypeDef state)
{
  /* Handle File List Selection */ 
  if(state == JOY_UP)
  {
    if(audio_state == AUDIO_STATE_PLAY)
    {
      audio_state = AUDIO_STATE_VOLUME_UP;
    }
  }
  else if(state == JOY_DOWN)
  {
    if(audio_state == AUDIO_STATE_PLAY)
    {
      audio_state = AUDIO_STATE_VOLUME_DOWN;
    }
  }
  else if(state == JOY_RIGHT)
  {
    audio_state = AUDIO_STATE_NEXT;
  }  
  else if(state == JOY_LEFT)
  {
    audio_state = AUDIO_STATE_PREVIOUS;
  }    
  else if(state == JOY_SEL)
  {
    audio_state = (audio_state == AUDIO_STATE_WAIT) ? AUDIO_STATE_RESUME : AUDIO_STATE_PAUSE;
  }    
}

/**
  * @brief  Gets the file info.
  * @param  file_idx: File index
  * @param  info: Pointer to WAV file info
  * @retval Audio error
  */
static AUDIO_ErrorTypeDef AUDIO_GetFileInfo(uint16_t file_idx, WAV_InfoTypedef *info)
{
  uint8_t str [FILE_NAME_SIZE + 30];
  uint8_t fname [FILE_NAME_SIZE];    
  uint32_t bytesread;
  uint32_t duration;
  
  if(MTP_GetWavObjectName(file_idx, fname) == 0)
  {
    if(MTP_GetData(file_idx, 
                   0,
                   sizeof(WAV_InfoTypedef), 
                   (uint8_t*)&wav_info,
                   &bytesread) == 0)
    {
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
      sprintf((char *)str, "Playing file (%d/%d): %s", 
              file_idx + 1, MTP_GetWavObjectNumber(),
              (char *)fname);
      BSP_LCD_ClearStringLine(4);
      BSP_LCD_DisplayStringAtLine(4 ,str);
      
      BSP_LCD_SetTextColor(LCD_COLOR_CYAN); 
      sprintf((char *)str , "Sample rate : %lu Hz", info->SampleRate);
      BSP_LCD_ClearStringLine(6);
      BSP_LCD_DisplayStringAtLine(6 ,str);
      
      sprintf((char *)str , "Channels number : %d", info->NbrChannels);
      BSP_LCD_ClearStringLine(7);      
      BSP_LCD_DisplayStringAtLine(7 ,str);
      
      duration = info->FileSize / info->ByteRate; 
      sprintf((char *)str , "File Size : %lu MB [%02lu:%02lu]", info->FileSize/1024/1024, duration/60, duration%60 );
      BSP_LCD_ClearStringLine(8);
      BSP_LCD_DisplayStringAtLine(8 ,str);
      BSP_LCD_DisplayStringAt(263, LINE(8), (uint8_t *)"[00:00]", LEFT_MODE);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
      return AUDIO_ERROR_NONE;
    }
  }
  return AUDIO_ERROR_IO;
}

/**
  * @brief  Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
  if(audio_state == AUDIO_STATE_PLAY)
  {
    buffer_ctl.offset = BUFFER_OFFSET_FULL;
    BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)&buffer_ctl.buff[0], AUDIO_BUFFER_SIZE /2);
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{ 
  if(audio_state == AUDIO_STATE_PLAY)
  {
    buffer_ctl.offset = BUFFER_OFFSET_HALF;
    BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)&buffer_ctl.buff[AUDIO_BUFFER_SIZE /2], AUDIO_BUFFER_SIZE /2);
  }
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(void)
{
}
