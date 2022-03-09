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
#include "audio_player_app.h"

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
const uint8_t VOL_LIMIT_TABLE[] = 
{
  0,  15, 23, 30, 34, 38, 42, 45, 47, 50,
  52, 53, 55, 57, 58, 60, 61, 62, 63, 65,
  66, 67, 68, 69, 69, 70, 71, 72, 73, 73,
  74, 75, 75, 76, 77, 77, 78, 78, 79, 80,
  80, 81, 81, 82, 82, 83, 83, 84, 84, 84,
  85, 85, 86, 86, 87, 87, 87, 88, 88, 88,
  89, 89, 89, 90, 90, 90, 91, 91, 91, 92,
  92, 92, 93, 93, 93, 94, 94, 94, 94, 95,
  95, 95, 95, 96, 96, 96, 96, 97, 97, 97,
  97, 98, 98, 98, 98, 99, 99, 99, 99, 100, 
  100   
 };

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FIL wav_file;


static osMessageQId   AudioEvent = 0;
static osThreadId     AudioThreadId = 0;

/* Private function prototypes -----------------------------------------------*/
static void Audio_Thread(void const * argument);

#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
__IO uint32_t  AUDIO_EqInstance[SPIRIT_EQ_PERSIST_SIZE_IN_BYTES/4]  ;
TSpiritEQ_Band AUDIO_EQ_Bands[SPIRIT_EQ_MAX_BANDS];

__IO uint32_t AUDIO_LdCtrlPersistance[SPIRIT_LDCTRL_PERSIST_SIZE_IN_BYTES/4];
__IO uint32_t AUDIO_LdCtrlScratch[SPIRIT_LDCTRL_SCRATCH_SIZE_IN_BYTES/4];
TSpiritLdCtrl_Prms AUDIO_LdCtrlInstanceParams;
TSpiritEQ_Band *tmpEqBand;
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */

/* Private function prototypes -----------------------------------------------*/
static void Audio_Thread(void const * argument);
static void AUDIO_TransferComplete_CallBack(void);
static void AUDIO_HalfTransfer_CallBack(void);
static void AUDIO_Error_CallBack(void);



/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes audio
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Init(uint8_t volume)
{
#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
  uint32_t index = 0;
  __IO uint32_t ldness_value;
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */

  /* Try to Init Audio interface in different config in case of failure */
  BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, volume, I2S_AUDIOFREQ_48K);
  BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
  
  /* Initialize internal audio structure */
  haudio.out.state  = AUDIOPLAYER_STOP;
  haudio.out.mute   = MUTE_OFF;
  haudio.out.volume = volume;  

#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
  /* Enable the Eq */
  SpiritEQ_Init((TSpiritEq *)AUDIO_EqInstance, I2S_AUDIOFREQ_48K);
  
  /* Retrieve stored settings and set band params */
  SpiritEQ_FltGet((TSpiritEq *)AUDIO_EqInstance, &AUDIO_EQ_Bands[0], 0 );
  AUDIO_EQ_Bands[0].gainDb = k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_EQU1_BKP);
  SET_BAND_PRMS(&AUDIO_EQ_Bands[0], SPIRIT_EQ_FLT_TYPE_SHELVING_LOWPASS , 0, 1000, AUDIO_EQ_Bands[0].gainDb);
  
  SpiritEQ_FltGet((TSpiritEq *)AUDIO_EqInstance, &AUDIO_EQ_Bands[1], 1 );
  AUDIO_EQ_Bands[1].gainDb = k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_EQU2_BKP);    
  SET_BAND_PRMS(&AUDIO_EQ_Bands[1], SPIRIT_EQ_FLT_TYPE_PEAKING , 2000, 1000, AUDIO_EQ_Bands[1].gainDb);
  
  SpiritEQ_FltGet((TSpiritEq *)AUDIO_EqInstance, &AUDIO_EQ_Bands[2], 2 );
  AUDIO_EQ_Bands[2].gainDb = k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_EQU3_BKP);   
  SET_BAND_PRMS(&AUDIO_EQ_Bands[2], SPIRIT_EQ_FLT_TYPE_PEAKING , 5000, 3000, AUDIO_EQ_Bands[2].gainDb);
  
  SpiritEQ_FltGet((TSpiritEq *)AUDIO_EqInstance, &AUDIO_EQ_Bands[3], 3 );
  AUDIO_EQ_Bands[3].gainDb = k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_EQU4_BKP);;  
  SET_BAND_PRMS(&AUDIO_EQ_Bands[3], SPIRIT_EQ_FLT_TYPE_PEAKING , 10000, 6000, AUDIO_EQ_Bands[3].gainDb);
  
  SpiritEQ_FltGet((TSpiritEq *)AUDIO_EqInstance, &AUDIO_EQ_Bands[4], 4 );
  AUDIO_EQ_Bands[4].gainDb = k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_EQU5_BKP);  
  SET_BAND_PRMS(&AUDIO_EQ_Bands[4], SPIRIT_EQ_FLT_TYPE_SHELVING_HIPASS , 15000, 2000, AUDIO_EQ_Bands[4].gainDb);

  for (index = 0; index < SPIRIT_EQ_MAX_BANDS ; index++)
  {
    tmpEqBand = &AUDIO_EQ_Bands[index];
    SpiritEQ_FltSet((TSpiritEq *)AUDIO_EqInstance, tmpEqBand, index);
  }
  
  /* Enable Loundness Control */
  SpiritLdCtrl_Init((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, I2S_AUDIOFREQ_48K);
  SpiritLdCtrl_GetPrms((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, &AUDIO_LdCtrlInstanceParams);
  ldness_value = k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_LOUD_BKP);
  AUDIO_LdCtrlInstanceParams.gainQ8 = PERC_TO_LDNS_DB(ldness_value);
  SpiritLdCtrl_SetPrms((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, &AUDIO_LdCtrlInstanceParams);
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */
  
  /* Register audio BSP drivers callbacks */
  AUDIO_IF_RegisterCallbacks(AUDIO_TransferComplete_CallBack, 
                             AUDIO_HalfTransfer_CallBack, 
                             AUDIO_Error_CallBack);
    
    
  /* Create Audio Queue */
  osMessageQDef(AUDIO_Queue, 1, uint16_t);
  AudioEvent = osMessageCreate (osMessageQ(AUDIO_Queue), NULL); 
  
  /* Create Audio task */
  osThreadDef(osAudio_Thread, Audio_Thread, osPriorityRealtime, 0, 512);
  AudioThreadId = osThreadCreate (osThread(osAudio_Thread), NULL);  

  return AUDIOPLAYER_ERROR_NONE;
}

#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
/**
  * @brief  This function Set the new gain of the equilizer
  * @param  BandNum : equilizer band index
  * @param  NewGainValue : the new band gain.
  * @retval None.
*/
void AUDIO_SetEq(uint8_t BandNum, int16_t NewGainValue)
{
  /* GetBand */
  SpiritEQ_FltGet((TSpiritEq *)AUDIO_EqInstance, &AUDIO_EQ_Bands[BandNum], BandNum );
  
  /* SetNewValue */
  AUDIO_EQ_Bands[BandNum].gainDb = PERC_TO_EQUI_DB(NewGainValue);
  
  SpiritEQ_FltSet((TSpiritEq *)AUDIO_EqInstance, &AUDIO_EQ_Bands[BandNum], BandNum);
}

/**
  * @brief  This function Set Loudness Control Gain Value.
  * @param  NewGainValue in %.
  * @retval None.
  */
void AUDIO_SetLoudnessGain(int16_t NewGainValue)
{
  /* Get old Gain */
  SpiritLdCtrl_GetPrms((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, &AUDIO_LdCtrlInstanceParams);
  AUDIO_LdCtrlInstanceParams.gainQ8 = PERC_TO_LDNS_DB (NewGainValue);
  SpiritLdCtrl_Reset  ((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance);
  SpiritLdCtrl_SetPrms((TSpiritLdCtrl*)AUDIO_LdCtrlPersistance, &AUDIO_LdCtrlInstanceParams);
}

/**
  * @brief  This function Set the new gain of the equilizer
  * @param  BandNum : equilizer band index
  * @param  NewGainValue : the new band gain.
  * @retval None.
*/
void AUDIO_SetEqParams(uint32_t loudness_perc)
{
  
  k_BkupSaveParameter(CALIBRATION_AUDIOPLAYER_EQU1_BKP, AUDIO_EQ_Bands[0].gainDb);
  k_BkupSaveParameter(CALIBRATION_AUDIOPLAYER_EQU2_BKP, AUDIO_EQ_Bands[1].gainDb);
  k_BkupSaveParameter(CALIBRATION_AUDIOPLAYER_EQU3_BKP, AUDIO_EQ_Bands[2].gainDb);
  k_BkupSaveParameter(CALIBRATION_AUDIOPLAYER_EQU4_BKP, AUDIO_EQ_Bands[3].gainDb);
  k_BkupSaveParameter(CALIBRATION_AUDIOPLAYER_EQU5_BKP, AUDIO_EQ_Bands[4].gainDb);
  
  k_BkupSaveParameter(CALIBRATION_AUDIOPLAYER_LOUD_BKP, loudness_perc);
}
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */ 

/**
  * @brief  Get audio state
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_StateTypdef  AUDIOPLAYER_GetState(void)
{
  return haudio.out.state;
}

/**
  * @brief  Get audio volume
  * @param  None.
  * @retval Audio volume.
  */
uint32_t  AUDIOPLAYER_GetVolume(void)
{
  return haudio.out.volume;
}

/**
  * @brief  Set audio volume
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetVolume(uint32_t volume)
{
  if(BSP_AUDIO_OUT_SetVolume(VOL_LIMIT_TABLE[volume]) == AUDIO_OK)
  {
    haudio.out.volume = volume;
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
  haudio.out.state = AUDIOPLAYER_PLAY;
  osDelay(100);
    /* Fill whole buffer @ first time */
    if(f_read(&wav_file, 
              &haudio.buff[0], 
              AUDIO_OUT_BUFFER_SIZE, 
              (void *)&numOfReadBytes) == FR_OK)
    { 
      if(numOfReadBytes != 0)
      {
        BSP_AUDIO_OUT_Pause();
        BSP_AUDIO_OUT_SetFrequency(frequency);
        osThreadResume(AudioThreadId);
        BSP_AUDIO_OUT_Play((uint16_t*)&haudio.buff[0], AUDIO_OUT_BUFFER_SIZE);   
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
  switch(haudio.out.state)
  {
  case AUDIOPLAYER_START:
    haudio.out.state = AUDIOPLAYER_PLAY;
    break;    

  case AUDIOPLAYER_EOF:
     haudio.out.state = AUDIOPLAYER_EOF;
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
  haudio.out.state = AUDIOPLAYER_STOP; 
  
  BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW);
  BSP_AUDIO_OUT_DeInit();

  f_close(&wav_file); 
  
  if(AudioEvent != 0)
  {
    vQueueDelete(AudioEvent); 
    AudioEvent = 0;
  }
  if(AudioThreadId != 0)
  {
    osThreadTerminate(AudioThreadId);
    AudioThreadId = 0;
  }
 
  return AUDIOPLAYER_ERROR_NONE;  
}

/**
  * @brief  Stop audio stream.
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Stop(void)
{
  haudio.out.state = AUDIOPLAYER_STOP;
  BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW); 
  
  if(AudioThreadId != 0)
  {  
    osThreadSuspend(AudioThreadId); 
  }
  f_close(&wav_file);


  return AUDIOPLAYER_ERROR_NONE;
}


/**
  * @brief  Pause Audio stream
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Pause(void)
{
  if(AudioThreadId != 0)
  {
    osThreadSuspend(AudioThreadId); 
  }
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
  if(AudioThreadId != 0)
  {  
    osThreadResume(AudioThreadId);  
  }
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
  
  file_pos = (f_size(&wav_file)) / AUDIO_OUT_BUFFER_SIZE / 100; 
  file_pos *= (position * AUDIO_OUT_BUFFER_SIZE); 
  f_lseek(&wav_file, file_pos);
  
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
static void AUDIO_TransferComplete_CallBack(void)
{
  if(haudio.out.state == AUDIOPLAYER_PLAY)
  {
    BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)&haudio.buff[0], AUDIO_OUT_BUFFER_SIZE /2);
    osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_FULL, 0);    
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
static void AUDIO_HalfTransfer_CallBack(void)
{ 
  if(haudio.out.state == AUDIOPLAYER_PLAY)
  {
    BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)&haudio.buff[AUDIO_OUT_BUFFER_SIZE /2], AUDIO_OUT_BUFFER_SIZE /2);
    osMessagePut ( AudioEvent, PLAY_BUFFER_OFFSET_HALF, 0);    
  }
}

/**
  * @brief  Manages the DMA FIFO error interrupt.
  * @param  None
  * @retval None
  */
static void AUDIO_Error_CallBack(void)
{
  haudio.out.state = AUDIOPLAYER_ERROR;
}

/**
  * @brief  Audio task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void Audio_Thread(void const * argument)
{
  uint32_t numOfReadBytes;    
  osEvent event;  
  for(;;)
  {
    event = osMessageGet(AudioEvent, 100 );
    
    if( event.status == osEventMessage )
    {
      if(haudio.out.state == AUDIOPLAYER_PLAY)
      {
        switch(event.value.v)
        {
        case PLAY_BUFFER_OFFSET_HALF:
          if(f_read(&wav_file, 
                    &haudio.buff[0], 
                    AUDIO_OUT_BUFFER_SIZE/2, 
                    (void *)&numOfReadBytes) == FR_OK)
          { 
            if(numOfReadBytes == 0)
            {  
              haudio.out.state = AUDIOPLAYER_EOF;
            } 
            
#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
            SpiritEQ_Apply((void *)AUDIO_EqInstance,
                           /* NB_Channel */2, 
                           (int16_t *)&haudio.buff[0], 
                           numOfReadBytes  / 4);
            
            /* Apply Loudness settings */
            SpiritLdCtrl_Apply((TSpiritLdCtrl *)AUDIO_LdCtrlPersistance,
                               /* NB_Channel */ 2, 
                               (int16_t *)&haudio.buff[0], 
                               numOfReadBytes / 4, 
                               (void *)AUDIO_LdCtrlScratch);            
            
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */
          }
          else
          {
              haudio.out.state = AUDIOPLAYER_ERROR;    
          }
          break;  
          
        case PLAY_BUFFER_OFFSET_FULL:
          if(f_read(&wav_file, 
                    &haudio.buff[AUDIO_OUT_BUFFER_SIZE/2], 
                    AUDIO_OUT_BUFFER_SIZE/2, 
                    (void *)&numOfReadBytes) == FR_OK)
          { 
            if(numOfReadBytes == 0)
            { 
              haudio.out.state = AUDIOPLAYER_EOF;                     
            } 
#if defined(AUDIO_USE_SPIRIT_EQUALIZER)
            SpiritEQ_Apply((void *)AUDIO_EqInstance,
                           /* NB_Channel */2, 
                           (int16_t *)&haudio.buff[AUDIO_OUT_BUFFER_SIZE /2], 
                           numOfReadBytes  / 4);
            
            
            /* Apply Loudness settings */
            SpiritLdCtrl_Apply((TSpiritLdCtrl *)AUDIO_LdCtrlPersistance,
                               /* NB_Channel */ 2, 
                               (int16_t *)&haudio.buff[AUDIO_OUT_BUFFER_SIZE /2], 
                               numOfReadBytes / 4, 
                               (void *)AUDIO_LdCtrlScratch);                
#endif /* AUDIO_USE_SPIRIT_EQUALIZER */
          }
          else
          {
              haudio.out.state = AUDIOPLAYER_ERROR;   
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
