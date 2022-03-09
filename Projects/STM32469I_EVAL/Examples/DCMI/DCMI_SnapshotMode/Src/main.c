/**
  ******************************************************************************
  * @file    DCMI/DCMI_SnapshotMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describe how to configure the camera interface (DCMI) in snapshot
  *          mode to handle a single image capture in QVGA (320x240) resolution and RGB565
  *          format and display the obtained image on LCD screen.
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

/** @addtogroup DCMI_SnapshotMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t frame_buffer_ready = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void LCD_LL_ConvertFrameToARGB8888(uint32_t pSrc, uint32_t pDst);
static void OnError_Handler(uint32_t condition);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t  lcd_status = LCD_OK;

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

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();
  
  /* Initialize used Leds */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /*##-1- Initialise the LCD #################################################*/

  /* Initialize and start the LCD display in mode LCD_MODE_ADAPTED_COMMAND_TE_DSI_LINK
   * and orientation mode LCD_ORIENTATION_LANDSCAPE, using LCD_FRAME_BUFFER
   * as LCD frame buffer address.
   */
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);   
  OnError_Handler(lcd_status != LCD_OK);

  /* Prepare using DMA2D the 800x480 LCD frame buffer to display background color black */
  /* and title of the example                                                           */
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font16);

  /* Print example description */
  BSP_LCD_DisplayStringAt(0, 440, (uint8_t *)"DCMI Snapshot example", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 460, (uint8_t *)"Initialize Camera", CENTER_MODE);

  /*##-2- Camera Initialization and start capture ############################*/
  /* Initialize the Camera in QVGA mode */
  BSP_CAMERA_Init(CAMERA_R320x240);

  /* Wait 1s to let auto-loops in the camera module converge and lead to correct exposure */
  HAL_Delay(1000);

  /* Start the Camera Snapshot Capture */
  BSP_CAMERA_SnapshotStart((uint8_t *)CAMERA_FRAME_BUFFER);

  /* Wait until frame buffer is ready */
  while(frame_buffer_ready == 0) {;}
  
  /* Convert captured frame to ARGB8888 */
  LCD_LL_ConvertFrameToARGB8888((uint32_t)CAMERA_FRAME_BUFFER, (uint32_t)LCD_FRAME_BUFFER);

  BSP_LCD_ClearStringLine(460);
  BSP_LCD_DisplayStringAt(0, 460, (uint8_t *)"Capture OK - Test End", CENTER_MODE);
  
  /* Notify Test OK */
  BSP_LED_On(LED1);

  while (1)
  {
  }
}

/**
  * @brief  Camera Frame Event callback.
  */
void BSP_CAMERA_FrameEventCallback(void)
{
  frame_buffer_ready = 1;
}

/**
  * @brief  Converts an RGB565 camera frame buffer into a ARGB8888 LCD Frame buffer.
  * @param  pSrc: Pointer to source buffer in Camera frame buffer 
  * @param  pDst: Pointer to destination buffer in LCD frame buffer 
  * @retval None
  */
static void LCD_LL_ConvertFrameToARGB8888(uint32_t pSrc, uint32_t pDst)
{
  DMA2D_HandleTypeDef hdma2d_eval;
  uint32_t offset_lcd = 0;
  HAL_StatusTypeDef status; 
  
  /* Set display in the middle of the screen */
  offset_lcd =   ((((LcdResY - CameraResY) / 2) * LcdResX)    /* Middle of the screen on Y axis */
                 + ((LcdResX - CameraResX) / 2))              /* Middle of the screen on X axis */
                 * ARGB8888_BYTE_PER_PIXEL;
  
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_ARGB8888;             /* Output color out of PFC */
  hdma2d_eval.Init.OutputOffset = (LcdResX - CameraResX);
  
  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;                  /* fully opaque */
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;
  
  hdma2d_eval.Instance = DMA2D;
  
  /* DMA2D Initialization */
  status = HAL_DMA2D_Init(&hdma2d_eval);
  OnError_Handler(status != HAL_OK);
  
  status = HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1);
  OnError_Handler(status != HAL_OK);
  
  /* Convert frame to ARGB8888 pixels */
  if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)pSrc, (uint32_t)(pDst + offset_lcd), CameraResX, CameraResY) == HAL_OK)
  {
    /* Polling For DMA transfer */
    HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
  }
}

/**
  * @brief  On Error Handler on condition TRUE.
  * @param  condition : Can be TRUE or FALSE
  * @retval None
  */
static void OnError_Handler(uint32_t condition)
{
  if(condition)
  {
    BSP_LED_On(LED3);
    while(1) { ; } /* Blocking on error */
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
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
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
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;
  
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
void assert_failed(uint8_t *file, uint32_t line)
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
