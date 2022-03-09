/**
  ******************************************************************************
  * @file    Audio_playback_and_record/Src/waverecorder.c 
  * @author  MCD Application Team
  * @brief   I2S Audio recorder program.
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
#include "waverecorder.h" 
#include "string.h"

/** @addtogroup STM32F4-Discovery_Audio_Player_Recorder
* @{
*/ 

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  int32_t offset;
  uint32_t fptr;
}Audio_BufferTypeDef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Wave recorded counter.*/
__IO uint32_t WaveCounter = 0;

extern __IO uint32_t CmdIndex, LEDsState, TimeRecBase;

FIL WavFile;

/* USB variable to check if USB connected or not*/
extern MSC_ApplicationTypeDef AppliState;

/* Variable used to switch play from audio sample available on USB to recorded file*/
uint32_t WaveRecStatus = 0; 

uint8_t pHeaderBuff[44];
uint16_t WrBuffer[WR_BUFFER_SIZE];

static uint16_t RecBuf[PCM_OUT_SIZE*2];/* PCM stereo samples are saved in RecBuf */
static uint16_t InternalBuffer[INTERNAL_BUFF_SIZE];
__IO uint32_t ITCounter = 0;

Audio_BufferTypeDef  BufferCtl;

/* Temporary data sample */
__IO uint32_t AUDIODataReady = 0, AUDIOBuffOffset = 0;

WAVE_FormatTypeDef WaveFormat;

__IO FRESULT res;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static uint32_t WavProcess_EncInit(uint32_t Freq, uint8_t* pHeader);
static uint32_t WavProcess_HeaderInit(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct);
static uint32_t WavProcess_HeaderUpdate(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct);

/*  
  A single MEMS microphone MP45DT02 mounted on STM32F4-Discovery is connected 
  to the Inter-IC Sound (I2S) peripheral. The I2S is configured in master 
  receiver mode. In this mode, the I2S peripheral provides the clock to the MEMS 
  microphone through CLK_IN and acquires the data (Audio samples) from the MEMS 
  microphone through PDM_OUT.

  Data acquisition is performed in 16-bit PDM format and using I2S IT mode.
  
  In order to avoid data-loss, two buffers are used: 
   - When there are enough data to be transmitted to USB, there is a buffer reception
   switch. 
  
  To avoid data-loss:
  - IT ISR priority must be set at a higher priority than USB, this priority 
    order must be respected when managing other interrupts; 

  Note that a PDM Audio software decoding library provided in binary is used in 
  this application. For IAR EWARM toolchain, the library is labeled 
  "libPDMFilter_CM4_IAR.a".
*/


/**
  * @brief  Start Audio recording.
  * @param  pbuf: pointer to a buffer
  *         size: Buffer size
  * @retval None
  */
uint8_t WaveRecorderStart(uint16_t* pBuf, uint32_t wSize)
{
  return (BSP_AUDIO_IN_Record(pBuf, wSize));
}

/**
  * @brief  Stop Audio recording.
  * @param  None
  * @retval None
  */
uint32_t WaveRecorderStop(void)
{
  return BSP_AUDIO_IN_Stop();
}

/**
  * @brief  Update the recorded data. 
  * @param  None
  * @retval None
  */
void WaveRecorderProcess(void)
{     
  /* Current size of the recorded buffer */
  uint32_t byteswritten = 0; 

  WaveCounter = 0;
  LEDsState = LEDS_OFF;

  /* Remove Wave file if it exists on USB Flash Disk */
  f_unlink(REC_WAVE_NAME);
  
  /* Open the file to write on it */
  if((AppliState == APPLICATION_IDLE) || (f_open(&WavFile, REC_WAVE_NAME, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK))
  {   
    while(1)
    {
     /* Toggle LED5 in infinite loop to signal that: USB Flash Disk is not connected/removed
      or an issue has occurred when creating/opening Wave file */
      BSP_LED_Toggle(LED5); 
    }
  }
  else
  {
    WaveRecStatus = 1;
  }
  /* Initialize header file */
  WavProcess_EncInit(DEFAULT_AUDIO_IN_FREQ, pHeaderBuff);
  
  /* Write the header Wave */
  f_write(&WavFile, pHeaderBuff, 44, (void *)&byteswritten);
  
  /* Increment the Wave counter */  
  BufferCtl.fptr = byteswritten;
  
  BufferCtl.offset = BUFFER_OFFSET_NONE;
  BSP_AUDIO_IN_Init(DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);
  BSP_AUDIO_IN_Record((uint16_t*)&InternalBuffer[0], INTERNAL_BUFF_SIZE);
  
  /* Reset the time recording base variable */
  TimeRecBase = 0;
  ITCounter = 0;
  LEDsState = LED3_TOGGLE;
  
  while(AppliState != APPLICATION_IDLE)
  { 
    /* Wait for the recording time */  
    if(TimeRecBase <= DEFAULT_TIME_REC)
    {
      /* Check if there are Data to write in Usb Key */
      if(AUDIODataReady == 1)
      {
        /* write buffer in file */
        res = f_write(&WavFile, (uint8_t*)(WrBuffer+AUDIOBuffOffset), WR_BUFFER_SIZE, (void*)&byteswritten);
        if(res != FR_OK)
        {
          Error_Handler();
        }
        BufferCtl.fptr += byteswritten;
        AUDIODataReady = 0;
      }
      
      /* User button pressed */
      if(CmdIndex != CMD_RECORD)
      {
        /* Stop Audio Recording */
        WaveRecorderStop();
        /* Switch Command Index to Play */
        CmdIndex = CMD_PLAY;
        /* Toggoling LED6 to signal Play */
        LEDsState = LED6_TOGGLE;
        break;
      }
    }
    else /* End of recording time DEFAULT_TIME_REC */
    {
      /* Stop Audio Recording */
      WaveRecorderStop();
      /* Change Command Index to Stop */
      CmdIndex = CMD_STOP;
      /* Toggoling LED4 to signal Stop */
      LEDsState = LED4_TOGGLE;
      AUDIODataReady = 0;
      break;
    }
  }
  
  /* Update the data length in the header of the recorded Wave */    
  f_lseek(&WavFile, 0);
  
  /* Parse the wav file header and extract required information */
  WavProcess_HeaderUpdate(pHeaderBuff, &WaveFormat);
  f_write(&WavFile, pHeaderBuff, 44, (void*)&byteswritten);
  
  /* Close file and unmount MyFilesystem */
  f_close (&WavFile);
  f_mount(NULL, 0, 1);
  
  /* Change Command Index to Play */
  CmdIndex = CMD_PLAY;
}

/**
  * @brief Calculates the remaining file size and new position of the pointer.
  * @param None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
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
    AUDIODataReady = 1;
    AUDIOBuffOffset = WR_BUFFER_SIZE/2;
    ITCounter = 0;
  }
  else
  {
    ITCounter++;
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
    AUDIODataReady = 1;
    AUDIOBuffOffset = WR_BUFFER_SIZE/2;
    ITCounter = 0;
  }
  else
  {
    ITCounter++;
  }
}

/**
  * @brief  Encoder initialization.
  * @param  Freq: Sampling frequency.
  * @param  pHeader: Pointer to the WAV file header to be written.  
  * @retval 0 if success, !0 else.
  */
static uint32_t WavProcess_EncInit(uint32_t Freq, uint8_t* pHeader)
{
  /* Initialize the encoder structure */
  WaveFormat.SampleRate = Freq;        /* Audio sampling frequency */
  WaveFormat.NbrChannels = 2;          /* Number of channels: 1:Mono or 2:Stereo */
  WaveFormat.BitPerSample = 16;        /* Number of bits per sample (16, 24 or 32) */
  WaveFormat.FileSize = 0x001D4C00;    /* Total length of useful audio data (payload) */
  WaveFormat.SubChunk1Size = 44;       /* The file header chunk size */
  WaveFormat.ByteRate = (WaveFormat.SampleRate * \
                        (WaveFormat.BitPerSample/8) * \
                         WaveFormat.NbrChannels);            /* Number of bytes per second  (sample rate * block align)  */
  WaveFormat.BlockAlign = WaveFormat.NbrChannels * \
                         (WaveFormat.BitPerSample/8);      /* channels * bits/sample / 8 */
  
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
  /* write chunkID, must be 'RIFF'  ------------------------------------------*/
  pHeader[0] = 'R';
  pHeader[1] = 'I';
  pHeader[2] = 'F';
  pHeader[3] = 'F';
  
  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be be written back at the end of the 
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
  /* Write the file length ----------------------------------------------------*/
  /* The sampling time: this value will be be written back at the end of the 
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

/**
* @}
*/ 
