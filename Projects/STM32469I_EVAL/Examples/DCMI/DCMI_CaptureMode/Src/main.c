/**
  ******************************************************************************
  * @file    DCMI/DCMI_CaptureMode/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describe how to configure the camera in continuous mode
  *          and QVGA resolution.
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

/** @addtogroup DCMI_CaptureMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* When value is 1 : Workaround to patch the LTDC pitch if not implemented in LTDC HAL */
/* When define below is 0, its means this is done in HAL LTDC, no patch needed */
#define WORKAROUND_PATCH_LTDC_PITCH  1

#define QVGA_RES_X  320
#define QVGA_RES_Y  240

#define WVGA_RES_X  800
#define WVGA_RES_Y  480

#define ARGB8888_BYTE_PER_PIXEL  4
#define RGB565_BYTE_PER_PIXEL    2

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

LCD_OrientationTypeDef lcd_display_orientation = LCD_ORIENTATION_INVALID;

/* Camera resolution is QVGA (320x240) */
static uint32_t   CameraResX = QVGA_RES_X;
static uint32_t   CameraResY = QVGA_RES_Y;
static uint32_t   LcdResX    = WVGA_RES_X; /* WVGA landscape */
static uint32_t   LcdResY    = WVGA_RES_Y;

static uint32_t          offset_address_area_cam_in_lcd_buffer =  0;

static volatile uint32_t start_the_camera_capture = 0;
static volatile uint32_t cam_frame_buffer_ready = 0;
static uint32_t          loopCnt = 0;

LTDC_Layer_TypeDef * LTDC_Layer1_Dbg =  LTDC_Layer1;
LTDC_Layer_TypeDef * LTDC_Layer2_Dbg =  LTDC_Layer2;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);


static uint8_t CopyCamFrameBufferToLcdFrameBuffer(void *pSrc, void *pDst, uint32_t xSize, uint32_t ySize);
static uint8_t CameraFrameBufferRgb565_Init(uint32_t sizeX, uint32_t sizeY, uint32_t argb8888_Value);

#if (WORKAROUND_PATCH_LTDC_PITCH == 1)
static uint8_t LCD_LTDC_PatchPitch(void);
#endif /* WORKAROUND_PATCH_LTDC_PITCH == 1 */

static void    OnError_Handler(uint32_t condition);

/* Exported functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t  lcd_status = LCD_OK;
  CameraResX = QVGA_RES_X;
  CameraResY = QVGA_RES_Y;
  LcdResX    = WVGA_RES_X; /* WVGA landscape */
  LcdResY    = WVGA_RES_Y;
  loopCnt = 0;
  start_the_camera_capture = 0;
  cam_frame_buffer_ready = 0;
  offset_address_area_cam_in_lcd_buffer =  ((((LcdResY - CameraResY) / 2) * LcdResX)
                                           +   ((LcdResX - CameraResX) / 2))
                                          * ARGB8888_BYTE_PER_PIXEL;

  LTDC_Layer1_Dbg =  LTDC_Layer1;
  LTDC_Layer2_Dbg =  LTDC_Layer2;

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

  /* Wait until MFX is ready after reset */
  HAL_Delay(100);

  /* Initialize MFX */
  BSP_IO_Init();

  /* Initialize used Leds */
  BSP_LED_Init(LED3);

  /*##-1- Initialize the LCD #################################################*/
  //  BSP_LCD_InitEx(LCD_ORIENTATION_PORTRAIT); /* uncomment if Portrait orientation is needed */
  BSP_LCD_Init(); /* Uncomment if default config (landscape orientation) is needed */
  while(lcd_status != LCD_OK);

  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);   
  BSP_LCD_SelectLayer(0);  
  
  if(BSP_LCD_GetXSize() > BSP_LCD_GetYSize())
  {
    lcd_display_orientation = LCD_ORIENTATION_LANDSCAPE;
  }
  else
  {
    lcd_display_orientation = LCD_ORIENTATION_PORTRAIT;
  }

  /* Prepare using DMA2D the 800x480 LCD frame buffer to display background color black */
  /* and title of the example                                                           */
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font16);

  /* Print example description */
  BSP_LCD_DisplayStringAt(0, 440, (uint8_t *)"DCMI Capture example", CENTER_MODE);

  HAL_Delay(100);

#if (WORKAROUND_PATCH_LTDC_PITCH == 1)
  /* Patch the LTDC pitch */
  lcd_status = LCD_LTDC_PatchPitch();
  OnError_Handler(lcd_status != LCD_OK);
#endif /* WORKAROUND_PATCH_LTDC_PITCH == 1 */

  /* Initialize the camera frame buffer 320x240 in RGB565 to a fixed grey pattern */
  lcd_status = CameraFrameBufferRgb565_Init(CameraResX, CameraResY, 0x55555555);
  OnError_Handler(lcd_status != LCD_OK);

  /*##-2- Camera Initialization ############################*/
  /* Initialize the Camera in QVGA mode */
  BSP_CAMERA_Init(CAMERA_R320x240);

  /* Wait 1s to let auto-loops in the camera module converge and lead to correct exposure */
  HAL_Delay(1000);

  /*##-3- Camera Continuous capture start in QVGA resolution ############################*/
  BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);

  /* Wait until effective DMA P2M DCMI capture startup in Camera frame buffer */
  while(start_the_camera_capture == 0) {;}

  while (1)
  {
    loopCnt++;

    /* Wait until camera frame buffer is filled with new picture */
    while (cam_frame_buffer_ready == 0) {       ;    }

    /* Blocking copy Camera Frame buffer to LCD frame buffer area */
    /* Using the DMA2D in polling mode                            */
    lcd_status = CopyCamFrameBufferToLcdFrameBuffer((void*) CAMERA_FRAME_BUFFER,
                                                    (void*) (LCD_FRAME_BUFFER + offset_address_area_cam_in_lcd_buffer),
                                                    CameraResX, CameraResY);
    OnError_Handler(lcd_status != LCD_OK);

    cam_frame_buffer_ready = 0;
  }
}

/**
  * @brief  Camera Frame Event callback.
  */
void BSP_CAMERA_FrameEventCallback(void)
{
  if(start_the_camera_capture == 0)
  {
    /* Wait the first camera end of frame to start capture as DMA is started at that time */
    start_the_camera_capture = 1;
  }

  if((start_the_camera_capture == 1) && (cam_frame_buffer_ready == 0))
  {
    cam_frame_buffer_ready = 1;
  }
}


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Copy and convert Camera frame buffer (CameraResX, CameraResY) of format RGB565
  * to LCD frame buffer area of size (CameraResX, CameraResY) in ARGB8888.
  * @param  pSrc: Pointer to source buffer : Camera frame buffer start here
  * @param  pDst: Pointer to destination buffer LCD frame buffer area start here
  * @param  xSize: Buffer width (CameraResX here)
  * @param  ySize: Buffer height (CameraResY here)
  * @retval LCD Status : LCD_OK or LCD_ERROR
  */
static uint8_t CopyCamFrameBufferToLcdFrameBuffer(void *pSrc, void *pDst, uint32_t xSize, uint32_t ySize)
{
  DMA2D_HandleTypeDef hdma2d_eval;
  HAL_StatusTypeDef hal_status = HAL_OK;
  uint8_t lcd_status = LCD_ERROR;

  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_ARGB8888; /* Output color out of PFC */

  /* Output offset in pixels == nb of pixels to be added at end to come to the  */
  /* first pixel of the next line : on the output side of the DMA2D computation */
  hdma2d_eval.Init.OutputOffset = (LcdResX - CameraResX);

  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF; /* fully opaque */
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;

  hdma2d_eval.Instance = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)pSrc, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        hal_status = HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
        if(hal_status == HAL_OK)
        {
          /* return good status on exit */
          lcd_status = LCD_OK;
        }
      }
    }
  }

  return(lcd_status);
}

/**
  * @brief  Init camera frame buffer with fixed color in format RGB565
  *         to a LCD display frame buffer line in format ARGB8888 using DMA2D service.
  * @param  sizeX: Size in X of rectangular region of the Camera frame buffer to initialize (in pixels unit)
  * @param  sizeX: Size in X of rectangular region of the Camera frame buffer to initialize (in pixels unit)
  * @param  argb_Value : Initialization value (pattern ARGB8888) to be applied to all rectangular region selected.
  * @retval Status LCD_OK or LCD_ERROR
  */
static uint8_t CameraFrameBufferRgb565_Init(uint32_t sizeX, uint32_t sizeY, uint32_t argb8888_Value)
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  uint8_t lcd_status = LCD_ERROR;
  DMA2D_HandleTypeDef hdma2d_eval;

  if((sizeX <= CAMERA_VGA_RES_X) && (sizeY <= CAMERA_VGA_RES_Y))
  {
    /* Register to memory mode with RGB565 as colorMode */
    hdma2d_eval.Init.Mode         = DMA2D_R2M; /* Mode Register to Memory */
    hdma2d_eval.Init.ColorMode    = DMA2D_RGB565; /* Output color mode */
    hdma2d_eval.Init.OutputOffset = 0x0; /* No offset in output */

    hdma2d_eval.Instance = DMA2D;

    hal_status = HAL_DMA2D_Init(&hdma2d_eval);
    if(hal_status == HAL_OK)
    {
        if (HAL_DMA2D_Start(&hdma2d_eval, argb8888_Value, (uint32_t)CAMERA_FRAME_BUFFER, sizeX, sizeY) == HAL_OK)
        {
          /* Polling For DMA transfer */
          HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);

          /* Return good status on exit */
          lcd_status = LCD_OK;
        }
    }
  }

  return (lcd_status);
}

#if (WORKAROUND_PATCH_LTDC_PITCH == 1)
/**
  * @brief  Patch the pitch when not well managed in HAL LTDC.
  * @retval LCD_ERROR or LCD_OK
  */
static uint8_t LCD_LTDC_PatchPitch(void)
{
  uint8_t lcd_status = LCD_ERROR;
  uint32_t pitch_update = ((LcdResX * ARGB8888_BYTE_PER_PIXEL) << 16);

  /* clear the wrong pitch set before by call to HAL_LTDC_ConfigLayer() */
  LTDC_Layer2->CFBLR &= ~(LTDC_LxCFBLR_CFBP);

  /* Sets immediate update of LTDC pitch configured above */
  LTDC->SRCR |= LTDC_SRCR_IMR;

  /* update pitch in bytes : LTDC address increment from one line to the next */
  LTDC_Layer2->CFBLR |= pitch_update;

  /* Sets immediate update of LTDC pitch configured above */
  LTDC->SRCR |= LTDC_SRCR_IMR;

  /* Return good status on exit */
  lcd_status = LCD_OK;

  return(lcd_status);
}
#endif /* WORKAROUND_PATCH_LTDC_PITCH == 1 */

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
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */
