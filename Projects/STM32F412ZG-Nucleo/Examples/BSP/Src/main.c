/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the STM324xx_Nucleo BSP Drivers
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

/** @addtogroup STM32F4xx-Nucleo_Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{
  SHIELD_NOT_DETECTED = 0, 
  SHIELD_DETECTED
}ShieldStatus;

/* Private define -------------------------------------------------------------*/
/* tmp */
#define BUTTON_USER BUTTON_KEY
/* Private macro -------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------*/
uint8_t DemoIndex = 0;
uint8_t UserButtonPressed = 0x00;
uint32_t LedToggleDelay = 0x00;

/* Private function prototypes --------------------------------------------------*/
static void SystemClock_Config(void);
static ShieldStatus TFT_ShieldDetect(void);
#ifdef ADAFRUIT_TFT_JOY_SD_ID802
static void Display_DemoDescription(void);
#endif /* ADAFRUIT_TFT_JOY_SD_ID802 */

BSP_DemoTypedef  BSP_examples[]=
{
  {Joystick_demo, "JOYSTICK", 0},
  {LCD_demo, "LCD", 0}, 
  {SD_demo, "mSD", 0},
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t count = 0;

  /* Init HAL */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
  
  /* Retrieve System Core Clock */
  SystemCoreClock = HAL_RCC_GetHCLKFreq();
  
  /* Initialize User_Button on STM32F4xx-Nucleo ------------------*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI); 

  /* Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
     board. This is done by reading the state of IO PC.01 pin (mapped to JoyStick
     available on adafruit 1.8" TFT shield). If the state of PC.01 is high then
     the adafruit 1.8" TFT shield is available. */  
  if(TFT_ShieldDetect() == SHIELD_DETECTED)
  {
#ifdef ADAFRUIT_TFT_JOY_SD_ID802
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* Initialize the LCD */
    BSP_LCD_Init();

    Display_DemoDescription();

    while (1)
    {
      BSP_LED_Toggle(LED3);
      HAL_Delay(100);
      if(BSP_PB_GetState(BUTTON_USER) != RESET)
      {
        HAL_Delay(10);
        while (BSP_PB_GetState(BUTTON_USER) != RESET);
      
        BSP_examples[DemoIndex++].DemoFunc();
      
        if(DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
        {
          DemoIndex = 0;
        }
        Display_DemoDescription();
      }
    }
#endif /* ADAFRUIT_TFT_JOY_SD_ID802 */
  }  
  else /* there is no AdaFruit shield 802 connected */
  {
    /* Initialize Led1 on STM32F4xx-Nucleo ------------------*/
    BSP_LED_Init(LED1);  /* this is in conflict with Adafruit shield */
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* 0. Wait for User button to be pressed -------------------------------------*/
    while (BSP_PB_GetState(BUTTON_USER) != KEY_PRESSED)
    {
      /* Toggle LED3 */
      BSP_LED_Toggle(LED3);
      HAL_Delay(200);
    }
    /* Wait for User button is released */
    while (BSP_PB_GetState(BUTTON_USER) != KEY_NOT_PRESSED)
    {}

    BSP_LED_Off(LED3);

    /* Infinite loop */
    while (1)
    {
      if(UserButtonPressed==0x01)
      {
        count++;
        LedToggleDelay += 20*count;
        UserButtonPressed = 0x00;
        if(LedToggleDelay>500)
        {
          LedToggleDelay = 20;
          count = 0;
        }
      }
      /* Toggle LED1 & LED2 */
      BSP_LED_Toggle(LED1);
      HAL_Delay(LedToggleDelay);
      BSP_LED_Toggle(LED2);
      HAL_Delay(LedToggleDelay);
    }
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
#ifdef ADAFRUIT_TFT_JOY_SD_ID802

static void Display_DemoDescription(void)
{
  uint8_t desc[58];
  
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
  /* Clear the LCD */ 
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  
  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32F412ZG BSP", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 25, (uint8_t *)"Drivers examples", CENTER_MODE);
  
  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80)/2, 35, (uint8_t *)stlogo);
  
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 15, (uint8_t *)"Copyright (c) STM 2017", CENTER_MODE);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize()/2 + 15, BSP_LCD_GetXSize(), 40);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 25, (uint8_t *)"Use User Button to start", CENTER_MODE);
  sprintf((char *)desc,"%s example", BSP_examples[DemoIndex].DemoName);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 40, (uint8_t *)desc, CENTER_MODE);   
}
#endif /* ADAFRUIT_TFT_JOY_SD_ID802 */

/**
  * @brief  Check for user input.
  * @param  None
  * @retval Input state (1 : active / 0 : Inactive)
  */
uint8_t CheckForUserInput(void)
{
  if(BSP_PB_GetState(BUTTON_USER) != RESET)
  {
    HAL_Delay(10);
    while (BSP_PB_GetState(BUTTON_USER) != RESET);
    return 1 ;
  }
  return 0;
}



/**
  * @brief  Check the availability of adafruit 1.8" TFT shield on top of STM32NUCLEO
  *         board. This is done by reading the state of IO PC.01 pin (mapped to 
  *         JoyStick available on adafruit 1.8" TFT shield). If the state of PC.01 
  *         is high then the adafruit 1.8" TFT shield is available.
  * @param  None
  * @retval SHIELD_DETECTED: 1.8" TFT shield is available
  *         SHIELD_NOT_DETECTED: 1.8" TFT shield is not available
  */
static ShieldStatus TFT_ShieldDetect(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct; 

  /* Enable GPIO clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) != 0)
  {
    return SHIELD_DETECTED;
  }
  else
  {
    return SHIELD_NOT_DETECTED;
  }
}



/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==USER_BUTTON_PIN)
  {
    UserButtonPressed = 0x01;
  }
}


/**
  * @brief  This function handles the test program fail.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while(1)
  {
    /* red LED stays ON*/
    BSP_LED_On(LED2);
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
