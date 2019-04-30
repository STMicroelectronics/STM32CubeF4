/**
  ******************************************************************************
  * @file    information_win.c
  * @author  MCD Application Team
  * @brief   System information functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "DIALOG.h"
#include "k_module.h"
#include "information_res.c"

/** @addtogroup INFORMATION_MODULE
  * @{
  */

/** @defgroup INFORMATION
  * @brief information routines 
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  INFORMATION_board =
{  
  6,
  (uint8_t*)"",
  &bminfo,
  Startup,
}
;

/* Private defines -----------------------------------------------------------*/
#define ID_FRAMEWIN_INFO        (GUI_ID_USER + 0x01)
#define ID_INFO_EXIT            (GUI_ID_USER + 0x02)
#define ID_TEXT_COPYRIGHT       (GUI_ID_USER + 0x03)
#define ID_TEXT_TITLE           (GUI_ID_USER + 0x04)
#define ID_TEXT_BOARD           (GUI_ID_USER + 0x05)
#define ID_TEXT_CORE            (GUI_ID_USER + 0x06)
#define ID_TEXT_CPU             (GUI_ID_USER + 0x07)
#define ID_TEXT_VERSION         (GUI_ID_USER + 0x08)

#define ID_TEXT_BOARD_1           (GUI_ID_USER + 0x09)
#define ID_TEXT_CORE_1            (GUI_ID_USER + 0x0A)
#define ID_TEXT_CPU_1             (GUI_ID_USER + 0x0B)
#define ID_TEXT_VERSION_1         (GUI_ID_USER + 0x0C)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "Information", 0, 0,   0, 240, 240, FRAMEWIN_CF_MOVEABLE },
  { TEXT_CreateIndirect, "System Information", ID_TEXT_TITLE,  50,  50, 200, 30,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "Copyright (c) STMicroelectronics 2017", ID_TEXT_COPYRIGHT, 62, 228, 180, 10, 0, 0x0, 0 },
  
  { TEXT_CreateIndirect, "Core: ",       ID_TEXT_CORE,    55,  90,  40,  15,  0, 0x0,  0 },  
  { TEXT_CreateIndirect, "Board: ",      ID_TEXT_BOARD,   55,  125, 45,  15,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "CPU Speed: ",  ID_TEXT_CPU,     55,  160, 70,  15,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "Firm.Ver: ",   ID_TEXT_VERSION, 55,  195, 60,  15,  0, 0x0,  0 },
  
  { TEXT_CreateIndirect, "STM32F4",          ID_TEXT_CORE_1,   95,  90,  140,  15,  0, 0x0,  0 },  
  { TEXT_CreateIndirect, "STM32F412G-DISCO", ID_TEXT_BOARD_1,   100,  125, 135,  15,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "100MHz",           ID_TEXT_CPU_1,     125,  160, 110,  15,  0, 0x0,  0 },
  { TEXT_CreateIndirect, "V1.2.0",           ID_TEXT_VERSION_1, 115,  195, 120,  15,  0, 0x0,  0 }  
  
};

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Paints exit button
* @param  repeat_status: repeat button status
* @retval None
*/
static void _OnPaint_exit(BUTTON_Handle hObj) {

 GUI_DrawBitmap(&bminfo_exit, 20, 0);
}

/**
* @brief  callback for usb button
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
  * @brief  main callback for information
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = BUTTON_CreateEx(186, 3, 50, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_INFO_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);    
    
    /* Information */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TITLE);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph20B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);    
    
    /* ST Copyright */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_COPYRIGHT);
    TEXT_SetFont(hItem, &GUI_Font8_ASCII);
    TEXT_SetTextColor(hItem, GUI_WHITE);    
    
    
    /* Initialization of 'Board*/
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BOARD);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph16B);
    TEXT_SetTextColor(hItem, GUI_GREEN);

    /* Initialization of 'Core: STM32F-7 Series' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CORE);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph16B);
    TEXT_SetTextColor(hItem, GUI_GREEN);

    /* Initialization of 'CPU Speed : 200MHz' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph16B);
    TEXT_SetTextColor(hItem, GUI_GREEN);

    /* Initialization of 'Firmware Version*/
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VERSION);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph16B);
    TEXT_SetTextColor(hItem, GUI_GREEN); 
    
    /****************************************/
    /* Initialization of 'Board*/
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BOARD_1);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph16);
    TEXT_SetTextColor(hItem, GUI_LIGHTGREEN);
    
    /* Initialization of 'Core: STM32F-7 Series' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CORE_1);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph16);
    TEXT_SetTextColor(hItem, GUI_LIGHTGREEN);

    /* Initialization of 'CPU Speed : 200MHz' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU_1);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph16);
    TEXT_SetTextColor(hItem, GUI_LIGHTGREEN);

    /* Initialization of 'Firmware Version*/
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VERSION_1);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph16);
    TEXT_SetTextColor(hItem, GUI_LIGHTGREEN);     
    
    break;
    
  case WM_PAINT:
    /* Draw information background */
    GUI_SetBkColor(0xFF000000);
    GUI_Clear();  
    
    GUI_DrawBitmap(&bmcore, 10, 85);
    GUI_DrawBitmap(&bmboard, 10, 120);
    GUI_DrawBitmap(&bmcpu, 10, 155);
    GUI_DrawBitmap(&bmversion, 10, 190);
                   
    GUI_SetColor(0xCCC5B6);
    GUI_FillRect(0, 0, 240, 40);
    
    break;     
        
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    
    switch (Id) {
      
    case ID_INFO_EXIT:
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        GUI_EndDialog(pMsg->hWin, 0);
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
  * @brief  USB Storage window Startup
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
