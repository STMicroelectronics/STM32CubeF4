/**
  ******************************************************************************
  * @file    k_menu.c
  * @author  MCD Application Team
  * @brief   This file provides the kernel menu functions 
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

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MENU
  * @brief Kernel menu routines
  * @{
  */


/* External variables --------------------------------------------------------*/
extern int status;
extern GUI_CONST_STORAGE GUI_BITMAP bmwarning;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define ID_ICONVIEW_MENU               (GUI_ID_USER + 0x00)

#define ID_FRAMEWIN_0                  (GUI_ID_USER + 0x01)
#define ID_BUTTON_OK_MESSAGE           (GUI_ID_USER + 0x02)
#define ID_TEXT_FILENAME               (GUI_ID_USER + 0x03)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 WM_HWIN hIcon;
 WM_HWIN hWin ;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

 /* Info Dialog Create */
static const GUI_WIDGET_CREATE_INFO _aFileInfoDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "WARNING", ID_FRAMEWIN_0, 0, 0, 200, 200, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Please insert SDCard", ID_TEXT_FILENAME, 30, 80, 150, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OK", ID_BUTTON_OK_MESSAGE, 65, 120, 70, 70, 0, 0x0, 0 },
};

/**
  * @brief  Callback routine of desktop window
  * @param  pMsg:
  * @retval None
  */
static void _cbBk(WM_MESSAGE * pMsg) {
  GUI_RECT r;

  switch (pMsg->MsgId) 
  {      
  case WM_PAINT:
    WM_GetInsideRect(&r);
    GUI_ClearRectEx(&r);    
    break;
 
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Paints ok message button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_okMessage(BUTTON_Handle hObj) {

  GUI_SetBkColor(GUI_WHITE);
  GUI_Clear();

  if((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
  {
    GUI_SetColor(GUI_DARKGRAY);
    GUI_AA_FillCircle(35, 35, 35);
    GUI_SetBkColor(GUI_DARKGRAY);
    GUI_SetColor(GUI_WHITE);   
  }
  else
  {
    GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
    GUI_AA_FillCircle(35, 35, 35);
    GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
    GUI_SetColor(GUI_WHITE);    
  } 
  
  GUI_SetFont(&GUI_FontLubalGraph20B);
  GUI_DispStringAt("OK", 25, 10);
}

/**
  * @brief  callback for ok message button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_okMessage(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_okMessage(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
  * @brief  Callback routine of Info dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbFileInfoDialog(WM_MESSAGE * pMsg) {
  int     NCode;
  int     Id;
  WM_HWIN hItem;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OK_MESSAGE);
    WM_SetCallback(hItem, _cbButton_okMessage);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_FILENAME);
    TEXT_SetFont(hItem, &GUI_FontAvantGarde16B);
status = 1;
    break;
        
  case WM_PAINT:    
    GUI_DrawBitmap(&bmwarning, 65, 10);
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_OK_MESSAGE: /* Notifications sent by 'OK' */
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
  * @brief  callback routine of main menu.
  * @param  None.
  * @retval None
  */
static void cbWin(WM_MESSAGE * pMsg) {

  /* Animation */
  uint32_t NCode, Id;
  static uint8_t sel = 0;
  WM_MOTION_INFO    * pInfo;
  WM_HWIN hItem;
  static int IsPressed = 0;
  
  switch (pMsg->MsgId) 
  {      
  case WM_PAINT:
    break;
    
  case WM_MOTION:   
    pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
    if (pInfo) {
      switch (pInfo->Cmd) {
      case WM_MOTION_INIT:
        pInfo->SnapX = LCD_GetXSize()/2;
        pInfo->SnapY = LCD_GetYSize()/2;
        break;
           
    case WM_MOTION_MOVE:
      IsPressed = 0;
      break;

      }
    }
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);     
    NCode = pMsg->Data.v;  
    
    switch (NCode) 
    {  
    case WM_NOTIFICATION_CLICKED:   
      
      IsPressed = 1;
      if (Id == ID_ICONVIEW_MENU)
      { 
        sel = ICONVIEW_GetSel(pMsg->hWinSrc);
      }    
      
      break;
      
    case WM_NOTIFICATION_RELEASED:
      if (IsPressed) {
        if (Id == ID_ICONVIEW_MENU)
        {
          if(sel < k_ModuleGetNumber())
          { 
            WM_MOTION_Enable(0);
            module_prop[sel].in_use = 1;
            if(sel != 0)
            {
              status = 1;
            }
              
            if((sel == 0) || (sel == 1) || (sel == 2))
            {
              if(k_StorageGetStatus(MSD_DISK_UNIT))
              {
                module_prop[sel].module->startup(pMsg->hWin, - WM_GetWindowOrgX(pMsg->hWin), 0);
              }
              else
              {
                hItem = GUI_CreateDialogBox(_aFileInfoDialogCreate, 
                                            GUI_COUNTOF(_aFileInfoDialogCreate), 
                                            _cbFileInfoDialog, 
                                            pMsg->hWin, 
                                            (20-WM_GetWindowOrgX(pMsg->hWin)), 20);
                WM_MakeModal(hItem);  
              }
            }           
            else
            {
              module_prop[sel].module->startup(pMsg->hWin, - WM_GetWindowOrgX(pMsg->hWin), 0);
            }
            sel = 0;
          }
        }
        IsPressed = 0;  
      }

    case WM_NOTIFICATION_CHILD_DELETED:
      if(pMsg->hWinSrc != hIcon)
      {
        WM_MOTION_Enable(1);
      }
      
      break;
    
    default:
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}
/**
  * @brief  set the GUI profile
  * @param  None.
  * @retval None.
  */
void k_SetGuiProfile(void) 
{
  BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
  DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
  
  FRAMEWIN_SetDefaultTextColor(0, GUI_WHITE);
  FRAMEWIN_SetDefaultTextColor(1, GUI_WHITE);      
  
  FRAMEWIN_SetDefaultBarColor(0, GUI_STCOLOR_LIGHTBLUE);
  FRAMEWIN_SetDefaultBarColor(1, GUI_STCOLOR_LIGHTBLUE);
  
  FRAMEWIN_SetDefaultFont(&GUI_FontAvantGarde32);
  
  FRAMEWIN_SetDefaultClientColor(GUI_WHITE);  
  FRAMEWIN_SetDefaultTitleHeight(35);
  FRAMEWIN_SetDefaultTextAlign(GUI_TA_HCENTER);
  
  WINDOW_SetDefaultBkColor(GUI_WHITE);

  GUI_SetDefaultFont(&GUI_FontLubalGraph20);
  
  TEXT_SetDefaultTextColor(GUI_STCOLOR_LIGHTBLUE);
  TEXT_SetDefaultFont(&GUI_FontLubalGraph20);
}

/**
  * @brief  Initializes the menu.
  * @param  None.
  * @retval None
  */
void k_InitMenu(void) 
{
  uint8_t i = 0;
  
  WM_SetCallback(WM_HBKWIN, _cbBk);
  WM_SetWindowPos(WM_HBKWIN, -LCD_GetXSize()/2, 0, 2 * LCD_GetXSize() + 10, LCD_GetYSize() );
  WM_MOTION_Enable(1);
  
  hWin = WM_CreateWindowAsChild(0, 0,  3 * (LCD_GetXSize() /2), LCD_GetYSize(), WM_HBKWIN,
           WM_CF_SHOW | WM_CF_MOTION_X, cbWin, 0);
  
  hIcon = ICONVIEW_CreateEx(0, 
                            0, 
                            3 * (LCD_GetXSize() /2), 
                            LCD_GetYSize(), 
                            hWin, 
                            WM_CF_SHOW,
                            0,
                            ID_ICONVIEW_MENU, 
                            110, 
                            110);
  
  
  ICONVIEW_SetSpace(hIcon, GUI_COORD_Y, 10);
  ICONVIEW_SetFrame(hIcon, GUI_COORD_Y, 5);
  ICONVIEW_SetSpace(hIcon, GUI_COORD_X, 10);
  ICONVIEW_SetFrame(hIcon, GUI_COORD_X, 5);  
  
  ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_BK, GUI_BLACK);
    
  for (i = 0; i < k_ModuleGetNumber(); i++)
  {
    ICONVIEW_AddBitmapItem(hIcon,module_prop[i].module->icon, (char *)module_prop[i].module->name);
  }
  
  WM_SetWindowPos(hWin, 0, 0, 3 * (LCD_GetXSize() /2), LCD_GetYSize());
 WM_MOTION_SetDefaultPeriod(100);
}  

/**
  * @}
  */

/**
  * @}
  */
