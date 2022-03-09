/**
  ******************************************************************************
  * @file    system_win.c
  * @author  MCD Application Team
  * @brief   System information functions
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
#include "system_res.c"


/** @addtogroup SYSTEM_MODULE
  * @{
  */

/** @defgroup SYSTEM
  * @brief system routines 
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  system_info =
{
  4,
  "System",
  &bminfo,
  Startup,
  NULL,
}
;

SystemSettingsTypeDef settings;



/* Private defines -----------------------------------------------------------*/

#define ID_FRAMEWIN_INFO        (GUI_ID_USER + 0x02)
#define ID_IMAGE_INFO           (GUI_ID_USER + 0x03)
#define ID_TEXT_BOARD           (GUI_ID_USER + 0x04)
#define ID_TEXT_CORE            (GUI_ID_USER + 0x05)
#define ID_TEXT_CPU             (GUI_ID_USER + 0x06)
#define ID_TEXT_VERSION         (GUI_ID_USER + 0x07)
#define ID_TEXT_COPYRIGHT       (GUI_ID_USER + 0x08)
#define ID_MULTIPAGE            (GUI_ID_USER + 0x0A)
#define ID_IMAGE_LOGO           (GUI_ID_USER + 0x0B)    

#define ID_CHECKBOX_SPRITES           (GUI_ID_USER + 0x12)
#define ID_CHECKBOX_CPU180            (GUI_ID_USER + 0x14)
#define ID_CHECKBOX_FLEXSKIN          (GUI_ID_USER + 0x16)

#define ID_TEXT_WARNING0                  (GUI_ID_USER + 0x22)
#define ID_TEXT_WARNING1                 (GUI_ID_USER + 0x23)
#define ID_SPINBOX_HOUR  (GUI_ID_USER + 0x25)
#define ID_TEXT_0  (GUI_ID_USER + 0x26)
#define ID_TEXT_1  (GUI_ID_USER + 0x27)
#define ID_SPINBOX_MINUTE  (GUI_ID_USER + 0x28)
#define ID_BUTTON_APPLYDATESETTINGS  (GUI_ID_USER + 0x29)
#define ID_BUTTON_APPLYCLKSETTINGS  (GUI_ID_USER + 0x2B)
#define ID_BUTTON_1  (GUI_ID_USER + 0x2A)
#define ID_TEXT_2  (GUI_ID_USER + 0x2C)
#define ID_CALENDAR (GUI_ID_USER + 0x2D)
#define ID_SPINBOX_SEC  (GUI_ID_USER + 0x2E)

#define  PI                                3.14
#define  AA_FACTOR                         3

#define X0                                 49
#define Y0                                 48
/*********************************************************************
*
*       _aDialog
*
* Purpose
*   Dialog resource using a WINDOW widget
*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  
  { FRAMEWIN_CreateIndirect, "System", ID_FRAMEWIN_INFO, 0, 0, 240, 294, 0, 0x64, 0 },
  { MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE, 5, 20, 220, 240, 0, 0x0, 0 },  
};

static const GUI_WIDGET_CREATE_INFO _aDialogSystemInformation[] = {
  { WINDOW_CreateIndirect, "System Information", 0, 0,   0, 220, 240, FRAMEWIN_CF_MOVEABLE },
  { TEXT_CreateIndirect, "Board : STM32429I-DISCO", ID_TEXT_BOARD, 10, 35, 200, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Core: STM32429", ID_TEXT_CORE, 10, 55, 200, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "CPU Speed : up to 180MHz", ID_TEXT_CPU, 10, 75, 200, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Firmware Version : 1.5.0", ID_TEXT_VERSION, 10, 95, 200, 20, 0, 0x0, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_LOGO, 90, 130, 40, 20, 0, 0, 0 },  
  { TEXT_CreateIndirect, "Copyright (c) STMicroelectronics 2017", ID_TEXT_COPYRIGHT, 10, 160, 200, 20, 0, 0x0, 0 },  
};

static const GUI_WIDGET_CREATE_INFO _aDialogGeneralSettings[] = {
  { WINDOW_CreateIndirect, "General Settings", 0, 0,   0, 220, 240, FRAMEWIN_CF_MOVEABLE },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_SPRITES, 26, 30, 147, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_CPU180, 26, 60, 215, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_FLEXSKIN, 26, 90, 147, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "[!! Restart the demonstration to apply", ID_TEXT_WARNING0, 6, 180, 210, 20, 0, 0x0, 0 }, 
  { TEXT_CreateIndirect, "    the new settings !!]", ID_TEXT_WARNING1, 6, 200, 210, 20, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogDateSettings[] = {
  { WINDOW_CreateIndirect, "Date Settings", 0, 0,   0, 220, 240, FRAMEWIN_CF_MOVEABLE },
  { BUTTON_CreateIndirect, "Apply settings", ID_BUTTON_APPLYDATESETTINGS, 50, 180, 120, 37, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogClockSettings[] = {
  { WINDOW_CreateIndirect, "Clock Settings", 0, 0,   0, 220, 240, FRAMEWIN_CF_MOVEABLE },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_HOUR, 26, 123, 37, 30, 0, 0x0, 0 },  
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_MINUTE, 90, 123, 36, 30, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_SEC, 153, 123, 37, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Apply settings", ID_BUTTON_APPLYCLKSETTINGS, 50, 180, 120, 37, 0, 0x0, 0 },
};

static const GUI_POINT aPoints[3][4] = {
   
  /* Hour Needle */
  {{ 0 * AA_FACTOR, 2 * AA_FACTOR},
   {-1 * AA_FACTOR,-4 * AA_FACTOR},
   { 0 * AA_FACTOR,-6 * AA_FACTOR},
   { 1 * AA_FACTOR,-4 * AA_FACTOR}},

  /* Min Needle */
  {{ 0 * AA_FACTOR, 2 * AA_FACTOR},
   {-1 * AA_FACTOR,-2 * AA_FACTOR},
   { 0 * AA_FACTOR,-8 * AA_FACTOR},
   { 1 * AA_FACTOR,-2 * AA_FACTOR}},
   /* Sec Needle */
  {{0 * AA_FACTOR, 1 * AA_FACTOR},
   { 1 * AA_FACTOR, 1 * AA_FACTOR},
   { 1 * AA_FACTOR,-34 * AA_FACTOR},
   {0 * AA_FACTOR,-34 * AA_FACTOR}},
};


GUI_POINT aPointsDest[3][4];
/*******************************************************************
*
* Drawing routine
*
********************************************************************
*/
static void DrawNeedle(uint32_t index, uint16_t x0, uint16_t y0) 
{
  /* draw Needles */
  if(index == 2)
  {
    GUI_SetColor(GUI_RED);
    GUI_AA_FillPolygon(aPointsDest[index], 4, AA_FACTOR * x0, AA_FACTOR * y0);
  }
  else 
  {
    GUI_SetColor(GUI_LIGHTBLUE);
    GUI_AA_FillPolygon(aPointsDest[index], 4, AA_FACTOR * x0, AA_FACTOR * y0);
  }
}

/**
  * @brief  Update clock
  * @param  x0:   x position
  * @param  y0:   y position
  * @param  hour: updated hour
  * @param  min:  updated minute
  * @param  sec:  updated second      
  * @retval None
  */
static void GUI_UpdateClock (uint16_t x0, uint16_t y0, uint8_t hour, uint8_t min, uint8_t sec)
{
  int8_t i = 0;
  int32_t SinHQ, CosHQ ,a = 0;
  uint16_t xPos, yPos;
    
  GUI_AA_EnableHiRes();
  GUI_AA_SetFactor(AA_FACTOR);
  
  GUI_SetColor(GUI_LIGHTGRAY);   
  GUI_AA_DrawArc(AA_FACTOR * x0, AA_FACTOR * y0, AA_FACTOR * 39, AA_FACTOR * 39, 0, 360);  
  GUI_SetColor(GUI_GRAY);  
  GUI_AA_DrawArc(AA_FACTOR * x0, AA_FACTOR * y0, AA_FACTOR * 40, AA_FACTOR * 40, 0, 360);  
  GUI_SetColor(GUI_WHITE);  
  GUI_AA_FillCircle(AA_FACTOR * x0, AA_FACTOR * y0, AA_FACTOR * 38); 
  GUI_SetBkColor(GUI_TRANSPARENT);

  GUI_SetBkColor(GUI_WHITE);
  GUI_SetColor(GUI_GRAY);  
  GUI_DispStringAt("12", x0 - 5, y0 - 38 + 5);
  GUI_DispStringAt( "6", x0 - 2, y0 + 38 - 13);
  GUI_DispStringAt( "9", x0 - 38 + 5, y0 - 5);
  GUI_DispStringAt( "3", x0 + 38 - 10, y0 - 3);
  
  for (i = 0; i <= 12; i++) 
  {
    a = i * 30000;
    SinHQ = GUI__SinHQ(a);
    CosHQ = GUI__CosHQ(a);

    xPos = x0 + ((36 * CosHQ) >> 16);
    yPos = y0 - ((36 * SinHQ) >> 16);
    
    GUI_AA_FillCircle(AA_FACTOR * xPos, AA_FACTOR * yPos, AA_FACTOR * 1); 
  }
  
  GUI_MagnifyPolygon(aPointsDest[0], aPoints[0], 4, 4);
  GUI_RotatePolygon(aPointsDest[0], aPointsDest[0], 4, - 2 * PI * (float)((float)hour + (float)min /60) / 12);    
  DrawNeedle(0, x0, y0);
  
  GUI_MagnifyPolygon(aPointsDest[1], aPoints[1], 4, 4);
  GUI_RotatePolygon(aPointsDest[1], aPointsDest[1], 4, - 2 * PI * (float)((float)min + (float)sec / 60) / 60);
  DrawNeedle(1, x0, y0);  
  
  GUI_MagnifyPolygon(aPointsDest[2], aPoints[2], 4, 1);
  GUI_RotatePolygon(aPointsDest[2], aPointsDest[2], 4, - 2 * PI * sec / 60);    
  DrawNeedle(2, x0, y0); 
  GUI_AA_DisableHiRes();
}

/**
  * @brief  callback for System Information
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */
static void _cbSystemInformation(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  char str[40];
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    /* Initialization of 'Board : STM324x9I' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BOARD);
    TEXT_SetFont(hItem, GUI_FONT_13_ASCII);
    TEXT_SetTextColor(hItem, GUI_DARKGRAY);

    /* Initialization of 'Core: STM32F-4 Series' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CORE);
    TEXT_SetFont(hItem, GUI_FONT_13_ASCII);
    TEXT_SetTextColor(hItem, GUI_DARKGRAY);

    /* Initialization of 'CPU Speed : 180MHz' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CPU);
    TEXT_SetFont(hItem, GUI_FONT_13_ASCII);
    TEXT_SetTextColor(hItem, GUI_DARKGRAY);

    /* Initialization of 'Firmware Version : 1.0' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VERSION);
    TEXT_SetFont(hItem, GUI_FONT_13_ASCII);
    TEXT_SetTextColor(hItem, GUI_DARKGRAY);
    strcpy(str, "Demo rev: 1.5.0");
    TEXT_SetText(hItem, str);

    /* ST Copyright */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_COPYRIGHT);
    TEXT_SetFont(hItem, GUI_FONT_13_ASCII);
    TEXT_SetTextColor(hItem, GUI_DARKGRAY);
    
    /* Set ST Logo */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_LOGO);
    IMAGE_SetBitmap(hItem, &bmSTLogo40x20);
    
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}


/**
  * @brief  callback for General Settings
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */

static void _cbGeneralSettings(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  uint32_t tmp = 0;
  
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) {
    
  case WM_INIT_DIALOG:    
    
    settings.d32 = k_BkupRestoreParameter(CALIBRATION_GENERAL_SETTINGS_BKP);
    
    /* Initialization of 'Checkbox' (sprite field) */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_SPRITES);
    CHECKBOX_SetText(hItem, "Enable Sprites");
    CHECKBOX_SetState(hItem, settings.b.enable_sprite);
        
    /* Initialization of 'Checkbox' (CPU field) */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_CPU180);
    CHECKBOX_SetText(hItem, "Run CPU at 180 MHz");
    CHECKBOX_SetState(hItem, settings.b.use_180Mhz);      
    /* Initialization of 'Checkbox' (flex skin field) */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_FLEXSKIN);
    CHECKBOX_SetText(hItem, "Disable Flex skin");
    CHECKBOX_SetState(hItem, settings.b.disable_flex_skin);       
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WARNING0);
    TEXT_SetTextColor(hItem, GUI_DARKRED);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_WARNING1);
    TEXT_SetTextColor(hItem, GUI_DARKRED);
    break;
    
  case WM_DELETE:  
    /* Save Setting before delete settings frame */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_SPRITES);
    settings.b.enable_sprite = CHECKBOX_IsChecked(hItem);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_CPU180);
    settings.b.use_180Mhz = CHECKBOX_IsChecked(hItem);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_FLEXSKIN);
    settings.b.disable_flex_skin = CHECKBOX_IsChecked(hItem);
    
    tmp = k_BkupRestoreParameter(CALIBRATION_GENERAL_SETTINGS_BKP);
	
    /* check if new settings have to be saved */
    if(settings.d32 != tmp)
    {
      k_BkupSaveParameter(CALIBRATION_GENERAL_SETTINGS_BKP, settings.d32); 
    }
    break;    
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  callback for Date Settings
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */

static void _cbDateSettings(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;  
  RTC_DateTypeDef          Date;  
  static CALENDAR_DATE  hDate;
  
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) {
    
  case WM_INIT_DIALOG:
    
    k_GetDate(&Date);  

    CALENDAR_Create(pMsg->hWin,12, 12, 2015 + Date.Year, Date.Month, Date.Date, 2, ID_CALENDAR, WM_CF_SHOW);
    CALENDAR_SetDefaultFont(CALENDAR_FI_CONTENT,GUI_FONT_13_1 );
    CALENDAR_SetDefaultFont(CALENDAR_FI_HEADER, GUI_FONT_13_1);
      
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    switch (NCode) {
      
    case WM_NOTIFICATION_RELEASED: 
      switch (Id) {
      case ID_BUTTON_APPLYDATESETTINGS:

        hItem = WM_GetDialogItem(pMsg->hWin, ID_CALENDAR);
        CALENDAR_GetSel (hItem, &hDate);
        
        if((hDate.Day > 0) && (hDate.Day <= 31) && 
           (hDate.Month > 0)&& (hDate.Month <= 12) && 
             (hDate.Year >= 1900))
        {
          Date.Date = hDate.Day;
          Date.Month = hDate.Month;
          Date.Year = hDate.Year - 2015;
          Date.WeekDay = 0;
          k_SetDate(&Date);
        }
        break;  
      }
      break;
    }      
    break;    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  callback for Clock Settings
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */

static void _cbClockSettings(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;  
  RTC_TimeTypeDef          Time; 
  static WM_HTIMER hTimerTime;
  static uint8_t DisableAutoRefresh = 0;
  
  hItem = pMsg->hWin;
  switch (pMsg->MsgId) {
    
  case WM_TIMER:
    WM_InvalidateWindow(pMsg->hWin);
    WM_RestartTimer(pMsg->Data.v, 1000);
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hTimerTime);
    DisableAutoRefresh = 0;
    break;
    
  case WM_INIT_DIALOG:
    
    hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 1000, 0); 
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOUR);
    SPINBOX_SetRange(hItem, 0, 23);
      
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MINUTE);
    SPINBOX_SetRange(hItem, 0, 59);
      
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_SEC);
    SPINBOX_SetRange(hItem, 0, 59);
      
    break;
    
  case WM_PAINT:
    if(DisableAutoRefresh == 0)
    {
      k_GetTime(&Time);   
      GUI_UpdateClock (110, 60, Time.Hours, Time.Minutes, Time.Seconds);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOUR);
      SPINBOX_SetValue(hItem, Time.Hours);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MINUTE);
      SPINBOX_SetValue(hItem, Time.Minutes);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_SEC);
      SPINBOX_SetValue(hItem, Time.Seconds); 
    }
    else
    {
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOUR);
      Time.Hours = SPINBOX_GetValue(hItem);
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MINUTE);
      Time.Minutes = SPINBOX_GetValue(hItem); 
      
      hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_SEC);
      Time.Seconds = SPINBOX_GetValue(hItem); 
      
      GUI_UpdateClock (110, 60, Time.Hours, Time.Minutes, Time.Seconds);      
    }
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    switch (NCode) {
      
    case WM_NOTIFICATION_CLICKED:      /* React only if released */
      switch (Id) {
      case ID_SPINBOX_HOUR:
      case ID_SPINBOX_MINUTE:
      case ID_SPINBOX_SEC:
        if(DisableAutoRefresh == 0)
        {         
          DisableAutoRefresh = 1;
        } 
        break;        
      }
      break;
      
    case WM_NOTIFICATION_RELEASED: 
      switch (Id) {
      case ID_BUTTON_APPLYCLKSETTINGS:
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_SEC);
        Time.Seconds = SPINBOX_GetValue(hItem);
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_MINUTE);
        Time.Minutes = SPINBOX_GetValue(hItem); 
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_HOUR);
        Time.Hours = SPINBOX_GetValue(hItem); 
        
        k_SetTime(&Time);

        DisableAutoRefresh = 0;
        break;  
      }
      break;
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

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem, hDialog;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    /* Initialization of 'System Information'  */
    hItem = pMsg->hWin;
    FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);    
    
    /* Initialization of 'Image' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_INFO);
    IMAGE_SetBitmap(hItem, &bminfo);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE);
    

   /* Create and attach the MULTIPAGE dialog windows */
    hDialog = GUI_CreateDialogBox(_aDialogSystemInformation, 
                                  GUI_COUNTOF(_aDialogSystemInformation), 
                                  &_cbSystemInformation, WM_UNATTACHED, 0, 0);
    MULTIPAGE_AddPage(hItem, hDialog, "System Information");
    
    hDialog = GUI_CreateDialogBox(_aDialogGeneralSettings, 
                                  GUI_COUNTOF(_aDialogGeneralSettings), 
                                  &_cbGeneralSettings, WM_UNATTACHED, 0, 0);
    MULTIPAGE_AddPage(hItem, hDialog, "General Settings");
    
    hDialog = GUI_CreateDialogBox(_aDialogDateSettings, 
                                  GUI_COUNTOF(_aDialogDateSettings), 
                                  &_cbDateSettings, WM_UNATTACHED, 0, 0);
    
    MULTIPAGE_AddPage(hItem, hDialog, "Date Settings");
    
    hDialog = GUI_CreateDialogBox(_aDialogClockSettings, 
                                  GUI_COUNTOF(_aDialogClockSettings), 
                                  &_cbClockSettings, WM_UNATTACHED, 0, 0);
    MULTIPAGE_AddPage(hItem, hDialog, "Clock Settings");
    
    MULTIPAGE_SelectPage(hItem, 0);  
    
    break;
   
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  System info window Startup
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
