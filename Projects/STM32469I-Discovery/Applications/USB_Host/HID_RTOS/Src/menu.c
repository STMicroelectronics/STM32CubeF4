/**
  ******************************************************************************
  * @file    USB_Host/HID_RTOS/Src/menu.c 
  * @author  MCD Application Team
  * @brief   This file implements Menu Functions
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
HID_DEMO_StateMachine hid_demo;
osSemaphoreId MenuEvent;
__IO uint8_t ButtonPressed = 0;

/* Private function prototypes -----------------------------------------------*/
static void USBH_MouseDemo(USBH_HandleTypeDef *phost);
static void USBH_KeybdDemo(USBH_HandleTypeDef *phost);
static void HID_MenuThread(void const *argument);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Demo state machine.
  * @param  None
  * @retval None
  */
void HID_MenuInit(void)
{
  /* Create Menu Semaphore */
  osSemaphoreDef(osSem);

  MenuEvent = osSemaphoreCreate(osSemaphore(osSem), 1); 
  
  /* Force menu to show Item 0 by default */
  osSemaphoreRelease(MenuEvent);
  
  /* Menu task */
  osThreadDef(Menu_Thread, HID_MenuThread, osPriorityHigh, 0, 8 * configMINIMAL_STACK_SIZE);
  osThreadCreate(osThread(Menu_Thread), NULL);
}

/**
  * @brief  Updates the Menu. 
  * @param  None
  * @retval None
  */
void HID_UpdateMenu(void)
{
  /* Force menu to show Item 0 by default */
  hid_demo.state = HID_DEMO_IDLE;
  osSemaphoreRelease(MenuEvent);
}

/**
  * @brief  User task
  * @param  pvParameters not used
  * @retval None
  */
void HID_MenuThread(void const *argument)
{
  for(;;)
  {
    if(osSemaphoreWait(MenuEvent , osWaitForever) == osOK)
    {
      switch(hid_demo.state)
      {
      case HID_DEMO_IDLE:
          hid_demo.state = HID_DEMO_START;
          osSemaphoreRelease(MenuEvent);
        break;  
        
      case HID_DEMO_START:
        if(Appli_state == APPLICATION_READY)
        {
          /* Wait for User Input */
          if(ButtonPressed == 1)
          {
            ButtonPressed = 0;
            if(USBH_HID_GetDeviceType(&hUSBHost) == HID_KEYBOARD)
            {
              hid_demo.keyboard_state = HID_KEYBOARD_START;
              hid_demo.state = HID_DEMO_KEYBOARD;
              HID_KeyboardMenuProcess();
            }
            else if(USBH_HID_GetDeviceType(&hUSBHost) == HID_MOUSE)
            {
              hid_demo.mouse_state = HID_MOUSE_START;
              hid_demo.state = HID_DEMO_MOUSE;
              HID_MouseMenuProcess();
            }
            else
            {
              LCD_ErrLog("No supported HID device!\n");
              hid_demo.state = HID_DEMO_IDLE;
            }
          }
          osSemaphoreRelease(MenuEvent);          
        }
        break;
        
      case HID_DEMO_MOUSE:
        if(Appli_state == APPLICATION_READY)
        {
          USBH_MouseDemo(&hUSBHost);
        }
        break;
        
      case HID_DEMO_KEYBOARD:
        if(Appli_state == APPLICATION_READY)  
        {
          USBH_KeybdDemo(&hUSBHost);
        }   
        break;
        
      default:
        break;
      }
      
      if(Appli_state == APPLICATION_DISCONNECT)
      {
        Appli_state = APPLICATION_IDLE; 
        LCD_LOG_ClearTextZone();
        BSP_LCD_ClearStringLine(19);
        LCD_ErrLog("HID device disconnected!\n");
        hid_demo.state = HID_DEMO_IDLE;
      }
    }
  }
}

/**
  * @brief  The function is a callback about HID Data events
  * @param  phost: Selected device
  * @retval None
  */
void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
  osSemaphoreRelease(MenuEvent);
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{  
  if(GPIO_Pin == WAKEUP_BUTTON_PIN)
  {
    osSemaphoreRelease(MenuEvent);
    ButtonPressed = 1;
  }
}

/**
  * @brief  Main routine for Mouse application
  * @param  phost: Host handle
  * @retval None
  */
static void USBH_MouseDemo(USBH_HandleTypeDef *phost)
{
  HID_MOUSE_Info_TypeDef *m_pinfo;  
  
  m_pinfo = USBH_HID_GetMouseInfo(phost);
  
  if(m_pinfo != NULL)
  {
    /* Handle Mouse data position */
    USR_MOUSE_ProcessData(&mouse_info);
    
    if(m_pinfo->buttons[0])
    {
      HID_MOUSE_ButtonPressed(0);
    }
    else
    {
      HID_MOUSE_ButtonReleased(0);
    }
    
    if(m_pinfo->buttons[1])
    {
      HID_MOUSE_ButtonPressed(2);
    }
    else
    {
      HID_MOUSE_ButtonReleased(2);
    }
    
    if(m_pinfo->buttons[2])
    {
      HID_MOUSE_ButtonPressed(1);
    }
    else
    {
      HID_MOUSE_ButtonReleased(1);
    }
  }
}

/**
  * @brief  Main routine for Keyboard application
  * @param  phost: Host handle
  * @retval None
  */
static void USBH_KeybdDemo(USBH_HandleTypeDef *phost)
{
  HID_KEYBD_Info_TypeDef *k_pinfo; 
  char c;
  
  k_pinfo = USBH_HID_GetKeybdInfo(phost);
  
  if(k_pinfo != NULL)
  {
    c = USBH_HID_GetASCIICode(k_pinfo);
    if(c != 0)
    {
      USR_KEYBRD_ProcessData(c);
    }
  }
}
