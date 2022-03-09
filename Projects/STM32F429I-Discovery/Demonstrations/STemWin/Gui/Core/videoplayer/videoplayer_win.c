/**
  ******************************************************************************
  * @file    videoplayer_win.c
  * @author  MCD Application Team
  * @brief   Video player functions
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
#include "videoplayer_res.c"
#include "k_modules_res.h"
#include "k_storage.h"
#include "k_rtc.h"    
#include "k_mem.h"    
#include "../Modules/filebrowser/filebrowser_app.h"

/** @addtogroup VIDEO_PLAYER_MODULE
  * @{
  */

/** @defgroup VIDEO_PLAYER
  * @brief video player routines
  * @{
  */

/* External variables --------------------------------------------------------*/
extern int module_active;

/* Private function prototypes -----------------------------------------------*/
typedef union
{
  uint32_t d32;
  struct
  {
    uint32_t repeat         : 2;
    uint32_t pause          : 2;    
    uint32_t reserved       : 28;
  }b;
}
VideoSettingsTypeDef;


static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void VideoDirectOpen(char *filename);
extern int GUI_MOVIE_ShowScaled   (GUI_MOVIE_HANDLE hMovie, int xPos, int yPos, int num, int denom, int DoLoop);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  video_player =
{
  6,
  "Video Player",
  &bmvideoplayer,
  Startup,
  VideoDirectOpen,
}
;

/* Private defines -----------------------------------------------------------*/
#define ID_FRAMEWIN_INFO       (GUI_ID_USER + 0x01)

/* Play List */
#define ID_VIDEO_LIST          (GUI_ID_USER + 0x10)

#define ID_VIDEO_FILE       (GUI_ID_USER + 0x07)
#define ID_VIDEO_FPS        (GUI_ID_USER + 0x08)
#define ID_VIDEO_PROGRESS   (GUI_ID_USER + 0x09)
#define ID_VIDEO_DURATION   (GUI_ID_USER + 0x0A)
#define ID_VIDEO_EXIT       (GUI_ID_USER + 0x0B)

/* Buttons */
#define ID_REPEAT_BUTTON       (GUI_ID_USER + 0x20)
#define ID_CLOSE_BUTTON        (GUI_ID_USER + 0x21)
#define ID_PREVIOUS_BUTTON     (GUI_ID_USER + 0x23)
#define ID_PLAY_BUTTON         (GUI_ID_USER + 0x24)
#define ID_NEXT_BUTTON         (GUI_ID_USER + 0x25)
#define ID_STOP_BUTTON         (GUI_ID_USER + 0x26)
#define ID_PAUSE_BUTTON        (GUI_ID_USER + 0x27)
#define ID_OPEN_BUTTON         (GUI_ID_USER + 0x28)
#define ID_PLAY_LIST_BUTTON    (GUI_ID_USER + 0x29)
#define ID_FULL_SCREEN_BUTTON  (GUI_ID_USER + 0x2A)
#define ID_ADD_BUTTON          (GUI_ID_USER + 0x2B)

/* video Progress slider */
#define ID_PROGRESS_SLIDER     (GUI_ID_USER + 0x31)


#define ID_SLIDER_TIMER        1
#define ID_PLAYLIST_TIMER      2

#define VIDEO_IDLE             0
#define VIDEO_PLAY             1
#define VIDEO_STOP             2
#define VIDEO_PAUSE            3

#define REPEAT_NONE            0
#define REPEAT_ONCE            1
#define REPEAT_ALL             2

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static uint8_t _StartPlay(char * filename);
static void _StopPlay(void);
static void _PausePlay(void);
static void _ResumePlay(void);
static uint8_t _ShowFullScreen(void);
static uint8_t _CloseFullScreen(void);
/* _aDialogCreate */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect,   "Video Player", ID_FRAMEWIN_INFO,    0,   0, 240, 294, 0, 0x64, 0 },
  { LISTVIEW_CreateIndirect, "Listview",     ID_VIDEO_LIST,       5, 226, 174,  65, 0,  0x0, 0 },   
  { SLIDER_CreateIndirect,   "Slider",      ID_PROGRESS_SLIDER,   5, 150, 205,  20, 0,  0x0, 0 },
}; 

static WM_HWIN VIDEOPLAYER_hWin, hFrame, hBigFrame;
static char const                *apDrives = "0:";
static const char                acMask_video[] = ".video";
static const char                acMask_dir[] = ".dir";

static FILELIST_FileTypeDef  *pVideoList;
static  CHOOSEFILE_INFO      *pFileInfo;
static uint8_t              playlist_select = 0;
uint8_t VideoPlayer_State = VIDEO_IDLE;
static uint8_t                    FullScreen = 0;
static uint32_t                   Video_file_pos = 0;
static GUI_MOVIE_HANDLE hMovie = 0;
static WM_HTIMER hPlaylistTimer;
FIL Video_File;
GUI_MOVIE_INFO             Video_Info;
static VideoSettingsTypeDef       PlayerSettings;





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
  * @brief  Paints play button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_play(BUTTON_Handle hObj) {
  int Index;

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;

  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  if((VideoPlayer_State == VIDEO_IDLE) || (VideoPlayer_State == VIDEO_PAUSE))
  {
    
    if(Index)
    {
      GUI_DrawBitmap(&bmplay_pressed, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmplay_not_pressed, 0, 0);
    }
  }
  else
  {
    if(Index)
    {
      GUI_DrawBitmap(&bmpause_pressed, 0, 0);
    }
    else
    {
      GUI_DrawBitmap(&bmpause_not_pressed, 0, 0);
    }
  }
}

/**
  * @brief  Paints stop button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_stop(BUTTON_Handle hObj) {
  int Index;

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
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
  * @brief  Paints previous button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_previous(BUTTON_Handle hObj) {
  int Index;

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
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
  * @brief  Paints next button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_next(BUTTON_Handle hObj) {
  int Index;

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
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
  * @brief  Paints add to playlist button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_add(BUTTON_Handle hObj) {
  int Index;

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
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
  * @brief  Paints open button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_open(BUTTON_Handle hObj) {
  int Index;

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
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

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
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
  * @brief  Paints Full Screen button
  * @param  hObj: button handle
  * @retval None
  */
static void _OnPaint_fullscreen(BUTTON_Handle hObj) {
  int Index;

  Index = (WIDGET_GetState(hObj) & BUTTON_STATE_PRESSED) ? 1 : 0;
  
  GUI_SetBkColor(FRAMEWIN_GetDefaultClientColor());
  GUI_Clear();
  
  if(Index)
  {
    GUI_DrawBitmap(&bmwindowfullscreen, 0, 0);
  }
  else
  {
    GUI_DrawBitmap(&bmwindownofullscreen, 0, 0);
  }
}

/**
  * @brief  callback for play button
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
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
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
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
  * @brief  callback for repeat button
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
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
  * @brief  callback for next button
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
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
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
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
  * @brief  callback for add to playlist button
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
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
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
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
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
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
  * @brief  callback for full screen button
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbButton_fullscreen(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      _OnPaint_fullscreen(pMsg->hWin);
      break;
    default:
      /* The original callback */
      BUTTON_Callback(pMsg);
      break;
  }
}

/**
  * @brief  Get data.
  * @param  p: Handle to file structure
  * @param  ppData: pointer to data buffer to be read
  * @param  NumBytesReq: number of bytes to be read
  * @param  Off: offset in the file    
  * @retval number of read bytes
  */
static int _GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off)
{
  unsigned int NumBytesRead;
  FIL * phFile;

  phFile = (FIL *)p;
  
  /* Set file pointer to the required position */
  f_lseek(phFile, Off);
  
  /* Read data into buffer */
  f_read(phFile, (U8 *)*ppData, NumBytesReq, &NumBytesRead); 
  
  /* Return number of available bytes */
  return NumBytesRead;  
}

/**
  * @brief  Callback function for movie player. Uses multiple buffering if
  *         available to avoid tearing effects.
  * @param  hMem:         pointer to parent handle
  * @param  Notification: notification ID
  * @param  CurrentFrame: current playing frame
  * @retval None
  */
void _cbNotify(GUI_HMEM hMem, int Notification, U32 CurrentFrame) 
{
  long progress;
  WM_HWIN hItem;  
  
  switch (Notification) {
  case GUI_MOVIE_NOTIFICATION_PREDRAW:
    GUI_MULTIBUF_Begin();
        
    break;
  case GUI_MOVIE_NOTIFICATION_POSTDRAW:
    GUI_MULTIBUF_End();
    progress = 100 * GUI_MOVIE_GetFrameIndex(hMovie)/ Video_Info.NumFrames;
    hItem = WM_GetDialogItem(VIDEOPLAYER_hWin, ID_PROGRESS_SLIDER);
    SLIDER_SetValue(hItem, progress);
    break;
  case GUI_MOVIE_NOTIFICATION_STOP:
    
    if(GUI_MOVIE_GetFrameIndex(hMovie) >= (Video_Info.NumFrames - 1))
    {  
      if( FullScreen == 1)
      {
        WM_InvalidateWindow(hBigFrame);
        WM_Update(hBigFrame);
      }   
      _StopPlay();
      
      if(PlayerSettings.b.repeat != REPEAT_NONE)
      {
        if (PlayerSettings.b.repeat == REPEAT_ALL)
        {
          if(Video_file_pos < (pVideoList->ptr - 1))
          {
            Video_file_pos++;
          }
          else 
          {        
            Video_file_pos = 0; 
          }
          LISTVIEW_SetSel(WM_GetDialogItem(VIDEOPLAYER_hWin, ID_VIDEO_LIST), Video_file_pos);          
        }
        
        _StartPlay((char *)pVideoList->file[Video_file_pos].name);
      }
      else
      {
        hItem = WM_GetDialogItem(VIDEOPLAYER_hWin, ID_PLAY_BUTTON);
        WM_InvalidateWindow(hItem);
        WM_Update(hItem);
      }   
    }
    break;
  }
}

/**
  * @brief  callback for mute button
  * @param  Xpos:  window X position
  * @param  Ypos:  window Y position
  * @param  Xsize: window X size 
  * @param  Ysize: window Y size
  * @retval None
  */
void __GetWindowRect(int *XPos, int *YPos, int *XSize, int *YSize)
{
  *XPos = 6;
  *YPos = 34;
  *XSize = 226;
  *YSize = 140;
}

/**
  * @brief  Start play
  * @param  filename: pointer to the video file name
  * @retval None
  */
static uint8_t _StartPlay(char * filename) 
{
  int XPos, YPos, XSize, YSize, nx, ny, n;
  
  
  if(f_open(&Video_File, filename, FA_OPEN_EXISTING | FA_READ) == FR_OK)
  {
    GUI_MOVIE_GetInfoEx(_GetData, &Video_File, &Video_Info);
    
    if((Video_Info.xSize == 0) || (Video_Info.ySize == 0) || 
       (Video_Info.xSize > 1024) || (Video_Info.ySize > 768))
    {
      return 1;
    }
    
    hMovie = GUI_MOVIE_CreateEx(_GetData, &Video_File, _cbNotify);
    VideoPlayer_State = VIDEO_PLAY;
    
    if (FullScreen == 0)
    {
      __GetWindowRect(&XPos, &YPos, &XSize, &YSize);
      
      nx = (XSize * 1000) / Video_Info.xSize;
      ny = (YSize * 1000) / Video_Info.ySize; 
      
      if (nx < ny) {
        n = nx;
      } else {
        n = ny;
      }
      
      XPos = XPos + (XSize - ((Video_Info.xSize * n) / 1000)) / 2;
      YPos = YPos + (YSize - ((Video_Info.ySize * n) / 1000)) / 2;
      GUI_MOVIE_ShowScaled(hMovie, XPos, YPos, n, 1000, 0);
    }
    else
    {
      if((Video_Info.xSize > LCD_GetXSize()) ||(Video_Info.ySize > LCD_GetYSize()))
      {
        nx = (LCD_GetXSize() * 1000) / Video_Info.xSize;
        ny = (LCD_GetYSize() * 1000) / Video_Info.ySize; 
        
        if (nx < ny) {
          n = nx;
        } else {
          n = ny;
        }
        
        XPos = (LCD_GetXSize() - ((Video_Info.xSize * n) / 1000)) / 2;
        YPos = (LCD_GetYSize() - ((Video_Info.ySize * n) / 1000)) / 2;
        
        GUI_MOVIE_ShowScaled(hMovie, XPos, YPos, n, 1000, 0);
      }
      else
      {
        XPos = (LCD_GetXSize() - Video_Info.xSize) / 2;
        YPos = (LCD_GetYSize() - Video_Info.ySize) / 2;
        
        GUI_MOVIE_Show(hMovie, XPos, YPos, 0); 
      }
    }
  }
  return 0;
}


/**
  * @brief  callback for Full Screen window
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbFullScreen(WM_MESSAGE * pMsg) 
{
  GUI_RECT r;
  const GUI_PID_STATE * pState;
  
  switch (pMsg->MsgId) 
  {
  case WM_PAINT:
    WM_GetInsideRect(&r);
    GUI_ClearRectEx(&r);
    break;
    
  case  WM_TOUCH:
    pState = (const GUI_PID_STATE *)pMsg->Data.p;
    if (pState) 
    {
      if (pState->Pressed == 1)
      {
        _CloseFullScreen();
        WM_DeleteWindow(pMsg->hWin);
      }
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  Show full screen
  * @param  None
  * @retval None
  */
static uint8_t _ShowFullScreen(void) 
{
  int XPos, YPos, nx, ny, n;
  WM_HWIN hItem;  
  
  if(hMovie != 0)
  {
    FullScreen = 1;
    hBigFrame = WM_CreateWindowAsChild(0, 0, LCD_GetXSize(), LCD_GetYSize(), 
                                       WM_HBKWIN, 
                                       WM_CF_SHOW | WM_CF_STAYONTOP, 
                                       _cbFullScreen, 
                                       0);
    
    if((Video_Info.xSize > LCD_GetXSize()) ||(Video_Info.ySize > LCD_GetYSize()))
    {
      nx = (LCD_GetXSize() * 1000) / Video_Info.xSize;
      ny = (LCD_GetYSize() * 1000) / Video_Info.ySize; 
      
      if (nx < ny) {
        n = nx;
      } else {
        n = ny;
      }
      
      XPos = (LCD_GetXSize() - ((Video_Info.xSize * n) / 1000)) / 2;
      YPos = (LCD_GetYSize() - ((Video_Info.ySize * n) / 1000)) / 2;
      
      GUI_MOVIE_ShowScaled(hMovie, XPos, YPos, n, 1000, 0);
    }
    else
    {
      XPos = (LCD_GetXSize() - Video_Info.xSize) / 2;
      YPos = (LCD_GetYSize() - Video_Info.ySize) / 2;
      
      GUI_MOVIE_Show(hMovie, XPos, YPos, 0); 
      VideoPlayer_State = VIDEO_PLAY;
      hItem = WM_GetDialogItem(VIDEOPLAYER_hWin, ID_PLAY_BUTTON);
      WM_InvalidateWindow(hItem);
      WM_Update(hItem);      
    }
  }
  return 0;
}

/**
  * @brief  Close full screen
  * @param  None
  * @retval None
  */
static uint8_t _CloseFullScreen(void) 
{
  WM_HWIN hItem;    
  int XPos, YPos, XSize, YSize, nx, ny, n;
  
  __GetWindowRect(&XPos, &YPos, &XSize, &YSize);
  
  nx = (XSize * 1000) / Video_Info.xSize;
  ny = (YSize * 1000) / Video_Info.ySize; 
  
  if (nx < ny) {
    n = nx;
  } else {
    n = ny;
  }
  
  XPos = XPos + (XSize - ((Video_Info.xSize * n) / 1000)) / 2;
  YPos = YPos + (YSize - ((Video_Info.ySize * n) / 1000)) / 2;

  GUI_MOVIE_ShowScaled(hMovie, XPos, YPos, n, 1000, 0);
  hItem = WM_GetDialogItem(VIDEOPLAYER_hWin, ID_PLAY_BUTTON);
  WM_InvalidateWindow(hItem);
  WM_Update(hItem);
  FullScreen = 0;
  return 0;
}
/**
  * @brief  Stop play 
  * @param  None
  * @retval None
  */
static void _StopPlay(void) 
{
  if(hMovie != 0)
  {
    GUI_MOVIE_Delete (hMovie);
    hMovie = 0;
  }
  VideoPlayer_State = VIDEO_IDLE;
  f_close(&Video_File);  
}

/**
  * @brief  Pause play
  * @param  None
  * @retval None
  */
static void _PausePlay(void) 
{
  if( VideoPlayer_State == VIDEO_PLAY)
  {
    GUI_MOVIE_Pause(hMovie);
    
    VideoPlayer_State = VIDEO_PAUSE; 
  }
}


/**
  * @brief  Resume play
  * @param  None
  * @retval None
  */
static void _ResumePlay(void) 
{
  if( VideoPlayer_State == VIDEO_PAUSE)
  {
    GUI_MOVIE_Play(hMovie);
    
    VideoPlayer_State = VIDEO_PLAY; 
  }
}
/**
  * @brief  callback for video window
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbVideoWindow(WM_MESSAGE * pMsg) 
{
  GUI_RECT r;
 
  switch (pMsg->MsgId) 
  {
  case WM_PAINT:
    WM_GetInsideRect(&r);
    GUI_ClearRectEx(&r);
    break;
    
  default:
    WM_DefaultProc(pMsg);
  }
}

/**
  * @brief  callback for progress slider.
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
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
  * @brief  Add entire folder to play list.
  * @param  Foldername: pointer to folder name.
  * @retval None
  */
static void _AddEntireFolder(char *Foldername)
{
  FRESULT res;
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
      
      if (pVideoList->ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == 0)
        {
          if(((strstr(fn, ".emf")) || (strstr(fn, ".EMF"))) && (VIDEOPLAYER_hWin != 0))
          {
            strcpy(tmp, Foldername);
            strcat(tmp, "/");
            strcat(tmp, fn);
            strncpy((char *)pVideoList->file[pVideoList->ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
            hItem = WM_GetDialogItem(VIDEOPLAYER_hWin, ID_VIDEO_LIST);
            LISTVIEW_AddRow(hItem, NULL);  
            FILEMGR_GetFileOnly (tmp, fn);
            LISTVIEW_SetItemText(hItem, 0, pVideoList->ptr, fn);
            pVideoList->ptr++;
          }
        }
      }   
    }
  }
  f_closedir(&dir);
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
  static uint8_t        prev_usb_status = 0;
   
  switch (pMsg->MsgId) 
  {
  case WM_CREATE:
    prev_usb_status = k_StorageGetStatus(USB_DISK_UNIT);    
    hStatusTimer = WM_CreateTimer(pMsg->hWin, 0, 500, 0);      
    break;
    
  case WM_TIMER:
    if(prev_usb_status != k_StorageGetStatus(USB_DISK_UNIT))
    {
      prev_usb_status = k_StorageGetStatus(USB_DISK_UNIT);
      if(pVideoList->file[Video_file_pos].name[0] == '0')
      {
        if(VideoPlayer_State != VIDEO_IDLE)
        {
          _StopPlay();  
          hItem = WM_GetDialogItem(VIDEOPLAYER_hWin, ID_PROGRESS_SLIDER);
          SLIDER_SetValue(hItem, 0);
          
          hItem = WM_GetDialogItem(VIDEOPLAYER_hWin, ID_PLAY_BUTTON);
          WM_InvalidateWindow(hItem);
          WM_Update(hItem); 
          WM_InvalidateWindow(hFrame);  
        }
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
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN  hItem, hClient;
  int      NCode;
  int      result;  
  int      Id, Index, newpos;
  GUI_RECT r;
  int  ItemNbr;
  static char tmp[FILEMGR_FILE_NAME_SIZE];  
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:

    
    pVideoList = (FILELIST_FileTypeDef *)k_malloc(sizeof(FILELIST_FileTypeDef));
    pFileInfo = (CHOOSEFILE_INFO *)k_malloc(sizeof(CHOOSEFILE_INFO));
    pVideoList->ptr = 0;
    
    
    PlayerSettings.d32 = k_BkupRestoreParameter(CALIBRATION_VIDEOPLAYER_SETTING_BKP);
    
    /* Initialization of 'Listview' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_VIDEO_LIST);
    LISTVIEW_AddColumn(hItem, 165, "Video", GUI_TA_VCENTER | GUI_TA_LEFT);
    LISTVIEW_SetGridVis(hItem, 0);
    LISTVIEW_SetAutoScrollV(hItem, 1);
    LISTVIEW_SetBkColor(hItem, LISTVIEW_CI_UNSEL, GUI_BLACK);
    LISTVIEW_SetTextColor(hItem, LISTVIEW_CI_UNSEL, GUI_CYAN);

    hItem = BUTTON_CreateEx(201, 182, 35, 35, pMsg->hWin, WM_CF_SHOW, 0, ID_STOP_BUTTON);
    WM_SetCallback(hItem, _cbButton_stop);    
    
    hItem = BUTTON_CreateEx(72, 182, 35, 35, pMsg->hWin, WM_CF_SHOW, 0, ID_PREVIOUS_BUTTON);
    WM_SetCallback(hItem, _cbButton_previous);     
    
    hItem = BUTTON_CreateEx(110, 175,  50, 50, pMsg->hWin, WM_CF_SHOW, 0, ID_PLAY_BUTTON);
    WM_SetCallback(hItem, _cbButton_play);
    
    hItem = BUTTON_CreateEx(163, 182, 35, 35, pMsg->hWin, WM_CF_SHOW, 0, ID_NEXT_BUTTON);
    WM_SetCallback(hItem, _cbButton_next);
    
    hItem = BUTTON_CreateEx(36,  184, 30,  30, pMsg->hWin, WM_CF_SHOW, 0, ID_REPEAT_BUTTON);
    WM_SetCallback(hItem, _cbButton_repeat);
    
    hItem = BUTTON_CreateEx(184, 229, 50, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_ADD_BUTTON);
    WM_SetCallback(hItem, _cbButton_add);      
      
    hItem = BUTTON_CreateEx(184, 260, 50, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_OPEN_BUTTON);
    WM_SetCallback(hItem, _cbButton_open); 

    hItem = BUTTON_CreateEx(2, 184, 30, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_CLOSE_BUTTON);
    WM_SetCallback(hItem, _cbButton_close);    
    
    hItem = BUTTON_CreateEx(210, 147, 25, 25, pMsg->hWin, WM_CF_SHOW, 0, ID_FULL_SCREEN_BUTTON);
    WM_SetCallback(hItem, _cbButton_fullscreen);      
    
    hClient = WM_GetClientWindow(pMsg->hWin);
    WM_GetClientRectEx(hClient, &r);
    hFrame = WM_CreateWindowAsChild(r.x0 + 5, r.y0 + 5, r.x1 - 10, r.y1 - 148, hClient, WM_CF_SHOW, _cbVideoWindow , 0);
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PLAY_BUTTON);    
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGRESS_SLIDER);
    SLIDER_SetNumTicks(hItem, 25);
    
    hItem = WM_CreateWindowAsChild(5,
                                   150,
                                   205,
                                   19,
                                   pMsg->hWin, 
                                   WM_CF_SHOW | WM_CF_HASTRANS | WM_CF_BGND,
                                   _cbDrawProgressSlider, 
                                   0);
    WM_CreateWindowAsChild(240, 320, 1, 1, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbMediaConnection , 0);
    break;
    
  case WM_TIMER:
    Id = WM_GetTimerId(pMsg->Data.v);
    if (Id == ID_PLAYLIST_TIMER)
    {
      playlist_select = 0;
    }    
    break; 
    
  case WM_DELETE:       
    k_BkupSaveParameter(CALIBRATION_VIDEOPLAYER_SETTING_BKP, PlayerSettings.d32);    
    WM_DeleteTimer(hPlaylistTimer);
    break;

    
case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    
    switch(Id) {
      
      /* Notification sent by "Close Button" */  
    case ID_FULL_SCREEN_BUTTON: 
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        if( VideoPlayer_State != VIDEO_IDLE)
        {
          _ShowFullScreen();
        }
        break;
      }
      break;
      
    /* Notifications sent by 'Add' Button */
    case ID_ADD_BUTTON: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        pFileInfo->pfGetData = k_GetData;
        pFileInfo->pMask = acMask_video;     
        hItem = CHOOSEFILE_Create(pMsg->hWin,  10, 10, 220, 210, &apDrives, 1, 0, "Add a video file to playlist", 0, pFileInfo);
        if(VideoPlayer_State == VIDEO_PLAY)
        {
          GUI_MOVIE_Pause(hMovie);
        }
        WM_MakeModal(hItem);
        result = GUI_ExecCreatedDialog(hItem);
        if (result == 0) 
        {      
          if((strstr(pFileInfo->pRoot, ".emf")) || (strstr(pFileInfo->pRoot, ".EMF")))
          {
            if(pVideoList->ptr < FILEMGR_LIST_DEPDTH)
            {
              strcpy((char *)pVideoList->file[pVideoList->ptr].name, pFileInfo->pRoot);
              FILEMGR_GetFileOnly ((char *)tmp, (char *)pFileInfo->pRoot);
              hItem = WM_GetDialogItem(pMsg->hWin, ID_VIDEO_LIST);
              
              LISTVIEW_AddRow(hItem, NULL);         
              LISTVIEW_SetItemText(hItem, 0, pVideoList->ptr, tmp);
              pVideoList->ptr++;
            }
          }
          
          WM_InvalidateWindow(hFrame);
        }
        if(VideoPlayer_State == VIDEO_PLAY)
        {
          GUI_MOVIE_Play(hMovie);  
        }
        break;
        
      }
      break;       
      
    /* Notifications sent by 'Open' Button */
    case ID_OPEN_BUTTON: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        
        pFileInfo->pfGetData = k_GetData;
        pFileInfo->pMask = acMask_dir;     
        hItem = CHOOSEFILE_Create(pMsg->hWin,  10, 10, 220, 210, &apDrives, 1, 0, "Add a folder", 0, pFileInfo);        
        if(VideoPlayer_State == VIDEO_PLAY)
        {
          GUI_MOVIE_Pause(hMovie);
        }
        WM_MakeModal(hItem);
        result = GUI_ExecCreatedDialog(hItem);
        if (result == 0) 
        {  
          _AddEntireFolder(pFileInfo->pRoot);
          
          WM_InvalidateWindow(hFrame);
        }
        if(VideoPlayer_State == VIDEO_PLAY)
        {
          GUI_MOVIE_Play(hMovie);
        }
        break;
      }
      break;      
      
      /* Notification sent by "Full Screen button" */  
    case ID_CLOSE_BUTTON: 
      switch (NCode) {
      case WM_NOTIFICATION_RELEASED:
        k_free(pVideoList); 
        k_free(pFileInfo);   
        _StopPlay();
        GUI_EndDialog(pMsg->hWin, 0);
        module_active = (-1);
        
        break;
      }
      break;      
      
      /* Notification sent by "Play Button" */  
    case ID_PLAY_BUTTON: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        
        if(VideoPlayer_State == VIDEO_IDLE)
        {
          if (pVideoList->ptr > 0)
          {
            _StartPlay((char *)pVideoList->file[Video_file_pos].name);
            LISTVIEW_SetSel(WM_GetDialogItem(VIDEOPLAYER_hWin, ID_VIDEO_LIST), Video_file_pos);
          }
          else
          {
            pFileInfo->pfGetData = k_GetData;
            pFileInfo->pMask = acMask_video;     
            hItem = CHOOSEFILE_Create(pMsg->hWin,  10, 10, 220, 210, &apDrives, 1, 0, "Open a video file", 0, pFileInfo);
            WM_MakeModal(hItem);
            result = GUI_ExecCreatedDialog(hItem);
            if (result == 0) 
            { 
              if((strstr(pFileInfo->pRoot, ".emf")) || (strstr(pFileInfo->pRoot, ".EMF")))
              {                   
                
                pVideoList->ptr = 0;
                
                strcpy((char *)pVideoList->file[pVideoList->ptr].name, pFileInfo->pRoot);
                FILEMGR_GetFileOnly (tmp, pFileInfo->pRoot);
                hItem = WM_GetDialogItem(pMsg->hWin, ID_VIDEO_LIST);
                
                /* Update Play list */
                strcpy((char *)pVideoList->file[pVideoList->ptr].name, pFileInfo->pRoot);
                
                ItemNbr = LISTVIEW_GetNumRows(hItem);
                while(ItemNbr--)
                {
                  LISTVIEW_DeleteRow(hItem, ItemNbr);
                }
                
                LISTVIEW_AddRow(hItem, NULL);         
                LISTVIEW_SetItemText(hItem, 0, pVideoList->ptr, tmp);
                pVideoList->ptr++;  
                Video_file_pos = 0;
                LISTVIEW_SetSel(hItem, 0);
                _StartPlay((char *)pVideoList->file[Video_file_pos].name);
                WM_InvalidateWindow(hFrame);
                
              }
            }
          }
        }
        else if(VideoPlayer_State == VIDEO_PLAY)
        {
          _PausePlay();         
        }
        else if(VideoPlayer_State == VIDEO_PAUSE)
        {
          _ResumePlay();  
        }        
        break;
      }
      break;

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
      
    case ID_STOP_BUTTON:
      _StopPlay();  
      hItem = WM_GetDialogItem(VIDEOPLAYER_hWin, ID_PROGRESS_SLIDER);
      SLIDER_SetValue(hItem, 0);
      WM_InvalidateWindow(hFrame);
      break;
      
    case ID_NEXT_BUTTON: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        if(Video_file_pos < (pVideoList->ptr - 1))
        {
          /*Play Next */
          Video_file_pos++;
        }
        else if(PlayerSettings.b.repeat == REPEAT_ALL)
        {
          Video_file_pos = 0; 
        }           
        LISTVIEW_SetSel(WM_GetDialogItem(VIDEOPLAYER_hWin, ID_VIDEO_LIST), Video_file_pos);        

        if(VideoPlayer_State == VIDEO_PLAY)
        {
          _StopPlay();
          _StartPlay((char *)pVideoList->file[Video_file_pos].name);
          WM_InvalidateWindow(hFrame);
        }
        break;
      }
      break;
      
      
    case ID_PREVIOUS_BUTTON: 
      switch(NCode) {
      case WM_NOTIFICATION_RELEASED:
        
        if( pVideoList->ptr > 0)
        {
          if(Video_file_pos > 0)
          {   
            Video_file_pos--;            
          }
          else if(PlayerSettings.b.repeat == REPEAT_ALL)
          {
            Video_file_pos = (pVideoList->ptr - 1); 
          } 
          LISTVIEW_SetSel(WM_GetDialogItem(VIDEOPLAYER_hWin, ID_VIDEO_LIST), Video_file_pos);
          
          if(VideoPlayer_State == VIDEO_PLAY)
          {
            _StopPlay();
            _StartPlay((char *)pVideoList->file[Video_file_pos].name);
            WM_InvalidateWindow(hFrame);
          }
        }    
        break;
        
      }
      break;
      
    /* Notifications sent by 'progress' Slider */
    case ID_PROGRESS_SLIDER: 
      if(NCode == WM_NOTIFICATION_CLICKED)
      {
        if(VideoPlayer_State != VIDEO_IDLE)
        {
          GUI_MOVIE_Pause(hMovie);
          hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGRESS_SLIDER);
          newpos = (SLIDER_GetValue(hItem) * Video_Info.NumFrames)/100;
          GUI_MOVIE_GotoFrame(hMovie, newpos);
          if(VideoPlayer_State == VIDEO_PLAY)
          {
            GUI_MOVIE_Play(hMovie);
          }
        }
      }
      break;
      
    /* Notifications sent by 'ListView' Slider */
    case ID_VIDEO_LIST: 
      if(NCode == WM_NOTIFICATION_CLICKED)
      {
        hItem = WM_GetDialogItem(pMsg->hWin, ID_VIDEO_LIST);
        Index = LISTVIEW_GetSel(hItem);
        
        if(Index < pVideoList->ptr)
        {
          Video_file_pos = Index;
          
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
            
            if(Index < pVideoList->ptr)
            {
              if(VideoPlayer_State != VIDEO_IDLE)
              {
                _StopPlay();
              } 
              
              _StartPlay((char *)pVideoList->file[Index].name); 
              hItem = WM_GetDialogItem(VIDEOPLAYER_hWin, ID_PLAY_BUTTON);
              WM_InvalidateWindow(hItem);
              WM_Update(hItem); 
              WM_InvalidateWindow(hFrame);
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
  * @brief  Video window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  Video_file_pos = 0;
  pVideoList->ptr = 0;
  VideoPlayer_State = VIDEO_IDLE;
  VIDEOPLAYER_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWin, xpos, ypos);
  
}


/**
  * @brief  Video direct open
  * @param  filename: video file name
  * @retval None
  */
static void VideoDirectOpen(char *filename)
{
  WM_HWIN hItem;
  int ItemNbr;  
  static char tmp[FILEMGR_FILE_NAME_SIZE];    
  
  VIDEOPLAYER_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_GetDesktopWindowEx(1), 0, 26);
  
  if(VIDEOPLAYER_hWin != 0)
  {      
      pVideoList->ptr = 0;
      
      FILEMGR_GetFileOnly (tmp, filename);
      hItem = WM_GetDialogItem(VIDEOPLAYER_hWin, ID_VIDEO_LIST);
      
      /* Update Playlist */
      strcpy((char *)pVideoList->file[pVideoList->ptr].name, filename);
      
      ItemNbr = LISTVIEW_GetNumRows(hItem);
      while(ItemNbr--)
      {
        LISTVIEW_DeleteRow(hItem, ItemNbr);
      }
      
      LISTVIEW_AddRow(hItem, NULL);         
      LISTVIEW_SetItemText(hItem, 0, pVideoList->ptr, tmp);
      pVideoList->ptr++;  
      Video_file_pos = 0;
      LISTVIEW_SetSel(hItem, 0);
      _StartPlay((char *)pVideoList->file[Video_file_pos].name);
      
      WM_InvalidateWindow(hFrame);

  }
}
/**
  * @}
  */

/**
  * @}
  */
