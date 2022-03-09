/**
  ******************************************************************************
  * @file    BSP/Src/audio.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the audio headphone microphone feature
  *          in the stm32412g_discovery driver
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

#define AUDIO_BLOCK_SIZE   ((uint32_t)0xFFFE)

#define HEADBAND_HEIGHT    64

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t  internal_buffer[AUDIO_BLOCK_SIZE];

/* Global variables ---------------------------------------------------------*/
__IO uint32_t  audio_rec_buffer_state;

/* Private function prototypes -----------------------------------------------*/
static void AudioRec_SetHint(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Audio Play demo
  * @param  None
  * @retval None
  */
void AudioRecAnalog_demo (void)
{
  AudioRec_SetHint();

  /* Initialize Audio Recorder */
  if (BSP_AUDIO_IN_InitEx(INPUT_DEVICE_ANALOG_MIC, DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) == AUDIO_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO RECORD INIT OK  ", CENTER_MODE);
  }
  else
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO RECORD INIT FAIL", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)" Try to reset board ", CENTER_MODE);
  }

  audio_rec_buffer_state = BUFFER_OFFSET_NONE;

  /* Display the state on the screen */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"       RECORDING...     ", CENTER_MODE);

  /* Start Recording */
  BSP_AUDIO_IN_Record(internal_buffer, AUDIO_BLOCK_SIZE);

  /* Wait end of one block recording */
  while((audio_rec_buffer_state & BUFFER_OFFSET_FULL) != BUFFER_OFFSET_FULL);

  /* Stop recorder */
  BSP_AUDIO_IN_Stop();

  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"RECORDING DONE", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 50, (uint8_t *)"START PLAYBACK...", CENTER_MODE);

  /* -----------Start Playback -------------- */
  /* Initialize audio IN at REC_FREQ*/ 
  BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 90, DEFAULT_AUDIO_IN_FREQ);
  
  /* Play the recorded buffer*/
  AUDIO_Start((uint32_t)(&internal_buffer[0]), AUDIO_BLOCK_SIZE * 2);  /* Use Audio play demo to playback sound */
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 35, (uint8_t *)"PLAYBACK DONE", CENTER_MODE);
  
  while (1)
  {
    /* IMPORTANT: AUDIO_Process() should be called within a periodic process */    
    AUDIO_Process();
    
    if (CheckForUserInput() > 0)
    {
      /* Stop Player before close Test */
      BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
      BSP_AUDIO_OUT_DeInit();
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
  BSP_LCD_DisplayStringAt(0, 1, (uint8_t *)"AUDIO REC ANALOG", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 20, (uint8_t *)"Record audio from analog", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t *)"microphone and play", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 50, (uint8_t *)"Press BUTTON for next", CENTER_MODE);
}

/*------------------------------------------------------------------------------
       Callbacks implementation:
           the callbacks API are defined __weak in the stm32746g_discovery_audio.c file
           and their implementation should be done the user code if they are needed.
           Below some examples of callback implementations.
  ----------------------------------------------------------------------------*/
/**
  * @brief Manages the DMA Transfer complete interrupt.
  * @param None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  audio_rec_buffer_state |= BUFFER_OFFSET_FULL;
  return;
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
