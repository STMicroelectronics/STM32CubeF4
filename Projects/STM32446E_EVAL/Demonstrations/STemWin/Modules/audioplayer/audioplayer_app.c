/**
  ******************************************************************************
  * @file    audioplayer_app.c
  * @author  MCD Application Team
  * @brief   Audio player application functions
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
#include "audioplayer_app.h"
#include "stm32446e_eval_audio.h"

/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_APPLICATION
  * @brief audio application routines
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FIL wav_file;
static AUDIOPLAYER_ProcessTypdef haudio;
static osMessageQId   AudioEvent;
static osThreadId     AudioThreadId = 0;

/* Private function prototypes -----------------------------------------------*/
static void Audio_Thread(void const * argument);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Init(void)
{
  /* Initialize internal audio structure */
  haudio.state  = AUDIOPLAYER_STOP;
  haudio.mute   = MUTE_OFF;
  haudio.volume = AUDIO_DEFAULT_VOLUME;  

  /* Create Audio Queue */
  osMessageQDef(AUDIO_Queue, 1, uint16_t);
  AudioEvent = osMessageCreate (osMessageQ(AUDIO_Queue), NULL); 
  
  /* Create Audio task */
  osThreadDef(osAudio_Thread, Audio_Thread, osPriorityHigh, 0, 4 * configMINIMAL_STACK_SIZE);
  AudioThreadId = osThreadCreate (osThread(osAudio_Thread), NULL);  

  BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
  return AUDIOPLAYER_ERROR_NONE;
}
/**
  * @brief  Get audio state
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_StateTypdef  AUDIOPLAYER_GetState(void)
{
  return haudio.state;
}

/**
  * @brief  Get audio volume
  * @param  None.
  * @retval Audio volume.
  */
uint32_t  AUDIOPLAYER_GetVolume(void)
{
  return haudio.volume;
}

/**
  * @brief  Set audio volume
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetVolume(uint32_t volume)
{
  if(BSP_AUDIO_OUT_SetVolume(volume) == AUDIO_OK)
  {
    haudio.volume = volume;
    return AUDIOPLAYER_ERROR_NONE;    
  }
  else
  {
    return AUDIOPLAYER_ERROR_HW;
  }
}

/**
  * @brief  Play audio stream
  * @param  frequency: Audio frequency used to play the audio stream.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Play(uint32_t frequency)
{
  uint32_t numOfReadBytes;
  haudio.state = AUDIOPLAYER_PLAY;
  
    /* Fill whole buffer @ first time */
    if(f_read(&wav_file, 
              &haudio.buffer[0], 
              AUDIO_BUFFER_SIZE, 
              (void *)&numOfReadBytes) == FR_OK)
    { 
      if(numOfReadBytes != 0)
      {
        BSP_AUDIO_OUT_SetFrequency(frequency);
        osThreadResume(AudioThreadId);
        BSP_AUDIO_OUT_Play((uint16_t*)&haudio.buffer[0], AUDIO_BUFFER_SIZE);   
        return AUDIOPLAYER_ERROR_NONE;
      }
    }
  return AUDIOPLAYER_ERROR_IO;

}

/**
  * @brief  Audio player process
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Process(void)
{
  switch(haudio.state)
  {
  case AUDIOPLAYER_START:
    haudio.state = AUDIOPLAYER_PLAY;
    break;    

  case AUDIOPLAYER_EOF:
     AUDIOPLAYER_NotifyEndOfFile();
    break;    
    
  case AUDIOPLAYER_ERROR:
     AUDIOPLAYER_Stop();
    break;
    
  case AUDIOPLAYER_STOP:
  case AUDIOPLAYER_PLAY:    
  default:
    break;
  }
  
  return AUDIOPLAYER_ERROR_NONE;
}
/**
  * @brief  Audio player DeInit
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_DeInit(void)
{
  f_close(&wav_file);

  vQueueDelete(AudioEvent);  
  osThreadTerminate(AudioThreadId);
  AUDIOPLAYER_Mute(1);
  haudio.state = AUDIOPLAYER_STOP;      
  BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Stop audio stream.
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Stop(void)
{    
  AUDIOPLAYER_Mute(1); 
  BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);    
  f_close(&wav_file);  
  osThreadSuspend(AudioThreadId); 

  haudio.state = AUDIOPLAYER_STOP;      

  return AUDIOPLAYER_ERROR_NONE;
}


/**
  * @brief  Pause Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Pause(void)
{
  osThreadSuspend(AudioThreadId);  
  BSP_AUDIO_OUT_Pause();
  return AUDIOPLAYER_ERROR_NONE;
}


/**
  * @brief  Resume Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Resume(void)
{
  osThreadResume(AudioThreadId);  
  BSP_AUDIO_OUT_Resume();
  return AUDIOPLAYER_ERROR_NONE;
}
/**
  * @brief  Sets audio stream position
  * @param  position: stream position.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetPosition(uint32_t position)
{
  long file_pos;
  
  file_pos = f_size(&wav_file)/ AUDIO_BUFFER_SIZE / 100; 
  file_pos *= (position * AUDIO_BUFFER_SIZE);
  BSP_AUDIO_OUT_Pause(); 
  GUI_Delay(100);
  f_lseek(&wav_file, file_pos);
  BSP_AUDIO_OUT_Resume(); 
  
  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Sets the volume at mute
  * @param  state: could be MUTE_ON to mute sound or MUTE_OFF to unmute 
  *                the codec and restore previous volume level.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Mute(uint8_t state)
{
   BSP_AUDIO_OUT_SetMute(state);
   
   return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  Get the wav file information.
  * @param  file: wav file.
  * @param  info: pointer to wav file structure
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_GetFileInfo(char* file, WAV_InfoTypedef* info)
{
  uint32_t numOfReadBytes;
  AUDIOPLAYER_ErrorTypdef ret = AUDIOPLAYER_ERROR_IO;
  FIL fsfile;
  
  if( f_open(&fsfile, file, FA_OPEN_EXISTING | FA_READ) == FR_OK) 
  {
    /* Fill the buffer to Send */
    if(f_read(&fsfile, info, sizeof(WAV_InfoTypedef), (void *)&numOfReadBytes) == FR_OK)
    {
      if((info->ChunkID == 0x46464952) && (info->AudioFormat == 1))
      {
        ret = AUDIOPLAYER_ERROR_NONE;
      }
    }
    f_close(&fsfile);      
  }
  return ret;
}

/**
  * @brief  Select wav file.
  * @param  file: wav file.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SelectFile(char* file)
{
  AUDIOPLAYER_ErrorTypdef ret = AUDIOPLAYER_ERROR_IO;
  if( f_open(&wav_file, file, FA_OPEN_EXISTING | FA_READ) == FR_OK) 
  {
    f_lseek(&wav_file, sizeof(WAV_InfoTypedef));
    ret = AUDIOPLAYER_ERROR_NONE;
  }
  return ret;  
}

/**
  * @brief  Get wav file progress.
  * @param  None
  * @retval file progress.
  */
uint32_t AUDIOPLAYER_GetProgress(void)
{
  return (wav_file.fptr);
}

/**
  * @brief  Manages the DMA Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
  if(haudio.state == AUDIOPLAYER_PLAY)
  {
    BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)&haudio.buffer[0], AUDIO_BUFFER_SIZE /2);
    osMessagePut ( AudioEvent, BUFFER_OFFSET_FULL, 0);    
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{ 
  if(haudio.state == AUDIOPLAYER_PLAY)
  {
    BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)&haudio.buffer[AUDIO_BUFFER_SIZE /2], AUDIO_BUFFER_SIZE /2);
    osMessagePut ( AudioEvent, BUFFER_OFFSET_HALF, 0);    
  }
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(void)
{
  haudio.state = AUDIOPLAYER_ERROR;
}

/**
  * @brief  Audio task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void Audio_Thread(void const * argument)
{
  uint32_t numOfReadBytes, remlen;    
  osEvent event;  
 
  for(;;)
  {
    event = osMessageGet(AudioEvent, 100 );
    
    if( event.status == osEventMessage )
    {
      if(haudio.state == AUDIOPLAYER_PLAY)
      {
        switch(event.value.v)
        {
        case BUFFER_OFFSET_HALF:
          
          remlen = f_size(&wav_file) - wav_file.fptr ;
            
          if(remlen < AUDIO_BUFFER_SIZE/2 )
          {
            BSP_AUDIO_OUT_SetMute(1);
            haudio.state = AUDIOPLAYER_EOF;
          }
  
          if(f_read(&wav_file, 
                    &haudio.buffer[0], 
                    AUDIO_BUFFER_SIZE/2, 
                    (void *)&numOfReadBytes) == FR_OK)
          { 
            if(numOfReadBytes == 0)
            {  
              BSP_AUDIO_OUT_SetMute(1);
              haudio.state = AUDIOPLAYER_EOF;
            } 
          }
          else
          {
            BSP_AUDIO_OUT_SetMute(1);
            haudio.state = AUDIOPLAYER_ERROR;    
          }
          break;  
          
        case BUFFER_OFFSET_FULL:
          
          
          remlen = f_size(&wav_file) - wav_file.fptr ;
            
          if(remlen < AUDIO_BUFFER_SIZE/2 )
          {
            BSP_AUDIO_OUT_SetMute(1);
            haudio.state = AUDIOPLAYER_EOF;            
          }
          
          if(f_read(&wav_file, 
                    &haudio.buffer[AUDIO_BUFFER_SIZE/2], 
                    AUDIO_BUFFER_SIZE/2, 
                    (void *)&numOfReadBytes) == FR_OK)
          { 
            if(numOfReadBytes == 0)
            { 
              BSP_AUDIO_OUT_SetMute(1);              
              haudio.state = AUDIOPLAYER_EOF;                     
            } 
          }
          else
          {
            BSP_AUDIO_OUT_SetMute(1);            
            haudio.state = AUDIOPLAYER_ERROR;   
          }
          break;   
          
        default:
          break;
        }
        
        
        
        
      }
      
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
