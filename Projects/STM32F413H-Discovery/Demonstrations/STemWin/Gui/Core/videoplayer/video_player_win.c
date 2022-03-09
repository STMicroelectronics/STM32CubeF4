/**
  ******************************************************************************
  * @file    video_player_win.c
  * @author  MCD Application Team
  * @brief   video player functions
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
#include "GUI_AVI.h"
#include "DIALOG.h"
#include "k_module.h"
#include "video_player_res.c"

/** @addtogroup VIDEO_PLAYER_MODULE
  * @{
  */

/** @defgroup VIDEO_PLAYER
  * @brief video player routines 
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos);
static void _StopPlay(FIL * file) ;
static uint8_t _StartPlay(char * filename, FIL * file, uint16_t x0, uint16_t y0) ;
static void _AddEntireFolder(char *Foldername);

/* Private typedef -----------------------------------------------------------*/
K_ModuleItem_Typedef  video_player_board =
{  
  5,
  (uint8_t*)"",
  &bmvideo5,
  Startup,
}
;

/* Private defines -----------------------------------------------------------*/
#define VIDEO_FRAME_ID            (GUI_ID_USER + 0x00)


#define FILENAME1_ID              (GUI_ID_USER + 0x19)
#define ID_BUTTON_EXIT_VIDEO      (GUI_ID_USER + 0x1B)

#define ID_TITLE                  (GUI_ID_USER + 0x51)
 
#define VIDEO_IDLE             0
#define VIDEO_PLAY             1
#define VIDEO_STOP             2
#define VIDEO_PAUSE            3

#define VIDEO_FRAME_TIME       40

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const GUI_WIDGET_CREATE_INFO _aDialog[] = 
{
  { WINDOW_CreateIndirect, "Window", VIDEO_FRAME_ID, 0, 0, 240, 240, 0, 0x0, 0 },
};

 GUI_AVI_HANDLE havi;

static WM_HWIN  VideoWin = 0, hFrame = 0;
uint8_t                         VideoPlayer_State = VIDEO_IDLE;
FIL                             Video_File;
uint8_t                         Video_Path[256];
static FILELIST_FileTypeDef     VideoList;
static uint32_t                 Video_FilePos = 0;
GUI_AVI_INFO                    Video_Info;

#define AVI_VIDEO_BUF_SIZE    (20 * 1024)
uint8_t    FrameBuffer[AVI_VIDEO_BUF_SIZE];
#define AUDIO_BUFFER_SIZE      (5 * 1024) 
uint8_t    AudioBuffer[AUDIO_BUFFER_SIZE + 8];

extern int status;

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Paints record button
  * @param  hObj: button handle
  * @retval None
*/
static void _OnPaint_exit(BUTTON_Handle hObj) 
{
    GUI_DrawBitmap(&bmvideo_exit, 20, 0);
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
  FRESULT res;
  
  if(Off != 0xFFFFFFFF)
  {
    /* Set file pointer to the required position */
    f_lseek((FIL *)p, Off);
  }
  
  /* Read data into buffer */
  res = f_read((FIL *)p, (U8 *)*ppData, NumBytesReq, &NumBytesRead);
  
  if((res != FR_OK) ||( NumBytesRead == 0))
  {
    NumBytesRead = 0;
  }
   
  /* Return number of available bytes */
  return NumBytesRead;  
}

/**
  * @brief  Stop play 
  * @param  None
  * @retval None
  */
static void _StopPlay(FIL * file) 
{
  GUI_AVI_Stop (havi);
  
  f_close(file); 
  WM_InvalidateWindow(hFrame);   
}

/**
  * @brief  Start play
  * @param  filename: pointer to the video file name
  * @retval None
  */
static uint8_t _StartPlay(char * filename, FIL * file, uint16_t x0, uint16_t y0) 
{  
  if(f_open(file, filename, FA_OPEN_EXISTING | FA_READ) == FR_OK)
  {
    GUI_AVI_Start   (havi, file, f_size(file));
    GUI_AVI_GetInfo (havi, &Video_Info);
        
    if((Video_Info.xSize == 0) || (Video_Info.ySize == 0) || 
       (Video_Info.xSize > LCD_GetXSize()) || (Video_Info.ySize > LCD_GetYSize()))
    {
      _StopPlay(file);
      VideoPlayer_State = VIDEO_STOP;
  
      return 1;
    }
    GUI_AVI_Show(havi, x0, y0, 0);
  }
  return 0;
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
  switch (Notification) 
  {      
  case GUI_AVI_NOTIFICATION_EOF:
    {
      _StopPlay(&Video_File);
      if(Video_FilePos < (VideoList.ptr - 1))
      {
        Video_FilePos++;
      }
      else 
      {        
        Video_FilePos = 0; 
      }

      _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File,0, 40);     
    } 
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

        VideoList.ptr = 0;
        Video_FilePos = 0;
        
        _StopPlay(&Video_File);
        
        GUI_EndDialog(VideoWin, 0);
           
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
  * @brief  main callback for Video Player
  * @param  pMsg: Pointer to Date structure
  * @retval None
  */

static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode;

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
   
    hItem = BUTTON_CreateEx(186, 3, 50, 30, pMsg->hWin, WM_CF_SHOW, 0, ID_BUTTON_EXIT_VIDEO);
    WM_SetCallback(hItem, _cbButton_exit);     

    /* Detect if no wav file in the SDCard */
    if(VideoList.ptr == 0)
    {
      hItem = TEXT_CreateEx(20,  10,  170,  25, pMsg->hWin, WM_CF_SHOW, 0, ID_TITLE, "No available AVI files");
      hItem = WM_GetDialogItem(pMsg->hWin, ID_TITLE);
      TEXT_SetFont(hItem, &GUI_FontLubalGraph20B); 
      TEXT_SetTextColor(hItem,GUI_WHITE); 
    }
    
    WM_CreateWindowAsChild(239, 239, 1, 1, pMsg->hWin, WM_CF_SHOW | WM_CF_HASTRANS, _cbMediaConnection , 0);
    
    _StartPlay((char *)VideoList.file[Video_FilePos].name, &Video_File,0, 40);
    
    break;
    
  case WM_PAINT:
    GUI_SetColor(0x7F14E7);
    GUI_FillRect(0, 0, 240, 40);    
    break;
    
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
        
    switch (NCode) {
    case WM_NOTIFICATION_RELEASED:      /* React only if released */
      switch (Id) {
      case ID_BUTTON_EXIT_VIDEO:
        status = 1;
        _StopPlay(&Video_File);
        GUI_EndDialog(pMsg->hWin, 0);
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
  static char tmp[FILEMGR_FILE_NAME_SIZE]; 
  
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
      
      if (VideoList.ptr < FILEMGR_LIST_DEPDTH)
      {
        if ((fno.fattrib & AM_DIR) == 0)
        {
          if((strstr(fn, ".avi")) || (strstr(fn, ".AVI")))
          {
            strcpy(tmp, Foldername);
            strcat(tmp, "/");
            strcat(tmp, fn);
            
            strncpy((char *)VideoList.file[VideoList.ptr].name, (char *)tmp, FILEMGR_FILE_NAME_SIZE);
            VideoList.ptr++;
          }
        }
      }   
    }
  }
  f_closedir(&dir);
}

/**
  * @brief  video player window Startup
  * @param  hWin: pointer to the parent handle.
  * @param  xpos: X position 
  * @param  ypos: Y position
  * @retval None
  */
static void Startup(WM_HWIN hWin, uint16_t xpos, uint16_t ypos)
{
  VideoPlayer_State = VIDEO_IDLE;  
  Video_FilePos = 0;
  
  havi = GUI_AVI_CreateEx(_GetData, _cbNotify) ;

  GUI_AVI_SetBuffers (havi, FrameBuffer, AVI_VIDEO_BUF_SIZE, AudioBuffer, AUDIO_BUFFER_SIZE);
  
  GUI_AVI_SelectMode(havi, 0);  
  
  VideoList.ptr = 0;
  _AddEntireFolder("0:");
  _AddEntireFolder("0:/Video");
  
  VideoWin = GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, xpos, ypos);
}

/**
  * @}
  */

/**
  * @}
  */
