/**
  ******************************************************************************
  * @file    home_alarm_win.c
  * @author  MCD Application Team
  * @brief   home alarm functions
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
#include "home_alarm_res.c"

/** @addtogroup HOME_ALARM_MODULE
  * @{
  */

/** @defgroup HOME_ALARM
  * @brief home alarm routines
  * @{
  */
  
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);


/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  home_alarm_board =
{
  6,
  "home alarm",
  open_alarm,
  0,
  Startup,
  NULL,
}
;

typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t camera         : 5;
    uint32_t alarm          : 2;    
  }b;
}
HomeSettingsTypeDef;

extern uint8_t   SelLayer;
static WM_HWIN          hCameraFrame;

#define CAM_1              0
#define CAM_2              1
#define CAM_3              2
#define CAM_4              3
#define CAM_5              4
#define CAM_DESACTIVATED   5

#define ALARM_ON           0
#define ALARM_OFF          1

uint32_t alarmStatus   = 0;
uint32_t cameraStatus  = 0;


/* Private defines -----------------------------------------------------------*/
#define ID_WINDOW_0               (GUI_ID_USER + 0x00)
#define ID_BUTTON_INFO_CLOSE      (GUI_ID_USER + 0x03)

#define ID_BUTTON_CAMERA       (GUI_ID_USER + 0x10)
#define ID_BUTTON_ALARM        (GUI_ID_USER + 0x11)

#define ID_TEXT_CAMERA1         (GUI_ID_USER + 0x12)
#define ID_TEXT_CAMERA2         (GUI_ID_USER + 0x13)
#define ID_TEXT_ALARM1          (GUI_ID_USER + 0x14)
#define ID_TEXT_ALARM2          (GUI_ID_USER + 0x15)
#define ID_TEXT_ALARM3          (GUI_ID_USER + 0x16)

#define ID_BUTTON_CAM1          (GUI_ID_USER + 0x17)
#define ID_BUTTON_CAM2          (GUI_ID_USER + 0x18)
#define ID_BUTTON_CAM3          (GUI_ID_USER + 0x19)
#define ID_BUTTON_CAM4          (GUI_ID_USER + 0x1A)
#define ID_BUTTON_CAM5          (GUI_ID_USER + 0x1B)
#define ID_BUTTON_CAM5_1        (GUI_ID_USER + 0x1C)

#define  ID_IMAGE               (GUI_ID_USER + 0x1D)

#define ID_BUTTON_EXIT            (GUI_ID_USER + 0x08)
#define ID_BUTTON_EXIT_FROM_CAM   (GUI_ID_USER + 0x20)
#define ID_BUTTON_MENU            (GUI_ID_USER + 0x21)
#define ID_CAM                    (GUI_ID_USER + 0x22)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x64, 0 }, 
  { TEXT_CreateIndirect,      "room",     ID_TEXT_ALARM1,      70,  370,  70,  20,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "alarm",    ID_TEXT_ALARM2,      130, 370,  70,  20,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "(on/off)", ID_TEXT_ALARM3,      95,  390,  70,  20,  0, 0x0,  0 },	
  { TEXT_CreateIndirect,      "watch",    ID_TEXT_CAMERA1,     70,  190,  70,  20,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "room",     ID_TEXT_CAMERA2,     140,  190,      70,  20,  0, 0x0,  0 },	
};

static WM_HWIN                     HOME_hWin; 
static HomeSettingsTypeDef         HomeSettings;
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
  * @brief  Paints camera button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_camera(BUTTON_Handle hObj, uint32_t alarm_status, uint32_t camera_status) {
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
    
  GUI_DrawBitmap(&bmcamera, 0, 0);
  
  if(HomeSettings.b.alarm == ALARM_ON)
  {     
    if(HomeSettings.b.camera != CAM_DESACTIVATED)
    {
      GUI_DrawBitmap(&bmdes_camera, 0, 0);
    }
  }
  else
  {
    GUI_DrawBitmap(&bmcamera, 0, 0);
  }
}

/**
  * @brief  callback for camera button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_camera(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_camera(pMsg->hWin, HomeSettings.b.alarm, HomeSettings.b.camera);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Paints alarm button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_alarm(BUTTON_Handle hObj, uint32_t alarm_status) {
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
    
  if(HomeSettings.b.alarm == ALARM_OFF)
  { 
    GUI_DrawBitmap(&bmdes_alarm, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmalarm, 0, 0);
  }
}

/**
  * @brief  Paints cam1 button 
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_cam1(BUTTON_Handle hObj, uint32_t alarm_status, uint32_t camera_status) {
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  GUI_DrawBitmap(&bmB1_OFF, 0, 0);
  
  if(HomeSettings.b.alarm == ALARM_ON)
  {
    if(HomeSettings.b.camera == CAM_1)
    {
      GUI_DrawBitmap(&bmB1_ON, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmB1_DES, 0, 0);
    }
  }
  else
  {
    GUI_DrawBitmap(&bmB1_OFF, 0, 0);
  }  
}

/**
  * @brief  Paints cam2 button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_cam2(BUTTON_Handle hObj, uint32_t alarm_status, uint32_t camera_status) {
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  GUI_DrawBitmap(&bmB2_OFF, 0, 0);
  
  if(HomeSettings.b.alarm == ALARM_ON)
  {
    if(HomeSettings.b.camera == CAM_2)
    {
      GUI_DrawBitmap(&bmB2_ON, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmB2_DES, 0, 0);
    }
  }
  else
  {
    GUI_DrawBitmap(&bmB2_OFF, 0, 0);
  }  
}

/**
  * @brief  Paints cam3 button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_cam3(BUTTON_Handle hObj, uint32_t alarm_status, uint32_t camera_status) {

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  GUI_DrawBitmap(&bmB3_OFF, 0, 0);
  
  if(HomeSettings.b.alarm == ALARM_ON)
  {
    if(HomeSettings.b.camera == CAM_3)
    {
      GUI_DrawBitmap(&bmB3_ON, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmB3_DES, 0, 0);
    }
  }
  else
  {
    GUI_DrawBitmap(&bmB3_OFF, 0, 0);
  }  
}

/**
  * @brief  Paints cam4 button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_cam4(BUTTON_Handle hObj, uint32_t alarm_status, uint32_t camera_status) {

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  GUI_DrawBitmap(&bmB4_OFF, 0, 0);
  
  if(HomeSettings.b.alarm == ALARM_ON)
  {
    if(HomeSettings.b.camera == CAM_4)
    {
      GUI_DrawBitmap(&bmB4_ON, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmB4_DES, 0, 0);
    }
  }
  else
  {
    GUI_DrawBitmap(&bmB4_OFF, 0, 0);
  }  
}

/**
  * @brief  Paints cam5 button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_cam5(BUTTON_Handle hObj, uint32_t alarm_status, uint32_t camera_status) {

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  GUI_DrawBitmap(&bmB5_OFF, 0, 0);
  
  if(HomeSettings.b.alarm == ALARM_ON)
  {
    if(HomeSettings.b.camera == CAM_5)
    {
      GUI_DrawBitmap(&bmB5_ON, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmB5_DES, 0, 0);
    }
  }
  else
  {
    GUI_DrawBitmap(&bmB5_OFF, 0, 0);
  }  
}

/**
  * @brief  Paints cam5_1 button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_cam5_1(BUTTON_Handle hObj, uint32_t alarm_status, uint32_t camera_status) {

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  GUI_DrawBitmap(&bmB6_OFF, 0, 0);
  
  if(HomeSettings.b.alarm == ALARM_ON)
  {
    if(HomeSettings.b.camera == CAM_5)
    {
      GUI_DrawBitmap(&bmB6_ON, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmB6_DES, 0, 0);
    }
  }
  else
  {
    GUI_DrawBitmap(&bmB6_OFF, 0, 0);
  }  
}

/**
  * @brief  Paints exit button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_exit1(BUTTON_Handle hObj) {

  GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_AA_FillCircle(0, 100, 100);

  GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_FontLubalGraph32);
  GUI_DispStringAt("back", 10, 35); 
}

/**
  * @brief  Paints menu button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_menu(BUTTON_Handle hObj) {

  GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_AA_FillCircle(100, 0, 100);

  GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_FontLubalGraph32);
  GUI_DispStringAt("Menu", 20, 20);
}

/**
  * @brief  callback for alarm button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_alarm(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_alarm(pMsg->hWin, HomeSettings.b.alarm);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for cam1 button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_cam1(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_cam1(pMsg->hWin, HomeSettings.b.alarm, HomeSettings.b.camera);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for cam2 button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_cam2(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_cam2(pMsg->hWin, HomeSettings.b.alarm, HomeSettings.b.camera);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for cam3 button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_cam3(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_cam3(pMsg->hWin, HomeSettings.b.alarm, HomeSettings.b.camera);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for cam4 button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_cam4(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_cam4(pMsg->hWin, HomeSettings.b.alarm, HomeSettings.b.camera);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for cam5 button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_cam5(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_cam5(pMsg->hWin, HomeSettings.b.alarm, HomeSettings.b.camera);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for cam5_1 button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_cam5_1(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_cam5_1(pMsg->hWin, HomeSettings.b.alarm, HomeSettings.b.camera);
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
static void _cbButton_exit1(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_exit1(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
  * @brief  callback for menu button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_menu(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
  case WM_PAINT:
    _OnPaint_menu(pMsg->hWin);
    break;
  default:
    /* The original callback */
    BUTTON_Callback(pMsg);
    break;
  }
}

/**
  * @brief  Callback function of the Camera frame
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbCameraWindow(WM_MESSAGE * pMsg)
{
  int     NCode;
  int     Id;
  WM_HWIN hItem;
    
  switch (pMsg->MsgId)
  {
    
  case WM_CREATE:
    if(HomeSettings.b.camera == CAM_1)
    {
      /* Title Initialization in play list */   
      hItem = TEXT_CreateEx(20, 30, 65, 24, pMsg->hWin, WM_CF_SHOW, 0, ID_CAM, "CAM 1");
      TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
      TEXT_SetTextColor(hItem, GUI_WHITE); 
    }
    else if(HomeSettings.b.camera == CAM_2)
    {
      /* Title Initialization in play list */   
      hItem = TEXT_CreateEx(20, 30, 65, 24, pMsg->hWin, WM_CF_SHOW, 0, ID_CAM, "CAM 2");
      TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
      TEXT_SetTextColor(hItem, GUI_WHITE); 
    }
    else if(HomeSettings.b.camera == CAM_3)
    {
      /* Title Initialization in play list */   
      hItem = TEXT_CreateEx(20, 30, 65, 24, pMsg->hWin, WM_CF_SHOW, 0, ID_CAM, "CAM 3");
      TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
      TEXT_SetTextColor(hItem, GUI_WHITE);
    }
    else if(HomeSettings.b.camera == CAM_4)
    {
      /* Title Initialization in play list */   
      hItem = TEXT_CreateEx(20, 30, 65, 24, pMsg->hWin, WM_CF_SHOW, 0, ID_CAM, "CAM 4");
      TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
      TEXT_SetTextColor(hItem, GUI_WHITE);
    }
    else
    {
      /* Title Initialization in play list */   
      hItem = TEXT_CreateEx(20, 30, 65, 24, pMsg->hWin, WM_CF_SHOW, 0, ID_CAM, "CAM 5");
      TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
      TEXT_SetTextColor(hItem, GUI_WHITE);      
    }  
    
    hItem = BUTTON_CreateEx(700, 0, 100, 100, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_MENU);
    WM_SetCallback(hItem, _cbButton_menu);     
    
    hItem = BUTTON_CreateEx(0, 379, 100, 100, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT_FROM_CAM);
    WM_SetCallback(hItem, _cbButton_exit1); 
    break;
    
  case WM_PAINT:
    if(HomeSettings.b.camera == CAM_1)
    {
      GUI_DrawBitmap(&bmcam1, 0, 0);      
    }
    else if(HomeSettings.b.camera == CAM_2)
    {
      GUI_DrawBitmap(&bmcam2, 0, 0);
    }
    else if(HomeSettings.b.camera == CAM_3)
    {
      GUI_DrawBitmap(&bmcam3, 0, 0);      
    }
    else if(HomeSettings.b.camera == CAM_4)
    {
      GUI_DrawBitmap(&bmcam4, 0, 0);      
    }
    else
    {
      GUI_DrawBitmap(&bmcam5, 0, 0);      
    }  
    GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
    GUI_AA_FillRoundedRect((-30), 10, 100, 75, 30);
    break;
    
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
 
    switch(Id) {
      
    case ID_BUTTON_EXIT_FROM_CAM: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        /* re-initialize the NOR pin*/
        GUI_EndDialog(pMsg->hWin, 0);
        WM_HideWindow(hCameraFrame);        
        WM_Exec();        
        WM_DeleteWindow(hCameraFrame);
        WM_InvalidateWindow(HOME_hWin);
        break;        
      }
      break;

    case ID_BUTTON_MENU: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        GUI_EndDialog(pMsg->hWin, 0);
        WM_DeleteWindow(hCameraFrame);
        WM_DeleteWindow(HOME_hWin);
        break;
      }
      break;
    }
    break;
    
  case WM_DELETE:
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}


static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;

  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    hItem = BUTTON_CreateEx(700, 0, 100, 100, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
    WM_SetCallback(hItem, _cbButton_exit);

    hItem = BUTTON_CreateEx(50, 40, 150, 135, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CAMERA);
    WM_SetCallback(hItem, _cbButton_camera); 

    hItem = BUTTON_CreateEx(50, 235, 150,  135, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_ALARM);
    WM_SetCallback(hItem, _cbButton_alarm);
    
    hItem = BUTTON_CreateEx(410, 302, 100, 133, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CAM1);
    WM_SetCallback(hItem, _cbButton_cam1); 

    hItem = BUTTON_CreateEx(510, 302, 180, 132, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CAM2);
    WM_SetCallback(hItem, _cbButton_cam2); 

    hItem = BUTTON_CreateEx(510, 192, 180, 111, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CAM3);
    WM_SetCallback(hItem, _cbButton_cam3); 

    hItem = BUTTON_CreateEx(510, 60, 180, 132, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CAM4);
    WM_SetCallback(hItem, _cbButton_cam4); 

    hItem = BUTTON_CreateEx(240, 60, 270, 242, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CAM5);
    WM_SetCallback(hItem, _cbButton_cam5); 

    hItem = BUTTON_CreateEx(240, 302, 170, 133, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CAM5_1);
    WM_SetCallback(hItem, _cbButton_cam5_1); 
    
    /* Title Initialization in play list */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CAMERA1);
    TEXT_SetText(hItem, "watch");
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);
	
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CAMERA2);
    TEXT_SetText(hItem, "room");
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE);	
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ALARM1);
    TEXT_SetText(hItem, "room");
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_LIGHTBLUE);			
		
    /* Title Initialization in play list */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ALARM2);
    TEXT_SetText(hItem, "alarm");
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE);    
    
    /* Duration */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_ALARM3);
    TEXT_SetText(hItem, "(on/off)");
    TEXT_SetFont(hItem, &GUI_FontLubalGraph24B);
    TEXT_SetTextColor(hItem, GUI_STCOLOR_DARKBLUE); 
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
    case ID_BUTTON_ALARM: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:

        if(HomeSettings.b.alarm == ALARM_ON)
        {
          HomeSettings.b.alarm = ALARM_OFF;
        }
        else
        {
          HomeSettings.b.alarm = ALARM_ON;
        }
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);    
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM4);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM5);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);  
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM5_1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);        
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAMERA);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem); 
        
        break;         
        
      }
      break;     


    case ID_BUTTON_CAMERA: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:

        if(HomeSettings.b.alarm == ALARM_ON)
        {
          if(HomeSettings.b.camera != CAM_DESACTIVATED)
          {      
            /* Camera frame initialization */            
            hCameraFrame = WM_CreateWindowAsChild(0, 0, 800, 480, pMsg->hWin,  WM_CF_SHOW | WM_CF_LATE_CLIP, _cbCameraWindow, 0);    
          }
        }     
        break;         
        
      }
      break;      
      
    case ID_BUTTON_CAM1: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:

        HomeSettings.b.camera = CAM_1;     
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);    
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM4);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM5);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM5_1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);        
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAMERA);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem); 
        
        break;
      }
      break; 
        
    case ID_BUTTON_CAM2: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:

        HomeSettings.b.camera = CAM_2;    
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);    
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM4);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM5);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM5_1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);  
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAMERA);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem); 
        
        break;
      }
      break;

    case ID_BUTTON_CAM3: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:

        HomeSettings.b.camera = CAM_3; 
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);    
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM4);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM5);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM5_1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);         
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAMERA);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem); 
        
        break;
      }
      break;

    case ID_BUTTON_CAM4: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:

        HomeSettings.b.camera = CAM_4;

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);    
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM5);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM5_1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem); 
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAMERA);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);         
        
        break;
      }
      break;

    case ID_BUTTON_CAM5: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:

        HomeSettings.b.camera = CAM_5;

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);    
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM4);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM5_1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);         
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAMERA);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);         
        
        break;
      }
      break;

    case ID_BUTTON_CAM5_1: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:

        HomeSettings.b.camera = CAM_5;

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM1);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);    
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM2);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM3);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM4);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);

        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAM5);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);           
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CAMERA);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);         
        
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
  * @brief  home alarm window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  HomeSettings.b.alarm  = ALARM_ON;
  HomeSettings.b.camera = CAM_DESACTIVATED;
  HOME_hWin = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, xpos, ypos);
}

/**
  * @}
  */

/**
  * @}
  */
