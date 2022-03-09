/**
  ******************************************************************************
  * @file    benchmark_win.c
  * @author  MCD Application Team
  * @brief   CPU benchmark functions
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
#include "benchmark_res.c"
#include "cpu_utils.h"
#include "k_rtc.h" 
#include "k_modules_res.h"

/** @addtogroup BENCHMARK_MODULE
  * @{
  */

/** @defgroup BENCHMARK
 * @brief benchmark routines description
 * @{
 */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static int Run_SpeedTest(void);
extern void RestoreDefaultFillProcess(void);

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define ID_FRAMEWIN_INFO        (GUI_ID_USER + 0x01)
#define ID_IMAGE_LIST           (GUI_ID_USER + 0x02)
#define ID_FRAMEWIN_CPU         (GUI_ID_USER + 0x03)
#define ID_CPU_GRAPH            (GUI_ID_USER + 0x04)
#define ID_TEXT_CPU             (GUI_ID_USER + 0x05)
#define ID_BENCH_CPU            (GUI_ID_USER + 0x06)
/* ID of system menu in Kernel */
#define ID_BUTTON_BKGND         (GUI_ID_USER + 0x01)
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
K_ModuleItem_Typedef  cpu_bench =
{
  1,
  "Benchmark",
  &bmcpu,
  Startup,
  NULL,
};

static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { FRAMEWIN_CreateIndirect, "Benchmark", ID_FRAMEWIN_INFO, 0, 0, 240, 294, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "00000000 Pixels/s ", ID_BENCH_CPU, 60 , 150, 160, 20, 0, 0, 0 }, 
  { BUTTON_CreateIndirect, "Start speed Benchmark", ID_BENCH_CPU, 40 , 170 , 160, 20, 0, 0, 0 },
  { LISTBOX_CreateIndirect, "List", ID_IMAGE_LIST, 5, 200, 220, 65, LISTBOX_CF_AUTOSCROLLBAR_V, 0x0, 0 }
};

static const GUI_COLOR _aColor[8] = {
  0x000000, 
  0x0000FF, 
  0x00FF00, 
  0x00FFFF, 
  0xFF0000, 
  0xFF00FF, 
  0xFFFF00, 
  0xFFFFFF
};

GRAPH_DATA_Handle hData;
GRAPH_SCALE_Handle hScale;
uint32_t Stop_Test = 0;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Get the number of pixels per second
  * @param  None
  * @retval Pixels per second
  */
static U32 _GetPixelsPerSecond(void) {
  GUI_COLOR Color, BkColor;
  U32 x0, y0, x1, y1, xSize, ySize;
  I32 t, t0;
  U32 Cnt, PixelsPerSecond, PixelCnt;

  /* Find an area which is not obstructed by any windows */
  xSize   = LCD_GetXSize();
  ySize   = LCD_GetYSize();
  Cnt     = 0;
  x0      = 0;
  x1      = xSize - 1;
  y0      = 65;
  y1      = ySize - 60 - 1;
  Color   = GUI_GetColor();
  BkColor = GUI_GetBkColor();
  GUI_SetColor(BkColor);

  /* Repeat fill as often as possible in 100 ms */
  t0 = GUI_GetTime();
  do {
    GUI_FillRect(x0, y0, x1, y1);
    GUI_Exec();
    Cnt++;
    t = GUI_GetTime();    
  } while ((t - (t0 + 100)) <= 0);

  /* Compute result */
  t -= t0;
  PixelCnt = (x1 - x0 + 1) * (y1 - y0 + 1) * Cnt;
  PixelsPerSecond = PixelCnt / t * 1000;   
  GUI_SetColor(Color);
  return PixelsPerSecond;
}

/**
  * @brief  Run speed test
  * @param  None
  * @retval Pixels per second
  */
static int Run_SpeedTest(void) {
  int      TimeStart, i;
  U32      PixelsPerSecond;
  unsigned aColorIndex[8];
  int      xSize, ySize, vySize;
  GUI_RECT Rect, ClipRect;
  xSize  = LCD_GetXSize();
  ySize  = LCD_GetYSize();
  vySize = LCD_GetVYSize();
#if GUI_SUPPORT_CURSOR
  GUI_CURSOR_Hide();
#endif
  if (vySize > ySize) {
    ClipRect.x0 = 0;
    ClipRect.y0 = 0;
    ClipRect.x1 = xSize;
    ClipRect.y1 = ySize;
    GUI_SetClipRect(&ClipRect);
  }

  Stop_Test = 0;
  
  for (i = 0; i< 8; i++) {
    aColorIndex[i] = GUI_Color2Index(_aColor[i]);
  }  
  TimeStart = GUI_GetTime();
  for (i = 0; ((GUI_GetTime() - TimeStart) < 5000) &&( Stop_Test == 0); i++) {
    GUI_SetColorIndex(aColorIndex[i&7]);
    
    /* Calculate random positions */
    Rect.x0 = rand() % xSize - xSize / 2;
    Rect.y0 = rand() % ySize - ySize / 2;
    Rect.x1 = Rect.x0 + 20 + rand() % xSize;
    Rect.y1 = Rect.y0 + 20 + rand() % ySize;
    GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, Rect.y1);

    /* Clip rectangle to visible area and add the number of pixels (for speed computation) */
    if (Rect.x1 >= xSize) {
      Rect.x1 = xSize - 1;
    }
    if (Rect.y1 >= ySize) {
      Rect.y1 = ySize - 1;
    }
    if (Rect.x0 < 0 ) {
      Rect.x0 = 0;
    }
    if (Rect.y1 < 0) {
      Rect.y1 = 0;
    }
    GUI_Exec();
    /* Allow short breaks so we do not use all available CPU time ... */
  }
  PixelsPerSecond = _GetPixelsPerSecond();
  GUI_SetClipRect(NULL);
  return PixelsPerSecond;
}

/**
  * @brief  Callback routine of the CPU Window
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
  * @brief  Callback routine of desktop window.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _ClearDesktop(WM_MESSAGE * pMsg) {
  
  switch (pMsg->MsgId) 
  {
  case WM_PAINT:
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    break;
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Callback routine of the Benchmark dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;
  WM_HWIN hGraph;
  int cpu_speed = 0;
  RTC_TimeTypeDef   RTC_Time;
  uint8_t sec, min, hour;
  char temp[50];
  WM_CALLBACK  *_cb;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    hItem = pMsg->hWin;
    FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0);   
    
    /* Initialization of 'CPU' */    
    hItem = TEXT_CreateEx(20, 30, 100, 25, pMsg->hWin, WM_CF_SHOW,0, 0x123,"");
    TEXT_SetFont(hItem, GUI_FONT_13B_1);
    TEXT_SetTextColor(hItem, 0x00804000);
    TEXT_SetText(hItem, "CPU Usage %:");
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BENCH_CPU);
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    TEXT_SetTextColor(hItem, GUI_DARKRED);

    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CPU_GRAPH);  
    FRAMEWIN_SetBarColor(hItem, 0, GUI_DARKGRAY);  
    FRAMEWIN_SetBarColor(hItem, 1, GUI_DARKGRAY);   
 
    hGraph = GRAPH_CreateEx(10, 45, 220, 110, pMsg->hWin, WM_CF_SHOW, 0, GUI_ID_GRAPH0);
    hData = GRAPH_DATA_YT_Create(GUI_LIGHTGREEN, 500, 0, 20);
    GRAPH_SetGridVis(hGraph, 1);
    GRAPH_SetBorder(hGraph, 20, 4, 5, 4); 
    GRAPH_AttachData(hGraph, hData);
    
    hScale = GRAPH_SCALE_Create(20, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 25);
    GRAPH_AttachScale(hGraph, hScale);  
    GRAPH_SCALE_SetTextColor(hScale, GUI_YELLOW);
    GRAPH_SetGridDistX(hGraph, 25);
    GRAPH_SetGridDistY(hGraph, 25);   
    
    WM_CreateWindowAsChild(80, 45, 354, 23, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbCpuWindow , 0); 
    
    break;

  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    switch (NCode) {
    case WM_NOTIFICATION_RELEASED:      /* React only if released */
      switch (Id) {
   
      case ID_BENCH_CPU:
        Stop_Test = 0;
        
        WM_HideWindow(pMsg->hWin);
        
        _cb = WM_GetCallback(WM_HBKWIN);
        WM_SetCallback(WM_HBKWIN, _ClearDesktop);
        cpu_speed = Run_SpeedTest();
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_LIST);
        
        k_GetTime(&RTC_Time);
        sec    =  RTC_Time.Seconds;
        min    =  RTC_Time.Minutes;
        hour   =  RTC_Time.Hours; 
    
        sprintf(temp,"[%02d:%02d:%02d]: %d Pix/s ", hour , min, sec, cpu_speed); 
        LISTBOX_AddString(hItem, temp);

        WM_ShowWindow(pMsg->hWin);
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BENCH_CPU);
        sprintf (temp, "%d  Pixels/s ", cpu_speed); 
        TEXT_SetText(hItem, temp);
        WM_SetCallback(WM_HBKWIN, _cb);
        hItem = WM_GetDialogItem(WM_HBKWIN, ID_BUTTON_BKGND);
        WM_InvalidateWindow(hItem);
        WM_InvalidateWindow(WM_HBKWIN);
        WM_Paint(WM_HBKWIN);
        break;
      }
      break;
    
    case WM_NOTIFICATION_CHILD_DELETED:
      Stop_Test = 1;
      break; 
    }
    break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  Benchmark window startup
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
