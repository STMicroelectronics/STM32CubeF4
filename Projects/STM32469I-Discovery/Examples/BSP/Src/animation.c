/**
  ******************************************************************************
  * @file    BSP/Src/animation.c
  * @author  MCD Application Team
  * @brief   This example code shows animation with LCD, DMA2D and DCMI for
  *          stm32469i_discovery driver
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

/* Defines ------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "anime.h"

#include "battery_20x35.h"
#include "logo_compo_134x124.h"
#include "icon_camera_60x60.h"
#include "clouds1_124x44.h"
#include "clouds2_124x44.h"
#include "clouds3_124x44.h"
#include "drop_29x35.h"
#include "gauge1_128x126.h"
#include "gauge2_128x126.h"
#include "gauge3_128x126.h"
#include "gyro_36x36.h"
#include "thermo_25x35.h"
#include "bluetooth_30x35.h"
#include "drop_29x35.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#define ANIMATION_STATUS_TEXT_POS 30            /* Number of lines from bottom of screen */

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

// Buffer construction
static anime_object_t anime_lcd_frame_buffer =
{
  (uint32_t *)LCD_FB_START_ADDRESS,
  BPP_ARGB8888,   /* 0 = ARGB8888 format */
  800,
  480,
  0x00000000, /* CLUT address */
  0,          /* CLUT size */
  0           /* animation pointer */
};

extern DMA2D_HandleTypeDef hdma2d_eval; /* DMA2D handle variable */

/* Private function prototypes -----------------------------------------------*/
static void     Animation_SetHint(void);

static void anime_draw_object(anime_object_t object, uint16_t pos_x, uint16_t pos_y, uint8_t transparency, anime_object_t buffer);

static void anime_draw_scene(anime_object_t object);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Blend Foreground object 'object', with background object 'buffer' shifted by position (pos_x, pos_y)
  *         @ destination address in 'buffer' + (pos_x, pos_y)
  * @param  object : Source object
  * @param  pos_x : Position of offset X drawing in the destination buffer
  * @param  pos_y : Position of offset Y drawing in the destination buffer
  * @param  transparency : Draw object 'object' in destination buffer 'buffer' shifted by pos_x, pos_y with this opacity parameter.
  * @param  buffer : Destination buffer of the drawing.
  * @retval None
  */
static void anime_draw_object(anime_object_t object,
                            uint16_t         pos_x,
                            uint16_t         pos_y,
                            uint8_t          transparency,
                            anime_object_t   buffer)
{
  uint32_t bytePerPixel = 0;

  if(buffer.bpp == BPP_ARGB8888)
  {
    bytePerPixel = 4;
  }
  else if(buffer.bpp == BPP_RGB888) /* RGB888 format */
  {
    bytePerPixel = 3;
  }
  else if ((buffer.bpp == BPP_RGB565)   ||
           (buffer.bpp == BPP_ARGB1555) ||
           (buffer.bpp == BPP_ARGB4444) ||
           (buffer.bpp == BPP_AL88))   /* RGB565 format */
  {
    bytePerPixel = 2;
  }
  else if ((buffer.bpp == BPP_L8)   ||
           (buffer.bpp == BPP_AL44) ||
           (buffer.bpp == BPP_A8))
  {
    bytePerPixel = 1;
  }

  /* Configure FG object */
  DMA2D->FGMAR = (uint32_t)(object.address);
  DMA2D->FGOR = 0x00000000; /* No offset in Src for FG object */

  /* If format of texture of 'object' contains an alpha information */
  if((object.bpp == BPP_ARGB8888)  ||
     (object.bpp == BPP_ARGB1555)  ||
     (object.bpp == BPP_ARGB4444)  ||
     (object.bpp == BPP_AL44)      ||
     (object.bpp == BPP_AL88)      ||
     (object.bpp == BPP_A8)        ||
     (object.bpp == BPP_A4))
  {
    /* Alpha Mode Choice */
    /* AM[1:0] = 2b10' => (0x2 << 16) : replace original alpha FG from texture by fixed alpha x original alpha texture */
    DMA2D->FGPFCCR = (transparency << 24) + (0x2 << 16) + object.bpp;
  }
  else
  {
    /* No Alpha in texture of 'object' */
    /* Alpha Mode Choice */
    /* AM[1:0] = 2b01' => (0x1 << 16) : replace original alpha FG by ALPHA[7:0] : ie 'transparency' parameter */
    DMA2D->FGPFCCR = (transparency << 24) + (0x1 << 16) + object.bpp;
  }

  DMA2D->NLR = (object.size_x << 16) + object.size_y;

  /* Configure BG object */
  /* Check if destination has direct color mode */
  DMA2D->BGMAR = (uint32_t)buffer.address + (bytePerPixel * ((pos_y * buffer.size_x) + pos_x));
  DMA2D->BGOR = buffer.size_x - object.size_x; /* Shall be same as output */
  /* ALPHA[7:0] is not used, AM[1:0] = 2b00' : no modification of the original alpha embedded in texture */
  DMA2D->BGPFCCR = buffer.bpp; /* CM[3:0] = buffer.bpp */
  DMA2D->OMAR = DMA2D->BGMAR;
  DMA2D->OOR = DMA2D->BGOR;
  DMA2D->OPFCCR = DMA2D->BGPFCCR;

  /* Clear flags */
  DMA2D->IFCR = 0x3F;
  /* Launch */
  DMA2D->CR = 0x00020001; /* Type of Transfer is M2M with blending and PFC + Start bit (0x01) */
  /* Wait till the copy is over */
  while (!DMA2D->ISR);
  DMA2D->IFCR = 0x02;
}

/**
  * @brief  Draw one by one all objects of scene using DMA2D operation in polling mode
  * @param  object
  * @retval None
  */
static void anime_draw_scene(anime_object_t object)
{
  anime_draw_object(battery_20x35, 50, 50, 0xFF, object);
  anime_draw_object(logo_compo_134x124, 666, 356, 0xFF, object);
  anime_draw_object(icon_camera_60x60, 209, 159, 0xFF, object);

  gauge1_128x126.animation++;
  switch(gauge1_128x126.animation)
  {
    case 1 :
      anime_draw_object(gauge1_128x126, 336, 177, 0xFF, object);
      break;
    case 2 :
      anime_draw_object(gauge2_128x126, 336, 177, 0xFF, object);
      break;
    default :
      anime_draw_object(gauge3_128x126, 336, 177, 0xFF, object);
      gauge1_128x126.animation = 0;
      break;
  } /* of switch(gauge1_128x126.animation) */

  clouds1_124x44.animation++;
  switch(clouds1_124x44.animation)
  {
    case 1 :
      anime_draw_object(clouds1_124x44, 600, 50, 0xFF, object);
      break;
    case 2 :
      anime_draw_object(clouds2_124x44, 600, 50, 0xFF, object);
      break;
    default :
      anime_draw_object(clouds3_124x44, 600, 50, 0xFF, object);
      clouds1_124x44.animation = 0;
      break;
  } /* of switch(clouds1_124x44.animation) */

  anime_draw_object(drop_29x35, 300, 400, 0xFF, object);
  anime_draw_object(thermo_25x35, 340, 400, 0xFF, object);
  anime_draw_object(gyro_36x36, 370, 400, 0xFF, object);
  anime_draw_object(bluetooth_30x35, 415, 400, 0xFF, object);
}

/* Public functions ---------------------------------------------------------*/

/**
  * @brief  Animation demo
  * @param  None
  * @retval None
  */
void Animation_demo (void)
{
  uint32_t exit = 0;
  uint32_t entry = 1;
  uint32_t initialState = 0;

  Animation_SetHint();

  /* Infinite loop */
  while (exit == 0)
  {
    if(entry == 1)  /* Entry */
    {
      entry = 0;

      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

      /* Clear first the LCD */
      BSP_LCD_Clear(LCD_COLOR_WHITE);

    }  /* of if(entry == 1) */


    /* Read initial state of button User */
    initialState = BSP_PB_GetState(BUTTON_USER);

    /* Toggle LED4 */
    BSP_LED_Toggle(LED4);

    /* Insert 100 ms delay */
    HAL_Delay(100);

    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);

    /* Insert 400 ms delay */
    HAL_Delay(400);

    /* update the scene */
    anime_draw_scene(anime_lcd_frame_buffer);

    /* Re check button state */
    if(BSP_PB_GetState(BUTTON_USER) != initialState)
    {
      exit = 1;
    }

  } /* of while (exit == 0) */

}

/**
  * @brief  Display Animation demo hint
  * @param  None
  * @retval None
  */
static void Animation_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set Camera Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 90);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"ANIMATION EXAMPLE", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"     Press USER button to exit     ", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 100, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 110);
  BSP_LCD_DrawRect(11, 101, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 112);
}

/**
  * @}
  */

/**
  * @}
  */
