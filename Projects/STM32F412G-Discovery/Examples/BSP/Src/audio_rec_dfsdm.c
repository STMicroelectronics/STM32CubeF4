/**
  ******************************************************************************
  * @file    BSP/Src/audio_rec_dfsdm.c
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
#include <stdio.h>
#include "string.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF = 1,
  BUFFER_OFFSET_FULL = 2,
}BUFFER_StateTypeDef;

extern AUDIO_ErrorTypeDef AUDIO_Start(uint32_t audio_start_address, uint32_t audio_file_size);

/* Private define ------------------------------------------------------------*/
#define AUDIO_BLOCK_SIZE   ((uint32_t)0xFFFE)
#define HEADBAND_HEIGHT    64
#define SCRATCH_BUFF_SIZE  512
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if defined ( __CC_ARM )  /* !< ARM Compiler */
  int32_t Scratch [SCRATCH_BUFF_SIZE] __attribute__((at(0x2000E000)));
#elif defined ( __ICCARM__ )  /* !< ICCARM Compiler */
#pragma location=0x2000E000
  int32_t Scratch [SCRATCH_BUFF_SIZE];
#elif defined ( __GNUC__ )  /* !< GNU Compiler */
  int32_t Scratch [SCRATCH_BUFF_SIZE] __attribute__((section(".scratch_section")));
#endif

extern uint16_t  internal_buffer[AUDIO_BLOCK_SIZE];
/* Global variables ---------------------------------------------------------*/
extern uint32_t  audio_rec_buffer_state;    /* defined in audio_rec_analog.c */

/* Private function prototypes -----------------------------------------------*/
static void AudioRec_SetHint(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Audio Play demo
  * @param  None
  * @retval None
  */
void AudioRecDfsdm_demo (void)
{
  AudioRec_SetHint();
  uint8_t  next_freq = 0;
  uint8_t  end_of_test = 0;
  JOYState_TypeDef JoyState = JOY_NONE;
  __IO uint32_t uwVolume = 70;
  uint32_t AudioFreq[8] = {I2S_AUDIOFREQ_8K ,I2S_AUDIOFREQ_11K, I2S_AUDIOFREQ_16K, I2S_AUDIOFREQ_22K, I2S_AUDIOFREQ_32K, I2S_AUDIOFREQ_44K, I2S_AUDIOFREQ_48K, I2S_AUDIOFREQ_96K};
  uint32_t *AudioFreq_ptr;
  uint8_t FreqStr[25] = {0};
  
  AudioFreq_ptr = AudioFreq + 6; /*I2S_AUDIOFREQ_48K*/
  BSP_JOY_Init(JOY_MODE_GPIO);
  
  while (end_of_test == 0)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect(0, HEADBAND_HEIGHT, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() - HEADBAND_HEIGHT);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 160, (uint8_t *)"TOP MP34 MICROPHONES TEST", CENTER_MODE);
    
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    
    sprintf((char*)FreqStr,"      FREQ: %lu     ", *AudioFreq_ptr);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 145, (uint8_t *)FreqStr, CENTER_MODE);
    
    /* Allocate channel buffer scratch */
    BSP_AUDIO_IN_AllocScratch (Scratch, SCRATCH_BUFF_SIZE);
    
    /* Initialize Audio Recorder */
    if (BSP_AUDIO_IN_Init(*AudioFreq_ptr, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) != AUDIO_OK)
    {
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 130, (uint8_t *)"Audio record init FAIL", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 115, (uint8_t *)"Try to reset board ", CENTER_MODE);
      BSP_AUDIO_IN_DeInit();
    }
    else
    {
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 130, (uint8_t *)"Audio record init OK", CENTER_MODE);
      
      audio_rec_buffer_state = BUFFER_OFFSET_NONE;
      
      /* Display the state on the screen */
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 115, (uint8_t *)"Buffering...", CENTER_MODE);
      
      /* Start Recording */
      BSP_AUDIO_IN_Record(internal_buffer, AUDIO_BLOCK_SIZE);
      
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 85, (uint8_t *)"Start Playback...", CENTER_MODE);
      
      /* -----------Start Playback -------------- */
      /* Initialize audio IN at REC_FREQ*/
      BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, uwVolume, *AudioFreq_ptr);
      
      /* Play the recorded buffer*/
      BSP_AUDIO_OUT_Play((uint16_t*)(&internal_buffer[0]), AUDIO_BLOCK_SIZE * 2);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 70, (uint8_t *)"Streaming to headphones", CENTER_MODE);
    }
    
    next_freq = 0;
    while (next_freq == 0)
    {
      /* Insert 100 ms delay */
      HAL_Delay(100);
      
      /* Get the Joystick State */
      JoyState = BSP_JOY_GetState();
      
      switch(JoyState)
      {
      case JOY_UP:
        /* Increase volume by 5% */
        if (uwVolume < 95)
          uwVolume += 5;
        else
          uwVolume = 100;
        BSP_AUDIO_OUT_SetVolume(uwVolume);
        break;
        
      case JOY_DOWN:
        /* Decrease volume by 5% */
        if (uwVolume > 5)
          uwVolume -= 5;
        else
          uwVolume = 0;
        BSP_AUDIO_OUT_SetVolume(uwVolume);
        break;
        
      case JOY_LEFT:
        /*Decrease Frequency */
        if (*AudioFreq_ptr != 8000)
        {
          AudioFreq_ptr--;
          next_freq = 1;
        }
        break;
        
      case JOY_RIGHT:
        /* Increase Frequency */
        if (*AudioFreq_ptr != 96000)
        {
          AudioFreq_ptr++;
          next_freq = 1;
        }
        break;
        
      default:
        break;
      }
      
      if (CheckForUserInput() > 0)
      {
        end_of_test = 1; 
        /* Stop Player before close Test */
        BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW);
        BSP_AUDIO_OUT_DeInit();
        BSP_AUDIO_IN_Stop();
        BSP_AUDIO_IN_DeInit(); 
        return;
      }
    }
    /* Stop Player before close Test */
    BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW);
    BSP_AUDIO_OUT_DeInit();
    BSP_AUDIO_IN_Stop();
    BSP_AUDIO_IN_DeInit();
  }
}

/**
  * @brief  Display Audio Record demo hint
  * @param  None
  * @retval None
  */
static void AudioRec_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set Audio Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"AUDIO REC DFSDM", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"Record and play audio", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"stream from microphones", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"Press BUTTON for next", CENTER_MODE);
}

/**
  * @}
  */

/**
  * @}
  */
