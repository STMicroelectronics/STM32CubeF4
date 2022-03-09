/**
  ******************************************************************************
  * @file    camera_win.c
  * @author  MCD Application Team
  * @brief   Camera functions source file
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
#include "DIALOG.h"
#include "k_module.h"
#include "camera_app.h"
#include "camera_res.c"
#include "k_modules_res.h"
#include "k_storage.h"
#include "k_rtc.h"
#include "k_mem.h"

/** @addtogroup CAMERA_MODULE
  * @{
  */

/** @defgroup CAMERA
* @brief camera routines
* @{
*/

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define ID_WINDOW_0        (GUI_ID_USER + 0x00)
#define ID_BUTTON_CLOSE    (GUI_ID_USER + 0x04)
#define ID_BUTTON_CAPTURE  (GUI_ID_USER + 0x05)
#define ID_BUTTON_SETTINGS (GUI_ID_USER + 0x06)
#define ID_RADIO           (GUI_ID_USER + 0x07)
#define ID_IMAGE           (GUI_ID_USER + 0x08)
#define ID_TEXT            (GUI_ID_USER + 0x09)

#define ID_FRAMEWIN_0      (GUI_ID_USER + 0x10)
#define ID_MULTIPAGE       (GUI_ID_USER + 0x11)
#define ID_BUTTON_OK       (GUI_ID_USER + 0x12)
#define ID_BUTTON_CANCEL   (GUI_ID_USER + 0x13)

#define ID_TEXT_0          (GUI_ID_USER + 0x20)
#define ID_TEXT_1          (GUI_ID_USER + 0x21)
#define ID_TEXT_2          (GUI_ID_USER + 0x22)
#define ID_SLIDER_0        (GUI_ID_USER + 0x23)
#define ID_SLIDER_1        (GUI_ID_USER + 0x24)
#define ID_TEXT_3          (GUI_ID_USER + 0x25)
#define ID_TEXT_4          (GUI_ID_USER + 0x26)

#define ID_FOLDER          (GUI_ID_USER + 0x30)
#define ID_BROWSE          (GUI_ID_USER + 0x31)
#define ID_FOLDER_CAPTION  (GUI_ID_USER + 0x32)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

CameraSettingsTypeDef CameraSettings;

static WM_HWIN CAMERA_hWin, hDialogCameraSettings, hDialogFileControl, hSettings;

WM_HWIN          hCameraFrame;
static __IO uint32_t FrameAvailable = 0;
static WM_HWIN chooser_select_folder;
static uint8_t capture_folder[FILEMGR_FULL_PATH_SIZE];
static uint8_t camera_disabled = 0;

CHOOSEFILE_INFO    *pFileInfo;

extern uint8_t   buffer_camera;

uint8_t  SD_Configured     = 0;
uint8_t  CAMERA_Configured = 0;


static char const        *apDrives[2] = {"0:", "1:"};
const char        acMask_folder[] = ".dir";

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
  { WINDOW_CreateIndirect, "Camera",  ID_WINDOW_0, 0, 0,  320, 215, 0, 0x0,    0 },
  { RADIO_CreateIndirect,  "Radio",   ID_RADIO,   10, 77, 80,  138, 0, 0x1407, 0 },
  { IMAGE_CreateIndirect,  "Image",   ID_IMAGE,   6,  2,  48,  48,  0, 0,      0 },
  { TEXT_CreateIndirect,   "Effects", ID_TEXT,    7,  52, 48,  20,  0, 0x0,    0 },
};

static const GUI_WIDGET_CREATE_INFO _aSettingsDialogCreate[] =
{
  { FRAMEWIN_CreateIndirect,  "Settings",  ID_FRAMEWIN_0,    20,  10, 280, 170, 0, 0x0, 0 },
  { MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE,     10,  15, 190, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "OK",        ID_BUTTON_OK,     210, 18, 50,  50,  0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "Cancel",    ID_BUTTON_CANCEL, 210, 85, 50,  50,  0, 0x0, 0 }
};

static const GUI_WIDGET_CREATE_INFO _aDialogCameraSettings[] =
{
  { WINDOW_CreateIndirect, "",                               0,   0,  0,  260, 140, FRAMEWIN_CF_MOVEABLE },
  { TEXT_CreateIndirect, "Brightness", ID_TEXT_0,            20,  20, 63, 20,  0,   0x64, 0 },
  { TEXT_CreateIndirect, "Contrast", ID_TEXT_1,              20,  60, 61, 20,  0,   0x0,  0 },
  { SLIDER_CreateIndirect, "Slider_Br", ID_SLIDER_0,         100, 20, 80, 20,  0,   0x0,  0 },
  { SLIDER_CreateIndirect, "Slider_Co", ID_SLIDER_1,         100, 60, 80, 20,  0,   0x0,  0 },
  { TEXT_CreateIndirect, "-2   -1   0    1    2", ID_TEXT_3, 100, 05, 80, 20,  0,   0x0,  0 },
  { TEXT_CreateIndirect, "-2   -1   0    1    2", ID_TEXT_4, 100, 45, 80, 20,  0,   0x64, 0 }
};

static const GUI_WIDGET_CREATE_INFO _aDialogFileControl[] = 
{
  { WINDOW_CreateIndirect, "",                                    0, 0, 0, 260, 140, FRAMEWIN_CF_MOVEABLE },
  { TEXT_CreateIndirect, "Capture Folder : ", ID_FOLDER_CAPTION,  15, 30, 120, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect,   "Edit",   ID_FOLDER,                   15, 50, 100, 20, 0,   0x64, 0 },
  { BUTTON_CreateIndirect, "Browse", ID_BROWSE,                   130, 50,  50, 20, 0,  0x0, 0 },
};

GUI_BITMAP frame0 =
{
  160,                                     /* XSize */
  120,                                     /* YSize */
  320,                                     /* BytesPerLine */
  16,                                      /* BitsPerPixel */
  (unsigned char *)(&buffer_camera),  /* Pointer to picture data */
  NULL,                                    /* Pointer to palette */
  GUI_DRAW_BMPM565
};

/* Private function prototypes -----------------------------------------------*/

static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static uint8_t IsFrameAvailable(void);

K_ModuleItem_Typedef  camera_capture =
{
  12,
  "Camera",
  &bmcamera,
  Startup,
  NULL,
}
;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Paints Close button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_close(BUTTON_Handle hObj)
{
  int Index;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if(Index)
  {
    GUI_DrawBitmap(&bmclose_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmclose_not_pressed, 0, 0);
  }
}

/**
  * @brief  Paints Capture button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_capture(BUTTON_Handle hObj)
{
  int Index;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if(Index)
  {
    GUI_DrawBitmap(&bmcapturebutton_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmcapturebutton_not_pressed, 0, 0);
  }
}

/**
  * @brief  Paints Settings button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_settings(BUTTON_Handle hObj)
{
  int Index;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if(Index)
  {
    GUI_DrawBitmap(&bmsettings_button_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmsettings_button_not_pressed, 0, 0);
  }
}

/**
  * @brief  callback for close button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_close(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      _OnPaint_close(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for Capture button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_capture(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      _OnPaint_capture(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for Settings button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_settings(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_PAINT:
      _OnPaint_settings(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Set Default folder (saved in backup SRAM)
  * @param  path : pointer to the Default folder
  * @retval None
  */
static void _Check_DefaultPath (uint8_t *path)
{
  if ((*((char *)(CAMERA_SAVE_PATH )) == '0') || (*((char *)(CAMERA_SAVE_PATH)) == '1'))
  {
    strncpy((char *)path, (char *)(CAMERA_SAVE_PATH), FILEMGR_FULL_PATH_SIZE);
  }
  else
  {
    strcpy((char *)path, "0:");
  }
}

/**
  * @brief  Callback function of the Camera Settings page
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbCameraSettings(WM_MESSAGE * pMsg)
{
  WM_HWIN hItem;

  switch (pMsg->MsgId)
  {
  case WM_INIT_DIALOG:  
    /* Initialization of 'Brightness' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, GUI_FONT_13B_1);
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    
    /* Initialization of 'Contrast' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, GUI_FONT_13B_1);
    TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
    
    /* Initialization of '-2   -1   0    1    2' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    
    /* Initialization of '-2   -1   0    1    2' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
    SLIDER_SetRange(hItem, 1, 5);
    SLIDER_SetNumTicks(hItem, 5);
    if(CameraSettings.b.brightness == 0) 
    {
      CameraSettings.b.brightness = 3;
    }
    SLIDER_SetValue(hItem, CameraSettings.b.brightness);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_1);
    SLIDER_SetRange(hItem, 1, 5);
    SLIDER_SetNumTicks(hItem, 5);
    if(CameraSettings.b.contrast == 0) 
    {
      CameraSettings.b.contrast = 3;
    }
    SLIDER_SetValue(hItem, CameraSettings.b.contrast);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}
/**
  * @brief  Callback function of the File Control page
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbFileControl(WM_MESSAGE * pMsg)
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  int     result;  
  
  switch (pMsg->MsgId)
  {
  case WM_INIT_DIALOG:
    /* Initialization of 'Brightness' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_FOLDER_CAPTION);
    TEXT_SetFont(hItem, GUI_FONT_13B_1);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_FOLDER);
    EDIT_SetText(hItem, (char *)capture_folder);

    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id)
    {
    case ID_BROWSE: /* Notifications sent by 'Radio' */
      switch(NCode)
      {
      case WM_NOTIFICATION_RELEASED:
        pFileInfo->pfGetData = k_GetData;
        pFileInfo->pMask = acMask_folder;
        BSP_SD_Init();
        SD_Configured = 1;
        
        chooser_select_folder = CHOOSEFILE_Create(CAMERA_hWin, 20, 20, 200, 150, apDrives, GUI_COUNTOF(apDrives), 0, "Select a folder", 0, pFileInfo);

        result = GUI_ExecCreatedDialog(chooser_select_folder);
        if (result == 0) 
        {  
          if(((pFileInfo->pRoot[0] == '0' ) || (pFileInfo->pRoot[0] == '1' )))
          {
            hItem = WM_GetDialogItem(hDialogFileControl, ID_FOLDER);
            EDIT_SetText(hItem, (char *)pFileInfo->pRoot);
            chooser_select_folder = 0;
            WM_InvalidateWindow(hDialogFileControl);
            WM_Paint(hDialogFileControl);
            strncpy((char *)(CAMERA_SAVE_PATH),pFileInfo->pRoot , FILEMGR_FULL_PATH_SIZE);
            strncpy((char *)capture_folder,pFileInfo->pRoot , FILEMGR_FULL_PATH_SIZE);
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
  * @brief  Callback function of the Settings frame
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbSettingsDialog(WM_MESSAGE * pMsg)
{
  int     Id, NCode;
  WM_HWIN hItem;
  
  switch (pMsg->MsgId)
  {
  case WM_INIT_DIALOG:
    
    /* Settings frame initialization */
    hItem = pMsg->hWin;
    FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);
    
    /* Create and attach the MULTIPAGE dialog windows */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE);
    
    hDialogCameraSettings = GUI_CreateDialogBox(_aDialogCameraSettings,
                                               GUI_COUNTOF(_aDialogCameraSettings),
                                               &_cbCameraSettings, WM_UNATTACHED, 0, 0);
    MULTIPAGE_AddPage(hItem, hDialogCameraSettings, "Camera Settings");
    
    hDialogFileControl = GUI_CreateDialogBox(_aDialogFileControl,
                                             GUI_COUNTOF(_aDialogFileControl),
                                             &_cbFileControl, WM_UNATTACHED, 0, 0);

    MULTIPAGE_AddPage(hItem, hDialogFileControl, "File Settings");
    
    MULTIPAGE_SelectPage(hItem, 0);
    
    /* 'OK' button initialization */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OK);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
    
    /* 'Cancel' button initialization */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
    
    break;
   
  case WM_DELETE:    
    camera_disabled = 0;   
    hSettings = 0;
    
    if(SD_Configured == 1)
    {
      BSP_CAMERA_Init(RESOLUTION_R160x120);
      CAMERA_Configured = 1;
      /* Apply new settings*/
      SD_Configured = 0;
    }
    
    /* Delete choosfile window */
    WM_DeleteWindow(chooser_select_folder);
       
    break;
    
  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
           
    switch (Id)
    {
    case ID_BUTTON_OK: /* Notification sent by "OK" button */
      switch (NCode)
      {
      case WM_NOTIFICATION_RELEASED:
        
        /* Save camera settings before delete settings frame */
        hItem = WM_GetDialogItem(hDialogCameraSettings, ID_SLIDER_0);
        CameraSettings.b.brightness = SLIDER_GetValue(hItem);
        hItem = WM_GetDialogItem(hDialogCameraSettings, ID_SLIDER_1);
        CameraSettings.b.contrast = SLIDER_GetValue(hItem);
        
        /* Back to normal mode (no effects) */
        hItem = WM_GetDialogItem(CAMERA_hWin, ID_RADIO);
        RADIO_SetValue(hItem, 0);
        if(CAMERA_GetState() != CAMERA_ERROR)
        {
          /* Apply new settings*/
          CAMERA_Set_ContrastBrightness(CameraSettings.b.contrast, CameraSettings.b.brightness);
          BSP_CAMERA_BlackWhiteConfig(CAMERA_BLACK_WHITE_NORMAL); 
        }
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;
    case ID_BUTTON_CANCEL: /* Notification sent by "Cancel" button */
      switch (NCode)
      {
      case WM_NOTIFICATION_RELEASED:
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;
    }
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
  GUI_RECT r;
  
  switch (pMsg->MsgId)
  {
    
  case WM_CREATE:
    CAMERA_Init();     
    break;
    
  case WM_PAINT:
    if(IsFrameAvailable() > 0)
    {
      GUI_DrawBitmap(&frame0, 0, 0);
    }
    else
    {
      WM_GetInsideRect(&r); 
      GUI_ClearRectEx(&r);      
     
      if(CAMERA_GetState() == CAMERA_ERROR)
      {
        GUI_DispStringAt("Error while Initializing Camera Interface", 45,  120); 
        GUI_DispStringAt("Please, check if the camera module is mounted.", 35,  140);
      }
    }    
    break;
    
  case WM_POST_PAINT:
    if(IsFrameAvailable() > 0)
    {
      if(camera_disabled == 0)
      {
        CAMERA_Resume();
      }
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Callback routine of informing user about exploring the disk
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbHint(WM_MESSAGE * pMsg) 
{
  GUI_RECT Rect;
  
  switch (pMsg->MsgId) 
  {
  case WM_PAINT:
    GUI_SetBkColor(GUI_LIGHTBLUE);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font16_1HK);
    GUI_DispStringHCenterAt("Saving image...", 50 , 10);
    GUI_SetFont(GUI_DEFAULT_FONT);
    WM_GetClientRect(&Rect);
    GUI_SetColor(GUI_DARKGRAY);
    GUI_DrawRectEx(&Rect);
    break;
    
  default:
    WM_DefaultProc(pMsg);
    break;  
  }
}

/**
  * @brief  Callback routine of the dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg)
{
  WM_HWIN  hItem, Hint;
  GUI_RECT r;
  int NCode, Id;
  uint32_t tmp_param = 0;
  
  switch (pMsg->MsgId)
  {
  case WM_INIT_DIALOG:
  
    pFileInfo = (CHOOSEFILE_INFO *)k_malloc(sizeof(CHOOSEFILE_INFO));
    CameraSettings.d32 = k_BkupRestoreParameter(CALIBRATION_CAMERA_SETTING_BKP);
    
    /* Initialization of 'Image' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE);
    IMAGE_SetBitmap(hItem, &bmwizard);
    
    /* Initialization of 'Effects' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT);
    TEXT_SetFont(hItem, GUI_FONT_16B_1);
    
    /* Initialization of 'Radio' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO);
    RADIO_SetText(hItem, "None", 0);
    RADIO_SetText(hItem, "B&W", 1);
    RADIO_SetText(hItem, "Negative", 2);
    RADIO_SetText(hItem, "Antique", 3);
    RADIO_SetText(hItem, "Blue", 4);
    RADIO_SetText(hItem, "Green", 5);
    RADIO_SetText(hItem, "Red", 6);
    
    /* Camera frame initialization */
    hItem = WM_GetClientWindow(pMsg->hWin);
    WM_GetClientRectEx(hItem, &r);
    hCameraFrame = WM_CreateWindowAsChild(r.x0 + 79, 
                                          r.y0 + 49, r.x1 - 159, 
                                          r.y1 - 93, 
                                          hItem, 
                                          WM_CF_SHOW | WM_CF_LATE_CLIP, 
                                          _cbCameraWindow, 
                                          0);
    
    /* Buttons initialization */
    hItem = BUTTON_CreateEx(266, 175, 30, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CLOSE);
    WM_SetCallback(hItem, _cbButton_close);
    
    hItem = BUTTON_CreateEx(256, 79, 50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_CAPTURE);
    WM_SetCallback(hItem, _cbButton_capture);
    
    hItem = BUTTON_CreateEx(266, 14, 30, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_SETTINGS);
    WM_SetCallback(hItem, _cbButton_settings);
    
    _Check_DefaultPath (capture_folder);
    FrameAvailable = 0;
    break;

  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    
    if(hSettings == 0)
    {
      switch(Id)
      {
      case ID_RADIO:
        if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
        {
          hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO);
          if(CAMERA_GetState() != CAMERA_ERROR)
          {
            CAMERA_SelectEffect(RADIO_GetValue(hItem));
          }
        }
        break;
      case ID_BUTTON_CLOSE: /* Notifications sent by 'Close' */
        switch(NCode)
        {
        case WM_NOTIFICATION_RELEASED:
          k_free(pFileInfo);
          CAMERA_Stop();
          GUI_EndDialog(pMsg->hWin, 0);
          break;
        }
        break;
      case ID_BUTTON_CAPTURE: /* Notifications sent by 'OK' */
        switch(NCode)
        {
        case WM_NOTIFICATION_RELEASED:
          /* Show Hint */
          if(CAMERA_GetState() == CAMERA_OK)
          {
            
            Hint = WM_CreateWindowAsChild(100,
                                          90,
                                          120, 32,
                                          pMsg->hWin,
                                          WM_CF_SHOW , 
                                          _cbHint, 
                                          0);
            
            GUI_Exec();
            if(CAMERA_GetState() == CAMERA_OK)
            {             
              CAMERA_SaveToFile(capture_folder);
            }
            WM_DeleteWindow(Hint);
          }
          break;
        }
        break;
      case ID_BUTTON_SETTINGS: /* Notifications sent by 'Settings' */
        switch(NCode)
        {
        case WM_NOTIFICATION_RELEASED:
          camera_disabled = 1;
          hSettings = GUI_CreateDialogBox(_aSettingsDialogCreate, GUI_COUNTOF(_aSettingsDialogCreate), _cbSettingsDialog, CAMERA_hWin, 0, 0);
          break;
        }
        break;
      }
    }
    break;
  case WM_DELETE:
    tmp_param = k_BkupRestoreParameter(CALIBRATION_CAMERA_SETTING_BKP);
    
    /* check if new settings have to be saved */
    if(CameraSettings.d32 != tmp_param)
    {
      k_BkupSaveParameter(CALIBRATION_CAMERA_SETTING_BKP, CameraSettings.d32); 
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  Camera window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  camera_disabled = 0;
  CAMERA_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, xpos, ypos);
}


/**
  * @brief  Frame Event callback.
  * @param  None
  * @retval None
*/
void BSP_CAMERA_FrameEventCallback(void)
{
  CAMERA_Suspend();
  if(FrameAvailable == 0)
  {
    FrameAvailable = 1;
  }
  WM_InvalidateWindow(hCameraFrame);
}

/**
  * @brief  Check whether the first Frame is Ready
  * @param  None
  * @retval Init status
  */
static uint8_t IsFrameAvailable(void)
{
  return FrameAvailable;
}


/**
* @}
*/ 

/**
* @}
*/ 
