/**
  ******************************************************************************
  * @file    Display/LCD_DSI_ImagesSlider/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use LCD DSI to display an image
  *          of size WVGA in mode landscape (800x480) using the STM32F4xx HAL API and BSP.
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
#include <string.h>
#include <stdio.h> 

#include "image1.h"
#include "image2.h"
#include "image3.h"
#include "image4.h"
#include "image5.h"
#include "image6.h"
#include "image7.h"
#include "image8.h"
#include "image9.h"
#include "image10.h"
#include "image11.h"
#include "image12.h"
#include "image13.h"
#include "image14.h"
#include "image15.h"
#include "image16.h"
#include "image17.h"
#include "image18.h"
#include "image19.h"
#include "image20.h"

/** @addtogroup STM32F4xx_HAL_Applications
  * @{
  */

/** @addtogroup LCD_DSI_ImagesSlider
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
extern LTDC_HandleTypeDef hltdc_eval;
static DMA2D_HandleTypeDef   hdma2d;
extern DSI_HandleTypeDef hdsi_eval;

/* Private define ------------------------------------------------------------*/
#define VSYNC           1  
#define VBP             1 
#define VFP             1
#define VACT            480
#define HSYNC           1
#define HBP             1
#define HFP             1
#define HACT            400      /* !!!! SCREEN DIVIDED INTO 2 AREAS !!!! */

#define LAYER0_ADDRESS  (LCD_FB_START_ADDRESS)
#define LCD_BG_LAYER_ADDRESS 0xC1300000

#define INVALID_AREA      0
#define LEFT_AREA         1
#define RIGHT_AREA        2

#define HORIZONTAL_SLIDER 0
#define VERTICAL_SLIDER   1
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t valid_buffer = 0;
static int32_t active_area = INVALID_AREA;
static uint32_t ImageIndex = 0;
static const uint32_t * image[] =
{
  image1, image2, image3, image4, image5,
  image6, image7, image8, image9, image10,
  image11, image12, image13, image14, image15,
  image16, image17, image18, image19, image20,
};

uint8_t pColLeft[]    = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
uint8_t pColRight[]   = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */
uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
uint8_t pSyncLeft[]   = {0x02, 0x15};             /* Scan @ 533 */

TS_StateTypeDef  TS_State = {0};
uint32_t x2, y2;
uint32_t x3, y3;       
uint32_t first_touch;
        
static uint8_t touch = 0;
static  uint8_t touchdetected = 0;
__IO uint32_t image_ID  = 0;
uint32_t MAX_IMAGE_ID   = 19;
uint32_t image_num = 0, draw_method = HORIZONTAL_SLIDER;
int32_t counter = 0; 

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void OnError_Handler(uint32_t condition);
static uint8_t LCD_Init(void);
void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address);
void LTDC_Init(void);
static void LL_CopyPicture(uint32_t *pSrc, uint32_t *pDst);
static void LL_DrawPicture(uint32_t *pSrc, int32_t xyPos);
static void LCD_DSI_HorizontalSlider(void);
static void LCD_DSI_VerticalSlider(void);

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

  BSP_IO_Init();
  
  /* Initialise QSPI */
  BSP_QSPI_Init();
  BSP_QSPI_EnableMemoryMappedMode(); 
  
  /* Initialize the SDRAM */
  BSP_SDRAM_Init();
  
  /* Initialize the LCD */
  lcd_status = LCD_Init();
  OnError_Handler(lcd_status != LCD_OK); 
  
  /* Initialize LTDC layer 0 iused for Hint */
  LCD_LayertInit(0, LAYER0_ADDRESS);     
  BSP_LCD_SelectLayer(0); 

  /* Configure the User Button in GPIO Mode */
  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);
  
   /* Initialize the Touch Screen */  
  BSP_TS_Init(800, 480);
   
  /* Set active display window */  
  HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft);
  HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_PASET, pPage);
 
  /* Update pitch : the draw is done on the whole physical X Size */
  HAL_LTDC_SetPitch(&hltdc_eval, BSP_LCD_GetXSize(), 0);
  
  
  /* Show first image */
  LL_CopyPicture((uint32_t *)image[ImageIndex], (uint32_t *)LAYER0_ADDRESS);
  
  valid_buffer = 1;
  active_area = LEFT_AREA;
  
  HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
   
  /* Infinite loop */
  while (1)
  {                             
    BSP_TS_GetState(&TS_State);
    if(draw_method == HORIZONTAL_SLIDER)
    {
    LCD_DSI_HorizontalSlider();
    }
    else
    {
     LCD_DSI_VerticalSlider(); 
    }
  } 
}

/**
  * @brief Manage the horizontal display of image on LCD DSI
  * @param None
  * @retval None
  */
static void LCD_DSI_HorizontalSlider(void)
{
  /*Touch Detected **********************************************************/
  if(TS_State.touchDetected)
  {
    touchdetected = 1;
    if(touch == 0)
    {
      /* Get X and Y position of the first touch post calibrated */ 
      first_touch = TS_State.touchX[0];
      touch = 1;
    }
    x3 = TS_State.touchX[0];
    /* Follows Right ********************************************************/
    if (first_touch < x3)
    {
      while(valid_buffer);
      
      if(image_ID == 0)
      {       
        LL_DrawPicture((uint32_t *)image[MAX_IMAGE_ID], (x3 - first_touch - 800));
      }
      else
      {       
        LL_DrawPicture((uint32_t *)image[image_ID - 1], (x3 - first_touch - 800));
      } 
      
      HAL_Delay(80);
      LL_DrawPicture((uint32_t *)image[image_ID], (x3 - first_touch));
      HAL_Delay(40);
      valid_buffer = 1;
      HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
      HAL_Delay(20);
    }
    /************************************************************************/
    
    /* Follows Left *********************************************************/
    else
    {
      while(valid_buffer);        
      
      LL_DrawPicture((uint32_t *)image[image_ID], -(first_touch - x3));
      HAL_Delay(80);
      if(image_ID == MAX_IMAGE_ID)
      {        
        LL_DrawPicture((uint32_t *)image[0], (800 - first_touch + x3));
      }
      else
      {  
        LL_DrawPicture((uint32_t *)image[image_ID + 1], (800 - first_touch + x3));
      }
      HAL_Delay(40);
      
      valid_buffer = 1;
      HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
      HAL_Delay(20);
    }
  }
  /**************************************************************************/
  
  /* Touch Released *********************************************************/
  if(!TS_State.touchDetected)
  {
    if(touchdetected == 1)
    {
      /* Get X and Y position of the first touch post calibrated */ 
      x2 = TS_State.touchX[0];
      /* Move Right *********************************************************/
      if(x2 > first_touch)
      {
        if(x2 > (first_touch + 100))   
        { 
          if(image_ID > 0)
          {
            image_ID--;
          }
          else
          {
            image_ID = MAX_IMAGE_ID;
          }
          
          LL_CopyPicture((uint32_t *)image[image_ID], (uint32_t *)(0xC0200000));
          
          for(counter= (x2 - first_touch -800); counter < 0; counter++)
          {
            while(valid_buffer);     
            counter += 30;
            
            if(counter < 0)
            {
              
              LL_DrawPicture((uint32_t *)(0xC0200000), counter);
              valid_buffer = 1;
              HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
              HAL_Delay(50);
            }
          }
          
          for(counter=0; counter < 2; counter++)
          {
            while(valid_buffer);     
            
            LL_DrawPicture((uint32_t *)(0xC0200000), 0);
            valid_buffer = 1;
            HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
            HAL_Delay(50);
          }
          
          touchdetected = 0;
          touch = 0;            
        }
        /********************************************************************/
        
        /* Snap Right *******************************************************/
        else
        {
          for(counter=0; counter < (x2 - first_touch + 1); counter++)
          {
            counter += 10;
            if(counter < (x2 - first_touch + 1))
            {
              while(valid_buffer);
              if(image_ID == 0)
              {
                LL_DrawPicture((uint32_t *)image[MAX_IMAGE_ID], (((x2 - first_touch) - counter) - 800));
              }
              else
              {
                LL_DrawPicture((uint32_t *)image[image_ID - 1], (((x2 - first_touch) - counter) - 800));
              }
              LL_DrawPicture((uint32_t *)image[image_ID], ((x2 - first_touch) - counter));
              valid_buffer = 1;
              HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
            }
            else
            {
              LL_DrawPicture((uint32_t *)image[image_ID], 0);
              valid_buffer = 1;
              HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
              HAL_Delay(50); 
            }
          }   
          
          for(counter=0; counter < 2; counter++)
          {
            while(valid_buffer);     
            
            LL_DrawPicture((uint32_t *)image[image_ID], 0);
            valid_buffer = 1;
            HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
            HAL_Delay(50);
          }
          
          touchdetected = 0;
          touch = 0;               
        }        
      }
      /**********************************************************************/
      
      /* Move Left **********************************************************/
      else
      {
        if(first_touch > (x2 + 100))   
        {
          if(image_ID < MAX_IMAGE_ID)
          {
            image_ID++;
          }
          else
          {
            image_ID = 0;
          }
          
          LL_CopyPicture((uint32_t *)image[image_ID], (uint32_t *)(0xC0200000));
          
          for(counter=(800 - first_touch + x2); counter > 0; counter--)
          {
            while(valid_buffer);     
            counter -= 30;
            
            if(counter > 0)
            {
              LL_DrawPicture((uint32_t *)(0xC0200000), counter);
              
              valid_buffer = 1;
              HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
              HAL_Delay(50);
            }
          }
          
          for(counter=0; counter < 1; counter++)
          {
            while(valid_buffer);     
            
            LL_DrawPicture((uint32_t *)(0xC0200000), 0);
            valid_buffer = 1;
            HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
            HAL_Delay(50);
          }
          
          touchdetected = 0;
          touch = 0;           
        }
        /********************************************************************/
        
        /* Snap Left ********************************************************/
        else
        {
          for(counter=0; counter < (first_touch - x2 + 1); counter++)
          {
            counter += 10;
            if(counter < (first_touch - x2 + 1))
            {
              while(valid_buffer);
              LL_DrawPicture((uint32_t *)image[image_ID], (x2 - first_touch + counter));
              
              if(image_ID == MAX_IMAGE_ID)
              {        
                LL_DrawPicture((uint32_t *)image[0], ((800 + x2 - first_touch + counter)));     
              }
              else
              {
                LL_DrawPicture((uint32_t *)image[image_ID + 1], ((800 + x2 - first_touch + counter)));
              }
              valid_buffer = 1;
              HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
            }
            else
            {
              LL_DrawPicture((uint32_t *)image[image_ID], 0);
              valid_buffer = 1;
              HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
              HAL_Delay(50); 
            }
          }
          
          for(counter=0; counter < 2; counter++)
          {
            while(valid_buffer);     
            
            LL_DrawPicture((uint32_t *)image[image_ID], 0);
            valid_buffer = 1;
            HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
            HAL_Delay(50);
          }
          
          touchdetected = 0;
          touch = 0; 
        }
        /********************************************************************/          
      }
    }
  }
}

/**
  * @brief Manage the vertical display of image on LCD DSI
  * @param None
  * @retval None
  */
static void LCD_DSI_VerticalSlider(void)
{
  /*Touch Detected **********************************************************/
  if(TS_State.touchDetected)
  {
    touchdetected = 1;
    if(touch == 0)
    {
      /* Get X and Y position of the first touch post calibrated */ 
      first_touch = TS_State.touchY[0];
      touch = 1;
    }
    y3 = TS_State.touchY[0];
    /* Follows Right ********************************************************/
    if (first_touch < y3)
    {
      while(valid_buffer);
      
      
      if(image_ID == 0)
      {       
        LL_DrawPicture((uint32_t *)image[MAX_IMAGE_ID], (y3 - first_touch - 480));
      }
      else
      {       
        LL_DrawPicture((uint32_t *)image[image_ID - 1], (y3 - first_touch - 480));
      } 
      
      HAL_Delay(80);
      LL_DrawPicture((uint32_t *)image[image_ID], (y3 - first_touch));
      HAL_Delay(40);
      valid_buffer = 1;
      HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
      HAL_Delay(20);
    }
    /************************************************************************/
    
    /* Follows Left *********************************************************/
    else
    {
      while(valid_buffer);        
      
      LL_DrawPicture((uint32_t *)image[image_ID], -(first_touch - y3));
      HAL_Delay(80);
      if(image_ID == MAX_IMAGE_ID)
      {        
        LL_DrawPicture((uint32_t *)image[0], (480 - first_touch + y3));
      }
      else
      {  
        LL_DrawPicture((uint32_t *)image[image_ID + 1], (480 - first_touch + y3));
      }
      HAL_Delay(40);
      
      valid_buffer = 1;
      HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
      HAL_Delay(20);
    }
  }
  /**************************************************************************/
  
  /* Touch Released *********************************************************/
  if(!TS_State.touchDetected)
  {
    if(touchdetected == 1)
    {
      /* Get X and Y position of the first touch post calibrated */ 
      y2 = TS_State.touchY[0];
      /* Move Right *********************************************************/
      if(y2 > first_touch)
      {
        if(y2 > (first_touch + 100))   
        { 
          if(image_ID > 0)
          {
            image_ID--;
          }
          else
          {
            image_ID = MAX_IMAGE_ID;
          }
          
          LL_CopyPicture((uint32_t *)image[image_ID], (uint32_t *)(0xC0200000));
          
          for(counter= (y2 - first_touch -480); counter < 0; counter++)
          {
            while(valid_buffer);     
            counter += 30;
            
            if(counter < 0)
            {
              
              LL_DrawPicture((uint32_t *)(0xC0200000), counter);
              valid_buffer = 1;
              HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
              HAL_Delay(50);
            }
          }
          
          for(counter=0; counter < 2; counter++)
          {
            while(valid_buffer);     
            
            LL_DrawPicture((uint32_t *)(0xC0200000), 0);
            valid_buffer = 1;
            HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
            HAL_Delay(50);
          }
          
          touchdetected = 0;
          touch = 0;            
        }
        /********************************************************************/
        
        /* Snap Right *******************************************************/
        else
        {
          for(counter=0; counter < (y2 - first_touch + 1); counter++)
          {
            counter += 10;
            if(counter < (y2 - first_touch + 1))
            {
              while(valid_buffer);
              if(image_ID == 0)
              {
                LL_DrawPicture((uint32_t *)image[MAX_IMAGE_ID], (((y2 - first_touch) - counter) - 480));
              }
              else
              {
                LL_DrawPicture((uint32_t *)image[image_ID - 1], (((y2 - first_touch) - counter) - 480));
              }
              LL_DrawPicture((uint32_t *)image[image_ID], ((y2 - first_touch) - counter));
              valid_buffer = 1;
              HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
            }
            else
            {
              LL_DrawPicture((uint32_t *)image[image_ID], 0);
              valid_buffer = 1;
              HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
              HAL_Delay(50); 
            }
          }   
          
          for(counter=0; counter < 2; counter++)
          {
            while(valid_buffer);     
            
            LL_DrawPicture((uint32_t *)image[image_ID], 0);
            valid_buffer = 1;
            HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
            HAL_Delay(50);
          }
          
          touchdetected = 0;
          touch = 0;               
        }        
      }
      /**********************************************************************/
      
      /* Move Left **********************************************************/
      else
      {
        if(first_touch > (y2 + 100))   
        {
          if(image_ID < MAX_IMAGE_ID)
          {
            image_ID++;
          }
          else
          {
            image_ID = 0;
          }
          
          LL_CopyPicture((uint32_t *)image[image_ID], (uint32_t *)(0xC0200000));
          
          for(counter=(480 - first_touch + y2); counter > 0; counter--)
          {
            while(valid_buffer);     
            counter -= 30;
            
            if(counter > 0)
            {
              LL_DrawPicture((uint32_t *)(0xC0200000), counter);
              
              valid_buffer = 1;
              HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
              HAL_Delay(50);
            }
          }
          
          for(counter=0; counter < 1; counter++)
          {
            while(valid_buffer);     
            
            LL_DrawPicture((uint32_t *)(0xC0200000), 0);
            valid_buffer = 1;
            HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
            HAL_Delay(50);
          }
          
          touchdetected = 0;
          touch = 0;           
        }
        /********************************************************************/
        
        /* Snap Left ********************************************************/
        else
        {
          for(counter=0; counter < (first_touch - y2 + 1); counter++)
          {
            counter += 10;
            if(counter < (first_touch - y2 + 1))
            {
              while(valid_buffer);
              LL_DrawPicture((uint32_t *)image[image_ID], (y2 - first_touch + counter));
              
              if(image_ID == MAX_IMAGE_ID)
              {        
                LL_DrawPicture((uint32_t *)image[0], ((800 + y2 - first_touch + counter)));     
              }
              else
              {
                LL_DrawPicture((uint32_t *)image[image_ID + 1], ((480 + y2 - first_touch + counter)));
              }
              valid_buffer = 1;
              HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
            }
            else
            {
              LL_DrawPicture((uint32_t *)image[image_ID], 0);
              valid_buffer = 1;
              HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
              HAL_Delay(50); 
            }
          }
          
          for(counter=0; counter < 2; counter++)
          {
            while(valid_buffer);     
            
            LL_DrawPicture((uint32_t *)image[image_ID], 0);
            valid_buffer = 1;
            HAL_DSI_LongWrite(&hdsi_eval, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pSyncLeft);
            HAL_Delay(50);
          }
          
          touchdetected = 0;
          touch = 0; 
        }
        /********************************************************************/          
      }
    }
  }
}

/**
  * @brief  End of Refresh DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
  if(valid_buffer > 0)
  {
    if(active_area == LEFT_AREA)
    {
      /* Mask the TE */
      HAL_DSI_ShortWrite(hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_TEOFF, 0x00);
      
      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(&hdsi_eval);
      /* Update LTDC configuration */
      LTDC_LAYER(&hltdc_eval, 0)->CFBAR = LAYER0_ADDRESS + 400 * 4;
      __HAL_LTDC_RELOAD_IMMEDIATE_CONFIG(&hltdc_eval);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(&hdsi_eval);
      
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColRight);
      /* Refresh the right part of the display */
      HAL_DSI_Refresh(hdsi);    
      
    }
    else if(active_area == RIGHT_AREA)
    {
      
      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(&hdsi_eval);
      /* Update LTDC configuration */
      LTDC_LAYER(&hltdc_eval, 0)->CFBAR = LAYER0_ADDRESS;
      __HAL_LTDC_RELOAD_IMMEDIATE_CONFIG(&hltdc_eval);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(&hdsi_eval);
      
      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft); 
      valid_buffer = 0;     
    }
  }
  active_area = (active_area == LEFT_AREA)? RIGHT_AREA : LEFT_AREA; 
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

/**
  * @brief  Initializes the DSI LCD. 
  * The ititialization is done as below:
  *     - DSI PLL ititialization
  *     - DSI ititialization
  *     - LTDC ititialization
  *     - OTM8009A LCD Display IC Driver ititialization
  * @param  None
  * @retval LCD state
  */
static uint8_t LCD_Init(void)
{
  DSI_PHY_TimerTypeDef PhyTimings; 
  DSI_CmdCfgTypeDef CmdCfg;
  DSI_LPCmdTypeDef LPCmd;
  DSI_PLLInitTypeDef dsiPllInit;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  
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
  
  /* LCD clock configuration */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 417 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 417 MHz / 5 = 83.4 MHz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 83.4 / 2 = 41.7 MHz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 417;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
  
  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hdsi_eval.Instance = DSI;
  
  HAL_DSI_DeInit(&(hdsi_eval));
  
  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF  = DSI_PLL_OUT_DIV1;  

  hdsi_eval.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  hdsi_eval.Init.TXEscapeCkdiv = 0x4;
  HAL_DSI_Init(&(hdsi_eval), &(dsiPllInit));
    
  /* Configure the DSI for Command mode */
  CmdCfg.VirtualChannelID      = 0;
  CmdCfg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
  CmdCfg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
  CmdCfg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.ColorCoding           = DSI_RGB888;
  CmdCfg.CommandSize           = HACT;
  CmdCfg.TearingEffectSource   = DSI_TE_DSILINK;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh      = DSI_AR_ENABLE;
  CmdCfg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_ENABLE;
  HAL_DSI_ConfigAdaptedCommandMode(&hdsi_eval, &CmdCfg);
  
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
  HAL_DSI_ConfigCommand(&hdsi_eval, &LPCmd);

  /* Configure DSI PHY HS2LP and LP2HS timings */
  PhyTimings.ClockLaneHS2LPTime = 35;
  PhyTimings.ClockLaneLP2HSTime = 35;
  PhyTimings.DataLaneHS2LPTime = 35;
  PhyTimings.DataLaneLP2HSTime = 35;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 10;
  HAL_DSI_ConfigPhyTimer(&hdsi_eval, &PhyTimings);
  
  /* Initialize LTDC */
  LTDC_Init();
  
  /* Start DSI */
  HAL_DSI_Start(&(hdsi_eval));
    
  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)*/
  OTM8009A_Init(OTM8009A_COLMOD_RGB888, LCD_ORIENTATION_LANDSCAPE);
  
   /* Reconfigure the DSI for HS Command mode */
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
  HAL_DSI_ConfigCommand(&hdsi_eval, &LPCmd);
  
  HAL_DSI_ConfigFlowControl(&hdsi_eval, DSI_FLOW_CONTROL_BTA);
  
  /* Refresh the display */
  HAL_DSI_Refresh(&hdsi_eval);
  
  return LCD_OK;
}

/**
  * @brief  Initialize the LTDC
  * @param  None
  * @retval None
  */
void LTDC_Init(void)
{
  /* DeInit */
  hltdc_eval.Instance = LTDC;
  HAL_LTDC_DeInit(&hltdc_eval);
  
  /* LTDC Config */
  /* Timing and polarity */
  hltdc_eval.Init.HorizontalSync = HSYNC;
  hltdc_eval.Init.VerticalSync = VSYNC;
  hltdc_eval.Init.AccumulatedHBP = HSYNC+HBP;
  hltdc_eval.Init.AccumulatedVBP = VSYNC+VBP;
  hltdc_eval.Init.AccumulatedActiveH = VSYNC+VBP+VACT;
  hltdc_eval.Init.AccumulatedActiveW = HSYNC+HBP+HACT;
  hltdc_eval.Init.TotalHeigh = VSYNC+VBP+VACT+VFP;
  hltdc_eval.Init.TotalWidth = HSYNC+HBP+HACT+HFP;
  
  /* background value */
  hltdc_eval.Init.Backcolor.Blue = 0;
  hltdc_eval.Init.Backcolor.Green = 0;
  hltdc_eval.Init.Backcolor.Red = 0;
  
  /* Polarity */
  hltdc_eval.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc_eval.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc_eval.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc_eval.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc_eval.Instance = LTDC;

  HAL_LTDC_Init(&hltdc_eval);
}

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address)
{
    LCD_LayerCfgTypeDef  Layercfg;

  /* Layer Init */
  Layercfg.WindowX0 = 0;
  Layercfg.WindowX1 = BSP_LCD_GetXSize()/2;
  Layercfg.WindowY0 = 0;
  Layercfg.WindowY1 = BSP_LCD_GetYSize(); 
  Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  Layercfg.FBStartAdress = Address;
  Layercfg.Alpha = 255;
  Layercfg.Alpha0 = 0;
  Layercfg.Backcolor.Blue = 0;
  Layercfg.Backcolor.Green = 0;
  Layercfg.Backcolor.Red = 0;
  Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  Layercfg.ImageWidth = BSP_LCD_GetXSize()/2;
  Layercfg.ImageHeight = BSP_LCD_GetYSize();
  
  HAL_LTDC_ConfigLayer(&hltdc_eval, &Layercfg, LayerIndex); 
}


/**
  * @brief EXTI line detection callbacks.
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(draw_method == VERTICAL_SLIDER)
  {
    draw_method = HORIZONTAL_SLIDER;
  }
  else
  {
  draw_method = VERTICAL_SLIDER;  
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
static void LL_CopyPicture(uint32_t *pSrc, uint32_t *pDst)
{   
  uint32_t destination = (uint32_t)pDst;
  uint32_t source      = (uint32_t)pSrc;
     
/*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  hdma2d.Init.Mode         = DMA2D_M2M;
  hdma2d.Init.ColorMode    = DMA2D_ARGB8888;
  hdma2d.Init.OutputOffset = 0;     
  
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
      if (HAL_DMA2D_Start(&hdma2d, source, destination, 480, 800) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&hdma2d, 100);
      }
    }
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
static void LL_DrawPicture(uint32_t *pSrc, int32_t xyPos)
{   
  uint32_t inputOffset = 0;
  uint32_t outputOffset  = 0;
  uint32_t pDst = LAYER0_ADDRESS;
  uint32_t width  = 0, height = 0;
  uint32_t source = (uint32_t)pSrc;
  
  /* draw_method == VERTICAL_SLIDER */ 
  if(draw_method == VERTICAL_SLIDER)
  {
    width = 800;
    if(xyPos < 0)
    {
      source = (uint32_t)pSrc + (-xyPos)*800*4;
      height = (480 + xyPos);
      inputOffset = 0;
      outputOffset = 0;
    }
    else
    { 
      
      pDst   = LAYER0_ADDRESS + (xyPos)*800*4; 
      height = 480 - xyPos;    
      inputOffset = 0;
      outputOffset = 0;   
    }
  }
  else /* draw_method == HORIZONTAL_SLIDER */ 
  {    
    height = 480;
    if(xyPos < 0)
    {
      source = (uint32_t)pSrc + (-xyPos)*4;
      width = (800 + xyPos);
      inputOffset = -xyPos;
      outputOffset = -xyPos;
    }
    else
    {
      pDst   = LAYER0_ADDRESS + (xyPos)*4; 
      width = 800 - xyPos;    
      inputOffset = xyPos;
      outputOffset = xyPos;   
    }    
  }
  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/ 
  hdma2d.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d.Init.ColorMode    = DMA2D_ARGB8888;
  hdma2d.Init.OutputOffset = outputOffset;     
  
  /*##-2- DMA2D Callbacks Configuration ######################################*/
  hdma2d.XferCpltCallback  = NULL;
  
  /*##-3- Foreground Configuration ###########################################*/
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].InputOffset = inputOffset;
  
  hdma2d.Instance          = DMA2D; 
  
  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&hdma2d, source, (uint32_t)pDst, width, height) == HAL_OK)
      {
        /* Polling For DMA transfer */  
        HAL_DMA2D_PollForTransfer(&hdma2d, 500);
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
