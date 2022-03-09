/**
  ******************************************************************************
  * @file    memory_device_app.c
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
#include "GUI.h"

/* Bitmap resources */
#include "garage.c"
#include "Wheel2.c"
#include "Wheel3.c"
#include "Wheel4.c"
#include "Wheel5.c"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAG                        300
#define TITLE_SIZE                 40
#define MIN_TIME_PER_FRAME_SHIFT   10
#define MIN_TIME_PER_FRAME_ENLARGE 10
#define BORDER_SIZE                4
#define FINAL_STEP                 10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef struct {
  const GUI_BITMAP GUI_UNI_PTR * pBitmap;
  GUI_MEMDEV_Handle              hMem;
  int                            xSize;
  int                            ySize;
} IMAGE;

/* Table of pictures to be used */
static IMAGE _aImage[] = {
  { &bmWheel4 },
  { &bmWheel3 },
  { &bmWheel2 },
  { &bmWheel5 }
};

/* Calculate the number of images used */
static int _NumImages = sizeof(_aImage) / sizeof(_aImage[0]);

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Create the pictures memdev and draw them inside
  * @param  pImage: pointer to the pictures structure
  * @retval int
  */
static int _GetImage(IMAGE * pImage) 
{
  /* Get the picture sizes */
  pImage->xSize = pImage->pBitmap->XSize;
  pImage->ySize = pImage->pBitmap->YSize;
  /* Allocate the needed memdev pictures */
  pImage->hMem  = GUI_MEMDEV_CreateFixed(0, 0, pImage->xSize, pImage->ySize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUICC_M8888I);
  /* Exit if any issue occurred on the memdev creation */
  if (pImage->hMem == 0) 
  {
    return 1;
  }
  /* Select the memdev created and paint the picturte on it */
  GUI_MEMDEV_Select(pImage->hMem);
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
  GUI_DrawBitmap(pImage->pBitmap, 0, 0);
  /* Select the display after finishing the job */
  GUI_MEMDEV_Select(0);
  return 0;
}

/**
  * @brief  Create the needed memdevs and do the automated rotate & zoom of the pictures
  * @param  None
  * @retval int
  */
static int _RotateAndZoomImage(void) 
{
  static GUI_MEMDEV_Handle hDst  = 0;
  static GUI_MEMDEV_Handle  hLCD  = 0;
  static int                Index = 0;
  IMAGE                     *pImage;  
  int                       TimeContinue;
  int                       TimeNow;
  int                       xSize;
  int                       ySize;
  int                       Mag;
  int                       a;
  int                       i;
  /* Declaration of a function pointer for the rotate function */
  void                      (*pfRotate)(GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag);

  /* Assign the function pointer: use the rotation High Quality memdev rotation */
  pfRotate = GUI_MEMDEV_Rotate;
  pImage   = &_aImage[Index];
  /* Get the respective width and height of the screen */
  xSize    = LCD_GetXSize();
  ySize    = LCD_GetYSize();
  
  /* Clean up LCD context, draw a given background */  
  GUI_DrawBitmap(&bmgarage, 0, 0);
  
  /* Create the LCD memory devices if not existent */
  if (hLCD == 0) 
  {
    hLCD = GUI_MEMDEV_CreateFixed(0, 0, xSize, ySize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUICC_M8888I);
    /* Exit if any issue when creating the memdev */
    if (hLCD == 0)
    {
      return 1;
    }
    /* Initialise the memdev with what's displayed */
    GUI_MEMDEV_CopyFromLCD(hLCD);
  }  
  /* Create the Destination memory devices if not existent */
  if (hDst == 0)
  {
    /* Exit if any issue when creating the memdev */
    hDst = GUI_MEMDEV_CreateFixed(0, 0, xSize, ySize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUICC_M8888I);
    if (hDst == 0) 
    {
      return 1;
    }
  }
  /* Reserve a part to display the title of the application */
  ySize -= TITLE_SIZE;
  
  /* Rotate and enlarge while moving in */
  for (i = 2; (i <= 100); i += 6) 
  {
    TimeContinue = GUI_GetTime() + MIN_TIME_PER_FRAME_ENLARGE;
    GUI_MEMDEV_Select(hDst);
    GUI_MEMDEV_Write(hLCD);
    pfRotate(pImage->hMem, hDst, (xSize - pImage->xSize) / 2 - (xSize * (100 - i)) / 300, TITLE_SIZE + (ySize - pImage->ySize) / 2, (i - 100) * 450, i * 10);
    GUI_MEMDEV_Select(0);
    /* Display the result on the screen */
    GUI_MULTIBUF_Begin();
    GUI_MEMDEV_CopyToLCD(hDst);
    GUI_MULTIBUF_End();

    
    /* Add some delay to achieve minimum timing */
    TimeNow = GUI_GetTime();
    if (TimeContinue > TimeNow) 
    {
      GUI_Delay(TimeContinue - TimeNow);
    } 
    else
    {
      GUI_Exec();
    }
  }
  
  /* Rotate, enlarge and shrink on spot */
  Mag = 1000;
  for (a = 5; a <= 360; a += 30) 
  {
    TimeContinue = GUI_GetTime() + MIN_TIME_PER_FRAME_ENLARGE;
    /* Calculate magnification */
    if (a <= 180)
    {
      Mag = (Mag * 1050) / 1000;
    }
    else
    {
      Mag = (Mag * 1000) / 1050;
    }
    /* Do animation: write the result every time to the LCD */
    GUI_MEMDEV_Select(hDst);
    GUI_MEMDEV_Write(hLCD);
    pfRotate(pImage->hMem, hDst, (xSize - pImage->xSize) / 2, TITLE_SIZE + (ySize - pImage->ySize) / 2, a * 1000, 1000);
    GUI_MEMDEV_Select(0);
    /* Display the result on the screen */
    GUI_MULTIBUF_Begin();
    GUI_MEMDEV_CopyToLCD(hDst);
    GUI_MULTIBUF_End();
    
    /* Add some delay to achieve minimum timing */
    TimeNow = GUI_GetTime();
    if (TimeContinue > TimeNow) 
    {
      GUI_Delay(TimeContinue - TimeNow);
    }
    else
    {
      GUI_Exec();
    }
  }
  
  /* Clean up LCD context, Draw the Bkc again */
  GUI_MEMDEV_Select(hLCD);
  GUI_DrawBitmap(&bmgarage, 0, 0);
  
  /* Rotate and enlarge while moving out */
  for (i = 2; (i <= 100); i += 6) 
  {
    TimeContinue = GUI_GetTime() + MIN_TIME_PER_FRAME_ENLARGE;
    GUI_MEMDEV_Select(hDst);
    GUI_MEMDEV_Write(hLCD);
    pfRotate(pImage->hMem, hDst, (xSize - pImage->xSize) / 2 + (xSize * i) / 300, TITLE_SIZE + (ySize - pImage->ySize) / 2, i * 450, (100 - i) * 10);
    GUI_MEMDEV_Select(0);
    /* Display the result on the screen */
    GUI_MULTIBUF_Begin();
    GUI_MEMDEV_CopyToLCD(hDst);
    GUI_MULTIBUF_End();
    
    /* Add some delay to achieve minimum timing */
    TimeNow = GUI_GetTime();
    if (TimeContinue > TimeNow) 
    {
      GUI_Delay(TimeContinue - TimeNow);
    }
    else
    {
      GUI_Exec();
    }
  }  
  /* Display the result on the screen */
  GUI_MULTIBUF_Begin();
  GUI_MEMDEV_CopyToLCD(hLCD);
  GUI_MULTIBUF_End();

  /* Go to the next picture */
  Index = (Index + 1) % _NumImages;
  
  /* Delete the MEMDEVS used */
  GUI_MEMDEV_Delete(hLCD);
  GUI_MEMDEV_Delete(hDst);
  hDst = 0;
  hLCD = 0; 
  
  return 0;
}


/**
  * @brief  Create the needed memdevs and do the automated rotate & shift of the pictures
  * @param  None
  * @retval int
  */
static int _ShiftRotateAndZoomImage(void) 
{
  static GUI_MEMDEV_Handle    hDstEnlarge = 0;
  static GUI_MEMDEV_Handle    hLCDEnlarge = 0;
  static GUI_MEMDEV_Handle    hDstShift   = 0;
  static GUI_MEMDEV_Handle    hLCD        = 0;
  static int                  Index       = 0;
  IMAGE                       *pImage;  
  int                         TimeContinue;
  int                         DispSpaceX;
  int                         DispSpaceY;
  int                         TimeNow;
  int                         Index0;
  int                         Index1;
  int                         Index2;
  int                         Index3;
  int                         xSize;
  int                         ySize;
  int                         Step;
  int                         Add;
  int                         Mag;
  int                         a;
  int                         i;
  /* Declaration of a function pointer for the rotate function */
  void                        (*pfRotate)(GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag);

  /* Get the respective width and height of the screen */
  xSize      = LCD_GetXSize();
  ySize      = LCD_GetYSize() - TITLE_SIZE;
  DispSpaceX = xSize / 3;
  DispSpaceY = (ySize * 3) / 5;
  
  /* Clean up LCD context, draw the Bkc */
  GUI_DrawBitmap(&bmgarage, 0, 0);
  
  /* Create LCD memory device if not existing */
  if (hLCD == 0) 
  {
    hLCD        = GUI_MEMDEV_CreateFixed(0, TITLE_SIZE + ySize / 5, xSize, DispSpaceY, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUICC_M8888I);
    /* Exit if any issue on the creation of the memdev */
    if (hLCD == 0) 
    {
      return 1;
    }
    /* Initialiose the memdev with what's on the screen */
    GUI_MEMDEV_CopyFromLCD(hLCD);
  }
  /* Create destination memory device if not existing */
  if (hDstShift == 0) 
  {
    hDstShift   = GUI_MEMDEV_CreateFixed(0, TITLE_SIZE + ySize / 5, xSize, DispSpaceY, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUICC_M8888I);
    /* Exit if any issue on the creation of the memdev */
    if (hDstShift == 0) 
    {
      return 1;
    }
  }
  /* Create the zoom destination memory device if not existing */
  if (hDstEnlarge == 0) 
  {
    hDstEnlarge = GUI_MEMDEV_CreateFixed(xSize / 6, TITLE_SIZE, (xSize * 2) / 3, ySize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUICC_M8888I);
    /* Exit if any issue on the creation of the memdev */
    if (hDstEnlarge == 0) 
    {
      return 1;
    }
  }
  /* Create the zoom LCD memory device if not existing */
  if (hLCDEnlarge == 0) 
  {
    hLCDEnlarge = GUI_MEMDEV_CreateFixed(xSize / 6, TITLE_SIZE, (xSize * 2) / 3, ySize, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUICC_M8888I);
    /* Exit if any issue on the creation of the memdev */
    if (hLCDEnlarge == 0) 
    {
      return 1;
    }
  }
  /* Select the display screen */
  GUI_MEMDEV_Select(0);

  /* Get image indices */  
  Index0 = (Index + 0) % _NumImages;
  Index1 = (Index + 1) % _NumImages;
  Index2 = (Index + 2) % _NumImages;
  Index3 = (Index + 3) % _NumImages;
  
  /* Choose the rotation API to use: HQ rotation isn't used here */ 
  pfRotate = GUI_MEMDEV_Rotate;
  /* Perform shifting */  
  for (i = 1; i <= FINAL_STEP; i++) 
  {
    TimeContinue = GUI_GetTime() + MIN_TIME_PER_FRAME_SHIFT;
    Step         = (DispSpaceX * i) / FINAL_STEP;
    
    /* Calculate rotation */
    a = (-Step * 360 * 1000) / DispSpaceX;
    
    /* Do the animation */
    GUI_MEMDEV_Select(hDstShift);
    GUI_MEMDEV_Write(hLCD);

    pImage        = &_aImage[Index0];
    pfRotate(pImage->hMem, hDstShift, ((xSize - pImage->xSize) / 2) - (DispSpaceX * 2) + Step, (DispSpaceY - pImage->ySize) / 2, a, MAG);
    pImage        = &_aImage[Index1];
    pfRotate(pImage->hMem, hDstShift, ((xSize - pImage->xSize) / 2) -  DispSpaceX      + Step, (DispSpaceY - pImage->ySize) / 2, a, MAG);
    pImage        = &_aImage[Index2];
    pfRotate(pImage->hMem, hDstShift, ((xSize - pImage->xSize) / 2)                    + Step, (DispSpaceY - pImage->ySize) / 2, a, MAG);
    pImage        = &_aImage[Index3];
    pfRotate(pImage->hMem, hDstShift, ((xSize - pImage->xSize) / 2) +  DispSpaceX      + Step, (DispSpaceY - pImage->ySize) / 2, a, MAG);

    /* Display the result */
    GUI_MULTIBUF_Begin();
    GUI_MEMDEV_CopyToLCD(hDstShift);
    GUI_MULTIBUF_End();
    
    /* Add some delay to achieve minimum timing */
    TimeNow = GUI_GetTime();
    if (TimeContinue > TimeNow) 
    {
      GUI_Delay(TimeContinue - TimeNow);
    }
    else 
    {
      GUI_Exec();
    }
  }
  
  /* Enlarge & shrink */  
  pImage = &_aImage[Index1];
  GUI_MEMDEV_CopyFromLCD(hLCDEnlarge);
  GUI_MEMDEV_Select(hDstEnlarge);
  Add = 1;
  Mag = MAG;
  i   = 0;
  do 
  {
    TimeContinue = GUI_GetTime() + MIN_TIME_PER_FRAME_ENLARGE;
    
    /* Use HQ or not */
    if (i == 0) 
    {
      pfRotate = GUI_MEMDEV_RotateHQ;
    }
    else
    {
      pfRotate = GUI_MEMDEV_Rotate;
    }
    
    /* Calculate magnification */
    if (Add > 0) 
    {
      Mag = (Mag * 1050) / 1000;
    } 
    else
    {
      Mag = (Mag * 1000) / 1050;
    }
    if (Mag < MAG)
    {
      Mag = MAG;
    }
    
    /* Do the animation */
    GUI_MEMDEV_Write(hLCDEnlarge);
    pfRotate(pImage->hMem, hDstEnlarge, (((xSize * 2) / 3) - pImage->xSize) / 2, (ySize - pImage->ySize) / 2, 0, Mag);
    /* Display the result */
    GUI_MULTIBUF_Begin();
    GUI_MEMDEV_CopyToLCD(hDstEnlarge);
    GUI_MULTIBUF_End();
    i += Add;
    if (i == 20) 
    {
      Add = -Add;
    }
   
    /* Add some delay to achieve minimum timing */    
    TimeNow = GUI_GetTime();
    if (TimeContinue > TimeNow) 
    {
      GUI_Delay(TimeContinue - TimeNow);
    }
  } while (i > 0);
  
  /* Modify the pictures positions */
  if (Index == 0) 
  {
    Index = _NumImages - 1;
  }
  else
  {
    Index--;
  }
  
  /* Delete the MEMDEVS used */
  GUI_MEMDEV_Delete(hLCD);
  GUI_MEMDEV_Delete(hDstShift);
  GUI_MEMDEV_Delete(hDstEnlarge);
  GUI_MEMDEV_Delete(hLCDEnlarge);
  hLCD = 0;
  hDstShift = 0;  
  hDstEnlarge = 0;
  hLCDEnlarge = 0; 
  
  return 0;
}

/**
  * @brief  Main application
  * @param  None
  * @retval None
  */
void MainTask(void) 
{
  int xSize;
  int ySize;
  int i;

  /* Get the screen size */
  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize(); 
  
  /* Get the used images and write them to their respective memdevs */
  for (i = 0; i < _NumImages; i++) 
  {
    if (_GetImage(&_aImage[i])) {
      return;
    }
  }
  
  /* Loop infinetly on the animations */
  while (1) 
  {
    /* Rotation & zoom animation */
    if (_RotateAndZoomImage()) 
    {
      break;
    }
    /* Then the shift, rotate & zoom one */
    if (_ShiftRotateAndZoomImage()) 
    {
      break;
    }    
  }
  /* In case of an error will be here */
  GUI_SetBkColor(GUI_BLACK);
  GUI_SetColor(GUI_WHITE);
  GUI_Clear();
  GUI_DispStringHCenterAt("Error occurred. Application stopped!", xSize / 2, ySize / 2);
  /* Loop on the error message */
  while (1) 
  {
    GUI_Delay(100);
  }
}
