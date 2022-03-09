/**
  ******************************************************************************
  * @file    BSP/Src/audio.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the audio feature in the 
  *          stm32469i_eval driver
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
#include "stm32446e_eval_audio.h"
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
#define AUDIO_FILE_SIZE               0x3E000
#define AUDIO_START_OFFSET_ADDRESS    0            /* Offset relative to audio file header size */
#define AUDIO_FILE_ADDRESS            FLASH_DATA_ADDRESS   /* Audio file address */

/* Private typedef -----------------------------------------------------------*/
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

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static AUDIO_BufferTypeDef  buffer_ctl;
static AUDIO_PLAYBACK_StateTypeDef  audio_state;
__IO uint32_t uwVolume = 20;
__IO uint32_t uwPauseEnabledStatus = 0;

static uint32_t AudioFreq[9] = {8000 ,11025, 16000, 22050, 32000, 44100, 48000, 96000, 192000};
static JOYState_TypeDef JoyState = JOY_NONE;

/* Private function prototypes -----------------------------------------------*/
static void Audio_SetHint(void);
static uint32_t GetData(void *pdata, uint32_t offset, uint8_t *pbuf, uint32_t NbrOfData);
AUDIO_ErrorTypeDef AUDIO_Start(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Audio Play demo
  * @param  None
  * @retval None
  */
void AudioPlay_demo (void)
{ 
  uint32_t *AudioFreq_ptr;
  uint8_t status = 0;
  uint8_t FreqStr[256] = {0};
 
  AudioFreq_ptr = AudioFreq+6; /*AF_48K*/
  uwPauseEnabledStatus = 1; /* 0 when audio is running, 1 when Pause is on */
  uwVolume = 50;

  Audio_SetHint();
    
  status = BSP_JOY_Init(JOY_MODE_GPIO);

  if (status != IO_OK)
  {    
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 100, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 85, (uint8_t *)"Joystick cannot be initialized", CENTER_MODE);
  }

  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_BOTH, uwVolume, *AudioFreq_ptr) == 0)
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
  (EVAL_AUDIO_TransferComplete_CallBack() or EVAL_AUDIO_HalfTransfer_CallBack()...
  */
  AUDIO_Start();

  /* Display the state on the screen */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 85, (uint8_t *)"       PLAYING...     ", CENTER_MODE);

  sprintf((char*)FreqStr,"       VOL:    %lu     ",uwVolume);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 55, (uint8_t *)FreqStr, CENTER_MODE);

  sprintf((char*)FreqStr,"      FREQ: %lu     ",*AudioFreq_ptr);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 40, (uint8_t *)FreqStr, CENTER_MODE);

  /* IMPORTANT:
     AUDIO_Process() is called by the SysTick Handler, as it should be called 
     within a periodic process */

  /* Infinite loop */
  while(1)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    /* Get the Joystick State */
    JoyState = BSP_JOY_GetState();

    switch(JoyState)
    {
      case JOY_UP:
        /* Increase volume by 5% */
        if (uwVolume < 95)
        { 
          uwVolume += 5;
        }
        else
        {
          uwVolume = 100;
        }
        sprintf((char*)FreqStr,"       VOL:    %lu     ",uwVolume);
        BSP_AUDIO_OUT_SetVolume(uwVolume);
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 55, (uint8_t *)FreqStr, CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"                      ", CENTER_MODE);

        break;

      case JOY_DOWN:
        /* Decrease volume by 5% */
        if (uwVolume > 5)
        {
          uwVolume -= 5;
        }
        else
        {
          uwVolume = 0;
        }
        sprintf((char*)FreqStr,"       VOL:    %lu     ",uwVolume);
        BSP_AUDIO_OUT_SetVolume(uwVolume);
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 55, (uint8_t *)FreqStr, CENTER_MODE);
        BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"                      ", CENTER_MODE);

        break;

      case JOY_LEFT:
        /*Decrease Frequency */
          if (*AudioFreq_ptr != 8000)
          {
            AudioFreq_ptr--;
            sprintf((char*)FreqStr,"      FREQ: %lu     ", *AudioFreq_ptr);
            BSP_AUDIO_OUT_Pause();
            BSP_AUDIO_OUT_SetFrequency(*AudioFreq_ptr);
            BSP_AUDIO_OUT_Resume();
          }
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 40, (uint8_t *)FreqStr, CENTER_MODE);
          BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"                      ", CENTER_MODE);
        break;

      case JOY_RIGHT:
        /* Increase Frequency */
          if (*AudioFreq_ptr != 96000)
          {
            AudioFreq_ptr++;
            sprintf((char*)FreqStr,"      FREQ: %lu     ",*AudioFreq_ptr);
            BSP_AUDIO_OUT_Pause();
            BSP_AUDIO_OUT_SetFrequency(*AudioFreq_ptr);
            BSP_AUDIO_OUT_Resume();
          }
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 40, (uint8_t *)FreqStr, CENTER_MODE);
          BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"                      ", CENTER_MODE);
        break;

      case JOY_SEL:
        /* Set Pause / Resume */
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
        break;
        
      default:
        break;
    }
    
    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);

    /* Insert 100 ms delay */
    HAL_Delay(100);

    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);

    /* Insert 100 ms delay */
    HAL_Delay(100);
    if(CheckForUserInput() > 0)
    {
      BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
      BSP_AUDIO_OUT_DeInit();
      return;
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
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 90);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO EXAMPLE", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press User button for next menu            ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"Use Joystick Up/Down    to change Volume   ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Use Joystick Left/Right to change Frequency", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 75, (uint8_t *)"Click on Joystick for Pause/Resume         ", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 100, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 110);
  BSP_LCD_DrawRect(11, 101, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 112);

}


/**
  * @brief  Starts Audio streaming.    
  * @param  None
  * @retval Audio error
  */ 
AUDIO_ErrorTypeDef AUDIO_Start(void)
{
  uint32_t bytesread;
  
  buffer_ctl.state = BUFFER_OFFSET_NONE;
  bytesread = GetData( (void *)AUDIO_FILE_ADDRESS,
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
    
    if(buffer_ctl.fptr >= AUDIO_FILE_SIZE)
    {
      /* Play audio sample again ... */
      buffer_ctl.fptr = 0; 
      error_state = AUDIO_ERROR_EOF;
    }

    /* 1st half buffer played; so fill it and continue playing from bottom*/
    if(buffer_ctl.state == BUFFER_OFFSET_HALF)
    {
      bytesread = GetData((void *)AUDIO_FILE_ADDRESS,
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
      bytesread = GetData((void *)AUDIO_FILE_ADDRESS,
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
  while(((offset + ReadDataNbr) < AUDIO_FILE_SIZE) && (ReadDataNbr < NbrOfData))
  {
    pbuf[ReadDataNbr]= lptr [offset + ReadDataNbr];
    ReadDataNbr++;
  }
  return ReadDataNbr;
}

/*------------------------------------------------------------------------------
       Callbacks implementation:
           the callbacks API are defined __weak in the stm32469i_eval_audio.c file
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
  while (BSP_PB_GetState(BUTTON_KEY) != RESET)
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
