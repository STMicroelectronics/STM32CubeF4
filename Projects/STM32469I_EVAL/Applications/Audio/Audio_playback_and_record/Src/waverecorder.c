/**
  ******************************************************************************
  * @file    Audio/Audio_playback_and_record/Src/waverecorder.c 
  * @author  MCD Application Team
  * @brief   This file provides the Audio In (record) interface API
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
#include "waverecorder.h" 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
uint8_t pHeaderBuff[44];

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static AUDIO_IN_BufferTypeDef  BufferCtl;

extern WAVE_FormatTypeDef WaveFormat;

/* Private function prototypes -----------------------------------------------*/
static uint32_t WavProcess_EncInit(uint32_t Freq, uint8_t* pHeader);
static uint32_t WavProcess_HeaderInit(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct);
static uint32_t WavProcess_HeaderUpdate(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct);

/* Private functions ---------------------------------------------------------*/

/*  
  A double MEMS microphone MP45DT02 mounted on STM324X9I-EVAL is connected 
  to the Inter-IC Sound (I2S) peripheral. The I2S is configured in master 
  receiver mode. In this mode, the I2S peripheral provides the clock to the MEMS 
  microphones through CLK_IN and acquires the data (Audio samples) from the MEMS 
  microphone through PDM_OUT.

  Data acquisition is performed in 16-bit PDM format and using I2S DMA mode.
  
  DMA is configured in circular mode

  In order to avoid data-loss, a 128 bytes buffer is used (BufferCtl.pdm_buff): 
   - When a DMA half transfer is detected using the call back BSP_AUDIO_IN_HalfTransfer_CallBack()
    PDM frame has been received (64 bytes), a conversion to PCM is done 
    and then this PCM frame is saved in RecBuf.
  - After converting/filtering samples from PDM to PCM, the samples are stored in USB buffer.
  - These two steps are repeated  when the DMA Transfer complete interrupt is detected
  - When half of internal USB buffer is reach, an evacuation though USB is done.
  
  To avoid data-loss:
  - IT ISR priority must be set at a higher priority than USB, this priority 
    order must be respected when managing other interrupts; 
  - The processing time of converting/filtering samples from PDM to PCM 
    PDM_Filter_64_LSB()) should be lower than the time required to fill a 
    single buffer. 

  Note that a PDM Audio software decoding library provided in binary is used in 
  this application. For IAR EWARM toolchain, the library is labeled 
  "libPDMFilter_CM4_IAR.a".
*/

/**
  * @brief  Starts Audio streaming.    
  * @param  None
  * @retval Audio error
  */ 
AUDIO_ErrorTypeDef AUDIO_REC_Start(void)
{
  uint32_t byteswritten = 0;
  uint8_t str[FILEMGR_FILE_NAME_SIZE + 20]; 
  
  /* Create a new file system */
  if(f_open(&WavFile, REC_WAVE_NAME, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
  {  
    /* Initialize header file */
    WavProcess_EncInit(DEFAULT_AUDIO_IN_FREQ, pHeaderBuff);
    
    /* Write header file */
    if(f_write(&WavFile, pHeaderBuff, 44, (void*)&byteswritten) == FR_OK)
    {
      AudioState = AUDIO_STATE_RECORD;
      
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
      sprintf((char *)str, "Recording file: %s", 
              (char *)REC_WAVE_NAME);
      BSP_LCD_ClearStringLine(4);
      BSP_LCD_DisplayStringAtLine(4, str);
      
      BSP_LCD_SetTextColor(LCD_COLOR_CYAN); 
      sprintf((char *)str,  "Sample rate : %d Hz", (int)DEFAULT_AUDIO_IN_FREQ);
      BSP_LCD_ClearStringLine(6);
      BSP_LCD_DisplayStringAtLine(6, str);
      
      sprintf((char *)str,  "Channels number : %d", (int)DEFAULT_AUDIO_IN_CHANNEL_NBR);
      BSP_LCD_ClearStringLine(7);      
      BSP_LCD_DisplayStringAtLine(7, str);
      
      sprintf((char *)str, "File Size :");
      BSP_LCD_ClearStringLine(8);
      BSP_LCD_DisplayStringAtLine(8, str);
      
      BSP_LCD_DisplayStringAt(250, LINE(14), (uint8_t *)"  [RECORD]", LEFT_MODE);
      { 
        if(byteswritten != 0)
        {
          BSP_AUDIO_IN_Init(DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);
          BSP_AUDIO_IN_Record((uint16_t*)&BufferCtl.pdm_buff[0], AUDIO_IN_PDM_BUFFER_SIZE);
          BufferCtl.fptr = byteswritten;
          BufferCtl.pcm_ptr = 0;
          BufferCtl.offset = 0;
          BufferCtl.wr_state = BUFFER_EMPTY;
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
AUDIO_ErrorTypeDef AUDIO_REC_Process(void)
{
  uint32_t byteswritten = 0;
  AUDIO_ErrorTypeDef audio_error = AUDIO_ERROR_NONE;
  uint32_t elapsed_time; 
  static uint32_t prev_elapsed_time = 0xFFFFFFFF;
  uint8_t str[10];  
  
  switch(AudioState)
  {
  case AUDIO_STATE_RECORD:   
    
    /* MAX Recording time reached, so stop audio interface and close file */
    if(BufferCtl.fptr >= REC_SAMPLE_LENGTH)
    {
      AudioState = AUDIO_STATE_STOP;
      break;
    }
    
    /* Check if there are Data to write in Usb Key */
    if(BufferCtl.wr_state == BUFFER_FULL)
    {
      /* write buffer in file */
      if(f_write(&WavFile, (uint8_t*)(BufferCtl.pcm_buff + BufferCtl.offset), 
                 AUDIO_IN_PCM_BUFFER_SIZE, 
                 (void*)&byteswritten) != FR_OK)
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAtLine(16, (uint8_t *)"RECORD FAIL");
        return AUDIO_ERROR_IO;
      }
      BufferCtl.fptr += byteswritten;
      BufferCtl.wr_state =  BUFFER_EMPTY;
    }
    
    /* Display elapsed time */
    elapsed_time = BufferCtl.fptr / (DEFAULT_AUDIO_IN_FREQ * DEFAULT_AUDIO_IN_CHANNEL_NBR * 2); 
    if(prev_elapsed_time != elapsed_time)
    {
      prev_elapsed_time = elapsed_time;
      sprintf((char *)str, "[%02d:%02d]", (int)(elapsed_time /60), (int)(elapsed_time%60));
      BSP_LCD_SetTextColor(LCD_COLOR_CYAN); 
      BSP_LCD_DisplayStringAt(263, LINE(8), str, LEFT_MODE);
      sprintf((char *)str, "%4d KB", (int)((int32_t)BufferCtl.fptr/1024));
      BSP_LCD_DisplayStringAt(83, LINE(8), str, LEFT_MODE);
    }
    break;
    
  case AUDIO_STATE_STOP:
    /* Stop recorder */
    BSP_AUDIO_IN_Stop();
    
    /* Create a new file system */
    if(f_lseek(&WavFile, 0) == FR_OK)
    {
      /* Update the wav file header save it into wav file */
      WavProcess_HeaderUpdate(pHeaderBuff, &WaveFormat);
      
      if(f_write(&WavFile, pHeaderBuff, sizeof(WAVE_FormatTypeDef), (void*)&byteswritten) == FR_OK)
      {   
        audio_error = AUDIO_ERROR_EOF;
      }
      else
      {
        audio_error = AUDIO_ERROR_IO;
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAtLine(16, (uint8_t *)"RECORD FAIL");          
      }
    }
    else
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_DisplayStringAtLine(16, (uint8_t *)"RECORD FAIL");
      audio_error = AUDIO_ERROR_IO;      
    }
    AudioState = AUDIO_STATE_IDLE;      
    /* Close file */
    f_close(&WavFile);
    break;
    
  case AUDIO_STATE_PAUSE:
    BSP_LCD_DisplayStringAt(250, LINE(14), (uint8_t *)"  [PAUSE]   ", LEFT_MODE);
    BSP_AUDIO_IN_Pause();
    AudioState = AUDIO_STATE_WAIT;
    break;
    
  case AUDIO_STATE_RESUME:
    BSP_LCD_DisplayStringAt(250, LINE(14), (uint8_t *)"  [RECORD]   ", LEFT_MODE);
    BSP_AUDIO_IN_Resume();
    AudioState = AUDIO_STATE_RECORD;
    break;
    
  case AUDIO_STATE_VOLUME_UP: 
    if(AudioInVolume <= 90)
    {
      AudioInVolume += 10;
    }
    BSP_AUDIO_IN_SetVolume(AudioInVolume);
    AudioState = AUDIO_STATE_RECORD;
    break;
    
  case AUDIO_STATE_VOLUME_DOWN:    
    if(AudioInVolume >= 10)
    {
      AudioInVolume -= 10;
    }
    BSP_AUDIO_IN_SetVolume(AudioInVolume);
    AudioState = AUDIO_STATE_RECORD;
    break;
    
  case AUDIO_STATE_NEXT:
  case AUDIO_STATE_PREVIOUS:   
    AudioState = AUDIO_STATE_RECORD;
    break;
    
  case AUDIO_STATE_WAIT: 
  case AUDIO_STATE_IDLE:
  case AUDIO_STATE_INIT: 
  default:
    /* Do Nothing */
    break;
  }
  return audio_error;
}

/**
  * @brief  Calculates the remaining file size and new position of the pointer.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  /* PDM to PCM data convert */
  BSP_AUDIO_IN_PDMToPCM((uint16_t*)&BufferCtl.pdm_buff[AUDIO_IN_PDM_BUFFER_SIZE/2], 
                        &BufferCtl.pcm_buff[BufferCtl.pcm_ptr]);
  
  BufferCtl.pcm_ptr+= AUDIO_IN_PDM_BUFFER_SIZE/4/2;
  
  if(BufferCtl.pcm_ptr == AUDIO_IN_PCM_BUFFER_SIZE/2)
  {
    BufferCtl.wr_state   =  BUFFER_FULL;
    BufferCtl.offset  = 0;
  }
  
  if(BufferCtl.pcm_ptr >= AUDIO_IN_PCM_BUFFER_SIZE)
  {
    BufferCtl.wr_state   =  BUFFER_FULL;
    BufferCtl.offset  = AUDIO_IN_PCM_BUFFER_SIZE/2;    
    BufferCtl.pcm_ptr = 0;
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{ 
  /* PDM to PCM data convert */
  BSP_AUDIO_IN_PDMToPCM((uint16_t*)&BufferCtl.pdm_buff[0], 
                        &BufferCtl.pcm_buff[BufferCtl.pcm_ptr]);
  
  BufferCtl.pcm_ptr+= AUDIO_IN_PDM_BUFFER_SIZE/4/2;
  
  if(BufferCtl.pcm_ptr == AUDIO_IN_PCM_BUFFER_SIZE/2)
  {
    BufferCtl.wr_state   =  BUFFER_FULL;
    BufferCtl.offset  = 0;
  }
  
  if(BufferCtl.pcm_ptr >= AUDIO_IN_PCM_BUFFER_SIZE)
  {
    BufferCtl.wr_state   =  BUFFER_FULL;
    BufferCtl.offset  = AUDIO_IN_PCM_BUFFER_SIZE/2;    
    BufferCtl.pcm_ptr = 0;
  }
}

/*******************************************************************************
                            Static Functions
*******************************************************************************/

/**
  * @brief  Encoder initialization.
  * @param  Freq: Sampling frequency.
  * @param  pHeader: Pointer to the WAV file header to be written.  
  * @retval 0 if success, !0 else.
  */
static uint32_t WavProcess_EncInit(uint32_t Freq, uint8_t *pHeader)
{  
  /* Initialize the encoder structure */
  WaveFormat.SampleRate = Freq;        /* Audio sampling frequency */
  WaveFormat.NbrChannels = 2;          /* Number of channels: 1:Mono or 2:Stereo */
  WaveFormat.BitPerSample = 16;        /* Number of bits per sample (16, 24 or 32) */
  WaveFormat.FileSize = 0x001D4C00;    /* Total length of useful audio data (payload) */
  WaveFormat.SubChunk1Size = 44;       /* The file header chunk size */
  WaveFormat.ByteRate = (WaveFormat.SampleRate * \
                        (WaveFormat.BitPerSample/8) * \
                         WaveFormat.NbrChannels);     /* Number of bytes per second  (sample rate * block align)  */
  WaveFormat.BlockAlign = WaveFormat.NbrChannels * \
                         (WaveFormat.BitPerSample/8); /* channels * bits/sample / 8 */
  
  /* Parse the wav file header and extract required information */
  if(WavProcess_HeaderInit(pHeader, &WaveFormat))
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Initialize the wave header file
  * @param  pHeader: Header Buffer to be filled
  * @param  pWaveFormatStruct: Pointer to the wave structure to be filled.
  * @retval 0 if passed, !0 if failed.
  */
static uint32_t WavProcess_HeaderInit(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct)
{
  /* Write chunkID, must be 'RIFF'  ------------------------------------------*/
  pHeader[0] = 'R';
  pHeader[1] = 'I';
  pHeader[2] = 'F';
  pHeader[3] = 'F';
  
  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the 
     recording operation.  Example: 661500 Bytes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  pHeader[4] = 0x00;
  pHeader[5] = 0x4C;
  pHeader[6] = 0x1D;
  pHeader[7] = 0x00;
  /* Write the file format, must be 'WAVE' -----------------------------------*/
  pHeader[8]  = 'W';
  pHeader[9]  = 'A';
  pHeader[10] = 'V';
  pHeader[11] = 'E';
  
  /* Write the format chunk, must be'fmt ' -----------------------------------*/
  pHeader[12]  = 'f';
  pHeader[13]  = 'm';
  pHeader[14]  = 't';
  pHeader[15]  = ' ';
  
  /* Write the length of the 'fmt' data, must be 0x10 ------------------------*/
  pHeader[16]  = 0x10;
  pHeader[17]  = 0x00;
  pHeader[18]  = 0x00;
  pHeader[19]  = 0x00;
  
  /* Write the audio format, must be 0x01 (PCM) ------------------------------*/
  pHeader[20]  = 0x01;
  pHeader[21]  = 0x00;
  
  /* Write the number of channels, ie. 0x01 (Mono) ---------------------------*/
  pHeader[22]  = pWaveFormatStruct->NbrChannels;
  pHeader[23]  = 0x00;
  
  /* Write the Sample Rate in Hz ---------------------------------------------*/
  /* Write Little Endian ie. 8000 = 0x00001F40 => byte[24]=0x40, byte[27]=0x00*/
  pHeader[24]  = (uint8_t)((pWaveFormatStruct->SampleRate & 0xFF));
  pHeader[25]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 8) & 0xFF);
  pHeader[26]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 16) & 0xFF);
  pHeader[27]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 24) & 0xFF);
  
  /* Write the Byte Rate -----------------------------------------------------*/
  pHeader[28]  = (uint8_t)((pWaveFormatStruct->ByteRate & 0xFF));
  pHeader[29]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 8) & 0xFF);
  pHeader[30]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 16) & 0xFF);
  pHeader[31]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 24) & 0xFF);
  
  /* Write the block alignment -----------------------------------------------*/
  pHeader[32]  = pWaveFormatStruct->BlockAlign;
  pHeader[33]  = 0x00;
  
  /* Write the number of bits per sample -------------------------------------*/
  pHeader[34]  = pWaveFormatStruct->BitPerSample;
  pHeader[35]  = 0x00;
  
  /* Write the Data chunk, must be 'data' ------------------------------------*/
  pHeader[36]  = 'd';
  pHeader[37]  = 'a';
  pHeader[38]  = 't';
  pHeader[39]  = 'a';
  
  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  pHeader[40]  = 0x00;
  pHeader[41]  = 0x4C;
  pHeader[42]  = 0x1D;
  pHeader[43]  = 0x00;
  
  /* Return 0 if all operations are OK */
  return 0;
}

/**
  * @brief  Initialize the wave header file
  * @param  pHeader: Header Buffer to be filled
  * @param  pWaveFormatStruct: Pointer to the wave structure to be filled.
  * @retval 0 if passed, !0 if failed.
  */
static uint32_t WavProcess_HeaderUpdate(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct)
{
  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the 
     recording operation.  Example: 661500 Bytes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  pHeader[4] = (uint8_t)(BufferCtl.fptr);
  pHeader[5] = (uint8_t)(BufferCtl.fptr >> 8);
  pHeader[6] = (uint8_t)(BufferCtl.fptr >> 16);
  pHeader[7] = (uint8_t)(BufferCtl.fptr >> 24);
  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  BufferCtl.fptr -=44;
  pHeader[40] = (uint8_t)(BufferCtl.fptr); 
  pHeader[41] = (uint8_t)(BufferCtl.fptr >> 8);
  pHeader[42] = (uint8_t)(BufferCtl.fptr >> 16);
  pHeader[43] = (uint8_t)(BufferCtl.fptr >> 24); 
  
  /* Return 0 if all operations are OK */
  return 0;
}
