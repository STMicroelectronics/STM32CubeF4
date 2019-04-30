/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/Inc/main.h
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
#include "usbh_msc.h"
#include "usbh_hid.h"
#include "usbh_hid_parser.h"
#include "stm324xg_eval_io.h"
#include "lcd_log.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "usbh_diskio_dma.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
  APPLI_HID_IDLE = 0,
  APPLI_HID_WAIT,
  APPLI_HID_START,
  APPLI_HID_MOUSE,
  APPLI_HID_KEYBOARD,
}Demo_HID_State;

typedef enum {
  HID_MOUSE_IDLE = 0,
  HID_MOUSE_WAIT,
  HID_MOUSE_START,
}Demo_mouse_State;

typedef enum {
  HID_KEYBOARD_IDLE = 0,
  HID_KEYBOARD_WAIT,
  HID_KEYBOARD_START,
}Demo_keyboard_State;

typedef enum {
  DEMO_IDLE = 0,
  DEMO_WAIT,
  DEMO_MSC_START,
  DEMO_HID_START,
  DEMO_HID,
  DEMO_MSC,
  DEMO_MSC_REENUMERATE,
  DEMO_HID_REENUMERATE,
}Demo_State;

typedef enum {
  APPLI_MSC_IDLE = 0,
  APPLI_MSC_WAIT,
  APPLI_MSC_FILE_OPERATIONS,
  APPLI_MSC_EXPLORER,
}Demo_MSC_State;

typedef struct _DemoStateMachine {
  __IO Demo_State           state;
  __IO Demo_MSC_State       msc_state;
  __IO Demo_HID_State       hid_state;
  __IO Demo_mouse_State     mouse_state;
  __IO Demo_keyboard_State  keyboard_state;
  __IO uint8_t              select;
}DEMO_StateMachine;

typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_FS_DISCONNECT,
  APPLICATION_FS_START,
  APPLICATION_FS_READY,
  APPLICATION_HS_DISCONNECT,
  APPLICATION_HS_START,
  APPLICATION_HS_READY,
  APPLICATION_RUNNING,
}DUAL_ApplicationTypeDef;

extern DEMO_StateMachine demo;
extern USBH_HandleTypeDef hUSBHost_FS;
extern USBH_HandleTypeDef hUSBHost_HS;
extern HCD_HandleTypeDef hhcd_FS;
extern HCD_HandleTypeDef hhcd_HS;
extern FATFS USBH_fatfs;
extern DUAL_ApplicationTypeDef Appli_HS_state;
extern DUAL_ApplicationTypeDef Appli_FS_state;
extern uint8_t *DEMO_MOUSE_menu[];
extern uint8_t prev_select;

/* Exported constants --------------------------------------------------------*/
#define hUSBHost                           hUSBHost_HS
/* Exported macro ------------------------------------------------------------*/
#define LCD_UsrLogY(...)                   LCD_LineColor = LCD_COLOR_YELLOW;\
                                           printf(__VA_ARGS__);\

/* Exported functions ------------------------------------------------------- */
FRESULT Explore_Disk(char *path, uint8_t recu_level);
void MSC_File_Operations(void);
void Toggle_Leds(void);
void Menu_Init(void);
void MSC_MenuProcess(void);
void HID_MenuProcess(void);
void Demo_SelectItem(uint8_t **menu, uint8_t item);
void HID_MouseMenuProcess(void);
void HID_KeyboardMenuProcess(void);
void HID_MOUSE_ButtonReleased(uint8_t button_idx);
void HID_MOUSE_ButtonPressed(uint8_t button_idx);
void USR_MOUSE_ProcessData(HID_MOUSE_Info_TypeDef *data);
void USR_KEYBRD_ProcessData(uint8_t data);
void DUAL_MenuProcess(void);
void DUAL_MenuInit(void);
void Demo_ProbeKey(JOYState_TypeDef state);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
