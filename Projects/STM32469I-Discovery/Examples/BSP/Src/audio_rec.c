/**
  ******************************************************************************
  * @file    BSP/Src/audio_rec.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the audio feature in the
  *          stm32469i_discovery driver
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

#include <string.h>

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private define ------------------------------------------------------------*/
#define AUDIO_IN_PCM_BUFFER_SIZE                   (4*2304) /* buffer size in half-word */
#define AUDIO_IN_PDM_BUFFER_SIZE                   INTERNAL_BUFF_SIZE
/* Audio file size and start address are defined here since the audio file is
   stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_START_OFFSET_ADDRESS    (0)            /* Offset relative to audio file header size */

/* Private typedef -----------------------------------------------------------*/
typedef enum {
  REC_NONE_STATE = 0, 
  REC_RESET_STATE,     
  RECORD_ONGOING,   
  REC_ERROR_STATE,
}AUDIOREC_StateTypeDef;

typedef enum {
  BUFFER_EMPTY = 0,
  BUFFER_FULL,
}WR_BUFFER_StateTypeDef;

typedef enum {
  TRANSFER_NONE = 0,
  TRANSFER_HALF,
  TRANSFER_FULL
} IT_StateTypeDef;

typedef struct {
  int32_t offset;
  uint32_t rec_length;
  uint32_t pcm_ptr;
  WR_BUFFER_StateTypeDef wr_state;
  IT_StateTypeDef it_state;
  uint16_t pdm_buff[AUDIO_IN_PDM_BUFFER_SIZE];
  uint16_t pcm_buff[AUDIO_IN_PCM_BUFFER_SIZE];
}Audio_BufferTypeDef;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
Audio_BufferTypeDef  buffer_ctl;
//static uint32_t uwVolume = 20;
static uint8_t AudioRec_State;
static uint32_t AudioRecFullBuff_addr;

static __IO uint16_t CurrentPosition;
#ifdef JOYSTICK
static JOYState_TypeDef JoyState = JOY_NONE;
#endif

/* Private function prototypes -----------------------------------------------*/
static void AudioRec_SetHint(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Audio Play demo
  * @param  None
  * @retval None
  */
void AudioRec_demo (void)
{
  uint16_t NbRecord_done = 0;
  uint8_t FreqStr[256] = {0};
  AudioRec_State = REC_NONE_STATE;
  buffer_ctl.rec_length = 0;
  /* Initialize AudioRecFullBuff at the address of the audio_play reader */

  AudioRecFullBuff_addr = AUDIO_REC_START_ADDR;

  AudioRec_SetHint();

#ifdef JOYSTICK

  /* Initialize Joystick */
  if (BSP_JOY_Init(JOY_MODE_GPIO) != IO_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"Joystick cannot be initialized", CENTER_MODE);
  }
#endif

  /* Initialize Audio Recorder */
  if (BSP_AUDIO_IN_Init(DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR) == AUDIO_OK)
  {
    AudioRec_State = REC_RESET_STATE;
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO RECORD INIT OK  ", CENTER_MODE);
  }
  else
  {
    AudioRec_State = REC_ERROR_STATE;
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO RECORD INIT FAIL", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"  Try to reset board ", CENTER_MODE);
  }
  
  /* Infinite loop */
  while (buffer_ctl.rec_length < AUDIO_REC_TOTAL_SIZE)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

    if (buffer_ctl.it_state == TRANSFER_HALF)
    {
      /* PDM to PCM data convert */
      BSP_AUDIO_IN_PDMToPCM((uint16_t*)&buffer_ctl.pdm_buff[0],
                            &buffer_ctl.pcm_buff[buffer_ctl.pcm_ptr]);

      buffer_ctl.pcm_ptr+= AUDIO_IN_PDM_BUFFER_SIZE/4/2;

      if(buffer_ctl.pcm_ptr == AUDIO_IN_PCM_BUFFER_SIZE/2)
      {
        buffer_ctl.wr_state   =  BUFFER_FULL;
        buffer_ctl.offset  = 0;
      }

      if(buffer_ctl.pcm_ptr >= AUDIO_IN_PCM_BUFFER_SIZE)
      {
        buffer_ctl.wr_state   =  BUFFER_FULL;
        buffer_ctl.offset  = AUDIO_IN_PCM_BUFFER_SIZE/2;
        buffer_ctl.pcm_ptr = 0;
      }
      buffer_ctl.it_state = TRANSFER_NONE;
    }

    if (buffer_ctl.it_state == TRANSFER_FULL)
    {
      /* PDM to PCM data convert */
      if (BSP_AUDIO_IN_PDMToPCM((uint16_t*)&buffer_ctl.pdm_buff[INTERNAL_BUFF_SIZE/2], &buffer_ctl.pcm_buff[buffer_ctl.pcm_ptr]) == 0)
      {
        buffer_ctl.pcm_ptr+= AUDIO_IN_PDM_BUFFER_SIZE/4/2;

        if(buffer_ctl.pcm_ptr == AUDIO_IN_PCM_BUFFER_SIZE/2)
        {
          buffer_ctl.wr_state =  BUFFER_FULL;
          buffer_ctl.offset = 0;
        }

        if(buffer_ctl.pcm_ptr >= AUDIO_IN_PCM_BUFFER_SIZE)
        {
          buffer_ctl.wr_state =  BUFFER_FULL;
          buffer_ctl.offset = AUDIO_IN_PCM_BUFFER_SIZE/2;
          buffer_ctl.pcm_ptr = 0;
         }
      }
      buffer_ctl.it_state = TRANSFER_NONE;
      NbRecord_done++;

    }

    /* PCM buffer is full, copy it into the record_file */
    if (buffer_ctl.wr_state == BUFFER_FULL)
    {
      memcpy((uint32_t *)AudioRecFullBuff_addr, (uint8_t*)(buffer_ctl.pcm_buff + buffer_ctl.offset), AUDIO_IN_PCM_BUFFER_SIZE);
      AudioRecFullBuff_addr += AUDIO_IN_PCM_BUFFER_SIZE;
      buffer_ctl.rec_length += AUDIO_IN_PCM_BUFFER_SIZE;
      buffer_ctl.wr_state =  BUFFER_EMPTY;
    }

#ifdef JOYSTICK
    /* Get the Joystick State */
    JoyState = BSP_JOY_GetState();
    switch (JoyState)
    {
      case JOY_SEL:
#endif
        if (AudioRec_State == REC_RESET_STATE)
        {
          /* Display the state on the screen */
          BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
          BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"       RECORDING...     ", CENTER_MODE);
          /* Start Recording */
          buffer_ctl.rec_length = 0;
          buffer_ctl.wr_state = BUFFER_EMPTY;
          buffer_ctl.it_state = TRANSFER_NONE;
          BSP_AUDIO_IN_Record((uint16_t*)&buffer_ctl.pdm_buff[0], INTERNAL_BUFF_SIZE);
          AudioRec_State = RECORD_ONGOING;
        }
#ifdef JOYSTICK
        break;
      default:
        break;
    }
#endif

    if (CheckForUserInput() > 0)
    {
      /* Stop recorder and exit the test (go to next test) */
      BSP_AUDIO_IN_Stop();
      return;
    }
  }

  /* Stop recorder once buffer_ctl.rec_length < AUDIO_REC_TOTAL_SIZE*/
  BSP_AUDIO_IN_Stop();

  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  sprintf((char*)FreqStr, "RECORDING DONE, %d frames, START PLAYBACK", NbRecord_done);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)FreqStr, CENTER_MODE);
  NbRecord_done = buffer_ctl.rec_length / (DEFAULT_AUDIO_IN_FREQ * DEFAULT_AUDIO_IN_CHANNEL_NBR * 2);
  sprintf((char*)FreqStr, "Elapsed_time: %d frames", NbRecord_done);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 45, (uint8_t *)FreqStr, CENTER_MODE);



  /* -----------Start Playback -------------- */

  /* Initialize audio IN at REC_FREQ*/
  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 95, DEFAULT_AUDIO_IN_FREQ/2) == 0)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO CODEC   OK  ", CENTER_MODE);
  }
  else
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO CODEC  FAIL ", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)" Try to reset board ", CENTER_MODE);
  }

  /* Play the recorded buffer*/
  if(AUDIO_Play_Start((uint32_t *)AUDIO_REC_START_ADDR, (uint32_t)AUDIO_REC_TOTAL_SIZE) == 0)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO PLAYBACK   OK  ", CENTER_MODE);
  } else
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO PLAYBACK  FAIL ", CENTER_MODE);
  }
  
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 25, (uint8_t *)"PLAYBACK DONE", CENTER_MODE);

  while (1)
  {
    /* Toggle LED4 */
    BSP_LED_Toggle(LED4);

    /* Insert 100 ms delay */
    HAL_Delay(100);
    if (CheckForUserInput() > 0)
    {
      /* Set LED4 */
      BSP_LED_On(LED4);
      /* Stop recorder */
      BSP_AUDIO_IN_Stop();
      /* Stop Player before close Test */
      BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
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
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 90);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO RECORD EXAMPLE", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press User button for next menu            ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"Click on Joystick to start recording       ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Make sure jumpers are in the right state   ", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 100, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 110);
  BSP_LCD_DrawRect(11, 101, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 112);

}

/*------------------------------------------------------------------------------
       Callbacks implementation:
           the callbacks API are defined __weak in the stm32469i_discovery_audio.c file
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

  buffer_ctl.it_state = TRANSFER_FULL;
  return;
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{

  buffer_ctl.it_state = TRANSFER_HALF;
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
  BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"TRANSFER  ERROR", CENTER_MODE);

  /* Stop the program with an infinite loop */
  while (BSP_PB_GetState(BUTTON_USER) != PB_RESET)
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
