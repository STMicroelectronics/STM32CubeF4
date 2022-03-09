/**
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/Src/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "usbh_core.h"
#include "usbh_cdc.h"
#include "stm324x9i_eval_io.h"
#include "stm324x9i_eval_sd.h"
#include "lcd_log.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* Exported constants --------------------------------------------------------*/
#define FILEMGR_LIST_DEPDTH                     24
#define FILEMGR_FILE_NAME_SIZE                  40
#define FILEMGR_FULL_PATH_SIZE                 256
#define FILEMGR_MAX_LEVEL                        4   
#define FILETYPE_DIR                             0
#define FILETYPE_FILE                            1

/* Exported types ------------------------------------------------------------*/
typedef enum {
  CDC_DEMO_IDLE = 0,
  CDC_DEMO_WAIT,  
  CDC_DEMO_SEND,
  CDC_DEMO_RECEIVE,
  CDC_DEMO_CONFIGURATION,
  CDC_DEMO_REENUMERATE,
}CDC_Demo_State;

typedef enum {
  CDC_SEND_IDLE = 0,
  CDC_SEND_WAIT, 
  CDC_SEND_SELECT_FILE,
  CDC_SEND_TRANSMIT_FILE,   
}CDC_Send_State;

typedef enum {
  CDC_RECEIVE_IDLE = 0,
  CDC_RECEIVE_WAIT,    
  CDC_RECEIVE_RECEIVE,
}CDC_Receive_State;

typedef enum {
  CDC_CONFIGURATION_IDLE = 0,
  CDC_CONFIGURATION_WAIT,    
  CDC_CONFIGURATION_SET_BAUD_RATE,   
  CDC_CONFIGURATION_SET_DATA_BITS,    
  CDC_CONFIGURATION_SET_PARITY,    
  CDC_CONFIGURATION_SET_STOP_BIT,     
}CDC_Configuration_State;

typedef struct _DemoStateMachine {
  __IO CDC_Demo_State          state;
  __IO CDC_Send_State          Send_state;
  __IO CDC_Receive_State       Receive_state;
  __IO CDC_Configuration_State Configuration_state;    
  __IO uint8_t                 select;
  __IO uint8_t                 lock;
}CDC_DEMO_StateMachine;

typedef struct _DemoSettings {
  __IO uint8_t BaudRateIdx;
  __IO uint8_t DataBitsIdx;
  __IO uint8_t ParityIdx;
  __IO uint8_t StopBitsIdx;
}CDC_DEMO_Settings;

typedef struct _DemoSettingStateMachine {
  CDC_DEMO_Settings settings;
  __IO uint8_t      select;
  __IO uint8_t      lock;
}CDC_DEMO_SETTING_StateMachine;

typedef enum {
  CDC_SELECT_MENU = 0,
  CDC_SELECT_FILE ,  
  CDC_SELECT_CONFIG,    
}CDC_DEMO_SelectMode;

typedef enum {
  APPLICATION_IDLE = 0,  
  APPLICATION_START,
  APPLICATION_READY,    
  APPLICATION_RUNNING,
  APPLICATION_DISCONNECT,
}CDC_ApplicationTypeDef;

typedef struct _FILELIST_LineTypeDef {
  uint8_t type;
  uint8_t name[FILEMGR_FILE_NAME_SIZE];
}FILELIST_LineTypeDef;

typedef struct _FILELIST_FileTypeDef {
  FILELIST_LineTypeDef file[FILEMGR_LIST_DEPDTH] ;
  uint16_t             ptr; 
}FILELIST_FileTypeDef;

extern USBH_HandleTypeDef hUSBHost;
extern CDC_ApplicationTypeDef Appli_state;
extern uint8_t *DEMO_CONFIGURATION_menu[];
extern CDC_DEMO_SelectMode CdcSelectMode;
extern CDC_DEMO_SETTING_StateMachine CdcSettingsState;
extern CDC_DEMO_StateMachine CdcDemo;
extern uint8_t PrevSelect;
extern FILELIST_FileTypeDef FileList;
extern uint8_t *DEMO_SEND_menu[];

/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Toggle_Leds(void);
void Menu_Init(void);
void CDC_SelectItem(uint8_t **menu, uint8_t item);
void CDC_Handle_Send_Menu(void);
void CDC_Handle_Receive_Menu(void);
void CDC_Handle_Configuration_Menu(void);
void CDC_ChangeSelectMode(CDC_DEMO_SelectMode select_mode);
void CDC_AdjustSettingMenu(void);
void CDC_SelectItem(uint8_t **menu, uint8_t item);
void CDC_ChangeSelectMode(CDC_DEMO_SelectMode select_mode);
void CDC_SelectSettingsItem(uint8_t item);
void CDC_MenuProcess(void);
void CDC_Settings_ProbeKey(JOYState_TypeDef state);
void CDC_SendFile_ProbeKey(JOYState_TypeDef state);
uint8_t SD_StorageInit(void);
FRESULT SD_StorageParse(void);
void LCD_ClearTextZone(void);
void GetDefaultConfiguration(void);
#endif /* __MAIN_H */
