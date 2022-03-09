/**
  ******************************************************************************
  * @file    gardening_control_win.c
  * @author  MCD Application Team
  * @brief   gardening control functions
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
#include "gardening_control_res.c"

/** @addtogroup GARDENING_CONTROL_MODULE
  * @{
  */

/** @defgroup GARDENING_CONTROL
  * @brief gardening control routines
  * @{
  */
  
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  gardening_control_board =
{
  5,
  "garden control",
  open_gardening,
  0,
  Startup,
  NULL,
}
;

/* Private defines -----------------------------------------------------------*/
#define ID_WINDOW_0               (GUI_ID_USER + 0x00)
#define ID_BUTTON_INFO_CLOSE      (GUI_ID_USER + 0x03)

#define ID_BUTTON_CONTROL        (GUI_ID_USER + 0x05)
#define ID_BUTTON_SPRINKLERS     (GUI_ID_USER + 0x06)
#define ID_BUTTON_DROWISE        (GUI_ID_USER + 0x07)

#define ID_TEXT_CONTROL          (GUI_ID_USER + 0x09)
#define ID_TEXT_CONTROL2         (GUI_ID_USER + 0x0A)
#define ID_TEXT_CONTROL3         (GUI_ID_USER + 0x0B)
#define ID_TEXT_SPRINKLERS       (GUI_ID_USER + 0x0C)
#define ID_TEXT_DROWISE          (GUI_ID_USER + 0x0D)

#define ID_BUTTON_EXIT            (GUI_ID_USER + 0x08)


/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x64, 0 }, 
  { TEXT_CreateIndirect,      "main",      ID_TEXT_CONTROL,        80,  325,  100,  30,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "control",    ID_TEXT_CONTROL2,        135,  325,  100,  30,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "(on/off)",    ID_TEXT_CONTROL3,        100,  345,  100,  30,  0, 0x0,  0 },	
  { TEXT_CreateIndirect,      "sprinklers", ID_TEXT_SPRINKLERS,    355,  325,  100,  30,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "drowise",    ID_TEXT_DROWISE,        610, 325,  100,  30,  0, 0x0,  0 },
  
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Paints exit button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_exit(BUTTON_Handle hObj) {

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_AA_FillCircle(100, 0, 100);

  GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_FontLubalGraph32);
  GUI_DispStringAt("Menu", 20, 20);
}

/**
  * @brief  callback for Exit button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_exit(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_exit(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints control button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_control(BUTTON_Handle hObj) {
  int Index;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if(Index)
  {
    GUI_DrawBitmap(&bmdes_control_sub, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmcontrol_sub, 0, 0);
  }
}

/**
  * @brief  callback for control button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_control(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_control(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints sprinklers button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_sprinklers(BUTTON_Handle hObj) {
  int Index;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if(Index)
  {
    GUI_DrawBitmap(&bmdes_garden_sub, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmgarden_sub, 0, 0);
  }
}

/**
  * @brief  callback for sprinklers button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_sprinklers(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_sprinklers(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints drowise button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_drowise(BUTTON_Handle hObj) {
  int Index;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if(Index)
  {
    GUI_DrawBitmap(&bmdes_water_sub, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmwater_sub, 0, 0);
  }
}

/**
  * @brief  callback for drowise button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_drowise(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_drowise(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Callback routine of the main dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    hItem = BUTTON_CreateEx(700, 0, 100,100, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);

    hItem = BUTTON_CreateEx(50,  150, 180, 180, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CONTROL);
    WM_SetCallback(hItem, _cbButton_control);     
      
    hItem = BUTTON_CreateEx(310, 140, 180, 180, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_SPRINKLERS);
    WM_SetCallback(hItem, _cbButton_sprinklers); 

    hItem = BUTTON_CreateEx(570, 140, 180,  180, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_DROWISE);
    WM_SetCallback(hItem, _cbButton_drowise);     

    /* Title Initialization in play list */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CONTROL);
    TEXT_SetText(hItem, "main");
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);
	
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CONTROL2);
    TEXT_SetText(hItem, "control");
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE);	
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CONTROL3);
    TEXT_SetText(hItem, "(on/off)");
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE);			
		
    /* Title Initialization in play list */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_SPRINKLERS);
    TEXT_SetText(hItem, "sprinklers");
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);    
    
    /* Duration */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_DROWISE);
    TEXT_SetText(hItem, "dropwise");
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE);

    break;     
    
  case WM_PAINT:
    
    break;     
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    
    
    switch(Id) {
    case ID_BUTTON_EXIT: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        
        GUI_EndDialog(pMsg->hWin, 0);
        
        break;
      }
      break; 
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }    
}


/**
  * @brief  gardening control window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{

  GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, xpos, ypos);
}

/**
  * @}
  */

/**
  * @}
  */
