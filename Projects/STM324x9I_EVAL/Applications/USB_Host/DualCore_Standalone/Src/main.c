/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/Src/main.c
  * @author  MCD Application Team
  * @brief   USB host DUALCORE demo main file
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
/* Includes ------------------------------------------------------------------ */
#include "main.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
USBH_HandleTypeDef hUSBHost_FS;
USBH_HandleTypeDef hUSBHost_HS;
DUAL_ApplicationTypeDef Appli_FS_state = APPLICATION_IDLE;
DUAL_ApplicationTypeDef Appli_HS_state = APPLICATION_IDLE;
char USBDISKPath[4];            /* USB Host logical drive path */

/* Private function prototypes ----------------------------------------------- */
static void SystemClock_Config(void);
static void USBH_HS_UserProcess(USBH_HandleTypeDef * phost, uint8_t id);
static void USBH_FS_UserProcess(USBH_HandleTypeDef * phost, uint8_t id);
static void DUAL_InitApplication(void);

/* Private functions --------------------------------------------------------- */

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization: - Configure the Flash prefetch,
   * instruction and Data caches - Configure the Systick to generate an
   * interrupt each 1 msec - Set NVIC Group Priority to 4 - Global MSP (MCU
   * Support Package) initialization */
  HAL_Init();

  /* Configure the system clock to 168 MHz */
  SystemClock_Config();

  /* Init Dual Core Application */
  DUAL_InitApplication();

  /* Init HS Core */
  USBH_Init(&hUSBHost_HS, USBH_HS_UserProcess, 1);

  /* Init FS Core */
  USBH_Init(&hUSBHost_FS, USBH_FS_UserProcess, 0);

  /* Add Supported Classes */
  USBH_RegisterClass(&hUSBHost_HS, USBH_MSC_CLASS);
  USBH_RegisterClass(&hUSBHost_FS, USBH_HID_CLASS);

  /* Start Host Process */
  USBH_Start(&hUSBHost_FS);
  USBH_Start(&hUSBHost_HS);

  /* Run Application (Blocking mode) */
  while (1)
  {
    /* USB Host Background tasks */
    USBH_Process(&hUSBHost_FS);
    USBH_Process(&hUSBHost_HS);

    /* DUAL Menu Process */
    DUAL_MenuProcess();
  }
}

/**
  * @brief  DUALCORE application Init.
  * @param  None
  * @retval None
  */
static void DUAL_InitApplication(void)
{
  /* Configure Key Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

  /* Configure Joystick in EXTI mode */
  BSP_JOY_Init(JOY_MODE_EXTI);

  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Initialize the LCD */
  BSP_LCD_Init();

  /* LCD Layer Initialization */
  BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS);

  /* Selects the LCD Layer */
  BSP_LCD_SelectLayer(1);

  /* Enables the display */
  BSP_LCD_DisplayOn();

  /* Init the LCD Log module */
  LCD_LOG_Init();

  LCD_LOG_SetHeader((uint8_t *) " USB OTG DualCore Host");

  LCD_UsrLog("USB Host library started.\n");

  /* Start DualCore Interface */
  USBH_UsrLog("Initializing hardware....");
  DUAL_MenuInit();
}

/**
  * @brief  User Process
  * @param  phost: Host FS Handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_FS_UserProcess(USBH_HandleTypeDef * phost, uint8_t id)
{
  switch (id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
    Appli_FS_state = APPLICATION_FS_DISCONNECT;

    break;

  case HOST_USER_CLASS_ACTIVE:
    Appli_FS_state = APPLICATION_FS_READY;
    break;

  case HOST_USER_CONNECTION:
    Appli_FS_state = APPLICATION_FS_START;
    break;
  }
}

/**
  * @brief  User Process
  * @param  phost: Host HS Handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_HS_UserProcess(USBH_HandleTypeDef * phost, uint8_t id)
{
  switch (id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
    Appli_HS_state = APPLICATION_HS_DISCONNECT;
    if(f_mount(NULL, "", 0) != FR_OK)
    {
      LCD_ErrLog("ERROR : Cannot DeInitialize FatFs! \n");
    }
    if (FATFS_UnLinkDriver(USBDISKPath) != 0)
    {
      LCD_ErrLog("ERROR : Cannot UnLink FatFS Driver! \n");
    }
    break;

  case HOST_USER_CLASS_ACTIVE:
    Appli_HS_state = APPLICATION_HS_READY;
    break;

  case HOST_USER_CONNECTION:
    Appli_HS_state = APPLICATION_HS_START;
    if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
    {
      if (f_mount(&USBH_fatfs, "", 0) != FR_OK)
      {
        LCD_ErrLog("ERROR : Cannot Initialize FatFs! \n");
      }
    }
    break;
  }
}

/**
  * @brief  Toggles LEDs to show user input state.
  * @param  None
  * @retval None
  */
void Toggle_Leds(void)
{
  static uint32_t ticks;

  if (ticks++ == 100)
  {
    BSP_LED_Toggle(LED1);
    BSP_LED_Toggle(LED2);
    BSP_LED_Toggle(LED3);
    BSP_LED_Toggle(LED4);
    ticks = 0;
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

  /* The voltage scaling allows optimizing the power consumption when the
   * device is clocked below the maximum system frequency, to update the
   * voltage scaling value regarding system frequency refer to product
   * datasheet.  */
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
   * clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t * file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
   * number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
   * line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
