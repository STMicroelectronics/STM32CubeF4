/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/Src/dual_menu.c
  * @author  MCD Application Team
  * @brief   Dual Core Process
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
uint8_t *DEMO_main_menu[] =
{
  (uint8_t *)"      1 - Mass Storage demo                                                 ",
  (uint8_t *)"      2 - Humain Interface demo                                             ",
  (uint8_t *)"      3 - Re-Enumerate MSC                                                  ",
  (uint8_t *)"      4 - Re-Enumerate HID                                                  ",
};

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t prev_select = 0;
uint8_t joy_select = 0;
DEMO_StateMachine demo;
JOYState_TypeDef JoyState = JOY_NONE;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Menu Init
  * @param  None
  * @retval None
  */
void DUAL_MenuInit(void)
{
  /* Start HID Interface */
  LCD_UsrLogY("USB Host Full speed initialized.\n");
  /* Start MSC Interface */
  LCD_UsrLogY("USB Host High speed initialized.\n");

  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_DisplayStringAtLine(24, (uint8_t *)"Use [Joystick Left/Right] to scroll up/down");
  BSP_LCD_DisplayStringAtLine(25, (uint8_t *)"Use [Joystick Up/Down] to scroll DUAL menu");
  Demo_SelectItem(DEMO_main_menu, 0);
}

/**
  * @brief  Manages DUAL Menu Process.
  * @param  None
  * @retval None
  */
void DUAL_MenuProcess(void)
{
  switch(demo.state)
  {
  case DEMO_IDLE:
    Demo_SelectItem(DEMO_main_menu, 0);
    LCD_UsrLog("Select an operation to continue.\n");
    demo.state = DEMO_WAIT;
    demo.select = 0;
    break;
  case DEMO_WAIT:

    if(demo.select != prev_select)
    {
      prev_select = demo.select;
      Demo_SelectItem(DEMO_main_menu, demo.select & 0x7F);

      /* Handle select item */
      if(demo.select & 0x80)
      {
        demo.select &= 0x7F;

        switch(demo.select)
        {
        case 0:
          demo.state = DEMO_MSC_START;
          break;

        case 1:
          demo.state = DEMO_HID_START;
          break;

        case 2:
          demo.state = DEMO_MSC_REENUMERATE;
          break;

        case 3:
          demo.state = DEMO_HID_REENUMERATE;
          break;

        default:
          break;
        }
      }
    }
    break;

  case DEMO_MSC_START:
    if(Appli_HS_state == APPLICATION_HS_READY)
    {
      /* If HS device connected*/
      demo.state = DEMO_MSC;
      demo.msc_state = APPLI_MSC_IDLE;
    }
    else
    {
      LCD_ErrLog("No Mass storage device connected.\n");
      demo.state = DEMO_WAIT;
    }
    break;

  case DEMO_HID_START:
    if(Appli_FS_state == APPLICATION_FS_READY)
    {
      /*if FS device connected*/
      demo.state = DEMO_HID;
      demo.hid_state = APPLI_HID_IDLE;
    }
    else
    {
      LCD_ErrLog("No HID device connected.\n");
      demo.state = DEMO_WAIT;
    }
    break;

  case DEMO_MSC:
    MSC_MenuProcess();
    break;

  case DEMO_HID:
    HID_MenuProcess();
    break;

  case DEMO_MSC_REENUMERATE: /* Re-Enumeration MSC */
    LCD_DbgLog("Starting Enumeration MSC....");
    LCD_DbgLog("\n");
    USBH_ReEnumerate(&hUSBHost_HS);
    demo.state = DEMO_WAIT;
    break;

  case DEMO_HID_REENUMERATE: /* Re-Enumeration HID */
    LCD_DbgLog("Starting Enumeration HID....");
    LCD_DbgLog("\n");
    USBH_ReEnumerate(&hUSBHost_FS);
    demo.state = DEMO_WAIT;
    break;

  default:
    break;
  }

  if(joy_select == 1)
  {
    Demo_ProbeKey(JoyState);

    switch(JoyState)
    {
    case JOY_LEFT:
      LCD_LOG_ScrollBack();
      break;

    case JOY_RIGHT:
      LCD_LOG_ScrollForward();
      break;

    default:
      break;
    }

    joy_select = 0;

    /* Clear joystick interrupt pending bits */
    BSP_IO_ITClear();
  }
  if(Appli_FS_state == APPLICATION_FS_DISCONNECT)
  {
    Appli_FS_state = APPLICATION_IDLE;
    LCD_LOG_ClearTextZone();
    LCD_ErrLog("device FS disconnected!\n");
    demo.state = DEMO_IDLE;
    demo.select = 0;
  }

  if(Appli_HS_state == APPLICATION_HS_DISCONNECT)
  {
    Appli_HS_state = APPLICATION_IDLE;
    LCD_LOG_ClearTextZone();
    LCD_ErrLog("device HS disconnected!\n");
    demo.state = DEMO_IDLE;
    demo.select = 0;
  }
}

/**
  * @brief  Probes the joystick state.
  * @param  state: Joystick state
  * @retval None
  */
void Demo_ProbeKey(JOYState_TypeDef state)
{
  if((state == JOY_UP) && (demo.select > 0))
  {
    demo.select--;
  }
  else if(((demo.state == DEMO_WAIT) && (state == JOY_DOWN) && (demo.select < 3)) ||
          ((demo.state == DEMO_MSC)  && (state == JOY_DOWN) && (demo.select < 2)) ||
          ((demo.state == DEMO_HID)  && (state == JOY_DOWN) && (demo.select < 1)))
  {
    demo.select++;
  }
  else if(state == JOY_SEL)
  {
    demo.select |= 0x80;
  }
}

/**
  * @brief  Manages the menu on the screen.
  * @param  menu: Menu table
  * @param  item: Selected item to be highlighted
  * @retval None
  */
void Demo_SelectItem(uint8_t **menu, uint8_t item)
{
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

  switch(item)
  {
  case 0:
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(26, menu[0]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(27, menu[1]);
    BSP_LCD_DisplayStringAtLine(28, menu[2]);
    BSP_LCD_DisplayStringAtLine(29, menu[3]);
    break;

  case 1:
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(26, menu[0]);
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(27, menu[1]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(28, menu[2]);
    BSP_LCD_DisplayStringAtLine(29, menu[3]);
    break;

  case 2:
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(26, menu[0]);
    BSP_LCD_DisplayStringAtLine(27, menu[1]);
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(28, menu[2]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(29, menu[3]);
    break;

  case 3:
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(26, menu[0]);
    BSP_LCD_DisplayStringAtLine(27, menu[1]);
    BSP_LCD_DisplayStringAtLine(28, menu[2]);
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(29, menu[3]);

    break;

  default:
    break;
  }
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == MFX_IRQOUT_PIN)
  {
    /* The different functionalities of MFX (TS, Joystick, SD detection, etc. )
    can be configured in exti mode to generate an IRQ on given events.
    The MFX IRQ_OUT pin is unique and common to all functionalities, so if several
    functionalities are configured in exit mode, the MCU has to enquire MFX about
    the IRQ source (see BSP_IO_ITGetStatus). Communication with Mfx is done by I2C.
    Often the sw requires ISRs (irq service routines) to be quick while communication
    with I2C can be considered relatively long (hundreds of usec depending on I2C clk).
    In order to avoid to use "blocking I2C communication" on interrupt service routines
    it's suggested (as alternative to this implementation) to use dedicated semaphore*/

    joy_select = 1;

    /* Get the Joystick State */
    JoyState = BSP_JOY_GetState();
  }
   HAL_Delay(400);
}
