/**
  ******************************************************************************
  * @file    vnc_server_win.c
  * @author  MCD Application Team
  * @brief   vnc server functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014 STMicroelectronics.
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
#include "vnc_server_res.c"
#include "vnc_app.h"

/** @addtogroup VNC_SERVER_MODULE
  * @{
  */

/** @defgroup VNC SERVER
  * @brief vnc server routines
  * @{
  */
  
/* External variables --------------------------------------------------------*/
extern ICONVIEW_Handle hIcon ;
static WM_HWIN hVncWin;
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
void VNC_SERVER_LogMessage (const char *message);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  vnc_server_board =
{
  7,
  "vnc server ",
  open_vnc,
  0,
  Startup,
  NULL,
}
;
 
/* Private defines -----------------------------------------------------------*/
#define ID_WINDOW_0               (GUI_ID_USER + 0x00)
#define ID_IMAGE_0                (GUI_ID_USER + 0x01)
#define ID_BUTTON_INFO_CLOSE      (GUI_ID_USER + 0x02)

#define ID_BUTTON_EXIT            (GUI_ID_USER + 0x03)
#define START_VNC                 (GUI_ID_USER + 0x04)
#define GO_BACKGROUND             (GUI_ID_USER + 0x05)
#define ID_VNC_STATUS             (GUI_ID_USER + 0x06)
#define ID_VNC_CONNECTION         (GUI_ID_USER + 0x07)
#define ID_LOCK                   (GUI_ID_USER + 0x08)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x64, 0 }, 
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_0, 33, 250, 574, 51, 0, 0, 0 },
  { TEXT_CreateIndirect, "VNC Connection OFF", ID_VNC_STATUS, 30, 25, 400, 35, 0, 0x0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_VNC_CONNECTION, 400, 150, 360, 120, 0, 0, 0 },
};

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Paints lock button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_lock(BUTTON_Handle hObj) {
            
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  if (VNC_GetLockState())
  {
    GUI_DrawBitmap(&bmlock, 0, 0);
    
    
  }
  else
  {
    GUI_DrawBitmap(&bmunlock, 0, 0);
  }
}

/**
* @brief  Paints backgound button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_bg(BUTTON_Handle hObj) {

  int Index = 0;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if (Index)
  {
    GUI_DrawBitmap(&bmbg, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmbg, 0, 0);
  }
}

/**
* @brief  Paints record button
* @param  hObj: button handle
* @retval None
*/
static void _OnPaint_start(BUTTON_Handle hObj) {

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  if(VNC_GetState() == VNC_IDLE)
  {
    if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
    {
      GUI_DrawBitmap(&bmvnc_play_pressed, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmvnc_play_unpressed, 0, 0);
    }
  }
  else 
  {
    if ((WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED))
    {
      GUI_DrawBitmap(&bmstop_pressed, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmstop_unpressed, 0, 0);
    }
  }
}


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
  * @brief  callback for background button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_bg(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_bg(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for start button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_start(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_start(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for lock button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_lock(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_lock(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
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
  * @brief  Callback routine of the vnc server dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    hItem = BUTTON_CreateEx(700, 0, 100, 100, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_VNC_CONNECTION);
    IMAGE_SetBitmap(hItem, &bmconnection_ko);
    
    hItem = BUTTON_CreateEx(534, 345, 72, 72, pMsg->hWin, WM_CF_SHOW, 0, START_VNC);
    WM_SetCallback(hItem, _cbButton_start);
    
    hItem = BUTTON_CreateEx(670, 340, 60, 80, pMsg->hWin, WM_CF_SHOW, 0, GO_BACKGROUND);
    WM_SetCallback(hItem, _cbButton_bg);
    
    hItem = BUTTON_CreateEx(400, 340, 80, 80, pMsg->hWin, WM_CF_SHOW, 0, ID_LOCK);
    WM_SetCallback(hItem, _cbButton_lock); 
    
    hItem = WM_GetDialogItem(pMsg->hWin, GO_BACKGROUND);
    BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);    
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_VNC_STATUS);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph32);
    
    break;     
    
  case WM_PAINT:
    
    GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
    GUI_SetPenSize(5);
    GUI_DrawBitmap(&bmconnect, 0, 70);
    GUI_DrawCircle(570, 380, 63);
    GUI_AA_FillCircle(570, 380, 68);
        
    GUI_AA_FillRoundedRect(390, 330, 770, 430, 20);
    
    GUI_SetColor(GUI_WHITE);
    GUI_AA_FillCircle(570, 380, 65);
    GUI_SetBkColor(GUI_WHITE);
    GUI_AA_FillRoundedRect(393, 333, 767, 427, 20);
    
    GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
    GUI_AA_FillCircle(570, 380, 56);
    
    break;    

  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    
    
    switch(Id) {  
      
    case START_VNC: /* Notifications sent by 'Start' */
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        
        if((VNC_GetState() == VNC_IDLE)||
          (VNC_GetState() == VNC_INIT))
        {
          VNC_SERVER_LogMessage("Starting VNC connection...");
          hItem = WM_GetDialogItem(pMsg->hWin, ID_VNC_CONNECTION);
          IMAGE_SetBitmap(hItem, &bmno_connection);
          
          VNC_SetState (VNC_INIT);
          WM_Exec();
          VNC_SERVER_Start();
        }
        else
        {
          VNC_SERVER_Stop();
        }
        break;
        
      case WM_NOTIFICATION_CLICKED:
        hItem = WM_GetDialogItem(pMsg->hWin, START_VNC);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);   
        break;
      }
      break;
    case GO_BACKGROUND: /* Notifications sent by 'Background' */
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        WM_HideWindow(pMsg->hWin);
        module_prop[k_ModuleGetIndex(&vnc_server_board)].win_state = 1;
        ST_AnimatedIconView_SetItemText(hIcon,k_ModuleGetIndex(&vnc_server_board),"vnc server[B]");
        ST_AnimatedIconView_SetDualTextColor(hIcon, ICONVIEW_CI_SEL, GUI_STCOLOR_LIGHTBLUE, GUI_STCOLOR_DARKBLUE); 
        break;
      }
      break;
      
    case ID_BUTTON_EXIT: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        if(VNC_GetState() != VNC_IDLE)
        {
          VNC_SERVER_Stop();
        }
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break; 
      
    case ID_LOCK: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        if(VNC_GetState() == VNC_IDLE)
        {
          if(VNC_GetLockState())
          {
            VNC_SetLockState(0);
          }
          else
          {
            VNC_SetLockState(1);
          }
        }
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
  * @brief  VNC module status
  * @retval VNC module running status
  */
uint8_t VNC_IsRunning(void)
{
  return (module_prop[k_ModuleGetIndex(&vnc_server_board)].in_use);
}
/**
  * @brief  VNC server window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{

 if(module_prop[k_ModuleGetIndex(&vnc_server_board)].win_state == 0)
 {
  hVncWin = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, xpos, ypos);
 }
 else
 {
  WM_ShowWindow(hVncWin); 
  ST_AnimatedIconView_SetItemText(hIcon,k_ModuleGetIndex(&vnc_server_board),"vnc server ");
  ST_AnimatedIconView_SetDualTextColor(hIcon, ICONVIEW_CI_SEL, GUI_STCOLOR_LIGHTBLUE, GUI_STCOLOR_DARKBLUE); 
 }
}

/**
  * @brief  VNC server log message
  * @param  *message: log message.
  * @retval None
  */
void VNC_SERVER_LogMessage (const char *message)
{
  WM_HWIN hItem;
  
  hItem = WM_GetDialogItem(hVncWin, ID_VNC_STATUS);
  TEXT_SetFont(hItem, &GUI_FontLubalGraph32);
  TEXT_SetText(hItem, message);
}


/**
  * @brief  VNC server status
  * @param  status: VNC module status.
  * @retval None
  */
void VNC_SERVER_StatusChanged (uint8_t status)
{
  WM_HWIN hItem;
  
  hItem = WM_GetDialogItem(hVncWin, ID_VNC_CONNECTION);
  
  switch (status)
  {
  case VNC_IDLE:
  case VNC_LINK_DOWN:   
  case VNC_ERROR:  
    IMAGE_SetBitmap(hItem, &bmconnection_ko);
    WM_InvalidateWindow(hItem);
    WM_Update(hItem);        
    break;
  case VNC_LINK_UP:  
  case VNC_WAIT_FOR_ADDRESS:
    IMAGE_SetGIF(hItem, vnc_connection, sizeof(vnc_connection));
    WM_InvalidateWindow(hItem);
    WM_Update(hItem);        
    break;
    
  case VNC_ADDRESS_ASSIGNED:
  case VNC_CONN_ESTABLISHED:
    IMAGE_SetBitmap(hItem, &bmconnection_ok);
    hItem = WM_GetDialogItem(hVncWin, START_VNC);
    WM_InvalidateWindow(hItem);
    WM_Update(hItem);       
    break;
  default:
    break;
  }
}
/**
  * @}
  */

/**
  * @}
  */
