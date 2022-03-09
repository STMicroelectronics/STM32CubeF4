/**
  ******************************************************************************
  * @file    FatFs/FatFs_uSD_RTOS/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body
  *          This sample code shows how to use FatFs with uSD card drive in RTOS
  *          mode.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
char SDPath[4]; /* SD card logical drive path */

typedef enum {
  CARD_CONNECTED,
  CARD_DISCONNECTED,
}SD_ConnectionStateTypeDef;

osMessageQId ConnectionEvent;

static uint8_t isInitialized = 0;
static uint8_t isFsCreated = 0;
uint8_t workBuffer[2 * _MAX_SS];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void FS_AppThread(void const *argument);
static void FS_FileOperations(void);
static void SD_Initialize(void);

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

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  /* Initialize IO expander */
  BSP_IO_Init();

  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* ###########################################################################
   When the uSD Card is used; the Camera module must be unplugged, this is due
   to the shared pins between the two devices.

   Otherwise, you have to set camera sensor in Power Down mode, by calling the
   BSP_CAMERA_PwrDown() available under stm32446e_eval_camera.c BSP driver */

   /* Assert the camera RSTI pin */
   /* Camera power down sequence */
   BSP_IO_ConfigPin(RSTI_PIN, IO_MODE_OUTPUT);
   /* Assert the camera RSTI pin (active low) */
   BSP_IO_WritePin(RSTI_PIN, BSP_IO_PIN_RESET);

  /*##-1- Link the micro SD disk I/O driver And Create RTOS environment ######*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {

    SD_Initialize();
    osThreadDef(uSDThread, FS_AppThread, osPriorityNormal, 0, 8 * configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(uSDThread), NULL);

    /* Create Storage Message Queue */
    osMessageQDef(osqueue, 10, uint16_t);
    ConnectionEvent = osMessageCreate (osMessageQ(osqueue), NULL);

 }
  /*##-2- Start scheduler ####################################################*/
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}

/**
  * @brief  Start task
  * @param  pvParameters not used
  * @retval None
  */
static void FS_AppThread(void const *argument)
{
  osEvent event;

  if(BSP_SD_IsDetected())
  {
    osMessagePut ( ConnectionEvent, CARD_CONNECTED, osWaitForever);
  }
  /* Infinite Loop */
  for( ;; )
  {
    event = osMessageGet( ConnectionEvent, osWaitForever );

    if( event.status == osEventMessage )
    {
      switch(event.value.v)
      {
      case CARD_CONNECTED:
        BSP_LED_Off(LED3);
        SD_Initialize();
        FS_FileOperations();
        break;

      case CARD_DISCONNECTED:
        BSP_LED_Off(LED1);
        BSP_LED_On(LED3);

        if(isInitialized == 1)
        {
          isInitialized = 0;
          f_mount(NULL, (TCHAR const*)"", 0);
        }
        break;
      }
    }
  }
}

/**
  * @brief  Main routine for Mass Storage Class
  * @param  None
  * @retval None
  */
static void FS_FileOperations(void)
{
  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */
  uint8_t wtext[] = "This is STM32 working with FatFs uSD + FreeRTOS"; /* File write buffer */
  uint8_t rtext[100];                                   /* File read buffer */

  /* Register the file system object to the FatFs module */
  if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) == FR_OK)
  {
    /* check whether the FS has been already created */
    if (isFsCreated == 0)
    {
      if(f_mkfs(SDPath, FM_ANY, 0, workBuffer, sizeof(workBuffer)) != FR_OK)
      {
        BSP_LED_On(LED3);
        return;
      }
      isFsCreated = 1;
    }
    /* Create and Open a new text file object with write access */
    if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
    {
      /* Write data to the text file */
      res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);

      if((byteswritten > 0) && (res == FR_OK))
      {
        /* Close the open text file */
        f_close(&MyFile);

        /* Open the text file object with read access */
        if(f_open(&MyFile, "STM32.TXT", FA_READ) == FR_OK)
        {
          /* Read data from the text file */
          res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);

          if((bytesread > 0) && (res == FR_OK))
          {
            /* Close the open text file */
            f_close(&MyFile);

            /* Compare read data with the expected data */
            if((bytesread == byteswritten))
            {
              /* Success of the demo: no error occurrence */
              BSP_LED_On(LED1);
              return;
            }
          }
        }
      }
    }
  }
  /* Error */
  BSP_LED_On(LED3);
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(MFX_IRQOUT_PIN == GPIO_Pin)
  {
    if(BSP_SD_IsDetected())
    {
      osMessagePut ( ConnectionEvent, CARD_CONNECTED, osWaitForever);
    }
    else
    {
      BSP_LED_Off(LED1);
      BSP_LED_Off(LED3);
      osMessagePut ( ConnectionEvent, CARD_DISCONNECTED, osWaitForever);
    }
  }
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
  *         The SD clock configuration from PLLSAI:
  *            PLLSAIM                        = 8
  *            PLLSAIN                        = 384
  *            PLLSAIP                        = 8
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInitStruct;
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
    Error_Handler();
 }
  /* activate the OverDrive to reach the 180 Mhz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

  /* generate 48Mhz for SD card clock */
  RCC_PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDIO | RCC_PERIPHCLK_CK48;
  RCC_PeriphClkInitStruct.SdioClockSelection = RCC_SDIOCLKSOURCE_CK48;
  RCC_PeriphClkInitStruct.Clk48ClockSelection = RCC_CK48CLKSOURCE_PLLSAIP;
  RCC_PeriphClkInitStruct.PLLSAI.PLLSAIM = 8;
  RCC_PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  RCC_PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }
}

static void SD_Initialize(void)
{
  if (isInitialized == 0)
  {
    if (BSP_SD_Init() == MSD_OK)
    {
      BSP_SD_ITConfig();
      isInitialized = 1;
    }
    else
    {
      BSP_LED_On(LED3);
      /* wait until the uSD is plugged */
      while (BSP_SD_IsDetected() != SD_PRESENT)
      {}
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
  BSP_LED_Off(LED1);
  BSP_LED_On(LED3);
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
