/**
  ******************************************************************************
  * @file    Display/LCD_AnimatedPictureFromSDCard/Src/main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
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

/** @addtogroup STM32F4xx_HAL_Applications
  * @{
  */

/** @addtogroup LCD_AnimatedPictureFromSDCard
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SD_FatFs;  /* File system object for SD card logical drive */
char SD_Path[4]; /* SD card logical drive path */
DIR directory;
char* pDirectoryFiles[MAX_BMP_FILES];
uint8_t ubNumberOfFiles = 0;
uint32_t uwBmplen = 0;
extern LTDC_HandleTypeDef  hltdc_eval;
extern DSI_HandleTypeDef hdsi_eval;
extern LCD_Driver_TypeDef Lcd_Driver_Type;
DSI_VidCfgTypeDef hdsivideo_handle;

/* Internal Buffer defined in SDRAM memory */
uint8_t *uwInternalBuffer;

/* Private function prototypes -----------------------------------------------*/
static void LCD_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static uint8_t LCD_Init(void);
static LCD_Driver_TypeDef LCD_ReadType(LCD_Driver_TypeDef Lcd_type);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{  
  uint32_t counter = 0;
  uint8_t str[30];
  uwInternalBuffer = (uint8_t *)INTERNAL_BUFFER_START_ADDRESS;

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 200 MHz */
  SystemClock_Config();
  
  /* Configure LED3 */
  BSP_LED_Init(LED3);
  
  /*##-1- Configure LCD ######################################################*/
  LCD_Config();  
  
  /*##-2- Link the SD Card disk I/O driver ###################################*/
  if(FATFS_LinkDriver(&SD_Driver, SD_Path) != 0)
  {
    Error_Handler();
  }
  else
  {
    /*##-3- Initialize the Directory Files pointers (heap) ###################*/
    for (counter = 0; counter < MAX_BMP_FILES; counter++)
    {
      pDirectoryFiles[counter] = malloc(MAX_BMP_FILE_NAME);
      if(pDirectoryFiles[counter] == NULL)
      {
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        
        BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"  Cannot allocate memory ");
        Error_Handler();
        
      }
    }
    
    /*##-4- Display Background picture #######################################*/
    /* Select Background Layer  */
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_BACKGROUND);
    
    /* Register the file system object to the FatFs module */
    if(f_mount(&SD_FatFs, (TCHAR const*)SD_Path, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      /* Set the Text Color */
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      
      BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"  FatFs Initialization Error ");
    }
    else
    {    
      /* Open directory */
      if (f_opendir(&directory, (TCHAR const*)"/BACK") != FR_OK)
      {
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        
        BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    Open directory.. fails");
        Error_Handler();
      }
    }
    
    if (Storage_CheckBitmapFile("BACK/image.bmp", &uwBmplen) == 0)
    {
      /* Format the string */
      Storage_OpenReadFile(uwInternalBuffer, "BACK/image.bmp");
      /* Write bmp file on LCD frame buffer */
      BSP_LCD_DrawBitmap(0, 0, uwInternalBuffer);
    }
    else
    {
      /* Set the Text Color */
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      
      BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    File type not supported. "); 
      Error_Handler();
    }        
    
    /*##-5- Display Foreground picture #######################################*/
    /* Select Foreground Layer  */
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_FOREGROUND);
    
    /* Decrease the foreground transparency */
    BSP_LCD_SetTransparency(LTDC_ACTIVE_LAYER_FOREGROUND, 200); 
    
    /* Get the BMP file names on root directory */
    ubNumberOfFiles = Storage_GetDirectoryBitmapFiles("/TOP", pDirectoryFiles);
    
    if (ubNumberOfFiles == 0)
    {
      for (counter = 0; counter < MAX_BMP_FILES; counter++)
      {
        free(pDirectoryFiles[counter]);
      }
      /* Set the Text Color */
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      
      BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    No Bitmap files...      ");
      Error_Handler();
    } 
  }
  
  HAL_Delay(100);
  BSP_LCD_DisplayOn(); /*turn display On after background initialization */
  
  /* Infinite loop */
  while(1)
  { 
    counter = 0;
    
    while (counter < ubNumberOfFiles)
    {
      /* Format the string */
      sprintf ((char*)str, "TOP/%-11.11s", pDirectoryFiles[counter]);
      
      if (Storage_CheckBitmapFile((const char*)str, &uwBmplen) == 0)
      {
        /* Format the string */
        sprintf ((char*)str, "TOP/%-11.11s", pDirectoryFiles[counter]);
        
        /* Open a file and copy its content to a buffer */
        Storage_OpenReadFile(uwInternalBuffer, (const char*)str);
        
        HAL_Delay(100);
        
        /* Write bmp file on LCD frame buffer */
        BSP_LCD_DrawBitmap(0, 0, uwInternalBuffer);
        
        /* Jump to next image */
        counter++;   
      }
      else
      {
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED); 
        
        BSP_LCD_DisplayStringAtLine(7, (uint8_t *) str);        
        BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    File type not supported. "); 
        Error_Handler();
      }
    }
  }
}

/**
  * @brief  LCD configuration.
  * @param  None
  * @retval None
  */
static void LCD_Config(void)
{
  /* LCD DSI initialization in mode Video Burst  */
  LCD_Init();
  
  BSP_LCD_DisplayOff(); /*turn display Off during the initialization */
  
  /* Activate background layers */
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND, LCD_BG_LAYER_ADDRESS); 
  
  /* Set LCD Background Layer  */
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_BACKGROUND); 
  /* Clear the Background Layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
 
  /* Activate foreground and background layers */  
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_FOREGROUND, LCD_FG_LAYER_ADDRESS);   
  /* Set LCD Foreground Layer  */
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_FOREGROUND);
  /* Clear the Foreground Layer */ 
  BSP_LCD_Clear(LCD_COLOR_BLACK); 

  /* Configure and enable the Color Keying feature */
  BSP_LCD_SetColorKeying(1, 0); 

  /* Configure the transparency for foreground: Increase the transparency */
  BSP_LCD_SetTransparency(LTDC_ACTIVE_LAYER_FOREGROUND, 100);
}

static uint8_t LCD_Init(void)
{
  DSI_PLLInitTypeDef dsiPllInit;
  DSI_PHY_TimerTypeDef  PhyTimings;
  static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  uint32_t LcdClock  = 19200; /*!< LcdClk = 19200 kHz */

  uint32_t laneByteClk_kHz = 0;
  uint32_t                   VSA; /*!< Vertical start active time in units of lines */
  uint32_t                   VBP; /*!< Vertical Back Porch time in units of lines */
  uint32_t                   VFP; /*!< Vertical Front Porch time in units of lines */
  uint32_t                   VACT; /*!< Vertical Active time in units of lines = imageSize Y in pixels to display */
  uint32_t                   HSA; /*!< Horizontal start active time in units of lcdClk */
  uint32_t                   HBP; /*!< Horizontal Back Porch time in units of lcdClk */
  uint32_t                   HFP; /*!< Horizontal Front Porch time in units of lcdClk */
  uint32_t                   HACT; /*!< Horizontal Active time in units of lcdClk = imageSize X in pixels to display */
  
  
  /* Toggle Hardware Reset of the DSI LCD using
  * its XRES signal (active low) */
  BSP_LCD_Reset();
  
  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  BSP_LCD_MspInit();
  
/*************************DSI Initialization***********************************/  
  
  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hdsi_eval.Instance = DSI;
  
  HAL_DSI_DeInit(&(hdsi_eval));
  
#if !defined(USE_STM32469I_DISCO_REVA)
  dsiPllInit.PLLNDIV  = 125;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV2;
  dsiPllInit.PLLODF   = DSI_PLL_OUT_DIV1;
#else  
  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF   = DSI_PLL_OUT_DIV1;
#endif
  laneByteClk_kHz = 62500; /* 500 MHz / 8 = 62.5 MHz = 62500 kHz */
  
  /* Set number of Lanes */
  hdsi_eval.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  
  /* TXEscapeCkdiv = f(LaneByteClk)/15.62 = 4 */
  hdsi_eval.Init.TXEscapeCkdiv = laneByteClk_kHz/15620; 
  
  HAL_DSI_Init(&(hdsi_eval), &(dsiPllInit));
  
  /* Start DSI */
  HAL_DSI_Start(&(hdsi_eval));
  
  /* Enable the DSI BTW for read operations */
  HAL_DSI_ConfigFlowControl(&hdsi_eval, DSI_FLOW_CONTROL_BTA);
  Lcd_Driver_Type = LCD_ReadType(Lcd_Driver_Type);

  BSP_LCD_Reset();
  HAL_DSI_Stop(&hdsi_eval);
  /* The following values are same for portrait and landscape orientations */
  if(Lcd_Driver_Type == LCD_CTRL_NT35510)
  {
    VSA  = NT35510_480X800_VSYNC;
    VBP  = NT35510_480X800_VBP;
    VFP  = NT35510_480X800_VFP;
    HSA  = NT35510_480X800_HSYNC;
    HBP  = NT35510_480X800_HBP;
    HFP  = NT35510_480X800_HFP;
    HACT = NT35510_800X480_WIDTH;
    VACT = NT35510_800X480_HEIGHT;
  }
  else
  {
    VSA  = OTM8009A_480X800_VSYNC;        /* 1 */
    VBP  = OTM8009A_480X800_VBP;          /* 15 */
    VFP  = OTM8009A_480X800_VFP;          /* 16 */
    HSA  = OTM8009A_480X800_HSYNC;        /* 2 */
    HBP  = OTM8009A_480X800_HBP;          /* 20 */
    HFP  = OTM8009A_480X800_HFP;          /* 20 */
    HACT = OTM8009A_800X480_WIDTH;        /* 800 */
    VACT = OTM8009A_800X480_HEIGHT;       /* 480 */
  }

  hdsivideo_handle.VirtualChannelID = LCD_OTM8009A_ID;
  hdsivideo_handle.ColorCoding = LCD_DSI_PIXEL_DATA_FMT_RBG888;
  hdsivideo_handle.VSPolarity = DSI_VSYNC_ACTIVE_HIGH;
  hdsivideo_handle.HSPolarity = DSI_HSYNC_ACTIVE_HIGH;
  hdsivideo_handle.DEPolarity = DSI_DATA_ENABLE_ACTIVE_HIGH;  
  hdsivideo_handle.Mode = DSI_VID_MODE_BURST; /* Mode Video burst ie : one LgP per line */
  hdsivideo_handle.NullPacketSize = 0xFFF;
  hdsivideo_handle.NumberOfChunks = 0;
  hdsivideo_handle.PacketSize                = HACT; /* Value depending on display orientation choice portrait/landscape */ 
  hdsivideo_handle.HorizontalSyncActive      = (HSA * laneByteClk_kHz) / LcdClock;
  hdsivideo_handle.HorizontalBackPorch       = (HBP * laneByteClk_kHz) / LcdClock;
  hdsivideo_handle.HorizontalLine            = ((HACT + HSA + HBP + HFP) * laneByteClk_kHz) / LcdClock; /* Value depending on display orientation choice portrait/landscape */
  hdsivideo_handle.VerticalSyncActive        = VSA;
  hdsivideo_handle.VerticalBackPorch         = VBP;
  hdsivideo_handle.VerticalFrontPorch        = VFP;
  hdsivideo_handle.VerticalActive            = VACT; /* Value depending on display orientation choice portrait/landscape */
  
  /* Enable or disable sending LP command while streaming is active in video mode */
  hdsivideo_handle.LPCommandEnable = DSI_LP_COMMAND_ENABLE; /* Enable sending commands in mode LP (Low Power) */
  
  /* Largest packet size possible to transmit in LP mode in VSA, VBP, VFP regions */
  /* Only useful when sending LP packets is allowed while streaming is active in video mode */
  hdsivideo_handle.LPLargestPacketSize = 16;
  
  /* Largest packet size possible to transmit in LP mode in HFP region during VACT period */
  /* Only useful when sending LP packets is allowed while streaming is active in video mode */
  hdsivideo_handle.LPVACTLargestPacketSize = 0;
  
  
  /* Specify for each region of the video frame, if the transmission of command in LP mode is allowed in this region */
  /* while streaming is active in video mode                                                                         */
  hdsivideo_handle.LPHorizontalFrontPorchEnable = DSI_LP_HFP_ENABLE;   /* Allow sending LP commands during HFP period */
  hdsivideo_handle.LPHorizontalBackPorchEnable  = DSI_LP_HBP_ENABLE;   /* Allow sending LP commands during HBP period */
  hdsivideo_handle.LPVerticalActiveEnable = DSI_LP_VACT_ENABLE;  /* Allow sending LP commands during VACT period */
  hdsivideo_handle.LPVerticalFrontPorchEnable = DSI_LP_VFP_ENABLE;   /* Allow sending LP commands during VFP period */
  hdsivideo_handle.LPVerticalBackPorchEnable = DSI_LP_VBP_ENABLE;   /* Allow sending LP commands during VBP period */
  hdsivideo_handle.LPVerticalSyncActiveEnable = DSI_LP_VSYNC_ENABLE; /* Allow sending LP commands during VSync = VSA period */
  
  /* Configure DSI Video mode timings with settings set above */
  HAL_DSI_ConfigVideoMode(&(hdsi_eval), &(hdsivideo_handle));

  /* Configure DSI PHY HS2LP and LP2HS timings */
  PhyTimings.ClockLaneHS2LPTime = 35;
  PhyTimings.ClockLaneLP2HSTime = 35;
  PhyTimings.DataLaneHS2LPTime = 35;
  PhyTimings.DataLaneLP2HSTime = 35;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 10;
  HAL_DSI_ConfigPhyTimer(&hdsi_eval, &PhyTimings);
  

/*************************End DSI Initialization*******************************/ 
  
  
/************************LTDC Initialization***********************************/  
  
  /* Timing Configuration */    
  hltdc_eval.Init.HorizontalSync = (HSA - 1);
  hltdc_eval.Init.AccumulatedHBP = (HSA + HBP - 1);
  hltdc_eval.Init.AccumulatedActiveW = (HACT + HSA + HBP - 1);
  hltdc_eval.Init.TotalWidth = (HACT + HSA + HBP + HFP - 1);
  
  /* Initialize the LCD pixel width and pixel height */
  hltdc_eval.LayerCfg->ImageWidth  = HACT;
  hltdc_eval.LayerCfg->ImageHeight = VACT;   
  
  
  /* LCD clock configuration */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192 MHz / 5 = 38.4 MHz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 38.4 MHz / 2 = 19.2 MHz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct); 
  
  /* Background value */
  hltdc_eval.Init.Backcolor.Blue = 0;
  hltdc_eval.Init.Backcolor.Green = 0;
  hltdc_eval.Init.Backcolor.Red = 0;
  hltdc_eval.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc_eval.Instance = LTDC;
  
  /* Get LTDC Configuration from DSI Configuration */
  HAL_LTDC_StructInitFromVideoConfig(&(hltdc_eval), &(hdsivideo_handle));
  
  /* Initialize the LTDC */  
  HAL_LTDC_Init(&hltdc_eval);

  /* Enable the DSI host and wrapper after the LTDC initialization
     To avoid any synchronization issue, the DSI shall be started after enabling the LTDC */

  HAL_DSI_Start(&(hdsi_eval));
  
#if !defined(DATA_IN_ExtSDRAM)
  /* Initialize the SDRAM */
  BSP_SDRAM_Init();
#endif /* DATA_IN_ExtSDRAM */
  
  /* Initialize the font */
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
/************************End LTDC Initialization*******************************/
  
  /* Checking the ID to determine the type of component */
  if(Lcd_Driver_Type == LCD_CTRL_NT35510)
  {
    /***********************NT35510 Initialization********************************/ 

    /* Initialize the NT35510 LCD Display IC Driver (3K138 LCD IC Driver)
    *  depending on configuration set in 'hdsivideo_handle'.
    */
    NT35510_Init(NT35510_FORMAT_RGB888, LCD_ORIENTATION_LANDSCAPE);
  }
  /***********************End NT35510 Initialization****************************/
  else if(Lcd_Driver_Type == LCD_CTRL_OTM8009A)
  {
    /***********************OTM8009A Initialization********************************/

    /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
    *  depending on configuration set in 'hdsivideo_handle'.
    */
    OTM8009A_Init(OTM8009A_FORMAT_RGB888, OTM8009A_ORIENTATION_LANDSCAPE);
  }
  /***********************End OTM8009A Initialization****************************/
  else
  {
    return LCD_ERROR;
  }
  return LCD_OK;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  BSP_LCD_DisplayOn();  /*turn display On in case of error to display the error message */
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
  {
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
    Error_Handler();
  }

  /* Activate the OverDrive to reach the 180 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
     Error_Handler();
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
    Error_Handler();
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

/**
  * @brief  Check if the component ID is correct.
  * @param  Lcd_type Driver Type Control NT35510 or OTM8009A
  */
static LCD_Driver_TypeDef LCD_ReadType(LCD_Driver_TypeDef Lcd_type)
{
  uint16_t read_id;
  /* Read the NT35510 ID */
  read_id = NT35510_ReadID();
  if(read_id == NT35510_ID)
  {
    Lcd_type= LCD_CTRL_NT35510;
  }
  else
  {
    /* Read the OTM8009A ID */
    read_id = OTM8009A_ReadID();
    if(read_id == OTM8009A_ID)
    {
      Lcd_type= LCD_CTRL_OTM8009A;
    }
    else
    {
      Lcd_type= LCD_CTRL_NONE;
    }
  }

  return Lcd_type;
}

/**
  * @}
  */

/**
  * @}
  */ 
