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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "DIALOG.h"
#include "MESSAGEBOX.h"
#include "k_rtc.h"
#include "k_log.h"
#include "k_storage.h"
#include "k_module.h"

#include "k_res.c"
#include "cpu_utils.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_MENU
  * @brief Kernel menu routines
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#define ID_TIMER_TIME                  (GUI_ID_USER + 0x00)
#define ID_BUTTON_BKGND                (GUI_ID_USER + 0x01)
#define ID_ICONVIEW_MENU               (GUI_ID_USER + 0x02)
#define ID_MENU_LOG                    (GUI_ID_USER + 0x03)
#define ID_MENU_PMGR                   (GUI_ID_USER + 0x04)
#define ID_MENU_EXIT                   (GUI_ID_USER + 0x05)
#define ID_MENU_PERF                   (GUI_ID_USER + 0x06)


#define ID_FRAMEWIN_PERFORMANCE       (GUI_ID_USER + 0x10)
#define ID_GRAPH_CPU                  (GUI_ID_USER + 0x11)
#define ID_BUTTON_HIDE                (GUI_ID_USER + 0x12)

#define ID_FRAMEWIN_KERNELLOG         (GUI_ID_USER + 0x20)
#define ID_BUTTON_CANCEL_KERNELLOG    (GUI_ID_USER + 0x21)
#define ID_MULTIEDIT_KERNELLOG        (GUI_ID_USER + 0x22)

#define ID_FRAMEWIN_PROCESSMANAGER       (GUI_ID_USER + 0x30)
#define ID_BUTTON_CANCEL_PROCESSMANAGER  (GUI_ID_USER + 0x31)
#define ID_LISTVIEW_PROCESSMANAGER       (GUI_ID_USER + 0x32)

ICONVIEW_Handle hIcon = 0;
 
static GRAPH_DATA_Handle hData = 0;
static GRAPH_SCALE_Handle hScale = 0;
static WM_HWIN  hPerformance = 0;
static WM_HWIN  hProcess = 0;
static WM_HWIN  hLog = 0;
static WM_HWIN  hStatusWin = 0;
int    module_active = (-1);

xTaskStatusType  ProcessStatus[16] ={{0}};
static SystemSettingsTypeDef    settings;
uint8_t strMonth[][12] = {"Jan",
                           "Feb",
                           "Mar",
                           "Apr",
                           "May",
                           "Jun",
                           "Jul",
                           "Aug",
                           "Sep",
                           "Oct",
                           "Nov",
                           "Dec"};

/* Array of menu items */
static MENU_ITEM _aMenuItems[] = 
{
  {"Kernel Log"         , ID_MENU_LOG,  0},
  {"Process Viewer"    , ID_MENU_PMGR, 0},
  {"CPU Usage", ID_MENU_PERF, 0},  
  {0                    , 0           ,  MENU_IF_SEPARATOR},
  {"Cancel"             , ID_MENU_EXIT, 0},
};

/* _aDialogCreate */
static const GUI_WIDGET_CREATE_INFO _aPerformanceDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "CPU Usage", ID_FRAMEWIN_PERFORMANCE, 20,  5,   250, 150, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "", ID_GRAPH_CPU,                        6,   9,   228, 100, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Hide", ID_BUTTON_HIDE,                 80, 110, 80,  30,  0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aKernelLogDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Kernel Log", ID_FRAMEWIN_KERNELLOG,  20,  5,   250, 150, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_BUTTON_CANCEL_KERNELLOG,   80, 110, 80,  30,  0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "Multiedit", ID_MULTIEDIT_KERNELLOG, 6,   9,   228, 100, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aProcessManagerDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Process viewer", ID_FRAMEWIN_PROCESSMANAGER, 20,   5,   250, 150, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Cancel", ID_BUTTON_CANCEL_PROCESSMANAGER,      80,  110, 80,  30,  0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_PROCESSMANAGER,       5,    5,   228, 100, 0, 0x0, 0 },
};

/**
  * @brief  Callback function of the CPU window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbCpuWindow(WM_MESSAGE * pMsg) {
  
  static WM_HTIMER hTimerTime; 
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    /* Create timer */
    hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 400, 0);        
    break;
    
  case WM_TIMER:
    GRAPH_DATA_YT_AddValue(hData , osGetCPUUsage());
    WM_InvalidateWindow(pMsg->hWin);
    WM_RestartTimer(pMsg->Data.v, 400);
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hTimerTime);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Callback function of the Task window
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbTaskWindow(WM_MESSAGE * pMsg) {
  
  static WM_HTIMER hTimerTime; 

  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    /* Create timer */
    hTimerTime = WM_CreateTimer(pMsg->hWin, 0, 1000, 0);        
    break;
    
  case WM_TIMER:
    WM_InvalidateWindow(WM_GetParent(pMsg->hWin));
    WM_RestartTimer(pMsg->Data.v, 1000);
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hTimerTime);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}


/**
  * @brief  Update Log Viewer
  * @param  Msg: pointer to new message to be logged
  * @retval None
  */
void k_UpdateLog(char *Msg)
{
  WM_HWIN hItem;
  
  if(hLog != 0)
  {
    hItem = WM_GetDialogItem(hLog, ID_MULTIEDIT_KERNELLOG);
    MULTIEDIT_SetText(hItem, Msg);
    MULTIEDIT_SetCursorOffset(hItem, LOG_IN_ptr);    
  }
}
/**
  * @brief  Callback function of the kernel log dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbKernelLogDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleVis(hItem, 0);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_KERNELLOG);
    MULTIEDIT_SetFont(hItem, GUI_FONT_8_1);
    MULTIEDIT_SetTextColor(hItem, MULTIEDIT_CI_READONLY, GUI_GREEN); 
    MULTIEDIT_SetBkColor(hItem, MULTIEDIT_CI_READONLY, GUI_BLACK); 
    MULTIEDIT_SetReadOnly(hItem, 1);
    MULTIEDIT_SetText(hItem, (char *)pLOG_CacheBuffer);
    MULTIEDIT_SetCursorOffset(hItem, LOG_IN_ptr);
    MULTIEDIT_SetAutoScrollV(hItem, 1);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_CANCEL_KERNELLOG: /* Notifications sent by 'Cancel' button */
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        GUI_EndDialog(pMsg->hWin, 0); 
        hLog = 0;
        break;
      }
      break;
    case ID_MULTIEDIT_KERNELLOG: /* Notifications sent by 'Multiedit' */
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
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
  * @brief  Update process manager view.
  * @param  hItem: pointer to window handle
  * @retval None
  */
static void _UpdateProcessManagerView(WM_HWIN  hItem) {
  int      Idx;
  char     str[3];
  int16_t  tasks_nbr;
  
  tasks_nbr = uxTaskGetSystemState( ProcessStatus, 16, NULL );
  
  /*Limit view size */
  if(tasks_nbr > 16)
  {
    tasks_nbr = 16;
  }
  
  for (Idx = 0; Idx < tasks_nbr; Idx ++)
  {
    LISTVIEW_SetItemText(hItem, 0, Idx, (char *)(ProcessStatus[Idx].pcTaskName)); 
    sprintf(str, "%lu", ProcessStatus[Idx].uxCurrentPriority);
    LISTVIEW_SetItemText(hItem, 1, Idx, str);
    
    switch (ProcessStatus[Idx].eCurrentState)
    {
    case eReady:
      LISTVIEW_SetItemText(hItem, 2, Idx, "Ready"); 
      break;
      
    case eBlocked:
      LISTVIEW_SetItemText(hItem, 2, Idx, "Blocked"); 
      break;
      
    case eDeleted:
      LISTVIEW_SetItemText(hItem, 2, Idx, "Deleted"); 
      break;
      
    case eSuspended:
      LISTVIEW_SetItemText(hItem, 2, Idx, "Suspended"); 
      break;
      
    case eRunning:
      LISTVIEW_SetItemText(hItem, 2, Idx, "Running"); 
      break;        
      
    default:
      LISTVIEW_SetItemText(hItem, 2, Idx, "Unknown"); 
      break;        
    }
  }
  LISTVIEW_SetSort(hItem, 1, 0);
}
  
/**
  * @brief  Callback function of the process manager dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbProcessManagerDialog(WM_MESSAGE * pMsg) {
  WM_HWIN  hItem;
  int      NCode;
  int      Id, Idx;

  switch (pMsg->MsgId) {
    
  case WM_INIT_DIALOG:

    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleVis(hItem, 0);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_PROCESSMANAGER);
    LISTVIEW_AddColumn(hItem, 100, "Tasks", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 60, "Priority", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 60, "State", GUI_TA_HCENTER | GUI_TA_VCENTER);
    
    LISTVIEW_SetGridVis(hItem, 1);
    LISTVIEW_SetTextAlign(hItem, 0, GUI_TA_LEFT);
    LISTVIEW_SetTextAlign(hItem, 1, GUI_TA_HCENTER);
    LISTVIEW_SetTextAlign(hItem, 2, GUI_TA_HCENTER);
    HEADER_SetDragLimit(LISTVIEW_GetHeader(hItem), 1);
    LISTVIEW_SetCompareFunc(hItem, 1, LISTVIEW_CompareDec);
    LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_SELFOCUS, GUI_LIGHTBLUE);  
  
    for (Idx = 0; Idx < 16; Idx ++)
    {
      LISTVIEW_AddRow(hItem, NULL);
    }
  
    _UpdateProcessManagerView(hItem);
    
    WM_CreateWindowAsChild(470, 0, 10, 10, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbTaskWindow , 0); 
    break;

  case WM_PAINT:
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_PROCESSMANAGER);
    _UpdateProcessManagerView(hItem);
    break;

  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_CANCEL_PROCESSMANAGER: /* Notifications sent by 'Cancel' button */
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        GUI_EndDialog(pMsg->hWin, 0);
        hProcess = 0;       
        break;
      }
      break;
    case ID_LISTVIEW_PROCESSMANAGER: /* Notifications sent by 'Listview' */
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
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
  * @brief  Callback function of the performance dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbPerformanceDialog(WM_MESSAGE * pMsg) {
  int NCode;
  int Id;
  WM_HWIN hGraph;

  GUI_RECT Rect;

  switch (pMsg->MsgId) 
  {
    
  case WM_INIT_DIALOG:

    FRAMEWIN_SetTitleVis(pMsg->hWin, 0);
    hGraph = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_CPU);
    hData = GRAPH_DATA_YT_Create(GUI_LIGHTGREEN, 500, 0, 20);
    GRAPH_SetGridVis(hGraph, 1);
    GRAPH_SetBorder(hGraph, 30, 2, 2, 2); 
    GRAPH_AttachData(hGraph, hData);
    hScale = GRAPH_SCALE_Create(20, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 25);
    GRAPH_AttachScale(hGraph, hScale);  
    GRAPH_SCALE_SetTextColor(hScale, GUI_YELLOW);
    GRAPH_SetGridDistX(hGraph, 25);
    GRAPH_SetGridDistY(hGraph, 25);
    WM_GetClientRect(&Rect);
    WM_CreateWindowAsChild(470, 0, 10, 10, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbCpuWindow , 0); 
    
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
      
    case ID_BUTTON_HIDE: /* Notifications sent by Hide Button */
      switch(NCode) 
      {
      case WM_NOTIFICATION_RELEASED:
        
        if(hPerformance != 0)
        {
          WM_HideWindow(hPerformance);
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
  * @brief  Adds one menu item to the given menu
  * @param  hMenu:    pointer to the handle of menu
  * @param  hSubmenu: pointer to the handle of Sub menu
  * @param  pText:    pointer to menu item description
  * @param  Id:       ID of the menu item
  * @param  Flags:    window creation flags
  * @retval None
  */
static void _AddMenuItem(MENU_Handle hMenu, MENU_Handle hSubmenu, const char* pText, U16 Id, U16 Flags) 
{
  MENU_ITEM_DATA Item;
  Item.pText    = pText;
  Item.hSubmenu = hSubmenu;
  Item.Flags    = Flags;
  Item.Id       = Id;
  MENU_AddItem(hMenu, &Item);
}

/**
  * @brief  The function opens a popup menu at the given position. It returns
  *         immediately after creation. On the first call it creates the menu.
  * @param  hParent:    pointer to the handle of the parent
  * @param  pMenuItems: pointer to menu items 
  * @param  NumItems:   number of menu items 
  * @param  x:          x position of the popup
  * @param  y:          y position of the popup 
  * @retval None
  */
static void _OpenPopup(WM_HWIN hParent, MENU_ITEM * pMenuItems, int NumItems, int x, int y) 
{
  static MENU_Handle hMenu;
  if (!hMenu) {
    int i;
    /* Create the popup window only one time */
    hMenu = MENU_CreateEx(0, 0, 0, 0, WM_UNATTACHED, 0, MENU_CF_VERTICAL, 0);
    MENU_SetBkColor(hMenu, MENU_CI_SELECTED, GUI_LIGHTBLUE);
    
    for (i = 0; i < NumItems; i++) {
      _AddMenuItem(hMenu, 0, pMenuItems[i].sText, pMenuItems[i].Id, pMenuItems[i].Flags);
    }
  }

  MENU_Popup(hMenu, hParent, x, y, 0, 0, 0);
}

/**
  * @brief  Callback routine of desktop window.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbBk(WM_MESSAGE * pMsg) {
  
  MENU_MSG_DATA* pData;
  uint32_t NCode, Id;
  static int sel = (-1);
  
  switch (pMsg->MsgId) 
  {

  case WM_MENU:
    /* Process the menu message */
    pData = (MENU_MSG_DATA*)pMsg->Data.p;
    switch (pData->MsgType) 
    {
    case MENU_ON_ITEMSELECT:
      /* A menu item has been selected */
      switch (pData->ItemId) 
      {  
        
      case ID_MENU_LOG:
        if (hLog == 0)
        {
          hLog = GUI_CreateDialogBox(_aKernelLogDialogCreate, 
                                     GUI_COUNTOF(_aProcessManagerDialogCreate), 
                                     _cbKernelLogDialog, 
                                     pMsg->hWin, 
                                     FRAMEWIN_CF_MOVEABLE | WM_CF_FGND | WM_CF_STAYONTOP, 
                                     50);  
        }
        else
        {
          WM_ShowWindow(hLog);
          WM_BringToTop(hLog);
        }        
        break;
        
      case ID_MENU_PMGR:
        if(hProcess == 0)
        {
          hProcess = GUI_CreateDialogBox(_aProcessManagerDialogCreate, 
                                         GUI_COUNTOF(_aProcessManagerDialogCreate), 
                                         _cbProcessManagerDialog, 
                                         pMsg->hWin, 
                                         FRAMEWIN_CF_MOVEABLE | WM_CF_FGND | WM_CF_STAYONTOP, 
                                         50);  
        }
        else
        {
          WM_ShowWindow(hProcess);
          WM_BringToTop(hProcess);
        }
        break;
        
      case ID_MENU_PERF:
        if(hPerformance == 0)
        {
          hPerformance = GUI_CreateDialogBox(_aPerformanceDialogCreate, 
                                             GUI_COUNTOF(_aPerformanceDialogCreate), 
                                             _cbPerformanceDialog, 
                                             pMsg->hWin, 
                                             FRAMEWIN_CF_MOVEABLE | WM_CF_FGND | WM_CF_STAYONTOP, 
                                             50);
        }
        else
        {
          WM_ShowWindow(hPerformance);
          WM_BringToTop(hPerformance);
        }
        break;
        
      case ID_MENU_EXIT:
        break;
      }
    }
    break;
      
  case WM_PAINT:
    GUI_DrawBitmap(&bmbackground, 0,0);   
    if(hIcon)
    {
      WM_BringToBottom(hIcon);
    }
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);     
    NCode = pMsg->Data.v;  
    
    switch (NCode) 
    {        
    case WM_NOTIFICATION_CLICKED:   
      if (Id == ID_ICONVIEW_MENU)
      { 
        
        sel = ICONVIEW_GetSel(pMsg->hWinSrc);
        
        if(hPerformance != 0)
        {
          WM_HideWindow(hPerformance);
        } 
        
        if((sel >= 0) && (sel < k_ModuleGetNumber()))
        {    
          if(sel < 8)
          {
            ICONVIEW_SetSel(pMsg->hWinSrc, -1);
          }
          else
          {
            ICONVIEW_SetSel(pMsg->hWinSrc, k_ModuleGetNumber() + 1); 
          }
        }
      }    
      break;
      
    case WM_NOTIFICATION_RELEASED:
      if (Id == ID_ICONVIEW_MENU)
      {
        
        if((sel >= 0) && (sel < k_ModuleGetNumber()))
        { 
          module_prop[sel].module->startup(pMsg->hWin, 0, 25);
          module_active = sel;
          sel = -1;
        }
      }
      else if ((Id == ID_BUTTON_BKGND) && module_active != 0)
      {
        /* Create popup menu after touching the display */
        _OpenPopup(WM_HBKWIN, _aMenuItems, GUI_COUNTOF(_aMenuItems),0 , 25);  
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
  * @brief  Callback routine of desktop window status.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbStatus(WM_MESSAGE * pMsg) {
  int xSize, ySize;
  static uint8_t TempStr[50];
  static WM_HTIMER hTimerTime; 

  RTC_TimeTypeDef   RTC_Time;
  RTC_DateTypeDef   RTC_DateStructure;
  uint8_t sec, min, hour, day, month;
  uint16_t year;
  WM_HWIN hWin;

  hWin = pMsg->hWin;
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    hTimerTime = WM_CreateTimer(hWin, ID_TIMER_TIME, 1000, 0);
    break;
    
  case WM_DELETE:
    WM_DeleteTimer(hTimerTime);

    break;
    
  case WM_TIMER:
    if( WM_GetTimerId(pMsg->Data.v) == ID_TIMER_TIME)
    {
      WM_InvalidateWindow(hStatusWin);
      WM_RestartTimer(pMsg->Data.v, 1000);
    }

    break;
    
  case WM_PAINT:
    xSize = WM_GetWindowSizeX(hWin);
    ySize = WM_GetWindowSizeY(hWin);
    
    /* Draw background */
    GUI_SetColor(0x40303030);
    GUI_FillRect(0, 0, xSize , ySize - 3);
    GUI_SetColor(0x808080);
    GUI_DrawHLine(ySize - 2, 0, xSize );
    GUI_SetColor(0x404040);
    GUI_DrawHLine(ySize - 1, 0, xSize );

    /* Draw time & Date */
    GUI_SetTextMode(GUI_TM_TRANS);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_13B_ASCII);
    
    k_GetTime(&RTC_Time);
    sec    =  RTC_Time.Seconds;
    min    =  RTC_Time.Minutes;
    hour   =  RTC_Time.Hours;
    
    k_GetDate(&RTC_DateStructure);
    
    sprintf((char *)TempStr, "%02d:%02d:%02d", hour , min, sec);
    GUI_DispStringAt((char *)TempStr, xSize - 50, 4);
    
    year =  RTC_DateStructure.Year + 2015;
    month =  RTC_DateStructure.Month;
    day =  RTC_DateStructure.Date;
    
    if((day > 0) && (day <= 31) && 
       (month > 0)&& (month <= 12) && 
         (year >= 1900))
    {
      sprintf((char *)TempStr, "%02d, %s, %04d", day , strMonth[month-1], year);
    }
    else
    {
      sprintf((char *)TempStr, "01, Jan, 2015");
    }

    GUI_DispStringHCenterAt((char *)TempStr, (xSize / 2), 4);
    
    /* USB */
    if(k_StorageGetStatus(USB_DISK_UNIT))
    {
       GUI_DrawBitmap(&bmusbdisk, xSize - 80, 0);
    }
    
    /* MSD */
    if(k_StorageGetStatus(MSD_DISK_UNIT))
    {
       GUI_DrawBitmap(&bmmicrosd, xSize - 80 - 30 * k_StorageGetStatus(USB_DISK_UNIT) , 0);
    }    
    
    sprintf((char *)TempStr, "CPU : %d %%", osGetCPUUsage());
    
    if(osGetCPUUsage() > 95 )
    {
      /* waiting for user input when using dialog box*/
      sprintf((char *)TempStr, "CPU : 95 %%");
    }
    GUI_DispStringAt( (char *)TempStr, 50, 4);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Paints of the owner drawn button.
  * @param  hObj: pointer to button handle
  * @retval None
  */
static void _OnPaint(BUTTON_Handle hObj) {
  int Index;

  GUI_RECT Rect;
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();  
  
  WM_GetClientRect(&Rect);
  
  /* Draw button background */
  if(Index)
  {
    GUI_SetColor(0x40000000 | GUI_GRAY);
  }
  else
  {
    GUI_SetColor(0x40000000 | GUI_DARKGRAY);
  }

  GUI_FillRectEx(&Rect);

  /* Draw black shape */
  GUI_SetColor(GUI_DARKGRAY);
  GUI_DrawRectEx(&Rect);
  
  GUI_DrawBitmap(&bmSTLogo40x20, 0, 0);
}

/**
  * @brief  callback for background button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg); 
      break;
  }
}

/**
  * @brief  set the GUI profile
  * @param  None.
  * @retval None
  */
void k_SetGuiProfile(void) 
{
 
  settings.d32 = k_BkupRestoreParameter(CALIBRATION_GENERAL_SETTINGS_BKP);

  if(settings.b.disable_flex_skin == 0)
  {
    FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);  
    PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
    RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
    SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
    SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
    SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
    BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
    
    FRAMEWIN_SetDefaultTextColor(0, GUI_DARKGRAY);
    FRAMEWIN_SetDefaultTextColor(1, GUI_DARKGRAY);      
  }
  else
  {
    FRAMEWIN_SetDefaultSkinClassic();  
    PROGBAR_SetDefaultSkinClassic();
    RADIO_SetDefaultSkinClassic();
    SCROLLBAR_SetDefaultSkinClassic();
    SLIDER_SetDefaultSkinClassic();
    SPINBOX_SetDefaultSkinClassic();
    BUTTON_SetDefaultSkinClassic();
    DROPDOWN_SetDefaultSkinClassic();
    MULTIPAGE_SetDefaultSkinClassic();
    CHECKBOX_SetDefaultSkinClassic();

    FRAMEWIN_SetDefaultTextColor(0, GUI_WHITE);
    FRAMEWIN_SetDefaultTextColor(1, GUI_WHITE);   
  }

  FRAMEWIN_SetDefaultFont(GUI_FONT_10_ASCII);

  FRAMEWIN_SetDefaultClientColor(GUI_LIGHTGRAY | 0x20F50000);  
  FRAMEWIN_SetDefaultTitleHeight(15);
  
  TREEVIEW_SetDefaultBkColor(TREEVIEW_CI_SEL, GUI_LIGHTGRAY | 0x20E50000);
  TREEVIEW_SetDefaultBkColor(TREEVIEW_CI_UNSEL, GUI_LIGHTGRAY | 0x20E50000);
  TREEVIEW_SetDefaultTextColor(TREEVIEW_CI_SEL, GUI_BLUE);
  
  WINDOW_SetDefaultBkColor(GUI_LIGHTGRAY | 0x20E50000);
  MULTIPAGE_SetDefaultBkColor(GUI_LIGHTGRAY | 0x20E50000, 0);
  MULTIPAGE_SetDefaultBkColor(GUI_LIGHTGRAY | 0x20E50000, 1);    
  MULTIPAGE_SetDefaultFont(GUI_FONT_10_ASCII); 
  CHECKBOX_SetDefaultBkColor(GUI_LIGHTGRAY | 0x20E50000);
  
  CALENDAR_SetDefaultSize(CALENDAR_SI_HEADER, 20 );
  CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_X, 20 );
  CALENDAR_SetDefaultSize(CALENDAR_SI_CELL_Y, 18 ); 
  
  MULTIPAGE_SetDefaultBkColor (GUI_WHITE, 1);
    
  SPINBOX_SetDefaultButtonSize(15);
  LISTVIEW_SetDefaultGridColor(GUI_WHITE);
  SCROLLBAR_SetDefaultWidth(16);

  HEADER_SetDefaultBkColor(0x00C5903E);
  HEADER_SetDefaultTextColor(GUI_WHITE);
  SCROLLBAR_SetDefaultColor(GUI_LIGHTGRAY | 0x20F50000, SCROLLBAR_CI_THUMB);  
  SCROLLBAR_SetDefaultColor(GUI_LIGHTGRAY | 0x20F50000, SCROLLBAR_CI_SHAFT);  
  SCROLLBAR_SetDefaultColor(GUI_LIGHTGRAY | 0x20F50000, SCROLLBAR_CI_ARROW);    
  CHOOSEFILE_SetDelim('/');
}

/**
  * @brief  Initializes the menu.
  * @param  None.
  * @retval None
  */
void k_InitMenu(void) 
{

  WM_HWIN  hItem;
  uint8_t i = 0;
  
  
  settings.d32 = k_BkupRestoreParameter(CALIBRATION_GENERAL_SETTINGS_BKP);

  WM_SetCallback(WM_HBKWIN, _cbBk);
  
  hStatusWin = WM_CreateWindowAsChild(0,
                                      0,
                                      LCD_GetXSize(),
                                      25,
                                      WM_HBKWIN, 
                                      WM_CF_SHOW | WM_CF_LATE_CLIP , 
                                      _cbStatus, 
                                      0);
     
  hIcon = ICONVIEW_CreateEx(0, 
                            20, 
                            LCD_GetXSize(), 
                            LCD_GetYSize()- 26, 
                            WM_HBKWIN, 
                            WM_CF_SHOW | WM_CF_HASTRANS,
                            ICONVIEW_CF_AUTOSCROLLBAR_V,
                            ID_ICONVIEW_MENU, 
                            92, 
                            90);
  /* Initialise the selection number */
  ICONVIEW_SetSel(hIcon, (-1));
  
  ICONVIEW_SetFont(hIcon, &GUI_Font13B_ASCII);
  
  ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_LIGHTBLUE |GUI_TRANSPARENT);
  
  ICONVIEW_SetSpace(hIcon, GUI_COORD_Y, 10);
  
  ICONVIEW_SetFrame(hIcon, GUI_COORD_Y, 10);
    
  for (i = 0; i < k_ModuleGetNumber(); i++)
  {
    ICONVIEW_AddBitmapItem(hIcon,module_prop[i].module->icon, (char *)module_prop[i].module->name);
  }

  hItem = BUTTON_CreateEx(0, 0, 40, 23, WM_GetDesktopWindowEx(0), WM_CF_SHOW, 0, ID_BUTTON_BKGND);
  WM_SetCallback(hItem, _cbButton);
}

/**
  * @}
  */

/**
  * @}
  */
