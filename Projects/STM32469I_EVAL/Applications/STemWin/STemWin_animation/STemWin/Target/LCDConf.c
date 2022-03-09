/**
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
#include "LCDConf.h"
#include "GUI_Private.h"
#include "GUIDRV_Lin.h"

/** @addtogroup LCD CONFIGURATION
  * @{
  */

/** @defgroup LCD CONFIGURATION
  * @brief This file contains the LCD Configuration
  * @{
  */ 

/** @defgroup LCD CONFIGURATION_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup LCD CONFIGURATION_Private_Defines
  * @{
  */ 

/* Define the possible screen orientations  */
#define ROTATION_0   0
#define ROTATION_CW  1
#define ROTATION_180 2
#define ROTATION_CCW 3

/* Define the possible color modes */
#define CMS_ARGB8888 1
#define CMS_RGB888   2
#define CMS_RGB565   3
#define CMS_ARGB1555 4
#define CMS_ARGB4444 5
#define CMS_L8       6
#define CMS_AL44     7
#define CMS_AL88     8

/* Define physical screen sizes */
#define XSIZE_PHYS 800
#define YSIZE_PHYS 480

/* Define the number of buffers to use (minimum 1) */
#define NUM_BUFFERS  3
/* Define the number of virtual buffers to use (minimum 1) */
#define NUM_VSCREENS 1

/* Redefine number of layers to be used, must be equal or less than in GUIConf.h */
#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS 1

/* Define Layer 0 color mode, orientation and sizes */
#define COLOR_MODE_0  CMS_RGB565
#define ORIENTATION_0 ROTATION_0
#define XSIZE_0       XSIZE_PHYS
#define YSIZE_0       YSIZE_PHYS

/* Define Layer 1 color mode, orientation and sizes */
#define COLOR_MODE_1  CMS_ARGB4444
#define ORIENTATION_1 ROTATION_0
#define XSIZE_1       XSIZE_PHYS
#define YSIZE_1       YSIZE_PHYS

/* Define the background color shown where no layer is active */
#define BK_COLOR GUI_DARKBLUE

/* Define the screen area: can be 2 or 4 */
#define ZONES  2
#define VSYNC  1
#define VBP    1
#define VFP    1
#define VACT   YSIZE_PHYS
#define HSYNC  1
#define HBP    1
#define HFP    1
#define HACT   XSIZE_PHYS/ZONES

static volatile char       TransferInProgress  = 0;
void DMA2D_IRQHandler(void);


/* Choose Layer 0 color conversion depending of the color mode choosed */
#if   (COLOR_MODE_0 == CMS_ARGB8888)
  #define COLOR_CONVERSION_0 GUICC_M8888I
#elif (COLOR_MODE_0 == CMS_RGB888)
  #define COLOR_CONVERSION_0 GUICC_M888
#elif (COLOR_MODE_0 == CMS_RGB565)
  #define COLOR_CONVERSION_0 GUICC_M565
#elif (COLOR_MODE_0 == CMS_ARGB1555)
  #define COLOR_CONVERSION_0 GUICC_M1555I
#elif (COLOR_MODE_0 == CMS_ARGB4444)
  #define COLOR_CONVERSION_0 GUICC_M4444I
#elif (COLOR_MODE_0 == CMS_L8)
  #define COLOR_CONVERSION_0 GUICC_8666
#elif (COLOR_MODE_0 == CMS_AL44)
  #define COLOR_CONVERSION_0 GUICC_1616I
#elif (COLOR_MODE_0 == CMS_AL88)
  #define COLOR_CONVERSION_0 GUICC_88666I
#else
  #error Illegal color mode 0!
#endif

/* Choose Choose Layer 0 gui driver depending of the color mode choosed */
#if   (COLOR_MODE_0 == CMS_ARGB8888)
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_32
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_32
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_32
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_32
  #endif
#elif (COLOR_MODE_0 == CMS_RGB888)
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_24
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_24
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_24
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_24
  #endif
#elif (COLOR_MODE_0 == CMS_RGB565)   \
   || (COLOR_MODE_0 == CMS_ARGB1555) \
   || (COLOR_MODE_0 == CMS_ARGB4444) \
   || (COLOR_MODE_0 == CMS_AL88)
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_16
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_16
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_16
  #endif
#elif (COLOR_MODE_0 == CMS_L8)   \
   || (COLOR_MODE_0 == CMS_AL44)
  #if   (ORIENTATION_0 == ROTATION_0)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_8
  #elif (ORIENTATION_0 == ROTATION_CW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_8
  #elif (ORIENTATION_0 == ROTATION_180)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_8
  #elif (ORIENTATION_0 == ROTATION_CCW)
    #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_8
  #endif
#endif

/* Choose Layer 1 color conversion depending of the color mode//orientation choosed */
#if (GUI_NUM_LAYERS > 1)
#if   (COLOR_MODE_1 == CMS_ARGB8888)
  #define COLOR_CONVERSION_1 GUICC_M8888I
#elif (COLOR_MODE_1 == CMS_RGB888)
  #define COLOR_CONVERSION_1 GUICC_M888
#elif (COLOR_MODE_1 == CMS_RGB565)
  #define COLOR_CONVERSION_1 GUICC_M565
#elif (COLOR_MODE_1 == CMS_ARGB1555)
  #define COLOR_CONVERSION_1 GUICC_M1555I
#elif (COLOR_MODE_1 == CMS_ARGB4444)
  #define COLOR_CONVERSION_1 GUICC_M4444I
#elif (COLOR_MODE_1 == CMS_L8)
  #define COLOR_CONVERSION_1 GUICC_8666
#elif (COLOR_MODE_1 == CMS_AL44)
  #define COLOR_CONVERSION_1 GUICC_1616I
#elif (COLOR_MODE_1 == CMS_AL88)
  #define COLOR_CONVERSION_1 GUICC_88666I
#else
  #error Illegal color mode 0!
#endif

/* Choose Choose Layer 1 gui driver depending of the color mode/orientation choosed */
#if   (COLOR_MODE_1 == CMS_ARGB8888)
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_32
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_32
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_32
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_32
  #endif
#elif (COLOR_MODE_1 == CMS_RGB888)
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_24
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_24
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_24
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_24
  #endif
#elif (COLOR_MODE_1 == CMS_RGB565)   \
   || (COLOR_MODE_1 == CMS_ARGB1555) \
   || (COLOR_MODE_1 == CMS_ARGB4444) \
   || (COLOR_MODE_1 == CMS_AL88)
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_16
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_16
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_16
  #endif
#elif (COLOR_MODE_1 == CMS_L8)   \
   || (COLOR_MODE_1 == CMS_AL44)
  #if   (ORIENTATION_1 == ROTATION_0)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_8
  #elif (ORIENTATION_1 == ROTATION_CW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSX_8
  #elif (ORIENTATION_1 == ROTATION_180)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OXY_8
  #elif (ORIENTATION_1 == ROTATION_CCW)
    #define DISPLAY_DRIVER_1   GUIDRV_LIN_OSY_8
  #endif
#endif

#else

/* Use complete display automatically in case of only one layer */
#undef XSIZE_0
#undef YSIZE_0
#define XSIZE_0 XSIZE_PHYS
#define YSIZE_0 YSIZE_PHYS
     
#endif

/* Check the that configuration used is OK */
#if NUM_BUFFERS > 3
  #error More than 3 buffers make no sense and are not supported in this configuration file!
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   NUM_BUFFERS
  #define NUM_BUFFERS 1
#else
  #if (NUM_BUFFERS <= 0)
    #error At least one buffer needs to be defined!
  #endif
#endif
#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS 1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens together with multiple buffers are not allowed!
#endif
     
/* Define the offset of the frame to be used */
#define LCD_FRAME_BUFFER ((U32)0xC0000000)

/* DSI tearing effect GPIO */
#define __DSI_MASK_TE()   (GPIOJ->AFR[0] &= (0xFFFFF0FFU))
#define __DSI_UNMASK_TE() (GPIOJ->AFR[0] |= ((uint32_t)(GPIO_AF13_DSI) << 8))

     
/**
  * @}
  */
/* Redirect bulk conversion to DMA2D routines */
#define DEFINE_DMA2D_COLORCONVERSION(PFIX, PIXELFORMAT)                                                        \
static void _Color2IndexBulk_##PFIX##_DMA2D(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Color2IndexBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}                                                                                                              \
static void _Index2ColorBulk_##PFIX##_DMA2D(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex) { \
  _DMA_Index2ColorBulk(pIndex, pColor, NumItems, SizeOfIndex, PIXELFORMAT);                                    \
}
  
/**
  * @}
  */ 
/** @defgroup LCD CONFIGURATION_Private_Variables
  * @{
  */
LTDC_HandleTypeDef                   hltdc;
DSI_HandleTypeDef     	             hdsi;
static LCD_LayerPropTypedef          layer_prop[GUI_NUM_LAYERS];

/* Screen needed definitions */
uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */

/* ZONES of the screen */
uint8_t pCols[ZONES][4] =
{
#if (ZONES == 4 )
  {0x00, 0x00, 0x00, 0xC7}, /*   0 -> 199 */
  {0x00, 0xC8, 0x01, 0x8F}, /* 200 -> 399 */
  {0x01, 0x90, 0x02, 0x57}, /* 400 -> 599 */
  {0x02, 0x58, 0x03, 0x1F}, /* 600 -> 799 */
#elif (ZONES == 2 )
  {0x00, 0x00, 0x01, 0x8F}, /*   0 -> 399 */
  {0x01, 0x90, 0x03, 0x1F}  
#elif (ZONES == 1 )
  {0x00, 0x00, 0x01, 0x3F}, /*   0 -> 799 */
#endif  
};

volatile int32_t LCD_ActiveRegion    = 1;
volatile int32_t LCD_Refershing      = 0;



/* Array for speeding up nibble conversion for A4 bitmaps */
static const U8 _aMirror[] = {
  0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0,
  0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x81, 0x91, 0xA1, 0xB1, 0xC1, 0xD1, 0xE1, 0xF1,
  0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72, 0x82, 0x92, 0xA2, 0xB2, 0xC2, 0xD2, 0xE2, 0xF2,
  0x03, 0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x83, 0x93, 0xA3, 0xB3, 0xC3, 0xD3, 0xE3, 0xF3,
  0x04, 0x14, 0x24, 0x34, 0x44, 0x54, 0x64, 0x74, 0x84, 0x94, 0xA4, 0xB4, 0xC4, 0xD4, 0xE4, 0xF4,
  0x05, 0x15, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75, 0x85, 0x95, 0xA5, 0xB5, 0xC5, 0xD5, 0xE5, 0xF5,
  0x06, 0x16, 0x26, 0x36, 0x46, 0x56, 0x66, 0x76, 0x86, 0x96, 0xA6, 0xB6, 0xC6, 0xD6, 0xE6, 0xF6,
  0x07, 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77, 0x87, 0x97, 0xA7, 0xB7, 0xC7, 0xD7, 0xE7, 0xF7,
  0x08, 0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78, 0x88, 0x98, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8,
  0x09, 0x19, 0x29, 0x39, 0x49, 0x59, 0x69, 0x79, 0x89, 0x99, 0xA9, 0xB9, 0xC9, 0xD9, 0xE9, 0xF9,
  0x0A, 0x1A, 0x2A, 0x3A, 0x4A, 0x5A, 0x6A, 0x7A, 0x8A, 0x9A, 0xAA, 0xBA, 0xCA, 0xDA, 0xEA, 0xFA,
  0x0B, 0x1B, 0x2B, 0x3B, 0x4B, 0x5B, 0x6B, 0x7B, 0x8B, 0x9B, 0xAB, 0xBB, 0xCB, 0xDB, 0xEB, 0xFB,
  0x0C, 0x1C, 0x2C, 0x3C, 0x4C, 0x5C, 0x6C, 0x7C, 0x8C, 0x9C, 0xAC, 0xBC, 0xCC, 0xDC, 0xEC, 0xFC,
  0x0D, 0x1D, 0x2D, 0x3D, 0x4D, 0x5D, 0x6D, 0x7D, 0x8D, 0x9D, 0xAD, 0xBD, 0xCD, 0xDD, 0xED, 0xFD,
  0x0E, 0x1E, 0x2E, 0x3E, 0x4E, 0x5E, 0x6E, 0x7E, 0x8E, 0x9E, 0xAE, 0xBE, 0xCE, 0xDE, 0xEE, 0xFE,
  0x0F, 0x1F, 0x2F, 0x3F, 0x4F, 0x5F, 0x6F, 0x7F, 0x8F, 0x9F, 0xAF, 0xBF, 0xCF, 0xDF, 0xEF, 0xFF,
};

/* Frame buffer tables to be used */
static const U32      _aAddr[]   = { LCD_FRAME_BUFFER, LCD_FRAME_BUFFER + XSIZE_PHYS * YSIZE_PHYS * sizeof(U32) * NUM_VSCREENS * NUM_BUFFERS };
U32                   LCD_Addr[] = { LCD_FRAME_BUFFER, LCD_FRAME_BUFFER + XSIZE_PHYS * YSIZE_PHYS * sizeof(U32) * NUM_VSCREENS * NUM_BUFFERS };

static int _aPendingBuffer[2] = { -1, -1 };
static int _aBufferIndex[GUI_NUM_LAYERS];
static int _axSize[GUI_NUM_LAYERS];
static int _aySize[GUI_NUM_LAYERS];
static int _aBytesPerPixels[GUI_NUM_LAYERS];
static U32 _aBuffer[XSIZE_PHYS * sizeof(U32) * 3];
static U32 * _pBuffer_DMA2D = &_aBuffer[XSIZE_PHYS * sizeof(U32) * 0];

/* Array of color conversions for each layer */
static const LCD_API_COLOR_CONV * _apColorConvAPI[] = 
{
  COLOR_CONVERSION_0,
#if GUI_NUM_LAYERS > 1
  COLOR_CONVERSION_1,
#endif
};

/* Array of orientations for each layer */
static const int _aOrientation[] = 
{
  ORIENTATION_0,
#if GUI_NUM_LAYERS > 1
  ORIENTATION_1,
#endif
};

/**
  * @}
  */ 

/** @defgroup LCD CONFIGURATION_Private_FunctionPrototypes
  * @{
  */ 
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat);

/* Color conversion routines using DMA2D */
DEFINE_DMA2D_COLORCONVERSION(M8888I, LTDC_PIXEL_FORMAT_ARGB8888)
/* Internal pixel format of emWin is 32 bit, because of that ARGB8888 */
DEFINE_DMA2D_COLORCONVERSION(M888,   LTDC_PIXEL_FORMAT_ARGB8888) 
DEFINE_DMA2D_COLORCONVERSION(M565,   LTDC_PIXEL_FORMAT_RGB565)
DEFINE_DMA2D_COLORCONVERSION(M1555I, LTDC_PIXEL_FORMAT_ARGB1555)
DEFINE_DMA2D_COLORCONVERSION(M4444I, LTDC_PIXEL_FORMAT_ARGB4444)

void LCD_SetUpdateRegion(int idx);
void LCD_ReqTear(void);
void DSI_IRQHandler(void);


/**
  * @brief  Get the used pixel format
  * @param  LayerIndex: Layer index used 
  * @retval LTDC pixel format
  */
static U32 _GetPixelformat(int LayerIndex) 
{
  const LCD_API_COLOR_CONV * pColorConvAPI;

  if (LayerIndex >= GUI_COUNTOF(_apColorConvAPI)) 
  {
    return 0;
  }
  pColorConvAPI = _apColorConvAPI[LayerIndex];
  if        (pColorConvAPI == GUICC_M8888I) 
  {
    return LTDC_PIXEL_FORMAT_ARGB8888;
  }
  else if (pColorConvAPI == GUICC_M888) 
  {
    return LTDC_PIXEL_FORMAT_RGB888;
  }
  else if (pColorConvAPI == GUICC_M565) 
  {
    return LTDC_PIXEL_FORMAT_RGB565;
  }
  else if (pColorConvAPI == GUICC_M1555I)
  {
    return LTDC_PIXEL_FORMAT_ARGB1555;
  }
  else if (pColorConvAPI == GUICC_M4444I) 
  {
    return LTDC_PIXEL_FORMAT_ARGB4444;
  }
  else if (pColorConvAPI == GUICC_8666  ) 
  {
    return LTDC_PIXEL_FORMAT_L8;
  }
  else if (pColorConvAPI == GUICC_1616I ) 
  {
    return LTDC_PIXEL_FORMAT_AL44;
  }
  else if (pColorConvAPI == GUICC_88666I) 
  {
    return LTDC_PIXEL_FORMAT_AL88;
  }
  /* We'll hang in case of bad configuration */
  while (1);
}


/**
  * @brief  Enable/Disable the DMA2D interrupt
  * @param  DMA2D_IT: DM2D interrupt to enable
  * @param  NewState: ENABLE/DISABLE
  * @retval LTDC pixel format
  */
static void _DMA2D_ITConfig(U32 DMA2D_IT, int NewState) {
  if (NewState != DISABLE) {    
    DMA2D->CR |= DMA2D_IT;    
  } else {    
    DMA2D->CR &= (U32)~DMA2D_IT;    
  }
}

/**
  * @brief  DCS or Generic short/long write command
  * @param  NbParams: Number of parameters. It indicates the write command mode:
  *                 If inferior to 2, a long write command is performed else short.
  * @param  pParams: Pointer to parameter values table.
  * @retval None
  */
void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams)
{
  if(NbrParams <= 1)
  {
    HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, pParams[0], pParams[1]); 
  }
  else
  {
    HAL_DSI_LongWrite(&hdsi,  0, DSI_DCS_LONG_PKT_WRITE, NbrParams, pParams[NbrParams], pParams); 
  } 
}

/**
  * @brief  BSP LCD Reset
  *         Hw reset the LCD DSI activating its XRES signal (active low for some time)
  *         and desactivating it later.
  *         This signal is only cabled on eval Rev B and beyond.
  */
static void LCD_LL_Reset(void)
{
  GPIO_InitTypeDef  gpio_init_structure;

  __HAL_RCC_GPIOK_CLK_ENABLE();

  /* Configure the GPIO on PK7 */
  gpio_init_structure.Pin   = GPIO_PIN_7;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOK, &gpio_init_structure);

    /* Activate XRES active low */
  HAL_GPIO_WritePin(GPIOK, GPIO_PIN_7, GPIO_PIN_RESET);
  HAL_Delay(20); /* wait 20 ms */

  /* Deactivate XRES */
  HAL_GPIO_WritePin(GPIOK, GPIO_PIN_7, GPIO_PIN_SET);
  HAL_Delay(10); /* wait 10 ms */
}

/**
  * @brief  Initialize the BSP LCD Msp.
  * Application can surcharge if needed this function implementation
  */
void BSP_LCD_MspInit(void)
{
  /** @brief Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();
  
  /** @brief Toggle Sw reset of LTDC IP */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();
  
  /** @brief Enable the DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();
  
  /** @brief Toggle Sw reset of DMA2D IP */
  __HAL_RCC_DMA2D_FORCE_RESET();
  __HAL_RCC_DMA2D_RELEASE_RESET();
  
  /** @brief Enable DSI Host and wrapper clocks */
  __HAL_RCC_DSI_CLK_ENABLE();
  
  /** @brief Soft Reset the DSI Host and wrapper */
  __HAL_RCC_DSI_FORCE_RESET();
  __HAL_RCC_DSI_RELEASE_RESET();
  
  /** @brief NVIC configuration for DSI interrupt that is now enabled */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0xF, 0);
  HAL_NVIC_EnableIRQ(LTDC_IRQn);
  
  /** @brief NVIC configuration for DSI interrupt that is now enabled */
  HAL_NVIC_SetPriority(DSI_IRQn, 0xF, 0);
  HAL_NVIC_EnableIRQ(DSI_IRQn);
  
  
  _DMA2D_ITConfig(DMA2D_CR_TCIE, ENABLE);
  HAL_NVIC_SetPriority(DMA2D_IRQn, 0xF, 0x0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);
}

/**
  * @brief  Initialize the LTDC
  * @param  None
  * @retval None
  */
static void LTDC_Init(void)
{
  /* DeInit */
  hltdc.Instance = LTDC;
  HAL_LTDC_DeInit(&hltdc);

  /* LTDC Config */
  /* Timing and polarity */
  hltdc.Init.HorizontalSync = HSYNC;
  hltdc.Init.VerticalSync = VSYNC;
  hltdc.Init.AccumulatedHBP = HSYNC+HBP;
  hltdc.Init.AccumulatedVBP = VSYNC+VBP;
  hltdc.Init.AccumulatedActiveH = VSYNC+VBP+VACT;
  hltdc.Init.AccumulatedActiveW = HSYNC+HBP+HACT;
  hltdc.Init.TotalHeigh = VSYNC+VBP+VACT+VFP;
  hltdc.Init.TotalWidth = HSYNC+HBP+HACT+HFP;

  /* background value */
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;

  /* Polarity */
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Instance = LTDC;

  HAL_LTDC_Init(&hltdc);
}

/**
  * @brief  Initialize the LCD Controller.
  * @param  None
  * @retval None
  */
static void LCD_LL_Init(void) 
{   
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  GPIO_InitTypeDef          GPIO_Init_Structure;
  static DSI_PHY_TimerTypeDef PhyTimings;
  static DSI_CmdCfgTypeDef         CmdCfg;
  static DSI_LPCmdTypeDef          LPCmd;
  static DSI_PLLInitTypeDef        PLLInit;

  /* Toggle Hardware Reset of the DSI LCD using
   * its XRES signal (active low) */
  LCD_LL_Reset();

  /* Call first MSP Initialize only in case of first initialization
   * This will set IP blocks LTDC, DSI and DMA2D
   * - out of reset
   * - clocked
   * - NVIC IRQ related to IP blocks enabled
   */
  BSP_LCD_MspInit();

  /* LCD clock configuration */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 384 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 384 MHz / 7 = 54.857 MHz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 54.857 MHz / 2 = 27.429 MHz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 7;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;

  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct); 

  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hdsi.Instance = DSI;

  HAL_DSI_DeInit(&(hdsi));

  PLLInit.PLLNDIV  = 100;
  PLLInit.PLLIDF   = DSI_PLL_IN_DIV5;
  PLLInit.PLLODF   = DSI_PLL_OUT_DIV1;

  hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  hdsi.Init.TXEscapeCkdiv = 0x4;
  HAL_DSI_Init(&(hdsi), &(PLLInit));

  /* Configure the DSI for Command mode */
  CmdCfg.VirtualChannelID      = 0;
  CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
  CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
  CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.ColorCoding           = DSI_RGB565;
  CmdCfg.CommandSize           = HACT;
  CmdCfg.TearingEffectSource   = DSI_TE_EXTERNAL;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh      = DSI_AR_DISABLE;
  CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
  HAL_DSI_ConfigAdaptedCommandMode(&hdsi, &CmdCfg);

  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_ENABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_ENABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_ENABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_ENABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_ENABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_ENABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_ENABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_ENABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_ENABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_ENABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_ENABLE;
  HAL_DSI_ConfigCommand(&hdsi, &LPCmd);

  /* Configure DSI PHY HS2LP and LP2HS timings */
  PhyTimings.ClockLaneHS2LPTime = 35;
  PhyTimings.ClockLaneLP2HSTime = 35;
  PhyTimings.DataLaneHS2LPTime = 35;
  PhyTimings.DataLaneLP2HSTime = 35;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 10;
  HAL_DSI_ConfigPhyTimer(&hdsi, &PhyTimings);
  
  /* Initialize LTDC */
  LTDC_Init();

  /* Start DSI */
  HAL_DSI_Start(&(hdsi));

  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
   *  depending on configuration set in 'hdsivideo_handle'.
   */

  /* Send Display off DCS Command to display */
  HAL_DSI_ShortWrite(&(hdsi),
                     0,
                     DSI_DCS_SHORT_PKT_WRITE_P1,
                     OTM8009A_CMD_DISPOFF,
                     0x00);

  OTM8009A_Init(OTM8009A_FORMAT_RBG565, LCD_ORIENTATION_LANDSCAPE);

  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_DISABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_DISABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_DISABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_DISABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_DISABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_DISABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_DISABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_DISABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_DISABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_DISABLE;
  HAL_DSI_ConfigCommand(&hdsi, &LPCmd);

  HAL_DSI_ConfigFlowControl(&hdsi, DSI_FLOW_CONTROL_BTA);

  /* Enable GPIOJ clock */
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  /* Configure DSI_TE pin from MB1166 : Tearing effect on separated GPIO from KoD LCD */
  /* that is mapped on GPIOJ2 as alternate DSI function (DSI_TE)                      */
  /* This pin is used only when the LCD and DSI link is configured in command mode    */
  /* Not used in DSI Video mode.                                                      */
  GPIO_Init_Structure.Pin       = GPIO_PIN_2;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DSI;
  HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);
}

/**
  * @brief  Initialise the LCD Controller
  * @param  LayerIndex : layer Index.
  * @retval None
  */
static void LCD_LL_LayerInit(U32 LayerIndex, U32 address)  
{  
  static uint32_t       LUT[256];
  uint32_t              i;

  if (LayerIndex < GUI_NUM_LAYERS)
  {
    LTDC_LayerCfgTypeDef  Layercfg;
    /* Layer Init */
    Layercfg.WindowX0 = 0;
    Layercfg.WindowX1 = HACT;
    Layercfg.WindowY0 = 0;
    Layercfg.WindowY1 = YSIZE_PHYS; 
    Layercfg.PixelFormat = _GetPixelformat(LayerIndex);
    Layercfg.FBStartAdress = address;
    Layercfg.Alpha = 255;
    Layercfg.Alpha0 = 0;
    Layercfg.Backcolor.Blue = 0;
    Layercfg.Backcolor.Green = 0;
    Layercfg.Backcolor.Red = 0;
    Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    Layercfg.ImageWidth = HACT;
    Layercfg.ImageHeight = YSIZE_PHYS;
    
    HAL_LTDC_ConfigLayer(&hltdc, &Layercfg, LayerIndex);

    /* Enable LUT on demand */
    if (LCD_GetBitsPerPixelEx(LayerIndex) <= 8)
    {
      /* Enable usage of LUT for all modes with <= 8bpp*/
      HAL_LTDC_EnableCLUT(&hltdc, LayerIndex);
    }
    else
    {
      /* Optional CLUT initialization for AL88 mode (16bpp)*/
      if (layer_prop[LayerIndex].pColorConvAPI == GUICC_88666I)
      {
        HAL_LTDC_EnableCLUT(&hltdc, LayerIndex);

        for (i = 0; i < 256; i++)
        {
          LUT[i] = LCD_API_ColorConv_8666.pfIndex2Color(i);
        }
        HAL_LTDC_ConfigCLUT(&hltdc, LUT, 256, LayerIndex);
      }
    }
  }  
}
  
/**
  * @brief  DMA2D interrupt handler
  * @param  None
  * @retval None
  */
void DMA2D_IRQHandler(void) 
{
  /* Clear the Transfer complete interrupt */
  DMA2D->IFCR = (U32)DMA2D_IFSR_CTCIF;
  DMA2D->IFCR = (U32)DMA2D_IFCR_CCTCIF;
  /* Release the DMA2D for the next transfer */
  TransferInProgress = 0;
}

/**
  * @brief  DMA2D wait for interrupt
  * @param  None
  * @retval None
  */
static void _DMA2D_ExecOperation(void) 
{  
  /* If no concurrent operation, do not wait */
  if ( TransferInProgress == 0 )
    return;
  
  /* Wait until the transfer is done */
  while(TransferInProgress) {
  }
}

/**
  * @brief  DMA2D Copy buffers
  * @param  LayerIndex: Layer index
  * @param  pSrc      : Source buffer pointer
  * @param  pDst      : Destination buffer pointer
  * @param  xSize     : Horizontal size
  * @param  ySize     : Vertical size
  * @param  OffLineSrc: Source Line offset
  * @param  OffLineDst: Destination Line offset
  * @retval None
  */
static void _DMA_Copy(int LayerIndex, void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst) 
{
  U32 PixelFormat;
  
  /* Wait if there is a transfer in progress */
  _DMA2D_ExecOperation();  
  
  /* Take the TransferInProgress flag */
  TransferInProgress = 1;
  
  /* Get the layer pixel format used */
  PixelFormat    = _GetPixelformat(LayerIndex);
  
  /* Setup DMA2D Configuration */  
  DMA2D->CR      = 0x00000000UL | (1 << 9);
  DMA2D->FGMAR   = (U32)pSrc;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->FGOR    = OffLineSrc;
  DMA2D->OOR     = OffLineDst;
  DMA2D->FGPFCCR = PixelFormat;
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;
  
  /* Start the transfer, and enable the transfer complete IT */  
  DMA2D->CR     |= (1|DMA2D_IT_TC);
  
  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();
}

/**
  * @brief  DMA2D Copy RGB 565 buffer
  * @param  pSrc      : Source buffer pointer
  * @param  pDst      : Destination buffer pointer
  * @param  xSize     : Horizontal size
  * @param  ySize     : Vertical size
  * @param  OffLineSrc: Source Line offset
  * @param  OffLineDst: Destination Line offset
  * @retval None
  */
static void _DMA_CopyRGB565(const void * pSrc, void * pDst, int xSize, int ySize, int OffLineSrc, int OffLineDst)
{
   /* Wait if there is a transfer in progress */
  _DMA2D_ExecOperation();
  
  /* Take the TransferInProgress flag */ 
  TransferInProgress = 1;

  /* Setup DMA2D Configuration */  
  DMA2D->CR      = 0x00000000UL | (1 << 9);
  DMA2D->FGMAR   = (U32)pSrc;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->FGOR    = OffLineSrc;
  DMA2D->OOR     = OffLineDst;
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_RGB565;
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;
  
  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR     |= (1|DMA2D_IT_TC);
  
  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation(); 
}

/**
  * @brief  DMA2D Fill buffer
  * @param  LayerIndex: Layer index
  * @param  pDst      : Destination buffer pointer
  * @param  xSize     : Horizontal size
  * @param  ySize     : Vertical size
  * @param  OffLineSrc: Source Line offset
  * @param  OffLineDst: Destination Line offset
  * @param  ColorIndex: Color to be used for the Fill operation
  * @retval None
  */
static void _DMA_Fill(int LayerIndex, void * pDst, int xSize, int ySize, int OffLine, U32 ColorIndex) 
{
  U32 PixelFormat;
  
  /* Wait if there is a transfer in progress */
  _DMA2D_ExecOperation();
  
  /* Take the TransferInProgress flag */
  TransferInProgress = 1;

  /* Get the layer pixel format used */
  PixelFormat = _GetPixelformat(LayerIndex);

  /* Setup DMA2D Configuration */  
  DMA2D->CR      = 0x00030000UL | (1 << 9);
  DMA2D->OCOLR   = ColorIndex;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->OOR     = OffLine;
  DMA2D->OPFCCR  = PixelFormat;
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;
  
  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR     |= (1|DMA2D_IT_TC);
  
  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation(); 
}

/**
  * @brief  DMA2D Alpha blending bulk
  * @param  pColorFG : Foregroung color
  * @param  pColorBG : Background color
  * @param  pColorDst: Destination color
  * @param  NumItems : Number of lines
  * @retval None
  */
static void _DMA_AlphaBlendingBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) 
{  
  /* Wait if there is a transfer in progress */
  _DMA2D_ExecOperation();
  
  /* Take the TransferInProgress flag */
  TransferInProgress = 1;
  
  /* Setup DMA2D Configuration */  
  DMA2D->CR      = 0x00020000UL | (1 << 9);
  DMA2D->FGMAR   = (U32)pColorFG;
  DMA2D->BGMAR   = (U32)pColorBG;
  DMA2D->OMAR    = (U32)pColorDst;
  DMA2D->FGOR    = 0;
  DMA2D->BGOR    = 0;
  DMA2D->OOR     = 0;
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
  DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
  DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_ARGB8888;
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;
  
  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR     |= (1|DMA2D_IT_TC);
  
  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation(); 
}

/**
  * @brief  Mixing bulk colors
  * @param  pColorFG : Foregroung color
  * @param  pColorBG : Background color
  * @param  pColorDst: Destination color
  * @param  Intens   : Color intensity
  * @param  NumItems : Number of lines
  * @retval None
  */
static void _DMA_MixColorsBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U8 Intens, U32 NumItems) 
{
  /* Wait if there is a transfer in progress */
  _DMA2D_ExecOperation();
  
  /* Take the TransferInProgress flag */
  TransferInProgress = 1;

  /* Setup DMA2D Configuration */
  DMA2D->CR      = 0x00020000UL | (1 << 9);
  DMA2D->FGMAR   = (U32)pColorFG;
  DMA2D->BGMAR   = (U32)pColorBG;
  DMA2D->OMAR    = (U32)pColorDst;
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888
                 | (1UL << 16)
                 | ((U32)Intens << 24);
  DMA2D->BGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888
                 | (0UL << 16)
                 | ((U32)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_PIXEL_FORMAT_ARGB8888;
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;
  
  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR     |= (1|DMA2D_IT_TC);

  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();
}

/**
  * @brief  Color conversion
  * @param  pSrc          : Source buffer
  * @param  pDst          : Destination buffer
  * @param  PixelFormatSrc: Source pixel format
  * @param  PixelFormatDst: Destination pixel format
  * @param  NumItems      : Number of lines
  * @retval None
  */
static void _DMA_ConvertColor(void * pSrc, void * pDst,  U32 PixelFormatSrc, U32 PixelFormatDst, U32 NumItems) 
{
  /* Wait if there is a transfer in progress */
  _DMA2D_ExecOperation();
  
  /* Take the TransferInProgress flag */
  TransferInProgress = 1;

  /* Setup DMA2D Configuration */
  DMA2D->CR      = 0x00010000UL | (1 << 9);
  DMA2D->FGMAR   = (U32)pSrc;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->FGOR    = 0;
  DMA2D->OOR     = 0;
  DMA2D->FGPFCCR = PixelFormatSrc;
  DMA2D->OPFCCR  = PixelFormatDst;
  DMA2D->NLR     = (U32)(NumItems << 16) | 1;
  
  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR     |= (1|DMA2D_IT_TC);
  
  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();  
}

/**
  * @brief  Draw L8 picture
  * @param  pSrc          : Source buffer
  * @param  pDst          : Destination buffer
  * @param  OffSrc        : Source Offset
  * @param  OffDst        : Destination Offset
  * @param  PixelFormatDst: Destination pixel format
  * @param  xSize         : Picture horizontal size
  * @param  ySize         : Picture vertical size
  * @retval None
  */
static void _DMA_DrawBitmapL8(void * pSrc, void * pDst,  U32 OffSrc, U32 OffDst, U32 PixelFormatDst, U32 xSize, U32 ySize) 
{
  /* Wait if there is a transfer in progress */
  _DMA2D_ExecOperation();
  
  /* Take the TransferInProgress flag */
  TransferInProgress = 1;

  /* Setup DMA2D Configuration */
  DMA2D->CR      = 0x00010000UL | (1 << 9);
  DMA2D->FGMAR   = (U32)pSrc;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->FGOR    = OffSrc;
  DMA2D->OOR     = OffDst;
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_L8;
  DMA2D->OPFCCR  = PixelFormatDst;
  DMA2D->NLR     = (U32)(xSize << 16) | ySize;
  
  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR     |= (1|DMA2D_IT_TC);
  
  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation(); 
}

/**
  * @brief  Draw A4 picture
  * @param  pSrc          : Source buffer
  * @param  pDst          : Destination buffer
  * @param  OffSrc        : Source Offset
  * @param  OffDst        : Destination Offset
  * @param  PixelFormatDst: Destination pixel format
  * @param  xSize         : Picture horizontal size
  * @param  ySize         : Picture vertical size
  * @retval int
  */
static int _DMA_DrawBitmapA4(void * pSrc, void * pDst,  U32 OffSrc, U32 OffDst, U32 PixelFormatDst, U32 xSize, U32 ySize) 
{
  U8 * pRD;
  U8 * pWR;
  U32 NumBytes, Color, Index;

  /* Check size of conversion buffer */
  NumBytes = ((xSize + 1) & ~1) * ySize;
  if ((NumBytes > sizeof(_aBuffer)) || (NumBytes == 0)) 
  {
    return 1;
  }
  
  /* Wait if there is a transfer in progress */ 
  _DMA2D_ExecOperation();
  
  /* Take the TransferInProgress flag */
  TransferInProgress = 1;

  /* Conversion (swapping nibbles) */
  pWR = (U8 *)_aBuffer;
  pRD = (U8 *)pSrc;
  do 
  {
    *pWR++ = _aMirror[*pRD++];
  } while (--NumBytes);
  
  /* Get current drawing color (ARGB) */
  Index = LCD_GetColorIndex();
  Color = LCD_Index2Color(Index);

  /* Setup DMA2D Configuration */
  DMA2D->CR = 0x00020000UL;
  DMA2D->FGCOLR  = Color;
  DMA2D->FGMAR   = (U32)_aBuffer;
  DMA2D->FGOR    = 0;
  DMA2D->FGPFCCR = 0xA;
  DMA2D->NLR     = (U32)((xSize + OffSrc) << 16) | ySize;
  DMA2D->BGMAR   = (U32)pDst;
  DMA2D->BGOR    = OffDst - OffSrc;
  DMA2D->BGPFCCR = PixelFormatDst;
  DMA2D->OMAR    = DMA2D->BGMAR;
  DMA2D->OOR     = DMA2D->BGOR;
  DMA2D->OPFCCR  = DMA2D->BGPFCCR;
  
  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR     |= (1|DMA2D_IT_TC);
  
  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation(); 
  
  return 0;
}

/**
  * @brief  Draw alpha bitmap
  * @param  pDst       : Destination buffer
  * @param  pSrc       : Source buffer
  * @param  xSize      : Picture horizontal size
  * @param  ySize      : Picture vertical size
  * @param  OffLineSrc : Source line offset
  * @param  OffLineDst : Destination line offset
  * @param  PixelFormat: Pixel format
  * @retval None
  */
static void _DMA_DrawAlphaBitmap(void * pDst, const void * pSrc, int xSize, int ySize, int OffLineSrc, int OffLineDst, int PixelFormat) 
{
  /* Wait if there is a transfer in progress */ 
  _DMA2D_ExecOperation();
  
  /* Take the TransferInProgress flag */
  TransferInProgress = 1;
  
  /* Setup DMA2D Configuration */ 
  DMA2D->CR      = 0x00020000UL | (1 << 9);
  DMA2D->FGMAR   = (U32)pSrc;
  DMA2D->BGMAR   = (U32)pDst;
  DMA2D->OMAR    = (U32)pDst;
  DMA2D->FGOR    = OffLineSrc;
  DMA2D->BGOR    = OffLineDst;
  DMA2D->OOR     = OffLineDst;
  DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_ARGB8888;
  DMA2D->BGPFCCR = PixelFormat;
  DMA2D->OPFCCR  = PixelFormat;
  DMA2D->NLR     = (U32)(xSize << 16) | (U16)ySize;

  /* Start the transfer, and enable the transfer complete IT */
  DMA2D->CR     |= (1|DMA2D_IT_TC);
  
  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();
}

/**
  * @brief  Load LUT
  * @param  pColor  : CLUT address
  * @param  NumItems: Number of items to load
  * @retval None
  */
static void _DMA_LoadLUT(LCD_COLOR * pColor, U32 NumItems)
{
  /* Wait if there is a transfer in progress */ 
  _DMA2D_ExecOperation();
  
  /* Take the TransferInProgress flag */
  TransferInProgress = 1;
  /* Setup DMA2D Configuration */
  DMA2D->FGCMAR  = (U32)pColor;
  DMA2D->FGPFCCR  = LTDC_PIXEL_FORMAT_ARGB8888
                  | ((NumItems - 1) & 0xFF) << 8;
  
    DMA2D->CR     |= DMA2D_IT_CTC;
    
  /* Start loading */
  DMA2D->FGPFCCR |= (1 << 5);
  
  /* Wait for the end of the transfer */
  _DMA2D_ExecOperation();
}

/**
  * @brief  DMA2D Alpha blending
  * @param  pColorFG : Foreground color
  * @param  pColorBG : Background color
  * @param  pColorDst: Destination color
  * @param  NumItems : Number of items
  * @retval None
  */
static void _DMA_AlphaBlending(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems) 
{
  _DMA_AlphaBlendingBulk(pColorFG, pColorBG, pColorDst, NumItems);
}

/**
  * @brief  Converts the given index values to 32 bit colors.
  * @param  pIndex     : Index value
  * @param  pColor     : Color relative to the index
  * @param  NumItems   : Number of items
  * @param  SizeOfIndex: Size of index color
  * @param  PixelFormat: Pixel format
  * @retval None
  */
static void _DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat) 
{
  _DMA_ConvertColor(pIndex, pColor, PixelFormat, LTDC_PIXEL_FORMAT_ARGB8888, NumItems);
}

/**
  * @brief  Converts a 32 bit colors to an index value.
  * @param  pColor     : Color relative to the index
  * @param  pIndex     : Index value
  * @param  NumItems   : Number of items
  * @param  SizeOfIndex: Size of index color
  * @param  PixelFormat: Pixel format
  * @retval None
  */
static void _DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex, U32 PixelFormat)
{
  _DMA_ConvertColor(pColor, pIndex, LTDC_PIXEL_FORMAT_ARGB8888, PixelFormat, NumItems);
}

/**
  * @brief  LCD Mix color bulk.
  * @param  pFG    : Foreground buffer
  * @param  pBG    : Background buffer
  * @param  pDst   : Destination buffer
  * @param  OffFG  : Foreground offset
  * @param  OffBG  : Background offset
  * @param  OffDest: Destination offset
  * @param  xSize  : Horizontal size
  * @param  ySize  : Vertical size
  * @param  Intens : Color Intensity
  * @retval None
  */
static void _LCD_MixColorsBulk(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens) 
{
  int y;
  
  GUI_USE_PARA(OffFG);
  GUI_USE_PARA(OffDest);
   
  /* Do the same operation for all the lines */
  for (y = 0; y < ySize; y++) 
  {
    /* Use DMA2D for mixing up */
    _DMA_MixColorsBulk(pFG, pBG, pDst, Intens, xSize);
    pFG  += xSize + OffFG;
    pBG  += xSize + OffBG;
    pDst += xSize + OffDest;
  }
}

/**
  * @brief  Get buffer size.
  * @param  LayerIndex: Layer index
  * @retval U32       : Buffer size 
  */
static U32 _GetBufferSize(int LayerIndex) 
{
  U32 BufferSize;

  BufferSize = _axSize[LayerIndex] * _aySize[LayerIndex] * _aBytesPerPixels[LayerIndex];
  
  return BufferSize;
}

/**
  * @brief  LCD Copy buffer
  * @param  LayerIndex: Layer index
  * @param  IndexSrc  : Source index
  * @param  IndexDst  : Destination index
  * @retval None 
  */
static void _LCD_CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst) 
{
  U32 BufferSize, AddrSrc, AddrDst;
  
  BufferSize = _GetBufferSize(LayerIndex);
  AddrSrc    = _aAddr[LayerIndex] + BufferSize * IndexSrc;
  AddrDst    = _aAddr[LayerIndex] + BufferSize * IndexDst;
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, _axSize[LayerIndex], _aySize[LayerIndex], 0, 0);
  /* After this function has been called all drawing operations are routed to Buffer[IndexDst] */
  _aBufferIndex[LayerIndex] = IndexDst;
}

/**
  * @brief  LCD Copy rectangle
  * @param  LayerIndex: Layer index
  * @param  x0        : Horizontal rect origin
  * @param  y0        : Vertical rect origin
  * @param  x1        : Horizontal rect end
  * @param  y1        : Vertical rect end
  * @param  xSize     : Rectangle width
  * @param  ySize     : Rectangle height
  * @retval None 
  */
static void _LCD_CopyRect(int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize)
{
  U32 BufferSize, AddrSrc, AddrDst;
  int OffLine;

  BufferSize = _GetBufferSize(LayerIndex);
  AddrSrc = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y0 * _axSize[LayerIndex] + x0) * _aBytesPerPixels[LayerIndex];
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y1 * _axSize[LayerIndex] + x1) * _aBytesPerPixels[LayerIndex];
  OffLine = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, xSize, ySize, OffLine, OffLine);
}

/**
  * @brief  LCD fill rectangle
  * @param  LayerIndex: Layer index
  * @param  x0        : Horizontal rect origin
  * @param  y0        : Vertical rect origin
  * @param  x1        : Horizontal rect end
  * @param  y1        : Vertical rect end
  * @param  PixelIndex: Color to be used for the Fill operation
  * @retval None 
  */
static void _LCD_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex) 
{
  U32 BufferSize, AddrDst;
  int xSize, ySize;
  
  /* Depending on the draw mode, do it differently */
  if (GUI_GetDrawMode() == GUI_DM_XOR) 
  {
    /* Use SW Fill rectangle */
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, NULL);
    LCD_FillRect(x0, y0, x1, y1);
    /* Then set custom callback function for fillrect operation */
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);
  }
  else
  {
    xSize = x1 - x0 + 1;
    ySize = y1 - y0 + 1;
    BufferSize = _GetBufferSize(LayerIndex);
    AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y0 * _axSize[LayerIndex] + x0) * _aBytesPerPixels[LayerIndex];
    _DMA_Fill(LayerIndex, (void *)AddrDst, xSize, ySize, _axSize[LayerIndex] - xSize, PixelIndex);
  }
}

/**
  * @brief  Draw 32 bits per pixel bitmap
  * @param  LayerIndex  : Layer index
  * @param  x           : start horizontal position on the screen
  * @param  y           : start vertical position on the screen
  * @param  p           : Source buffer
  * @param  xSize       : Horizontal bitmap size
  * @param  ySize       : Vertical bitmap size
  * @param  BytesPerLine: Number of bytes per Line
  * @retval None 
  */
static void _LCD_DrawBitmap32bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine) 
{
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
 
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine / 4) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/**
  * @brief  Draw 16 bits per pixel bitmap
  * @param  LayerIndex  : Layer index
  * @param  x           : start horizontal position on the screen
  * @param  y           : start vertical position on the screen
  * @param  p           : Source buffer
  * @param  xSize       : Horizontal bitmap size
  * @param  ySize       : Vertical bitmap size
  * @param  BytesPerLine: Number of bytes per Line
  * @retval None 
  */
static void _LCD_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine) 
{
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
 
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine / 2) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/**
  * @brief  Draw 8 bits per pixel bitmap
  * @param  LayerIndex  : Layer index
  * @param  x           : start horizontal position on the screen
  * @param  y           : start vertical position on the screen
  * @param  p           : Source buffer
  * @param  xSize       : Horizontal bitmap size
  * @param  ySize       : Vertical bitmap size
  * @param  BytesPerLine: Number of bytes per Line
  * @retval None 
  */
static void _LCD_DrawBitmap8bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine)
{
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;
  
  PixelFormat = _GetPixelformat(LayerIndex);
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = BytesPerLine - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_DrawBitmapL8((void *)p, (void *)AddrDst, OffLineSrc, OffLineDst, PixelFormat, xSize, ySize);
}

/**
  * @brief  Draw 4 bits per pixel bitmap
  * @param  LayerIndex  : Layer index
  * @param  x           : start horizontal position on the screen
  * @param  y           : start vertical position on the screen
  * @param  p           : Source buffer
  * @param  xSize       : Horizontal bitmap size
  * @param  ySize       : Vertical bitmap size
  * @param  BytesPerLine: Number of bytes per Line
  * @retval int 
  */
static int _LCD_DrawBitmap4bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine) 
{
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;
  
  if (x < 0) 
  {
    return 1;
  }
  if ((x + xSize) >= _axSize[LayerIndex]) 
  {
    return 1;
  }
  if (y < 0) 
  {
    return 1;
  }
  if ((y + ySize) >= _aySize[LayerIndex]) 
  {
    return 1;
  }
  PixelFormat = _GetPixelformat(LayerIndex);
  
  /* Check if destination has direct color mode */
  if (PixelFormat > LTDC_PIXEL_FORMAT_ARGB4444) 
  {
    return 1;
  }
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine * 2) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  return _DMA_DrawBitmapA4((void *)p, (void *)AddrDst, OffLineSrc, OffLineDst, PixelFormat, xSize, ySize);;
}

/**
  * @brief  Draw 16 bits per pixel memory device
  * @param  pDst           : Destination buffer
  * @param  pSrc           : Source buffer
  * @param  xSize          : Horizontal memory device size
  * @param  ySize          : Vertical memory device size
  * @param  BytesPerLineDst: Destination number of bytes per Line
  * @param  BytesPerLineSrc: Source number of bytes per Line
  * @retval None 
  */
static void _LCD_DrawMemdev16bpp(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc) 
{
  int OffLineSrc, OffLineDst;
 
  OffLineSrc = (BytesPerLineSrc / 2) - xSize;
  OffLineDst = (BytesPerLineDst / 2) - xSize;
  _DMA_CopyRGB565(pSrc, pDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/**
  * @brief  Draw alpha memory device
  * @param  pDst           : Destination buffer
  * @param  pSrc           : Source buffer
  * @param  xSize          : Horizontal memory device size
  * @param  ySize          : Vertical memory device size
  * @param  BytesPerLineDst: Destination number of bytes per Line
  * @param  BytesPerLineSrc: Source number of bytes per Line
  * @retval None 
  */
static void _LCD_DrawMemdevAlpha(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc) 
{
  int OffLineSrc, OffLineDst;
 
  OffLineSrc = (BytesPerLineSrc / 4) - xSize;
  OffLineDst = (BytesPerLineDst / 4) - xSize;
  _DMA_DrawAlphaBitmap(pDst, pSrc, xSize, ySize, OffLineSrc, OffLineDst, LTDC_PIXEL_FORMAT_ARGB8888);
}

/**
  * @brief  Draw alpha Bitmap
  * @param  LayerIndex  : Layer index
  * @param  x           : Horizontal position on the screen
  * @param  y           : vertical position on the screen
  * @param  xSize       : Horizontal bitmap size
  * @param  ySize       : Vertical bitmap size
  * @param  BytesPerLine: Bytes per Line
  * @retval None 
  */
static void _LCD_DrawBitmapAlpha(int LayerIndex, int x, int y, const void * p, int xSize, int ySize, int BytesPerLine) 
{
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  U32 PixelFormat;

  PixelFormat = _GetPixelformat(LayerIndex);
  BufferSize = _GetBufferSize(LayerIndex);
  AddrDst = _aAddr[LayerIndex] + BufferSize * _aBufferIndex[LayerIndex] + (y * _axSize[LayerIndex] + x) * _aBytesPerPixels[LayerIndex];
  OffLineSrc = (BytesPerLine / 4) - xSize;
  OffLineDst = _axSize[LayerIndex] - xSize;
  _DMA_DrawAlphaBitmap((void *)AddrDst, p, xSize, ySize, OffLineSrc, OffLineDst, PixelFormat);
}


/**
  * @brief  Translates the given colors into index values for the display controller
  * @param  pLogPal   : Palette 
  * @param  pBitmap   : Bitmap
  * @param  LayerIndex: Layer index
  * @retval LCD_PIXELINDEX 
  */
static LCD_PIXELINDEX * _LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, const GUI_BITMAP GUI_UNI_PTR * pBitmap, int LayerIndex) 
{
  void (* pFunc)(void);
  int DoDefault = 0;

  /* Check if we have a non transparent device independent bitmap */
  if (pBitmap->BitsPerPixel == 8) 
  {
    pFunc = LCD_GetDevFunc(LayerIndex, LCD_DEVFUNC_DRAWBMP_8BPP);
    if (pFunc) 
    {
      if (pBitmap->pPal) 
      {
        if (pBitmap->pPal->HasTrans) 
        {
          DoDefault = 1;
        }
      }
      else
      {
        DoDefault = 1;
      }
    }
    else
    {
      DoDefault = 1;
    }
  }
  else 
  {
    DoDefault = 1;
  }
  
  /* Default palette management for other cases */
  if (DoDefault) 
  {
    /* Return a pointer to the index values to be used by the controller */
    return LCD_GetpPalConvTable(pLogPal);
  }

  /* Load LUT using DMA2D */
  _DMA_LoadLUT((U32 *)pLogPal->pPalEntries, pLogPal->NumEntries);
  
  /* Return something not NULL */
  return _pBuffer_DMA2D;
}

/**
  * @brief  Set LCD updated region.
  * @param  idx: active region index.
  * @retval None
  */
void LCD_SetUpdateRegion(int idx)
{
  HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pCols[idx]);
}


/**
  * @brief  Tearing Effect DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_TearingEffectCallback(DSI_HandleTypeDef *hdsi)
{
  uint32_t index = 0;
  
  __DSI_MASK_TE();
  
  for(index = 0; index < GUI_NUM_LAYERS; index++)
  {
	  if(_aPendingBuffer[index] >= 0)
      {
        GUI_MULTIBUF_ConfirmEx(index,  _aPendingBuffer[index]);
        _aPendingBuffer[index] = -1;
      } 
  }    
  LCD_ActiveRegion = 1;
  HAL_DSI_Refresh(hdsi); 
}  

/**
  * @brief  End of Refresh DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
  uint32_t index = 0;
  static uint32_t TestAdd = 0;
  if(LCD_ActiveRegion < ZONES )
  {
    /* Disable DSI Wrapper */
    __HAL_DSI_WRAPPER_DISABLE(hdsi);
    for(index = 0; index < GUI_NUM_LAYERS; index ++)
    {
      TestAdd = LCD_Addr[index];
      /* Update LTDC configuration */      
      LTDC_LAYER(&hltdc, index)->CFBAR  = TestAdd + LCD_ActiveRegion  * HACT * 2;
    }
    __HAL_LTDC_RELOAD_CONFIG(&hltdc);
    __HAL_DSI_WRAPPER_ENABLE(hdsi);
    LCD_SetUpdateRegion(LCD_ActiveRegion++);
    /* Refresh the right part of the display */
    HAL_DSI_Refresh(hdsi);
  }
  else
  {
    __HAL_DSI_WRAPPER_DISABLE(hdsi);
    for(index = 0; index < GUI_NUM_LAYERS; index ++)
    {
      LTDC_LAYER(&hltdc, index)->CFBAR  = TestAdd;
    }
    __HAL_LTDC_RELOAD_CONFIG(&hltdc);
    __HAL_DSI_WRAPPER_ENABLE(hdsi);  
    LCD_SetUpdateRegion(0); 
    LCD_Refershing = 0;
   }
}

/**
  * @brief  Request TE at scanline.
  * @param  None
  * @retval None
  */
void LCD_ReqTear(void)
{
  static uint8_t ScanLineParams[2];
#if (ZONES == 4 )
  uint16_t scanline = 283;
#elif (ZONES == 2 )
  uint16_t scanline = 533;
#endif
  ScanLineParams[0] = scanline >> 8;
  ScanLineParams[1] = scanline & 0x00FF;
  
  HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 2, 0x44, ScanLineParams);     
  /* set_tear_on */
  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_TEEON, 0x00);  
}


/**
  * @brief  Function is called by the display driver for several purposes
  * @param  LayerIndex: Index of layer to be configured
  * @param  Cmd       : Commands
  * @param  pData     : Pointer to a LCD_X_DATA structure
  * @retval int 
  */
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) 
{
  int r = 0;
  U32 addr;

  switch (Cmd) 
  {
    case LCD_X_INITCONTROLLER: 
    {
      /* Called during the initialization process in order to set up the display controller and put it into operation */
      break;
    }
    case LCD_X_SETORG: 
    {
      /* Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p */
      LCD_X_SETORG_INFO * p;
      
      p = (LCD_X_SETORG_INFO *)pData;
      addr = _aAddr[LayerIndex] + p->yPos * _axSize[LayerIndex] * _aBytesPerPixels[LayerIndex];
      HAL_LTDC_SetAddress(&hltdc, addr, LayerIndex);
      break;
    }
    case LCD_X_SHOWBUFFER: 
    {
      /* Required if multiple buffers are used. The 'Index' element of p contains the buffer index. */
      LCD_X_SHOWBUFFER_INFO * p;
      
      p = (LCD_X_SHOWBUFFER_INFO *)pData;
      
      /* UnMask the tearing effect */
      __DSI_UNMASK_TE();
      LCD_Refershing = 1;
      /* Wait for the LCD_Refreshing */
      while(LCD_Refershing);
      
      /* Update the index of the buffer to use */
      LCD_Addr[LayerIndex] = _aAddr[LayerIndex] + _axSize[LayerIndex] * _aySize[LayerIndex] * _aBytesPerPixels[LayerIndex] * p->Index;
      _aPendingBuffer[LayerIndex] = p->Index;
      
     __HAL_DSI_WRAPPER_DISABLE(&hdsi);

      LTDC_LAYER(&hltdc, LayerIndex)->CFBAR  = LCD_Addr[LayerIndex];
    __HAL_LTDC_RELOAD_CONFIG(&hltdc);
    __HAL_DSI_WRAPPER_ENABLE(&hdsi);
      break;
    }
    case LCD_X_SETLUTENTRY: 
    {
      /* Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p */
      LCD_X_SETLUTENTRY_INFO * p;
      
      p = (LCD_X_SETLUTENTRY_INFO *)pData;
      HAL_LTDC_ConfigCLUT(&hltdc, (uint32_t*)p->Color, p->Pos, LayerIndex);
      break;
    }
    case LCD_X_ON: 
    {
      /* Required if the display controller should support switching on and off */
      __HAL_LTDC_ENABLE(&hltdc);
      break;
    }
    case LCD_X_OFF: 
    {
      /* Required if the display controller should support switching on and off */
      __HAL_LTDC_DISABLE(&hltdc);
      break;
    }
    case LCD_X_SETVIS: 
    {
      /* Required for setting the layer visibility which is passed in the 'OnOff' element of pData */
      LCD_X_SETVIS_INFO * p;
      
      p = (LCD_X_SETVIS_INFO *)pData;
      
      if(p->OnOff  == ENABLE )
      {
        __HAL_DSI_WRAPPER_DISABLE(&hdsi);
        __HAL_LTDC_LAYER_ENABLE(&hltdc, LayerIndex); 
        __HAL_DSI_WRAPPER_ENABLE(&hdsi);
      }
      else
      {
        __HAL_DSI_WRAPPER_DISABLE(&hdsi);
        __HAL_LTDC_LAYER_DISABLE(&hltdc, LayerIndex); 
        __HAL_DSI_WRAPPER_ENABLE(&hdsi);
      }
      __HAL_LTDC_RELOAD_CONFIG(&hltdc); 

      HAL_DSI_Refresh(&hdsi);
      break;
    }
    case LCD_X_SETPOS:
    {
      /* Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData */
      LCD_X_SETPOS_INFO * p;
      
      p = (LCD_X_SETPOS_INFO *)pData;    
      HAL_LTDC_SetWindowPosition(&hltdc, p->xPos, p->yPos, LayerIndex);
      break;
    }
    case LCD_X_SETSIZE:
    {
      /* Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData */
      LCD_X_SETSIZE_INFO * p;
      int xPos, yPos;

      GUI_GetLayerPosEx(LayerIndex, &xPos, &yPos);
      p = (LCD_X_SETSIZE_INFO *)pData;
      if (LCD_GetSwapXYEx(LayerIndex))
      {
        _axSize[LayerIndex] = p->ySize;
        _aySize[LayerIndex] = p->xSize;
      }
      else
      {
        _axSize[LayerIndex] = p->xSize;
        _aySize[LayerIndex] = p->ySize;
      }
      HAL_LTDC_SetWindowPosition(&hltdc, xPos, yPos, LayerIndex);
      break;
    }
    case LCD_X_SETALPHA: 
    {
      /* Required for setting the alpha value which is passed in the 'Alpha' element of pData */
      LCD_X_SETALPHA_INFO * p;

      p = (LCD_X_SETALPHA_INFO *)pData;
      HAL_LTDC_SetAlpha(&hltdc, p->Alpha, LayerIndex);
      break;
    }
    case LCD_X_SETCHROMAMODE: 
    {
      /* Required for setting the chroma mode which is passed in the 'ChromaMode' element of pData */
      LCD_X_SETCHROMAMODE_INFO * p;
      
      p = (LCD_X_SETCHROMAMODE_INFO *)pData;
      if(p->ChromaMode != 0)
      {
        HAL_LTDC_EnableColorKeying(&hltdc, LayerIndex);
      }
      else
      {
        HAL_LTDC_DisableColorKeying(&hltdc, LayerIndex);      
      }
      break;
    }
    case LCD_X_SETCHROMA: 
    {
      /* Required for setting the chroma value which is passed in the 'ChromaMin' and 'ChromaMax' element of pData */
      LCD_X_SETCHROMA_INFO * p;
      U32 Color;

      p = (LCD_X_SETCHROMA_INFO *)pData;
      Color = ((p->ChromaMin & 0xFF0000) >> 16) | (p->ChromaMin & 0x00FF00) | ((p->ChromaMin & 0x0000FF) << 16);
      HAL_LTDC_ConfigColorKeying(&hltdc, Color, LayerIndex);
      break;
    }
    default:
      r = -1;
      break;
    }
  
    return r;
}

/**
  * @brief  Called during the initialization process in order to set up the
  *         display driver configuration
  * @param  None
  * @retval None 
  */
void LCD_X_Config(void) 
{
  int i;
  U32 PixelFormat; 
  
  /* At first initialize use of multiple buffers on demand */
#if (NUM_BUFFERS > 1)
  for (i = 0; i < GUI_NUM_LAYERS; i++) 
  {
    GUI_MULTIBUF_ConfigEx(i, NUM_BUFFERS);
  }
#endif
  
  /* Set display driver and color conversion for 1st layer */
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_0, COLOR_CONVERSION_0, 0, 0);
  
  /* Set size of 1st layer */
  if (LCD_GetSwapXYEx(0)) 
  {
    LCD_SetSizeEx (0, YSIZE_0, XSIZE_0);
    LCD_SetVSizeEx(0, YSIZE_0 * NUM_VSCREENS, XSIZE_0);
  }
  else
  {
    LCD_SetSizeEx (0, XSIZE_0, YSIZE_0);
    LCD_SetVSizeEx(0, XSIZE_0, YSIZE_0 * NUM_VSCREENS);
  }
#if (GUI_NUM_LAYERS > 1)
  /* Set display driver and color conversion for 2nd layer */
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_1, COLOR_CONVERSION_1, 0, 1);
    
  /* Set size of 2nd layer */
  if (LCD_GetSwapXYEx(1)) 
  {
    LCD_SetSizeEx (1, YSIZE_0, XSIZE_1);
    LCD_SetVSizeEx(1, YSIZE_1 * NUM_VSCREENS, XSIZE_1);
  }
  else 
  {
    LCD_SetSizeEx (1, XSIZE_1, YSIZE_1);
    LCD_SetVSizeEx(1, XSIZE_1, YSIZE_1 * NUM_VSCREENS);
  }
#endif
  /* Setting up VRam address and get the pixel size */
  for (i = 0; i < GUI_NUM_LAYERS; i++) 
  {
    /* Setting up VRam address */
    LCD_SetVRAMAddrEx(i, (void *)(_aAddr[i]));
    /* Get the pixel size */
    _aBytesPerPixels[i] = LCD_GetBitsPerPixelEx(i) >> 3;
  }
  /* Setting up custom functions */
  for (i = 0; i < GUI_NUM_LAYERS; i++) 
  {
    PixelFormat = _GetPixelformat(i);
    /* Set custom function for copying complete buffers (used by multiple buffering) using DMA2D */
    LCD_SetDevFunc(i, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))_LCD_CopyBuffer);
    if (PixelFormat <= LTDC_PIXEL_FORMAT_ARGB4444) 
    {
      /* Set custom function for filling operations using DMA2D */
      LCD_SetDevFunc(i, LCD_DEVFUNC_FILLRECT, (void(*)(void))_LCD_FillRect);
    }
    if (_aOrientation[i] == ROTATION_0)
    {
      /* Set custom function for copy recxtangle areas (used by GUI_CopyRect()) using DMA2D */
      LCD_SetDevFunc(i, LCD_DEVFUNC_COPYRECT, (void(*)(void))_LCD_CopyRect);
      
      /* Set functions for direct color mode layers. Won't work with indexed color modes because of missing LUT for DMA2D destination */
      if (PixelFormat <= LTDC_PIXEL_FORMAT_ARGB4444) 
      {
        /* Set up custom drawing routine for index based bitmaps using DMA2D */
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_8BPP, (void(*)(void))_LCD_DrawBitmap8bpp);
      }
      
      /* Set up drawing routine for 16bpp bitmap using DMA2D */
      if (PixelFormat == LTDC_PIXEL_FORMAT_RGB565) 
      {
        /* Set up drawing routine for 16bpp bitmap using DMA2D. Makes only sense with RGB565 */
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_16BPP, (void(*)(void))_LCD_DrawBitmap16bpp);
      }
      
      /* Set up drawing routine for 32bpp bitmap using DMA2D */
      if (PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888) 
      {
        /* Set up drawing routine for 32bpp bitmap using DMA2D. Makes only sense with ARGB8888 */
        LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_32BPP, (void(*)(void))_LCD_DrawBitmap32bpp);
      }
    }
  }
  
  /* Set up custom color conversion using DMA2D, works only for direct color modes because of missing LUT for DMA2D destination */

  /* Set up custom bulk color conversion using DMA2D for ARGB1555 */
  GUICC_M1555I_SetCustColorConv(_Color2IndexBulk_M1555I_DMA2D, _Index2ColorBulk_M1555I_DMA2D);

  /* Set up custom bulk color conversion using DMA2D for RGB565 */  
  GUICC_M565_SetCustColorConv  (_Color2IndexBulk_M565_DMA2D,   _Index2ColorBulk_M565_DMA2D);

  /* Set up custom bulk color conversion using DMA2D for ARGB4444 */
  GUICC_M4444I_SetCustColorConv(_Color2IndexBulk_M4444I_DMA2D, _Index2ColorBulk_M4444I_DMA2D);

  /* Set up custom bulk color conversion using DMA2D for RGB888 */
  GUICC_M888_SetCustColorConv  (_Color2IndexBulk_M888_DMA2D,   _Index2ColorBulk_M888_DMA2D);

  /* Set up custom bulk color conversion using DMA2D for ARGB8888 */
  GUICC_M8888I_SetCustColorConv(_Color2IndexBulk_M8888I_DMA2D, _Index2ColorBulk_M8888I_DMA2D);
  
  /* Set up custom alpha blending function using DMA2D */
  GUI_SetFuncAlphaBlending(_DMA_AlphaBlending);
  
  /* Set up custom function for translating a bitmap palette into index values.
   * Required to load a bitmap palette into DMA2D CLUT in case of a 8bpp indexed bitmap
   */
  GUI_SetFuncGetpPalConvTable(_LCD_GetpPalConvTable);
  
  /* Set up custom function for mixing up arrays of colors using DMA2D */
  GUI_SetFuncMixColorsBulk(_LCD_MixColorsBulk);
  
  /* Set up custom function for drawing AA4 characters */
  GUI_AA_SetpfDrawCharAA4(_LCD_DrawBitmap4bpp);
  
#if GUI_SUPPORT_MEMDEV  
  /* Set up custom function for drawing 16bpp memory devices */
  GUI_MEMDEV_SetDrawMemdev16bppFunc(_LCD_DrawMemdev16bpp);
  
  /* Set up custom function for Alpha drawing operations */
  GUI_SetFuncDrawAlpha(_LCD_DrawMemdevAlpha, _LCD_DrawBitmapAlpha);
#endif
  
  /* Complete the initialisation */
  LCD_LL_Init ();
  
  __HAL_DSI_WRAPPER_DISABLE(&hdsi);

  LCD_LL_LayerInit(0, _aAddr[0]);
  HAL_LTDC_SetPitch(&hltdc, XSIZE_PHYS, 0);
#if (GUI_NUM_LAYERS > 1)
  LCD_LL_LayerInit(1, _aAddr[1]);
  HAL_LTDC_SetPitch(&hltdc, XSIZE_PHYS, 1);
#endif

  __HAL_DSI_WRAPPER_ENABLE(&hdsi);
  
  HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pCols[0]);
  HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_PASET, pPage);
    
  HAL_Delay(20);
  
  LCD_ReqTear();
  /* Send Display off DCS Command to display */
  HAL_DSI_ShortWrite(&(hdsi),
                     0,
                     DSI_DCS_SHORT_PKT_WRITE_P1,
                     OTM8009A_CMD_DISPON,
                     0x00);
  
}

/**
  * @brief  This function handles DSI global interrupt request.
  * @param  None
  * @retval None
  */
void DSI_IRQHandler(void)
{
  HAL_DSI_IRQHandler(&hdsi);
}
