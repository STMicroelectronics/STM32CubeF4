/**
  ******************************************************************************
  * @file    animation_app.c
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
#include "WM.h"

/* Bitmap resources */
#include "Background.c"
#include "cloud.c"
#include "dog1_walk1.c"
#include "dog1_walk2.c"
#include "dog1_walk1_r.c"
#include "dog1_walk2_r.c"
#include "Balloon.c"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef struct {
  int xSize, ySize;
  int xPart, yPart;
  int xPos, yPos;
  int AnimDirection;
  int Size;
  int ObjectSize;
} ANIM_DATA;

/* Walking state */
static int Walk = 0;
/* Walking direction */
static int AnimDirection;

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Called at the beginning and the end of the Animation
  * @param  State: State of the animation (START, END)
  * @param  pVoid: Pointer to enter more needed data
  * @retval None
  */
static void _SliceInfo(int State, void * pVoid) 
{
  GUI_USE_PARA(pVoid);
  /* Add the usage of multibuffering to avoid flickering */
  switch (State) {
  case GUI_ANIM_START:
    GUI_MULTIBUF_Begin();
    break;
  case GUI_ANIM_END:
    GUI_MULTIBUF_End();
    break;
  }
}

/**
  * @brief  Prepare the animation drawing operation
  * @param  pInfo  : Animation information passed by emWin to the application
  * @param  pVoid  : pointer to enter more needed data
  * @param  ppData : Data (pointer) pointer to application data
  * @param  Index  : Index of animation
  * @retval None
  */
static void _PrepareDrawing(GUI_ANIM_INFO * pInfo, void * pVoid, ANIM_DATA ** ppData, int Index)
{
  ANIM_DATA * pData;  
  int i = 0;
  
  /* Use custom void pointer for pointing to application defined data structure */
  pData = *ppData = (ANIM_DATA *)pVoid;
  
  /* Calculate x-position depending of the animation value */
  if(Index == 0)
  {
    i = AnimDirection;
  }
  else
  {
    i = -AnimDirection;
  }
  
  switch (i) {
  case +1:
    pData->xPos = ((pData->xSize - pData->xPart * 1) * pInfo->Pos) / (GUI_ANIM_RANGE);
    break;
  case -1:
    pData->xPos = pData->xSize - pData->xPart - 1 - ((pData->xSize - pData->xPart * 1) * pInfo->Pos) / (GUI_ANIM_RANGE);
    break;
  }
  
  /* Calculate object size in dependence of position */
  pData->ObjectSize = (pData->Size);
}

/**
  * @brief  Draw the dog animation
  * @param  pInfo  : Animation information passed by emWin to the application
  * @param  pVoid  : pointer to enter more needed data
  * @retval None
  */
static void _AnimDrawDog(GUI_ANIM_INFO * pInfo, void * pVoid) 
{
  ANIM_DATA * pData;
  
  /* Prepare the animation drawing */
  _PrepareDrawing(pInfo, pVoid, &pData, 0);
  /* Draw the fixed background for all the animations */
  GUI_DrawBitmap(&bmBackground, 0, 0);
  
  /* Setup the dog y position to fit with the Bkc picture */
  pData->yPos = 226;
  
  /* Draw the dog picture depending of the walking state */
  if(Walk%2 == 0)
  {
     if(AnimDirection == 1)
     {
       GUI_DrawBitmap(&bmdog1_walk1, pData->xPos, pData->yPos);
     }
     else
     {
       GUI_DrawBitmap(&bmdog1_walk1_r, pData->xPos, pData->yPos);
     }
     /* Increment the walking state */
     Walk++;
  }
  else
  {
    if(AnimDirection == 1)
    {
      GUI_DrawBitmap(&bmdog1_walk2, pData->xPos, pData->yPos);
    }
    else
    {
      GUI_DrawBitmap(&bmdog1_walk2_r, pData->xPos, pData->yPos);
    }
    Walk = 0;
  }  
}

/**
  * @brief  Draw the cloud animation
  * @param  pInfo  : Animation information passed by emWin to the application
  * @param  pVoid  : pointer to enter more needed data
  * @retval None
  */
static void _AnimDrawCloud(GUI_ANIM_INFO * pInfo, void * pVoid)
{
 
  ANIM_DATA * pData;

  /* Prepare the cloud animation */
  _PrepareDrawing(pInfo, pVoid, &pData, 1);
  /* Fix the cloud y position on the screen */
  pData->yPos = 20;
  /* Draw the cloud depending of the calculated position */
  GUI_DrawBitmap(&bmcloud, pData->xPos, pData->yPos);      
}

/**
  * @brief  Draw the balloon animation
  * @param  pInfo  : Animation information passed by emWin to the application
  * @param  pVoid  : pointer to enter more needed data
  * @retval None
  */
static void _AnimDrawballoon(GUI_ANIM_INFO * pInfo, void * pVoid) {
  
  
    ANIM_DATA * pData;

  /* Prepare the balloon animation */
  _PrepareDrawing(pInfo, pVoid, &pData, 0);
  
  /* Fix the Y position */
  pData->yPos = 60;
  
  /* Draw the picture on the needed position */
  GUI_DrawBitmap(&bmBalloon, pData->xPos, pData->yPos);
}

/**
  * @brief  Play all the animations
  * @param  None  
  * @retval None
  */
static void _AnimationDemo(void) 
{
  GUI_ANIM_HANDLE hAnim;  
  ANIM_DATA DogData;
  ANIM_DATA CloudData;
  ANIM_DATA BalloonData;

  /* Create animation object 
   * Duration = 10s
   * Min_time_frame = 150
   */
  hAnim = GUI_ANIM_Create(10000, 150, &DogData, _SliceInfo);
  
  /* Fill Dog data to be used on the animation */  
  DogData.xSize = LCD_GetXSize();
  DogData.ySize = LCD_GetYSize();
  DogData.xPart = bmdog1_walk2.XSize;
  DogData.yPart = bmdog1_walk2.YSize;
  
  /* Add Dog animation item: ANIM_LINEAR animation */
  GUI_ANIM_AddItem(hAnim, 0, 10000, ANIM_LINEAR, &DogData, _AnimDrawDog);
  
  /* Fill Cloud data to be used on the animation */  
  CloudData.xSize = LCD_GetXSize();
  CloudData.ySize = LCD_GetYSize();
  CloudData.xPart = bmcloud.XSize;
  CloudData.yPart = bmcloud.YSize;
  
  /* Add Cloud animation item: ANIM_ACCELDECEL animation */
  GUI_ANIM_AddItem(hAnim,  0, 10000, ANIM_ACCELDECEL,     &CloudData, _AnimDrawCloud);
   
  /* Add Balloon animation item */
  /* Fill Balloon data to be used on the animation */  
  BalloonData.xSize = LCD_GetXSize();
  BalloonData.ySize = LCD_GetYSize();
  BalloonData.xPart = bmBalloon.XSize;
  BalloonData.yPart = bmBalloon.YSize;  
  
  GUI_ANIM_AddItem(hAnim,  0, 10000, ANIM_ACCELDECEL,     &BalloonData , _AnimDrawballoon);
  
  /* Start the 3 animations */  
  GUI_ANIM_Start(hAnim);  
     
  /* Execute animation until end is reached */
  while (GUI_ANIM_Exec(hAnim) == 0) 
  {
    /* Idle time for other tasks */    
    GUI_X_Delay(5);
  }
  
  /* Delete animation object */
  GUI_ANIM_Delete(hAnim);
}

/**
  * @brief  Main application 
  * @param  None  
  * @retval None
  */
void MainTask(void) 
{	  
  /* Infinite loop on the animation demo */
  while (1) 
  {
    GUI_Delay(5);
    /* Change Animation direction every time */
    for (AnimDirection = 1; AnimDirection >= -1; AnimDirection -= 2) 
    {
      _AnimationDemo();    
    }
  }
}
