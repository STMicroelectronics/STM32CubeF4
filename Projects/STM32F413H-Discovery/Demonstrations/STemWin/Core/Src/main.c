/**
******************************************************************************
* @file    main.c 
* @author  MCD Application Team
* @brief   This file provides main program functions
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics International N.V. 
* All rights reserved.</center></h2>
*
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted, provided that the following conditions are met:
*
* 1. Redistribution of source code must retain the above copyright notice, 
*    this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
* 3. Neither the name of STMicroelectronics nor the names of other 
*    contributors to this software may be used to endorse or promote products 
*    derived from this software without specific written permission.
* 4. This software, including modifications and/or derivative works of this 
*    software, must execute solely and exclusively on microcontroller or
*    microprocessor devices manufactured by or for STMicroelectronics.
* 5. Redistribution and use of this software other than as permitted under 
*    this license is void and will automatically terminate your rights under 
*    this license. 
*
* THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
* RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
* SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h" 

/** @addtogroup CORE
* @{
*/

/** @defgroup MAIN
* @brief main file
* @{
*/ 

/** @defgroup MAIN_Private_TypesDefinitions
* @{
*/ 

/**
* @}
*/ 

/** @defgroup MAIN_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup MAIN_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup MAIN_Private_Variables
* @{
*/
/**
* @}
*/ 


/** @defgroup MAIN_Private_FunctionPrototypes
* @{
*/ 

static void SystemClock_Config(void);
static void GUIThread(void const * argument);

extern K_ModuleItem_Typedef  video_player_board;
extern K_ModuleItem_Typedef  audio_player_board;
extern K_ModuleItem_Typedef  audio_recorder_board;
extern K_ModuleItem_Typedef  analog_clock_board;
extern K_ModuleItem_Typedef  USB_Storage_board;
extern K_ModuleItem_Typedef  INFORMATION_board;

osTimerId lcd_timer;

uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
uint32_t UserButton_press = 0;

/**
* @}
*/ 

/** @defgroup MAIN_Private_Functions
* @{
*/ 

/**
* @brief  Main program
* @param  None
* @retval int
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
  
  /* Configure the system clock to 100 MHz */
  SystemClock_Config(); 
  
  /* Initialize Joystick, Touch screen and Leds */
  k_BspInit();
  
  /* Initializes backup domain */
  k_CalendarBkupInit();  
  
  /* Initialize wakeup button */
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_GPIO);  
  
  /* Create GUI task */
  osThreadDef(GUI_Thread, GUIThread, osPriorityNormal, 0, 4096);
  osThreadCreate (osThread(GUI_Thread), NULL); 
  
  /* Add Modules*/
  k_ModuleInit();
  
  /* Link modules */   
  k_ModuleAdd(&video_player_board);
  k_ModuleAdd(&audio_player_board);
  k_ModuleAdd(&audio_recorder_board);
  k_ModuleAdd(&analog_clock_board);
  k_ModuleAdd(&USB_Storage_board);
  k_ModuleAdd(&INFORMATION_board); 
  
  GUI_X_InitOS();
  
  /* Check whether or not USER button is pressed
  at start-up (if yes, triggers a user-requested
  Touch Screen calibration) */
  if (BSP_PB_GetState(BUTTON_WAKEUP) == GPIO_PIN_SET)
  {
    UserButton_press = 1;  
  }  
  
  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}

/**
* @brief  Start task
* @param  argument: pointer that is passed to the thread function as start argument.
* @retval None
*/
static void GUIThread(void const * argument)
{    
  /* Initialize Storage Units */
  k_StorageInit(); 
  
  /* Initialize GUI */
  GUI_Init();
  
  if(TouchScreen_IsCalibrationDone() == 0)
  {
    Touchscreen_Calibration();
  }
  else
  {
    BSP_TS_InitEx(240, 240, TS_ORIENTATION_LANDSCAPE_ROT180);
  }
  
  /* Configure TS interrupt */ 
  BSP_TS_ITConfig();
  
  /* Demo Startup */
  k_StartUp();
  
  /* Enable memory devices */
  WM_SetCreateFlags(WM_CF_MEMDEV);  
  
  /* Set General Graphical proprieties */
  k_SetGuiProfile();
  
  /* Show the main menu */
  k_InitMenu();
  
  /* Gui background Task */
  while(1) {
    GUI_Exec(); /* Do the background work ... Update windows etc.) */    
    osDelay(20); /* Nothing left to do for the moment ... Idle processing */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
  
  HAL_StatusTypeDef ret = HAL_OK;
  
  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
  clocked below the maximum system frequency, to update the voltage scaling value 
  regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
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
    while(1);
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
    while(1);
  }
  
  /* Select PLLI2S output as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CK48;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CK48CLKSOURCE_PLLI2SQ;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 8;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
  PeriphClkInitStruct.PLLI2S.PLLI2SQ = 4;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 4;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct)  != HAL_OK)
  {
    while(1);
  }  
}


#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n", 
  file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}

#endif


/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
