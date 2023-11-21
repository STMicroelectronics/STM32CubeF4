/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_LowPower/Src/main.c
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
#include "cmsis_os.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define blckqSTACK_SIZE   configMINIMAL_STACK_SIZE
#define QUEUE_SIZE        (uint32_t) 1

/* Private variables ---------------------------------------------------------*/
osMessageQId osQueue;

/* The number of items the queue can hold.  This is 1 as the Rx task will
   remove items as they are added so the Tx task should always find the queue
   empty. */
#define QUEUE_LENGTH             (1)

/* The rate at which the Tx task sends to the queue. */
#define TX_DELAY                 (500)

/* The value that is sent from the Tx task to the Rx task on the queue. */
#define QUEUED_VALUE             (100)

/* The length of time the LED will remain on for.  It is on just long enough
   to be able to see with the human eye so as not to distort the power readings too
   much. */
#define LED_TOGGLE_DELAY         (20)

/* Private function prototypes -----------------------------------------------*/
static void QueueReceiveThread(const void *argument);
static void QueueSendThread(const void *argument);
static void GPIO_ConfigAN(void);
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
  
  /* Configure GPIO's to AN to reduce power consumption */
  GPIO_ConfigAN();
  
  /* Configure LED1 */
  BSP_LED_Init(LED1);
  
  /* Create the queue used by the two threads */
  osMessageQDef(osqueue, QUEUE_LENGTH, uint16_t);
  osQueue = osMessageCreate (osMessageQ(osqueue), NULL);
  
  /* Note the Tx has a lower priority than the Rx when the threads are
     spawned. */
  osThreadDef(RxThread, QueueReceiveThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(RxThread), NULL);
  
  osThreadDef(TxThread, QueueSendThread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(TxThread), NULL);
  
  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */
  for(;;);
}

/**
  * @brief  Message Queue Producer Thread.
  * @param  argument: Not used
  * @retval None
  */
static void QueueSendThread (const void *argument)
{
  for(;;)
  {		
    /* Place this thread into the blocked state until it is time to run again.
       The kernel will place the MCU into the Retention low power sleep state
       when the idle thread next runs. */
    osDelay(TX_DELAY);

    /* Send to the queue - causing the queue receive thread to flash its LED.
       It should not be necessary to block on the queue send because the Rx
       thread will already have removed the last queued item. */    
    osMessagePut(osQueue, (uint32_t)QUEUED_VALUE, 0);
  }
}

/**
  * @brief  Message Queue Consumer Thread.
  * @param  argument: Not used
  * @retval None
  */
static void QueueReceiveThread(const void *argument)
{
  osEvent event;
  
  for(;;)
  {
    /* Wait until something arrives in the queue. */
    event = osMessageGet(osQueue, osWaitForever);
    
    /*  To get here something must have arrived, but is it the expected
        value?  If it is, turn the LED on for a short while. */
    if(event.status == osEventMessage)
    {
      if(event.value.v == QUEUED_VALUE)
      {  
        BSP_LED_On(LED1);
        osDelay(LED_TOGGLE_DELAY);
        BSP_LED_Off(LED1);
      }			
    }		
  }
}

/**
  * @brief  MCU Pre Sleep process.
  * @param  ulExpectedIdleTime: Not used
  * @retval None
  */
void PreSleepProcessing(uint32_t ulExpectedIdleTime)
{
  /* Called by the kernel before it places the MCU into a sleep mode because
  configPRE_SLEEP_PROCESSING() is #defined to PreSleepProcessing().
  
  NOTE:  Additional actions can be taken here to get the power consumption
  even lower.  For example, peripherals can be turned	off here, and then back
  on again in the post sleep processing function.  For maximum power saving
  ensure all unused pins are in their lowest power state. */

  /* Avoid compiler warnings about the unused parameter. */
  (void) ulExpectedIdleTime;
  
  /* Disable the peripheral clock during Low Power (Sleep) mode.*/
  __HAL_RCC_GPIOG_CLK_SLEEP_DISABLE();
}

/**
  * @brief  MCU Post Sleep process.
  * @param  ulExpectedIdleTime: Not used
  * @retval None
  */
void PostSleepProcessing(uint32_t ulExpectedIdleTime)
{
  /* Called by the kernel when the MCU exits a sleep mode because
     configPOST_SLEEP_PROCESSING is #defined to PostSleepProcessing(). */
  
  /* Avoid compiler warnings about the unused parameter. */
  (void) ulExpectedIdleTime;
}

/**
  * @brief  Configure all GPIO's to AN to reduce the power consumption
  * @param  None
  * @retval None
  */
static void GPIO_ConfigAN(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct); 
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
  __HAL_RCC_GPIOF_CLK_DISABLE();
  __HAL_RCC_GPIOG_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  __HAL_RCC_GPIOI_CLK_DISABLE();
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
