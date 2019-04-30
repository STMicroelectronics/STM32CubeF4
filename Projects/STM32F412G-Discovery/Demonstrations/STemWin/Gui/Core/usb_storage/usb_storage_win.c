/**
  ******************************************************************************
  * @file    usb_storage_win.c
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
#include "usb_storage_res.c"
#include "k_storage.h"
#include "usbd_app.h"

/** @addtogroup USB_STORAGE_MODULE
  * @{
  */

/** @defgroup USB_STORAGE
  * @brief usb storage routines 
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  USB_Storage_board =
{  
  4,
  (uint8_t*)"",
  &bmusb5,
  Startup,
}
;

typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t sd_mounted         : 1;    
    uint32_t connection         : 1;
  }b;
}
USBDSettingsTypeDef;

USBDSettingsTypeDef         USBDSettings;

/* Private defines -----------------------------------------------------------*/
#define ID_USB                  (GUI_ID_USER + 0x01)
#define ID_USB_EXIT             (GUI_ID_USER + 0x02)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "USB Storage Disk", 0, 0,   0, 240, 240, FRAMEWIN_CF_MOVEABLE }
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Callback function of the CPU window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMSDConnectionStatus(WM_MESSAGE * pMsg) {
  
  static WM_HTIMER hTimerTime;
  WM_HWIN hItem;  
  static uint8_t msd_connection_Changed = 0;
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    /* Create timer */
    hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 500, 0);        
    break;
    
  case WM_TIMER:
    
    if(msd_connection_Changed != k_StorageGetStatus(MSD_DISK_UNIT))
    {
      msd_connection_Changed = k_StorageGetStatus(MSD_DISK_UNIT);
    }
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_USB);
    WM_Invalidate(hItem);

    break; 
    
  case WM_DELETE:
    msd_connection_Changed = 0;
    WM_DeleteTimer(hTimerTime);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Paints usb button
  * @param  repeat_status: repeat button status
  * @retval None
  */
static void _OnPaint_usb(BUTTON_Handle hObj) {
 
  if(USBDSettings.b.connection == DISCONNECTED)
  {
    GUI_DrawBitmap(&bmusb_sub, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmusb_sub_des, 0, 0);
  }
}

/**
* @brief  Paints usb button
* @param  repeat_status: repeat button status
* @retval None
*/
static void _OnPaint_exit(BUTTON_Handle hObj) {

	GUI_DrawBitmap(&bmusb_exit, 20, 0);
}

/**
  * @brief  callback for usb button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_usb(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_usb(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
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
  * @brief  main callback for USB Devices
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    WM_CreateWindowAsChild(230, 0, 10, 10, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbMSDConnectionStatus , 0);
    
    hItem = BUTTON_CreateEx(0, 40, 240, 200, pMsg->hWin, WM_CF_SHOW, 0, ID_USB);
    WM_SetCallback(hItem, _cbButton_usb);
    
    hItem = BUTTON_CreateEx(186, 3, 50, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_USB_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);    
    
    break;
    
  case WM_PAINT:
    
    GUI_SetColor(0x4A281B);
    GUI_FillRect(0, 0, 240, 40);
    
    break;     
    
  case WM_DELETE:
    USBDSTOR_Stop();
    break;    
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    
    switch (Id) {
      
    case ID_USB:
      if(NCode == WM_NOTIFICATION_RELEASED)
      {    
        if(USBDSettings.b.connection == DISCONNECTED)
        {         
          USBDSettings.b.connection = CONNECTED;
          USBDSTOR_Connect();
        }
        else
        {          
          USBDSettings.b.connection = DISCONNECTED;
          USBDSTOR_Disconnect();          
        }
      }      
      break;
      
    case ID_USB_EXIT:
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
  USBDSettings.b.connection = DISCONNECTED;
  USBDSTOR_Start();
  GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, xpos, ypos);
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
