/**
  ******************************************************************************
  * @file    BSP/Src/camera.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the camera feature in the
  *          stm32446e_eval driver
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
#include <stdio.h>
#include "stm32446e_eval_audio.h"


/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Camera Frame Buffer address */
#define CAMERA_FRAME_BUFFER       SDRAM_WRITE_READ_ADDR  

#define CAMERA_STATUS_TEXT_POS    30       /* Number of lines from bottom of screen */

#define CAMERA_QVGA_RES_X         320
#define CAMERA_QVGA_RES_Y         240
#define CAMERA_QQVGA_RES_X        160
#define CAMERA_QQVGA_RES_Y        120

#define CAMERA_RES_INDEX_MIN      CAMERA_R160x120
#define CAMERA_RES_INDEX_MAX      CAMERA_R320x240

#define CAMERA_CONTRAST_MIN       CAMERA_CONTRAST_LEVEL0
#define CAMERA_CONTRAST_MAX       CAMERA_CONTRAST_LEVEL4
#define CAMERA_BRIGHTNESS_MIN     CAMERA_BRIGHTNESS_LEVEL0
#define CAMERA_BRIGHTNESS_MAX     CAMERA_BRIGHTNESS_LEVEL4

#define CAMERA_FRAME_BUFFER_CROP  (SDRAM_WRITE_READ_ADDR + (CAMERA_QVGA_RES_X * CAMERA_QVGA_RES_Y * 2))

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint32_t   CameraResIndex;
static uint32_t   CameraChangeResolutionAsked;


/* Private function prototypes -----------------------------------------------*/
static void Camera_SetHint(void);
void BSP_CAMERA_FrameEventCallback(void);
void Camera_DisplayFrame(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Camera demo
  * @param  None
  * @retval None
  */
void Camera_demo (void)
{ 
  uint8_t status = 0;
  uint16_t  *ptrCamera;
  uint32_t  i;
  uint32_t  exit = 0;

  Camera_SetHint();

  status = BSP_JOY_Init(JOY_MODE_GPIO);

  /* Initialize the SDRAM for CAMERA_FRAME_BUFFER */
  BSP_SDRAM_Init();

  if (status != IO_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- CAMERA_STATUS_TEXT_POS, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- CAMERA_STATUS_TEXT_POS - 15, (uint8_t *)"Joystick cannot be initialized", CENTER_MODE);
    status = 0;
  }

  /*## Camera Initialization and start capture ############################*/

  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- CAMERA_STATUS_TEXT_POS, (uint8_t *)"Wait for camera initialization...", CENTER_MODE);

  /* Init Camera buffer */
  ptrCamera = (uint16_t*)(CAMERA_FRAME_BUFFER);
  CameraResIndex = CAMERA_R160x120;     /* Set QQVGA default resolution */
  CameraChangeResolutionAsked = 1;
  for (i=0; i<(CAMERA_QVGA_RES_X * CAMERA_QVGA_RES_Y); i++)   /* Camera buffer initialization */
  {
    ptrCamera[i]=0xFFFF;
  }

  /* Initialize IO */
  BSP_IO_Init();

  /* Infinite loop */
  while(exit == 0)
  {
    if (CameraChangeResolutionAsked)  /* A camera change resolution has been asked */
    {
      CameraChangeResolutionAsked = 0;
      if (CameraResIndex > CAMERA_R160x120)
      { /* If camera stream already started, it has to be stopped before reconfiguration */
        /* Stop camera stream */
        status = BSP_CAMERA_Stop();
        ASSERT(status != CAMERA_OK);

        /* Clear screen */
        Camera_SetHint();

        BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- CAMERA_STATUS_TEXT_POS, (uint8_t *)"Wait for camera initialization...", CENTER_MODE);
      }
 
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

      status = 0;

      switch (CameraResIndex)
      {
        case CAMERA_R160x120:
          /* Initialize the Camera */
          status = BSP_CAMERA_Init(RESOLUTION_R160x120);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- CAMERA_STATUS_TEXT_POS, (uint8_t *)"       QQVGA camera stream       ", CENTER_MODE);
          break;

        case CAMERA_R320x240:
          /* Initialize the Camera */
          status = BSP_CAMERA_Init(RESOLUTION_R320x240);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- CAMERA_STATUS_TEXT_POS, (uint8_t *)"       QVGA camera stream        ", CENTER_MODE);
          break;
      }

      if (status != CAMERA_OK)
      {
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- CAMERA_STATUS_TEXT_POS - 15, (uint8_t *)"        CAMERA INIT ERROR        ", CENTER_MODE);
      }


      /* Start / Restart camera stream */
      BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);
    }  /* (CameraResIndex != CameraCurrentResIndex) */


    /* Toggle LED4 */
    BSP_LED_Toggle(LED3);

    /* Insert 100 ms delay */
    HAL_Delay(100);

    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);

    /* Insert 100 ms delay */
    HAL_Delay(100);
    if(CheckForUserInput() > 0)
    {
      if (CameraResIndex < CAMERA_RES_INDEX_MAX)
      {
        CameraResIndex++;     /* Switch to higher resolution */
        CameraChangeResolutionAsked = 1;
      }
      else
      {
        BSP_CAMERA_DeInit();
        exit = 1;
      }
    }
  }

  /* Stop camera stream */
  status = BSP_CAMERA_Stop();
  ASSERT(status != CAMERA_OK);

  /* End of camera demo */
}

/**
  * @brief  Display Camera demo hint
  * @param  None
  * @retval None
  */
static void Camera_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set Camera Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 90);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"CAMERA EXAMPLE", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"     Press User button for next resolution     ", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 100, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 110);
  BSP_LCD_DrawRect(11, 101, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 112);
}

/**
  * @brief  Frame Event callback.
  * @param  None
  * @retval None
*/
void BSP_CAMERA_FrameEventCallback(void)
{
  uint32_t xsizeLCD, ysizeLCD;
  uint32_t xsizeCamera, ysizeCamera;
  uint32_t xoffset, yoffset;
  uint32_t line, column, i, j;
  uint16_t *ptrCamera, *ptrCameraCrop;

  xsizeLCD =  BSP_LCD_GetXSize();
  ysizeLCD =  BSP_LCD_GetYSize();

  BSP_CAMERA_Suspend();
  /* Display on LCD */
  switch (CameraResIndex)
  {
    case CAMERA_R160x120:
      xsizeCamera = CAMERA_QQVGA_RES_X;
      ysizeCamera = CAMERA_QQVGA_RES_Y;
      xoffset = 80;
      yoffset = 105;
      break;

    case CAMERA_R320x240:
    default:
      xsizeCamera = CAMERA_QVGA_RES_X;
      ysizeCamera = CAMERA_QVGA_RES_Y;
      xoffset = 0;
      yoffset = 0;
      break;
  }

  if ((xsizeCamera > xsizeLCD) || (ysizeCamera > ysizeLCD))
  {
    /* As the camera output resolution is greater than display resolution, a cropped image has to be generated */
    ptrCamera = (uint16_t *)(CAMERA_FRAME_BUFFER);
    ptrCameraCrop = (uint16_t *)(CAMERA_FRAME_BUFFER_CROP);
    xoffset = (xsizeCamera - xsizeLCD) / 2;
    yoffset = (ysizeCamera - ysizeLCD) / 2;
    i=0;
    j=xoffset;
    for (line = 0; line < ysizeLCD; line ++)
    {
      for (column = 0; column < xsizeLCD; column ++)
      {
        ptrCameraCrop[i++] = ptrCamera[j++];
      }
      j += xsizeCamera - xsizeLCD;
    }
    BSP_LCD_DrawRGBImage(0, 0, xsizeLCD , ysizeLCD, (uint8_t *)ptrCameraCrop);
  }
  else
  {
    BSP_LCD_DrawRGBImage(xoffset, yoffset, xsizeCamera , ysizeCamera, (uint8_t *)CAMERA_FRAME_BUFFER);
  }
  BSP_CAMERA_Resume();
}


/**
  * @}
  */ 

/**
  * @}
  */ 
