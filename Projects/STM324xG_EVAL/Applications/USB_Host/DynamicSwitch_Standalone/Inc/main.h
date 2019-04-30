/**
  ******************************************************************************
  * @file    USB_Host/DynamicSwitch_Standalone/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "usbh_core.h"
#include "stm324xg_eval_io.h"
#include "lcd_log.h"
#include "usbh_audio.h"
#include "stm324xg_eval_sd.h"
#include "usbh_msc.h"
#include "ff.h"
#include "usbh_hid.h"
#include "usbh_hid_parser.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "usbh_diskio_dma.h"

/* Exported constants --------------------------------------------------------*/
#define FILEMGR_LIST_DEPDTH                     24
#define FILEMGR_FILE_NAME_SIZE                  40
#define FILEMGR_FULL_PATH_SIZE                 256
#define FILEMGR_MAX_LEVEL                        4

#define FILETYPE_DIR                             0
#define FILETYPE_FILE                            1

/* Exported types ------------------------------------------------------------*/
typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_DISCONNECT,
  APPLICATION_MSC,
  APPLICATION_HID,
  APPLICATION_AUDIO,
  APPLICATION_RUNNING,
}DS_ApplicationTypeDef;

typedef enum {
  HID_DEMO_IDLE = 0,
  HID_DEMO_WAIT,
  HID_DEMO_START,
  HID_DEMO_MOUSE,
  HID_DEMO_KEYBOARD,
  HID_DEMO_REENUMERATE,
}HID_Demo_State;

typedef enum {
  HID_MOUSE_IDLE = 0,
  HID_MOUSE_WAIT,
  HID_MOUSE_START,
}HID_mouse_State;

typedef enum {
  HID_KEYBOARD_IDLE = 0,
  HID_KEYBOARD_WAIT,
  HID_KEYBOARD_START,
}HID_keyboard_State;

typedef struct _DemoStateMachine {
  __IO HID_Demo_State     state;
  __IO HID_mouse_State    mouse_state;
  __IO HID_keyboard_State keyboard_state;
  __IO uint8_t            select;
  __IO uint8_t            lock;
}HID_DEMO_StateMachine;

typedef enum {
  MSC_DEMO_IDLE = 0,
  MSC_DEMO_WAIT,
  MSC_DEMO_FILE_OPERATIONS,
  MSC_DEMO_EXPLORER,
  MSC_REENUMERATE,
}MSC_Demo_State;

typedef struct _MSC_DemoStateMachine {
  __IO MSC_Demo_State state;
  __IO uint8_t        select;
}MSC_DEMO_StateMachine;

typedef struct AUDIO_Info_t {
  uint32_t ChunkID;       /* 0  */
  uint32_t FileSize;      /* 4  */
  uint32_t FileFormat;    /* 8  */
  uint32_t SubChunk1ID;   /* 12 */
  uint32_t SubChunk1Size; /* 16 */
  uint16_t AudioFormat;   /* 20 */
  uint16_t NbrChannels;   /* 22 */
  uint32_t SampleRate;    /* 24 */
  uint32_t ByteRate;      /* 28 */
  uint16_t BlockAlign;    /* 32 */
  uint16_t BitPerSample;  /* 34 */
  uint32_t SubChunk2ID;   /* 36 */
  uint32_t SubChunk2Size; /* 40 */
}WAV_InfoTypedef;

typedef struct _FILELIST_LineTypeDef {
  uint8_t type;
  uint8_t name[FILEMGR_FILE_NAME_SIZE];
}FILELIST_LineTypeDef;

typedef struct _FILELIST_FileTypeDef {
  FILELIST_LineTypeDef  file[FILEMGR_LIST_DEPDTH] ;
  uint16_t              ptr;
}FILELIST_FileTypeDef;

typedef enum {
  AUDIO_DEMO_IDLE = 0,
  AUDIO_DEMO_WAIT,
  AUDIO_DEMO_EXPLORE,
  AUDIO_DEMO_PLAYBACK,
  AUDIO_REENUMERATE
}AUDIO_Demo_State;

typedef struct _AUDIO_DemoStateMachine {
  __IO AUDIO_Demo_State state;
  __IO uint8_t          select;
  __IO uint8_t          lock;
}AUDIO_DEMO_StateMachine;

typedef enum {
  AUDIO_SELECT_MENU = 0,
  AUDIO_PLAYBACK_CONTROL ,
}AUDIO_DEMO_SelectMode;

typedef enum {
  AUDIO_STATE_IDLE = 0,
  AUDIO_STATE_WAIT,
  AUDIO_STATE_INIT,
  AUDIO_STATE_CONFIG,
  AUDIO_STATE_PLAY,
  AUDIO_STATE_NEXT,
  AUDIO_STATE_PREVIOUS,
  AUDIO_STATE_FORWARD,
  AUDIO_STATE_BACKWARD,
  AUDIO_STATE_PAUSE,
  AUDIO_STATE_RESUME,
  AUDIO_STATE_VOLUME_UP,
  AUDIO_STATE_VOLUME_DOWN,
}AUDIO_PLAYBACK_StateTypeDef;

typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
}AUDIO_ErrorTypeDef;

extern uint8_t prev_select;
extern FATFS USBH_fatfs;
extern DS_ApplicationTypeDef Appli_state;
extern USBH_HandleTypeDef hUSBHost;
extern HID_MOUSE_Info_TypeDef mouse_info;
extern uint8_t *DEMO_MOUSE_menu[];
extern HID_DEMO_StateMachine hid_demo;
extern AUDIO_DEMO_StateMachine audio_demo;
extern FILELIST_FileTypeDef FileList;
extern AUDIO_PLAYBACK_StateTypeDef audio_state;
extern MSC_DEMO_StateMachine msc_demo;
extern AUDIO_DEMO_SelectMode audio_select_mode;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DS_MenuProcess(void);
void Toggle_Leds(void);
void Menu_Init(void);
void AUDIO_MenuSelectItem(uint8_t **menu, uint8_t item);
void AUDIO_MenuProcess(void);
void AUDIO_MenuProbeKey(JOYState_TypeDef state);
void AUDIO_PlaybackProbeKey(JOYState_TypeDef state);
uint8_t SD_StorageInit(void);
FRESULT SD_StorageParse(void);
AUDIO_ErrorTypeDef AUDIO_Start(uint8_t idx);
AUDIO_ErrorTypeDef AUDIO_Process(void);
AUDIO_ErrorTypeDef AUDIO_Stop(void);
FRESULT Explore_Disk(char *path, uint8_t recu_level);
void Audio_ChangeSelectMode(AUDIO_DEMO_SelectMode select_mode);
void MSC_File_Operations(void);
void MSC_MenuProcess(void);
void MSC_DEMO_ProbeKey(JOYState_TypeDef state);
void HID_SelectItem(uint8_t **menu, uint8_t item);
void HID_MenuProcess(void);
void HID_MouseMenuProcess(void);
void HID_KeyboardMenuProcess(void);
void HID_MOUSE_ButtonReleased(uint8_t button_idx);
void HID_MOUSE_ButtonPressed(uint8_t button_idx);
void USR_MOUSE_ProcessData(HID_MOUSE_Info_TypeDef *data);
void USR_KEYBRD_ProcessData(uint8_t data);
void HID_DEMO_ProbeKey(JOYState_TypeDef state);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
