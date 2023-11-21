/**
  @page HID_Standalone USB Host Human Interface (HID) application
  
  @verbatim
  ******************** (C) COPYRIGHT 2017 STMicroelectronics *******************
  * @file    USB_Host/HID_Standalone/readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the USB Host HID application.
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
   @endverbatim

@par Application Description 

This application shows how to use the USB host application based on the HID class on the STM32F4xx devices.

This is a typical application on how to use the STM32F413xx USB OTG Host peripheral to interact with an USB 
HID Device such as a Mouse or a Keyboard.

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 100 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from the PLL I2S.

The 48 MHz clock for the USB FS can be derived from one of the two following sources:
  – PLL clock (clocked by the HSE): If the USB uses the PLL as clock source, the PLL clock must be programmed
    to output 48 MHz frequency (USBCLK = PLLVCO/PLLQ).
  – PLLI2S clock (clocked by the HSE): If the USB uses the PLLI2S as clock source, the PLLI2S clock must be programmed
    to output 48 MHz frequency (USBCLK = PLLI2SVCO/PLLI2SQ).

When the application is started, the connected HID device (Mouse/Keyboard) is detected in HID mode and 
gets initialized. The STM32 MCU behaves as a HID Host, it enumerates the device and extracts VID, PID, 
manufacturer name, Serial no and product name information and displays it on the LCD screen. 
This application is based on interacting with a HID device (Mouse/Keyboard) through a HID routine.
LCD and joystick are provided by Adafruit 1.8" TFT shield to be plugged on Arduino connectors.

A menu is displayed and the user can select any operation from the menu using the Joystick buttons:
 - "Start HID" operation starts the appropriate HID application to the attached HID Device.
   - "Start Mouse / Re-Initialize" operation starts Mouse HID application. Moving the mouse will move
    the pointer in the display rectangle and if a button is pressed, the corresponding rectangle will be
    highlighted in Blue.
   - "Start Keyboard / Clear" operation starts Keyboard HID application. Taped Keyboard characters are 
   displayed on the LCD screen.
 - "Re-Enumerate" operation performs a new Enumeration of the device.

While program is running, LED2 and LED3 are toggling in Infinite loop.
 
@note In case of using an AZERTY keyboard, user should add "AZERTY_KEYBOARD" define to ensure correct 
      displaying taped characters.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

For more details about the STM32Cube USB Host library, please refer to UM1720  
"STM32Cube USB Host library".

@par USB Library Configuration

It is possible to fine tune needed USB Host features by modifying defines values in USBH configuration
file "usbh_conf.h" available under the project includes directory, in a way to fit the application
requirements, such as:
- Level of debug: USBH_DEBUG_LEVEL
                  0: No debug messages
                  1: Only User messages are shown
                  2: User and Error messages are shown
                  3: All messages and internal debug messages are shown
   By default debug messages are displayed on the debugger IO terminal; to redirect the Library
   messages on the LCD screen, lcd_log.c driver need to be added to the application sources.

@par Keywords

Connectivity, USB_Host, HID, Full Speed, High Speed, Joystick, Mouse, Keyboard, enumerate

@par Directory contents

  - USB_Host/HID_Standalone/Src/main.c                  Main program
  - USB_Host/HID_Standalone/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - USB_Host/HID_Standalone/Src/stm32f4xx_it.c          Interrupt handlers
  - USB_Host/HID_Standalone/Src/menu.c                  HID State Machine
  - USB_Host/HID_Standalone/Src/usbh_conf.c             General low level driver configuration
  - USB_Host/HID_Standalone/Src/mouse.c                 HID mouse functions file
  - USB_Host/HID_Standalone/Src/keyboard.c              HID keyboard functions file
  - USB_Host/HID_Standalone/Inc/main.h                  Main program header file
  - USB_Host/HID_Standalone/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - USB_Host/HID_Standalone/Inc/lcd_log_conf.h          LCD log configuration file
  - USB_Host/HID_Standalone/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - USB_Host/HID_Standalone/Inc/usbh_conf.h             USB Host driver Configuration file
 

@par Hardware and Software environment

  - This application runs on STM32F413xx/STM32F423xx devices.
    
  - This application has been tested with STMicroelectronics NUCLEO-F413ZH boards 
    and can be easily tailored to any other supported device and development board.

  - NUCLEO-F413ZH Set-up
    - Plug the USB device mouse/ Keyboard into the NUCLEO-F413ZH board through 
      'USB micro A-Male to A-Female' cable into CN13 connector to use USB Full Speed (FS)

  - Make sure that the jumper JP4 is closed

  - Adafruit 1.8" TFT shield must be connected on CN7, CN8, CN9 and CN10 Arduino connectors, 
    for more details please refer to UM1726.

  - Make sure that the SB165, SB121 are closed and the SB174, SB146, SB122 are opened on the NUCLEO-F413ZH board (bottom side) 

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 

 */
