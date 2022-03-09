/**
  ******************************************************************************
  * @file    SAI/SAI_Audio/Src/main.c
  * @author  MCD Application Team
  * @brief   This example provides a description of how to configure SAI
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
#include "audio_if.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup SAI_Audio
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   "     STM32F4xx    "
#define MESSAGE2   " Device running on  " 
#define MESSAGE3   "   STM32446E-EVAL    "

   
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t uwVolume = AUDIO_DEFAULT_VOLUME;
__IO uint8_t uwPauseEnabledStatus = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization: global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz MHz */
  SystemClock_Config();

  /* -1- Initialize LEDs mounted on EVAL board */
  /* Configure LED1*/
  BSP_LED_Init(LED1);
  
  /* -2- Configure User push-button in Gpio mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);
  
  /* Initialize the LCD */
  BSP_LCD_Init();

  
  /* Display message on EVAL LCD **********************************************/
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_BLUE);  
  
  /* Set the LCD Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, LINE(0), (uint8_t *)MESSAGE1, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)MESSAGE2, CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *)MESSAGE3, CENTER_MODE);
  
  /* Turn on LEDs available on EVAL *******************************************/
  BSP_LED_On(LED1);
  BSP_LED_On(LED3);
  
  BSP_LCD_SetFont(&Font16);

  /* Initialize the Audio codec and all related peripherals (SAI, I2C, IOs...) */  
  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_BOTH, uwVolume, SAI_AUDIO_FREQUENCY_48K) == 0)
  {
    BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"====================", CENTER_MODE);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"Short press on User button: Volume Down ", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"Long  press on User button: Pause/Resume", CENTER_MODE);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"====================", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"   AUDIO CODEC OK   ", CENTER_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"  AUDIO CODEC  FAIL ", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, LINE(6), (uint8_t *)" Try to reset board ", CENTER_MODE);
  }
  
  /* 
  Start playing the file from a circular buffer, once the DMA is enabled, it is 
  always in running state. Application has to fill the buffer with the audio data 
  using Transfer complete and/or half transfer complete interrupts callbacks 
  (EVAL_AUDIO_TransferComplete_CallBack() or EVAL_AUDIO_HalfTransfer_CallBack()...
  */
  AUDIO_Start();
  
  /* Display the state on the screen */
  BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"       PLAYING...     ", CENTER_MODE);

   
  /* IMPORTANT:
     AUDIO_Process() is called by the SysTick Handler, as it should be called 
     within a periodic process */
   
  /* Infinite loop */
  while(1)
  {

    if (BSP_PB_GetState(BUTTON_TAMPER) != RESET)
    {
       /* User push-button pressed, check if it is a long or a short press */

      /* Insert 350 ms delay */
      HAL_Delay(350);
      
      if (BSP_PB_GetState(BUTTON_TAMPER) == RESET)
      {
        /* User push-button short press : Volume down */
        /* Decrease volume by 15% */
        if (uwVolume > 20)
        {
          uwVolume -= 15;
        }
        else
        {
          uwVolume = 100;
        }
        /* Apply the new volume to the codec */
        BSP_AUDIO_OUT_SetVolume(uwVolume);

        if (uwVolume > 85)
          BSP_LCD_DisplayStringAt(0, LINE(11), (uint8_t *)"     VOL: <     X>    ", CENTER_MODE);
        else if (uwVolume > 70)
          BSP_LCD_DisplayStringAt(0, LINE(11), (uint8_t *)"     VOL: <    X >    ", CENTER_MODE);
        else if (uwVolume > 55)
          BSP_LCD_DisplayStringAt(0, LINE(11), (uint8_t *)"     VOL: <   X  >    ", CENTER_MODE);
        else if (uwVolume > 40)
          BSP_LCD_DisplayStringAt(0, LINE(11), (uint8_t *)"     VOL: <  X   >    ", CENTER_MODE);
        else if (uwVolume > 25)
          BSP_LCD_DisplayStringAt(0, LINE(11), (uint8_t *)"     VOL: < X    >    ", CENTER_MODE);
        else
          BSP_LCD_DisplayStringAt(0, LINE(11), (uint8_t *)"     VOL: <X     >    ", CENTER_MODE);
      }
      else
      {
        /* User push-button pressed for more then 350ms : Pause/Resume  */
        if (uwPauseEnabledStatus == 1)
        { /* Pause is enabled, call Resume */
          BSP_AUDIO_OUT_Resume();
          uwPauseEnabledStatus = 0;
          BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"       PLAYING ...    ", CENTER_MODE);
        } else
        { /* Pause the playback */
          BSP_AUDIO_OUT_Pause();
          uwPauseEnabledStatus = 1;
          BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"       PAUSE ...    ", CENTER_MODE);
        }
        while (BSP_PB_GetState(BUTTON_TAMPER) != RESET);
      }
    }
    
    /* Toggle LED1 */
    BSP_LED_Toggle(LED1);
    
    /* Insert 100 ms delay */
    HAL_Delay(100);

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
  BSP_LCD_DisplayStringAt(0, LINE(12), (uint8_t *)"       DMA ERROR      ", CENTER_MODE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_ClearStringLine(10);

  /* Stop the program with an infinite loop */
  while (1)
  {}

  /* could also generate a system reset to recover from the error */
  /* .... */
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
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
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 180 MHz Frequency */  
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
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
void assert_failed(uint8_t* file, uint32_t line)
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
