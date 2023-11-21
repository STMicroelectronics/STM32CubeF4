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

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup I2S_Audio
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MESSAGE1   "     STM32F40xx     "
#define MESSAGE2   " Device running on  " 
#define MESSAGE3   "   STM324xG-EVAL    "

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t uwCommand = AUDIO_PAUSE;
__IO uint32_t uwVolume = AUDIO_DEFAULT_VOLUME;

/* Variable to indicate that push buttons will be used for switching between 
   Headphone and Speaker output modes. */
uint32_t uwSpHpSwitch = 0;

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
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  
  /* Configure LEDs, Push buttons and LCD available on EVAL board ************/
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Configure push Buttons */
  /* Key button used for Pause/Resume */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO); 
  /* Wakeup button used for Volume High */    
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_GPIO); 
  /* Tamper button used for Volume Low */ 
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
  BSP_LCD_DisplayStringAtLine(0, (uint8_t *)MESSAGE1);
  BSP_LCD_DisplayStringAtLine(1, (uint8_t *)MESSAGE2);
  BSP_LCD_DisplayStringAtLine(2, (uint8_t *)MESSAGE3);

  /* Turn On LEDs available on EVAL *******************************************/
  BSP_LED_On(LED1);
  BSP_LED_On(LED2);
  BSP_LED_On(LED3);
  BSP_LED_On(LED4);

  /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOExpander, IOs...) */  
  if(AUDIO_Init() == AUDIO_ERROR_NONE)
  {
    BSP_LCD_DisplayStringAtLine(3, (uint8_t *)"====================");
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"Key   : Play/Pause ");
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *)"Tamper: Vol+/Headph");
    BSP_LCD_DisplayStringAtLine(6, (uint8_t *)"Wakeup: Vol-/Spkr  ");
    BSP_LCD_DisplayStringAtLine(7, (uint8_t *)"====================");
    BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"  AUDIO CODEC   OK  ");    
  }
  else
  {
    BSP_LCD_DisplayStringAtLine(4, (uint8_t *)"  AUDIO CODEC  FAIL ");
    BSP_LCD_DisplayStringAtLine(5, (uint8_t *)" Try to reset board ");
  }
  
  /* 
  Start playing the file from a circular buffer, once the DMA is enabled, it is 
  always in running state. Application has to fill the buffer with the audio data 
  using Transfer complete and/or half transfer complete interrupts callbacks 
  (EVAL_AUDIO_TransferComplete_CallBack() or EVAL_AUDIO_HalfTransfer_CallBack()...
  */
  AUDIO_Start();
  
  /* Display the state on the screen */
  BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"       PLAYING      ");
  
  /* Infinite loop */
  while (1)
  {    
    /* Check on the Pause/Resume button */
    if(BSP_PB_GetState(BUTTON_KEY) == RESET)
    {
      /* Wait to avoid rebound */
      while (BSP_PB_GetState(BUTTON_KEY) == RESET);
      
      if(uwCommand == AUDIO_PAUSE)
      {
        BSP_AUDIO_OUT_Pause();
        /* Display the current state of the player */
        BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"       PAUSED       ");
        
        /* Next time Resume command should be processed */
        uwCommand = AUDIO_RESUME;
        
        /* Push buttons will be used to switch between Speaker and Headphone modes */
        uwSpHpSwitch = 1;
      }
      else
      {
        BSP_AUDIO_OUT_Resume();
        /* Display the current state of the player */
        BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"       PLAYING      ");
        
        /* Next time Pause command should be processed */
        uwCommand = AUDIO_PAUSE;
        
        /* Push buttons will be used to control volume level */
        uwSpHpSwitch = 0;
      }
    }
    
    /* Check on the Volume high button */
    if (BSP_PB_GetState(BUTTON_WAKEUP) != RESET)
    {
      /* Check if the current state is paused (push buttons are used for volume control or for 
      speaker/headphone mode switching) */
      if (uwSpHpSwitch)
      {
        /* Set output to Speaker */
        BSP_AUDIO_OUT_SetOutputMode(OUTPUT_DEVICE_SPEAKER);
        
        /* Display the current state of the player */
        BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"       SPEAKER      ");
      }
      else
      {
        /* Wait to avoid rebound */
        while (BSP_PB_GetState(BUTTON_WAKEUP) != RESET);
        
        /* Decrease volume by 5% */
        if (uwVolume > 5)
          uwVolume -= 5; 
        else
          uwVolume = 0; 
        
        /* Apply the new volume to the codec */
        BSP_AUDIO_OUT_SetVolume(uwVolume);
        BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"       VOL:   -     ");
      }
    }    
    
    /* Check on the Volume high button */
    if (BSP_PB_GetState(BUTTON_TAMPER) == RESET)
    {
      /* Check if the current state is paused (push buttons are used for volume control or for 
         speaker/headphone mode switching) */
      if (uwSpHpSwitch)
      {
        /* Set output to Headphone */
        BSP_AUDIO_OUT_SetOutputMode(OUTPUT_DEVICE_HEADPHONE);
        
        /* Display the current state of the player */
        BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"      HEADPHONE     ");
      }
      else
      {
        /* Wait to avoid rebound */
        while (BSP_PB_GetState(BUTTON_TAMPER) == RESET);
        
        /* Increase volume by 5% */
        if (uwVolume < 95)
          uwVolume += 5; 
        else
          uwVolume = 100; 
        
        /* Apply the new volume to the codec */
        BSP_AUDIO_OUT_SetVolume(uwVolume);
        BSP_LCD_DisplayStringAtLine(9, (uint8_t *)"       VOL:   +     ");  
      }
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
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
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
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  /* STM32F405x/407x/415x/417x Revision Z and upper devices: prefetch is supported  */
  if (HAL_GetREVID() >= 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
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
