/**
  ******************************************************************************
  * @file    audio_recorder_win.c
  * @author  MCD Application Team
  * @brief   audio_recorde functions
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

#include "audio_recorder_res.c"
#include "../Common/audio_if.h"
#include "audio_recorder_app.h"


/** @addtogroup AUDIO_RECORDER_MODULE
  * @{
  */

/** @defgroup AUDIO_RECORDER
  * @brief audio recorder routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);

extern FATFS mSDDISK_FatFs;         /* File system object for MSD disk logical drive */
extern char mSDDISK_Drive[4];       /* MSD Host logical drive number */

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  audio_recorder_board =
{  
  3,
  (uint8_t*)"",
  &bmrecorder5,
  Startup,
}
;

/* Private defines -----------------------------------------------------------*/
#define ID_MAIN_FRAME                   (GUI_ID_USER + 0x00)
#define ID_BUTTON_EXIT                  (GUI_ID_USER + 0x02)
                                        
#define ID_IMAGE_0                      (GUI_ID_USER + 0x03)
#define ID_RECORD_CANCEL                (GUI_ID_USER + 0x04)
#define ID_RECORD_REC_CANCEL_PLAY       (GUI_ID_USER + 0x05)
#define ID_RECORD_SAVE                  (GUI_ID_USER + 0x06)
#define ID_ELAPSED_TIME                 (GUI_ID_USER + 0x07)

#define ID_PROCESS_TIMER                (GUI_ID_USER + 0x0E)

#define ID_TITLE                        (GUI_ID_USER + 0x0F)

#define RECORDER_MODE_REC_DISABLED            0x00  
#define RECORDER_MODE_REC_IDLE                0x01  
#define RECORDER_MODE_RECORDING               0x02 
#define RECORDER_MODE_PLAY_IDLE               0x03
#define RECORDER_MODE_PLAYING                 0x04
#define RECORDER_MODE_REC_PAUSE               0x05

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "", ID_MAIN_FRAME, 0, 0, 240, 240, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "00:00", ID_ELAPSED_TIME, 85, 180, 80, 32, 0, 0x0, 0 },
  { TEXT_CreateIndirect, ""     , ID_TITLE,        20, 10,  170,  25, 0, 0x0, 0 },
};

static WM_HWIN               hMainWin;
extern K_ModuleItem_Typedef  audio_player_board;
static uint8_t               RecorderMode; 
static uint8_t               play = 0;
static char                  FileName[256]; 
RNG_HandleTypeDef            hRNG;
uint32_t duration = 0;
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Paints exit button
  * @param  hObj: button handle
  * @retval None
  */
static void _BuildFileName(void) {

  hRNG.Instance = RNG;
  __HAL_RCC_RNG_CLK_ENABLE();
  HAL_RNG_Init(&hRNG);
  sprintf(FileName, "record-%04lu.wav", HAL_RNG_GetRandomNumber(&hRNG) % 10000);
}

/**
  * @brief  Paints record button
  * @param  hObj: button handle
  * @retval None
*/
static void _OnPaint_exit(BUTTON_Handle hObj) 
{
    GUI_DrawBitmap(&bmexit, 20, 0);
} 

/**
  * @brief  Paints cancel button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_cancel(BUTTON_Handle hObj) 
{
  if(RecorderMode == RECORDER_MODE_RECORDING)
  {
    GUI_DrawBitmap(&bmrecord_cancel, 10, 10); 
  }
  else
  {
    GUI_DrawBitmap(&bmcancel_des, 10, 10); 
  }
}

/**
  * @brief  Paints save button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_save(BUTTON_Handle hObj) 
{
  if(RecorderMode == RECORDER_MODE_RECORDING)
  {
    GUI_DrawBitmap(&bmsave, 10, 10); 
  }
  else
  {
    GUI_DrawBitmap(&bmsave_des, 10, 10); 
  }
}

/**
  * @brief  Paints record button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_record(BUTTON_Handle hObj) {
  
  if(RecorderMode == RECORDER_MODE_REC_DISABLED)   
  {
    GUI_DrawBitmap(&bmrecord_des, 0, 0);  
  }
  else if(RecorderMode == RECORDER_MODE_PLAYING)   
  {
    GUI_DrawBitmap(&bmplay_des, 0, 0); 
  }
  else if(RecorderMode == RECORDER_MODE_REC_IDLE)
  {
    GUI_DrawBitmap(&bmrecord, 0, 0);
  }
  else if(RecorderMode == RECORDER_MODE_PLAY_IDLE)
  {
    GUI_DrawBitmap(&bmrecord_play, 0, 0); 
  }
  else if(RecorderMode == RECORDER_MODE_RECORDING)
  {
    GUI_DrawBitmap(&bmrecord_des, 0, 0); 
  }
}

/**
  * @brief  callback for record button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_record(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_record(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for exit button
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
  * @brief  callback for cancel button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_cancel(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_cancel(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for save button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_save(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_save(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}
/**
  * @brief  Callback function of the media connection status
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbMediaConnection(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  static WM_HTIMER      hStatusTimer;  
  static uint8_t        prev_msd_status = 0;
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    prev_msd_status = k_StorageGetStatus(MSD_DISK_UNIT);    
    hStatusTimer = WM_CreateTimer(pMsg->hWin, 0, 500, 0); 
    
    break;
    
  case WM_TIMER:
    if(prev_msd_status != k_StorageGetStatus(MSD_DISK_UNIT))
    {
      prev_msd_status = k_StorageGetStatus(MSD_DISK_UNIT);
      if(prev_msd_status == 0)
      {
        if( RecorderMode == RECORDER_MODE_RECORDING)
        {
          AUDIO_RECORDER_StopRec();
        }
        
        else if( RecorderMode == RECORDER_MODE_PLAYING)
        {
          AUDIO_RECORDER_StopPlayer();
        }
                
        RecorderMode = RECORDER_MODE_REC_DISABLED;
        
        hItem = WM_GetDialogItem(hMainWin, ID_RECORD_CANCEL);
        WM_DisableWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(hMainWin, ID_RECORD_REC_CANCEL_PLAY);
        WM_DisableWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(hMainWin, ID_RECORD_SAVE);
        WM_DisableWindow(hItem); 
        WM_Update(hItem);
      }
      else
      {
        RecorderMode = RECORDER_MODE_REC_IDLE;
        
        hItem = WM_GetDialogItem(hMainWin, ID_RECORD_CANCEL);
        WM_EnableWindow(hItem);
        WM_Update(hItem);        
        
        hItem = WM_GetDialogItem(hMainWin, ID_RECORD_REC_CANCEL_PLAY);
        WM_EnableWindow(hItem);
        WM_Update(hItem);
        
        hItem = WM_GetDialogItem(hMainWin, ID_RECORD_SAVE);
        WM_EnableWindow(hItem); 
        WM_Update(hItem);        
      }
    }
    WM_RestartTimer(pMsg->Data.v, 500);
    break;
    
  case WM_DELETE:
    if(hStatusTimer != 0)
    {
      WM_DeleteTimer(hStatusTimer);
      hStatusTimer = 0;
    }
    break;   
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  callback for audio process
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbAudioProcess(WM_MESSAGE * pMsg) {

  uint32_t Id;
  static WM_HTIMER hProcessTimer;
  WM_HWIN hItem;
  char tmp[] = "00:00/00:00";  
  
  switch (pMsg->MsgId) 
  {

  case WM_CREATE:
    hProcessTimer = WM_CreateTimer(pMsg->hWin, ID_PROCESS_TIMER, 1000, 0);

    break;
    
  case WM_TIMER:
    
    Id = WM_GetTimerId(pMsg->Data.v);
   
    if(Id == ID_PROCESS_TIMER)
    {
      if( RecorderMode == RECORDER_MODE_RECORDING)
      {
        /*Set elapsed time */
        duration = AUDIO_RECORDER_GetElapsedTime();     
        sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
        
        hItem = WM_GetDialogItem(hMainWin, ID_ELAPSED_TIME);
        TEXT_SetText(hItem, tmp);  
        
      } 
      if( RecorderMode == RECORDER_MODE_PLAYING)
      {
        /*Set elapsed time */
        duration = AUDIO_RECORDER_GetTotalTime() - AUDIO_RECORDER_GetPlayedTime();
        sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60);
        hItem = WM_GetDialogItem(hMainWin, ID_ELAPSED_TIME);
        TEXT_SetText(hItem, tmp);  
        play = 1;
        
      }     
      if((play == 1) && (duration <= 1))
      {
          RecorderMode = RECORDER_MODE_REC_IDLE;
          hItem = WM_GetDialogItem(hMainWin, ID_RECORD_REC_CANCEL_PLAY);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
          
          hItem = WM_GetDialogItem(hMainWin, ID_TITLE);
          TEXT_SetText(hItem, "                    ");    
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);
          
          play = 0;
      }
      
      WM_RestartTimer(pMsg->Data.v, 1000);
    }
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hProcessTimer);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Notify State Change
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
void _cbNotifyStateChange (void) 
{
  WM_HWIN hItem;  
  if(AUDIO_RECORDER_GetState() == AUDIO_RECORDER_SUSPENDED)
  {
    if(RecorderMode == RECORDER_MODE_PLAYING)
    {
      RecorderMode = RECORDER_MODE_PLAY_IDLE;
      hItem = WM_GetDialogItem(hMainWin, ID_RECORD_REC_CANCEL_PLAY);
      WM_InvalidateWindow(hItem);
      WM_Update(hItem);        
    }
    
    if(RecorderMode == RECORDER_MODE_RECORDING)
    {
      RecorderMode = RECORDER_MODE_REC_IDLE;
    }
        
    if(hMainWin != 0)
    {
      hItem = WM_GetDialogItem(hMainWin, ID_ELAPSED_TIME);        
      TEXT_SetText(hItem, "00:00"); 
      WM_Update(hItem);
      
    }
  } 
}

/**
  * @brief  main callback for Audio Recorder
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    
    hItem = BUTTON_CreateEx(80, 90, 80, 80, pMsg->hWin, WM_CF_SHOW, 0, ID_RECORD_REC_CANCEL_PLAY);
    WM_SetCallback(hItem, _cbButton_record); 

    hItem = BUTTON_CreateEx(186, 3, 50, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT);
    WM_SetCallback(hItem, _cbButton_exit); 
    
    hItem = BUTTON_CreateEx(160, 100, 60, 60, pMsg->hWin, WM_CF_SHOW, 0, ID_RECORD_SAVE);
    WM_SetCallback(hItem, _cbButton_save);     
    
    hItem = BUTTON_CreateEx(15, 100, 50, 60, pMsg->hWin, WM_CF_SHOW, 0, ID_RECORD_CANCEL);
    WM_SetCallback(hItem, _cbButton_cancel);      
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE);
    TEXT_SetFont     (hItem, &GUI_Font20B_1);
    TEXT_SetTextColor(hItem, GUI_WHITE);    
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_ELAPSED_TIME);
    TEXT_SetFont(hItem, &GUI_FontLubalGraph32);
    TEXT_SetTextColor(hItem, GUI_WHITE);
    
      RecorderMode = RECORDER_MODE_REC_IDLE;
      WM_CreateWindowAsChild(0,
                             100,
                             10,
                             10,
                             pMsg->hWin, 
                             WM_CF_SHOW | WM_CF_HASTRANS,
                             _cbAudioProcess, 
                             0);
      
      WM_CreateWindowAsChild(239, 239, 1, 1, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbMediaConnection , 0);     
      break;
      
  case WM_PAINT:
    
    /* Draw audio recorder background */
    GUI_SetBkColor(0xFF000000);
    GUI_Clear();  
    
    GUI_SetColor(0x00D0FE);
    GUI_FillRect(0, 0, 240, 40);
    
    GUI_SetColor(0x2223D3);
    GUI_AA_FillCircle(120, 140, 100);
    
    break;    
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    
    switch (Id) {
      
    case ID_BUTTON_EXIT:      
      
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        if( RecorderMode == RECORDER_MODE_RECORDING)
        {
          AUDIO_RECORDER_StopRec();
        }
        
        else if( RecorderMode == RECORDER_MODE_PLAYING)
        {
          AUDIO_RECORDER_StopPlayer();
        }

        AUDIO_RECORDER_DeInit();
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;        
      
    case ID_RECORD_SAVE: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
                
        if(RecorderMode == RECORDER_MODE_RECORDING)
        {
          AUDIO_RECORDER_StopRec();
          RecorderMode = RECORDER_MODE_PLAY_IDLE;
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE);
          TEXT_SetText(hItem, "                    ");
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);   
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_REC_CANCEL_PLAY);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem); 
        }        
        break;
      }
      break;        
      
    case ID_RECORD_CANCEL: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        
        if(RecorderMode == RECORDER_MODE_RECORDING)
        {
          AUDIO_RECORDER_StopRec();
          RecorderMode = RECORDER_MODE_REC_IDLE;
          AUDIO_RECORDER_RemoveAudioFile(FileName);       
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE);
          TEXT_SetText(hItem, "                    ");
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);    
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_REC_CANCEL_PLAY);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);     
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_CANCEL);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);     
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_SAVE);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);    
          break;
        }
      }
      break;
      

    case ID_RECORD_REC_CANCEL_PLAY: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        
        if(( RecorderMode == RECORDER_MODE_REC_IDLE) || ( RecorderMode == RECORDER_MODE_PLAY_IDLE))
        {
        /* Start Record */
        if( RecorderMode == RECORDER_MODE_REC_IDLE) 
        {
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE);
          TEXT_SetText(hItem, "Recording ...");
          WM_InvalidateWindow(hItem);
          WM_Update(hItem); 
          
          _BuildFileName();   

          AUDIO_RECORDER_SelectFile(FileName, FA_CREATE_ALWAYS | FA_WRITE);
          AUDIO_RECORDER_StartRec(DEFAULT_AUDIO_IN_FREQ);
          RecorderMode = RECORDER_MODE_RECORDING;           
        }
        
        /* Start Play */
        else if( RecorderMode == RECORDER_MODE_PLAY_IDLE) 
        {
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE);
          TEXT_SetText(hItem, "Playing ...");
          WM_InvalidateWindow(hItem);
          WM_Update(hItem); 
          
          RecorderMode = RECORDER_MODE_PLAYING;
          AUDIO_RECORDER_SelectFile(FileName, FA_OPEN_EXISTING | FA_READ);          
          AUDIO_RECORDER_Play(DEFAULT_AUDIO_IN_FREQ);           
        }
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_REC_CANCEL_PLAY);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);     
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_CANCEL);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);     
        
        hItem = WM_GetDialogItem(pMsg->hWin, ID_RECORD_SAVE);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);              
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
  * @brief  Audio recorder window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  AUDIO_RECORDER_Init(50);
  hMainWin = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, xpos, ypos);
}

/**
  * @}
  */

/**
  * @}
  */
