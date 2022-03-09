/**
  ******************************************************************************
  * @file    BSP/Src/audio.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the audio feature in the 
  *          stm32412g_discovery driver
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
#include "stm32412g_discovery_audio.h"
#include <stdio.h>


/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 



/* Private define ------------------------------------------------------------*/

/*Since SysTick is set to 1ms (unless to set it quicker) */ 
/* to run up to 48khz, a buffer around 1000 (or more) is requested*/
/* to run up to 96khz, a buffer around 2000 (or more) is requested*/
#define AUDIO_BUFFER_SIZE       2048
#define AUDIO_DEFAULT_VOLUME    70

/* Audio file size and start address are defined here since the audio file is
   stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_FILE_SIZE               547732
#define AUDIO_START_OFFSET_ADDRESS    0            /* Offset relative to audio file header size */
#define AUDIO_FILE_ADDRESS            FLASH_DATA_ADDRESS   /* Audio file address */

#define HEADBAND_HEIGHT         64

/* Private typedef -----------------------------------------------------------*/
typedef enum {
  AUDIO_STATE_IDLE = 0,
  AUDIO_STATE_INIT,    
  AUDIO_STATE_PLAYING,  
}AUDIO_PLAYBACK_StateTypeDef;

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

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static AUDIO_BufferTypeDef  buffer_ctl;
static AUDIO_PLAYBACK_StateTypeDef  audio_state;
static uint32_t  AudioStartAddress;
static uint32_t  AudioFileSize;
__IO uint32_t uwVolume = 20;
__IO uint32_t uwPauseEnabledStatus = 0;

static uint32_t AudioFreq[8] = {8000 ,11025, 16000, 22050, 32000, 44100, 48000, 96000};
static JOYState_TypeDef JoyState = JOY_NONE;

/* Private function prototypes -----------------------------------------------*/
static void Audio_SetHint(void);
static uint32_t GetData(void *pdata, uint32_t offset, uint8_t *pbuf, uint32_t NbrOfData);
AUDIO_ErrorTypeDef AUDIO_Start(uint32_t audio_start_address, uint32_t audio_file_size);
AUDIO_ErrorTypeDef AUDIO_Stop(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Audio Play demo
  * @retval None
  */
void AudioPlay_demo (void)
{ 
  uint32_t *AudioFreq_ptr;
  uint8_t status = 0;
  uint8_t FreqStr[25] = {0};
  
  AudioFreq_ptr = AudioFreq + 6; /*AF_48K*/
  uwPauseEnabledStatus = 1; /* 0 when audio is running, 1 when Pause is on */
  uwVolume = 50;
  
  Audio_SetHint();
  
  status = BSP_JOY_Init(JOY_MODE_GPIO);
  
  if (status != HAL_OK)
  {    
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 100, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 85, (uint8_t *)"Joystick init error", CENTER_MODE);
  }
  
  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, uwVolume, *AudioFreq_ptr) == 0)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 100, (uint8_t *)"  AUDIO CODEC   OK  ", CENTER_MODE);
  }
  else
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 100, (uint8_t *)"  AUDIO CODEC  FAIL ", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 85, (uint8_t *)" Try to reset board ", CENTER_MODE);
  }
  
  /* 
  Start playing the file from a circular buffer, once the DMA is enabled, it is 
  always in running state. Application has to fill the buffer with the audio data 
  using Transfer complete and/or half transfer complete interrupts callbacks 
  (DISCOVERY_AUDIO_TransferComplete_CallBack() or DISCOVERY_AUDIO_HalfTransfer_CallBack()...
  */
  AUDIO_Start(AUDIO_FILE_ADDRESS, AUDIO_FILE_SIZE);
  
  /* Display the state on the screen */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 85, (uint8_t *)"       PLAYING...     ", CENTER_MODE);
  
  sprintf((char*)FreqStr,"       VOL:    %lu     ",uwVolume);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 55, (uint8_t *)FreqStr, CENTER_MODE);
  
  sprintf((char*)FreqStr,"      FREQ: %lu     ",*AudioFreq_ptr);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 40, (uint8_t *)FreqStr, CENTER_MODE);
  
  /* Infinite loop */
  while(1)
  {
    /* IMPORTANT: AUDIO_Process() should be called within a periodic process */    
    AUDIO_Process();
    
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    /* Get the Joystick State */
    JoyState = BSP_JOY_GetState();
    
    switch(JoyState)
    {
    case JOY_UP:
      HAL_Delay(100);
      /* Increase volume by 5% */
      if (uwVolume < 95)
        uwVolume += 5;
      else
        uwVolume = 100;
      sprintf((char*)FreqStr,"       VOL:    %lu     ",uwVolume);
      BSP_AUDIO_OUT_SetVolume(uwVolume);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 55, (uint8_t *)FreqStr, CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"                      ", CENTER_MODE);
      break;
      
    case JOY_DOWN:
      HAL_Delay(100);
      /* Decrease volume by 5% */
      if (uwVolume > 5)
        uwVolume -= 5;
      else
        uwVolume = 0;
      sprintf((char*)FreqStr,"       VOL:    %lu     ",uwVolume);
      BSP_AUDIO_OUT_SetVolume(uwVolume);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 55, (uint8_t *)FreqStr, CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"                      ", CENTER_MODE);
      break;
      
    case JOY_LEFT:
      HAL_Delay(100);
      /*Decrease Frequency */
      if (*AudioFreq_ptr != 8000)
      {
        AudioFreq_ptr--;
        BSP_AUDIO_OUT_SetFrequency(*AudioFreq_ptr);
      }
      sprintf((char*)FreqStr,"      FREQ: %lu     ", *AudioFreq_ptr);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 40, (uint8_t *)FreqStr, CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"                      ", CENTER_MODE);
      break;
      
    case JOY_RIGHT:
      HAL_Delay(100);
      /* Increase Frequency */
      if (*AudioFreq_ptr != 96000)
      {
        AudioFreq_ptr++;
        BSP_AUDIO_OUT_SetFrequency(*AudioFreq_ptr);
      }
      sprintf((char*)FreqStr,"      FREQ: %lu     ",*AudioFreq_ptr);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 40, (uint8_t *)FreqStr, CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"                      ", CENTER_MODE);
      break;
      
    case JOY_SEL:
      /* Set Pause / Resume or Exit */
      HAL_Delay(200);
      if (BSP_JOY_GetState() == JOY_SEL)  /* Long press on joystick selection button : Pause/Resume */
      {
        if (uwPauseEnabledStatus == 1)
        { /* Pause is enabled, call Resume */
          BSP_AUDIO_OUT_Resume();
          uwPauseEnabledStatus = 0;
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 85, (uint8_t *)"       PLAYING...     ", CENTER_MODE);
        } else
        { /* Pause the playback */
          BSP_AUDIO_OUT_Pause();
          uwPauseEnabledStatus = 1;
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 85, (uint8_t *)"       PAUSE  ...     ", CENTER_MODE);
        }
        BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"                      ", CENTER_MODE);
        HAL_Delay(200);
      }
      else  /* Short press on joystick selection button : exit */
      {
        BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
        BSP_AUDIO_OUT_DeInit();
        return;
      }
      break;
      
    default:
      break;
    }
  }
}

/**
  * @brief  Display Audio demo hint
  * @param  None
  * @retval None
  */
static void Audio_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Audio Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO PLAY", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"JOY_CLICK = exit / pause", LEFT_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"JOY U/D   = change Volume", LEFT_MODE);
  BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"JOY L/R   = change Freq", LEFT_MODE);
}


/**
  * @brief  Starts Audio streaming.    
  * @param  audio_start_address : buffer start address
  * @param  audio_file_size : buffer size in bytes
  * @retval Audio error
  */ 
AUDIO_ErrorTypeDef AUDIO_Start(uint32_t audio_start_address, uint32_t audio_file_size)
{
  uint32_t bytesread;
  
  buffer_ctl.state = BUFFER_OFFSET_NONE;
  AudioStartAddress = audio_start_address;
  AudioFileSize = audio_file_size;
  bytesread = GetData( (void *)AudioStartAddress,
                      0,
                      &buffer_ctl.buff[0],
                      AUDIO_BUFFER_SIZE);
  if(bytesread > 0)
  {
    BSP_AUDIO_OUT_Play((uint16_t*)&buffer_ctl.buff[0], AUDIO_BUFFER_SIZE);
    audio_state = AUDIO_STATE_PLAYING;      
    buffer_ctl.fptr = bytesread;
    return AUDIO_ERROR_NONE;
  }
  return AUDIO_ERROR_IO;
}

/**
  * @brief  Manages Audio process. 
  * @param  None
  * @retval Audio error
  */
uint8_t AUDIO_Process(void)
{
  uint32_t bytesread;
  AUDIO_ErrorTypeDef error_state = AUDIO_ERROR_NONE;  
  
  switch(audio_state)
  {
  case AUDIO_STATE_PLAYING:
    
    if(buffer_ctl.fptr >= AudioFileSize)
    {
      /* Play audio sample again ... */
      buffer_ctl.fptr = 0; 
      error_state = AUDIO_ERROR_EOF;
    }

    /* 1st half buffer played; so fill it and continue playing from bottom*/
    if(buffer_ctl.state == BUFFER_OFFSET_HALF)
    {
      bytesread = GetData((void *)AudioStartAddress,
                          buffer_ctl.fptr,
                          &buffer_ctl.buff[0],
                          AUDIO_BUFFER_SIZE /2);
      
      if( bytesread >0)
      { 
        buffer_ctl.state = BUFFER_OFFSET_NONE;
        buffer_ctl.fptr += bytesread; 
      }
    }
    
    /* 2nd half buffer played; so fill it and continue playing from top */    
    if(buffer_ctl.state == BUFFER_OFFSET_FULL)
    {
      bytesread = GetData((void *)AudioStartAddress,
                          buffer_ctl.fptr, 
                          &buffer_ctl.buff[AUDIO_BUFFER_SIZE /2],
                          AUDIO_BUFFER_SIZE /2);
      if( bytesread > 0)
      {
        buffer_ctl.state = BUFFER_OFFSET_NONE;
        buffer_ctl.fptr += bytesread;
      }
    }
    break;
    
  default:
    error_state = AUDIO_ERROR_NOTREADY;
    break;
  }
  
  return (uint8_t) error_state;
}

/**
  * @brief  Gets Data from storage unit.
  * @param  None
  * @retval None
  */
static uint32_t GetData(void *pdata, uint32_t offset, uint8_t *pbuf, uint32_t NbrOfData)
{
  uint8_t *lptr = pdata;
  uint32_t ReadDataNbr;
  
  ReadDataNbr = 0;
  while(((offset + ReadDataNbr) < AudioFileSize) && (ReadDataNbr < NbrOfData))
  {
    pbuf[ReadDataNbr]= lptr [offset + ReadDataNbr];
    ReadDataNbr++;
  }
  return ReadDataNbr;
}

/*------------------------------------------------------------------------------
       Callbacks implementation:
           the callbacks API are defined __weak in the stm32412g_discovery_audio.c file
           and their implementation should be done the user code if they are needed.
           Below some examples of callback implementations.
  ----------------------------------------------------------------------------*/
/**
  * @brief  Manages the full Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
  if(audio_state == AUDIO_STATE_PLAYING)
  {
    /* allows AUDIO_Process() to refill 2nd part of the buffer  */
    buffer_ctl.state = BUFFER_OFFSET_FULL;
  }
}

/**
  * @brief  Manages the DMA Half Transfer complete event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
  if(audio_state == AUDIO_STATE_PLAYING)
  {
    /* allows AUDIO_Process() to refill 1st part of the buffer  */
    buffer_ctl.state = BUFFER_OFFSET_HALF;
  }
}

/**
  * @brief  Manages the DMA FIFO error event.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_OUT_Error_CallBack(void)
{
  /* Display message on the LCD screen */
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"       DMA  ERROR     ", CENTER_MODE);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

  /* Stop the program with an infinite loop */
  while (BSP_PB_GetState(BUTTON_WAKEUP) != RESET)
  { return;}

  /* could also generate a system reset to recover from the error */
  /* .... */
}


  
/**
  * @}
  */ 

/**
  * @}
  */ 
