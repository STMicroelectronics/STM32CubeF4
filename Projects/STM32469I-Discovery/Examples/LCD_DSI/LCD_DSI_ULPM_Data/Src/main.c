/**
  ******************************************************************************
  * @file    LCD_DSI/LCD_DSI_ULPM_Data/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to operate the DSI ULPM (Ultra Low Power Mode)
  *          on data lane only in a use case with display in WVGA Landscape
  *          of size (800x480) using the STM32F4xx HAL API and BSP.
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
#include "candies_800x480_argb8888.h"
#include <string.h>
#include <stdio.h>

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup LCD_DSI_ULPM_Data
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
extern DSI_HandleTypeDef hdsi_eval;
extern LTDC_HandleTypeDef  hltdc_eval;
static DMA2D_HandleTypeDef   hdma2d;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t frameCnt = 0;
char str_display[60] = "";

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void OnError_Handler(uint32_t condition);
static uint8_t CheckForUserInput(void);
static void CopyPicture(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize);
/* Private functions ---------------------------------------------------------*/

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
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
  uint8_t  lcd_status = LCD_OK;

  /* This sample code displays a fixed image 800x480 on LCD KoD in */
  /* orientation mode landscape and DSI mode video burst           */

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

  /* Initialize used LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);
  
  /* Configure user push-button */
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_GPIO);
  
  /* Initialize the LCD DSI in Video Burst mode with LANDSCAPE orientation */
  lcd_status = BSP_LCD_Init();
  OnError_Handler(lcd_status != LCD_OK);
  
  /* Program a line event at line 0 */
  HAL_LTDC_ProgramLineEvent(&hltdc_eval, 0);  
  
  /* Copy texture to be displayed on LCD from Flash to SDRAM */
  CopyPicture((uint32_t *)&candies_800x480_argb8888, (uint32_t *)LCD_FB_START_ADDRESS, 0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
  
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND, LCD_FB_START_ADDRESS);
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_BACKGROUND);
  
  /* Prepare area to display frame number in the image displayed on LCD */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 400, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font16);
  
  /* Display title */
  BSP_LCD_DisplayStringAt(0, 420, (uint8_t *) "LCD_DSI_ULPM_Data example", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 440, (uint8_t *) "Press TAMPER button to enter ULPM", CENTER_MODE);
  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font16);
  
  /* Infinite loop */
  while (1)
  {
    /* Clear previous line */
    BSP_LCD_ClearStringLine(460);

    /* New text to display */
    sprintf(str_display, ">> Frame Nb : %lu", frameCnt);

    /* Print updated frame number */
    BSP_LCD_DisplayStringAt(0, 460, (uint8_t *)str_display, CENTER_MODE);

    if (CheckForUserInput() > 0)
    {
      /* Display Off with ULPM management Data lane only integrated */
      BSP_LCD_DisplayOff();
      HAL_Delay(100); 

      /* Switch Off DSI_CR_EN bit */
      __HAL_DSI_DISABLE(&hdsi_eval);
      /* Switch Off LTDCEN bit  */
      __HAL_LTDC_DISABLE(&hltdc_eval); 
      
      /* Enter ultra low power mode (data lane only integrated) */
      HAL_DSI_EnterULPMData(&hdsi_eval);
      BSP_LED_On(LED1);

      HAL_Delay(2000);

      BSP_LCD_ClearStringLine(440);
      BSP_LCD_DisplayStringAt(0, 440, (uint8_t *) " Exited ULPM with success - Press To enter Again ULPM. ", CENTER_MODE);

      /* Exit ultra low power mode (data lane only integrated) */
      HAL_DSI_ExitULPMData(&hdsi_eval);
      BSP_LED_Off(LED1);

      /* Switch On LTDCEN bit */
      __HAL_LTDC_ENABLE(&hltdc_eval);
      /* Switch On DSI_CR_EN bit */ 
      __HAL_DSI_ENABLE(&hdsi_eval);

      /* Display On with ULPM exit Data lane only integrated */
      BSP_LCD_DisplayOn();          
    }
  }
}

/**
 * @brief  Check for user input.
 * @param  None
 * @retval Input state (1 : active / 0 : Inactive)
 */
static uint8_t CheckForUserInput(void)
{
  if(BSP_PB_GetState(BUTTON_WAKEUP) == GPIO_PIN_RESET)
  {
    HAL_Delay(10);
    do
    {
      /* Clear previous line */
      BSP_LCD_ClearStringLine(460);

      /* New text to display */
      sprintf(str_display, ">> Frame Nb : %lu", frameCnt);

      /* Print updated frame number */
      BSP_LCD_DisplayStringAt(0, 460, (uint8_t *)str_display, CENTER_MODE);

    } while (BSP_PB_GetState(BUTTON_WAKEUP) == GPIO_PIN_RESET);

    return 1 ;
  }
  return 0;
}

/**
  * @brief  Line Event callback.
  * @param  hltdc: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc)
{
    /* Increment frame count */
    frameCnt++;
    
    HAL_LTDC_ProgramLineEvent(hltdc, 0);
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
#if defined(USE_STM32469I_DISCO_REVA)
  RCC_OscInitStruct.PLL.PLLM = 25;
#else
  RCC_OscInitStruct.PLL.PLLM = 8;
#endif /* USE_STM32469I_DISCO_REVA */
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


/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color
  * @param  xSize: Buffer width
  * @param  ColorMode: Input color mode   
  * @retval None
  */
static void CopyPicture(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{   
  
  uint32_t destination = (uint32_t)pDst + (y * 800 + x) * 4;
  uint32_t source      = (uint32_t)pSrc;
  
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  hdma2d.Init.Mode         = DMA2D_M2M;
  hdma2d.Init.ColorMode    = DMA2D_ARGB8888;
  hdma2d.Init.OutputOffset = 800 - xsize;     
  
  /*##-2- DMA2D Callbacks Configuration ######################################*/
  hdma2d.XferCpltCallback  = NULL;
  
  /*##-3- Foreground Configuration ###########################################*/
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].InputOffset = 0;

  hdma2d.Instance          = DMA2D; 
   
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&hdma2d, source, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&hdma2d, 100);
      }
    }
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
