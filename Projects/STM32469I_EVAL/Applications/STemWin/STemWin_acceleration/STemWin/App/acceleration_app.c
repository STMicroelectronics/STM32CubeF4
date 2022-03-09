/**
  ******************************************************************************
  * @file    acceleration_app.c
  * @author  MCD Application Team
  * @brief   This file provides main GUI application program functions
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
#include <string.h>
#include "GUI_Private.h"

/* Bitmap generated resources */
#include "village.c"
#include "sun.c"
#include "sun_rain.c"
#include "moon.c"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FADE_PERIOD 750

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef struct {
  const GUI_BITMAP * pBitmap;  
} BITMAP_ITEM;

static const BITMAP_ITEM _aBitmapItem[] = {
  {&bmsun},
  {&bmsun_rain},  
  {&bmmoon},  
};

/* Private function prototypes -----------------------------------------------*/
void MainTask(void);


/**
  * @brief  Function that will draw on the Memory device
  * @param  index   : Plan index
  * @param  xSize   : Plan horizontal size
  * @param  ySize   : Plan vertical size
  * @param  hPicture: Picture memory device to use
  * @param  hMemWork: Target memory device
  * @retval None
  */
static void _PaintMemdev(int index, int xSize, int ySize, GUI_MEMDEV_Handle hPicture, GUI_MEMDEV_Handle hMemWork) {
  int Color = 0xFF3691c6;
  
  /* Select the picture memory device */
  GUI_MEMDEV_Select(hPicture);
  /* First set a transparent Background */
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
  /* Then draw the picture on it */
  GUI_DrawBitmap(_aBitmapItem[index].pBitmap, (_aBitmapItem[0].pBitmap->XSize - _aBitmapItem[index].pBitmap->XSize)/2, 
                 (_aBitmapItem[0].pBitmap->YSize - _aBitmapItem[index].pBitmap->YSize)/2);
  /* Select the memory device to be paint */
  GUI_MEMDEV_Select(hMemWork);
  switch(index)
  {
    case 0:
      /* Sunny sky */
      Color = 0xFF3691c6;
      break;
    case 1:
      /* Rainy sky */
      Color = 0xFF1b558a;
      break;
    case 2:
      /* Night sky */
      Color = 0xFF0b3153;
      break;
  default:
    break;
  }  
  /* Fill the screen using the needed color */
  GUI_SetColor(Color);
  GUI_FillRect(0, 0, (xSize - 1), (ySize - 1)); 
  GUI_DrawBitmap(&bmvillage, 0, (ySize - bmvillage.YSize));
  /* Picture to display */
  GUI_MEMDEV_WriteAt(hPicture, 20, 20);
  /* Select the display, once all draws are done */
  GUI_MEMDEV_Select(0);
}


/**
  * @brief  Function to modify a value to a given print format
  * @param  value   : value to format
  * @param  NumDecs : Decimal part to display
  * @param  Shift   : Shifted part to display
  * @param  acBuffer: String to be used for the display
  * @retval None
  */
static void _MakeString(int value, int NumDecs, int Shift,  char * acBuffer) {
  int i;
  int a;
  int s;

  acBuffer[NumDecs--] = '0' + (a = value) % 10;
  for (i = 1, s = 1; i <= NumDecs; i++) {
    if (i == Shift) {
      acBuffer[NumDecs - i + s] = '.';
      s = 0;
    }
    acBuffer[NumDecs - i + s] = '0' + (a /= 10) % 10;
  }
}

/**
  * @brief  Function to display the performance results
  * @param  xSize        : Plan horizontal size
  * @param  ySize        : Plan vertical size
  * @param  ResultDrawing: Drawing performance result
  * @param  ResultFading : Fading performance result
  * @param  ResultTotal  : Total = (Drawing and Fading) performance result
  * @retval None
  */
static void _DrawResult(int xSize, int ySize, int ResultDrawing, int ResultFading, int ResultTotal) {
  const char   acDrawing[] = "Drawing: ";
  const char   acFading [] = "Fading: ";
  const char   acTotal  [] = "Total: ";
  const char * apString[3];
  char         acValue[7] = {0};
  int        * apValues[3];
  unsigned     i;
  int          xMid;
  int          yMid;
  int          ySizeFont;
  int          ySizeTotal;
  int          xPosText;
  int          yPosText;
  int          xSizeText;
  int          xSizeLabel;
  int          xSizeValue;
  int          xSizeTotal;

  apString[0] = acDrawing;
  apString[1] = acFading;
  apString[2] = acTotal;
  apValues[0] = &ResultDrawing;
  apValues[1] = &ResultFading;
  apValues[2] = &ResultTotal;
  
  /* Set the font to be used */
  GUI_SetFont(GUI_FONT_20B_ASCII);
  
  /* Calculate position on the display screen */
  xMid = xSize / 2;
  yMid = ySize / 2;
  ySizeFont = GUI_GetFontSizeY();
  ySizeTotal = 4 * ySizeFont + 2 * 10;
  yPosText = (yMid - ySizeTotal / 2) / 2;
  for (i = xSizeLabel = 0; i < GUI_COUNTOF(apString); i++ )
  {
	  xSizeText = GUI_GetStringDistX(apString[i]);
	  if (xSizeLabel < xSizeText) 
	  {
		  xSizeLabel = xSizeText;
	  }
  }
  xSizeValue = GUI_GetStringDistX("00.000");
  xSizeTotal = xSizeLabel + xSizeValue;
  xPosText = xMid - xSizeTotal / 2;
  
  /* Display the title */
  GUI_SetColor(GUI_BLACK);
  GUI_SetTextAlign(GUI_TA_HCENTER);
  ySizeFont = GUI_GetFontDistY();
  GUI_DispStringHCenterAt("Performance Vs Accelerations", xSize / 2, (ySize - ySizeFont * 3) / 4);
  
  /* Draw different results */
  GUI_SetBkColor(0xFF3691c6);
  GUI_SetColor(GUI_WHITE);
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_Clear();
  GUI_DispStringHCenterAt("Result [MPixels/s]:", xMid, yPosText);
  GUI_SetTextAlign(GUI_TA_LEFT);
  for (i = 0; i < GUI_COUNTOF(apString); i++ )
  {
	  GUI_DispStringAt(apString[i], xPosText, yPosText + (i + 1) * ySizeFont + 10);
  }
  for (i = 0; i < GUI_COUNTOF(apString); i++ )
  {
    _MakeString(*apValues[i], 5, 3, acValue);
    GUI_DispStringAt(acValue, xPosText + xSizeLabel, yPosText + (i + 1) * ySizeFont + 10);
  }
}

/**
  * @brief  Main application
  * @param  None
  * @retval None
  */
void MainTask(void) {
  GUI_MEMDEV_Handle ahMem[2];
  GUI_MEMDEV_Handle hPicture;
  int               xSize;
  int               ySize;
  unsigned          i;
  int               j;
  int               TimeStart;
  int               TimeNow;
  int               Cnt;
  int               ResultFading;
  int               ResultDrawing;
  int               ResultTotal;

  static int 		k = 0;
  static int 		f = 0;
  
  /* First get the sizes of the screen used */  
  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  
  /* Create Picture device*/  
  hPicture = GUI_MEMDEV_CreateFixed(0, 0, _aBitmapItem[0].pBitmap->XSize, _aBitmapItem[0].pBitmap->YSize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUICC_M8888I);
  /* Exit in case of an error */
  if(hPicture == 0)
	  return;
  
  /* Create screen devices */
  for(i = 0; i < GUI_COUNTOF(ahMem); i++)
  {
	ahMem[i] = GUI_MEMDEV_CreateFixed(0, 0, xSize, ySize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUICC_M8888I);
	if(ahMem[i] == 0)
		return;
	else
	{
          /* Paint the needed bitmaps on the MemDevs */
          _PaintMemdev(k++, xSize, ySize, hPicture, ahMem[i]);
	}
  }

  /* Select the display screen */
  GUI_MEMDEV_Select(0);
  
  /* Animation loop */
  while (1) 
  {    
    /* Show fading */
    for (j = 0; j < 3; j++)
	{
      for (i = 0; i < 3; i++) 
	  {
        /* Do the different Fades */
		GUI_MEMDEV_FadeDevices(ahMem[f ^ 1], ahMem[f], FADE_PERIOD);        
        /* Paint the needed bitmaps on the MemDevs */
        _PaintMemdev((k++)%3, xSize, ySize, hPicture, ahMem[f]);
        f++;
        if(f == 2)
          f = 0;
      }
    }    
    k = 0;
    
    /* Use the latest displayed screen */
	GUI_MEMDEV_CopyFromLCD(ahMem[0]);
	
	/* Measure the drawing performance achieved */
    TimeStart = GUI_GetTime();
    Cnt = 0;
    do {
      GUI_MEMDEV_Write(ahMem[0]);
      TimeNow = GUI_GetTime();
      Cnt++;
    } while (TimeNow < (TimeStart + 1000));
    ResultDrawing = (xSize * ySize * Cnt) / (TimeNow - TimeStart);
    
    /* Measure the total performance */    
    TimeStart = GUI_GetTime();
    Cnt = 0;
    do {
      GUI_MEMDEV__FadeDevice(ahMem[0], ahMem[0], ahMem[0], Cnt & 0xff);
      GUI_MEMDEV_Write(ahMem[1]);
      TimeNow = GUI_GetTime();
      Cnt++;
    } while (TimeNow < (TimeStart + 1000));
    ResultTotal  = (xSize * ySize * Cnt) / (TimeNow - TimeStart);
    
    /* Measure the fading performance */
    TimeStart = GUI_GetTime();
    Cnt = 0;
    do {
      GUI_MEMDEV__FadeDevice(ahMem[0], ahMem[0], ahMem[0], Cnt & 0xff);
      TimeNow = GUI_GetTime();
      Cnt++;
    } while (TimeNow < (TimeStart + 1000));
    ResultFading  = (xSize * ySize * Cnt) / (TimeNow - TimeStart);
    /* Draw different results */
    GUI_MEMDEV_Select(ahMem[1]);
    _DrawResult(xSize, ySize, ResultDrawing, ResultFading, ResultTotal);
    GUI_MEMDEV_Select(0);
    GUI_MEMDEV_FadeDevices(ahMem[1], ahMem[0], FADE_PERIOD);
    GUI_Delay(3000);
    
    for(i = 0; i < GUI_COUNTOF(ahMem); i++)
    {
		/* Paint the needed bitmaps on the MemDevs */
        _PaintMemdev(k++, xSize, ySize, hPicture, ahMem[i]);	
    }
  }
}
