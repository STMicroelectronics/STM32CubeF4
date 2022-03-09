/**
  ******************************************************************************
  * @file    LCD_Paint/Src/main.c
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
#include "color.h"
#include "save.h"

/** @addtogroup STM32F4xx_HAL_Applications
  * @{
  */

/** @addtogroup LCD_Paint
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

typedef struct
{
  uint16_t bfType;       /* specifies the file type */
  uint32_t bfSize;       /* specifies the size in bytes of the bitmap file */
  uint16_t bfReserved1;  /* reserved : must be 0 */
  uint16_t bfReserved2;  /* reserved : must be 0 */
  uint32_t bOffBits;     /* species the offset in bytes from the bitmapfileheader to the bitmap bits */
  uint16_t Padding;      /* padding to multiple of 32 bits */
  
} BitMapFileHeader_Typedef;

typedef struct
{
  uint32_t biSize;           /* specifies the number of bytes required by the struct */
  uint32_t biWidth;          /* specifies width in pixels */
  uint32_t biHeight;         /* species height in pixels */
  uint16_t biPlanes;         /* specifies the number of color planes, must be 1 */
  uint16_t biBitCount;       /* specifies the number of bit per pixel */
  uint32_t biCompression;    /* specifies the type of compression */
  uint32_t biSizeImage;      /* size of image in bytes */
  uint32_t biXPelsPerMeter;  /* number of pixels per meter in x axis */
  uint32_t biYPelsPerMeter;  /* number of pixels per meter in y axis */
  uint32_t biClrUsed;        /* number of colors used by the bitmap */
  uint32_t biClrImportant;   /* number of colors that are important */
  
} BitMapFileInfoHeader_Typedef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;   /* File system object for SD card logical drive */
FIL MyFile;      /* File object */
char SDPath[4];  /* SD card logical drive path */
static uint8_t buffer[_MAX_SS]; /* a work buffer for the f_mkfs() */

/* BMP file information to save the drawing pad to file BMP in RGB888 format */
static BitMapFileHeader_Typedef     bmpFileHeader;
static BitMapFileInfoHeader_Typedef bmpFileInfoHeader;

static uint8_t *  p_bmp_converted_pixel_data = (uint8_t *)CONVERTED_FRAME_BUFFER;

static uint32_t Radius = 10;
TS_StateTypeDef  TS_State = {0};
extern LTDC_HandleTypeDef  hltdc_eval;
/* Private function prototypes -----------------------------------------------*/
static void Draw_Menu(void);
static void GetPosition(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static void Save_Picture(void);
static void LTDC_Operation(uint32_t Enable_LTDC);
static void Prepare_Picture(void);
static void Update_Color(void);
static void Update_Size(uint8_t size);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t  lcd_status = LCD_OK;
  uint32_t ts_status  = TS_OK;

  p_bmp_converted_pixel_data = (uint8_t *)CONVERTED_FRAME_BUFFER;

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  /* Init MFX needed by TS, SD detect and CameraPwrDown */
  BSP_IO_Init();

  /* Configure LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /*##-1- LCD Initialization #################################################*/
  /* Initialize and start the LCD display in mode 'lcd_mode'
   *  Using LCD_FB_START_ADDRESS as frame buffer displayed contents.
   *  This buffer is modified by the BSP (draw fonts, objects depending on BSP calls).
   */

  /* Set Portrait orientation if needed, by default orientation is set to
     Landscape */
  
  /* Initialize DSI LCD */
  BSP_LCD_Init(); /* Uncomment if default config (landscape orientation) is needed */
  while(lcd_status != LCD_OK);

  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);   

  /* Clear the LCD Background layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /*##-2- Touch screen initialization ########################################*/
  BSP_TS_ResetTouchData(&TS_State);

  /* If calibration is not yet done, proceed with calibration */
  if(TouchScreen_IsCalibrationDone() == 0)
  {
    ts_status = Touchscreen_Calibration();
    if(ts_status == TS_OK)
    {
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"Touchscreen calibration success.", CENTER_MODE);
    }
  } /* of if (TouchScreen_IsCalibrationDone() == 0) */

  /* ###########################################################################
     When the uSD Card is used; the Camera module must be unplugged, this is due
     to the shared pins between the two devices.

     Otherwise, you have to set camera sensor in Power Down mode, by calling the
     BSP_CAMERA_PwrDown() available under stm32469i_eval_camera.c BSP driver */

  /* Set camera sensor in Power Down mode */
  BSP_CAMERA_PwrDown();

  /*##-3- Link the SD Card disk I/O driver ###################################*/

  /* Clear the LCD and display waiting message */
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 - 27, (uint8_t*)"Please WAIT few seconds", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 - 12, (uint8_t*)"Creating FAT file system on SD card", CENTER_MODE);

  if(FATFS_LinkDriver(&SD_Driver, SDPath) != 0)
  {
    /* FatFs Initialization Error */
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 3, (uint8_t*)"FAT FS Error !!", CENTER_MODE);
    Error_Handler();
  }
  
  /*##-4- Register the file system object to the FatFs module ################*/
  if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
  {
    /* FatFs Initialization Error */
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 3, (uint8_t*)"FAT FS Error !!", CENTER_MODE);
    Error_Handler();
  }
  /* Create a FAT file system (format) on the logical drive */
  if(f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, buffer, sizeof(buffer)) != FR_OK)
  {
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 3, (uint8_t*)"FAT FS Error !!", CENTER_MODE);
    Error_Handler();
  }
  
  /*##-5- Draw the menu ######################################################*/
  Draw_Menu();

  /* Infinite loop */
  while (1)
  {
  /*##-6- Configure the touch screen and Get the position ####################*/
    GetPosition();
  }
}

/**
  * @brief  Configures and gets Touch screen position.
  * @param  None
  * @retval None
  */
static void GetPosition(void)
{
  static uint32_t color_width;
  static uint32_t color;
  uint16_t x, y;

  color_width = 35;

  /* Get Touch screen position */
  BSP_TS_GetState(&TS_State);
  if(TS_State.touchDetected)
  {
    /* A touch occurred, read the touch coordinates */
    /* Get X and Y position of the first touch post calibrated */
    x = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
    y = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);

    if((x > (90 + Radius)) & (y > (7 + Radius) ) & ( x < (BSP_LCD_GetXSize() - (7 + Radius ))) & (y < (BSP_LCD_GetYSize() - (95 + Radius ))))
    {
      BSP_LCD_FillCircle((x), (y), Radius);
    }
    else if((x > 0 ) & ( x < 90 ))
    {
      if((y > 0 ) & (y < color_width ))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        Update_Size(Radius);
      }
      else if((y > color_width ) & (y < (2 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
        Update_Size(Radius);
      }
      else if((y > (2 * color_width)) & (y < (3 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
        Update_Size(Radius);
      }
      else if((y > (3 * color_width)) & (y < (4 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_LIGHTMAGENTA);
        Update_Size(Radius);
      }
      else if((y > (4 * color_width)) & (y < (5 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
        Update_Size(Radius);
      }
      else if((y > (5 * color_width)) &(y < (6 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
        Update_Size(Radius);
      }
      else if((y > (6 * color_width)) &(y < (7 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_BROWN);
        Update_Size(Radius);
      }
      else if((y > (7 * color_width)) & (y < (8 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        Update_Size(Radius);
      }
      else if((y > (8 * color_width)) & (y < (9 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
        Update_Size(Radius);
      }
      else if((y > (9 * color_width)) & (y < (10 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
        Update_Size(Radius);
      }
      else if((y > (10 * color_width)) & (y < (11 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
        Update_Size(Radius);
      }
      else if((y > (11 * color_width)) & (y < (12 * color_width)))
      {
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
        Update_Size(Radius);
      }
      else if((y > (12 * color_width)) & (y < (14 * color_width)))
      {
        /* Get the current text color */
        color = BSP_LCD_GetTextColor();
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        /* Clear the working window */
        BSP_LCD_FillRect(97, 8, 694, 374);
        BSP_LCD_SetTextColor(color);
      }
      else
      {
        x = 0;
        y = 0;
      }
      Update_Color();
    }
    else if((x > 100) & (y > (BSP_LCD_GetYSize() - 90)) & (y < (BSP_LCD_GetYSize()) ) & ( x < 170))
    {
      Radius = 15;
      Update_Size(Radius);
    }
    else if((x > 170) & (y > (BSP_LCD_GetYSize() - 90)) & (y < (BSP_LCD_GetYSize()) ) & ( x < 240))
    {
      Radius = 10;
      Update_Size(Radius);
    }
    else if((x > 240) & (y > (BSP_LCD_GetYSize() - 90)) & (y < (BSP_LCD_GetYSize()) ) & ( x < 310))
    {
      Radius = 5;
      Update_Size(Radius);
    }
    else if(((x > (BSP_LCD_GetXSize()-5) ) & (y > (14 * color_width)) & (y < (15 * color_width))) | ((x < 90) & (y < 5)))
    {
      TS_State.touchX[0] = 0;
      TS_State.touchY[0] = 0;
    }
    else if((x > 320) & (y > (BSP_LCD_GetYSize() - 90)) & (x < 410) & (y < BSP_LCD_GetYSize() - 5))
    {
      Save_Picture();
    }
  }
}

/**
  * @brief  Draws the menu.
  * @param  None
  * @retval None
  */
static void Draw_Menu(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Draw color image */
  BSP_LCD_DrawBitmap(0, 0, (uint8_t *)color);

  /* Draw save image */
  BSP_LCD_DrawBitmap(320, (BSP_LCD_GetYSize() - 90), (uint8_t *)save);

  /* Set Black as text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  /* Draw working window */
  BSP_LCD_DrawRect(91, 0, (BSP_LCD_GetXSize()-91), (BSP_LCD_GetYSize()-90));
  BSP_LCD_DrawRect(93, 3, (BSP_LCD_GetXSize()-96), (BSP_LCD_GetYSize()-96));
  BSP_LCD_DrawRect(95, 5, (BSP_LCD_GetXSize()-100), (BSP_LCD_GetYSize()-100));
  BSP_LCD_DrawRect(97, 7, (BSP_LCD_GetXSize()-104), (BSP_LCD_GetYSize()-104));

  /* Draw size icons */
  BSP_LCD_FillRect(100, (BSP_LCD_GetYSize() - 85), 210, 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(135, (BSP_LCD_GetYSize() - 45), 15);
  BSP_LCD_FillCircle(205, (BSP_LCD_GetYSize() - 45), 10);
  BSP_LCD_FillCircle(275, (BSP_LCD_GetYSize() - 45), 5);

  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  BSP_LCD_SetFont(&Font8);
  BSP_LCD_DisplayStringAt(430, (BSP_LCD_GetYSize() - 75), (uint8_t *)"Selected Color  Size", LEFT_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillRect(450, (BSP_LCD_GetYSize() - 50), 30, 30);
  BSP_LCD_FillCircle(520, (BSP_LCD_GetYSize() - 35), Radius);
}

/**
  * @brief  Saves the picture in microSD.
  * @param  None
  * @retval None
  */
void Save_Picture(void)
{
  FRESULT res1 = FR_OK;
  FRESULT res2 = FR_OK;
  FRESULT res3 = FR_OK;      /* FatFs function common result code */
  uint32_t byteswritten = 0;     /* File write count */
  uint32_t bmpHeaderByteCnt = 0;
  uint32_t bmpFileInfoHeaderByteCnt = 0;
  uint32_t bmpFilePixelBytesCnt = 0;
  static uint32_t counter = 0;
  uint8_t str[30];
  uint16_t tmp_size;

  /* Check if the SD card is plugged in the slot */
  if(BSP_SD_IsDetected() != SD_PRESENT)
  {
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 20, (uint8_t*)"No SD card detected !!", RIGHT_MODE);
    Error_Handler();
  }
  else
  {
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 20, (uint8_t*)"Saving BMP to SD card", RIGHT_MODE);

    /* Format the string */
    sprintf((char *)str, "image_%lu.bmp", counter);

    /* -- Prepare Bitmap file (BMP) header */

    bmpFileHeader.bfType = 0x4D42; /* BMP file type */

    /* Offset in bytes from start of file to first pixel data = size in bytes of complete BMP header          */
    /* careful the structure is padded on multiple of 32 bits by the compiler : the Padding should be removed */
    bmpFileHeader.bOffBits = sizeof(BitMapFileHeader_Typedef) +
                             sizeof(BitMapFileInfoHeader_Typedef) - sizeof(uint32_t) - sizeof(uint16_t);

    /* BMP complete file size is size of pad in RGB888 : 24bpp = 3 bytes per pixel + complete header size */
    bmpFileHeader.bfSize = ((BSP_LCD_GetXSize() - 105) * (BSP_LCD_GetYSize() - 105 + 1) * RGB888_BYTE_PER_PIXEL);
    bmpFileHeader.bfSize += bmpFileHeader.bOffBits;

    bmpFileHeader.bfReserved1 = 0x0000;
    bmpFileHeader.bfReserved2 = 0x0000;

    bmpFileInfoHeader.biSize = 40;       /* 40 bytes in bitmap info header */
    bmpFileInfoHeader.biWidth = (BSP_LCD_GetXSize() - 105);
    bmpFileInfoHeader.biHeight = (BSP_LCD_GetYSize() - 105);
    bmpFileInfoHeader.biPlanes = 1;      /* one single plane */
    bmpFileInfoHeader.biBitCount = 24;   /* RGB888 : 24 bits per pixel */
    bmpFileInfoHeader.biCompression = 0; /* no compression */

    /* This is number of pixel bytes in file : sizeX * (sizeY + padding pixels) * RGB888_BYTE_PER_PIXEL */
    bmpFileInfoHeader.biSizeImage = ((BSP_LCD_GetXSize() - 105) * (BSP_LCD_GetYSize() - 105 + 1) * RGB888_BYTE_PER_PIXEL);

    bmpFileInfoHeader.biXPelsPerMeter = 0; /* not used */
    bmpFileInfoHeader.biYPelsPerMeter = 0; /* not used */
    bmpFileInfoHeader.biClrUsed = 0;       /* not used */
    bmpFileInfoHeader.biClrImportant = 0;  /* not used */

    /* -- End Prepare Bitmap file (BMP) header */

    /*##-1- Prepare the image to be saved ####################################*/
    Prepare_Picture();

    /* Disable the LTDC to avoid charging the bandwidth for nothing while the BMP file is */
    /* written to SD card */
    LTDC_Operation(0);

    /*##-2- Create and Open a new bmp file object with write access ##########*/
    if(f_open(&MyFile, (const char*)str, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
      /* 'image.bmp' file Open for write Error */
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 20, (uint8_t*)"     BMP File Creation Error !!", RIGHT_MODE);
      Error_Handler();
    }
    else
    {
      /*##-3- Write data to the BMP file #####################################*/
      /* Write the BMP header step 1 : first write BMP header : all but padding is written to file */
      res1 = f_write(&MyFile, (uint16_t *)&(bmpFileHeader.bfType),
                     sizeof(uint16_t),
                     (void *)&bmpHeaderByteCnt);
      byteswritten += bmpHeaderByteCnt;
      
      /* LSB of size in bytes of BMP file */
      tmp_size = (uint16_t)(bmpFileHeader.bfSize & 0x0000FFFF);
      res1 = f_write(&MyFile, (uint16_t *)&(tmp_size),
                     sizeof(uint16_t),
                     (void *)&bmpHeaderByteCnt);
      byteswritten += bmpHeaderByteCnt;
      
      /* MSB of size in bytes of BMP file */
      tmp_size = (uint16_t)((bmpFileHeader.bfSize & 0xFFFF0000) >> 16);
      res1 = f_write(&MyFile, (uint16_t *)&(tmp_size),
                     sizeof(uint16_t),
                     (void *)&bmpHeaderByteCnt);
      byteswritten += bmpHeaderByteCnt;
      
      res1 = f_write(&MyFile, (uint16_t *)&(bmpFileHeader.bfReserved1),
                     sizeof(uint16_t),
                     (void *)&bmpHeaderByteCnt);
      byteswritten += bmpHeaderByteCnt;
      
      res1 = f_write(&MyFile, (uint16_t *)&(bmpFileHeader.bfReserved2),
                     sizeof(uint16_t),
                     (void *)&bmpHeaderByteCnt);
      byteswritten += bmpHeaderByteCnt;
      
      res1 = f_write(&MyFile, (uint32_t *)&(bmpFileHeader.bOffBits),
                     sizeof(uint32_t),
                     (void *)&bmpHeaderByteCnt);
      byteswritten += bmpHeaderByteCnt;
      
      if(res1 != FR_OK)
      {
      /* Reactivate LTDC */
        LTDC_Operation(1);
        f_close(&MyFile);
        BSP_LCD_ClearStringLine(BSP_LCD_GetYSize() - 20);
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 20, (uint8_t*)"     BMP File Header Saving Error !!", RIGHT_MODE);
        Error_Handler();
      }

      byteswritten += bmpHeaderByteCnt;

      if(res1 == FR_OK)
      {
        /* Write the BMP header step 2 : second write BMP file info header */
        res2 = f_write(&MyFile, (BitMapFileInfoHeader_Typedef *)&bmpFileInfoHeader,
                       sizeof(BitMapFileInfoHeader_Typedef),
                       (void *)&bmpFileInfoHeaderByteCnt);
        
        if(res2 != FR_OK)
        {
          /* Reactivate LTDC */
          LTDC_Operation(1);
          f_close(&MyFile);
          BSP_LCD_ClearStringLine(BSP_LCD_GetYSize() - 20);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 20, (uint8_t*)"     BMP File Header Info Saving Error !!", RIGHT_MODE);
          Error_Handler();
        }
      }

      byteswritten += bmpFileInfoHeaderByteCnt;

      if((res1 == FR_OK) && (res2 == FR_OK))
      {
        /* Write pixel data in the the BMP file */
        res3 = f_write(&MyFile, (uint8_t *)p_bmp_converted_pixel_data,
                       bmpFileInfoHeader.biSizeImage,
                       (void *)&bmpFilePixelBytesCnt);

        /* Reactivate LTDC */
        LTDC_Operation(1);
        
        if(res3 != FR_OK)
        {
          if(res3 == FR_DISK_ERR)
          {
            f_close(&MyFile);
            BSP_LCD_ClearStringLine(BSP_LCD_GetYSize() - 20);
            BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 20, (uint8_t*)"     File Saving Error DISKERR !!", RIGHT_MODE);
          }

          Error_Handler();
        }
      }

      byteswritten += bmpFilePixelBytesCnt;

      if((res1 != FR_OK) || (res2 != FR_OK) || (res3 != FR_OK) || (byteswritten == 0))
      {
        /* 'image.bmp' file Write or EOF Error */
        BSP_LCD_ClearStringLine(BSP_LCD_GetYSize() - 20);
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 20, (uint8_t*)"     BMP File Saving Error !!", RIGHT_MODE);
        Error_Handler();
      }
      else
      {
        /*##-4- Close the open BMP file ######################################*/
        f_close(&MyFile);
        
        /* Success of the demo: no error occurrence */
        BSP_LED_On(LED1);
        
        BSP_LCD_ClearStringLine(BSP_LCD_GetYSize() - 20);
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 20, (uint8_t*)"             BMP File Saved.", RIGHT_MODE);
        
        /* Wait for 2s */
        HAL_Delay(2000);
        
        BSP_LCD_ClearStringLine(BSP_LCD_GetYSize() - 20);

        /* Select Layer 1 */
        BSP_LED_Off(LED1);
        counter++;
      }
    }
  }
}

/**
  * @brief  Disable/Enable LTDC activity as in DSI Video mode the LTDC is all the time
  *         in active window pumping data with its DMA, it creates a huge bandwidth consumption
  *         that can penalize other IPs, here the save to SD functionality.
  * @param  Enable_LTDC : 0 to disable LTDC, 1 to re-enable LTDC
  * @retval None
  */
static void LTDC_Operation(uint32_t Enable_LTDC)
{
  if(Enable_LTDC == 1)
  {
    __HAL_LTDC_ENABLE(&(hltdc_eval)); /* Switch back On bit LTDCEN */
  }
  else if (Enable_LTDC == 0)
  {
    __HAL_LTDC_DISABLE(&(hltdc_eval)); /* Switch Off bit LTDCEN */
  }
}

/**
  * @brief  Prepares the picture to be saved in microSD.
  * @param  None
  * @retval None
  */
static void Prepare_Picture(void)
{
  const uint32_t x0 = 98;
  const uint32_t x1 = BSP_LCD_GetXSize()- 7;
  const uint32_t y0 = 7;
  const uint32_t y1 = BSP_LCD_GetYSize()- 98;
  uint32_t addrSrc = LCD_FRAME_BUFFER;
  uint32_t addrDst = CONVERTED_FRAME_BUFFER;
  static DMA2D_HandleTypeDef hdma2d_eval;
  uint32_t lineCnt = 0;

  /* Configure the DMA2D Mode, Color Mode and output offset : used to convert ARGB8888 to RGB888 */
  /* used in BMP file format                                                                     */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_RGB888; /* DMA2D Output format */
  hdma2d_eval.Init.OutputOffset = 0;

  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888; /* DMA2D input format */
  hdma2d_eval.LayerCfg[1].InputOffset = 0;

  hdma2d_eval.Instance = DMA2D;
  
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) != HAL_OK)
    {
      Error_Handler();
    }
  }
  else 
  {
    Error_Handler();
  }
  
  /* Go to start of last drawing pad useful line from LCD frame buffer */
  addrSrc += (((y1 * BSP_LCD_GetXSize()) + x0) * ARGB8888_BYTE_PER_PIXEL);
  
  /* Copy and Convert picture from LCD frame buffer in ARGB8888 to Converted frame buffer in
   * RGB888 pixel format for all the useful lines of the drawing pad */
  for(lineCnt = y0; lineCnt <= y1; lineCnt++)
  {
    if (HAL_DMA2D_Start(&hdma2d_eval, addrSrc, addrDst, (x1 - x0), 1) == HAL_OK)
    {
      /* Polling For DMA transfer */
      HAL_DMA2D_PollForTransfer(&hdma2d_eval, 20);
    }

    /* Increment the destination address by one line RGB888, this will add one padding pixel */
    addrDst += ((x1 - x0) * RGB888_BYTE_PER_PIXEL) + RGB888_BYTE_PER_PIXEL;
    
    /* Decrement the source address by one line */
    addrSrc -= (BSP_LCD_GetXSize() * ARGB8888_BYTE_PER_PIXEL);
  }
}

/**
  * @brief  Updates the selected color
  * @param  None
  * @retval None
  */
static void Update_Color(void)
{
  static uint32_t color;

  /* Get the current text color */
  color = BSP_LCD_GetTextColor();

  /* Update the selected color icon */
  BSP_LCD_SetTextColor(color);
  BSP_LCD_FillRect(450, (BSP_LCD_GetYSize() - 50), 30, 30);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawRect(450, (BSP_LCD_GetYSize() - 50), 30, 30);
  BSP_LCD_SetTextColor(color);
}

/**
  * @brief  Updates the selected size
  * @param  size: Size to be updated
  * @retval None
  */
static void Update_Size(uint8_t size)
{
  static uint32_t color;

  /* Get the current text color */
  color = BSP_LCD_GetTextColor();
  
  /* Update the selected size icon */
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(520, (BSP_LCD_GetYSize() - 35), 20);
  BSP_LCD_SetTextColor(color);
  BSP_LCD_FillCircle(520, (BSP_LCD_GetYSize() - 35), size);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawCircle(520, (BSP_LCD_GetYSize() - 35), size);
  BSP_LCD_SetTextColor(color);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
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
