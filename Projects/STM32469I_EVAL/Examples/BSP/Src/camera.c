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
#include "stm32469i_eval_audio.h"


/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Camera Frame Buffer address */

#define CAMERA_STATUS_TEXT_POS    30            /* Number of lines from bottom of screen */

#define CAMERA_RES_INDEX_MIN      CAMERA_R160x120
#define CAMERA_RES_INDEX_MAX      CAMERA_R640x480

#define CAMERA_CONTRAST_MIN       CAMERA_CONTRAST_LEVEL0
#define CAMERA_CONTRAST_MAX       CAMERA_CONTRAST_LEVEL4
#define CAMERA_BRIGHTNESS_MIN     CAMERA_BRIGHTNESS_LEVEL0
#define CAMERA_BRIGHTNESS_MAX     CAMERA_BRIGHTNESS_LEVEL4

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static JOYState_TypeDef JoyState = JOY_NONE;
static uint32_t   CameraResX;
static uint32_t   CameraResY;
static uint32_t   CameraResIndex;
static uint32_t   CameraChangeResolutionAsked;
static volatile uint32_t   CameraFrameBufferInitComplete = 0;
static volatile uint32_t   CameraFrameBufferInitError = 0;
static uint32_t   offset_cam = 0;
static uint32_t   offset_lcd = 0;
static uint32_t   display_line_counter = 0;
static uint32_t   display_rotated_line_counter = 0; /* used when rotating by 90 degrees image from camera only */
static uint32_t   refresh_authorized = 0;
static uint32_t   special_effect = CAMERA_BLACK_WHITE;        /* No special effect applied */
static uint32_t   color_effect = CAMERA_BLACK_WHITE_NORMAL;
static uint32_t   contrast = CAMERA_CONTRAST_LEVEL2;          /* Mid-level brightness */
static uint32_t   brightness = CAMERA_BRIGHTNESS_LEVEL2;      /* Mid-level contrast */

/* Global variables */
volatile uint32_t Camera_AllowDma2dCopyCamFrmBuffToLcdFrmBuff = 0;

/* Imported global variables */
extern DMA2D_HandleTypeDef hdma2d_eval; /* DMA2D handle variable */

/* Private function prototypes -----------------------------------------------*/
static void Camera_SetHint(void);

static void ConvertCameraLineRgb565ToLcdLineARGB8888(void *pSrc,
                                                     void *pDst,
                                                     uint32_t xSize);

static void ConvertCameraLineRgb565ToLcdColumnARGB8888(void *pSrc,
                                                       void *pDst,
                                                       uint32_t xSize,
                                                       uint32_t lineStridePixels);

static void Camera_FrameBuffer_Init_TransferComplete(DMA2D_HandleTypeDef *hdma2d);

static void Camera_FrameBuffer_Init_TransferError(DMA2D_HandleTypeDef *hdma2d);

static uint32_t CameraFrameBufferRgb565_Init(uint32_t sizeX,
                                             uint32_t sizeY,
                                             uint32_t argb8888_Value);

void BSP_CAMERA_LineEventCallback(void);

static void Camera_LineEventCallbackNoRotation(void);
static void Camera_LineEventCallbackRotation90(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Camera demo
  * @param  None
  * @retval None
  */
void Camera_demo (void)
{
  uint8_t  status = 0;
  uint32_t exit = 0;
  uint32_t camera_status = CAMERA_OK;
  uint32_t argb8888_Value = 0x00556677;

  Camera_SetHint();

  /* Activate joystick as used for contrast, brightness and visual effect settings */
  /* on image from camera sensor                                                   */
  status = BSP_JOY_Init(JOY_MODE_GPIO);
  if (status != IO_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS - 15, (uint8_t *)"Joystick cannot be initialized", CENTER_MODE);
  }

  /*## Camera Initialization and start capture ############################*/

  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"Wait for camera initialization...", CENTER_MODE);

  CameraResIndex = CAMERA_R160x120;     /* Set QQVGA default resolution */
  CameraChangeResolutionAsked = 1;
  CameraResX = CAMERA_QQVGA_RES_X;
  CameraResY = CAMERA_QQVGA_RES_Y;

  /* Infinite loop */
  while (exit == 0)
  {
    if (CameraChangeResolutionAsked)  /* A camera change resolution has been asked */
    {
      CameraChangeResolutionAsked = 0;

      if (CameraResIndex > CAMERA_R160x120)
      { /* If camera stream already started, it has to be stopped before reconfiguration */
        /* Stop camera stream */
        refresh_authorized = 0;
        camera_status = BSP_CAMERA_Stop();
        BSP_TEST_APPLI_ASSERT(camera_status != CAMERA_OK);

        /* Clear screen */
        Camera_SetHint();

        BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"Wait for camera initialization...", CENTER_MODE);
      }

      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

      switch (CameraResIndex)
      {
        case CAMERA_R160x120:
          CameraResX = CAMERA_QQVGA_RES_X;
          CameraResY = CAMERA_QQVGA_RES_Y;
          /* Initialize the Camera */
          camera_status = BSP_CAMERA_Init(RESOLUTION_R160x120);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"       QQVGA camera stream       ", CENTER_MODE);
          break;

        case CAMERA_R320x240:
          CameraResX = CAMERA_QVGA_RES_X;
          CameraResY = CAMERA_QVGA_RES_Y;
          /* Initialize the Camera */
          camera_status = BSP_CAMERA_Init(RESOLUTION_R320x240);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"       QVGA camera stream        ", CENTER_MODE);
          break;

        case CAMERA_R480x272:
          CameraResX = CAMERA_480x272_RES_X;
          CameraResY = CAMERA_480x272_RES_Y;
          /* Initialize the Camera */
          camera_status = BSP_CAMERA_Init(RESOLUTION_R480x272);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"      480x272 camera stream      ", CENTER_MODE);
          break;

        case CAMERA_R640x480:
          CameraResX = CAMERA_VGA_RES_X;
          CameraResY = CAMERA_VGA_RES_Y;
          /* Initialize the Camera */
          camera_status = BSP_CAMERA_Init(RESOLUTION_R640x480);
          BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"        VGA camera stream        ", CENTER_MODE);
          break;

        default :
          break;
      }

      if (camera_status != CAMERA_OK)
      {
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS - 15, (uint8_t *)"        CAMERA INIT ERROR        ", CENTER_MODE);
      }

      offset_cam = 0;
      offset_lcd = 0;
      display_line_counter = 0;
      display_rotated_line_counter = 0;

      special_effect = CAMERA_BLACK_WHITE;      /* No special effect applied */
      color_effect = CAMERA_BLACK_WHITE_NORMAL;
      contrast = CAMERA_CONTRAST_LEVEL2;        /* Mid-level brightness */
      brightness = CAMERA_BRIGHTNESS_LEVEL2;    /* Mid-level contrast */

      refresh_authorized = 1;

      CameraFrameBufferInitComplete = 0;
      CameraFrameBufferInitError    = 0;

      if(CameraResIndex == CAMERA_R640x480)
      {
        /* Clear first the LCD */
        BSP_LCD_Clear(LCD_COLOR_WHITE);
      }

      /* Init or Re-Init Camera frame buffer by using DMA2D engine in mode Register to Memory */
      camera_status = CameraFrameBufferRgb565_Init(CAMERA_VGA_RES_X,
                                                   CAMERA_VGA_RES_Y,
                                                   argb8888_Value);
      BSP_TEST_APPLI_ASSERT(camera_status != CAMERA_OK);

      /* Wait end of DMA2D operation of error : via IT callback update */
      while((CameraFrameBufferInitComplete == 0) && (CameraFrameBufferInitError == 0)) {;}

      /* Assert if error : no transfer complete */
      BSP_TEST_APPLI_ASSERT(CameraFrameBufferInitComplete != 1);

      /* Start / Restart camera stream */
      BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FB_START_ADDR);

      /* Allow DMA2D copy from Camera frame buffer to LCD Frame buffer location */
      Camera_AllowDma2dCopyCamFrmBuffToLcdFrmBuff = 1;

    }  /* (CameraResIndex != CameraCurrentResIndex) */

    /* Get the Joystick State */
    JoyState = BSP_JOY_GetState();

    switch (JoyState)
    {
      case JOY_UP:
        if (brightness < CAMERA_BRIGHTNESS_MAX)
        {
          brightness++;
        }
        else
        {
          JoyState = JOY_NONE;  /* Cancel brightness change */
        }
        break;

      case JOY_DOWN:
        if (brightness > CAMERA_BRIGHTNESS_MIN)
        {
          brightness--;
        }
        else
        {
          JoyState = JOY_NONE;  /* Cancel brightness change */
        }
        break;

      case JOY_LEFT:
        if (contrast > CAMERA_CONTRAST_MIN)
        {
          contrast--;
        }
        else
        {
          JoyState = JOY_NONE;  /* Cancel contrast change */
        }
        break;

      case JOY_RIGHT:
        if (contrast < CAMERA_CONTRAST_MAX)
        {
          contrast++;
        }
        else
        {
          JoyState = JOY_NONE;  /* Cancel contrast change */
        }
        break;

      case JOY_SEL:
        switch (special_effect)
        {
          case CAMERA_BLACK_WHITE :
            if (color_effect < CAMERA_BLACK_WHITE_NORMAL)
            {
              /* From BW effect to another BW effect */
              color_effect++;
            }
            else
            {
              /* From BW or none effect to color effect */
              special_effect = CAMERA_COLOR_EFFECT;
              color_effect = CAMERA_COLOR_EFFECT_ANTIQUE;
            }
            break;

          case CAMERA_COLOR_EFFECT :
            if (color_effect < CAMERA_COLOR_EFFECT_RED)
            {
              /* From color effect to another color effect */
              color_effect++;
            }
            else
            {
              /* From color effect to BW effect */
              special_effect = CAMERA_BLACK_WHITE;
              color_effect = CAMERA_BLACK_WHITE_BW;
            }
            break;

          default :
            /* None effect */
            special_effect = CAMERA_BLACK_WHITE;
            color_effect = CAMERA_BLACK_WHITE_NORMAL;
            break;
        }
        break;

      default:
        break;
    }

    if ((JoyState == JOY_UP) || (JoyState == JOY_DOWN) || (JoyState == JOY_LEFT) || (JoyState == JOY_RIGHT))
    {
      /* A brightness change has been asked */
      BSP_CAMERA_ContrastBrightnessConfig(contrast, brightness);
      if ((JoyState == JOY_UP) || (JoyState == JOY_DOWN))
      {
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"        Brightness change        ", CENTER_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"         Contrast change         ", CENTER_MODE);
      }
    }

    if (JoyState == JOY_SEL)
    {
      /* A color effect change has been asked */
      switch (special_effect)
      {
        case CAMERA_BLACK_WHITE :
          BSP_CAMERA_BlackWhiteConfig(color_effect);
          switch (color_effect)
          {
            case CAMERA_BLACK_WHITE_NORMAL :
              BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"        No effect applied        ", CENTER_MODE);
              break;
            case CAMERA_BLACK_WHITE_BW :
              BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)" Black and white effect applied  ", CENTER_MODE);
              break;
            case CAMERA_BLACK_WHITE_NEGATIVE :
              BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"     Negative effect applied     ", CENTER_MODE);
              break;
            case CAMERA_BLACK_WHITE_BW_NEGATIVE :
              BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"   BW negative effect applied    ", CENTER_MODE);
              break;
          }
          break;

        case CAMERA_COLOR_EFFECT :
          BSP_CAMERA_ColorEffectConfig(color_effect);
          switch (color_effect)
          {
            case CAMERA_COLOR_EFFECT_ANTIQUE :
              BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"   Sepia color effect applied    ", CENTER_MODE);
              break;
            case CAMERA_COLOR_EFFECT_BLUE :
              BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"    Blue color effect applied    ", CENTER_MODE);
              break;
            case CAMERA_COLOR_EFFECT_GREEN :
              BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"   Green color effect applied    ", CENTER_MODE);
              break;
            case CAMERA_COLOR_EFFECT_RED :
              BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - CAMERA_STATUS_TEXT_POS, (uint8_t *)"     Red color effect applied    ", CENTER_MODE);
              break;
          }
          break;
      }
    }

    /* Toggle LED4 */
    BSP_LED_Toggle(LED3);

    /* Insert 100 ms delay */
    HAL_Delay(100);

    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);

    /* Insert 100 ms delay */
    HAL_Delay(100);
    if (CheckForUserInput() > 0)
    {
      if (CameraResIndex < CAMERA_RES_INDEX_MAX)
      {
        CameraResIndex++;     /* Switch to higher resolution */
        CameraChangeResolutionAsked = 1;

        /* Disallow DMA2D copy from Camera frame buffer to LCD Frame buffer location */
        Camera_AllowDma2dCopyCamFrmBuffToLcdFrmBuff = 0;

        /* Insert 10 ms delay */
        HAL_Delay(10);
      }
      else
      {
        BSP_CAMERA_DeInit();
        exit = 1;
      }
    }
  }

  /* Stop camera stream */
  camera_status = BSP_CAMERA_Stop();
  BSP_TEST_APPLI_ASSERT(camera_status != CAMERA_OK);

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
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"Use Joystick Up/Down    to change brightness   ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"Use Joystick Left/Right to change contrast     ", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 75, (uint8_t *)"Use Joystick Select     to change visual effect", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 100, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 110);
  BSP_LCD_DrawRect(11, 101, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 112);
}

/**
  * @brief  DMA2D Camera frame buffer init Transfer completed callback
  * @param  hdma2d: DMA2D handle.
  * @retval None
  */
static void Camera_FrameBuffer_Init_TransferComplete(DMA2D_HandleTypeDef *hdma2d)
{
 CameraFrameBufferInitComplete = 1;
}

/**
  * @brief  DMA2D Camera frame buffer init Transfer error callback
  * @param  hdma2d: DMA2D handle.
  * @retval None
  */
static void Camera_FrameBuffer_Init_TransferError(DMA2D_HandleTypeDef *hdma2d)
{
 CameraFrameBufferInitError = 1;
}


/**
  * @brief  Init camera frame buffer with fixed color in format RGB565
  *         to a LCD display frame buffer line in format ARGB8888 using DMA2D service.
  * @param  sizeX: Size in X of rectangular region of the Camera frame buffer to initialize (in pixels unit)
  * @param  sizeX: Size in X of rectangular region of the Camera frame buffer to initialize (in pixels unit)
  * @param  argb_Value : Initialization value (pattern ARGB8888) to be applied to all rectangular region selected.
  * @retval Status CAMERA_OK or CAMERA_ERROR
  */
static uint32_t CameraFrameBufferRgb565_Init(uint32_t sizeX, uint32_t sizeY, uint32_t argb8888_Value)
{
  HAL_StatusTypeDef hal_status = HAL_OK;
  uint32_t status = CAMERA_ERROR;

  if((sizeX <= CAMERA_VGA_RES_X) && (sizeY <= CAMERA_VGA_RES_Y))
  {
    /* Register to memory mode with RGB565 as colorMode */
    hdma2d_eval.Init.Mode         = DMA2D_R2M; /* Mode Register to Memory */
    hdma2d_eval.Init.ColorMode    = DMA2D_RGB565; /* Output color mode */
    hdma2d_eval.Init.OutputOffset = 0x0; /* No offset in output */

    /* Set callback functions on transfer complete and transfer error */
    hdma2d_eval.XferCpltCallback  = Camera_FrameBuffer_Init_TransferComplete;
    hdma2d_eval.XferErrorCallback = Camera_FrameBuffer_Init_TransferError;

    hdma2d_eval.Instance = DMA2D;

    hal_status = HAL_DMA2D_Init(&hdma2d_eval);
    BSP_TEST_APPLI_ASSERT(hal_status != HAL_OK);

    hal_status = HAL_DMA2D_Start_IT(&hdma2d_eval,
                                    argb8888_Value, /* Color value in Register to Memory DMA2D mode */
                                    (uint32_t)CAMERA_FB_START_ADDR,  /* DMA2D output buffer */
                                    sizeX,  /* width of buffer in pixels */
                                    sizeY); /* height of buffer in lines */
    BSP_TEST_APPLI_ASSERT(hal_status != HAL_OK);

    status = CAMERA_OK;
  }

  return (status);
}

/**
  * @brief  Converts a camera buffer line of format RGB565
  *         to a LCD display frame buffer line in format ARGB8888 using DMA2D service.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @param  xSize: Buffer width
  * @retval None
  */
static void ConvertCameraLineRgb565ToLcdLineARGB8888(void *pSrc, void *pDst, uint32_t xSize)
{
  HAL_StatusTypeDef hal_status = HAL_OK;

  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_ARGB8888;
  hdma2d_eval.Init.OutputOffset = 0;

  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_RGB565;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;

  hdma2d_eval.Instance = DMA2D;

  /* DMA2D Initialization */
  if (HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK)
  {
    if (HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)pSrc, (uint32_t)pDst, xSize, 1) == HAL_OK)
      {
        /* Polling For DMA transfer */
        hal_status = HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
        BSP_TEST_APPLI_ASSERT(hal_status != HAL_OK);
      }
    }
  }
}

/**
  * @brief  Converts a camera buffer line of format RGB565
  *         to a LCD display frame buffer column in format ARGB8888 using DMA2D service.
  *         This handles the rotation of 90 degrees hour direction of camera buffer.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Pointer to destination buffer
  * @param  xSize: Buffer width
  * @param  lineStridePixels: line stride in pixels unit
  * @retval None
  */
static void ConvertCameraLineRgb565ToLcdColumnARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t lineStridePixels)
{
  HAL_StatusTypeDef hal_status = HAL_OK;

  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_ARGB8888;
  hdma2d_eval.Init.OutputOffset = lineStridePixels;

  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = DMA2D_RGB565;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;

  hdma2d_eval.Instance = DMA2D;

  /* DMA2D Initialization */
  if (HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK)
  {
    if (HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK)
    {
      /* xSize x 1 = size in pixels to copy */
      /* Width = 1, Height = xSize          */
      if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)pSrc, (uint32_t)pDst, 1, xSize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        hal_status = HAL_DMA2D_PollForTransfer(&hdma2d_eval, 10);
        BSP_TEST_APPLI_ASSERT(hal_status != HAL_OK);
      }
    }
  }
}

/**
  * @brief  Camera line event callback
  * @param  None
  * @retval None
  */
void BSP_CAMERA_LineEventCallback(void)
{
  /* If DMA2D copy is allowed */
  if(Camera_AllowDma2dCopyCamFrmBuffToLcdFrmBuff == 1)
  {
    if(BSP_CAMERA_GetRotation() == CAMERA_NO_ROTATION)
    {
      Camera_LineEventCallbackNoRotation();
    }
    else if(BSP_CAMERA_GetRotation() == CAMERA_ROTATION_90)
    {
      Camera_LineEventCallbackRotation90();
    }
    else
    {
      /* Do nothing */
      ;
    }
  }
}

/**
  * @brief  Camera line event callback when no camera image rotation is required
  * @param  None
  * @retval None
  */
static void Camera_LineEventCallbackNoRotation(void)
{
  uint32_t LcdResX = BSP_LCD_GetXSize();
  uint32_t LcdResY = BSP_LCD_GetYSize();

  if (refresh_authorized)
  {
    if ((offset_lcd == 0) && (CameraResX <= LcdResX) && (CameraResY <= LcdResY))
    {
      /* If Camera resolution is lower than LCD resolution, set display in the middle of the screen */
      offset_lcd =   ((((LcdResY - CameraResY) / 2) * LcdResX)   /* Middle of the screen on Y axis */
                      +   ((LcdResX - CameraResX) / 2))             /* Middle of the screen on X axis */
                     * sizeof(uint32_t);

      if ((CameraResY == CAMERA_QQVGA_RES_Y) || (CameraResIndex == CAMERA_R480x272))
      { /* Add offset for QQVGA */
        offset_lcd += 40 * LcdResX * sizeof(uint32_t);
      }
    }

    if (display_line_counter < CameraResY)
    {
      if (display_line_counter < LcdResY)
      {
        if (CameraResX < LcdResX)
        {
          ConvertCameraLineRgb565ToLcdLineARGB8888((uint32_t *)(CAMERA_FB_START_ADDR + offset_cam),
                                                   (uint32_t *)(LCD_FB_START_ADDRESS + offset_lcd),
                                                   CameraResX);
        }
        else
        {
          ConvertCameraLineRgb565ToLcdLineARGB8888((uint32_t *)(CAMERA_FB_START_ADDR + offset_cam),
                                                   (uint32_t *)(LCD_FB_START_ADDRESS + offset_lcd),
                                                   LcdResX);
        }

        offset_cam  = offset_cam + (CameraResX * sizeof(uint16_t));
        offset_lcd  = offset_lcd + (LcdResX * sizeof(uint32_t));
      }

      display_line_counter++;
    }
    else
    {
      offset_cam = 0;
      offset_lcd = 0;
      display_line_counter = 0;
    }
  }
}

/**
  * @brief  Camera line event callback when camera image rotation of 90 degrees hour
  *         direction is required.
  * @param  None
  * @retval None
  */
static void Camera_LineEventCallbackRotation90(void)
{
  uint32_t LcdResX = BSP_LCD_GetXSize();
  uint32_t LcdResY = BSP_LCD_GetYSize();

  if (refresh_authorized)
  {
    if ((offset_lcd == 0) && (CameraResY <= LcdResX) && (CameraResX <= LcdResY))
    {
      /* If Camera resolution is lower than LCD resolution, set display in the middle of the screen */
      offset_lcd =   ((((LcdResY - CameraResX) / 2) * LcdResX)   /* Middle of the screen on Y axis */
                      -   ((LcdResX - CameraResY) / 2))             /* Middle of the screen on X axis */
                     * sizeof(uint32_t);

      if ((CameraResY == CAMERA_QQVGA_RES_Y) || (CameraResIndex == CAMERA_R480x272))
      { /* Add offset for QQVGA : + 40 LCD lines */
        offset_lcd += 40 * LcdResX * sizeof(uint32_t);
      }
    }

    /* Count the number of line rotated copied to be able to stop when limit reached */
    if (display_rotated_line_counter < CameraResY)
    {
      /* if number of rotated line copied is reaching horizontal resolution of display LCD : stop copying also */
      if (display_rotated_line_counter < LcdResX)
      {
        /* Depending on CameraResX versus LcdResY                             */
        /* clamp the column size copied to LcdResY if (CameraResX > LcdResY) */
        if (CameraResX <= LcdResY)
        {
          ConvertCameraLineRgb565ToLcdColumnARGB8888((uint32_t *)(CAMERA_FB_START_ADDR + offset_cam),
                                                     (uint32_t *)(LCD_FB_START_ADDRESS + offset_lcd),
                                                     CameraResX,
                                                     (LcdResX - 1)); /* lineStridePixels for DMA is always one horizontal line LCD - 1 pixels */
        }
        else
        {
          ConvertCameraLineRgb565ToLcdColumnARGB8888((uint32_t *)(CAMERA_FB_START_ADDR + offset_cam),
                                                     (uint32_t *)(LCD_FB_START_ADDRESS + offset_lcd),
                                                     LcdResY,
                                                     (LcdResX - 1)); /* lineStridePixels for DMA is always one horizontal line LCD - 1 pixels */
        }

        offset_cam  = offset_cam + (CameraResX * sizeof(uint16_t)); /* uint16_t : because format camera data is RGB565 */

        /* offset_lcd is decremented after each line rotated copied by one pixel of the LCD buffer = 4 bytes (ARGB8888) */
        /* from 'offset_lcd' pre computed in first if statement of the current function */
        offset_lcd  = offset_lcd - sizeof(uint32_t); /* uint32_t : because format of LCD Frame buffer is ARGB8888 */
      }

      display_rotated_line_counter++;
    }
    else
    {
      offset_cam = 0;
      offset_lcd = 0;
      display_rotated_line_counter = 0;
    }
  }
}


/**
  * @}
  */

/**
  * @}
  */
