/**
  ******************************************************************************
  * @file    settings_win.c
  * @author  MCD Application Team
  * @brief   settings functions
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
#include "settings_res.c"

/** @addtogroup SETTINGS_MODULE
  * @{
  */

/** @defgroup SETTINGS
  * @brief settings routines
  * @{
  */
  
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  settings_board =
{
  8,
  "system info",
  settings,
  0,
  Startup,
  NULL,
}
;

/* Private defines -----------------------------------------------------------*/
#define ID_WINDOW_0               (GUI_ID_USER + 0x00)

#define ID_TEXT_BOARD             (GUI_ID_USER + 0x02)
#define ID_TEXT_CORE              (GUI_ID_USER + 0x03)
#define ID_TEXT_CPU               (GUI_ID_USER + 0x04)
#define ID_TEXT_VERSION           (GUI_ID_USER + 0x05)
#define ID_TEXT_COPYRIGHT         (GUI_ID_USER + 0x06)
#define ID_TEXT_BOARD_1           (GUI_ID_USER + 0x07)
#define ID_TEXT_BOARD_2           (GUI_ID_USER + 0x0C)

#define ID_TEXT_CORE_1            (GUI_ID_USER + 0x08)
#define ID_TEXT_CPU_1             (GUI_ID_USER + 0x09)
#define ID_TEXT_VERSION_1         (GUI_ID_USER + 0x0A)
#define ID_TEXT_TITLE             (GUI_ID_USER + 0x0B)

#define ID_BUTTON_EXIT            (GUI_ID_USER + 0x20)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "", ID_WINDOW_0,                    0,    0,  800, 480, 0, 0x64, 0 }, 
  { TEXT_CreateIndirect, "System Information", ID_TEXT_TITLE,  290,  30, 300, 40,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "Board", ID_TEXT_BOARD,               90,   155, 80, 20,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "Core", ID_TEXT_CORE,                 285,  155, 80, 20,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "CPU Speed", ID_TEXT_CPU,             445,  155, 100, 20,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "Firm.Ver", ID_TEXT_VERSION,          645,  155, 90, 20,  0, 0x0,  0 },  

  { TEXT_CreateIndirect, " STM32469I-", ID_TEXT_BOARD_1,  60,  295, 120, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "   DISCO   " ,  ID_TEXT_BOARD_2,  65,  315, 100, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, " STM32F4", ID_TEXT_CORE_1,     260, 300, 100, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, " 180MHz", ID_TEXT_CPU_1,       450, 300, 100, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "V1.4.1", ID_TEXT_VERSION_1,    655, 300, 100, 20, 0, 0x0, 0 }, 
  
  { TEXT_CreateIndirect, "Copyright (c) STMicroelectronics 2016", ID_TEXT_COPYRIGHT, 550, 453, 240, 20, 0, 0x0, 0 },
};

static WM_HTIMER                  hTimer; 
uint32_t frame = 0;

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
  * @brief  Callback routine of the main dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    frame = 0;
    hTimer = WM_CreateTimer(pMsg->hWin, 0, 50, 0);
  
    hItem = BUTTON_CreateEx(700, 0, 100, 100, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);
    
    /* Initialization of 'Board*/
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BOARD);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE);

    /* Initialization of 'Core: STM32F-4 Series' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CORE);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE);

    /* Initialization of 'CPU Speed : 180MHz' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE);

    /* Initialization of 'Firmware Version*/
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VERSION);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE); 

    /* Initialization of 'Board*/
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BOARD_1);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);

    /* Initialization of 'Board */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BOARD_2);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);    
    
    /* Initialization of 'Core: STM32F-4 Series' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CORE_1);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);

    /* Initialization of 'CPU Speed : 180MHz' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU_1);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);

    /* Initialization of 'Firmware Version*/
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VERSION_1);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE); 
    
    /* ST Copyright */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_COPYRIGHT);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph16);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE);

    /* ST Copyright */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph32B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);
    
    break;     
    
  case WM_PAINT:    
    GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
    GUI_AA_DrawRoundedRect(30,  140, 200, 340, 30);
    GUI_AA_DrawRoundedRect(220, 140, 390, 340, 30);
    GUI_AA_DrawRoundedRect(410, 140, 580, 340, 30);
    GUI_AA_DrawRoundedRect(600, 140, 770, 340, 30);
    
    GUI_DrawBitmap(open_board[frame]   , 65,  190);
    GUI_DrawBitmap(open_mcu[frame]     , 255, 190);
    GUI_DrawBitmap(open_cpu[frame]     , 445, 190);
    GUI_DrawBitmap(open_fversion[frame], 635, 190);
      
    break;     

  case WM_TIMER:
    if(frame < 4)
    {
      WM_InvalidateWindow(pMsg->hWin);
      WM_RestartTimer(pMsg->Data.v, 50);
      frame++;
    }
    else
    {
      if(hTimer != 0)
      {
        WM_DeleteTimer(hTimer);
        hTimer = 0;
      }  
    }    
    break;
       
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
       
    switch(Id) {
    case ID_BUTTON_EXIT: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        if(hTimer != 0)
        {
          WM_DeleteTimer(hTimer);
          hTimer = 0;
        }         
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
  * @brief  system information window Startup
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
