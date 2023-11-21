/**
  ******************************************************************************
  * @file    I2S/I2S_Audio/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
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
#include "stlogo.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup I2S_Audio
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint32_t   ChunkID;       /* 0 */ 
  uint32_t   FileSize;      /* 4 */
  uint32_t   FileFormat;    /* 8 */
  uint32_t   SubChunk1ID;   /* 12 */
  uint32_t   SubChunk1Size; /* 16*/  
  uint16_t   AudioFormat;   /* 20 */ 
  uint16_t   NbrChannels;   /* 22 */   
  uint32_t   SampleRate;    /* 24 */
  
  uint32_t   ByteRate;      /* 28 */
  uint16_t   BlockAlign;    /* 32 */  
  uint16_t   BitPerSample;  /* 34 */  
  uint32_t   SubChunk2ID;   /* 36 */   
  uint32_t   SubChunk2Size; /* 40 */    

}WAVE_FormatTypeDef;

typedef enum 
{
  AUDIO_STATE_IDLE = 0,
  AUDIO_STATE_INIT,
  AUDIO_STATE_PLAYING,
}AUDIO_PLAYBACK_StateTypeDef;

/* Private define ------------------------------------------------------------*/
/* Audio file size and start offset address are defined here since the audio wave file is 
   stored in Flash memory as a constant table of 16-bit data 
 */
#define AUDIO_FILE_SIZE               147500       /* Size of audio file */
#define AUDIO_START_OFFSET_ADDRESS    44           /* Offset relative to audio file header size */
#define AUDIO_FILE_ADDRESS            0x08080000   /* Audio file address */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t uwCommand = AUDIO_PAUSE;
__IO uint32_t uwVolume = 70;
  uint8_t Volume_string[20] = {0};

uint32_t AudioTotalSize = 0xFFFF;  /* This variable holds the total size of the audio file */
uint32_t AudioRemSize   = 0xFFFF;  /* This variable holds the remaining data in audio file */
uint16_t* CurrentPos;              /* This variable holds the current position address of audio data */
static AUDIO_PLAYBACK_StateTypeDef  audio_state;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Display_ExampleDescription(void);
static void AudioPlay_SetHint(void);
static void AudioPlay_DisplayInfos(WAVE_FormatTypeDef * format);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  WAVE_FormatTypeDef *waveformat =  NULL;

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
   */
  HAL_Init();

  /* Configure the system clock to 100 MHz */
  SystemClock_Config();

  /* Initialize the LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /*##-1- Initialize the Keys Push buttons and LCD #####################*/

  /* Initialize all joystick's buttons in GPIO mode */
  /* Used buttons are as follows
     + Joystick Sel push-button : Pause / Resume
     + Joystick UP push-button : Volume High
     + Joystick DOWN push-button : Volume Low
   */
  BSP_JOY_Init(JOY_MODE_GPIO);

  /* Initialize the LCD */
  BSP_LCD_Init();

  /*##-2- Display welcome messages on LCD ####################################*/  
  Display_ExampleDescription();

  /* Wait for Joystick Sel push-button press before starting the Example */
  while (BSP_JOY_GetState() != JOY_SEL)
  {
  }
  
  /* Wait for Joystick Sel push-button release before starting the Example */
  while (BSP_JOY_GetState() == JOY_SEL)
  {
  }

  /*##-3- Display Example Template ###########################################*/  
  AudioPlay_SetHint();

  /*##-4- Turn on LEDs available on STM32412G-DISCOVERY Eval board ###################*/  
  BSP_LED_On(LED1);
  BSP_LED_On(LED2);
  BSP_LED_On(LED3);
  BSP_LED_On(LED4);
  
  /* Set audio initialization state */ 
  audio_state = AUDIO_STATE_INIT;

  /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOs...) */
  if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, uwVolume, I2S_AUDIOFREQ_8K) != AUDIO_OK)
  {
    /* Initialization Error */
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(20, 116, (uint8_t*)"Initialization problem", CENTER_MODE);
    Error_Handler();
  }
  else
  {
    BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(20, 116, (uint8_t *)"Audio Codec Ready", LEFT_MODE);
  }

  /* Set audio playing state */
  audio_state = AUDIO_STATE_PLAYING;

  /*##-5- Display information related to control and Playback state #*/
  /* Retrieve Wave Sample rate */
  waveformat = (WAVE_FormatTypeDef*)AUDIO_FILE_ADDRESS;
  AudioPlay_DisplayInfos(waveformat);
  
  /*##-6- Start AUDIO playback #####################################*/
  /*
  Normal mode description:
      Start playing the audio file (using DMA).
      Using this mode, the application can run other tasks in parallel since
      the DMA is handling the Audio Transfer instead of the CPU.
      The only task remaining for the CPU will be the management of the DMA
      Transfer Complete interrupt or the Half Transfer Complete interrupt in
      order to load again the buffer and to calculate the remaining data.
  Circular mode description:
     Start playing the file from a circular buffer, once the DMA is enabled it
     always run. User has to fill periodically the buffer with the audio data
     using Transfer complete and/or half transfer complete interrupts callbacks
     (BSP_AUDIO_OUT_TransferComplete_CallBack() or BSP_AUDIO_OUT_HalfTransfer_CallBack()...
     In this case the audio data file is smaller than the DMA max buffer
     size 65535 so there is no need to load buffer continuously or manage the
     transfer complete or Half transfer interrupts callbacks.
   */
   
  /* Set the total number of data to be played (count in half-word) */
  AudioTotalSize = (AUDIO_FILE_SIZE - AUDIO_START_OFFSET_ADDRESS)/(waveformat->NbrChannels);
  /* Set the current audio pointer position */
  CurrentPos = (uint16_t*)(AUDIO_FILE_ADDRESS + AUDIO_START_OFFSET_ADDRESS);
  /* Start the audio player */
  BSP_AUDIO_OUT_Play((uint16_t*)CurrentPos, (uint32_t)(AUDIO_FILE_SIZE - AUDIO_START_OFFSET_ADDRESS));
  /* Update the remaining number of data to be played */
  AudioRemSize = AudioTotalSize - DMA_MAX(AudioTotalSize);
  /* Update the current audio pointer position */
  CurrentPos += DMA_MAX(AudioTotalSize);
  /* Display the state on the screen */
  BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
  BSP_LCD_DisplayStringAt(0, 215, (uint8_t *)"Playback on-going", CENTER_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  /* Infinite loop */
  while(1)
  {
    /* Check on the Pause/Resume button */
    if(BSP_JOY_GetState() == JOY_SEL)
    {
      /* Wait to avoid rebound */
      while(BSP_JOY_GetState() == JOY_SEL);
      if(uwCommand == AUDIO_PAUSE)
      {
        BSP_AUDIO_OUT_Pause();
        /* Display the current state of the player */
        BSP_LCD_SetTextColor(LCD_COLOR_BROWN);
        BSP_LCD_DisplayStringAt(0, 215, (uint8_t *)"Playback paused  ", CENTER_MODE);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

        /* Next time Resume command should be processed */
        uwCommand = AUDIO_RESUME;
      }
      else
      {
        BSP_AUDIO_OUT_Resume();
        /* Display the current state of the player */
        BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
        BSP_LCD_DisplayStringAt(0, 215, (uint8_t *)"Playback on-going", CENTER_MODE);
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

        /* Next time Pause command should be processed */
        uwCommand = AUDIO_PAUSE;
      }
    }
    /* Check on the Volume Low button */
    if (BSP_JOY_GetState() == JOY_DOWN)
    {
      /* Wait to avoid rebound */
      while (BSP_JOY_GetState() == JOY_DOWN);
      /* Decrease volume by 5% */
      if (uwVolume > 5)
      {
        uwVolume -= 5;
      }
      else
      {
        uwVolume = 0;
      }

      /* Apply the new volume to the codec */
      BSP_AUDIO_OUT_SetVolume(uwVolume);
      sprintf((char *) Volume_string, "Volume : %lu%% ", uwVolume);
      BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-79, Volume_string, LEFT_MODE);
    }
    /* Check on the Volume High button */
    if (BSP_JOY_GetState() == JOY_UP)
    {
      /* Wait to avoid rebound */
      while (BSP_JOY_GetState() == JOY_UP);
      /* Increase volume by 5% */
      if (uwVolume < 95)
      {
        uwVolume += 5;
      }
      else
      {
        uwVolume = 100;
      }

      /* Apply the new volume to the codec */
      BSP_AUDIO_OUT_SetVolume(uwVolume);
      sprintf((char *) Volume_string, "Volume : %lu%% ", uwVolume);
      BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-79, Volume_string, LEFT_MODE);
    }

    /* Toggle LED3 */
    BSP_LED_Toggle(LED3);

    /* Insert 100 ms delay */
    HAL_Delay(100);

    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);

    /* Insert 100 ms delay */
    HAL_Delay(100);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 100000000
  *            HCLK(Hz)                       = 100000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 200
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}

/**
  * @brief  Display main example message
  * @param  None
  * @retval None
  */
static void Display_ExampleDescription(void)
{
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Clear the LCD */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);

  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32F412xG Example", CENTER_MODE);
  
  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 35, (uint8_t *)stlogo);
  
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 35, (uint8_t *)"Copyright (c)", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 20, (uint8_t *)"STMicroelectronics 2017", CENTER_MODE);
  
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 - 10, BSP_LCD_GetXSize(), 60);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_DARKBLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2, (uint8_t *)"Press Joystick sel-button", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 15, (uint8_t *)"to start :", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 30, (uint8_t *)"AUDIO playback Example", CENTER_MODE);
}

/**
  * @brief  Display AUDIO Playback Demo Hint
  * @param  None
  * @retval None
  */
static void AudioPlay_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 95);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t*)"AUDIO Playback", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"This example provides basic", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"implementation of audio features", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 75, (uint8_t*)"through the I2S peripheral ", CENTER_MODE);

   /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 105, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 115);
  BSP_LCD_DrawRect(11, 106, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 117);

  /* Prepare LCD to display */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(12, 107, BSP_LCD_GetXSize() - 24, BSP_LCD_GetYSize()- 119);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
}

/**
  * @brief  Display audio file and control information
  * @param  format : structure containing information of the audio file
  * @retval None
  */
static void AudioPlay_DisplayInfos(WAVE_FormatTypeDef * format)
{
  uint8_t string[50] = {0};

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  sprintf((char *) string, "Sampling frequency : %lu Hz", format->SampleRate);
  BSP_LCD_DisplayStringAt(20, 131, string, LEFT_MODE);

  if (format->NbrChannels == 2)
  {
    sprintf((char *) string, "Format : %d bits stereo", format->BitPerSample);
    BSP_LCD_DisplayStringAt(20, 146, string, LEFT_MODE);
  }
  else if (format->NbrChannels == 1)
  {
    sprintf((char *) string, "Format : %d bits mono", format->BitPerSample);
    BSP_LCD_DisplayStringAt(20, 146, string, LEFT_MODE);
  }
  
  sprintf((char *) Volume_string, "Volume : %lu%% ", uwVolume);
  BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-79, Volume_string, LEFT_MODE);
  
  BSP_LCD_DisplayStringAt(20, 176, (uint8_t *)"Joy Sel: Pause/Resume", LEFT_MODE);
  BSP_LCD_DisplayStringAt(20, 191, (uint8_t *)"Joy UP/DOWN: change Volume", LEFT_MODE);
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
  if (audio_state == AUDIO_STATE_PLAYING)
  {
    /* Calculate the remaining audio data in the file and the new size
    for the DMA transfer. If the Audio files size is less than the DMA max
    data transfer size, so there is no calculation to be done, just restart
    from the beginning of the file ... */
    /* Check if the end of file has been reached */
    if(AudioRemSize > 0)
    { 
      /* Replay from the current position */
      BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)CurrentPos, DMA_MAX(AudioRemSize));
      
      /* Update the current pointer position */
      CurrentPos += DMA_MAX(AudioRemSize);
      
      /* Update the remaining number of data to be played */
      AudioRemSize -= DMA_MAX(AudioRemSize);
    }
    else
    {
      /* Set the current audio pointer position */
      CurrentPos = (uint16_t*)(AUDIO_FILE_ADDRESS + AUDIO_START_OFFSET_ADDRESS);
      /* Replay from the beginning */
      BSP_AUDIO_OUT_Play((uint16_t*)CurrentPos,  (uint32_t)(AUDIO_FILE_SIZE - AUDIO_START_OFFSET_ADDRESS));
      /* Update the remaining number of data to be played */
      AudioRemSize = AudioTotalSize - DMA_MAX(AudioTotalSize);
      /* Update the current audio pointer position */
      CurrentPos += DMA_MAX(AudioTotalSize);
    }
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
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(0, 215, (uint8_t *)"    DMA  ERROR    ", CENTER_MODE);

  /* Stop the program with an infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Stop the program with an infinite loop */
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */
