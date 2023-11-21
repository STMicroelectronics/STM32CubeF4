/**
  ******************************************************************************
  * @file    BSP/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the STM32412G_DISCOVERY BSP Drivers
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

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t DemoIndex = 0;
uint8_t NbLoop = 1;
uint32_t SdmmcTest = 0; 
/* Global variables ----------------------------------------------------------*/
uint8_t SDDetectIT = 0;
/* Global extern variables ---------------------------------------------------*/
#ifndef USE_FULL_ASSERT
uint16_t ErrorCounter = 0;
#endif
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Display_DemoDescription(void);

BSP_DemoTypedef  BSP_examples[]=
{
  {Joystick_demo, "JOYSTICK", 0},
  {LCD_demo, "LCD", 0},
  {TS_demo, "TS", 0},  
  {SD_demo, "mSD", 0},
  {SD_DMA_demo, "mSD in DMA Mode", 0},  
  {SD_exti_demo, "mSD exti detect", 0},
  {Log_demo, "LCD LOG", 0},
  {EEPROM_demo, "EEPROM", 0},
  {QSPI_demo, "QSPI", 0},
  {AudioPlay_demo, "AUDIO PLAY", 0},
  {AudioRecAnalog_demo, "AUDIO REC ANALOG", 0},
  {AudioRecDfsdm_demo, "AUDIO REC DFSDM", 0},
};

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
  
  /* Configure the system clock to 100 Mhz */
  SystemClock_Config();

  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2); 
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4); 

  BSP_LED_On(LED1);
  BSP_LED_On(LED2); 
  BSP_LED_On(LED3);
  BSP_LED_On(LED4); 
 
  /* Configure the User Button in GPIO Mode */
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_GPIO);

  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  BSP_LCD_Init();

  Display_DemoDescription();
     
  /* Wait For User inputs */
  while (1)
  {
    if(BSP_PB_GetState(BUTTON_WAKEUP) != RESET)
    {
      while (BSP_PB_GetState(BUTTON_WAKEUP) != RESET);   /* Wait for button released */
      
      BSP_examples[DemoIndex++].DemoFunc();
      
      if(DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
      {
        /* Increment number of loops which be used by EEPROM example */
        NbLoop++;
        DemoIndex = 0;
      }
      Display_DemoDescription();
    }
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
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
    Error_Handler();
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
    Error_Handler();
  }
  
}

/**
  * @brief  Display main demo messages.
  * @param  None
  * @retval None
  */
static void Display_DemoDescription(void)
{
  uint8_t desc[58];
  
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Clear the LCD */ 
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);  
  
  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 2, (uint8_t *)"STM32F412G BSP", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 14, (uint8_t *)"Drivers examples", CENTER_MODE);
  
  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 30, (uint8_t *)stlogo);
  
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()-12, (uint8_t *)"STMicroelectronics 2017", CENTER_MODE);
  
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 1, BSP_LCD_GetXSize(), 60);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 20, (uint8_t *)"Press button to start :", CENTER_MODE);
  sprintf((char *)desc,"%s example", BSP_examples[DemoIndex].DemoName);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 35, (uint8_t *)desc, CENTER_MODE);
}

/**
  * @brief  Check for user input.
  * @param  None
  * @retval Input state (1 : active / 0 : Inactive)
  */
uint8_t CheckForUserInput(void)
{
  if(BSP_PB_GetState(BUTTON_WAKEUP) != RESET)
  {
    HAL_Delay(100);
    while (BSP_PB_GetState(BUTTON_WAKEUP) != RESET);
    return 1 ;
  }
  return 0;
}

/**
  * @brief EXTI line detection callbacks.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t debounce_time = 0;

  if(GPIO_Pin == BUTTON_WAKEUP)
  {
    /* Prevent debounce effect for user key */
    if((HAL_GetTick() - debounce_time) > 50)
    {
      debounce_time = HAL_GetTick();
    }
  }
  else if (GPIO_Pin == SD_DETECT_PIN)
  {
    /* SDDetectIT global variable set to notice SD_exti_demo changing state on SD_DETECT_PIN */
    SDDetectIT = 1;
  }
}

#ifdef USE_FULL_ASSERT

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
#endif /* USE_FULL_ASSERT */ 

/**
  * @}
  */ 

/**
  * @}
  */
