/**
  ******************************************************************************
  * @file    imagebrowser_win.c
  * @author  MCD Application Team
  * @brief   Image Browser functions
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
#include "k_rtc.h"
#include "imagebrowser_res.c"
#include "k_modules_res.h"
#include "k_storage.h"
#include "k_mem.h"   
#include "../Modules/filebrowser/filebrowser_app.h"

/** @addtogroup IMAGE_BROWSER_MODULE
  * @{
  */

/** @defgroup IMAGE_BROWSER
  * @brief image browser routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t ss_timer : 4;
    uint32_t reserved : 28;  
  }b;
}
ImageSettingsTypeDef;

/* Private defines -----------------------------------------------------------*/

#define OFF               0x00
#define ON                0x01

#define ID_WINDOW_0          (GUI_ID_USER + 0x00)
#define ID_SETTINGS_BUTTON   (GUI_ID_USER + 0x01)
#define ID_CLOSE_BUTTON      (GUI_ID_USER + 0x02)
#define ID_SLIDE_BUTTON      (GUI_ID_USER + 0x03)
#define ID_NEXT_BUTTON       (GUI_ID_USER + 0x04)
#define ID_PREVIOUS_BUTTON   (GUI_ID_USER + 0x05)
#define ID_IMAGE_LIST        (GUI_ID_USER + 0x06)
#define ID_OPEN_BUTTON       (GUI_ID_USER + 0x07)
#define ID_ADD_BUTTON        (GUI_ID_USER + 0x08)
                          
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x10)
#define ID_MULTIPAGE      (GUI_ID_USER + 0x13)
#define ID_BUTTON_OK      (GUI_ID_USER + 0x19)
#define ID_BUTTON_CANCEL  (GUI_ID_USER + 0x1A)
                          
#define ID_TEXT_0         (GUI_ID_USER + 0x23)
#define ID_SPINBOX_0      (GUI_ID_USER + 0x24)
#define ID_TEXT_1         (GUI_ID_USER + 0x25)
#define ID_TEXT_2         (GUI_ID_USER + 0x26)

#define ID_SLIDER_TIMER        1
#define ID_PLAYLIST_TIMER      2


#define IMAGE_TYPE_UNKNOWN  0
#define IMAGE_TYPE_BMP      1
#define IMAGE_TYPE_JPG      2

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] =
{
  { WINDOW_CreateIndirect,  "Window",           ID_WINDOW_0,       0,   0,  240, 294, 0, 0x0, 0 },
  { LISTBOX_CreateIndirect, "List",             ID_IMAGE_LIST,     5, 226, 174, 65, 0, 0x0, 0 }
};

static const GUI_WIDGET_CREATE_INFO _aSettingsDialogCreate[] =
{
  { FRAMEWIN_CreateIndirect,  "Settings",            ID_FRAMEWIN_0,    15, 100, 205, 120, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "OK",                  ID_BUTTON_OK,     135, 10,  55,  30,  0, 0x0, 0 },
  { BUTTON_CreateIndirect,    "Cancel",              ID_BUTTON_CANCEL, 135, 55, 55,  30,  0, 0x0, 0 },
  { TEXT_CreateIndirect,      "Slide show timer : ", ID_TEXT_1,        10,  20, 100, 20,  0, 0x0, 0 },
  { TEXT_CreateIndirect,      "(seconds)",           ID_TEXT_2,        80, 47, 50,  20,  0, 0x0, 0 }, 
  { SPINBOX_CreateIndirect,   "Spinbox",             ID_SPINBOX_0,     10,  43, 60,  25,  0, 0x0, 0 }  
};


static uint8_t _acBuffer[1024];
FIL Image_File;

static WM_HWIN IMAGE_hWin, imFrame;

static WM_HTIMER hTimerTime;

__IO uint32_t slideshow_state;
__IO uint32_t IMAGE_Enlarge;



char const        *apDrives = "0:";
const char        acMask_img[] = ".img";
const char        acMask_dir[] = ".dir";
uint32_t effects;
ImageSettingsTypeDef ImSettings;
static uint16_t          file_pos = 0;
static uint8_t IMAGE_Type = IMAGE_TYPE_UNKNOWN;
static uint8_t              playlist_select = 0;
static WM_HTIMER            hPlaylistTimer;

static  FILELIST_FileTypeDef *pImageList;
static  CHOOSEFILE_INFO      *pFileInfo;
 
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void ImageDirectOpen(char *filename);

K_ModuleItem_Typedef  image_browser =
{
  3,
  "Image Viewer",
  &bmimage,
  Startup,
  ImageDirectOpen,
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Paints Open button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_open(BUTTON_Handle hObj) {
  int Index;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if(Index)
  {
    GUI_DrawBitmap(&bmopen_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmopen_not_pressed, 0, 0);
  }
}

/**
  * @brief  Paints Previous button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_previous(BUTTON_Handle hObj) {
  int Index;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if(Index)
  {
    GUI_DrawBitmap(&bmprevious_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmprevious_not_pressed, 0, 0);
  }
}

/**
  * @brief  Paints Play button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_play(BUTTON_Handle hObj) {
  int Index;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if(Index)
  {
    if(slideshow_state == ON)
    {
      GUI_DrawBitmap(&bmpause_pressed, 0, 0);
    }
    else if(slideshow_state == OFF)
    {
      GUI_DrawBitmap(&bmplay_pressed, 0, 0);
    }
  }
  else
  {
    if(slideshow_state == ON)
    {
      GUI_DrawBitmap(&bmpause_not_pressed, 0, 0);
    }
    else if(slideshow_state == OFF)
    {
      GUI_DrawBitmap(&bmplay_not_pressed, 0, 0);
    }
  }
}

/**
  * @brief  Paints Next button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_next(BUTTON_Handle hObj) {
  int Index;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  if(Index)
  {
    GUI_DrawBitmap(&bmnext_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmnext_not_pressed, 0, 0);
  }
}

/**
  * @brief  Paints Settings button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_settings(BUTTON_Handle hObj) {
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
  * @brief  Paints Close button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_close(BUTTON_Handle hObj) {
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
  * @brief  Paints Add to play list button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_add(BUTTON_Handle hObj) {
  int Index;

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    GUI_DrawBitmap(&bmadd_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmadd_not_pressed, 0, 0);
  }
}

/**
  * @brief  callback for open button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_open(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_open(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg); 
      break;
  }
}

/**
  * @brief  callback for previous button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbButton_previous(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_previous(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for play button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbButton_play(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_play(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for next button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbButton_next(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_next(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for settings button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbButton_settings(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
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
  * @brief  callback for close button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE 
  * @retval None
  */
static void _cbButton_close(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
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
  * @brief  callback for add to play list button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_add(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_add(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Add entire folder to play list.
  * @param  Foldername: pointer to folder name.
  * @retval None
  */
static void _AddEntireFolder(char *Foldername)
{
  FRESULT res = FR_OK;
  FILINFO fno;
  DIR dir;
  char *fn;
  char tmp[FILEMGR_FULL_PATH_SIZE]; 
  WM_HWIN hItem;  
  
  res = f_opendir(&dir, Foldername);
  
  if (res == FR_OK)
  {
    
    while (1)
    {
      res = f_readdir(&dir, &fno);
      
      if (res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }
      
      fn = fno.fname;
      
      if (pImageList->ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == 0)
        {
          if((strstr(fn, ".jpg")) || (strstr(fn, ".bmp")) || (strstr(fn, ".JPG")) || (strstr(fn, ".BMP")))
          {
            strcpy(tmp, Foldername);
            strcat(tmp, "/");
            strcat(tmp, fn);
            
            strncpy((char *)pImageList->file[pImageList->ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
            hItem = WM_GetDialogItem(IMAGE_hWin, ID_IMAGE_LIST);
            FILEMGR_GetFileOnly (tmp, fn);
            LISTBOX_AddString(hItem, fn);
            pImageList->ptr++;
            
          }
        }
      }   
    }
  }
  f_closedir(&dir);
}
/**
  * @brief  Callback function of the settings frame
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbSettingsDialog(WM_MESSAGE * pMsg)
{
  int     Id, NCode;
  WM_HWIN hItem;
  uint32_t tmp_param = 0;
 

  switch (pMsg->MsgId)
  {
  case WM_INIT_DIALOG:
  
    WM_MakeModal(pMsg->hWin);
    /* Settings frame initialization */
    hItem = pMsg->hWin;
    FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);
    
    /* Create and attach the MULTIPAGE dialog windows */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE);
    
    /* 'OK' button initialization */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_OK);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
    
    /* 'Cancel' button initialization */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_CANCEL);
    BUTTON_SetFont(hItem, GUI_FONT_13B_1);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, GUI_FONT_13B_1);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
    
    SPINBOX_SetRange(hItem, 1, 10);
       
    if(ImSettings.b.ss_timer > 0)
    {
       SPINBOX_SetValue(hItem, ImSettings.b.ss_timer);
    }
    else
    {
      ImSettings.b.ss_timer = 1;
      SPINBOX_SetValue(hItem, 1);
    }
    
    break;
    
  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    
    switch (Id)
    {
    /* Notification sent by "OK" button */
    case ID_BUTTON_OK: 
      switch (NCode) {
        
      case WM_NOTIFICATION_RELEASED:      
        
        /* Save image settings before delete settings frame */
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
        ImSettings.b.ss_timer = SPINBOX_GetValue(hItem);
        
        WM_RestartTimer(hTimerTime, (ImSettings.b.ss_timer * 1000));
        
        tmp_param = k_BkupRestoreParameter(CALIBRATION_IMAGE_SETTINGS_BKP);
        
        /* check if new settings have to be saved */
        if(ImSettings.d32 != tmp_param)
        {
          k_BkupSaveParameter(CALIBRATION_IMAGE_SETTINGS_BKP, ImSettings.d32); 
        }   
        
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;
    /* Notification sent by "Cancel" button */  
    case ID_BUTTON_CANCEL: 
      switch (NCode) {
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
  * @brief  called one time at the beginning to retrieve overhead
  *         information and, after this, several times to retrieve the actual image data
  * @param  p:           application defined void pointer
  * @param  ppData:      pointer to the location the requested data resides in
  * @param  NumBytesReq: number of requested bytes.
  * @param  Off:         defines the offset to use for reading the source data
  * @retval The number of bytes which were actually read (int)
  */
static int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off)
{
  unsigned int NumBytesRead;
  FIL * phFile;
  
  phFile = (FIL *)p;

  /* Check buffer size */
  if (NumBytesReq > sizeof(_acBuffer)) {
    NumBytesReq = sizeof(_acBuffer);
  }
  
  /* Set file pointer to the required position */
  f_lseek(phFile, Off);

  /* Read data into buffer */
  f_read(phFile, _acBuffer, NumBytesReq, &NumBytesRead); 

  /* Set data pointer to the beginning of the buffer */
  *ppData =(unsigned char *) _acBuffer;

  /* Return number of available bytes */
  return NumBytesRead;  
}

/**
  * @brief  draw the image after scaling it
  * @param  hFile: pointer to the image file
  * @param  Enlarge: 0 = window mode
  *                  1 = full screen mode
  * @param  type:    specify the image type
  *                  this parameter can be:
  *                  - IMAGE_TYPE_JPG
  *                  - IMAGE_TYPE_BMP
  * @retval None
  */
static int _DrawImage(FIL *hFile, uint8_t Enlarge, uint8_t type) 
{
  int XPos, YPos, XSize, YSize, nx, ny, n;
  GUI_JPEG_INFO Info;
  
  if(type == IMAGE_TYPE_BMP)
  {
    Info.XSize = GUI_BMP_GetXSizeEx(_GetData, hFile);
    Info.YSize = GUI_BMP_GetYSizeEx(_GetData, hFile);    
  }
  else if (type == IMAGE_TYPE_JPG)
  {
    GUI_JPEG_GetInfoEx(_GetData, hFile, &Info);
  }
  
  if((Info.XSize == 0) || (Info.YSize == 0) || 
     (Info.XSize > 1024) || (Info.YSize > 768))
  {
    return 1;
  }
  if(Enlarge == 0)
  {
    nx = 225000 / Info.XSize;
    ny = 154000 / Info.YSize;
  }
  else
  {
    nx = 240000 / Info.XSize;
    ny = 320000 / Info.YSize;
  }
  
  if (nx < ny) {
    n = nx;
  } else {
    n = ny;
  }
  
  XSize = Info.XSize * n / 1000;
  YSize = Info.YSize * n / 1000;
  if(Enlarge == 0)
  {
    XPos = (230 - XSize) / 2;
    YPos = (159 - YSize) / 2;
  }
  else
  {
    XPos = (240 - XSize) / 2;
    YPos = (320 - YSize) / 2;
  }
  if(type == IMAGE_TYPE_BMP)
  {
    return GUI_BMP_DrawScaledEx(_GetData, hFile, XPos, YPos, n, 1000);
  }
  else if (type == IMAGE_TYPE_JPG)
  {
    return GUI_JPEG_DrawScaledEx(_GetData, hFile, XPos, YPos, n, 1000);
  }
  
  return 0;
}

/**
  * @brief  Callback function of the image frame
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbImageWindow(WM_MESSAGE * pMsg)
{
  WM_HWIN  hItem;
  GUI_RECT r;
  int Id;
  const GUI_PID_STATE * pState;
  
  switch (pMsg->MsgId)
  {
  case  WM_TOUCH:
    pState = (const GUI_PID_STATE *)pMsg->Data.p;
    if(pImageList->ptr > 0)
    {
      if (pState) {
        if (pState->Pressed == 1)
        {
          IMAGE_Enlarge ^= 1;
          if (IMAGE_Enlarge)
          {
            WM_AttachWindowAt(pMsg->hWin, WM_HBKWIN, 0, 0);
            WM_SetSize(pMsg->hWin, LCD_GetXSize(), LCD_GetYSize());
          }
          else
          {
            WM_AttachWindowAt(pMsg->hWin, IMAGE_hWin, 5, 5);
            WM_SetSize(pMsg->hWin, 230, 159);
          }
          WM_InvalidateWindow(pMsg->hWin);
        }
      }
    }
    break;
      
    
  case WM_PAINT:
    WM_GetInsideRect(&r);
    GUI_ClearRectEx(&r);
    if (pImageList->ptr > 0)
    {
      if(_DrawImage(&Image_File, IMAGE_Enlarge, IMAGE_Type) > 0)
      {
        f_close(&Image_File);
		GUI_ClearRectEx(&r);
        GUI_DispStringAt("Error : Image cannot be displayed", 18, 80);
        if(hTimerTime != 0)
        {
          slideshow_state = OFF;
          
          hItem = WM_GetDialogItem(IMAGE_hWin, ID_SLIDE_BUTTON);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
    
          
          WM_DeleteTimer(hTimerTime);
          hTimerTime = 0;
        }        
      }
    }
    break;
    
  case WM_TIMER:
    
   Id = WM_GetTimerId(pMsg->Data.v);
            
   if(Id == ID_SLIDER_TIMER)
   {
     if(pImageList->ptr > 1)
     {
       if (file_pos < (pImageList->ptr - 1))
       {
         file_pos++;
       }
       else if (file_pos == (pImageList->ptr - 1))
       {
         file_pos=0;
       }
       
       if((strstr((char *)pImageList->file[file_pos].name, ".bmp")) || (strstr((char *)pImageList->file[file_pos].name, ".BMP")))
       {
         IMAGE_Type = IMAGE_TYPE_BMP;
       }
       else if((strstr((char *)pImageList->file[file_pos].name, ".jpg")) || (strstr((char *)pImageList->file[file_pos].name, ".JPG")))
       {
         IMAGE_Type = IMAGE_TYPE_JPG;
       }
       
       f_close(&Image_File);
       f_open(&Image_File, (char const *)pImageList->file[file_pos].name, FA_OPEN_EXISTING | FA_READ);
       WM_InvalidateWindow(imFrame);
       hItem = WM_GetDialogItem(IMAGE_hWin, ID_IMAGE_LIST);
       LISTBOX_SetSel(hItem, file_pos);
     }
     
     WM_RestartTimer(pMsg->Data.v, (ImSettings.b.ss_timer * 1000));
   }
   else if (Id == ID_PLAYLIST_TIMER)
   {
     playlist_select = 0;
   }    
   break;
    
  case WM_DELETE:
    if(hTimerTime != 0)
    {
      WM_DeleteTimer(hTimerTime);
      hTimerTime = 0;
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
  
}

/**
  * @brief  Callback routine of the dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg)
{
  WM_HWIN  hItem;
  GUI_RECT r;
  int      Id, NCode, Index;
  int      result;
  char tmp[FILEMGR_FILE_NAME_SIZE];
  
  switch (pMsg->MsgId)
  {
  case WM_INIT_DIALOG:
    
    
    pImageList = (FILELIST_FileTypeDef *)k_malloc(sizeof(FILELIST_FileTypeDef));
    pFileInfo = (CHOOSEFILE_INFO *)k_malloc(sizeof(CHOOSEFILE_INFO));
    
    pImageList->ptr = 0;
    file_pos = 0;
    effects = 0;
    
    ImSettings.d32 = k_BkupRestoreParameter(CALIBRATION_IMAGE_SETTINGS_BKP);
    if(ImSettings.b.ss_timer == 0)
    {
      ImSettings.b.ss_timer = 1;
    }

    
    /* Image frame initialization */
    IMAGE_Enlarge = 0;
    hItem = WM_GetClientWindow(pMsg->hWin);
    WM_GetClientRectEx(hItem, &r);
    imFrame = WM_CreateWindowAsChild(r.x0 + 5, r.y0 + 5, r.x1 - 10, r.y1 - 135, hItem, WM_CF_SHOW, _cbImageWindow, 0);
    
    /* Buttons initialization */
    
    hItem = BUTTON_CreateEx(50, 179, 35,  35, pMsg->hWin, WM_CF_SHOW, 0, ID_PREVIOUS_BUTTON);
    WM_SetCallback(hItem, _cbButton_previous);
    
    hItem = BUTTON_CreateEx(95, 172,  50,  50, pMsg->hWin, WM_CF_SHOW, 0, ID_SLIDE_BUTTON);
    WM_SetCallback(hItem, _cbButton_play);
    slideshow_state = OFF;
    
    hItem = BUTTON_CreateEx(155, 179, 35,  35, pMsg->hWin, WM_CF_SHOW, 0, ID_NEXT_BUTTON);
    WM_SetCallback(hItem, _cbButton_next);
    
    
    hItem = BUTTON_CreateEx(184, 229, 50,  30, pMsg->hWin, WM_CF_SHOW, 0, ID_ADD_BUTTON);
    WM_SetCallback(hItem, _cbButton_add);
    
    hItem = BUTTON_CreateEx(184, 260, 50,  30, pMsg->hWin, WM_CF_SHOW, 0, ID_OPEN_BUTTON);
    WM_SetCallback(hItem, _cbButton_open);
    
    hItem = BUTTON_CreateEx(200, 182, 30,  30, pMsg->hWin, WM_CF_SHOW, 0, ID_SETTINGS_BUTTON);
    WM_SetCallback(hItem, _cbButton_settings);
    
    hItem = BUTTON_CreateEx(10, 182, 30,  30, pMsg->hWin, WM_CF_SHOW, 0, ID_CLOSE_BUTTON);
    WM_SetCallback(hItem, _cbButton_close);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_LIST);
    LISTBOX_SetBkColor(hItem, LISTBOX_CI_SEL, GUI_BLUE);
    LISTBOX_SetTextColor(hItem, LISTBOX_CI_SEL, GUI_WHITE);
    LISTBOX_SetBkColor(hItem, LISTBOX_CI_UNSEL, GUI_BLACK);
    LISTBOX_SetTextColor(hItem, LISTBOX_CI_UNSEL, GUI_CYAN);
    LISTBOX_SetAutoScrollV(hItem, 1);
    
    break;
     
  case WM_TIMER:
    playlist_select = 0;
    break; 
    
 case WM_PAINT:
    DrawRect3D(5, 170, 230, 54);
    DrawRect3D(182, 226, 53, 65);
    
    break;
  case WM_NOTIFY_PARENT:
    Id = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    
    switch (Id) {
    /* Notification sent by "Button_Settings" */   
    case ID_SETTINGS_BUTTON:
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        GUI_CreateDialogBox(_aSettingsDialogCreate, GUI_COUNTOF(_aSettingsDialogCreate), _cbSettingsDialog, IMAGE_hWin, 0, 0);
        break;
      }
      break;
      
     /* Notifications sent by 'ListView' Slider */
    case ID_IMAGE_LIST: 
      if(NCode == WM_NOTIFICATION_CLICKED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_LIST);
        Index = LISTBOX_GetSel(hItem);
        
        if(Index < pImageList->ptr)
        {
          if(playlist_select == 0)
          {
            hPlaylistTimer = WM_CreateTimer(pMsg->hWin, ID_PLAYLIST_TIMER, 500, 0);           
            playlist_select = (Index + 1);
          }
          
          else if(playlist_select == (Index + 1))
          {
            WM_DeleteTimer(hPlaylistTimer); 
            hPlaylistTimer = 0;          
            playlist_select = 0;
            
            if(Index < pImageList->ptr)
            {
              file_pos = Index;
              if((strstr((char *)pImageList->file[file_pos].name, ".bmp")) || (strstr((char *)pImageList->file[file_pos].name, ".BMP")))
              {
                IMAGE_Type = IMAGE_TYPE_BMP;
              }
              else if((strstr((char *)pImageList->file[file_pos].name, ".jpg")) || (strstr((char *)pImageList->file[file_pos].name, ".JPG")))
              {
                IMAGE_Type = IMAGE_TYPE_JPG;
              }        
              f_close(&Image_File);
              f_open(&Image_File, (char const *)pImageList->file[file_pos].name, FA_OPEN_EXISTING | FA_READ);
              WM_InvalidateWindow(imFrame);
            }
          }
        }
      }
      break;   
      
    /* Notification sent by "Button_Close" */
    case ID_CLOSE_BUTTON: 
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        f_close(&Image_File);
        k_free(pImageList); 
        k_free(pFileInfo);        
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;
      
    /* Notification sent by "Button_Open" */  
    case ID_OPEN_BUTTON: 
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        
        pFileInfo->pfGetData = k_GetData;
        pFileInfo->pMask = acMask_dir;
        hItem = CHOOSEFILE_Create(pMsg->hWin, 10, 10, 220, 210, &apDrives, 1, 0, "Add a folder", 0, pFileInfo); 
        WM_MakeModal(hItem);
        result = GUI_ExecCreatedDialog(hItem);
        if (result == 0) 
        {    
          if(pImageList->ptr == 0)
          {
            _AddEntireFolder(pFileInfo->pRoot);
            file_pos = 0;
            if((strstr((char *)pImageList->file[file_pos].name, ".bmp")) || (strstr((char *)pImageList->file[file_pos].name, ".BMP")))
            {
              IMAGE_Type = IMAGE_TYPE_BMP;
            }
            else if((strstr((char *)pImageList->file[file_pos].name, ".jpg")) || (strstr((char *)pImageList->file[file_pos].name, ".JPG")))
            {
              IMAGE_Type = IMAGE_TYPE_JPG;
            }
            
            f_open(&Image_File, (char *)pImageList->file[file_pos].name, FA_OPEN_EXISTING | FA_READ);
            WM_InvalidateWindow(imFrame);
          }
          else
          {
            _AddEntireFolder(pFileInfo->pRoot);
          }
        }
        
        break;
      }
      break;
      
    /* Notification sent by "Button_Open" */  
    case ID_ADD_BUTTON: 
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        
        pFileInfo->pfGetData = k_GetData;
        pFileInfo->pMask = acMask_img;
        hItem = CHOOSEFILE_Create(pMsg->hWin, 10, 10, 220, 210, &apDrives, 1, 0, "Add an image to playlist", 0, pFileInfo);    
        WM_MakeModal(hItem);
        result = GUI_ExecCreatedDialog(hItem);
        if (result == 0) 
        {            
          if((strstr(pFileInfo->pRoot, ".jpg")) || (strstr(pFileInfo->pRoot, ".bmp")) || (strstr(pFileInfo->pRoot, ".JPG")) || (strstr(pFileInfo->pRoot, ".BMP")))
          {
            strcpy((char *)pImageList->file[pImageList->ptr].name, pFileInfo->pRoot);
            FILEMGR_GetFileOnly(tmp, (char *)pFileInfo->pRoot);
            hItem = WM_GetDialogItem(IMAGE_hWin, ID_IMAGE_LIST);
            LISTBOX_AddString(hItem, tmp);
            LISTBOX_SetSel(hItem, pImageList->ptr);
            pImageList->ptr++;
            file_pos = pImageList->ptr - 1;
            f_close(&Image_File);
            
            if((strstr((char *)pImageList->file[file_pos].name, ".bmp")) || (strstr((char *)pImageList->file[file_pos].name, ".BMP")))
            {
              IMAGE_Type = IMAGE_TYPE_BMP;
            }
            else if((strstr((char *)pImageList->file[file_pos].name, ".jpg")) || (strstr((char *)pImageList->file[file_pos].name, ".JPG")))
            {
              IMAGE_Type = IMAGE_TYPE_JPG;
            }
            
            f_open(&Image_File, (char *)pImageList->file[file_pos].name, FA_OPEN_EXISTING | FA_READ);
            WM_InvalidateWindow(imFrame);
          }           
        }
        break;
      }
      break;
      
    /* Notification sent by "Button_SlideShow" */  
    case ID_SLIDE_BUTTON: 
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        if(pImageList->ptr > 1)
        {
          f_close(&Image_File);
          f_open(&Image_File, (char const *)pImageList->file[file_pos].name, FA_OPEN_EXISTING | FA_READ);
          
          if((strstr((char *)pImageList->file[file_pos].name, ".bmp")) || (strstr((char *)pImageList->file[file_pos].name, ".BMP")))
          {
            IMAGE_Type = IMAGE_TYPE_BMP;
          }
          else if((strstr((char *)pImageList->file[file_pos].name, ".jpg")) || (strstr((char *)pImageList->file[file_pos].name, ".JPG")))
          {
            IMAGE_Type = IMAGE_TYPE_JPG;
          }
          
          WM_InvalidateWindow(imFrame);
          if (slideshow_state == OFF)
          {
            hTimerTime = WM_CreateTimer(imFrame, ID_SLIDER_TIMER, (ImSettings.b.ss_timer * 1000) , 0);
          }
          else if(hTimerTime != 0)
          {
            WM_DeleteTimer(hTimerTime);
            hTimerTime = 0;
          } 
          
          slideshow_state = (slideshow_state == OFF ? ON : OFF);
        }
        break;
      }
      break;
      
    /* Notification sent by "Button_Next" */  
    case ID_NEXT_BUTTON: 
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        
        if(pImageList->ptr > 0)
        {
          if (file_pos < (pImageList->ptr - 1))
          {
            file_pos++;
            f_close(&Image_File);
          }
          else
          {
            file_pos = 0; 
            f_close(&Image_File);            
          }
            
            if((strstr((char *)pImageList->file[file_pos].name, ".bmp")) || (strstr((char *)pImageList->file[file_pos].name, ".BMP")))
            {
              IMAGE_Type = IMAGE_TYPE_BMP;
            }
            else if((strstr((char *)pImageList->file[file_pos].name, ".jpg")) || (strstr((char *)pImageList->file[file_pos].name, ".JPG")))
            {
              IMAGE_Type = IMAGE_TYPE_JPG;
            }
            
            f_open(&Image_File, (char const *)pImageList->file[file_pos].name, FA_OPEN_EXISTING | FA_READ);
            WM_InvalidateWindow(imFrame);
            hItem = WM_GetDialogItem(IMAGE_hWin, ID_IMAGE_LIST);
            LISTBOX_SetSel(hItem, file_pos);
        }
        break;
      }
      break;
      
    /* Notification sent by "Button_Previous" */  
    case ID_PREVIOUS_BUTTON: 
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:   
        
        if(pImageList->ptr > 0)
        {
          if (file_pos > 0 )
          {
            file_pos--;
            f_close(&Image_File);
          }
          else
          {
            file_pos = (pImageList->ptr - 1);
            f_close(&Image_File);
          }
            
            if((strstr((char *)pImageList->file[file_pos].name, ".bmp")) || (strstr((char *)pImageList->file[file_pos].name, ".BMP")))
            {
              IMAGE_Type = IMAGE_TYPE_BMP;
            }
            else if((strstr((char *)pImageList->file[file_pos].name, ".jpg")) || (strstr((char *)pImageList->file[file_pos].name, ".JPG")))
            {
              IMAGE_Type = IMAGE_TYPE_JPG;
            }
            
            f_open(&Image_File, (char const *)pImageList->file[file_pos].name, FA_OPEN_EXISTING | FA_READ);
            WM_InvalidateWindow(imFrame);
            hItem = WM_GetDialogItem(IMAGE_hWin, ID_IMAGE_LIST);
            LISTBOX_SetSel(hItem, file_pos);
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
  * @brief  Image browser window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  IMAGE_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, xpos, ypos);
}

/**
  * @brief  Image direct open
  * @param  filename: Image file name
  * @retval None
  */
static void ImageDirectOpen(char *filename)
{
  WM_HWIN hItem;  
  static char tmp[FILEMGR_FILE_NAME_SIZE];    
  IMAGE_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_GetDesktopWindowEx(1), 0, 26);

  
  strcpy((char *)pImageList->file[pImageList->ptr].name, filename);
  FILEMGR_GetFileOnly(tmp, (char *)filename);
  hItem = WM_GetDialogItem(IMAGE_hWin, ID_IMAGE_LIST);
  LISTBOX_AddString(hItem, tmp);
  LISTBOX_SetSel(hItem, pImageList->ptr);
  pImageList->ptr++;
  file_pos = pImageList->ptr - 1;
  f_close(&Image_File);
  
  if((strstr((char *)pImageList->file[file_pos].name, ".bmp")) || (strstr((char *)pImageList->file[file_pos].name, ".BMP")))
  {
    IMAGE_Type = IMAGE_TYPE_BMP;
  }
  else if((strstr((char *)pImageList->file[file_pos].name, ".jpg")) || (strstr((char *)pImageList->file[file_pos].name, ".JPG")))
  {
    IMAGE_Type = IMAGE_TYPE_JPG;
  }
  
  f_open(&Image_File, (char *)pImageList->file[file_pos].name, FA_OPEN_EXISTING | FA_READ);
  WM_InvalidateWindow(imFrame);
}
/**
  * @}
  */

/**
  * @}
  */
