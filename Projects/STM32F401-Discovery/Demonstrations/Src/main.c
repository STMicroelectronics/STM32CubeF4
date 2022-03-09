/**
  ******************************************************************************
  * @file    Demonstrations/Src/main.c 
  * @author  MCD Application Team
  * @brief   This demo describes how to use accelerometer to control mouse on 
  *          PC.
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

/** @addtogroup STM32F4xx_HAL_Demonstrations
  * @{
  */

/** @addtogroup Demo
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY_PRESSED     0x01
#define KEY_NOT_PRESSED 0x00

/* TIM4 Autoreload and Capture Compare register values */
#define TIM_ARR        (uint16_t)1999
#define TIM_CCR        (uint16_t)1000

#define CURSOR_STEP     7

/* Private macro -------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x
#define MAX_AB(a,b)       (a < b) ? (b) : a

/* Private variables ---------------------------------------------------------*/
__IO uint8_t UserButtonPressed = 0x00;
__IO uint8_t DemoEnterCondition = 0x00;

/* Variables used for accelerometer */
__IO int16_t X_Offset, Y_Offset;
int16_t Buffer[3];

/* MEMS thresholds {Low/High} */
static int16_t ThreadholdAcceleroLow = -1000, ThreadholdAcceleroHigh = 1000;

/* Variables used for USB */
USBD_HandleTypeDef  hUSBDDevice;

/* Variables used for timer */
uint16_t PrescalerValue = 0;
TIM_HandleTypeDef htim4;
TIM_OC_InitTypeDef sConfigTim4;

/* Variables used during Systick ISR */
uint8_t Counter  = 0x00;
__IO uint16_t MaxAcceleration = 0;

/* Private function prototypes -----------------------------------------------*/
static uint32_t Demo_USBConfig(void);
static void TIM4_Config(void);
static void Demo_Exec(void);
static uint8_t *USBD_HID_GetPos(void);
static void SystemClock_Config(void);
static void Error_Handler(void);

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

  /* Configure LED4, LED3, LED5 and LED6 */
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  /* Configure the system clock to 84 MHz */
  SystemClock_Config();

  /* Initialize User_Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI); 

  /* Execute Demo application */
  Demo_Exec();
  
  /* Infinite loop */
  while (1)
  {    
  }
}

/**
  * @brief  Execute the demo application.
  * @param  None
  * @retval None
  */
static void Demo_Exec(void)
{
  uint8_t togglecounter = 0x00;
  
  /* Initialize Accelerometer MEMS*/
  if(BSP_ACCELERO_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }

  while(1)
  {
    DemoEnterCondition = 0x00;
    
    /* Reset UserButton_Pressed variable */
    UserButtonPressed = 0x00;
    
    /* Configure LEDs to be managed by GPIO */
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);
    BSP_LED_Init(LED5);
    BSP_LED_Init(LED6);
    
    /* SysTick end of count event each 10ms */
    SystemCoreClock = HAL_RCC_GetHCLKFreq();
    SysTick_Config(SystemCoreClock / 100);  
    
    /* Turn OFF all LEDs */
    BSP_LED_Off(LED4);
    BSP_LED_Off(LED3);
    BSP_LED_Off(LED5);
    BSP_LED_Off(LED6);
    
    /* Waiting User Button is pressed */
    while (UserButtonPressed == 0x00)
    {
      /* Toggle LED4 */
      BSP_LED_Toggle(LED4);
      HAL_Delay(10);
      /* Toggle LED4 */
      BSP_LED_Toggle(LED3);
      HAL_Delay(10);
      /* Toggle LED4 */
      BSP_LED_Toggle(LED5);
      HAL_Delay(10);
      /* Toggle LED4 */
      BSP_LED_Toggle(LED6);
      HAL_Delay(10);
      togglecounter ++;
      if (togglecounter == 0x10)
      {
        togglecounter = 0x00;
        while (togglecounter < 0x10)
        {
          BSP_LED_Toggle(LED4);
          BSP_LED_Toggle(LED3);
          BSP_LED_Toggle(LED5);
          BSP_LED_Toggle(LED6);
          HAL_Delay(10);
          togglecounter ++;
        }
        togglecounter = 0x00;
      }
    }
    
    /* Waiting User Button is Released */
    while (BSP_PB_GetState(BUTTON_KEY) != KEY_NOT_PRESSED)
    {}
    UserButtonPressed = 0x00;
    
    /* TIM4 channels configuration */
    TIM4_Config();
  
    DemoEnterCondition = 0x01; 
    
    /* USB configuration */
    Demo_USBConfig();
    
    /* Waiting User Button is pressed */
    while (UserButtonPressed == 0x00)
    {}
    
    /* Waiting User Button is Released */
    while (BSP_PB_GetState(BUTTON_KEY) != KEY_NOT_PRESSED)
    {}
    
    /* Disconnect the USB device */
    USBD_Stop(&hUSBDDevice);
    USBD_DeInit(&hUSBDDevice);
    if(HAL_TIM_PWM_DeInit(&htim4) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
  }
}

/**
  * @brief  Initializes the USB for the demonstration application.
  * @param  None
  * @retval None
  */
static uint32_t Demo_USBConfig(void)
{
  /* Init Device Library */
  USBD_Init(&hUSBDDevice, &HID_Desc, 0);
  
  /* Add Supported Class */
  USBD_RegisterClass(&hUSBDDevice, USBD_HID_CLASS);
  
  /* Start Device Process */
  USBD_Start(&hUSBDDevice);
  
  return 0;
}

/**
  * @brief  Configures the TIM Peripheral.
  * @param  None
  * @retval None
  */
static void TIM4_Config(void)
{
  /* -----------------------------------------------------------------------
  TIM4 Configuration: Output Compare Timing Mode:
  
  In this example TIM4 input clock (TIM4CLK) is set to 2 * APB1 clock (PCLK1), 
  since APB1 prescaler is different from 1 (APB1 Prescaler = 4, see system_stm32f4xx.c file).
  TIM4CLK = 2 * PCLK1  
  PCLK1 = HCLK / 4 
  => TIM4CLK = 2*(HCLK / 4) = HCLK/2 = SystemCoreClock/2
  
  To get TIM4 counter clock at 2 KHz, the prescaler is computed as follows:
  Prescaler = (TIM4CLK / TIM4 counter clock) - 1
  Prescaler = (84 MHz/(2 * 2 KHz)) - 1 = 41999
  
  To get TIM4 output clock at 1 Hz, the period (ARR)) is computed as follows:
  ARR = (TIM4 counter clock / TIM4 output clock) - 1
  = 1999
  
  TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 50%
  TIM4 Channel2 duty cycle = (TIM4_CCR2/ TIM4_ARR)* 100 = 50%
  TIM4 Channel3 duty cycle = (TIM4_CCR3/ TIM4_ARR)* 100 = 50%
  TIM4 Channel4 duty cycle = (TIM4_CCR4/ TIM4_ARR)* 100 = 50%
  
  ==> TIM4_CCRx = TIM4_ARR/2 = 1000  (where x = 1, 2, 3 and 4).
  ----------------------------------------------------------------------- */ 
  
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 2000) - 1;
  
  /* Time base configuration */
  htim4.Instance             = TIM4;
  htim4.Init.Period          = TIM_ARR;
  htim4.Init.Prescaler       = PrescalerValue;
  htim4.Init.ClockDivision   = 0;
  htim4.Init.CounterMode     = TIM_COUNTERMODE_UP;
  if(HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* TIM PWM1 Mode configuration: Channel */
  /* Output Compare Timing Mode configuration: Channel1 */
  sConfigTim4.OCMode = TIM_OCMODE_PWM1;
  sConfigTim4.OCIdleState = TIM_CCx_ENABLE;
  sConfigTim4.Pulse = TIM_CCR;
  sConfigTim4.OCPolarity = TIM_OCPOLARITY_HIGH;
  
  /* Output Compare PWM1 Mode configuration: Channel1 */
  if(HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigTim4, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Output Compare PWM1 Mode configuration: Channel2 */
  if(HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigTim4, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Output Compare PWM1 Mode configuration: Channel3 */
  if(HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigTim4, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  /* Output Compare PWM1 Mode configuration: Channel4 */
  if(HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigTim4, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  SYSTICK callback.
  * @param  None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  uint8_t *buf;
  uint16_t Temp_X, Temp_Y = 0x00;
  uint16_t NewARR_X, NewARR_Y = 0x00;
  
 if (DemoEnterCondition != 0x00)
  {
    buf = USBD_HID_GetPos();
    if((buf[1] != 0) ||(buf[2] != 0))
    {
      USBD_HID_SendReport (&hUSBDDevice, 
                           buf,
                           4);
    } 
    Counter ++;
    if (Counter == 10)
    {
      /* Reset Buffer used to get accelerometer values */
      Buffer[0] = 0;
      Buffer[1] = 0;
      
      /* Disable All TIM4 Capture Compare Channels */
      HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
      HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
      HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
      HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);

      /* Read Acceleration*/
      BSP_ACCELERO_GetXYZ(Buffer);
      
      /* Set X and Y positions */
      X_Offset = Buffer[0];
      Y_Offset = Buffer[1];
      
      /* Update New autoreload value in case of X or Y acceleration*/
      /* Basic acceleration X_Offset and Y_Offset are divide by 40 to fir with ARR range */
      NewARR_X = TIM_ARR - ABS(X_Offset/40);
      NewARR_Y = TIM_ARR - ABS(Y_Offset/40);
      
      /* Calculation of Max acceleration detected on X or Y axis */
      Temp_X = ABS(X_Offset/40);
      Temp_Y = ABS(Y_Offset/40);
      MaxAcceleration = MAX_AB(Temp_X, Temp_Y);

      if(MaxAcceleration != 0)
      {
        
        /* Reset CNT to a lowest value (equal to min CCRx of all Channels) */
        __HAL_TIM_SET_COUNTER(&htim4,(TIM_ARR-MaxAcceleration)/2);
        
        if (X_Offset < ThreadholdAcceleroLow)
        {
          
          /* Sets the TIM4 Capture Compare for Channel1 Register value */
          /* Equal to NewARR_X/2 to have duty cycle equal to 50% */
          __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, NewARR_X/2);  
          
          /* Time base configuration */      
          __HAL_TIM_SET_AUTORELOAD(&htim4, NewARR_X);
          
          /* Enable TIM4 Capture Compare Channel1 */
          HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);  
          
        }
        else if (X_Offset > ThreadholdAcceleroHigh)
        {
          
          /* Sets the TIM4 Capture Compare for Channel3 Register value */
          /* Equal to NewARR_X/2 to have duty cycle equal to 50% */
          __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, NewARR_X/2);                  
          
          /* Time base configuration */      
          __HAL_TIM_SET_AUTORELOAD(&htim4, NewARR_X);
          
          /* Enable TIM4 Capture Compare Channel3 */
          HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);  
          
        }
        if (Y_Offset > ThreadholdAcceleroHigh)
        { 
          
          /* Sets the TIM4 Capture Compare for Channel2 Register value */
          /* Equal to NewARR_Y/2 to have duty cycle equal to 50% */
          __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2,NewARR_Y/2);    
    
          /* Time base configuration */      
          __HAL_TIM_SET_AUTORELOAD(&htim4, NewARR_Y);

          /* Enable TIM4 Capture Compare Channel2 */
          HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);    
          
        }      
        else if (Y_Offset < ThreadholdAcceleroLow)
        { 
          
          /* Sets the TIM4 Capture Compare for Channel4 Register value */
          /* Equal to NewARR_Y/2 to have duty cycle equal to 50% */
          __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, NewARR_Y/2);   
          
          /* Time base configuration */      
          __HAL_TIM_SET_AUTORELOAD(&htim4, NewARR_Y);
          
          /* Enable TIM4 Capture Compare Channel4 */
          HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);    
          
        }
      }
      Counter = 0x00;
    }  
  }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == KEY_BUTTON_PIN) 
  {
    UserButtonPressed = 0x01;
  }
}

/**
  * @brief  USBD_HID_GetPos
  * @param  None
  * @retval Pointer to report
  */
static uint8_t *USBD_HID_GetPos(void)
{
  static uint8_t HID_Buffer[4] = {0};
  
  HID_Buffer[1] = 0;
  HID_Buffer[2] = 0;
  /* LEFT Direction */
  if((X_Offset) < ThreadholdAcceleroLow)
  {
    HID_Buffer[1] -= CURSOR_STEP;
  }
  /* RIGHT Direction */ 
  if((X_Offset) > ThreadholdAcceleroHigh)
  {
   HID_Buffer[1] += CURSOR_STEP;
  } 
  /* DOWN Direction */
  if((Y_Offset) < ThreadholdAcceleroLow)
  {
    HID_Buffer[2] += CURSOR_STEP;
  }
  /* UP Direction */ 
  if((Y_Offset) > ThreadholdAcceleroHigh)
  {
    HID_Buffer[2] -= CURSOR_STEP;
  } 
  
  return HID_Buffer;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED4 on */
  BSP_LED_On(LED4);
  while(1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 84000000
  *            HCLK(Hz)                       = 84000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 4
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale2 mode
  *            Flash Latency(WS)              = 2
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
 
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
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
