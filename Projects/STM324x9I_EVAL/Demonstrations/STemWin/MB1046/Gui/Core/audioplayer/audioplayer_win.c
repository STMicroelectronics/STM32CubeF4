/**
  ******************************************************************************
  * @file    audioplayer_win.c
  * @author  MCD Application Team
  * @brief   Audio player functions
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
#include "audioplayer_app.h"
#include "audioplayer_res.c"
#include "k_modules_res.h"
#include "k_storage.h"
#include "k_rtc.h"    
#include "k_mem.h"    
#include "../Modules/filebrowser/filebrowser_app.h"

/** @addtogroup AUDIO_PLAYER_MODULE
  * @{
  */

/** @defgroup AUDIO_PLAYER
  * @brief audio player routines
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void AudioDirectOpen(char *filename);
/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  audio_player =
{
  8,
  "Audio Player",
  &bmaudioplayer,
  Startup,
  AudioDirectOpen,
};
typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t repeat         : 2;
    uint32_t pause          : 2;    
    uint32_t mute           : 1;
    uint32_t volume         : 8;   
    uint32_t reserved       : 21;
  }b;
}
AudioSettingsTypeDef;
/* Private defines -----------------------------------------------------------*/

#define REPEAT_NONE            0
#define REPEAT_ONCE            1
#define REPEAT_ALL             2

#define ID_PROCESS_TIMER       1
#define ID_PLAYLIST_TIMER      2

#define ID_FRAMEWIN_INFO      (GUI_ID_USER + 0x01)

/* Play List */
#define ID_WAVFILE_LIST       (GUI_ID_USER + 0x10)

/* Button */
#define ID_REPEAT_BUTTON      (GUI_ID_USER + 0x20)
#define ID_CLOSE_BUTTON       (GUI_ID_USER + 0x21)
#define ID_MUTE_BUTTON        (GUI_ID_USER + 0x22)
#define ID_PREVIOUS_BUTTON    (GUI_ID_USER + 0x23)
#define ID_PLAY_BUTTON        (GUI_ID_USER + 0x24)
#define ID_NEXT_BUTTON        (GUI_ID_USER + 0x25)
#define ID_STOP_BUTTON        (GUI_ID_USER + 0x27)
#define ID_PAUSE_BUTTON       (GUI_ID_USER + 0x28)
#define ID_WAV_BUTTON         (GUI_ID_USER + 0x29)
#define ID_PLAY_LIST_BUTTON   (GUI_ID_USER + 0x2A)
#define ID_ADD_BUTTON         (GUI_ID_USER + 0x2B)

/* Volume and song duration sliders */
#define ID_VOLUME_SLIDER      (GUI_ID_USER + 0x30)
#define ID_DURATION_SLIDER    (GUI_ID_USER + 0x31)

/* Audio information */
#define ID_TITLE_CAPTION      (GUI_ID_USER + 0x40)
#define ID_TITLE              (GUI_ID_USER + 0x41)
#define ID_ELAPSED_TIME       (GUI_ID_USER + 0x42)
#define ID_AUTHOR_CAPTION     (GUI_ID_USER + 0x43)
#define ID_AUTHOR             (GUI_ID_USER + 0x44)

/* Play List Information*/
#define ID_SAMPLING_CAPTION   (GUI_ID_USER + 0x52)
#define ID_SAMPLING_VALUE     (GUI_ID_USER + 0x53)
#define ID_TOTAL_TIME         (GUI_ID_USER + 0x54)

/* file chooser */
#define ID_FRAMEWIN_0         (GUI_ID_USER + 0x60)
#define ID_MULTIPAGE          (GUI_ID_USER + 0x61)
#define ID_RADIO_0            (GUI_ID_USER + 0x62)
#define ID_EDIT_71            (GUI_ID_USER + 0x63)
#define ID_EDIT_70            (GUI_ID_USER + 0x64)
#define ID_BUTTON_70          (GUI_ID_USER + 0x65)
#define ID_BUTTON_71          (GUI_ID_USER + 0x66)
#define ID_BUTTON_72          (GUI_ID_USER + 0x67)
#define ID_BUTTON_73          (GUI_ID_USER + 0x68)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect,    "Audio Player", ID_FRAMEWIN_INFO,     0,   0,   480, 246, 0, 0x64, 0 },
  { LISTVIEW_CreateIndirect,  "Listview",     ID_WAVFILE_LIST,      280, 06,  190, 235, 0, 0x0,  0 },  
  { SLIDER_CreateIndirect,    "Slider",       ID_VOLUME_SLIDER,     145, 105, 125, 20,  0, 0x0,  0 },
  { SLIDER_CreateIndirect,    "Slider1",      ID_DURATION_SLIDER,   15,  75,  255, 20,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "Title",        ID_TITLE_CAPTION,     20,  10,  30,  30,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "",             ID_TITLE,             55,  10,  150, 30,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "Duration",     ID_ELAPSED_TIME,      220, 10,  50,  30,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "AUTHOR",       ID_AUTHOR_CAPTION,    20,  25,  50,  30,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "AUTHOR",       ID_AUTHOR,            70,  25,  50,  30,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "samplingRate", ID_SAMPLING_CAPTION,  20,  40,  50,  30,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "Value",        ID_SAMPLING_VALUE,    80,  40,  50,  30,  0, 0x0,  0 },
  { TEXT_CreateIndirect,      "Time",         ID_TOTAL_TIME,        20,  55,  50,  30,  0, 0x0,  0 },
};

static WM_HWIN AUDIOPLAYER_hWin;  
static WM_HTIMER hProcessTimer;
static WM_HTIMER hPlaylistTimer;
static WM_HWIN  hTimerWin;

static WAV_InfoTypedef         WavInfo;
static  FILELIST_FileTypeDef  *pWavList;
static  CHOOSEFILE_INFO       *pFileInfo;
 
static char const            *apDrives[2] = {"0:", "1:"};
static const char            acMask_audio[] = ".audio";
static const char            acMask_dir[] = ".dir";
static uint16_t              file_pos = 0; 
static AudioSettingsTypeDef         PlayerSettings;
static uint8_t              playlist_select = 0;
/* Private functions ---------------------------------------------------------*/

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
    if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
    {
      if(PlayerSettings.b.pause == PAUSE_ACTIVE)
      {
        GUI_DrawBitmap(&bmplay_pressed, 0, 0);
      }
      else if(PlayerSettings.b.pause == PLAY_ACTIVE)
      {
        GUI_DrawBitmap(&bmpause_pressed, 0, 0);
      }
    }
    else  if(AUDIOPLAYER_GetState() == AUDIOPLAYER_STOP)
    {
      if(PlayerSettings.b.pause == PLAY_ACTIVE)
      {
        GUI_DrawBitmap(&bmplay_pressed, 0, 0);
      }
      else if(PlayerSettings.b.pause == PAUSE_ACTIVE)
      {
        GUI_DrawBitmap(&bmpause_pressed, 0, 0);
      }
    }
  }
  else
  {
    if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
    {    
      if(PlayerSettings.b.pause == PAUSE_ACTIVE)
      {    
        GUI_DrawBitmap(&bmplay_not_pressed, 0, 0);
      }
      else if(PlayerSettings.b.pause == PLAY_ACTIVE)
      {
        GUI_DrawBitmap(&bmpause_not_pressed, 0, 0);
      }
    }
    else  if(AUDIOPLAYER_GetState() == AUDIOPLAYER_STOP)
    {
      if(PlayerSettings.b.pause == PLAY_ACTIVE)
      {    
        GUI_DrawBitmap(&bmplay_not_pressed, 0, 0);
      }
      else if(PlayerSettings.b.pause == PAUSE_ACTIVE)
      {
        GUI_DrawBitmap(&bmpause_not_pressed, 0, 0);
      }
    }
  }
}

/**
  * @brief  Paints Stop button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_stop(BUTTON_Handle hObj) {
  int Index;

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  if(Index)
  {
    GUI_DrawBitmap(&bmstop_pressed, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmstop_not_pressed, 0, 0);
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
  * @brief  Paints repeat button
  * @param  repeat_status: repeat button status
  * @retval None
  */
static void _OnPaint_repeat(uint32_t repeat_status) {
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
    
  if(PlayerSettings.b.repeat == REPEAT_NONE)
  {
    GUI_DrawBitmap(&bmrepeat_off, 0, 0);
  }
  else if(PlayerSettings.b.repeat == REPEAT_ONCE)
  {
    GUI_DrawBitmap(&bmrepeat1, 0, 0);
  }
  else if(PlayerSettings.b.repeat == REPEAT_ALL)
  {
    GUI_DrawBitmap(&bmrepeat_all, 0, 0);
  }
}

/**
  * @brief  Paints speaker button
  * @param  speaker_status: speaker button status
  * @retval None
  */
static void _OnPaint_speaker(uint32_t speaker_status) {
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  if(PlayerSettings.b.mute == MUTE_ON)
  {
    GUI_DrawBitmap(&bmspeaker_mute, 0, 0);
  }
  else if(PlayerSettings.b.mute == MUTE_OFF)
  {
    GUI_DrawBitmap(&bmspeaker_not_mute, 0, 0);
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
  * @brief  callback for stop button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_stop(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_stop(pMsg->hWin);
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
  * @brief  callback for repeat button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_repeat(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_repeat(PlayerSettings.b.repeat);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for speaker button
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_speaker(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_speaker(PlayerSettings.b.mute);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  callback for audio process
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbAudioProcess(WM_MESSAGE * pMsg) {

  uint32_t Id;
  
  WM_HWIN hItem;
  char tmp[12] = "00:00";
  long progress;
  uint32_t duration;
  
  switch (pMsg->MsgId) 
  {

  case WM_CREATE:
    hProcessTimer = WM_CreateTimer(pMsg->hWin, ID_PROCESS_TIMER, 1000, 0);   
    break;
    
  case WM_TIMER:
    
    Id = WM_GetTimerId(pMsg->Data.v);
            
    if(Id == ID_PROCESS_TIMER)
    {
      if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
      {
        progress = AUDIOPLAYER_GetProgress();
        
        /*Set elapsed time */
        duration = progress / WavInfo.ByteRate;     
        sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
        
        hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_ELAPSED_TIME);
        TEXT_SetText(hItem, tmp);  
        
        /* Set progress slider position */
        hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_DURATION_SLIDER);
        progress = (long)(progress/(WavInfo.FileSize/100));
        
        SLIDER_SetValue(hItem, progress);
        WM_Update(hItem);
      } 
      
      AUDIOPLAYER_Process();
      WM_RestartTimer(pMsg->Data.v, 1000);
    }
    else if (Id == ID_PLAYLIST_TIMER)
    {
      playlist_select = 0;
    }
    break; 
    
  case WM_DELETE:
    WM_DeleteTimer(hProcessTimer);
    if(hPlaylistTimer)
    {
     WM_DeleteTimer(hPlaylistTimer); 
     hPlaylistTimer = 0;
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Add entire folder to play list.
  * @param  Foldername: pointer to folder name.
  * @retval None
  */
static void _AddEntireFolder(char *Foldername)
{
  FRESULT res;
  uint32_t duration;
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
      
      if (pWavList->ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == 0)
        {
          if(((strstr(fn, ".wav")) || (strstr(fn, ".WAV"))) && (AUDIOPLAYER_hWin != 0))
          {
            strcpy(tmp, Foldername);
            strcat(tmp, "/");
            strcat(tmp, fn);
            if(AUDIOPLAYER_GetFileInfo(tmp, &WavInfo) == 0)
            {
              
              strncpy((char *)pWavList->file[pWavList->ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
              hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_WAVFILE_LIST);
              LISTVIEW_AddRow(hItem, NULL);  
              FILEMGR_GetFileOnly (tmp, fn);
              
              LISTVIEW_SetItemText(hItem, 0, pWavList->ptr, fn);
              
              duration = WavInfo.FileSize / WavInfo.ByteRate; 
              sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
              LISTVIEW_SetItemText(hItem, 1, pWavList->ptr, tmp);
            
              pWavList->ptr++;
            }
          }
        }
      }   
    }
  }
  f_closedir(&dir);
}


/**
  * @brief  Play wav file.
  * @param  filename: pointer to file name.
  * @retval None
  */
static void _PlayFile(char *filename)
{
  int duration;
  static char tmp[FILEMGR_FILE_NAME_SIZE];  
  WM_HWIN hItem;
  
  if(AUDIOPLAYER_GetFileInfo(filename, &WavInfo) == 0)
  {
    /* Title */
    FILEMGR_GetFileOnly (tmp, filename);
    TEXT_SetText(WM_GetDialogItem(AUDIOPLAYER_hWin, ID_TITLE), tmp);  
    
    /* Total Time */
    duration = WavInfo.FileSize / WavInfo.ByteRate;     
    sprintf((char *)tmp , "%02d:%02d", duration/60, duration%60 );
    hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_TOTAL_TIME);
    TEXT_SetText(hItem, tmp); 
    
    /* Author */
    hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_AUTHOR);
    TEXT_SetText(hItem, "Unknown");  
    
    /* Sampling Rate */
    hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_SAMPLING_VALUE);
    sprintf((char *)tmp , "%lu Hz", WavInfo.SampleRate);        
    TEXT_SetText(hItem, tmp);  
    
    /* Open audio file */
    if(AUDIOPLAYER_SelectFile(filename) == 0)
    {
      /* start playing */
      AUDIOPLAYER_Play(WavInfo.SampleRate);
      if(PlayerSettings.b.mute == MUTE_ON)
      {
        AUDIOPLAYER_Mute(MUTE_ON);
      }
    }
  }
  else
  {
    TEXT_SetText(WM_GetDialogItem(AUDIOPLAYER_hWin, ID_TITLE), "Unknown");  
    hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_TOTAL_TIME);
    TEXT_SetText(hItem, "--:--");     
    hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_SAMPLING_VALUE);    
    TEXT_SetText(hItem, "Unsupported Format");  
  }  
}

/**
  * @brief  Notify the end of wav file.
  * @param  None.
  * @retval Audio state.
  */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyEndOfFile(void)
{
  WM_HWIN hItem;
  
  AUDIOPLAYER_Stop();
  
  if(PlayerSettings.b.repeat != REPEAT_NONE)
  {
    if (PlayerSettings.b.repeat == REPEAT_ALL)
    {
      if(file_pos < (pWavList->ptr - 1))
      {
        file_pos++;
      }
      else 
      {        
        file_pos = 0; 
      }
      LISTVIEW_SetSel(WM_GetDialogItem(AUDIOPLAYER_hWin, ID_WAVFILE_LIST), file_pos);      
    }
    
    _PlayFile((char *)pWavList->file[file_pos].name);
  }
  else
  {
    PlayerSettings.b.pause = PLAY_ACTIVE; 
    hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_PLAY_BUTTON);
    WM_InvalidateWindow(hItem);
    WM_Update(hItem);
  }

  return AUDIOPLAYER_ERROR_NONE;
}

/**
  * @brief  callback for progress slider.
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDrawProgressSlider(WM_MESSAGE * pMsg) {
  
  GUI_RECT Rect;
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    
    break;
        
  case WM_PAINT:
    WM_GetInsideRect(&Rect);
    
    GUI_DrawGradientH(Rect.x0, Rect.y0, Rect.x1, Rect.y1, GUI_WHITE, 0x3F0000);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  callback for volume slider
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDrawVolumeSlider(WM_MESSAGE * pMsg) {
  
  GUI_RECT Rect;
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    
    break;
    
    
  case WM_PAINT:
    WM_GetInsideRect(&Rect);
    
    GUI_DrawGradientH(Rect.x0, Rect.y0, Rect.x1, Rect.y1, GUI_WHITE, 0x3F0000);
    break;
    
  default:
    WM_DefaultProc(pMsg);
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
  static uint8_t        prev_sd_status = 0;
  static uint8_t        prev_usb_status = 0;
  
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    prev_sd_status = k_StorageGetStatus(MSD_DISK_UNIT);
    prev_usb_status = k_StorageGetStatus(USB_DISK_UNIT);    
    hStatusTimer = WM_CreateTimer(pMsg->hWin, 0, 500, 0);      
    break;
    
  case WM_TIMER:
    if(prev_sd_status != k_StorageGetStatus(MSD_DISK_UNIT))
    {
      prev_sd_status = k_StorageGetStatus(MSD_DISK_UNIT);
      if(pWavList->file[file_pos].name[0] == '1')
      {
        if(PlayerSettings.b.pause == PAUSE_ACTIVE)
        {  
          PlayerSettings.b.pause = PLAY_ACTIVE;
        } 
        AUDIOPLAYER_Stop();
        hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_ELAPSED_TIME);
        TEXT_SetText(hItem, "00:00");            

        hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_PLAY_BUTTON);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);          
      }
    }
    else if(prev_usb_status != k_StorageGetStatus(USB_DISK_UNIT))
    {
      prev_usb_status = k_StorageGetStatus(USB_DISK_UNIT);
      if(pWavList->file[file_pos].name[0] == '0')
      {
        
        if(PlayerSettings.b.pause == PAUSE_ACTIVE)
        {  
          PlayerSettings.b.pause = PLAY_ACTIVE;
        } 
        AUDIOPLAYER_Stop();
        hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_ELAPSED_TIME);
        TEXT_SetText(hItem, "00:00");            

        hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_PLAY_BUTTON);
        WM_InvalidateWindow(hItem);
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
  * @brief  Callback routine of the dialog
  * @param  pMsg: pointer to data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id, ItemNbr;
  int      result;  
  int duration, volume, index;
  static char tmp[FILEMGR_FILE_NAME_SIZE];  
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
      
    pWavList = (FILELIST_FileTypeDef *)k_malloc(sizeof(FILELIST_FileTypeDef));
    pFileInfo = (CHOOSEFILE_INFO *)k_malloc(sizeof(CHOOSEFILE_INFO));
    pWavList->ptr = 0;
      
    /* Initialization of 'Listview' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WAVFILE_LIST);
    LISTVIEW_AddColumn(hItem, 132, "Track", GUI_TA_VCENTER | GUI_TA_LEFT);
    LISTVIEW_AddColumn(hItem, 55, "Duration", GUI_TA_VCENTER | GUI_TA_RIGHT);
    LISTVIEW_SetGridVis(hItem, 0);
    LISTVIEW_SetAutoScrollV(hItem, 1);
    LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_UNSEL, GUI_BLACK);
    LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_UNSEL, GUI_CYAN);
    
    /* Title Initialization in play list */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE_CAPTION);
    TEXT_SetText(hItem, "TITLE:");
    TEXT_SetTextColor(hItem, GUI_CYAN);
    
    /* Title Initialization in play list */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE);
    TEXT_SetTextColor(hItem, GUI_CYAN);    
    
    /* Duration */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_ELAPSED_TIME);
    TEXT_SetText(hItem, "00:00");
    TEXT_SetFont(hItem, GUI_FONT_20B_1);
    TEXT_SetTextColor(hItem, GUI_LIGHTGRAY);
    
    /* Author initialization */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_AUTHOR_CAPTION);
    TEXT_SetText(hItem, "AUTHOR:");
    TEXT_SetTextColor(hItem, GUI_CYAN);
    
    /* Author */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_AUTHOR);
    TEXT_SetText(hItem, "");
    TEXT_SetTextColor(hItem, GUI_CYAN);
    
    /* Sampling Rate */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SAMPLING_CAPTION);
    TEXT_SetText(hItem, "SAMPLING:");
    TEXT_SetTextColor(hItem, GUI_CYAN);
    
    /* Sampling Rate Value */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SAMPLING_VALUE);
    TEXT_SetText(hItem, "");
    TEXT_SetTextColor(hItem, GUI_CYAN);
    
    /* Duration */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TOTAL_TIME);
    TEXT_SetText(hItem, "00:00");
    TEXT_SetTextColor(hItem, GUI_CYAN);
    
    /* Initialization of 'WAV' Button */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_WAV_BUTTON);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    
    /* Initialization of 'Play List' Button */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_LIST_BUTTON);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    
    /* Initialization of 'Add' Button */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_ADD_BUTTON);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    
    volume = AUDIOPLAYER_GetVolume();
    hItem = WM_GetDialogItem(pMsg->hWin, ID_VOLUME_SLIDER);
    SLIDER_SetValue(hItem, volume);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_DURATION_SLIDER);
    SLIDER_SetNumTicks(hItem, 25);
    
    PlayerSettings.d32 = k_BkupRestoreParameter(CALIBRATION_AUDIOPLAYER_SETTING_BKP);
    PlayerSettings.b.mute = MUTE_OFF;  
    PlayerSettings.b.pause = PLAY_ACTIVE;  
      
    hItem = BUTTON_CreateEx(25,  100, 30,  30, pMsg->hWin, WM_CF_SHOW, 0, ID_REPEAT_BUTTON);
    WM_SetCallback(hItem, _cbButton_repeat);
    
    hItem = BUTTON_CreateEx(100,  95, 40,  40, pMsg->hWin, WM_CF_SHOW, 0, ID_MUTE_BUTTON);
    WM_SetCallback(hItem, _cbButton_speaker);    
    
    hItem = BUTTON_CreateEx(22,  147, 35,  35, pMsg->hWin, WM_CF_SHOW, 0, ID_STOP_BUTTON);
    WM_SetCallback(hItem, _cbButton_stop);    
       
    hItem = BUTTON_CreateEx(89, 147, 35,  35, pMsg->hWin, WM_CF_SHOW, 0, ID_PREVIOUS_BUTTON);
    WM_SetCallback(hItem, _cbButton_previous);   
    
    hItem = BUTTON_CreateEx(148, 140, 50,  50, pMsg->hWin, WM_CF_SHOW, 0, ID_PLAY_BUTTON);
    WM_SetCallback(hItem, _cbButton_play);
    
    hItem = BUTTON_CreateEx(222, 147, 35,  35, pMsg->hWin, WM_CF_SHOW, 0, ID_NEXT_BUTTON);
    WM_SetCallback(hItem, _cbButton_next);
    
    hItem = WM_GetClientWindow(pMsg->hWin);
    hItem = BUTTON_CreateEx(20, 205, 50,  30, pMsg->hWin, WM_CF_SHOW, 0, ID_WAV_BUTTON);
    WM_SetCallback(hItem, _cbButton_open); 
    
    hItem = BUTTON_CreateEx(80, 205, 50,  30, pMsg->hWin, WM_CF_SHOW, 0, ID_ADD_BUTTON);
    WM_SetCallback(hItem, _cbButton_add);      
       
    hItem = BUTTON_CreateEx(235,  205, 30,  30, pMsg->hWin, WM_CF_SHOW, 0, ID_CLOSE_BUTTON);
    WM_SetCallback(hItem, _cbButton_close);
    
    hTimerWin = WM_CreateWindowAsChild(0,
                           100,
                           10,
                           10,
                           pMsg->hWin, 
                           WM_CF_SHOW | WM_CF_HASTRANS,
                           _cbAudioProcess, 
                           0);
    
    hItem = WM_CreateWindowAsChild(15,
                                   75,
                                   255,
                                   20,
                                   pMsg->hWin, 
                                   WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_BGND,
                                   _cbDrawProgressSlider, 
                                   0);
    
    hItem = WM_CreateWindowAsChild(145,
                                   
                                   105,
                                   125,
                                   20,
                                   pMsg->hWin, 
                                   WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_BGND,
                                   _cbDrawVolumeSlider, 
                                   0);    
    
    WM_CreateWindowAsChild(479, 250, 1, 1, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbMediaConnection , 0); 
    
    break;
    
  case WM_PAINT:
    
    GUI_SetBkColor(GUI_BLACK);
    GUI_ClearRect(15, 6, 270, 70); 
    DrawRect3D(15, 135, 255, 60);
    DrawRect3D(15, 200, 255, 40);
    break;
    
  case WM_DELETE:
    AUDIOPLAYER_DeInit();
    
    k_free(pWavList);
    k_free(pFileInfo);
    
    pWavList->ptr = 0;
    PlayerSettings.b.mute = MUTE_OFF;       
    PlayerSettings.b.pause = PLAY_ACTIVE;        
    k_BkupSaveParameter(CALIBRATION_AUDIOPLAYER_SETTING_BKP, PlayerSettings.d32);
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;

    switch(Id) {
      
    /* Notification sent by "Button_Close" */  
    case ID_CLOSE_BUTTON: 
      switch (NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;
      
    /* Notifications sent by 'Repeat' Button */
    case ID_REPEAT_BUTTON:
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_REPEAT_BUTTON);   
        
        if(PlayerSettings.b.repeat == REPEAT_NONE)
        {
          PlayerSettings.b.repeat = REPEAT_ONCE;
        }
        else if(PlayerSettings.b.repeat == REPEAT_ONCE)
        {
          PlayerSettings.b.repeat = REPEAT_ALL;
        }
        else if(PlayerSettings.b.repeat == REPEAT_ALL)
        {
          PlayerSettings.b.repeat = REPEAT_NONE;
        }
      }
      break;
      
    /* Notifications sent by 'Mute' Button */
    case ID_MUTE_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_MUTE_BUTTON);         
        if(PlayerSettings.b.mute == MUTE_OFF)
        {          
          AUDIOPLAYER_Mute(MUTE_ON);
          PlayerSettings.b.mute = MUTE_ON;
        }
        else
        {         
          AUDIOPLAYER_Mute(MUTE_OFF);
          PlayerSettings.b.mute = MUTE_OFF;
        }
      }
      break;
      
      
    /* Notifications sent by 'Volume' Slider */
    case ID_VOLUME_SLIDER: 
      if(NCode == WM_NOTIFICATION_VALUE_CHANGED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_VOLUME_SLIDER);
        AUDIOPLAYER_SetVolume(SLIDER_GetValue(hItem));
        if(PlayerSettings.b.mute == MUTE_ON)
        {          
          AUDIOPLAYER_Mute(MUTE_OFF);
          PlayerSettings.b.mute = MUTE_OFF;
        }
      }
      break;
      
      
    /* Notifications sent by 'progress' Slider */
    case ID_DURATION_SLIDER: 
      if(NCode == WM_NOTIFICATION_CLICKED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_DURATION_SLIDER);
        AUDIOPLAYER_SetPosition(SLIDER_GetValue(hItem));
        
        if(PlayerSettings.b.mute == MUTE_ON)
        {
          AUDIOPLAYER_Mute(MUTE_ON);
        }
      }
      break;
      
      
    /* Notifications sent by 'ListView' Slider */
    case ID_WAVFILE_LIST: 
      if(NCode == WM_NOTIFICATION_CLICKED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_WAVFILE_LIST);
        index = LISTVIEW_GetSel(hItem);
        
        if(index < pWavList->ptr)
        {
          if(playlist_select == 0)
          {
            hPlaylistTimer = WM_CreateTimer(hTimerWin, ID_PLAYLIST_TIMER, 500, 0);           
            playlist_select = (index + 1);
          }
          
          else if(playlist_select == (index + 1))
          {
            WM_DeleteTimer(hPlaylistTimer); 
            hPlaylistTimer = 0;          
            playlist_select = 0;
            
            if(index < pWavList->ptr)
            {
                
              if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
              {             
                AUDIOPLAYER_Stop();
              }
              
              PlayerSettings.b.pause = PLAY_ACTIVE;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem);
              
              file_pos = index;
              _PlayFile((char *)pWavList->file[index].name); 
            }
          }
        }
      }
      break;
      
    /* Notifications sent by 'Play' Button */
    case ID_PLAY_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        
        if(AUDIOPLAYER_GetState() == AUDIOPLAYER_STOP)
        {
          if(pWavList->ptr > 0)
          {
            _PlayFile((char *)pWavList->file[file_pos].name);
            LISTVIEW_SetSel(WM_GetDialogItem(AUDIOPLAYER_hWin, ID_WAVFILE_LIST), file_pos);
          }
          else
          {
            pFileInfo->pfGetData = k_GetData;
            pFileInfo->pMask = acMask_audio;     
            hItem = CHOOSEFILE_Create(pMsg->hWin,  47, 10, 385, 215, apDrives, GUI_COUNTOF(apDrives), 0, "Open an audio file", 0, pFileInfo); 
            WM_MakeModal(hItem);
            result = GUI_ExecCreatedDialog(hItem);
            if (result == 0) 
            { 
              if((strstr(pFileInfo->pRoot, ".wav")) || (strstr(pFileInfo->pRoot, ".WAV")))
              {         
                if(AUDIOPLAYER_GetFileInfo(pFileInfo->pRoot, &WavInfo) == 0)
                {
                  if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
                  {
                    /* Stop current audio sample */
                    AUDIOPLAYER_Stop();   
                  }
                  
                  pWavList->ptr = 0;
                  file_pos = 0;
                  
                  strcpy((char *)pWavList->file[pWavList->ptr].name, pFileInfo->pRoot);
                  FILEMGR_GetFileOnly (tmp, pFileInfo->pRoot);
                  hItem = WM_GetDialogItem(pMsg->hWin, ID_WAVFILE_LIST);
                  
                  /* Update Play list */
                  strcpy((char *)pWavList->file[pWavList->ptr].name, pFileInfo->pRoot);
                  
                  ItemNbr = LISTVIEW_GetNumRows(hItem);
                  while(ItemNbr--)
                  {
                    LISTVIEW_DeleteRow(hItem, ItemNbr);
                  }
                  
                  LISTVIEW_AddRow(hItem, NULL);         
                  LISTVIEW_SetItemText(hItem, 0, pWavList->ptr, tmp);
                  duration = WavInfo.FileSize / WavInfo.ByteRate; 
                  sprintf((char *)tmp , "%02d:%02d", duration/60, duration%60 );
                  LISTVIEW_SetItemText(hItem, 1, pWavList->ptr, tmp);
                  pWavList->ptr++;  
                  
                  LISTVIEW_SetSel(hItem, 0);
                  _PlayFile(pFileInfo->pRoot);              
                }
              }
            }
          }
        }
        else if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
        {
          PlayerSettings.b.pause = (PlayerSettings.b.pause == PLAY_ACTIVE ? PAUSE_ACTIVE : PLAY_ACTIVE);
          
          if(PlayerSettings.b.pause == PAUSE_ACTIVE)
          {            
            AUDIOPLAYER_Pause();
          }
          else if(PlayerSettings.b.pause == PLAY_ACTIVE)
          {            
            AUDIOPLAYER_Resume();
          }
        }
      }
      break;
      
      
    /* Notifications sent by 'STOP' Button */
    case ID_STOP_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
        {
          if(PlayerSettings.b.pause == PAUSE_ACTIVE)
          {  
            PlayerSettings.b.pause = PLAY_ACTIVE;
          } 
          AUDIOPLAYER_Stop();
          hItem = WM_GetDialogItem(pMsg->hWin, ID_ELAPSED_TIME);
          TEXT_SetText(hItem, "00:00");            
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem);          
        }
      }
      break;
      
      
    /* Notifications sent by 'Next' Button */
    case ID_NEXT_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        if( pWavList->ptr > 0)
        {        
          if(file_pos < (pWavList->ptr - 1))
          {
            file_pos++; 
          }
          else if(PlayerSettings.b.repeat == REPEAT_ALL)
          {
            file_pos = 0; 
          }
          LISTVIEW_SetSel(WM_GetDialogItem(AUDIOPLAYER_hWin, ID_WAVFILE_LIST), file_pos); 
          
          if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
          {    
            if(PlayerSettings.b.pause == PAUSE_ACTIVE)
            {  
              PlayerSettings.b.pause = PLAY_ACTIVE;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem);
            }
            
            AUDIOPLAYER_Stop();
            _PlayFile((char *)pWavList->file[file_pos].name); 
          }
        }
      }
      break;
      
      /* Notifications sent by 'Previous' Button */
    case ID_PREVIOUS_BUTTON: 
      if(NCode == WM_NOTIFICATION_RELEASED)
      {
        if( pWavList->ptr > 0)
        {
          if(file_pos > 0)
          {   
            file_pos--;           
          }
          else if(PlayerSettings.b.repeat == REPEAT_ALL)
          {
            file_pos = (pWavList->ptr - 1); 
          }          
          LISTVIEW_SetSel(WM_GetDialogItem(AUDIOPLAYER_hWin, ID_WAVFILE_LIST), file_pos);          
          
          if(AUDIOPLAYER_GetState() == AUDIOPLAYER_PLAY)
          {  
            if(PlayerSettings.b.pause == PAUSE_ACTIVE)
            {  
              PlayerSettings.b.pause = PLAY_ACTIVE;
              hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem);
            }
            
            AUDIOPLAYER_Stop();
            _PlayFile((char *)pWavList->file[file_pos].name);              
          }
        }
      }
      break;    
      
    /* Notifications sent by 'Open' Button */
    case ID_WAV_BUTTON: 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        
        break;
      case WM_NOTIFICATION_RELEASED:
        
        pFileInfo->pfGetData = k_GetData;
        pFileInfo->pMask = acMask_dir;     
        hItem = CHOOSEFILE_Create(pMsg->hWin,  47, 10, 385, 215, apDrives, GUI_COUNTOF(apDrives), 0, "Add a folder", 0, pFileInfo);        
        WM_MakeModal(hItem);
        result = GUI_ExecCreatedDialog(hItem);
        if (result == 0) 
        { 
          _AddEntireFolder(pFileInfo->pRoot);
        }
        break;
      }
      break;
            
    /* Notifications sent by 'Add' Button */
    case ID_ADD_BUTTON: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        
        pFileInfo->pfGetData = k_GetData;
        pFileInfo->pMask = acMask_audio;     
        hItem = CHOOSEFILE_Create(pMsg->hWin,  47, 10, 385, 215, apDrives, GUI_COUNTOF(apDrives), 0, "Add to playlist", 0, pFileInfo);
        WM_MakeModal(hItem);
        result = GUI_ExecCreatedDialog(hItem);
        if (result == 0) 
        { 
          if((strstr(pFileInfo->pRoot, ".wav")) || (strstr(pFileInfo->pRoot, ".WAV")))
          {
            if(pWavList->ptr < FILEMGR_LIST_DEPDTH)
            {
              strcpy((char *)pWavList->file[pWavList->ptr].name, pFileInfo->pRoot);
              FILEMGR_GetFileOnly (tmp, pFileInfo->pRoot);
              hItem = WM_GetDialogItem(pMsg->hWin, ID_WAVFILE_LIST);
              
              if(AUDIOPLAYER_GetFileInfo(pFileInfo->pRoot, &WavInfo) == 0)
              {
                LISTVIEW_AddRow(hItem, NULL);         
                LISTVIEW_SetItemText(hItem, 0, pWavList->ptr, tmp);
                duration = WavInfo.FileSize / WavInfo.ByteRate; 
                sprintf((char *)tmp , "%02d:%02d", duration/60, duration%60 );
                LISTVIEW_SetItemText(hItem, 1, pWavList->ptr, tmp);
                pWavList->ptr++;      
              }
            }
          }
        }  
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
  * @brief  Audio player window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  AUDIOPLAYER_Init();  
  AUDIOPLAYER_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, xpos, ypos);
}

/**
  * @brief  Audio direct open
  * @param  filename: audio file name
  * @retval None
  */
static void AudioDirectOpen(char *filename)
{
  WM_HWIN hItem;  
  static char tmp[FILEMGR_FILE_NAME_SIZE];    
  pWavList->ptr = 0;
  uint32_t duration;
  
  AUDIOPLAYER_Init();
  AUDIOPLAYER_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_GetDesktopWindowEx(1), 0, 26);
  
  if(AUDIOPLAYER_hWin != 0)
  {
    if(AUDIOPLAYER_GetFileInfo(filename, &WavInfo) == 0)
    {
      /* Update Play list */
      pWavList->ptr = 0;
      file_pos = 0;

      FILEMGR_GetFileOnly (tmp, filename);
      hItem = WM_GetDialogItem(AUDIOPLAYER_hWin, ID_WAVFILE_LIST);
      
      strcpy((char *)pWavList->file[pWavList->ptr].name, filename);
           
      LISTVIEW_AddRow(hItem, NULL);         
      LISTVIEW_SetItemText(hItem, 0, pWavList->ptr, tmp);
      duration = WavInfo.FileSize / WavInfo.ByteRate; 
      sprintf((char *)tmp , "%02lu:%02lu", duration/60, duration%60 );
      LISTVIEW_SetItemText(hItem, 1, pWavList->ptr, tmp);
      pWavList->ptr++;  
      
      LISTVIEW_SetSel(hItem, 0);
      _PlayFile(filename);              
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
