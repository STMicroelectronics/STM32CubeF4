/**
  ******************************************************************************
  * @file    camera_app.c
  * @author  MCD Application Team
  * @brief   Camera application functions source file
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
#include "camera_app.h"
#include "main.h"
#include "k_bsp.h"
#include "k_rtc.h"
#include "k_storage.h"

/** @addtogroup CAMERA_MODULE
  * @{
  */

/** @defgroup CAMERA_APPLICATION
 * @brief camera application routines
 * @{
 */

/* External variables --------------------------------------------------------*/
extern CameraSettingsTypeDef CameraSettings;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define BMP_PIXEL16_TO_R(pixel)       ((pixel & 0x1F) << 3)
#define BMP_PIXEL16_TO_G(pixel)       (((pixel >> 5) & 0x3F) << 2)
#define BMP_PIXEL16_TO_B(pixel)       (((pixel >> 11) & 0x1F) << 3)

/* Private variables ---------------------------------------------------------*/
const uint8_t BMPHeader_QQVGA24Bit[] =
  {
    0x42, 0x4D,              /* Offet0: BMP Magic Number */
    0x36, 0xE1, 0x00, 0x00,  /* Offset2 : filesz : Size of the BMP file 120*160*3 + 54 */
    0x00, 0x00, 0x00, 0x00,  /* Offset6, Offset8 : Reserved0, Reserved1 =0 */
    0x36, 0x00, 0x00, 0x00,  /* Offset10 : bmp_offset: Offset of bitmap data (pixels)  = 54 = 0x36 */
    0x28, 0x00, 0x00, 0x00,  /* Offset14 : header_sz : The number of bytes in the header (from this point). */
    0xA0, 0x00, 0x00, 0x00,  /* Offset18 : width 320 */
    0x78, 0x00, 0x00, 0x00,  /* Offset2 : height 240 */
    0x01, 0x00,              /* Offset26 : nplanes */
    0x18, 0x00,              /* Offset24: Bits per Pixels */
    0x00, 0x00, 0x00, 0x00,  /* Offset30: compress_type = 0  */
    0x00, 0x58, 0x02, 0x00,  /* Offset34 : bmp bytes size*/
    0x00, 0x00, 0x00, 0x00,  /* Offset38 : X Resolution : Pixel per meters = 0 */
    0x00, 0x00, 0x00, 0x00,  /* Offset42 : Y Resolution : Pixel per meters */
    0x00, 0x00, 0x00, 0x00,  /* Offset46 : Number of Colours = 0 */
    0x00, 0x00, 0x00, 0x00,  /* Offset50 : Important Colours = 0 */
  };

uint32_t CameraError = CAMERA_ERROR;

/* Private function prototypes -----------------------------------------------*/
static void RGB16toRGB24(uint8_t *pDestBuffer, uint8_t *pSrcBuffer);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Set Contrast and brightness levels.
  * @param  contrast_level: Contrast level
  * @param  brightness_level: Brightness level
  * @retval None
  */
void CAMERA_Set_ContrastBrightness(uint32_t contrast_Level, uint32_t brightness_level)
{
  uint32_t contrast = 0, brithness = 0;
  
  switch (contrast_Level)
  {
  case 1:
    contrast = CAMERA_CONTRAST_LEVEL0;
    break;
  case 2:
    contrast = CAMERA_CONTRAST_LEVEL1;
    break;
  case 3:
    contrast = CAMERA_CONTRAST_LEVEL2;
    break;
  case 4:
    contrast = CAMERA_CONTRAST_LEVEL3;
    break;
  case 5:
    contrast = CAMERA_CONTRAST_LEVEL4;
    break;
  }
  switch (brightness_level)
  {
  case 1:
    brithness = CAMERA_BRIGHTNESS_LEVEL0;
    break;
  case 2:
    brithness = CAMERA_BRIGHTNESS_LEVEL1;
    break;
  case 3:
    brithness = CAMERA_BRIGHTNESS_LEVEL2;
    break;
  case 4:
    brithness = CAMERA_BRIGHTNESS_LEVEL3;
    break;
  case 5:
    brithness = CAMERA_BRIGHTNESS_LEVEL4;
    break;
  } 
  BSP_CAMERA_ContrastBrightnessConfig(contrast, brithness);
}

/**
  * @brief  Select effect.
  * @param  effect: Camera effect.
  * @retval None
  */
void CAMERA_SelectEffect(uint32_t effect)
{
  switch (effect)
  {
  case 0:
    CAMERA_Set_ContrastBrightness(CameraSettings.b.contrast, CameraSettings.b.brightness);
    break;
  case 1:
    BSP_CAMERA_BlackWhiteConfig(CAMERA_BLACK_WHITE_BW);
    break;
  case 2:
    BSP_CAMERA_BlackWhiteConfig(CAMERA_BLACK_WHITE_NEGATIVE);
    break;
  case 3:
    BSP_CAMERA_ColorEffectConfig(CAMERA_COLOR_EFFECT_ANTIQUE);
    break;
  case 4:
    BSP_CAMERA_ColorEffectConfig(CAMERA_COLOR_EFFECT_BLUE);
    break;
  case 5:
    BSP_CAMERA_ColorEffectConfig(CAMERA_COLOR_EFFECT_GREEN);
    break;
  case 6:
    BSP_CAMERA_ColorEffectConfig(CAMERA_COLOR_EFFECT_RED);
    break;    
  }
}

/**
  * @brief  Initialize the camera module.
  * @param  None
  * @retval None
  */
void CAMERA_Init(void)
{  
  /* Initialize the Camera */
  CameraError = BSP_CAMERA_Init(RESOLUTION_R160x120);
  if( CameraError != CAMERA_ERROR)
  {
    
    GUI_Delay(100);
    /* Start the capture */
    BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);
    
    CAMERA_Set_ContrastBrightness(CameraSettings.b.contrast, CameraSettings.b.brightness);
  }
}

/**
  * @brief  Suspend the camera capture.
  * @param  None
  * @retval None
  */
void CAMERA_Suspend(void)
{
  if( CameraError != CAMERA_ERROR)
  {  
    /* Suspend the CAMERA capture */
    BSP_CAMERA_Suspend();
  }
}


/**
  * @brief  Resume the camera capture.
  * @param  None
  * @retval None
  */
void CAMERA_Resume(void)
{
  if( CameraError != CAMERA_ERROR)
  {
    /* Resume the CAMERA capture */
    BSP_CAMERA_Resume();
  }
}


/**
  * @brief  Stop the camera capture.
  * @param  None
  * @retval None
  */
void CAMERA_Stop(void)
{
  if( CameraError != CAMERA_ERROR)
  {  
    /* Disable Camera request and Disable DCMI capture */
    BSP_CAMERA_Stop(); 
  }
}


/**
  * @brief  Return Camera State
  * @param  None
  * @retval Camera State
  */
uint8_t CAMERA_GetState(void)
{
  return CameraError;
}

/**
  * @brief  Camera error callback.
  * @param  None
  * @retval None
  */
void BSP_CAMERA_ErrorCallback(void)
{
  if( CameraError != CAMERA_ERROR)
  {
    BSP_CAMERA_Resume();
  }
}

/**
  * @brief  Save the data to specified file.
  * @param  path: pointer to the saving path
  * @retval File saved
  */
uint8_t  CAMERA_SaveToFile(uint8_t *path)
{
  RTC_TimeTypeDef   Time;
  RTC_DateTypeDef   Date;
  FIL               file;
  uint32_t  NumWrittenData;
  uint8_t ret = 1;
  
  char filename[FILEMGR_FILE_NAME_SIZE];
  char fullpath[FILEMGR_FILE_NAME_SIZE];
  
    /* Create filename */
    k_GetTime(&Time);
    k_GetDate(&Date);
    sprintf((char *)filename, "/Camera_%02d%02d%04d_%02d%02d%02d.bmp", 
            Date.Date,
            Date.Month,
            Date.Year + 2015,
            Time.Hours,
            Time.Minutes,
            Time.Seconds);
    strcpy((char *)fullpath, (char *)path);
    strcat ((char *)fullpath, (char *)filename);
    
    BSP_CAMERA_Suspend();
    /* Can not create file */
    if (f_open(&file, (char *)fullpath, FA_CREATE_NEW | FA_WRITE) == FR_OK)
    {
      /* Write the received data into the file */
      if (f_write(&file, (char *)BMPHeader_QQVGA24Bit, RGB_HEADER_SIZE, (UINT *)&NumWrittenData) == FR_OK)
      {
        f_sync(&file);
        /* Convert RGB16 image to RGB24 */
        RGB16toRGB24((uint8_t *)CAMERA_CVRT_BUFFER, (uint8_t *)CAMERA_FRAME_BUFFER);
        
        if (f_write(&file, (char *)CAMERA_CVRT_BUFFER, MAX_IMAGE_SIZE, (UINT*)&NumWrittenData)== FR_OK)
        {
          /*File Written correctly */
          ret = 0;
        }
        
      }
      f_close(&file);
    }
    
    BSP_CAMERA_Resume();
  return ret;
}

/**
  * @brief  Convert RGB16 image to RGB24.
  * @param  pDestBuffer: buffer hold the data in RGB24 format
  * @param  pSrcBuffer: buffer hold the data in RGB16 format    
  * @retval None
  */
static void RGB16toRGB24(uint8_t *pDestBuffer, uint8_t *pSrcBuffer)
{
  uint16_t *pSrc;
  uint8_t *pDest;
  uint32_t i = 0, j = 0;
  uint16_t value;
  
  pSrc = (uint16_t*) & pSrcBuffer[IMAGE_BUFFER_SIZE] - 1;
  pDest = (uint8_t*) & pDestBuffer[0];
  
  for (i = IMAGE_COLUMN_SIZE; i > 0; i-- )
  {
    for ( j = 0; j < 2 * IMAGE_LINE_SIZE;  j += 2 )
    {
      value    = (uint16_t) * pSrc;
      
      *pDest++   = BMP_PIXEL16_TO_R(value);
      *pDest++   = BMP_PIXEL16_TO_G(value);
      *pDest++   = BMP_PIXEL16_TO_B(value);
      pSrc--;
    }
  }
}

/**
* @}
*/ 

/**
* @}
*/
