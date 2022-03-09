/**
  ******************************************************************************
  * @file    BSP/Src/audio_rec_dfsdm.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the audio feature in the
  *          stm32f413h_discovery driver
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

#define AUDIO_BLOCK_SIZE   ((uint32_t)0xFFFE)
#define AUDIO_NB_BLOCKS    ((uint32_t)4)
#define HEADBAND_HEIGHT    64

/* Private define ------------------------------------------------------------*/
#define SCRATCH_BUFF_SIZE  512
/* Private macro -------------------------------------------------------------*/
uint16_t  internal_buffer[AUDIO_BLOCK_SIZE];

/* Global variables ---------------------------------------------------------*/
__IO uint32_t  audio_rec_buffer_state;

#if defined ( __CC_ARM )  /* !< ARM Compiler */
  int32_t Scratch [SCRATCH_BUFF_SIZE] __attribute__((at(0x2000E000)));
#elif defined ( __ICCARM__ )  /* !< ICCARM Compiler */
#pragma location=0x2000E000
int32_t Scratch [SCRATCH_BUFF_SIZE];
#elif defined ( __GNUC__ )  /* !< GNU Compiler */
  int32_t Scratch [SCRATCH_BUFF_SIZE] __attribute__((section(".scratch_section")));
#endif

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
  
  __IO uint32_t uwVolume = 70;
  uint32_t AudioFreq[1] = {I2S_AUDIOFREQ_44K};
  uint32_t *AudioFreq_ptr;
  uint8_t FreqStr[25] = {0};
  
  AudioFreq_ptr = AudioFreq; 
  
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(0, HEADBAND_HEIGHT, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() - HEADBAND_HEIGHT);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  
  
  sprintf((char*)FreqStr,"      FREQ: %lu     ", *AudioFreq_ptr);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 145, (uint8_t *)FreqStr, CENTER_MODE);
  
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
    /* Initialize audio OUT at REC_FREQ*/
    BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, uwVolume, *AudioFreq_ptr);
    
    /* Play the recorded buffer*/
    BSP_AUDIO_OUT_Play(&internal_buffer[0], AUDIO_BLOCK_SIZE * 2);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 70, (uint8_t *)"Streaming to headphones", CENTER_MODE);
  }
  while (1)
  {
    if (CheckForUserInput() > 0)
    {
      /* Stop Player before close Test */
      BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW);
      BSP_AUDIO_OUT_DeInit();
      BSP_AUDIO_IN_Stop();
      BSP_AUDIO_IN_DeInit(); 
      return;
    }
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
  * @brief Manages the DMA Transfer complete interrupt.
  * @param None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  audio_rec_buffer_state |= BUFFER_OFFSET_FULL;
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  audio_rec_buffer_state = BUFFER_OFFSET_HALF;
}

/**
  * @brief  Audio IN Error callback function.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_Error_Callback(void)
{
  /* This function is called when an Interrupt due to transfer error on or peripheral
     error occurs. */
  /* Display message on the LCD screen */
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"       DMA  ERROR     ", CENTER_MODE);

  /* Stop the program with an infinite loop */
  while (BSP_PB_GetState(BUTTON_WAKEUP) != RESET)
  {
    return;
  }
  /* could also generate a system reset to recover from the error */
  /* .... */
}
/**
  * @}
  */

/**
  * @}
  */
