/**
  ******************************************************************************
  * @file    Audio/Audio_playback_and_record/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef hTimLed;
TIM_OC_InitTypeDef sConfigLed;

/* Counter for User button presses. Defined as external in waveplayer.c file */
__IO uint32_t PressCount = 0;

/* Wave Player Pause/Resume Status. Defined as external in waveplayer.c file */
__IO uint32_t PauseResumeStatus = IDLE_STATUS;   
                                                   
extern uint32_t AudioPlayStart;

/* Re-play Wave file status on/off.
   Defined as external in waveplayer.c file */
__IO uint32_t RepeatState = REPEAT_ON;

/* Capture Compare Register Value.
   Defined as external in stm32f4xx_it.c file */
__IO uint16_t CCR1Val = 16826;              
                                            
extern __IO uint32_t LEDsState;

/* Save MEMS ID */
uint8_t MemsID = 0; 

__IO uint32_t CmdIndex = CMD_PLAY;
__IO uint32_t PbPressCheck = 0;

FATFS USBDISKFatFs;          /* File system object for USB disk logical drive */
char USBDISKPath[4];         /* USB Host logical drive path */
USBH_HandleTypeDef hUSB_Host; /* USB Host handle */

MSC_ApplicationTypeDef AppliState = APPLICATION_IDLE;
static uint8_t  USBH_USR_ApplicationState = USBH_USR_FS_INIT;

/* Private function prototypes -----------------------------------------------*/
static void TIM_LED_Config(void);
static void SystemClock_Config(void);
static void USBH_UserProcess(USBH_HandleTypeDef *pHost, uint8_t vId);
static void MSC_Application(void);
static void COMMAND_AudioExecuteApplication(void);

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
  
  /* Configure LED3, LED4, LED5 and LED6 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);
  
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  
  /* Initialize MEMS Accelerometer mounted on STM32F4-Discovery board */
  if(BSP_ACCELERO_Init() != ACCELERO_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  MemsID = BSP_ACCELERO_ReadID();
  
  /* Turn ON LED4: start of application */
  BSP_LED_On(LED4);
  
  /* Configure TIM4 Peripheral to manage LEDs lighting */
  TIM_LED_Config();
  
  /* Initialize the Repeat state */
  RepeatState = REPEAT_ON;
  
  /* Turn OFF all LEDs */
  LEDsState = LEDS_OFF;
  
  /* Configure USER Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
  
  /*##-1- Link the USB Host disk I/O driver ##################################*/
  if(FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
  { 
    /*##-2- Init Host Library ################################################*/
    USBH_Init(&hUSB_Host, USBH_UserProcess, 0);
    
    /*##-3- Add Supported Class ##############################################*/
    USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);
    
    /*##-4- Start Host Process ###############################################*/
    USBH_Start(&hUSB_Host);
    
    /* Run Application (Blocking mode)*/
    while (1)
    {
      switch(AppliState)
      {
      case APPLICATION_START:
        MSC_Application();
        break;      
      case APPLICATION_IDLE:
      default:
        break;      
      }
      
      /* USBH_Background Process */
      USBH_Process(&hUSB_Host);
    }
  }
  
  /* TrueStudio compilation error correction */
  while (1)
  {
  }
}

/**
  * @brief  User Process
  * @param  phost: Host Handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess (USBH_HandleTypeDef *pHost, uint8_t vId)
{  
  switch (vId)
  { 
  case HOST_USER_SELECT_CONFIGURATION:
    break;
    
  case HOST_USER_DISCONNECTION:
    WavePlayer_CallBack();
    AppliState = APPLICATION_IDLE;
    f_mount(NULL, (TCHAR const*)"", 0);          
    break;
    
  case HOST_USER_CLASS_ACTIVE:
    AppliState = APPLICATION_START;
    break;
    
  case HOST_USER_CONNECTION:
    break;
    
  default:
    break; 
  }
}

/**
  * @brief  Main routine for Mass storage application
  * @param  None
  * @retval None
  */
static void MSC_Application(void)
{
  switch (USBH_USR_ApplicationState)
  {
  case USBH_USR_AUDIO:
    /* Go to Audio menu */
    COMMAND_AudioExecuteApplication();
    
    /* Set user initialization flag */
    USBH_USR_ApplicationState = USBH_USR_FS_INIT;
    break;
    
  case USBH_USR_FS_INIT:
    /* Initializes the File System */
    if (f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 0 ) != FR_OK ) 
    {
      /* FatFs initialisation fails */
      Error_Handler();
    }
    
    /* Go to menu */
    USBH_USR_ApplicationState = USBH_USR_AUDIO;
    break;
    
  default:
    break;
  }
}

/**
  * @brief  COMMAND_AudioExecuteApplication.
  * @param  None
  * @retval None
  */
static void COMMAND_AudioExecuteApplication(void)
{
  /* Execute the command switch the command index */
  switch (CmdIndex)
  {
    /* Start Playing from USB Flash memory */
  case CMD_PLAY:
    if (RepeatState == REPEAT_ON)
      WavePlayerStart();
    break;
    
    /* Start Recording in USB Flash memory */ 
  case CMD_RECORD:
    RepeatState = REPEAT_ON;
    WaveRecorderProcess();
    break;
    
  default:
    break;
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
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }  
}

/**
  * @brief  Configures TIM4 Peripheral for LEDs lighting.
  * @param  None
  * @retval None
  */
static void TIM_LED_Config(void)
{
  uint16_t prescalervalue = 0;
  uint32_t tmpvalue = 0;

  /* TIM4 clock enable */
  __HAL_RCC_TIM4_CLK_ENABLE();

  /* Enable the TIM4 global Interrupt */
  HAL_NVIC_SetPriority(TIM4_IRQn, 6, 0);  
  HAL_NVIC_EnableIRQ(TIM4_IRQn);
  
  /* -----------------------------------------------------------------------
  TIM4 Configuration: Output Compare Timing Mode:  
    To get TIM4 counter clock at 550 KHz, the prescaler is computed as follows:
    Prescaler = (TIM4CLK / TIM4 counter clock) - 1
    Prescaler = ((f(APB1) * 2) /550 KHz) - 1
  
    CC update rate = TIM4 counter clock / CCR_Val = 32.687 Hz
    ==> Toggling frequency = 16.343 Hz  
  ----------------------------------------------------------------------- */
  
  /* Compute the prescaler value */
  tmpvalue = HAL_RCC_GetPCLK1Freq();
  prescalervalue = (uint16_t) ((tmpvalue * 2) / 550000) - 1;
  
  /* Time base configuration */
  hTimLed.Instance = TIM4;
  hTimLed.Init.Period = 65535;
  hTimLed.Init.Prescaler = prescalervalue;
  hTimLed.Init.ClockDivision = 0;
  hTimLed.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_OC_Init(&hTimLed) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Output Compare Timing Mode configuration: Channel1 */
  sConfigLed.OCMode = TIM_OCMODE_TIMING;
  sConfigLed.OCIdleState = TIM_OCIDLESTATE_SET;
  sConfigLed.Pulse = CCR1Val;
  sConfigLed.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigLed.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigLed.OCFastMode = TIM_OCFAST_ENABLE;
  sConfigLed.OCNIdleState = TIM_OCNIDLESTATE_SET;
  
  /* Initialize the TIM4 Channel1 with the structure above */
  if(HAL_TIM_OC_ConfigChannel(&hTimLed, &sConfigLed, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Start the Output Compare */
  if(HAL_TIM_OC_Start_IT(&hTimLed, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
  {
  }
}

/**
  * @brief  Output Compare callback in non blocking mode 
  * @param  htim : TIM OC handle
  * @retval None
  */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  uint32_t capture = 0; 
  
  /* Set click recognition only for L1S302DL*/
  if ((AudioPlayStart != 0x00) && (MemsID == I_AM_LIS302DL))
  {
    /* Read click and status registers*/
    BSP_ACCELERO_Click_ITClear();  
  }
  
  if (LEDsState == LED3_TOGGLE)
  {
    /* Toggling LED3 */
    BSP_LED_Toggle(LED3);
    BSP_LED_Off(LED6);
    BSP_LED_Off(LED4);
  }
  else if (LEDsState == LED4_TOGGLE)
  {
    /* Toggling LED4 */
    BSP_LED_Toggle(LED4);
    BSP_LED_Off(LED6);
    BSP_LED_Off(LED3);
  }
  else if (LEDsState == LED6_TOGGLE)
  {
    /* Toggling LED6 */
    BSP_LED_Off(LED3);
    BSP_LED_Off(LED4);
    BSP_LED_Toggle(LED6);
  }
  else if (LEDsState == STOP_TOGGLE)
  {
    /* Turn ON LED6 */
    BSP_LED_On(LED6);
  }
  else if (LEDsState == LEDS_OFF)
  {
    /* Turn OFF all LEDs */
    BSP_LED_Off(LED3);
    BSP_LED_Off(LED4);
    BSP_LED_Off(LED5);
    BSP_LED_Off(LED6);
  }
  /* Get the TIM4 Input Capture 1 value */
  capture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
  
  /* Set the TIM4 Capture Compare1 Register value */
  __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (CCR1Val + capture));
}

 /**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_0) 
  {
    if (PbPressCheck == 0)
    {
      HAL_Delay(10);
      /* Test on the command: Recording */
      if (CmdIndex == CMD_RECORD)
      {
        RepeatState = REPEAT_ON;
        
        /* Switch to Play command */
        CmdIndex = CMD_PLAY;
      }
      /* Test on the command: Playing */
      else if (CmdIndex == CMD_PLAY)
      {
        /* Switch to Record command */
        CmdIndex = CMD_RECORD;
      }
      else
      {
        RepeatState = REPEAT_ON;
        /* Default Command Index: Play command */
        CmdIndex = CMD_PLAY; 
      }
      PbPressCheck = 1;
    }
    else
    {
      PbPressCheck = 0;
    }
  }
  
  if(GPIO_Pin == GPIO_PIN_1) 
  {
    if (PressCount == 1)
    {
      /* Resume playing Wave status */
      PauseResumeStatus = RESUME_STATUS;
      PressCount = 0;
    }
    else
    {
      /* Pause playing Wave status */
      PauseResumeStatus = PAUSE_STATUS;
      PressCount = 1;
    }
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
#endif

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
