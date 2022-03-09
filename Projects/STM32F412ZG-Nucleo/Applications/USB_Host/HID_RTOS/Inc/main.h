/**
  ******************************************************************************
  * @file    USB_Host/HID_RTOS/Inc/main.h 
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
#include "stm32f4xx_nucleo_144.h"
#include "stm32_adafruit_lcd.h"
#include "lcd_log.h"
#include "usbh_hid.h"
#include "usbh_hid_parser.h"

/* Exported types ------------------------------------------------------------*/
typedef enum { 
  HID_DEMO_IDLE = 0,
  HID_DEMO_START,
  HID_DEMO_MOUSE,  
  HID_DEMO_KEYBOARD,
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
  APPLICATION_IDLE = 0,
  APPLICATION_DISCONNECT,  
  APPLICATION_START,
  APPLICATION_READY,    
  APPLICATION_RUNNING,
}HID_ApplicationTypeDef;

extern USBH_HandleTypeDef hUSBHost;
extern HID_ApplicationTypeDef Appli_state;
extern HID_MOUSE_Info_TypeDef mouse_info;
extern HID_DEMO_StateMachine hid_demo;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void HID_SelectItem(uint8_t **menu, uint8_t item);
void HID_MenuInit(void);
void HID_UpdateMenu(void);
void HID_MouseMenuProcess(void);
void HID_KeyboardMenuProcess(void);
void HID_MOUSE_ButtonReleased(uint8_t button_idx);
void HID_MOUSE_ButtonPressed(uint8_t button_idx);
void USR_MOUSE_ProcessData(HID_MOUSE_Info_TypeDef *data);
void USR_KEYBRD_ProcessData(uint8_t data);

#endif /* __MAIN_H */
